#ifndef _LIQ_DEFINE_H_
#define _LIQ_DEFINE_H_

#include <common/prerequest_maya.h>

#define LIQ_CANCEL_FEEDBACK_MESSAGE MString( "Liquid -> RIB Generation Cancelled!\n" )
#define LIQ_CHECK_CANCEL_REQUEST    if ( liqRibTranslator::m_escHandler.isInterruptRequested() ) throw( LIQ_CANCEL_FEEDBACK_MESSAGE )
#define LIQ_ADD_SLASH_IF_NEEDED(a) if(a.length()){ if ( a.asChar()[a.length() - 1] != '/' ) { a += "/";}}
//#define LIQ_ANIM_EXT MString(".%0*d");
//#define LIQ_SET_EXT MString( ".%0*s");


#ifdef _WIN32
#  define RM_CMD "cmd.exe /c del"
#else
#  define RM_CMD "/bin/rm"
#endif

//LIQCHECKSTATUS is replaced by IfMErrorMsgReturnIt(stat, msg)


//#undef LIQ_ERROR
#define LIQ_ERROR(MSG) MGlobal::displayError(MString("")+MSG);


#define INFO(EXPR,ENDL) {cout<<#EXPR<<" "<<EXPR<<" ";if(ENDL)cout<<endl;}
#define CHECKERR(STAT,MSG)       \
	if ( MS::kSuccess != STAT ) {   \
	cerr <<"Failed: " <<MSG << " : "<<STAT<< endl;        \
	return MS::kFailure;    \
	}

#define CHECKERRNO(STATUS,MSG,ACTION){ if(STATUS == -1){perror(MSG); ACTION; return MS::kFailure;}}


#endif//_LIQ_DEFINE_H_