#ifndef _RendererMgr_H_
#define _RendererMgr_H_

#include <string>
#include <map>
#include "rendererinterface.h"
#include "liqAbstractFactory.h"

namespace liquid
{


class LIQUID_EXPORT RendererMgr
{
public:
	static RendererMgr* getInstancePtr();
	virtual ~RendererMgr();

	bool setFactory(const std::string& renderername);
	void unsetFactory();

	static void registFactory(const std::string& renderername, AbstractFactory* factory);
	static void unregistFactory(const std::string& renderername);

	RendererInterface* getRenderer(){ return m_renderer; }
	
	void test();
	void prologue();
	void epilogue();

	void install();
	void uninstall();

protected:
	RendererMgr();
	RendererMgr(const RendererMgr&);
	RendererMgr& operator=(const RendererMgr&);

	static RendererMgr *m_instance;
	RendererInterface *m_renderer;
	//std::map<std::string, RendererInterface*> m_registeredRenderers;

	AbstractFactory* m_factory;
	static std::map<std::string, AbstractFactory*> m_factories;

};

}
#endif//_RendererMgr_H_