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
		if(m_assembly->materials().get_by_name(m_nodename.c_str()) == nullptr)
		{
			m_assembly->materials().insert(
				asr::MaterialFactory::create( m_nodename.c_str(), material_params )
				);
		}
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
		case ashikhmin_brdf:	
			m_bsdf_model = "ashikhmin_brdf";
			createBSDF_ashikhmin_brdf(); break;
		case bsdf_mix:	
			m_bsdf_model = "bsdf_mix";
			createBSDF_bsdf_mix();		break;
		case kelemen_brdf:	
			m_bsdf_model = "kelemen_brdf";
			createBSDF_kelemen_brdf();	break;
		case lambertian_brdf:	
			m_bsdf_model = "lambertian_brdf";
			createBSDF_lambertian_brdf(); break;
		case null_bsdf:	
			m_bsdf_model = "null_bsdf";
			createBSDF_null_bsdf();		break;
		case specular_brdf:	
			m_bsdf_model = "specular_brdf";
			createBSDF_specular_brdf();	break;
		case specular_btdf:	
			m_bsdf_model = "specular_btdf";
			createBSDF_specular_btdf();	break;
		default:
			liquidMessage2(messageError, "BSDF type %d is unknown.", model);
		}
	}
	void MaterialFactory2::createEDF(EDF_Model model)
	{
		switch(model)
		{
		case diffuse_edf:	
			m_edf_model = "diffuse_edf";
			createEDF_diffuse_edf();	break;
		default:
			liquidMessage2(messageError, "EDF type %d is unknown.", model);
		}
	}
	void MaterialFactory2::createSurfaceShader(SurfaceShader_Model model)
	{
		std::string ret;
		switch(model)
		{
		case ao_surface_shader:	
			m_surface_shader_model = "ao_surface_shader";
			createSurfaceShader_ao();		break;
		case constant_surface_shader:	
			m_surface_shader_model = "constant_surface_shader";
			createSurfaceShader_constant(); break;
		case diagnostic_surface_shader:	
			m_surface_shader_model = "diagnostic_surface_shader";
			createSurfaceShader_diagnostic();break;
		case fast_sss_surface_shader:	
			m_surface_shader_model = "fast_sss_surface_shader";
			createSurfaceShader_fast_sss();	break;
		case physical_surface_shader:	
			m_surface_shader_model = "physical_surface_shader";
			createSurfaceShader_physical();	break;
		case smoke_surface_shader:		
			m_surface_shader_model = "smoke_surface_shader";
			createSurfaceShader_smoke();	break;
		case voxel_ao_surface_shader:	
			m_surface_shader_model = "voxel_ao_surface_shader";
			createSurfaceShader_voxel_ao();	break;
		default:
			liquidMessage2(messageError, "SurfaceShader type %d is unknown.", model);
			ret = "";
		}
	}
	//////////////////////////////////////////////////////////////////////////
	void MaterialFactory2::createBSDF_ashikhmin_brdf()
	{
		CM_TRACE_FUNC("MaterialFactory2::createBSDF_ashikhmin_brdf()");
		liquidMessage2( messageError, "the type of [%s] is not implemented yet.", m_bsdf_model.c_str() );
	}

	void MaterialFactory2::createBSDF_bsdf_mix()
	{
		CM_TRACE_FUNC("MaterialFactory2::createBSDF_bsdf_mix()");

		std::string bsdf_name(getBSDFName(m_nodename,m_bsdf_model));//<nodename>_bsdf_mix

		asr::ParamArray bsdf_params;
		{
			std::string param;

			{
				param = "bsdf0";//entity_types:bsdf
				std::string param_node;

				const std::string plugName(m_bsdf_model+"_"+param);//bsdf_mix_bsdf0

				MString fullPlugName((m_nodename+"."+plugName).c_str());//<nodename>.bsdf_mix_bsdf0
				int connected = liquidmaya::ShaderMgr::getSingletonPtr()->convertibleConnection(fullPlugName.asChar());
				if(connected != 1)
				{
					liquidMessage2(messageError, "[%s] is not linked in", fullPlugName.asChar());
				}else{//the color plug is linked in.
					param_node = m_nodename+"_"+plugName;//<nodename>_bsdf_mix_bsdf0
					//bsdf0 value
					MString srcBSDFModel; 
					IfMErrorWarn(MGlobal::executeCommand("getAttr (\""+fullPlugName+"\")", srcBSDFModel));

					MStringArray srcPlug;
					IfMErrorWarn(MGlobal::executeCommand("listConnections -source true -plugs true \""+fullPlugName+"\"", srcPlug));
					assert(srcPlug.length()==1);
					MStringArray src;
					srcPlug[0].split('.',src);
					MString srcNode(src[0]);

					//
					param_node = getBSDFName(srcNode.asChar(), srcBSDFModel.asChar());
				}
				//
				bsdf_params.insert(param.c_str(), param_node.c_str());
			}
			//////////////////////////////////////////////////////////////////////////
			{
				param = "bsdf1";//entity_types:bsdf
				std::string param_node;

				const std::string plugName(m_bsdf_model+"_"+param);//bsdf_mix_bsdf1

				MString fullPlugName((m_nodename+"."+plugName).c_str());//<nodename>.bsdf_mix_bsdf1
				int connected = liquidmaya::ShaderMgr::getSingletonPtr()->convertibleConnection(fullPlugName.asChar());
				if(connected != 1)
				{
					liquidMessage2(messageError, "[%s] is not linked in", fullPlugName.asChar());
				}else{//the color plug is linked in.
					param_node = m_nodename+"_"+plugName;//<nodename>_bsdf_mix_bsdf1
					//bsdf1 value
					MString srcBSDFModel; 
					IfMErrorWarn(MGlobal::executeCommand("getAttr (\""+fullPlugName+"\")", srcBSDFModel));

					MStringArray srcPlug;
					IfMErrorWarn(MGlobal::executeCommand("listConnections -source true -plugs true \""+fullPlugName+"\"", srcPlug));
					assert(srcPlug.length()==1);
					MStringArray src;
					srcPlug[0].split('.',src);
					MString srcNode(src[0]);

					//
					param_node = getBSDFName(srcNode.asChar(), srcBSDFModel.asChar());
				}
				//
				bsdf_params.insert(param.c_str(), param_node.c_str());
			}
			//////////////////////////////////////////////////////////////////////////
			{
				param = "weight0";//entity_types:(float)|texture_instance
				std::string param_node;

				const std::string plugName(m_bsdf_model+"_"+param);//bsdf_mix_weight0

				MString fullPlugName((m_nodename+"."+plugName).c_str());//<nodename>.bsdf_mix_weight0
				int connected = liquidmaya::ShaderMgr::getSingletonPtr()->convertibleConnection(fullPlugName.asChar());
				if(connected != 1)
				{
					double weight0; 
					IfMErrorWarn(MGlobal::executeCommand("getAttr (\""+fullPlugName+"\")", weight0));
					MString strWeight;
					strWeight.set(weight0);

					param_node = strWeight.asChar();
				}else{//the color plug is linked in.
					MStringArray srcPlug;
					IfMErrorWarn(MGlobal::executeCommand("listConnections -source true -plugs true \""+fullPlugName+"\"", srcPlug));
					assert(srcPlug.length()==1);
					MStringArray src;
					srcPlug[0].split('.',src);
					MString srcNode(src[0]);

					visitFile(srcNode.asChar());
					//
					param_node = getTextureInstanceName(srcNode.asChar());
				}
				//
				bsdf_params.insert(param.c_str(), param_node.c_str());
			}
			//////////////////////////////////////////////////////////////////////////
			{
				param = "weight1";//entity_types:(float)|texture_instance
				std::string param_node;

				const std::string plugName(m_bsdf_model+"_"+param);//bsdf_mix_weight1

				MString fullPlugName((m_nodename+"."+plugName).c_str());//<nodename>.bsdf_mix_weight1
				int connected = liquidmaya::ShaderMgr::getSingletonPtr()->convertibleConnection(fullPlugName.asChar());
				if(connected != 1)
				{
					double weight0;
					IfMErrorWarn(MGlobal::executeCommand("getAttr (\""+fullPlugName+"\")", weight0));
					MString strWeight;
					strWeight.set(weight0);

					param_node = strWeight.asChar();
				}else{//the color plug is linked in.
					MStringArray srcPlug;
					IfMErrorWarn(MGlobal::executeCommand("listConnections -source true -plugs true \""+fullPlugName+"\"", srcPlug));
					assert(srcPlug.length()==1);
					MStringArray src;
					srcPlug[0].split('.',src);
					MString srcNode(src[0]);

					visitFile(srcNode.asChar());
					//
					param_node = getTextureInstanceName(srcNode.asChar());
				}
				//
				bsdf_params.insert(param.c_str(), param_node.c_str());
			}
		}
		//
		if(m_assembly->bsdfs().get_by_name(bsdf_name.c_str()) == nullptr)
		{
			m_assembly->bsdfs().insert(
				asr::BSDFMixFactory().create(
					bsdf_name.c_str(),
					bsdf_params
				)
			);
		}
		material_params.insert( "bsdf", bsdf_name.c_str() );
	}

	void MaterialFactory2::createBSDF_kelemen_brdf()
	{
		CM_TRACE_FUNC("MaterialFactory2::createBSDF_kelemen_brdf()");
		liquidMessage2( messageError, "the type of [%s] is not implemented yet.", m_bsdf_model.c_str() );

	}

	void MaterialFactory2::createBSDF_lambertian_brdf()
	{
		CM_TRACE_FUNC("MaterialFactory2::createBSDF_lambertian_brdf()");

		std::string bsdf_name(getBSDFName(m_nodename,m_bsdf_model));//<nodename>_lambert_brdf

		asr::ParamArray bsdf_params;
		{
			std::string param;

			{
				param = "reflectance";//entity_types:color|texture_instance
				std::string param_node;

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

					createColor3(m_assembly->colors(), param_node.c_str(), val[0], val[1], val[2]);
				}else{//the color plug is linked in.
					MStringArray srcPlug;
					IfMErrorWarn(MGlobal::executeCommand("listConnections -source true -plugs true \""+fullPlugName+"\"", srcPlug));
					assert(srcPlug.length()==1);
					MStringArray src;
					srcPlug[0].split('.',src);
					MString srcNode(src[0]);

					visitFile(srcNode.asChar());
					//
					param_node = getTextureInstanceName(srcNode.asChar());
				}
				//
				bsdf_params.insert(param.c_str(), param_node.c_str());
			}
			//////////////////////////////////////////////////////////////////////////
			{
				param = "reflectance_multiplier";//entity_types:color|texture_instance
				std::string param_node;

				const std::string plugName(m_bsdf_model+"_"+param);//lambert_brdf_reflectance

				MString fullPlugName((m_nodename+"."+plugName).c_str());//<nodename>.lambert_brdf_reflectance_multiplier
				int connected = liquidmaya::ShaderMgr::getSingletonPtr()->convertibleConnection(fullPlugName.asChar());
				if(connected != 1)
				{
					MDoubleArray val; 
					val.setLength(3);
					IfMErrorWarn(MGlobal::executeCommand("getAttr (\""+fullPlugName+"\")", val));
					//only use the Red color
					MString strVal0;
					strVal0.set(val[0]);
					param_node = strVal0.asChar();
				}else{//the color plug is linked in.
					MStringArray srcPlug;
					IfMErrorWarn(MGlobal::executeCommand("listConnections -source true -plugs true \""+fullPlugName+"\"", srcPlug));
					assert(srcPlug.length()==1);
					MStringArray src;
					srcPlug[0].split('.',src);
					MString srcNode(src[0]);

					visitFile(srcNode.asChar());
					//
					param_node = getTextureInstanceName(srcNode.asChar());
				}
				//
				bsdf_params.insert(param.c_str(), param_node.c_str());
			}

		}
		//
		if(m_assembly->bsdfs().get_by_name(bsdf_name.c_str()) == nullptr)
		{
			m_assembly->bsdfs().insert(
				asr::LambertianBRDFFactory().create(
					bsdf_name.c_str(),
					bsdf_params
				)
			);
		}
		material_params.insert( "bsdf", bsdf_name.c_str() );
	}

	void MaterialFactory2::createBSDF_null_bsdf()
	{
		CM_TRACE_FUNC("MaterialFactory2::createBSDF_null_bsdf()");
		liquidMessage2( messageError, "the type of [%s] is not implemented yet.", m_bsdf_model.c_str() );

	}

	void MaterialFactory2::createBSDF_specular_brdf()
	{
		CM_TRACE_FUNC("MaterialFactory2::createBSDF_specular_brdf()");

		std::string bsdf_name(getBSDFName(m_nodename,m_bsdf_model));//<nodename>_specular_brdf

		asr::ParamArray bsdf_params;
		{
			std::string param;

			{
				param = "reflectance";//entity_types:color|texture_instance
				std::string param_node;

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

					createColor3(m_assembly->colors(), param_node.c_str(), val[0], val[1], val[2]);
				}else{//the color plug is linked in.
					MStringArray srcPlug;
					IfMErrorWarn(MGlobal::executeCommand("listConnections -source true -plugs true \""+fullPlugName+"\"", srcPlug));
					assert(srcPlug.length()==1);
					MStringArray src;
					srcPlug[0].split('.',src);
					MString srcNode(src[0]);

					visitFile(srcNode.asChar());
					//
					param_node = getTextureInstanceName(srcNode.asChar());
				}
				//
				bsdf_params.insert(param.c_str(), param_node.c_str());
			}
			//////////////////////////////////////////////////////////////////////////
			{
				param = "reflectance_multiplier";//entity_types:texture_instance
				std::string param_node;

				const std::string plugName(m_bsdf_model+"_"+param);//specular_brdf_reflectance_multiplier

				MString fullPlugName((m_nodename+"."+plugName).c_str());//<nodename>.specular_brdf_reflectance_multiplier
				int connected = liquidmaya::ShaderMgr::getSingletonPtr()->convertibleConnection(fullPlugName.asChar());
				if(connected != 1)
				{
					MDoubleArray val; 
					val.setLength(3);
					IfMErrorWarn(MGlobal::executeCommand("getAttr (\""+fullPlugName+"\")", val));
					//only use the Red color
					MString strVal0;
					strVal0.set(val[0]);
					param_node = strVal0.asChar();
				}else{//the color plug is linked in.
					MStringArray srcPlug;
					IfMErrorWarn(MGlobal::executeCommand("listConnections -source true -plugs true \""+fullPlugName+"\"", srcPlug));
					assert(srcPlug.length()==1);
					MStringArray src;
					srcPlug[0].split('.',src);
					MString srcNode(src[0]);

					visitFile(srcNode.asChar());
					//
					param_node = getTextureInstanceName(srcNode.asChar());
				}
				//
				bsdf_params.insert(param.c_str(), param_node.c_str());
			}
			//////////////////////////////////////////////////////////////////////////

		}
		//
		if(m_assembly->bsdfs().get_by_name(bsdf_name.c_str()) == nullptr)
		{
			m_assembly->bsdfs().insert(
				asr::SpecularBRDFFactory().create(
					bsdf_name.c_str(),
					bsdf_params
				)
			);
		}
		material_params.insert( "bsdf", bsdf_name.c_str() );
	}

	void MaterialFactory2::createBSDF_specular_btdf()
	{
		CM_TRACE_FUNC("MaterialFactory2::createBSDF_specular_btdf()");
		liquidMessage2( messageError, "the type of [%s] is not implemented yet.", m_bsdf_model.c_str() );

	}

	//
	void MaterialFactory2::createEDF_diffuse_edf()
	{
		CM_TRACE_FUNC("MaterialFactory2::createEDF_diffuse_edf()");

		std::string edf_name(getEDFName(m_nodename,m_edf_model));//<nodename>_diffuse_edf
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

				createColor3(m_assembly->colors(), param_node.c_str(), v[0], v[1], v[2]);
				//
				edf_params.insert(param.c_str(), param_node.c_str());
			}
		}
		//
		if( !edf_params.empty() )
		{
			if(m_assembly->edfs().get_by_name(edf_name.c_str()) == nullptr)
			{
				m_assembly->edfs().insert(
					asr::DiffuseEDFFactory().create(
					edf_name.c_str(),
					edf_params
					)
				);
			}
			//
			material_params.insert("edf", edf_name.c_str());
		}

	}

	//
	void MaterialFactory2::createSurfaceShader_ao()
	{
		CM_TRACE_FUNC("MaterialFactory2::createSurfaceShader_ao()");
		liquidMessage2( messageError, "the type of [%s] is not implemented yet.", m_surface_shader_model.c_str() );

	}

	void MaterialFactory2::createSurfaceShader_constant()
	{
		CM_TRACE_FUNC("MaterialFactory2::createSurfaceShader_constant()");

		std::string surfaceshader_name(getSurfaceShaderName(m_nodename,m_surface_shader_model));//<nodename>_constant_surface_shader

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

				createColor3(m_assembly->colors(), param_node.c_str(), val[0], val[1], val[2]);
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
		if(m_assembly->surface_shaders().get_by_name(surfaceshader_name.c_str()) == nullptr)
		{
			m_assembly->surface_shaders().insert(
				asr::ConstantSurfaceShaderFactory().create(
					surfaceshader_name.c_str(),
					surfaceshader_params
				)
			);
		}
		material_params.insert( "surface_shader", surfaceshader_name.c_str() );
	}

	void MaterialFactory2::createSurfaceShader_diagnostic()
	{
		CM_TRACE_FUNC("MaterialFactory2::createSurfaceShader_diagnostic()");
		liquidMessage2( messageError, "the type of  [%s] is not implemented yet.", m_surface_shader_model.c_str() );

	}

	void MaterialFactory2::createSurfaceShader_fast_sss()
	{
		CM_TRACE_FUNC("MaterialFactory2::createSurfaceShader_fast_sss()");
		liquidMessage2( messageError, "the type of  [%s] is not implemented yet.", m_surface_shader_model.c_str() );

	}

	void MaterialFactory2::createSurfaceShader_physical()
	{
		CM_TRACE_FUNC("MaterialFactory2::createSurfaceShader_physical()");

		std::string surfaceshader_name(getSurfaceShaderName(m_nodename,m_surface_shader_model));

		if(m_assembly->surface_shaders().get_by_name(surfaceshader_name.c_str()) == nullptr)
		{
			m_assembly->surface_shaders().insert(
				asr::PhysicalSurfaceShaderFactory().create(
					surfaceshader_name.c_str(),
					asr::ParamArray()
				)
			);
		}
		material_params.insert( "surface_shader", surfaceshader_name.c_str() );

	}

	void MaterialFactory2::createSurfaceShader_smoke()
	{
		CM_TRACE_FUNC("MaterialFactory2::createSurfaceShader_smoke()");
		liquidMessage2( messageError, "the type of [%s] is not implemented yet.", m_surface_shader_model.c_str() );

	}

	void MaterialFactory2::createSurfaceShader_voxel_ao()
	{
		CM_TRACE_FUNC("MaterialFactory2::createSurfaceShader_voxel_ao()");
		liquidMessage2( messageError, "the type of [%s] is not implemented yet.", m_surface_shader_model.c_str() );

	}
	void MaterialFactory2::visitFile(const char* node)
	{
		CM_TRACE_FUNC("MaterialFactory2::visitFile("<<node<<")");

		//generate texture and construct texture node
		MString fileImageName(getFileNodeImageName(node));
		MString fileTextureName;
		{
			//test "fileImageName" exist or not.
			if( access(fileImageName.asChar(), 0) != 0){
				liquidMessage2(messageError,"%s not exist!", fileImageName.asChar());
				assert(0&&"image not exist.");
			}

			bool needToConvert;//whether fileImageName is exr texture
			{
				std::string fileImageName_(fileImageName.asChar());
				std::size_t i_last_dot = fileImageName_.find_last_of('.');
				if( i_last_dot == std::string::npos ){
					liquidMessage2(messageWarning,"%s has no extention!", fileImageName_.c_str());
					assert(0&&"warrning: texture name has not extention.");
				}
				std::string imgext(fileImageName_.substr(i_last_dot+1));//imgext=exr
				std::transform(imgext.begin(),imgext.end(),imgext.begin(),tolower);

				needToConvert = (imgext != "exr");
			}

			fileTextureName = (needToConvert)? (fileImageName+".exr") : fileImageName;

			//generate texture
			if ( access(fileTextureName.asChar(), 0) != 0 )//not exist
			{
				makeTexture( fileImageName.asChar(), fileTextureName.asChar() );
			}
			//construct texture node
			//if (ei_file_exists(fileTextureName))
			{
				//ei_texture(fileImageName.asChar());
				//	ei_file_texture(fileTextureName.asChar(), eiFALSE);
				//ei_end_texture();
			}
		}

		// AS stuff
		//texture
		if( m_assembly->textures().get_by_name(node) == nullptr)
		{
			asr::ParamArray texture_params;
			texture_params.insert("filename", fileTextureName.asChar());
			texture_params.insert("color_space", "srgb");

			asf::SearchPaths search_paths;
			{
				MString dirname;
				MGlobal::executeCommand("dirname \""+fileTextureName+"\"", dirname, false, true);
				search_paths.push_back(dirname.asChar());
			}
			asf::auto_release_ptr<asr::Texture> texture = 
				asr::DiskTexture2dFactory().create(node, texture_params, search_paths);

			std::size_t texture_index = m_assembly->textures().insert(texture);
		}


		//instance
		if( m_assembly->texture_instances().get_by_name(getTextureInstanceName(node).c_str()) == nullptr)
		{
			const std::string texture_instance_name = getTextureInstanceName(node);

			asr::ParamArray texture_instance_params;
			texture_instance_params.insert("addressing_mode", "clamp");
			texture_instance_params.insert("filtering_mode", "bilinear");

			asf::auto_release_ptr<asr::TextureInstance> texture_instance =
				asr::TextureInstanceFactory::create(
				texture_instance_name.c_str(),
				texture_instance_params,
				node
				);

			m_assembly->texture_instances().insert(texture_instance);
		}

	}

}//namespace appleseed