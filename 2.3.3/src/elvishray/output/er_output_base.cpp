#include <liqConfig.h>
#ifdef _USE_ELVISHRAY_
#include "er_output_base.h"
#include "../er_log.h"

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