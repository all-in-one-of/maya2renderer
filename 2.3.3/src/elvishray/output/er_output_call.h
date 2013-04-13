#include <liqConfig.h>
#ifdef _USE_ELVISHRAY_

#ifndef _ER_OUTPUT_CALL_H_
#define _ER_OUTPUT_CALL_H_
#include "../../common/prerequest_std.h"
#include "er_output_base.h"

namespace elvishray
{	
	class OutputCall : public OutputBase
	{
	public:
		OutputCall();
		~OutputCall();

	protected:
		OutputCall(const OutputCall &o);
		OutputCall& operator=(const OutputCall &o);


		//----------------------------------------------------
		// ER API interfaces
		//----------------------------------------------------
	public:
		virtual void ei_context();
		virtual void ei_end_context();


	};
}

#endif//_ER_OUTPUT_CALL_H_
#endif//_USE_ELVISHRAY_