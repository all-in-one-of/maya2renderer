#include <liqConfig.h>
#ifdef _USE_ELVISHRAY_

#ifndef _ER_OUTPUT_ESS_H_
#define _ER_OUTPUT_ESS_H_
#include "../../common/prerequest_std.h"
#include "er_output_base.h"

namespace elvishray
{	
	class OutputESS : public OutputBase
	{
	public:
		OutputESS(const std::string &filefullpath);
		~OutputESS();

	protected:
		OutputESS();
		OutputESS(const OutputESS &o);
		OutputESS& operator=(const OutputESS &o);

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

#endif//_ER_OUTPUT_ESS_H_
#endif//_USE_ELVISHRAY_