#include "rm_nodePlugConnection.h"
#include <liqGlobalHelpers.h>
#include "rm_nodePlugInfo.h"

namespace renderman
{
	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	NodePlugConnection::NodePlugConnection()
	{
	}
	NodePlugConnection::NodePlugConnection(const NodePlugConnection& o)
	{
		m_node		= o.m_node;
		inputSrc	= o.inputSrc;
		inputDes	= o.inputDes;
		outputSrc	= o.outputSrc;
	}
	NodePlugConnection::~NodePlugConnection()
	{
	}

	NodePlugConnection& NodePlugConnection::operator=(const NodePlugConnection& o)
	{
		NodePlugConnection tmp(o);
		this->swap(tmp);
		return *this;
	}
	void NodePlugConnection::swap(NodePlugConnection& o)
	{
		using std::swap;

		swap(this->m_node,		o.m_node);
		swap(this->inputSrc,	o.inputSrc);
		swap(this->inputDes,	o.inputDes);
		swap(this->outputSrc,	o.outputSrc);
	}
	std::string NodePlugConnection::log()const
	{
		std::string ret;

		ret += "//inputSrc: ";
		for(std::size_t i = 0; i<inputSrc.size(); ++i)
		{
			ret += inputSrc[ i ] + ", ";
		}
		ret +="\n";

		ret += "//inputDes: ";
		for(std::size_t i = 0; i<inputDes.size(); ++i)
		{
			ret += inputDes[ i ] + ", ";
		}
		ret +="\n";

		ret += "//outputSrc: ";
		for(std::size_t i = 0; i<outputSrc.size(); ++i)
		{
			ret += outputSrc[ i ] + ", ";
		}
		ret +="\n";	

		return ret;
	}
	void NodePlugConnection::print(const std::string& indent, const std::string& prefix)const
	{
		printf("%s %s-------------:\n", indent.c_str(), m_node.c_str());
		printf("%s\n", log().c_str() );
		printf("\n");
	}
	void swap(NodePlugConnection& a, NodePlugConnection& b)
	{
		a.swap(b);
	}
	//////////////////////////////////////////////////////////////////////////
	NodePlugConnectionMgr::NodePlugConnectionMgr()
	{
		m_NodePlugInfoMgr = NULL;
	}
	//
	NodePlugConnectionMgr::~NodePlugConnectionMgr()
	{

	}
	//
	void NodePlugConnectionMgr::add(
		const MString &currentNode,
		const MStringArray& inputSrc,
		const MStringArray& inputDes,
		const MStringArray& outputSrc)
	{
		CM_TRACE_FUNC("NodePlugConnectionMgr::add("<<currentNode.asChar()<<")");
		NodePlugConnection n;
		m_data.push_back(n);
		std::vector<NodePlugConnection>::reverse_iterator i = m_data.rbegin();

		i->m_node = currentNode.asChar();

		i->inputSrc.reserve(inputSrc.length());
		for(std::size_t j = 0; j<inputSrc.length(); ++j){
			i->inputSrc.push_back(inputSrc[j].asChar());
		}

		i->inputDes.reserve(inputDes.length());
		for(std::size_t j = 0; j<inputDes.length(); ++j){
			i->inputDes.push_back(inputDes[j].asChar());
		}

		i->outputSrc.reserve(outputSrc.length());
		for(std::size_t j = 0; j<outputSrc.length(); ++j){
			i->outputSrc.push_back(outputSrc[j].asChar());
		}
	}
	//
	void NodePlugConnectionMgr::print(const std::string& indent, const std::string& prefix)const
	{
		printf("\n-----------------------------------\n");
		printf("NodePlugConnectionMgr\n");
		printf("-----------------------------------\n");
		std::vector<NodePlugConnection>::const_iterator i = m_data.begin();
		std::vector<NodePlugConnection>::const_iterator e = m_data.end();
		for(; i!=e; ++i)
		{
			i->print(indent, prefix);			
		}
		printf("\n");
	}
	//
	std::string NodePlugConnectionMgr::cookRSLFunctionCallString(const std::size_t I)const
	{
		CM_TRACE_FUNC("NodePlugConnectionMgr::cookRSLFunctionCallString("<<I<<")");
		//1.cook parameter list
		const NodePlugConnection& node = get(I);

		assert(m_NodePlugInfoMgr);

		std::vector<std::string> paramlist;
		paramlist.reserve(node.inputSrc.size()+node.outputSrc.size());
		// add inputSrc to param list
		for(std::size_t i=0; i<node.inputSrc.size(); ++i)
		{
 			const PlugInfoT *pSrc 
				= m_NodePlugInfoMgr->getPlugInfo(node.inputSrc[i]);
 			const PlugInfoT *pDes
				= m_NodePlugInfoMgr->getPlugInfo(node.inputDes[i]);
 			
 			if(pSrc->rsltype == pDes->rsltype){
				paramlist.push_back(node.inputSrc[ i ]) ;
 			} else {
 				//type conversion
 				liquidMessage2(messageInfo, 
 					"\"%s\" is converted to \"%s\".",
 					node.inputSrc[i].c_str(), pDes->rsltype.c_str());
  				paramlist.push_back(pDes->rsltype + " " + node.inputSrc[ i ]) ;
 			}
		}
		// add inputDes to param list
		for(std::size_t i=0; i<node.outputSrc.size(); ++i)
		{
			paramlist.push_back(node.outputSrc[ i ]);
		}

		//2. add shader function name
		std::string ret;
		ret += getShaderName(node.m_node.c_str()).asChar();//shader name
		ret += "(";
		
		//3. add ',' into parameter list
		if( paramlist.size()>0 )
			ret += paramlist[ 0 ];
		for(std::size_t i=1; i<paramlist.size(); ++i)
		{
			ret += ", " + paramlist[ i ];
		}

		ret += ");";

		//4. replace '.' with '_'
		ret = boost::replace_all_copy(ret, ".", "_");
		ret = boost::replace_all_copy(ret, "|", "_" );
		ret = boost::replace_all_copy(ret, ":", "_" );
		return ret;
	}
	std::string NodePlugConnectionMgr::log(const std::size_t I)const
	{
		CM_TRACE_FUNC("NodePlugConnectionMgr::log("<<I<<")");
		return get(I).log();
	}
	void NodePlugConnectionMgr::setNodePlugMgr(NodePlugInfo *nodePlugInfo)
	{
		CM_TRACE_FUNC("NodePlugConnectionMgr::setNodePlugMgr(...)");
		m_NodePlugInfoMgr = nodePlugInfo;
	}
	void NodePlugConnectionMgr::reset()
	{
		CM_TRACE_FUNC("NodePlugConnectionMgr::reset()");

		m_data.clear();
	}
}