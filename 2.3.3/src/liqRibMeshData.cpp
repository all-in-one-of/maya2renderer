/*
**
** The contents of this file are subject to the Mozilla Public License Version
** 1.1 (the "License"); you may not use this file except in compliance with
** the License. You may obtain a copy of the License at
** http://www.mozilla.org/MPL/
**
** Software distributed under the License is distributed on an "AS IS" basis,
** WITHOUT WARRANTY OF ANY KIND, either express or implied. See the License
** for the specific language governing rights and limitations under the
** License.
**
** The Original Code is the Liquid Rendering Toolkit.
**
** The Initial Developer of the Original Code is Colin Doncaster. Portions
** created by Colin Doncaster are Copyright (C) 2002. All Rights Reserved.
**
** Contributor(s): Berj Bannayan.
**
**
** The RenderMan (R) Interface Procedures and Protocol are:
** Copyright 1988, 1989, Pixar
** All Rights Reserved
**
**
** RenderMan (R) is a registered trademark of Pixar
*/

/* ______________________________________________________________________
**
** Liquid Rib Mesh Data Source
** ______________________________________________________________________
*/
#include <liqRibMeshData.h>
// Standard/Boost headers
#include <vector>
#include <iostream>
#include <boost/scoped_array.hpp>
// Renderman Headers
//extern "C" {
#include "ri_interface.h"
//}

// Maya headers
#include <maya/MPlug.h>
#include <maya/MFloatVectorArray.h>
#include <maya/MGlobal.h>
#include <maya/MItMeshPolygon.h>
#include <maya/MIntArray.h>
#include <maya/MFnMesh.h>
#include <maya/MTransformationMatrix.h>
#include <maya/MMatrix.h>
#include <maya/MFloatPointArray.h>

// Liquid headers
#include <liquid.h>
#include <liqGlobalHelpers.h>
#include <liqGlobalVariable.h>
#include "renderermgr.h"
//#include "renderman/rm_renderer.h"


using namespace boost;
using namespace std;

/** Create a RIB compatible representation of a Maya polygon mesh.
 */
