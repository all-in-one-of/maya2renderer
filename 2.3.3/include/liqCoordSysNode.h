
#ifndef liqCoordSysNode_H
#define liqCoordSysNode_H

#include <common/prerequest_maya.h>

class liqCoordSysNode : public MPxLocatorNode
{
  public:
	  liqCoordSysNode(){}
	  virtual ~liqCoordSysNode(){}

    virtual MStatus         compute( const MPlug& plug, MDataBlock& data );

    virtual void            draw( M3dView & view, const MDagPath & path,
                                  M3dView::DisplayStyle displaystyle,
                                  M3dView::DisplayStatus displaystatus );

    virtual bool            isBounded() const;
    virtual MBoundingBox    boundingBox() const;

    static  void *          creator();
    static  MStatus         initialize();

    static  MObject         aDummy;
    static  MObject         aCoordType;
    static  MObject         aCoordColor;
    static  MObject         aCoordOpacity;

  public:
    static	MTypeId		      id;

  private:
    int     m_coordType;
    MColor  m_coordColor;

private:
	liqCoordSysNode(const liqCoordSysNode&);
	liqCoordSysNode& operator=(const liqCoordSysNode&);

};

#endif//liqCoordSysNode_H
