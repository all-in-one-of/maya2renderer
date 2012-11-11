#ifndef liqHeroRibWriterMgr_H
#define liqHeroRibWriterMgr_H

#include <vector>
#include <liquid.h>
#include "liqtypes.h"

struct liqGlobalVariable;

class LIQUID_EXPORT tHeroRibWriterMgr
{
public:
	tHeroRibWriterMgr();
	virtual ~tHeroRibWriterMgr();

	TempControlBreak write(struct liqGlobalVariable &liqglo___, 
		const structJob &currentJob___,
		const long scanTime__,
		const bool m_outputLightsInDeepShadows__
		//,std::vector<structJob>  jobList__
		);
// 	static void ribPrologue_samples(liqFloat xsamples, liqFloat ysamples);
// 	static void ribPrologue_shadingrate(liqFloat size);
// 	static void ribPrologue_filter(
// 		liquidlong m_rFilter,
// 		liqFloat m_rFilterX, liqFloat m_rFilterY
// 		);
// 	static void ribPrologue_hider(HiderType hidertype);
// 	static void ribPrologue_pass(liqString pass);
// 
// 	static void framePrologue_display(const structJob &currentJob);

protected:
	tHeroRibWriterMgr(const tHeroRibWriterMgr&);
	tHeroRibWriterMgr& operator=(const tHeroRibWriterMgr&);
};
#endif//liqHeroRibWriterMgr_H
