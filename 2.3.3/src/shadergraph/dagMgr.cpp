#include "dagMgr.h"
#include <common/mayacheck.h>
#include <liqlog.h>
#include "shadermgr.h"
#include "convertShadingNetwork.h"

namespace liquidmaya
{
	DagMgr::DagMgr()
	{

	}
	//
	DagMgr::~DagMgr()
	{

	}
	//
	void DagMgr::BreadthFirstTraversal(const MString &root, MStringArray& result)
	{
		CM_TRACE_FUNC("DagMgr::BreadthFirstTraversal("<<root.asChar()<<", ret)");

		begin(root);

		while( ! m_todo.empty() )
		{
			std::string next(m_todo.front());
			m_todo.pop();

			pushInputPlugs(next.c_str());
		}


		while( ! m_UpStreamNodes.empty() )
		{
			if( ! isVisited(m_UpStreamNodes.front()) )
			{
				visit(m_UpStreamNodes.front());
			}
			m_UpStreamNodes.pop_front();
		}

		end(result);
		
	}
	//
	bool DagMgr::isVisited(const std::string &node)
	{
		CM_TRACE_FUNC("DagMgr::isVisited("<<node<<", ret)");
		
		return ( std::find( m_result.begin(), m_result.end(), node ) != m_result.end() );
	}
	void DagMgr::visit(const std::string &node)
	{
		CM_TRACE_FUNC("DagMgr::visit("<<node<<", ret)");
		m_result.push_back(node);
	}
	void DagMgr::pushInputPlugs(const MString &node)
	{
		CM_TRACE_FUNC("DagMgr::pushInputPlugs("<<node.asChar()<<", ret)");

		MStringArray inputPlugs;
		ShaderMgr::getSingletonPtr()->getValidConnection(node.asChar(), inputPlugs);
		for(size_t i=0; i < inputPlugs.length(); ++i)
		{
			const MString plug(node+"."+inputPlugs[i]);

			if( ConvertShadingNetwork::convertibleConnection(plug) != CT_None )
			{
				MStringArray inNodes;
				IfMErrorWarn(MGlobal::executeCommand( ("listConnections -source on -destination off \""+plug+"\""), inNodes));

				//append inNodes to m_todo
				for(std::size_t i=0; i<inNodes.length(); ++i)
				{
					m_todo.push(inNodes[i].asChar());//push back
					m_UpStreamNodes.push_front(inNodes[i].asChar());//push front
				}
			}
		}
	}
	//
	void DagMgr::begin(const MString &root)
	{
		CM_TRACE_FUNC("DagMgr::isVisited("<<root.asChar()<<")");

		m_UpStreamNodes.clear();
		m_result.clear();
		//clear m_todo
		while( ! m_todo.empty() ) 
			m_todo.pop();

		pushInputPlugs(root);
	}
	//
	void DagMgr::end(MStringArray& result)
	{
		CM_TRACE_FUNC("DagMgr::end()");

		result.clear();

		for(std::vector<std::string>::iterator i = m_result.begin();
			i != m_result.end(); ++i)
		{
			result.append( i->c_str() );
		}
		//std::cout<< " upstream nodes are("<<result.length()<<"): ["<< result <<"]"<<std::endl;

	}
}//namespace liquidmaya