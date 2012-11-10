#ifndef _LOG_MGR_H_
#define _LOG_MGR_H_

#include <fstream>
#include <string>

#ifdef _WIN32
#	ifdef LIQUID_SEPARATE
#		ifdef LIQUID_EXPORT_DLL
#			define LOGMGR_EXPORT _declspec(dllexport)
#		else
#			define LOGMGR_EXPORT _declspec(dllimport)
#		endif
#	else
#		define LOGMGR_EXPORT
#	endif
#else
#	define LOGMGR_EXPORT
#endif

namespace liquid
{
	class LOGMGR_EXPORT LogMgr
	{
	public:
		LogMgr();
		//LogMgr* getInstancePtr();
		virtual ~LogMgr();

		void open(const char* logfile);
		void close();

		void log___(const std::string &data)
		{
			m_logfile << data <<std::endl;
		}
		
		std::ofstream& get() { return m_logfile; }

	protected:
		LogMgr(const LogMgr&);
		LogMgr& operator=(const LogMgr&);

		//static LogMgr* m_instance;

		std::ofstream m_logfile;

	};
}


#endif//_LOG_MGR_H_