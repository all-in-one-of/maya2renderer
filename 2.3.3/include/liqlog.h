#ifndef liqLog_H
#define liqLog_H

#include <common/prerequest_std.h>
#include "liqtypes.h"

#include <trace/trace.hpp>
//#include <liqGlobalHelpers.h>
#include <liqGlobalVariable.h>

// CM_TRACE - begin
#define CM_TRACE_OPEN(trace_file)	cm::Trace::openFile(trace_file)
#define CM_TRACE_CLOSE()			cm::Trace::closeFile()

#define CM_TRACE_FUNC(func_name)	\
	cm::Trace __CM_TRACE__;         \
	if( liqglo.m_logTraceFunction ){\
		std::stringstream __CM_TRACE_SSTR;  \
		__CM_TRACE_SSTR<<func_name;         \
		__CM_TRACE__.LogMsg(__CM_TRACE_SSTR.str());\
	}
// CM_TRACE - end


#define LOG_BUFFER_LEN 1024

#define liquidMessage2(msgtype, msg,...) \
{\
	sprintf_s(gLogBuffer, LOG_BUFFER_LEN, (msg),__VA_ARGS__ );\
	liquidMessage(MString(gLogBuffer), msgtype );\
}

#define liqM(msa) liqMerge(msa,'|').asChar()

//output debug info to rib file
#define liqRIBMsg( msg,...) \
{\
	if(liqRibTranslator::getInstancePtr()->m_outputDebugMsgToRib){\
	sprintf_s(gLogBuffer, LOG_BUFFER_LEN, (msg),__VA_ARGS__ );\
	_liqRIBMsg(gLogBuffer);\
	}\
}

enum liquidVerbosityType 
{
	verbosityOff = 0,
	verbosityErrors = 1,
	messageError = 1,
	verbosityErrorsWarnings = 2,
	messageWarning = 2,
	verbosityAll = 3,
	messageInfo = 3
};

LIQUID_EXPORT void _liqRIBMsg(const char* msg);
LIQUID_EXPORT void liquidMessage( const MString &msg, liquidVerbosityType type );
LIQUID_EXPORT bool liqAssert(const MString & msg);
LIQUID_EXPORT bool liqAssert(const MString &title, const MString & msg, const MString &bYes);
LIQUID_EXPORT bool liqAssert(const MString &title, const MString & msg, const MString &bYes, const MString &bNo);
LIQUID_EXPORT MString liqMerge(const MStringArray& sa, const char split);
LIQUID_EXPORT void _logFunctionCall(const char* log);

extern LIQUID_EXPORT char gLogBuffer[LOG_BUFFER_LEN];

#endif//liqLog_H