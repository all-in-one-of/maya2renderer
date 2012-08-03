#include <liqConfig.h>
#ifdef _USE_APPLESEED_

#if defined(DEBUG) || defined(_DEBUG)
#	pragma comment( lib, "../../../dependence/appleseed/appleseed-1.1.0-alpha-14-0-g4e46ed9-win32-vs100-devkit/lib/Debug/appleseed.lib")
#	pragma comment( lib, "../../../dependence/appleseed/appleseed-1.1.0-alpha-14-0-g4e46ed9-win32-vs100-devkit/lib/Debug/appleseed.shared.lib")
//#	pragma comment( lib, "../../../dependence/appleseed/appleseed-1.1.0-alpha-12-24-g7ad29e2-win32-vs100-devkit/lib/Debug/Half.lib")
#	if USE_OPENEXR_CPP_INTERFACE
#		pragma comment( lib, "../../../dependence/appleseed/appleseed-1.1.0-alpha-12-24-g7ad29e2-win32-vs100-devkit/lib/Debug/Iex.lib")
#		pragma comment( lib, "../../../dependence/appleseed/appleseed-1.1.0-alpha-12-24-g7ad29e2-win32-vs100-devkit/lib/Debug/IlmImf.lib")
#		pragma comment( lib, "../../../dependence/appleseed/appleseed-1.1.0-alpha-12-24-g7ad29e2-win32-vs100-devkit/lib/Debug/IlmThread.lib")
#		pragma comment( lib, "../../../dependence/appleseed/appleseed-1.1.0-alpha-12-24-g7ad29e2-win32-vs100-devkit/lib/Debug/Imath.lib")
#	endif
#else//#if defined(DEBUG) || defined(_DEBUG)
#	pragma comment( lib, "../../../dependence/appleseed/appleseed-1.1.0-alpha-13-0-gc745e2c-win32-vs100-devkit/lib/Release/appleseed.lib")
#	pragma comment( lib, "../../../dependence/appleseed/appleseed-1.1.0-alpha-13-0-gc745e2c-win32-vs100-devkit/lib/Release/appleseed.shared.lib")
//#	pragma comment( lib, "../../../dependence/appleseed/appleseed-1.1.0-alpha-12-24-g7ad29e2-win32-vs100-devkit/lib/Release/Half.lib")
#	if USE_OPENEXR_CPP_INTERFACE
#		pragma comment( lib, "../../../dependence/appleseed/appleseed-1.1.0-alpha-12-24-g7ad29e2-win32-vs100-devkit/lib/Release/Iex.lib")
#		pragma comment( lib, "../../../dependence/appleseed/appleseed-1.1.0-alpha-12-24-g7ad29e2-win32-vs100-devkit/lib/Release/IlmImf.lib")
#		pragma comment( lib, "../../../dependence/appleseed/appleseed-1.1.0-alpha-12-24-g7ad29e2-win32-vs100-devkit/lib/Release/IlmThread.lib")
#		pragma comment( lib, "../../../dependence/appleseed/appleseed-1.1.0-alpha-12-24-g7ad29e2-win32-vs100-devkit/lib/Release/Imath.lib")
#	endif
#endif
	
#endif//_USE_APPLESEED_