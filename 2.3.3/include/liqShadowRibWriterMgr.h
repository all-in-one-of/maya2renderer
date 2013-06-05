#ifndef liqShadowRibWriterMgr_H
#define liqShadowRibWriterMgr_H

#include <common/prerequest_std.h>
#include <liqtypes.h>
#include <liqRefactoring.h>
#include <liqStructJob.h>

struct liqGlobalVariable;

class LIQUID_EXPORT tShadowRibWriterMgr
{
public:
	tShadowRibWriterMgr();
	virtual ~tShadowRibWriterMgr();

	TempControlBreak write(struct liqGlobalVariable &liqglo___, 
		const structJob &currentJob___,
		const long scanTime__,
		const bool m_outputLightsInDeepShadows__,
		std::vector<structJob>  jobList__);
// 	static void ribPrologue_samples(liqFloat xsamples, liqFloat ysamples);
// 	static void ribPrologue_shadingrate(liqFloat size);
// 	static void ribPrologue_filter(
// 		liquidlong m_rFilter,
// 		liqFloat m_rFilterX, liqFloat m_rFilterY);
// 	static void ribPrologue_pass(liqString pass);
// 
// 	static void framePrologue_display(const structJob &currentJob);

protected:
	tShadowRibWriterMgr(const tShadowRibWriterMgr&);
	tShadowRibWriterMgr& operator=(const tShadowRibWriterMgr&);
};
#endif//liqShadowRibWriterMgr_H