liqRibMeshData::liqRibMeshData( MObject mesh )
: numFaces( 0 ),
  numPoints ( 0 ),
  numNormals ( 0 ),
  nverts(),
  verts(),
  vertexParam(NULL),
  normalParam(NULL)
{
	CM_TRACE_FUNC("liqRibMeshData::liqRibMeshData("<<MFnDagNode(mesh).fullPathName().asChar()<<")");

	unsigned int i;
	unsigned int j;
  areaLight = false;
  LIQDEBUGPRINTF( "-> creating mesh\n" );
  MFnMesh fnMesh( mesh );
  objDagPath = fnMesh.dagPath();
  MStatus astatus;
  
  name = fnMesh.name();
  areaLight =( liquidGetPlugValue( fnMesh, "areaIntensity", areaIntensity, astatus ) == MS::kSuccess )? true : false ; 

  if ( areaLight ) 
  {
    MDagPath meshDagPath;
    meshDagPath = fnMesh.dagPath();
    MTransformationMatrix worldMatrix = meshDagPath.inclusiveMatrix();
    MMatrix worldMatrixM = worldMatrix.asMatrix();
    worldMatrixM.get( transformationMatrix );
  }

  numPoints = fnMesh.numVertices();
  numNormals = fnMesh.numNormals();

  // UV sets -------------------
  //
  //const unsigned numSTs( fnMesh.numUVs() );
  const unsigned numUVSets( fnMesh.numUVSets() );
  MString currentUVSetName;
  MStringArray extraUVSetNames;
  fnMesh.getCurrentUVSetName( currentUVSetName );
  {
    MStringArray UVSetNames;
    fnMesh.getUVSetNames( UVSetNames );

    for ( unsigned i( 0 ); i<numUVSets; i++ ) 
      if ( UVSetNames[i] != currentUVSetName ) 
        extraUVSetNames.append( UVSetNames[i] );
  }

  numFaces = fnMesh.numPolygons();
  const unsigned numFaceVertices( fnMesh.numFaceVertices() );

	if ( numPoints < 1 )
	{
//		MGlobal::displayInfo( MString( "fnMesh: " ) + fnMesh.name() );
//		cerr << "Liquid : Could not export degenerate mesh '"<< fnMesh.fullPathName( &astatus ).asChar() << "'" << endl << flush;
		return;
	}

  unsigned face = 0;
  unsigned faceVertex = 0;
  unsigned count;
  unsigned vertex;
  unsigned normal;
  float S;
  float T;
  MPoint point;
  liqTokenPointer pointsPointerPair;
  liqTokenPointer normalsPointerPair;
  liqTokenPointer pFaceVertexSPointer;
  liqTokenPointer pFaceVertexTPointer;

  // Allocate memory and tokens
  numFaces = numFaces;
  nverts = shared_array< RtInt >( new RtInt[ numFaces ] );
  verts = shared_array< RtInt >( new RtInt[ numFaceVertices ] );

  pointsPointerPair.set( "P", rPoint, numPoints );
  pointsPointerPair.setDetailType( rVertex );

  if ( numNormals == numPoints ) 
  {
    normalsPointerPair.set( "N", rNormal, numPoints );
    normalsPointerPair.setDetailType( rVertex );
  } 
  else 
  {
    normalsPointerPair.set( "N", rNormal, numFaceVertices );
    normalsPointerPair.setDetailType( rFaceVarying );
  }
  	
  // uv
  std::vector<liqTokenPointer> UVSetsArray;
  UVSetsArray.reserve( 1 + extraUVSetNames.length() );

  liqTokenPointer currentUVSetUPtr;
  liqTokenPointer currentUVSetVPtr;
  liqTokenPointer currentUVSetNamePtr;
  liqTokenPointer extraUVSetsUPtr;
  liqTokenPointer extraUVSetsVPtr;
  liqTokenPointer extraUVSetsNamePtr;
  if(liqglo.liqglo_outputMeshAsRMSArrays)
  {
	  currentUVSetUPtr.set( "s", rFloat, numFaceVertices );
	  currentUVSetUPtr.setDetailType( rFaceVarying );

	  currentUVSetVPtr.set( "t", rFloat, numFaceVertices );
	  currentUVSetVPtr.setDetailType( rFaceVarying );

	  currentUVSetNamePtr.set( "currentUVSet", rString, 1 );
	  currentUVSetNamePtr.setDetailType( rConstant );

	  if( numUVSets > 1 )
	  {
		  extraUVSetsUPtr.set( "u_uvSet", rFloat, numFaceVertices, numUVSets-1 );
		  extraUVSetsUPtr.setDetailType( rFaceVarying );

		  extraUVSetsVPtr.set( "v_uvSet", rFloat, numFaceVertices, numUVSets-1 );
		  extraUVSetsVPtr.setDetailType( rFaceVarying );

		  extraUVSetsNamePtr.set( "extraUVSets", rString, numUVSets-1 );
		  extraUVSetsNamePtr.setDetailType( rConstant );
	  }
  }
  else
  {
	  if ( numUVSets > 0 ) 
	  {
		liqTokenPointer pFaceVertexPointerPair;

		pFaceVertexPointerPair.set( "st", rFloat, numFaceVertices, 2 );
		pFaceVertexPointerPair.setDetailType( rFaceVarying );

		UVSetsArray.push_back( pFaceVertexPointerPair );

		for ( unsigned j( 0 ); j<extraUVSetNames.length(); j++) 
		{
		  liqTokenPointer pFaceVertexPointerPair;

		  pFaceVertexPointerPair.set( extraUVSetNames[j].asChar(), rFloat, numFaceVertices, 2 );
		  pFaceVertexPointerPair.setDetailType( rFaceVarying );

		  UVSetsArray.push_back( pFaceVertexPointerPair );
		}

		if( liqglo.liqglo_outputMeshUVs ) 
		{
		  // Match MTOR, which also outputs face-varying STs as well for some reason - Paul
		  // not anymore - Philippe
		  pFaceVertexSPointer.set( "u", rFloat, numFaceVertices );
		  pFaceVertexSPointer.setDetailType( rFaceVarying );

		  pFaceVertexTPointer.set( "v", rFloat, numFaceVertices );
		  pFaceVertexTPointer.setDetailType( rFaceVarying );
		}
	  }
  }

  vertexParam = pointsPointerPair.getTokenFloatArray();
  normalParam = normalsPointerPair.getTokenFloatArray();

  // Read the mesh from Maya
  MFloatVectorArray normals;
  fnMesh.getNormals( normals );

  for ( MItMeshPolygon polyIt ( mesh ); polyIt.isDone() == false; polyIt.next() ) 
  {
    count = polyIt.polygonVertexCount();
    nverts[face] = count;
	for( i=0; i<count; i++ )    // boucle sur les vertex de la face
    {
      vertex = polyIt.vertexIndex( i );
      verts[faceVertex] = vertex;
      point = polyIt.point( i, MSpace::kObject );
      pointsPointerPair.setTokenFloat( vertex, point.x, point.y, point.z );
      normal = polyIt.normalIndex( i );

      if( numNormals == numPoints ) 
        normalsPointerPair.setTokenFloat( vertex, normals[normal].x, normals[normal].y, normals[normal].z );
      else 
        normalsPointerPair.setTokenFloat( faceVertex, normals[normal].x, normals[normal].y, normals[normal].z );

	  if( liqglo.liqglo_outputMeshAsRMSArrays )
	  {
		  for( j=0; j<numUVSets; j++ )
		  {
			  if(j==0)
			  {
				  MString uvSetName = currentUVSetName;
				  // set uvSet name
				  currentUVSetNamePtr.setTokenString( 0, currentUVSetName.asChar() );
				  // set uv values
				  fnMesh.getPolygonUV( face, i, S, T, &uvSetName );

				  currentUVSetUPtr.setTokenFloat( faceVertex, S );
#ifdef DURANDUBOI
				  currentUVSetVPtr.setTokenFloat( faceVertex, T );
#else
				  currentUVSetVPtr.setTokenFloat( faceVertex, 1-T );
#endif
			  }
			  else
			  {
				  MString uvSetName = extraUVSetNames[j-1];
				  // set uvSet name
				  extraUVSetsNamePtr.setTokenString( j-1, extraUVSetNames[j-1].asChar() );
				  // set uv values
				  fnMesh.getPolygonUV( face, i, S, T, &uvSetName );
				  extraUVSetsUPtr.setTokenFloat( (numFaceVertices*(j-1)) + faceVertex, S );
#ifdef DURANDUBOI
				  extraUVSetsVPtr.setTokenFloat( (numFaceVertices*(j-1)) + faceVertex, T );
#else
				  extraUVSetsVPtr.setTokenFloat( (numFaceVertices*(j-1)) + faceVertex, 1-T );
#endif
			  }
		  }
	  }
	  else
	  {
		  if ( numUVSets ) 
		  {
			  for( j=0; j<numUVSets; j++ )
			  {
				  MString uvSetName;
				  if(j==0)
				  {
					  uvSetName = currentUVSetName;
				  }
				  else
				  {
					  uvSetName = extraUVSetNames[j-1];
				  }
				  fnMesh.getPolygonUV( face, i, S, T, &uvSetName );
				  UVSetsArray[j].setTokenFloat( faceVertex, 0, S );
				  UVSetsArray[j].setTokenFloat( faceVertex, 1, 1-T );
				  //printf("V%d  %s : %f %f  =>  %f %f \n", i, uvSetName.asChar(), S, T, S, 1-T);

				  if( liqglo.liqglo_outputMeshUVs && j==0)
				  {
					  // Match MTOR, which always outputs face-varying STs as well for some reason - Paul
					  pFaceVertexSPointer.setTokenFloat( faceVertex, S );
					  pFaceVertexTPointer.setTokenFloat( faceVertex, 1-T );
				  }
			  }
		  }
		}
      // printf( "[%d] faceVertex = %d  vertex = %d\n", i, faceVertex, vertex );

      ++faceVertex;
    }
    ++face;
  }
  // Add tokens to array and clean up after
  tokenPointerArray.push_back( pointsPointerPair );
  tokenPointerArray.push_back( normalsPointerPair );

  if(liqglo.liqglo_outputMeshAsRMSArrays)
  {
	  tokenPointerArray.push_back( currentUVSetNamePtr );
	  tokenPointerArray.push_back( currentUVSetUPtr );
	  tokenPointerArray.push_back( currentUVSetVPtr );
	  if( numUVSets > 1 )
	  {
		  tokenPointerArray.push_back( extraUVSetsNamePtr );
		  tokenPointerArray.push_back( extraUVSetsUPtr );
		  tokenPointerArray.push_back( extraUVSetsVPtr );
	  }
  }
  else
  {
	  if( UVSetsArray.size() ) 
		  tokenPointerArray.insert( tokenPointerArray.end(), UVSetsArray.begin(), UVSetsArray.end() );

	  if( liqglo.liqglo_outputMeshUVs ) 
	  {
		  tokenPointerArray.push_back( pFaceVertexSPointer );
		  tokenPointerArray.push_back( pFaceVertexTPointer );
	  }
  }

  addAdditionalSurfaceParameters( mesh );
}

