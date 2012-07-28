#include "as_material3.h"

#include "../common/prerequest_maya.h"
#include "as_renderer.h"
#include "../renderermgr.h"
#include "../shadergraph/shadermgr.h"

namespace appleseed
{
	//////////////////////////////////////////////////////////////////////////
	MaterialFactory3::MaterialFactory3()
		:m_renderer(nullptr),
		m_project(nullptr)
	{
		m_renderer = dynamic_cast<appleseed::Renderer*>( liquid::RendererMgr::getInstancePtr()->getRenderer() );
		assert(m_renderer != NULL );
	}
	MaterialFactory3::~MaterialFactory3()
	{

	}
	void MaterialFactory3::begin(const char* node)
	{
		CM_TRACE_FUNC("begin("<<node<<")");

		//m_assembly = m_renderer->getAssembly().get();
		//assert(m_assembly != nullptr);
		m_project = m_renderer->getProject().get();
		assert(m_project != nullptr);

		m_nodename = node;
	}
	void MaterialFactory3::end()
	{
		m_project->get_scene()->set_environment(
			asr::EnvironmentFactory::create( m_nodename.c_str(),	material_params )
		);
	}
	//////////////////////////////////////////////////////////////////////////
	void MaterialFactory3::createEnvironment(const std::string &modelname)
	{
		m_env_model = modelname;

		if("generic_environment"==modelname){
			createEnvironment(generic_environment);
		}else{
			liquidMessage2(messageError, "Environment type %s is unknown.", modelname.c_str());
		}
	}
	void MaterialFactory3::createEnvironmentEDF(const std::string &modelname)
	{
		m_env_edf_model = modelname;

		if("constant_environment_edf"==modelname){
			createEnvironmentEDF(constant_environment_edf);
		}else if("constant_hemisphere_environment_edf"==modelname){
			createEnvironmentEDF(constant_hemisphere_environment_edf);
		}else if("gradient_environment_edf"==modelname){
			createEnvironmentEDF(gradient_environment_edf);
		}else if("latlong_map_environment_edf"==modelname){
			createEnvironmentEDF(latlong_map_environment_edf);
		}else if("mirrorball_map_environment_edf"==modelname){
			createEnvironmentEDF(mirrorball_map_environment_edf);
		}else{
			liquidMessage2(messageError, "EnvironmentEDF type %s is unknown.", modelname.c_str());
		}
	}
	void MaterialFactory3::createEnvironmentShader(const std::string &modelname)
	{
		m_env_shader_model = modelname;

		if("edf_environment_shader"==modelname){
			createEnvironmentShader(edf_environment_shader);
		}else{
			liquidMessage2(messageError, "EnvironmentShader type %s is unknown.", modelname.c_str());
		}
	}
	//////////////////////////////////////////////////////////////////////////
	void MaterialFactory3::createEnvironment(EnvironmentModel model)
	{
		switch(model)
		{
		case generic_environment:
			m_env_model = "generic_environment";
			createEnvironment_generic();
			break;
		default:
			liquidMessage2(messageError, "Environment type %d is unknown.", model);
		}
	}
	void MaterialFactory3::createEnvironmentEDF(EnvironmentEDFModel model)
	{
		switch(model)
		{
		case constant_environment_edf:				
			m_env_edf_model	 = "constant_environment_edf";
			createEnvironmentEDF_constant();
			break;
		case constant_hemisphere_environment_edf:	
			m_env_edf_model	 = "constant_hemisphere_environment_edf";
			createEnvironmentEDF_constant_hemisphere();	
			break;
		case gradient_environment_edf:				
			m_env_edf_model	 = "gradient_environment_edf";
			createEnvironmentEDF_gradient();	
			break;
		case latlong_map_environment_edf:			
			m_env_edf_model	 = "latlong_map_environment_edf";
			createEnvironmentEDF_latlong_map();	
			break;
		case mirrorball_map_environment_edf:		
			m_env_edf_model	 = "mirrorball_map_environment_edf";
			createEnvironmentEDF_mirrorball_map();	
			break;
		default:
			liquidMessage2(messageError, "EnvironmentEDF type %d is unknown.", model);
		}
	}
	void MaterialFactory3::createEnvironmentShader(EnvironmentShaderModel model)
	{
		std::string ret;
		switch(model)
		{
		case edf_environment_shader:
			m_env_shader_model = "edf_environment_shader"; 	
			createEnvironmentShader_edf();
			break;
		default:
			liquidMessage2(messageError, "EnvironmentShader type %d is unknown.", model);
			ret = "";
		}
	}
	//////////////////////////////////////////////////////////////////////////
	void MaterialFactory3::createEnvironment_generic()
	{
		CM_TRACE_FUNC("MaterialFactory3::createEnvironment_generic()");

//		std::string env_name(m_nodename+"_"+m_env_model);//<nodename>_generic_environment
//
// 		asr::ParamArray env_params;
// 		{
// 			std::string param_node;
// 			const std::string param("env_model");
// 			const std::string plugName(m_env_model+"_"+param);//generic_environment_env_model
// 
// 			MString fullPlugName((m_nodename+"."+plugName).c_str());//<nodename>.generic_environment_env_model
// 			int connected = liquidmaya::ShaderMgr::getSingletonPtr()->convertibleConnection(fullPlugName.asChar());
// 			if(connected != 1)
// 			{
// 				param_node = m_nodename+"_"+plugName;//<nodename>_generic_environment_env_model
// 				//
// 				MDoubleArray val; 
// 				val.setLength(3);
// 				IfMErrorWarn(MGlobal::executeCommand("getAttr (\""+fullPlugName+"\")", val));
// 
// 				float color[] = { val[0], val[1], val[2] };
// 				m_project->get_scene()->colors().insert(
// 					asr::ColorEntityFactory::create(
// 					param_node.c_str(),
// 					asr::ParamArray().insert("color_space", "srgb"), asr::ColorValueArray(3, color)
// 					)
// 					);
// 			}else{//the color plug is linked in.
// 				MStringArray srcPlug;
// 				IfMErrorWarn(MGlobal::executeCommand("listConnections -source true -plugs true \""+fullPlugName+"\"", srcPlug));
// 				assert(srcPlug.length()==1);
// 				MStringArray src;
// 				srcPlug[0].split('.',src);
// 				MString srcNode(src[0]);
// 				//
// 				param_node = getTextureInstanceName(srcNode.asChar());
// 			}
// 			//
// 			env_params.insert(param.c_str(), param_node.c_str());
// 		}
		//

	}

