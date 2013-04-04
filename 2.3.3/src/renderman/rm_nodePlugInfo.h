#include <liqConfig.h>
#ifdef _USE_RENDERMAN_

#ifndef RM_NODE_PLUG_INFO_H
#define RM_NODE_PLUG_INFO_H

#include "../common/prerequest_std.h"

namespace renderman
{
	//////////////////////////////////////////////////////////////////////////
	class PlugInfoT
	{
	public:
		PlugInfoT();
		PlugInfoT(const PlugInfoT& o);
		~PlugInfoT();

		PlugInfoT& operator=(const PlugInfoT& o);

		void swap(PlugInfoT& o);

		void print(const std::string& indent, const std::string& prefix)const;

		std::string name;	//plug name, e.g. outColor, transparency, ...
		std::string rsltype;//rsl type , e.g. color, float, float2, vector, normal, ...

	};
	void swap(PlugInfoT& a, PlugInfoT& b);
	//////////////////////////////////////////////////////////////////////////
	//
	class NodePlug
	{
	public:
		NodePlug();
		NodePlug(const NodePlug& o);
		~NodePlug();

		NodePlug& operator=(const NodePlug& o);
		void add(const char* plug, const char* rsltype);

		void swap(NodePlug& o);

		void print(const std::string& indent, const std::string& prefix)const;

		const PlugInfoT* getPlugInfo(const std::string &plugname) const;

		std::string m_node;
		std::vector<PlugInfoT> m_plugs;
	};
	void swap(NodePlug& a, NodePlug& b);
	//////////////////////////////////////////////////////////////////////////
	//m_data
	//    |
	//    |__<node0>
	//    |   |__<plug0> --><PlugInfo>
	//    |   |__<plug1> --><PlugInfo>
	//    |
	//    |__<node1>
	//        |__<plug0> --><PlugInfo>
	//        |__<plug1> --><PlugInfo>
	//        |__<plug2> --><PlugInfo>
	class NodePlugInfo
	{
	public:
		NodePlugInfo();
		~NodePlugInfo();
		
		void add(const char* node, const char* plug, const char* rsltype);
		
		void print(const std::string& indent, const std::string& prefix);

		const PlugInfoT* getPlugInfo(const std::string &node, const std::string& plug) const;
		const PlugInfoT* getPlugInfo(const std::string &node_dot_plug) const;

	protected:
		std::vector<NodePlug> m_data;

	private:
		NodePlugInfo(const NodePlugInfo&);
		NodePlugInfo& operator=(const NodePlugInfo&);
	};
	//-------------------------------------------------------
	//
	//-------------------------------------------------------


}

#endif//RM_NODE_PLUG_INFO_H

#endif//_USE_RENDERMAN_