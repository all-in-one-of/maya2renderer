#ifndef _PREREQUEST_STD_H_
#define _PREREQUEST_STD_H_

#include <sys/types.h>

#ifndef _WIN32
	#include <sys/time.h>
	#include <sys/stat.h>
	// Dynamic Object Headers
	#include <dlfcn.h>
#endif

#ifdef _WIN32
#pragma warning(disable:4786)
#endif

#ifdef _WIN32
	#include <process.h>
	#include <io.h>
	#include <direct.h>
	#include <sys/types.h>
	#include <sys/stat.h>
#else
	#include <unistd.h>
	#include <stdlib.h>
	#include <pwd.h>
#endif

#include <stddef.h>
#include <math.h>
#include <float.h>

#include <cassert>
#include <ctime>

#include <iostream>
#include <algorithm>
#include <vector>
#include <stack>
#include <set>
#include <map>
#include <string>
#include <sstream>
#include <fstream>
#include <iomanip>

#include <boost/bind.hpp>
#include <boost/smart_ptr/shared_ptr.hpp>
#include <boost/shared_array.hpp>
#include <boost/scoped_array.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/tokenizer.hpp>

#include <boost/algorithm/string/case_conv.hpp>
#include <boost/algorithm/string/replace.hpp>
#include <boost/algorithm/string/trim.hpp>
#include <boost/algorithm/string.hpp>

#include <boost/lambda/lambda.hpp>
#include <boost/lambda/bind.hpp>
#include <boost/lambda/algorithm.hpp>

#include <boost/format.hpp>

#include <boost/filesystem.hpp>
#endif//_PREREQUEST_STD_H_
