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

		void print(const std::string& indent, const std::string& prefix);

		std::string name;	//plug name, e.g. outColor, transparency, ...
		std::string rsltype;//rsl type , e.g. color, float, float2, vector, normal, ...

	};
	void swap(PlugInfoT& a, PlugInfoT& b);
	//////////////////////////////////////////////////////////////////////////
	//map plugname to PlugInfoT
	typedef std::string NodeNameT;
	typedef std::string PlugNameT;
	class PlugInfoPairsT
	{
	public:
		PlugInfoPairsT();
		PlugInfoPairsT(const PlugInfoPairsT& o);
		~PlugInfoPairsT();

		PlugInfoPairsT& operator=(const PlugInfoPairsT& o);
		void insert(const PlugNameT& key, const PlugInfoT& data);

		void swap(PlugInfoPairsT& o);

		void print(const std::string& indent, const std::string& prefix);

		const PlugInfoT* getPlugInfo(const std::string &plugname) const;
	protected:
		std::map<PlugNameT, PlugInfoT> m_data;
	};
	void swap(PlugInfoPairsT& a, PlugInfoPairsT& b);
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

		void begin(const char* node);
		void add(const char* plugname, const char* rsltype);
		void end();

		void print(const std::string& indent, const std::string& prefix);

		const PlugInfoT* getPlugInfo(const std::string &node, const std::string& plug) const;
		const PlugInfoT* getPlugInfo(const std::string &nodeplug) const;

	protected:


		std::map<NodeNameT, PlugInfoPairsT> m_data;
		
	private:
		std::string    m_currentNode;
		PlugInfoPairsT m_currentPairs;

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