
#ifndef _ER_ER_SKYLIGHT__NODE_VISITOR_H_
#define _ER_ER_SKYLIGHT__NODE_VISITOR_H_

#include <rnode_visitor_interface.h>

class MFnPlugin;

namespace elvishray
{

	class erSkylightNodeVisitor : public liquid::RenderNodeVisitorInterface2
	{
	public:
		static erSkylightNodeVisitor* getInstancePtr();
		static bool regist(MFnPlugin &plugin);
		static bool unregist(MFnPlugin &plugin);

		virtual void setValidConnection(liquidmaya::ShaderConnectionMap& scmap);
		virtual bool visit(const char *node);
		virtual bool onCreateInstance(const char *node);

	private:
		erSkylightNodeVisitor();
		~erSkylightNodeVisitor();

		static erSkylightNodeVisitor* m_instance;

	private:
		erSkylightNodeVisitor(const erSkylightNodeVisitor&);
		erSkylightNodeVisitor& operator=(const erSkylightNodeVisitor&);

	};


}//namespace

#endif//_ER_ER_SKYLIGHT__NODE_VISITOR_H_
