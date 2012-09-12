#include "as_helper5.h"

#include "../common/prerequest_maya.h"
#include "as_renderer.h"
#include "../renderermgr.h"
#include "../shadergraph/shadermgr.h"

namespace appleseed
{
	//////////////////////////////////////////////////////////////////////////
	Helper5::Helper5()
		:m_assembly(nullptr)
	{
		Renderer* m_renderer = dynamic_cast<appleseed::Renderer*>( liquid::RendererMgr::getInstancePtr()->getRenderer() );
		assert(m_renderer != nullptr );

		m_assembly = m_renderer->getAssembly().get();
		assert(m_assembly != nullptr);
	}
	Helper5::~Helper5()
	{
	}
	void Helper5::begin(const char* nodename)
	{
		m_nodename = nodename;
	}
	void Helper5::end()
	{
	}
	std::string Helper5::addChannel(const std::string& param_name_maya, const std::string& param_type_as)
	{
		std::string param_value;
		const std::string plugName(param_name_maya);

		MString fullPlugName((m_nodename+"."+plugName).c_str());
		int connected = liquidmaya::ShaderMgr::getSingletonPtr()->convertibleConnection(fullPlugName.asChar());
		if( connected == 0 )
		{
			if( isType("color", param_type_as) )
			{
				MDoubleArray val; 
				val.setLength(3);
				IfMErrorWarn(MGlobal::executeCommand("getAttr (\""+fullPlugName+"\")", val));

				param_value = m_nodename+"_"+plugName;

				if("transparency"==param_name_maya)
				{
					createColor4(m_assembly->colors(), param_value.c_str(),
						1.0f - val[0], 1.0f - val[1], 1.0f - val[2], 1.0f - val[0]);
				}else{
					createColor3(m_assembly->colors(), param_value.c_str(), val[0], val[1], val[2]);
				}
			}
			else if( isType("scalar", param_type_as) )
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
			if( isType("texture_instance", param_type_as) )
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
			else if( isType("bsdf", param_type_as) )
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
				if( isType("color", param_type_as) )
				{
					MDoubleArray val; 
					val.setLength(3);
					IfMErrorWarn(MGlobal::executeCommand("getAttr (\""+fullPlugName+"\")", val));

					param_value = m_nodename+"_"+plugName;
					createColor3(m_assembly->colors(), param_value.c_str(), val[0], val[1], val[2]);
				}
				else if( isType("scalar", param_type_as) )
				{
					MDoubleArray val; 
					val.setLength(3);
					IfMErrorWarn(MGlobal::executeCommand("getAttr (\""+fullPlugName+"\")", val));

					//val contains (r,b,g) value, but I only use val[0] for 'scalar'
					MString strVal0;
					strVal0.set(val[0]);
					param_value = strVal0.asChar();
				}
				else if( isType("texture_instance", param_type_as) )
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
		return param_value;
	}
	bool Helper5::isType(const std::string& type, const std::string& entity_types)const
	{
		return (std::string::npos != entity_types.find(type));
	}
	std::string Helper5::fullTransparentColor()
	{
		std::string transparentColorName("liqFullTransparent");

		MVector transparent;
		transparent.x = transparent.y = transparent.z = 1.0f;//full transparent

		createColor3(m_assembly->colors(), transparentColorName.c_str(), 
			transparent.x, transparent.y, transparent.z);

		return transparentColorName;
	}



}//namespace appleseed