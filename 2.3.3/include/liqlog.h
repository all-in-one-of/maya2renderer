#ifndef liqLog_H
#define liqLog_H

#include <trace/trace.hpp>
#include <liqGlobalHelpers.h>
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

LIQUID_EXPORT void _liqRIBMsg(const char* msg);
//output debug info to rib file
#define liqRIBMsg( msg,...) \
{\
	if(liqRibTranslator::getInstancePtr()->m_outputDebugMsgToRib){\
		sprintf_s(gLogBuffer, LOG_BUFFER_LEN, (msg),__VA_ARGS__ );\
		_liqRIBMsg(gLogBuffer);\
	}\
}

LIQUID_EXPORT void _logFunctionCall(const char* log);

extern LIQUID_EXPORT char gLogBuffer[LOG_BUFFER_LEN];

#endif//liqLog_H