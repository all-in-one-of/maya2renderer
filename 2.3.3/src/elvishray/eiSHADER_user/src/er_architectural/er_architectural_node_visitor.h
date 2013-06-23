
#ifndef _ER_ARCHITECTURAL_NODE_VISITOR_H_
#define _ER_ARCHITECTURAL_NODE_VISITOR_H_

#include <rnode_visitor_interface.h>

class MFnPlugin;

namespace elvishray
{

	class ArchitecturalNodeVisitor : public liquid::RenderNodeVisitorInterface2
	{
	public:
		static ArchitecturalNodeVisitor* getInstancePtr();
		static bool regist(MFnPlugin &plugin);
		static bool unregist(MFnPlugin &plugin);

		virtual void setValidConnection(liquidmaya::ShaderConnectionMap& scmap);
		virtual bool visit(const char *node);
		virtual bool onCreateInstance(const char *node);

	private:
		ArchitecturalNodeVisitor();
		~ArchitecturalNodeVisitor();

		static ArchitecturalNodeVisitor* m_instance;

	private:
		ArchitecturalNodeVisitor(const ArchitecturalNodeVisitor&);
		ArchitecturalNodeVisitor& operator=(const ArchitecturalNodeVisitor&);

	};


}//namespace

#endif//_ER_ARCHITECTURAL_NODE_VISITOR_H_
