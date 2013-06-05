//-

#include "liqIPRNodeMessageCmd.h"
#include <liqGlobalHelpers.h>
#include <common/mayacheck.h>
#include "renderermgr.h"
#include "liqRibTranslator.h"
#include "shadergraph/shadermgr.h"


#define kRegisterFlagLong		"-register"
#define kRegisterFlag			"-rgt"
#define kUnregisterFlagLong		"-unregister"
#define kUnregisterFlag			"-urg"
#define kIsRunningIPRLong		"-isrunning"
#define kIsRunningIPR			"-ir"

// This table will keep track of the registered callbacks
// so they can be removed when the plug-ins is unloaded.
//
MCallbackIdArray liqIPRNodeMessage::callbackIds;

//////////////////////////////////////////////////////////////////////////
//
// Callback function
//
//    Prints out plug information when connections are made or broken.
//    See MNodeMessage.h for all of the available AttributeMessage types.
//
//////////////////////////////////////////////////////////////////////////

void liquidIPR_AttributeChangedCallback( MNodeMessage::AttributeMessage msg, 
	MPlug & plug, MPlug & otherPlug, void *userData )
{
	MGlobal::displayInfo( "liquidIPR_AttributeChangedCallback(msg, "+ plug.name()+","+otherPlug.name()+", userData)");
	//liquidMessage2(messageInfo, "msg=%0x", msg);

	static std::size_t iHowMantThreadEnters = 0;
	iHowMantThreadEnters++;
	//liquidMessage2(messageInfo, "HowMantThreadEnters=%d", iHowMantThreadEnters);

	if ( msg & MNodeMessage::kConnectionMade ) {
		MGlobal::displayInfo("Connection made ");
	}
	else if ( msg & MNodeMessage::kConnectionBroken ) {
		MGlobal::displayInfo("Connection broken ");
	}
	else if ( msg & MNodeMessage::kAttributeEval ) {
		MGlobal::displayInfo("kAttributeEval");
	}
	else if ( msg & MNodeMessage::kAttributeSet ) {
		MGlobal::displayInfo("kAttributeSet");

		//liqRibTranslator::getInstancePtr()->IPRRenderBegin();

		//if( canExport() )
		{
			liquid::RendererMgr::getInstancePtr()->getRenderer()
				->IPR_AttributeChangedCallback(msg, plug, otherPlug, userData);
		}

		//liqRibTranslator::getInstancePtr()->IPRRenderEnd();
	}
	else {
		liquidMessage2(messageInfo, "else: msg=%0x", msg);
	}

// 	cout << plug.info();
// 	if ( msg & MNodeMessage::kOtherPlugSet ) {
// 		if ( msg & MNodeMessage::kIncomingDirection ) {
// 				cout << "  <--  " << otherPlug.info();
// 		} else {
// 				cout << "  -->  " << otherPlug.info();
// 		}
// 	}
// 	cout << endl;
	--iHowMantThreadEnters;

}

void liquidIPR_NodeDirtyCallback( MObject& node,void *userData )
{
	MFnDependencyNode nodeFn(node);
	MGlobal::displayInfo( "liquidIPR_NodeDirtyCallback(" + nodeFn.name()+", userData)");

	//liquid::RendererMgr::getInstancePtr()->getRenderer()
	//	->IPR_NodeDirtyCallback(node, userData);
}

void liquidIPR_NodeDirtyPlugCallback( MObject& node,MPlug& plug,void* userData )
{
	MFnDependencyNode nodeFn(node);
	MGlobal::displayInfo( "liquidIPR_NodeDirtyPlugCallback( " + nodeFn.name()+","+plug.name()+", userData)");


	liqRibTranslator::getInstancePtr()->IPRRenderBegin();

	liquid::RendererMgr::getInstancePtr()->getRenderer()
		->IPR_NodeDirtyPlugCallback(node, plug, userData);

	liqRibTranslator::getInstancePtr()->IPRRenderEnd();
}
//////////////////////////////////////////////////////////////////////////
//
// Command class implementation
//
//////////////////////////////////////////////////////////////////////////
static int isRunningIPR = 0;// 1 - is running, 0 - is not running.
liqIPRNodeMessage::liqIPRNodeMessage()
{
}
liqIPRNodeMessage::~liqIPRNodeMessage()
{

}

void* liqIPRNodeMessage::creator()
{
	return new liqIPRNodeMessage();
}

