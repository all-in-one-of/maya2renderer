#ifndef _RM_SHADING_NETWORK_H_
#define _RM_SHADING_NETWORK_H_

#include <shadergraph/ShadingNetworkUserBase.h>

namespace renderman
{
	class ShadingNetworkUser : public liquid::ShadingNetworkUserBase
	{
	public:
		ShadingNetworkUser();
		virtual ~ShadingNetworkUser();

		virtual bool beforeExport_user();
		virtual void export_user();
		virtual void afterExport_user();
	};
}

#endif//_RM_SHADING_NETWORK_H_