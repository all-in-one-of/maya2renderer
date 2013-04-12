#include <liqConfig.h>
#ifdef _USE_ELVISHRAY_

#include "er_renderer.h"
#include <liqRibMeshData.h>
#include "../common/prerequest_maya.h"
#include "../common/mayacheck.h"
#include "ercall.h"
#include "er_log_helper.h"
//#include <liqRibNode.h>
#include <liqRibTranslator.h>
namespace elvishray
{
	static void _write0(liqRibMeshData* pData, const structJob &currentJob__);
	static void _write1(liqRibMeshData* pData, const structJob &currentJob__);
	static void _write(liqRibMeshData* pData, const structJob &currentJob);
	static void _exportUVFromNodePlug( const liqRibNodePtr &ribNode__, unsigned int sample);
	static const liqFloat* _exportVertexFromNodePlug( const liqRibNodePtr &ribNode__, unsigned int sample);
	static int getVertexInexInPolygon( const int gvi, const MIntArray &polygonVertices);

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
//			RiReadArchive( const_cast< RtToken >( pData->getRibFileFullPath().asChar() ), NULL, RI_NULL );
		}
	}
	//
	void _exportVertexFromNodePlug(
		const liqRibNodePtr &ribNode__,
		unsigned int sample,
		const MIntArray &triangleVertices)
	{	
		CM_TRACE_FUNC("_exportVertexFromNodePlug("<<ribNode__->name.asChar()<<","<<sample<<", triangleVertices.size="<<triangleVertices.length()<<")");

		const liqFloat* vertex_buf = _exportVertexFromNodePlug(ribNode__, sample);

		// add vertex position to ER
#ifdef _OLD_WAY_
		MStatus status;
		MFnMesh fnMesh(mesh->objDagPath, &status);
		IfMErrorWarn(status);
		for(size_t i=0; i<fnMesh.numVertices(); ++i)
		{
			_S( ei_tab_add_vector( vertex_buf[3*i+0],vertex_buf[3*i+1],vertex_buf[3*i+2] ) );
		}
#else
		for(size_t i=0; i<triangleVertices.length(); ++i)
		{
			int vi = triangleVertices[i];
			//_s("//"<<vi);
			_S( ei_tab_add_vector( vertex_buf[3*vi+0],vertex_buf[3*vi+1],vertex_buf[3*vi+2] ) );
		}
#endif
	}
	static const liqFloat* _exportVertexFromNodePlug(
		const liqRibNodePtr &ribNode__,
		unsigned int sample)
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
		return vertex.getTokenFloatArray();
	}
	//
	static void _write(liqRibMeshData* pData, const structJob &currentJob__)
	{
		//_write0(pData, currentJob__);
		_write1(pData, currentJob__);
	}
	static void _write0(liqRibMeshData* pData, const structJob &currentJob__)
	{
		CM_TRACE_FUNC("er_writeMeshData.cpp::_write0("<<pData->getFullPathName()<<","<<currentJob__.name.asChar()<<")");

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
		_s("\n//############################### mesh #");
		_s("//shape full path name="<<mesh->getFullPathName());
#ifdef TRANSFORM_SHAPE_PAIR
		const std::string objectName(ribNode__->name.asChar());//shape
#else// SHAPE SHAPE_object PAIR
		const std::string objectName(getObjectName(ribNode__->name.asChar()));//shape+"_object"
#endif
		_S( ei_object( "poly", objectName.c_str() ) );
		_s("{");
		_d( eiTag tag );

		//vertex position
		_s("//### vertex positions, fnMesh.numVertices()="<<fnMesh.numVertices() );
		_d( tag = ei_tab(EI_TYPE_VECTOR, 1024) )
		_S( ei_pos_list( tag ) );

		//_exportVertexFromDagNode(fnMesh);
		_exportVertexFromNodePlug(ribNode__, sample_first, triangleVertices);

		_S( ei_end_tab() );

		{//deform motion
			if( sample_first != sample_last )
			{
				_s("//### vertex deform positions, " );
				_d( tag = ei_tab(EI_TYPE_VECTOR, 1024) )
				_S( ei_motion_pos_list( tag ) );

				_exportVertexFromNodePlug(ribNode__, sample_last, triangleVertices);

				_S( ei_end_tab() );
			}
		}

// 		_s("//### vertex color");
// 		for(int i=0; i<position.length(); i++)
// 		{
// 			_S( ei_vertex(i) );
// 			//_S( ei_variable_color( "Cs", color( 1.0f, 0.0f, 1.0f ) ) );
// 		}

		// get normal for each vertex
		// but the render result seems very weird, see test/test_er_light/output_img_std/er_pointlight.perspShape.1.elvishray_vertex_normal.bmp
		// so I ommit this section temporarily.
		_s("//### N ###");
		if(fnMesh.numVertices() == fnMesh.numNormals())//smooth normal, like a sphere
		{

			_d( tag = eiNULL_TAG );
			_S( ei_declare("N", EI_VARYING, EI_TYPE_TAG, &tag) );
			_d( tag = ei_tab(EI_TYPE_VECTOR, 1024) )
			_S( ei_variable("N", &tag) );
			MVector nml;
#ifdef _OLD_WAY_
			for(size_t i = 0; i<fnMesh.numVertices(); ++i)
			{
				IfMErrorWarn(fnMesh.getVertexNormal(i, false, nml, MSpace::kObject));
				_S( ei_tab_add_vector(nml.x, nml.y, nml.z) );
			}
#else
			for(size_t i=0; i<triangleVertices.length(); ++i)
			{
				int vi = triangleVertices[i];
				//_s("//"<<vi);
				IfMErrorWarn(fnMesh.getVertexNormal(vi, false, nml, MSpace::kObject));
				_S( ei_tab_add_vector(nml.x, nml.y, nml.z) );
			}
#endif


			_S( ei_end_tab() );
		}else{//sharp edge, like a cube
			// in this case, like a cube, a vertex has a specified normal corresponding to each adjacent polygon.
			// but elvishray's only allow a vertex to be assigned only one normal.
			// so I can't generate the normal list for this case.
			_s("//numNormals != numPoints, ER doesn't support this case now");
		}
		_s("//### N ### end");

		if( currentUVsetName.length() != 0 )//there is a current uv set
		{
			MFloatArray u_coords;
			MFloatArray v_coords;

			IfMErrorWarn( fnMesh.getUVs(u_coords,v_coords,&currentUVsetName) );
			_s("//### UV("<<currentUVsetName.asChar()<<"), size="<< fnMesh.numUVs(currentUVsetName) );

			int numUVSets = fnMesh.numUVSets();
			_s("//# numUVSets("<<numUVSets );
			MStringArray uvsetNames;
			IfMErrorWarn( fnMesh.getUVSetNames(uvsetNames) );
			for(std::size_t i = 0; i< uvsetNames.length(); ++i){
				_s("//# uvsetNames["<<i<<"]="<<uvsetNames[i].asChar() );
			}
			MStringArray uvsetFamilyNames;
			IfMErrorWarn( fnMesh.getUVSetFamilyNames(uvsetFamilyNames) );
			for(std::size_t i = 0; i< uvsetFamilyNames.length(); ++i){
				_s("//# uvsetFamilyNames["<<i<<"]="<<uvsetFamilyNames[i].asChar() );
			}

			// uv
			_d( tag = eiNULL_TAG );
			_S( ei_declare("uv", EI_VARYING, EI_TYPE_TAG, &tag) );
			_d( tag = ei_tab(EI_TYPE_VECTOR2, 1024) )
			_S( ei_variable("uv", &tag) );
#ifdef _OLD_WAY_			
			for(size_t i = 0; i<fnMesh.numUVs(currentUVsetName); ++i)
			{
				_S( ei_tab_add_vector2(u_coords[i], v_coords[i]) );
			}
#else
			_exportUVFromNodePlug(ribNode__, sample_first);
#endif
			_S( ei_end_tab() );
		}

		_s("//### triangles, size="<< triangleCounts.length());
		_d( tag = ei_tab(EI_TYPE_INDEX, 1024) )
		_S( ei_triangle_list( tag ) );
#ifdef _OLD_WAY_		
		for(size_t i=0; i<triangleVertices.length(); i=i+3)
		{
			_S( ei_tab_add_index(triangleVertices[i])); 
			_S( ei_tab_add_index(triangleVertices[i+1])); 
			_S( ei_tab_add_index(triangleVertices[i+2])); 
		}
#else
		for(size_t i=0; i<triangleVertices.length(); ++i)
		{
			_S( ei_tab_add_index(i));
		}
#endif
		_S( ei_end_tab() );
		_s("}//"<<objectName);
		_S( ei_end_object() );
	}
	//
	static void _exportUVFromNodePlug( const liqRibNodePtr &ribNode__, unsigned int sample )
	{	
		CM_TRACE_FUNC("_exportUVFromNodePlug("<<ribNode__->name.asChar()<<","<<sample<<")");

		MStatus status;

		const liqRibDataPtr ribdata = ribNode__->object(sample)->getDataPtr();
		liqRibMeshData* mesh = (liqRibMeshData*)(ribdata.get());
		const std::vector<liqTokenPointer>& tokenPointerArray = mesh->tokenPointerArray;

		liqTokenPointer uv;
		for( std::vector< liqTokenPointer >::const_iterator iter( tokenPointerArray.begin() ); iter != tokenPointerArray.end(); ++iter ) 
		{
			if( "facevarying float[2] st" == const_cast< liqTokenPointer* >( &( *iter ) )->getDetailedTokenName() )
			{
				uv = *iter;
				break;
			}
		}
		assert( !uv.empty() );
		const liqFloat* uv_buf = uv.getTokenFloatArray();

		MFnMesh fnMesh(mesh->objDagPath, &status);
		IfMErrorWarn(status);

		const unsigned numFaceVertices( fnMesh.numFaceVertices() );
		// add uv to ER
		for(size_t i=0; i<numFaceVertices; ++i)
		{
			_S( ei_tab_add_vector2( uv_buf[2*i+0], 1.0f - uv_buf[2*i+1] ) );
		}

	}
	static void _write1(liqRibMeshData* pData, const structJob &currentJob__)
	{
		CM_TRACE_FUNC("er_writeMeshData.cpp::_write1("<<pData->getFullPathName()<<","<<currentJob__.name.asChar()<<")");

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

		const liqFloat *P = _exportVertexFromNodePlug(ribNode__, sample_first);
		liqFloat *tmp = NULL;
		if( sample_first != sample_last )
		{
			tmp = const_cast<liqFloat*>(_exportVertexFromNodePlug(ribNode__, sample_last));
		}
		const liqFloat *P_mb = tmp;

		std::vector<MVector> POSITION;
		std::vector<MVector> POSITION_mb;
		std::vector<std::size_t> INDEX;//global vertex index
		std::vector<MVector> NORMAL;
		std::vector<MVector> UV;

		int numPolygons = fnMesh.numPolygons();
		for(int gpi = 0; gpi< numPolygons; ++gpi)//gpi: global polygon index
		{
			//  for one polygon

			MIntArray vertexList;
			fnMesh.getPolygonVertices(gpi, vertexList);
			assert( vertexList.length() == fnMesh.polygonVertexCount( gpi ) );
			// vertex index in polygon: i  <---> global vertex index: vertexList[i]

			//int vertexCountInPolygon   = fnMesh.polygonVertexCount( gpi );
			int triangleCountInPolygon = triangleCounts[ gpi ];
			for(int ti = 0; ti<triangleCountInPolygon; ++ti)//ti: triangle index in a polygon
			{
				//  for one triangle

				int gvi[3];//global vertex index
				fnMesh.getPolygonTriangleVertices(gpi, ti, gvi);

				//position, triangle index list
				INDEX.push_back(POSITION.size());
				POSITION.push_back(MVector(P[3*gvi[0]+0], P[3*gvi[0]+1], P[3*gvi[0]+2]));
				INDEX.push_back(POSITION.size());
				POSITION.push_back(MVector(P[3*gvi[1]+0], P[3*gvi[1]+1], P[3*gvi[1]+2]));
				INDEX.push_back(POSITION.size());
				POSITION.push_back(MVector(P[3*gvi[2]+0], P[3*gvi[2]+1], P[3*gvi[2]+2]));
				//position motion blur
				if( sample_first != sample_last )
				{
					POSITION_mb.push_back(MVector(P_mb[3*gvi[0]+0], P_mb[3*gvi[0]+1], P_mb[3*gvi[0]+2]));
					POSITION_mb.push_back(MVector(P_mb[3*gvi[1]+0], P_mb[3*gvi[1]+1], P_mb[3*gvi[1]+2]));
					POSITION_mb.push_back(MVector(P_mb[3*gvi[2]+0], P_mb[3*gvi[2]+1], P_mb[3*gvi[2]+2]));
				}


				//normal
				MVector normal0, normal1, normal2;
				fnMesh.getVertexNormal(gvi[0], false, normal0);
				fnMesh.getVertexNormal(gvi[1], false, normal1);
				fnMesh.getVertexNormal(gvi[2], false, normal2);
				
				NORMAL.push_back(normal0);
				NORMAL.push_back(normal1);
				NORMAL.push_back(normal2);


				//uv
				float u0, v0;
				float u1, v1;
				float u2, v2;
				int vi0, vi1, vi2;// vertex index in polygon
				vi0 = getVertexInexInPolygon( gvi[0], vertexList);
				vi1 = getVertexInexInPolygon( gvi[1], vertexList);
				vi2 = getVertexInexInPolygon( gvi[2], vertexList);
				fnMesh.getPolygonUV(gpi, vi0, u0, v0, &currentUVsetName);
				fnMesh.getPolygonUV(gpi, vi1, u1, v1, &currentUVsetName);
				fnMesh.getPolygonUV(gpi, vi2, u2, v2, &currentUVsetName);

				UV.push_back(MVector(u0, v0));
				UV.push_back(MVector(u1, v1));
				UV.push_back(MVector(u2, v2));


			}//for(int ti = 0; ti<triangleCountInPolygon; ++ti)
		}//for(int gpi = 0; gpi< numPolygons; ++gpi)



		// geometry data (shape)
		_s("\n//############################### mesh #");
		_s("//shape full path name="<<mesh->getFullPathName());
#ifdef TRANSFORM_SHAPE_PAIR
		const std::string objectName(ribNode__->name.asChar());//shape
#else// SHAPE SHAPE_object PAIR
		const std::string objectName(getObjectName(ribNode__->name.asChar()));//shape+"_object"
#endif


		_S( ei_object( "poly", objectName.c_str() ) );
		_s("{");
		_d( eiTag tag );

		//vertex position
		_s("//### vertex positions, fnMesh.numVertices()="<<fnMesh.numVertices() );
		_d( tag = ei_tab(EI_TYPE_VECTOR, 1024) )
		_S( ei_pos_list( tag ) );
		for(std::size_t i=0; i<POSITION.size(); ++i)
		{
			_S( ei_tab_add_vector( POSITION[i].x, POSITION[i].y, POSITION[i].z ) );
		}
		_S( ei_end_tab() );

		//deform motion
		if( sample_first != sample_last )
		{
			_s("//### vertex deform positions, " );
			_d( tag = ei_tab(EI_TYPE_VECTOR, 1024) )
			_S( ei_motion_pos_list( tag ) );
			for(std::size_t i=0; i<POSITION_mb.size(); ++i)
			{
				_S( ei_tab_add_vector( POSITION_mb[i].x, POSITION_mb[i].y, POSITION_mb[i].z ) );
			}
			_S( ei_end_tab() );
		}
		

// 		_s("//### vertex color");
// 		for(int i=0; i<position.length(); i++)
// 		{
// 			_S( ei_vertex(i) );
// 			//_S( ei_variable_color( "Cs", color( 1.0f, 0.0f, 1.0f ) ) );
// 		}

		// get normal for each vertex
		// but the render result seems very weird, see test/test_er_light/output_img_std/er_pointlight.perspShape.1.elvishray_vertex_normal.bmp
		// so I ommit this section temporarily.
		_s("//### N ###");
		if(fnMesh.numVertices() == fnMesh.numNormals())//smooth normal, like a sphere
		{
			_d( tag = eiNULL_TAG );
			_S( ei_declare("N", EI_VARYING, EI_TYPE_TAG, &tag) );
			_d( tag = ei_tab(EI_TYPE_VECTOR, 1024) )
			_S( ei_variable("N", &tag) );
			for(size_t i=0; i<NORMAL.size(); ++i)
			{
				_S( ei_tab_add_vector(NORMAL[i].x, NORMAL[i].y, NORMAL[i].z) );
			}
			_S( ei_end_tab() );
		}else{//sharp edge, like a cube
			// in this case, like a cube, a vertex has a specified normal corresponding to each adjacent polygon.
			// but elvishray's only allow a vertex to be assigned only one normal.
			// so I can't generate the normal list for this case.
			_s("//numNormals != numPoints, ER doesn't support this case now");
		}
		_s("//### N ### end");

		if( currentUVsetName.length() != 0 )//there is a current uv set
		{
			_s("//### UV("<<currentUVsetName.asChar()<<"), size="<< fnMesh.numUVs(currentUVsetName) );

			int numUVSets = fnMesh.numUVSets();
			_s("//# numUVSets("<<numUVSets );
			MStringArray uvsetNames;
			IfMErrorWarn( fnMesh.getUVSetNames(uvsetNames) );
			for(std::size_t i = 0; i< uvsetNames.length(); ++i){
				_s("//# uvsetNames["<<i<<"]="<<uvsetNames[i].asChar() );
			}
			MStringArray uvsetFamilyNames;
			IfMErrorWarn( fnMesh.getUVSetFamilyNames(uvsetFamilyNames) );
			for(std::size_t i = 0; i< uvsetFamilyNames.length(); ++i){
				_s("//# uvsetFamilyNames["<<i<<"]="<<uvsetFamilyNames[i].asChar() );
			}

			// uv
			_d( tag = eiNULL_TAG );
			_S( ei_declare("uv", EI_VARYING, EI_TYPE_TAG, &tag) );
			_d( tag = ei_tab(EI_TYPE_VECTOR2, 1024) )
			_S( ei_variable("uv", &tag) );
			for(size_t i = 0; i<UV.size(); ++i)
			{
				_S( ei_tab_add_vector2(UV[i].x, UV[i].y) );
			}
			_S( ei_end_tab() );
		}

		_s("//### triangles, size="<< INDEX.size());
		_d( tag = ei_tab(EI_TYPE_INDEX, 1024) )
		_S( ei_triangle_list( tag ) );
		for(size_t i=0; i<INDEX.size(); ++i)
		{
			_S( ei_tab_add_index(INDEX[i]));
		}
		_S( ei_end_tab() );
		_s("}//"<<objectName);
		_S( ei_end_object() );
	}
	//
	static int getVertexInexInPolygon( const int gvi, const MIntArray &polygonVertices)
	{
		for(int i = 0; i<polygonVertices.length(); ++i)
		{
			if( gvi == polygonVertices[ i ] )
			{
				return i;
			}
		}
		liquidMessage2(messageError, "global vertex index \"%d\" not found", gvi);
		return -1;
	}
}//namespace elvishray

#endif//_USE_ELVISHRAY_