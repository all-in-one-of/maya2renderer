#include "shaderValidConnection.h"
#include <liqlog.h>

namespace liquidmaya
{

	void ShaderValidConnection::setValidConnection_mi()
	{
		//mib_amb_occlusion
		validConnectionMap.begin("mib_amb_occlusion");
		validConnectionMap.append("samples");
		validConnectionMap.append("brightR");
		validConnectionMap.append("brightG");
		validConnectionMap.append("brightB");
		validConnectionMap.append("bright");
		validConnectionMap.append("darkR");
		validConnectionMap.append("darkG");
		validConnectionMap.append("darkB");
		validConnectionMap.append("dark");
		validConnectionMap.append("spread");
		validConnectionMap.append("max_distance");
		validConnectionMap.append("reflective");
// 		validConnectionMap.append("output_mode");
// 		validConnectionMap.append("occlusion_in_alpha");
// 		validConnectionMap.append("falloff");
// 		validConnectionMap.append("id_inclexcl");
// 		validConnectionMap.append("id_nonself");
		validConnectionMap.append("outValueR");
		validConnectionMap.append("outValueG");
		validConnectionMap.append("outValueB");
		validConnectionMap.append("outValue");
		validConnectionMap.end();	

	}
}//namespace liquidmaya