#include "as_material4.h"

#include "../common/prerequest_maya.h"
#include "as_renderer.h"
#include "../renderermgr.h"
#include "../shadergraph/shadermgr.h"

namespace appleseed
{
	//////////////////////////////////////////////////////////////////////////
	MaterialFactory4::MaterialFactory4()
		:m_renderer(nullptr),
		m_assembly(nullptr)
	{
		m_renderer = dynamic_cast<appleseed::Renderer*>( liquid::RendererMgr::getInstancePtr()->getRenderer() );
		assert(m_renderer != NULL );
	}
	MaterialFactory4::~MaterialFactory4()
	{

	}
	void MaterialFactory4::begin(const char* node)
	{
		CM_TRACE_FUNC("begin("<<node<<")");

		m_assembly = m_renderer->getAssembly().get();
		assert(m_assembly != nullptr);

		m_nodename = node;
	}
	void MaterialFactory4::end()
	{
// 		if(m_assembly->materials().get_by_name(m_nodename.c_str()) == nullptr)
// 		{
// 			m_assembly->materials().insert(
// 				asr::MaterialFactory::create( m_nodename.c_str(), material_params )
// 				);
// 		}
	}
	//////////////////////////////////////////////////////////////////////////
	void MaterialFactory4::createBSDF(const std::string &modelname)
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
	void MaterialFactory4::createEDF(const std::string &modelname)
	{
		m_edf_model = modelname;

		if("diffuse_edf"==modelname){
			createEDF(diffuse_edf);
		}else{
			liquidMessage2(messageError, "EDF type %s is unknown.", modelname.c_str());
		}
	}
	void MaterialFactory4::createSurfaceShader(const std::string &modelname)
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
	void MaterialFactory4::createBSDF(BSDF_Model model)
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
	void MaterialFactory4::createEDF(EDF_Model model)
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
	void MaterialFactory4::createSurfaceShader(SurfaceShader_Model model)
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
	void MaterialFactory4::createBSDF_ashikhmin_brdf()
	{
		CM_TRACE_FUNC("MaterialFactory4::createBSDF_ashikhmin_brdf()");
		
		Helper4 o(m_nodename.c_str(), m_assembly);
		o.beginBSDF(m_bsdf_model);
		o.addVariableBSDF("diffuse_reflectance",			"color|texture_instance");
		o.addVariableBSDF("diffuse_reflectance_multiplier",	"scalar|texture_instance");
		o.addVariableBSDF("glossy_reflectance",				"color|texture_instance");
		o.addVariableBSDF("glossy_reflectance_multiplier",	"scalar|texture_instance");
		o.addVariableBSDF("shininess_u",					"scalar|texture_instance");
		o.addVariableBSDF("shininess_v",					"scalar|texture_instance");
		o.endBSDF();

		//material_params.insert( "bsdf", getBSDFName(m_nodename, m_bsdf_model).c_str() );	
	}

	void MaterialFactory4::createBSDF_bsdf_mix()
	{
		CM_TRACE_FUNC("MaterialFactory4::createBSDF_bsdf_mix()");

		Helper4 o(m_nodename.c_str(), m_assembly);
		o.beginBSDF(m_bsdf_model);
		o.addVariableBSDF("bsdf0",		"bsdf");
		o.addVariableBSDF("bsdf1",		"bsdf");
		o.addVariableBSDF("weight0",	"scalar|texture_instance");
		o.addVariableBSDF("weight1",	"scalar|texture_instance");
		o.endBSDF();

		//material_params.insert( "bsdf", getBSDFName(m_nodename, m_bsdf_model).c_str() );
	}

