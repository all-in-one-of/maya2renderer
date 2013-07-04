#include "rm_rnode_visitor.h"
#include "rm_rnode_visitor_mgr.h"

namespace renderman
{
	RenderNodeVisitor::RenderNodeVisitor()
	{

	}
	//
	RenderNodeVisitor::~RenderNodeVisitor()
	{

	}
	//
	void RenderNodeVisitor::setValidConnection_SubRenderer(liquidmaya::ShaderConnectionMap& validConnectionMap)
	{

		//
		RNodeVisitorMgr::getInstancePtr()->setValidConnection(validConnectionMap);
	}

}
