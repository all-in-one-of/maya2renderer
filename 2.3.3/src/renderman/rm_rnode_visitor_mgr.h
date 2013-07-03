#ifndef _RM_RENDER_NODE_VISITOR_MGR_H_
#define _RM_RENDER_NODE_VISITOR_MGR_H_

#include <common/prerequest_std.h>
#include <rnode_visitor_interface.h>
#include "rm_export.h"

namespace renderman
{
	class RM_EXPORT RNodeVisitorMgr
	{
	public:
		static RNodeVisitorMgr* getInstancePtr();

		bool regist(const std::string &rnodetype, 
			liquid::RenderNodeVisitorInterface2* mgr);
		bool unregist(const std::string &rnodetype);

		bool hasNodeType(const char *nodeType)const;
		void setValidConnection(liquidmaya::ShaderConnectionMap& validConnectionMap);
		bool visit(const char *node);
		bool onCreateInstance(const char *node);

	protected:
		RNodeVisitorMgr();
		~RNodeVisitorMgr();

		static RNodeVisitorMgr* m_instance;

	protected:
		std::map<
			std::string, 
			liquid::RenderNodeVisitorInterface2*
		> m_map;
	};

}//namespace

#endif//_RM_RENDER_NODE_VISITOR_MGR_H_