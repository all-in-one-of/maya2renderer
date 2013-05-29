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
** Contributor(s): Philippe Leprince.
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
** Liquid Rib pfxToon Curve Data Source
** ______________________________________________________________________
*/
#include <liqRibPfxToonData.h>
// Boost headers
#include <boost/scoped_array.hpp>
// RenderMan jeaders
//extern "C" {
//#include "ri_interface.h"
//}
// Maya headers
#include <maya/MDoubleArray.h>
#include <maya/MItCurveCV.h>
#include <maya/MPoint.h>
#include <maya/MFnNurbsCurve.h>
#include <maya/MPlug.h>
#include <maya/MFnPfxGeometry.h>
#include <maya/MRenderLineArray.h>
#include <maya/MRenderLine.h>
#include <maya/MVectorArray.h>
#include <maya/MIntArray.h>
#include <maya/MFloatArray.h>
#include <maya/MColorArray.h>
#include <maya/MDagModifier.h>
#include <maya/MSelectionList.h>
#include <maya/MMatrix.h>

// Liquid headers
#include <liquid.h>
#include <liqGlobalHelpers.h>
#include <liqGlobalVariable.h>
//#include "renderman/rm_helper.h"
#include "renderermgr.h"

using namespace boost;



/** Create a RIB compatible representation of a Maya pfxToon node as RiCurves.
 */
