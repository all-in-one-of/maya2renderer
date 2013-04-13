#include <liqConfig.h>
#ifdef _USE_ELVISHRAY_

#include "er_output_mgr.h"
#include <liqlog.h>
#include "output/er_output_call.h"
#include "output/er_output_ess.h"
#include "output/er_output_esa.h"

namespace elvishray
{
	OutputMgr::OutputMgr()
	{
		CM_TRACE_FUNC("OutputMgr::OutputMgr()");
	}
	OutputMgr::~OutputMgr()
	{
		CM_TRACE_FUNC("OutputMgr::~OutputMgr()");
	}
	void OutputMgr::setOutputImagePath(const std::string &output_image_path)
	{
		CM_TRACE_FUNC("OutputMgr::setOutputImagePath("<<output_image_path<<")");
		m_output_image_path = output_image_path;
	}
	OutputBase* OutputMgr::createOutput(OutputType outputtype)
	{
		CM_TRACE_FUNC("OutputMgr::createOutput("<<outputtype<<")");
		OutputBase* ret = NULL;

		switch(outputtype)
		{
		case OutputMgr::OT_CALL:
			ret = new OutputCall();
			break;
		case OutputMgr::OT_ESS:
			ret = new OutputESS(m_output_image_path);
			break;
		case OutputMgr::OT_ESA:
			ret = new OutputESA(m_output_image_path);
			break;
		default:
			liquidMessage2(messageError, "OutputMgr::createOutput()> unknown output type \"%d\"", outputtype);
			ret = NULL;
			break;	
		}
		return ret;
	}
	int OutputMgr::add(OutputType outputtype)
	{
		CM_TRACE_FUNC("OutputMgr::add("<<outputtype<<")");
		if(m_receiver_types.end() == std::find(m_receiver_types.begin(), m_receiver_types.end(), outputtype) )
		{
			m_receiver_types.push_back(outputtype);
			return 0;
		}

		liquidMessage2(messageError, "OutputMgr::add()> output type \"%d\" already exists.", outputtype);
		return -1;//already exist.
	}
	int OutputMgr::init()
	{
		CM_TRACE_FUNC("OutputMgr::init()");
		for(std::size_t i=0; i<m_receiver_types.size(); ++i)
		{
			OutputBase* newreceiver = createOutput( m_receiver_types[i] );
			m_receivers.push_back(newreceiver);
		}
		return 0;
	}
	int OutputMgr::uninit()
	{
		CM_TRACE_FUNC("OutputMgr::uninit()");
		//destruct m_receivers
		std::vector<elvishray::OutputBase*>::iterator i= m_receivers.begin();
		std::vector<elvishray::OutputBase*>::iterator e= m_receivers.end();
		for(; i!=e; ++i)
		{
			delete (*i);
		}
		m_receivers.clear();

		//destruct m_receiver_types
		m_receiver_types.clear();

		return 0;
	}
	//----------------------------------------------------
	// ER API interfaces
	//----------------------------------------------------
	void OutputMgr::ei_context()
	{
		//CM_TRACE_FUNC("OutputMgr::ei_context()");
		std::vector<elvishray::OutputBase*>::iterator i= m_receivers.begin();
		std::vector<elvishray::OutputBase*>::iterator e= m_receivers.end();
		for(; i!=e; ++i)
		{
			(*i)->ei_context();
		}
	}
	void OutputMgr::ei_end_context()
	{
		//CM_TRACE_FUNC("OutputMgr::ei_end_context()");
		std::vector<elvishray::OutputBase*>::iterator i= m_receivers.begin();
		std::vector<elvishray::OutputBase*>::iterator e= m_receivers.end();
		for(; i!=e; ++i)
		{
			(*i)->ei_end_context();
		}
	}


}//namespace elvishray
#endif//_USE_ELVISHRAY_