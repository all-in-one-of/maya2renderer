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
** Liquid Rib Paint Effects Data Source
** ______________________________________________________________________
*/
#include <liqRibPfxData.h>

#include <liqlog.h>
//#include <liqGlobalHelpers.h>
//#include <liqRibNode.h>
//#include <liqGlobalVariable.h>
//#include "renderman/rm_helper.h"
#include "renderermgr.h"

using namespace boost;



// For debug purposes: set to 1, 2, 3 to see only mainLines, also leafLines and also flowerLines
#define MAX_DETAIL 3

/** Create a RIB compatible representation of a Maya Paint Effectrs object.
 */
liqRibPfxData::liqRibPfxData( MObject pfxGeo, ObjectType type )
  : grain( 0 ),
    nverts(),
    CVs()
{
	CM_TRACE_FUNC("liqRibPfxData::e("<<MFnDagNode(pfxGeo).fullPathName().asChar()<<","<<type<<")");

	LIQDEBUGPRINTF( "-> creating painteffects curves\n" );
	MStatus status( MS::kSuccess );
	pfxtype = type;
	MFnPfxGeometry pfx( pfxGeo, &status );
	assert( MS::kSuccess == status );

	bool hasShader( 0 );
	MPlug rmanSGPlug;
	if( type == MRT_PfxTube )
		rmanSGPlug = pfx.findPlug( MString( "liquidTubeShadingGroup" ), &status );
	if( type == MRT_PfxLeaf )
		rmanSGPlug = pfx.findPlug( MString( "liquidLeafShadingGroup" ), &status );
	if( type == MRT_PfxPetal )
		rmanSGPlug = pfx.findPlug( MString( "liquidPetalShadingGroup" ), &status );
	if( status == MS::kSuccess && rmanSGPlug.isConnected() )
	{
		MPlugArray rmSGArray;
		rmanSGPlug.connectedTo( rmSGArray, true, true );
		MObject rmSGObj( rmSGArray[0].node() );
		MFnDependencyNode SGDepNode( rmSGObj );
		if( SGDepNode.typeName() == "shadingEngine" )
			hasShader = 1;
	}

	// create random number to be picked up by shader
	srand( 1 );

	// camera facing attributes
	bool camFacing[3] = { false, false, false };
	MPlug pFacing = pfx.findPlug( "liquidTubeCameraFacing", status );
	if( status == MS::kSuccess )
		pFacing.getValue( camFacing[0] );
	pFacing = pfx.findPlug( "liquidLeafCameraFacing", status );
	if( status == MS::kSuccess )
		pFacing.getValue( camFacing[1] );
	pFacing = pfx.findPlug( "liquidPetalCameraFacing", status );
	if( status == MS::kSuccess )
		pFacing.getValue( camFacing[2] );

	// uniform width attributes
	bool uniformWidth[3] = { false, false, false };
	MPlug pUniform = pfx.findPlug( "liquidTubeUniformWidth", status );
	if( status == MS::kSuccess )
		pUniform.getValue( uniformWidth[0] );
	pUniform = pfx.findPlug( "liquidLeafUniformWidth", status );
	if( status == MS::kSuccess )
		pUniform.getValue( uniformWidth[1] );
	pUniform = pfx.findPlug( "liquidPetalUniformWidth", status );
	if( status == MS::kSuccess )
		pUniform.getValue( uniformWidth[2] );

	// uniform rand attribute
	bool hasCurveID = false;
	MPlug curveID = pfx.findPlug( "liquidCurveID", status );
	if( status == MS::kSuccess )
		curveID.getValue( hasCurveID );

	MRenderLineArray lines[ 3 ];

	status = pfx.getLineData( lines[ 0 ], lines[ 1 ], lines[ 2 ],
                            true,  // lines
                            true,  // twist
                            true,  // width
                            true,  // flatness
                            false, // parameter
                            true,  // color
                            true,  // incandescence
                            true,  // transparency
                            false  // worldSpace
                          );
	assert( MS::kSuccess == status );
	LIQDEBUGPRINTF( "-> painteffects line data found\n" );

	/* we have to try and be clever and work out how the lines are connected or else you just end
	* up with ugly line segments, boooo!
	*/

	// If there's only main lines, do twist
	// bool doFirstTwist( !lines[ 1 ].length() && !lines[ 2 ].length() );

	// default: MRT_PfxTube
	unsigned setOn( 0 );
	if( type == MRT_PfxLeaf )
		setOn = 1;
	if( type == MRT_PfxPetal )
		setOn = 2;

	unsigned totalVertex( 0 );
	unsigned totalVarying( 0 );
	unsigned numLines( lines[ setOn ].length() );

	for( unsigned lineOn( 0 ); lineOn < numLines; lineOn++ )
	{
		MRenderLine pfxLine( lines[ setOn ].renderLine( lineOn, &status ) );
		MVectorArray pfxVerts( pfxLine.getLine() );
		totalVarying += pfxVerts.length();
	}
	totalVertex = totalVarying + ( 2 * numLines );

	if( totalVarying )
	{
		hasFeature[ setOn ] = true;

		// read other attributes from the lines
		CVs = shared_array< liqFloat >(              new liqFloat[ totalVertex * 3 ] );
		shared_array< liqFloat > curveTwist(         new liqFloat[ totalVarying * 3 ] );
		shared_array< liqFloat > uniformCurveWidth(  new liqFloat[ numLines ] );
		shared_array< liqFloat > curveID(			new liqFloat[ numLines ] );
		shared_array< liqFloat > curveWidth(         new liqFloat[ totalVarying ] );
		shared_array< liqFloat > curveFlatness(      new liqFloat[ totalVarying ] );
		//shared_array< liqFloat > curveParameter(     new liqFloat[ totalVarying ] );
		shared_array< liqFloat > curveColor(         new liqFloat[ totalVarying * 3 ] );
		shared_array< liqFloat > curveIncandescence( new liqFloat[ totalVarying * 3 ] );
		shared_array< liqFloat > curveOpacity(       new liqFloat[ totalVarying * 3 ] );

		LIQDEBUGPRINTF( "-> memory allocated for pfx data\n" );

		liqFloat* cvPtr( CVs.get() );
		liqFloat* twistPtr( curveTwist.get() );
		liqFloat* uniformWidthPtr( uniformCurveWidth.get() );
		liqFloat* curveIDPtr( curveID.get() );
		liqFloat* widthPtr( curveWidth.get() );
		liqFloat* flatnessPtr( curveFlatness.get() );
		//liqFloat* parameterPtr( curveParameter.get() );
		liqFloat* colorPtr( curveColor.get() );
		liqFloat* incandescencePtr( curveIncandescence.get() );
		liqFloat* opacityPtr( curveOpacity.get() );

		bool hasUniformWidth( false );
		bool hasWidth( false );
		bool hasFlatness( false );
		//bool hasParameter( false );
		bool hasColor( false );
		bool hasIncandescence( false );
		bool hasOpacity( false );

		for( unsigned lineOn( 0 ); lineOn < lines[ setOn ].length(); lineOn++ )
		{
			MRenderLine pfxLine( lines[ setOn ].renderLine( lineOn, &status ) );
			if( MS::kSuccess == status )
			{
				const MVectorArray& pfxVerts( pfxLine.getLine() );
				const MVectorArray& pfxTwist( pfxLine.getTwist() );
				const MDoubleArray& pfxWidth( pfxLine.getWidth() );
				const MDoubleArray& pfxFlatness( pfxLine.getFlatness() );
				//const MDoubleArray& pfxParameter( pfxLine.getParameter() );
				const MVectorArray& pfxColor( pfxLine.getColor() );
				const MVectorArray& pfxIncandescence( pfxLine.getIncandescence() );
				const MVectorArray& pfxTransparency( pfxLine.getTransparency() );

				unsigned pOn( 0 );
				for( ; pOn < pfxVerts.length(); pOn++ )
				{
					if( !camFacing[ setOn ] && pfxTwist.length() )
					{
						// MGlobal::displayInfo( MString( "camFacing: " ) + pfxTwist.length() );
						*twistPtr++ = pfxTwist[ pOn ].x;
						*twistPtr++ = pfxTwist[ pOn ].y;
						*twistPtr++ = pfxTwist[ pOn ].z;
					}
					if( !uniformWidth[ setOn ] && pfxWidth.length() )
					{
						hasWidth = true;
						*widthPtr++ = pfxWidth[ pOn ] * 0.75; // 0.75 is the scale factor that seems to match the viewport mesh best
					}
					if( hasCurveID )
					{
						if( !pOn )
							*curveIDPtr++ = rand();
					}
					if( uniformWidth[ setOn ] )
					{
						hasUniformWidth = true;
						// only first value
						if( !pOn )
							*uniformWidthPtr++ = pfxWidth[ pOn ] * 0.75; // 0.75 is the scale factor that seems to match the viewport mesh best
					}
					// output only if no shader is assigned
					if( pfxFlatness.length() && !hasShader )
					{
						hasFlatness = true;
						*flatnessPtr++ = pfxFlatness[ pOn ];
					}
					
					/*if( pfxParameter.length() )
					{
						hasParameter = true;
						*parameterPtr++ = pfxParameter[ pOn ];
					}
					*/
					// output only if no shader is assigned
					if( pfxColor.length() && !hasShader )
					{
						hasColor = true;
						*colorPtr++ = pfxColor[ pOn ].x;
						*colorPtr++ = pfxColor[ pOn ].y;
						*colorPtr++ = pfxColor[ pOn ].z;
					}
					// output only if no shader is assigned
					if( pfxIncandescence.length() && !hasShader )
					{
						hasIncandescence = true;
						*incandescencePtr++ = pfxIncandescence[ pOn ].x;
						*incandescencePtr++ = pfxIncandescence[ pOn ].y;
						*incandescencePtr++ = pfxIncandescence[ pOn ].z;
					}
					// output only if no shader is assigned
					if( pfxTransparency.length() && !hasShader )
					{
						hasOpacity = true;
						*opacityPtr++ = 1.0f - pfxTransparency[ pOn ].x;
						*opacityPtr++ = 1.0f - pfxTransparency[ pOn ].y;
						*opacityPtr++ = 1.0f - pfxTransparency[ pOn ].z;
					}

					if( pOn )
					{
						// some vertex in between start and end (pOn > 0)
						if( ( 1 == setOn ) && ( pOn ==  pfxVerts.length() - 1 ) )
						{
							// leaves need to be capped
							MVector compensate( pfxVerts[ pOn ] - pfxVerts[ pOn - 1 ] );
							compensate.normalize();
							const_cast< MVectorArray& >( pfxVerts )[ pOn ] += *( widthPtr - 1 ) * compensate;
						}

						*cvPtr++ = pfxVerts[ pOn ].x;
						*cvPtr++ = pfxVerts[ pOn ].y;
						*cvPtr++ = pfxVerts[ pOn ].z;

					}
					else
					{
						// start vertices (pOn == 0)

						if( 1 == setOn )
						{
							// leaves need to be capped
							MVector compensate( pfxVerts[ 1 ] - pfxVerts[ 0 ] );
							compensate.normalize();
							const_cast< MVectorArray& >( pfxVerts )[ 0 ] += -*( widthPtr - 1 ) * compensate;
						}
						const MVector tmpVertex( pfxVerts[ 0 ] - ( pfxVerts[ 1 ] - pfxVerts[ 0 ] ) );
						*cvPtr++ = tmpVertex.x;
						*cvPtr++ = tmpVertex.y;
						*cvPtr++ = tmpVertex.z;

						*cvPtr++ = pfxVerts[ 0 ].x;
						*cvPtr++ = pfxVerts[ 0 ].y;
						*cvPtr++ = pfxVerts[ 0 ].z;
					}
				}
				// end vertex
				// last vertex has already been pushed

				const MVector tmpVertex( pfxVerts[ pOn - 1 ] + ( pfxVerts[ pOn - 1 ] - pfxVerts[ pOn - 2 ] ) );
				*cvPtr++ = tmpVertex.x;
				*cvPtr++ = tmpVertex.y;
				*cvPtr++ = tmpVertex.z;

				// record number of vertices for this curve
				nverts[ setOn ].push_back( pfxVerts.length()+2 );
			}
		}

		LIQDEBUGPRINTF( "-> number of pfx curve CVs: %u\n", totalVertex );
		LIQDEBUGPRINTF( "-> number of pfx curves: %u\n", nverts[ setOn ].size() );
		LIQDEBUGPRINTF( "-> allocating memory for pfx data\n" );

		LIQDEBUGPRINTF( "-> flowerLine data created\n" );

		liqTokenPointer pointsPointerPair;
		pointsPointerPair.set( "P", rPoint, totalVertex );
		pointsPointerPair.setDetailType( rVertex );
		pointsPointerPair.setTokenFloats( CVs );
		pfxTokenPointerArrays[ setOn ].push_back( pointsPointerPair );

		if ( !camFacing[ setOn ] )
		{
			liqTokenPointer twistPointerPair;
			twistPointerPair.set( "N", rNormal, totalVarying );
			twistPointerPair.setDetailType( rVarying );
			twistPointerPair.setTokenFloats( curveTwist );
			pfxTokenPointerArrays[ setOn ].push_back( twistPointerPair );
		}
		if ( hasCurveID )
		{
			liqTokenPointer curveIDPointerPair;
			curveIDPointerPair.set( "curveID", rFloat, numLines );
			curveIDPointerPair.setDetailType( rUniform );
			curveIDPointerPair.setTokenFloats( curveID );
			pfxTokenPointerArrays[ setOn ].push_back( curveIDPointerPair );
		}
		if ( hasUniformWidth )
		{
			liqTokenPointer uniformWidthPointerPair;
			uniformWidthPointerPair.set( "width", rFloat, numLines );
			uniformWidthPointerPair.setDetailType( rUniform );
			uniformWidthPointerPair.setTokenFloats( uniformCurveWidth );
			pfxTokenPointerArrays[ setOn ].push_back( uniformWidthPointerPair );
		}
		if ( hasWidth )
		{
			liqTokenPointer widthPointerPair;
			widthPointerPair.set( "width", rFloat, totalVarying );
			widthPointerPair.setDetailType( rVarying );
			widthPointerPair.setTokenFloats( curveWidth );
			pfxTokenPointerArrays[ setOn ].push_back( widthPointerPair );
		}
		if ( hasColor )
		{
			liqTokenPointer colorPointerPair;
			colorPointerPair.set( "Cs", rColor, totalVarying );
			colorPointerPair.setDetailType( rVarying );
			colorPointerPair.setTokenFloats( curveColor );
			pfxTokenPointerArrays[ setOn ].push_back( colorPointerPair );
		}
		if ( hasOpacity )
		{
			liqTokenPointer opacityPointerPair;
			opacityPointerPair.set( "Os", rColor, totalVarying );
			opacityPointerPair.setDetailType( rVarying );
			opacityPointerPair.setTokenFloats( curveOpacity );
			pfxTokenPointerArrays[ setOn ].push_back( opacityPointerPair );
		}

		if ( hasFlatness )
		{
			liqTokenPointer flatnessPointerPair;
			flatnessPointerPair.set( "pfxflatness", rFloat, totalVarying );
			flatnessPointerPair.setDetailType( rVarying );
			flatnessPointerPair.setTokenFloats( curveFlatness );
			pfxTokenPointerArrays[ setOn ].push_back( flatnessPointerPair );
		}
		
/*		if ( hasParameter )
		{
			liqTokenPointer parameterPointerPair;
			parameterPointerPair.set( "t", rFloat, totalVarying );
			parameterPointerPair.setDetailType( rVarying );
			parameterPointerPair.setTokenFloats( curveParameter );
			pfxTokenPointerArrays[ setOn ].push_back( parameterPointerPair );
		}
*/		
		if ( hasIncandescence )
		{
			liqTokenPointer incandescencePointerPair;
			incandescencePointerPair.set( "pfxincandescence", rColor, totalVarying );
			incandescencePointerPair.setDetailType( rVarying );
			incandescencePointerPair.setTokenFloats( curveIncandescence );
			pfxTokenPointerArrays[ setOn ].push_back( incandescencePointerPair );
		}

		liqTokenPointer elementPointerPair;
		elementPointerPair.set( "pfxelement", rFloat );
		elementPointerPair.setDetailType( rConstant );
		elementPointerPair.setTokenFloat( 0, setOn );
		pfxTokenPointerArrays[ setOn ].push_back( elementPointerPair );
	}//if( totalVarying ) 
	else
	{
		hasFeature[ setOn ] = false;
	}
	
	// free memory for lines arrays, are not freed by pfx destructor - Alf
	lines[0].deleteArray();
	lines[1].deleteArray();
	lines[2].deleteArray();
}

