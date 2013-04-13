#include <liqConfig.h>
#ifdef _USE_ELVISHRAY_
#include "er_output_call.h"
#include <eiAPI/ei.h>

namespace elvishray
{
	OutputCall::OutputCall()
		:OutputBase()
	{
	}
	OutputCall::~OutputCall()
	{
	}
	//----------------------------------------------------
	// ER API interfaces
	//----------------------------------------------------
	void OutputCall::ei_context()
	{
		//ei_context();
	}
	void OutputCall::ei_end_context()
	{
		//ei_end_context();
	}
}//namespace elvishray
#endif//_USE_ELVISHRAY_