	void MaterialFactory4::createBSDF_kelemen_brdf()
	{
		CM_TRACE_FUNC("MaterialFactory4::createBSDF_kelemen_brdf()");

		Helper4 o(m_nodename.c_str(), m_assembly);
		o.beginBSDF(m_bsdf_model);
		o.addVariableBSDF("matte_reflectance",					"color|texture_instance");
		o.addVariableBSDF("matte_reflectance_multiplier",		"scalar|texture_instance");
		o.addVariableBSDF("specular_reflectance",				"color");
		o.addVariableBSDF("specular_reflectance_multiplier",	"scalar");
		o.addVariableBSDF("roughness",							"scalar");

		o.endBSDF();

		//material_params.insert( "bsdf", getBSDFName(m_nodename, m_bsdf_model).c_str() );

	}

	void MaterialFactory4::createBSDF_lambertian_brdf()
	{
		CM_TRACE_FUNC("MaterialFactory4::createBSDF_lambertian_brdf()");

		Helper4 o(m_nodename.c_str(), m_assembly);
		o.beginBSDF(m_bsdf_model);
		o.addVariableBSDF("reflectance",			"color|texture_instance");
		o.addVariableBSDF("reflectance_multiplier", "scalar|texture_instance");
		o.endBSDF();

		//material_params.insert( "bsdf", getBSDFName(m_nodename, m_bsdf_model).c_str() );
	}

	void MaterialFactory4::createBSDF_null_bsdf()
	{
		CM_TRACE_FUNC("MaterialFactory4::createBSDF_null_bsdf()");
		liquidMessage2( messageError, "the type of [%s] is not implemented yet.", m_bsdf_model.c_str() );

	}

	void MaterialFactory4::createBSDF_specular_brdf()
	{
		CM_TRACE_FUNC("MaterialFactory4::createBSDF_specular_brdf()");
		
		Helper4 o(m_nodename.c_str(), m_assembly);
		o.beginBSDF(m_bsdf_model);
		o.addVariableBSDF("reflectance",			"color|texture_instance");
		o.addVariableBSDF("reflectance_multiplier", "scalar|texture_instance");
		o.endBSDF();

		//material_params.insert( "bsdf", getBSDFName(m_nodename,m_bsdf_model).c_str() );
	}

	void MaterialFactory4::createBSDF_specular_btdf()
	{
		CM_TRACE_FUNC("MaterialFactory4::createBSDF_specular_btdf()");

		Helper4 o(m_nodename.c_str(), m_assembly);
		o.beginBSDF(m_bsdf_model);
		o.addVariableBSDF("reflectance",			"color|texture_instance");
		o.addVariableBSDF("reflectance_multiplier", "scalar|texture_instance");
		o.addVariableBSDF("transmittance",			"color|texture_instance");
		o.addVariableBSDF("transmittance_multiplier",	"scalar|texture_instance");
		o.addVariableBSDF("from_ior",					"scalar");
		o.addVariableBSDF("to_ior",						"scalar");
		o.endBSDF();

		//material_params.insert( "bsdf", getBSDFName(m_nodename,m_bsdf_model).c_str() );

	}

	//
	void MaterialFactory4::createEDF_diffuse_edf()
	{
		CM_TRACE_FUNC("MaterialFactory4::createEDF_diffuse_edf()");

		Helper4 o(m_nodename.c_str(), m_assembly);
		o.beginEDF(m_edf_model);
		o.addVariableEDF("exitance",			"color|texture_instance");
		o.addVariableEDF("exitance_multiplier",	"scalar|texture_instance");
		o.endEDF();
		
		//material_params.insert("edf", getEDFName(m_nodename, m_edf_model).c_str() );
	}

	//
	void MaterialFactory4::createSurfaceShader_ao()
	{
		CM_TRACE_FUNC("MaterialFactory4::createSurfaceShader_ao()");
	
		Helper4 o(m_nodename.c_str(), m_assembly);
		o.beginSS(m_surface_shader_model);
		o.addVariableSS("sampling_method",	"string");//cosine, uniform
		o.addVariableSS("samples",	"scalar");
		o.addVariableSS("max_distance",	"scalar");
		o.endSS();

		//material_params.insert( "surface_shader", getSurfaceShaderName(m_nodename,m_surface_shader_model).c_str() );
	}

