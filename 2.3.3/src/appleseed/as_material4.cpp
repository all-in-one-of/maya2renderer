#include "as_material4.h"

#include "../common/prerequest_maya.h"
#include "as_renderer.h"
#include "../renderermgr.h"
#include "../shadergraph/shadermgr.h"

namespace appleseed
{
	//////////////////////////////////////////////////////////////////////////
	Helper4::Helper4(const char* nodename)
		:m_nodename(nodename),
		m_renderer(nullptr),
		m_assembly(nullptr)
	{
		m_renderer = dynamic_cast<appleseed::Renderer*>( liquid::RendererMgr::getInstancePtr()->getRenderer() );
		assert(m_renderer != nullptr );

		m_assembly = m_renderer->getAssembly().get();
		assert(m_assembly != nullptr);
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

		}else{// $(fullPlugName) is connected out
			// if $(fullPlugName) plug is connected out to "bsdf0"/"bsdf1" of a "bsdf_mix" node,
			// we also need to create this plug for appleseed

			// get destination node(s)
			MStringArray desNodePlug;
			IfMErrorWarn(MGlobal::executeCommand("listConnections -destination true -plugs true \""+fullPlugName+"\"", desNodePlug));
			
			// check whether $(fullPlugName) is connected to a BSDF node
			bool isConnectedToA_BSDFMixNode = false;
			MString desPlug;
			for(std::size_t i = 0; i< desNodePlug.length(); ++i)
			{
				MStringArray des;
				desNodePlug[i].split('.',des);
				MString desNode(des[0]);

				//destination node BSDF type
				MString desNodeBSDFType;
				IfMErrorWarn(MGlobal::executeCommand( "getAttr \""+desNode+".rmanShaderType\"", desNodeBSDFType));

				if(desNodeBSDFType == "bsdf_mix")
				{
					isConnectedToA_BSDFMixNode = true;
					desPlug = des[1];
				}
			}

			// if $(fullPlugName) is connected out to "bsdf0"/"bsdf1" of a "bsdf_mix" node
			// we also need to create this plug for appleseed
			if( isConnectedToA_BSDFMixNode && (desPlug=="bsdf0" ||desPlug=="bsdf1") )
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