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
#include <liqRibCameraData.h>
// Standard/Boost headers
#include <vector>
#include <iostream>
#include <boost/scoped_array.hpp>
// Renderman Headers
//extern "C" {
//#include "ri_interface.h"
//}

// Maya headers
#include <maya/MPlug.h>
#include <maya/MFloatVectorArray.h>
#include <maya/MGlobal.h>
#include <maya/MItMeshPolygon.h>
#include <maya/MIntArray.h>
#include <maya/MFnCamera.h>
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
liqRibCameraData::liqRibCameraData( MObject obj )
{
	CM_TRACE_FUNC("liqRibCameraData::liqRibCameraData("<<MFnDagNode(obj).fullPathName().asChar()<<")");

	LIQDEBUGPRINTF( "-> creating camera\n" );
	MStatus astatus;

	MFnCamera fnCamera( obj );
	fnCamera.getPath(objDagPath);
	name = fnCamera.name();

	MString fullpathname(objDagPath.fullPathName());
	MTransformationMatrix worldMatrix = objDagPath.inclusiveMatrix();
	//MMatrix invmatrix(worldMatrix.asMatrixInverse());
	MMatrix worldMatrixM = worldMatrix.asMatrix();
	worldMatrixM.get( transformationMatrix );
}


void liqRibCameraData::write(const MString &ribFileName, const structJob &currentJob, const bool bReference)
{
	CM_TRACE_FUNC("liqRibCameraData::write("<<ribFileName.asChar()<<",job="<<currentJob.name.asChar()<<","<<bReference<<")");

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
//		//vector< liqInt > nloops( numFaces, 1 );
//		// Alternatively (heap version):
//		boost::scoped_array< liqInt > nloops( new liqInt[ this->getNumFaces() ] );
//		std::fill( nloops.get(), nloops.get() + this->getNumFaces(), ( liqInt )1 );
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
//		//vector< liqInt > nloops( numFaces, 1 );
//		// Alternatively (heap version):
//		boost::scoped_array< liqInt > nloops( new liqInt[ this->getNumFaces() ] );
//		std::fill( nloops.get(), nloops.get() + this->getNumFaces(), ( liqInt )1 );
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
bool liqRibCameraData::compare( const liqRibData & otherObj ) const
{
	CM_TRACE_FUNC("liqRibCameraData::compare("<<otherObj.getFullPathName()<<")");

  LIQDEBUGPRINTF( "-> comparing camera\n" );
  if ( otherObj.type() != MRT_Camera ) 
    return false;

  const liqRibMeshData& other = (liqRibMeshData&)otherObj;
  //...

  return true;
}


/** Return the geometry type.
 */
ObjectType liqRibCameraData::type() const
{
  LIQDEBUGPRINTF( "-> returning camera type\n" );
  return MRT_Camera;
}