	void MaterialFactory4::createSurfaceShader_constant()
	{
		CM_TRACE_FUNC("MaterialFactory4::createSurfaceShader_constant()");

		Helper4 o(m_nodename.c_str(), m_assembly);
		o.beginSS(m_surface_shader_model);
		o.addVariableSS("color",			"color|texture_instance");
		//o.addVariableSS("alpha_source", "?|?");
		o.addVariableSS("color_multiplier",	"scalar|texture_instance");
		o.addVariableSS("alpha_multiplier",	"scalar|texture_instance");
		o.endSS();

		//material_params.insert( "surface_shader", getSurfaceShaderName(m_nodename,m_surface_shader_model).c_str() );
	}

	void MaterialFactory4::createSurfaceShader_diagnostic()
	{
		CM_TRACE_FUNC("MaterialFactory4::createSurfaceShader_diagnostic()");
		
		Helper4 o(m_nodename.c_str(), m_assembly);
		o.beginSS(m_surface_shader_model);
		o.addVariableSS("mode",	"string");
		o.endSS();

		//material_params.insert( "surface_shader", getSurfaceShaderName(m_nodename,m_surface_shader_model).c_str() );
	}

	void MaterialFactory4::createSurfaceShader_fast_sss()
	{
		CM_TRACE_FUNC("MaterialFactory4::createSurfaceShader_fast_sss()");

		Helper4 o(m_nodename.c_str(), m_assembly);
		o.beginSS(m_surface_shader_model);
		o.addVariableSS("scale",			"scalar");
		o.addVariableSS("ambient_sss",		"scalar");
		o.addVariableSS("view_dep_sss",		"scalar");
		o.addVariableSS("diffuse",			"scalar");
		o.addVariableSS("power",			"scalar");
		o.addVariableSS("distortion",		"scalar");
		o.addVariableSS("albedo",			"color|texture_instance");
		o.addVariableSS("light_samples",	"scalar");
		o.addVariableSS("occlusion_samples","scalar");
		o.endSS();

		//material_params.insert( "surface_shader", getSurfaceShaderName(m_nodename,m_surface_shader_model).c_str() );
	}

	void MaterialFactory4::createSurfaceShader_physical()
	{
		CM_TRACE_FUNC("MaterialFactory4::createSurfaceShader_physical()");

		Helper4 o(m_nodename.c_str(), m_assembly);
		o.beginSS(m_surface_shader_model);
		o.addVariableSS("color_multiplier",	"scalar|texture_instance");
		o.addVariableSS("alpha_multiplier",	"scalar|texture_instance");
		o.addVariableSS("aerial_persp_mode",		"string");//none, environment_shader, sky_color
		o.addVariableSS("aerial_persp_sky_color",	"color");
		o.addVariableSS("aerial_persp_distance",	"scalar");
		o.addVariableSS("aerial_persp_intensity",	"scalar");
		o.endSS();

		//material_params.insert( "surface_shader", getSurfaceShaderName(m_nodename,m_surface_shader_model).c_str() );

	}

	void MaterialFactory4::createSurfaceShader_smoke()
	{
		CM_TRACE_FUNC("MaterialFactory4::createSurfaceShader_smoke()");
		liquidMessage2( messageError, "the type of [%s] is not implemented yet.", m_surface_shader_model.c_str() );

	}

