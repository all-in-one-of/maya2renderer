#include "as_renderer.h"
#include <liqRibMeshData.h>
#include "../common/prerequest_maya.h"
#include "../common/mayacheck.h"
#include "as_log_helper.h"
//#include <liqRibNode.h>
#include <liqRibTranslator.h>
#include "objExport.h"
#include "as_GlobalNodeHelper.h"

namespace appleseed
{
	//
	void _exportVertexFromNodePlug(	const liqRibNodePtr &ribNode__,	
		unsigned int sample,std::vector<asf::Vector3d>& v);
	static void _write(liqRibMeshData* pData, const structJob &currentJob__, GlobalNodeHelper* m_gnode);
	/////////////////////// mesh stuffs for outpu /////////////////////////////
	struct Mesh
	{
		//std::string              m_name;
		std::vector<asf::Vector3d>			m_vertices;
		std::vector<asf::IMeshWalker::Face> m_faces;
	};
	//
	struct MeshWalker : public asf::IMeshWalker
	{
		MFnMesh fnMesh;

		Mesh m_mesh;
		const liqRibNodePtr &ribNode__;
		
		MIntArray triangleCounts,triangleVertices;
		MString currentUVsetName;

		explicit MeshWalker(const liqRibNodePtr &ribNode)
			: ribNode__(ribNode)
		{
		}

		virtual std::string get_name() const
		{
			return ribNode__->name.asChar();
		}

		virtual std::size_t get_vertex_count() const
		{
			MStatus status;
			int num = fnMesh.numVertices(&status);
			IfMErrorWarn(status);
			return num;
		}

		virtual asf::Vector3d get_vertex(const std::size_t i) const
		{
			return m_mesh.m_vertices[i];
		}

		virtual std::size_t get_vertex_normal_count() const
		{
			if(fnMesh.numVertices() == fnMesh.numNormals())//smooth normal, like a sphere
			{
				MStatus status;
				int num = fnMesh.numVertices(&status);
				IfMErrorWarn(status);
				return num;
			}else{
				return 0;
			}
		}

		virtual asf::Vector3d get_vertex_normal(const std::size_t i) const
		{
			if(fnMesh.numVertices() == fnMesh.numNormals())//smooth normal, like a sphere
			{
				MVector nml;
				IfMErrorWarn(fnMesh.getVertexNormal(i, false, nml, MSpace::kObject));

				return asf::Vector3d(nml.x, nml.y, nml.z);
			}else{
				return asf::Vector3d();
			}
		}

		virtual std::size_t get_tex_coords_count() const
		{
// 			MFloatArray uArray, vArray;
// 			fnMesh.getUVs( uArray, vArray );
// 			std::size_t num = uArray.length();
// 			return num;
			return 0;
		}

		virtual asf::Vector2d get_tex_coords(const std::size_t i) const
		{
			float u,v;
			IfMErrorWarn( fnMesh.getUV(i, u, v, &currentUVsetName) );
			return asf::Vector2d(u,v);
		}

		virtual std::size_t get_face_count() const
		{
			std::size_t sum = 0;
			for(std::size_t i=0; i<triangleCounts.length(); ++i){
				sum += triangleCounts[ i ];
			}
			return sum;
		}

