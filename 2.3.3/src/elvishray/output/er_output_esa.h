#include <liqConfig.h>
#ifdef _USE_ELVISHRAY_

#ifndef _ER_OUTPUT_ESA_H_
#define _ER_OUTPUT_ESA_H_
#include "../../common/prerequest_std.h"
#include "er_output_base.h"

namespace elvishray
{	
	class OutputESA : public OutputBase
	{
	public:
		OutputESA(const std::string &filefullpath);
		~OutputESA();

	protected:
		OutputESA();
		OutputESA(const OutputESA &o);
		OutputESA& operator=(const OutputESA &o);

	protected:
		std::ofstream m_outputfile;

		//----------------------------------------------------
		// ER API interfaces
		//----------------------------------------------------
	public:
		virtual void ei_context();
		virtual void ei_end_context();



	};
}

#endif//_ER_OUTPUT_ESA_H_
#endif//_USE_ELVISHRAY_