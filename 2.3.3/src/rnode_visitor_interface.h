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
	//
	class LIQUID_EXPORT RenderNodeVisitorInterface2
	{
	public:
		RenderNodeVisitorInterface2(){}
		virtual ~RenderNodeVisitorInterface2() = 0 {}

		virtual void setValidConnection(liquidmaya::ShaderConnectionMap& scmap) = 0;
		virtual bool visit(const char *node) = 0;
		virtual bool onCreateInstance(const char *node) = 0;

	};
}//namespace liquid

#endif//_RENDER_NODE_VISITOR_INTERFACE_H_
