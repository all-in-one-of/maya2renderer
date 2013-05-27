#pragma once
#include "prerequest_as.h"

namespace renderer{
	class Assembly;
}

namespace appleseed
{
	class Renderer;
	//
	enum BSDF_Model
	{
		ashikhmin_brdf,
		bsdf_mix,
		kelemen_brdf,
		lambertian_brdf,
		null_bsdf,
		specular_brdf,
		specular_btdf,
		BSDF_Model_COUNT,
	};
	enum EDF_Model
	{
		diffuse_edf,
		EDF_Model_COUNT,
	};
	enum SurfaceShader_Model
	{
		ao_surface_shader,
		constant_surface_shader,
		diagnostic_surface_shader,
		fast_sss_surface_shader,
		physical_surface_shader,
		smoke_surface_shader,
		voxel_ao_surface_shader,
		SurfaceShader_Model_COUNT,
	};
	//
	class MaterialFactory
	{
	public:
		MaterialFactory();
		~MaterialFactory();//this class trends not to be inherited.
		
		void begin(const char* node);
		void end();

		void createBSDF(const std::string &modelname);
		void createEDF(const std::string &modelname);
		void createSurfaceShader(const std::string &modelname);

		void addSurfaceShader(const std::string& nodeName);//e.g. nodeName is "mib_amb_occlusion1"

	protected:
		MaterialFactory(const MaterialFactory& );
		MaterialFactory& operator=(const MaterialFactory&);

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

	private:
		Renderer* m_renderer;
		asr::Assembly* m_assembly;
		asr::ParamArray material_params;
		std::string m_nodename;
	};
}//namespace appleseed