/**      Print data about this mesh.
 */
void liqRibMeshData::printMesh()
{
	int i;
    scoped_array< RtInt > nloops( new RtInt[ numFaces ] );
	unsigned numTokens( tokenPointerArray.size() );

	LIQDEBUGPRINTF("numFace %d \n", numFaces);
	LIQDEBUGPRINTF("nloops (%d) [ ", numFaces);
	for(i=0; i<numFaces; i++)
	{
		LIQDEBUGPRINTF(" %d", nloops[i]);
	}
	LIQDEBUGPRINTF("]\n");

	int nvertsSize = nverts.use_count();
	RtInt *nvertsPtr = nverts.get();
	LIQDEBUGPRINTF("nverts (%d) [ ", nvertsSize);
	for(i=0; i<nvertsSize; i++)
	{
		LIQDEBUGPRINTF(" %d", nvertsPtr[i]);
	}
	LIQDEBUGPRINTF("]\n");


	int vertsSize = verts.use_count();
	RtInt *vertsPtr = verts.get();
	LIQDEBUGPRINTF("nverts (%d) [ ", vertsSize);
	for(i=0; i<vertsSize; i++)
	{
		LIQDEBUGPRINTF(" %d", vertsPtr[i]);
	}
	LIQDEBUGPRINTF("]\n");

	LIQDEBUGPRINTF("numTokens (%d)\n", numTokens);
	LIQDEBUGPRINTF("\n");
	// print tokens & pointers
}

