#include "as_material2.h"

#include "../common/prerequest_maya.h"
#include "as_renderer.h"
#include "../renderermgr.h"
#include "../shadergraph/shadermgr.h"

namespace appleseed
{
	//////////////////////////////////////////////////////////////////////////
	MaterialFactory2::MaterialFactory2()
		:m_renderer(nullptr),
		m_assembly(nullptr)
	{
		m_renderer = dynamic_cast<appleseed::Renderer*>( liquid::RendererMgr::getInstancePtr()->getRenderer() );
		assert(m_renderer != NULL );
	}
	MaterialFactory2::~MaterialFactory2()
	{

	}
	void MaterialFactory2::begin(const char* node)
	{
		CM_TRACE_FUNC("begin("<<node<<")");

		m_assembly = m_renderer->getAssembly().get();
		assert(m_assembly != nullptr);

		m_nodename = node;
	}
	void MaterialFactory2::end()
	{
		m_assembly->materials().insert(
			asr::MaterialFactory::create( m_nodename.c_str(), material_params )
		);
	}
	//////////////////////////////////////////////////////////////////////////
	void MaterialFactory2::createBSDF(const std::string &modelname)
	{
		m_bsdf_model = modelname;

		if("ashikhmin_brdf"==modelname){
			createBSDF(ashikhmin_brdf);
		}else if("bsdf_mix"==modelname){
			createBSDF(bsdf_mix);
		}else if("kelemen_brdf"==modelname){
			createBSDF(kelemen_brdf);
		}else if("lambertian_brdf"==modelname){
			createBSDF(lambertian_brdf);
		}else if("null_bsdf"==modelname){
			createBSDF(null_bsdf);
		}else if("specular_brdf"==modelname){
			createBSDF(specular_brdf);
		}else if("specular_btdf"==modelname){
			createBSDF(specular_btdf);
		}else{
			liquidMessage2(messageError, "BSDF type %s is unknown.", modelname.c_str());
		}
	}
	void MaterialFactory2::createEDF(const std::string &modelname)
	{
		m_edf_model = modelname;

		if("diffuse_edf"==modelname){
			createEDF(diffuse_edf);
		}else{
			liquidMessage2(messageError, "EDF type %s is unknown.", modelname.c_str());
		}
	}
	void MaterialFactory2::createSurfaceShader(const std::string &modelname)
	{
		m_surface_shader_model = modelname;

		if("ao_surface_shader"==modelname){
			createSurfaceShader(ao_surface_shader);
		}else if("constant_surface_shader"==modelname){
			createSurfaceShader(constant_surface_shader);
		}else if("diagnostic_surface_shader"==modelname){
			createSurfaceShader(diagnostic_surface_shader);
		}else if("fast_sss_surface_shader"==modelname){
			createSurfaceShader(fast_sss_surface_shader);
		}else if("physical_surface_shader"==modelname){
			createSurfaceShader(physical_surface_shader);
		}else if("smoke_surface_shader"==modelname){
			createSurfaceShader(smoke_surface_shader);
		}else if("voxel_ao_surface_shader"==modelname){
			createSurfaceShader(voxel_ao_surface_shader);
		}else{
			liquidMessage2(messageError, "SurfaceShader type %s is unknown.", modelname.c_str());
		}
	}
	//////////////////////////////////////////////////////////////////////////
	void MaterialFactory2::createBSDF(BSDF_Model model)
	{
		switch(model)
		{
		case ashikhmin_brdf:	createBSDF_ashikhmin_brdf();break;
		case bsdf_mix:			createBSDF_bsdf_mix();		break;
		case kelemen_brdf:		createBSDF_kelemen_brdf();	break;
		case lambertian_brdf:	createBSDF_lambertian_brdf(); break;
		case null_bsdf:			createBSDF_null_bsdf();		break;
		case specular_brdf:		createBSDF_specular_brdf();	break;
		case specular_btdf:		createBSDF_specular_btdf();	break;
		default:
			liquidMessage2(messageError, "BSDF type %d is unknown.", model);
		}
	}
	void MaterialFactory2::createEDF(EDF_Model model)
	{
		switch(model)
		{
		case diffuse_edf:	createEDF_diffuse_edf();	break;
		default:
			liquidMessage2(messageError, "EDF type %d is unknown.", model);
		}
	}
	void MaterialFactory2::createSurfaceShader(SurfaceShader_Model model)
	{
		std::string ret;
		switch(model)
		{
		case ao_surface_shader:			createSurfaceShader_ao();		break;
		case constant_surface_shader:	createSurfaceShader_constant(); break;
		case diagnostic_surface_shader:	createSurfaceShader_diagnostic();break;
		case fast_sss_surface_shader:	createSurfaceShader_fast_sss();	break;
		case physical_surface_shader:	createSurfaceShader_physical();	break;
		case smoke_surface_shader:		createSurfaceShader_smoke();	break;
		case voxel_ao_surface_shader:	createSurfaceShader_voxel_ao();	break;
		default:
			liquidMessage2(messageError, "SurfaceShader type %d is unknown.", model);
			ret = "";
		}
	}
	//////////////////////////////////////////////////////////////////////////
	void MaterialFactory2::createBSDF_ashikhmin_brdf()
	{
		CM_TRACE_FUNC("MaterialFactory2::createBSDF_ashikhmin_brdf()");
		liquidMessage2( messageError, "the type of node [%s] is not implemented yet.", m_nodename.c_str() );
	}

