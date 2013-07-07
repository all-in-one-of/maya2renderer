#include "rm_renderer.h"
#include <liqRibLightData.h>
#include <liqGlobalVariable.h>
#include <liqGlobalHelpers.h>
#include <liqShader.h>
#include "rm_helper.h"
#include "../renderermgr.h"
namespace renderman
{
	bool Renderer::writeLight_pre(const liqRibNodePtr& ribNode, const structJob& currentJob)
	{
		CM_TRACE_FUNC("Renderer::writeLight_pre("<<ribNode->name.asChar()<<","<<currentJob.name.asChar()<<")");

		RtInt on( 1 );
		// We need to enclose lights in attribute blocks because of the
		// new added attribute support
		RiAttributeBegin();

		// All this stuff below should be handled by a new attribute class
		LIQDEBUGPRINTF( "-> RibNodeName " );
		RtString RibNodeName = getLiquidRibName( ribNode->name.asChar() );
		LIQDEBUGPRINTF( "= %s.\n", (char *)RibNodeName  );
		RiAttribute( "identifier", "name", &RibNodeName, RI_NULL );
		if( ribNode->trace.sampleMotion )
			RiAttribute( "trace", (RtToken) "samplemotion", &on, RI_NULL );
		if( ribNode->trace.displacements )
			RiAttribute( "trace", (RtToken) "displacements", &on, RI_NULL );
		if( ribNode->trace.bias != 0.01f )
		{
			RtFloat bias = ribNode->trace.bias;
			RiAttribute( "trace", (RtToken) "bias", &bias, RI_NULL );
		}
		if( ribNode->trace.maxDiffuseDepth != 1 )
		{
			RtInt mddepth = ribNode->trace.maxDiffuseDepth;
			RiAttribute( "trace", (RtToken) "maxdiffusedepth", &mddepth, RI_NULL );
		}
		if( ribNode->trace.maxSpecularDepth != 2 )
		{
			RtInt msdepth = ribNode->trace.maxSpecularDepth;
			RiAttribute( "trace", (RtToken) "maxspeculardepth", &msdepth, RI_NULL );
		}

		if ( liqglo.liquidRenderer.renderName == MString("3Delight") )
		{
			//RiArchiveRecord( RI_COMMENT, "light attributes for 3delight is omitted. You can open them." );
			liqRibLightData *lightdata = dynamic_cast<liqRibLightData*>(ribNode->object(0)->getDataPtr().get());
			assert( lightdata );
			if ( lightdata->delightLight.emitPhotons )
			{
				RtString value = "on";
				RiAttribute( "light", (RtToken) "emitphotons", &value, RI_NULL );
			}
			if ( lightdata->delightLight.autoShadows )
			{
				RtString value = "on";
				RiAttribute( "light", (RtToken) "shadows", &value, RI_NULL );
			}
			if ( lightdata->delightLight.autoShadowsSamples != 1 )
			{
				RtInt value = lightdata->delightLight.autoShadowsSamples;
				RiAttribute( "light", (RtToken) "samples", &value, RI_NULL );
			}
			if ( lightdata->delightLight.SamplingStrategy != lightdata->delightLight::SAMPLING_STRATEGY_NONE )
			{
				RtString values[] = { "lightsource", "illuminance" };
				RiAttribute( "light", (RtToken) "samplingstrategy", &values[(int)(lightdata->delightLight.SamplingStrategy)], RI_NULL );
			}
		}
		return true;
	}
	//
	void Renderer::writeLight_post(const liqRibNodePtr& ribNode, const structJob& currentJob)
	{
		CM_TRACE_FUNC("Renderer::writeLight_post("<<ribNode->name.asChar()<<","<<currentJob.name.asChar()<<")");

		// The next line pops the light...
		RiAttributeEnd();
		// close light by default // [1/30/2012 yaoyansi]
		RiIlluminate( ribNode->object(0)->lightHandle(), 0 );
	}
	//
	liqLightHandle Renderer::exportAmbientLight(
		const std::string &shadertype, 
		const std::string &shaderinstance, 
		const liqFloat & intensity,
		const liqColor & color,
		const liqMatrix &transform)
	{
		CM_TRACE_FUNC("Renderer::exportAmbientLight("<<shadertype<<","<<shaderinstance<<",...)");

		liqMatrix transformationMatrixScaledZ;
		liqRibLightData::scaleZ_forRenderman(
			transformationMatrixScaledZ, transform
			);
		RiConcatTransform( * const_cast< RtMatrix* >( &transformationMatrixScaledZ ) );
		return RiLightSource( "ambientlight",
			"intensity",  &intensity,
			"lightcolor", color,
			RI_NULL );
	}
	liqLightHandle Renderer::exportAmbientLight(const liqRibLightData *lightdata, const structJob &currentJob)
	{
		CM_TRACE_FUNC("Renderer::exportAmbientLight("<<lightdata->getName()<<","<<currentJob.name.asChar()<<")");
				
		const liqMatrix &t = lightdata->transformationMatrix;

		liqMatrix transformationMatrixScaledZ;
		liqRibLightData::scaleZ_forRenderman(
			transformationMatrixScaledZ, t
			);
		RiConcatTransform( * const_cast< RtMatrix* >( &transformationMatrixScaledZ ) );
		return RiLightSource( "ambientlight",
			"intensity",  &lightdata->intensity,
			"lightcolor", lightdata->color,
			RI_NULL );
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
		const liqMatrix &transform)
	{
		CM_TRACE_FUNC("Renderer::exportDistantLight("<<shadertype<<","<<shaderinstance<<",...)");

		liqMatrix transformationMatrixScaledZ;
		liqRibLightData::scaleZ_forRenderman(
			transformationMatrixScaledZ, transform
			);
		RiConcatTransform( * const_cast< RtMatrix* >( &transformationMatrixScaledZ ) );
		return RiLightSource( 
			"liquiddistant",
			"intensity",              &i_intensity,
			"lightcolor",             &i_lightcolor,
			"string shadowname",      &i_shadowname,
			"float shadowbias",       &i_shadowbias,
			"float shadowblur",       &i_shadowblur,
			"float shadowsamples",    &i_shadowsamples,
			"float shadowfiltersize", &i_shadowfiltersize,
			"color shadowcolor",      &i_shadowcolor,
			"float lightID",          &i_lightID,
			"string __category",      &o_category,

			"float __shadowF",			&o_shadowF,
			"color __shadowC",			&o_shadowC,
			"color __unshadowed_Cl",	&o_unshadowed_Cl,
			"float __nondiffuse",		&o_nondiffuse,
			"float __nonspecular",		&o_nonspecular,
			RI_NULL 
			);
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
		const liqMatrix &transform)
	{
		CM_TRACE_FUNC("Renderer::exportPointLight("<<shadertype<<","<<shaderinstance<<",...)");

		liqMatrix transformationMatrixScaledZ;
		liqRibLightData::scaleZ_forRenderman(
			transformationMatrixScaledZ, transform
			);
		RiConcatTransform( * const_cast< RtMatrix* >( &transformationMatrixScaledZ ) );
		return RiLightSource( "liquidpoint",
			"intensity",                  &i_intensity,
			"lightcolor",                 &i_lightcolor,
			"float decay",                &i_decay,
			"string shadownamepx",        &i_shadownamepx,
			"string shadownamenx",        &i_shadownamenx,
			"string shadownamepy",        &i_shadownamepy,
			"string shadownameny",        &i_shadownameny,
			"string shadownamepz",        &i_shadownamepz,
			"string shadownamenz",        &i_shadownamenz,
			"float shadowbias",           &i_shadowbias,
			"float shadowblur",           &i_shadowblur,
			"float shadowsamples",        &i_shadowsamples,
			"float shadowfiltersize",     &i_shadowfiltersize,
			"color shadowcolor",          &i_shadowcolor,
			"float lightID",              &i_lightID,
			"string __category",          &o_category,
			"float __shadowF",			&o_shadowF,
			"color __shadowC",			&o_shadowC,
			"color __unshadowed_Cl",	    &o_unshadowed_Cl,
			"float __nondiffuse",         &o_nondiffuse,
			"float __nonspecular",        &o_nonspecular,
			RI_NULL );
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
		const liqMatrix &transform)
	{
		CM_TRACE_FUNC("Renderer::exportSpotLight("<<shadertype<<","<<shaderinstance<<",...)");

		liqFloat _i_coneangle     = i_coneangle * 0.5f;
		liqFloat _i_penumbraangle = i_penumbraangle;

		liqMatrix transformationMatrixScaledZ;
		liqRibLightData::scaleZ_forRenderman(
			transformationMatrixScaledZ, transform
			);
		RiConcatTransform( * const_cast< RtMatrix* >( &transformationMatrixScaledZ ) );
		return RiLightSource( "liquidspot",
			"intensity",                    &i_intensity,
			"lightcolor",                   &i_lightcolor,
			"float coneangle",              &_i_coneangle,
			"float penumbraangle",          &_i_penumbraangle,
			"float dropoff",                &i_dropoff,
			"float decay",                  &i_decay,

			"float barndoors",              &i_barndoors,
			"float leftbarndoor",           &i_leftbarndoor,
			"float rightbarndoor",          &i_rightbarndoor,
			"float topbarndoor",            &i_topbarndoor,
			"float bottombarndoor",         &i_bottombarndoor,

			"float decayRegions",           &i_decayRegions,
			"float startDistance1",         &i_startDistance1,
			"float endDistance1",           &i_endDistance1,
			"float startDistance2",         &i_startDistance2,
			"float endDistance2",           &i_endDistance2,
			"float startDistance3",         &i_startDistance3,
			"float endDistance3",           &i_endDistance3,
			"float startDistanceIntensity1",&i_startDistanceIntensity1,
			"float endDistanceIntensity1",  &i_endDistanceIntensity1,
			"float startDistanceIntensity2",&i_startDistanceIntensity2,
			"float endDistanceIntensity2",  &i_endDistanceIntensity2,
			"float startDistanceIntensity3",&i_startDistanceIntensity3,
			"float endDistanceIntensity3",  &i_endDistanceIntensity3,

			"string shadowname",            &i_shadowname,
			"float shadowbias",             &i_shadowbias,
			"float shadowblur",             &i_shadowblur,
			"float shadowsamples",          &i_shadowsamples,
			"float shadowfiltersize",       &i_shadowfiltersize,
			"color shadowcolor",            &i_shadowcolor,
			"color shadowcolorSurf",        &i_shadowcolorsurf,
			"float shadowcolorMix",         &i_shadowcolormix,

			"float lightID",                &i_lightID,
			"string __category",            &i_category,

			"color __shadowC",              &o_shadowC,
			"float __shadowF",              &o_shadowF,
			"float __barn",				    &o_barn,
			"color __unshadowed_Cl",        &o_unshadowed_Cl,
			"float __nondiffuse",           &o_nondiffuse,
			"float __nonspecular",          &o_nonspecular,
			RI_NULL );
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
		const liqMatrix &transform)
	{
		CM_TRACE_FUNC("Renderer::exportAreaLight("<<shadertype<<","<<shaderinstance<<",...)");

		liqMatrix transformationMatrixScaledZ;
		liqRibLightData::scaleZ_forRenderman(
			transformationMatrixScaledZ, transform
			);
		RiConcatTransform( * const_cast< RtMatrix* >( &transformationMatrixScaledZ ) );
		return RiLightSource( "liquidarea",
			"float intensity",            &i_intensity,
			"color lightcolor",           &i_lightcolor,
			"float decay",                &i_decay,
			"string coordsys",            &i_coordsys,
			"float lightsamples",         &i_lightsamples,
			"float doublesided",          &i_doublesided,
			"string shadowname",          &i_shadowname,
			"color shadowcolor",          &i_shadowcolor,
			"string hitmode",             &i_hitmode,
			"string lightmap",            &i_lightmap,
			"float lightmapsaturation",   &i_lightmapsaturation,
			"float lightID",              &i_lightID,
			"string __category",          &i_category,

			"float __nonspecular",			&o_nonspecular,
			"float __shadowF",				&o_shadowF, 
			"color __shadowC",				&o_shadowC,
			"color __unshadowed_Cl",		&o_unshadowed_Cl,
			"float __arealightIntensity",	&o_arealightIntensity,
			"color __arealightColor",		&o_arealightColor,
			RI_NULL );
	}
	liqLightHandle Renderer::exportAreaLight(const liqRibLightData *lightdata, const structJob &currentJob)
	{
		CM_TRACE_FUNC("Renderer::exportAreaLight("<<lightdata->getName()<<","<<currentJob.name.asChar()<<")");
		const liqMatrix &t = lightdata->transformationMatrix;

		liqMatrix transformationMatrixScaledZ;
		liqRibLightData::scaleZ_forRenderman(
			transformationMatrixScaledZ, t
			);
		RiConcatTransform( * const_cast< RtMatrix* >( &transformationMatrixScaledZ ) );		return RiLightSource( "liquidarea_dummy", RI_NULL );
	}
	liqLightHandle Renderer::exportVolumeLight(const liqRibLightData *lightdata, const structJob &currentJob)
	{
		CM_TRACE_FUNC("Renderer::exportVolumeLight("<<lightdata->getName()<<","<<currentJob.name.asChar()<<")");
			
		return RiLightSource( "liquidvolumelight_dummy", RI_NULL );
	}
	liqLightHandle Renderer::exportUserDefinedLight(const liqRibLightData *lightdata, const structJob &currentJob)
	{
		CM_TRACE_FUNC("Renderer::exportUserDefinedLight("<<lightdata->getName()<<","<<currentJob.name.asChar()<<")");
		
		//1.export the light shader first
		assert(lightdata->rmanLightShader);
		liquidmaya::ShaderMgr::getSingletonPtr()->exportShaderNode(lightdata->rmanLightShader->getName().c_str());

		//2.refer to the light shader
		const liqMatrix &t = lightdata->transformationMatrix;

		liqMatrix transformationMatrixScaledZ;
		liqRibLightData::scaleZ_forRenderman(
			transformationMatrixScaledZ, t
			);
		RiConcatTransform( * const_cast< RtMatrix* >( &transformationMatrixScaledZ ) );		RiArchiveRecord(RI_COMMENT, "refer to the user-defined light shader [%s]", lightdata->rmanLightShader->getName().c_str(), RI_NULL );
		RiArchiveRecord(RI_VERBATIM, "LightSource \"%s\" %d \n", 
			lightdata->rmanLightShader->getName().c_str(), 
			lightdata->rmanLightShader->shaderHandler.asUnsigned());
		//lightdata->rmanLightShader->write();
		
#ifdef RIBLIB_AQSIS
		return reinterpret_cast<RtLightHandle>(static_cast<ptrdiff_t>(lightdata->rmanLightShader->shaderHandler.asInt()));
#else
		/* !!!! In Generic libRib light handle is unsigned int */
		LIQDEBUGPRINTF( "-> assigning light handle: " );
		return (const RtLightHandle)(const void *)( lightdata->rmanLightShader->shaderHandler.asUnsigned() );
		//LIQDEBUGPRINTF( "%u\n", (unsigned int)(long)(const void *)handle );
#endif
	}

