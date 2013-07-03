#ifndef _RM_EXPORT_H_
#define _RM_EXPORT_H_

//
#define RM_EXPORT_DLL
//
#ifdef _WIN32
#	ifdef RM_EXPORT_DLL
#		define RM_EXPORT _declspec(dllexport)
#	else
#		define RM_EXPORT _declspec(dllimport)
#	endif
#else
#	define RM_EXPORT
#endif

#include <common/prerequest_std.h>
#include "rm_nodePlugInfo.h"
//
RM_EXPORT std::ofstream& getOutfstreamRef();
RM_EXPORT renderman::NodePlugInfo& getNodePlugInfoRef();


#endif//_ER_EXPORT_H_