liqRibPfxToonData::liqRibPfxToonData( MObject pfxToon )
  : nverts(),
    CVs(),
    curveWidth(),
    cvColor(),
    cvOpacity()
{
	CM_TRACE_FUNC("liqRibPfxToonData::liqRibPfxToonData("<<MFnDagNode(pfxToon).fullPathName().asChar()<<")");

  LIQDEBUGPRINTF( "-> creating pfxToon curves\n" );
  MStatus status( MS::kSuccess );

  // Update the pfxToon node with the renderCamera's position
  // otherwise the resulting outline might be incorrect
  MDagPath cameraPath;
  MSelectionList camList;
  camList.add( liqglo.liqglo_renderCamera );
  camList.getDagPath( 0, cameraPath );
  MMatrix cam_mat( cameraPath.inclusiveMatrix() );
  MFnDependencyNode pfxToonNode( pfxToon );
  pfxToonNode.findPlug( "cameraPointX" ).setValue( cam_mat( 3, 0 ) );
  pfxToonNode.findPlug( "cameraPointY" ).setValue( cam_mat( 3, 1 ) );
  pfxToonNode.findPlug( "cameraPointZ" ).setValue( cam_mat( 3, 2 ) );


  MFnPfxGeometry pfxtoon( pfxToon, &status );

  if ( status == MS::kSuccess ) {
    MRenderLineArray profileArray;
    MRenderLineArray creaseArray;
    MRenderLineArray intersectionArray;

    bool doLines          = true;
    bool doTwist          = false;
    bool doWidth          = true;
    bool doFlatness       = false;
    bool doParameter      = false;
    bool doColor          = true;
    bool doIncandescence  = false;
    bool doTransparency   = true;
    bool doWorldSpace     = false;

    status = pfxtoon.getLineData( profileArray, creaseArray, intersectionArray, doLines, doTwist, doWidth, doFlatness, doParameter, doColor, doIncandescence, doTransparency, doWorldSpace );

    if ( status == MS::kSuccess ) 
    {
      // Het the lines and fill the arrays.
      ncurves = profileArray.length();
      {
        MFnDependencyNode pfxNode( pfxToon );
        MString info( "[liquid] pfxToon node " );
        info += pfxNode.name() + " : " + ncurves + " curves.";
        printf("%s\n", info );
      }

      unsigned totalNumberOfVertices( 0 );

      if ( ncurves > 0 ) 
      {
        nverts = shared_array< liqInt >( new liqInt[ ncurves ] );

        // Calculate storage requirments.
        // This is a lot more efficient than all those reallocs()
        // (or resize()s if we used a vector) that were done before
        // in the main loop below.
        for ( unsigned i( 0 ); i < ncurves; i++ ) {
          MRenderLine theLine( profileArray.renderLine( i, &status ) );
          if ( MS::kSuccess == status ) {
            MVectorArray vertices( theLine.getLine() );
            totalNumberOfVertices += vertices.length();
          }
        }

        // Allocate memory
        CVs = shared_array< liqFloat >( new liqFloat[ totalNumberOfVertices * 3 ] );
        if ( !CVs ) 
        {
          //MString err( "liqRibPfxToonData failed to allocate CV memory!" );
          //cout << err << endl << flush;
          //throw( err );
          liquidMessage2(messageError, "[error] liqRibPfxToonData failed to allocate CV memory!" );
          return;
        }

        curveWidth = shared_array< liqFloat >( new liqFloat[ totalNumberOfVertices ] );
        if ( !curveWidth ) 
        {
          //MString err( "liqRibPfxToonData failed to allocate per vertex width memory!" );
          //cout << err << endl << flush;
          //throw( err );
          liquidMessage2(messageError, "[error] liqRibPfxToonData failed to allocate per vertex width memory!" );
          return;
        }

        cvColor = shared_array< liqFloat >( new liqFloat[ totalNumberOfVertices * 3 ] );
        if ( !cvColor ) 
        {
          //MString err( "liqRibPfxToonData failed to allocate CV color memory!" );
          //cout << err << endl << flush;
          //throw(err);
          liquidMessage2(messageError, "[error] liqRibPfxToonData failed to allocate CV color memory!" );
          return;
        }

        cvOpacity = shared_array< liqFloat >( new liqFloat[ totalNumberOfVertices * 3 ] );
        if ( !cvOpacity ) 
        {
          //MString err("liqRibPfxToonData failed to allocate CV opacity memory !");
          //cout << err << endl << flush;
          //throw( err );
          liquidMessage2(messageError, "[error] liqRibPfxToonData failed to allocate CV opacity memory !" );
          return;
        }

        liqFloat* cvPtr;
        liqFloat* widthPtr;
        liqFloat* colorPtr;
        liqFloat* opacityPtr;

        totalNumberOfVertices = 0;

        for ( unsigned i( 0 ); i < ncurves; i++ ) 
        {
          MRenderLine theLine( profileArray.renderLine( i, &status ) );

          if ( MS::kSuccess == status ) {

            const MVectorArray& vertices(           theLine.getLine() );
            const MDoubleArray& width(              theLine.getWidth() );
            const MVectorArray& vertexColor(        theLine.getColor() );
            const MVectorArray& vertexTransparency( theLine.getTransparency() );

            //cout <<"line "<<i<<" contains "<<vertices.length()<<" vertices."<<endl;
            //cout <<vertexColor<<endl;

            nverts[i] = vertices.length();
            totalNumberOfVertices += vertices.length();

            cvPtr      = CVs.get()        + ( totalNumberOfVertices * 3 - nverts[ i ] * 3 ) ;
            widthPtr   = curveWidth.get() + ( totalNumberOfVertices     - nverts[ i ] ) ;
            colorPtr   = cvColor.get()    + ( totalNumberOfVertices * 3 - nverts[ i ] * 3 ) ;
            opacityPtr = cvOpacity.get()  + ( totalNumberOfVertices * 3 - nverts[ i ] * 3 ) ;

            for ( unsigned vertIndex( 0 ); vertIndex < vertices.length(); vertIndex++ ) 
            {
              *cvPtr++      = ( liqFloat ) vertices[ vertIndex ].x;
              *cvPtr++      = ( liqFloat ) vertices[ vertIndex ].y;
              *cvPtr++      = ( liqFloat ) vertices[ vertIndex ].z;

              *widthPtr++   = ( liqFloat )width[ vertIndex ];

              *colorPtr++   = ( liqFloat )vertexColor[ vertIndex ].x ;
              *colorPtr++   = ( liqFloat )vertexColor[ vertIndex ].y ;
              *colorPtr++   = ( liqFloat )vertexColor[ vertIndex ].z ;

              *opacityPtr++ = ( liqFloat )( 1.0f - vertexTransparency[ vertIndex ].x ) ;
              *opacityPtr++ = ( liqFloat )( 1.0f - vertexTransparency[ vertIndex ].y ) ;
              *opacityPtr++ = ( liqFloat )( 1.0f - vertexTransparency[ vertIndex ].z ) ;
            }
          }
        }

        // Store for output
        liqTokenPointer points_pointerPair;
        if ( !points_pointerPair.set( "P", rPoint, true, false, totalNumberOfVertices ) ) 
        {
          //MString err( "liqRibPfxToonData: liqTokenPointer failed to allocate CV memory !" );
          //cout << err << endl;
          //throw(err);
          liquidMessage2(messageError, "[error] liqRibPfxToonData: liqTokenPointer failed to allocate CV memory !" );
          return;
        }
        points_pointerPair.setDetailType( rVertex );
        points_pointerPair.setTokenFloats( CVs );
        tokenPointerArray.push_back( points_pointerPair );

        // Store width params
        liqTokenPointer width_pointerPair;
        if ( !width_pointerPair.set( "width", rFloat, true, false, totalNumberOfVertices ) ) 
        {
          //MString err("liqRibPfxToonData: liqTokenPointer failed to allocate width memory !");
          //cout <<err<<endl;
          //throw(err);
          liquidMessage2(messageError, "[error] liqRibPfxToonData: liqTokenPointer failed to allocate width memory !" );
          return;
        }
        width_pointerPair.setDetailType( rVarying );
        width_pointerPair.setTokenFloats( curveWidth );
        tokenPointerArray.push_back( width_pointerPair );

        // Store color params
        liqTokenPointer color_pointerPair;
        if ( !color_pointerPair.set( "pfxToon_vtxColor", rColor, true, false, totalNumberOfVertices ) ) 
        {
          //MString err("liqRibPfxToonData: liqTokenPointer failed to allocate color memory !");
          //cout <<err<<endl;
          //throw(err);
          liquidMessage2(messageError, "[error] liqRibPfxToonData: liqTokenPointer failed to allocate color memory !" );
          return;
        }
        color_pointerPair.setDetailType( rVertex );
        color_pointerPair.setTokenFloats( cvColor );
        tokenPointerArray.push_back( color_pointerPair );

        // Store opacity params
        liqTokenPointer opacity_pointerPair;
        if ( !opacity_pointerPair.set( "pfxToon_vtxOpacity", rColor, true, false, totalNumberOfVertices ) ) 
        {
          //MString err("liqRibPfxToonData: liqTokenPointer failed to allocate opacity memory !");
          //cout <<err<<endl<<flush;
          //throw(err);
          liquidMessage2(messageError, "[error] liqRibPfxToonData: liqTokenPointer failed to allocate opacity memory !" );
          return;
        }
        opacity_pointerPair.setDetailType( rVertex );
        opacity_pointerPair.setTokenFloats( cvOpacity );
        tokenPointerArray.push_back( opacity_pointerPair );

        addAdditionalSurfaceParameters( pfxToon );

      }
    }
  }
}


