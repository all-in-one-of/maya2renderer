#ifndef _ER_EXPORT_H_
#define _ER_EXPORT_H_

//
#define ER_EXPORT_DLL
//
#ifdef _WIN32
#	ifdef ER_EXPORT_DLL
#		define ER_EXPORT _declspec(dllexport)
#	else
#		define ER_EXPORT _declspec(dllimport)
#	endif
#else
#	define ER_EXPORT
#endif
//


#endif//_ER_EXPORT_H_