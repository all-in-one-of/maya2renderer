#ifndef _RM_FACTORY_H_
#define _RM_FACTORY_H_

#include "../liqAbstractFactory.h"

namespace renderman
{
	class Renderer;
	class RenderNodeVisitor;
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

	private:
		Factory(const Factory&);
		Factory& operator=(const Factory&);

		Renderer* m_renderer;
		RenderNodeVisitor* m_rendernode_visitor;
	};
}

#endif//_RM_FACTORY_H_