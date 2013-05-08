#include <liqConfig.h>
#ifdef _USE_ELVISHRAY_

#include "er_renderer.h"
#include <liqRibCameraData.h>
#include "../common/prerequest_maya.h"
#include "../common/mayacheck.h"
#include "ercall.h"

//#include <liqRibNode.h>
#include <liqRibTranslator.h>
namespace elvishray
{
	void Renderer::write(
		/*const*/ liqRibCameraData* pData,
		const MString &fileName, 
		const structJob &currentJob,
		const bool bReference)
	{
		CM_TRACE_FUNC("Renderer::write("<<pData->getFullPathName()<<","<<fileName.asChar()<<","<<currentJob.name.asChar()<<",ref="<<bReference<<")");

		if( !bReference ){//write data at first time
			assert(pData->getRibFileFullPath().length()==0);
			pData->setRibFileFullPath(fileName);

// 			renderman::Helper o;
// 			o.RiBeginRef(pData->getRibFileFullPath().asChar());
			_write_camera(pData, currentJob);
// 			o.RiEndRef();

		}else{
			//write the reference
			assert(pData->getRibFileFullPath() == fileName);
//			RiReadArchive( const_cast< RtToken >( pData->getRibFileFullPath().asChar() ), NULL, RI_NULL );
		}
	}
	//
	void Renderer::_write_camera(liqRibCameraData* pData, const structJob &currentJob)
	{
		CM_TRACE_FUNC("er_writeCameraData.cpp::_write_camera("<<pData->getFullPathName()<<","<<currentJob.name.asChar()<<")");
		OutputMgr &o = Renderer::o;

		liqRibNodePtr ribNode__ = liqRibTranslator::getInstancePtr()->htable->find(
			pData->objDagPath.fullPathName(), 
			pData->objDagPath,
			MRT_Unknown
			);
		assert( ribNode__!=0 );
		assert( ribNode__->path().fullPathName() == pData->objDagPath.fullPathName() );

		const bool bMotionBlur =
			ribNode__->motion.transformationBlur &&
			( ribNode__->object( 1 ) ) &&
			//( ribNode__->object(0)->type != MRT_Locator ) && // Why the fuck do we not allow motion blur for locators?
			( currentJob.pass != rpShadowMap || currentJob.shadowType == stDeep );

		bool bGeometryMotion = 
			liqglo.liqglo_doDef 
			&& bMotionBlur
			&& ( ribNode__->object(0)->type != MRT_RibGen );

		unsigned int sample_first = 0;
		unsigned int sample_last = bGeometryMotion? (liqglo.liqglo_motionSamples - 1):sample_first;

		o.ln();
		
		const liqRibDataPtr camera = ribNode__->object(sample_first)->getDataPtr();

		//


		//
		MStatus status;
		MFnCamera fnCamera(camera->objDagPath, &status);
		IfMErrorMsgWarn(status, ribNode__->name);

		//
		float focal;
		float aperture;
		float aspect;
		float width=currentJob.width, height=currentJob.height;


// 		// use liquid settings
// 		focal = currentJob.camera[0].focalLength;
// 		aperture = 2.0 * focal * tan(currentJob.camera[0].hFOV/2.0);
// 		aspect = currentJob.aspectRatio;
// 		_s("// liquid settings: focal="<<focal <<", aperture = "<<aperture<<", aspect="<<aspect );
// 		//e.g. focal=3.5, aperture = 2.7, aspect=1
		// use maya settings
		//NOTE: we will caculate these setting from structJob data.[yaoyansi]
		focal = fnCamera.focalLength(&status);
		IfMErrorWarn(status);
		double horizontalFOV,verticalFOV;
		fnCamera.getPortFieldOfView(width, height, horizontalFOV,verticalFOV);
		aperture = 2.0f * focal * tan(horizontalFOV /2.0f);
		aspect = aperture / (2.0f * focal * tan(verticalFOV / 2.0f));
		o.a(boost::str(boost::format(" maya settings: focal=%f, aperture = %f, aspect=%f")%focal %aperture %aspect ));
		//e.g. focal=35, aperture = 36, aspect=1.33333.

		//get camera transform matrix
		//currentJob.camera[0].mat.get( m ) );
		MObject transformNode = currentJob.path.transform(&status );
		IfMErrorWarn(status);
		MFnDagNode transform (transformNode, &status);
		IfMErrorWarn(status);
		MTransformationMatrix   m0(transform.transformationMatrix());
		liqMatrix m;		
		IfMErrorWarn(m0.asMatrix().get(m));

		bool bDepthOfField;//enable DOF on this camera?
		liquidGetPlugValue(fnCamera,"depthOfField", bDepthOfField, status);
		o.a(boost::str(boost::format("Depth of Field on camera \"%s\" is turned %s in Maya") %currentJob.camera[0].name.asChar() %(bDepthOfField?"on":"off")));
		bDepthOfField = bDepthOfField && liqglo.doDof && currentJob.pass != rpShadowMap;

		MStringArray LensShaders, EnvironmentShaders;
		{
			//lens shader
			if(bDepthOfField)
			{
				gatherCameraShaders(LensShaders, currentJob.camera[0].name, "liqLensShader");

			}
			//env shader
			gatherCameraShaders(EnvironmentShaders, currentJob.camera[0].name, "liqEnvironmentShader");
		}

		o.ln();
		o.a("############################### camera #");
#ifdef TRANSFORM_SHAPE_PAIR
		const std::string objectName(ribNode__->name.asChar());//shape
#else// SHAPE SHAPE_object PAIR
		const std::string objectName(getObjectName(ribNode__->name.asChar()));//shape+"_object"
#endif

		o.ei_camera( objectName.c_str() );
		//_S( ei_frame( lframe, off ) );

		cameraOutput(currentJob);

		o.ei_focal( focal );
		o.ei_aperture( aperture / fnCamera.overscan() );
		o.ei_aspect( aspect );
		if( currentJob.pass != rpShadowMap && liqglo.liqglo_rotateCamera  == true ) {
			o.ei_resolution(height, width);
		}else{ 
			o.ei_resolution(width, height);
		}

		if( liqglo.m_renderViewCrop )
		{
			unsigned int left, right, bottom, top;
			MRenderView::getRenderRegion(left, right, bottom, top);
			o.ei_window(left, right+1, height-top, height-bottom+1);
		} 

		o.ei_clip( currentJob.camera[0].neardb, currentJob.camera[0].fardb);

		//lens shader
		for( std::size_t i = 0; i<LensShaders.length(); ++i)
		{
			o.ei_lens_shader( LensShaders[i].asChar() );
		}
		//env shader
		for( std::size_t i = 0; i<EnvironmentShaders.length(); ++i)
		{
			o.ei_environment_shader(EnvironmentShaders[i].asChar());
		}
		o.ei_end_camera();

	}
}//namespace elvishray

#endif//_USE_ELVISHRAY_