	void MaterialFactory4::createSurfaceShader_voxel_ao()
	{
		CM_TRACE_FUNC("MaterialFactory4::createSurfaceShader_voxel_ao()");
		liquidMessage2( messageError, "the type of [%s] is not implemented yet.", m_surface_shader_model.c_str() );

	}
	//////////////////////////////////////////////////////////////////////////
	Helper4::Helper4(const char* nodename, asr::Assembly* assembly)
		:m_nodename(nodename),
		m_assembly(assembly)
	{

	}
	Helper4::~Helper4()
	{

	}
	void Helper4::beginBSDF(const std::string& bsdf_model)
	{
		m_bsdf_model = bsdf_model;
		m_bsdf_params.clear();
	}
	void Helper4::endBSDF()
	{
		if(m_assembly->bsdfs().get_by_name(m_nodename.c_str()) == nullptr)
		{
			if("ashikhmin_brdf"==m_bsdf_model)
			{
				m_assembly->bsdfs().insert(
					asr::AshikhminBRDFFactory().create(
					m_nodename.c_str(),
					m_bsdf_params
					)
					);
			}
			else if("kelemen_brdf"==m_bsdf_model)
			{
				m_assembly->bsdfs().insert(
					asr::KelemenBRDFFactory().create(
					m_nodename.c_str(),
					m_bsdf_params
					)
				);
			}
			else if("lambertian_brdf"==m_bsdf_model)
			{
				m_assembly->bsdfs().insert(
					asr::LambertianBRDFFactory().create(
					m_nodename.c_str(),
					m_bsdf_params
					)
				);
			}
			else if("specular_brdf"==m_bsdf_model)
			{
				m_assembly->bsdfs().insert(
					asr::SpecularBRDFFactory().create(
					m_nodename.c_str(),
					m_bsdf_params
					)
				);
			}
			else if("bsdf_mix"==m_bsdf_model)
			{
				m_assembly->bsdfs().insert(
					asr::BSDFMixFactory().create(
					m_nodename.c_str(),
					m_bsdf_params
					)
				);
			}
			else if("specular_btdf"==m_bsdf_model)
			{
				m_assembly->bsdfs().insert(
					asr::SpecularBTDFFactory().create(
					m_nodename.c_str(),
					m_bsdf_params
					)
				);
			}
			else{
				liquidMessage2( messageError, "\"%s\" is not implemented yet.", m_bsdf_model.c_str() );

			}
		}
	}
	void Helper4::addVariableBSDF( const std::string& param_name, const std::string& entity_types )
	{
		std::string param_value;
		const std::string plugName(param_name);

		MString fullPlugName((m_nodename+"."+plugName).c_str());
		int connected = liquidmaya::ShaderMgr::getSingletonPtr()->convertibleConnection(fullPlugName.asChar());
		if( connected == 0 )
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
					//visitFile(srcNode.asChar());
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

				param_value = srcNode.asChar();
			}
			else{
				liquidMessage2(messageWarning,"only \"texture_instance\",\"bsdf\" are handled for a connected-in plug in BSDF."
					"the plug of %s is unhandled.", fullPlugName.asChar());
				param_value = "unhandled";
			}

		}else{//the plug(param_name) is connected out
			// if the plug(param_name) plug is connected out to "bsdf0"/"bsdf1" of a "bsdf_mix" node,
			// we also need to create this plug for appleseed

			// get destination node
			MStringArray desNodePlug;
			IfMErrorWarn(MGlobal::executeCommand("listConnections -destination true -plugs true \""+fullPlugName+"\"", desNodePlug));
			assert(desNodePlug.length()==1);
			MStringArray des;
			desNodePlug[0].split('.',des);
			MString desNode(des[0]);
			MString desPlug(des[1]);

			//destination node BSDF type
			MString desNodeBSDFType;
			IfMErrorWarn(MGlobal::executeCommand( "getAttr \""+desNode+".rmanShaderType\"", desNodeBSDFType));

			// if the plug(param_name) is connected out to "bsdf0"/"bsdf1" of a "bsdf_mix" node
			// we also need to create this plug for appleseed
			if( desNodeBSDFType=="bsdf_mix" && (desPlug=="bsdf0" ||desPlug=="bsdf1") )
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
				else if( isType("texture_instance", entity_types) )
				{
					MStringArray srcPlug;
					IfMErrorWarn(MGlobal::executeCommand("listConnections -source true -plugs true \""+fullPlugName+"\"", srcPlug));
					assert(srcPlug.length()==1);
					MStringArray src;
					srcPlug[0].split('.',src);
					MString srcNode(src[0]);
					if( is2DTexture(srcNode) || is3DTexture(srcNode) )
					{
						//visitFile(srcNode.asChar());
						param_value = getTextureInstanceName(srcNode.asChar());
					}else{
						liquidMessage2(messageWarning,"type of %s is unhandled.", srcNode.asChar());
						param_value = "unhandled";
					}
				}
				else {
					liquidMessage2(messageWarning,"only \"color\",\"scalar\",\"texture_instance\" are handled for an connected-out plug in BSDF. "
						"the plug of %s is unhandled.", fullPlugName.asChar());
					param_value = "unhandled";
				}
			}//if( nodetype=="bsdf_mix" && (desPlug=="bsdf0" ||desPlug=="bsdf1") )
			else {
				liquidMessage2(messageWarning,"%s is connected out. But not connected to brdf node, or not brdf0/brdf1 of a brdf node."
					" So I don't create the value for this plug.", fullPlugName.asChar());
			}
		}
		//
		m_bsdf_params.insert(param_name.c_str(), param_value.c_str());
	}
	bool Helper4::isType(const std::string& type, const std::string& entity_types)const
	{
		return (std::string::npos != entity_types.find(type));
	}

	void Helper4::beginEDF(const std::string& edf_model)
	{
		m_edf_model = edf_model;
		m_edf_params.clear();
	}
	void Helper4::endEDF()
	{
		if(m_assembly->edfs().get_by_name(m_nodename.c_str()) == nullptr)
		{
			if("diffuse_edf"==m_edf_model)
			{
				m_assembly->edfs().insert(
					asr::DiffuseEDFFactory().create(
					m_nodename.c_str(),
					m_edf_params
					)
				);
			}else{
				liquidMessage2(messageError,"appleseed only support diffuse_edf model");
			}
		}
	}
	void Helper4::addVariableEDF(const std::string& param_name, const std::string& entity_types )
	{
		std::string param_value;
		const std::string plugName(param_name);

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
					//visitFile(srcNode.asChar());
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
	void Helper4::beginSS(const std::string& ss_model)
	{
		m_ss_model = ss_model;
		m_ss_params.clear();
	}
	void Helper4::endSS()
	{
		if(m_assembly->surface_shaders().get_by_name(m_nodename.c_str()) == nullptr)
		{
			if("ao_surface_shader"==m_ss_model)
			{
				m_assembly->surface_shaders().insert(
					asr::AOSurfaceShaderFactory().create(
					m_nodename.c_str(),
					m_ss_params
					)
				);
			}
			else if("diagnostic_surface_shader"==m_ss_model)
			{
				m_assembly->surface_shaders().insert(
					asr::DiagnosticSurfaceShaderFactory().create(
					m_nodename.c_str(),
					m_ss_params
					)
					);
			}
			else if("fast_sss_surface_shader"==m_ss_model)
			{
				m_assembly->surface_shaders().insert(
					asr::FastSubSurfaceScatteringSurfaceShaderFactory().create(
					m_nodename.c_str(),
					m_ss_params
					)
					);
			}
			else if("physical_surface_shader"==m_ss_model)
			{
				m_assembly->surface_shaders().insert(
					asr::PhysicalSurfaceShaderFactory().create(
					m_nodename.c_str(),
					m_ss_params
					)
				);
			}
			else if("constant_surface_shader"==m_ss_model)
			{
				m_assembly->surface_shaders().insert(
					asr::ConstantSurfaceShaderFactory().create(
					m_nodename.c_str(),
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
	void Helper4::addVariableSS(const std::string& param_name, const std::string& entity_types )
	{
		//std::string ss_name(getSurfaceShaderName(m_nodename,m_ss_model));

		asr::ParamArray ss_params;
		{
			std::string param_value;
			const std::string plugName(param_name);

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
						//visitFile(srcNode.asChar());
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