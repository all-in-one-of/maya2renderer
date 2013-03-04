//-

#include "liqIPRNodeMessageCmd.h"
#include "common/mayacheck.h"
#include "renderermgr.h"
#include "liqRibTranslator.h"

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
	liquidMessage2(messageInfo, "msg=%0x", msg);

	static std::size_t iHowMantThreadEnters = 0;
	iHowMantThreadEnters++;
	liquidMessage2(messageInfo, "HowMantThreadEnters=%d", iHowMantThreadEnters);

	if ( msg & MNodeMessage::kConnectionMade ) {
		cout << "Connection made ";
	}
	else if ( msg & MNodeMessage::kConnectionBroken ) {
		cout << "Connection broken ";
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
			IfMErrorWarn(registerCallback());
			isRunningIPR = 1;
			liqRibTranslator::getInstancePtr()->IPRRenderBegin();
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
			liquidMessage2(messageError,"Parameter '%s' is undefined in liqIPRNodeMessage.", arg.asChar());
			return MS::kUnknownParameter;
		}
	}

	return MS::kSuccess;
}

MStatus liqIPRNodeMessage::registerCallback()
{
	//MGlobal::displayInfo( "liqIPRNodeMessage::registerCallback()");

	MStatus 		stat;
	MObject 		node;
	MSelectionList 	list;
    MCallbackId     id;
    
	// Register node callbacks for all nodes on the active list.
	//
	IfMErrorWarn(MGlobal::getActiveSelectionList( list ));

    for ( unsigned int i=0; i<list.length(); i++ )
    {
        IfMErrorWarn(list.getDependNode( i, node ));
		MFnDependencyNode nodeFn(node);
		MGlobal::displayInfo(MString("add callback for node: ") + nodeFn.name());

        //AttributeChangedCallback
	    id = MNodeMessage::addAttributeChangedCallback( node, liquidIPR_AttributeChangedCallback, NULL,  &stat);
		IfMErrorWarn(stat);
	    if ( stat ) {
		    callbackIds.append( id );
    	} else {
	    	cout << "MNodeMessage.addAttributeChangedCallback failed\n";
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