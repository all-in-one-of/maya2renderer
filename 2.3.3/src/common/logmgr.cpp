#include "logmgr.h"

namespace liquid
{
	//LogMgr* LogMgr::m_instance = 0;

	LogMgr::LogMgr()
	{

	}
	//
	LogMgr::~LogMgr()
	{

	}
	void LogMgr::open(const char* logfile, std::ios_base::open_mode mod)
	{
		m_logfile.open(logfile, mod);
	}
	void LogMgr::close()
	{
		m_logfile.close();
	}
	//
	// LogMgr* LogMgr::getInstancePtr()
	// {
	// 	if (m_instance==0)
	// 	{
	// 		m_instance = new LogMgr();
	// 	}
	// 	return m_instance;
	// }
	//
}