
#ifndef _SHADER_MGR_H_
#define _SHADER_MGR_H_

#include "../common/prerequest_maya.h"
#include "../common/prerequest_std.h"
#include "liqtypes.h"
#include "convertShadingNetwork.h"
#include "shaderValidConnection.h"


namespace liquidmaya
{

	class OutputAgentInterface;
	//
	class LIQUID_EXPORT ShaderMgr
	{
	public:
		static ShaderMgr* getSingletonPtr();
		~ShaderMgr();

		//
		void exportShaderGraph();

		//
		ConnectionType convertibleConnection(const char*plugname);

		// @nodename	e.g. "lambert1","blinn1",...
		void getValidConnection(const char* nodename, MStringArray& connections) const;
		bool hasShaderType(const char* shadertype)const;

	private:
		ShaderMgr();
		static ShaderMgr* m_instance;

		//
		ConvertShadingNetwork shadingnetwork;

		//
		ShaderValidConnection shaderValidConnection;

	private:
		ShaderMgr(const ShaderMgr&);
		ShaderMgr& operator=(const ShaderMgr&);
	};
}


#endif//_SHADER_MGR_H_