void liqRibPfxData::write(const MString &ribFileName, const structJob &currentJob, const bool bReference)
{
	CM_TRACE_FUNC("liqRibPfxData::write("<<ribFileName.asChar()<<",job="<<currentJob.name.asChar()<<","<<bReference<<")");

	liquid::RendererMgr::getInstancePtr()->
		getRenderer()->write(this, ribFileName, currentJob, bReference);

// 	assert(liqglo.m_ribFileOpen&&"liqRibPfxData.cpp");
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
/**
 *  Write the RIB for this paint effect.
 */
 //void liqRibPfxData::_write(const structJob &currentJob)
 //{
 //	CM_TRACE_FUNC("liqRibPfxData::write(job="<<currentJob.name<<")");
 //
 //	LIQDEBUGPRINTF( "-> writing painteffects curves\n" );
 //
 //	unsigned setOn( 0 );
 //	if( pfxtype == MRT_PfxLeaf )
 //		setOn = 1;
 //	if( pfxtype == MRT_PfxPetal )
 //		setOn = 2;
 //
 //	if( hasFeature[ setOn ] )
 //	{
 //		unsigned numTokens( pfxTokenPointerArrays[ setOn ].size() );
 //		scoped_array< RtToken > tokenArray( new RtToken[ numTokens ] );
 //		scoped_array< RtPointer > pointerArray( new RtPointer[ numTokens ] );
 //		assignTokenArraysV( pfxTokenPointerArrays[ setOn ], tokenArray.get(), pointerArray.get() );
 //
 //		RiCurvesV( "cubic", nverts[ setOn ].size(), const_cast< liqInt* >( &nverts[ setOn ][ 0 ] ), "nonperiodic", numTokens, tokenArray.get(), pointerArray.get() );
 //	}
 //}


unsigned liqRibPfxData::granularity() const
{
	CM_TRACE_FUNC("liqRibPfxData::granularity()");

	unsigned i( 0 );
	unsigned setOn( 0 );
	if( pfxtype == MRT_PfxLeaf )
		setOn = 1;
	if( pfxtype == MRT_PfxPetal )
		setOn = 2;
	if( hasFeature[ setOn ] )
		++i;
	return i;
}


//bool liqRibPfxData::writeNextGrain()
//{
//	CM_TRACE_FUNC("liqRibPfxData::writeNextGrain()");
//
//	LIQDEBUGPRINTF( "-> writing painteffects curves\n" );
//
//	if( hasFeature[ grain ] )
//	{
//		unsigned numTokens( pfxTokenPointerArrays[ grain ].size() );
//		scoped_array< RtToken > tokenArray( new RtToken[ numTokens ] );
//		scoped_array< RtPointer > pointerArray( new RtPointer[ numTokens ] );
//		assignTokenArraysV( pfxTokenPointerArrays[ grain ], tokenArray.get(), pointerArray.get() );
//
//		RiCurvesV( "cubic", nverts[ grain ].size(), const_cast< liqInt* >( &nverts[ grain ][ 0 ] ), "nonperiodic", numTokens, tokenArray.get(), pointerArray.get() );
//	}
//
//	if( 2 < ++grain )
//	{
//		grain = 0;
//		return false;
//	}
//	else
//		return true;
//}


/** Compare this curve to the other for the purpose of determining
 *  if it is animated.
 */
bool liqRibPfxData::compare( const liqRibData & otherObj ) const
{
	CM_TRACE_FUNC("liqRibPfxData::compare("<<otherObj.getFullPathName().asChar()<<")");

	LIQDEBUGPRINTF( "-> comparing painteffects curves\n");

	if ( MRT_PfxTube != otherObj.type() && MRT_PfxLeaf != otherObj.type() && MRT_PfxPetal != otherObj.type()  )
		return false;

	const liqRibPfxData & other = ( liqRibPfxData& )otherObj;

	unsigned setOn( 0 );
	if( pfxtype == MRT_PfxLeaf )
		setOn = 1;
	if( pfxtype == MRT_PfxPetal )
		setOn = 2;
	if ( ( nverts[setOn][0] != other.nverts[setOn][0] ) ||
		( nverts[setOn].size() != other.nverts[setOn].size() ) )
		return false;

	// Check CVs
	for ( unsigned i( 0 ); i < nverts[0][0] * 3; i++ )
	{
		if ( !equiv( CVs[i], other.CVs[i] ) )
			return false;
	}
	return true;
}


/** Return the geometry type.
 */
ObjectType liqRibPfxData::type() const
{
	CM_TRACE_FUNC("liqRibPfxData::type()");
	LIQDEBUGPRINTF( "-> returning painteffects curves type\n" );
	return pfxtype;
}
