#include "shaderOutputMgr.h"
#include "shaderOutput.h"



namespace liquidmaya{


ShaderOutputMgr* ShaderOutputMgr::m_instance = 0;
//
ShaderOutputMgr::ShaderOutputMgr()
{
}
//
ShaderOutputMgr::~ShaderOutputMgr()
{
}
//
ShaderOutputMgr* ShaderOutputMgr::getSingletonPtr()
{
	if( !m_instance){
		m_instance = new ShaderOutputMgr();
	}
	return m_instance;
}
//
void ShaderOutputMgr::addReceiver(ShaderOutputVisitor* visitor)
{
	receivers.push_back( visitor );
}
//
void ShaderOutputMgr::deleteReceivers()
{
	std::vector<ShaderOutputVisitor*>::iterator i = receivers.begin();
	std::vector<ShaderOutputVisitor*>::iterator e = receivers.end();
	for( ; i != e; ++i )
	{
		delete (*i);
	}
	receivers.clear();
}
//
//
void ShaderOutputMgr::notify_initShaderData(const MString& startingNode, const MString &mayaplug)
{
	std::vector<ShaderOutputVisitor*>::iterator i = receivers.begin();
	std::vector<ShaderOutputVisitor*>::iterator e = receivers.end();
	for( ; i != e; ++i )
	{
		(*i)->initShaderData(startingNode, mayaplug);
	}
}
void ShaderOutputMgr::notify_preOutput(const char* shaderNodeName)
{
	std::vector<ShaderOutputVisitor*>::iterator i = receivers.begin();
	std::vector<ShaderOutputVisitor*>::iterator e = receivers.end();
	for( ; i != e; ++i )
	{
		(*i)->preOutput(shaderNodeName);
	}
}
void ShaderOutputMgr::notify_outputBegin(const char* shaderNodeName)
{
	std::vector<ShaderOutputVisitor*>::iterator i = receivers.begin();
	std::vector<ShaderOutputVisitor*>::iterator e = receivers.end();
	for( ; i != e; ++i )
	{
		(*i)->outputBegin(shaderNodeName);
	}
}
void ShaderOutputMgr::notify_outputUpstreamShader(const char* shaderNodeName)
{
	std::vector<ShaderOutputVisitor*>::iterator i = receivers.begin();
	std::vector<ShaderOutputVisitor*>::iterator e = receivers.end();
	for( ; i != e; ++i )
	{
		(*i)->outputUpstreamShader(shaderNodeName);
	}
}
void ShaderOutputMgr::notify_outputShaderMethod()
{
	std::vector<ShaderOutputVisitor*>::iterator i = receivers.begin();
	std::vector<ShaderOutputVisitor*>::iterator e = receivers.end();
	for( ; i != e; ++i )
	{
		(*i)->outputShaderMethod();
	}
}
void ShaderOutputMgr::notify_addShaderMethodBody(
	const MString &currentNode,
	const MStringArray& inputSrc,
	const MStringArray& inputDes,
	const MStringArray& outputSrc
	)
{
	std::vector<ShaderOutputVisitor*>::iterator i = receivers.begin();
	std::vector<ShaderOutputVisitor*>::iterator e = receivers.end();
	for( ; i != e; ++i )
	{
		(*i)->addShaderMethodBody(
			currentNode,
			inputSrc,
			inputDes,
			outputSrc);
	}
}
void ShaderOutputMgr::notify_addShaderMethodVariavles(
	const MString &node,
	const MString &plug,
	const MString &type_,
	const int arraysize,
	const MString &details)
{
	std::vector<ShaderOutputVisitor*>::iterator i = receivers.begin();
	std::vector<ShaderOutputVisitor*>::iterator e = receivers.end();
	for( ; i != e; ++i )
	{
		(*i)->addShaderMethodVariavles(
			node,
			plug,
			type_,
			arraysize,
			details
		);
	}
}
void ShaderOutputMgr::notify_outputEnd()
{
	std::vector<ShaderOutputVisitor*>::iterator i = receivers.begin();
	std::vector<ShaderOutputVisitor*>::iterator e = receivers.end();
	for( ; i != e; ++i )
	{
		(*i)->outputEnd();
	}
}
void ShaderOutputMgr::notify_postOutput()
{
	std::vector<ShaderOutputVisitor*>::iterator i = receivers.begin();
	std::vector<ShaderOutputVisitor*>::iterator e = receivers.end();
	for( ; i != e; ++i )
	{
		(*i)->postOutput();
	}
}
void ShaderOutputMgr::notify_outputShadingGroup(const char* shadingGroupNode)
{
	std::vector<ShaderOutputVisitor*>::iterator i = receivers.begin();
	std::vector<ShaderOutputVisitor*>::iterator e = receivers.end();
	for( ; i != e; ++i )
	{
		(*i)->outputShadingGroup(shadingGroupNode);
	}
}
//
void ShaderOutputMgr::initShaderData(const MString& startingNode, const MString &mayaplug)
{
	notify_initShaderData(startingNode, mayaplug);
}
void ShaderOutputMgr::preOutput(const char* shaderNodeName)
{
	notify_preOutput(shaderNodeName);
}
void ShaderOutputMgr::outputBegin(const char* shaderNodeName)
{
	notify_outputBegin(shaderNodeName);
}
void ShaderOutputMgr::outputUpstreamShader(const char* shaderNodeName)
{
	notify_outputUpstreamShader(shaderNodeName);
}
void ShaderOutputMgr::outputShaderMethod()
{
	notify_outputShaderMethod();
}
void ShaderOutputMgr::addShaderMethodBody(
						 const MString &currentNode,
						 const MStringArray& inputSrc,
						 const MStringArray& inputDes,
						 const MStringArray& outputSrc)
{
	notify_addShaderMethodBody(
		currentNode,
		inputSrc,
		inputDes,
		outputSrc
		);
}
void ShaderOutputMgr::addShaderMethodVariavles(
	const MString &node,
	const MString &plug,
	const MString &type_,
	const int arraysize,
	const MString &details)
{
	notify_addShaderMethodVariavles(
		node,
		plug,
		type_,
		arraysize,
		details
	);
}
void ShaderOutputMgr::outputEnd()
{
	notify_outputEnd();
}
void ShaderOutputMgr::postOutput()
{
	notify_postOutput();
}
void ShaderOutputMgr::outputShadingGroup(const char* shadingGroupNode)
{
	notify_outputShadingGroup(shadingGroupNode);
}
//

}//namespace liquidmaya