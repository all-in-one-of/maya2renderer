#pragma once
#include "prerequest_as.h"

namespace renderer{
	class Assembly;
}

namespace appleseed
{
	class Renderer;
	//
	enum EnvironmentModel
	{
		generic_environment,
		EnvironmentModelCOUNT,
	};
	enum EnvironmentEDFModel
	{
		constant_environment_edf,
		constant_hemisphere_environment_edf,
		gradient_environment_edf,
		latlong_map_environment_edf,
		mirrorball_map_environment_edf,
		EnvironmentEDFModelCOUNT,
	};
	enum EnvironmentShaderModel
	{
		edf_environment_shader,
		EnvironmentShaderModelCOUNT,
	};
	//
	class MaterialFactory3
	{
	public:
		MaterialFactory3();
		~MaterialFactory3();//this class trends not to be inherited.
		
		void begin(const char* node);
		void end();

		void createEnvironment(const std::string &modelname);
		void createEnvironmentEDF(const std::string &modelname);
		void createEnvironmentShader(const std::string &modelname);

		void createEnvironment(EnvironmentModel model);
		void createEnvironmentEDF(EnvironmentEDFModel model);
		void createEnvironmentShader(EnvironmentShaderModel model);

	protected:
		MaterialFactory3(const MaterialFactory3& );
		MaterialFactory3& operator=(const MaterialFactory3&);



		void createEnvironment_generic();

		void createEnvironmentEDF_constant();
		void createEnvironmentEDF_constant_hemisphere();
		void createEnvironmentEDF_gradient();
		void createEnvironmentEDF_latlong_map();
		void createEnvironmentEDF_mirrorball_map();

		void createEnvironmentShader_edf();

		void visitEnvBall(const char* node);
		void visitEnvSphere(const char* node);
		void visitFile(const char* node);

	private:
		Renderer* m_renderer;
		asr::Project *m_project;
		asr::ParamArray material_params;
		std::string m_nodename;

		std::string m_env_model;
		std::string m_env_edf_model;
		std::string m_env_shader_model;
		std::string m_env_edf_name;//for env shader
	};
}//namespace appleseed