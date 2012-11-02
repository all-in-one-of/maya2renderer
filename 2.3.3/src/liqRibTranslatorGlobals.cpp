// Maya headers
#include <maya/MGlobal.h>
#include <maya/MSyntax.h>
#include <maya/MAnimControl.h>
#include <maya/MSelectionList.h>
#include <maya/MPlug.h>
#include <maya/MFnStringArrayData.h>
#include <maya/MFnIntArrayData.h>

// Liquid headers
#include <liquid.h>
#include <liqRibTranslator.h>
#include <liqGlobalHelpers.h>
#include <liqRenderer.h>
#include <liqGlobalVariable.h>


/**
 * Checks to see if the liquidGlobals are available.
 */
bool liqRibTranslator::liquidInitGlobals()
{
	//CM_TRACE_FUNC("liqRibTranslator::liquidInitGlobals()");

	//trace log is not created, can't call CM_TRACE_FUNC("liqRibTranslator::liquidInitGlobals()");
	MStatus status;
	MSelectionList rGlobalList;
	status = rGlobalList.add( "liquidGlobals" );
	if( rGlobalList.length() > 0 ) 
	{
		status.clear();
		status = rGlobalList.getDependNode( 0, liqglo.rGlobalObj );
		if( status == MS::kSuccess ) 
			return true;
		else 
			return false;
	} 
	return false;
}

/**
 * Read the values from the render globals and set internal values.
 */
