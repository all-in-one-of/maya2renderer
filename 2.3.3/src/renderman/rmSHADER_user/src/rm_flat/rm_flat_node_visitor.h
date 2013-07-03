
#ifndef _RM_FLAT_NODE_VISITOR_H_
#define _RM_FLAT_NODE_VISITOR_H_

#include <rnode_visitor_interface.h>

class MFnPlugin;

namespace renderman
{

	class FlatNodeVisitor : public liquid::RenderNodeVisitorInterface2
	{
	public:
		static FlatNodeVisitor* getInstancePtr();
		static bool regist(MFnPlugin &plugin);
		static bool unregist(MFnPlugin &plugin);

		virtual void setValidConnection(liquidmaya::ShaderConnectionMap& scmap);
		virtual bool visit(const char *node);
		virtual bool onCreateInstance(const char *node);

	private:
		FlatNodeVisitor();
		~FlatNodeVisitor();

		static FlatNodeVisitor* m_instance;

	private:
		FlatNodeVisitor(const FlatNodeVisitor&);
		FlatNodeVisitor& operator=(const FlatNodeVisitor&);

	};


}//namespace

#endif//_RM_FLAT_NODE_VISITOR_H_
