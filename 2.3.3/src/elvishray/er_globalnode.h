#ifndef _ER_GlobalsNode_H_
#define _ER_GlobalsNode_H_

#include "../common/prerequest_maya.h"

namespace elvishray
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
		static MObject aoutput_call;
		static MObject aoutput_esa;
		static MObject aoutput_ess;

	private:
		GlobalNode(const GlobalNode&);
		GlobalNode& operator=(const GlobalNode&);
	};

}//namespace elvishray



#endif//_ER_GlobalsNode_H_
