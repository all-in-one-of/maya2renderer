#ifndef liqCameraMgr_H
#define liqCameraMgr_H

#include <common/prerequest_std.h>
#include <common/prerequest_maya.h>
#include <liqRibHT.h>

class tCameraMgr
{
public:
	tCameraMgr();
	virtual ~tCameraMgr();
	void portFieldOfView( int port_width, int port_height,
		double& horizontal,
		double& vertical,
		const MFnCamera& fnCamera )const;	
	void computeViewingFrustum ( double     window_aspect,
		double&    left,
		double&    right,
		double&    bottom,
		double&    top,
		const MFnCamera& cam )const;
	MStatus getCameraTransform(const MFnCamera& cam, structCamera &camStruct );
	void getCameraFilmOffset(const MFnCamera& cam, structCamera &camStruct );
	void getCameraInfo(const MFnCamera& cam, structCamera &camStruct )const;
	MStatus getCameraData( std::vector<structJob>::iterator &iter__ , const int sample__);
	MStatus getCameraData( structJob &iter__ , const int sample__);

private:
	tCameraMgr(const tCameraMgr&);
	tCameraMgr& operator=(const tCameraMgr&);
};

#endif//liqCameraMgr_H