void liqRibMeshData::write(const MString &ribFileName, const structJob &currentJob, const bool bReference)
{
	CM_TRACE_FUNC("liqRibMeshData::write("<<ribFileName.asChar()<<",job="<<currentJob.name.asChar()<<","<<bReference<<")");

	liquid::RendererMgr::getInstancePtr()->
		getRenderer()->write(this, ribFileName, currentJob, bReference);
// 	if( !bReference ){//write data at first time
// 		assert(m_ribFileFullPath.length()==0);
// 		m_ribFileFullPath = ribFileName;
// 
// 		renderman::Helper o;
// 		o.RiBeginRef(m_ribFileFullPath.asChar());
// 		_write(currentJob);
// 		o.RiEndRef();
// 
// 	}else{
// 		//write the reference
// 		assert(m_ribFileFullPath == ribFileName);
// 		RiReadArchive( const_cast< RtToken >( m_ribFileFullPath.asChar() ), NULL, RI_NULL );
// 	}
}
/**      Write the RIB for this mesh.
 */
//void liqRibMeshData::_write(const structJob &currentJob)
//{
//	if( this->isEmpty() )
//	{
//		liquidMessage( "Could not export degenerate mesh", messageError );
//		return;
//	}
////
//	if(this->isAreaLight())
//	{
//		RtLightHandle handle = INVALID_LIGHT_INDEX;
//
//		{ // What happens if we're inside a motion block????? This whole approach of Liquid is flawed...
//			LIQDEBUGPRINTF( "-> mesh is area light\n" );
//			//	RiAttributeBegin();
//			RtString ribname = const_cast< char* >( this->getName().asChar() );
//			RiAttribute( "identifier", "name", &ribname, RI_NULL );
//			RtMatrix tmp;
//			memcpy( tmp, this->getTransformationMatrixPtr(), sizeof( RtMatrix ) );
//			RiTransform( tmp );
//			float areaIntensity = this->getAreaIntensity();
//			handle = RiAreaLightSource( "arealight", "intensity", &areaIntensity, RI_NULL );
//		}
//		//
//		//mesh data begin
//		//
//		// Each loop has one polygon, so we just want an array of 1's of
//		// the correct size. Stack version.
//		//vector< RtInt > nloops( numFaces, 1 );
//		// Alternatively (heap version):
//		boost::scoped_array< RtInt > nloops( new RtInt[ this->getNumFaces() ] );
//		std::fill( nloops.get(), nloops.get() + this->getNumFaces(), ( RtInt )1 );
//
//		unsigned numTokens( this->tokenPointerArray.size() );
//		boost::scoped_array< RtToken > tokenArray( new RtToken[ numTokens ] );
//		boost::scoped_array< RtPointer > pointerArray( new RtPointer[ numTokens ] );
//		assignTokenArraysV( this->tokenPointerArray, tokenArray.get(), pointerArray.get() );
//
//		RiPointsGeneralPolygonsV( this->getNumFaces(),
//			&nloops[ 0 ],
//			this->getNverts().get(),
//			this->getVerts().get(),
//			numTokens,
//			tokenArray.get(),
//			pointerArray.get() );
//		//mesh data end
//
//		{
//			// RiAttributeEnd();
//			RiIlluminate( handle, 1 );
//		}
//	}else{
//		//mesh data begin
//		//
//		// Each loop has one polygon, so we just want an array of 1's of
//		// the correct size. Stack version.
//		//vector< RtInt > nloops( numFaces, 1 );
//		// Alternatively (heap version):
//		boost::scoped_array< RtInt > nloops( new RtInt[ this->getNumFaces() ] );
//		std::fill( nloops.get(), nloops.get() + this->getNumFaces(), ( RtInt )1 );
//
//		unsigned numTokens( this->tokenPointerArray.size() );
//		boost::scoped_array< RtToken > tokenArray( new RtToken[ numTokens ] );
//		boost::scoped_array< RtPointer > pointerArray( new RtPointer[ numTokens ] );
//		assignTokenArraysV( this->tokenPointerArray, tokenArray.get(), pointerArray.get() );
//
//		RiPointsGeneralPolygonsV( this->getNumFaces(),
//			&nloops[ 0 ],
//			this->getNverts().get(),
//			this->getVerts().get(),
//			numTokens,
//			tokenArray.get(),
//			pointerArray.get() );
//		//mesh data end//	
//	}
//
//}
//
/** Compare this mesh to the other for the purpose of determining
 *  if it's animated.
 */
