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
}//namespace appleseed
//#endif//_AS_Helper_H

#include "as_helper.inl"

#endif//_USE_APPLESEED_