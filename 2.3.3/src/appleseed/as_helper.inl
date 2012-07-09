#include <liqConfig.h>
#ifdef _USE_APPLESEED_


#include"as_helper.h"
#include "../common/prerequest_std.h"
#include "../common/mayacheck.h"
#include <liqGlobalHelpers.h>
#include <liqlog.h>

namespace appleseed
{
	template<typename T>
	void convertMatrix(asf::Matrix<T, 4, 4> &des, const liqFloat src[4][4])
	{
		des[0] = src[0][0]; des[4] = src[0][1]; des[8]  = src[0][2]; des[12] = src[0][3]; 
		des[1] = src[1][0]; des[5] = src[1][1]; des[9]  = src[1][2]; des[13] = src[1][3]; 
		des[2] = src[2][0]; des[6] = src[2][1]; des[10] = src[2][2]; des[14] = src[2][3]; 
		des[3] = src[3][0]; des[7] = src[3][1]; des[11] = src[3][2]; des[15] = src[3][3]; 
	}

}//namespace appleseed
#endif//_USE_APPLESEED_