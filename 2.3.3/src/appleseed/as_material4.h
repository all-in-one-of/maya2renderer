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
	class Helper4
	{
	public:
		Helper4(const char* nodename);
		~Helper4();

		//BSDF
		void beginBSDF(const std::string& bsdf_model);
		void endBSDF();
		//\p model		lambertian_brdf, diffuse_edf, 
		//\p asName		reflectance
		//\p asTypes	color|texture_instance, scalar, 
		void addVariableBSDF(const std::string& param_name_as, const std::string& entity_type_as, const std::string& param_name_maya );
		//EDF
		void beginEDF(const std::string& edf_model);
		void endEDF();
		void addVariableEDF(const std::string& param_name_as, const std::string& entity_type_as, const std::string& param_name_maya );
		bool hasEDF(){ return !m_edf_params.empty(); }
		// Surface Shader
		void beginSS(const std::string& ss_model);
		void endSS();
		void addVariableSS(const std::string& param_name_as, const std::string& entity_type_as, const std::string& param_name_maya );


	protected:
		bool isType(const std::string& type, const std::string& entity_types)const;

		Renderer* m_renderer;
		asr::Assembly* m_assembly;

		const std::string m_nodename; 

		asr::ParamArray m_bsdf_params;
		asr::ParamArray m_edf_params;
		asr::ParamArray m_ss_params;//surface shader params

		std::string m_bsdf_model;
		std::string m_edf_model;
		std::string m_ss_model;//surface shader model
	};
}//namespace appleseed


#endif//_USE_APPLESEED_