
#ifndef _ER_TEST_LIGHT_NODE_VISITOR_H_
#define _ER_TEST_LIGHT_NODE_VISITOR_H_

#include <rnode_visitor_interface.h>

class MFnPlugin;

namespace elvishray
{

	class TestLightNodeVisitor : public liquid::RenderNodeVisitorInterface2
	{
	public:
		static TestLightNodeVisitor* getInstancePtr();
		static bool regist(MFnPlugin &plugin);
		static bool unregist(MFnPlugin &plugin);

		virtual void setValidConnection(liquidmaya::ShaderConnectionMap& scmap);
		virtual bool visit(const char *node);
		virtual bool onCreateInstance(const char *node);

	private:
		TestLightNodeVisitor();
		~TestLightNodeVisitor();

		static TestLightNodeVisitor* m_instance;

	private:
		TestLightNodeVisitor(const TestLightNodeVisitor&);
		TestLightNodeVisitor& operator=(const TestLightNodeVisitor&);

	};


}//namespace

#endif//_ER_TEST_LIGHT_NODE_VISITOR_H_
