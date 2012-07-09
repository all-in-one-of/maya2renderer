#include <liqConfig.h>
#ifdef _USE_APPLESEED_

#include "as_renderer.h"
#include "../common/prerequest_maya.h"
#include "../common/mayacheck.h"
#include <liqRibLightData.h>
#include "log_helper.h"

namespace appleseed
{
	//
	void addLightGroupForLight(const MString& lightTransformNode)
	{
		CM_TRACE_FUNC("addLightGroupForLight("<<lightTransformNode.asChar()<<")");

		MStringArray meshNodes;
#ifdef TRANSFORM_SHAPE_PAIR
		IfMErrorWarn(MGlobal::executeCommand( "lightlink -q -set 0 -shapes 0 -transforms 1 -light "+lightTransformNode, meshNodes));//transform nodes
#else// SHAPE SHAPE_object PAIR
		IfMErrorWarn(MGlobal::executeCommand( "lightlink -q -set 0 -shapes 1 -transforms 0 -light "+lightTransformNode, meshNodes));//shape nodes
#endif

	}
	//
	bool Renderer::writeLight_pre(const liqRibNodePtr& ribNode, const structJob &currentJob)
	{

		CM_TRACE_FUNC("Renderer::writeLight_pre("<<ribNode->name.asChar()<<","<<currentJob.name.asChar()<<")");

		RtString RibNodeName = getLiquidRibName( ribNode->name.asChar() );
		_s("\n// Renderer::exportLight(\""+std::string(RibNodeName)+"\");");

		return true;
	}
	//
	void Renderer::writeLight_post(const liqRibNodePtr& ribNode, const structJob &currentJob)
	{
		CM_TRACE_FUNC("Renderer::writeLight_post("<<ribNode->name.asChar()<<","<<currentJob.name.asChar()<<")");
	}
	//
	liqLightHandle Renderer::exportAmbientLight(
		const std::string &shadertype, 
		const std::string &shaderinstance, 
		const liqFloat & intensity,
		const liqColor & color,
		const liqMatrix &t)
	{
		CM_TRACE_FUNC("Renderer::exportAmbientLight("<<shadertype<<","<<shaderinstance<<",...)");

		_s("\n// Renderer::exportAmbientLight()");
		std::string shaderinstanceFullPath( toFullDagPath(shaderinstance) );

		std::string sShaderInstanceName(shaderinstanceFullPath+"_shader");
		//todo

		return (liqLightHandle)(0);
	}
	//
	liqLightHandle Renderer::exportDistantLight(
		const std::string &shadertype, 
		const std::string &shaderinstance, 
		const liqFloat &i_intensity,
		const liqColor &i_lightcolor,
		const liqString &i_shadowname,
		const liqFloat &i_shadowbias,
		const liqFloat &i_shadowblur,
		const liqFloat &i_shadowsamples,
		const liqFloat &i_shadowfiltersize,
		const liqColor &i_shadowcolor,
		const liqFloat  &i_lightID,
		const liqString &o_category,
		const liqFloat &o_shadowF,
		const liqColor &o_shadowC,
		const liqColor &o_unshadowed_Cl,
		const liqFloat &o_nondiffuse,
		const liqFloat &o_nonspecular,
		const liqMatrix &t)
	{
		CM_TRACE_FUNC("Renderer::exportDistantLight("<<shadertype<<","<<shaderinstance<<",...)");

		_s("// Renderer::exportDistantLight()");
		std::string shaderinstanceFullPath( toFullDagPath(shaderinstance) );

		std::string sShaderInstanceName(shaderinstanceFullPath+"_shader");
		//todo

		return (liqLightHandle)(0);
	}
	//
	liqLightHandle Renderer::exportPointLight(
		const std::string &shadertype, 
		const std::string &shaderinstance, 
		const liqFloat &i_intensity,
		const liqColor &i_lightcolor,
		const liqFloat &i_decay,
		const liqString &i_shadownamepx,
		const liqString &i_shadownamenx,
		const liqString &i_shadownamepy,
		const liqString &i_shadownameny,
		const liqString &i_shadownamepz,
		const liqString &i_shadownamenz,
		const liqFloat &i_shadowbias,
		const liqFloat &i_shadowblur,
		const liqFloat &i_shadowsamples,
		const liqFloat &i_shadowfiltersize,
		const liqColor &i_shadowcolor,
		const liqFloat  &i_lightID,
		const liqString &o_category,
		const liqFloat &o_shadowF,
		const liqColor &o_shadowC,
		const liqColor &o_unshadowed_Cl,
		const liqFloat &o_nondiffuse,
		const liqFloat &o_nonspecular,
		const liqMatrix &t)
	{
		CM_TRACE_FUNC("Renderer::exportPointLight("<<shadertype<<","<<shaderinstance<<",...)");

		_s("\n// Renderer::exportPointLight()");
		std::string shaderinstanceFullPath( toFullDagPath(shaderinstance) );

		std::string sShaderInstanceName(shaderinstanceFullPath+"_shader");
		std::string sLightObjectName(shaderinstanceFullPath+"_object");

		// AS stuff
		std::string sLightExitance(shaderinstanceFullPath+"_exitance");
		//
		current_assembly->colors().insert(
			asr::ColorEntityFactory::create(
				sLightExitance.c_str(),
				asr::ParamArray()
				.insert("color_space", "srgb")
				.insert("multiplier", "30.0"),
				asr::ColorValueArray(3, i_lightcolor)
			)
		);

		// Create a point light called "light" and insert it into the assembly.
		asf::auto_release_ptr<asr::Light> light(
			asr::PointLightFactory().create(
			shaderinstanceFullPath.c_str(),//instance name
			asr::ParamArray()
			.insert("exitance", sLightExitance)
			)
		);

		// transform matrix
		asf::Matrix4d as_mtx;
		convertMatrix<double>(as_mtx, t);
		light->set_transform( asf::Transformd( as_mtx ) );

		//
		current_assembly->lights().insert(light);

		return (liqLightHandle)(0);
	}
	//
	liqLightHandle Renderer::exportSpotLight(
		const std::string &shadertype, 
		const std::string &shaderinstance, 
		const liqFloat &i_intensity,
		const liqColor &i_lightcolor,
		const liqFloat &i_coneangle,
		const liqFloat &i_penumbraangle,
		const liqFloat &i_dropoff,
		const liqFloat &i_decay,

		const liqFloat &i_barndoors,
		const liqFloat &i_leftbarndoor            ,
		const liqFloat &i_rightbarndoor           ,
		const liqFloat &i_topbarndoor             ,
		const liqFloat &i_bottombarndoor          ,

		const liqFloat &i_decayRegions            ,
		const liqFloat &i_startDistance1          ,
		const liqFloat &i_endDistance1            ,
		const liqFloat &i_startDistance2          ,
		const liqFloat &i_endDistance2            ,
		const liqFloat &i_startDistance3          ,
		const liqFloat &i_endDistance3            ,
		const liqFloat &i_startDistanceIntensity1,
		const liqFloat &i_endDistanceIntensity1,
		const liqFloat &i_startDistanceIntensity2,
		const liqFloat &i_endDistanceIntensity2,
		const liqFloat &i_startDistanceIntensity3,
		const liqFloat &i_endDistanceIntensity3,

		const liqString &i_shadowname,
		const liqFloat &i_shadowbias,
		const liqFloat &i_shadowblur,
		const liqFloat &i_shadowsamples,
		const liqFloat &i_shadowfiltersize,
		const liqColor &i_shadowcolor,
		const liqColor &i_shadowcolorsurf,
		const liqFloat &i_shadowcolormix,

		const liqFloat  &i_lightID,
		const liqString &i_category,
		const liqColor &o_shadowC,
		const liqFloat &o_shadowF,
		const liqFloat &o_barn,
		const liqColor &o_unshadowed_Cl,
		const liqFloat &o_nondiffuse,
		const liqFloat &o_nonspecular,
		const liqMatrix &t)
	{
		CM_TRACE_FUNC("Renderer::exportSpotLight("<<shadertype<<","<<shaderinstance<<",...)");

		_s("\n// Renderer::exportSpotLight()");
		std::string shaderinstanceFullPath( toFullDagPath(shaderinstance) );

		std::string sShaderInstanceName(shaderinstanceFullPath+"_shader");
		//todo

		return (liqLightHandle)(0);
	}
	//
	liqLightHandle Renderer::exportAreaLight(
		const std::string &shadertype, 
		const std::string &shaderinstance, 
		const liqFloat &i_intensity,
		const liqColor &i_lightcolor,
		const liqFloat &i_decay,
		const liqString  &i_coordsys,
		const liqFloat   &i_lightsamples,
		const liqFloat &i_doublesided,
		const liqString &i_shadowname,
		const liqColor &i_shadowcolor,
		const liqString &i_hitmode,
		const liqString &i_lightmap,
		const liqFloat &i_lightmapsaturation,
		const liqFloat  &i_lightID,
		const liqString &i_category,
		const liqFloat &o_nonspecular,
		const liqFloat &o_shadowF,
		const liqColor &o_shadowC,
		const liqColor &o_unshadowed_Cl,
		const liqFloat &o_arealightIntensity,
		const liqColor &o_arealightColor,
		const liqMatrix &t)
	{
		CM_TRACE_FUNC("Renderer::exportAreaLight("<<shadertype<<","<<shaderinstance<<",...)");

		_s("\n// Renderer::exportAreaLight()");
		std::string shaderinstanceFullPath( toFullDagPath(shaderinstance) );

		std::string sShaderInstanceName(shaderinstanceFullPath+"_shader");
		//todo

		return (liqLightHandle)(0);
	}
}//namespace appleseed
#endif//_USE_APPLESEED_