	void MaterialFactory3::createEnvironmentEDF_constant()
	{
		CM_TRACE_FUNC("MaterialFactory3::createEnvironmentEDF_constant()");

		std::string env_edf_name(m_nodename+"_"+m_env_edf_model);//<nodename>_constant_environment_edf

		asr::ParamArray env_edf_params;
		{
			std::string param_node;
			const std::string param("exitance");
			const std::string plugName(m_env_edf_model+"_"+param);//constant_environment_edf_exitance

			MString fullPlugName((m_nodename+"."+plugName).c_str());//<nodename>.constant_environment_edf_exitance
			int connected = liquidmaya::ShaderMgr::getSingletonPtr()->convertibleConnection(fullPlugName.asChar());
			if(connected != 1)
			{
				param_node = m_nodename+"_"+plugName;//<nodename>_constant_environment_edf_exitance
				//reflectance color
				MDoubleArray val; 
				val.setLength(3);
				IfMErrorWarn(MGlobal::executeCommand("getAttr (\""+fullPlugName+"\")", val));

				float color[] = { val[0], val[1], val[2] };
				m_project->get_scene()->colors().insert(
					asr::ColorEntityFactory::create(
					param_node.c_str(),
					asr::ParamArray().insert("color_space", "srgb"), asr::ColorValueArray(3, color)
					)
					);
			}else{//the color plug is linked in.
				MStringArray srcPlug;
				IfMErrorWarn(MGlobal::executeCommand("listConnections -source true -plugs true \""+fullPlugName+"\"", srcPlug));
				assert(srcPlug.length()==1);
				MStringArray src;
				srcPlug[0].split('.',src);
				MString srcNode(src[0]);
				//
				param_node = getTextureInstanceName(srcNode.asChar());
			}
			//
			env_edf_params.insert(param.c_str(), param_node.c_str());
		}
		//
		m_project->get_scene()->environment_edfs().insert(
			asr::ConstantEnvironmentEDFFactory().create(
			env_edf_name.c_str(),
			env_edf_params
			)
		);
		material_params.insert( "environment_edf", env_edf_name.c_str() );
		m_env_edf_name = env_edf_name;
	}

