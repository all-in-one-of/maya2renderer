#include <liqConfig.h>
#ifdef _USE_APPLESEED_
#ifdef _USE_SHAVE_
#include "as_renderer.h"
#include <liqRibPfxData.h>
#include "../common/prerequest_maya.h"
#include "../common/mayacheck.h"
#include "log_helper.h"
//#include <liqRibNode.h>
#include <liqRibTranslator.h>
#include <liqGlobalHelpers.h>
#include <liqRibShaveData.h>
#include<maya/shaveAPI.h>
#include<maya/shaveItHair.h>

namespace appleseed
{
	void Renderer::write(
		/*const*/ liqRibShaveData* pData,
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
 			_write_shavehair(pData, currentJob);
// 			o.RiEndRef();

		}else{
			//write the reference
			assert(pData->getRibFileFullPath() == fileName);
//			RiReadArchive( const_cast< RtToken >( pData->getRibFileFullPath().asChar() ), NULL, RI_NULL );
		}
	}
	//
	void Renderer::_write_shavehair(liqRibShaveData* pData, const structJob &currentJob__)
	{
		CM_TRACE_FUNC("_write_shavehair("<<pData->getFullPathName()<<","<<currentJob__.name.asChar()<<")");
		//

		//
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
			( !currentJob__.isShadow || currentJob__.deepShadows );

		bool bGeometryMotion = 
			liqglo.liqglo_doDef 
			&& bMotionBlur
			&& ( ribNode__->object(0)->type != MRT_RibGen );

		unsigned int sample_first = 0;
		unsigned int sample_last = bGeometryMotion? (liqglo.liqglo_motionSamples - 1):sample_first;

		_s("\n// Renderer::exportOneGeometry_Mesh("<<ribNode__->name.asChar()<<","<<sample_first<<","<<sample_last<<")");

		const liqRibDataPtr data = ribNode__->object(sample_first)->getDataPtr();

//
		MStatus status;


		MFnDagNode fnDN(data->objDagPath, &status);
		IfErrorWarn(status);

		// geometry data (shape)
		_s("\n//############################### shave hair #");
		_s("//shape full path name="<<data->getFullPathName());
#ifdef TRANSFORM_SHAPE_PAIR
		const std::string objectName(ribNode__->name.asChar());//shape
#else// SHAPE SHAPE_object PAIR
		const std::string objectName(getObjectName(ribNode__->name.asChar()));//shape+"_object"
#endif
		//todo
	}
	//
	void Renderer::generate_shavehair(liqRibNodePtr &ribNode__, liqRibShaveData* pData, const int degree)
	{
		CM_TRACE_FUNC("generate_pfxhair("<<ribNode__->getTransformNodeFullPath().asChar()<<")");

		MStatus status;

		shaveAPI::HairInfo hairInfo;
		status = shaveAPI::exportAllHair(&hairInfo);
		if(MFAIL(status)){
			liquidMessage2(messageError,"shaveAPI::exportAllHair(&hairInfo)=%s", status.errorString().asChar());
			return;
		}

		//todo
	}

}//namespace appleseed
#endif//_USE_SHAVE_
#endif//_USE_APPLESEED_
