#include <liqConfig.h>
#ifdef _USE_RENDERMAN_

#include "rm_helper.h"

#include "../common/prerequest_maya.h"
#include "liqGlobalHelpers.h"
#include <liqRibTranslator.h>
#include "ri_interface.h"

namespace renderman
{
	Helper::Helper()
	{

	}
	//
	Helper::Helper(const std::string& ribFile)
		:m_ribFile(ribFile)
	{

	}
	Helper::~Helper()
	{

	}
	//
	void Helper::RiBeginRef(const std::string& ribFile)
	{
		CM_TRACE_FUNC("Helper::RiBeginRef("<<ribFile<<")");

		m_ribFile = ribFile;

		//if m_ribFileFullPath is "", write the data into the current rib file.
		if( m_ribFile != "" ){
			//log
			RiArchiveRecord( RI_COMMENT, "output: %s", m_ribFile.c_str() );
			//write the data into another rib file.
			m_contex = RiGetContext();//push context
			liquidMessage("output rib: "+ MString(m_ribFile.c_str()) , messageInfo);
			RiBegin_liq( const_cast< RtToken >( m_ribFile.c_str() ) );
		}
	}
	//
	void Helper::RiEndRef()
	{
		CM_TRACE_FUNC("Helper::RiEndRef()");

		if( m_ribFile != "" ){
			RiEnd();
			RiContext(m_contex);//pop context
		}
	}
	//-------------------------------------------------------
	//
	//-------------------------------------------------------
	RibDataExportHelper::RibDataExportHelper(){}
	RibDataExportHelper::~RibDataExportHelper(){}
	//-------------------------------------------------------
	//
	//-------------------------------------------------------
	MString getShadingGroupFilePath(const MString &shadingGroupNodeName)
	{
		CM_TRACE_FUNC("getShadingGroupFilePath("<<shadingGroupNodeName.asChar()<<")");
		return getShaderDirectory()+sanitizeNodeName(shadingGroupNodeName+".rmsg");
	}
	MString getShaderFilePath_NoExt(const MString &shaderNodeName)
	{
		CM_TRACE_FUNC("getShaderFilePath_NoExt("<<shaderNodeName.asChar()<<")");
		return getShaderDirectory()+sanitizeNodeName(shaderNodeName);
	}
	MString getShaderFilePath_SRC(const MString &shaderNodeName)
	{
		CM_TRACE_FUNC("getShaderFilePath_SRC("<<shaderNodeName.asChar()<<")");
		return getShaderFilePath_NoExt(shaderNodeName)+".sl_my";
	}
	MString getShaderFilePath_SLO(const MString &shaderNodeName)
	{
		CM_TRACE_FUNC("getShaderFilePath_SLO("<<shaderNodeName.asChar()<<")");
		return getShaderFilePath_NoExt(shaderNodeName)+"."+liqglo.liquidRenderer.shaderExtension;
	}
	//----------------------------------------------------------------------
	void exportJobCamera(const structJob &job, const structCamera camera[])
	{
		CM_TRACE_FUNC("exportJobCamera("<<job.name.asChar()<<","<<",)");

		if ( camera[0].isOrtho )
		{
			liqFloat frameWidth, frameHeight;
			// the whole frame width has to be scaled according to the UI Unit
			frameWidth  = camera[0].orthoWidth  * 0.5 ;
			frameHeight = camera[0].orthoHeight * 0.5 ;
			RiProjection( "orthographic", RI_NULL );
			// if we are describing a shadow map camera,
			// we need to set the screenwindow to the default,
			// as shadow maps are always square.
			if( job.pass == rpShadowMap ) 
				RiScreenWindow( -frameWidth, frameWidth, -frameHeight, frameHeight );
			else 			                 
				RiScreenWindow( -1.0, 1.0, -1.0, 1.0 );
		}
		else
		{
			liqFloat fieldOfView = camera[0].hFOV * 180.0 / M_PI ;
			if ( job.pass == rpShadowMap && job.isPoint ) fieldOfView = job.camera[0].hFOV;

			RiProjection( "perspective", RI_FOV, &fieldOfView, RI_NULL );

			// if we are describing a shadow map camera,
			// we need to set the screenwindow to the default,
			// as shadow maps are always square.

			if ( job.pass == rpShadowMap == false )
			{
				double ratio = (double)job.width / (double)job.height;
				double left, right, bottom, top;
				if ( ratio <= 0 )
				{
					left    = -1 + camera[0].horizontalFilmOffset;
					right   =  1 + camera[0].horizontalFilmOffset;
					bottom  = -1 / ratio + camera[0].verticalFilmOffset;
					top     =  1 / ratio + camera[0].verticalFilmOffset;
				}
				else
				{
					left    = -ratio + camera[0].horizontalFilmOffset;
					right   =  ratio + camera[0].horizontalFilmOffset;
					bottom  = -1 + camera[0].verticalFilmOffset;
					top     =  1 + camera[0].verticalFilmOffset;
				}
				RiScreenWindow( left, right, bottom, top );
			}
			else
			{
				RiScreenWindow( -1.0, 1.0, -1.0, 1.0 );
			}
		}
		RiClipping( camera[0].neardb, camera[0].fardb );
		if ( liqglo.doDof && job.pass != rpShadowMap ) 
			RiDepthOfField( camera[0].fStop, camera[0].focalLength, camera[0].focalDistance );
		// if we motion-blur the cam, open the motion block
		//
		if ( liqglo.doCameraMotion && ( job.pass != rpShadowMap || job.shadowType == stDeep) )
		{
			if ( liqglo.liqglo_relativeMotion ) 
				RiMotionBeginV( liqglo.liqglo_motionSamples, liqglo.liqglo_sampleTimesOffsets );
			else 		                     
				RiMotionBeginV( liqglo.liqglo_motionSamples, liqglo.liqglo_sampleTimes );
		}

		// write the camera transform
		//
		liqMatrix cameraMatrix;
		camera[0].mat.get( cameraMatrix );
		RiTransform( cameraMatrix );

		// if we motion-blur the cam, write the subsequent motion samples and close the motion block
		//
		if ( liqglo.doCameraMotion && ( job.pass != rpShadowMap || job.shadowType == stDeep ) )
		{
			int mm = 1;
			while ( mm < liqglo.liqglo_motionSamples )
			{
				camera[mm].mat.get( cameraMatrix );
				RiTransform( cameraMatrix );
				++mm;
			}
			RiMotionEnd();
		}
	}
	MStatus MaxtrixMotionBlur(const liqRibNodePtr ribNode__, MDagPath &path__, const bool bMotionBlur)
	{
		CM_TRACE_FUNC("MaxtrixMotionBlur("<<ribNode__->name.asChar()<<","<<path__.fullPathName().asChar()<<","<<bMotionBlur<<")");
		MMatrix matrix;

		//refactor 30 from liqRibTranslator::objectBlock()
		if( bMotionBlur)
		{
			LIQDEBUGPRINTF( "-> writing matrix motion blur data\n" );
			// Moritz: replaced RiMotionBegin call with ..V version to allow for more than five motion samples
			if(liqglo.liqglo_relativeMotion)
				RiMotionBeginV( liqglo.liqglo_motionSamples, liqglo.liqglo_sampleTimesOffsets );
			else
				RiMotionBeginV( liqglo.liqglo_motionSamples, liqglo.liqglo_sampleTimes );
		}
#if 1
		liqMatrix ribMatrix;
		matrix = ribNode__->object( 0 )->matrix( path__.instanceNumber() );
		matrix.get( ribMatrix );

		if( liqglo.liqglo_relativeTransforms ) 
			RiConcatTransform( ribMatrix );
		else 
			RiTransform( ribMatrix );
#elif 0  // Bat : a way to have double transforms :
		double doubleTransformMatrix[4][4];
		matrix = ribNode->object( 0 )->matrix( path.instanceNumber() );
		matrix.get( doubleTransformMatrix );

		int txIntPart = (int)(doubleTransformMatrix[3][0]);
		float txFloatPart = doubleTransformMatrix[3][0] - txIntPart;

		int tyIntPart = (int)(doubleTransformMatrix[3][1]);
		float tyFloatPart = doubleTransformMatrix[3][1] - tyIntPart;

		int tzIntPart = (int)(doubleTransformMatrix[3][2]);
		float tzFloatPart = doubleTransformMatrix[3][2] - tzIntPart;

		liqFloat floatTransformMatrixWithIntegerTranslatePart[4][4];
		matrix.get( floatTransformMatrixWithIntegerTranslatePart );
		liqFloat floatIdentityMatrixWithFloatingTranslatePart[4][4] = { {1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1} };

		floatTransformMatrixWithIntegerTranslatePart[3][0] = txIntPart;
		floatTransformMatrixWithIntegerTranslatePart[3][1] = tyIntPart;
		floatTransformMatrixWithIntegerTranslatePart[3][2] = tzIntPart;

		floatIdentityMatrixWithFloatingTranslatePart[3][0] = txFloatPart;
		floatIdentityMatrixWithFloatingTranslatePart[3][1] = tyFloatPart;
		floatIdentityMatrixWithFloatingTranslatePart[3][2] = tzFloatPart;

		if( liqglo_relativeTransforms )
		{
			RiConcatTransform( floatIdentityMatrixWithFloatingTranslatePart );
			RiConcatTransform( floatTransformMatrixWithIntegerTranslatePart );
		}
		else
		{
			RiTransform( floatIdentityMatrixWithFloatingTranslatePart );
			RiConcatTransform( floatTransformMatrixWithIntegerTranslatePart );
		}
#endif
		// Output the world matrices for the motionblur
		// This will override the current transformation setting
		if( bMotionBlur)
		{
			path__ = ribNode__->path();
			liqMatrix ribMatrix;
			for( unsigned mm( 1 ); mm < liqglo.liqglo_motionSamples; mm++ ) {
				matrix = ribNode__->object( mm )->matrix( path__.instanceNumber() );
				matrix.get( ribMatrix );
				if( liqglo.liqglo_relativeTransforms ) 
					RiConcatTransform( ribMatrix );
				else 
					RiTransform( ribMatrix );
			}
			RiMotionEnd();
		}
		//refactor 30 end
		return MS::kSuccess;
	}
	//
	MStatus objectNonShadowAttribute(const liqRibNodePtr &ribNode__)
	{
		CM_TRACE_FUNC("objectNonShadowAttribute("<<ribNode__->name.asChar()<<")");

		liqString mode;
		liqInt off( 0 );
		liqInt on( 1 );
		if ( !liqRibTranslator::getInstancePtr()->m_skipShadingAttributes )
		{
			if( !ribNode__->shading.diceRasterOrient ) 
				RiAttribute( "dice", (liqToken) "rasterorient", &off, RI_NULL );
			if( ribNode__->shading.doubleShaded ) 
				RiAttribute( "sides", (liqToken) "doubleshaded", &on, RI_NULL );
		}
		if( !liqRibTranslator::getInstancePtr()->m_skipRayTraceAttributes )
		{
			if( liqglo.liquidRenderer.supports_RAYTRACE ) 
			{
				if( ribNode__->trace.sampleMotion ) 
					RiAttribute( "trace", (liqToken) "samplemotion", &on, RI_NULL );
				if( ribNode__->trace.displacements ) 
					RiAttribute( "trace", (liqToken) "displacements", &on, RI_NULL );
				if( ribNode__->trace.bias != 0.01f ) 
				{
					liqFloat bias( ribNode__->trace.bias );
					RiAttribute( "trace", (liqToken) "bias", &bias, RI_NULL );
				}
				if( ribNode__->trace.maxDiffuseDepth != 1 ) 
				{
					liqInt mddepth( ribNode__->trace.maxDiffuseDepth );
					RiAttribute( "trace", (liqToken) "maxdiffusedepth", &mddepth, RI_NULL );
				}
				if( ribNode__->trace.maxSpecularDepth != 2 ) 
				{
					liqInt msdepth( ribNode__->trace.maxSpecularDepth );
					RiAttribute( "trace", (liqToken) "maxspeculardepth", &msdepth, RI_NULL );
				}
			}//if( liqglo.liquidRenderer.supports_RAYTRACE ) 
		}//if( !m_skipRayTraceAttributes )
		if( !ribNode__->visibility.camera ) 
			RiAttribute( "visibility", (liqToken) "camera", &off, RI_NULL );
#ifdef GENERIC_RIBLIB      
		extern int useAdvancedVisibilityAttributes;
		useAdvancedVisibilityAttributes = false;
#endif
		// old-style raytracing visibility support
		// philippe: if raytracing is off in the globals, trace visibility is turned off for all objects, transmission is set to TRANSPARENT for all objects
		if ( liqglo.liquidRenderer.supports_RAYTRACE )
		{
			if ( !liqglo.liquidRenderer.supports_ADVANCED_VISIBILITY ) 
			{
				if(liqglo.rt_useRayTracing)
				{
					if ( ribNode__->visibility.trace ) 
						RiAttribute( "visibility", (liqToken) "trace", &on, RI_NULL );
					else                             
						RiAttribute( "visibility", (liqToken) "trace", &off, RI_NULL );

					if( ribNode__->visibility.transmission != liqRibNode::visibility::TRANSMISSION_TRANSPARENT ) 
					{
						liqString trans;
						switch( ribNode__->visibility.transmission ) 
						{
						case liqRibNode::visibility::TRANSMISSION_OPAQUE:
							trans = "opaque";
							break;
						case liqRibNode::visibility::TRANSMISSION_OS:
							trans = "Os";
							break;
						case liqRibNode::visibility::TRANSMISSION_SHADER:
						default:
							trans = "shader";
						}
						RiAttribute( "visibility", (liqToken) "string transmission", &trans, RI_NULL );
					}
				}

			}//if ( !liqglo.liquidRenderer.supports_ADVANCED_VISIBILITY ) 
			else // philippe : prman 12.5 visibility support
			{
#ifdef GENERIC_RIBLIB         
				useAdvancedVisibilityAttributes = true;
#endif
				if ( liqglo.rt_useRayTracing )
				{
					if( !liqRibTranslator::getInstancePtr()->m_skipVisibilityAttributes )
					{
						if(  ribNode__->visibility.diffuse ) 
							RiAttribute( "visibility", (liqToken) "int diffuse", &on, RI_NULL );

						if(  ribNode__->visibility.specular ) 
							RiAttribute( "visibility", (liqToken) "int specular", &on, RI_NULL );

						if(  ribNode__->visibility.newtransmission ) 
							RiAttribute( "visibility", (liqToken) "int transmission", &on, RI_NULL );

						//if( ribNode__->visibility.camera ) 
						//	RiAttribute( "visibility", (liqToken) "int camera", &on, RI_NULL );

						//if(  ribNode__->visibility.photon ) 
						//	RiAttribute( "visibility", (liqToken) "int photon", &on, RI_NULL );

						//if( ribNode__->visibility.midpoint ) 
						//	RiAttribute( "visibility", (liqToken) "int midpoint", &on, RI_NULL );
					}

					if( !liqRibTranslator::getInstancePtr()->m_skipRayTraceAttributes )
					{
						if( liqglo.rt_useRayTracing && ribNode__->hitmode.diffuse != liqRibNode::hitmode::DIFFUSE_HITMODE_PRIMITIVE ) 
						{
							switch( ribNode__->hitmode.diffuse ) 
							{
							case liqRibNode::hitmode::DIFFUSE_HITMODE_SHADER:
								mode = "shader";
								break;
							case liqRibNode::hitmode::DIFFUSE_HITMODE_CACHE:
								mode = "cache";
								break;
							case liqRibNode::hitmode::DIFFUSE_HITMODE_PRIMITIVE:
							default:
								mode = "primitive";
							}
							RiAttribute( "shade", (liqToken) "string diffusehitmode", &mode, RI_NULL );
						}

						if( ribNode__->hitmode.specular != liqRibNode::hitmode::SPECULAR_HITMODE_SHADER ) 
						{
							switch( ribNode__->hitmode.specular ) 
							{
							case liqRibNode::hitmode::SPECULAR_HITMODE_PRIMITIVE:
								mode = "primitive";
								break;
							case liqRibNode::hitmode::SPECULAR_HITMODE_CACHE:
								mode = "cache";
								break;
							case liqRibNode::hitmode::SPECULAR_HITMODE_SHADER:
							default:
								mode = "shader";
							}
							RiAttribute( "shade", (liqToken) "string specularhitmode", &mode, RI_NULL );
						}

						if( ribNode__->hitmode.transmission != liqRibNode::hitmode::TRANSMISSION_HITMODE_SHADER ) 
						{
							switch( ribNode__->hitmode.transmission ) 
							{
							case liqRibNode::hitmode::TRANSMISSION_HITMODE_PRIMITIVE:
								mode = "primitive";
								break;
							case liqRibNode::hitmode::TRANSMISSION_HITMODE_CACHE:
								mode = "cache";
								break;
							case liqRibNode::hitmode::TRANSMISSION_HITMODE_SHADER:
							default:
								mode = "shader";
							}
							RiAttribute( "shade", (liqToken) "string transmissionhitmode", &mode, RI_NULL );
						}
					}//if( !m_skipRayTraceAttributes )
				}//if ( rt_useRayTracing )
				if( !liqRibTranslator::getInstancePtr()->m_skipShadingAttributes )
				{
					if( ribNode__->hitmode.camera != liqRibNode::hitmode::CAMERA_HITMODE_SHADER ) 
					{
						switch( ribNode__->hitmode.camera ) 
						{
						case liqRibNode::hitmode::CAMERA_HITMODE_PRIMITIVE:
							mode = "primitive";
							break;
						case liqRibNode::hitmode::CAMERA_HITMODE_CACHE:
							mode = "cache";
							break;
						case liqRibNode::hitmode::CAMERA_HITMODE_SHADER:
						default:
							mode = "shader";
						}
						RiAttribute( "shade", (liqToken) "string camerahitmode", &mode, RI_NULL );
					}
				}
			} //else philippe : prman 12.5 visibility support 
			// irradiance attributes
			if( ribNode__->irradiance.shadingRate != 1.0f ) 
			{
				liqFloat rate = ribNode__->irradiance.shadingRate;
				RiAttribute( "irradiance", (liqToken) "shadingrate", &rate, RI_NULL );
			}

			if( ribNode__->irradiance.nSamples != 64 ) 
			{
				liqInt samples = ribNode__->irradiance.nSamples;
				RiAttribute( "irradiance", (liqToken) "nsamples", &samples, RI_NULL );
			}

			if( ribNode__->irradiance.maxError != 0.5f ) 
			{
				liqFloat merror = ribNode__->irradiance.maxError;
				RiAttribute( "irradiance", (liqToken) "float maxerror", &merror, RI_NULL );
			}

			if( ribNode__->irradiance.maxPixelDist != 30.0f ) 
			{
				liqFloat mpd = ribNode__->irradiance.maxPixelDist;
				RiAttribute( "irradiance", (liqToken) "float maxpixeldist", &mpd, RI_NULL );
			}

			if( ribNode__->irradiance.handle != "" ) 
			{
				liqString handle = const_cast< char* >( ribNode__->irradiance.handle.asChar() );
				RiAttribute( "irradiance", (liqToken) "handle", &handle, RI_NULL );
			}

			if( ribNode__->irradiance.fileMode != liqRibNode::irradiance::FILEMODE_NONE ) 
			{
				liqString mode;
				switch( ribNode__->irradiance.fileMode ) 
				{
				case liqRibNode::irradiance::FILEMODE_READ:
					mode = "r";
					break;
				case liqRibNode::irradiance::FILEMODE_WRITE:
					mode = "w";
					break;
				case liqRibNode::irradiance::FILEMODE_READ_WRITE:
				default:
					mode = "rw";
				}
				RiAttribute( "irradiance", (liqToken) "filemode", &mode, RI_NULL );
			}

			// ymesh: photon visibility support
			if ( liqglo.rt_useRayTracing && ribNode__->visibility.photon ) 
				RiAttribute( "visibility", (liqToken) "int photon", &on, RI_NULL );

			if( ribNode__->photon.globalMap != "" ) 
			{
				liqString map = const_cast< char* >( ribNode__->photon.globalMap.asChar() );
				RiAttribute( "photon", (liqToken) "globalmap", &map, RI_NULL );
			}

			if( ribNode__->photon.causticMap != "" ) 
			{
				liqString map = const_cast< char* >( ribNode__->photon.causticMap.asChar() );
				RiAttribute( "photon", (liqToken) "causticmap", &map, RI_NULL );
			}

			if( ribNode__->photon.shadingModel != liqRibNode::photon::SHADINGMODEL_MATTE ) 
			{
				switch( ribNode__->photon.shadingModel  ) 
				{
				case liqRibNode::photon::SHADINGMODEL_GLASS:
					mode = "glass";
					break;
				case liqRibNode::photon::SHADINGMODEL_WATER:
					mode = "water";
					break;
				case liqRibNode::photon::SHADINGMODEL_CHROME:
					mode = "chrome";
					break;
				case liqRibNode::photon::SHADINGMODEL_TRANSPARENT:
					mode = "chrome";
					break;
				case liqRibNode::photon::SHADINGMODEL_DIALECTRIC:
					mode = "dielectric";
					break;
				case liqRibNode::photon::SHADINGMODEL_MATTE:
				default:
					mode = "matte";
				}
				RiAttribute( "photon", (liqToken) "shadingmodel", &mode, RI_NULL );
			}

			if( ribNode__->photon.estimator != 100 ) 
			{
				liqInt estimator = ribNode__->photon.estimator;
				RiAttribute( "photon", (liqToken) "estimator", &estimator, RI_NULL );
			}

		}

		// 3Delight sss group
		if( ribNode__->delightSSS.doScatter && liqglo.liquidRenderer.renderName == MString("3Delight") )
		{
#ifdef GENERIC_RIBLIB         
			useAdvancedVisibilityAttributes = true;
#endif			  
			liqString groupName = const_cast< char* >( ribNode__->delightSSS.groupName.asChar() );
			RiAttribute( "visibility", (liqToken) "string subsurface", &groupName, RI_NULL );

			liqColor scattering, absorption;
			scattering[0] = ribNode__->delightSSS.scattering.r;
			scattering[1] = ribNode__->delightSSS.scattering.g;
			scattering[2] = ribNode__->delightSSS.scattering.b;

			absorption[0] = ribNode__->delightSSS.absorption.r;
			absorption[1] = ribNode__->delightSSS.absorption.g;
			absorption[2] = ribNode__->delightSSS.absorption.b;

			if ( scattering[0] && scattering[1] && scattering[2] )
				RiAttribute( "subsurface",
				(liqToken) "scattering", &scattering,
				(liqToken) "absorption", &absorption,
				RI_NULL );

			liqFloat refractionindex = ribNode__->delightSSS.refraction;
			liqFloat shadingrate = ribNode__->delightSSS.shadingRate;
			liqFloat scale = ribNode__->delightSSS.scale;

			RiAttribute( "subsurface",
				(liqToken) "refractionindex", &refractionindex,
				(liqToken) "shadingrate", &shadingrate,
				(liqToken) "scale", &scale, 
				RI_NULL );

			liqColor meanfreepath, reflectance;
			meanfreepath[0] = ribNode__->delightSSS.meanfreepath.r;
			meanfreepath[1] = ribNode__->delightSSS.meanfreepath.g;
			meanfreepath[2] = ribNode__->delightSSS.meanfreepath.b;

			reflectance[0] = ribNode__->delightSSS.reflectance.r;
			reflectance[1] = ribNode__->delightSSS.reflectance.g;
			reflectance[2] = ribNode__->delightSSS.reflectance.b;

			if ( reflectance[0] && reflectance[1] && reflectance[2] )
				RiAttribute( "subsurface",
				(liqToken) "meanfreepath", &meanfreepath,
				(liqToken) "reflectance", &reflectance, 
				RI_NULL );

			if ( ribNode__->delightSSS.referencecamera != "" )
			{
				liqString referenceCamera = const_cast< char* >( ribNode__->delightSSS.referencecamera.asChar() );
				RiAttribute( "subsurface", (liqToken) "string referencecamera", &referenceCamera, RI_NULL );
			}
		}

		//strategy
		{
			if(ribNode__->strategy.strategy_!="grids"){
				liqString strategy = const_cast<char*>(ribNode__->strategy.strategy_.asChar());
				RiAttribute("shade",(liqToken)"string strategy", &strategy, RI_NULL);
			}

			if(ribNode__->strategy.volumeIntersectionStrategy!="exclusive"){
				liqString volumeIntersectionStrategy = const_cast<char*>(ribNode__->strategy.volumeIntersectionStrategy.asChar());
				RiAttribute("shade",(liqToken)"string volumeintersectionstrategy", &volumeIntersectionStrategy, RI_NULL);
			}

			if(ribNode__->strategy.volumeIntersectionPriority >0.0001){
				liqFloat volumeIntersectionPriority = ribNode__->strategy.volumeIntersectionPriority;
				RiAttribute("shade",(liqToken)"float volumeintersectionpriority", &volumeIntersectionPriority, RI_NULL);
			}
		}

		//trim curve 
		{
			liqString sense = "inside";

			switch(ribNode__->trimcurve.sense){
			case liqRibNode::trimcurve::INSIDE:  
				{
					sense  = "inside";
					//RiAttribute("trimcurve",(liqToken)"string sense", &sense, RI_NULL);//default value, need not to write.
				}break;
			case liqRibNode::trimcurve::OUTSIDE:
				{
					sense  = "outside";
					RiAttribute("trimcurve",(liqToken)"string sense", &sense, RI_NULL);	
				}break;
			default:
				assert(0);
			}

		}//trim curve 

		//stitch
		{
			if( ribNode__->stitch.enable != true ){
				liqInt enable = (ribNode__->stitch.enable)? 1: 0;
				RiAttribute("stitch",(liqToken)"int enable", &enable, RI_NULL);	
			}
			if( ribNode__->stitch.traceenable != false ){
				liqInt traceenable = (ribNode__->stitch.traceenable)? 1: 0;
				RiAttribute("stitch",(liqToken)"int traceenable", &traceenable, RI_NULL);	
			}
			if( ribNode__->stitch.newgroup != false ){
				liqInt newgroup = (ribNode__->stitch.newgroup)? 1: 0;
				RiAttribute("stitch",(liqToken)"int newgroup", &newgroup, RI_NULL);	
			}	
		}//stitch

		//stochastic
		{
			if( ribNode__->stochastic.sigma != 0 ){
				liqInt sigma = ribNode__->stochastic.sigma;
				RiAttribute("stochastic",(liqToken)"int sigma", &sigma, RI_NULL);	
			}
			if( ribNode__->stochastic.pointfalloff != 0 ){
				liqInt pointfalloff = ribNode__->stochastic.pointfalloff;
				RiAttribute("stochastic",(liqToken)"int pointfalloff", &pointfalloff, RI_NULL);	
			}
		}//stochastic

		//dice
		{
			if( ribNode__->dice.binary != 0 ){
				liqInt binary = ribNode__->dice.binary;
				RiAttribute("dice",(liqToken)"int binary", &binary, RI_NULL);
			}
			if( ribNode__->dice.hair != 0 ){
				liqInt hair = ribNode__->dice.hair;
				RiAttribute("dice",(liqToken)"int hair", &hair, RI_NULL);
			}
			//strategy
			liqString strategy = "planarprojection";
			switch(ribNode__->dice.strategy){
			case liqRibNode::dice::PLANAR_PROJECTION:
				{
					strategy = "planarprojection";
					//RiAttribute("dice",(liqToken)"string strategy", &strategy, RI_NULL); //default value
				}break;
			case liqRibNode::dice::SPHERICAL_PROJECTION:
				{
					strategy = "sphericalprojection";
					RiAttribute("dice",(liqToken)"string strategy", &strategy, RI_NULL);
				}break;
			default:
				assert(0);
			}
			//referencecamera
			liqString referencecamera = "worldcamera";
			switch(ribNode__->dice.referencecamera){
			case liqRibNode::dice::WORLD_CAMERA:
				{
					referencecamera = "worldcamera";
					//RiAttribute("dice",(liqToken)"string referencecamera", &referencecamera, RI_NULL); //default value
				}break;
			case liqRibNode::dice::FRAME_CAMERA:
				{
					referencecamera = "framecamera";
					RiAttribute("dice",(liqToken)"string referencecamera", &referencecamera, RI_NULL);
				}break;
			default:
				assert(0);
			}

		}//dice

		//derivatives
		{
			if( ribNode__->derivatives.centered != 1 ){
				liqInt centered = ribNode__->derivatives.centered;
				RiAttribute("derivatives",(liqToken)"int centered", &centered, RI_NULL);
			}
			if( ribNode__->derivatives.extrapolate != 1 ){
				liqInt extrapolate = ribNode__->derivatives.extrapolate;
				RiAttribute("derivatives",(liqToken)"int extrapolate", &extrapolate, RI_NULL);
			}
		}//derivatives

		//procedural
		{
			if( ribNode__->procedural.attribute.length()>0 ){
				liqString attribute = const_cast<char*>(ribNode__->procedural.attribute.asChar());
				RiAttribute("procedural",(liqToken)"string attribute", &attribute, RI_NULL);
			}
		}//procedural

		if( liqglo.liqglo_doMotion && 
			ribNode__->motion.factor != 1.0f && 
			( ribNode__->motion.deformationBlur || ribNode__->motion.transformationBlur ) ) 
			RiGeometricApproximation( "motionfactor", ribNode__->motion.factor );

		ribNode__->writeUserAttributes();

		return MS::kSuccess;
	}
	//
	void F1(const liqRibNodePtr &ribNode__,  liqShader &currentShader)
	{
		CM_TRACE_FUNC("F1("<<ribNode__->name.asChar()<<","<<currentShader.getName()<<")");
		// Output color overrides or color ====>>>>>  WILL BE DONE IN liqShader::write -begin //r773 going to omit in r773
		if(ribNode__->shading.color.r != -1.0)
		{
			liqColor rColor;
			rColor[0] = ribNode__->shading.color[0];
			rColor[1] = ribNode__->shading.color[1];
			rColor[2] = ribNode__->shading.color[2];
			RiArchiveRecord( RI_COMMENT, "mark10" );
			RiColor( rColor );
		}
		else{
			RiArchiveRecord( RI_COMMENT, "mark11" );
			RiColor( currentShader.rmColor );
		}

		if(ribNode__->shading.opacity.r != -1.0)
		{
			liqColor rOpacity;
			rOpacity[0] = ribNode__->shading.opacity[0];
			rOpacity[1] = ribNode__->shading.opacity[1];
			rOpacity[2] = ribNode__->shading.opacity[2];
			RiOpacity( rOpacity );
		}
		else
			RiOpacity( currentShader.rmOpacity );
		//====>>>>>  WILL BE DONE IN liqShader::write  -end
	}