MStatus liqIPRNodeMessage::doIt( const MArgList& args)
//
// Takes the  nodes that are on the active selection list and adds an
// attriubte changed callback to each one.
//
{	
	MStatus 		stat;

	for( unsigned i( 0 ); i < args.length(); i++ ) 
	{
		MString arg = args.asString( i, &stat );
		IfMErrorWarn(stat);

		if( (arg == kRegisterFlag) || (arg == kRegisterFlagLong) ){
			isRunningIPR = 1;
			liqRibTranslator::getInstancePtr()->IPRRenderBegin();
			IfMErrorWarn(registerCallback());
			//liqRibTranslator::getInstancePtr()->IPRDoIt();
		}
		else if( (arg == kUnregisterFlag) || (arg == kUnregisterFlagLong) ){
			IfMErrorWarn(unregisterCallback());

			liqRibTranslator::getInstancePtr()->IPRRenderEnd();
			isRunningIPR = 0;
		}
		else if( (arg == kIsRunningIPR) || (arg == kIsRunningIPRLong) ){
			setResult(isRunningIPR);
		}
		else{
			liquidMessage2(messageError,"Parameter [%s] is undefined in liqIPRNodeMessage.", arg.asChar());
			return MS::kUnknownParameter;
		}
	}

	return MS::kSuccess;
}
//
MStatus liqIPRNodeMessage::registerCallback()
{
	//MGlobal::displayInfo( "liqIPRNodeMessage::registerCallback()");

	MStatus 		stat;
	std::vector<MString> updateObjectName;
    MCallbackId     id;
    
	gatherUpdateObjects(updateObjectName);

    for ( unsigned int i=0; i<updateObjectName.size(); i++ )
    {
		MSelectionList selection;
		IfMErrorWarn(MGlobal::getSelectionListByName(updateObjectName[i], selection));

		if( selection.length() == 0 )//not found
			continue;

		MObject obj;
		IfMErrorWarn(selection.getDependNode( 0, obj ));

		MGlobal::displayInfo(MString("add callback for node: ") + updateObjectName[i]);

        //AttributeChangedCallback
	    id = MNodeMessage::addAttributeChangedCallback( obj, liquidIPR_AttributeChangedCallback, NULL,  &stat);
		IfMErrorWarn(stat);
	    if ( stat ) {
		    callbackIds.append( id );
    	} else {
	    	cout << "MNodeMessage.addAttributeChangedCallback("<<updateObjectName[i]<<") failed\n";
    	}

		//AttributeAddedOrRemovedCallback

		//NodeDirtyCallback
// 		id = MNodeMessage::addNodeDirtyCallback( node, liquidIPR_NodeDirtyCallback, NULL,  &stat);
// 		IfMErrorWarn(stat);
// 		if ( stat ) {
// 			callbackIds.append( id );
// 		} else {
// 			cout << "MNodeMessage.addNodeDirtyCallback failed\n";
// 		}

		//addNodeDirtyPlugCallback
// 		id = MNodeMessage::addNodeDirtyPlugCallback( node, liquidIPR_NodeDirtyPlugCallback, NULL,  &stat);
// 		IfMErrorWarn(stat);		
// 		if ( stat ) {
// 			callbackIds.append( id );
// 		} else {
// 			cout << "MNodeMessage.addNodeDirtyPlugCallback failed\n";
// 		}

		//addNameChangedCallback

		//addNodeAboutToDeleteCallback
		//addNodePreRemovalCallback
		//addNodeDestroyedCallback
		//addKeyableChangeOverride
		//
    }
    
	return MS::kSuccess;
}

