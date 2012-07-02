#include <liqConfig.h>
#ifdef _USE_APPLESEED_

#include "as_renderer.h"
#include <liqRibMeshData.h>
#include "../common/prerequest_maya.h"
#include "../common/mayacheck.h"
#include "log_helper.h"
//#include <liqRibNode.h>
#include <liqRibTranslator.h>

namespace appleseed
{
	static void _write(liqRibMeshData* pData, const structJob &currentJob__);

	void Renderer::write(
		/*const*/ liqRibMeshData* pData,
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
			_write(pData, currentJob);
// 			o.RiEndRef();

		}else{
			//write the reference
			assert(pData->getRibFileFullPath() == fileName);
			_writeRef(pData, currentJob);
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
		CM_TRACE_FUNC("_write("<<pData->getFullPathName()<<","<<currentJob__.name.asChar()<<")");

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

		// AS stuff
		IfMErrorWarn(MGlobal::executeCommand( "as_export(\""+MString(mesh->getFullPathName())+"\")"));



		//todo
	}
	//
	void Renderer::_writeRef(liqRibMeshData* pData, const structJob &currentJob__)
	{
		CM_TRACE_FUNC("Renderer::_writeRef("<<pData->getFullPathName()<<","<<currentJob__.name.asChar()<<")");
		
		MString dirname;
		IfMErrorWarn(MGlobal::executeCommand( "as_get_mesh_dirname(\""+MString(pData->getFullPathName())+"\")", dirname));
		MString basename;
		IfMErrorWarn(MGlobal::executeCommand( "as_get_mesh_basename(\""+MString(pData->getFullPathName())+"\")", basename));

		asf::SearchPaths search_paths;
		search_paths.push_back( dirname.asChar() );

		asr::MeshObjectArray objects =
			asr::MeshObjectReader::read(
			search_paths,
			pData->getFullPathName(),
			asr::ParamArray()
			.insert( "filename", basename.asChar() )
			);
		//
		// Insert all the objects into the assembly.
		for (size_t i = 0; i < objects.size(); ++i)
		{
			// Insert this object into the scene.
			asr::MeshObject* object = objects[i];
			current_assembly->objects().insert(asf::auto_release_ptr<asr::Object>(object));

			// Create the array of material names.
			asf::StringArray material_names;
			material_names.push_back("gray_material");

			// Create an instance of this object and insert it into the assembly.
			const std::string instance_name = std::string(object->get_name()) + "_inst";
			current_assembly->object_instances().insert(
				asr::ObjectInstanceFactory::create(
				instance_name.c_str(),
				asr::ParamArray(),
				*object,
				asf::Transformd(asf::Matrix4d::identity()),
				material_names
				)
			);
		}
	}

}//namespace appleseed

#endif//_USE_APPLESEED_