
#ifndef _ER_FLAT_COLOR_NODE_VISITOR_H_
#define _ER_FLAT_COLOR_NODE_VISITOR_H_

#include <rnode_visitor_interface.h>

class MFnPlugin;

namespace elvishray
{

	class FlatColorNodeVisitor : public liquid::RenderNodeVisitorInterface2
	{
	public:
		static FlatColorNodeVisitor* getInstancePtr();
		static bool regist(MFnPlugin &plugin);
		static bool unregist(MFnPlugin &plugin);

		virtual void setValidConnection(liquidmaya::ShaderConnectionMap& scmap);
		virtual bool visit(const char *node);
		virtual bool onCreateInstance(const char *node);

	private:
		FlatColorNodeVisitor();
		~FlatColorNodeVisitor();

		static FlatColorNodeVisitor* m_instance;

	private:
		FlatColorNodeVisitor(const FlatColorNodeVisitor&);
		FlatColorNodeVisitor& operator=(const FlatColorNodeVisitor&);

	};


}//namespace

#endif//_ER_FLAT_COLOR_NODE_VISITOR_H_
