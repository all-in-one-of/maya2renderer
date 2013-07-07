#include "shadermgr.h"
#include <common/mayacheck.h>
#include <liqlog.h>

namespace liquidmaya{

ShaderMgr* ShaderMgr::m_instance = 0;

ShaderMgr::ShaderMgr()
{

}
//
ShaderMgr::~ShaderMgr()
{

}
//
ShaderMgr* ShaderMgr::getSingletonPtr()
{
	if(!m_instance){
		m_instance = new ShaderMgr();
	}
	return m_instance;
}
//
void ShaderMgr::exportShaderGraph()
{
	CM_TRACE_FUNC("ShaderMgr::exportShaderGraph()");

	shadingnetwork.exportShaderGraph();
}
void ShaderMgr::exportShaderNode(const MString& node)
{
	CM_TRACE_FUNC("ShaderMgr::exportShaderNode("<<node.asChar()<<")");

	shadingnetwork.exportShaderNode(node);
}
//
ConnectionType ShaderMgr::convertibleConnection(const char*plugname)
{
	CM_TRACE_FUNC("ShaderMgr::convertibleConnection("<<plugname<<")");
	return shadingnetwork.convertibleConnection(plugname);
}
//
void ShaderMgr::setValidConnection()
{
	CM_TRACE_FUNC("ShaderMgr::setValidConnection()");
	shaderValidConnection.setValidConnection();
}
void ShaderMgr::clearValidConnection()
{
	CM_TRACE_FUNC("ShaderMgr::clearValidConnection()");
	shaderValidConnection.clearValidConnection();
}
void ShaderMgr::getValidConnection(const char* nodename, MStringArray& connections) const
{
	CM_TRACE_FUNC("ShaderMgr::getValidConnection("<<nodename<<")");
	shaderValidConnection.getValidConnection(nodename, connections);
}
ShaderConnectionMap& ShaderMgr::getShaderConnectionMap()
{
	CM_TRACE_FUNC("ShaderMgr::getShaderConnectionMap()");
	return shaderValidConnection.getShaderConnectionMap();
}
bool ShaderMgr::hasShaderType(const char* shadertype)const
{
	CM_TRACE_FUNC("ShaderMgr::hasShaderType("<<shadertype<<")");
	return shaderValidConnection.hasShaderType(shadertype);
}

}//namespace liquidmaya