	//////////////////////////////////////////////////////////////////////////
	static void _write(liqRibLightData* pData, const structJob &currentJob);
	//
	void Renderer::write(
		liqRibLightData* pData,
		const MString &ribFileName,
		const structJob &currentJob,
		const bool bReference)
	{
		CM_TRACE_FUNC("Renderer::write("<<pData->getFullPathName()<<","<<ribFileName.asChar()<<","<<currentJob.name.asChar()<<",ref="<<bReference<<")");

		assert(liqglo.m_ribFileOpen &&"rm_writeLightData.cpp");


		if( !bReference ){//write data at first time
			assert(pData->getRibFileFullPath().length()==0&&"rm_writeLightData.cpp");
			pData->setRibFileFullPath(ribFileName);

			renderman::Helper o;
			o.RiBeginRef(pData->getRibFileFullPath().asChar());
			_write(pData, currentJob);
			o.RiEndRef();

		}else{
			//write the reference
			assert(pData->getRibFileFullPath() == ribFileName);
			RiReadArchive( const_cast< RtToken >( pData->getRibFileFullPath().asChar() ), NULL, RI_NULL );
		}
		
	}
	//
	static void _write(liqRibLightData* pData, const structJob &currentJob__)
	{
		CM_TRACE_FUNC("rm_writeLightData.cpp::_write("<<pData->getFullPathName()<<","<<currentJob__.name.asChar()<<",...)");

		
		if ( !pData->excludeFromRib ) 
		{
		LIQDEBUGPRINTF( "-> writing light %s \n", pData->lightName.asChar());

		//RiConcatTransform( * const_cast< RtMatrix* >( &transformationMatrix ) );
		if ( liqglo.liqglo_isShadowPass ) 
		{
			if ( pData->usingShadow ) 
			{
			RtString sName( const_cast< char* >( pData->shadowName.asChar() ) );
			// Hmmmmm got to set a LIQUIDHOME env var and use it ...
			// May be set relative name shadowPassLight and resolve path with RIB searchpath
			// Moritz: solved through default shader searchpath in liqRibTranslator
			pData->handle = liquid::RendererMgr::getInstancePtr()->
				getRenderer()->exportShadowPassLight("??", pData->lightName.asChar(), sName, pData->transformationMatrix);
			}
		} 
		else 
		{
			RtString cat( const_cast< char* >( pData->lightCategory.asChar() ) );

			switch ( pData->lightType ) 
			{
			case MRLT_Ambient:
				pData->handle = liquid::RendererMgr::getInstancePtr()->
					getRenderer()->exportAmbientLight(pData, currentJob__);
				break;
        
			case MRLT_Distant:
				{
					liqFloat    i_intensity = 1;
					liqColor i_lightcolor;	setColor(i_lightcolor, 1.0);
					assert(i_lightcolor[0] == 1.0f);
					assert(i_lightcolor[1] == 1.0f);
					assert(i_lightcolor[2] == 1.0f);
					liqString i_shadowname = "";  /* shadow map name or "raytrace" for traced shadows */
					liqFloat i_shadowbias = 0.01;
					liqFloat i_shadowblur = 0.0;
					liqFloat i_shadowsamples = 16; /* samples or rays */
					liqFloat i_shadowfiltersize = 1;
					liqColor i_shadowcolor;	setColor(i_shadowcolor, 0.0);
					liqFloat  i_lightID=0;
					liqString o_category="";
					liqFloat o_shadowF = 0;
					liqColor o_shadowC;	setColor(o_shadowC, 0.0);
					liqColor o_unshadowed_Cl;	setColor(o_unshadowed_Cl, 0.0);
					liqFloat o_nondiffuse = 0;  /* set to 1 to exclude from diffuse light */
					liqFloat o_nonspecular = 0; /* set to 1 to exclude from highlights */

					if ( liqglo.liqglo_doShadows && pData->usingShadow ) 
					{
						i_intensity = pData->intensity;
						setColor(i_lightcolor, pData->color);
						i_shadowname  = const_cast< char* >( pData->shadowName.asChar() );
						i_shadowfiltersize = pData->rayTraced ? pData->shadowRadius : pData->shadowFilterSize;
						i_shadowbias = pData->shadowBias;
						i_shadowsamples = pData->shadowSamples;
						i_shadowblur = pData->shadowBlur;
						setColor(i_shadowcolor, pData->shadowColor);
						o_nondiffuse = pData->nonDiffuse;
						o_nonspecular = pData->nonSpecular;
						o_category = cat;
						i_lightID = pData->lightID;
					}else{
						i_intensity = pData->intensity;
						setColor(i_lightcolor, pData->color);
						setColor(i_shadowcolor, pData->shadowColor);
						o_nondiffuse = pData->nonDiffuse;
						o_nonspecular = pData->nonSpecular;
						o_category = cat;
						i_lightID = pData->lightID;
					}
					pData->handle = liquid::RendererMgr::getInstancePtr()->
						getRenderer()->exportDistantLight(
						"distantlight", 
						pData->lightName.asChar(),  
						i_intensity,
						i_lightcolor,
						i_shadowname,
						i_shadowbias,
						i_shadowblur,
						i_shadowsamples,
						i_shadowfiltersize,
						i_shadowcolor,
						i_lightID,
						o_category,
						o_shadowF,
						o_shadowC,
						o_unshadowed_Cl,
						o_nondiffuse,
						o_nonspecular,
						pData->transformationMatrix
						);
				}
				break;
        
			case MRLT_Point:
				{
						liqFloat    i_intensity = 1;
						liqColor i_lightcolor;	setColor(i_lightcolor, 1);
						liqFloat i_decay = 0;
						liqString i_shadownamepx = "";
						liqString i_shadownamenx = "";
						liqString i_shadownamepy = "";
						liqString i_shadownameny = "";
						liqString i_shadownamepz = "";
						liqString i_shadownamenz = "";
						liqFloat i_shadowbias = 0.01;
						liqFloat i_shadowblur = 0.0;
						liqFloat i_shadowsamples = 16; /* samples or rays */
						liqFloat i_shadowfiltersize = 1.0;
						liqColor i_shadowcolor;	setColor(i_shadowcolor, 0.0);
						liqFloat  i_lightID=0;
						liqString o_category="";
						liqFloat	o_shadowF = 0;
						liqColor	o_shadowC;setColor(o_shadowC, 1.0);
						liqColor	o_unshadowed_Cl;setColor(o_unshadowed_Cl, 0.0);
						liqFloat o_nondiffuse = 0;  /* set to 1 to exclude from diffuse light */
						liqFloat o_nonspecular = 0; /* set to 1 to exclude from highlights */

					if ( liqglo.liqglo_doShadows && pData->usingShadow ) 
					{
						liqFloat    i_intensity = pData->intensity;
						setColor(i_lightcolor, pData->color);
						i_decay = pData->decay;
						{
							MString	px = pData->rayTraced ? "raytrace" : pData->autoShadowName( pPX );
							MString	nx = pData->rayTraced ? "raytrace" : pData->autoShadowName( pNX );
							MString	py = pData->rayTraced ? "raytrace" : pData->autoShadowName( pPY );
							MString	ny = pData->rayTraced ? "raytrace" : pData->autoShadowName( pNY );
							MString	pz = pData->rayTraced ? "raytrace" : pData->autoShadowName( pPZ );
							MString	nz = pData->rayTraced ? "raytrace" : pData->autoShadowName( pNZ );
							i_shadownamepx = const_cast<char*>( px.asChar() );
							i_shadownamenx = const_cast<char*>( nx.asChar() );
							i_shadownamepy = const_cast<char*>( py.asChar() );
							i_shadownameny = const_cast<char*>( ny.asChar() );
							i_shadownamepz = const_cast<char*>( pz.asChar() );
							i_shadownamenz = const_cast<char*>( nz.asChar() );
						}
						i_shadowbias = pData->shadowBias;
						i_shadowblur = pData->shadowBlur;
						i_shadowsamples = pData->shadowSamples; /* samples or rays */
						i_shadowfiltersize = pData->rayTraced ? pData->shadowRadius : pData->shadowFilterSize;
						setColor(i_shadowcolor, pData->shadowColor);
						i_lightID = pData->lightID;
						o_category= cat;
						o_nondiffuse = pData->nonDiffuse;  /* set to 1 to exclude from diffuse light */
						o_nonspecular = pData->nonSpecular; /* set to 1 to exclude from highlights */
					}else{
						i_intensity = pData->intensity;
						setColor(i_lightcolor, pData->color);
						i_decay = pData->decay;
						setColor(i_shadowcolor, pData->shadowColor);
						i_lightID = pData->lightID;					
						o_category= cat;
						o_nondiffuse = pData->nonDiffuse;  /* set to 1 to exclude from diffuse light */
						o_nonspecular = pData->nonSpecular; /* set to 1 to exclude from highlights */
					}
					pData->handle = liquid::RendererMgr::getInstancePtr()->
						getRenderer()->exportPointLight(
						"pointlight", 
						pData->lightName.asChar(),  
						i_intensity,
						i_lightcolor,
						i_decay,
						i_shadownamepx,
						i_shadownamenx,
						i_shadownamepy,
						i_shadownameny,
						i_shadownamepz,
						i_shadownamenz,
						i_shadowbias,
						i_shadowblur,
						i_shadowsamples,
						i_shadowfiltersize,
						i_shadowcolor,
						i_lightID,
						o_category,
						o_shadowF,
						o_shadowC,
						o_unshadowed_Cl,
						o_nondiffuse,
						o_nonspecular,
						pData->transformationMatrix
						);
				}
				break;
			case MRLT_Spot:
				{
					liqFloat i_intensity = 1;
					liqColor i_lightcolor;	setColor(i_lightcolor, 1.0);
					liqFloat i_coneangle               = 40;
					liqFloat i_penumbraangle           = 0;
					liqFloat i_dropoff                 = 0;
					liqFloat i_decay                   = 0;

					liqFloat i_barndoors               = 0;
					liqFloat i_leftbarndoor            = 10;
					liqFloat i_rightbarndoor           = 10;
					liqFloat i_topbarndoor             = 10;
					liqFloat i_bottombarndoor          = 10;

					liqFloat i_decayRegions            = 0;
					liqFloat i_startDistance1          = 1.0;
					liqFloat i_endDistance1            = 2.0;
					liqFloat i_startDistance2          = 3.0;
					liqFloat i_endDistance2            = 6.0;
					liqFloat i_startDistance3          = 8.0;
					liqFloat i_endDistance3            = 10.0;
					liqFloat i_startDistanceIntensity1 = 1.0;
					liqFloat i_endDistanceIntensity1   = 1.0;
					liqFloat i_startDistanceIntensity2 = 1.0;
					liqFloat i_endDistanceIntensity2   = 1.0;
					liqFloat i_startDistanceIntensity3 = 1.0;
					liqFloat i_endDistanceIntensity3   = 1.0;

					liqString i_shadowname       = "";
					liqFloat  i_shadowbias       = 0.01;
					liqFloat  i_shadowblur       = 0.0;
					liqFloat  i_shadowsamples    = 32;
					liqFloat  i_shadowfiltersize = 1;
					liqColor  i_shadowcolor;setColor(i_shadowcolor, 0.0f);
					liqColor  i_shadowcolorsurf;setColor(i_shadowcolorsurf, 0.0f);
					liqFloat  i_shadowcolormix  = -1;

					liqFloat  i_lightID          = 0;
					liqString i_category       = "";

					liqColor o_shadowC;setColor(o_shadowC, 0.0f);
					liqFloat o_shadowF        = 0;
					liqFloat o_barn           = 0;
					liqColor o_unshadowed_Cl;setColor(o_unshadowed_Cl, 0.0f);
					liqFloat o_nondiffuse     = 0;
					liqFloat o_nonspecular    = 0;

						if (liqglo.liqglo_doShadows && pData->usingShadow) 
						{
						/* if ( ( shadowName == "" ) || ( shadowName.substring( 0, 9 ) == "autoshadow" ) ) {
							shadowName = liqglo_texDir + autoShadowName();
						} */
						i_intensity = pData->intensity;
						setColor(i_lightcolor, pData->color);
						i_coneangle = pData->coneAngle;
						i_penumbraangle = pData->penumbraAngle;
						i_dropoff = pData->dropOff;
						i_decay = pData->decay;
						i_barndoors = pData->barnDoors;
						i_leftbarndoor = pData->leftBarnDoor;
						i_rightbarndoor = pData->rightBarnDoor;
						i_topbarndoor = pData->topBarnDoor;
						i_bottombarndoor = pData->bottomBarnDoor;
						i_decayRegions = pData->decayRegions;
						i_startDistance1 = pData->startDistance1;
						i_endDistance1   = pData->endDistance1;
						i_startDistance2 = pData->startDistance2;
						i_endDistance2   = pData->endDistance2;
						i_startDistance3 = pData->startDistance3;
						i_endDistance3   = pData->endDistance3;
						i_startDistanceIntensity1 = pData->startDistanceIntensity1;
						i_endDistanceIntensity1   = pData->endDistanceIntensity1;
						i_startDistanceIntensity2 = pData->startDistanceIntensity2;
						i_endDistanceIntensity2   = pData->endDistanceIntensity2;
						i_startDistanceIntensity3 = pData->startDistanceIntensity3;
						i_endDistanceIntensity3   = pData->endDistanceIntensity3;
						i_shadowname = const_cast< char* >( pData->shadowName.asChar() );
						i_shadowfiltersize = pData->rayTraced ? pData->shadowRadius : pData->shadowFilterSize;
						i_shadowbias = pData->shadowBias;
						i_shadowsamples = pData->shadowSamples;
						i_shadowblur = pData->shadowBlur;
						setColor(i_shadowcolor, pData->shadowColor);
						o_nondiffuse = pData->nonDiffuse;
						o_nonspecular = pData->nonSpecular;
						i_category = cat;
						i_lightID = pData->lightID;
					} 
					else 
					{
							i_intensity = pData->intensity;
							setColor(i_lightcolor, pData->color);
							setColor(i_shadowcolor, pData->shadowColor);
							i_coneangle = pData->coneAngle;
							i_penumbraangle = pData->penumbraAngle;
							i_dropoff = pData->dropOff;
							i_decay = pData->decay;
							i_barndoors = pData->barnDoors;
							i_leftbarndoor = pData->leftBarnDoor;
							i_rightbarndoor = pData->rightBarnDoor;
							i_topbarndoor = pData->topBarnDoor;
							i_bottombarndoor = pData->bottomBarnDoor;
							i_decayRegions = pData->decayRegions;
							i_startDistance1 = pData->startDistance1;
							i_endDistance1   = pData->endDistance1;
							i_startDistance2 = pData->startDistance2;
							i_endDistance2   = pData->endDistance2;
							i_startDistance3 = pData->startDistance3;
							i_endDistance3   = pData->endDistance3;
							i_startDistanceIntensity1 = pData->startDistanceIntensity1;
							i_endDistanceIntensity1   = pData->endDistanceIntensity1;
							i_startDistanceIntensity2 = pData->startDistanceIntensity2;
							i_endDistanceIntensity2   = pData->endDistanceIntensity2;
							i_startDistanceIntensity3 = pData->startDistanceIntensity3;
							i_endDistanceIntensity3   = pData->endDistanceIntensity3;
							i_shadowname = const_cast< char* >( pData->shadowName.asChar() );
							i_shadowbias = pData->shadowBias;
							i_shadowsamples = pData->shadowSamples;
							o_nondiffuse = pData->nonDiffuse;
							o_nonspecular = pData->nonSpecular;
							i_category = cat;
							i_lightID = pData->lightID;
						}
						pData->handle = liquid::RendererMgr::getInstancePtr()->
							getRenderer()->exportSpotLight(
							"spotlight", 
							pData->lightName.asChar(),
							i_intensity,
							i_lightcolor,
							i_coneangle,
							i_penumbraangle,
							i_dropoff,
							i_decay,

							i_barndoors,
							i_leftbarndoor,
							i_rightbarndoor,
							i_topbarndoor,
							i_bottombarndoor,

							i_decayRegions,
							i_startDistance1,
							i_endDistance1,
							i_startDistance2,
							i_endDistance2,
							i_startDistance3,
							i_endDistance3,
							i_startDistanceIntensity1,
							i_endDistanceIntensity1,
							i_startDistanceIntensity2,
							i_endDistanceIntensity2,
							i_startDistanceIntensity3,
							i_endDistanceIntensity3,

							i_shadowname,
							i_shadowbias,
							i_shadowblur,
							i_shadowsamples,
							i_shadowfiltersize,
							i_shadowcolor,
							i_shadowcolorsurf,
							i_shadowcolormix,

							i_lightID,
							i_category,

							o_shadowC,
							o_shadowF,
							o_barn,
							o_unshadowed_Cl,
							o_nondiffuse,
							o_nonspecular,
							pData->transformationMatrix
							);

				  
				}
				break;
			case MRLT_Rman: 
			{
				/*
				unsigned numTokens( tokenPointerArray.size() );
				scoped_array< RtToken > tokenArray( new RtToken[ numTokens ] );
				scoped_array< RtPointer > pointerArray( new RtPointer[ numTokens ] );
				assignTokenArraysV( tokenPointerArray, tokenArray.get(), pointerArray.get() );

				if ( liqglo_shortShaderNames ) 
				assignedRManShader = basename( const_cast< char* >( assignedRManShader.asChar() ) );
         
				RtString shaderName = const_cast< RtString >( assignedRManShader.asChar() );
				handle = RiLightSourceV( shaderName, numTokens, tokenArray.get(), pointerArray.get() );
				*/
				
				pData->handle = liquid::RendererMgr::getInstancePtr()->
					getRenderer()->exportUserDefinedLight(pData, currentJob__);
				break;
			}
			case MRLT_Area: 
			{
				liqFloat   i_intensity     = 1.0;
				liqColor   i_lightcolor;	setColor(i_lightcolor, 1.0);
				liqFloat   i_decay         = 2;
				liqString  i_coordsys      = "";
				liqFloat   i_lightsamples  = 32;
				liqFloat   i_doublesided   = 0;
				liqString  i_shadowname    = "";
				liqColor   i_shadowcolor;	setColor(i_lightcolor, 0.0);
				liqString  i_hitmode       = "primitive";

				liqString  i_lightmap      = "";
				liqFloat   i_lightmapsaturation  = 2.0;

				liqFloat  i_lightID        = 0;
				liqString i_category       = "";

				liqFloat o_nonspecular          = 1;
				liqFloat o_shadowF              = 0;
				liqColor o_shadowC;	setColor(o_shadowC, 0.0);
				liqColor o_unshadowed_Cl;	setColor(o_unshadowed_Cl, 0.0);
				liqFloat o_arealightIntensity   = 0;
				liqColor o_arealightColor;	setColor(o_arealightColor, 0.0);



				i_coordsys = const_cast< char* >(MString(pData->lightName+"CoordSys").asChar());
				RtString areacoordsys = i_coordsys;

		//           MString areashader( getenv("LIQUIDHOME") );
		//           areashader += "/lib/shaders/liquidarea";

				RtString rt_hitmode;
				switch( pData->hitmode ) 
				{
				case 1:
					rt_hitmode = const_cast< char* >( "primitive" );
					break;
				case 2:
					rt_hitmode = const_cast< char* >( "shader" );
					break;
				default:
					rt_hitmode = const_cast< char* >( "default" );
					break;
				}


				// if raytraced shadows are off, we get a negative value, so we correct it here.
				RtString rt_lightmap( const_cast< char* >( pData->lightMap.asChar() ) );
		 
				{
					i_intensity = pData->intensity;
					setColor(i_lightcolor, pData->color);
					i_decay = pData->decay;
					i_coordsys = areacoordsys;
					i_lightsamples = pData->shadowSamples;
					i_doublesided = pData->bothSidesEmit;
					i_shadowname = const_cast< char* >( pData->shadowName.asChar() );
					setColor(i_shadowcolor, pData->shadowColor);
					i_lightmap = rt_lightmap;
					i_lightID = pData->lightID;
					i_hitmode = rt_hitmode;
					i_category = cat;
				}

				pData->handle = liquid::RendererMgr::getInstancePtr()->
					getRenderer()->exportAreaLight(
					"arealight", 
					pData->lightName.asChar(),
					i_intensity,
					i_lightcolor,
					i_decay,
					i_coordsys,
					i_lightsamples,
					i_doublesided,
					i_shadowname,
					i_shadowcolor,
					i_hitmode,
					i_lightmap,
					i_lightmapsaturation,
					i_lightID,
					i_category,

					o_nonspecular,
					o_shadowF, 
					o_shadowC,
					o_unshadowed_Cl,
					o_arealightIntensity,
					o_arealightColor,
					pData->transformationMatrix
					);
				break;
			}
			case MRLT_Volume:
				{
					pData->handle = liquid::RendererMgr::getInstancePtr()->
						getRenderer()->exportVolumeLight(pData, currentJob__);
					break;
				}
			case MRLT_Unknown: {
				RiConcatTransform( * const_cast< RtMatrix* >( &pData->transformationMatrix ) );
				break;
			}
			}
		}
		}
	}
}//namespace renderman