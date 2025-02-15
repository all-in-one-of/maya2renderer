#ifndef _RendererMgr_H_
#define _RendererMgr_H_

#include <string>
#include <map>
#include "rendererinterface.h"
#include "liqAbstractFactory.h"
#include "./shadergraph/shadermgr.h"

namespace liquid
{
class ShadingNetworkUserBase;

class LIQUID_EXPORT RendererMgr
{
public:
	static RendererMgr* getInstancePtr();
	virtual ~RendererMgr();

	bool setFactory(const std::string& renderername);
	void unsetFactory();
	AbstractFactory* getFactory(const std::string& renderername);//this function will be removed in the future.
	//AbstractFactory* getCurrentFactory(){ assert(m_factory); return m_factory; }

	static void registFactory(const std::string& renderername, AbstractFactory* factory);
	static void unregistFactory(const std::string& renderername);

	RendererInterface* getRenderer(){ assert(m_renderer); return m_renderer; }
	
	void test();
	void prologue();
	void epilogue();

	void install();
	void uninstall();

	void iprBegin();
	void iprEnd();

	//shading network stuff
	bool beforeExport_user();
	void export_user();
	void afterExport_user();

protected:
	RendererMgr();
	RendererMgr(const RendererMgr&);
	RendererMgr& operator=(const RendererMgr&);

	static RendererMgr *m_instance;
	RendererInterface *m_renderer;
	//std::map<std::string, RendererInterface*> m_registeredRenderers;

	AbstractFactory* m_factory;
	static std::map<std::string, AbstractFactory*> m_factories;

	RenderNodeVisitorInterface* m_rendernode_visitor;
	liquidmaya::ShaderMgr* m_shadermgr;
	ShadingNetworkUserBase* m_shadingnetwork_user_Ref;
};

}//namespace liquid
#endif//_RendererMgr_H_