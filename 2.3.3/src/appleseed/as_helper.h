#include <liqConfig.h>
#ifdef _USE_APPLESEED_

#pragma once
//#ifndef _AS_Helper_H
//#define _AS_Helper_H
#include "../liqtypes.h"
#include "prerequest_as.h"
/* ______________________________________________________________________
**
** Liquid Globals Node Helper
** ______________________________________________________________________
*/
namespace appleseed
{
	template<typename T>
	void convertMatrix(asf::Matrix<T, 4, 4> &des, const liqFloat src[4][4]);
	void makeTexture(const char* srcImagePath, const char* desImagePath);

	std::string getTextureInstanceName(const std::string& texturename);

	class GlobalNodeHelper;
	std::string getConfiguration(GlobalNodeHelper *node, const std::string &category, const std::string &name);

	// \p $meshFullPathName   e.g. |pCube1|pCubeShape1
	// \r                     e.g. %root%/pCube1/pCubeShape1.obj
	std::string get_filesys_fullPathName(const std::string &meshFullPathName);
	// \p $meshFullPathName   e.g. |pCube1|pCubeShape1
	// \r                     e.g. %root%/pCube1
	std::string get_mesh_dirname(const std::string& meshFullPathName);
	// \p $meshFullPathName   e.g. |pCube1|pCubeShape1
	// \r                     e.g. pCubeShape1.obj
	std::string get_mesh_basename(const std::string& meshFullPathName);

	//
	std::string getBSDFName(const std::string& node, const std::string &bsdf_model);
	std::string getBSDFNameBack(const std::string& node, const std::string &bsdf_model);
	std::string getEDFName(const std::string& node, const std::string &edf_model);
	std::string getSurfaceShaderName(const std::string& node, const std::string & surface_shader_model);
	std::string getBSDFName(const std::string& node);
	std::string getBSDFNameBack(const std::string& node);
	std::string getEDFName(const std::string& node);
	std::string getSurfaceShaderName(const std::string& node);
	std::string getAlphaColorName(const std::string& node);
	std::string getNormalMapName(const std::string& node);
	//bool hasBackfaceMaterial(const std::string& shadingGroupNode);
	bool needToCreateBackfaceMaterial(const std::string& shadingGroupNode);
	bool isRefractionsOpen(const std::string& node);
	std::string getBackfaceMaterial(const std::string& shadingGroupNode);
	std::string getTransparencyName(const std::string& node);


	//\r    true: creat successfully,  false: not create.
	//r g b
	bool createColor3(asr::ColorContainer& colors, const char* name, const float r, const float g, const float b);
	//multiplier, r g b
	bool createColor3(asr::ColorContainer& colors, const char* name, const float multiplier, const float r, const float g, const float b);
	//r g b a
	bool createColor4(asr::ColorContainer& colors, const char* name, const float r, const float g, const float b, const float a);
	//multiplier, r g b	a
	bool createColor4(asr::ColorContainer& colors, const char* name, const float multiplier, const float r, const float g, const float b, const float a);


}//namespace appleseed
//#endif//_AS_Helper_H

#include "as_helper.inl"

#endif//_USE_APPLESEED_