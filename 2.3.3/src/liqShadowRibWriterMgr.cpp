#include <liqShadowRibWriterMgr.h>

#include "liqtypes.h"
//#include "ri_interface.h"
//#include <liqGlobalVariable.h>
#include <liqRibTranslator.h>
//#include <liqProcessLauncher.h>
#include "renderermgr.h"

tShadowRibWriterMgr::tShadowRibWriterMgr()
{

}
//
tShadowRibWriterMgr::~tShadowRibWriterMgr()
{

}
//
TempControlBreak tShadowRibWriterMgr::write(
	struct liqGlobalVariable &liqglo___, 
	const structJob &currentJob___ ,
	const long scanTime__,
	const bool m_outputLightsInDeepShadows__,
	std::vector<structJob>  jobList__
	)
{
	CM_TRACE_FUNC("tShadowRibWriterMgr::write(liqglo___,"<<currentJob___.name.asChar()<<","<<scanTime__<<","<<m_outputLightsInDeepShadows__<<",jobList__.size="<<jobList__.size()<<")");

	// BaseShadow------------------------------------------------------------
	if( liquid::RendererMgr::getInstancePtr()->
		getRenderer()->isBaseShadowReady(currentJob___) ) 
	{
		//begin
		liquid::RendererMgr::getInstancePtr()->
			getRenderer()->BaseShadowBegin(currentJob___);

		//[refactor][1.9.2.6 begin] from ribOutput()
		if( liqRibTranslator::getInstancePtr()->worldPrologue__(currentJob___) != MS::kSuccess )
			return TCB_Break;//break;

		if( currentJob___.pass == rpShadowMap 
			//[refactor][1.9.2.2 begin] from _doIt()
			&& currentJob___.shadowType == stDeep && m_outputLightsInDeepShadows__ 
			//[refactor][1.9.2.2 end] from _doIt()			
			) {
			if( liqRibTranslator::getInstancePtr()->lightBlock__(currentJob___) != MS::kSuccess ) 
				return TCB_Break;//break;
		}

		if( liqRibTranslator::getInstancePtr()->coordSysBlock__(currentJob___) != MS::kSuccess ) 
			return TCB_Break;//break;
		if( liqRibTranslator::getInstancePtr()->objectBlock__(currentJob___) != MS::kSuccess ) 
			return TCB_Break;//break;
		if( liqRibTranslator::getInstancePtr()->worldEpilogue__() != MS::kSuccess ) 
			return TCB_Break;//break;
		//[refactor][1.9.2.6 end] from ribOutput()
		//end
		liquid::RendererMgr::getInstancePtr()->
			getRenderer()->BaseShadowEnd(currentJob___);

		//------------------------------------------------------------
		//[refactor][1.9.2.3 begin] from _doIt()
		// mark all other jobs with the same set as done
		std::vector<structJob>::iterator iterCheck = jobList__.begin();
		while ( iterCheck != jobList__.end() ) 
		{
			if( iterCheck->shadowObjectSet == currentJob___.shadowObjectSet &&
				iterCheck->everyFrame == currentJob___.everyFrame &&
				iterCheck->renderFrame == currentJob___.renderFrame
				)
				iterCheck->shadowArchiveRibDone = true;
			++iterCheck;
		}
		liqglo.m_alfShadowRibGen = true;
		//[refactor][1.9.2.3 end] from _doIt()
		//------------------------------------------------------------
	}
	else{
		//todo....
	}
	//////////////////////////////////////////////////////////////////////////
	//ShadowPass----------------------------------------------------------------
	if( liquid::RendererMgr::getInstancePtr()->
		getRenderer()->isShadowPassReady(currentJob___) ) 
	{
		//begin
		liquid::RendererMgr::getInstancePtr()->
			getRenderer()->ShadowPassBegin(currentJob___);
//[refactor][1.9.2.5 begin] from ribOutput()
		if( liqRibTranslator::getInstancePtr()->ribPrologue__(currentJob___) == MS::kSuccess ) 
		{
			if( liqRibTranslator::getInstancePtr()->framePrologue__( scanTime__, currentJob___) != MS::kSuccess ) 
				return TCB_Break;//break;
//[refactor][1.9.2.5 end] from ribOutput()			
			// reference the correct shadow archive
			liquid::RendererMgr::getInstancePtr()->
				getRenderer()->readBaseShadow(currentJob___);

			//[refactor][1.9.2.7 begin] from ribOutput()	
			if( liqRibTranslator::getInstancePtr()->frameEpilogue__( scanTime__ ) != MS::kSuccess ) 
				return TCB_Break;//break;
			liqRibTranslator::getInstancePtr()->ribEpilogue__(currentJob___);
			//[refactor][1.9.2.7 end]  from ribOutput()
		}

		//end
		liquid::RendererMgr::getInstancePtr()->
			getRenderer()->ShadowPassEnd(currentJob___);
	}


	return TCB_OK;
}


