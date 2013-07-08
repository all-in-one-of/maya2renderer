#include "er_shadingnetwork.h"
#include <liqlog.h>

namespace elvishray
{
	ShadingNetworkUser::ShadingNetworkUser()
	{

	}
	ShadingNetworkUser::~ShadingNetworkUser()
	{

	}
	bool ShadingNetworkUser::beforeExport_user()
	{
		CM_TRACE_FUNC("ShadingNetworkUser::beforeExport_user()");

		//If false is returned, shading network will not exported.
		return true;
	}
	void ShadingNetworkUser::export_user()
	{
		CM_TRACE_FUNC("ShadingNetworkUser::export_user()");

	}
	void ShadingNetworkUser::afterExport_user()
	{
		CM_TRACE_FUNC("ShadingNetworkUser::afterExport_user()");
	}
}