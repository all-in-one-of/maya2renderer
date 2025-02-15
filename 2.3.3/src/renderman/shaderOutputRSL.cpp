#include "shaderOutputRSL.h"
#include <liqlog.h>
#include <liqShader.h>
#include <liqShaderFactory.h>
#include <liqGlobalVariable.h>
#include <liqGlobalHelpers.h>
#include <common/mayacheck.h>
#include <shadergraph/convertShadingNetwork.h>
#include <shadergraph/shadermgr.h>
#include "ri_interface.h"
#include "rm_helper.h"

namespace RSL
{
	void connectMStringArray(MString& des, const MStringArray& src)
	{
		des.clear();
		for(std::size_t i=0; i<src.length(); ++i){
			des += src[ i ] + ",";
		}
	}
//////////////////////////////////////////////////////////////////////////
OutputHelper::OutputHelper(std::ofstream& RSLfile, renderman::NodePlugInfo& nodePlugMgr)
:RSLfileRef(RSLfile), nodePlugMgrRef(nodePlugMgr)
{
	assert(RSLfileRef.is_open());
}
//
OutputHelper::~OutputHelper()
{

}
//
void OutputHelper::addRSLVariable(const MString& inputQualifier, MString rslType, const MString& rslName,
	const MString& mayaName, const MString& mayaNode)
{
	//process the children of the plug
	MStringArray child;
	IfMErrorMsgWarn(MGlobal::executeCommand("getChildren(\""+mayaNode+"\", \""+mayaName+"\")", child), ("getChildren(\""+mayaNode+"\", \""+mayaName+"\")"));

	for(std::size_t i = 0; i<child.length(); ++i)
	{
		// if this child plug is not connected, skip it.
		MStringArray connections;
		IfMErrorMsgWarn(MGlobal::executeCommand("listConnections -source true -destination true (\""+mayaNode+"."+child[i]+"\")", connections), ("listConnections -source true -destination true (\""+mayaNode+"."+child[i]+"\")"));
		if( connections.length() == 0 )
		{
			continue;
		}

		//get child type
		MString childType;
		IfMErrorMsgWarn(MGlobal::executeCommand("getAttr -type (\""+mayaNode+"."+child[i]+"\")", childType), ("getAttr -type (\""+mayaNode+"."+child[i]+"\")"));
		// process this child plug
		_addRSLVariable(inputQualifier, childType, child[i], child[i], mayaNode);
		nodePlugMgrRef.add(mayaNode.asChar(), child[i].asChar(), childType.asChar());
	}

	//process the plug itself
	_addRSLVariable(inputQualifier, rslType, rslName, mayaName, mayaNode);
	nodePlugMgrRef.add(mayaNode.asChar(), mayaName.asChar(), rslType.asChar());

}
void OutputHelper::_addRSLVariable(const MString& inputQualifier, MString rslType, const MString& rslName,
					const MString& mayaName, const MString& mayaNode)
{
	MString cmd;

	// If the user specified that the type was an array of floats
	// (eg "float2"), extract the size and set the type to float.
	int rslTypeSize = 1;

	MString matchedStr;
	IfMErrorWarn(MGlobal::executeCommand("match(\"float[0-9]+$\",\""+rslType+"\")", matchedStr));
	if(matchedStr != "")
	{
		IfMErrorWarn(MGlobal::executeCommand("match(\"[0-9]+$\",\""+rslType+"\")", matchedStr));
		rslTypeSize = matchedStr.asInt();
		rslType = "float";
	}

	// Create the plug's name, and check for convertible connections.
	MString plug(mayaNode+"."+mayaName);
	liquidmaya::ConnectionType connected = liquidmaya::ShaderMgr::getSingletonPtr()->convertibleConnection(plug.asChar());

	// If there are no convertible connections, then we have to
	// write out the variable into the shader's body.
	if( connected == liquidmaya::CT_None )
	{
		//rslTypeSize(int) --> rslTypeSizeStr(string)
		MString rslTypeSizeStr;
		rslTypeSizeStr.set(rslTypeSize);
		// Write out the description of the variable.
		//left side of the equator
		rslShaderBody += (" "+inputQualifier + " " + rslType + " " + rslName);
		rslShaderBody += ( rslTypeSize != 1 )? ( "[" + rslTypeSizeStr + "]" ) :"";
		//equater
		rslShaderBody += " = ";

		//right side of the equator - 1
		if( rslType == "string" || rslType == "matrix" )
		{
			//do nothing. avoid to generate things like this: 
			//string s0 = string "d:/a.tex";
			// which is can't be compiled in 3delight
		}else{
			if( rslTypeSize == 1 ){
				rslShaderBody += rslType +" ";//e.g. point p0 = point (1,0,0);
			}else{
				rslShaderBody += " ";//e.g. float f[2] = {1.0, 2.0};
			}
		}

		//right side of the equator - 2
		// Write out the value of the variable.
		if(   rslType=="color"
			||rslType=="point"
			||rslType=="normal"
			||rslType=="vector")
		{
			MDoubleArray val; val.setLength(3);
			IfMErrorMsgWarn(MGlobal::executeCommand("getAttr \""+plug+"\"", val), ("getAttr \""+plug+"\""));
			//val(double) --> valStr(string)
			MStringArray valStr; valStr.setLength(3);
			valStr[0].set(val[0]);
			valStr[1].set(val[1]);
			valStr[2].set(val[2]);
			rslShaderBody +="("+valStr[0]+","+valStr[1]+","+valStr[2]+")";
		}else if(rslType=="string"){
			MString val;
			IfMErrorMsgWarn(MGlobal::executeCommand("getAttr \""+plug+"\"", val), ("getAttr \""+plug+"\""));

			val = evaluateTheTextureNameValue(mayaName, val);

			rslShaderBody +="\""+val+"\"";
		}else if(rslType=="float"){
			if(rslTypeSize == 1){
				double val;
				IfMErrorMsgWarn(MGlobal::executeCommand("getAttr \""+plug+"\"", val), ("getAttr \""+plug+"\"") );
				//val(double) --> valStr(string)
				MString valStr;
				valStr.set(val);
				rslShaderBody += valStr;
			}else{
				rslShaderBody += "{ ";
				MDoubleArray val; val.setLength(rslTypeSize);
				IfMErrorMsgWarn(MGlobal::executeCommand("getAttr \""+plug+"\"", val), ("getAttr \""+plug+"\"") );
				for(int i=0; i<rslTypeSize; ++i){
					if( i != 0 ){
						rslShaderBody += ", ";
					}
					//val[i](double) --> valStr(string)
					MString valStr;
					valStr.set(val[i]);
					rslShaderBody += valStr;
				}
				rslShaderBody += " }";
			}
		}else if(rslType=="matrix"){
			MDoubleArray val; val.setLength(16);
			IfMErrorMsgWarn(MGlobal::executeCommand("getAttr \""+plug+"\"", val), ("getAttr \""+plug+"\"") );
			//val(double) --> valStr(string)
			MStringArray valStr; valStr.setLength(16);
			valStr[0].set(val[0]);   valStr[1].set(val[1]);  valStr[2].set(val[2]);    valStr[3].set(val[3]);
			valStr[4].set(val[4]);   valStr[5].set(val[5]);  valStr[6].set(val[6]);    valStr[7].set(val[7]);
			valStr[8].set(val[8]);   valStr[9].set(val[9]);   valStr[10].set(val[10]); valStr[11].set(val[11]);
			valStr[12].set(val[12]); valStr[13].set(val[13]); valStr[14].set(val[14]); valStr[15].set(val[15]);
			rslShaderBody +="("+
				valStr[0] +","+valStr[1] +","+valStr[2] +","+valStr[3] +","+
				valStr[4] +","+valStr[5] +","+valStr[6] +","+valStr[7] +","+
				valStr[8] +","+valStr[9] +","+valStr[10]+","+valStr[11]+","+
				valStr[12]+","+valStr[13]+","+valStr[14]+","+valStr[15]+
				")";
		}else{
			liquidMessage2(messageError, "rsl type [%s] is unhandled.", rslType.asChar());
		}
		rslShaderBody += ";\n";
	}//if( $connected == CT_None )
	// Otherwise, we have a convertible connection, so we'll be
	// adding the variable to the block's header.
	else{
		rslShaderHeader += " ";

		// Note if it's connected as an output.
		if(connected == liquidmaya::CT_Out || connected == liquidmaya::CT_InOut){
			rslShaderHeader += "output ";
		}

		// Write out the description.
		rslShaderHeader += ( rslType + " " + rslName );
		if( rslTypeSize != 1 )
		{
			rslShaderHeader += "[]";
		}
		rslShaderHeader += ";\n";

		// Note if it's connected as an output. set the value
		if(connected == liquidmaya::CT_Out || connected == liquidmaya::CT_InOut)
		{
			//rslTypeSize(int) --> rslTypeSizeStr(string)
			MString rslTypeSizeStr;
			rslTypeSizeStr.set(rslTypeSize);

			// Write out the value of the variable.
			if(   rslType=="color"
				||rslType=="point"
				||rslType=="normal"
				||rslType=="vector")
			{
				if(rslTypeSize == 1){
					MDoubleArray val; val.setLength(3);
					IfMErrorMsgWarn(MGlobal::executeCommand("getAttr \""+plug+"\"", val), ("getAttr \""+plug+"\"") );
					//val(double) --> valStr(string)
					MStringArray valStr; valStr.setLength(3);
					valStr[0].set(val[0]);
					valStr[1].set(val[1]);
					valStr[2].set(val[2]);
					rslShaderBody += " "+rslName+" = "+rslType+" ("+valStr[0]+","+valStr[1]+","+valStr[2]+")";
				}else{
					rslShaderBody += " "+rslName+"["+rslTypeSizeStr+"] is an array of "+rslType+", not implemented yet.";
				}
			}else if(rslType=="string"){
				if(rslTypeSize == 1){
					MString val;
					IfMErrorMsgWarn(MGlobal::executeCommand("getAttr \""+plug+"\"", val), ("getAttr \""+plug+"\""));

					val = evaluateTheTextureNameValue(mayaName, val);

					rslShaderBody += " "+rslName+" = "+rslType+"\""+val+"\"";
				}else{
					rslShaderBody += " "+rslName+"["+rslTypeSizeStr+"] is an array of "+rslType+", not implemented yet.";
				}
			}else if(rslType=="float"){
				if(rslTypeSize == 1){
					double val;
					IfMErrorMsgWarn(MGlobal::executeCommand("getAttr \""+plug+"\"", val), ("getAttr \""+plug+"\""));
					//val(double) --> valStr(string)
					MString valStr;
					valStr.set(val);
					rslShaderBody += " "+rslName+" = "+rslType+" "+valStr;
				}else{
					MDoubleArray val; val.setLength(rslTypeSize);
					IfMErrorMsgWarn(MGlobal::executeCommand("getAttr \""+plug+"\"", val), ("getAttr \""+plug+"\"") );
					for(int i=0; i<rslTypeSize; ++i){
						MString I;
						I.set(i);
						//val[i](double) --> valStr(string)
						MString valStr;
						valStr.set(val[i]);
						//rslShaderBody += valStr;
						rslShaderBody += " "+rslName+"["+I+"]"+" = "+rslType+" "+valStr+"; ";
					}
				}
			}else if(rslType=="matrix"){
				if(rslTypeSize == 1){
				MDoubleArray val; val.setLength(16);
				IfMErrorMsgWarn(MGlobal::executeCommand("getAttr \""+plug+"\"", val), ("getAttr \""+plug+"\"") );
				//val(double) --> valStr(string)
				MStringArray valStr; valStr.setLength(16);
				valStr[0].set(val[0]);   valStr[1].set(val[1]);  valStr[2].set(val[2]);    valStr[3].set(val[3]);
				valStr[4].set(val[4]);   valStr[5].set(val[5]);  valStr[6].set(val[6]);    valStr[7].set(val[7]);
				valStr[8].set(val[8]);   valStr[9].set(val[9]);   valStr[10].set(val[10]); valStr[11].set(val[11]);
				valStr[12].set(val[12]); valStr[13].set(val[13]); valStr[14].set(val[14]); valStr[15].set(val[15]);
				rslShaderBody +="("+
					valStr[0] +","+valStr[1] +","+valStr[2] +","+valStr[3] +","+
					valStr[4] +","+valStr[5] +","+valStr[6] +","+valStr[7] +","+
					valStr[8] +","+valStr[9] +","+valStr[10]+","+valStr[11]+","+
					valStr[12]+","+valStr[13]+","+valStr[14]+","+valStr[15]+
					")";
				}else{
					rslShaderBody += " "+rslName+"["+rslTypeSizeStr+"] is an array of "+rslType+", not implemented yet.";
				}
			}else{
				liquidMessage2(messageError, "rsl type [%s] is unhandled.", rslType.asChar());
			}
			rslShaderBody += ";\n";
		}//

		//
		if(connected == liquidmaya::CT_In)
		{
			MStringArray srcPlug;
			IfMErrorMsgWarn(MGlobal::executeCommand("listConnections -source true -destination off -plugs true \""+plug+"\"", srcPlug),
												   ("listConnections -source true -destination off -plugs true \""+plug+"\""));
			assert(srcPlug.length()==1);
			rslShaderBody +="//"+plug+" <-- "+srcPlug[0]+"\n";
		}

	}//else
}
//
void OutputHelper::addToRSL(const MString& code)
{
	rslShaderBody += (" "+code+"\n");
}
//
void OutputHelper::beginRSL (const MString& name)
{
	CM_TRACE_FUNC("OutputHelper::beginRSL("<<name.asChar()<<")");

	// "Open" the header and body.
	//
	rslShaderHeader  = ( "//shader name: " + name + "\n" );
	rslShaderHeader += ( "void " + getShaderName(name) + " (\n" );
	rslShaderBody = "{\n";
}
//
void OutputHelper::endRSL ()
{
	CM_TRACE_FUNC("OutputHelper::endRSL()");

	// "Close" the header and body.
	//
	rslShaderHeader += ")\n";
	rslShaderBody += "}\n";

	//include files
	std::set<std::string>::iterator i = includedFiles.begin();
	std::set<std::string>::iterator e = includedFiles.end();
	for(; i!=e; ++i){
		RSLfileRef << "#include \""<< *i<<"\"\n";//"" is better than <>
	}
	//
	RSLfileRef << (rslShaderHeader + rslShaderBody + "\n").asChar() ;
}
void OutputHelper::addInclude(const MString& file)
{
	CM_TRACE_FUNC("OutputHelper::addInclude("<<file.asChar()<<")");
	includedFiles.insert(file.asChar());
}
void OutputHelper::addIncludeUserShader(const MString &userShaderNodeType)
{
	CM_TRACE_FUNC("OutputHelper::addIncludeUserShader("<<userShaderNodeType.asChar()<<")");

	MString RM_SHADER_USER;
	IfMErrorWarn(MGlobal::executeCommand("getenv(\"RM_SHADER_USER\")", RM_SHADER_USER));
	
	addInclude(RM_SHADER_USER+"/src/"+userShaderNodeType+"/"+userShaderNodeType+".impl");
}
//////////////////////////////////////////////////////////////////////////
Visitor::Visitor()
{

}
//
Visitor::~Visitor()
{

}
//
void Visitor::initShaderData(const MString& startingNode)
{
	CM_TRACE_FUNC("Visitor::initShaderData("<<startingNode.asChar()<<")");

	shaderType = getRSLShaderType(startingNode);

	shaderData.setLength(3);
	shaderData[SHADER_METHOD_VARIAVLES_I] = "";// shader method variables
	shaderData[1]						  = "";// shader method body
	shaderData[SHADER_NAME_I]             = startingNode;// shader name
	
	m_NodePlugConnectionMgr.setNodePlugMgr( &m_NodePlugInfo );
	m_NodePlugConnectionMgr.reset();
}
void Visitor::preOutput(const char* shaderNodeName)
{
	CM_TRACE_FUNC("Visitor::preOutput("<<shaderNodeName<<")");

	this->shaderNodeName = shaderNodeName;
}
void Visitor::outputBegin(const char* shaderNodeName)
{
	CM_TRACE_FUNC("Visitor::outputBegin("<<shaderNodeName<<"), open shader file");
	RSLfile.open( renderman::getShaderFilePath_SRC(shaderNodeName).asChar() );

	RSLfile << "#include <"<<getAOVMacroDefineFileName()<<">" <<std::endl;

	RSLfile << "//surface shader name: " << shaderNodeName << std::endl;
	RSLfile << shaderType.asChar()<<" "<< getShaderName(shaderNodeName).asChar() << std::endl;
	
	//shader parameters
	RSLfile << "(" << std::endl;
	RSLfile << getSurfaceShaderAOVOutputParametersString() << std::endl;
	RSLfile << ")"<< std::endl;

	//shader body begin
	RSLfile << "{"<< std::endl;

	defineAOVVariables();

	RSLfile << "\n\n\n// define the shaders --------------------------------------------------------------------------\n";
}
void Visitor::outputUpstreamShader(const char* shaderNodeName)
{
	CM_TRACE_FUNC("Visitor::outputUpstreamShader("<<shaderNodeName<<")");

	MString nodetype;
	IfMErrorMsgWarn(MGlobal::executeCommand( ("nodeType \""+MString(shaderNodeName)+"\""), nodetype), ("nodeType \""+MString(shaderNodeName)+"\""));

	_outputUpstreamShader(shaderNodeName, nodetype.asChar());
}
void Visitor::outputShaderMethod()
{
	CM_TRACE_FUNC("Visitor::outputShaderMethod()");

//	m_NodePlugInfo.print("  ",">");
//	m_NodePlugConnectionMgr.print("  ", ">");

	RSLfile << "\n\n\n// call the shaders ----------------------------------------------------------------------------\n";
	RSLfile << shaderData[SHADER_METHOD_VARIAVLES_I].asChar();
	RSLfile << "\n";

	for(std::size_t i = 0; i<m_NodePlugConnectionMgr.size(); ++i)
	{
		RSLfile << m_NodePlugConnectionMgr.cookRSLFunctionCallString(i) <<std::endl;
		RSLfile << m_NodePlugConnectionMgr.log(i) << std::endl;	
	}
//	RSLfile << shaderData[SHADER_METHOD_BODY_I].asChar();
	RSLfile << "}\n";
}
void  Visitor::addShaderMethodBody(
						 const MString &currentNode,
						 const MStringArray& inputSrc,
						 const MStringArray& inputDes,
						 const MStringArray& outputSrc)
{
	CM_TRACE_FUNC("Visitor::addShaderMethodBody("<<currentNode.asChar()<<","<<liqM(inputSrc)<<","<<liqM(inputDes)<<","<<liqM(outputSrc)<<")");
	
	m_NodePlugConnectionMgr.add(currentNode, inputSrc, inputDes, outputSrc);

// 	MString varString;
// 	{
// 		MStringArray vars;
// 		for(std::size_t i=0; i<inputSrc.length(); ++i){
// 			vars.append( inputSrc[i] );
// 		}
// 		for(std::size_t i=0; i<outputSrc.length(); ++i){
// 			vars.append( outputSrc[i] );
// 		}
// 
// 		for(std::size_t index=0; index<vars.length(); ++index){
// 			varString += (index == vars.length() - 1)?(vars[index]):(vars[index]+", ");
// 		}
// 	}
// 
// 	// Add the current node method to the shader body
// 	shaderData[ SHADER_METHOD_BODY_I ] += " //call shader "+ getShaderName(currentNode) + ", var: "+varString+"\n";
// 	varString = boost::replace_all_copy(std::string(varString.asChar()),".","_").c_str();
// 	shaderData[ SHADER_METHOD_BODY_I ] += " " + getShaderName(currentNode) +"("+varString+");\n";
// 	
// 	// test the input and output of currentNode
// 	{	
// 		MString inputSrcStr; 
//		MString inputDesStr; 
//		MString outputSrcStr;
// 		connectMStringArray(inputSrcStr, inputSrc);
// 		connectMStringArray(inputDesStr, inputDes);
// 		connectMStringArray(outputSrcStr, outputSrc);
//		shaderData[ SHADER_METHOD_BODY_I ] += "//inputSrc: " + inputSrcStr +"\n";
//		shaderData[ SHADER_METHOD_BODY_I ] += "//inputDes: " + inputDesStr +"\n";
//		shaderData[ SHADER_METHOD_BODY_I ] += "//outputSrc:" + outputSrcStr +"\n\n";
// 	}
}
void Visitor::addShaderMethodVariavles(
	const MString &node,
	const MString &plug,
	const MString &type_,
	const int arraysize,
	const MString &details)
{
	CM_TRACE_FUNC("Visitor::addShaderMethodVariavles("<<node.asChar()<<","<<plug.asChar()<<","<<type_.asChar()<<","<<arraysize<<","<<details.asChar()<<")");

	if(arraysize == -1)
	{
		shaderData[SHADER_METHOD_VARIAVLES_I] += " "+type_+" "+ getVariableName(node, plug)+";\n";
	}else{
		MString strArraySize;
		strArraySize.set(arraysize);
		shaderData[SHADER_METHOD_VARIAVLES_I] += " "+type_+" "+ getVariableName(node, plug)+"["+strArraySize+"];\n";
	}
}
void Visitor::outputEnd()
{
	CM_TRACE_FUNC("Visitor::outputEnd(), close shader file");
	RSLfile.close();
}
void Visitor::postOutput()
{
	CM_TRACE_FUNC("Visitor::postOutput()");


	//compile the shader
	MString shaderdir(getShaderDirectory());
	MString outSLO(renderman::getShaderFilePath_SLO(shaderNodeName.c_str()));
	MString srcSL (renderman::getShaderFilePath_SRC(shaderNodeName.c_str()));

	MString result;
	//NOTE:
	//     the include directory can't contain '.' in PRman, so I move _3delight to %LIQUIDHOME%\..\dependence
	//"shader.exe -o \"outSLO\" -I\"%LIQUIDHOME%\..\dependence\_3delight" \"srcSL\""
	IfMErrorWarn(MGlobal::executeCommand("system(\""+liqglo.liquidRenderer.shaderCompiler+" -o \\\""+outSLO+"\\\" -I\\\"%LIQUIDHOME%/../dependence/_3delight\\\" -I\\\"%LIQUIDHOME%/lib/shaders/prman13.5\\\" \\\""+srcSL+"\\\"\")", result, true));

	MGlobal::displayInfo( "-------------------compile shader message begin ("+srcSL+")-------------------" );
	MGlobal::displayError( result );
	MGlobal::displayInfo( "-------------------compile shader message end   ("+srcSL+")-------------------" );

	//show the error if there is.
	std::string strRes(result.toLowerCase().asChar());
	if(strRes.find("error") != std::string::npos)
	{
		//don't use liquidMessage2() here, because the message buffer is not large enough.
		liqAssert("Error", "compile shader: "+srcSL+", see scriptEditor for more details.", "Yes");
	}


}
void Visitor::outputShadingGroup(const char* shadingGroupNode)
{
	CM_TRACE_FUNC("Visitor::outputShadingGroup("<<shadingGroupNode<<")");
	
	outputShadingGroup(shadingGroupNode, false);
}
void Visitor::outputShadingGroup(const char* shadingGroupNode, const bool bReference)
{
	CM_TRACE_FUNC("Visitor::outputShadingGroup("<<shadingGroupNode<<","<<bReference<<")");
	
	if(liqglo.m_writeDataIntoMainRibFile)
	{
		if( bReference ){
			//here is the right place where shading group data should be written into the main rib file.
			_outputShadingGroup(shadingGroupNode);
		}else{
			//do nothing
		}
	}else{
		if( !bReference ){
			renderman::Helper o;
			o.RiBeginRef(renderman::getShadingGroupFilePath(shadingGroupNode).asChar());
			_outputShadingGroup(shadingGroupNode);
			o.RiEndRef();
		}else{
			RiArchiveRecord( RI_COMMENT, "use Shading Group reference:" );
			RiReadArchive( const_cast< RtToken >( renderman::getShadingGroupFilePath(shadingGroupNode).asChar() ), NULL, RI_NULL );
		}
	}
}

void _outputShadingGroup(const char* shadingGroupNode)
{
	CM_TRACE_FUNC("_outputShadingGroup("<<shadingGroupNode<<")");
	//
	MStringArray surfaceShaders;
	MStringArray volumeShaders;
	MStringArray displacementShaders;
	{
		getlistConnections(shadingGroupNode, "surfaceShader", surfaceShaders);
		getlistConnections(shadingGroupNode, "volumeShader", volumeShaders);
		getlistConnections(shadingGroupNode, "displacementShader", displacementShaders);
	}
	//
	RiArchiveRecord(RI_COMMENT, "shading group: %s", shadingGroupNode);
	//surface shader
	if( surfaceShaders[0].length() != 0 ){
		MString nodetype;
		getNodeType(nodetype, surfaceShaders[0]);
		if( nodetype == "liquidSurface" ){
			liqShader& currentShader = 
				liqShaderFactory::instance().getShader( surfaceShaders[0].asChar() );
			currentShader.write();
		}else{
			RiSurface( const_cast<char *>(renderman::getShaderFilePath_NoExt(surfaceShaders[0]).asChar()), RI_NULL );
		}
	}else{
		RiArchiveRecord(RI_COMMENT, "no surface shader.");
	}
	//volume shader
	if( volumeShaders[0].length() != 0 ){
		MString nodetype;
		getNodeType(nodetype, volumeShaders[0]);
		if( nodetype == "liquidVolume" ){
			liqShader& currentShader = 
				liqShaderFactory::instance().getShader( volumeShaders[0].asChar() );
			currentShader.write();
		}else{
			RiArchiveRecord(RI_COMMENT, "I'm not sure which one should be used for the volume shader, RiAtmosphere(), RiInterior(), or RiExterior().");
			RiAtmosphere( const_cast<char *>(renderman::getShaderFilePath_NoExt(volumeShaders[0]).asChar()), RI_NULL );
		}
	}else{
		RiArchiveRecord(RI_COMMENT, "no volume shader.");
	}
	//displacement shader
	if( displacementShaders[0].length() != 0 ){
		MString nodetype;
		getNodeType(nodetype, displacementShaders[0]);
		if( nodetype == "liquidDisplacement" ){
			liqShader& currentShader = 
				liqShaderFactory::instance().getShader( displacementShaders[0].asChar() );
			currentShader.write();
		}else{
			RiArchiveRecord(RI_COMMENT, "I'm not sure which one should be used for the displacement shader, RiDeformation(), or RiDisplacement().");
			RiDisplacement( const_cast<char *>(renderman::getShaderFilePath_NoExt(displacementShaders[0]).asChar()), RI_NULL );
		}
	}else{
		RiArchiveRecord(RI_COMMENT, "no displacement shader.");
	}
}
MString Visitor::getRSLShaderType(const MString &startingNode)
{
	MString shaderType;

	MString nodeType(getNodeType(startingNode));

	//liquidShader
	if(nodeType=="liquidShader"){
		IfMErrorWarn(MGlobal::executeCommand("getAttr \""+startingNode+".rmanShaderType\";", shaderType));
	}
	//liquid Surface/Displacement/Volume/Light
	else if(nodeType=="liquidSurface"){
		shaderType = "surface";
	}
	else if(nodeType=="liquidDisplacement"){
		shaderType = "displacement";
	}
	else if(nodeType=="liquidVolume"){
		shaderType = "volume";
	}
	else if(nodeType=="liquidLight"){
		shaderType = "light";
	}
	//maya shader
	else{
		if(renderman::isClassification(nodeType, "shader/surface"))
		{
			shaderType = "surface";
			//It is weird that liquidLight is not displayed in Hypershade top tab 'light', 
			//so I put liquidLight into surface tab temporarily.
			if(nodeType=="light")
			{
				shaderType = "light";
			}
		}else if(renderman::isClassification(nodeType, "shader/displacement")){
			shaderType = "displacement";
		}else if(renderman::isClassification(nodeType, "shader/volume")){
			shaderType = "volume";
		}else if(renderman::isClassification(nodeType, "light")){
			shaderType = "light";
		}else if(renderman::isClassification(nodeType, "imageplane")){
			shaderType = "imager";
		}else{
			liquidMessage2(messageError,"don't know how to translate shader type[%s] of node[%s] to renderman", nodeType.asChar(), startingNode.asChar());
		}
	}

	return shaderType;
}
void Visitor::defineAOVVariables()
{
	CM_TRACE_FUNC("Visitor::defineAOVVariables("<<shaderNodeName<<")");

	RSLfile << "\n\n\n// define some extern variables which are used in 3delight shaders -----------------------------\n";
	RSLfile << " color __transparency = color (1,1,1);" << std::endl;
	RSLfile << " uniform float __reflects = 0;" << std::endl;
	RSLfile << " uniform float __refracts = 0;" << std::endl;

	std::stringstream aov_params;
	aov_params 
	//begin shader_extra_parameters aov_ambient
	<<"#ifdef USE_AOV_aov_ambient"	<<std::endl
	<<"		color aov_ambient = 0;"	<<std::endl
	<<"#endif"						<<std::endl
	//end shader_extra_parameters

	//begin shader_extra_parameters aov_diffuse
	<<"#ifdef USE_AOV_aov_diffuse"	<<std::endl
	<<"		color aov_diffuse = 0;"	<<std::endl
	<<"#endif"						<<std::endl
	//end shader_extra_parameters

	//begin shader_extra_parameters aov_specular
	<<"#ifdef USE_AOV_aov_specular"		<<std::endl
	<<"		color aov_specular = 0;"	<<std::endl
	<<"#endif"							<<std::endl
	//end shader_extra_parameters

	//begin shader_extra_parameters aov_reflection
	<<"#ifdef USE_AOV_aov_reflection"	<<std::endl
	<<"		color aov_reflection = 0;"	<<std::endl
	<<"#endif"							<<std::endl
	//end shader_extra_parameters

	//begin shader_extra_parameters aov_rt_reflection
	<<"#ifdef USE_AOV_aov_rt_reflection"	<<std::endl
	<<"		color aov_rt_reflection = 0;"	<<std::endl
	<<"#endif"								<<std::endl
	//end shader_extra_parameters

	//begin shader_extra_parameters aov_rt_reflection_alpha
	<<"#ifdef USE_AOV_aov_rt_reflection_alpha"	<<std::endl
	<<"		float aov_rt_reflection_alpha = 0;"	<<std::endl
	<<"#endif"									<<std::endl
	//end shader_extra_parameters

	//begin shader_extra_parameters aov_env_reflection
	<<"#ifdef USE_AOV_aov_env_reflection"	<<std::endl
	<<"		color aov_env_reflection = 0;"	<<std::endl
	<<"#endif"								<<std::endl
	//end shader_extra_parameters

	//begin shader_extra_parameters aov_refraction
	<<"#ifdef USE_AOV_aov_refraction"	<<std::endl
	<<"		color aov_refraction = 0;"	<<std::endl
	<<"#endif"							<<std::endl
	//end shader_extra_parameters

	//begin shader_extra_parameters aov_shadow
	<<"#ifdef USE_AOV_aov_shadow"	<<std::endl
	<<"		float aov_shadow = 0;"	<<std::endl
	<<"#endif"						<<std::endl
	//end shader_extra_parameters 

	//begin shader_extra_parameters aov_incandescence
	<<"#ifdef USE_AOV_aov_incandescence"	<<std::endl
	<<"		color aov_incandescence = 0;"	<<std::endl
	<<"#endif"								<<std::endl
	//end shader_extra_parameters

	//begin shader_extra_parameters aov_translucence
	<<"#ifdef USE_AOV_aov_translucence"		<<std::endl
	<<"		color aov_translucence = 0;"	<<std::endl
	<<"#endif"								<<std::endl
	//end shader_extra_parameters

	//begin shader_extra_parameters aov_ambient_no_shadow
	<<"#ifdef USE_AOV_aov_ambient_no_shadow"	<<std::endl
	<<"		color aov_ambient_no_shadow = 0;"	<<std::endl
	<<"#endif"									<<std::endl
	//end shader_extra_parameters

	//begin shader_extra_parameters aov_diffuse_no_shadow
	<<"#ifdef USE_AOV_aov_diffuse_no_shadow"	<<std::endl
	<<"		color aov_diffuse_no_shadow = 0;"	<<std::endl
	<<"#endif"									<<std::endl
	//end shader_extra_parameters

	//begin shader_extra_parameters aov_diffuse_intensity
	<<"#ifdef USE_AOV_aov_diffuse_intensity"	<<std::endl
	<<"		color aov_diffuse_intensity = 0;"	<<std::endl
	<<"#endif"									<<std::endl
	//end shader_extra_parameters

	//begin shader_extra_parameters aov_diffuse_intensity_no_shadow
	<<"#ifdef USE_AOV_aov_diffuse_intensity_no_shadow"	<<std::endl
	<<"		color aov_diffuse_intensity_no_shadow = 0;"	<<std::endl
	<<"#endif"											<<std::endl
	//end shader_extra_parameters

	//begin shader_extra_parameters aov_key_lights_diffuse_intensity
	<<"#ifdef USE_AOV_aov_key_lights_diffuse_intensity"		<<std::endl
	<<"		color aov_key_lights_diffuse_intensity = 0;"	<<std::endl
	<<"#endif"												<<std::endl
	//end shader_extra_parameters

	//begin shader_extra_parameters aov_key_lights_diffuse_intensity_no_shadow
	<<"#ifdef USE_AOV_aov_key_lights_diffuse_intensity_no_shadow"	<<std::endl
	<<"		color aov_key_lights_diffuse_intensity_no_shadow = 0;"	<<std::endl
	<<"#endif"														<<std::endl
	//end shader_extra_parameters

	//begin shader_extra_parameters aov_luminance_depth
	<<"#ifdef USE_AOV_aov_luminance_depth"	<<std::endl
	<<"		color aov_luminance_depth = 0;"	<<std::endl
	<<"#endif"								<<std::endl
	//end shader_extra_parameters

	//begin shader_extra_parameters aov_specular_no_shadow
	<<"#ifdef USE_AOV_aov_specular_no_shadow"	<<std::endl
	<<"		color aov_specular_no_shadow = 0;"	<<std::endl
	<<"#endif"									<<std::endl
	//end shader_extra_parameters

	//begin shader_extra_parameters aov_specular_intensity
	<<"#ifdef USE_AOV_aov_specular_intensity"	<<std::endl
	<<"		color aov_specular_intensity = 0;"	<<std::endl
	<<"#endif"									<<std::endl
	//end shader_extra_parameters

	//begin shader_extra_parameters aov_specular_intensity_no_shadow
	<<"#ifdef USE_AOV_aov_specular_intensity_no_shadow"		<<std::endl
	<<"		color aov_specular_intensity_no_shadow = 0;"	<<std::endl
	<<"#endif"												<<std::endl
	//end shader_extra_parameters

	//begin shader_extra_parameters aov_key_lights_specular_intensity
	<<"#ifdef USE_AOV_aov_key_lights_specular_intensity"	<<std::endl
	<<"		color aov_key_lights_specular_intensity = 0;"	<<std::endl
	<<"#endif"												<<std::endl
	//end shader_extra_parameters

	//begin shader_extra_parameters aov_key_lights_specular_intensity_no_shadow
	<<"#ifdef USE_AOV_aov_key_lights_specular_intensity_no_shadow"	<<std::endl
	<<"		color aov_key_lights_specular_intensity_no_shadow = 0;"	<<std::endl
	<<"#endif"														<<std::endl
	//end shader_extra_parameters

	//begin shader_extra_parameters aov_color_no_shadow
	<<"#ifdef USE_AOV_aov_color_no_shadow"	<<std::endl
	<<"		color aov_color_no_shadow = 0;"	<<std::endl
	<<"#endif"								<<std::endl
	//end shader_extra_parameters

	//begin shader_extra_parameters aov_surface_color
	<<"#ifdef USE_AOV_aov_surface_color"	<<std::endl
	<<"		color aov_surface_color = 0;"	<<std::endl
	<<"#endif"								<<std::endl
	//end shader_extra_parameters

	//begin shader_extra_parameters aov_normal
	<<"#ifdef USE_AOV_aov_camera_space_normal"		<<std::endl
	<<"		normal aov_camera_space_normal = 0;"	<<std::endl
	<<"#endif"										<<std::endl
	//end shader_extra_parameters

	//begin shader_extra_parameters aov_facing_ratio
	<<"#ifdef USE_AOV_aov_facing_ratio"				<<std::endl
	<<"		float aov_facing_ratio = 0;"			<<std::endl
	<<"#endif"										<<std::endl
	//end shader_extra_parameters
	;
	//RSLfile << aov_params.str();
}
std::string Visitor::getSurfaceShaderAOVOutputParametersString()const
{
	CM_TRACE_FUNC("Visitor::getSurfaceShaderAOVOutputParametersString()");

	std::stringstream ret;
	ret 
//begin shader_extra_parameters aov_ambient
<<"#ifdef USE_AOV_aov_ambient"					<<std::endl
<<"		output varying color aov_ambient = 0;"	<<std::endl
<<"#endif"										<<std::endl
//end shader_extra_parameters

//begin shader_extra_parameters aov_diffuse
<<"#ifdef USE_AOV_aov_diffuse"					<<std::endl
<<"		output varying color aov_diffuse = 0;"	<<std::endl
<<"#endif"										<<std::endl
//end shader_extra_parameters

//begin shader_extra_parameters aov_specular
<<"#ifdef USE_AOV_aov_specular"					<<std::endl
<<"		output varying color aov_specular = 0;"	<<std::endl
<<"#endif"										<<std::endl
//end shader_extra_parameters

//begin shader_extra_parameters aov_reflection
<<"#ifdef USE_AOV_aov_reflection"					<<std::endl
<<"		output varying color aov_reflection = 0;"	<<std::endl
<<"#endif"											<<std::endl
//end shader_extra_parameters

//begin shader_extra_parameters aov_rt_reflection
<<"#ifdef USE_AOV_aov_rt_reflection"					<<std::endl
<<"		output varying color aov_rt_reflection = 0;"	<<std::endl
<<"#endif"												<<std::endl
//end shader_extra_parameters

//begin shader_extra_parameters aov_rt_reflection_alpha
<<"#ifdef USE_AOV_aov_rt_reflection_alpha"					<<std::endl
<<"		output varying float aov_rt_reflection_alpha = 0;"	<<std::endl
<<"#endif"													<<std::endl
//end shader_extra_parameters

//begin shader_extra_parameters aov_env_reflection
<<"#ifdef USE_AOV_aov_env_reflection"					<<std::endl
<<"		output varying color aov_env_reflection = 0;"	<<std::endl
<<"#endif"												<<std::endl
//end shader_extra_parameters

//begin shader_extra_parameters aov_refraction
<<"#ifdef USE_AOV_aov_refraction"					<<std::endl
<<"		output varying color aov_refraction = 0;"	<<std::endl
<<"#endif"											<<std::endl
//end shader_extra_parameters

//begin shader_extra_parameters aov_shadow
<<"#ifdef USE_AOV_aov_shadow"					<<std::endl
<<"		output varying float aov_shadow = 0;"	<<std::endl
<<"#endif"										<<std::endl
//end shader_extra_parameters 

//begin shader_extra_parameters aov_incandescence
<<"#ifdef USE_AOV_aov_incandescence"					<<std::endl
<<"		output varying color aov_incandescence = 0;"	<<std::endl
<<"#endif"												<<std::endl
//end shader_extra_parameters

//begin shader_extra_parameters aov_translucence
<<"#ifdef USE_AOV_aov_translucence"					<<std::endl
<<"		output varying color aov_translucence = 0;"	<<std::endl
<<"#endif"											<<std::endl
//end shader_extra_parameters

//begin shader_extra_parameters aov_ambient_no_shadow
<<"#ifdef USE_AOV_aov_ambient_no_shadow"					<<std::endl
<<"		output varying color aov_ambient_no_shadow = 0;"	<<std::endl
<<"#endif"													<<std::endl
//end shader_extra_parameters

//begin shader_extra_parameters aov_diffuse_no_shadow
<<"#ifdef USE_AOV_aov_diffuse_no_shadow"					<<std::endl
<<"		output varying color aov_diffuse_no_shadow = 0;"	<<std::endl
<<"#endif"													<<std::endl
//end shader_extra_parameters

//begin shader_extra_parameters aov_diffuse_intensity
<<"#ifdef USE_AOV_aov_diffuse_intensity"					<<std::endl
<<"		output varying color aov_diffuse_intensity = 0;"	<<std::endl
<<"#endif"													<<std::endl
//end shader_extra_parameters

//begin shader_extra_parameters aov_diffuse_intensity_no_shadow
<<"#ifdef USE_AOV_aov_diffuse_intensity_no_shadow"					<<std::endl
<<"		output varying color aov_diffuse_intensity_no_shadow = 0;"	<<std::endl
<<"#endif"															<<std::endl
//end shader_extra_parameters

//begin shader_extra_parameters aov_key_lights_diffuse_intensity
<<"#ifdef USE_AOV_aov_key_lights_diffuse_intensity"					<<std::endl
<<"		output varying color aov_key_lights_diffuse_intensity = 0;"	<<std::endl
<<"#endif"															<<std::endl
//end shader_extra_parameters

//begin shader_extra_parameters aov_key_lights_diffuse_intensity_no_shadow
<<"#ifdef USE_AOV_aov_key_lights_diffuse_intensity_no_shadow"					<<std::endl
<<"		output varying color aov_key_lights_diffuse_intensity_no_shadow = 0;"	<<std::endl
<<"#endif"																		<<std::endl
//end shader_extra_parameters

//begin shader_extra_parameters aov_luminance_depth
<<"#ifdef USE_AOV_aov_luminance_depth"					<<std::endl
<<"		output varying color aov_luminance_depth = 0;"	<<std::endl
<<"#endif"												<<std::endl
//end shader_extra_parameters

//begin shader_extra_parameters aov_specular_no_shadow
<<"#ifdef USE_AOV_aov_specular_no_shadow"					<<std::endl
<<"		output varying color aov_specular_no_shadow = 0;"	<<std::endl
<<"#endif"													<<std::endl
//end shader_extra_parameters

//begin shader_extra_parameters aov_specular_intensity
<<"#ifdef USE_AOV_aov_specular_intensity"					<<std::endl
<<"		output varying color aov_specular_intensity = 0;"	<<std::endl
<<"#endif"													<<std::endl
//end shader_extra_parameters

//begin shader_extra_parameters aov_specular_intensity_no_shadow
<<"#ifdef USE_AOV_aov_specular_intensity_no_shadow"					<<std::endl
<<"		output varying color aov_specular_intensity_no_shadow = 0;"	<<std::endl
<<"#endif"															<<std::endl
//end shader_extra_parameters

//begin shader_extra_parameters aov_key_lights_specular_intensity
<<"#ifdef USE_AOV_aov_key_lights_specular_intensity"					<<std::endl
<<"		output varying color aov_key_lights_specular_intensity = 0;"	<<std::endl
<<"#endif"																<<std::endl
//end shader_extra_parameters

//begin shader_extra_parameters aov_key_lights_specular_intensity_no_shadow
<<"#ifdef USE_AOV_aov_key_lights_specular_intensity_no_shadow"					<<std::endl
<<"		output varying color aov_key_lights_specular_intensity_no_shadow = 0;"	<<std::endl
<<"#endif"																		<<std::endl
//end shader_extra_parameters

//begin shader_extra_parameters aov_color_no_shadow
<<"#ifdef USE_AOV_aov_color_no_shadow"					<<std::endl
<<"		output varying color aov_color_no_shadow = 0;"	<<std::endl
<<"#endif"												<<std::endl
//end shader_extra_parameters

//begin shader_extra_parameters aov_surface_color
<<"#ifdef USE_AOV_aov_surface_color"					<<std::endl
<<"		output varying color aov_surface_color = 0;"	<<std::endl
<<"#endif"												<<std::endl
//end shader_extra_parameters

//begin shader_extra_parameters aov_normal
<<"#ifdef USE_AOV_aov_camera_space_normal"					<<std::endl
<<"		output varying normal aov_camera_space_normal = 0;"	<<std::endl
<<"#endif"													<<std::endl
//end shader_extra_parameters

//begin shader_extra_parameters aov_facing_ratio
<<"#ifdef USE_AOV_aov_facing_ratio"							<<std::endl
<<"		output varying float aov_facing_ratio = 0;"			<<std::endl
<<"#endif"													<<std::endl
//end shader_extra_parameters
;
		return ret.str();
}
std::string Visitor::getAOVMacroDefineFileName()const
{
	return "liquidAOVMacroDef.h";
}
//
std::ofstream& Visitor::getOutfstreamRef()
{
	return RSLfile;
}
renderman::NodePlugInfo& Visitor::getNodePlugInfoRef()
{
	return m_NodePlugInfo;
}
}//namespace RSL