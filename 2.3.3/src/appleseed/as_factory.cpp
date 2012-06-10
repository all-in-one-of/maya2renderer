#include "as_factory.h"
#include "as_shaderOutput.h"
#include "as_shaderOutputCall.h"
#include "as_renderer.h"
#include "../shadergraph/shaderOutputMgr.h"

namespace appleseed
{
	Factory::Factory()
	{
		m_renderer = 0;
	}
	//
	Factory::~Factory()
	{

	}
	//
	liquid::RendererInterface* Factory::createRenderer()
	{
		if(m_renderer==0){
			m_renderer = new Renderer();
		}
		return m_renderer;
	}
	void Factory::deleteRenderer()
	{
		if(m_renderer)
		{
			delete m_renderer;
			m_renderer = 0;
		}
	}

	//
	void Factory::createOutputReceiver()
	{
		liquidmaya::ShaderOutputMgr::getSingletonPtr()->addReceiver(new appleseed::Visitor());
		liquidmaya::ShaderOutputMgr::getSingletonPtr()->addReceiver(new appleseed::call::Visitor());
	}
	void Factory::deleteOutputReceiver()
	{
		liquidmaya::ShaderOutputMgr::getSingletonPtr()->deleteReceivers();
	}


}//namespace appleseed