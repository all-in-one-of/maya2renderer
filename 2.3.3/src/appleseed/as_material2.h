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
	class MaterialFactory2
	{
	public:
		MaterialFactory2();
		~MaterialFactory2();//this class trends not to be inherited.
		
		void begin(const char* node);
		void end();

		void createBSDF(const std::string &modelname);
		void createEDF(const std::string &modelname);
		void createSurfaceShader(const std::string &modelname);

	protected:
		MaterialFactory2(const MaterialFactory2& );
		MaterialFactory2& operator=(const MaterialFactory2&);

		void createBSDF(BSDF_Model model);
		void createEDF(EDF_Model model);
		void createSurfaceShader(SurfaceShader_Model model);

		void createBSDF_ashikhmin_brdf();
		void createBSDF_bsdf_mix();
		void createBSDF_kelemen_brdf();
		void createBSDF_lambertian_brdf();
		void createBSDF_null_bsdf();
		void createBSDF_specular_brdf();
		void createBSDF_specular_btdf();

		void createEDF_diffuse_edf();

		void createSurfaceShader_ao();
		void createSurfaceShader_constant();
		void createSurfaceShader_diagnostic();
		void createSurfaceShader_fast_sss();
		void createSurfaceShader_physical();
		void createSurfaceShader_smoke();
		void createSurfaceShader_voxel_ao();

		void visitFile(const char* node);

	private:
		Renderer* m_renderer;
		asr::Assembly* m_assembly;
		asr::ParamArray material_params;
		std::string m_nodename;

		std::string m_bsdf_model;
		std::string m_edf_model;
		std::string m_surface_shader_model;
	};
	//////////////////////////////////////////////////////////////////////////
	class Helper2
	{
	public:
		Helper2(const char* nodename, asr::Assembly* assembly);
		~Helper2();

		//BSDF
		void beginBSDF(const std::string& bsdf_model);
		void endBSDF();
		//\p model		lambertian_brdf, diffuse_edf, 
		//\p asName		reflectance
		//\p asTypes	color|texture_instance, scalar, 
		void addVariableBSDF(const std::string& param_name, const std::string& entity_types );
		//EDF
		void beginEDF(const std::string& edf_model);
		void endEDF();
		void addVariableEDF(const std::string& param_name, const std::string& entity_types );
		bool hasEDF(){ return !m_edf_params.empty(); }
		// Surface Shader
		void beginSS(const std::string& ss_model);
		void endSS();
		void addVariableSS(const std::string& param_name, const std::string& entity_types );


	protected:
		bool isType(const std::string& type, const std::string& entity_types)const;
		void visitFile(const char* node);

		const std::string m_nodename; 
		//bsdf
		asr::ParamArray m_bsdf_params;
		asr::ParamArray m_edf_params;
		asr::ParamArray m_ss_params;//surface shader params

		std::string m_bsdf_model;
		std::string m_edf_model;
		std::string m_ss_model;//surface shader model

		asr::Assembly* m_assembly;
	};
}//namespace appleseed