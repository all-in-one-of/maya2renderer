#include <liqConfig.h>
#ifdef _USE_ELVISHRAY_
#include "er_output_call.h"
#include <eiAPI/ei.h>
#include <trace/trace.hpp>

namespace elvishray
{
	OutputCall::OutputCall()
		:OutputBase()
	{
		CM_TRACE_FUNC("OutputCall::OutputCall()");
	}
	OutputCall::~OutputCall()
	{
		CM_TRACE_FUNC("OutputCall::~OutputCall()");
	}
	//----------------------------------------------------
	// ER API interfaces
	//----------------------------------------------------
	void OutputCall::ei_context()
	{
		//CM_TRACE_FUNC("OutputCall::ei_context()");
		//ei_context();
	}
	void OutputCall::ei_end_context()
	{
		//CM_TRACE_FUNC("OutputCall::ei_end_context()");
		//ei_end_context();
	}
}//namespace elvishray
#endif//_USE_ELVISHRAY_