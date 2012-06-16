#include "renderermgr.h"
#include "liqlog.h"
#include "./renderman/rm_factory.h"
#include "./elvishray/er_factory.h"
#include "./appleseed/as_factory.h"

namespace liquid
{

	RendererMgr* RendererMgr::m_instance = 0;
	//
	RendererMgr::RendererMgr()
	:m_renderer(0),
	m_factory(0)
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
		getRenderer()->openLog();
	}
	void RendererMgr::epilogue()
	{
		CM_TRACE_FUNC("RendererMgr::epilogue()");
		assert(m_renderer);
		getRenderer()->closeLog();
	}
	//
	void RendererMgr::createFactory(const std::string& renderername)
	{
		CM_TRACE_FUNC("RendererMgr::createFactory("<<renderername<<")");
		
		if(false){}
#ifdef _USE_RENDERMAN_
		else if(renderername=="renderman"){
			m_factory = new renderman::Factory();
		}
#endif
#ifdef _USE_ELVISHRAY_
		else if(renderername=="elvishray"){
			m_factory = new elvishray::Factory();
		}
#endif 
#ifdef _USE_APPLESEED_
		else if(renderername=="appleseed"){
			m_factory = new appleseed::Factory();
		}
#endif 
		else {
			liquidMessage2(messageError, "Unkown renderer:%s.",renderername.c_str() );
		}
	}
	void RendererMgr::deleteFactory()
	{
		CM_TRACE_FUNC("RendererMgr::deleteFactory()");

		delete m_factory;
		m_factory = 0;
	}
	//
	void RendererMgr::install()
	{
		CM_TRACE_FUNC("RendererMgr::install()");
		assert(m_factory);

		m_renderer = m_factory->createRenderer();
		m_factory->createOutputReceiver();

	}
	void RendererMgr::uninstall()
	{
		CM_TRACE_FUNC("RendererMgr::uninstall()");
		assert(m_factory);
		m_factory->deleteOutputReceiver();
		m_factory->deleteRenderer();
	}
}
