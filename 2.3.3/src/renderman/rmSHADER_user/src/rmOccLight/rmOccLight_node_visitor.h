
#ifndef _RM_OCC_LIGHT_NODE_VISITOR_H_
#define _RM_OCC_LIGHT_NODE_VISITOR_H_

#include <rnode_visitor_interface.h>

class MFnPlugin;

namespace renderman
{

	class OccLightNodeVisitor : public liquid::RenderNodeVisitorInterface2
	{
	public:
		static OccLightNodeVisitor* getInstancePtr();
		static bool regist(MFnPlugin &plugin);
		static bool unregist(MFnPlugin &plugin);

		virtual void setValidConnection(liquidmaya::ShaderConnectionMap& scmap);
		virtual bool visit(const char *node);
		virtual bool onCreateInstance(const char *node);

	private:
		OccLightNodeVisitor();
		~OccLightNodeVisitor();

		static OccLightNodeVisitor* m_instance;

	private:
		OccLightNodeVisitor(const OccLightNodeVisitor&);
		OccLightNodeVisitor& operator=(const OccLightNodeVisitor&);

	};


}//namespace

#endif//_RM_OCC_LIGHT_NODE_VISITOR_H_
