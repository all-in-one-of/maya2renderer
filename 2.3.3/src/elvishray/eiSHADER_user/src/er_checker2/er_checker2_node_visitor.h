
#ifndef _ER_CHECKER_NODE_VISITOR_H_
#define _ER_CHECKER_NODE_VISITOR_H_

#include <rnode_visitor_interface.h>

class MFnPlugin;

namespace elvishray
{

	class Checker2NodeVisitor : public liquid::RenderNodeVisitorInterface2
	{
	public:
		static Checker2NodeVisitor* getInstancePtr();
		static bool regist(MFnPlugin &plugin);
		static bool unregist(MFnPlugin &plugin);

		virtual void setValidConnection(liquidmaya::ShaderConnectionMap& scmap);
		virtual bool visit(const char *node);
		virtual bool onCreateInstance(const char *node);

	private:
		Checker2NodeVisitor();
		~Checker2NodeVisitor();

		static Checker2NodeVisitor* m_instance;

	private:
		Checker2NodeVisitor(const Checker2NodeVisitor&);
		Checker2NodeVisitor& operator=(const Checker2NodeVisitor&);

	};


}//namespace

#endif//_ER_CHECKER_NODE_VISITOR_H_
