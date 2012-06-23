#include <liqHeroRibWriterMgr.h>

#include <string>
#include "./common/prerequest_maya.h"

#include <liqGlobalVariable.h>
#include <liqRibTranslator.h>
#include "shadergraph/shadermgr.h"

tHeroRibWriterMgr::tHeroRibWriterMgr()
{

}
//
tHeroRibWriterMgr::~tHeroRibWriterMgr()
{

}
//
TempControlBreak tHeroRibWriterMgr::write(
struct liqGlobalVariable &liqglo___, 
	const structJob &currentJob___ ,
	const long scanTime__,
	const bool m_outputLightsInDeepShadows__
	//,std::vector<structJob>  jobList__
	)
{
	CM_TRACE_FUNC("tHeroRibWriterMgr::write(liqglo___,"<<currentJob___.name.asChar()<<","<<scanTime__<<","<<m_outputLightsInDeepShadows__<<")");
	
	//RiBegin() is moved to RM::Renderer::ribPrologue_begin()
	// full beauty/shadow rib generation
	//
	/* cout <<"  * build full rib"<<endl; */

//[refactor][1.9.2.5 begin] from ribOutput()
	if( liqRibTranslator::getInstancePtr()->ribPrologue__(currentJob___) == MS::kSuccess ) 
	{
		if( liqRibTranslator::getInstancePtr()->framePrologue__( scanTime__, currentJob___ ) != MS::kSuccess ) 
			return TCB_Break;//break;
//[refactor][1.9.2.5 end] from ribOutput()

		if (liqRibTranslator::getInstancePtr()->m_OutputShaderGraph){
			liquidmaya::ShaderMgr::getSingletonPtr()->exportShaderGraph();
		}

		//[refactor][1.9.2.6 begin] from ribOutput()
		if( liqRibTranslator::getInstancePtr()->worldPrologue__(currentJob___) != MS::kSuccess ) 
			return TCB_Break;//break;
		if( !currentJob___.isShadow || ( currentJob___.isShadow && currentJob___.deepShadows && m_outputLightsInDeepShadows__) ) 
			if( liqRibTranslator::getInstancePtr()->lightBlock__(currentJob___) != MS::kSuccess ) 
				return TCB_Break;//break;
		if( liqRibTranslator::getInstancePtr()->coordSysBlock__(currentJob___) != MS::kSuccess ) 
			return TCB_Break;//break;
		if( liqRibTranslator::getInstancePtr()->objectBlock__(currentJob___) != MS::kSuccess ) 
			return TCB_Break;//break;
		if( liqRibTranslator::getInstancePtr()->worldEpilogue__() != MS::kSuccess ) 
			return TCB_Break;//break;
		//[refactor][1.9.2.6 end] from ribOutput()

		//[refactor][1.9.2.7 begin] from ribOutput()
		if( liqRibTranslator::getInstancePtr()->frameEpilogue__( scanTime__ ) != MS::kSuccess ) 
			return TCB_Break;//break;
		liqRibTranslator::getInstancePtr()->ribEpilogue__(currentJob___);
		//[refactor][1.9.2.7 end]  ribOutput()

		// output info when done with the rib - Alf
		printf("Finished RIB generation %s\n", currentJob___.ribFileName.asChar());
	}
	//RiEnd() is moved to RM::Renderer::ribPrologue_end()

	return TCB_OK;
}
//
//
void tHeroRibWriterMgr::ribPrologue_samples(RtFloat xsamples, RtFloat ysamples)
{
	CM_TRACE_FUNC("tHeroRibWriterMgr::ribPrologue_samples("<<xsamples<<","<<ysamples<<")");

	RiPixelSamples( xsamples, ysamples );
}
//
void tHeroRibWriterMgr::ribPrologue_shadingrate(RtFloat size)
{
	CM_TRACE_FUNC("tHeroRibWriterMgr::ribPrologue_shadingrate("<<size<<")");

	RiShadingRate( size );
}
//
void tHeroRibWriterMgr::ribPrologue_filter(
	liquidlong m_rFilter,
	RtFloat m_rFilterX, RtFloat m_rFilterY)
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
	RtString hiderName;
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
void tHeroRibWriterMgr::ribPrologue_pass(RtString pass)
{
	CM_TRACE_FUNC("tHeroRibWriterMgr::ribPrologue_pass("<<pass<<")");

	RiOption( "user", "string pass", ( RtPointer )&pass, RI_NULL );	
}
//
void tHeroRibWriterMgr::framePrologue_display(const structJob &currentJob)
{
	CM_TRACE_FUNC("tHeroRibWriterMgr::framePrologue_display("<<currentJob.name.asChar()<<")");
	
			//refactor 14
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
			//refactor 14
			LIQDEBUGPRINTF( "-> Setting Display Options\n" );
			//refactor 18
			if( ( liqglo.m_cropX1 != 0.0 ) || ( liqglo.m_cropY1 != 0.0 ) || ( liqglo.m_cropX2 != 1.0 ) || ( liqglo.m_cropY2 != 1.0 ) ) 
			{
				// philippe : handle the rotated camera case
				if( liqglo.liqglo_rotateCamera == true ) 
					RiCropWindow( liqglo.m_cropY2, liqglo.m_cropY1, 1 - liqglo.m_cropX1, 1 - liqglo.m_cropX2 );
				else 
					RiCropWindow( liqglo.m_cropX1, liqglo.m_cropX2, liqglo.m_cropY1, liqglo.m_cropY2 );
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

				while ( m_channels_iterator != liqglo.m_channels.end() ) 
				{
					int       numTokens = 0;
					RtToken   tokens[5];
					RtPointer values[5];

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
						values[ numTokens ] = (RtPointer)quantize;
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
						values[ numTokens ] = ( RtPointer )&filter;
						numTokens++;

						filterwidth[0] = m_channels_iterator->pixelFilterX;
						filterwidth[1] = m_channels_iterator->pixelFilterY;
						tokens[ numTokens ] = "float filterwidth[2]";
						values[ numTokens ] = ( RtPointer )filterwidth;
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
					RiDisplayChannelV( ( RtToken )channel.str().c_str(), numTokens, tokens, values );
#else
					// || defined( GENERIC_RIBLIB )

					RiArchiveRecord( RI_VERBATIM, "DisplayChannel \"%s\" %s %s %s",
						const_cast< char* >( channel.str().c_str() ),  quantize_str.asChar(), dither_str.asChar(), filter_str.asChar() );

#endif
					m_channels_iterator++;
				}//while ( m_channels_iterator != m_channels.end() ) 
			}//if( liqglo.liquidRenderer.supports_DISPLAY_CHANNELS ) 
			
			
			// output display drivers
			RiArchiveRecord( RI_COMMENT, "Display Drivers:" );
			liqRIBMsg("Display 6");

			std::vector<StructDisplay>::iterator m_displays_iterator;
			m_displays_iterator = liqglo.m_displays.begin();

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
					//MString imageName( liqglo.m_pixDir );
					//imageName += parseString( liqglo.m_displays[ 0 ].name, false );
					imageName << liqRibTranslator::getInstancePtr()->generateImageName( "", currentJob );

					// TODO: It doesn't work on windoze...
					//MString host = "localhost";
					//if( !m_renderViewLocal ) 
					//  MGlobal::executeCommand( "strip(system(\"echo $HOST\"));", host );

					liqRIBMsg("Display 7");
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
					// we test for an absolute path before converting from rel to abs path in case the picture dir was overriden through the command line.
					//if( liqglo.m_pixDir.index( '/' ) != 0 ) imageName = liquidGetRelativePath( liqglo_relativeFileNames, imageName, liqglo_projectDir );
					if ( m_displays_iterator == liqglo.m_displays.begin() ) 
						imageName << liqRibTranslator::getInstancePtr()->generateImageName( "", currentJob );  
					else
						imageName << "+" << liqRibTranslator::getInstancePtr()->generateImageName( (*m_displays_iterator).name, currentJob ) ;

					// get display type ( tiff, openexr, etc )
					if( !isBatchMode() ){
						imageType = ((*m_displays_iterator).type == "")? 
							"framebuffer" : (*m_displays_iterator).type.asChar();
					}else {// if in batch mode, we always use "file"
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
						parameterString << "\" [";
						parameterString << ((*m_displays_iterator).xtraParams.type[p] > 0)? "":"\"";
						parameterString << (*m_displays_iterator).xtraParams.data[p].asChar();
						parameterString << ((*m_displays_iterator).xtraParams.type[p] > 0)? "] ":"\"] ";
					}

					liqRIBMsg("Display 8");
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
			//refactor 18
}