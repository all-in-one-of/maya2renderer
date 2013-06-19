#include "rm_factory.h"
#include "shaderOutputRSL.h"
#include "rm_renderer.h"
#include "../shadergraph/shaderOutputMgr.h"
#include "rm_rnode_visitor.h"

namespace renderman
{
	Factory::Factory()
	{
		m_renderer = 0;
		m_rendernode_visitor = 0;
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
		liquidmaya::ShaderOutputMgr::getSingletonPtr()->addReceiver(new RSL::Visitor());
	}
	void Factory::deleteOutputReceiver()
	{
		liquidmaya::ShaderOutputMgr::getSingletonPtr()->deleteReceivers();
	}
	//
	liquid::RenderNodeVisitorInterface* Factory::createRenderNodeVisitor()
	{
		m_rendernode_visitor = new RenderNodeVisitor();
		return m_rendernode_visitor;
	}
	liquid::RenderNodeVisitorInterface* Factory::deleteRenderNodeVisitor()
	{
		delete m_rendernode_visitor;
		m_rendernode_visitor = NULL;
		return m_rendernode_visitor;
	}

}
