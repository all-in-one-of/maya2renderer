#include "rm_heroRibWriterMgr.h"

#include <string>
#include "../common/prerequest_maya.h"

#include <liqGlobalVariable.h>
#include <liqRibTranslator.h>
#include "../shadergraph/shadermgr.h"
#include "ri_interface.h"

namespace renderman
{
tHeroRibWriterMgr::tHeroRibWriterMgr()
{

}
//
tHeroRibWriterMgr::~tHeroRibWriterMgr()
{

}
//
void tHeroRibWriterMgr::ribPrologue_samples(liqFloat xsamples, liqFloat ysamples)
{
	CM_TRACE_FUNC("tHeroRibWriterMgr::ribPrologue_samples("<<xsamples<<","<<ysamples<<")");

	RiPixelSamples( xsamples, ysamples );
}
//
void tHeroRibWriterMgr::ribPrologue_shadingrate(liqFloat size)
{
	CM_TRACE_FUNC("tHeroRibWriterMgr::ribPrologue_shadingrate("<<size<<")");

	RiShadingRate( size );
}
//
void tHeroRibWriterMgr::ribPrologue_filter(
	liquidlong m_rFilter,
	liqFloat m_rFilterX, liqFloat m_rFilterY)
{
	CM_TRACE_FUNC("tHeroRibWriterMgr::ribPrologue_filter("<<m_rFilter<<","<<m_rFilterX<<","<<m_rFilterY<<")");

		switch (m_rFilter) 
		{
		case pfBoxFilter:
			RiPixelFilter( RiBoxFilter, m_rFilterX, m_rFilterY );
			break;
		case pfTriangleFilter:
			RiPixelFilter( RiTriangleFilter, m_rFilterX, m_rFilterY );
			break;
		case pfCatmullRomFilter:
			RiPixelFilter( RiCatmullRomFilter, m_rFilterX, m_rFilterY );
			break;
		case pfGaussianFilter:
			RiPixelFilter( RiGaussianFilter, m_rFilterX, m_rFilterY );
			break;
		case pfSincFilter:
			RiPixelFilter( RiSincFilter, m_rFilterX, m_rFilterY );
			break;
#if defined ( DELIGHT ) || defined ( PRMAN ) || defined ( GENERIC_RIBLIB )
		case pfBlackmanHarrisFilter:
			RiArchiveRecord( RI_VERBATIM, "PixelFilter \"blackman-harris\" %g %g\n", m_rFilterX, m_rFilterY);
			break;
		case pfMitchellFilter:
			RiArchiveRecord( RI_VERBATIM, "PixelFilter \"mitchell\" %g %g\n", m_rFilterX, m_rFilterY);
			break;
		case pfSepCatmullRomFilter:
			RiArchiveRecord( RI_VERBATIM, "PixelFilter \"separable-catmull-rom\" %g %g\n", m_rFilterX, m_rFilterY);
			break;
		case pfBesselFilter:
			RiArchiveRecord( RI_VERBATIM, "PixelFilter \"bessel\" %g %g\n", m_rFilterX, m_rFilterY);
			break;
#endif
#if defined ( PRMAN ) || defined ( GENERIC_RIBLIB )
		case pfLanczosFilter:
			RiArchiveRecord( RI_VERBATIM, "PixelFilter \"lanczos\" %g %g\n", m_rFilterX, m_rFilterY);
			break;
		case pfDiskFilter:
			RiArchiveRecord( RI_VERBATIM, "PixelFilter \"disk\" %g %g\n", m_rFilterX, m_rFilterY);
			break;
#endif
		default:
			RiArchiveRecord( RI_COMMENT, "Unknown Pixel Filter selected" );
			break;
		}
		//RiPixelFilter( filterfunc, xwidth, ywidth );
}
//
void tHeroRibWriterMgr::ribPrologue_hider(HiderType hidertype)
{
	CM_TRACE_FUNC("tHeroRibWriterMgr::ribPrologue_hider("<<hidertype<<")");
				
	//[refactor 8] 
	liqString hiderName;
	switch( hidertype ) 
	{
	case htPhoton:
		hiderName = "photon";
		break;
	case htRaytrace:
		hiderName = "raytrace";
		break;
	case htOpenGL:
		hiderName = "OpenGL";
		break;
	case htZbuffer:
		hiderName = "zbuffer";
		break;
	case htDepthMask:
		hiderName = "depthmask";
		break;
	case htHidden:
	default:
		hiderName = "hidden";
	}
	MString hiderOptions = getHiderOptions( liqglo.liquidRenderer.renderName, hiderName );
	RiArchiveRecord( RI_VERBATIM, "Hider \"%s\" %s\n", hiderName, ( char* )hiderOptions.asChar() );
	//[refactor 8]
}
//
void tHeroRibWriterMgr::ribPrologue_pass(liqString pass)
{
	CM_TRACE_FUNC("tHeroRibWriterMgr::ribPrologue_pass("<<pass<<")");

	RiOption( "user", "string pass", ( liqPointer )&pass, RI_NULL );	
}
//
void tHeroRibWriterMgr::framePrologue_display(const structJob &currentJob)
{
	CM_TRACE_FUNC("tHeroRibWriterMgr::framePrologue_display("<<currentJob.name.asChar()<<")");
	
	//refactor 14-1 begin from liqRibTranslator.framePrologue()
	if( false == false && liqglo.liqglo_rotateCamera  == true )
	{
		// philippe : Rotated Camera Case
		RiFormat( currentJob.height, currentJob.width, currentJob.aspectRatio );
	}else{
		RiFormat( currentJob.width, currentJob.height, currentJob.aspectRatio );
	}
	//refactor 14-1 end

			//refactor 14 begin from liqRibTranslator::framePrologue()
			// Smooth Shading
			RiShadingInterpolation( "smooth" );
			// Quantization
			// overriden to floats when in rendering to Maya's renderView
			if( !liqglo.m_renderView && liqglo.quantValue != 0 )
			{
				int whiteValue = (int) pow( 2.0, liqglo.quantValue ) - 1;
				RiQuantize( RI_RGBA, whiteValue, 0, whiteValue, 0.5 );
			}
			else
			{
				RiQuantize( RI_RGBA, 0, 0, 0, 0 );
			}
			if( liqglo.m_rgain != 1.0 || liqglo.m_rgamma != 1.0 )
			{
				RiExposure( liqglo.m_rgain, liqglo.m_rgamma );
			}
			//refactor 14 end
			LIQDEBUGPRINTF( "-> Setting Display Options\n" );
			//refactor 18 begin
			if( ( liqglo.m_cropX1 != 0.0 ) || ( liqglo.m_cropY1 != 0.0 ) || ( liqglo.m_cropX2 != 1.0 ) || ( liqglo.m_cropY2 != 1.0 ) ) 
			{
				// philippe : handle the rotated camera case
				if( liqglo.liqglo_rotateCamera == true ) 
					RiCropWindow( liqglo.m_cropY2, liqglo.m_cropY1, 1 - liqglo.m_cropX1, 1 - liqglo.m_cropX2 );
				else 
					RiCropWindow( liqglo.m_cropX1, liqglo.m_cropX2, liqglo.m_cropY1, liqglo.m_cropY2 );
			}

			if ( !currentJob.isStereoPass ) 
				liqRibTranslator::getInstancePtr()->exportJobCamera( currentJob, currentJob.camera );
			else
			{
				// export right camera
				RiTransformBegin();
				liqRibTranslator::getInstancePtr()->exportJobCamera( currentJob, currentJob.rightCamera );
				RiArchiveRecord( RI_VERBATIM, "Camera \"%s\"\n", "right" );//RiCameraV( "right", 0, (char**)RI_NULL, (void**)RI_NULL );
				RiTransformEnd();
				// export left camera
				liqRibTranslator::getInstancePtr()->exportJobCamera( currentJob, currentJob.leftCamera );
			}

			// display channels
			if( liqglo.liquidRenderer.supports_DISPLAY_CHANNELS ) 
			{
				RiArchiveRecord( RI_COMMENT, "Display Channels:" );
				// philippe -> to do : move this to higher scope ?
				//MStringArray channeltype;
				//channeltype.append( "float" );
				//channeltype.append( "color" );
				//channeltype.append( "point" );
				//channeltype.append( "normal" );
				//channeltype.append( "vector" );
				std::string  channeltype[] = { "float", "color", "point", "normal", "vector" };

				std::vector<StructChannel>::iterator m_channels_iterator;
				m_channels_iterator = liqglo.m_channels.begin();
				
				bool isCiDeclared = 0;
				bool isADeclared = 0;
				while ( m_channels_iterator != liqglo.m_channels.end() ) 
				{
					int       numTokens = 0;
					liqToken   tokens[5];
					liqPointer values[5];

					std::stringstream channel;
					char* filter;
					int quantize[4];
					float filterwidth[2];
					float dither;

// #if defined( GENERIC_RIBLIB )          
					std::stringstream quantize_str;
					std::stringstream dither_str;
					std::stringstream filter_str;
// #endif
					channel << channeltype[m_channels_iterator->type];
					if ( m_channels_iterator->arraySize > 0 ) 
						channel << "[" << m_channels_iterator->arraySize << "]";
					channel << " " << m_channels_iterator->name.asChar();

					if( m_channels_iterator->quantize ) 
					{
						int max = ( int )pow( 2., m_channels_iterator->bitDepth ) - 1;
						dither = m_channels_iterator->dither;
						quantize[0] = quantize[2] = 0;
						quantize[1] = quantize[3] = max;
						tokens[ numTokens ] = "int[4] quantize";
						values[ numTokens ] = (liqPointer)quantize;
						numTokens++;
#if !defined( GENERIC_RIBLIB )               
					}
#else
						quantize_str << "\"int[4] quantize\" [ 0 " << max << " 0 " << max << " ]";
						dither_str << "\"float dither\" [" << dither << "]";
					}
					else
					{
						quantize_str << "\"int[4] quantize\" [ 0 0 0 0 ]"; 
						dither_str.clear();
					}
#endif

					if( m_channels_iterator->filter ) 
					{
						MString pixFilter( liqglo.liquidRenderer.pixelFilterNames[ m_channels_iterator->pixelFilter ] );
						filter = ( char* )pixFilter.asChar();

						liquidMessage2(messageInfo, ">>  m_channels_iterator->pixelFilter = %s\n", liqglo.liquidRenderer.pixelFilterNames[ m_channels_iterator->pixelFilter ].asChar() );
						liquidMessage2(messageInfo, ">>  pixFilter.asChar() = %s\n", pixFilter.asChar() );

						tokens[ numTokens ] = "string filter";
						values[ numTokens ] = ( liqPointer )&filter;
						numTokens++;

						filterwidth[0] = m_channels_iterator->pixelFilterX;
						filterwidth[1] = m_channels_iterator->pixelFilterY;
						tokens[ numTokens ] = "float filterwidth[2]";
						values[ numTokens ] = ( liqPointer )filterwidth;
						numTokens++;
#if !defined( GENERIC_RIBLIB )               
					}
#else
						filter_str << "\"string filter\" [\"" << pixFilter.asChar() << "\"] \"float filterwidth[2]\" [" << filterwidth[0] << " " << filterwidth[1] << "]";
					} 
					else
						filter_str.clear();
#endif

#if defined( DELIGHT )  || defined( PRMAN ) || defined( PIXIE )
					//if( liquidRenderer.renderName == MString("PRMan") )
					RiDisplayChannelV( ( liqToken )channel.str().c_str(), numTokens, tokens, values );
					if( channel.str() == "color Ci" )
					{
						isCiDeclared = 1;
					}
					else if( channel.str() == "float a" )
					{
						isADeclared = 1;
					}
#else
					// || defined( GENERIC_RIBLIB )
					RiArchiveRecord( RI_VERBATIM, "DisplayChannel \"%s\" %s %s %s",
						const_cast< char* >( channel.str().c_str() ),  quantize_str.asChar(), dither_str.asChar(), filter_str.asChar() );
#endif
					m_channels_iterator++;
				}//while ( m_channels_iterator != m_channels.end() ) 
#if defined ( DELIGHT ) || defined ( GENERIC_RIBLIB ) || defined ( PRMAN ) || defined (PIXIE)
				if ( liqRibTranslator::getInstancePtr()->m_isStereoCamera && currentJob.pass != rpShadowMap )
				{
					liqToken   *emptyTokens = NULL;
					liqPointer *emptyValues = NULL;
					if ( !isCiDeclared ) 
						RiDisplayChannelV( ( liqToken )"color Ci", 0, emptyTokens, emptyValues );
					if ( !isADeclared ) 
						RiDisplayChannelV( ( liqToken )"float a", 0, emptyTokens, emptyValues );
				}
#endif
			}//if( liqglo.liquidRenderer.supports_DISPLAY_CHANNELS ) 
			
			
			// output display drivers
			RiArchiveRecord( RI_COMMENT, "Display Drivers:" );
			RiArchiveRecord( RI_COMMENT, "Display 6");

			std::vector<StructDisplay>::iterator m_displays_iterator;
			m_displays_iterator = liqglo.m_displays.begin();

			// create right display for stereo
			if( liqRibTranslator::getInstancePtr()->m_isStereoCamera )
			{
				StructDisplay rightStereoAov;
				rightStereoAov.name = m_displays_iterator->name;
				rightStereoAov.name = m_displays_iterator->name;
				rightStereoAov.mode = "Ci,a";
				if( m_displays_iterator->type == "it" )
				{
					rightStereoAov.type = "tiff"; // should be optionnal ?....
				}
				else
				{
					rightStereoAov.type = m_displays_iterator->type;
				}
				rightStereoAov.enabled = m_displays_iterator->enabled;
				rightStereoAov.doQuantize = m_displays_iterator->doQuantize;
				rightStereoAov.bitDepth = m_displays_iterator->bitDepth;
				rightStereoAov.dither = m_displays_iterator->dither;
				rightStereoAov.doFilter = m_displays_iterator->doFilter;
				rightStereoAov.filter = m_displays_iterator->filter;
				rightStereoAov.filterX = m_displays_iterator->filterX;
				rightStereoAov.filterY = m_displays_iterator->filterY;
				rightStereoAov.xtraParams = m_displays_iterator->xtraParams;
				rightStereoAov.xtraParams.num = rightStereoAov.xtraParams.num + 1;
				rightStereoAov.xtraParams.names.append("camera");
				rightStereoAov.xtraParams.data.append("right");
				rightStereoAov.xtraParams.type.append(0);  // string
				m_displays_iterator ++;
				liqglo.m_displays.insert( m_displays_iterator, rightStereoAov );
				// replace iter at beginning
				m_displays_iterator = liqglo.m_displays.begin();
			}

			std::string paramType[] = { "string ", "float ",  "int " };
			
			//refactor 19
			while ( m_displays_iterator != liqglo.m_displays.end() ) 
			{
				std::stringstream parameterString;
				std::stringstream imageName;
				std::string imageType;
				std::string imageMode;
				std::stringstream quantizer;
				std::stringstream dither;
				std::stringstream filter;
				// check if additionnal displays are enabled
				// if not, call it off after the 1st iteration.
				if( liqglo.m_ignoreAOVDisplays && m_displays_iterator > liqglo.m_displays.begin() ) 
					break;

				// This is the override for the primary DD
				// when you render to maya's renderview.
				if( m_displays_iterator == liqglo.m_displays.begin() && liqglo.m_renderView ) 
				{
					//I use liqglo.m_displays[ 0 ].name for maya2renderer - yaoyansi
					imageName << liqglo.m_pixDir.asChar() << parseString( liqglo.m_displays[ 0 ].name, false ).asChar();
					//imageName << liqRibTranslator::getInstancePtr()->generateImageName( "", currentJob, currentJob.format);

					// TODO: It doesn't work on windoze...
					//MString host = "localhost";
					//if( !m_renderViewLocal ) 
					//  MGlobal::executeCommand( "strip(system(\"echo $HOST\"));", host );

					RiArchiveRecord( RI_COMMENT, "Display 7");
					RiArchiveRecord( RI_COMMENT, "Render To Maya renderView :" );
					RiArchiveRecord( RI_VERBATIM, "Display \"%s\" \"%s\" \"%s\" \"int merge\" [0] \"int mayaDisplayPort\" [%d] \"string host\" [\"%s\"]\n", 
						const_cast< char* >( imageName.str().c_str() ), "liqmaya", "rgba", liqglo.m_renderViewPort, "localhost" );

					// in this case, override the launch render settings
					if( liqglo.launchRender == false ) 
						liqglo.launchRender = true;
				} 
				else 
				{
					// check if display is enabled
					if( !(*m_displays_iterator).enabled ) 
					{
						m_displays_iterator++;
						continue;
					}
//added in r773, but omited in r775
// 					// get display name
// 					// defaults to scenename.0001.tif if left empty
// 					imageName = (*m_displays_iterator).name;
// 					if( imageName == "" )
// 					{
// 						if( m_isStereoCamera && m_displays_iterator == m_displays.begin() )
// 						{
// 							imageName = liqglo_sceneName + ".left.#." + outExt;
// 						}
// 						else if( m_isStereoCamera )
// 						{
// 							imageName = liqglo_sceneName + ".right.#." + outExt;
// 						}
// 						else
// 						{
// 							imageName = liqglo_sceneName + ".#." + outExt;
// 						}
// 					}
// 					imageName = m_pixDir + parseString( imageName, false );

					// we test for an absolute path before converting from rel to abs path in case the picture dir was overriden through the command line.
					//if( liqglo.m_pixDir.index( '/' ) != 0 ) imageName = liquidGetRelativePath( liqglo_relativeFileNames, imageName, liqglo_projectDir );
					if ( m_displays_iterator == liqglo.m_displays.begin() ) {
						//imageName << liqRibTranslator::getInstancePtr()->generateImageName( "", currentJob, currentJob.format );  
						//I use liqglo.m_displays[ 0 ].name for maya2renderer - yaoyansi
						imageName << liqglo.m_pixDir.asChar() << parseString( liqglo.m_displays[ 0 ].name, false ).asChar();					
						if( liqRibTranslator::getInstancePtr()->m_isStereoCamera )
							imageName<<".left.tif";
					}else{
						//imageName << "+" << liqRibTranslator::getInstancePtr()->generateImageName( (*m_displays_iterator).name, currentJob, imageType.c_str() ) ;
						//I use liqglo.m_displays[ 0 ].name for maya2renderer - yaoyansi
						imageName <<"+"<<liqglo.m_pixDir.asChar() << parseString( (*m_displays_iterator).name, false ).asChar();
						if( liqRibTranslator::getInstancePtr()->m_isStereoCamera )
							imageName<<".right.tif";
					}

					// get display type ( tiff, openexr, etc )
					if( !isBatchMode() ){
						imageType = ((*m_displays_iterator).type == "")? 
							"framebuffer" : (*m_displays_iterator).type.asChar();
					}else {// if in batch mode, we always use "file" -by yaoyansi
						imageType = "file";
					}
					// get display mode ( rgb, z or defined display channel )
					imageMode = ( (*m_displays_iterator).mode == "")? "rgba" : (*m_displays_iterator).mode.asChar();

					// get quantization params
					if( (*m_displays_iterator).doQuantize && m_displays_iterator > liqglo.m_displays.begin() ) 
					{
						if( (*m_displays_iterator).bitDepth != 0 ) 
						{
							int max = (int) pow( 2.0, (*m_displays_iterator).bitDepth ) - 1;
							quantizer << "\"float quantize[4]\" [ 0 " << max << " 0 " << max << " ]";
						} 
						else{
							quantizer << "\"float quantize[4]\" [ 0 0 0 0 ]";
						}

						dither << "\"float dither\" [" << (*m_displays_iterator).dither <<"]";

					} 
					else 
					{
						quantizer.clear();
						dither.clear();
					}

					// get filter params
					if( (*m_displays_iterator).doFilter && m_displays_iterator > liqglo.m_displays.begin() )
					{
						filter << "\"string filter\" [\"" <<  liqglo.liquidRenderer.pixelFilterNames[(*m_displays_iterator).filter].asChar() << "\"] ";
						filter << "\"float filterwidth[2]\" ";
						filter << "[" << (*m_displays_iterator).filterX << " " << (*m_displays_iterator).filterY << "]";
					} 
					else 
						filter.clear();

					// display driver specific arguments
					parameterString.clear();
					for ( int p = 0; p < (*m_displays_iterator).xtraParams.num; p++ ) 
					{
						parameterString << "\"";
						parameterString << paramType[ (*m_displays_iterator).xtraParams.type[p] ];
						parameterString << (*m_displays_iterator).xtraParams.names[p].asChar();
						parameterString << "\" [";//value begin
						if((*m_displays_iterator).xtraParams.type[p] == 0)//add "\"" for string value
							parameterString << "\"";
						parameterString << (*m_displays_iterator).xtraParams.data[p].asChar();//value
						if((*m_displays_iterator).xtraParams.type[p] == 0)//add "\"" for string value
							parameterString << "\"";
						parameterString << "] ";//value end
					}

					RiArchiveRecord( RI_COMMENT, "Display 8");
					// output call
					RiArchiveRecord( RI_VERBATIM, "Display \"%s\" \"%s\" \"%s\" %s %s %s %s\n", 
						const_cast< char* >( imageName.str().c_str() ), 
						imageType.c_str(), 
						imageMode.c_str(), 
						quantizer.str().c_str(), 
						dither.str().c_str(), 
						filter.str().c_str(), 
						parameterString.str().c_str() );
				}
				m_displays_iterator++;
			}//while ( m_displays_iterator != m_displays.end() )
			//refactor 19
			//refactor 18 end
}
}//namespace renderman