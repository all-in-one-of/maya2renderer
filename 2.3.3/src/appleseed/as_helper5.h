#include <liqConfig.h>
#ifdef _USE_APPLESEED_

#pragma once
#include "prerequest_as.h"
#include "as_material.h"

namespace renderer{
	class Assembly;
}

namespace appleseed
{
	class Renderer;
	//
	//////////////////////////////////////////////////////////////////////////
	class Helper5
	{
	public:
		Helper5();
		~Helper5();

		void begin(const char* nodename);
		void end();

		//\p entity_type_as	   color|texture_instance, scalar, 
		//\p param_name_maya   ambientColor,
		std::string addChannel(const std::string& param_name_maya, const std::string& param_type_as);
		std::string fullTransparentColor();

	protected:
		bool isType(const std::string& type, const std::string& entity_types)const;

		asr::Assembly* m_assembly;
		std::string m_nodename; 
	};
}//namespace appleseed


#endif//_USE_APPLESEED_