#include <liqConfig.h>
#ifdef _USE_ELVISHRAY_

#ifndef _ER_OUTPUT_MGR_H_
#define _ER_OUTPUT_MGR_H_
#include "output/er_output_base.h"

namespace elvishray
{	
	
	//
	class OutputMgr
	{
	public:
		enum OutputType
		{
			OT_CALL = 0,
			OT_ESS  = 1,
			OT_ESA  = 2,
			OT_COUNT
		};

	public:
		OutputMgr();
		~OutputMgr();
		
		void setOutputImagePath(const std::string &output_image_path);
		int add(OutputType outputtype);

		int init();
		int uninit();

	protected:
		OutputMgr(const OutputMgr& o);
		OutputMgr& operator=(const OutputMgr& o);

		OutputBase* createOutput(OutputType outputtype);
	protected:
		std::vector<OutputType> m_receiver_types;
		std::vector<elvishray::OutputBase*> m_receivers;
		std::string m_output_image_path;


		//----------------------------------------------------
		// ER API interfaces
		//----------------------------------------------------
	public:
		void ei_context();
		void ei_end_context();


	};
	//
}

#endif//_ER_OUTPUT_MGR_H_
#endif//_USE_ELVISHRAY_