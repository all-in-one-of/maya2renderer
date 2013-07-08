#include "er_factory.h"
#include "shaderOutputER.h"
#include "er_renderer.h"
#include "../shadergraph/shaderOutputMgr.h"
#include "er_rnode_visitor.h"
#include "er_shadingnetwork.h"

namespace elvishray
{
	Factory::Factory()
	{
		m_renderer = 0;
		m_rendernode_visitor = 0;
		m_shadingnetwork_user = 0;
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
		liquidmaya::ShaderOutputMgr::getSingletonPtr()->addReceiver(new ER::Visitor());
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
	//
	liquid::ShadingNetworkUserBase* Factory::createShadingNetworkUser()
	{
		m_shadingnetwork_user = new ShadingNetworkUser();
		return m_shadingnetwork_user;
	}
	liquid::ShadingNetworkUserBase* Factory::deleteShadingNetworkUser()
	{
		delete m_shadingnetwork_user;
		m_shadingnetwork_user = NULL;
		return m_shadingnetwork_user;
	}

}//namespace elvishray