		virtual asf::IMeshWalker::Face get_face(const std::size_t i) const
		{
			asf::IMeshWalker::Face face;
			//pos index
			face.m_v0 = triangleVertices[i];
			face.m_v1 = triangleVertices[i+1];
			face.m_v2 = triangleVertices[i+2];
			//normal index
// 			if(fnMesh.numVertices() == fnMesh.numNormals())//smooth normal, like a sphere
// 			{
// 				face.m_n0 = triangleVertices[i];
// 				face.m_n1 = triangleVertices[i+1];
// 				face.m_n2 = triangleVertices[i+2];
// 			}
			//
			//face.m_t0 = ;
			//face.m_t1 = ;
			//face.m_t2 = ;
			//face.m_material = ;
			return face;
		}
		//
		void set(std::size_t sample)
		{
			const liqRibDataPtr mesh = ribNode__->object(sample)->getDataPtr();

			IfMErrorWarn(fnMesh.setObject(mesh->objDagPath));

			IfMErrorWarn(fnMesh.getTriangles(triangleCounts, triangleVertices));
			IfMErrorWarn(fnMesh.getCurrentUVSetName(currentUVsetName));
			_exportVertexFromNodePlug( ribNode__, sample, m_mesh.m_vertices );

		}
	};
	//////////////////////////////////////////////////////////////////////////
	//
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
			_write(pData, currentJob, m_gnode);
// 			o.RiEndRef();

		}else{
			//write the reference
			assert(pData->getRibFileFullPath() == fileName);
		}
	}
	//
	void _exportVertexFromNodePlug(
		const liqRibNodePtr &ribNode__,
		unsigned int sample,
		bool useWorldSpace,
		std::vector<float>& v)
	{	
		CM_TRACE_FUNC("_exportVertexFromNodePlug("<<ribNode__->name.asChar()<<","<<sample<<")");

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
		const liqFloat* vertex_buf = vertex.getTokenFloatArray();

		MFnMesh fnMesh(mesh->objDagPath, &status);
		IfMErrorWarn(status);


		MMatrix obj2world;
		obj2world = ribNode__->object( sample )->matrix( ribNode__->path().instanceNumber() );

		// add vertex position
		v.clear();
		if(useWorldSpace)
		{
			for(size_t i=0; i<fnMesh.numVertices(); ++i)
			{
				MPoint p(vertex_buf[3*i+0], vertex_buf[3*i+1], vertex_buf[3*i+2]);
				p *= obj2world;
				v.push_back( p.x );
				v.push_back( p.y );
				v.push_back( p.z );
			}
		}else{
			for(size_t i=0; i<fnMesh.numVertices(); ++i)
			{
				v.push_back( vertex_buf[3*i+0] );
				v.push_back( vertex_buf[3*i+1] );
				v.push_back( vertex_buf[3*i+2] );
			}
		}

	}
	//
	static void _write(liqRibMeshData* pData, const structJob &currentJob__, GlobalNodeHelper *m_gnode)
	{
		CM_TRACE_FUNC("as_writeMeshData.cpp::_write("<<pData->getFullPathName()<<","<<currentJob__.name.asChar()<<")");

 		liqRibNodePtr ribNode__ = liqRibTranslator::getInstancePtr()->getHTable()->find(
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
			( currentJob__.pass != rpShadowMap || currentJob__.shadowType == stDeep );

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
		const std::string meshFullPathName(mesh->getFullPathName());
		_s("\n//############################### mesh #");
		_s("//shape full path name="<<meshFullPathName);
#ifdef TRANSFORM_SHAPE_PAIR
		const std::string objectName(ribNode__->name.asChar());//shape
#else// SHAPE SHAPE_object PAIR
		const std::string objectName(getObjectName(ribNode__->name.asChar()));//shape+"_object"
#endif

		// AS stuff

		// use appleseed interfaces to export obj mesh.
		MString objFilePath;
		objFilePath = get_filesys_fullPathName(meshFullPathName).c_str();
		//try to makedir
		try{
			boost::filesystem::path p(get_mesh_dirname(meshFullPathName));
			if( ! boost::filesystem::exists(p) )
			{
				boost::filesystem::create_directories( p );
			}
		}catch(...){
			liquidMessage2(messageError, "fail to create dir: [%s]", get_mesh_dirname(meshFullPathName).c_str() );
			liquidMessage2(messageInfo, "mesh: [%s]",meshFullPathName.c_str() );
			return;
		}

		//
#ifdef _USE_APPLESEED_MESH_EXPORTER_
		MeshWalker walker(ribNode__);

		walker.set(sample_first);
		asf::OBJMeshFileWriter writer(objFilePath.asChar());
		writer.write(walker);
		writer.close();

		if( sample_first != sample_last )// motion blur stuff
		{
			walker.set(sample_last);
			asf::OBJMeshFileWriter writer((objFilePath+"_mb.obj").asChar());
			writer.write(walker);
			writer.close();
		}
#else
		bool groups      = m_gnode->getBool("export_mesh_facet_groups"); 
		bool ptgroups    = m_gnode->getBool("export_mesh_vertex_groups");
		bool materials   = m_gnode->getBool("export_mesh_materials");
		bool smoothing   = m_gnode->getBool("export_mesh_smoothing");
		bool normals     = m_gnode->getBool("export_mesh_normals"); 

		ObjTranslator writer;
		writer.set(groups, ptgroups, materials, smoothing, normals);
		_exportVertexFromNodePlug(ribNode__, sample_first, true, writer.position);
		writer.write(objFilePath, meshFullPathName.c_str() );

		if( sample_first != sample_last )// motion blur stuff
		{
			ObjTranslator writer;
			writer.set(groups, ptgroups, materials, smoothing, normals);
			_exportVertexFromNodePlug(ribNode__, sample_last, true, writer.position);
			writer.write(objFilePath+"_mb.obj", meshFullPathName.c_str() );
		}
#endif
		//todo
	}


}//namespace appleseed