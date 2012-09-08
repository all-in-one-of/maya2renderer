#include <liqConfig.h>
#ifdef _USE_APPLESEED_

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

	std::string colorChannel;
	std::string diffuseChannel;
	std::string ambientColorChannel;
	std::string fullTransparent;
	std::string transparencyChannel;
	std::string translucenceChannel;

	Helper5 h;
	h.begin(node);
	colorChannel		= h.addChannel("color",			"color|texture_instance");
	diffuseChannel		= h.addChannel("diffuse",		"scalar|texture_instance");
	ambientColorChannel = h.addChannel("ambientColor",	"color|texture_instance");
	fullTransparent     = h.fullTransparentColor();
	transparencyChannel = h.addChannel("transparency",	"color|texture_instance");
	translucenceChannel = h.addChannel("translucence",  "scalar|texture_instance");
	h.end();

	//LambertianBRDF
	if(m_assembly->bsdfs().get_by_name(getBSDFBaseName(node).c_str()) == nullptr)
	{
		m_assembly->bsdfs().insert(
			asr::LambertianBRDFFactory().create(
			getBSDFBaseName(node).c_str(),
			asr::ParamArray()
				.insert("reflectance",				colorChannel.c_str())
				.insert("reflectance_multiplier",	diffuseChannel.c_str())
			)
		);
	}

	//surface shader
	std::string aoNode;
	if( hasAO(node, aoNode) ){
		//AO shader will be created in Visitor::visit_mib_amb_occlusion()
	}else{
		//physical surface shader
		std::string surfaceshader_name(getSurfaceShaderName(node));

		if(m_assembly->surface_shaders().get_by_name(surfaceshader_name.c_str()) == nullptr)
		{
			m_assembly->surface_shaders().insert(
				asr::PhysicalSurfaceShaderFactory().create(
				surfaceshader_name.c_str(),
				asr::ParamArray()
					.insert("aerial_persp_mode",		"sky_color")
					.insert("aerial_persp_sky_color",	ambientColorChannel.c_str() )
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

	//alpha map
	AlphaMapType amt = AMT_Null;
	MVector transparency;
	if( AMT_Null != (amt=getAlphaMap(node, &transparency.x, &transparency.y, &transparency.z, nullptr)) )
	{
		if(AMT_Color==amt)
		{
			//It seems that the alpha color can't achieve the right effect, so I omit it.
			//createColor3(m_assembly->colors(), getAlphaColorName(node).c_str(), 
			//	transparency.x, transparency.y, transparency.z);

			//SpecularBTDF
			const MString BTDF(MString(node)+"_transparent");

			if(m_assembly->bsdfs().get_by_name(BTDF.asChar()) == nullptr)
			{
				m_assembly->bsdfs().insert(
					asr::SpecularBTDFFactory().create(
					BTDF.asChar(),
					asr::ParamArray()
						.insert("reflectance",				colorChannel.c_str())
						.insert("reflectance_multiplier",	diffuseChannel.c_str())
						.insert("transmittance",			fullTransparent.c_str())
						.insert("from_ior",					1.0f)
						.insert("to_ior",					1.1f)
					)
				);
			}
			//compose final BSDF
			if(m_assembly->bsdfs().get_by_name(getBSDFName(node).c_str()) == nullptr)
			{
				m_assembly->bsdfs().insert(
					asr::BSDFMixFactory().create(
					getBSDFName(node).c_str(),
					asr::ParamArray()
						.insert("bsdf0", getBSDFBaseName(node).c_str())//lambert base
						.insert("bsdf1", BTDF.asChar())//transparency
						.insert("weight0", transparency.x)
						.insert("weight1", 1.0f - transparency.x)
					)
				);
			}
			//-------------------------------------------------------------
			//create the back face BSDF
			{
				//create back face SpecularBTDF
				const MString BTDF_BACK(BTDF+"_back");
				if(m_assembly->bsdfs().get_by_name(BTDF_BACK.asChar()) == nullptr)
				{
					m_assembly->bsdfs().insert(
						asr::SpecularBTDFFactory().create(
						BTDF_BACK.asChar(),
						asr::ParamArray()
							.insert("reflectance",				colorChannel.c_str())
							.insert("reflectance_multiplier",	diffuseChannel.c_str())
							.insert("transmittance",			fullTransparent.c_str())
							.insert("from_ior",					1.1f)
							.insert("to_ior",					1.0f)
						)
					);
				}
				//compose final BSDF_BACK
				if(m_assembly->bsdfs().get_by_name(getBSDFNameBack(node).c_str()) == nullptr)
				{
					m_assembly->bsdfs().insert(
						asr::BSDFMixFactory().create(
						getBSDFNameBack(node).c_str(),
						asr::ParamArray()
							.insert("bsdf0",   getBSDFBaseName(node).c_str())//lambert base
							.insert("bsdf1",   BTDF_BACK.asChar())//transparency
							.insert("weight0", transparency.x)
							.insert("weight1", 1.0f - transparency.x)
						)
					);
				}
			}

		}
		//////////////////////////////////////////////////////////////////////////
		else if(AMT_Texture==amt){
			// the texture will be created somewhere else
			//SpecularBTDF
			const MString BTDF(MString(node)+"_transparent");

			if(m_assembly->bsdfs().get_by_name(BTDF.asChar()) == nullptr)
			{
				m_assembly->bsdfs().insert(
					asr::SpecularBTDFFactory().create(
					BTDF.asChar(),
					asr::ParamArray()
						.insert("reflectance",				colorChannel.c_str())
						.insert("reflectance_multiplier",	diffuseChannel.c_str())
						.insert("transmittance",			transparencyChannel.c_str())
						.insert("transmittance_multiplier",	"1.0")
						.insert("from_ior",					1.0f)
						.insert("to_ior",					1.1f)
					)
				);
			}
			//compose final BSDF
			if(m_assembly->bsdfs().get_by_name(getBSDFName(node).c_str()) == nullptr)
			{
				m_assembly->bsdfs().insert(
					asr::BSDFMixFactory().create(
					getBSDFName(node).c_str(),
					asr::ParamArray()
						.insert("bsdf0", getBSDFBaseName(node).c_str())//lambert base
						.insert("bsdf1", BTDF.asChar())//transparency
						.insert("weight0", transparency.x)
						.insert("weight1", 1.0f - transparency.x)
					)
				);
			}
			//-------------------------------------------------------------
			//create the back face BSDF
			{
				//create back face SpecularBTDF
				const MString BTDF_BACK(BTDF+"_back");
				if(m_assembly->bsdfs().get_by_name(BTDF_BACK.asChar()) == nullptr)
				{
					m_assembly->bsdfs().insert(
						asr::SpecularBTDFFactory().create(
						BTDF_BACK.asChar(),
						asr::ParamArray()
							.insert("reflectance",				colorChannel.c_str())
							.insert("reflectance_multiplier",	diffuseChannel.c_str())
							.insert("transmittance",			transparencyChannel.c_str())
							.insert("transmittance_multiplier",	"1.0")
							.insert("from_ior",					1.1f)
							.insert("to_ior",					1.0f)
						)
					);
				}
				//compose final BSDF_BACK
				if(m_assembly->bsdfs().get_by_name(getBSDFNameBack(node).c_str()) == nullptr)
				{
					m_assembly->bsdfs().insert(
						asr::BSDFMixFactory().create(
						getBSDFNameBack(node).c_str(),
						asr::ParamArray()
							.insert("bsdf0",   getBSDFBaseName(node).c_str())//lambert base
							.insert("bsdf1",   BTDF_BACK.asChar())//transparency
							.insert("weight0", transparency.x)
							.insert("weight1", 1.0f - transparency.x)
						)
					);
				}
			}
		}else{
			liquidMessage2(messageError, "\"%s\"'s alphamap type\"%d\" is unhandled", node, amt);
		}
	}
	//this material is opacity
	else{
		if(m_assembly->bsdfs().get_by_name(getBSDFName(node).c_str()) == nullptr)
		{
			BSDFBuilder b(node);
			std::string dummyBSDF(b.GetOrCreateDummyBSDF());
			
			//compose finally BSDF
			m_assembly->bsdfs().insert(
				asr::BSDFMixFactory().create(
				getBSDFName(node).c_str(),
				asr::ParamArray()
					.insert("bsdf0", getBSDFBaseName(node).c_str())
					.insert("bsdf1", dummyBSDF.c_str() )
					.insert("weight0", 1.0f)
					.insert("weight1", 0.0f)
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
	
	MaterialFactory mf;

	mf.begin(node);

	//brdf
	if( liqglo.rt_useRayTracing ){
		mf.createBSDF("specular_brdf");
	}else{
		mf.createBSDF("lambertian_brdf");
	}

	//edf
	mf.createEDF("diffuse_edf");

	//surface shader
	bool isSurfaceShaderCreated = false;
	MString plug(MString(node) +".ambientColor");
	MStringArray nodes;
	IfMErrorWarn(MGlobal::executeCommand("listConnections -source true -plugs false \""+plug+"\"", nodes));
	if( nodes.length() != 0 )
	{
		MString srcNode(nodes[0]);
		MString srcNodeType;
		IfMErrorWarn(MGlobal::executeCommand("nodeType \""+srcNode+"\"", srcNodeType));
		if( srcNodeType == "mib_amb_occlusion" )
		{
			isSurfaceShaderCreated = true;
			mf.addSurfaceShader(srcNode.asChar());
		}else if( srcNodeType == "another node type" ){
			isSurfaceShaderCreated = true;			
			//todo...
		}
	}

	if( ! isSurfaceShaderCreated ){
		mf.createSurfaceShader("physical_surface_shader");
	}

	mf.end();
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
		liquidMessage2(messageError, "shader type %s is unknown.", shader.shader_type_ex.asChar() );
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
#endif//_USE_APPLESEED_