	void MaterialFactory2::createBSDF_bsdf_mix()
	{
		CM_TRACE_FUNC("MaterialFactory2::createBSDF_bsdf_mix()");
		liquidMessage2( messageError, "the type of node [%s] is not implemented yet.", m_nodename.c_str() );

	}

	void MaterialFactory2::createBSDF_kelemen_brdf()
	{
		CM_TRACE_FUNC("MaterialFactory2::createBSDF_kelemen_brdf()");
		liquidMessage2( messageError, "the type of node [%s] is not implemented yet.", m_nodename.c_str() );

	}

	void MaterialFactory2::createBSDF_lambertian_brdf()
	{
		CM_TRACE_FUNC("MaterialFactory2::createBSDF_lambertian_brdf()");

		std::string bsdf_name(m_nodename+"_"+m_bsdf_model);//<nodename>_lambert_brdf

		asr::ParamArray bsdf_params;
		{
			std::string param_node;
			const std::string param("reflectance");
			const std::string plugName(m_bsdf_model+"_"+param);//lambert_brdf_reflectance

			MString fullPlugName((m_nodename+"."+plugName).c_str());//<nodename>.lambert_brdf_reflectance
			int connected = liquidmaya::ShaderMgr::getSingletonPtr()->convertibleConnection(fullPlugName.asChar());
			if(connected != 1)
			{
				param_node = m_nodename+"_"+plugName;//<nodename>_lambert_brdf_reflectance
				//reflectance color
				MDoubleArray val; 
				val.setLength(3);
				IfMErrorWarn(MGlobal::executeCommand("getAttr (\""+fullPlugName+"\")", val));

				float color[] = { val[0], val[1], val[2] };
				m_assembly->colors().insert(
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
			bsdf_params.insert(param.c_str(), param_node.c_str());
		}
		//
		m_assembly->bsdfs().insert(
			asr::LambertianBRDFFactory().create(
				bsdf_name.c_str(),
				bsdf_params
			)
		);
		material_params.insert( "bsdf", bsdf_name.c_str() );
	}

	void MaterialFactory2::createBSDF_null_bsdf()
	{
		CM_TRACE_FUNC("MaterialFactory2::createBSDF_null_bsdf()");
		liquidMessage2( messageError, "the type of node [%s] is not implemented yet.", m_nodename.c_str() );

	}

	void MaterialFactory2::createBSDF_specular_brdf()
	{
		CM_TRACE_FUNC("MaterialFactory2::createBSDF_specular_brdf()");

		std::string bsdf_name(m_nodename+"_"+m_bsdf_model);//<nodename>_specular_brdf

		asr::ParamArray bsdf_params;
		{
			std::string param_node;
			const std::string param("reflectance");
			const std::string plugName(m_bsdf_model+"_"+param);//specular_brdf_reflectance

			MString fullPlugName((m_nodename+"."+plugName).c_str());//<nodename>.specular_brdf_reflectance
			int connected = liquidmaya::ShaderMgr::getSingletonPtr()->convertibleConnection(fullPlugName.asChar());
			if(connected != 1)
			{
				param_node = m_nodename+"_"+plugName;//<nodename>_specular_brdf_reflectance
				//reflectance color
				MDoubleArray val; 
				val.setLength(3);
				IfMErrorWarn(MGlobal::executeCommand("getAttr (\""+fullPlugName+"\")", val));

				float color[] = { val[0], val[1], val[2] };
				m_assembly->colors().insert(
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
			bsdf_params.insert(param.c_str(), param_node.c_str());
		}
		//
		m_assembly->bsdfs().insert(
			asr::SpecularBRDFFactory().create(
			bsdf_name.c_str(),
			bsdf_params
			)
			);
		material_params.insert( "bsdf", bsdf_name.c_str() );
	}

	void MaterialFactory2::createBSDF_specular_btdf()
	{
		CM_TRACE_FUNC("MaterialFactory2::createBSDF_specular_btdf()");
		liquidMessage2( messageError, "the type of node [%s] is not implemented yet.", m_nodename.c_str() );

	}

	//
	void MaterialFactory2::createEDF_diffuse_edf()
	{
		CM_TRACE_FUNC("MaterialFactory2::createEDF_diffuse_edf()");

		std::string edf_name(m_nodename+"_"+m_edf_model);//<nodename>_diffuse_edf
		//
		asr::ParamArray edf_params;
		{
			std::string param_node;
			const std::string param("exitance");
			const std::string plugName(m_edf_model+"_"+param);//diffuse_edf_exitance
			
			MString fullPlugName((m_nodename+"."+plugName).c_str());//<nodename>.diffuse_edf_exitance
			//
			MDoubleArray v; 
			v.setLength(3);
			IfMErrorWarn(MGlobal::executeCommand("getAttr (\""+fullPlugName+"\")", v));

			if( !isZero(v[0], v[1], v[2]) )
			{
				param_node = m_nodename+"_"+plugName;//<nodename>_diffuse_edf_exitance

				float val[] = { v[0], v[1], v[2] };
				m_assembly->colors().insert(
					asr::ColorEntityFactory::create(
					param_node.c_str(),
					asr::ParamArray().insert("color_space", "srgb"), asr::ColorValueArray(3, val)
					)
					);
				//
				edf_params.insert(param.c_str(), param_node.c_str());
			}
		}
		//
		if( !edf_params.empty() )
		{
			m_assembly->edfs().insert(
				asr::DiffuseEDFFactory().create(
				edf_name.c_str(),
				edf_params
				)
			);
			//
			material_params.insert("edf", edf_name.c_str());
		}

	}

	//
	void MaterialFactory2::createSurfaceShader_ao()
	{
		CM_TRACE_FUNC("MaterialFactory2::createSurfaceShader_ao()");
		liquidMessage2( messageError, "the type of node [%s] is not implemented yet.", m_nodename.c_str() );

	}

	void MaterialFactory2::createSurfaceShader_constant()
	{
		CM_TRACE_FUNC("MaterialFactory2::createSurfaceShader_constant()");

		std::string surfaceshader_name(m_nodename+"_"+m_surface_shader_model);//<nodename>_constant_surface_shader

		asr::ParamArray surfaceshader_params;
		{
			std::string param_node;
			const std::string param("color");
			const std::string plugName(m_surface_shader_model+"_"+param);//constant_surface_shader_color

			MString fullPlugName((m_nodename+"."+plugName).c_str());//<nodename>.constant_surface_shader_color
			int connected = liquidmaya::ShaderMgr::getSingletonPtr()->convertibleConnection(fullPlugName.asChar());
			if(connected != 1)
			{
				param_node = m_nodename+"_"+plugName;//<nodename>_constant_surface_shader_color

				MDoubleArray val; 
				val.setLength(3);
				IfMErrorWarn(MGlobal::executeCommand("getAttr (\""+fullPlugName+"\")", val));

				float color[] = { val[0], val[1], val[2] };
				m_assembly->colors().insert(
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
			surfaceshader_params.insert(param.c_str(), param_node.c_str());
		}
		//
		m_assembly->surface_shaders().insert(
			asr::ConstantSurfaceShaderFactory().create(
			surfaceshader_name.c_str(),
			surfaceshader_params
			)
			);
		material_params.insert( "surface_shader", surfaceshader_name.c_str() );
	}

	void MaterialFactory2::createSurfaceShader_diagnostic()
	{
		CM_TRACE_FUNC("MaterialFactory2::createSurfaceShader_diagnostic()");
		liquidMessage2( messageError, "the type of node [%s] is not implemented yet.", m_nodename.c_str() );

	}

	void MaterialFactory2::createSurfaceShader_fast_sss()
	{
		CM_TRACE_FUNC("MaterialFactory2::createSurfaceShader_fast_sss()");
		liquidMessage2( messageError, "the type of node [%s] is not implemented yet.", m_nodename.c_str() );

	}

	void MaterialFactory2::createSurfaceShader_physical()
	{
		CM_TRACE_FUNC("MaterialFactory2::createSurfaceShader_physical()");

		std::string surfaceshader_name(m_nodename+"_"+m_surface_shader_model);

		m_assembly->surface_shaders().insert(
			asr::PhysicalSurfaceShaderFactory().create(
				surfaceshader_name.c_str(),
				asr::ParamArray()
			)
		);
		material_params.insert( "surface_shader", surfaceshader_name.c_str() );

	}

	void MaterialFactory2::createSurfaceShader_smoke()
	{
		CM_TRACE_FUNC("MaterialFactory2::createSurfaceShader_smoke()");
		liquidMessage2( messageError, "the type of node [%s] is not implemented yet.", m_nodename.c_str() );

	}

	void MaterialFactory2::createSurfaceShader_voxel_ao()
	{
		CM_TRACE_FUNC("MaterialFactory2::createSurfaceShader_voxel_ao()");
		liquidMessage2( messageError, "the type of node [%s] is not implemented yet.", m_nodename.c_str() );

	}


}//namespace appleseed