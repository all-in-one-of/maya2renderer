#include <liqConfig.h>
#ifdef _USE_NEWRENDERER_

#pragma once
//#ifndef _NR_FACTORY_H_
//#define _NR_FACTORY_H_

#include "../liqAbstractFactory.h"

namespace newrenderer
{	
	class Renderer;
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

	private:
		Factory(const Factory&);
		Factory& operator=(const Factory&);

		Renderer* m_renderer;
	};
}

//#endif//_NR_FACTORY_H_
#endif//_USE_NEWRENDERER_