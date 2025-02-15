#include "er_rnode_visitor.h"
#include "er_rnode_visitor_mgr.h"

namespace elvishray
{
	RenderNodeVisitor::RenderNodeVisitor()
	{

	}
	//
	RenderNodeVisitor::~RenderNodeVisitor()
	{

	}
	//
	void RenderNodeVisitor::setValidConnection_SubRenderer(liquidmaya::ShaderConnectionMap& validConnectionMap)
	{
		//er_physicalsky
		validConnectionMap.begin("er_physicalsky");
		validConnectionMap.append("sun_dirX");
		validConnectionMap.append("sun_dirY");
		validConnectionMap.append("sun_dirZ");
		validConnectionMap.append("sun_dir");
		validConnectionMap.append("sun_disk_size");
		validConnectionMap.append("sun_disk_intensity");
		validConnectionMap.append("sun_glow_size");
		validConnectionMap.append("sun_glow_intensity");
		validConnectionMap.append("sun_glow_falloff");
		validConnectionMap.append("ground_colorR");
		validConnectionMap.append("ground_colorG");
		validConnectionMap.append("ground_colorB");
		validConnectionMap.append("ground_color");
		validConnectionMap.append("ground_blur");
		validConnectionMap.append("type");
		validConnectionMap.append("haze");
		validConnectionMap.append("zenith_colorR");
		validConnectionMap.append("zenith_colorG");
		validConnectionMap.append("zenith_colorB");
		validConnectionMap.append("zenith_color");
		validConnectionMap.append("a");
		validConnectionMap.append("b");
		validConnectionMap.append("c");
		validConnectionMap.append("d");
		validConnectionMap.append("e");
		validConnectionMap.append("intensity");
		validConnectionMap.end();




		//
		RNodeVisitorMgr::getInstancePtr()->setValidConnection(validConnectionMap);
	}

}
