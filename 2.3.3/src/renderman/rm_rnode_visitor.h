#ifndef _RM_RENDER_NODE_VISITOR_H_
#define _RM_RENDER_NODE_VISITOR_H_

#include <rnode_visitor_interface.h>

namespace renderman
{
	class RenderNodeVisitor : public liquid::RenderNodeVisitorInterface
	{
	public:
		RenderNodeVisitor();
		virtual ~RenderNodeVisitor();

		virtual void setValidConnection_SubRenderer(liquidmaya::ShaderConnectionMap& scmap);
	};
}

#endif//_RM_RENDER_NODE_VISITOR_H_