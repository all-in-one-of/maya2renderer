#include "rm_renderer.h"
#include <liqRibParticleData.h>
#include "rm_helper.h"
#include <liqGlobalVariable.h>
#include <common/prerequest_maya.h>
#include <liqGlobalHelpers.h>

#define DELIGHT

namespace renderman
{
	static void _write(liqRibParticleData* pData, const structJob &currentJob);
	//
	void Renderer::write(
		liqRibParticleData* pData,
		const MString &ribFileName,
		const structJob &currentJob,
		const bool bReference)
	{
		CM_TRACE_FUNC("Renderer::write("<<pData->getFullPathName()<<","<<ribFileName.asChar()<<","<<currentJob.name.asChar()<<",ref="<<bReference<<")");

		assert(liqglo.m_ribFileOpen &&"rm_writeParticleData.cpp");

		if(liqglo.m_writeDataIntoMainRibFile)
		{
			if( bReference ){
				_write(pData, currentJob);
			}else{
				//do nothing
			}
		}else{
	 		if( !bReference ){//write data at first time
				if( currentJob.pass != rpShadowMap ){
					assert(pData->getRibFileFullPath().length()==0&&"rm_writeClipPlaneData.cpp");
				}
				pData->setRibFileFullPath(ribFileName);

	 			renderman::Helper o;
	 			o.RiBeginRef(pData->getRibFileFullPath().asChar());
	 			_write(pData, currentJob);
	 			o.RiEndRef();
		 
	 		}else{
	 			//write the reference
	 			assert(pData->getRibFileFullPath() == ribFileName);
	 			RiReadArchive( const_cast< RtToken >( pData->getRibFileFullPath().asChar() ), NULL, RI_NULL );
	 		}
		}
	}
	//
	static void _write(liqRibParticleData* pData, const structJob &currentJob__)
	{
		CM_TRACE_FUNC("rm_writeParticleData.cpp::write("<<pData->getFullPathName()<<","<<currentJob__.name.asChar()<<",...)");

		LIQDEBUGPRINTF( "-> writing particles\n");

#ifdef DEBUG
		RiArchiveRecord( RI_COMMENT, "Number of Particles: %d", pData->m_numValidParticles );
		RiArchiveRecord( RI_COMMENT, "Number of Discarded Particles: %d", pData->m_numParticles - pData->m_numValidParticles );
#endif

		unsigned numTokens( pData->tokenPointerArray.size() );
		boost::scoped_array< RtToken > tokenArray( new RtToken[ numTokens ] );
		boost::scoped_array< RtPointer > pointerArray( new RtPointer[ numTokens ] );
		assignTokenArraysV( pData->tokenPointerArray, tokenArray.get(), pointerArray.get() );

		switch( pData->particleType ) 
		{
		case liqRibParticleData::MPTBlobbies: 
			{
				// Build an array that can be given to RiBlobby
				std::vector< RtString > stringArray;
				for( int i(0); i < pData->m_stringArray.size(); i++ ) 
				{
					stringArray.push_back( const_cast<char *>( pData->m_stringArray[i].c_str()) );
				}
				RiBlobbyV( pData->m_numValidParticles,
					pData->m_codeArray.size(), const_cast< RtInt* >( &pData->m_codeArray[0] ),
					pData->m_floatArray.size(), const_cast< RtFloat* >( &pData->m_floatArray[0] ),
					stringArray.size(), const_cast< RtString* >( &stringArray[0] ),
					numTokens,
					tokenArray.get(),
					const_cast< RtPointer* >( pointerArray.get() ) );
				pData->grain = 0;
			}
			break;

		case liqRibParticleData::MPTMultiPoint:
		case liqRibParticleData::MPTPoints:
			RiArchiveRecord( RI_COMMENT, "normal has to be reversed to show the MultiPoint/Points particles. //  [10/9/2012 yaoyansi]" );
			RiReverseOrientation();
#ifdef DELIGHT
		case liqRibParticleData::MPTSpheres:
		case liqRibParticleData::MPTSprites:
#endif
			{
				RiPointsV( pData->m_numValidParticles * pData->m_multiCount, numTokens, tokenArray.get(), pointerArray.get() );
			}
			break;

		case liqRibParticleData::MPTMultiStreak:
		case liqRibParticleData::MPTStreak: 
			{
				unsigned nStreaks( pData->m_numValidParticles * pData->m_multiCount / 2 );
				std::vector< RtInt > verts( nStreaks, 2 );
				// Alternatively:
				//   scoped_array< RtInt >verts( new RtInt[ nStreaks ] );
				//   fill( verts.get(), verts.get() + nStreaks, ( RtInt )2 );
				// Both ways are way faster than the frickin for() lop that was here before -- Moritz

				RiCurvesV( "linear", nStreaks, &verts[ 0 ], "nonperiodic", numTokens, tokenArray.get(), pointerArray.get() );
			}
			break;
#ifndef DELIGHT
		case liqRibParticleData::MPTSpheres: 
			{
				int posAttr  = -1,
					radAttr  = -1,
					colAttr  = -1,
					opacAttr = -1;

				for ( unsigned i = 0; i < pData->tokenPointerArray.size(); i++ )
				{
					const std::string tokenName( pData->tokenPointerArray[i].getTokenName() );
					if ( "P" == tokenName )
					{
						posAttr = i;
					}
					else if ( "radius" == tokenName )
					{
						radAttr = i;
					}
					else if ( "Cs" == tokenName )
					{
						colAttr = i;
					}
					else if ( "Os" == tokenName )
					{
						opacAttr = i;
					}
				}

				for ( unsigned i = 0; i < pData->m_numValidParticles; i++)
				{
					RiAttributeBegin();
					if ( colAttr != -1 )
					{
						RiColor( &((RtFloat*)pointerArray[colAttr])[i*3] );
					}
					if ( opacAttr != -1 )
					{
						RiOpacity( &((RtFloat*)pointerArray[opacAttr])[i*3] );
					}
					RiTransformBegin();
					RiTranslate(((RtFloat*)pointerArray[posAttr])[i*3+0],
						((RtFloat*)pointerArray[posAttr])[i*3+1],
						((RtFloat*)pointerArray[posAttr])[i*3+2]);

					RtFloat radius = ((RtFloat*)pointerArray[radAttr])[i];
					RiSphere(radius, -radius, radius, 360, RI_NULL);
					RiTransformEnd();
					RiAttributeEnd();
				}
			}
			break;

		case liqRibParticleData::MPTSprites: 
			{
				int posAttr   = -1,
					numAttr    = -1,
					twistAttr  = -1,
					scaleXAttr = -1,
					scaleYAttr = -1,
					colAttr    = -1,
					opacAttr   = -1;

				for ( unsigned i( 0 ); i < pData->tokenPointerArray.size(); i++ )
				{
					const std::string tokenName( pData->tokenPointerArray[i].getTokenName() );
					if ( "P" == tokenName )
					{
						posAttr = i;
					}
					else if ( "spriteNum" == tokenName )
					{
						numAttr = i;
					}
					else if ( "spriteTwist" == tokenName )
					{
						twistAttr = i;
					}
					else if ( "spriteScaleX" == tokenName )
					{
						scaleXAttr = i;
					}
					else if ( "spriteScaleY" == tokenName )
					{
						scaleYAttr = i;
					}
					else if ( "Cs" == tokenName )
					{
						colAttr = i;
					}
					else if ( "Os" == tokenName )
					{
						opacAttr = i;
					}
				}

				MVector camUp( 0, 1, 0 );
				MVector camRight( 1, 0, 0 );
				MVector camEye( 0, 0, 1 );

				camUp    *= currentJob__.camera[0].mat.inverse();
				camRight *= currentJob__.camera[0].mat.inverse();
				camEye   *= currentJob__.camera[0].mat.inverse();

				for( unsigned ui( 0 ); ui < pData->m_numValidParticles; ui++ ) 
				{
					MVector up( camUp );
					MVector right( camRight );

					float spriteRadiusX( 0.5 );
					float spriteRadiusY( 0.5 );
					RiAttributeBegin();

					RiArchiveRecord( RI_COMMENT, "normal has to be reversed to show the Sprite particles. //  [10/9/2012 yaoyansi]" );
					RiReverseOrientation();

					if ( -1 != colAttr ) 
						RiColor( &( ( RtFloat* )pointerArray[ colAttr ] )[ ui * 3 ] );

					if ( -1 != opacAttr ) 
						RiOpacity( &( ( RtFloat* )pointerArray[ opacAttr ] )[ ui * 3 ] );

					if ( -1 != twistAttr ) 
					{
						float twist( -( ( RtFloat* )pointerArray[ twistAttr ] )[ ui ] * M_PI / 180 );
						MQuaternion twistQ( twist, camEye );
						right = camRight.rotateBy( twistQ );
						up    = camUp.rotateBy( twistQ );
					}

					if ( scaleXAttr != -1 ) 
						spriteRadiusX *= ( ( RtFloat* )pointerArray[ scaleXAttr ] )[ ui ];

					if ( scaleYAttr != -1 ) 
						spriteRadiusY *= ( ( RtFloat* )pointerArray[ scaleYAttr ] )[ ui ];

					if ( posAttr != -1 ) 
					{
						float *P( &( ( RtFloat* ) pointerArray[ posAttr ] )[ ui * 3 ] );
						float spriteNumPP = 0;
						if ( numAttr != -1 ) 
							spriteNumPP = ( ( RtFloat* )pointerArray[ numAttr ] )[ ui ];

						float x0 = P[ 0 ] - spriteRadiusX * right[ 0 ] + spriteRadiusY * up[ 0 ];
						float y0 = P[ 1 ] - spriteRadiusX * right[ 1 ] + spriteRadiusY * up[ 1 ];
						float z0 = P[ 2 ] - spriteRadiusX * right[ 2 ] + spriteRadiusY * up[ 2 ];
						float x1 = P[ 0 ] + spriteRadiusX * right[ 0 ] + spriteRadiusY * up[ 0 ];
						float y1 = P[ 1 ] + spriteRadiusX * right[ 1 ] + spriteRadiusY * up[ 1 ];
						float z1 = P[ 2 ] + spriteRadiusX * right[ 2 ] + spriteRadiusY * up[ 2 ];
						float x2 = P[ 0 ] - spriteRadiusX * right[ 0 ] - spriteRadiusY * up[ 0 ];
						float y2 = P[ 1 ] - spriteRadiusX * right[ 1 ] - spriteRadiusY * up[ 1 ];
						float z2 = P[ 2 ] - spriteRadiusX * right[ 2 ] - spriteRadiusY * up[ 2 ];
						float x3 = P[ 0 ] + spriteRadiusX * right[ 0 ] - spriteRadiusY * up[ 0 ];
						float y3 = P[ 1 ] + spriteRadiusX * right[ 1 ] - spriteRadiusY * up[ 1 ];
						float z3 = P[ 2 ] + spriteRadiusX * right[ 2 ] - spriteRadiusY * up[ 2 ];

						float patch[ 12 ] = { x0, y0, z0,
							x1, y1, z1,
							x2, y2, z2,
							x3, y3, z3 };
						// !!! if not GENERIC_RIBLIB use RiPatch( "bilinear", "P", &patch, "float spriteNum", &spriteNum, RI_NULL );                                  
						// RiPatch( "bilinear", "P", &patch, "float spriteNum", (RtFloat*)&spriteNumPP, RI_NULL );
						// Patch "bilinear"  "P" [0.446265 0.316269 -0.647637 1.27725 0.316269 -1.20393 0.615752 -0.636188 -0.39446 1.44674 -0.636188 -0.950756 ]  "float spriteNum" [2 0 0 0 ]
						RiArchiveRecord( RI_VERBATIM, "Patch \"bilinear\" \"P\" [%f %f %f %f %f %f %f %f %f %f %f %f] \"float spriteNum\" [%f]", 
							x0, y0, z0,x1, y1, z1, x2, y2, z2,x3, y3, z3,
							spriteNumPP ); 
					} 
					else {
						RiIdentity();
					}
					RiAttributeEnd();
				}//for
			}
			break;

#endif // #ifndef DELIGHT


		case liqRibParticleData::MPTCloudy:
			{
				int posAttr  = -1,
					radAttr  = -1,
					colAttr  = -1,
					opacAttr = -1,
					rotAttr  = -1;

				for ( unsigned i = 0; i < pData->tokenPointerArray.size(); i++ )
				{
					const std::string tokenName( pData->tokenPointerArray[i].getTokenName() );
					if ( "P" == tokenName )
					{
						posAttr = i;
					}
					else if ( "radius" == tokenName )
					{
						radAttr = i;
					}
					else if ( "Cs" == tokenName )
					{
						colAttr = i;
					}
					else if ( "Os" == tokenName )
					{
						opacAttr = i;
					}
					else if ( "rotation" == tokenName )
					{
						rotAttr = i;
					}
				}
				// Build an array that can be given to RiBlobby
				std::vector< RtString > stringArray;
				for( unsigned int i(0); i < pData->m_stringArray.size(); i++ ) {
					stringArray.push_back( const_cast<char *>( pData->m_stringArray[i].c_str()) );
				}
				if(stringArray.size()==0)//added by yaoyansi, or it leads a crash on windows
					stringArray.push_back( "" );

				boost::scoped_array< RtToken > ithTokenArray( new RtToken[ numTokens ] );
				boost::scoped_array< RtPointer > ithPointerArray( new RtPointer[ numTokens ] );

				for ( unsigned i = 0; i < pData->m_numValidParticles; i++)
				{
					assignIthTokenArraysV( pData->tokenPointerArray, ithTokenArray.get(), ithPointerArray.get(), i );
					RiAttributeBegin();
					if ( colAttr != -1 )
					{
						RiColor( &((RtFloat*)pointerArray[colAttr])[i*3] );
					}
					if ( opacAttr != -1 )
					{
						RiOpacity( &((RtFloat*)pointerArray[opacAttr])[i*3] );
					}
					RiTransformBegin();
					RiTranslate(((RtFloat*)pointerArray[posAttr])[i*3+0],
						((RtFloat*)pointerArray[posAttr])[i*3+1],
						((RtFloat*)pointerArray[posAttr])[i*3+2]);

					if ( rotAttr != -1 )
					{
						RiRotate( (( RtFloat *) pointerArray[rotAttr])[i*3] * 360.0, 1.0, 0.0, 0.0 );
						RiRotate( (( RtFloat *) pointerArray[rotAttr])[i*3+1] * 360.0, 0.0, 1.0, 0.0 );
						RiRotate( (( RtFloat *) pointerArray[rotAttr])[i*3+2] * 360.0, 0.0, 0.0, 1.0 );
					}

					RtFloat radius = ((RtFloat*)pointerArray[radAttr])[i];
					RiScale( radius, radius, radius );
					//RiSphere(radius, -radius, radius, 360, RI_NULL);
					float dummy[] = { 0.0, 0.0, 0.0 }; // Worst case : three floats are needed
					RiBlobbyV( 1,
						pData->m_codeArray.size(), const_cast< RtInt* >( &pData->m_codeArray[0] ),
						pData->m_floatArray.size(), const_cast< RtFloat* >( &pData->m_floatArray[0] ),
						stringArray.size(), const_cast< RtString* >( &stringArray[0] ),
						numTokens, ithTokenArray.get(), ithPointerArray.get() );
//						"vertex color incandescence", (RtPointer *)( dummy ),
//						"vertex color Cs", (RtPointer *)( dummy ),
//						"vertex float selfshadow", (RtPointer *)( dummy ),
//						RI_NULL );
					RiTransformEnd();
					RiAttributeEnd();

				}
				break;
			}
		case liqRibParticleData::MPTNumeric:
			RiArchiveRecord( RI_COMMENT, "Numeric Particles are not supported" );
			break;
		case liqRibParticleData::MPTTube:
			RiArchiveRecord( RI_COMMENT, "Tube Particles are not supported" );
			break;

			break;
		}
	}
}//namespace renderman
