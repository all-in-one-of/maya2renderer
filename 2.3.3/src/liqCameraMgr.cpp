#include <liqCameraMgr.h>
#include <liqRibTranslator.h>
//
tCameraMgr::tCameraMgr()
{

}
//
tCameraMgr::~tCameraMgr()
{

}
/**
* Calculate the port field of view for the camera.
*/
void tCameraMgr::portFieldOfView( int port_width, int port_height,
									   double& horizontal,
									   double& vertical,
									   const MFnCamera& fnCamera )const
{
	CM_TRACE_FUNC("tCameraMgr::portFieldOfView("<<port_width<<","<<port_height<<",&horizontal,&vertical,&fnCamera)");
	// note : works well - api tested
	double left, right, bottom, top;
	double aspect = (double) port_width / port_height;
	computeViewingFrustum(aspect,left,right,bottom,top,fnCamera);

	double neardb = fnCamera.nearClippingPlane();
	horizontal    = atan( ( ( right - left ) * 0.5 ) / neardb ) * 2.0;
	vertical      = atan( ( ( top - bottom ) * 0.5 ) / neardb ) * 2.0;
}

/**
* Calculate the viewing frustrum for the camera.
*/
void tCameraMgr::computeViewingFrustum ( double     window_aspect,
											  double&    left,
											  double&    right,
											  double&    bottom,
											  double&    top,
											  const MFnCamera& cam )const
{
	CM_TRACE_FUNC("tCameraMgr::computeViewingFrustum("<<window_aspect<<","<<",&left,&right,&bottom,&top,"<<cam.fullPathName().asChar()<<")");

	double film_aspect   = cam.aspectRatio();
	double aperture_x    = cam.horizontalFilmAperture();
	double aperture_y    = cam.verticalFilmAperture();
	double offset_x      = cam.horizontalFilmOffset();
	double offset_y      = cam.verticalFilmOffset();
	double focal_to_near = cam.nearClippingPlane() / (cam.focalLength() * MM_TO_INCH);

	focal_to_near *= cam.cameraScale();

	double scale_x = 1.0;
	double scale_y = 1.0;
	double translate_x = 0.0;
	double translate_y = 0.0;

	switch ( cam.filmFit() ) 
	{
	case MFnCamera::kFillFilmFit:
		if( window_aspect < film_aspect ) 
			scale_x = window_aspect / film_aspect;
		else 
			scale_y = film_aspect / window_aspect;
		break;
	case MFnCamera::kHorizontalFilmFit:
		scale_y = film_aspect / window_aspect;
		if( scale_y > 1.0 ) 
			translate_y = cam.filmFitOffset() * ( aperture_y - ( aperture_y * scale_y ) ) / 2.0;
		break;
	case MFnCamera::kVerticalFilmFit:
		scale_x = window_aspect / film_aspect;
		if(scale_x > 1.0 ) 
			translate_x = cam.filmFitOffset() * ( aperture_x - ( aperture_x * scale_x ) ) / 2.0;
		break;
	case MFnCamera::kOverscanFilmFit:
		if( window_aspect < film_aspect ) 
			scale_y = film_aspect / window_aspect;
		else 
			scale_x = window_aspect / film_aspect;
		break;
	case MFnCamera::kInvalid:
		break;
	}

	left   = focal_to_near * (-.5 * aperture_x * scale_x + offset_x + translate_x );
	right  = focal_to_near * ( .5 * aperture_x * scale_x + offset_x + translate_x );
	bottom = focal_to_near * (-.5 * aperture_y * scale_y + offset_y + translate_y );
	top    = focal_to_near * ( .5 * aperture_y * scale_y + offset_y + translate_y );

	// NOTE :
	//      all the code above could be replaced by :
	//
	//      cam.getRenderingFrustum( window_aspect, left, right, bottom, top );
	//
	//      should we keep this for educationnal purposes or use the API call ??
}
/**
 * getCameraTransform
 */
