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
//#include "as_material2.h"
#include "as_material4.h"

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

	OutputHelper o;

	o.begin(node);

	o.addVariable("vector", "inColor",		"color",		node);
	o.addVariable("vector", "transparency", "transparency", node);
	o.addVariable("vector", "ambColor",		"ambientColor", node);
	o.addVariable("vector", "incandescence","incandescence",node);
	o.addVariable("float",  "diffusion",	"diffuse",		node);
	o.addVariable("float",  "eccentricity", "eccentricity", node);
	o.addVariable("float",  "specRollOff",	"specularRollOff",node);
	o.addVariable("vector", "specColor",	"specularColor",node);
	o.addVariable("vector", "outColor",		"outColor",		node);

//	MStringArray con;
//	IfMErrorWarn(MGlobal::executeCommand( ("listConnections(\""+MString(node)+"\" + \".reflectedColor\")"), con));
//	if( con.length() != 0 )
//	{
//		o.addVariable( "float", "reflectivity", "reflectivity", node);
//		o.addVariable( "vector", "refColor", "reflectedColor", node);
//		o.addTo( "Cspecular += ( reflectivity * refColor );");
//	}

	o.end();
}
void Visitor::visitHairTubeShader(const char* node)
{
	CM_TRACE_FUNC("Visitor::visitHairTubeShader("<<node<<")");
}
void Visitor::visitLambert(const char* node)
{
	CM_TRACE_FUNC("Visitor::visitLambert("<<node<<")");

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
	o.addVariableBSDF("diffuse_reflectance",			"color|texture_instance");
	o.addVariableBSDF("diffuse_reflectance_multiplier",	"scalar|texture_instance");
	o.addVariableBSDF("glossy_reflectance",				"color|texture_instance");
	o.addVariableBSDF("glossy_reflectance_multiplier",	"scalar|texture_instance");
	o.addVariableBSDF("shininess_u",					"scalar|texture_instance");
	o.addVariableBSDF("shininess_v",					"scalar|texture_instance");
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
	o.addVariableBSDF("bsdf0",		"bsdf");
	o.addVariableBSDF("bsdf1",		"bsdf");
	o.addVariableBSDF("weight0",	"scalar|texture_instance");
	o.addVariableBSDF("weight1",	"scalar|texture_instance");
	o.endBSDF();
}

void Visitor::createBSDF_kelemen_brdf(const char* node)
{
	CM_TRACE_FUNC("Visitor::createBSDF_kelemen_brdf("<<node<<")");

	Helper4 o(node);
	o.beginBSDF("kelemen_brdf");
	o.addVariableBSDF("matte_reflectance",					"color|texture_instance");
	o.addVariableBSDF("matte_reflectance_multiplier",		"scalar|texture_instance");
	o.addVariableBSDF("specular_reflectance",				"color");
	o.addVariableBSDF("specular_reflectance_multiplier",	"scalar");
	o.addVariableBSDF("roughness",							"scalar");
	o.endBSDF();
}

void Visitor::createBSDF_lambertian_brdf(const char* node)
{
	CM_TRACE_FUNC("Visitor::createBSDF_lambertian_brdf("<<node<<")");

	Helper4 o(node);
	o.beginBSDF("lambertian_brdf");
	o.addVariableBSDF("reflectance",			"color|texture_instance");
	o.addVariableBSDF("reflectance_multiplier", "scalar|texture_instance");
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
	o.addVariableBSDF("reflectance",			"color|texture_instance");
	o.addVariableBSDF("reflectance_multiplier", "scalar|texture_instance");
	o.endBSDF();
}

void Visitor::createBSDF_specular_btdf(const char* node)
{
	CM_TRACE_FUNC("Visitor::createBSDF_specular_btdf("<<node<<")");

	Helper4 o(node);
	o.beginBSDF("specular_btdf");
	o.addVariableBSDF("reflectance",			"color|texture_instance");
	o.addVariableBSDF("reflectance_multiplier", "scalar|texture_instance");
	o.addVariableBSDF("transmittance",			"color|texture_instance");
	o.addVariableBSDF("transmittance_multiplier",	"scalar|texture_instance");
	o.addVariableBSDF("from_ior",					"scalar");
	o.addVariableBSDF("to_ior",						"scalar");
	o.endBSDF();
}

//
void Visitor::createEDF_diffuse_edf(const char* node)
{
	CM_TRACE_FUNC("Visitor::createEDF_diffuse_edf("<<node<<")");

	Helper4 o(node);
	o.beginEDF("diffuse_edf");
	o.addVariableEDF("exitance",			"color|texture_instance");
	o.addVariableEDF("exitance_multiplier",	"scalar|texture_instance");
	o.endEDF();
}
//
void Visitor::createSurfaceShader_ao(const char* node)
{
	CM_TRACE_FUNC("Visitor::createSurfaceShader_ao("<<node<<")");

	Helper4 o(node);
	o.beginSS("ao_surface_shader");
	o.addVariableSS("sampling_method",	"string");//cosine, uniform
	o.addVariableSS("samples",	"scalar");
	o.addVariableSS("max_distance",	"scalar");
	o.endSS();
}

void Visitor::createSurfaceShader_constant(const char* node)
{
	CM_TRACE_FUNC("Visitor::createSurfaceShader_constant("<<node<<")");

	Helper4 o(node);
	o.beginSS("constant_surface_shader");
	o.addVariableSS("color",			"color|texture_instance");
	//o.addVariableSS("alpha_source", "?|?");
	o.addVariableSS("color_multiplier",	"scalar|texture_instance");
	o.addVariableSS("alpha_multiplier",	"scalar|texture_instance");
	o.endSS();
}

void Visitor::createSurfaceShader_diagnostic(const char* node)
{
	CM_TRACE_FUNC("Visitor::createSurfaceShader_diagnostic("<<node<<")");

	Helper4 o(node);
	o.beginSS("diagnostic_surface_shader");
	o.addVariableSS("mode",	"string");
	o.endSS();
}

void Visitor::createSurfaceShader_fast_sss(const char* node)
{
	CM_TRACE_FUNC("Visitor::createSurfaceShader_fast_sss("<<node<<")");

	Helper4 o(node);
	o.beginSS("fast_sss_surface_shader");
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
}

void Visitor::createSurfaceShader_physical(const char* node)
{
	CM_TRACE_FUNC("Visitor::createSurfaceShader_physical("<<node<<")");

	Helper4 o(node);
	o.beginSS("physical_surface_shader");
	o.addVariableSS("color_multiplier",	"scalar|texture_instance");
	o.addVariableSS("alpha_multiplier",	"scalar|texture_instance");
	o.addVariableSS("aerial_persp_mode",		"string");//none, environment_shader, sky_color
	o.addVariableSS("aerial_persp_sky_color",	"color");
	o.addVariableSS("aerial_persp_distance",	"scalar");
	o.addVariableSS("aerial_persp_intensity",	"scalar");
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