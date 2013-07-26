
#ifndef _ER_ERPHYSICALSKY_NODE_VISITOR_H_
#define _ER_ERPHYSICALSKY_NODE_VISITOR_H_

#include <rnode_visitor_interface.h>

class MFnPlugin;

namespace elvishray
{

	class erPhysicalskyNodeVisitor : public liquid::RenderNodeVisitorInterface2
	{
	public:
		static erPhysicalskyNodeVisitor* getInstancePtr();
		static bool regist(MFnPlugin &plugin);
		static bool unregist(MFnPlugin &plugin);

		virtual void setValidConnection(liquidmaya::ShaderConnectionMap& scmap);
		virtual bool visit(const char *node);
		virtual bool onCreateInstance(const char *node);

	private:
		erPhysicalskyNodeVisitor();
		~erPhysicalskyNodeVisitor();

		static erPhysicalskyNodeVisitor* m_instance;

	private:
		erPhysicalskyNodeVisitor(const erPhysicalskyNodeVisitor&);
		erPhysicalskyNodeVisitor& operator=(const erPhysicalskyNodeVisitor&);

	};


}//namespace

#endif//_ER_ERPHYSICALSKY_NODE_VISITOR_H_