MStatus liqIPRNodeMessage::unregisterCallback()
{
	//MGlobal::displayInfo( "liqIPRNodeMessage::unregisterCallback()");
// 	// Remove all callbacks
	//
	for (unsigned int i=0; i<callbackIds.length(); i++ ) 
	{
		if ( MS::kSuccess != MMessage::removeCallback( (MCallbackId)(liqIPRNodeMessage::callbackIds[i]) ) )
		{
			cout << "MMessage::removeCallback("<<i <<") failed\n";
		}
	}
	return MS::kSuccess;
}
void liqIPRNodeMessage::gatherUpdateObjects(std::vector<MString>& objects)
{
	MStatus status;

	//get selection list
	MSelectionList 	selection;
	IfMErrorWarn(MGlobal::getActiveSelectionList( selection ));

	for ( unsigned int i=0; i<selection.length(); i++ )
	{
		MObject node;
		IfMErrorWarn(selection.getDependNode( i, node ));

		MFnDependencyNode nodeFn(node, &status);
		IfMErrorWarn(status);

		const MString nodeName(nodeFn.name());
		objects.push_back(nodeName);//record current node

		if( isShaderNode(nodeName) )
		{
			onShaderNode(nodeName, objects);
		}
		else{
			onOtherNode(nodeName, objects);
		}




	}
}
//
bool liqIPRNodeMessage::isShaderNode(const MString &node)const 
{
	MString cmd;

	MString nodetype;
	cmd = "nodeType \""+node+"\"";
	IfMErrorWarn(MGlobal::executeCommand( cmd, nodetype));

	if( liquidmaya::ShaderMgr::getSingletonPtr()->hasShaderType(nodetype.asChar()) )
	{
		return true;
	}else{
		return false;
	}
}
//
void liqIPRNodeMessage::onShaderNode(const MString &shadernode, std::vector<MString> &updateObjectName)
{
	MString cmd;

	//1. on shader node itself
	addUpdateObject(updateObjectName, shadernode);//record source shader

	//2. on upstream shaders
	//get the upstream shaders of the shader node
	MStringArray upstreamShaders;
	cmd = "hyperShade -listUpstreamNodes \""+shadernode+"\"";
	IfMErrorMsgWarn(MGlobal::executeCommand( cmd, upstreamShaders), cmd);
	//record upstream shaders of the shader node
	for(std::size_t i=0; i<upstreamShaders.length(); ++i)
	{
		addUpdateObject(updateObjectName, upstreamShaders[i]);
	}
}
//
void liqIPRNodeMessage::onOtherNode(const MString &node, std::vector<MString> &updateObjectName)
{
	MStringArray descendents;
	MString cmd("listRelatives -allDescendents "+node);
	MGlobal::executeCommand(cmd, descendents);

	for(int i=0; i<descendents.length(); ++i)
	{
		addUpdateObject(updateObjectName, descendents[i]);//record descendents[i]
		
		MDagPath dagPath;
		getDagPathByName(dagPath, descendents[i].asChar());

		if( dagPath.node().hasFn(MFn::kTransform) )
		{
			onOtherNode(descendents[i], updateObjectName);//visit descendents[i]
		}
		else if( dagPath.node().hasFn(MFn::kMesh) )
		{
			std::vector<std::string> shaderPlugs;
			liquid::RendererMgr::getInstancePtr()->
				getRenderer()->getValidShaderPlugsInShadingGroup(shaderPlugs);


			IfMErrorWarn(dagPath.extendToShape());//extend to shape

			std::vector<std::string> shadingGroups;
			getShadingGroups(dagPath.fullPathName(), shadingGroups);
			for(std::size_t j=0; j<shadingGroups.size(); ++j)//for each shading group
			{
				MString shadingGroup(shadingGroups[j].c_str());

				for(std::size_t k=0; k<shaderPlugs.size(); ++k)//for each shader plug
				{
					MString shaderPlug(shaderPlugs[k].c_str());

					int isShaderPlugExist;
					cmd = "attributeQuery -node \""+shadingGroup+"\" -ex \""+shaderPlug+"\"";
					IfMErrorMsgWarn(MGlobal::executeCommand( cmd, isShaderPlugExist), cmd);
					if( isShaderPlugExist )
					{
						//get the source shade node of $shadingGroup.$shaderPlug
						MStringArray shaders;
						cmd = "listConnections -s true -d false -plugs false (\""+shadingGroup+"\" + \"."+shaderPlug+"\")";
						IfMErrorMsgWarn(MGlobal::executeCommand( cmd, shaders), cmd);
						
						if( shaders.length() > 0 )//has source shader node
						{
							onShaderNode(shaders[0], updateObjectName);
						}//if( shaders.length() > 0 )//has source shader node

					}//if( isShaderPlugExist )
				}//for each shader plug


			}//for each shading group
		}//kMesh
	}//for(int i=0; i<descendents.length(); ++i)

}
//
void liqIPRNodeMessage::addUpdateObject(std::vector<MString> &updateObjectName, const MString& objectname)
{
	updateObjectName.push_back(objectname);
}
