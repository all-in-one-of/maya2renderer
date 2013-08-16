#ifndef _LOG_MGR_H_
#define _LOG_MGR_H_

#include <fstream>
#include <string>
#include <liqtypes.h>

namespace liquid
{
	class LIQUID_EXPORT LogMgr
	{
	public:
		LogMgr();
		//LogMgr* getInstancePtr();
		virtual ~LogMgr();

		void open(const char* logfile, std::ios_base::open_mode mod);
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