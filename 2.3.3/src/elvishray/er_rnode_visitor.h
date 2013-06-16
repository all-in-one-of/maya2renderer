#ifndef _ER_RENDER_NODE_VISITOR_H_
#define _ER_RENDER_NODE_VISITOR_H_

#include <rnode_visitor_interface.h>

namespace elvishray
{
	class RenderNodeVisitor : public liquid::RenderNodeVisitorInterface
	{
	public:
		RenderNodeVisitor();
		virtual ~RenderNodeVisitor();

		virtual void setValidConnection_SubRenderer(liquidmaya::ShaderConnectionMap& scmap);
	};
}

#endif//_ER_RENDER_NODE_VISITOR_H_