	////////////////////////////////////////
	void F2(const bool m_shaderDebug, const liqRibNodePtr &ribNode__)
	{ 
		CM_TRACE_FUNC("F2("<<m_shaderDebug<<","<<ribNode__->name.asChar()<<")");
		if( m_shaderDebug ) 
		{
			liqColor rColor,rOpacity;
			liqRIBMsg("shader debug is turned on, so the object is red.");
			// shader debug on !! everything goes red and opaque !!!
			rColor[0] = 1.;
			rColor[1] = 0.;
			rColor[2] = 0.;
			RiArchiveRecord( RI_COMMENT, "mark12" );
			RiColor( rColor );

			rOpacity[0] = 1.;
			rOpacity[1] = 1.;
			rOpacity[2] = 1.;
			RiOpacity( rOpacity );
		}else{
			if(ribNode__->shading.color.r != -1.0) 
			{
				liqColor rColor;
				rColor[0] = ribNode__->shading.color[0];
				rColor[1] = ribNode__->shading.color[1];
				rColor[2] = ribNode__->shading.color[2];
				RiArchiveRecord( RI_COMMENT, "mark13" );
				RiColor( rColor );
			} 
			else if( ( ribNode__->color.r != AS_NotEXist )&&( ribNode__->color.r != AS_ConnectedAsDes )) 
			{
				liqColor rColor;
				rColor[0] = ribNode__->color[0];
				rColor[1] = ribNode__->color[1];
				rColor[2] = ribNode__->color[2];
				RiArchiveRecord( RI_COMMENT, "mark14" );
				RiColor( rColor );
			}

			if(ribNode__->shading.opacity.r != -1.0) 
			{
				liqColor rOpacity;
				rOpacity[0] = ribNode__->shading.opacity[0];
				rOpacity[1] = ribNode__->shading.opacity[1];
				rOpacity[2] = ribNode__->shading.opacity[2];
				RiOpacity( rOpacity );
			} 
			else if( ( ribNode__->opacity.r != AS_NotEXist )&&( ribNode__->opacity.r != AS_ConnectedAsDes )) 
			{	
				liqColor rOpacity;
				rOpacity[0] = ribNode__->opacity[0];
				rOpacity[1] = ribNode__->opacity[1];
				rOpacity[2] = ribNode__->opacity[2];
				RiOpacity( rOpacity );
			}
		}
	}

}
#endif//_USE_RENDERMAN_