void liqRibTranslator::liquidReadGlobals()
{
	//CM_TRACE_FUNC("liqRibTranslator::liquidReadGlobals()");

	//trace log is not created, can't call CM_TRACE_FUNC("liqRibTranslator::liquidReadGlobals()");
	MStatus gStatus;
	MPlug gPlug;
	MFnDependencyNode rGlobalNode( liqglo.rGlobalObj );
	MString varVal;
	int var;
	// find the activeView for previews;
	width        = M3dView::active3dView().portWidth();
	height       = M3dView::active3dView().portHeight();

	if( MS::kSuccess!=liqglo.liquidRenderer.setRenderer() ){
		return /*MS::kFailure*/;
	}
	m_renderCommand = liqglo.liquidRenderer.renderCommand;

	// Display Channels - Read and store 'em !
	// philippe : channels are stored as structures in a vector
	{
		if( liqglo.liquidRenderer.supports_DISPLAY_CHANNELS ) 
		{
			liqglo.m_channels.clear();
			unsigned int nChannels = liquidGetPlugNumElements( rGlobalNode, "channelName", &gStatus );

			for ( unsigned i( 0 ); i < nChannels; i++ ) 
			{
				StructChannel theChannel;

				liquidGetPlugElementValue( rGlobalNode, i, "channelName", theChannel.name, gStatus );
				liquidGetPlugElementValue( rGlobalNode, i, "channelType", theChannel.type, gStatus );
				liquidGetPlugElementValue( rGlobalNode, i, "channelArraySize", theChannel.arraySize, gStatus );
				liquidGetPlugElementValue( rGlobalNode, i, "channelQuantize", theChannel.quantize, gStatus );
				liquidGetPlugElementValue( rGlobalNode, i, "channelBitDepth", theChannel.bitDepth, gStatus );
				liquidGetPlugElementValue( rGlobalNode, i, "channelDither", theChannel.dither, gStatus );
				liquidGetPlugElementValue( rGlobalNode, i, "channelFilter", theChannel.filter, gStatus );
				liquidGetPlugElementValue( rGlobalNode, i, "channelPixelFilter", theChannel.pixelFilter, gStatus );
				liquidGetPlugElementValue( rGlobalNode, i, "channelPixelFilterX", theChannel.pixelFilterX, gStatus );
				liquidGetPlugElementValue( rGlobalNode, i, "channelPixelFilterY", theChannel.pixelFilterY, gStatus );

				liqglo.m_channels.push_back( theChannel );
			}
		}
	}
	// Display Driver Globals - Read 'em and store 'em !
	liquidGetPlugValue( rGlobalNode, "ignoreAOVDisplays", liqglo.m_ignoreAOVDisplays, gStatus );
	{
		liqglo.m_displays.clear();
		unsigned int nDisplays = liquidGetPlugNumElements( rGlobalNode, "ddImageName", &gStatus );
		//cout <<"  DD : we have "<<nDisplays<<" displays..."<<endl;
		for ( unsigned int i(0); i<nDisplays; i++ ) 
		{
			StructDisplay theDisplay;

			liquidGetPlugElementValue( rGlobalNode, i, "ddImageName", theDisplay.name, gStatus );
			liquidGetPlugElementValue( rGlobalNode, i, "ddImageType", theDisplay.type, gStatus );
			liquidGetPlugElementValue( rGlobalNode, i, "ddImageMode", theDisplay.mode, gStatus );

			if( i==0 ) {
				theDisplay.enabled = true;
				theDisplay.doQuantize = true;
				theDisplay.doFilter = true;
			}
			else 
			{
				liquidGetPlugElementValue( rGlobalNode, i, "ddEnable", theDisplay.enabled, gStatus );
				liquidGetPlugElementValue( rGlobalNode, i, "ddQuantizeEnabled", theDisplay.doQuantize, gStatus );
				liquidGetPlugElementValue( rGlobalNode, i, "ddFilterEnabled", theDisplay.doFilter, gStatus );
			}

			if ( theDisplay.doQuantize )
			{
				liquidGetPlugElementValue( rGlobalNode, i, "ddBitDepth", theDisplay.bitDepth, gStatus );
				liquidGetPlugElementValue( rGlobalNode, i, "ddDither", theDisplay.dither, gStatus );
			}
			if ( theDisplay.doFilter )
			{
				liquidGetPlugElementValue( rGlobalNode, i, "ddPixelFilter", theDisplay.filter, gStatus );
				liquidGetPlugElementValue( rGlobalNode, i, "ddPixelFilterX", theDisplay.filterX, gStatus );
				liquidGetPlugElementValue( rGlobalNode, i, "ddPixelFilterY", theDisplay.filterY, gStatus );
			}

			// retrieve the extra parameters for this display
			MStringArray xtraParamsNames;
			MStringArray xtraParamsDatas;
			MIntArray    xtraParamsTypes;

			liquidGetPlugElementValue( rGlobalNode, i, "ddXtraParamNames", xtraParamsNames, gStatus );
			liquidGetPlugElementValue( rGlobalNode, i, "ddXtraParamTypes", xtraParamsTypes, gStatus );
			liquidGetPlugElementValue( rGlobalNode, i, "ddXtraParamDatas", xtraParamsDatas, gStatus );

			if(i==0) 
			{ // copy filter params from display 0
				liqglo.m_rFilter = theDisplay.filter;
				liqglo.m_rFilterX = theDisplay.filterX;
				liqglo.m_rFilterY = theDisplay.filterY;
				liqglo.quantValue = theDisplay.bitDepth;
				//liqglo_DDimageName = theDisplay.name; //we don't use liqglo_DDimageName, so we omit this line in r772
			}
			StructDDParam xtraDDParams;
			xtraDDParams.num   = xtraParamsNames.length();
			xtraDDParams.names = xtraParamsNames;
			xtraDDParams.data  = xtraParamsDatas;
			xtraDDParams.type  = xtraParamsTypes;
			theDisplay.xtraParams = xtraDDParams;

			liqglo.m_displays.push_back( theDisplay );
		}
	}
	liquidGetPlugValue( rGlobalNode, "shotName", liqglo.liqglo_shotName, gStatus ); // no substitution here
	liquidGetPlugValue( rGlobalNode, "shotVersion", liqglo.liqglo_shotVersion, gStatus ); // no substitution here
	liquidGetPlugValue( rGlobalNode, "rotateCamera",  liqglo.liqglo_rotateCamera, gStatus );
	liquidGetPlugValue( rGlobalNode, "relativeFileNames", liqglo.liqglo_relativeFileNames, gStatus );

	liquidGetPlugValue( rGlobalNode, "renderScriptFileName", m_userRenderScriptFileName, gStatus );
	liquidGetPlugValue( rGlobalNode, "beautyRibHasCameraName", liqglo.liqglo_beautyRibHasCameraName, gStatus );

	liquidGetPlugValue( rGlobalNode, "ribgenCommand", m_ribgenCommand, gStatus, true ); // get parsed result
	liquidGetPlugValue( rGlobalNode, "preJobCommand", varVal, gStatus, true );
	liquidGetPlugValue( rGlobalNode, "postJobCommand", varVal, gStatus, true );
	liquidGetPlugValue( rGlobalNode, "postFrameCommand", m_postFrameCommand, gStatus );
	liquidGetPlugValue( rGlobalNode, "preFrameCommand", m_preFrameCommand, gStatus );
	liquidGetPlugValue( rGlobalNode, "preCommand", liqglo.m_preCommand, gStatus );
	liquidGetPlugValue( rGlobalNode, "launchRender",  liqglo.launchRender, gStatus );


	liquidGetPlugValue( rGlobalNode, "renderCamera", liqglo.renderCamera, gStatus, true );
	liqglo.liqglo_renderCamera = liqglo.renderCamera;
	getCameraParameters(rGlobalNode);

	liquidGetPlugValue( rGlobalNode, "ribName", liqglo.liqglo_ribName, gStatus, true );
	//Note:  -ribName is set to liqglo.liqglo_sceneName originally. 
	//       Because we replace liqglo.liqglo_sceneName with liquidTransGetSceneName(), and delete liqglo.liqglo_sceneName,
	//       we use liqglo.liqglo_ribName to store the ribName which passed by commandline parameter '-ribName'
	//  [2/17/2012 yaoyansi]
	//liquidMessage2(messageWarning,"warrning: ribName is store in liqglo.liqglo_ribName instead of liqglo.liqglo_sceneName.[2/17/2012 yaoyansi]");




	liquidGetPlugValue( rGlobalNode, "alfredTags", liqglo.m_alfredTags, gStatus, true );
	liquidGetPlugValue( rGlobalNode, "alfredServices", liqglo.m_alfredServices, gStatus, true );
	liquidGetPlugValue( rGlobalNode, "dirmaps", liqglo.m_dirmaps, gStatus, true );
	liquidGetPlugValue( rGlobalNode, "key", liqglo.m_defGenKey, gStatus, true );
	liquidGetPlugValue( rGlobalNode, "service", liqglo.m_defGenService, gStatus, true );

	liquidGetPlugValue( rGlobalNode, "preframeMel", m_preFrameMel, gStatus );
	liquidGetPlugValue( rGlobalNode, "postframeMel", m_postFrameMel, gStatus );

	getHinderParameters(rGlobalNode);

	liquidGetPlugValue( rGlobalNode, "cropX1", liqglo.m_cropX1, gStatus );
	liquidGetPlugValue( rGlobalNode, "cropX2", liqglo.m_cropX2, gStatus );
	liquidGetPlugValue( rGlobalNode, "cropY1", liqglo.m_cropY1, gStatus );
	liquidGetPlugValue( rGlobalNode, "cropY2", liqglo.m_cropY2, gStatus );

	// RAYTRACING OPTIONS:BEGIN

	liquidGetPlugValue( rGlobalNode, "useRayTracing", liqglo.rt_useRayTracing, gStatus );
	liquidGetPlugValue( rGlobalNode, "traceMaxDepth", liqglo.rt_traceMaxDepth, gStatus );
	liquidGetPlugValue( rGlobalNode, "traceSpecularThreshold", liqglo.rt_traceSpecularThreshold, gStatus );
	liquidGetPlugValue( rGlobalNode, "traceBreadthFactor", liqglo.rt_traceBreadthFactor, gStatus ); 
	liquidGetPlugValue( rGlobalNode, "traceDepthFactor", liqglo.rt_traceDepthFactor, gStatus ); 
	liquidGetPlugValue( rGlobalNode, "traceRayContinuation", liqglo.rt_traceRayContinuation, gStatus ); 
	liquidGetPlugValue( rGlobalNode, "traceCacheMemory", liqglo.rt_traceCacheMemory, gStatus ); 
	liquidGetPlugValue( rGlobalNode, "traceDisplacements", liqglo.rt_traceDisplacements, gStatus ); 
	liquidGetPlugValue( rGlobalNode, "traceSampleMotion", liqglo.rt_traceSampleMotion, gStatus ); 
	liquidGetPlugValue( rGlobalNode, "traceBias", liqglo.rt_traceBias, gStatus ); 
	liquidGetPlugValue( rGlobalNode, "traceMaxDiffuseDepth", liqglo.rt_traceMaxDiffuseDepth, gStatus ); 
	liquidGetPlugValue( rGlobalNode, "traceMaxSpecularDepth", liqglo.rt_traceMaxSpecularDepth, gStatus ); 
	liquidGetPlugValue( rGlobalNode, "irradianceMaxError", liqglo.rt_irradianceMaxError, gStatus ); 
	liquidGetPlugValue( rGlobalNode, "irradianceMaxPixelDist", liqglo.rt_irradianceMaxPixelDist, gStatus ); 
  
	liquidGetPlugValue( rGlobalNode, "irradianceHandle", liqglo.rt_irradianceGlobalHandle, gStatus ); 
	liquidGetPlugValue( rGlobalNode, "irradianceFileMode", liqglo.rt_irradianceGlobalFileMode, gStatus );

	liquidGetPlugValue( rGlobalNode, "photonGlobalHandle", liqglo.rt_photonGlobalHandle, gStatus );
	// if ( gStatus == MS::kSuccess ) 
	// rt_photonGlobalHandle = parseString( liqglo.rt_photonGlobalHandle, false );  //  doEscaped = false
	liquidGetPlugValue( rGlobalNode, "causticGlobalHandle", liqglo.rt_causticGlobalHandle, gStatus );
	// if ( gStatus == MS::kSuccess ) 
	// rt_causticGlobalHandle = parseString( liqglo.rt_causticGlobalHandle, false );  //  doEscaped = false

	liquidGetPlugValue( rGlobalNode, "photonShadingModel", liqglo.rt_photonShadingModel, gStatus );
	liquidGetPlugValue( rGlobalNode, "photonEstimator", liqglo.rt_photonEstimator, gStatus );

    // RAYTRACING OPTIONS:END
	liquidGetPlugValue( rGlobalNode, "outputMayaPolyCreases", liqglo.liqglo_outputMayaPolyCreases, gStatus ); 
	liquidGetPlugValue( rGlobalNode, "useMtorSubdiv", liqglo.liqglo_useMtorSubdiv, gStatus ); 
	liquidGetPlugValue( rGlobalNode, "ribRelativeTransforms", liqglo.liqglo_relativeTransforms, gStatus ); 
	liquidGetPlugValue( rGlobalNode, "shortShaderNames", liqglo.liqglo_shortShaderNames, gStatus ); 
	liquidGetPlugValue( rGlobalNode, "expandAlfred", liqglo.m_alfredExpand, gStatus ); 
	liquidGetPlugValue( rGlobalNode, "createOutputDirectories", createOutputDirectories, gStatus ); 
	liquidGetPlugValue( rGlobalNode, "minCPU", m_minCPU, gStatus ); 
	liquidGetPlugValue( rGlobalNode, "maxCPU", m_maxCPU, gStatus );
	liquidGetPlugValue( rGlobalNode, "showProgress", m_showProgress, gStatus );
	liquidGetPlugValue( rGlobalNode, "expandShaderArrays", liqglo.liqglo_expandShaderArrays, gStatus );
	liquidGetPlugValue( rGlobalNode, "outputComments", liqglo.m_outputComments, gStatus );
	liquidGetPlugValue( rGlobalNode, "shaderDebug", m_shaderDebug, gStatus );
	liquidGetPlugValue( rGlobalNode, "deferredGen", liqglo.m_deferredGen, gStatus );
	liquidGetPlugValue( rGlobalNode, "deferredBlock", liqglo.m_deferredBlockSize, gStatus );
	liquidGetPlugValue( rGlobalNode, "useRenderScript", useRenderScript, gStatus );
	liquidGetPlugValue( rGlobalNode, "remoteRender", liqglo.remoteRender, gStatus );
	liquidGetPlugValue( rGlobalNode, "renderAllCurves", liqglo.liqglo_renderAllCurves, gStatus );
	liquidGetPlugValue( rGlobalNode, "illuminateByDefault", liqglo.m_illuminateByDefault, gStatus );
	liquidGetPlugValue( rGlobalNode, "liquidSetLightLinking", m_liquidSetLightLinking, gStatus );
	liquidGetPlugValue( rGlobalNode, "ignoreLights", m_ignoreLights, gStatus );
	liquidGetPlugValue( rGlobalNode, "ignoreSurfaces", m_ignoreSurfaces, gStatus );
	liquidGetPlugValue( rGlobalNode, "ignoreDisplacements", m_ignoreDisplacements, gStatus );
	liquidGetPlugValue( rGlobalNode, "ignoreVolumes", m_ignoreVolumes, gStatus );

	liquidGetPlugValue( rGlobalNode, "useNewTranslator", m_useNewTranslator, gStatus );
	liquidGetPlugValue( rGlobalNode, "outputShaderGraph", m_OutputShaderGraph, gStatus );
	liquidGetPlugValue( rGlobalNode, "outputDebugMsgToRib", m_outputDebugMsgToRib, gStatus );
	liquidGetPlugValue( rGlobalNode, "outputShadowPass", m_outputShadowPass, gStatus );
	liquidGetPlugValue( rGlobalNode, "outputHeroPass", m_outputHeroPass, gStatus );

	liquidGetPlugValue( rGlobalNode, "netRManRender", liqglo.useNetRman, gStatus );
	liquidGetPlugValue( rGlobalNode, "ignoreShadows", liqglo.liqglo_doShadows, gStatus );
	liqglo.liqglo_doShadows = !liqglo.liqglo_doShadows;

	liquidGetPlugValue( rGlobalNode, "shapeOnlyInShadowNames", liqglo.liqglo_shapeOnlyInShadowNames, gStatus );
	liquidGetPlugValue( rGlobalNode, "fullShadowRibs", liqglo.fullShadowRib, gStatus );
	liquidGetPlugValue( rGlobalNode, "binaryOutput", liqglo.liqglo_doBinary, gStatus );
	liquidGetPlugValue( rGlobalNode, "lazyCompute", m_lazyCompute, gStatus );
	liquidGetPlugValue( rGlobalNode, "outputShadersInShadows", m_outputShadersInShadows, gStatus );
	// Moritz: added new options for light/shader output in deep shadows
	liquidGetPlugValue( rGlobalNode, "outputShadersInDeepShadows", m_outputShadersInDeepShadows, gStatus );
	liquidGetPlugValue( rGlobalNode, "outputLightsInDeepShadows", m_outputLightsInDeepShadows, gStatus );

	liquidGetPlugValue( rGlobalNode, "outputMeshUVs", liqglo.liqglo_outputMeshUVs, gStatus );
	liquidGetPlugValue( rGlobalNode, "outputMeshAsRMSArrays", liqglo.liqglo_outputMeshAsRMSArrays, gStatus );
	liquidGetPlugValue( rGlobalNode, "compressedOutput", liqglo.liqglo_doCompression, gStatus );
	liquidGetPlugValue( rGlobalNode, "exportReadArchive", liqglo.m_exportReadArchive, gStatus );
	liquidGetPlugValue( rGlobalNode, "renderJobName", renderJobName, gStatus );
	liquidGetPlugValue( rGlobalNode, "doAnimation", m_animation, gStatus );
	if( m_animation ) 
	{
		MString frameSequence;
		liquidGetPlugValue( rGlobalNode, "frameSequence", frameSequence, gStatus );
		if( gStatus == MS::kSuccess ) 
			liqglo.frameNumbers = generateFrameNumbers( std::string( frameSequence.asChar() ) );
	}
	liquidGetPlugValue( rGlobalNode, "doPadding", liqglo.liqglo_doExtensionPadding, gStatus );
	if( liqglo.liqglo_doExtensionPadding ) 
		liquidGetPlugValue( rGlobalNode, "padding", liqglo.liqglo_outPadding, gStatus );
	{  
		int gWidth, gHeight;
		liquidGetPlugValue( rGlobalNode, "xResolution", gWidth, gStatus );
		liquidGetPlugValue( rGlobalNode, "yResolution", gHeight, gStatus );
		if( gWidth > 0 ) 
			width = gWidth;
		if( gHeight > 0 ) 
			height = gHeight;
	}


	liquidGetPlugValue( rGlobalNode, "transformationBlur", liqglo.liqglo_doMotion, gStatus );

	liquidGetPlugValue( rGlobalNode, "deformationBlur", liqglo.liqglo_doDef, gStatus );

	liquidGetPlugValue( rGlobalNode, "shutterEfficiency", liqglo.liqglo_shutterEfficiency, gStatus );
	liquidGetPlugValue( rGlobalNode, "motionBlurSamples", liqglo.liqglo_motionSamples, gStatus );
	if( liqglo.liqglo_motionSamples > LIQMAXMOTIONSAMPLES )
		liqglo.liqglo_motionSamples = LIQMAXMOTIONSAMPLES;
	liquidGetPlugValue( rGlobalNode, "relativeMotion", liqglo.liqglo_relativeMotion, gStatus );


	getOtherParameters(rGlobalNode);	
	getLimitsParameters(rGlobalNode);

	liquidGetPlugValue( rGlobalNode, "cleanRib", liqglo.cleanRib, gStatus );
	liquidGetPlugValue( rGlobalNode, "cleanRenderScript", cleanRenderScript, gStatus );
	liquidGetPlugValue( rGlobalNode, "cleanTex", cleanTextures, gStatus );
	liquidGetPlugValue( rGlobalNode, "cleanShad", cleanShadows, gStatus );
	liquidGetPlugValue( rGlobalNode, "justRib", m_justRib, gStatus );


	getStatisticsParameters(rGlobalNode);

	// Philippe : OBSOLETE ?


	// taking into account liquidRibRequest nodes and preposterous mel scripts - Alf
	{
		MStringArray requestArray;
		MString request;

		liquidGetPlugValue( rGlobalNode, "preFrameBegin", request, gStatus );
		if( request != "" )
			requestArray.append( request );
		// add rib request node values
		request = parseLiquidRibRequest( requestArray, "preFrame" );
		if( request != "" )
			liqglo.m_preFrameRIB = parseString( request );
		requestArray.clear();
		request.clear();

		liquidGetPlugValue( rGlobalNode, "preWorld", request, gStatus );
		if( request != "" )
			requestArray.append( request );
		request = parseLiquidRibRequest( requestArray, "preWorld" );
		if( request != "" )
			liqglo.m_preWorldRIB = parseString( request );
		requestArray.clear();
		request.clear();

		liquidGetPlugValue( rGlobalNode, "postWorld", request, gStatus );
		if( request != "" )
			requestArray.append( request );
		request = parseLiquidRibRequest( requestArray, "postWorld" );
		if( request != "" )
			liqglo.m_postWorldRIB = parseString( request );
		requestArray.clear();
		request.clear();

		liquidGetPlugValue( rGlobalNode, "preGeom", request, gStatus );
		if( request != "" )
			requestArray.append( request );
		request = parseLiquidRibRequest( requestArray, "preGeom" );
		if( request != "" )
			m_preGeomRIB = parseString( request );
	}
	liquidGetPlugValue( rGlobalNode, "renderScriptFormat", var, gStatus );
	if( gStatus == MS::kSuccess ) 
		m_renderScriptFormat = ( enum renderScriptFormat ) var;

	liquidGetPlugValue( rGlobalNode, "renderScriptCommand", varVal, gStatus );
	m_renderScriptCommand = ( varVal != "" )? parseString( varVal, false ) : "alfred";

	liquidGetPlugValue( rGlobalNode, "currentLayerRifParameters", liqglo.liqglo_rifParams, gStatus );

	setOutDirs();
	setSearchPaths();
}