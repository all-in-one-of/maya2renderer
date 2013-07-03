
#include "er_rnode_visitor_mgr.h"
#include <common/mayacheck.h>
#include "er_log.h"

namespace elvishray
{
	RNodeVisitorMgr* RNodeVisitorMgr::m_instance = NULL;
	//
	RNodeVisitorMgr* RNodeVisitorMgr::getInstancePtr()
	{
		if( m_instance == NULL )
		{
			m_instance = new RNodeVisitorMgr();
		}
		return m_instance;
	}
	//
	RNodeVisitorMgr::RNodeVisitorMgr()
	{

	}
	//
	RNodeVisitorMgr::~RNodeVisitorMgr()
	{

	}
	//
	bool RNodeVisitorMgr::regist(const std::string &rnodetype, 
		liquid::RenderNodeVisitorInterface2* mgr)
	{
		//CM_TRACE_FUNC("RNodeVisitorMgr::regist("<<rnodetype<<")");

		if(m_map.find(rnodetype) == m_map.end())//not found
		{
			m_map.insert(std::make_pair(rnodetype, mgr));
		}else{
			m_map[rnodetype] = mgr;
		}
		return true;
	}
	bool RNodeVisitorMgr::unregist(const std::string &rnodetype)
	{
		//CM_TRACE_FUNC("RNodeVisitorMgr::unregist("<<rnodetype<<")");

		m_map.erase(rnodetype);
		return true;
	}
	//
	bool RNodeVisitorMgr::hasNodeType(const char *nodeType)const
	{
		CM_TRACE_FUNC("RNodeVisitorMgr::hasNodeType("<<nodeType<<")");

		return ( m_map.find(nodeType) != m_map.end() );
	}
	//
	void RNodeVisitorMgr::setValidConnection(liquidmaya::ShaderConnectionMap& validConnectionMap)
	{
		CM_TRACE_FUNC("RNodeVisitorMgr::setValidConnection(...)");

		std::map<
			std::string, 
			liquid::RenderNodeVisitorInterface2*
		>::iterator 
		i = m_map.begin(),
		b = m_map.begin(),
		e = m_map.end();
		for(; i != e; ++i)
		{
			i->second->setValidConnection(validConnectionMap);
		}
	}
	bool RNodeVisitorMgr::visit(const char *node)
	{
		CM_TRACE_FUNC("RNodeVisitorMgr::visit("<<node<<")");

		MString nodeType;
		IfMErrorWarn( MGlobal::executeCommand( ("nodeType \""+MString(node)+"\""), nodeType) );

		std::map<
			std::string, 
			liquid::RenderNodeVisitorInterface2*
		>::iterator i = m_map.find(nodeType.asChar());
		
		if( i == m_map.end())//not found
		{
			return false;
		}
		
		return i->second->visit(node);
	}
	bool RNodeVisitorMgr::onCreateInstance(const char *node)
	{
		//in batch mode, this function is called after the rendering, 
		//so it complains the cm::trace file is not open, so I omit this CM_TRACE_FUNC()
		//CM_TRACE_FUNC("RNodeVisitorMgr::onCreateInstance("<<node<<")");

		MString nodeType;
		IfMErrorWarn( MGlobal::executeCommand( ("nodeType \""+MString(node)+"\""), nodeType) );

		std::map<
			std::string, 
			liquid::RenderNodeVisitorInterface2*
		>::iterator i = m_map.find(nodeType.asChar());

		if( i == m_map.end())//not found
		{
			return false;
		}

		return i->second->onCreateInstance(node);
	}
}//namespace