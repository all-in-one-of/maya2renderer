
#include <liqGlobalVariable.h>
#include <sstream>

LIQUID_EXPORT struct liqGlobalVariable liqglo;
LIQUID_EXPORT liquidVerbosityType liqglo_verbosity( verbosityAll );
LIQUID_EXPORT int  debugMode;

void initHinderParameters()
{

	liqglo.m_hiddenJitter = 1;
	// PRMAN 13 BEGIN
	liqglo.m_hiddenAperture[0] = 0.0;
	liqglo.m_hiddenAperture[1] = 0.0;
	liqglo.m_hiddenAperture[2] = 0.0;
	liqglo.m_hiddenAperture[3] = 0.0;
	liqglo.m_hiddenShutterOpening[0] = 0.0;
	liqglo.m_hiddenShutterOpening[0] = 1.0;
	// PRMAN 13 END
	liqglo.m_hiddenOcclusionBound = 0;
	liqglo.m_hiddenMpCache = true;
	liqglo.m_hiddenMpMemory = 6144;
	liqglo.m_hiddenMpCacheDir = ".";
	liqglo.m_hiddenSampleMotion = true;
	liqglo.m_hiddenSubPixel = 1;
	liqglo.m_hiddenExtremeMotionDof = false;
	liqglo.m_hiddenMaxVPDepth = -1;
	// PRMAN 13 BEGIN
	liqglo.m_hiddenSigma = false;
	liqglo.m_hiddenSigmaBlur = 1.0;
	// PRMAN 13 END

	liqglo.m_raytraceFalseColor = 0;
	liqglo.m_photonEmit = 0;
	liqglo.m_photonSampleSpectrum = 0;

	liqglo.m_depthMaskZFile = "";
	liqglo.m_depthMaskReverseSign = false;
	liqglo.m_depthMaskDepthBias = 0.01;



}
//
void getHinderParameters(const MFnDependencyNode& rGlobalNode)
{
	MStatus gStatus;
	MString varVal;
	// RENDER OPTIONS:BEGIN
	int var;
	if ( liquidGetPlugValue( rGlobalNode, "hider", var, gStatus ) == MS::kSuccess ) 
	{
		liqglo.liqglo_hider = (enum HiderType) var;
	}
	liquidGetPlugValue( rGlobalNode, "jitter", liqglo.m_hiddenJitter, gStatus );
	// PRMAN 13 BEGIN
	liquidGetPlugValue( rGlobalNode, "hiddenApertureNSides", liqglo.m_hiddenAperture[0], gStatus );
	liquidGetPlugValue( rGlobalNode, "hiddenApertureAngle", liqglo.m_hiddenAperture[1], gStatus );
	liquidGetPlugValue( rGlobalNode, "hiddenApertureRoundness", liqglo.m_hiddenAperture[2], gStatus );
	liquidGetPlugValue( rGlobalNode, "hiddenApertureDensity", liqglo.m_hiddenAperture[3], gStatus );   
	liquidGetPlugValue( rGlobalNode, "hiddenShutterOpeningOpen", liqglo.m_hiddenShutterOpening[0], gStatus );     
	liquidGetPlugValue( rGlobalNode, "hiddenShutterOpeningClose", liqglo.m_hiddenShutterOpening[1], gStatus );   
	// PRMAN 13 END
	liquidGetPlugValue( rGlobalNode, "hiddenOcclusionBound", liqglo.m_hiddenOcclusionBound, gStatus );  
	liquidGetPlugValue( rGlobalNode, "hiddenMpCache", liqglo.m_hiddenMpCache, gStatus ); 
	liquidGetPlugValue( rGlobalNode, "hiddenMpMemory", liqglo.m_hiddenMpMemory, gStatus ); 
	liquidGetPlugValue( rGlobalNode, "hiddenMpCacheDir", liqglo.m_hiddenMpCacheDir, gStatus );   
	liquidGetPlugValue( rGlobalNode, "hiddenSampleMotion", liqglo.m_hiddenSampleMotion, gStatus );     
	liquidGetPlugValue( rGlobalNode, "hiddenSubPixel", liqglo.m_hiddenSubPixel, gStatus );  
	liquidGetPlugValue( rGlobalNode, "hiddenExtremeMotionDof", liqglo.m_hiddenExtremeMotionDof, gStatus ); 
	liquidGetPlugValue( rGlobalNode, "hiddenMaxVPDepth", liqglo.m_hiddenMaxVPDepth, gStatus ); 
	// PRMAN 13 BEGIN  
	liquidGetPlugValue( rGlobalNode, "hiddenSigmaHiding", liqglo.m_hiddenSigma, gStatus );   
	liquidGetPlugValue( rGlobalNode, "hiddenSigmaBlur", liqglo.m_hiddenSigmaBlur, gStatus );   
	// PRMAN 13 END  
	liquidGetPlugValue( rGlobalNode, "raytraceFalseColor", liqglo.m_raytraceFalseColor, gStatus );     
	liquidGetPlugValue( rGlobalNode, "photonEmit", liqglo.m_photonEmit, gStatus );   
	liquidGetPlugValue( rGlobalNode, "photonSampleSpectrum", liqglo.m_photonSampleSpectrum, gStatus );  
	if ( liquidGetPlugValue( rGlobalNode, "depthMaskZFile", varVal, gStatus ) == MS::kSuccess )   
		liqglo.m_depthMaskZFile = parseString( varVal, false );   //  doEscaped = false
	liquidGetPlugValue( rGlobalNode, "depthMaskReverseSign", liqglo.m_depthMaskReverseSign, gStatus ); 
	liquidGetPlugValue( rGlobalNode, "depthMaskDepthBias", liqglo.m_depthMaskDepthBias, gStatus ); 
	// RENDER OPTIONS:END
}

