#ifndef _ER_GlobalsNode_H_
#define _ER_GlobalsNode_H_

#include <common/prerequest_maya.h>

namespace elvishray
{
	class GlobalNode : public MPxNode
	{
	  public:
		GlobalNode();
		virtual ~GlobalNode();

		static  void *    creator();
		static  MStatus   initialize();

		static  const MTypeId   getTypeId();
		static  const MString   getTypeName();

	private:
		//dev
		static MObject aoutput_call;
		static MObject aoutput_esa;
		static MObject aoutput_ess;
		//verose&link
		static MObject averbose;
		static MObject alink_eiIMG;
		static MObject alink_eiSHADER;
		static MObject alink_eiSHADER_maya;
		static MObject aotherlinks;

		//options
		static MObject acontrast;
		static MObject asamples_min;
		static MObject asamples_max;
		static MObject afilterType;
		static MObject afilterSize;
		static MObject aprogressive;
		//motion
		//trace_depth
		static MObject atrace_depth_transp;
		static MObject atrace_depth_glossy_reflect;
		static MObject atrace_depth_diffuse_reflect;
		static MObject atrace_depth_glossy_refract;
		static MObject atrace_depth_diffuse_refract;
		static MObject atrace_depth_sum;

		static MObject adisplace;
		static MObject amax_displace;
		static MObject aface;
		//approx
		static MObject aapprox_method;
		static MObject aapprox_any;
		static MObject aapprox_view_dep;
		static MObject aapprox_regular_usubdiv;
		static MObject aapprox_regular_vsubdiv;
		static MObject aapprox_length_edgelength;
		//static MObject aapprox_args0;
		//static MObject aapprox_args1;
		//static MObject aapprox_args2;
		//static MObject aapprox_args3;
		static MObject aapprox_sharp;
		static MObject aapprox_min_subdiv;
		static MObject aapprox_max_subdiv;
		static MObject aapprox_max_grid_size;
		static MObject aapprox_motion_factor;

	private:
		GlobalNode(const GlobalNode&);
		GlobalNode& operator=(const GlobalNode&);
	};

}//namespace elvishray



#endif//_ER_GlobalsNode_H_