	void MaterialFactory3::createEnvironmentEDF_constant_hemisphere()
	{
		CM_TRACE_FUNC("MaterialFactory3::createEnvironmentEDF_constant_hemisphere()");
		liquidMessage2( messageError, "the type of [%s] is not implemented yet.", m_env_model.c_str() );

	}

	void MaterialFactory3::createEnvironmentEDF_gradient()
	{
		CM_TRACE_FUNC("MaterialFactory3::createEnvironmentEDF_gradient()");
		liquidMessage2( messageError, "the type of [%s] is not implemented yet.", m_env_model.c_str() );

	}

	void MaterialFactory3::createEnvironmentEDF_latlong_map()
	{
		CM_TRACE_FUNC("MaterialFactory3::createEnvironmentEDF_latlong_map()");
		liquidMessage2( messageError, "the type of [%s] is not implemented yet.", m_env_model.c_str() );

	}

	void MaterialFactory3::createEnvironmentEDF_mirrorball_map()
	{
		CM_TRACE_FUNC("MaterialFactory3::createEnvironmentEDF_mirrorball_map()");
		liquidMessage2( messageError, "the type of [%s] is not implemented yet.", m_env_model.c_str() );

	}

	//
	void MaterialFactory3::createEnvironmentShader_edf()
	{
		CM_TRACE_FUNC("MaterialFactory3::createEnvironmentShader_edf()");

		std::string env_shader_name(m_nodename+"_"+m_env_shader_model);//<nodename>_edf_environment_shader

		asr::ParamArray env_shader_params;
		{
			//std::string param_node;
			const std::string param("environment_edf");
//			const std::string plugName(m_env_edf_model+"_"+param);//edf_environment_shader_environment_edf
//
//			MString fullPlugName((m_nodename+"."+plugName).c_str());//<nodename>.edf_environment_shader_environment_edf
// 			int connected = liquidmaya::ShaderMgr::getSingletonPtr()->convertibleConnection(fullPlugName.asChar());
// 			if(connected != 1)
// 			{
// 				param_node = m_nodename+"_"+plugName;//<nodename>_edf_environment_shader_environment_edf
// 				//
// 				MDoubleArray val; 
// 				val.setLength(3);
// 				IfMErrorWarn(MGlobal::executeCommand("getAttr (\""+fullPlugName+"\")", val));
// 
// 				float color[] = { val[0], val[1], val[2] };
// 				m_project->get_scene()->colors().insert(
// 					asr::ColorEntityFactory::create(
// 					param_node.c_str(),
// 					asr::ParamArray().insert("color_space", "srgb"), asr::ColorValueArray(3, color)
// 					)
// 					);
// 			}else{//the color plug is linked in.
// 				MStringArray srcPlug;
// 				IfMErrorWarn(MGlobal::executeCommand("listConnections -source true -plugs true \""+fullPlugName+"\"", srcPlug));
// 				assert(srcPlug.length()==1);
// 				MStringArray src;
// 				srcPlug[0].split('.',src);
// 				MString srcNode(src[0]);
// 				//
// 				param_node = getTextureInstanceName(srcNode.asChar());
// 			}
			//
			env_shader_params.insert(param.c_str(), m_env_edf_name.c_str());
		}
		//
		m_project->get_scene()->environment_shaders().insert(
			asr::EDFEnvironmentShaderFactory().create(
			env_shader_name.c_str(),
			env_shader_params
			)
		);
		material_params.insert( "environment_shader", env_shader_name.c_str() );
	}


}//namespace appleseed