MString getHiderOptions( MString rendername, MString hidername )
{
  std::stringstream ss;
	// PRMAN
	if( rendername == "PRMan" ) 
	{
		if( hidername == "hidden" ) 
		{
			ss << "\"int jitter\" [" << liqglo.m_hiddenJitter << "] ";

			// PRMAN 13 BEGIN
			if( liqglo.m_hiddenAperture[0] != 0.0 ||
				liqglo.m_hiddenAperture[1] != 0.0 ||
				liqglo.m_hiddenAperture[2] != 0.0 ||
				liqglo.m_hiddenAperture[3] != 0.0 ) 
				ss << "\"float aperture[4]\" [" << liqglo.m_hiddenAperture[0] << " " << liqglo.m_hiddenAperture[1] << " " << liqglo.m_hiddenAperture[2] << " " << liqglo.m_hiddenAperture[3] << "] ";

			if ( liqglo.m_hiddenShutterOpening[0] != 0.0 && liqglo.m_hiddenShutterOpening[1] != 1.0 ) 
				ss << "\"float[2] shutteropening\" ["<< liqglo.m_hiddenShutterOpening[0] << " " << liqglo.m_hiddenShutterOpening[1] << "] ";
			// PRMAN 13 END

			if( liqglo.m_hiddenOcclusionBound != 0.0 ) 
				ss << "\"occlusionbound\" [" << liqglo.m_hiddenOcclusionBound << "] ";

			if( liqglo.m_hiddenMpCache != true ) 
				ss << "\"int mpcache\" [0] ";

			if( liqglo.m_hiddenMpMemory != 6144 ) 
				ss << "\"mpcache\" [" << liqglo.m_hiddenMpMemory << "] ";

			if( liqglo.m_hiddenMpCacheDir != "" ) 
				ss << "\"mpcachedir\" [\"" << liqglo.m_hiddenMpCacheDir.asChar() << "\"] ";

			if( liqglo.m_hiddenSampleMotion != true ) 
				ss << "\"int samplemotion\" [0] ";

			if( liqglo.m_hiddenSubPixel != 1 ) 
				ss << "\"subpixel\" [" << liqglo.m_hiddenSubPixel << "] ";

			if( liqglo.m_hiddenExtremeMotionDof != false ) 
				ss << "\"extrememotiondof\" [1] ";

			if( liqglo.m_hiddenMaxVPDepth != -1 ) 
				ss << "\"maxvpdepth\" [" << liqglo.m_hiddenMaxVPDepth << "] ";

			// PRMAN 13 BEGIN
			if( liqglo.m_hiddenSigma != false ) 
				ss << "\"int sigma\" [1] " << "\"sigmablur\" [" << liqglo.m_hiddenSigmaBlur << "] ";
			// PRMAN 13 END
		} 
		else if( hidername == "photon" ) 
		{
			if( liqglo.m_photonEmit != 0 ) 
				ss << " \"int emit\" [" << liqglo.m_photonEmit << "] ";
		} 
		else if( hidername == "depthmask" ) 
		{
			ss << "\"zfile\" [\"" << liqglo.m_depthMaskZFile.asChar() << "\"] ";
			ss << "\"reversesign\" [\"" << liqglo.m_depthMaskReverseSign << "\"] ";
			ss << "\"depthbias\" [" << liqglo.m_depthMaskDepthBias << "] ";

		}
	}
	// 3DELIGHT
	if( rendername == "3Delight" ) 
	{
		if( hidername == "hidden" ) 
		{
			ss << "\"int jitter\" [" << liqglo.m_hiddenJitter << "] ";

			if ( liqglo.m_hiddenSampleMotion != true ) 
				ss << "\"int samplemotion\" [0] ";

			if ( liqglo.m_hiddenExtremeMotionDof != false ) 
				ss << "\"int extrememotiondof\" [1] ";
		}
	}
	// PIXIE
	if( rendername == "Pixie" ) 
	{
		if( hidername == "hidden" ) 
		{
			ss << "\"float jitter\" [" << liqglo.m_hiddenJitter << "] ";
		} 
		else if( hidername == "raytrace" ) {
			if ( liqglo.m_raytraceFalseColor != 0 ) 
				ss << "\"int falsecolor\" [1] ";
			else if( hidername == "photon" ) 
			{
				if( liqglo.m_photonEmit != 0 ) 
					ss << " \"int emit\" [" << liqglo.m_photonEmit << "] ";
				if( liqglo.m_photonSampleSpectrum ) 
					ss << " \"int samplespectrum\" [1] ";
			}
		}
	}

	// AQSIS
	if( rendername == "Aqsis" ) 
	{
		// no known options
	}

	// AIR
	if( rendername == "Air" ) 
	{
		// no known options
	}
	MString options( ss.str().c_str() );
	return options;
}


