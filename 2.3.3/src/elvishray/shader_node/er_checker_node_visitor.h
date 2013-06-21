
#ifndef _ER_CHECKER_NODE_VISITOR_H_
#define _ER_CHECKER_NODE_VISITOR_H_

#include <rnode_visitor_interface.h>

class MFnPlugin;

namespace elvishray
{

	class CheckerNodeVisitor : public liquid::RenderNodeVisitorInterface2
	{
	public:
		static CheckerNodeVisitor* getInstancePtr();
		static bool regist(MFnPlugin &plugin);
		static bool unregist(MFnPlugin &plugin);

		virtual void setValidConnection(liquidmaya::ShaderConnectionMap& scmap);
		virtual bool visit(const char *node);
		virtual bool onCreateInstance(const char *node);

	private:
		CheckerNodeVisitor();
		~CheckerNodeVisitor();

		static CheckerNodeVisitor* m_instance;

	private:
		CheckerNodeVisitor(const CheckerNodeVisitor&);
		CheckerNodeVisitor& operator=(const CheckerNodeVisitor&);

	};


}//namespace

#endif//_ER_CHECKER_NODE_VISITOR_H_
