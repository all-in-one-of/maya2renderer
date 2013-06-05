#include <liqLightMgr.h>

#include <liqRibTranslator.h>
#include <liqStructJob.h>
#include <liqGlobalHelpers.h>
//
tLightMgr::tLightMgr()
{

}
//
tLightMgr::~tLightMgr()
{

}
//
void tLightMgr::scanScene(const float lframe__, const int sample__,
						  boost::shared_ptr< liqRibHT > &htable__,
						  int &count__,
						  MStatus &returnStatus__)
{
	CM_TRACE_FUNC("tLightMgr::scanScene("<<lframe__<<","<<sample__<<", htable__, count, returnStatus__)");
	//[refactor 9] begin from scanScene()
	MItDag dagLightIterator( MItDag::kDepthFirst, MFn::kLight, &returnStatus__);

	for (; !dagLightIterator.isDone(); dagLightIterator.next()) 
	{
#if (Refactoring == 0)
		LIQ_CHECK_CANCEL_REQUEST;
#endif
		MDagPath path;
		MObject currentNode;
		currentNode = dagLightIterator.item();
		MFnDagNode dagNode;
		dagLightIterator.getPath( path );
		if(MS::kSuccess != returnStatus__) 
			continue;
		if(!currentNode.hasFn(MFn::kDagNode)) 
			continue;
		returnStatus__ = dagNode.setObject( currentNode );
		if(MS::kSuccess != returnStatus__) 
			continue;

		bool useSamples( ( sample__ > 0 ) && isObjectMotionBlur( path ) );

		// scanScene: if it's a light then insert it into the hash table
		if( currentNode.hasFn( MFn::kLight ) ) 
		{
			if( currentNode.hasFn( MFn::kAreaLight ) ) 
			{
				// add a coordSys node if necessary
				MStatus status;
				bool coordsysExists = false;
				// get the coordsys name
				MFnDependencyNode areaLightDep( currentNode );
				MString coordsysName = areaLightDep.name()+"CoordSys";
				// get the transform
				MObject transform = path.transform();
				// check the coordsys does not exist yet under the transform
				MFnDagNode transformDag( transform );
				int numChildren = transformDag.childCount();
				if( numChildren > 1 ) 
				{
					for ( unsigned int i=0; i<numChildren; i++ ) 
					{
						MObject childObj = transformDag.child( i, &status );
						if( status == MS::kSuccess && childObj.hasFn( MFn::kLocator ) ) 
						{
							MFnDependencyNode test(childObj);
							if( test.name() == coordsysName ) 
								coordsysExists = true;
						}
					}
				}
				if( !coordsysExists ) 
				{
					// create the coordsys
					MDagModifier coordsysNode;
					MObject coordsysObj  = coordsysNode.createNode( "liquidCoordSys", transform, &status );
					if( status == MS::kSuccess ) 
					{
						// rename node to match light name
						coordsysNode.doIt();
						if( status == MS::kSuccess ) 
						{
							MFnDependencyNode coordsysDep( coordsysObj );
							coordsysDep.setName( coordsysName );
						}
					}
				}
			}

			htable__->insert( path, lframe__, ( useSamples )? sample__ : 0, MRT_Light,	count__++ );

			continue;
		}
	}
	//[refactor 9] end to scanScene()
}
//
MStatus tLightMgr::buildShadowJob( 
	structJob &thisJob___, bool m_lazyCompute__
	)
{
	CM_TRACE_FUNC("tLightMgr::buildShadowJob(thisJob___, "<<m_lazyCompute__<<")");

	MStatus returnStatus__;
	MStatus status__;
	MDagPath lightPath__;
	//
	// lights
	//
	//[refactor 2.1] begin
	MItDag dagIterator( MItDag::kDepthFirst, MFn::kLight, &returnStatus__ );
	for (; !dagIterator.isDone(); dagIterator.next()) 
	{
		if( !dagIterator.getPath( lightPath__ ) ) 
			continue;

		thisJob___.pass = rpNone; // reset RenderPass type
		thisJob___.isPoint       = false;
		thisJob___.pointDir = pPX;
		thisJob___.shadowObjectSet.clear();
		thisJob___.shadowArchiveRibDone = false;
		thisJob___.shadowType    = stStandart;
		thisJob___.shadowHiderType = shNone;
		thisJob___.hasShadowCam = false;
		thisJob___.shadowAggregation = false;
		thisJob___.midPointRatio = 0;
		thisJob___.samples = 1;
		thisJob___.shadingRate = 1.0;
		thisJob___.shadingRateFactor = 1.0;
		thisJob___.aspectRatio = 1.0;
		thisJob___.volume = viNone;
		thisJob___.deepShadowOption.clear();
		thisJob___.renderFrame   = liqglo.liqglo_lframe;
		thisJob___.everyFrame    = true;
		thisJob___.skip          = false;

		bool usesDepthMap = false;
		MFnLight fnLightNode( lightPath__ );
		liquidGetPlugValue( fnLightNode, "useDepthMapShadows", usesDepthMap, status__ );
		if( usesDepthMap && areObjectAndParentsVisible( lightPath__ ) ) 
		{
			thisJob___.pass = rpShadowMap;
			thisJob___.imageMode = "z";
			thisJob___.format = "shadow";

			// philippe : we grab the job's resolution now instead of in the output phase
			// that way , we can make sure one light can generate many shadow maps
			// with different resolutions
			thisJob___.aspectRatio = 1.0;
			liquidGetPlugValue( fnLightNode, "dmapResolution", thisJob___.width, status__ );
			thisJob___.height = thisJob___.width;

			// Get to our shader node.
			//
			MPlug liquidLightShaderNodeConnection;
			MStatus liquidLightShaderStatus;
			liquidLightShaderNodeConnection = fnLightNode.findPlug( "liquidLightShaderNode", &liquidLightShaderStatus );
			if( liquidLightShaderStatus == MS::kSuccess && liquidLightShaderNodeConnection.isConnected() )
			{
				MPlugArray liquidLightShaderNodePlugArray;
				liquidLightShaderNodeConnection.connectedTo( liquidLightShaderNodePlugArray, true, true );
				MFnDependencyNode fnLightShaderNode( liquidLightShaderNodePlugArray[0].node() );

				// Now grab the parameters.
				//
				bool deepShadows = false;
				liquidGetPlugValue( fnLightShaderNode, "deepShadows", deepShadows, status__ );

				// Only use the pixel samples and volume interpretation with deep shadows.
				//
				if( deepShadows )
				{
					thisJob___.shadowType = stDeep;

					int samples = (int)thisJob___.samples;
					liquidGetPlugValue( fnLightShaderNode, "pixelSamples", samples, status__ );
					thisJob___.samples = (short)samples;

					int volume = (int)thisJob___.volume;
					liquidGetPlugValue( fnLightShaderNode, "volumeInterpretation", volume, status__ );
					thisJob___.volume = (VolumeInterpretation)volume;

					thisJob___.imageMode    = liqglo.liquidRenderer.dshImageMode;        //"deepopacity";
					thisJob___.format       = liqglo.liquidRenderer.dshDisplayName;    //"deepshad";

					int displayImageMode = 0; // 0 = default
					liquidGetPlugValue( fnLightShaderNode, "liqDeepShadowsDisplayMode", displayImageMode, status__ );
					if ( displayImageMode ) thisJob___.imageMode = MString( "deepprevdisttotal" );

				}

				// philippe : check the shadow rendering frequency
				//
				liquidGetPlugValue( fnLightShaderNode, "everyFrame", thisJob___.everyFrame, status__ );

				// philippe : this is crucial, as we rely on the renderFrame to check
				// which frame the scene should be scanned for that job.
				// If the job is a shadow rendering once only at a given frame, we take the
				// renderAtFrame attribute, otherwise, the current time.
				//
				if( !thisJob___.everyFrame ) 
				{
					thisJob___.renderFrame = 0; // should be Reference frame from Globals 
					liquidGetPlugValue( fnLightShaderNode, "renderAtFrame", thisJob___.renderFrame, status__ ); 
				}
				// Check if the shadow aggregation option is used
				liquidGetPlugValue( fnLightShaderNode, "aggregateShadowMaps", thisJob___.shadowAggregation, status__ );  

				// philippe : check the shadow geometry set
				//
				liquidGetPlugValue( fnLightShaderNode, "geometrySet", thisJob___.shadowObjectSet, status__ );
				liquidGetPlugValue( fnLightShaderNode, "shadingRateFactor", thisJob___.shadingRateFactor, status__ );
			} 
			else 
			{
				/* Here we support the same options as those found on light shader nodes
				but we look for dynamic attributes, so we need a bit more error checking.
				*/
				//MPlug paramPlug = fnLightNode.findPlug( "deepShadows", &status );
				bool deepShadows = false;
				liquidGetPlugValue( fnLightNode, "deepShadows", deepShadows, status__ );
				if ( deepShadows ) 
				{
					thisJob___.shadowType = stDeep;

					int samples = (int)thisJob___.samples;
					liquidGetPlugValue( fnLightNode, "pixelSamples", samples, status__ );
					thisJob___.samples = (short)samples;

					int volume = (int)thisJob___.volume;
					liquidGetPlugValue( fnLightNode, "volumeInterpretation", volume, status__ );
					thisJob___.volume = (VolumeInterpretation)volume;

					thisJob___.imageMode    = liqglo.liquidRenderer.dshImageMode;        //"deepopacity";
					thisJob___.format       = liqglo.liquidRenderer.dshDisplayName;    //"deepshad";

					int displayImageMode = 0; // 0 = default
					liquidGetPlugValue( fnLightNode, "liqDeepShadowsDisplayMode", displayImageMode, status__ );
					if ( displayImageMode ) 
						thisJob___.imageMode = MString( "deepprevdisttotal" );

				}
				liquidGetPlugValue( fnLightNode, "everyFrame", thisJob___.everyFrame, status__ );
				if ( !thisJob___.everyFrame ) 
				{
					thisJob___.renderFrame = 0; // should be Reference frame from Globals 
					liquidGetPlugValue( fnLightNode, "renderAtFrame", thisJob___.renderFrame, status__ );  
				}
				liquidGetPlugValue( fnLightNode, "geometrySet", thisJob___.shadowObjectSet, status__ );  
				liquidGetPlugValue( fnLightNode, "shadingRateFactor", thisJob___.shadingRateFactor, status__ ); 
			}


			// this will store the shadow camera path and the test's result
			bool lightHasShadowCam = false;//1
			MDagPathArray shadowCamPath;

			if( lightPath__.hasFn( MFn::kSpotLight ) || lightPath__.hasFn( MFn::kDirectionalLight ) ) 
			{	
				tLightMgr::buildShadowJob_SpotAndDirectionLight(
					thisJob___, shadowCamPath, fnLightNode, lightPath__,
					m_lazyCompute__,
					lightHasShadowCam
					);
			} 
			else if( lightPath__.hasFn(MFn::kPointLight) ) 
			{
				tLightMgr::buildShadowJob_PointLight(
					thisJob___, shadowCamPath, fnLightNode, lightPath__,
					m_lazyCompute__,
					lightHasShadowCam
					);
			}//else if( lightPath__.hasFn(MFn::kPointLight) )  

			//================= jobs for shadow cameras ==================
			//[refactor][2.1.3 begin] from buildJobs()
			// if the job has shadow cameras, we will store them here
			//
			if( lightHasShadowCam )
			{
			int isAggregate = thisJob___.shadowAggregation;
			for( unsigned i( 0 ); i < shadowCamPath.length(); i++ )
			{
				if( !i && isAggregate )
					thisJob___.shadowAggregation = 0;
				else if ( isAggregate )
					thisJob___.shadowAggregation = 1;
				else
					thisJob___.shadowAggregation = 0;
				thisJob___.shadowCamPath = shadowCamPath[ i ];
				thisJob___.hasShadowCam = true;

				MFnDependencyNode shadowCamDepNode( shadowCamPath[ i ].node() );
				thisJob___.name = shadowCamDepNode.name();
				if( isAggregate )
					thisJob___.texName = fnLightNode.name(); //MFnDependencyNode( shadowCamPath[ i ].node() ).name();
				//					else
				//						thisJob.texName = "";
				//						thisJob.name = shadowCamDepNode.name();
				if( liquidGetPlugValue( shadowCamDepNode, "liqShadowResolution", thisJob___.width, status__ ) == MS::kSuccess )
					thisJob___.height = thisJob___.width;
				bool isMidPointShadow = false;
				liquidGetPlugValue( shadowCamDepNode, "liqMidPointShadow", isMidPointShadow, status__ );
				if ( isMidPointShadow ) thisJob___.shadowType = stMidPoint;

				thisJob___.midPointRatio = 0;
				liquidGetPlugValue( shadowCamDepNode, "liqMidPointRatio", thisJob___.midPointRatio, status__ );
					
				bool deepShadows = false;
				liquidGetPlugValue( shadowCamDepNode, "liqDeepShadows", deepShadows, status__ );
				if ( deepShadows ) 
				{  
					thisJob___.shadowType = stDeep;

					int samples = (int)thisJob___.samples;
					liquidGetPlugValue( shadowCamDepNode, "liqPixelSamples", samples, status__ );
					thisJob___.samples = (short)samples;

					int volume = (int)thisJob___.volume;
					liquidGetPlugValue( shadowCamDepNode, "liqVolumeInterpretation", volume, status__ );
					thisJob___.volume = (VolumeInterpretation)volume;

					int displayImageMode = 0; // 0 = default
					liquidGetPlugValue( shadowCamDepNode, "liqDeepShadowsDisplayMode", displayImageMode, status__ );
					if ( displayImageMode ) thisJob___.imageMode = MString( "deepprevdisttotal" );
				}

				liquidGetPlugValue( shadowCamDepNode, "liqEveryFrame", thisJob___.everyFrame, status__ );
				// as previously : this is important as thisJob.renderFrame corresponds to the
				// scene scanning time.
				if ( !thisJob___.everyFrame )
				{
					thisJob___.renderFrame = 0; // should be Reference frame from Globals 
					liquidGetPlugValue( shadowCamDepNode, "liqRenderAtFrame", thisJob___.renderFrame, status__ );
				}
				liquidGetPlugValue( shadowCamDepNode, "liqGeometrySet", thisJob___.shadowObjectSet, status__ );
				liquidGetPlugValue( shadowCamDepNode, "liqShadingRateFactor", thisJob___.shadingRateFactor, status__ );
				// test if the file is already on disk...
				if( m_lazyCompute__ )
				{
					MString fileName( liqRibTranslator::getInstancePtr()->generateFileName( ( fileGenMode )fgm_shadow_tex, thisJob___ ) );
					if( fileExists( fileName ) )
						continue;
				}
				liqRibTranslator::getInstancePtr()->jobList.push_back( thisJob___ );
			}//for( unsigned i( 0 ); i < shadowCamPath.length(); i++ )
			//[refactor][2.1.3 end] from buildJobs()
			}//if( lightHasShadowCam )
		} // if( usesDepthMap && areObjectAndParentsVisible( lightPath__ ) ) 
	} // light dagIterator
	return returnStatus__;
}
//
//
MStatus tLightMgr::buildShadowCameraJob( 
	structJob &thisJob___, bool m_lazyCompute__
	)
{
	CM_TRACE_FUNC("tLightMgr::buildShadowCameraJob(thisJob___, "<<m_lazyCompute__<<")");

	MStatus returnStatus__ = MS::kSuccess;
	MStatus status__;
	//
	// cameras
	//
	//[refactor][2.1.4 begin] from buildJob()
	MDagPath cameraPath;
	MItDag dagCameraIterator( MItDag::kDepthFirst, MFn::kCamera, &returnStatus__ );
	for ( ; !dagCameraIterator.isDone(); dagCameraIterator.next() ) 
	{
		if( !dagCameraIterator.getPath(cameraPath) ) 
			continue;
		bool usesDepthMap( false );
		MFnCamera fnCameraNode( cameraPath );
		liquidGetPlugValue( fnCameraNode, "useDepthMapShadows", usesDepthMap, status__ );
		if( usesDepthMap && areObjectAndParentsVisible( cameraPath ) ) 
		{
			thisJob___.pass = rpShadowMap;
			//
			// We have a shadow job, so find out if we need to use deep shadows,
			// and the pixel sample count
			//
			//thisJob___.shadowPixelSamples = 1;
			//thisJob___.shadowVolumeInterpretation = 1;

			thisJob___.samples = 1;
			thisJob___.volume = viNone;

			bool deepShadows = false;
			liquidGetPlugValue( fnCameraNode, "deepShadows", deepShadows, status__ );

			// Only use the pixel samples and volume interpretation with deep shadows.
			//
			if( deepShadows )
			{
				thisJob___.shadowType = stDeep;

				int samples = (int)thisJob___.samples;
				liquidGetPlugValue( fnCameraNode, "pixelSamples", samples, status__ );
				thisJob___.samples = (short)samples;

				int volume = (int)thisJob___.volume;
				liquidGetPlugValue( fnCameraNode, "volumeInterpretation", volume, status__ );
				thisJob___.volume = (VolumeInterpretation)volume;

				thisJob___.imageMode    = liqglo.liquidRenderer.dshImageMode;        //"deepopacity";
				thisJob___.format       = liqglo.liquidRenderer.dshDisplayName;    //"deepshad";

				int displayImageMode = 0; // 0 = default
				liquidGetPlugValue( fnCameraNode, "liqDeepShadowsDisplayMode", displayImageMode, status__ );
				if ( displayImageMode )
					thisJob___.imageMode = MString("deepprevdisttotal");

			}

			thisJob___.hasShadowCam = true;
			thisJob___.shadowCamPath = cameraPath;
			thisJob___.path = cameraPath;
			thisJob___.name = fnCameraNode.name();
			//thisJob___.isShadow = true;
			thisJob___.isPoint = false;
			thisJob___.isShadowPass = false;

			// check to see if the minmax shadow option is used
			bool isMinMaxShadow = false;
			liquidGetPlugValue( fnCameraNode, "liquidMinMaxShadow", isMinMaxShadow, status__ ); 
			if ( isMinMaxShadow ) thisJob___.shadowType = stMinMax;

			// check to see if the midpoint shadow option is used
			bool isMidPointShadow = false;
			liquidGetPlugValue(  fnCameraNode, "useMidDistDmap", isMidPointShadow, status__ ); 
			if ( isMidPointShadow ) thisJob___.shadowType = stMidPoint;

			bool computeShadow( true );
			if( m_lazyCompute__ ) 
			{
				MString fileName( liqRibTranslator::getInstancePtr()->generateFileName( ( fileGenMode )fgm_shadow_tex, thisJob___ ) );
				if( fileExists( fileName ) ) continue;
			}

			if( computeShadow ) 
				liqRibTranslator::getInstancePtr()->jobList.push_back( thisJob___ );
		}
		//[refactor][2.1.4 end] from buildJob()
	} // camera dagIterator

	return returnStatus__;
}
//
void tLightMgr::buildShadowJob_SpotAndDirectionLight(
	structJob &thisJob___, MDagPathArray &shadowCamPath, 
	const MFnLight &fnLightNode, const MDagPath &lightPath__,
	const bool m_lazyCompute__,
	bool &lightHasShadowCam
	)
{
	CM_TRACE_FUNC("tLightMgr::buildShadowJob_SpotAndDirectionLight(thisJob___, shadowCamPath, "<<fnLightNode.name().asChar()<<","<<lightPath__.fullPathName().asChar()<<","<<m_lazyCompute__<<")");

	MStatus status__;
	//[refactor 2.1.1] begin
	bool computeShadow = true;
	thisJob___.hasShadowCam = false;
	MPlug liquidLightShaderNodeConnection;
	MStatus liquidLightShaderStatus;
	liquidLightShaderNodeConnection = fnLightNode.findPlug( "liquidLightShaderNode", &liquidLightShaderStatus );

	if( liquidLightShaderStatus == MS::kSuccess && liquidLightShaderNodeConnection.isConnected() ) 
	{
		// a shader is connected to the light !
		MPlugArray liquidLightShaderNodePlugArray;
		liquidLightShaderNodeConnection.connectedTo( liquidLightShaderNodePlugArray, true, true );
		MFnDependencyNode fnLightShaderNode( liquidLightShaderNodePlugArray[0].node() );

		// has the main shadow been disabled ?
		liquidGetPlugValue( fnLightShaderNode, "generateMainShadow", computeShadow, status__ ); 

		// look for shadow cameras...
		MStatus stat;
		//[refactor 2.1.1.1] begin//added in liquidMaya r772   begin
		// at first, check if shadow main camera is specified
		// cerr << ">> at first, check if shadow main camera is specified for "  << lightPath.fullPathName().asChar() << endl;

		MString camName;
		liquidGetPlugValue( fnLightShaderNode, "shadowMainCamera", camName, stat ); 
		if ( stat == MS::kSuccess && camName != "" )
		{
			// cerr << ">> Light node has main shadow camera : " << camName.asChar() << endl;
			MDagPath cameraPath;
			MSelectionList camList;
			camList.add( camName );
			camList.getDagPath( 0, cameraPath );
			if ( cameraPath.hasFn( MFn::kCamera ) )
			{
				// cerr << ">> cameraPath : "<< cameraPath.fullPathName().asChar() << endl;
				thisJob___.hasShadowCam = true;
				thisJob___.shadowCamPath = cameraPath;
			}
			else
			{
				// cerr << ">> Invalid camera name " << endl;
				MString err("Invalid main shadow camera name " + camName + " for light " + lightPath__.fullPathName());
				liquidMessage( err, messageError );
			}
		}
		// now we're looking for extra cameras 
		//[refactor 2.1.1.1] end//added in liquidMaya r772   end

		MPlug shadowCamPlug = fnLightShaderNode.findPlug( "shadowCameras", &stat );
		// find the multi message attribute...
		if( stat == MS::kSuccess ) 
		{
			int numShadowCams = shadowCamPlug.numElements();
			//cout <<">> got "<<numShadowCams<<" shadow cameras"<<endl;
			// iterate through existing array elements
			for ( unsigned int i=0; i<numShadowCams; i++ ) 
			{
				stat.clear();
				MPlug camPlug = shadowCamPlug.elementByPhysicalIndex( i, &stat );
				if( stat != MS::kSuccess ) 
					continue;
				MPlugArray shadowCamPlugArray;

				// if the element is connected, keep going...
				if( camPlug.connectedTo( shadowCamPlugArray, true, false ) ) 
				{
					MFnDependencyNode shadowCamDepNode = shadowCamPlugArray[0].node();
					//cout <<"shadow camera plug "<<i<<" is connected to "<<shadowCamDepNode.name()<<endl;

					MDagPath cameraPath;
					cameraPath.getAPathTo( shadowCamPlugArray[0].node(), cameraPath);
					//cout <<"cameraPath : "<<cameraPath.fullPathName()<<endl;
					shadowCamPath.append( cameraPath );
					lightHasShadowCam = true;//2
				}
			}
		}
	}
	thisJob___.path = lightPath__;
	thisJob___.name = fnLightNode.name();
	//thisJob___.texName = "";
	//thisJob___.isShadow = true;
	//thisJob___.isPoint = false;
	//thisJob___.isShadowPass = false;

	// check to see if the minmax shadow option is used
	bool isMinMaxShadow = false;
	liquidGetPlugValue( fnLightNode, "liquidMinMaxShadow", isMinMaxShadow, status__ ); 
	if ( isMinMaxShadow ) thisJob___.shadowType = stMinMax;

	// check to see if the midpoint shadow option is used
	bool isMidPointShadow = false;
	liquidGetPlugValue( fnLightNode, "useMidDistDmap", isMidPointShadow, status__ ); 
	if ( isMidPointShadow ) thisJob___.shadowType = stMidPoint;

	// in lazy compute mode, we check if the map is already on disk first.
	if( m_lazyCompute__ && computeShadow ) 
	{
		MString fileName( liqRibTranslator::getInstancePtr()->generateFileName( (fileGenMode) fgm_shadow_tex, thisJob___ ) );
		if( fileExists( fileName ) ) 
			computeShadow = false;
	}
	//
	// store the main shadow map    *****************************
	//
	if( computeShadow )
		liqRibTranslator::getInstancePtr()->jobList.push_back( thisJob___ );
	// We have to handle point lights differently as they need 6 shadow maps!

	//[refactor 2.1.1] end
}
//
void tLightMgr::buildShadowJob_PointLight(
	structJob &thisJob___, MDagPathArray &shadowCamPath, 
	const MFnLight &fnLightNode, 
	const MDagPath &lightPath__,
	const bool m_lazyCompute__,
	bool &lightHasShadowCam
	)
{
	CM_TRACE_FUNC("tLightMgr::buildShadowJob_PointLight(thisJob___, shadowCamPath, "<<fnLightNode.name().asChar()<<","<<lightPath__.fullPathName().asChar()<<","<<m_lazyCompute__<<")");

	MStatus status__;

	thisJob___.hasShadowCam = false;
	thisJob___.path = lightPath__;
	thisJob___.name = fnLightNode.name();
	//thisJob.isShadow = true;

	thisJob___.isShadowPass = false;
	thisJob___.isPoint = true;

	// check to see if the midpoint shadow option is used
	bool isMidPointShadow = false;
	liquidGetPlugValue( fnLightNode, "useMidDistDmap", isMidPointShadow, status__ );
	if ( isMidPointShadow ) thisJob___.shadowType = stMidPoint;

	//[refactor 2.1.2] begin
	for ( unsigned dirOn( 0 ); dirOn < 6; dirOn++ ) 
	{
		thisJob___.pointDir = ( PointLightDirection )dirOn;

		bool computeShadow = true;
		MStatus liquidLightShaderStatus;
		MPlug liquidLightShaderNodeConnection( fnLightNode.findPlug( "liquidLightShaderNode", &liquidLightShaderStatus ) );

		if( liquidLightShaderStatus == MS::kSuccess && liquidLightShaderNodeConnection.isConnected() ) 
		{
			// a shader is connected to the light !
			MPlugArray liquidLightShaderNodePlugArray;
			liquidLightShaderNodeConnection.connectedTo( liquidLightShaderNodePlugArray, true, true );
			MFnDependencyNode fnLightShaderNode( liquidLightShaderNodePlugArray[0].node() );

			// has the main shadow been disabled ?
			liquidGetPlugValue( fnLightShaderNode, "generateMainShadow", computeShadow, status__ );

			// look for shadow cameras...
			MStatus stat;
			MPlug shadowCamPlug( fnLightShaderNode.findPlug( "shadowCameras", &stat ) );

			// find the multi message attribute...
			if( stat == MS::kSuccess ) 
			{
				int numShadowCams = shadowCamPlug.numElements();
				//cout <<">> got "<<numShadowCams<<" shadow cameras"<<endl;
				// iterate through existing array elements
				for ( unsigned i( 0 ); i < numShadowCams; i++ ) 
				{
					stat.clear();
					MPlug camPlug = shadowCamPlug.elementByPhysicalIndex( i, &stat );
					if( stat != MS::kSuccess ) 
						continue;
					MPlugArray shadowCamPlugArray;

					// if the element is connected, keep going...
					if( camPlug.connectedTo( shadowCamPlugArray, true, false ) ) 
					{
						MFnDependencyNode shadowCamDepNode = shadowCamPlugArray[0].node();
						//cout <<"shadow camera plug "<<i<<" is connected to "<<shadowCamDepNode.name()<<endl;

						MDagPath cameraPath;
						cameraPath.getAPathTo( shadowCamPlugArray[0].node(), cameraPath);
						//cout <<"cameraPath : "<<cameraPath.fullPathName()<<endl;
						shadowCamPath.append( cameraPath );
						lightHasShadowCam = true;//3
					}
				}
			}
		}
		if( m_lazyCompute__ ) {
			MString fileName( liqRibTranslator::getInstancePtr()->generateFileName( 
				( fileGenMode )fgm_shadow_tex, thisJob___ ) 
			);
			if( fileExists( fileName ) ) 
				computeShadow = false;
		}
		if( computeShadow ) 
			liqRibTranslator::getInstancePtr()->jobList.push_back( thisJob___ );
	}//for ( unsigned dirOn( 0 ); dirOn < 6; dirOn++ )  
	//[refactor 2.1.2] end
}