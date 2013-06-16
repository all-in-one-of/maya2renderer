#ifndef _RENDER_NODE_VISITOR_INTERFACE_H_
#define _RENDER_NODE_VISITOR_INTERFACE_H_

#include "liqtypes.h"
#include "./shadergraph/shaderValidConnection.h"

namespace liquid
{
	//visit the render nodes of your renderer 
	class LIQUID_EXPORT RenderNodeVisitorInterface
	{
	public:
		RenderNodeVisitorInterface(){}
		virtual ~RenderNodeVisitorInterface() = 0 {}

		virtual void setValidConnection_SubRenderer(liquidmaya::ShaderConnectionMap& scmap) = 0;

	};
}//namespace liquid

#endif//_RENDER_NODE_VISITOR_INTERFACE_H_
