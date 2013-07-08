#include "convertShadingNetwork.h"

#include <common/mayacheck.h>
#include <liqGlobalHelpers.h>
#include <liqShader.h>
#include <liqShaderFactory.h>
#include <liqlog.h>
#include "shadermgr.h"
#include "shaderOutputMgr.h"
#include "../renderermgr.h"
#include "ShadingNetworkUserBase.h"

//#include "../renderman/rm_helper.h"

namespace liquidmaya{

void connectMStringArray(MString& des, const MStringArray& src)
{
	des.clear();
	for(std::size_t i=0; i<src.length(); ++i){
		des += src[ i ] + ",";
	}
}

ConvertShadingNetwork::ConvertShadingNetwork()
{
}
//
ConvertShadingNetwork::~ConvertShadingNetwork()
{

}
//

//
void ConvertShadingNetwork::exportShaderGraph()
{
	CM_TRACE_FUNC("ConvertShadingNetwork::exportShaderGraph()");

	if( beforeExport() )
	{
		__export();
		afterExport();
	}

}
//
bool ConvertShadingNetwork::beforeExport()
{
	CM_TRACE_FUNC("ConvertShadingNetwork::beforeExport()");

	exportedShader.clear();
	exportedShadingGroup.clear();
	return liquid::RendererMgr::getInstancePtr()->beforeExport_user();
}
//
void ConvertShadingNetwork::afterExport()
{
	CM_TRACE_FUNC(" ConvertShadingNetwork::afterExport()");

	liquid::RendererMgr::getInstancePtr()->afterExport_user();
}
//
bool ConvertShadingNetwork::nodeIsConvertible ( const MString& node ) const
{
	CM_TRACE_FUNC("ConvertShadingNetwork::nodeIsConvertible("<<node.asChar()<<")");

	MString cmd;

	MString nodetype;
	cmd = "nodeType \""+node+"\"";
	IfMErrorWarn(MGlobal::executeCommand( cmd, nodetype));

	if( ShaderMgr::getSingletonPtr()->hasShaderType(nodetype.asChar()) )
	{
		return true;
	}else{
		//liquidMessage2(messageError, "node type [%s] (%s) is not supported", nodetype.asChar(), node.asChar());
		return false;
	}
}
//
bool ConvertShadingNetwork::ensurePlugExists(const MString& plug) const
{	
	CM_TRACE_FUNC("ConvertShadingNetwork::ensurePlugExists("<<plug.asChar()<<")");

	MStringArray components;
	plug.split('.', components);
	MString node(components[0]);
	MString attribute(components[1]);

	//$attribute = `substitute "\\[.*" $attribute ""`;
	MString cmd("substitute \"\\[.*\" "+attribute+" \"\"");
	IfMErrorWarn(MGlobal::executeCommand( cmd, attribute));

	int exist;
	cmd = "attributeExists \""+attribute+"\" \""+node+"\"";
	IfMErrorWarn(MGlobal::executeCommand( cmd, exist));
	if( !exist ){
		liquidMessage2(messageError, ("Attribute '" + attribute + "' is not part of the '" + node + "'." ).asChar());
		return false;
	}
	return true;
}
//
ConnectionType 
ConvertShadingNetwork::convertibleConnection(const MString& plug) const
{
	CM_TRACE_FUNC("ConvertShadingNetwork::convertibleConnection("<<plug.asChar()<<")");

	// Check to see if the plug even exists!
	if( !ensurePlugExists(plug) ){
		liquidMessage2(messageError, "plug [%s] not exist.", plug.asChar());
		return (ConnectionType)0;
	}
	
	ConnectionType connectionType = CT_None;
	bool isIn = false, isOut = false;

	MStringArray connections;

	// Check to see if the plug is connected as an output
	// to any node(s) we can convert.
	MString cmd;
	cmd = "listConnections -source off -destination on \""+plug+"\"";
	IfMErrorWarn(MGlobal::executeCommand( cmd, connections));
	
	for(size_t i=0; i<connections.length(); ++i)
	{
		if( nodeIsConvertible(connections[i]) ){
			isOut = true;
			break;
		}
	}
	
	// Check to see if the plug is connected as an input from any
	// node(s) we can convert.
	cmd = "listConnections -source on -destination off \""+plug+"\"";
	IfMErrorWarn(MGlobal::executeCommand( cmd, connections));
	
	for(size_t i=0; i<connections.length(); ++i)
	{
		if( nodeIsConvertible(connections[i]) ){
			isIn = true;
			break;
		}
	}

	if( isOut && isIn ){
		connectionType = CT_InOut;
	} else if( isOut && !isIn ){
		connectionType = CT_Out;
	} else if( !isOut && isIn ){
		connectionType = CT_In;
	} else if( !isOut && !isIn ){
		connectionType = CT_None;
	}
	// Otherwise, return "no convertible connection".
	return connectionType;
}
//addRSLVariable()
//addToRSL
//endRSL ()
//beginRSL ( string $name )
int ConvertShadingNetwork::getUpstreamConvertibleNodes ( const MString& currentNode, 
								 MStringArray& nodes, MIntArray& numConnections)
{
	CM_TRACE_FUNC("ConvertShadingNetwork::getUpstreamConvertibleNodes("<<currentNode.asChar()<<", nodes, numConnections)");

	// If the current node has already been visited
	for(std::size_t i=0; i<nodes.length(); ++i)
	{
		if( nodes[i]==currentNode)
			return 1;
	}

	// If the node is not supported
	if( !nodeIsConvertible( currentNode ) )
	{
		return 0;
	}

	const int index = nodes.length();

	// Append the current node to the list of nodes
	if( nodes.length()<(index+1) ){
		nodes.setLength(index+1);
	}
	nodes[index] = currentNode;
	
	// Initialize the number of input connections to 0 for the newly found node
	if( numConnections.length()<(index+1) ){
		numConnections.setLength(index+1);
	}
	numConnections[index] = 0;//numConnections.set(0, index);//
	//std::cout<<"numConnections[]="<<numConnections<<std::endl;

	// Get the list of supported connections from the current node
	MStringArray validConnections;
	ShaderMgr::getSingletonPtr()->getValidConnection(currentNode.asChar(), validConnections);
	// Get the list of up stream nodes along supported connections
	std::set<const std::string> upstreamNodes;
	for(size_t i=0; i < validConnections.length(); ++i)
	{
		const MString validConnection( validConnections[i] );
		const MString plug(currentNode+"."+validConnection);
		
		if( convertibleConnection(plug) != CT_None )
		{
			MStringArray connection;
			IfMErrorWarn(MGlobal::executeCommand( ("listConnections -source on -destination off \""+plug+"\""), connection));
		
			//append connection to upstreamNodes
			for(std::size_t i=0; i<connection.length(); ++i){
				upstreamNodes.insert(connection[i].asChar());
			}
		}
	}

	// Remove duplicate nodes from the upstreamNodes
	// upstreamNodes is std::set, so no need to remove the duplicate

	//
	std::set<const std::string>::const_iterator i2 = upstreamNodes.begin();
	std::set<const std::string>::const_iterator e2 = upstreamNodes.end();
	for(; i2 != e2; ++i2)
	{
		MString node( i2->c_str() );

		if( numConnections.length()<(index+1) ){
			numConnections.setLength(index+1);
		}
		numConnections[index] += 
			getUpstreamConvertibleNodes(node, nodes, numConnections);
	}
	//std::cout<<"numConnections[]="<<numConnections<<std::endl;
	return 1;
}
//
void ConvertShadingNetwork::addNodeInputVariable(const MString& plug, MStringArray& inputSrc, MStringArray& inputDes)
{
	CM_TRACE_FUNC("ConvertShadingNetwork::addNodeInputVariable("<<plug.asChar()<<", inputSrc, inputDes)");

	MString cmd;

	MStringArray inputPlugs;
	IfMErrorWarn(MGlobal::executeCommand( ("listConnections -plugs true \""+plug+"\""), inputPlugs));
	MString varName;
	MString inputNode;
	int inputIndex = inputSrc.length();
	assert(inputSrc.length() == inputDes.length());

	for(size_t i=0; i<inputPlugs.length(); ++i)
	{
		const MString inputPlug( inputPlugs[i] );

		MStringArray buffer;
		inputPlug.split('.', buffer);

		inputNode = buffer[0];

		if( nodeIsConvertible( inputNode ) )
		{
			// Ensure that the connected plug is being exported
			// (ie supported by the converter).
			MString inputAttribute = buffer[ 1 ];
			bool notConnected = true;
			
			MStringArray validConnections;
			ShaderMgr::getSingletonPtr()->getValidConnection(inputNode.asChar(), validConnections);
			
			for(size_t i=0; i < validConnections.length(); ++i)
			{
				const MString connection(validConnections[i]);

				if(connection == inputAttribute){
					notConnected = false;
					break;
				}
			}
			if(notConnected == true){
				continue;
			}

			
			if( inputSrc.length()<(inputIndex+1) ){
				inputSrc.setLength(inputIndex+1);
				inputDes.setLength(inputIndex+1);
			}
			inputSrc[inputIndex] = inputPlug.asChar();
			inputDes[inputIndex] = plug.asChar();
			inputIndex++;
		}
	}

}
//
void ConvertShadingNetwork::addNodeOutputVariable(
	const MString& node, const MString& plug, 
	const MString& node_plug, MStringArray& outputSrc)
{
	CM_TRACE_FUNC("ConvertShadingNetwork::addNodeOutputVariable("<<node.asChar()<<","<<plug.asChar()<<","<<node_plug.asChar()<<", outputVars)");
	//DEBUG
	if(node_plug=="place2dTexture1.mirrorU"){
		int i = 0;
	}

	MString plugtype, detail;
	int arraysize = -1;

	MString nodetype;
	getNodeType(nodetype, node);

	if(   nodetype == "liquidShader"
		||nodetype == "liquidSurface"
		||nodetype == "liquidDisplacement"
		||nodetype == "liquidVolume")
	{
			MStringArray rmanParams, rmanTypes, rmanDetails;
		MIntArray rmanArraySizes;
		
		IfMErrorWarn(MGlobal::executeCommand( ("getAttr \""+node+".rmanParams\""), rmanParams ));
	
		//find parameter index of $plug in $node.rmanParams
		int paramIndex = -1;
		for(int i = 0; i< rmanParams.length(); ++i)
		{
			MString pi(rmanParams[i]);
			if(rmanParams[i]==plug){
				paramIndex = i;
				break;
			}
		}
		if(paramIndex==-1){
			liquidMessage2(messageError, "can't find parameter [%s] in [%s.rmanParams]", plug.asChar(), node.asChar());
			return;
		}

		IfMErrorWarn(MGlobal::executeCommand( ("getAttr \""+node+".rmanTypes\""),		rmanTypes ));
		IfMErrorWarn(MGlobal::executeCommand( ("getAttr \""+node+".rmanArraySizes\""),	rmanArraySizes ));
		IfMErrorWarn(MGlobal::executeCommand( ("getAttr \""+node+".rmanDetails\""),		rmanDetails ));

		plugtype  = rmanTypes[paramIndex];
		arraysize = rmanArraySizes[paramIndex];
		detail    = rmanDetails[paramIndex].asInt();
	}//if("liquidShader"...)
	// for Maya Shaders(e.g. lambert, blinn, ...)
	else{
		//check if the plug is used as color
		int isUsedAsColor = false;
		{
			//step 1
			IfMErrorWarn(MGlobal::executeCommand( ("attributeQuery -node "+node+" -usedAsColor \""+plug+"\""), isUsedAsColor ));

			//step 2
			if( isUsedAsColor == false )
			{
				int doesChildRExist = false;
				int doesChildGExist = false;
				int doesChildBExist = false;

				IfMErrorWarn(MGlobal::executeCommand( "attributeExists(\""+plug+"R\", \""+node+"\")",doesChildRExist ) );
				IfMErrorWarn(MGlobal::executeCommand( "attributeExists(\""+plug+"G\", \""+node+"\")",doesChildGExist ) );
				IfMErrorWarn(MGlobal::executeCommand( "attributeExists(\""+plug+"B\", \""+node+"\")",doesChildBExist ) );
				
				if( doesChildRExist && doesChildGExist && doesChildBExist )
				{
					isUsedAsColor = true;
				}
			}
		}

		if( isUsedAsColor )//node_plug is used as color
		{
			plugtype = "color";
			detail    = "";
			arraysize = -1;
		}
		//node_plug is not color type
		else{
			MString mayaPlugType_formated;
			{
				MString mayaPlugType;
				IfMErrorWarn(MGlobal::executeCommand( ("getAttr -type \""+node_plug+"\""), mayaPlugType ));

				mayaPlugType_formated = convertMayaTypeToRSLFloat(mayaPlugType.asChar()).c_str();
			}

			MString matchedStr;
			IfMErrorWarn(MGlobal::executeCommand( ("match(\"float[0-9]*$\", \""+mayaPlugType_formated+"\")"), matchedStr));
			if( matchedStr != "" )
			{
				MString typeSize;
				IfMErrorWarn(MGlobal::executeCommand( ("match(\"[0-9]*$\", \""+mayaPlugType_formated+"\")"), typeSize));

				if(typeSize.length() == 0)//float
				{
					plugtype  = "float";
					detail    = "";
					arraysize = -1;
				}
				else if(typeSize.asInt() == 3){//float3, take it as vector
					plugtype  = "vector";
					detail    = "";
					arraysize = -1;
				}else{// float array
					plugtype  = "float";
					detail    = "";
					arraysize = typeSize.asInt();
				}
			}else{
				liquidMessage2(messageError, "cant handle plug[%s](type[%s])", node_plug.asChar(), plugtype.asChar());
			}
		}//if( isUsedAsColor ) else
	}//if("liquidShader"...) else


	liquidmaya::ShaderOutputMgr::getSingletonPtr()->
		addShaderMethodVariavles(
			node, 
			plug, 
			plugtype,
			arraysize,
			detail
		);

	const int outputIndex = outputSrc.length();
	if( outputSrc.length()<(outputIndex+1) ){
		outputSrc.setLength(outputIndex+1);
	}
	outputSrc[outputIndex] = (node+"."+plug);
}
//
void ConvertShadingNetwork::getNodeVariables(
	const MString& node, const MStringArray& validConnections, 
	MStringArray&  inputSrc, MStringArray&  inputDes, MStringArray& outputSrc)
{
	CM_TRACE_FUNC("ConvertShadingNetwork::getNodeVariables("<<node.asChar()<<", validConnections,  inputSrc, inputDes, outputSrc)");

	for( std::size_t i = 0; i<validConnections.length(); ++i )
	{
		const MString validConnection(validConnections[i]);
		const MString plug(node+"."+validConnection);
		ConnectionType connectionType = convertibleConnection(plug);
		
		// If the connection is neither an input or output connection
		if( connectionType == CT_None )
		{
			continue;
		}

		// Get input variable
		if( connectionType == CT_In )
		{
			addNodeInputVariable( plug, inputSrc, inputDes );
		}
		// Get output variable
		else if( connectionType == CT_Out || connectionType == CT_InOut ){
			addNodeOutputVariable( node, validConnection, plug,
				outputSrc );
		}
	}
}
//
void ConvertShadingNetwork::decrementDownstreamConnections(
	const MString& node, const MStringArray& nodes, 
	MIntArray& numConnections, const MStringArray& validConnections
	)
{
	CM_TRACE_FUNC("ConvertShadingNetwork::decrementDownstreamConnections("<<node.asChar()<<", nodes, numConnections, validConnections)");

	std::set<const std::string> downstreamNodes;

	// Get the list of down stream nodes along supported connections
	for(std::size_t i=0; i<validConnections.length(); ++i)
	{
		const MString validConnection(validConnections[i]);
		const MString plug(node+"."+validConnection);

		MStringArray con;
		IfMErrorWarn(MGlobal::executeCommand( "listConnections -source off -destination on \""+plug+"\"", con));
		for(std::size_t i=0; i<con.length(); ++i)
		{
			downstreamNodes.insert(con[i].asChar());
		}
	}
	// Remove duplicate nodes from the list
	// Because downstreamNodes is std::set, no need to remove duplicate

	// For each down stream node, decrement the number of connections
	int numNodes = nodes.length();
	std::set<const std::string>::const_iterator i = downstreamNodes.begin();
	std::set<const std::string>::const_iterator e = downstreamNodes.end();
	for( ; i != e; ++i )
	{
		MString downstreamNode( i->c_str() );
		for(int index = 0; index<numNodes; ++index)
		{
			if(downstreamNode==nodes[index])
			{
				(numConnections[index])--;
				break;
			}
		}
	}
	//std::cout<<"numConnections[]="<<numConnections<<std::endl;
	downstreamNodes.clear();
}
void ConvertShadingNetwork::traverseGraphAndOutputNodeFunctions( 
	const MStringArray& nodes, MIntArray& numConnections)
{
	CM_TRACE_FUNC("ConvertShadingNetwork::traverseGraphAndOutputNodeFunctions( nodes, numConnections)");

	MString cmd;

	//test
	int n0 = nodes.length();
	int n1 = numConnections.length();

	for(size_t index = 0; index< nodes.length(); ++index)
	{
		MString currentNode(nodes[index]);
		// if the current node has all of its required inputs, then
		// write out the information for the current node and decrement
		// the number of inputs for all nodes awaiting the completion of
		// the current node
		if( numConnections[index] == 0 )
		{
			// write out the current node's function
			ShaderOutputMgr::getSingletonPtr()->outputUpstreamShader(currentNode.asChar());//shader->writeRSL(currentNode.asChar());

			// Get the list of supported connections from the current node			
			MStringArray validConnections;
			ShaderMgr::getSingletonPtr()->getValidConnection(currentNode.asChar(), validConnections);

			decrementDownstreamConnections( currentNode,
				nodes,
				numConnections,
				validConnections );

			MStringArray inputSrc; 
			MStringArray inputDes;
			MStringArray outputSrc;
			getNodeVariables( currentNode,
				validConnections,
				inputSrc,
				inputDes,
				outputSrc);

			liquidmaya::ShaderOutputMgr::getSingletonPtr()->
				addShaderMethodBody(currentNode, inputSrc, inputDes, outputSrc);



			// We are done with the current node
			numConnections[ index ] = -1;

			// Start back at the beginning
			index = -1;
		}
		//std::cout<<"numConnections[]="<<numConnections<<std::endl;
	}
}
//
void ConvertShadingNetwork::outputShaderMethod()
{
	CM_TRACE_FUNC("ConvertShadingNetwork::outputShaderMethod()");

	liquidmaya::ShaderOutputMgr::getSingletonPtr()->
		outputShaderMethod();

}
//
void ConvertShadingNetwork::convertShadingNetworkToRSL(const MString& startingNode)
{
	CM_TRACE_FUNC("ConvertShadingNetwork::convertShadingNetworkToRSL("<<startingNode.asChar()<<")");

	MString cmd;

	MStringArray nodes;
	MIntArray numConnections;

	liquidmaya::ShaderOutputMgr::getSingletonPtr()->
		initShaderData(startingNode);

	getUpstreamConvertibleNodes(startingNode, nodes, numConnections);
	//std::cout<<"numConnections[]="<<numConnections<<std::endl;
	liquidmaya::ShaderOutputMgr::getSingletonPtr()->
		preOutput(startingNode.asChar());
	liquidmaya::ShaderOutputMgr::getSingletonPtr()->
		outputBegin(startingNode.asChar());

	// Traverse the graph outputing functions for nodes that have received all
	// of their respective inputs
	traverseGraphAndOutputNodeFunctions(nodes, numConnections);
	
	// Output the shader method
	outputShaderMethod();

	liquidmaya::ShaderOutputMgr::getSingletonPtr()->
		outputEnd();
	liquidmaya::ShaderOutputMgr::getSingletonPtr()->
		postOutput();

	//...
}
//
void ConvertShadingNetwork::__export()
{
	CM_TRACE_FUNC("ConvertShadingNetwork::__export()");

	MString cmd;

	MStringArray geometryArray;
	//IfMErrorWarn(MGlobal::executeCommand( "ls -selection -dag -shapes", geometryArray));
	IfMErrorWarn(MGlobal::executeCommand( "ls -geometry", geometryArray));

	if(geometryArray.length()==0){
		liquidMessage2(messageWarning ,"no shape is selected, no shader will be exported.\n");
	}

	for(std::size_t i=0; i<geometryArray.length(); ++i)
	{
		const MString node(geometryArray[i]);

		//string $sgNodes[] = `listConnections -type "shadingEngine" -destination on ( $node + ".instObjGroups" )`;//add -type "shadingEngine" for multy renderlayers
		std::vector<std::string> sgNodes;
		getShadingGroups(node, sgNodes);

		if( sgNodes.size() !=0 )
		{
			if( canShadingGroupExported(sgNodes[0].c_str()) )
			{
				//1.shaders must be exported before shading group
				{
					//get the shader plugs in ShadingGroup
					std::vector<std::string> plugs;
					liquid::RendererMgr::getInstancePtr()->
						getRenderer()->getValidShaderPlugsInShadingGroup(plugs);
					//export the plugs
					for_each(plugs.begin(), plugs.end(),
						boost::bind( &ConvertShadingNetwork::exportShaderInShadingGroup, this, sgNodes[0].c_str(), _1 )
					);
				}
				//2.begin
				exportShadingGroupBegin(sgNodes[0].c_str());
				//3.export shading group
				outputShadingGroup(sgNodes[0].c_str());
				//4.end
				exportShadingGroupEnd(sgNodes[0].c_str());
			}
// 			if( sgNodes.size() > 1 )
// 			{
// 				std::ostringstream s;
// 				std::copy(sgNodes.begin(), sgNodes.end(), std::ostream_iterator<std::string>(s, ","));
// 				liquidMessage2(messageError, "[%s] has many shading groups[%s]. only the 1st is exported.", node.asChar(), s.str().c_str());
// 			}
		}else{
			liquidMessage2(messageInfo, ("["+node +"] has not shading group, skip.").asChar() );
		}
	}//for(std::size_t i=0; i<geometryArray.length(); ++i)

	//////////////////////////////////////////////////////////////////////////
	//export the shaders which are attached to liquidGlobals.renderCamera
	MString renderCamera;
	IfMErrorWarn(MGlobal::executeCommand( "getAttr liquidGlobals.renderCamera", renderCamera));

	exportShaderNodeInPlug(renderCamera, "miEnvironmentShader");
	exportShaderNodeInPlug(renderCamera, "liqVolumeShader");
	exportShaderNodeInPlug(renderCamera, "liqEnvironmentShader");
	exportShaderNodeInPlug(renderCamera, "liqLensShader");
	//////////////////////////////////////////////////////////////////////////


	liquid::RendererMgr::getInstancePtr()->export_user();

}
//
void ConvertShadingNetwork::outputShadingGroup(const MString& shadingGroupNode)
{
	CM_TRACE_FUNC("ConvertShadingNetwork::outputShadingGroup("<<shadingGroupNode.asChar()<<")");

	liquidmaya::ShaderOutputMgr::getSingletonPtr()->
		outputShadingGroup(shadingGroupNode.asChar());
}
//
void ConvertShadingNetwork::exportShaderInShadingGroup(
	const MString& sgNode, 
	const std::string& plug_)
{
	CM_TRACE_FUNC("ConvertShadingNetwork::exportShaderInShadingGroup("<<sgNode.asChar()<<","<<plug_<<")");

	exportShaderNodeInPlug(sgNode, plug_.c_str());
}
// export shader node $node
// $node ---($node islinked to)----> $desPlug
void ConvertShadingNetwork::_exportShaderNode(const MString& node)
{
	CM_TRACE_FUNC("ConvertShadingNetwork::exportShaderNode("<<node.asChar()<<")");

	if( canShaderExported(node) )
	{
		//1.begin
		exportShaderBegin(node);

		//2.export shader
		MString nodetype;
		MString cmd("nodeType \""+node+"\"");
		IfMErrorWarn(MGlobal::executeCommand( cmd, nodetype));

		if(nodetype=="liquidSurface"||nodetype=="liquidVolume"||nodetype=="liquidDisplacement"){
			//liquidMessage2(messageInfo, ("["+startingNode+"]'s type is ["+nodetype+"], no need to convert").asChar());
			liqShader &currentShader = liqShaderFactory::instance().getShader( node.asChar() );
			currentShader.write();
		}else{
			convertShadingNetworkToRSL(node);
		}

		//3.end
		exportShaderEnd(node);
	}
}
void ConvertShadingNetwork::exportShaderNode(const MString& node)
{
	_exportShaderNode(node);
}
// export the node which is linked to $node.$plug
void ConvertShadingNetwork::_exportShaderNodeInPlug(const MString& node, const MString& plug)
{
	CM_TRACE_FUNC("ConvertShadingNetwork::_exportShaderNodeInPlug("<<node.asChar()<<","<<plug.asChar()<<")");

	MStringArray srcNodes;
	IfMErrorWarn(MGlobal::executeCommand( "listConnections -source on \""+node+"."+plug+"\"", srcNodes));

	if(srcNodes.length() == 0){
		liquidMessage2(messageInfo, "[%s.%s] has no source connections, skip the export of this plug.", node.asChar(), plug.asChar());
		return;
	}
	assert(srcNodes.length() == 1);
	const MString srcNode(srcNodes[0]);

	_exportShaderNode(srcNode);

}
//
void ConvertShadingNetwork::exportShaderNodeInPlug(const MString& node, const MString& plug)
{
	CM_TRACE_FUNC("ConvertShadingNetwork::exportShaderNodeInPlug("<<node.asChar()<<","<<plug.asChar()<<")");
	
	int bPlugExist = false;
	IfMErrorWarn(MGlobal::executeCommand( "attributeExists(\""+plug+"\", \""+node+"\") ", bPlugExist));

	if(bPlugExist)
	{
		_exportShaderNodeInPlug(node, plug);
	}else{
		liquidMessage2(messageInfo, "[%s.%s] not exist, skip the export of it.", node.asChar(), plug.asChar());
	}
}
//
bool ConvertShadingNetwork::canShaderExported(const MString& shaderName)
{
	CM_TRACE_FUNC("ConvertShadingNetwork::canShaderExported("<<shaderName.asChar()<<")");

	if(shaderName.length()==0)
	{
		liquidMessage2(messageInfo, ("shader name is empty,") );
		return false;
	}
	if(exportedShader.end() != std::find(exportedShader.begin(), exportedShader.end(), shaderName) )
	{
		liquidMessage2(messageInfo, ("["+shaderName+"] is already exported, skip.").asChar() );
		return false;
	}

	return true;
}
void ConvertShadingNetwork::exportShaderBegin(const MString& shaderName)
{
	CM_TRACE_FUNC("ConvertShadingNetwork::exportShaderBegin("<<shaderName.asChar()<<")");
}
//void ConvertShadingNetwork::exportShader(const MString& shaderName){}
void ConvertShadingNetwork::exportShaderEnd(const MString& shaderName)
{
	CM_TRACE_FUNC("ConvertShadingNetwork::exportShaderEnd("<<shaderName.asChar()<<")");
	// if shaderName already exists in exportedShader, 
	//canShaderExported() will return false, and exportShaderEnd() will not called.
	exportedShader.push_back(shaderName);
}
//
bool ConvertShadingNetwork::canShadingGroupExported(const MString& shadingGroupName)
{
	CM_TRACE_FUNC("ConvertShadingNetwork::canShadingGroupExported("<<shadingGroupName.asChar()<<")");

	if(shadingGroupName.length()==0)
	{
		liquidMessage2(messageInfo, ("shading group name is empty,") );
		return false;
	}
	if(exportedShadingGroup.end() != std::find(exportedShadingGroup.begin(), exportedShadingGroup.end(), shadingGroupName) )
	{
		liquidMessage2(messageInfo, ("["+shadingGroupName+"] is already exported, skip.").asChar() );
		return false;
	}

	return true;
}
void ConvertShadingNetwork::exportShadingGroupBegin(const MString& shadingGroupName)
{
	CM_TRACE_FUNC("ConvertShadingNetwork::exportShadingGroupBegin("<<shadingGroupName.asChar()<<")");
}
//void ConvertShadingNetwork::exportShadingGroup(const MString& shadingGroupName){}
void ConvertShadingNetwork::exportShadingGroupEnd(const MString& shadingGroupName)
{
	CM_TRACE_FUNC("ConvertShadingNetwork::exportShadingGroupEnd("<<shadingGroupName.asChar()<<")");
	// if shadingGroupName already exists in exportedShadingGroup, 
	//canShadingGroupExported() will return false, and exportShadingGroupEnd() will not called.
	exportedShadingGroup.push_back(shadingGroupName);
}
// void ConvertShadingNetwork::exportLightShader()
// {
// 	CM_TRACE_FUNC("ConvertShadingNetwork::exportLightShader()");
// 
// }
}//namespace liquidmaya