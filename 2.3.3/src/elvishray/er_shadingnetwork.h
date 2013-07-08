#ifndef _ER_SHADING_NETWORK_H_
#define _ER_SHADING_NETWORK_H_

#include <shadergraph/ShadingNetworkUserBase.h>

namespace elvishray
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

#endif//_ER_SHADING_NETWORK_H_