//
void initLimitsParameters()
{
	liqglo.bucketSize[0] = 16;
	liqglo.bucketSize[1] = 16;
	liqglo.gridSize = 256;
	liqglo.textureMemory = 2048;
	liqglo.eyeSplits = 10;
	liqglo.othreshold[0] = 0.996; liqglo.othreshold[1] = 0.996; liqglo.othreshold[2] = 0.996;
	liqglo.zthreshold[0] = 0.996; liqglo.zthreshold[1] = 0.996; liqglo.zthreshold[2] = 0.996;
}
void getLimitsParameters(const MFnDependencyNode& rGlobalNode)
{
	MStatus gStatus;
	MString varVal;
	// RENDER OPTIONS:BEGIN
	int var;
	liquidGetPlugValue( rGlobalNode, "limitsBucketXSize", liqglo.bucketSize[0], gStatus );
	liquidGetPlugValue( rGlobalNode, "limitsBucketYSize", liqglo.bucketSize[1], gStatus );
	liquidGetPlugValue( rGlobalNode, "limitsGridSize", liqglo.gridSize, gStatus );
	liquidGetPlugValue( rGlobalNode, "limitsTextureMemory", liqglo.textureMemory, gStatus );
	liquidGetPlugValue( rGlobalNode, "limitsEyeSplits", liqglo.eyeSplits, gStatus );

	liquidGetPlugValue( rGlobalNode, "limitsOThreshold", liqglo.othreshold, gStatus );
	liquidGetPlugValue( rGlobalNode, "limitsZThreshold", liqglo.zthreshold, gStatus );
}
void writeLimitsOptions()
{
// moved to renderman::Renderer::writeLimitsOptions()
// 	if( liqglo.bucketSize != 0 )    
// 		RiOption( "limits", "bucketsize", ( liqPointer ) &liqglo.bucketSize, RI_NULL );
// 	if( liqglo.gridSize != 0 )      
// 		RiOption( "limits", "gridsize", ( liqPointer ) &liqglo.gridSize, RI_NULL );
// 	if( liqglo.textureMemory != 0 ) 
// 		RiOption( "limits", "texturememory", ( liqPointer) &liqglo.textureMemory, RI_NULL );
// 	if( liqglo.liquidRenderer.supports_EYESPLITS ) 
// 		RiOption( "limits", "eyesplits", ( liqPointer ) &liqglo.eyeSplits, RI_NULL );
// 
// 	if(liqglo.liquidRenderer.renderName == MString("PRMan") || liqglo.liquidRenderer.renderName == MString("3Delight") )
// 	{
// 		liqColor othresholdC = {liqglo.othreshold[0], liqglo.othreshold[1], liqglo.othreshold[2]};
// 		RiOption( "limits", "othreshold", &othresholdC, RI_NULL );
// 		liqColor zthresholdC = {liqglo.zthreshold[0], liqglo.zthreshold[1], liqglo.zthreshold[2]};
// 		RiOption( "limits", "zthreshold", &zthresholdC, RI_NULL );
// 	}
}
//
//
void initStatisticsParameters()
{
	liqglo.m_statistics        = 0;
	liqglo.m_statisticsFile    = "";
}
void getStatisticsParameters(const MFnDependencyNode& rGlobalNode)
{
	MStatus gStatus;
	MString varVal;
	// RENDER OPTIONS:BEGIN
	int var;
	liquidGetPlugValue( rGlobalNode, "statistics", liqglo.m_statistics, gStatus );
	liquidGetPlugValue( rGlobalNode, "statisticsFile", varVal, gStatus );
	if( varVal != "" ) 
		liqglo.m_statisticsFile = parseString( varVal, false );
}
void writeStatisticsOptions()
{
// moved to renderman::Renderer::writeStatisticsOptions()
// 		if( liqglo.m_statistics != 0 )  
// 		{
// 			if( liqglo.m_statistics < 4 ) 
// 				RiOption( "statistics", "endofframe", ( liqPointer ) &liqglo.m_statistics, RI_NULL );
// 			else 
// 			{
// 				//cout <<"xml stats "<<endl;
// 				int stats = 1;
// 				RiOption( "statistics", "int endofframe", ( liqPointer ) &stats, RI_NULL );
// 				RiArchiveRecord( RI_VERBATIM, "Option \"statistics\" \"xmlfilename\" [\"%s\"]\n", const_cast< char* > ( liqglo.m_statisticsFile.asChar() ) );
// 			}
// 		}
}
//
void initOtherParameters()
{
	liqglo.m_rFilterX = 1;
	liqglo.m_rFilterY = 1;
	liqglo.m_rFilter = pfBoxFilter;

	//m_pixelFilterNames... moved to liqRenderer::liqRenderer()

	liqglo.pixelSamples = 1;
	liqglo.shadingRate = 1.0;
	liqglo.m_renderView        = false;
	liqglo.m_bakeNonRasterOrient    = false;
	liqglo.m_bakeNoCullBackface    = false;
	liqglo.m_bakeNoCullHidden    = false;
	liqglo.outFormat = "it";

	liqglo.m_preFrameRIB.clear();
	liqglo.launchRender = false;
	liqglo.outExt = "tif";
	liqglo.m_pixDir = "rmanpix/";

	//debug
	liqglo.m_logFunctionCall = false;
	liqglo.m_logMsgFlush     = false;
	liqglo.m_logTraceFunction= true;
	liqglo.m_showliquidMessage= true;

	liqglo.m_ribFileOpen     = false;
	liqglo.m_writeDataIntoMainRibFile = true;

	liqglo.liqglo_exportAllShadersParams = false;
	liqglo.liqglo_skipDefaultMatte = false;

	liqglo.iprRendering = 0;
}
void getOtherParameters(const MFnDependencyNode& rGlobalNode)
{
	MStatus gStatus;
	MString varVal;
	liquidGetPlugValue( rGlobalNode, "pixelSamples", liqglo.pixelSamples, gStatus );
	liquidGetPlugValue( rGlobalNode, "shadingRate", liqglo.shadingRate, gStatus );

	if ( liquidGetPlugValue( rGlobalNode, "imageDriver", varVal, gStatus )== MS::kSuccess )
		liqglo.outFormat = parseString( varVal, false );
	if ( liquidGetPlugValue( rGlobalNode, "bakeRasterOrient", liqglo.m_bakeNonRasterOrient, gStatus )== MS::kSuccess )
		liqglo.m_bakeNonRasterOrient = !liqglo.m_bakeNonRasterOrient;
	if ( liquidGetPlugValue( rGlobalNode, "bakeCullBackface", liqglo.m_bakeNoCullBackface, gStatus )== MS::kSuccess )
		liqglo.m_bakeNoCullBackface = !liqglo.m_bakeNoCullBackface;
	if ( liquidGetPlugValue( rGlobalNode, "bakeCullHidden", liqglo.m_bakeNoCullHidden, gStatus )== MS::kSuccess )
		liqglo.m_bakeNoCullHidden = !liqglo.m_bakeNoCullHidden;

	liquidGetPlugValue( rGlobalNode, "launchRender", liqglo.launchRender, gStatus );

	//debug
	liquidGetPlugValue( rGlobalNode, "logFunctionCall", liqglo.m_logFunctionCall, gStatus );
	liquidGetPlugValue( rGlobalNode, "logMsgFlush",     liqglo.m_logMsgFlush,     gStatus );
	liquidGetPlugValue( rGlobalNode, "logTraceFunction",liqglo.m_logTraceFunction,gStatus );
	liquidGetPlugValue( rGlobalNode, "showliquidMessage",  liqglo.m_showliquidMessage,gStatus );

}
//
void initCameraParameters()
{
	liqglo.doDof = false;
	liqglo.doCameraMotion = false;           // camera motion blocks
	liqglo.liqglo_rotateCamera = false;      // rotate the camera 90 degrees around Z axis
	liqglo.shutterConfig = OPEN_ON_FRAME;
	liqglo.aspectRatio = 1.0;
	liqglo.quantValue = 8;
	liqglo.m_rgain = 1.0;
	liqglo.m_rgamma = 1.0;
	// display channels defaults
	liqglo.m_channels.clear();

	// Display Driver Defaults
	liqglo.m_displays.clear();


	liqglo.m_renderViewCrop    = false;
	liqglo.m_renderViewLocal   = true;
	liqglo.m_renderViewPort    = 6667;
	liqglo.m_renderViewTimeOut = 10;


	liqglo.m_cropX1 = liqglo.m_cropY1 = 0.0;
	liqglo.m_cropX2 = liqglo.m_cropY2 = 1.0;
}
void getCameraParameters(const MFnDependencyNode& rGlobalNode)
{
	MStatus gStatus;
	MString varVal;
	int var;
	liquidGetPlugValue( rGlobalNode, "rotateCamera", liqglo.liqglo_rotateCamera, gStatus );
	liquidGetPlugValue( rGlobalNode, "cameraBlur",   liqglo.doCameraMotion, gStatus );
	liquidGetPlugValue( rGlobalNode, "depthOfField", liqglo.doDof, gStatus );
	liquidGetPlugValue( rGlobalNode, "shutterConfig", var, gStatus );
	if( gStatus == MS::kSuccess ) 
		liqglo.shutterConfig = ( enum ShutterConfig ) var;
	liquidGetPlugValue( rGlobalNode, "pixelAspectRatio", liqglo.aspectRatio, gStatus );


	liquidGetPlugValue( rGlobalNode, "cropX1", liqglo.m_cropX1, gStatus );
	liquidGetPlugValue( rGlobalNode, "cropX2", liqglo.m_cropX2, gStatus );
	liquidGetPlugValue( rGlobalNode, "cropY1", liqglo.m_cropY1, gStatus );
	liquidGetPlugValue( rGlobalNode, "cropY2", liqglo.m_cropY2, gStatus );

	liquidGetPlugValue( rGlobalNode, "gain", liqglo.m_rgain, gStatus );
	liquidGetPlugValue( rGlobalNode, "gamma", liqglo.m_rgamma, gStatus );
	liquidGetPlugValue( rGlobalNode, "renderViewLocal", liqglo.m_renderViewLocal, gStatus );
	liquidGetPlugValue( rGlobalNode, "renderViewPort", liqglo.m_renderViewPort, gStatus );
	liquidGetPlugValue( rGlobalNode, "renderViewTimeOut", liqglo.m_renderViewTimeOut, gStatus );
}
 