#ifndef RM_HELPER_H
#define RM_HELPER_H

#include "../common/prerequest_std.h"

#include "ri_interface.h"
#include "liqRibLightData.h"
#include "liqRibMeshData.h"
#include "liqRibShaveData.h"
#include "liqRibPfxHairData.h"
#include "liqRibPfxData.h"
#include "liqRibNode.h"
#include "liqRibHT.h"

namespace renderman
{
	class Helper
	{
	public:
		Helper();
		explicit Helper(const std::string& ribFile);
		~Helper();

		void RiBeginRef(const std::string& ribFile);
		void RiEndRef();

	protected:
		std::string m_ribFile;
		RtContextHandle m_contex;
	};
	//-------------------------------------------------------
	//
	//-------------------------------------------------------
	class RibDataExportHelper
	{
	public:
		RibDataExportHelper();
		~RibDataExportHelper();
	};
	//-------------------------------------------------------
	//
	//-------------------------------------------------------
	MString getShadingGroupFilePath(const MString &shadingGroupNodeName);
	MString getShaderFilePath_NoExt(const MString &shaderNodeName);
	MString getShaderFilePath_SRC(const MString &shaderNodeName);
	MString getShaderFilePath_SLO(const MString &shaderNodeName);
	//-------------------------------------------------------
	void exportJobCamera(const structJob &job, const structCamera camera[]);
	MStatus MaxtrixMotionBlur(const liqRibNodePtr ribNode__, MDagPath &path__, const bool bMotionBlur);
	MStatus objectNonShadowAttribute(const liqRibNodePtr &ribNode__);

	//If $nodename.$plugname is connected from other plug, return 1; else return 0;
	int isPlugConnectedIn(const MString &nodename, const MString &plugname);
	//
	int isBumpMapConnected(const char* nodename);

}

#endif//RM_HELPER_H