#include "nr_renderer.h"
#include <liqRibMeshData.h>
#include "../common/prerequest_maya.h"
#include "../common/mayacheck.h"
#include "log_helper.h"
//#include <liqRibNode.h>
#include <liqRibTranslator.h>

namespace newrenderer
{
	static void _write(liqRibMeshData* pData, const structJob &currentJob);

	void Renderer::write(
		/*const*/ liqRibMeshData* pData,
		const MString &fileName, 
		const structJob &currentJob,
		const bool bReference)
	{
		CM_TRACE_FUNC("Renderer::write("<<pData->getFullPathName()<<","<<fileName<<","<<currentJob.name<<",ref="<<bReference<<")");

		if( !bReference ){//write data at first time
			assert(pData->getRibFileFullPath().length()==0);
			pData->setRibFileFullPath(fileName);

// 			renderman::Helper o;
// 			o.RiBeginRef(pData->getRibFileFullPath().asChar());
			_write(pData, currentJob);
// 			o.RiEndRef();

		}else{
			//write the reference
			assert(pData->getRibFileFullPath() == fileName);
//			RiReadArchive( const_cast< RtToken >( pData->getRibFileFullPath().asChar() ), NULL, RI_NULL );
		}
	}
	//
	void _exportVertexFromNodePlug(
		const liqRibNodePtr &ribNode__,
		unsigned int sample)
	{	
		CM_TRACE_FUNC("_exportVertexFromNodePlug("<<ribNode__->name<<","<<sample<<")");

		MStatus status;

		const liqRibDataPtr ribdata = ribNode__->object(sample)->getDataPtr();
		liqRibMeshData* mesh = (liqRibMeshData*)(ribdata.get());
		const std::vector<liqTokenPointer>& tokenPointerArray = mesh->tokenPointerArray;

		liqTokenPointer vertex;
		for( std::vector< liqTokenPointer >::const_iterator iter( tokenPointerArray.begin() ); iter != tokenPointerArray.end(); ++iter ) 
		{
			if( "P" == const_cast< liqTokenPointer* >( &( *iter ) )->getDetailedTokenName() )// find the Position data
			{
				vertex = *iter;
				break;
			}
		}
		assert( !vertex.empty() );
		const RtFloat* vertex_buf = vertex.getTokenFloatArray();

		MFnMesh fnMesh(mesh->objDagPath, &status);
		IfMErrorWarn(status);

		// add vertex position
		for(size_t i=0; i<fnMesh.numVertices(); ++i)
		{
			//_S( ei_tab_add_vector( vertex_buf[3*i+0],vertex_buf[3*i+1],vertex_buf[3*i+2] ) );
		}

	}
	//
	static void _write(liqRibMeshData* pData, const structJob &currentJob__)
	{
		CM_TRACE_FUNC("_write("<<pData->getFullPathName()<<","<<currentJob__.name<<")");

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

		const liqRibDataPtr mesh = ribNode__->object(sample_first)->getDataPtr();

//


		//
		MStatus status;
		MFnMesh fnMesh(mesh->objDagPath, &status);
		IfMErrorWarn(status);

		MIntArray triangleCounts,triangleVertices;
		IfMErrorWarn(fnMesh.getTriangles(triangleCounts, triangleVertices));

		MString currentUVsetName;
		IfMErrorWarn(fnMesh.getCurrentUVSetName(currentUVsetName));

		// geometry data (shape)
		_s("\n//############################### mesh #");
		_s("//shape full path name="<<mesh->getFullPathName());
#ifdef TRANSFORM_SHAPE_PAIR
		const std::string objectName(ribNode__->name.asChar());//shape
#else// SHAPE SHAPE_object PAIR
		const std::string objectName(getObjectName(ribNode__->name.asChar()));//shape+"_object"
#endif

		//todo
	}
}//namespace elvishray
