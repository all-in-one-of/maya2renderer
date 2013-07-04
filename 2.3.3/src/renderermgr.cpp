#include "renderermgr.h"
#include "liqlog.h"

#include "./common/prerequest_maya.h"
#include "./common/mayacheck.h"

#include "./renderman/rm_factory.h"
#include "./elvishray/er_factory.h"
#include "./appleseed/as_factory.h"
#include "./shadergraph/shadermgr.h"

namespace liquid
{
	RendererMgr* RendererMgr::m_instance = 0;
	std::map<std::string, AbstractFactory*> RendererMgr::m_factories;
	//
	RendererMgr::RendererMgr()
	:m_renderer(0),
	m_factory(0),
	m_rendernode_visitor(0),
	m_shadermgr(0)
	{

	}
	//
	RendererMgr::~RendererMgr()
	{

	}
	//
	RendererMgr* RendererMgr::getInstancePtr()
	{
		if(m_instance==0){
			m_instance = new RendererMgr();
		}
		return m_instance;
	}
	//
// 	void RendererMgr::registerRenderer(const std::string &renderername, RendererInterface* renderer)
// 	{
// 		m_registeredRenderers.insert(make_pair(renderername, renderer));
// 	}
	//
	void RendererMgr::test()
	{
		CM_TRACE_FUNC("RendererMgr::test()");

		assert(m_renderer);
		m_renderer->test();
	}
	void RendererMgr::prologue()
	{
		CM_TRACE_FUNC("RendererMgr::prologue()");
		assert(m_renderer);
	}
	void RendererMgr::epilogue()
	{
		CM_TRACE_FUNC("RendererMgr::epilogue()");
		assert(m_renderer);
	}
	//
	bool RendererMgr::setFactory(const std::string& renderername)
	{
		CM_TRACE_FUNC("RendererMgr::setFactory("<<renderername<<")");
		
		std::map<std::string, AbstractFactory*>::iterator 
			i = m_factories.find(renderername);
		if( i != m_factories.end() )
		{
			m_factory = i->second;
			return true;
		} else {
			liquidMessage2(messageError, "Unkown renderer:%s.",renderername.c_str() );
			return false;
		}
	}
	void RendererMgr::unsetFactory()
	{
		CM_TRACE_FUNC("RendererMgr::unsetFactory()");

		m_factory = 0;
	}
	AbstractFactory* RendererMgr::getFactory(const std::string& renderername)
	{
		CM_TRACE_FUNC("RendererMgr::getFactory("<<renderername<<")");
		
		std::map<std::string, AbstractFactory*>::iterator 
			i = m_factories.find(renderername);
		if( i != m_factories.end() )
		{
			return m_factory = i->second;
		} else {
			liquidMessage2(messageError, "Renderer is not found:%s.",renderername.c_str() );
			return NULL;
		}
	}
	//
	void RendererMgr::install()
	{
		CM_TRACE_FUNC("RendererMgr::install()");
		assert(m_factory);

		m_renderer = m_factory->createRenderer();
		m_factory->createOutputReceiver();
		m_rendernode_visitor = m_factory->createRenderNodeVisitor();
		liquidmaya::ShaderMgr::getSingletonPtr()->setValidConnection();
		m_rendernode_visitor->setValidConnection_SubRenderer(
			liquidmaya::ShaderMgr::getSingletonPtr()->getShaderConnectionMap()
		);
	}
	void RendererMgr::uninstall()
	{
		CM_TRACE_FUNC("RendererMgr::uninstall()");
		assert(m_factory);
		liquidmaya::ShaderMgr::getSingletonPtr()->clearValidConnection();
		m_rendernode_visitor = m_factory->deleteRenderNodeVisitor();
		if( m_rendernode_visitor != NULL ){
			liquidMessage2(messageError,"memory leak in RendererMgr::uninstall()");
		}
		m_factory->deleteOutputReceiver();
		m_factory->deleteRenderer();
		m_renderer = NULL;
	}
	void RendererMgr::registFactory(const std::string& renderername, AbstractFactory* factory)
	{
		if( m_factories.find(renderername) != m_factories.end() )
		{
			return;//already registed
		}

		m_factories.insert(std::make_pair(renderername, factory));

		IfMErrorWarn(MGlobal::executeCommand("registerLiquidSubRenderer(\""+MString(renderername.c_str())+"\")",  true));
	}
	void RendererMgr::unregistFactory(const std::string& renderername)
	{
		std::map<std::string, AbstractFactory*>::iterator i
			= m_factories.find(renderername);
		if( i == m_factories.end() )
		{
			return;//not registed
		}
		delete i->second;
		m_factories.erase(renderername);
		
		IfMErrorWarn(MGlobal::executeCommand("unregisterLiquidSubRenderer(\""+MString(renderername.c_str())+"\")",  true));
	}
	void RendererMgr::iprBegin()
	{
		assert( getRenderer() );
		getRenderer()->iprBegin();
	}
	void RendererMgr::iprEnd()
	{
		assert( getRenderer() );
		getRenderer()->iprEnd();
	}
}
