#include <liqConfig.h>
#ifdef _USE_RENDERMAN_

#include "rm_helper.h"

#include "../common/prerequest_maya.h"
#include "liqGlobalHelpers.h"
#include <liqRibTranslator.h>

namespace renderman
{
	Helper::Helper()
	{

	}
	//
	Helper::Helper(const std::string& ribFile)
		:m_ribFile(ribFile)
	{

	}
	Helper::~Helper()
	{

	}
	//
	void Helper::RiBeginRef(const std::string& ribFile)
	{
		CM_TRACE_FUNC("Helper::RiBeginRef("<<ribFile<<")");

		m_ribFile = ribFile;

		//if m_ribFileFullPath is "", write the data into the current rib file.
		if( m_ribFile != "" ){
			//log
			RiArchiveRecord( RI_COMMENT, "output: %s", m_ribFile.c_str() );
			//write the data into another rib file.
			m_contex = RiGetContext();//push context
			liquidMessage("output rib: "+ std::string(m_ribFile.c_str()) , messageInfo);
			RiBegin_liq( const_cast< RtToken >( m_ribFile.c_str() ) );
		}
	}
	//
	void Helper::RiEndRef()
	{
		CM_TRACE_FUNC("Helper::RiEndRef()");

		if( m_ribFile != "" ){
			RiEnd();
			RiContext(m_contex);//pop context
		}
	}
	//-------------------------------------------------------
	//
	//-------------------------------------------------------
	RibDataExportHelper::RibDataExportHelper(){}
	RibDataExportHelper::~RibDataExportHelper(){}
	//-------------------------------------------------------
	//
	//-------------------------------------------------------
	MString getShadingGroupFilePath(const MString &shadingGroupNodeName)
	{
		CM_TRACE_FUNC("getShadingGroupFilePath("<<shadingGroupNodeName.asChar()<<")");
		return getShaderDirectory()+sanitizeNodeName(shadingGroupNodeName+".rmsg");
	}
	MString getShaderFilePath_NoExt(const MString &shaderNodeName)
	{
		CM_TRACE_FUNC("getShaderFilePath_NoExt("<<shaderNodeName.asChar()<<")");
		return getShaderDirectory()+sanitizeNodeName(shaderNodeName);
	}
	MString getShaderFilePath_SRC(const MString &shaderNodeName)
	{
		CM_TRACE_FUNC("getShaderFilePath_SRC("<<shaderNodeName.asChar()<<")");
		return getShaderFilePath_NoExt(shaderNodeName)+".sl_my";
	}
	MString getShaderFilePath_SLO(const MString &shaderNodeName)
	{
		CM_TRACE_FUNC("getShaderFilePath_SLO("<<shaderNodeName.asChar()<<")");
		return getShaderFilePath_NoExt(shaderNodeName)+".slo";
	}
	MString getShaderName(const MString &shaderName)
	{
		CM_TRACE_FUNC("getShaderName("<<shaderName.asChar()<<")");
		return sanitizeNodeName(shaderName);
	}
	MString getVariableName(const MString &varName)
	{
		CM_TRACE_FUNC("getVariableName("<<varName.asChar()<<")");
		return sanitizeNodeName(varName);
	}
}
#endif//_USE_RENDERMAN_