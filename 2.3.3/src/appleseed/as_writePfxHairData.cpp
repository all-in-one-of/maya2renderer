#include <liqConfig.h>
#ifdef _USE_APPLESEED_

#include "as_renderer.h"
#include <liqRibPfxHairData.h>
#include "../common/prerequest_maya.h"
#include "../common/mayacheck.h"
#include "log_helper.h"
//#include <liqRibNode.h>
#include <liqRibTranslator.h>
#include <liqGlobalHelpers.h>

namespace appleseed
{
	void Renderer::write(
		/*const*/ liqRibPfxHairData* pData,
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
 			_write_pfxhair(pData, currentJob);
// 			o.RiEndRef();

		}else{
			//write the reference
			assert(pData->getRibFileFullPath() == fileName);
//			RiReadArchive( const_cast< RtToken >( pData->getRibFileFullPath().asChar() ), NULL, RI_NULL );
		}
	}
	//
	void Renderer::_write_pfxhair(liqRibPfxHairData* pData, const structJob &currentJob__)
	{
		CM_TRACE_FUNC("_write_pfxhair("<<pData->getFullPathName()<<","<<currentJob__.name.asChar()<<")");
		//
		if( pData->isEmpty() ) 
		{
			liquidMessage2(messageWarning, "pfxHair is empty: %s",  pData->getFullPathName());
			return;
		}

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


//		//
		MStatus status;

		int degree;
		MFnDagNode fnDN(data->objDagPath);
		IfMErrorWarn( liquidGetPlugValue(fnDN, "degree", degree, status) );
//
//		MIntArray triangleCounts,triangleVertices;
//		IfMErrorWarn(fnMesh.getTriangles(triangleCounts, triangleVertices));
//
//		MString currentUVsetName;
//		IfMErrorWarn(fnMesh.getCurrentUVSetName(currentUVsetName));
//
		// geometry data (shape)
		_s("\n//############################### mesh #");
		_s("//shape full path name="<<data->getFullPathName());
#ifdef TRANSFORM_SHAPE_PAIR
		const std::string objectName(ribNode__->name.asChar());//shape
#else// SHAPE SHAPE_object PAIR
		const std::string objectName(getObjectName(ribNode__->name.asChar()));//shape+"_object"
#endif

		//todo
	}
	//
	static int getSegment(const int degree) 
	{
		int ret = 0;
		switch(degree)
		{
		case 1: case 2: case 3: 
			ret = degree; break;
		default:
			liquidMessage2(messageError, "invalid pfxhair degree:%d", degree);
		}
		return ret;
	}
	void Renderer::generate_pfxhair(liqRibNodePtr &ribNode__, liqRibPfxHairData* phair, const int degree)
	{
		CM_TRACE_FUNC("generate_pfxhair("<<ribNode__->getTransformNodeFullPath().asChar()<<")");

		MStatus status;
		MFnPfxGeometry pfxhair(phair->objDagPath, &status);
		IfMErrorWarn(status);



		MRenderLineArray profileArray;
		MRenderLineArray creaseArray;
		MRenderLineArray intersectionArray;
		MRenderLineArray copy;

		bool doLines          = true;
		bool doTwist          = true;
		bool doWidth          = true;
		bool doFlatness       = false;
		bool doParameter      = false;
		bool doColor          = true;
		bool doIncandescence  = false;
		bool doTransparency   = true;
		bool doWorldSpace     = false;

		IfMErrorWarn( pfxhair.getLineData( profileArray, creaseArray, intersectionArray, doLines, doTwist, doWidth, doFlatness, doParameter, doColor, doIncandescence, doTransparency, doWorldSpace) );

		//todo

	}
}//namespace appleseed

#endif//_USE_APPLESEED_