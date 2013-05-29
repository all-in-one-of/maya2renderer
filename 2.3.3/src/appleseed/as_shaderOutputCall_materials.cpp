#include "as_shaderOutputCall.h"
#include <liqlog.h>
#include "../common/mayacheck.h"
#include "../shadergraph/convertShadingNetwork.h"
#include "../shadergraph/shadermgr.h"
#include "as_material.h"
#include <liqGlobalVariable.h>
#include <liqShader.h>
#include <liqShaderFactory.h>
#include "as_helper.h"
#include "as_material4.h"
#include "as_helper5.h"
#include "as_renderer.h"
#include "../renderermgr.h"
#include "as_bsdfBuilder.h"

namespace appleseed{
namespace call{
// @node	maya shader node name
void Visitor::visitAnisotropic(const char* node)
{
	CM_TRACE_FUNC("Visitor::visitAnisotropic("<<node<<")");

}
void Visitor::visitBlinn(const char* node)
{
	CM_TRACE_FUNC("Visitor::visitBlinn("<<node<<")");

	if( m_assembly == NULL )
	{
		Renderer* m_renderer = dynamic_cast<appleseed::Renderer*>( liquid::RendererMgr::getInstancePtr()->getRenderer() );
		assert(m_renderer != NULL );

		m_assembly = m_renderer->getAssembly().get();
		assert(m_assembly != nullptr);
	}
	MStatus status;
	MObject mnode;
	getDependNodeByName(mnode, node);

	MVector color;
	IfMErrorWarn(liquidGetPlugValue(mnode, "color", color, status));
	double diffuse;
	IfMErrorWarn(liquidGetPlugValue(mnode, "diffuse", diffuse, status));


	BSDFBuilder b(node);
	b.begin();
	std::string colorBSDFName( b.getPlugBSDF("color", "lambertian_brdf") );
	std::string ambientColorBSDFName( b.getPlugBSDF("ambientColor", "lambertian_brdf") );
	std::string incandescenceBSDFName( b.getPlugBSDF("incandescence", "lambertian_brdf") );
	std::string specularColorBSDFName( b.getPlugBSDF("specularColor", "lambertian_brdf") );

	//bsdfmix0 = color * diffuse + ambientColor
	std::string bsdfmix0( b.addBSDFToNode(colorBSDFName, diffuse, 
									ambientColorBSDFName, 1.0f) );
	//bsdfmix1 = bsdfmix0 + incandescence
	std::string bsdfmix1( b.addBSDFToNode(bsdfmix0, 1.0, 
									incandescenceBSDFName, 1.0f) );
	//bsdfmix2 = bsdfmix1 + specularColor
	std::string bsdfmix2( b.addBSDFToNode(bsdfmix1, 1.0, 
									specularColorBSDFName, 1.0f) );

	b.end(bsdfmix2);

	//surface shader
	std::string aoNode;
	if( hasAO(node, aoNode) ){
		//mf.createSurfaceShader("ao_surface_shader");
	}else{
		std::string surfaceshader_name(getSurfaceShaderName(node));

		if(m_assembly->surface_shaders().get_by_name(surfaceshader_name.c_str()) == nullptr)
		{
			m_assembly->surface_shaders().insert(
				asr::PhysicalSurfaceShaderFactory().create(
				surfaceshader_name.c_str(),
				asr::ParamArray()
				)
				);
		}
	}
}
void Visitor::visitHairTubeShader(const char* node)
{
	CM_TRACE_FUNC("Visitor::visitHairTubeShader("<<node<<")");
}
void Visitor::visitLambert(const char* node)
{
	CM_TRACE_FUNC("Visitor::visitLambert("<<node<<")");
	
	if( m_assembly == NULL )
	{
		Renderer* m_renderer = dynamic_cast<appleseed::Renderer*>( liquid::RendererMgr::getInstancePtr()->getRenderer() );
		assert(m_renderer != NULL );

		m_assembly = m_renderer->getAssembly().get();
		assert(m_assembly != nullptr);
	}
	MStatus status;
	MObject mnode;
	getDependNodeByName(mnode, node);

	std::string colorChannel;
	std::string diffuseChannel;
	std::string ambientColorChannel;
	//std::string translucenceChannel;
	std::string transparencyChannel;

	Helper5 o;
	o.begin(node);
	colorChannel		= o.addChannel("color",			"color|texture_instance");
	diffuseChannel		= o.addChannel("diffuse",		"scalar|texture_instance");
	ambientColorChannel = o.addChannel("ambientColor",	"color|texture_instance");
	//translucenceChannel = o.addChannel("translucence",  "scalar|texture_instance");
	transparencyChannel = o.addChannel("transparency",  "color|texture_instance");
	o.end();

	MVector transparency;
	IfMErrorWarn(liquidGetPlugValue(mnode, "transparency", transparency, status));
	MColor rgb(MColor::kRGB, transparency.x, transparency.y, transparency.z);
	float transparencyH, transparencyS, transparencyV;
	rgb.get(MColor::kHSV, transparencyH, transparencyS, transparencyV);

	MVector ambientColor;
	IfMErrorWarn(liquidGetPlugValue(mnode, "ambientColor", ambientColor, status));

	//surface shader
	std::string aoNode;
	if( hasAO(node, aoNode) ){
		//AO shader will be created in Visitor::visit_mib_amb_occlusion()
	}else{
		//physical surface shader
		std::string surfaceshader_name(getSurfaceShaderName(node));

		if(m_assembly->surface_shaders().get_by_name(surfaceshader_name.c_str()) == nullptr)
		{
			std::string pss_alpha_multiplier(node);
			pss_alpha_multiplier += "_pss_alpha_multiplier";
			createColor3(m_assembly->colors(), pss_alpha_multiplier.c_str(),
				1.0f-transparency.x, 1.0f-transparency.y, 1.0f-transparency.z);

			std::string pss_color_multiplier(node);
			pss_alpha_multiplier += "_pss_color_multiplier";
			createColor3(m_assembly->colors(), pss_color_multiplier.c_str(),
				1.0f+ambientColor.x, 1.0f+ambientColor.y, 1.0f+ambientColor.z);
			//
			m_assembly->surface_shaders().insert(
				asr::PhysicalSurfaceShaderFactory().create(
				surfaceshader_name.c_str(),
				asr::ParamArray()
					//.insert("aerial_persp_mode",		"sky_color")
					//.insert("aerial_persp_sky_color",	ambientColorChannel.c_str() )
					.insert("aerial_persp_mode",		"environment_shader")
					.insert("color_multiplier",			pss_color_multiplier.c_str())
					//.insert("alpha_multiplier",		pss_alpha_multiplier.c_str())
					.insert("alpha_multiplier",			1.0)
				)
			);
		}
	}

	//EDF
	MVector incandescence;
	if( hasEDF(node, &incandescence.x, &incandescence.y, &incandescence.z) )
	{
		MString incandescenceColorName(MString(node)+"_incandescence");
		createColor3(m_assembly->colors(), incandescenceColorName.asChar(), 
			incandescence.x, incandescence.y, incandescence.z);

		if(m_assembly->edfs().get_by_name(getEDFName(node).c_str()) == nullptr)
		{
			m_assembly->edfs().insert(
				asr::DiffuseEDFFactory().create(
				getEDFName(node).c_str(),
				asr::ParamArray()
					.insert("exitance", incandescenceColorName.asChar())
				)
			);
		}
	}

	//BSDF
	AlphaMapType amt = AMT_Null;
	if( AMT_Null != (amt=getAlphaMap(node, &transparency.x, &transparency.y, &transparency.z, nullptr)) )
	{
		if(AMT_Color==amt)
		{
			//------------------------------
			// specular_btdf

			//refractiveIndex
			double refractiveIndex;
			IfMErrorWarn(liquidGetPlugValue(mnode, "refractiveIndex", refractiveIndex, status));
			
			const std::string fullTransparent(getFullTransparentColorName(m_assembly->colors()));
			//specular_btdf front
			std::string TRANS_FRONT(node);
			TRANS_FRONT+="_TRANS_FRONT";
			if(m_assembly->bsdfs().get_by_name(TRANS_FRONT.c_str()) == nullptr)
			{
				m_assembly->bsdfs().insert(
					asr::SpecularBTDFFactory().create(
					TRANS_FRONT.c_str(),
					asr::ParamArray()
						.insert("reflectance",				colorChannel.c_str())
						.insert("reflectance_multiplier",	diffuseChannel.c_str())
						.insert("transmittance",			transparencyChannel.c_str())//colorChannel, fullTransparent, transparencyChannel
						.insert("transmittance_multiplier",	1.0f)
						.insert("from_ior",					1.0f)
						.insert("to_ior",					refractiveIndex)
					)
				);
			}
			//specular_btdf back
			std::string TRANS_BACK(node);
			TRANS_BACK+="_TRANS_BACK";
			if(m_assembly->bsdfs().get_by_name(TRANS_BACK.c_str()) == nullptr)
			{
				m_assembly->bsdfs().insert(
					asr::SpecularBTDFFactory().create(
					TRANS_BACK.c_str(),
					asr::ParamArray()
						.insert("reflectance",				colorChannel.c_str())
						.insert("reflectance_multiplier",	diffuseChannel.c_str())
						.insert("transmittance",			transparencyChannel.c_str())//colorChannel, fullTransparent, transparencyChannel
						.insert("transmittance_multiplier",	1.0f)
						.insert("from_ior",					refractiveIndex)
						.insert("to_ior",					1.0f)
					)
				);
			}
			//-------------------------------------------------------------
			//BASE
			std::string BASE(node);
			BASE+="_BASE";
			if(m_assembly->bsdfs().get_by_name(BASE.c_str()) == nullptr)
			{
				m_assembly->bsdfs().insert(
					asr::LambertianBRDFFactory().create(
					BASE.c_str(),
					asr::ParamArray()
						.insert("reflectance",				colorChannel.c_str())
						.insert("reflectance_multiplier",	diffuseChannel.c_str())
					)
				);
			}
			//--------------------
			//blender BASE and specular_btdf
			float transparencyV1 = sqrtf(sqrtf(sqrtf(transparencyV)));
			//BSDF
			if(m_assembly->bsdfs().get_by_name(getBSDFName(node).c_str()) == nullptr)
			{
				m_assembly->bsdfs().insert(
					asr::BSDFMixFactory().create(
					getBSDFName(node).c_str(),
					asr::ParamArray()
						.insert("bsdf0",	BASE.c_str())
						.insert("bsdf1",	TRANS_FRONT.c_str())
						.insert("weight0",	1.0f - transparencyV1)
						.insert("weight1",	transparencyV1)
					)
				);
			}
			//BSDF_BACK
			if(m_assembly->bsdfs().get_by_name(getBSDFNameBack(node).c_str()) == nullptr)
			{
				m_assembly->bsdfs().insert(
					asr::BSDFMixFactory().create(
					getBSDFNameBack(node).c_str(),
					asr::ParamArray()
						.insert("bsdf0",	BASE.c_str())
						.insert("bsdf1",	TRANS_BACK.c_str())
						.insert("weight0",	1.0f - transparencyV1)
						.insert("weight1",	transparencyV1)
					)
				);
			}
		}
		//////////////////////////////////////////////////////////////////////////
		else if(AMT_Texture==amt)
		{	
			//------------------------------
			// specular_btdf

			//refractiveIndex
			double refractiveIndex;
			IfMErrorWarn(liquidGetPlugValue(mnode, "refractiveIndex", refractiveIndex, status));

			const std::string fullTransparent(getFullTransparentColorName(m_assembly->colors()));
			//BSDF front
			if(m_assembly->bsdfs().get_by_name(getBSDFName(node).c_str()) == nullptr)
			{
				m_assembly->bsdfs().insert(
					asr::SpecularBTDFFactory().create(
					getBSDFName(node).c_str(),
					asr::ParamArray()
						.insert("reflectance",				colorChannel.c_str())
						.insert("reflectance_multiplier",	diffuseChannel.c_str())
						.insert("transmittance",			transparencyChannel.c_str())//colorChannel, fullTransparent, transparencyChannel
						.insert("transmittance_multiplier",	1.0f)
						.insert("from_ior",					1.0f)
						.insert("to_ior",					refractiveIndex)
					)
				);
			}
			//BSDF back
			if(m_assembly->bsdfs().get_by_name(getBSDFNameBack(node).c_str()) == nullptr)
			{
				m_assembly->bsdfs().insert(
					asr::SpecularBTDFFactory().create(
					getBSDFNameBack(node).c_str(),
					asr::ParamArray()
						.insert("reflectance",				colorChannel.c_str())
						.insert("reflectance_multiplier",	diffuseChannel.c_str())
						.insert("transmittance",			transparencyChannel.c_str())//colorChannel, fullTransparent, transparencyChannel
						.insert("transmittance_multiplier",	1.0f)
						.insert("from_ior",					refractiveIndex)
						.insert("to_ior",					1.0f)
					)
				);
			}
		}else{
			liquidMessage2(messageError, "[%s]'s alphamap type [%d] is unhandled", node, amt);
		}
	}else{// this material is opacity
		//BRDF
		if(m_assembly->bsdfs().get_by_name(getBSDFName(node).c_str()) == nullptr)
		{
			m_assembly->bsdfs().insert(
				asr::LambertianBRDFFactory().create(
				getBSDFName(node).c_str(),
				asr::ParamArray()
					.insert("reflectance",				colorChannel.c_str())
					.insert("reflectance_multiplier",	diffuseChannel.c_str())
				)
			);
		}
	}


	//normal map
	//nothing to do

}
// @node	maya shader node name
void Visitor::visitLayeredShader(const char* node)
{
	CM_TRACE_FUNC("Visitor::visitLayeredShader("<<node<<")");
}
// @node	maya shader node name
void Visitor::visitOceanShader(const char* node)
{
	CM_TRACE_FUNC("Visitor::visitOceanShader("<<node<<")");
}
// @node	maya shader node name
void Visitor::visitPhong(const char* node)
{
	CM_TRACE_FUNC("Visitor::visitPhong("<<node<<")");
	if( m_assembly == NULL )
	{
		Renderer* m_renderer = dynamic_cast<appleseed::Renderer*>( liquid::RendererMgr::getInstancePtr()->getRenderer() );
		assert(m_renderer != NULL );

		m_assembly = m_renderer->getAssembly().get();
		assert(m_assembly != nullptr);
	}
	MStatus status;
	MObject mnode;
	getDependNodeByName(mnode, node);

	std::string colorChannel;
	std::string diffuseChannel;
	std::string ambientColorChannel;
	std::string transparencyChannel;
	std::string specularColorChannel;

	Helper5 o;
	o.begin(node);
	colorChannel		= o.addChannel("color",			"color|texture_instance");
	diffuseChannel		= o.addChannel("diffuse",		"scalar|texture_instance");
	ambientColorChannel = o.addChannel("ambientColor",	"color|texture_instance");
	transparencyChannel = o.addChannel("transparency",  "color|texture_instance");
	specularColorChannel= o.addChannel("specularColor",  "color|texture_instance");
	o.end();

	MVector transparency;
	IfMErrorWarn(liquidGetPlugValue(mnode, "transparency", transparency, status));
	MColor rgb(MColor::kRGB, transparency.x, transparency.y, transparency.z);
	float transparencyH, transparencyS, transparencyV;
	rgb.get(MColor::kHSV, transparencyH, transparencyS, transparencyV);

	MVector ambientColor;
	IfMErrorWarn(liquidGetPlugValue(mnode, "ambientColor", ambientColor, status));

	float cosinePower;
	IfMErrorWarn(liquidGetPlugValue(mnode, "cosinePower", cosinePower, status));
	//spread
	float spread = 10.0f * cosinePower;

	float reflectivity = 1.0f;
	IfMErrorWarn(liquidGetPlugValue(mnode, "reflectivity", reflectivity, status));


	//surface shader
	std::string aoNode;
	if( hasAO(node, aoNode) ){
		//AO shader will be created in Visitor::visit_mib_amb_occlusion()
	}else{
		//physical surface shader
		std::string surfaceshader_name(getSurfaceShaderName(node));

		if(m_assembly->surface_shaders().get_by_name(surfaceshader_name.c_str()) == nullptr)
		{
			std::string pss_alpha_multiplier(node);
			pss_alpha_multiplier += "_pss_alpha_multiplier";
			createColor3(m_assembly->colors(), pss_alpha_multiplier.c_str(),
				1.0f-transparency.x, 1.0f-transparency.y, 1.0f-transparency.z);

			std::string pss_color_multiplier(node);
			pss_alpha_multiplier += "_pss_color_multiplier";
			createColor3(m_assembly->colors(), pss_color_multiplier.c_str(),
				1.0f+ambientColor.x, 1.0f+ambientColor.y, 1.0f+ambientColor.z);
			//
			m_assembly->surface_shaders().insert(
				asr::PhysicalSurfaceShaderFactory().create(
				surfaceshader_name.c_str(),
				asr::ParamArray()
					.insert("aerial_persp_mode",		"sky_color")
					.insert("aerial_persp_sky_color",	ambientColorChannel.c_str() )
					
					//.insert("aerial_persp_mode",		"environment_shader")
					//.insert("color_multiplier",			pss_color_multiplier.c_str())//.insert("alpha_multiplier",		pss_alpha_multiplier.c_str())
					//.insert("alpha_multiplier",			1.0)
				)
			);
		}
	}

	//EDF
	MVector incandescence;
	if( hasEDF(node, &incandescence.x, &incandescence.y, &incandescence.z) )
	{
		MString incandescenceColorName(MString(node)+"_incandescence");
		createColor3(m_assembly->colors(), incandescenceColorName.asChar(), 
			incandescence.x, incandescence.y, incandescence.z);

		if(m_assembly->edfs().get_by_name(getEDFName(node).c_str()) == nullptr)
		{
			m_assembly->edfs().insert(
				asr::DiffuseEDFFactory().create(
				getEDFName(node).c_str(),
				asr::ParamArray()
					.insert("exitance", incandescenceColorName.asChar())
				)
			);
		}
	}

	//BSDF
	AlphaMapType amt = AMT_Null;
	if( AMT_Null != (amt=getAlphaMap(node, &transparency.x, &transparency.y, &transparency.z, nullptr)) )
	{
		if(AMT_Color==amt)
		{
			//------------------------------
			// specular_btdf

			//refractiveIndex
			double refractiveIndex;
			IfMErrorWarn(liquidGetPlugValue(mnode, "refractiveIndex", refractiveIndex, status));

			const std::string fullTransparent(getFullTransparentColorName(m_assembly->colors()));
			//specular_btdf front
			std::string TRANS_FRONT(node);
			TRANS_FRONT+="_TRANS_FRONT";
			if(m_assembly->bsdfs().get_by_name(TRANS_FRONT.c_str()) == nullptr)
			{
				m_assembly->bsdfs().insert(
					asr::SpecularBTDFFactory().create(
					TRANS_FRONT.c_str(),
					asr::ParamArray()
						.insert("reflectance",				colorChannel.c_str())
						.insert("reflectance_multiplier",	diffuseChannel.c_str())
						.insert("transmittance",			transparencyChannel.c_str())//colorChannel, fullTransparent, transparencyChannel
						.insert("transmittance_multiplier",	1.0f)
						.insert("from_ior",					1.0f)
						.insert("to_ior",					refractiveIndex)
					)
				);
			}
			//specular_btdf back
			std::string TRANS_BACK(node);
			TRANS_BACK+="_TRANS_BACK";
			if(m_assembly->bsdfs().get_by_name(TRANS_BACK.c_str()) == nullptr)
			{
				m_assembly->bsdfs().insert(
					asr::SpecularBTDFFactory().create(
					TRANS_BACK.c_str(),
					asr::ParamArray()
						.insert("reflectance",				colorChannel.c_str())
						.insert("reflectance_multiplier",	diffuseChannel.c_str())
						.insert("transmittance",			transparencyChannel.c_str())//colorChannel, fullTransparent, transparencyChannel
						.insert("transmittance_multiplier",	1.0f)
						.insert("from_ior",					refractiveIndex)
						.insert("to_ior",					1.0f)
					)
				);
			}
			//-------------------------------------------------------------
			//BASE
			std::string BASE(node);
			BASE+="_BASE";
			if(m_assembly->bsdfs().get_by_name(BASE.c_str()) == nullptr)
			{
				m_assembly->bsdfs().insert(
					asr::AshikhminBRDFFactory().create(
					BASE.c_str(),
					asr::ParamArray()
						.insert("diffuse_reflectance",				colorChannel.c_str())
						.insert("diffuse_reflectance_multiplier",	diffuseChannel.c_str())
						.insert("glossy_reflectance",				specularColorChannel.c_str())
						.insert("glossy_reflectance_multiplier",	reflectivity)
						.insert("shininess_u",	spread)
						.insert("shininess_v",	spread)
					)
				);
			}
			//--------------------
			//blender BASE and specular_btdf
			float transparencyV1 = sqrtf(sqrtf(sqrtf(transparencyV)));
			//BSDF
			if(m_assembly->bsdfs().get_by_name(getBSDFName(node).c_str()) == nullptr)
			{
				m_assembly->bsdfs().insert(
					asr::BSDFMixFactory().create(
					getBSDFName(node).c_str(),
					asr::ParamArray()
						.insert("bsdf0",	BASE.c_str())
						.insert("bsdf1",	TRANS_FRONT.c_str())
						.insert("weight0",	1.0f - transparencyV1)
						.insert("weight1",	transparencyV1)
					)
				);
			}
			//BSDF_BACK
			if(m_assembly->bsdfs().get_by_name(getBSDFNameBack(node).c_str()) == nullptr)
			{
				m_assembly->bsdfs().insert(
					asr::BSDFMixFactory().create(
					getBSDFNameBack(node).c_str(),
					asr::ParamArray()
						.insert("bsdf0",	BASE.c_str())
						.insert("bsdf1",	TRANS_BACK.c_str())
						.insert("weight0",	1.0f - transparencyV1)
						.insert("weight1",	transparencyV1)
					)
				);
			}
		}
		//////////////////////////////////////////////////////////////////////////
		else if(AMT_Texture==amt)
		{	
			//------------------------------
			// specular_btdf

			//refractiveIndex
			double refractiveIndex;
			IfMErrorWarn(liquidGetPlugValue(mnode, "refractiveIndex", refractiveIndex, status));

			const std::string fullTransparent(getFullTransparentColorName(m_assembly->colors()));
			//BSDF front
			if(m_assembly->bsdfs().get_by_name(getBSDFName(node).c_str()) == nullptr)
			{
				m_assembly->bsdfs().insert(
					asr::SpecularBTDFFactory().create(
					getBSDFName(node).c_str(),
					asr::ParamArray()
						.insert("reflectance",				colorChannel.c_str())
						.insert("reflectance_multiplier",	diffuseChannel.c_str())
						.insert("transmittance",			transparencyChannel.c_str())//colorChannel, fullTransparent, transparencyChannel
						.insert("transmittance_multiplier",	1.0f)
						.insert("from_ior",					1.0f)
						.insert("to_ior",					refractiveIndex)
					)
				);
			}
			//BSDF back
			if(m_assembly->bsdfs().get_by_name(getBSDFNameBack(node).c_str()) == nullptr)
			{
				m_assembly->bsdfs().insert(
					asr::SpecularBTDFFactory().create(
					getBSDFNameBack(node).c_str(),
					asr::ParamArray()
						.insert("reflectance",				colorChannel.c_str())
						.insert("reflectance_multiplier",	diffuseChannel.c_str())
						.insert("transmittance",			transparencyChannel.c_str())//colorChannel, fullTransparent, transparencyChannel
						.insert("transmittance_multiplier",	1.0f)
						.insert("from_ior",					refractiveIndex)
						.insert("to_ior",					1.0f)
					)
				);
			}
		}else{
			liquidMessage2(messageError, "[%s]'s alphamap type [%d] is unhandled", node, amt);
		}
	}else{// this material is opacity
		//BRDF
		if(m_assembly->bsdfs().get_by_name(getBSDFName(node).c_str()) == nullptr)
		{
			m_assembly->bsdfs().insert(
				asr::AshikhminBRDFFactory().create(
				getBSDFName(node).c_str(),
				asr::ParamArray()
					.insert("diffuse_reflectance",				colorChannel.c_str())
					.insert("diffuse_reflectance_multiplier",	diffuseChannel.c_str())
					.insert("glossy_reflectance",				specularColorChannel.c_str())
					.insert("glossy_reflectance_multiplier",	reflectivity)
					.insert("shininess_u",	spread)
					.insert("shininess_v",	spread)
				)
			);
		}
	}

}
// @node	maya shader node name
void Visitor::visitPhongE(const char* node)
{
	CM_TRACE_FUNC("Visitor::visitPhongE("<<node<<")");
}
// @node	maya shader node name
void Visitor::visitRampShader(const char* node)
{
	CM_TRACE_FUNC("Visitor::visitRampShader("<<node<<")");
}
// @node	maya shader node name
void Visitor::visitShadingMap(const char* node)
{
	CM_TRACE_FUNC("Visitor::visitShadingMap("<<node<<")");
}
// @node	maya shader node name
void Visitor::visitSurfaceShader(const char* node)
{
	CM_TRACE_FUNC("Visitor::visitSurfaceShader("<<node<<")");
}
// @node	maya shader node name
void Visitor::visitUseBackground(const char* node)
{
	CM_TRACE_FUNC("Visitor::visitUseBackground("<<node<<")");
}
/// Volumetric ///
// @node	maya shader node name
void Visitor::visitEnvFog(const char* node)
{
	CM_TRACE_FUNC("Visitor::visitEnvFog("<<node<<")");
}
// @node	maya shader node name
void Visitor::visitFluidShape(const char* node)
{
	CM_TRACE_FUNC("Visitor::visitFluidShape("<<node<<")");
}
// @node	maya shader node name
void Visitor::visitLightFog(const char* node)
{
	CM_TRACE_FUNC("Visitor::visitLightFog("<<node<<")");
}
// @node	maya shader node name
void Visitor::visitParticleCloud(const char* node)
{
	CM_TRACE_FUNC("Visitor::visitParticleCloud("<<node<<")");
}
// @node	maya shader node name
void Visitor::visitVolumeFog(const char* node)
{
	CM_TRACE_FUNC("Visitor::visitVolumeFog("<<node<<")");
}
// @node	maya shader node name
void Visitor::visitVolumeShader(const char* node)
{
	CM_TRACE_FUNC("Visitor::visitVolumeShader("<<node<<")");
}
/// DISPLACEMENT ///
// @node	maya shader node name
void Visitor::visitCMuscleShader(const char* node)
{
	CM_TRACE_FUNC("Visitor::visitCMuscleShader("<<node<<")");
}
// @node	maya shader node name
void Visitor::visitDisplacementShader(const char* node)
{
	CM_TRACE_FUNC("Visitor::visitDisplacementShader("<<node<<")");
}

/// Lights ///
// @node	maya shader node name
void Visitor::visitAmbientLight(const char* node)
{
	CM_TRACE_FUNC("Visitor::visitAmbientLight("<<node<<")");
}
// @node	maya shader node name
void Visitor::visitAreaLight(const char* node)
{
	CM_TRACE_FUNC("Visitor::visitAreaLight("<<node<<")");
}
// @node	maya shader node name
void Visitor::visitDirectionalLight(const char* node)
{
	CM_TRACE_FUNC("Visitor::visitDirectionalLight("<<node<<")");
}
// @node	maya shader node name
void Visitor::visitPointLight(const char* node)
{
	CM_TRACE_FUNC("Visitor::visitPointLight("<<node<<")");
}
// @node	maya shader node name
void Visitor::visitSpotLight(const char* node)
{
	CM_TRACE_FUNC("Visitor::visitSpotLight("<<node<<")");
}
// @node	maya shader node name
void Visitor::visitVolumeLight(const char* node)
{
	CM_TRACE_FUNC("Visitor::visitVolumeLight("<<node<<")");
}
// @node	maya shader node name
void Visitor::visit_liquidShader(const char* node)
{
	CM_TRACE_FUNC("Visitor::visit_liquidShader("<<node<<")");
	liqShader &shader = liqShaderFactory::instance().getShader( node );

	//Surface Shader
	if( shader.shader_type_ex == "ao_surface_shader")
		createSurfaceShader_ao(node);
	else if(shader.shader_type_ex == "constant_surface_shader")
		createSurfaceShader_constant(node);
	else if(shader.shader_type_ex == "diagnostic_surface_shader")
		createSurfaceShader_diagnostic(node);
	else if(shader.shader_type_ex == "fast_sss_surface_shader")
		createSurfaceShader_fast_sss(node);
	else if(shader.shader_type_ex == "physical_surface_shader")
		createSurfaceShader_physical(node);
	else if(shader.shader_type_ex == "smoke_surface_shader")
		createSurfaceShader_smoke(node);
	else if(shader.shader_type_ex == "voxel_ao_surface_shader")
		createSurfaceShader_voxel_ao(node);
	//BSDF
	else if( shader.shader_type_ex == "ashikhmin_brdf" )
		createBSDF_ashikhmin_brdf(node);
	else if( shader.shader_type_ex == "bsdf_mix")
		createBSDF_bsdf_mix(node);
	else if( shader.shader_type_ex == "kelemen_brdf")
		createBSDF_kelemen_brdf(node);
	else if( shader.shader_type_ex == "lambertian_brdf")
		createBSDF_lambertian_brdf(node);
	else if( shader.shader_type_ex == "null_bsdf")
		createBSDF_null_bsdf(node);
	else if( shader.shader_type_ex ==  "specular_brdf")
		createBSDF_specular_brdf(node);
	else if( shader.shader_type_ex ==  "specular_btdf")
		createBSDF_specular_btdf(node);
	//EDF
	else if( shader.shader_type_ex == "diffuse_edf" )
		createEDF_diffuse_edf(node);
	else {
		liquidMessage2(messageError, "shader type [%s] is unknown.", shader.shader_type_ex.asChar() );
	}
}
//
void Visitor::createBSDF_ashikhmin_brdf(const char* node)
{
	CM_TRACE_FUNC("Visitor::createBSDF_ashikhmin_brdf("<<node<<")");

	Helper4 o(node);
	o.beginBSDF("ashikhmin_brdf");
	o.addVariableBSDF("diffuse_reflectance",			"color|texture_instance",	"diffuse_reflectance");
	o.addVariableBSDF("diffuse_reflectance_multiplier",	"scalar|texture_instance",	"diffuse_reflectance_multiplier");
	o.addVariableBSDF("glossy_reflectance",				"color|texture_instance",	"glossy_reflectance");
	o.addVariableBSDF("glossy_reflectance_multiplier",	"scalar|texture_instance",	"glossy_reflectance_multiplier");
	o.addVariableBSDF("shininess_u",					"scalar|texture_instance",	"shininess_u");
	o.addVariableBSDF("shininess_v",					"scalar|texture_instance",	"shininess_v");
	o.endBSDF();
}

void Visitor::createBSDF_bsdf_mix(const char* node)
{
	CM_TRACE_FUNC("Visitor::createBSDF_bsdf_mix("<<node<<")");

	// The reason that I define "outPlugForLiquid" in bsdfmix.pl is to connect two bsdf_mix nodes, 
	// then the source bsdf_mix node will be visited when liquidMaya traverse the Maya shading graph network.
	// but I don't add "outPlugForLiquid" to bsdf_mix cause it is not defined in appleseed,
	Helper4 o(node);
	o.beginBSDF("bsdf_mix");	
	o.addVariableBSDF("bsdf0",		"bsdf",						"bsdf0");
	o.addVariableBSDF("bsdf1",		"bsdf",						"bsdf1");
	o.addVariableBSDF("weight0",	"scalar|texture_instance",	"weight0");
	o.addVariableBSDF("weight1",	"scalar|texture_instance",	"weight1");
	o.endBSDF();
}

void Visitor::createBSDF_kelemen_brdf(const char* node)
{
	CM_TRACE_FUNC("Visitor::createBSDF_kelemen_brdf("<<node<<")");

	Helper4 o(node);
	o.beginBSDF("kelemen_brdf");
	o.addVariableBSDF("matte_reflectance",					"color|texture_instance",	"matte_reflectance");
	o.addVariableBSDF("matte_reflectance_multiplier",		"scalar|texture_instance",	"matte_reflectance_multiplier");
	o.addVariableBSDF("specular_reflectance",				"color",					"specular_reflectance");
	o.addVariableBSDF("specular_reflectance_multiplier",	"scalar",					"specular_reflectance_multiplier");
	o.addVariableBSDF("roughness",							"scalar",					"roughness");
	o.endBSDF();
}

void Visitor::createBSDF_lambertian_brdf(const char* node)
{
	CM_TRACE_FUNC("Visitor::createBSDF_lambertian_brdf("<<node<<")");

	Helper4 o(node);
	o.beginBSDF("lambertian_brdf");
	o.addVariableBSDF("reflectance",			"color|texture_instance",	"reflectance");
	o.addVariableBSDF("reflectance_multiplier", "scalar|texture_instance",	"reflectance_multiplier");
	o.endBSDF();
}

void Visitor::createBSDF_null_bsdf(const char* node)
{
	CM_TRACE_FUNC("Visitor::createBSDF_null_bsdf("<<node<<")");
	liquidMessage2( messageError, "the type of null_bsdf is not implemented yet." );
}

void Visitor::createBSDF_specular_brdf(const char* node)
{
	CM_TRACE_FUNC("Visitor::createBSDF_specular_brdf("<<node<<")");

	Helper4 o(node);
	o.beginBSDF("specular_brdf");
	o.addVariableBSDF("reflectance",			"color|texture_instance",	"reflectance");
	o.addVariableBSDF("reflectance_multiplier", "scalar|texture_instance",	"reflectance_multiplier");
	o.endBSDF();
}

void Visitor::createBSDF_specular_btdf(const char* node)
{
	CM_TRACE_FUNC("Visitor::createBSDF_specular_btdf("<<node<<")");

	Helper4 o(node);
	o.beginBSDF("specular_btdf");
	o.addVariableBSDF("reflectance",			"color|texture_instance",	"reflectance");
	o.addVariableBSDF("reflectance_multiplier", "scalar|texture_instance",	"reflectance_multiplier");
	o.addVariableBSDF("transmittance",			"color|texture_instance",	"transmittance");
	o.addVariableBSDF("transmittance_multiplier","scalar|texture_instance",	"transmittance_multiplier");
	o.addVariableBSDF("from_ior",				"scalar",					"from_ior");
	o.addVariableBSDF("to_ior",					"scalar",					"to_ior");
	o.endBSDF();
}

//
void Visitor::createEDF_diffuse_edf(const char* node)
{
	CM_TRACE_FUNC("Visitor::createEDF_diffuse_edf("<<node<<")");

	Helper4 o(node);
	o.beginEDF("diffuse_edf");
	o.addVariableEDF("exitance",			"color|texture_instance",	"exitance");
	o.addVariableEDF("exitance_multiplier",	"scalar|texture_instance",	"exitance_multiplier");
	o.endEDF();
}
//
void Visitor::createSurfaceShader_ao(const char* node)
{
	CM_TRACE_FUNC("Visitor::createSurfaceShader_ao("<<node<<")");

	Helper4 o(node);
	o.beginSS("ao_surface_shader");
	o.addVariableSS("sampling_method",	"string", "sampling_method");//cosine, uniform
	o.addVariableSS("samples",			"scalar", "samples");
	o.addVariableSS("max_distance",		"scalar", "max_distance");
	o.endSS();
}

void Visitor::createSurfaceShader_constant(const char* node)
{
	CM_TRACE_FUNC("Visitor::createSurfaceShader_constant("<<node<<")");

	Helper4 o(node);
	o.beginSS("constant_surface_shader");
	o.addVariableSS("color",			"color|texture_instance", "color");
	//o.addVariableSS("alpha_source", "?|?");
	o.addVariableSS("color_multiplier",	"scalar|texture_instance", "color_multiplier");
	o.addVariableSS("alpha_multiplier",	"scalar|texture_instance", "alpha_multiplier");
	o.endSS();
}

void Visitor::createSurfaceShader_diagnostic(const char* node)
{
	CM_TRACE_FUNC("Visitor::createSurfaceShader_diagnostic("<<node<<")");

	Helper4 o(node);
	o.beginSS("diagnostic_surface_shader");
	o.addVariableSS("mode",	"string", "mode");
	o.endSS();
}

void Visitor::createSurfaceShader_fast_sss(const char* node)
{
	CM_TRACE_FUNC("Visitor::createSurfaceShader_fast_sss("<<node<<")");

	Helper4 o(node);
	o.beginSS("fast_sss_surface_shader");
	o.addVariableSS("scale",			"scalar",					"scale");
	o.addVariableSS("ambient_sss",		"scalar",					"ambient_sss");
	o.addVariableSS("view_dep_sss",		"scalar",					"view_dep_sss");
	o.addVariableSS("diffuse",			"scalar",					"diffuse");
	o.addVariableSS("power",			"scalar",					"power");
	o.addVariableSS("distortion",		"scalar",					"distortion");
	o.addVariableSS("albedo",			"color|texture_instance",	"albedo");
	o.addVariableSS("light_samples",	"scalar",					"light_samples");
	o.addVariableSS("occlusion_samples","scalar",					"occlusion_samples");
	o.endSS();
}

void Visitor::createSurfaceShader_physical(const char* node)
{
	CM_TRACE_FUNC("Visitor::createSurfaceShader_physical("<<node<<")");

	Helper4 o(node);
	o.beginSS("physical_surface_shader");
	o.addVariableSS("color_multiplier",	"scalar|texture_instance", "color_multiplier");
	o.addVariableSS("alpha_multiplier",	"scalar|texture_instance", "alpha_multiplier");
	o.addVariableSS("aerial_persp_mode",		"string",			"aerial_persp_mode");//none, environment_shader, sky_color
	o.addVariableSS("aerial_persp_sky_color",	"color",			"aerial_persp_sky_color");
	o.addVariableSS("aerial_persp_distance",	"scalar",			"aerial_persp_distance");
	o.addVariableSS("aerial_persp_intensity",	"scalar",			"aerial_persp_intensity");
	o.endSS();
}

void Visitor::createSurfaceShader_smoke(const char* node)
{
	CM_TRACE_FUNC("Visitor::createSurfaceShader_smoke("<<node<<")");
	liquidMessage2( messageError, "the type of smoke_surface_shader is not implemented yet." );
}

void Visitor::createSurfaceShader_voxel_ao(const char* node)
{
	CM_TRACE_FUNC("Visitor::createSurfaceShader_voxel_ao("<<node<<")");
	liquidMessage2( messageError, "the type of voxel_ao_surface_shader is not implemented yet." );
}
}//namespace call
}//namespace appleseed