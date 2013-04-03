#include <liqConfig.h>
#ifdef _USE_RENDERMAN_

#include "rm_nodePlugInfo.h"

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
	void PlugInfoT::print(const std::string& indent, const std::string& prefix)
	{
		printf("%s \t%s \t %s\n", indent.c_str(), name.c_str(), rsltype.c_str());
	}
	void swap(PlugInfoT& a, PlugInfoT& b)
	{
		a.swap(b);
	}
	//////////////////////////////////////////////////////////////////////////
	PlugInfoPairsT::PlugInfoPairsT()
	{
	}
	PlugInfoPairsT::PlugInfoPairsT(const PlugInfoPairsT& o)
	{
		std::map<PlugNameT, PlugInfoT>::const_iterator i = o.m_data.begin();
		std::map<PlugNameT, PlugInfoT>::const_iterator e = o.m_data.end();
		for(; i!=e; ++i)
		{
			m_data[i->first] = i->second;
		}
	}
	PlugInfoPairsT::~PlugInfoPairsT()
	{
	}

	PlugInfoPairsT& PlugInfoPairsT::operator=(const PlugInfoPairsT& o)
	{
		PlugInfoPairsT t(o);
		this->swap(t);
		return *this;
	}
	void PlugInfoPairsT::insert(const PlugNameT& key, const PlugInfoT& data)
	{
		m_data[ key ] = data;
	}
	void PlugInfoPairsT::swap(PlugInfoPairsT& o)
	{
		using std::swap;

		swap(this->m_data,	o.m_data);
	}
	void PlugInfoPairsT::print(const std::string& indent, const std::string& prefix)
	{
		std::map<PlugNameT, PlugInfoT>::iterator i = m_data.begin();
		std::map<PlugNameT, PlugInfoT>::iterator e = m_data.end();
		for(; i!=e; ++i)
		{
			printf("%s \t%s:\t\t", indent.c_str(), i->first.c_str());
			i->second.print(indent+indent, prefix);
		}
		printf("\n");

	}
	void swap(PlugInfoPairsT& a, PlugInfoPairsT& b)
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
	void NodePlugInfo::begin(const char* node)
	{
		m_currentNode.clear();//reset

		m_currentNode = node;

	}
	void NodePlugInfo::add(const char* plugname, const char* rsltype)
	{
		PlugInfoT n;
		n.name    = plugname;
		n.rsltype = rsltype;

		m_currentPairs.insert(plugname, n);
	}
	void NodePlugInfo::end()
	{
		assert( !m_currentNode.empty() );

		m_data[ m_currentNode ] = m_currentPairs;



	}

	void NodePlugInfo::print(const std::string& indent, const std::string& prefix)
	{
		std::map<NodeNameT, PlugInfoPairsT>::iterator i = m_data.begin();
		std::map<NodeNameT, PlugInfoPairsT>::iterator e = m_data.end();
	
		for(; i!=e; ++i)
		{
			printf("%s node:%s--------------\n", indent.c_str(), i->first.c_str());
			i->second.print(indent+indent, prefix);
		}
		printf("\n");
	}
}
#endif//_USE_RENDERMAN_