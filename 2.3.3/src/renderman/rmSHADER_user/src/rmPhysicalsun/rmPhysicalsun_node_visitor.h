
#ifndef _RM_PHYSICAL_SUN_NODE_VISITOR_H_
#define _RM_PHYSICAL_SUN_NODE_VISITOR_H_

#include <rnode_visitor_interface.h>

class MFnPlugin;

namespace renderman
{

	class PhysicalsunNodeVisitor : public liquid::RenderNodeVisitorInterface2
	{
	public:
		static PhysicalsunNodeVisitor* getInstancePtr();
		static bool regist(MFnPlugin &plugin);
		static bool unregist(MFnPlugin &plugin);

		virtual void setValidConnection(liquidmaya::ShaderConnectionMap& scmap);
		virtual bool visit(const char *node);
		virtual bool onCreateInstance(const char *node);

	private:
		PhysicalsunNodeVisitor();
		~PhysicalsunNodeVisitor();

		static PhysicalsunNodeVisitor* m_instance;

	private:
		PhysicalsunNodeVisitor(const PhysicalsunNodeVisitor&);
		PhysicalsunNodeVisitor& operator=(const PhysicalsunNodeVisitor&);

	};


}//namespace

#endif//_RM_PHYSICAL_SUN_NODE_VISITOR_H_
