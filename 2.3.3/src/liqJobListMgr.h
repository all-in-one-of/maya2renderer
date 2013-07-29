#ifndef _LIQ_JOB_LIST_MGR_H_
#define _LIQ_JOB_LIST_MGR_H_

#include <liqtypes.h>
#include <liqStructJob.h>

class LIQUID_EXPORT liqJobListMgr
{
public:
	static liqJobListMgr* getInstancePtr();
	~liqJobListMgr();

	//std::vector<structJob>& getJobListRef(){ return jobList; }
	//std::vector<structJob>& getShadowListRef(){ return shadowList; }
	//std::vector<structJob>& getReflectionListRef(){ return refList; }
	//std::vector<structJob>& getEnvListRef(){ return envList; }
	//std::vector<structJob>& getTexListRef(){ return txtList; }
	std::vector<structJob>  jobList;
	std::vector<structJob>  shadowList;
	std::vector<structJob>  refList;  // reflections list
	std::vector<structJob>  envList;  // environments list
	std::vector<structJob>  txtList;  // make textures list

protected:
	liqJobListMgr();

	static liqJobListMgr* m_instance;



};

#endif//_LIQ_JOB_LIST_MGR_H_