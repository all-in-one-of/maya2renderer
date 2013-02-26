//-

#include "liqIPRNodeMessageCmd.h"
#include "common/mayacheck.h"
#include "renderermgr.h"
#include "liqRibTranslator.h"

#define kRegisterFlagLong		"-register"
#define kRegisterFlag			"-rgt"
#define kUnregisterFlagLong		"-unregister"
#define kUnregisterFlag			"-urg"

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

		liqRibTranslator::getInstancePtr()->IPRRenderBegin();

		//if( canExport() )
		{
			liquid::RendererMgr::getInstancePtr()->getRenderer()
				->IPR_AttributeChangedCallback(msg, plug, otherPlug, userData);
		}

		liqRibTranslator::getInstancePtr()->IPRRenderEnd();
	}
	else {
		return;
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

MSyntax	liqIPRNodeMessage::newSyntax()
{
	MSyntax syntax;
	IfMErrorWarn(syntax.addFlag( kRegisterFlag, kRegisterFlagLong ));
	IfMErrorWarn(syntax.addFlag( kUnregisterFlag, kUnregisterFlagLong ));
	return syntax;
}

MStatus liqIPRNodeMessage::doIt( const MArgList& args)
//
// Takes the  nodes that are on the active selection list and adds an
// attriubte changed callback to each one.
//
{	
	MStatus 		stat;

	MArgDatabase argData( newSyntax(), args , &stat);
	IfMErrorWarn(stat);

	// Parse command flags.
	//
// 	if ( argData.isFlagSet( kRegisterFlag, &stat ) ) 
// 	{	
// 		IfMErrorWarn(stat);
// 		IfMErrorWarn(registerCallback());
// 	} 
// 	else if ( argData.isFlagSet( kUnregisterFlag, &stat ) ) {
// 		IfMErrorWarn(stat);
// 		IfMErrorWarn(unregisterCallback());
// 	}
	//DEBUG:
	IfMErrorWarn(unregisterCallback());
	IfMErrorWarn(registerCallback());

	return MS::kSuccess;
}

MStatus liqIPRNodeMessage::registerCallback()
{
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
// 	    id = MNodeMessage::addAttributeChangedCallback( node, liquidIPR_AttributeChangedCallback, NULL,  &stat);
// 		IfMErrorWarn(stat);
// 	    if ( stat ) {
// 		    callbackIds.append( id );
//     	} else {
// 	    	cout << "MNodeMessage.addAttributeChangedCallback failed\n";
//     	}

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
		id = MNodeMessage::addNodeDirtyPlugCallback( node, liquidIPR_NodeDirtyPlugCallback, NULL,  &stat);
		IfMErrorWarn(stat);		
		if ( stat ) {
			callbackIds.append( id );
		} else {
			cout << "MNodeMessage.addNodeDirtyPlugCallback failed\n";
		}

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
// 	// Remove all callbacks
// 	//
// 	for (unsigned int i=0; i<callbackIds.length(); i++ ) 
// 	{
// 		if ( MS::kSuccess != MMessage::removeCallback( (MCallbackId)(liqIPRNodeMessage::callbackIds[i]) ) )
// 		{
// 			cout << "MMessage::removeCallback("<<i <<") failed\n";
// 		}
// 	}
	return MS::kSuccess;
}