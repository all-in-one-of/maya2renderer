#include <liqConfig.h>
#ifdef _USE_ELVISHRAY_
#include "er_output_base.h"
#include <trace/trace.hpp>

namespace elvishray
{
	OutputBase::OutputBase()
	{
		CM_TRACE_FUNC("OutputBase::OutputBase()");
	}
	OutputBase::~OutputBase()
	{
		CM_TRACE_FUNC("OutputBase::~OutputBase()");
	}
}//namespace elvishray
#endif//_USE_ELVISHRAY_