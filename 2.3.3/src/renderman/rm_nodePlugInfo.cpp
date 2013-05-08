#include <liqConfig.h>
#ifdef _USE_RENDERMAN_

#include "rm_nodePlugInfo.h"
#include "liqGlobalHelpers.h"
#include <liqlog.h>

namespace renderman
{
	//////////////////////////////////////////////////////////////////////////
	PlugInfoT::PlugInfoT()
	{
	}
	PlugInfoT::PlugInfoT(const PlugInfoT& o)
	{
		name	= o.name;
		rsltype = o.rsltype;
	}
	PlugInfoT::~PlugInfoT()
	{
	}
	PlugInfoT& PlugInfoT::operator=(const PlugInfoT& o)
	{
		PlugInfoT t(o);
		this->swap(t);
		return *this;
	}
	void PlugInfoT::swap(PlugInfoT& o)
	{
		using std::swap;

		swap(this->name,	o.name);
		swap(this->rsltype, o.rsltype);
	}
	void PlugInfoT::print(const std::string& indent, const std::string& prefix)const
	{
		printf("%s \t%s \t %s\n", indent.c_str(), name.c_str(), rsltype.c_str());
	}
	void swap(PlugInfoT& a, PlugInfoT& b)
	{
		a.swap(b);
	}
	//////////////////////////////////////////////////////////////////////////
	NodePlug::NodePlug()
	{
	}
	NodePlug::NodePlug(const NodePlug& o)
	{
		m_plugs.clear();

		m_node = o.m_node;

		this->m_plugs.resize(o.m_plugs.size());
		std::copy( o.m_plugs.begin(), o.m_plugs.end(), this->m_plugs.begin() );
	}
	NodePlug::~NodePlug()
	{
	}

	NodePlug& NodePlug::operator=(const NodePlug& o)
	{
		NodePlug t(o);
		this->swap(t);
		return *this;
	}
	void NodePlug::add(const char* plug, const char* rsltype)
	{
		CM_TRACE_FUNC("NodePlug::add("<<plug<<", "<<rsltype<<")");
		//try to update
		std::vector<PlugInfoT>::reverse_iterator i = m_plugs.rbegin();
		std::vector<PlugInfoT>::reverse_iterator e = m_plugs.rend();
		for(; i!=e; ++i)
		{
			if( i->name == plug )//found and update
			{
				i->rsltype = convertMayaTypeToRSLFloat(rsltype);
				return;
			}
		}

		if( i == e )//not found, add a new one
		{
			PlugInfoT n;
			n.name    = plug;
			n.rsltype = convertMayaTypeToRSLFloat(rsltype);

			m_plugs.push_back(n);
		}
	}
	void NodePlug::swap(NodePlug& o)
	{
		using std::swap;

		swap(this->m_node,		o.m_node);
		swap(this->m_plugs,		o.m_plugs);
	}
	void NodePlug::print(const std::string& indent, const std::string& prefix)const
	{
		printf("%s %s %s---------\n", indent.c_str(), prefix.c_str(), m_node.c_str() );
		std::vector<PlugInfoT>::const_iterator i = m_plugs.begin();
		std::vector<PlugInfoT>::const_iterator e = m_plugs.end();
		for(; i!=e; ++i)
		{
			i->print(indent+indent, prefix);
		}
		printf("\n");
	}
	const PlugInfoT* NodePlug::getPlugInfo(const std::string &plugname) const
	{
		CM_TRACE_FUNC("NodePlug::getPlugInfo("<<plugname<<")");
		std::vector<PlugInfoT>::const_iterator i = m_plugs.begin();
		std::vector<PlugInfoT>::const_iterator e = m_plugs.end();
		for(; i!=e; ++i)
		{
			if( i->name == plugname )//found
			{
				return  &(*i);
			}
		}
		liquidMessage2(messageError, "plug \"%s\" not found.", plugname.c_str());
		return NULL;
	}
	void swap(NodePlug& a, NodePlug& b)
	{
		a.swap(b);
	}
	//////////////////////////////////////////////////////////////////////////
	NodePlugInfo::NodePlugInfo()
	{
	}
	//
	NodePlugInfo::~NodePlugInfo()
	{

	}
	//
	void NodePlugInfo::add(const char* node, const char* plug, const char* rsltype)
	{
		CM_TRACE_FUNC("NodePlugInfo::add("<<node<<","<<plug<<","<<rsltype<<")");
		//try to find and update
		std::vector<NodePlug>::reverse_iterator i = m_data.rbegin();
		std::vector<NodePlug>::reverse_iterator e = m_data.rend();
		for(; i!=e; ++i)
		{
			if( i->m_node == node )//found and update
			{
				i->add(plug, rsltype);
				return;
			}
		}
		
		//not found, add a new one
		if( i == e )
		{
			NodePlug n;
			n.m_node = node;
			n.add(plug, rsltype);

			m_data.push_back(n);
		}
	}

	void NodePlugInfo::print(const std::string& indent, const std::string& prefix)
	{
		std::vector<NodePlug>::iterator i = m_data.begin();
		std::vector<NodePlug>::iterator e = m_data.end();
		for(; i!=e; ++i)
		{
			i->print(indent+indent, prefix);
		}
		printf("\n");
		if(liqglo.m_logMsgFlush)
		{
			_flushall();
		}
	}
	const PlugInfoT* 
		NodePlugInfo::getPlugInfo(const std::string &nodename, const std::string& plug) const
	{
		CM_TRACE_FUNC("NodePlugInfo::getPlugInfo("<<nodename<<","<<plug<<")");
		std::vector<NodePlug>::const_iterator i = m_data.begin();
		std::vector<NodePlug>::const_iterator e = m_data.end();

		for(; i!=e; ++i)
		{
			if( i->m_node == nodename )//found
			{
				return i->getPlugInfo(plug);
			}
		}

		liquidMessage2(messageError, "node \"%s\" not found.", nodename.c_str());
		return NULL;
	}
	const PlugInfoT* NodePlugInfo::getPlugInfo(const std::string &node_dot_plug) const
	{
		CM_TRACE_FUNC("NodePlugInfo::getPlugInfo("<<node_dot_plug<<")");
		std::size_t i = node_dot_plug.find_first_of('.');
		std::string node(node_dot_plug.substr(0, i));
		std::string plug(node_dot_plug.substr(i+1));

		return getPlugInfo(node, plug);
	}
}
#endif//_USE_RENDERMAN_