MStatus tCameraMgr::getCameraTransform(const MFnCamera& cam, structCamera &camStruct )
{
	CM_TRACE_FUNC("tCameraMgr::getCameraTransform("<<cam.fullPathName().asChar()<<",)");
	MStatus status;
	MDagPath cameraPath;
	cam.getPath(cameraPath);
	//MString fullpathname(cameraPath.fullPathName());
	MTransformationMatrix xform( cameraPath.inclusiveMatrix(&status) );
	if ( status != MS::kSuccess ) // error ?!... set identity...
	{
		char errorMsg[512];
		sprintf(errorMsg, "Cannot get transfo matrix for camera '%s' \n", cam.name().asChar());
		//liquidMessage(errorMsg, messageError );
		printf(errorMsg);
		MMatrix id;
		camStruct.mat = id;
		return MS::kFailure;
	}
	// MMatrix mxform = xform.asMatrix();
	// printf("CAM MATRIX '%s' : \n", cam.name().asChar() );
	// printf("%f %f %f %f \n", mxform(0, 0), mxform(0, 1), mxform(0, 2), mxform(0, 3));
	// printf("%f %f %f %f \n", mxform(1, 0), mxform(1, 1), mxform(1, 2), mxform(1, 3));
	// printf("%f %f %f %f \n", mxform(2, 0), mxform(2, 1), mxform(2, 2), mxform(2, 3));
	// printf("%f %f %f %f \n", mxform(3, 0), mxform(3, 1), mxform(3, 2), mxform(3, 3));

	// the camera is pointing toward negative Z
	double scale[] = { 1, 1, -1 };
	xform.setScale( scale, MSpace::kTransform );

	// scanScene:
	// philippe : rotate the main camera 90 degrees around Z-axis if necessary
	// ( only in main camera )
	MMatrix camRotMatrix;
	if ( liqglo.liqglo_rotateCamera == true )
	{
		float crm[4][4] =	{	{  0.0,  1.0,  0.0,  0.0 },
								{ -1.0,  0.0,  0.0,  0.0 },
								{  0.0,  0.0,  1.0,  0.0 },
								{  0.0,  0.0,  0.0,  1.0 }	};
		camRotMatrix = crm;
	}
	camStruct.mat = xform.asMatrixInverse() * camRotMatrix;
	return MS::kSuccess;
}
/**
 * getCameraFilmOffset
 */
