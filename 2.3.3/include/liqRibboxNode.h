
#ifndef liqRibboxNode_H
#define liqRibboxNode_H

#include <common/prerequest_maya.h>

class liqNodeSwatch;

class liqRibboxNode : public MPxNode
{
	public:
                      liqRibboxNode();
    virtual          ~liqRibboxNode();

    //virtual MStatus   compute( const MPlug&, MDataBlock& );
    virtual void      postConstructor();

    static  void *    creator();
    static  MStatus   initialize();

    //  Id tag for use with binary file format
	static  MTypeId   id;
	static MString getTypeName(){ return "liquidRibBox"; }
    liqNodeSwatch*    renderSwatch;

	private:

    // Input attributes
    static MObject aRmanShader;
    static MObject aRmanShaderLong;
    static MObject aRibbox;

    // Output attributes
    static MObject aOutColor;

	bool    swatchInit;

private:
	liqRibboxNode(const liqRibboxNode&);
	liqRibboxNode& operator=(const liqRibboxNode&);
};




#endif
