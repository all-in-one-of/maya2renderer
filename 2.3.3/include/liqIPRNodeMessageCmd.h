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
	static MString getName(){ return "liqIPRNodeMessage";}
	virtual bool	isUndoable() const{ return false; }
	virtual bool	hasSyntax() const{ return true; }

	MStatus registerCallback();
	MStatus unregisterCallback();

protected:
	void gatherUpdateObjects(std::vector<MString>& objects);
	bool isShaderNode(const MString &node)const;
	void onShaderNode(const MString &shadernode, std::vector<MString> &updateObjectName);
	void onOtherNode(const MString &shadernode, std::vector<MString> &updateObjectName);
	void addUpdateObject(std::vector<MString> &updateObjectName, const MString& objectname);

	static MCallbackIdArray callbackIds;
};

#endif