//
void liqRibPfxToonData::write(const MString &ribFileName, const structJob &currentJob, const bool bReference)
{
	CM_TRACE_FUNC("liqRibPfxToonData::write("<<ribFileName.asChar()<<",job="<<currentJob.name.asChar()<<","<<bReference<<")");
	
	liquid::RendererMgr::getInstancePtr()->
		getRenderer()->write(this, ribFileName, currentJob, bReference);
// 	assert(liqglo.m_ribFileOpen&&"liqRibPfxToonData");
// 
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
/** Write the RIB for this paint effects toon line.
 */
// void liqRibPfxToonData::_write(const structJob &currentJob)
// {
// 	CM_TRACE_FUNC("liqRibPfxToonData::_write(job="<<currentJob.name.asChar()<<")");
// 
//   LIQDEBUGPRINTF( "-> writing pfxToon curve\n" );
// 
//   if ( 0 < ncurves  ) 
//   {
//     unsigned numTokens( tokenPointerArray.size() );
//     scoped_array< RtToken > tokenArray( new RtToken[ numTokens ] );
//     scoped_array< RtPointer > pointerArray( new RtPointer[ numTokens ] );
//     assignTokenArraysV( tokenPointerArray, tokenArray.get(), pointerArray.get() );
// 
//     RiCurvesV( "linear", ncurves, nverts.get(), "nonperiodic", numTokens, tokenArray.get(), pointerArray.get() );
//   } else 
//     RiIdentity(); // Make sure we don't create empty motion blocks
//   
// }

/** Compare this curve to the other for the purpose of determining
 *  if it is animated.
 */
bool liqRibPfxToonData::compare( const liqRibData & otherObj ) const
{
	CM_TRACE_FUNC("liqRibPfxToonData::compare("<<otherObj.getFullPathName()<<")");

  LIQDEBUGPRINTF( "-> comparing pfxToon curves\n");
  if ( otherObj.type() != MRT_PfxToon ) 
    return false;
  //const liqRibPfxToonData & other = (liqRibPfxToonData&)otherObj;

  // // Check CVs
  // last = nverts[0] * 3;
  // for ( i = 0; i < last; ++i ) {
  //     if ( !equiv( CVs[i], other.CVs[i] ) ) return false;
  // }

  return true;
}

/** Return the geometry type
 */
ObjectType liqRibPfxToonData::type() const
{
	CM_TRACE_FUNC("liqRibPfxToonData::type()");

  LIQDEBUGPRINTF( "-> returning pfxToon curve type\n" );
  return MRT_PfxToon;
}