bool liqRibMeshData::compare( const liqRibData & otherObj ) const
{
	CM_TRACE_FUNC("liqRibMeshData::compare("<<otherObj.getFullPathName()<<")");

  unsigned numFaceVertices( 0 );

  LIQDEBUGPRINTF( "-> comparing mesh\n" );
  if ( otherObj.type() != MRT_Mesh ) 
    return false;
  const liqRibMeshData& other = (liqRibMeshData&)otherObj;

  if ( numFaces != other.numFaces )     return false;
  if ( numPoints != other.numPoints )   return false;
  if ( numNormals != other.numNormals ) return false;

  for ( unsigned i( 0 ); i < numFaces; ++i ) 
  {
    if ( nverts[i] != other.nverts[i] ) 
      return false;
    numFaceVertices += nverts[i];
  }

  for ( unsigned i( 0 ); i < numFaceVertices; ++i ) 
    if ( verts[i] != other.verts[i] ) 
      return false;

  for ( unsigned i( 0 ); i < numPoints; ++i ) 
  {
    const unsigned a( i * 3 );
    const unsigned b( a + 1 );
    const unsigned c( a + 2 );
    if (  !equiv( vertexParam[a], other.vertexParam[a] ) ||
          !equiv( vertexParam[b], other.vertexParam[b] ) ||
          !equiv( vertexParam[c], other.vertexParam[c] ) )
    {
      return false;
    }
  }

  for ( unsigned i( 0 ); i < numNormals; ++i ) 
  {
    const unsigned a = i * 3;
    const unsigned b = a + 1;
    const unsigned c = a + 2;
    if (  !equiv( normalParam[a], other.normalParam[a] ) ||
          !equiv( normalParam[b], other.normalParam[b] ) ||
          !equiv( normalParam[c], other.normalParam[c] ) )
    {
      return false;
    }
  }
  return true;
}


/** Return the geometry type.
 */
ObjectType liqRibMeshData::type() const
{
  LIQDEBUGPRINTF( "-> returning mesh type\n" );
  return ( areaLight )? MRT_Light : MRT_Mesh;
}

