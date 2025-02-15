#ifndef liqJobScriptMgr_H
#define liqJobScriptMgr_H

#include <common/prerequest_std.h>
#include <common/prerequest_maya.h>
#include <liqtypes.h>
#include <liqRenderScript.h>

class tJobScriptMgr
{
public:
	tJobScriptMgr(liqRenderScript& script);
	virtual ~tJobScriptMgr();

	void setCommonParameters(
		const std::string &renderJobName__,
		const bool useNetRman__,
		const liquidlong m_minCPU__,
		const liquidlong m_maxCPU__,
		const MString &m_dirmaps__
	);
	void addJob(const std::string &title, 
		const std::string &command, const bool remote
	);
	void addDefferedJob(const int currentBlock__,
		const unsigned int frameIndex__,
		const MString &framePreCommand__,
		const MString &frameRibgenCommand__
	);
	void addDefferedJob(
		liqRenderScript::Job &deferredJob__,
		const int currentBlock__,
		const unsigned int frameIndex__,
		const MString &framePreCommand__,
		const MString &frameRibgenCommand__
	);
	
	void cleanupDefferedJob();
	void cleanupRenderScript(const MString &renderScriptName__);
	void cleanupPostJob(const MString &m_postJobCommand__);

	void writeRenderScript(const renderScriptFormat format, const MString &renderScriptName__);

protected:
	liqRenderScript &m_jobScript;

	//
protected:
	tJobScriptMgr();
private:
	tJobScriptMgr(const tJobScriptMgr&);
	tJobScriptMgr& operator=(const tJobScriptMgr&);
};

#endif//liqJobScriptMgr_H