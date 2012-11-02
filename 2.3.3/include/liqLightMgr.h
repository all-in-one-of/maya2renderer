#ifndef liqLightMgr_H
#define liqLightMgr_H

#include <boost/shared_ptr.hpp>
#include <maya/MStatus.h>
#include <liqRibHT.h>

class MFnLight;
class MDagPath;

class tLightMgr
{
public:
	tLightMgr();
	virtual ~tLightMgr();
	void scanScene(
		const float lframe__, const int sample__,
		boost::shared_ptr< liqRibHT > &htable__,
		int &count__,
		MStatus &returnStatus__);
	static MStatus buildShadowJob( 
		structJob &thisJob___, bool m_lazyCompute__
		);
	static MStatus tLightMgr::buildShadowCameraJob( 
		structJob &thisJob___, bool m_lazyCompute__
		);
	static void buildShadowJob_SpotAndDirectionLight(
		structJob &thisJob____, MDagPathArray &shadowCamPath, 
		const MFnLight &fnLightNode, const MDagPath &lightPath__,
		const bool m_lazyCompute__,
		bool &lightHasShadowCam
		);
	static void buildShadowJob_PointLight(
		structJob &thisJob____, MDagPathArray &shadowCamPath, 
		const MFnLight &fnLightNode, 
		const MDagPath &lightPath__,
		const bool m_lazyCompute__,
		bool &lightHasShadowCam
		);
private:
	tLightMgr(const tLightMgr&);
	tLightMgr& operator=(const tLightMgr&);
};

#endif//liqLightMgr_H