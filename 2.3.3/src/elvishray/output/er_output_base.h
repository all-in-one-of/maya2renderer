#include <liqConfig.h>
#ifdef _USE_ELVISHRAY_

#ifndef _ER_OUTPUT_BASE_H_
#define _ER_OUTPUT_BASE_H_
#include "../../common/prerequest_std.h"

namespace elvishray
{	
	class OutputBase
	{
	public:
		OutputBase();
		virtual ~OutputBase();

	protected:
		OutputBase(const OutputBase &o);
		OutputBase& operator=(const OutputBase &o);


		//----------------------------------------------------
		// ER API interfaces
		//----------------------------------------------------
	public:
		virtual void ei_context() = 0;
		virtual void ei_end_context() = 0;


	};
}

#endif//_ER_OUTPUT_BASE_H_
#endif//_USE_ELVISHRAY_