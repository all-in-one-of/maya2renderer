#include "as_material.h"

#include "../common/prerequest_maya.h"
#include "as_renderer.h"
#include "../renderermgr.h"
#include "../shadergraph/shadermgr.h"

namespace appleseed
{
	//////////////////////////////////////////////////////////////////////////
	MaterialFactory::MaterialFactory()
		:m_renderer(nullptr),
		m_assembly(nullptr)
	{
		m_renderer = dynamic_cast<appleseed::Renderer*>( liquid::RendererMgr::getInstancePtr()->getRenderer() );
		assert(m_renderer != NULL );
	}
	MaterialFactory::~MaterialFactory()
	{

	}
	void MaterialFactory::begin(const char* node)
	{
		CM_TRACE_FUNC("begin("<<node<<")");

		m_assembly = m_renderer->getAssembly().get();
		assert(m_assembly != nullptr);

		m_nodename = node;
	}
	void MaterialFactory::end()
	{
		if(m_assembly->materials().get_by_name(m_nodename.c_str()) == nullptr)
		{
			m_assembly->materials().insert(
				asr::MaterialFactory::create( m_nodename.c_str(), material_params )
			);
		}
	}
	//////////////////////////////////////////////////////////////////////////
	void MaterialFactory::createBSDF(const std::string &modelname)
	{
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
			liquidMessage2(messageError, "BSDF type [%s] is unknown.", modelname.c_str());
		}
	}
	void MaterialFactory::createEDF(const std::string &modelname)
	{
		if("diffuse_edf"==modelname){
			createEDF(diffuse_edf);
		}else{
			liquidMessage2(messageError, "EDF type [%s] is unknown.", modelname.c_str());
		}
	}
	void MaterialFactory::createSurfaceShader(const std::string &modelname)
	{
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
			liquidMessage2(messageError, "SurfaceShader type [%s] is unknown.", modelname.c_str());
		}
	}
	//////////////////////////////////////////////////////////////////////////
	void MaterialFactory::createBSDF(BSDF_Model model)
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
			liquidMessage2(messageError, "BSDF type [%d] is unknown.", model);
		}
	}
	void MaterialFactory::createEDF(EDF_Model model)
	{
		switch(model)
		{
		case diffuse_edf:	createEDF_diffuse_edf();	break;
		default:
			liquidMessage2(messageError, "EDF type [%d] is unknown.", model);
		}
	}
	void MaterialFactory::createSurfaceShader(SurfaceShader_Model model)
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
			liquidMessage2(messageError, "SurfaceShader type [%d] is unknown.", model);
			ret = "";
		}
	}
	//////////////////////////////////////////////////////////////////////////
	void MaterialFactory::createBSDF_ashikhmin_brdf()
	{
		CM_TRACE_FUNC("MaterialFactory::createBSDF_ashikhmin_brdf()");
		liquidMessage2( messageError, "the type of node [%s] is not implemented yet.", m_nodename.c_str() );
	}

	void MaterialFactory::createBSDF_bsdf_mix()
	{
		CM_TRACE_FUNC("MaterialFactory::createBSDF_bsdf_mix()");
		liquidMessage2( messageError, "the type of node [%s] is not implemented yet.", m_nodename.c_str() );

	}

	void MaterialFactory::createBSDF_kelemen_brdf()
	{
		CM_TRACE_FUNC("MaterialFactory::createBSDF_kelemen_brdf()");
		liquidMessage2( messageError, "the type of node [%s] is not implemented yet.", m_nodename.c_str() );

	}

	void MaterialFactory::createBSDF_lambertian_brdf()
	{
		CM_TRACE_FUNC("MaterialFactory::createBSDF_lambertian_brdf()");

		std::string bsdf_name(m_nodename+"_lambert_brdf");

		asr::ParamArray bsdf_params;
		{
			std::string color_name;

			MString plug(MString(m_nodename.c_str())+".color");
			int connected = liquidmaya::ShaderMgr::getSingletonPtr()->convertibleConnection(plug.asChar());
			if(connected != 1)
			{
				color_name = m_nodename+"_color";
				//reflectance color
				MDoubleArray val; 
				val.setLength(3);
				IfMErrorWarn(MGlobal::executeCommand("getAttr \""+MString(m_nodename.c_str())+".color\"", val));

				createColor3(m_assembly->colors(), color_name.c_str(), val[0], val[1], val[2]);
			}else{//the color plug is linked in.
				MStringArray srcPlug;
				IfMErrorWarn(MGlobal::executeCommand("listConnections -source true -plugs true \""+plug+"\"", srcPlug));
				assert(srcPlug.length()==1);
				MStringArray src;
				srcPlug[0].split('.',src);
				MString srcNode(src[0]);
				//
				color_name = getTextureInstanceName(srcNode.asChar());
			}
			//
			bsdf_params.insert("reflectance", color_name.c_str());
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

	void MaterialFactory::createBSDF_null_bsdf()
	{
		CM_TRACE_FUNC("MaterialFactory::createBSDF_null_bsdf()");
		liquidMessage2( messageError, "the type of node [%s] is not implemented yet.", m_nodename.c_str() );

	}

	void MaterialFactory::createBSDF_specular_brdf()
	{
		CM_TRACE_FUNC("MaterialFactory::createBSDF_specular_brdf()");

		std::string bsdf_name(m_nodename+"_specular_brdf");

		asr::ParamArray bsdf_params;
		{
			std::string color_name;

			MString plug(MString(m_nodename.c_str())+".color");
			int connected = liquidmaya::ShaderMgr::getSingletonPtr()->convertibleConnection(plug.asChar());
			if(connected != 1)
			{
				color_name = m_nodename+"_color";
				//reflectance color
				MDoubleArray val; 
				val.setLength(3);
				IfMErrorWarn(MGlobal::executeCommand("getAttr \""+MString(m_nodename.c_str())+".color\"", val));

				createColor3(m_assembly->colors(), color_name.c_str(), val[0], val[1], val[2]);
			}else{//the color plug is linked in.
				MStringArray srcPlug;
				IfMErrorWarn(MGlobal::executeCommand("listConnections -source true -plugs true \""+plug+"\"", srcPlug));
				assert(srcPlug.length()==1);
				MStringArray src;
				srcPlug[0].split('.',src);
				MString srcNode(src[0]);
				//
				color_name = getTextureInstanceName(srcNode.asChar());
			}
			//
			bsdf_params.insert("reflectance", color_name.c_str());
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

	void MaterialFactory::createBSDF_specular_btdf()
	{
		CM_TRACE_FUNC("MaterialFactory::createBSDF_specular_btdf()");
		liquidMessage2( messageError, "the type of node [%s] is not implemented yet.", m_nodename.c_str() );

	}

	//
	void MaterialFactory::createEDF_diffuse_edf()
	{
		CM_TRACE_FUNC("MaterialFactory::createEDF_diffuse_edf()");

		std::string edf_name(m_nodename+"_edf");
		//
		asr::ParamArray edf_params;
		{
			std::string incandescence_name(m_nodename+"_incandescence");
			//
			MDoubleArray val; 
			val.setLength(3);
			IfMErrorWarn(MGlobal::executeCommand("getAttr \""+MString(m_nodename.c_str())+".incandescence\"", val));

			//if( !isZero(val[0], val[1], val[2]) )
			{
				createColor3(m_assembly->colors(), incandescence_name.c_str(), val[0], val[1], val[2]);

				//
				edf_params.insert("exitance", incandescence_name.c_str());
			}
		}
		//
		//if( !edf_params.empty() )
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
	void MaterialFactory::createSurfaceShader_ao()
	{
		CM_TRACE_FUNC("MaterialFactory::createSurfaceShader_ao()");
		liquidMessage2( messageError, "the type of node [%s] is not implemented yet.", m_nodename.c_str() );

	}

	void MaterialFactory::createSurfaceShader_constant()
	{
		CM_TRACE_FUNC("MaterialFactory::createSurfaceShader_constant()");
		liquidMessage2( messageError, "the type of node [%s] is not implemented yet.", m_nodename.c_str() );

	}

	void MaterialFactory::createSurfaceShader_diagnostic()
	{
		CM_TRACE_FUNC("MaterialFactory::createSurfaceShader_diagnostic()");
		liquidMessage2( messageError, "the type of node [%s] is not implemented yet.", m_nodename.c_str() );

	}

	void MaterialFactory::createSurfaceShader_fast_sss()
	{
		CM_TRACE_FUNC("MaterialFactory::createSurfaceShader_fast_sss()");
		liquidMessage2( messageError, "the type of node [%s] is not implemented yet.", m_nodename.c_str() );

	}

	void MaterialFactory::createSurfaceShader_physical()
	{
		CM_TRACE_FUNC("MaterialFactory::createSurfaceShader_physical()");

		std::string surfaceshader_name(getSurfaceShaderName(m_nodename));

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

	void MaterialFactory::createSurfaceShader_smoke()
	{
		CM_TRACE_FUNC("MaterialFactory::createSurfaceShader_smoke()");
		liquidMessage2( messageError, "the type of node [%s] is not implemented yet.", m_nodename.c_str() );

	}

	void MaterialFactory::createSurfaceShader_voxel_ao()
	{
		CM_TRACE_FUNC("MaterialFactory::createSurfaceShader_voxel_ao()");
		liquidMessage2( messageError, "the type of node [%s] is not implemented yet.", m_nodename.c_str() );

	}

	//
	void MaterialFactory::addSurfaceShader(const std::string& nodeName)
	{
		material_params.insert( "surface_shader", nodeName.c_str() );
	}

}//namespace appleseed