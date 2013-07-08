#ifndef _SHADING_NETWORK_USER_BASE_H_
#define _SHADING_NETWORK_USER_BASE_H_

namespace liquid
{
	class ShadingNetworkUserBase
	{
	public:
		ShadingNetworkUserBase(){}
		virtual ~ShadingNetworkUserBase(){}

		virtual bool beforeExport_user() = 0;
		virtual void export_user() = 0;
		virtual void afterExport_user() = 0;
	};
}//namespace liquid

#endif//_SHADING_NETWORK_USER_BASE_H_