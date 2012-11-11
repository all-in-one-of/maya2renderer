#ifndef liqShadowRibWriterMgr_H
#define liqShadowRibWriterMgr_H

#include <vector>
#include <liquid.h>


struct liqGlobalVariable;

namespace renderman
{
class tShadowRibWriterMgr
{
public:
	tShadowRibWriterMgr();
	virtual ~tShadowRibWriterMgr();

	static void ribPrologue_samples(liqFloat xsamples, liqFloat ysamples);
	static void ribPrologue_shadingrate(liqFloat size);
	static void ribPrologue_filter(
		liquidlong m_rFilter,
		liqFloat m_rFilterX, liqFloat m_rFilterY);
	static void ribPrologue_pass(liqString pass);

	static void framePrologue_display(const structJob &currentJob);

protected:
	tShadowRibWriterMgr(const tShadowRibWriterMgr&);
	tShadowRibWriterMgr& operator=(const tShadowRibWriterMgr&);
};
}//namespace renderman
#endif//liqShadowRibWriterMgr_H