//
//void tShadowRibWriterMgr::ribPrologue_samples(liqFloat xsamples, liqFloat ysamples)
//{
//	CM_TRACE_FUNC("tShadowRibWriterMgr::ribPrologue_samples("<<xsamples<<","<<ysamples<<")");
//
//	RiPixelSamples( xsamples, ysamples );
//}
////
//void tShadowRibWriterMgr::ribPrologue_shadingrate(liqFloat size)
//{
//	CM_TRACE_FUNC("tShadowRibWriterMgr::ribPrologue_shadingrate("<<size<<")");
//
//	RiShadingRate( size );
//}
////
//void tShadowRibWriterMgr::ribPrologue_filter(
//	liquidlong m_rFilter,
//	liqFloat m_rFilterX, liqFloat m_rFilterY)
//{
//	CM_TRACE_FUNC("tShadowRibWriterMgr::ribPrologue_filter("<<m_rFilter<<","<<m_rFilterX<<","<<m_rFilterY<<")");
//
//	switch (m_rFilter) 
//	{
//	case pfBoxFilter:
//		RiPixelFilter( RiBoxFilter, m_rFilterX, m_rFilterY );
//		break;
//	case pfTriangleFilter:
//		RiPixelFilter( RiTriangleFilter, m_rFilterX, m_rFilterY );
//		break;
//	case pfCatmullRomFilter:
//		RiPixelFilter( RiCatmullRomFilter, m_rFilterX, m_rFilterY );
//		break;
//	case pfGaussianFilter:
//		RiPixelFilter( RiGaussianFilter, m_rFilterX, m_rFilterY );
//		break;
//	case pfSincFilter:
//		RiPixelFilter( RiSincFilter, m_rFilterX, m_rFilterY );
//		break;
//#if defined ( DELIGHT ) || defined ( PRMAN ) || defined ( GENERIC_RIBLIB )
//	case pfBlackmanHarrisFilter:
//		RiArchiveRecord( RI_VERBATIM, "PixelFilter \"blackman-harris\" %g %g\n", m_rFilterX, m_rFilterY);
//		break;
//	case pfMitchellFilter:
//		RiArchiveRecord( RI_VERBATIM, "PixelFilter \"mitchell\" %g %g\n", m_rFilterX, m_rFilterY);
//		break;
//	case pfSepCatmullRomFilter:
//		RiArchiveRecord( RI_VERBATIM, "PixelFilter \"separable-catmull-rom\" %g %g\n", m_rFilterX, m_rFilterY);
//		break;
//	case pfBesselFilter:
//		RiArchiveRecord( RI_VERBATIM, "PixelFilter \"bessel\" %g %g\n", m_rFilterX, m_rFilterY);
//		break;
//#endif
//#if defined ( PRMAN ) || defined ( GENERIC_RIBLIB )
//	case pfLanczosFilter:
//		RiArchiveRecord( RI_VERBATIM, "PixelFilter \"lanczos\" %g %g\n", m_rFilterX, m_rFilterY);
//		break;
//	case pfDiskFilter:
//		RiArchiveRecord( RI_VERBATIM, "PixelFilter \"disk\" %g %g\n", m_rFilterX, m_rFilterY);
//		break;
//#endif
//	default:
//		RiArchiveRecord( RI_COMMENT, "Unknown Pixel Filter selected" );
//		break;
//	}
//}
////
//void tShadowRibWriterMgr::ribPrologue_pass(liqString pass)
//{
//	CM_TRACE_FUNC("tShadowRibWriterMgr::ribPrologue_pass("<<pass<<")");
//
//	RiOption( "user", "string pass", ( liqPointer )&pass, RI_NULL );	
//}
////
//void tShadowRibWriterMgr::framePrologue_display(const structJob &currentJob)
//{
//	CM_TRACE_FUNC("tShadowRibWriterMgr::framePrologue_display(job="<<currentJob.name.asChar()<<")");
//
//	//refactor 14-1 begin from liqRibTranslator::framePrologue()
//	if( currentJob.pass != rpShadowMap && liqglo.liqglo_rotateCamera  == true )
//	{
//		// philippe : Rotated Camera Case
//		RiFormat( currentJob.height, currentJob.width, currentJob.aspectRatio );
//	}else{
//		RiFormat( currentJob.width, currentJob.height, currentJob.aspectRatio );
//	}
//	//refactor 14-1 end
//
//	if( currentJob.pass != rpShadowMap )
//	{
//		//refactor 14 begin to tHeroRibWriterMgr::framePrologue_display()
//		// Smooth Shading
//		RiShadingInterpolation( "smooth" );
//		// Quantization
//		// overriden to floats when in rendering to Maya's renderView
//		if( !liqglo.m_renderView && liqglo.quantValue != 0 )
//		{
//			int whiteValue = (int) pow( 2.0, liqglo.quantValue ) - 1;
//			RiQuantize( RI_RGBA, whiteValue, 0, whiteValue, 0.5 );
//		}
//		else
//		{
//			RiQuantize( RI_RGBA, 0, 0, 0, 0 );
//		}
//		if( liqglo.m_rgain != 1.0 || liqglo.m_rgamma != 1.0 )
//		{
//			RiExposure( liqglo.m_rgain, liqglo.m_rgamma );
//		}
//		//refactor 14 end
//	}
//
//			if( currentJob.pass == rpShadowMap &&(currentJob.shadowType != stDeep || currentJob.samples == 1 ) )
//			{	
//				//refactor 15
//				if( liqglo.liquidRenderer.renderName == MString("Pixie") )
//				{
//					liqFloat zero = 0;
//					RiHider( "hidden", "jitter", &zero, RI_NULL );
//				}
//				else
//				{
//					liqInt zero = 0;
//					RiHider( "hidden", "int jitter", &zero, RI_NULL );
//				}
//				//refactor 15
//			}
//			if( currentJob.pass == rpShadowMap && currentJob.shadowType == stMidPoint )
//			{
//				//refactor 16
//				liqString midPoint = "midpoint";
//				liqFloat midRatio = /*liqglo.liqglo_*/currentJob.midPointRatio;
//
//				RiHider( "hidden", "depthfilter", &midPoint, RI_NULL );
//
//				if ( /*liqglo.liqglo_*/currentJob.midPointRatio != 0 )
//					RiHider( "hidden", "midpointratio", &midRatio, RI_NULL ); // Output to rib jami
//							//refactor 16
//			}
//			//-----------------------------------------------------
//			LIQDEBUGPRINTF( "-> Setting Display Options\n" );
//			//if( currentJob.pass == rpShadowMap )
//			//MString relativeShadowName( liquidSanitizePath( liquidGetRelativePath( liqglo_relativeFileNames, liqglo_currentJob.imageName, liqglo_projectDir ) ) );
//			//refactor 17 begin
//			if( currentJob.shadowType != stMinMax )
//			{
//				if( currentJob.shadowType == stDeep )
//				{
//					// RiDeclare( "volumeinterpretation", "string" );
//					liqString volume = "continuous";
//
//					if ( currentJob.volume != viContinuous )
//						volume = "discrete"; 
//
//					if( liqglo.liquidRenderer.renderName == MString("3Delight") )
//					{
//						RiArchiveRecord( RI_COMMENT, "Display 1");
//						RiDisplay( const_cast< char* >( /*liqglo.liqglo_*/currentJob.imageName.asChar()),
//							const_cast< char* >( /*liqglo.liqglo_*/currentJob.format.asChar() ),
//							(liqToken)/*liqglo.liqglo_*/currentJob.imageMode.asChar(),
//							"string volumeinterpretation", &volume,
//							RI_NULL );
//					}
//					else
//					{
//						// Deep shadows cannot be the primary output driver in PRMan & co.
//						// We need to create a null output zfile first, and use the deep
//						// shadows as a secondary output.
//						//
//						if( liqglo.liquidRenderer.renderName != MString("Pixie") )
//						{
//							RiArchiveRecord( RI_COMMENT, "Display 2");
//							RiDisplay( "null", "null", "z", RI_NULL );
//						}
//
//						MString deepFileImageName = "+" + /*liqglo.liqglo_*/currentJob.imageName;
//
//						RiArchiveRecord( RI_COMMENT, "Display 3");
//						RiDisplay( const_cast< char* >( deepFileImageName.asChar() ),
//							const_cast< char* >( /*liqglo.liqglo_*/currentJob.format.asChar() ),
//							(liqToken)/*liqglo.liqglo_*/currentJob.imageMode.asChar(),
//							"string volumeinterpretation", &volume,
//							RI_NULL );
//					}
//				}//if( liqglo.liqglo_currentJob.deepShadows )
//				else
//				{
//					RiArchiveRecord( RI_COMMENT, "Display 4");
//					liqInt aggregate( /*liqglo.liqglo_*/currentJob.shadowAggregation );
//					RiDisplay( const_cast< char* >( /*liqglo.liqglo_*/currentJob.imageName.asChar() ),
//						const_cast< char* >( /*liqglo.liqglo_*/currentJob.format.asChar() ),
//						(liqToken)/*liqglo.liqglo_*/currentJob.imageMode.asChar(),
//						"int aggregate", &aggregate,
//						RI_NULL );
//				}
//			}//if( !liqglo.liqglo_currentJob.isMinMaxShadow )
//			else
//			{
//				RiArchiveRecord( RI_COMMENT, "Display 5");
//				RiArchiveRecord( RI_COMMENT, "Display Driver:" );
//				liqInt minmax = 1;
//				RiDisplay( const_cast< char* >( (/*liqglo.liqglo_*/currentJob.imageName+(int)liqglo.liqglo_lframe).asChar() ),//const_cast< char* >( parseString(liqglo_currentJob.imageName).asChar() ),
//					const_cast< char* >(/*liqglo.liqglo_*/currentJob.format.asChar()),
//					(liqToken)/*liqglo.liqglo_*/currentJob.imageMode.asChar(),
//					"minmax", &minmax,
//					RI_NULL );
//			}
//			liqRibTranslator::getInstancePtr()->exportJobCamera( currentJob, currentJob.camera );
//			//refactor 17 end
//			//}
//}
//