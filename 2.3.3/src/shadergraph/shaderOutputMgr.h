#ifndef _SHADER_OUTPUT_MGR_H_
#define _SHADER_OUTPUT_MGR_H_

#include "../common/prerequest_std.h"
#include "../common/prerequest_maya.h"
#include "liqtypes.h"

namespace liquidmaya{

	class ShaderOutputVisitor;
	//
	class LIQUID_EXPORT ShaderOutputMgr
	{
	public:
		static ShaderOutputMgr* getSingletonPtr();

		~ShaderOutputMgr();

		void initShaderData(const MString& startingNode, const MString &mayaplug);
		void preOutput(const char* shaderNodeName);
		void outputBegin(const char* shaderNodeName);
		void outputUpstreamShader(const char* shaderNodeName);
		void outputShaderMethod();
		void addShaderMethodBody(
			const MString &currentNode,
			const MStringArray& inputSrc,
			const MStringArray& outputSrc);
		void addShaderMethodVariavles(
			const MString &node,
			const MString &plug,
			const MString &type_,
			const int arraysize,
			const MString &details
			);
		void outputEnd();
		void postOutput();

		void outputShadingGroup(const char* shadingGroupNode);

		void addReceiver(ShaderOutputVisitor* visitor);
		void deleteReceivers();

	private:
		ShaderOutputMgr();

		static ShaderOutputMgr* m_instance;
		
		void notify_initShaderData(const MString& startingNode, const MString &mayaplug);
		void notify_preOutput(const char* shaderNodeName);
		void notify_outputBegin(const char* shaderNodeName);
		void notify_outputUpstreamShader(const char* shaderNodeName);
		void notify_outputShaderMethod();
		void notify_addShaderMethodBody(
			const MString &currentNode,
			const MStringArray& inputSrc,
			const MStringArray& outputSrc
			);
		void notify_addShaderMethodVariavles(
			const MString &node,
			const MString &plug,
			const MString &type_,
			const int arraysize,
			const MString &details);
		void notify_outputEnd();
		void notify_postOutput();
		void notify_outputShadingGroup(const char* shadingGroupNode);

		std::vector<ShaderOutputVisitor*> receivers;
	private:
		ShaderOutputMgr(const ShaderOutputMgr&);
		ShaderOutputMgr& operator=(const ShaderOutputMgr&);
	};

}//namespace liquidmaya
#endif//_SHADER_OUTPUT_H_