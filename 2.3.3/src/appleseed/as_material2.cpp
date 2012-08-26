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

		Helper2 o(m_nodename.c_str(), m_assembly);
		o.beginBSDF(m_bsdf_model);
		o.addVariableBSDF("bsdf0",		"bsdf");
		o.addVariableBSDF("bsdf1",		"bsdf");
		o.addVariableBSDF("weight0",	"scalar|texture_instance");
		o.addVariableBSDF("weight1",	"scalar|texture_instance");
		o.endBSDF();

		material_params.insert( "bsdf", getBSDFName(m_nodename, m_bsdf_model).c_str() );
	}

	void MaterialFactory2::createBSDF_kelemen_brdf()
	{
		CM_TRACE_FUNC("MaterialFactory2::createBSDF_kelemen_brdf()");
		liquidMessage2( messageError, "the type of [%s] is not implemented yet.", m_bsdf_model.c_str() );

	}

	void MaterialFactory2::createBSDF_lambertian_brdf()
	{
		CM_TRACE_FUNC("MaterialFactory2::createBSDF_lambertian_brdf()");

		Helper2 o(m_nodename.c_str(), m_assembly);
		o.beginBSDF(m_bsdf_model);
		o.addVariableBSDF("reflectance",			"color|texture_instance");
		o.addVariableBSDF("reflectance_multiplier", "scalar|texture_instance");
		o.endBSDF();

		material_params.insert( "bsdf", getBSDFName(m_nodename, m_bsdf_model).c_str() );
	}

	void MaterialFactory2::createBSDF_null_bsdf()
	{
		CM_TRACE_FUNC("MaterialFactory2::createBSDF_null_bsdf()");
		liquidMessage2( messageError, "the type of [%s] is not implemented yet.", m_bsdf_model.c_str() );

	}

	void MaterialFactory2::createBSDF_specular_brdf()
	{
		CM_TRACE_FUNC("MaterialFactory2::createBSDF_specular_brdf()");
		
		Helper2 o(m_nodename.c_str(), m_assembly);
		o.beginBSDF(m_bsdf_model);
		o.addVariableBSDF("reflectance",			"color|texture_instance");
		o.addVariableBSDF("reflectance_multiplier", "scalar|texture_instance");
		o.endBSDF();

		material_params.insert( "bsdf", getBSDFName(m_nodename,m_bsdf_model).c_str() );
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

		Helper2 o(m_nodename.c_str(), m_assembly);
		o.beginEDF(m_edf_model);
		o.addVariableEDF("exitance",			"color|texture_instance");
		o.addVariableEDF("exitance_multiplier",	"scalar|texture_instance");
		o.endEDF();
		
		material_params.insert("edf", getEDFName(m_nodename, m_edf_model).c_str() );

		//std::string edf_name(getEDFName(m_nodename,m_edf_model));//<nodename>_diffuse_edf
		////
		//asr::ParamArray edf_params;
		//{
		//	std::string param_node;
		//	const std::string param("exitance");
		//	const std::string plugName(m_edf_model+"_"+param);//diffuse_edf_exitance
		//	
		//	MString fullPlugName((m_nodename+"."+plugName).c_str());//<nodename>.diffuse_edf_exitance
		//	//
		//	MDoubleArray v; 
		//	v.setLength(3);
		//	IfMErrorWarn(MGlobal::executeCommand("getAttr (\""+fullPlugName+"\")", v));

		//	if( !isZero(v[0], v[1], v[2]) )
		//	{
		//		param_node = m_nodename+"_"+plugName;//<nodename>_diffuse_edf_exitance

		//		createColor3(m_assembly->colors(), param_node.c_str(), v[0], v[1], v[2]);
		//		//
		//		edf_params.insert(param.c_str(), param_node.c_str());
		//	}
		//}
		////
		//if( !edf_params.empty() )
		//{
		//	if(m_assembly->edfs().get_by_name(edf_name.c_str()) == nullptr)
		//	{
		//		m_assembly->edfs().insert(
		//			asr::DiffuseEDFFactory().create(
		//			edf_name.c_str(),
		//			edf_params
		//			)
		//		);
		//	}
		//	//
		//	material_params.insert("edf", edf_name.c_str());
		//}

	}

	//
	void MaterialFactory2::createSurfaceShader_ao()
	{
		CM_TRACE_FUNC("MaterialFactory2::createSurfaceShader_ao()");
	
		Helper2 o(m_nodename.c_str(), m_assembly);
		o.beginSS(m_surface_shader_model);
		o.addVariableSS("sampling_method",	"string");//cosine, uniform
		o.addVariableSS("samples",	"scalar");
		o.addVariableSS("max_distance",	"scalar");
		o.endSS();

		material_params.insert( "surface_shader", getSurfaceShaderName(m_nodename,m_surface_shader_model).c_str() );
	}

	void MaterialFactory2::createSurfaceShader_constant()
	{
		CM_TRACE_FUNC("MaterialFactory2::createSurfaceShader_constant()");

		Helper2 o(m_nodename.c_str(), m_assembly);
		o.beginSS(m_surface_shader_model);
		o.addVariableSS("color",			"color|texture_instance");
		//o.addVariableSS("alpha_source", "?|?");
		o.addVariableSS("color_multiplier",	"scalar|texture_instance");
		o.addVariableSS("alpha_multiplier",	"scalar|texture_instance");
		o.endSS();

		material_params.insert( "surface_shader", getSurfaceShaderName(m_nodename,m_surface_shader_model).c_str() );
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

		Helper2 o(m_nodename.c_str(), m_assembly);
		o.beginSS(m_surface_shader_model);
		o.addVariableSS("color_multiplier",	"scalar|texture_instance");
		o.addVariableSS("alpha_multiplier",	"scalar|texture_instance");
		o.addVariableSS("aerial_persp_mode",		"string");//none, environment_shader, sky_color
		o.addVariableSS("aerial_persp_sky_color",	"color");
		o.addVariableSS("aerial_persp_distance",	"scalar");
		o.addVariableSS("aerial_persp_intensity",	"scalar");
		o.endSS();

		material_params.insert( "surface_shader", getSurfaceShaderName(m_nodename,m_surface_shader_model).c_str() );

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
	//////////////////////////////////////////////////////////////////////////
	Helper2::Helper2(const char* nodename, asr::Assembly* assembly)
		:m_nodename(nodename),
		m_assembly(assembly)
	{

	}
	Helper2::~Helper2()
	{

	}
	void Helper2::beginBSDF(const std::string& bsdf_model)
	{
		m_bsdf_model = bsdf_model;
		m_bsdf_params.clear();
	}
	void Helper2::endBSDF()
	{
		if(m_assembly->bsdfs().get_by_name(getBSDFName(m_nodename,m_bsdf_model).c_str()) == nullptr)
		{
			if("lambertian_brdf"==m_bsdf_model)
			{
				m_assembly->bsdfs().insert(
					asr::LambertianBRDFFactory().create(
					getBSDFName(m_nodename,m_bsdf_model).c_str(),
					m_bsdf_params
					)
				);
			}
			else if("specular_brdf"==m_bsdf_model)
			{
				m_assembly->bsdfs().insert(
					asr::SpecularBRDFFactory().create(
					getBSDFName(m_nodename,m_bsdf_model).c_str(),
					m_bsdf_params
					)
				);
			}
			else if("bsdf_mix"==m_bsdf_model)
			{
				m_assembly->bsdfs().insert(
					asr::BSDFMixFactory().create(
					getBSDFName(m_nodename,m_bsdf_model).c_str(),
					m_bsdf_params
					)
				);
			}
		}
	}
	void Helper2::addVariableBSDF( const std::string& param_name, const std::string& entity_types )
	{
		std::string param_value;
		const std::string plugName(m_bsdf_model+"_"+param_name);

		MString fullPlugName((m_nodename+"."+plugName).c_str());
		int connected = liquidmaya::ShaderMgr::getSingletonPtr()->convertibleConnection(fullPlugName.asChar());
		if(connected == 0)
		{
			if( isType("color", entity_types) )
			{
				MDoubleArray val; 
				val.setLength(3);
				IfMErrorWarn(MGlobal::executeCommand("getAttr (\""+fullPlugName+"\")", val));

				param_value = m_nodename+"_"+plugName;
				createColor3(m_assembly->colors(), param_value.c_str(), val[0], val[1], val[2]);
			}
			else if( isType("scalar", entity_types) )
			{
				MDoubleArray val; 
				val.setLength(3);
				IfMErrorWarn(MGlobal::executeCommand("getAttr (\""+fullPlugName+"\")", val));

				//val contains (r,b,g) value, but I only use val[0] for 'scalar'
				MString strVal0;
				strVal0.set(val[0]);
				param_value = strVal0.asChar();
			}
			else {
				liquidMessage2(messageWarning,"only \"color\",\"scalar\" are handled for an unconnected plug in BSDF. "
					"the plug of %s is unhandled.", fullPlugName.asChar());
				param_value = "unhandled";
			}
		}
		else if(connected == 1)//the color plug is linked in.
		{
			if( isType("texture_instance", entity_types) )
			{
				MStringArray srcPlug;
				IfMErrorWarn(MGlobal::executeCommand("listConnections -source true -plugs true \""+fullPlugName+"\"", srcPlug));
				assert(srcPlug.length()==1);
				MStringArray src;
				srcPlug[0].split('.',src);
				MString srcNode(src[0]);
				if( is2DTexture(srcNode) || is3DTexture(srcNode) )
				{
					visitFile(srcNode.asChar());
					param_value = getTextureInstanceName(srcNode.asChar());
				}else{
					liquidMessage2(messageWarning,"type of %s is unhandled.", srcNode.asChar());
					param_value = "unhandled";
				}
			}
			else if( isType("bsdf", entity_types) )
			{
				//bsdf0 value
				MString srcBSDFModel; 
				IfMErrorWarn(MGlobal::executeCommand("getAttr (\""+fullPlugName+"\")", srcBSDFModel));

				MStringArray srcPlug;
				IfMErrorWarn(MGlobal::executeCommand("listConnections -source true -plugs true \""+fullPlugName+"\"", srcPlug));
				assert(srcPlug.length()==1);
				MStringArray src;
				srcPlug[0].split('.',src);
				MString srcNode(src[0]);

				param_value = getBSDFName(srcNode.asChar(), srcBSDFModel.asChar());
			}
			else{
				liquidMessage2(messageWarning,"only \"texture_instance\",\"bsdf\" are handled for a connected-in plug in BSDF."
					"the plug of %s is unhandled.", fullPlugName.asChar());
				param_value = "unhandled";
			}

		}else{
			liquidMessage2(messageWarning,"%s is connected out.", fullPlugName.asChar());
		}
		//
		m_bsdf_params.insert(param_name.c_str(), param_value.c_str());
	}
	bool Helper2::isType(const std::string& type, const std::string& entity_types)const
	{
		return (std::string::npos != entity_types.find(type));
	}
	void Helper2::visitFile(const char* node)
	{
		CM_TRACE_FUNC("Helper2::visitFile("<<node<<")");

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
	void Helper2::beginEDF(const std::string& edf_model)
	{
		m_edf_model = edf_model;
		m_edf_params.clear();
	}
	void Helper2::endEDF()
	{
		if(m_assembly->edfs().get_by_name(getEDFName(m_nodename, m_edf_model).c_str()) == nullptr)
		{
			if("diffuse_edf"==m_edf_model)
			{
				m_assembly->edfs().insert(
					asr::DiffuseEDFFactory().create(
					getEDFName(m_nodename, m_edf_model).c_str(),
					m_edf_params
					)
				);
			}else{
				liquidMessage2(messageError,"appleseed only support diffuse_edf model");
			}
		}
	}
	void Helper2::addVariableEDF(const std::string& param_name, const std::string& entity_types )
	{
		std::string param_value;
		const std::string plugName(m_edf_model+"_"+param_name);//diffuse_edf_exitance

		MString fullPlugName((m_nodename+"."+plugName).c_str());//<nodename>.diffuse_edf_exitance
		int connected = liquidmaya::ShaderMgr::getSingletonPtr()->convertibleConnection(fullPlugName.asChar());
		if(connected == 0)
		{
			if( isType("color", entity_types) )
			{
				MDoubleArray val; 
				val.setLength(3);
				IfMErrorWarn(MGlobal::executeCommand("getAttr (\""+fullPlugName+"\")", val));

				param_value = m_nodename+"_"+plugName;//<nodename>_lambertian_brdf_reflectance
				createColor3(m_assembly->colors(), param_value.c_str(), val[0], val[1], val[2]);
			}
			else if( isType("scalar", entity_types) )
			{
				MDoubleArray val; 
				val.setLength(3);
				IfMErrorWarn(MGlobal::executeCommand("getAttr (\""+fullPlugName+"\")", val));

				//val contains (r,b,g) value, but I only use val[0] for 'scalar'
				MString strVal0;
				strVal0.set(val[0]);
				param_value = strVal0.asChar();
			}
			else {
				liquidMessage2(messageWarning,"only \"color\",\"scalar\" are handled for an unconnected plug in EDF. "
					"the plug of %s is unhandled.", fullPlugName.asChar());
				param_value = "unhandled";
			}
		}
		else if(connected == 1)//the color plug is linked in.
		{
			if( isType("texture_instance", entity_types) )
			{
				MStringArray srcPlug;
				IfMErrorWarn(MGlobal::executeCommand("listConnections -source true -plugs true \""+fullPlugName+"\"", srcPlug));
				assert(srcPlug.length()==1);
				MStringArray src;
				srcPlug[0].split('.',src);
				MString srcNode(src[0]);
				if( is2DTexture(srcNode) || is3DTexture(srcNode) )
				{
					visitFile(srcNode.asChar());
					param_value = getTextureInstanceName(srcNode.asChar());
				}
				else{
					liquidMessage2(messageWarning,"type of %s is unhandled.", srcNode.asChar());
					param_value = "unhandled";
				}
			}
			else{
				liquidMessage2(messageWarning,"only \"texture_instance\" is handled for a connected-in plug in EDF."
					"the plug of %s is unhandled.", fullPlugName.asChar());
				param_value = "unhandled";
			}
		}else{
			liquidMessage2(messageWarning,"%s is connected out.", fullPlugName.asChar());
		}
		//
		m_edf_params.insert(param_name.c_str(), param_value.c_str());
	}
	//
	void Helper2::beginSS(const std::string& ss_model)
	{
		m_ss_model = ss_model;
		m_ss_params.clear();
	}
	void Helper2::endSS()
	{
		if(m_assembly->surface_shaders().get_by_name(getSurfaceShaderName(m_nodename,m_ss_model).c_str()) == nullptr)
		{
			if("ao_surface_shader"==m_ss_model)
			{
				m_assembly->surface_shaders().insert(
					asr::AOSurfaceShaderFactory().create(
					getSurfaceShaderName(m_nodename, m_ss_model).c_str(),
					m_ss_params
					)
					);
			}
			else if("physical_surface_shader"==m_ss_model)
			{
				m_assembly->surface_shaders().insert(
					asr::PhysicalSurfaceShaderFactory().create(
					getSurfaceShaderName(m_nodename, m_ss_model).c_str(),
					m_ss_params
					)
				);
			}
			else if("constant_surface_shader"==m_ss_model)
			{
				m_assembly->surface_shaders().insert(
					asr::ConstantSurfaceShaderFactory().create(
					getSurfaceShaderName(m_nodename, m_ss_model).c_str(),
					m_ss_params
					)
				);
			}
			//
			else 
			{
				liquidMessage2(messageError, "surface model \"%s\" is not supported.",m_ss_model.c_str() );
			}
		}
	}
	void Helper2::addVariableSS(const std::string& param_name, const std::string& entity_types )
	{
		std::string ss_name(getSurfaceShaderName(m_nodename,m_ss_model));

		asr::ParamArray ss_params;
		{
			std::string param_value;
			const std::string plugName(m_ss_model+"_"+param_name);

			MString fullPlugName((m_nodename+"."+plugName).c_str());
			int connected = liquidmaya::ShaderMgr::getSingletonPtr()->convertibleConnection(fullPlugName.asChar());
			if(connected ==0)
			{
				if( isType("color", entity_types) )
				{
					MDoubleArray val; 
					val.setLength(3);
					IfMErrorWarn(MGlobal::executeCommand("getAttr (\""+fullPlugName+"\")", val));

					param_value = m_nodename+"_"+plugName;
					createColor3(m_assembly->colors(), param_value.c_str(), val[0], val[1], val[2]);
				}
				else if( isType("scalar", entity_types) )
				{
					MDoubleArray val; 
					val.setLength(3);
					IfMErrorWarn(MGlobal::executeCommand("getAttr (\""+fullPlugName+"\")", val));

					//val contains (r,b,g) value, but I only use val[0] for 'scalar'
					MString strVal0;
					strVal0.set(val[0]);
					param_value = strVal0.asChar();
				}
				else if( isType("string", entity_types))
				{
					MString val;
					IfMErrorWarn(MGlobal::executeCommand("getAttr (\""+fullPlugName+"\")", val));
					param_value = val.asChar();
				}
				else {
					liquidMessage2(messageWarning,"only \"color\",\"scalar\",\"string\" are handled for an unconnected plug in Surface Shader. "
						"the plug of %s is unhandled.", fullPlugName.asChar());
					param_value = "unhandled";
				}
			}
			else if(connected == 1)//the plug is linked in.
			{
				if( isType("texture_instance", entity_types) )
				{
					MStringArray srcPlug;
					IfMErrorWarn(MGlobal::executeCommand("listConnections -source true -plugs true \""+fullPlugName+"\"", srcPlug));
					assert(srcPlug.length()==1);
					MStringArray src;
					srcPlug[0].split('.',src);
					MString srcNode(src[0]);

					if( is2DTexture(srcNode) || is3DTexture(srcNode) )
					{
						visitFile(srcNode.asChar());
						param_value = getTextureInstanceName(srcNode.asChar());
					}else{
						liquidMessage2(messageWarning,"only \"texture2D\",\"texture3D\" are handled for a texture_instance connected-in plug in Surface Shader."
							"the plug of %s is unhandled.", fullPlugName.asChar());
						param_value = "unhandled";
					}
				}
				else{
					liquidMessage2(messageWarning,"only \"texture_instance\" is handled for a connected-in plug in Surface Shader."
						"the plug of %s is unhandled.", fullPlugName.asChar());
					param_value = "unhandled";
				}
			}else{
				liquidMessage2(messageWarning,"%s is connected out.", fullPlugName.asChar());
			}
			//
			m_ss_params.insert(param_name.c_str(), param_value.c_str());
		}
	}


}//namespace appleseed