#ifndef liqIprNodeMessageCmd_H
#define liqIprNodeMessageCmd_H

#include "common/prerequest_maya.h"


//////////////////////////////////////////////////////////////////////////
//
// Command class declaration
//
//////////////////////////////////////////////////////////////////////////

class liqIPRNodeMessage : public MPxCommand
{
public:
	liqIPRNodeMessage();
	virtual	~liqIPRNodeMessage(); 
	virtual MStatus	doIt( const MArgList& args );
	static void*	creator();
	static MSyntax	newSyntax();
	virtual bool	isUndoable() const{ return false; }
	virtual bool	hasSyntax() const{ return true; }

	MStatus liqIPRNodeMessage::registerCallback();
	MStatus liqIPRNodeMessage::unregisterCallback();


	static MCallbackIdArray callbackIds;
};

#endif
