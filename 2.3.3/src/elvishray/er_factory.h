#ifndef _ER_FACTORY_H_
#define _ER_FACTORY_H_

#include "../liqAbstractFactory.h"

namespace elvishray
{	
	class Renderer;
	class RenderNodeVisitor;
	class ShadingNetworkUser;
	//Create Factory
	class Factory: public liquid::AbstractFactory
	{
	public:
		Factory();
		virtual ~Factory();
	
		virtual liquid::RendererInterface* createRenderer();
		virtual void deleteRenderer();

		virtual void createOutputReceiver();
		virtual void deleteOutputReceiver();

		virtual liquid::RenderNodeVisitorInterface* createRenderNodeVisitor();
		virtual liquid::RenderNodeVisitorInterface* deleteRenderNodeVisitor();

		virtual liquid::ShadingNetworkUserBase* createShadingNetworkUser();
		virtual liquid::ShadingNetworkUserBase* deleteShadingNetworkUser();

	private:
		Factory(const Factory&);
		Factory& operator=(const Factory&);

		Renderer* m_renderer;
		RenderNodeVisitor* m_rendernode_visitor;
		ShadingNetworkUser* m_shadingnetwork_user;
	};
}

#endif//_ER_FACTORY_H_