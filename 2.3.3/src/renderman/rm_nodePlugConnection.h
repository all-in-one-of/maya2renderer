#ifndef RM_NODE_PLUG_CONNECTION_H
#define RM_NODE_PLUG_CONNECTION_H

#include <common/prerequest_std.h>
#include <common/prerequest_maya.h>
namespace renderman
{
	class NodePlugInfo;
	//////////////////////////////////////////////////////////////////////////
	//

	class NodePlugConnection
	{
	public:
		NodePlugConnection();
		NodePlugConnection(const NodePlugConnection& o);
		~NodePlugConnection();

		NodePlugConnection& operator=(const NodePlugConnection& o);

		void swap(NodePlugConnection& o);

		std::string log()const;
		void print(const std::string& indent, const std::string& prefix)const;

		std::string m_node;//node name
		std::vector<std::string> inputSrc;
		std::vector<std::string> inputDes;
		std::vector<std::string> outputSrc;
	protected:
	};
	void swap(NodePlugConnection& a, NodePlugConnection& b);
	//////////////////////////////////////////////////////////////////////////
	//m_data
	//    |
	//    |__<node0>
	//    |   |__<inputSrc> --><...>
	//    |   |__<inputDes> --><...>
	//    |   |__<outputSrc>--><...>
	//    |
	//    |__<node1>
	//    |   |__<inputSrc> --><...>
	//    |   |__<inputDes> --><...>
	//    |   |__<outputSrc>--><...>
	class NodePlugConnectionMgr
	{
	public:
		NodePlugConnectionMgr();
		~NodePlugConnectionMgr();

		void NodePlugConnectionMgr::add(
			const MString &currentNode,
			const MStringArray& inputSrc,
			const MStringArray& inputDes,
			const MStringArray& outputSrc);
		void print(const std::string& indent, const std::string& prefix)const;
		
		std::string cookRSLFunctionCallString(const std::size_t I)const;
		std::string log(const std::size_t I)const;

		std::size_t size(){ return m_data.size(); }
		const NodePlugConnection&
			get(std::size_t i)const { return m_data[ i ]; }

		void setNodePlugMgr(NodePlugInfo *nodePlugInfo);
		void reset();

	protected:
		std::vector<NodePlugConnection> m_data;
		NodePlugInfo *m_NodePlugInfoMgr;

	private:
		NodePlugConnectionMgr(const NodePlugConnectionMgr&);
		NodePlugConnectionMgr& operator=(const NodePlugConnectionMgr&);
	};


}

#endif//RM_NODE_PLUG_CONNECTION_H