#ifndef _RM_GlobalsNode_H_
#define _RM_GlobalsNode_H_

#include <common/prerequest_maya.h>

namespace renderman
{
	class GlobalNode : public MPxNode
	{
	  public:
		GlobalNode();
		virtual ~GlobalNode();

		static  void *    creator();
		static  MStatus   initialize();

		static  const MTypeId   typeId;
		static  const MString   typeName;
	private:
		//
		static MObject aoutput_call;

	private:
		GlobalNode(const GlobalNode&);
		GlobalNode& operator=(const GlobalNode&);
	};

}//namespace renderman



#endif//_RM_GlobalsNode_H_
