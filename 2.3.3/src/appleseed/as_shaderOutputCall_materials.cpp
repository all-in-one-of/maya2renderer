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

	//MStatus status;
	//MString svalue;
	//
	//MObject mnode;
	//getDependNodeByName(mnode, shader.getName().c_str());

	//int use_bsdf = 0;
	//int use_edf = 0;
	//int use_alpha_map = 0;
	//int use_normal_map = 0;
	//liquidGetPlugValue(mnode, "use_bsdf", use_bsdf, status); 
	//IfMErrorWarn(status);
	//liquidGetPlugValue(mnode, "use_edf", use_edf, status); 
	//IfMErrorWarn(status);
	//liquidGetPlugValue(mnode, "use_alpha_map", use_alpha_map, status); 
	//IfMErrorWarn(status);
	//liquidGetPlugValue(mnode, "use_normal_map", use_normal_map, status); 
	//IfMErrorWarn(status);

	//MaterialFactory2 mf;
	//mf.begin(shader.getName().c_str());
	//
	//if(use_bsdf){
	//	liquidGetPlugValue(mnode, "bsdf_model", svalue, status); IfMErrorWarn(status);
	//	mf.createBSDF(svalue.asChar());
	//}
	//if(use_edf){
	//	liquidGetPlugValue(mnode, "edf_model", svalue, status); IfMErrorWarn(status);
	//	mf.createEDF(svalue.asChar());
	//}

	//liquidGetPlugValue(mnode, "surface_shader_model", svalue, status); IfMErrorWarn(status);
	//mf.createSurfaceShader(svalue.asChar());
	//
	//if(use_alpha_map){
	//}
	//if(use_normal_map){
	//}

	//mf.end();

	if( shader.shader_type_ex == "ao_surface_shader"
	 || shader.shader_type_ex == "constant_surface_shader"
	 || shader.shader_type_ex == "diagnostic_surface_shader"
	 || shader.shader_type_ex == "fast_sss_surface_shader"
	 || shader.shader_type_ex == "physical_surface_shader"
	 || shader.shader_type_ex == "smoke_surface_shader"
	 || shader.shader_type_ex == "voxel_ao_surface_shader" )
	{
		MaterialFactory4 mf;
		mf.begin(node);
		mf.createSurfaceShader(shader.shader_type_ex.asChar());
		mf.end();
	}
	else if( shader.shader_type_ex == "ashikhmin_brdf"
		  || shader.shader_type_ex == "bsdf_mix"
		  || shader.shader_type_ex == "kelemen_brdf"
		  || shader.shader_type_ex == "lambertian_brdf"
		  || shader.shader_type_ex == "null_bsdf"
		  || shader.shader_type_ex == "smoke_surface_shader"
		  || shader.shader_type_ex == "specular_brdf"
		  || shader.shader_type_ex == "specular_btdf" )
	{
		MaterialFactory4 mf;
		mf.begin(node);
		mf.createBSDF(shader.shader_type_ex.asChar());
		mf.end();
	}
	else if( shader.shader_type_ex == "diffuse_edf" )
	{
		MaterialFactory4 mf;
		mf.begin(node);
		mf.createEDF(shader.shader_type_ex.asChar());
		mf.end();
	}
}
}//namespace call
}//namespace appleseed
#endif//_USE_APPLESEED_