void tCameraMgr::getCameraFilmOffset(const MFnCamera& cam, structCamera &camStruct )
{
	CM_TRACE_FUNC("tCameraMgr::getCameraFilmOffset("<<cam.fullPathName().asChar()<<",)");
	// film back offsets
	double hSize, vSize, hOffset, vOffset;
	cam.getFilmFrustum( cam.focalLength(), hSize, vSize, hOffset, vOffset );

	double imr = ((float)camStruct.width / (float)camStruct.height);
	double fbr = hSize / vSize;
	double ho, vo;

	// convert inches to mm !
	hOffset *= 25.4;
	vOffset *= 25.4;
	switch( cam.filmFit() )
	{
	case MFnCamera::kVerticalFilmFit:
	case MFnCamera::kFillFilmFit:
		{
			ho = hOffset / vSize * 2.0;
			vo = vOffset / vSize * 2.0;
			break;
		}
	case MFnCamera::kHorizontalFilmFit:
	case MFnCamera::kOverscanFilmFit:
		{
			ho = hOffset / ( vSize * fbr / imr ) * 2.0;
			vo = vOffset / ( vSize * fbr / imr ) * 2.0;
			break;
		}
	default:
		{
			ho = 0;
			vo = 0;
			break;
		}
	}
	camStruct.horizontalFilmOffset = ho;
	camStruct.verticalFilmOffset   = vo;
}
/**
* Get information about the given camera.
*/
void tCameraMgr::getCameraInfo(const MFnCamera& cam, structCamera &camStruct )const
{
	CM_TRACE_FUNC("tCameraMgr::getCameraInfo("<<cam.fullPathName().asChar()<<",)");

	// Resolution can change if camera film-gate clips image
	// so we must keep camera width/height separate from render
	// globals width/height.
	//
	camStruct.width  = liqRibTranslator::getInstancePtr()->width;
	camStruct.height = liqRibTranslator::getInstancePtr()->height;

	// If we are using a film-gate then we may need to
	// adjust the resolution to simulate the 'letter-boxed'
	// effect.
	if( cam.filmFit() == MFnCamera::kHorizontalFilmFit ) 
	{
		if( !liqRibTranslator::getInstancePtr()->ignoreFilmGate ) 
		{
			double new_height = camStruct.width / ( cam.horizontalFilmAperture() / cam.verticalFilmAperture() );
			if( new_height < camStruct.height ) 
				camStruct.height = ( int )new_height;
		}

		double hfov, vfov;
		portFieldOfView( camStruct.width, camStruct.height, hfov, vfov, cam );
		camStruct.fov_ratio = hfov / vfov;
	}
	else if( cam.filmFit() == MFnCamera::kVerticalFilmFit ) 
	{
		double new_width = camStruct.height / ( cam.verticalFilmAperture() / cam.horizontalFilmAperture() );
		double hfov, vfov;

		// case 1 : film-gate smaller than resolution
		//         film-gate on
		if( ( new_width < camStruct.width ) && ( !liqRibTranslator::getInstancePtr()->ignoreFilmGate ) ) 
		{
			camStruct.width = ( int )new_width;
			camStruct.fov_ratio = 1.0;
		}
		// case 2 : film-gate smaller than resolution
		//         film-gate off
		else if( ( new_width < camStruct.width ) && ( liqRibTranslator::getInstancePtr()->ignoreFilmGate ) ) 
		{
			portFieldOfView( ( int )new_width, camStruct.height, hfov, vfov, cam );
			camStruct.fov_ratio = hfov / vfov;
		}
		// case 3 : film-gate larger than resolution
		//         film-gate on
		else if( !liqRibTranslator::getInstancePtr()->ignoreFilmGate ) 
		{
			portFieldOfView( ( int )new_width, camStruct.height, hfov, vfov, cam );
			camStruct.fov_ratio = hfov / vfov;
		}
		// case 4 : film-gate larger than resolution
		//         film-gate off
		else if( liqRibTranslator::getInstancePtr()->ignoreFilmGate ) 
		{
			portFieldOfView( ( int )new_width, camStruct.height, hfov, vfov, cam );
			camStruct.fov_ratio = hfov / vfov;
		}
	}
	else if( cam.filmFit() == MFnCamera::kOverscanFilmFit ) 
	{
		double new_height = camStruct.width / ( cam.horizontalFilmAperture() / cam.verticalFilmAperture() );
		double new_width = camStruct.height / ( cam.verticalFilmAperture() / cam.horizontalFilmAperture() );

		if( new_width < camStruct.width ) 
		{
			if( !liqRibTranslator::getInstancePtr()->ignoreFilmGate ) {
				camStruct.width = ( int ) new_width;
				camStruct.fov_ratio = 1.0;
			}
			else 
			{
				double hfov, vfov;
				portFieldOfView( ( int )new_width, camStruct.height, hfov, vfov, cam );
				camStruct.fov_ratio = hfov / vfov;
			}
		}
		else {
			if( !liqRibTranslator::getInstancePtr()->ignoreFilmGate )
				camStruct.height = ( int ) new_height;
			double hfov, vfov;
			portFieldOfView( camStruct.width, camStruct.height, hfov, vfov, cam );
			camStruct.fov_ratio = hfov / vfov;
		}
	}
	else if( cam.filmFit() == MFnCamera::kFillFilmFit ) 
	{
		double new_width = camStruct.height / ( cam.verticalFilmAperture() / cam.horizontalFilmAperture() );
		double hfov, vfov;
		if( new_width >= camStruct.width ) 
		{
			portFieldOfView( ( int )new_width, camStruct.height, hfov, vfov, cam );
			camStruct.fov_ratio = hfov / vfov;
		}
		else 
		{
			portFieldOfView( camStruct.width, camStruct.height, hfov, vfov, cam );
			camStruct.fov_ratio = hfov / vfov;
		}
	}
}
//
MStatus tCameraMgr::getCameraData( std::vector<structJob>::iterator &iter__ , const int sample__)
{
	CM_TRACE_FUNC("tCameraMgr::getCameraData(iter__,"<<sample__<<")");
	MStatus status;
	//[refactor 12] begin from  liqRibTranslator::scanScene()
	MDagPath path;
	MFnCamera   fnCamera( iter__->path );
	iter__->gotJobOptions = false;
	status.clear();
	MPlug cPlug = fnCamera.findPlug( MString( "ribOptions" ), &status );
	if( status == MS::kSuccess ) 
	{
		cPlug.getValue( iter__->jobOptions );
		iter__->gotJobOptions = true;
	}
	getCameraInfo( fnCamera, iter__->camera[sample__] );
	iter__->width = iter__->camera[sample__].width;
	iter__->height = iter__->camera[sample__].height;
	// scanScene: Renderman specifies shutter by time open
	// so we need to convert shutterAngle to time.
	// To do this convert shutterAngle to degrees and
	// divide by 360.
	//
	iter__->camera[sample__].shutter = fnCamera.shutterAngle() * 0.5 / M_PI;
	liqglo.liqglo_shutterTime = iter__->camera[sample__].shutter;
	iter__->camera[sample__].motionBlur     = fnCamera.isMotionBlur();				

	// scanScene: The camera's fov may not match the rendered image in Maya
	// if a film-fit is used. 'fov_ratio' is used to account for
	// this.
	//
	iter__->camera[sample__].hFOV = fnCamera.horizontalFieldOfView()/iter__->camera[sample__].fov_ratio;

	if ( fnCamera.isClippingPlanes() )
	{
		iter__->camera[sample__].neardb   = fnCamera.nearClippingPlane();
		iter__->camera[sample__].fardb    = fnCamera.farClippingPlane();
	}
	else
	{
		iter__->camera[sample__].neardb    = 0.001;    // TODO: these values are duplicated elsewhere in this file
		iter__->camera[sample__].fardb    = 250000.0; // TODO: these values are duplicated elsewhere in this file
	}

	iter__->camera[sample__].orthoWidth     = fnCamera.orthoWidth();
	iter__->camera[sample__].orthoHeight    = fnCamera.orthoWidth() * ((float)iter__->camera[sample__].height / (float)iter__->camera[sample__].width);

	iter__->camera[sample__].focalLength    = fnCamera.focalLength();
	iter__->camera[sample__].focalDistance  = fnCamera.focusDistance();
	iter__->camera[sample__].fStop          = fnCamera.fStop();
	iter__->camera[sample__].isOrtho		= fnCamera.isOrtho();
	iter__->camera[sample__].name           = fnCamera.fullPathName();
	getCameraFilmOffset( fnCamera, iter__->camera[sample__] );

	// convert focal length to scene units
	MDistance flenDist(iter__->camera[sample__].focalLength,MDistance::kMillimeters);
	iter__->camera[sample__].focalLength = flenDist.as(MDistance::uiUnit());
	getCameraTransform( fnCamera, iter__->camera[sample__] );
	//////////////////////////////////////////////////////////////////////////
	//[refactor 12.1] begin from liqRibTranslator::scanScene()
	// check stereo
	MString camType = fnCamera.typeName();
	bool isStereoCamera = false;
	if( camType == "stereoRigCamera" )
	{
		isStereoCamera = true;
		structCamera centralCameraPath = iter__->camera[sample__];
		// look for right and left cams
		MObject camTransform = fnCamera.parent(0, &status);
		if(status!=MS::kSuccess)
		{
			char errorMsg[512];
			sprintf(errorMsg, "Cannot find transform for camera %s.", fnCamera.name().asChar());
			//liquidMessage(errorMsg, messageError );
			printf(errorMsg);
			return MS::kFailure;
		}
		MFnDagNode fnCamTransform(camTransform, &status);
		if(status!=MS::kSuccess)
		{
			char errorMsg[512];
			sprintf(errorMsg, "Cannot init MFnDagNode for camera %s's transform.", fnCamera.name().asChar());
			//liquidMessage(errorMsg, messageError );
			printf(errorMsg);
			return MS::kFailure;
		}
		// get left one
		cPlug = fnCamTransform.findPlug( MString( "leftCam" ), &status );
		if(status!=MS::kSuccess)
		{
			char errorMsg[512];
			sprintf(errorMsg, "Cannot find plug 'leftCam' on %s", fnCamTransform.name().asChar());
			//liquidMessage(errorMsg, messageError );
			printf(errorMsg);
			return MS::kFailure;
		}
		MPlugArray plugArray;
		cPlug.connectedTo(plugArray, 1, 0, &status);
		if( plugArray.length() == 0 )
		{
			char errorMsg[512];
			sprintf(errorMsg, "Nothing connected in %s.leftCam \n", fnCamTransform.name().asChar());
			//liquidMessage(errorMsg, messageError );
			printf(errorMsg);
			return MS::kFailure;
		}
		MPlug leftCamPlug = plugArray[0];
		MObject leftCamTransformNode = leftCamPlug.node();
		MFnTransform fnLeftTrCam(leftCamTransformNode, &status);
		if( status != MS::kSuccess )
		{
			char errorMsg[512];
			sprintf(errorMsg, "cannot init MFnTransfrom for left camera '%s' ...\n", leftCamPlug.name().asChar());
			//liquidMessage(errorMsg, messageError );
			printf(errorMsg);
			return MS::kFailure;
		}
		MObject leftCamNode = fnLeftTrCam.child(0);
		MFnCamera fnLeftCam(leftCamNode, &status);
		if( status != MS::kSuccess )
		{
			char errorMsg[512];
			sprintf(errorMsg, "cannot init MFnCamera for left camera '%s' ...\n", fnLeftTrCam.name().asChar());
			//liquidMessage(errorMsg, messageError );
			printf(errorMsg);
			return MS::kFailure;
		}

		// get right one
		cPlug = fnCamTransform.findPlug( MString( "rightCam" ), &status );
		if(status!=MS::kSuccess)
		{
			char errorMsg[512];
			sprintf(errorMsg, "Cannot find plug 'rightCam' on %s", fnCamTransform.name().asChar());
			//liquidMessage(errorMsg, messageError );
			printf(errorMsg);
			return MS::kFailure;
		}
		cPlug.connectedTo(plugArray, 1, 0, &status);
		if( plugArray.length() == 0 )
		{
			char errorMsg[512];
			sprintf(errorMsg, "Nothing connected in %s.rightCam \n", fnCamTransform.name().asChar());
			//liquidMessage(errorMsg, messageError );
			printf(errorMsg);
			return MS::kFailure;
		}
		MPlug rightCamPlug = plugArray[0];
		MObject rightCamTransformNode = rightCamPlug.node();

		MFnTransform fnRightTrCam(rightCamTransformNode, &status);
		if( status != MS::kSuccess )
		{
			char errorMsg[512];
			sprintf(errorMsg, "cannot init MFnTransfrom for right camera '%s' ...\n", rightCamPlug.name().asChar());
			//liquidMessage(errorMsg, messageError );
			printf(errorMsg);
			return MS::kFailure;
		}
		MObject rightCamNode = fnRightTrCam.child(0);
		MFnCamera fnRightCam(rightCamNode, &status);
		if( status != MS::kSuccess )
		{
			char errorMsg[512];
			sprintf(errorMsg, "cannot init MFnCamera for right camera '%s' ...\n", fnRightTrCam.name().asChar());
			//liquidMessage(errorMsg, messageError );
			printf(errorMsg);
			return MS::kFailure;
		}
		/////////////////////////////
		getCameraInfo( fnLeftCam, iter__->leftCamera[sample__] );
		iter__->leftCamera[sample__].orthoWidth     = fnLeftCam.orthoWidth();
		iter__->leftCamera[sample__].orthoHeight    = fnLeftCam.orthoWidth() * ((float)iter__->camera[sample__].height / (float)iter__->camera[sample__].width);
		iter__->leftCamera[sample__].focalLength    = fnLeftCam.focalLength();
		iter__->leftCamera[sample__].focalDistance  = fnLeftCam.focusDistance();
		iter__->leftCamera[sample__].fStop          = fnLeftCam.fStop();
		iter__->leftCamera[sample__].isOrtho		= fnLeftCam.isOrtho();
		iter__->leftCamera[sample__].name			= fnLeftCam.name();
		getCameraFilmOffset( fnLeftCam, iter__->leftCamera[sample__] );
		// convert focal length to scene units
		MDistance flenLDist(iter__->leftCamera[sample__].focalLength, MDistance::kMillimeters);
		iter__->leftCamera[sample__].focalLength = flenLDist.as(MDistance::uiUnit());
		getCameraTransform( fnLeftCam, iter__->leftCamera[sample__] );
		// scanScene: The camera's fov may not match the rendered image in Maya
		// if a film-fit is used. 'fov_ratio' is used to account for
		// this.
		//
		//iter->leftCamera[sample].hFOV = fnLeftCam.horizontalFieldOfView()/iter->leftCamera[sample].fov_ratio;
		iter__->leftCamera[sample__].hFOV   = iter__->camera[sample__].hFOV;
		iter__->leftCamera[sample__].neardb = iter__->camera[sample__].neardb;
		iter__->leftCamera[sample__].fardb  = iter__->camera[sample__].fardb;

		getCameraInfo( fnRightCam, iter__->rightCamera[sample__] );
		iter__->rightCamera[sample__].orthoWidth	= fnRightCam.orthoWidth();
		iter__->rightCamera[sample__].orthoHeight	= fnRightCam.orthoWidth() * ((float)iter__->camera[sample__].height / (float)iter__->camera[sample__].width);
		iter__->rightCamera[sample__].focalLength	= fnRightCam.focalLength();
		iter__->rightCamera[sample__].focalDistance	= fnRightCam.focusDistance();
		iter__->rightCamera[sample__].fStop			= fnRightCam.fStop();
		iter__->rightCamera[sample__].isOrtho		= fnRightCam.isOrtho();
		iter__->rightCamera[sample__].name			= fnRightCam.name();
		getCameraFilmOffset( fnRightCam, iter__->rightCamera[sample__] );
		// convert focal length to scene units
		MDistance flenRDist(iter__->rightCamera[sample__].focalLength, MDistance::kMillimeters);
		iter__->rightCamera[sample__].focalLength = flenRDist.as(MDistance::uiUnit());
		getCameraTransform( fnRightCam, iter__->rightCamera[sample__] );
		// scanScene: The camera's fov may not match the rendered image in Maya
		// if a film-fit is used. 'fov_ratio' is used to account for
		// this.
		//
		//iter->rightCamera[sample].hFOV = fnRightCam.horizontalFieldOfView()/iter->rightCamera[sample].fov_ratio;
		iter__->rightCamera[sample__].hFOV   = iter__->camera[sample__].hFOV;
		iter__->rightCamera[sample__].neardb = iter__->camera[sample__].neardb;
		iter__->rightCamera[sample__].fardb  = iter__->camera[sample__].fardb;

		iter__->camera[sample__].rightCam = &(iter__->rightCamera[sample__]);
		iter__->camera[sample__].leftCam  = &(iter__->leftCamera[sample__]);
	}//if( camType == "stereoRigCamera" ) 
	iter__->isStereoPass = isStereoCamera;
	iter__->aspectRatio  = liqglo.aspectRatio;
	//[refactor 12.1] end 
	//////////////////////////////////////////////////////////////////////////

	// scanScene: Determine what information to write out (RGB, alpha, zbuffer)
	//
	iter__->imageMode.clear();

	bool isOn;
	MPlug boolPlug;
	boolPlug = fnCamera.findPlug( "image" );

	boolPlug.getValue( isOn );
	if( isOn ) 
	{
		// We are writing RGB info
		//
		iter__->imageMode = "rgb";
		iter__->format = liqglo.outFormat;
	}
	boolPlug = fnCamera.findPlug( "mask" );
	boolPlug.getValue( isOn );
	if( isOn ) 
	{
		// We are writing alpha channel info
		//
		iter__->imageMode += "a";
		iter__->format = liqglo.outFormat;
	}
	boolPlug = fnCamera.findPlug( "depth" );
	boolPlug.getValue( isOn );
	if( isOn ) 
	{
		if ( !isStereoCamera  )
		{
			// We are writing z-buffer info
			//
			iter__->imageMode = "z";
			iter__->format = "zfile";
		}else
			liquidMessage( "Cannot render depth for stereo camera.", messageWarning );
	}// isOn && !isStereoCamera
	//[refactor 12] end
	return MS::kSuccess;
}
//
MStatus tCameraMgr::getCameraData( structJob &job__ , const int sample__)
{
	CM_TRACE_FUNC("tCameraMgr::getCameraData("<<job__.name.asChar()<<","<<sample__<<")");
	MStatus status;
	//[refactor 12] begin from  liqRibTranslator::scanScene()
	MDagPath path;
	MFnCamera   fnCamera( job__.path );
	job__.gotJobOptions = false;
	status.clear();
	MPlug cPlug = fnCamera.findPlug( MString( "ribOptions" ), &status );
	if( status == MS::kSuccess ) 
	{
		cPlug.getValue( job__.jobOptions );
		job__.gotJobOptions = true;
	}
	getCameraInfo( fnCamera, job__.camera[sample__] );
	job__.width = job__.camera[sample__].width;
	job__.height = job__.camera[sample__].height;
	// scanScene: Renderman specifies shutter by time open
	// so we need to convert shutterAngle to time.
	// To do this convert shutterAngle to degrees and
	// divide by 360.
	//
	job__.camera[sample__].shutter = fnCamera.shutterAngle() * 0.5 / M_PI;
	liqglo.liqglo_shutterTime = job__.camera[sample__].shutter;
	job__.camera[sample__].motionBlur     = fnCamera.isMotionBlur();				

	// scanScene: The camera's fov may not match the rendered image in Maya
	// if a film-fit is used. 'fov_ratio' is used to account for
	// this.
	//
	job__.camera[sample__].hFOV = fnCamera.horizontalFieldOfView()/job__.camera[sample__].fov_ratio;

	if ( fnCamera.isClippingPlanes() )
	{
		job__.camera[sample__].neardb   = fnCamera.nearClippingPlane();
		job__.camera[sample__].fardb    = fnCamera.farClippingPlane();
	}
	else
	{
		job__.camera[sample__].neardb    = 0.001;    // TODO: these values are duplicated elsewhere in this file
		job__.camera[sample__].fardb    = 250000.0; // TODO: these values are duplicated elsewhere in this file
	}

	job__.camera[sample__].orthoWidth     = fnCamera.orthoWidth();
	job__.camera[sample__].orthoHeight    = fnCamera.orthoWidth() * ((float)job__.camera[sample__].height / (float)job__.camera[sample__].width);

	job__.camera[sample__].focalLength    = fnCamera.focalLength();
	job__.camera[sample__].focalDistance  = fnCamera.focusDistance();
	job__.camera[sample__].fStop          = fnCamera.fStop();
	job__.camera[sample__].isOrtho		= fnCamera.isOrtho();
	job__.camera[sample__].name           = fnCamera.fullPathName();
	getCameraFilmOffset( fnCamera, job__.camera[sample__] );

	// convert focal length to scene units
	MDistance flenDist(job__.camera[sample__].focalLength,MDistance::kMillimeters);
	job__.camera[sample__].focalLength = flenDist.as(MDistance::uiUnit());
	getCameraTransform( fnCamera, job__.camera[sample__] );
	//////////////////////////////////////////////////////////////////////////
	//[refactor 12.1] begin from liqRibTranslator::scanScene()
	// check stereo
	MString camType = fnCamera.typeName();
	bool isStereoCamera = false;
	if( camType == "stereoRigCamera" )
	{
		isStereoCamera = true;
		structCamera centralCameraPath = job__.camera[sample__];
		// look for right and left cams
		MObject camTransform = fnCamera.parent(0, &status);
		if(status!=MS::kSuccess)
		{
			char errorMsg[512];
			sprintf(errorMsg, "Cannot find transform for camera %s.", fnCamera.name().asChar());
			//liquidMessage(errorMsg, messageError );
			printf(errorMsg);
			return MS::kFailure;
		}
		MFnDagNode fnCamTransform(camTransform, &status);
		if(status!=MS::kSuccess)
		{
			char errorMsg[512];
			sprintf(errorMsg, "Cannot init MFnDagNode for camera %s's transform.", fnCamera.name().asChar());
			//liquidMessage(errorMsg, messageError );
			printf(errorMsg);
			return MS::kFailure;
		}
		// get left one
		cPlug = fnCamTransform.findPlug( MString( "leftCam" ), &status );
		if(status!=MS::kSuccess)
		{
			char errorMsg[512];
			sprintf(errorMsg, "Cannot find plug 'leftCam' on %s", fnCamTransform.name().asChar());
			//liquidMessage(errorMsg, messageError );
			printf(errorMsg);
			return MS::kFailure;
		}
		MPlugArray plugArray;
		cPlug.connectedTo(plugArray, 1, 0, &status);
		if( plugArray.length() == 0 )
		{
			char errorMsg[512];
			sprintf(errorMsg, "Nothing connected in %s.leftCam \n", fnCamTransform.name().asChar());
			//liquidMessage(errorMsg, messageError );
			printf(errorMsg);
			return MS::kFailure;
		}
		MPlug leftCamPlug = plugArray[0];
		MObject leftCamTransformNode = leftCamPlug.node();
		MFnTransform fnLeftTrCam(leftCamTransformNode, &status);
		if( status != MS::kSuccess )
		{
			char errorMsg[512];
			sprintf(errorMsg, "cannot init MFnTransfrom for left camera '%s' ...\n", leftCamPlug.name().asChar());
			//liquidMessage(errorMsg, messageError );
			printf(errorMsg);
			return MS::kFailure;
		}
		MObject leftCamNode = fnLeftTrCam.child(0);
		MFnCamera fnLeftCam(leftCamNode, &status);
		if( status != MS::kSuccess )
		{
			char errorMsg[512];
			sprintf(errorMsg, "cannot init MFnCamera for left camera '%s' ...\n", fnLeftTrCam.name().asChar());
			//liquidMessage(errorMsg, messageError );
			printf(errorMsg);
			return MS::kFailure;
		}

		// get right one
		cPlug = fnCamTransform.findPlug( MString( "rightCam" ), &status );
		if(status!=MS::kSuccess)
		{
			char errorMsg[512];
			sprintf(errorMsg, "Cannot find plug 'rightCam' on %s", fnCamTransform.name().asChar());
			//liquidMessage(errorMsg, messageError );
			printf(errorMsg);
			return MS::kFailure;
		}
		cPlug.connectedTo(plugArray, 1, 0, &status);
		if( plugArray.length() == 0 )
		{
			char errorMsg[512];
			sprintf(errorMsg, "Nothing connected in %s.rightCam \n", fnCamTransform.name().asChar());
			//liquidMessage(errorMsg, messageError );
			printf(errorMsg);
			return MS::kFailure;
		}
		MPlug rightCamPlug = plugArray[0];
		MObject rightCamTransformNode = rightCamPlug.node();

		MFnTransform fnRightTrCam(rightCamTransformNode, &status);
		if( status != MS::kSuccess )
		{
			char errorMsg[512];
			sprintf(errorMsg, "cannot init MFnTransfrom for right camera '%s' ...\n", rightCamPlug.name().asChar());
			//liquidMessage(errorMsg, messageError );
			printf(errorMsg);
			return MS::kFailure;
		}
		MObject rightCamNode = fnRightTrCam.child(0);
		MFnCamera fnRightCam(rightCamNode, &status);
		if( status != MS::kSuccess )
		{
			char errorMsg[512];
			sprintf(errorMsg, "cannot init MFnCamera for right camera '%s' ...\n", fnRightTrCam.name().asChar());
			//liquidMessage(errorMsg, messageError );
			printf(errorMsg);
			return MS::kFailure;
		}
		/////////////////////////////
		//left camera data
		getCameraInfo( fnLeftCam, job__.leftCamera[sample__] );
		job__.leftCamera[sample__].orthoWidth     = fnLeftCam.orthoWidth();
		job__.leftCamera[sample__].orthoHeight    = fnLeftCam.orthoWidth() * ((float)job__.camera[sample__].height / (float)job__.camera[sample__].width);
		job__.leftCamera[sample__].focalLength    = fnLeftCam.focalLength();
		job__.leftCamera[sample__].focalDistance  = fnLeftCam.focusDistance();
		job__.leftCamera[sample__].fStop          = fnLeftCam.fStop();
		job__.leftCamera[sample__].isOrtho		= fnLeftCam.isOrtho();
		job__.leftCamera[sample__].name			= fnLeftCam.name();
		getCameraFilmOffset( fnLeftCam, job__.leftCamera[sample__] );
		// convert focal length to scene units
		MDistance flenLDist(job__.leftCamera[sample__].focalLength, MDistance::kMillimeters);
		job__.leftCamera[sample__].focalLength = flenLDist.as(MDistance::uiUnit());
		getCameraTransform( fnLeftCam, job__.leftCamera[sample__] );
		// scanScene: The camera's fov may not match the rendered image in Maya
		// if a film-fit is used. 'fov_ratio' is used to account for
		// this.
		//
		//iter->leftCamera[sample].hFOV = fnLeftCam.horizontalFieldOfView()/iter->leftCamera[sample].fov_ratio;
		job__.leftCamera[sample__].hFOV   = job__.camera[sample__].hFOV;
		job__.leftCamera[sample__].neardb = job__.camera[sample__].neardb;
		job__.leftCamera[sample__].fardb  = job__.camera[sample__].fardb;
		
		//right camera data
		getCameraInfo( fnRightCam, job__.rightCamera[sample__] );
		job__.rightCamera[sample__].orthoWidth	= fnRightCam.orthoWidth();
		job__.rightCamera[sample__].orthoHeight	= fnRightCam.orthoWidth() * ((float)job__.camera[sample__].height / (float)job__.camera[sample__].width);
		job__.rightCamera[sample__].focalLength	= fnRightCam.focalLength();
		job__.rightCamera[sample__].focalDistance	= fnRightCam.focusDistance();
		job__.rightCamera[sample__].fStop			= fnRightCam.fStop();
		job__.rightCamera[sample__].isOrtho		= fnRightCam.isOrtho();
		job__.rightCamera[sample__].name			= fnRightCam.name();
		getCameraFilmOffset( fnRightCam, job__.rightCamera[sample__] );
		// convert focal length to scene units
		MDistance flenRDist(job__.rightCamera[sample__].focalLength, MDistance::kMillimeters);
		job__.rightCamera[sample__].focalLength = flenRDist.as(MDistance::uiUnit());
		getCameraTransform( fnRightCam, job__.rightCamera[sample__] );
		// scanScene: The camera's fov may not match the rendered image in Maya
		// if a film-fit is used. 'fov_ratio' is used to account for
		// this.
		//
		//iter->rightCamera[sample].hFOV = fnRightCam.horizontalFieldOfView()/iter->rightCamera[sample].fov_ratio;
		job__.rightCamera[sample__].hFOV   = job__.camera[sample__].hFOV;
		job__.rightCamera[sample__].neardb = job__.camera[sample__].neardb;
		job__.rightCamera[sample__].fardb  = job__.camera[sample__].fardb;

		job__.camera[sample__].rightCam = &(job__.rightCamera[sample__]);
		job__.camera[sample__].leftCam  = &(job__.leftCamera[sample__]);
	}//if( camType == "stereoRigCamera" ) 
	job__.isStereoPass = isStereoCamera;
	job__.aspectRatio  = liqglo.aspectRatio;
	//[refactor 12.1] end 
	//////////////////////////////////////////////////////////////////////////

	// scanScene: Determine what information to write out (RGB, alpha, zbuffer)
	//
	job__.imageMode.clear();

	bool isOn;
	MPlug boolPlug;
	boolPlug = fnCamera.findPlug( "image" );

	boolPlug.getValue( isOn );
	if( isOn ) 
	{
		// We are writing RGB info
		//
		job__.imageMode = "rgb";
		job__.format = liqglo.outFormat;
	}
	boolPlug = fnCamera.findPlug( "mask" );
	boolPlug.getValue( isOn );
	if( isOn ) 
	{
		// We are writing alpha channel info
		//
		job__.imageMode += "a";
		job__.format = liqglo.outFormat;
	}
	boolPlug = fnCamera.findPlug( "depth" );
	boolPlug.getValue( isOn );
	if( isOn ) 
	{
		if ( !isStereoCamera  )
		{
			// We are writing z-buffer info
			//
			job__.imageMode = "z";
			job__.format = "zfile";
		}else
			liquidMessage( "Cannot render depth for stereo camera.", messageWarning );
	}// isOn && !isStereoCamera
	//[refactor 12] end
	return MS::kSuccess;
}
