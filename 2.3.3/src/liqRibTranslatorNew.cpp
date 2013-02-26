/*
**
**
** The contents of this file are subject to the Mozilla Public License Version
** 1.1  (the "License"); you may not use this file except in compliance with
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
** Copyright 1988, 1 989, Pixar
** All Rights Reserved
**
**
** RenderMan (R) is a registered trademark of Pixar
*/


#include <liqRibTranslator.h>

#include <sys/types.h>

#ifndef _WIN32
#include <sys/time.h>
#include <sys/stat.h>
// Dynamic Object Headers
#include <dlfcn.h>
#endif

#ifdef _WIN32
#pragma warning(disable:4786)
#endif

#ifdef _WIN32
#include <process.h>
#include <io.h>
#include <direct.h>
#else
#include <unistd.h>
#include <stdlib.h>
#include <pwd.h>
#endif
#include <time.h>
#include <algorithm>
#include <sstream>

#include <boost/scoped_array.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/tokenizer.hpp>
#include <boost/algorithm/string/case_conv.hpp>

// Renderman Headers
//extern "C" {
#include "liqtypes.h"
//#include "ri_interface.h"
//}

#ifdef _WIN32
#  define RM_CMD "cmd.exe /c del"
#else
#  define RM_CMD "/bin/rm"
#endif


// Maya headers
#include "./common/prerequest_maya.h"
// Liquid headers
#include <liquid.h>
#include <liqRibHT.h>
#include <liqRenderer.h>
#include <liqRibLightData.h>
#include <liqGlobalHelpers.h>
#include <liqProcessLauncher.h>
#include <liqCustomNode.h>
#include <liqShaderFactory.h>
#include <liqExpression.h>
#include <liqRenderScript.h>
#include <liqGlobalVariable.h>
#include <liqJobScriptMgr.h>
#include <liqFrameScriptMgr.h>
#include <liqLightMgr.h>
#include <liqLocatorMgr.h>
#include <liqShadowRibWriterMgr.h>
#include <liqHeroRibWriterMgr.h>
#include <liqRibCamera.h>

#include "common/mayacheck.h"
#include "renderermgr.h"
#include "shadergraph/shadermgr.h"

#define _Refactor_doTexure_doShadow

using namespace boost;
using namespace std;

typedef int RtError;



#if 0
#ifdef _WIN32
// Hmmmmmmmm what's this ?
int RiNColorSamples;
#endif
// these are little storage variables to keep track of the current graphics state and will eventually be wrapped in
// a specific class
#endif


//void liqRibTranslator::freeShaders( void )
//{
//  LIQDEBUGPRINTF( "-> freeing shader data.\n" );
//  m_shaders.clear();
//  LIQDEBUGPRINTF( "-> finished freeing shader data.\n" );
//}

// Hmmmmm should change magic to Liquid


#if defined( DELIGHT ) || defined( ENTROPY ) ||  defined( PRMAN ) || defined( AIR ) || defined( PIXIE ) || defined( GENERIC_RIBLIB ) 
extern void liqRibTranslatorErrorHandler( liqInt code, liqInt severity, char* message );
#else
extern void liqRibTranslatorErrorHandler( liqInt code, liqInt severity, const char* message );
#endif

#if defined(_WIN32)/* && !defined(DEFINED_LIQUIDVERSION)*/
// unix build gets this from the Makefile
extern const char *LIQUIDVERSION;
#endif

liqRibTranslator* liqRibTranslator::mInstance = NULL;

/**
* This method actually does the renderman output.
*/
MStatus liqRibTranslator::_doItNew( const MString& originalLayer )
{
	CM_TRACE_FUNC(boost::format("liqRibTranslatorNew::_doItNew(args,%s)")%originalLayer.asChar());
	
	//[refactor][1.1 begin] from _doItNew()
	// append the progress flag for render job feedback
// 	if( useRenderScript )
// 		if( ( m_renderCommand == MString( "render" ) ) || ( m_renderCommand == MString( "prman" ) ) || ( m_renderCommand == MString( "renderdl" ) ) ) 
// 			m_renderCommand = m_renderCommand + " -Progress";
	//[refactor][1.1 end] from _doItNew()

	if( (!liqglo.m_deferredGen && m_justRib) || liqglo.m_exportReadArchive )
		useRenderScript = false;
	liquidMessage2(messageInfo,"useRenderScript=%d", useRenderScript);

	if(useRenderScript){
		return _doItNewWithRenderScript(originalLayer);
	}else{
		return _doItNewWithoutRenderScript(originalLayer);
	}

}
//
void liqRibTranslator::calaculateSamplingTime(const long scanTime__)
{
	CM_TRACE_FUNC("liqRibTranslatorNew::calaculateSamplingTime("<<scanTime__<<")");
	
	//[refactor][1.9.1.1 begin] from _doIt()
	float sampleinc( ( liqglo.liqglo_shutterTime * m_blurTime ) / ( liqglo.liqglo_motionSamples - 1 ) );
	for ( unsigned msampleOn( 0 ); msampleOn < liqglo.liqglo_motionSamples; msampleOn++ ) 
	{
		float subframe;
		switch( liqglo.shutterConfig ) 
		{
		case OPEN_ON_FRAME:
		default:
			subframe = scanTime__ + ( msampleOn * sampleinc );
			break;
		case CENTER_ON_FRAME:
			subframe = ( scanTime__ - ( liqglo.liqglo_shutterTime * m_blurTime * 0.5 ) ) + msampleOn * sampleinc;
			break;
		case CENTER_BETWEEN_FRAMES:
			subframe = scanTime__ + ( 0.5 * ( 1 - ( liqglo.liqglo_shutterTime * m_blurTime ) ) ) + ( msampleOn * sampleinc );
			break;
		case CLOSE_ON_NEXT_FRAME:
			subframe = scanTime__ + ( 1 - ( liqglo.liqglo_shutterTime * m_blurTime ) ) + ( msampleOn * sampleinc );
			break;
		}
		liqglo.liqglo_sampleTimes[ msampleOn ] = subframe;
		liqglo.liqglo_sampleTimesOffsets[ msampleOn ] = msampleOn * sampleinc;
	}
	//[refactor][1.9.1.1 end] from _doIt()
}

/**
* Set up data for the current job.
*/
MStatus liqRibTranslator::buildJobs__()
{
	CM_TRACE_FUNC("liqRibTranslatorNew::buildJobs__()");
	LIQDEBUGPRINTF( "-> beginning to build job list\n" );
	MStatus returnStatus = MS::kSuccess;
	MStatus status;
	MObject cameraNode;
	//MDagPath lightPath;

	jobList.clear();
	shadowList.clear();
	txtList.clear();

	//[refactor 35] begin from liqRibTranslator.buildJobs()
	structJob thisJob;

	thisJob.pass = rpNone; // reset RenderPass type
	thisJob.name.clear();
	thisJob.texName.clear();
	thisJob.height = thisJob.width = 0;
	thisJob.renderName = m_renderCommand;
	thisJob.isShadowPass  = false;
	thisJob.isPoint       = false;
	thisJob.renderFrame   = liqglo.liqglo_lframe;
	thisJob.everyFrame    = true;
	thisJob.isStereoPass  = false;
	thisJob.aspectRatio   = 1.0;
	thisJob.samples		  = 1;
	thisJob.shadingRate	  = 1.0;
	thisJob.shadingRateFactor = 1.0;
	thisJob.skip          = false;
	//[refactor 35] end 

	MItDependencyNodes dependencyNodesIter(MFn::kDependencyNode, &returnStatus);
	if(returnStatus==MS::kSuccess)
	{
		//char *currentName = NULL;
		for(; !dependencyNodesIter.isDone(); dependencyNodesIter.next())
		{
			MFnDependencyNode dependencyNodeFn(dependencyNodesIter.thisNode());
			if( dependencyNodeFn.typeName()=="liquidSurface"
				|| dependencyNodeFn.typeName()=="liquidDisplacement"
				|| dependencyNodeFn.typeName()=="liquidVolume"
				|| dependencyNodeFn.typeName()=="liquidLight"
				|| dependencyNodeFn.typeName()=="liquidShader"
				)
			{
				MString name(dependencyNodeFn.name());
				liqShader currentShader(dependencyNodesIter.thisNode());
				currentShader.buildJobs();
			}
		}
	}
	// what we do here is make all of the lights with depth shadows turned on into
	// cameras and add them to the renderable camera list *before* the main camera
	// so all the automatic depth map shadows are complete before the main pass

	if( liqglo.liqglo_doShadows ) 
	{
		returnStatus = buildShadowJobs__(thisJob);
	} // liqglo_doShadows



	{
		tRibCameraMgr::gatherDataForJob(
			liqglo, thisJob, jobList,
			m_outputShadowPass, m_outputHeroPass
		);
	}
		
	//[refactor 4] begin from buildJobs()
	// If we didn't find a renderable camera then give up
	if( jobList.size() == 0 ) 
	{
		liquidMessage ( "No Renderable Jobs in the list", messageWarning );
		// MString cError( "No Renderable Camera Found!" );
		// throw( cError );
		// return MS::kFailure;
	}

	LIQDEBUGPRINTF("  step through the jobs and setup their names..." );
	//cerr << "step through the jobs and setup their names..." << endl << flush;

	// step through the jobs and setup their names
	vector<structJob>::iterator iter = jobList.begin();
	while ( iter != jobList.end() )
	{
		LIQ_CHECK_CANCEL_REQUEST;
		thisJob = *iter;

		// set the skip flag for the job
		iter->skip   = false;

		if ( iter->pass == rpHeroPass )
		{
			iter->ribFileName = generateFileName( fgm_hero_rib, thisJob );
			iter->imageName = generateFileName( ( fileGenMode )fgm_image, thisJob );
			if ( liqglo.liqglo_singleFrameShadowsOnly )
			{
				// singleFrameShadowsOnly on hero pass
				iter->skip   = true;
			}
		} 
		else if ( iter->pass == rpShadowMap )
		{
			iter->ribFileName = generateFileName( fgm_shadow_rib, thisJob );

			MString varVal;
			MString userShadowName;
			MFnDagNode lightNode( thisJob.path );

			if ( liquidGetPlugValue( lightNode, "liquidShadowName", varVal, status ) == MS::kSuccess ) 
				userShadowName = parseString( varVal, false );

			iter->imageName = ( userShadowName.length() )? userShadowName : generateFileName( ( fileGenMode )fgm_shadow_tex, thisJob );

			if ( !liqglo.liqglo_doShadows ) // shadow generation disabled
			{
				iter->skip   = true;
			}
			else if ( !iter->everyFrame )
			{
				int firstFrame = 1;
				if ( liqglo.frameNumbers.size() )
					firstFrame = liqglo.frameNumbers[ 0 ];

				if ( liqglo.liqglo_noSingleFrameShadows || liqglo.liqglo_lframe > firstFrame && iter->renderFrame != liqglo.liqglo_lframe )
				{
					// noSingleFrameShadows or rendering past the first frame of the sequence
					iter->skip   = true;
				}
			}
			else if ( iter->everyFrame && liqglo.liqglo_singleFrameShadowsOnly )
			{
				// singleFrameShadowsOnly on regular shadows
				iter->skip   = true;
			}
			thisJob = *iter;
			shadowList.push_back( thisJob );
		}
		++iter;
	}//while ( iter != jobList.end() )

	// sort the shadow jobs to put the reference frames first
#ifndef _WIN32
	sort( jobList.begin(), jobList.end(), renderFrameSort );
	sort( shadowList.begin(), shadowList.end(), renderFrameSort );
	sort( txtList.begin(), txtList.end(), renderFrameSort );
#else
	sort( jobList.begin(), jobList.end(), renderFrameSort );
	sort( shadowList.begin(), shadowList.end(), renderFrameSort );
	sort( txtList.begin(), txtList.end(), renderFrameSort );
#endif
	ribStatus = kRibBegin;
	return MS::kSuccess;
	//[refactor 4] end from buildJobs()
}



//
TempControlBreak liqRibTranslator::processOneFrame(
	const unsigned int frameIndex,
	struct liqGlobalVariable &liqglo__
	)
{	
	CM_TRACE_FUNC("liqRibTranslatorNew::processOneFrame("<<frameIndex<<", liqglo__)");

	//[refactor][1.6 begin] from _doIt()
	liqShaderFactory::instance().clearShaders();

	liqglo__.liqglo_lframe = liqglo.frameNumbers[ frameIndex ];

	if( m_showProgress ) 
		printProgress( 1, liqglo.frameNumbers.size(), frameIndex );



	liqglo__.m_alfShadowRibGen = false;
	liqglo__.liqglo_preReadArchive.clear();
	liqglo__.liqglo_preRibBox.clear();
	liqglo__.liqglo_preReadArchiveShadow.clear();
	liqglo__.liqglo_preRibBoxShadow.clear();
	//[refactor][1.6 begin] from _doIt()

	//[refactor][1.7 begin] from _doIt() 
	// make sure all the global strings are parsed for this frame
	const MString framePreCommand(parseString( liqglo.m_preCommand, false));
	const MString frameRibgenCommand(parseString( m_ribgenCommand, false ));
	const MString frameRenderCommand( parseString( m_renderCommand + " " + liqglo__.liquidRenderer.renderCmdFlags, false ));
	const MString framePreFrameCommand(parseString( m_preFrameCommand, false ));
	const MString framePostFrameCommand(parseString( m_postFrameCommand, false ));
	//[refactor][1.7 end] from _doIt() 

	//***************************************************************
	LIQDEBUGPRINTF( "-> building jobs\n" );
	// Hmmmmmm not really clean ....
	if( buildJobs__() != MS::kSuccess ) 
		return TCB_Break;//break;
	//***************************************************************

	if( !liqglo.m_deferredGen ) 
	{
		if( m_showProgress ) 
			printProgress( 2, liqglo.frameNumbers.size(), frameIndex );

		long lastScannedFrame = -100000;
		long scanTime = liqglo__.liqglo_lframe;
		//hashTableInited = false;
		//
		// start iterating through the job list   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-
		//
		if( jobList.size() == 0 ) 
		{
			liquidMessage( "Nothing to render!", messageWarning );
			return TCB_MS_Success;//return MS::kSuccess;
		}
		//cout <<"Job iteration start -------------------------------------"<<endl;
		//cout <<"    nsfs:"<<liqglo_noSingleFrameShadows<<"  sfso:"<<liqglo_singleFrameShadowsOnly<<endl;
		vector< structJob >::iterator iter( jobList.begin() );
		for (; iter != jobList.end(); ++iter ) 
		{
			m_currentMatteMode = false;
			//[refactor] [1.9.1 begin] from _doIt()
			structJob/*liqglo__.liqglo_*/ &currentJob = *iter;

			if( /*liqglo__.liqglo_*/currentJob.skip ) 
				continue;

			//cout <<">> outputing job ["<<liqglo_lframe<<"] ->"<<liqglo_currentJob.name.asChar()<<" -> "<<liqglo_currentJob.imageName.asChar()<<endl;
			// set the scan time based on the job's render frame
			//
			scanTime = /*liqglo__.liqglo_*/currentJob.renderFrame;
			// if we changed the frame to calculate a shadow at a different time,
			// we need to rescan the scene, otherwise not.
			//
			if( lastScannedFrame != scanTime ) 
			{
				LIQDEBUGPRINTF( "Scanning at time: %u \n", scanTime );

				htable = boost::shared_ptr< liqRibHT >( new liqRibHT() );
				//hashTableInited = true;
				LIQDEBUGPRINTF( "Created hash table...\n" );

				//  calculate sampling time
				//[refactor][1.9.1.1 begin] from _doIt()
				calaculateSamplingTime(scanTime);
				//[refactor][1.9.1.1 end] from _doIt()

				//cout <<"about to scan... "<<endl;
				// scan the scene
				//
				if( liqglo.doCameraMotion || liqglo__.liqglo_doMotion || liqglo__.liqglo_doDef ) 
				{
					for ( int msampleOn = 0; msampleOn < liqglo__.liqglo_motionSamples; msampleOn++ ) 
						scanScene__( liqglo__.liqglo_sampleTimes[ msampleOn ] , msampleOn );
				} else {
					liqglo__.liqglo_sampleTimes[ 0 ] = scanTime;
					liqglo__.liqglo_sampleTimesOffsets[ 0 ] = 0;
					scanScene__( scanTime, 0 );
				}
				//cout <<"    + scene scan done !"<<endl;

				// mark the frame as already scanned
				lastScannedFrame = scanTime;
				//liqglo__.liqglo_currentJob = *iter;
			}

			//[refactor] generate textures and shadow textures before it is used. -begin -yaoyansi
			if( liqglo.launchRender ) 
			{
#ifdef _Refactor_doTexure_doShadow
				// write out make texture pass
				doTextures(txtList);

				if( liqglo.liqglo_doShadows ) 
				{
					doShadows(shadowList);
				}
#endif
			}else{
				liquidMessage2(messageWarning, "liqglo.launchRender is false, skip doTextures()/doShadows()");
			}
			//[refactor] generate textures and shadow textures before it is used. -end -yaoyansi

			//
			// start scene parsing ------------------------------------------------------------------
			//
			liqglo__.liqglo_isShadowPass = currentJob.isShadowPass;



			//************************************************************************************
			LIQDEBUGPRINTF( "-> setting RiOptions\n" );
			//************************************************************************************
			//[refactor][36] begin
			liquid::RendererMgr::getInstancePtr()->
				getRenderer()->setRenderScriptFormatAndCompress(liqglo__.liqglo_doBinary, liqglo__.liqglo_doCompression);
			//[refactor][36] end
			// world RiReadArchives and Rib Boxes ************************************************
			//
			//[refactor][37] begin
			if( /*liqglo__.liqglo_*/currentJob.pass == rpShadowMap && !liqglo.fullShadowRib )
			{
				tShadowRibWriterMgr shadowRibWriterMgr;
				TempControlBreak tcb = 
					shadowRibWriterMgr.write(
					liqglo__, 
					/*liqglo__.liqglo_*/currentJob, 
					scanTime,
					m_outputLightsInDeepShadows,
					jobList);
				PROCESS_TEMP_CONTROL_BREAK_CONTINUE(tcb)
			}//if( liqglo_currentJob.isShadow && !fullShadowRib ) 
			//[refactor][37] end
			else 
			{
				bool bReady = liquid::RendererMgr::getInstancePtr()->
					getRenderer()->isHeroPassReady(currentJob);
				if(bReady)
				{
					liquid::RendererMgr::getInstancePtr()->
						getRenderer()->HeroPassBegin(currentJob);

					tHeroRibWriterMgr heroRibWriterMgr;
					TempControlBreak tcb = 
						heroRibWriterMgr.write(
						liqglo__, 
						/*liqglo__.liqglo_*/currentJob, 
						scanTime,
						m_outputLightsInDeepShadows
						//,jobList
						);

					liquid::RendererMgr::getInstancePtr()->
						getRenderer()->HeroPassEnd(currentJob);

					PROCESS_TEMP_CONTROL_BREAK_CONTINUE(tcb)
				}//if(bReady)
			}//if( currentJob.pass == rpShadowMap && !liqglo.fullShadowRib ) else


#ifdef RENDER_PIPE
			fclose( liqglo_ribFP );
#endif
			liqglo__.liqglo_ribFP = NULL;
			if( m_showProgress ) 
				printProgress( 3, liqglo.frameNumbers.size(), frameIndex );
		}//for (; iter != jobList.end(); ++iter ) 
	}//if( !m_deferredGen ) 
	//[refactor][1.9 end] from ribOutput()
	
	//[refactor][1.9.3 begin] from _doIt()
	// set the rib file for the 'view last rib' menu command
	// NOTE: this may be overridden later on in certain code paths
#if (Refactoring == 0)
	if( !m_deferredGen )
		lastRibName = liqglo__.liqglo_currentJob.ribFileName;
#endif
	//[refactor][1.9.3 end] from _doIt()
	return TCB_OK;
}
//
void liqRibTranslator::doRenderView()
{
	CM_TRACE_FUNC("liqRibTranslatorNew::doRenderView()");
	liquid::RendererMgr::getInstancePtr()->getRenderer()->doRenderView();
}
//
void liqRibTranslator::doTextures(const std::vector<structJob> &txtList_)
{
	CM_TRACE_FUNC("liqRibTranslatorNew::doTextures(size="<<txtList_.size()<<")");
	liquid::RendererMgr::getInstancePtr()->getRenderer()->doTextures(txtList_);
}
//
void liqRibTranslator::doShadows(const std::vector<structJob> &shadowList_)
{
	CM_TRACE_FUNC("liqRibTranslatorNew::doShadows(size="<<shadowList_.size()<<")");
	liquid::RendererMgr::getInstancePtr()->getRenderer()->doShadows(shadowList_);
}
//
void liqRibTranslator::postActions(const MString& originalLayer__)
{
	CM_TRACE_FUNC("liqRibTranslatorNew::postActions("<<originalLayer__.asChar()<<")");
	
	//[refactor][1.22 ]
	// return to the frame we were at before we ran the animation
	LIQDEBUGPRINTF( "-> setting frame to current frame.\n" );
	MGlobal::viewFrame (originalTime);

	if( originalLayer__ != "" ) 
	{
		MString cmd;
		cmd = "if( `editRenderLayerGlobals -q -currentRenderLayer` != \"" + originalLayer__ + "\" ) editRenderLayerGlobals -currentRenderLayer \"" + originalLayer__ + "\";";
		if(  MGlobal::executeCommand( cmd, false, false ) == MS::kFailure ) 
		{
			MString err;
			err = "Liquid : could not switch back to render layer \"" + originalLayer__ + "\" !";
			throw err;
		}
	}
	//[refactor][1.22 ]
}
//
MStatus liqRibTranslator::buildShadowJobs__(structJob &thisJob__)
{
	CM_TRACE_FUNC("liqRibTranslatorNew::buildShadowJobs__()");

	LIQDEBUGPRINTF("  setup shadows...\n" );
	MStatus returnStatus__ = MS::kSuccess;

	returnStatus__ = tLightMgr::buildShadowJob(thisJob__, m_lazyCompute);

	returnStatus__ = tLightMgr::buildShadowCameraJob(thisJob__, m_lazyCompute);


	return returnStatus__;
}
//
MStatus liqRibTranslator::scanScene__(float lframe, int sample )
{
	CM_TRACE_FUNC("liqRibTranslatorNew::scanScene__("<<lframe<<","<<sample<<")");
	int count =0;

	MTime mt( ( double )lframe, MTime::uiUnit() );
	if( MGlobal::viewFrame(mt) == MS::kSuccess ) 
	{
		// scanScene: execute pre-frame command
		if( m_preFrameMel != "" ) 
		{
			MString preFrameMel( parseString( m_preFrameMel, false ) );
			if( fileExists( preFrameMel  ) ) 
				MGlobal::sourceFile( preFrameMel );
			else 
			{
				if( MS::kSuccess == MGlobal::executeCommand( preFrameMel, false, false ) ) {
					liquidMessage2(messageInfo,"Liquid -> pre-frame script executed successfully.");
				}else {
					liquidMessage2(messageError,"Liquid -> ERROR :pre-frame script failed.");
				}
			}
		}

		MStatus returnStatus;
		// scanScene: Scan the scene for lights
		{
			tLightMgr lightMgr;
			lightMgr.scanScene(lframe, sample,htable, count, returnStatus);
		}
		{
			tLocatorMgr locatorMgr;
			locatorMgr.scanScene(lframe, sample,htable, count, returnStatus);
		}
		
		//[refactor 11] 
		if( !m_renderSelected && !m_exportSpecificList )
		{
			MItDag dagIterator( MItDag::kDepthFirst, MFn::kInvalid, &returnStatus);
			for (; !dagIterator.isDone(); dagIterator.next())
			{
				MDagPath path;
				dagIterator.getPath( path );
				MObject currentNode = dagIterator.item();
				if(!currentNode.hasFn(MFn::kDagNode))
					continue;
				returnStatus = scanSceneNodes( currentNode, path, lframe, sample, count, returnStatus );
				if( MS::kSuccess != returnStatus )
					continue;
			}
			dealwithParticleInstancedObjects(sample, lframe, count);//[refactor 11.1] begin/end
		}//  if ( !m_renderSelected && !m_exportSpecificList )
		else
		{
			// scanScene: find out the current selection for possible selected object output
			MSelectionList currentSelection;
			if ( m_renderSelected )
			{
				// scanScene: find out the current selection for possible selected object output
				MGlobal::getActiveSelectionList( currentSelection );
			}
			else   // m_exportSpecificList = 1
			{
				for ( unsigned int i = 0; i < m_objectListToExport.length() ; i++ )
					currentSelection.add( m_objectListToExport[i] );
			}
			MItSelectionList selIterator( currentSelection );
			for( ; !selIterator.isDone(); selIterator.next() )
			{
				MDagPath objectPath;
				selIterator.getDagPath( objectPath );
				
				MItDag dagIterator( MItDag::kDepthFirst, MFn::kInvalid, &returnStatus);			
				dagIterator.reset (objectPath.node(), MItDag::kDepthFirst, MFn::kInvalid );
				for (; !dagIterator.isDone(); dagIterator.next())
				{
					MDagPath path;
					dagIterator.getPath( path );
					MObject currentNode = dagIterator.item();
					if(!currentNode.hasFn(MFn::kDagNode))
						continue;
					returnStatus = scanSceneNodes( currentNode, path, lframe, sample, count, returnStatus );
					if( MS::kSuccess != returnStatus )
						continue;
				}
			}
			dealwithParticleInstancedObjects(sample, lframe, count);//[refactor 11.1] begin/end
		}




		vector<structJob>::iterator iter = jobList.begin();
		while ( iter != jobList.end() ) 
		{
			LIQ_CHECK_CANCEL_REQUEST;
			// scanScene: Get the camera/light info for this job at this frame
			/*MStatus status;*/

			if( iter->pass != rpShadowMap ) 
			{
				if ( MS::kSuccess != getCameraData( iter , sample) )
					return MS::kFailure;
			} //if( !iter->isShadow )  
			else 
			{
				getLightData( iter , sample);
			}//if( !iter->isShadow ) else
			++iter;
		}//while ( iter != jobList.end() )

		// post-frame script execution
		if( m_postFrameMel != "" ) 
		{
			MString postFrameMel( parseString( m_postFrameMel, false ) );
			if( fileExists( postFrameMel  ) ) 
				MGlobal::sourceFile( postFrameMel );
			else 
				if( MS::kSuccess == MGlobal::executeCommand( postFrameMel, false, false ) ) 
					printf("Liquid -> post-frame script executed successfully.\n");
				else 
					printf("Liquid -> ERROR :post-frame script failed.\n");
		}
		return MS::kSuccess;
	}
	return MS::kFailure;
}
//
void liqRibTranslator::dealwithParticleInstancedObjects(
	const float lframe__, const int sample__,
	int &count__ 
	)
{
	CM_TRACE_FUNC("liqRibTranslatorNew::dealwithParticleInstancedObjects("<<lframe__<<","<<sample__<<","<<count__<<")");
	
	// scanScene: Now deal with all the particle-instanced objects (where a
	// particle is replaced by an object or group of objects).
	//
	MItInstancer instancerIter;
	while( !instancerIter.isDone() )
	{
		MDagPath path( instancerIter.path() );
		MString instanceStr( ( MString )"|INSTANCE_" +
			instancerIter.instancerId() + (MString)"_" +
			instancerIter.particleId() + (MString)"_" +
			instancerIter.pathId() );

		MMatrix instanceMatrix( instancerIter.matrix() );

		bool useSamples( ( sample__ > 0 ) && isObjectMotionBlur( path ) );
		htable->insert( path, lframe__, 
			( useSamples )? sample__ : 0,	
			MRT_Unknown, count__++, 
			&instanceMatrix, instanceStr, instancerIter.particleId() );

		instancerIter.next();
	}
}
//
MStatus liqRibTranslator::getCameraData( vector<structJob>::iterator &iter__ , const int sample__)
{
	CM_TRACE_FUNC("liqRibTranslatorNew::getCameraData(iter__,"<<sample__<<")");
	MStatus status;
	//[refactor 12] begin from  liqRibTranslator::scanScene()
				MDagPath path;
				MFnCamera   fnCamera( iter__->path );
				iter__->gotJobOptions = false;
				status.clear();
				MPlug cPlug = fnCamera.findPlug( MString( "ribOptions" ), &status );
				if( status == MS::kSuccess ) 
				{
					cPlug.getValue( iter__->jobOptions );
					iter__->gotJobOptions = true;
				}
				getCameraInfo( fnCamera, iter__->camera[sample__] );
				iter__->width = iter__->camera[sample__].width;
				iter__->height = iter__->camera[sample__].height;
				// scanScene: Renderman specifies shutter by time open
				// so we need to convert shutterAngle to time.
				// To do this convert shutterAngle to degrees and
				// divide by 360.
				//
				iter__->camera[sample__].shutter = fnCamera.shutterAngle() * 0.5 / M_PI;
				liqglo.liqglo_shutterTime = iter__->camera[sample__].shutter;
				iter__->camera[sample__].motionBlur     = fnCamera.isMotionBlur();				
				
				// scanScene: The camera's fov may not match the rendered image in Maya
				// if a film-fit is used. 'fov_ratio' is used to account for
				// this.
				//
				iter__->camera[sample__].hFOV = fnCamera.horizontalFieldOfView()/iter__->camera[sample__].fov_ratio;

				if ( fnCamera.isClippingPlanes() )
				{
					iter__->camera[sample__].neardb   = fnCamera.nearClippingPlane();
					iter__->camera[sample__].fardb    = fnCamera.farClippingPlane();
				}
				else
				{
					iter__->camera[sample__].neardb    = 0.001;    // TODO: these values are duplicated elsewhere in this file
					iter__->camera[sample__].fardb    = 250000.0; // TODO: these values are duplicated elsewhere in this file
				}

				iter__->camera[sample__].orthoWidth     = fnCamera.orthoWidth();
				iter__->camera[sample__].orthoHeight    = fnCamera.orthoWidth() * ((float)iter__->camera[sample__].height / (float)iter__->camera[sample__].width);

				iter__->camera[sample__].focalLength    = fnCamera.focalLength();
				iter__->camera[sample__].focalDistance  = fnCamera.focusDistance();
				iter__->camera[sample__].fStop          = fnCamera.fStop();
				iter__->camera[sample__].isOrtho		= fnCamera.isOrtho();
				iter__->camera[sample__].name           = fnCamera.fullPathName();
				getCameraFilmOffset( fnCamera, iter__->camera[sample__] );

				// convert focal length to scene units
				MDistance flenDist(iter__->camera[sample__].focalLength,MDistance::kMillimeters);
				iter__->camera[sample__].focalLength = flenDist.as(MDistance::uiUnit());
				getCameraTransform( fnCamera, iter__->camera[sample__] );
				//////////////////////////////////////////////////////////////////////////
				//[refactor 12.1] begin from liqRibTranslator::scanScene()
				// check stereo
				MString camType = fnCamera.typeName();
				bool isStereoCamera = false;
				if( camType == "stereoRigCamera" )
				{
					isStereoCamera = true;
					structCamera centralCameraPath = iter__->camera[sample__];
					// look for right and left cams
					MObject camTransform = fnCamera.parent(0, &status);
					if(status!=MS::kSuccess)
					{
						char errorMsg[512];
						sprintf(errorMsg, "Cannot find transform for camera %s.", fnCamera.name().asChar());
						//liquidMessage(errorMsg, messageError );
						printf(errorMsg);
						return MS::kFailure;
					}
					MFnDagNode fnCamTransform(camTransform, &status);
					if(status!=MS::kSuccess)
					{
						char errorMsg[512];
						sprintf(errorMsg, "Cannot init MFnDagNode for camera %s's transform.", fnCamera.name().asChar());
						//liquidMessage(errorMsg, messageError );
						printf(errorMsg);
						return MS::kFailure;
					}
					// get left one
					cPlug = fnCamTransform.findPlug( MString( "leftCam" ), &status );
					if(status!=MS::kSuccess)
					{
						char errorMsg[512];
						sprintf(errorMsg, "Cannot find plug 'leftCam' on %s", fnCamTransform.name().asChar());
						//liquidMessage(errorMsg, messageError );
						printf(errorMsg);
						return MS::kFailure;
					}
					MPlugArray plugArray;
					cPlug.connectedTo(plugArray, 1, 0, &status);
					if( plugArray.length() == 0 )
					{
						char errorMsg[512];
						sprintf(errorMsg, "Nothing connected in %s.leftCam \n", fnCamTransform.name().asChar());
						//liquidMessage(errorMsg, messageError );
						printf(errorMsg);
						return MS::kFailure;
					}
					MPlug leftCamPlug = plugArray[0];
					MObject leftCamTransformNode = leftCamPlug.node();
					MFnTransform fnLeftTrCam(leftCamTransformNode, &status);
					if( status != MS::kSuccess )
					{
						char errorMsg[512];
						sprintf(errorMsg, "cannot init MFnTransfrom for left camera '%s' ...\n", leftCamPlug.name().asChar());
						//liquidMessage(errorMsg, messageError );
						printf(errorMsg);
						return MS::kFailure;
					}
					MObject leftCamNode = fnLeftTrCam.child(0);
					MFnCamera fnLeftCam(leftCamNode, &status);
					if( status != MS::kSuccess )
					{
						char errorMsg[512];
						sprintf(errorMsg, "cannot init MFnCamera for left camera '%s' ...\n", fnLeftTrCam.name().asChar());
						//liquidMessage(errorMsg, messageError );
						printf(errorMsg);
						return MS::kFailure;
					}

					// get right one
					cPlug = fnCamTransform.findPlug( MString( "rightCam" ), &status );
					if(status!=MS::kSuccess)
					{
						char errorMsg[512];
						sprintf(errorMsg, "Cannot find plug 'rightCam' on %s", fnCamTransform.name().asChar());
						//liquidMessage(errorMsg, messageError );
						printf(errorMsg);
						return MS::kFailure;
					}
					cPlug.connectedTo(plugArray, 1, 0, &status);
					if( plugArray.length() == 0 )
					{
						char errorMsg[512];
						sprintf(errorMsg, "Nothing connected in %s.rightCam \n", fnCamTransform.name().asChar());
						//liquidMessage(errorMsg, messageError );
						printf(errorMsg);
						return MS::kFailure;
					}
					MPlug rightCamPlug = plugArray[0];
					MObject rightCamTransformNode = rightCamPlug.node();

					MFnTransform fnRightTrCam(rightCamTransformNode, &status);
					if( status != MS::kSuccess )
					{
						char errorMsg[512];
						sprintf(errorMsg, "cannot init MFnTransfrom for right camera '%s' ...\n", rightCamPlug.name().asChar());
						//liquidMessage(errorMsg, messageError );
						printf(errorMsg);
						return MS::kFailure;
					}
					MObject rightCamNode = fnRightTrCam.child(0);
					MFnCamera fnRightCam(rightCamNode, &status);
					if( status != MS::kSuccess )
					{
						char errorMsg[512];
						sprintf(errorMsg, "cannot init MFnCamera for right camera '%s' ...\n", fnRightTrCam.name().asChar());
						//liquidMessage(errorMsg, messageError );
						printf(errorMsg);
						return MS::kFailure;
					}
					/////////////////////////////
					getCameraInfo( fnLeftCam, iter__->leftCamera[sample__] );
					iter__->leftCamera[sample__].orthoWidth     = fnLeftCam.orthoWidth();
					iter__->leftCamera[sample__].orthoHeight    = fnLeftCam.orthoWidth() * ((float)iter__->camera[sample__].height / (float)iter__->camera[sample__].width);
					iter__->leftCamera[sample__].focalLength    = fnLeftCam.focalLength();
					iter__->leftCamera[sample__].focalDistance  = fnLeftCam.focusDistance();
					iter__->leftCamera[sample__].fStop          = fnLeftCam.fStop();
					iter__->leftCamera[sample__].isOrtho		= fnLeftCam.isOrtho();
					iter__->leftCamera[sample__].name			= fnLeftCam.name();
					getCameraFilmOffset( fnLeftCam, iter__->leftCamera[sample__] );
					// convert focal length to scene units
					MDistance flenLDist(iter__->leftCamera[sample__].focalLength, MDistance::kMillimeters);
					iter__->leftCamera[sample__].focalLength = flenLDist.as(MDistance::uiUnit());
					getCameraTransform( fnLeftCam, iter__->leftCamera[sample__] );
					// scanScene: The camera's fov may not match the rendered image in Maya
					// if a film-fit is used. 'fov_ratio' is used to account for
					// this.
					//
					//iter->leftCamera[sample].hFOV = fnLeftCam.horizontalFieldOfView()/iter->leftCamera[sample].fov_ratio;
					iter__->leftCamera[sample__].hFOV   = iter__->camera[sample__].hFOV;
					iter__->leftCamera[sample__].neardb = iter__->camera[sample__].neardb;
					iter__->leftCamera[sample__].fardb  = iter__->camera[sample__].fardb;

					getCameraInfo( fnRightCam, iter__->rightCamera[sample__] );
					iter__->rightCamera[sample__].orthoWidth	= fnRightCam.orthoWidth();
					iter__->rightCamera[sample__].orthoHeight	= fnRightCam.orthoWidth() * ((float)iter__->camera[sample__].height / (float)iter__->camera[sample__].width);
					iter__->rightCamera[sample__].focalLength	= fnRightCam.focalLength();
					iter__->rightCamera[sample__].focalDistance	= fnRightCam.focusDistance();
					iter__->rightCamera[sample__].fStop			= fnRightCam.fStop();
					iter__->rightCamera[sample__].isOrtho		= fnRightCam.isOrtho();
					iter__->rightCamera[sample__].name			= fnRightCam.name();
					getCameraFilmOffset( fnRightCam, iter__->rightCamera[sample__] );
					// convert focal length to scene units
					MDistance flenRDist(iter__->rightCamera[sample__].focalLength, MDistance::kMillimeters);
					iter__->rightCamera[sample__].focalLength = flenRDist.as(MDistance::uiUnit());
					getCameraTransform( fnRightCam, iter__->rightCamera[sample__] );
					// scanScene: The camera's fov may not match the rendered image in Maya
					// if a film-fit is used. 'fov_ratio' is used to account for
					// this.
					//
					//iter->rightCamera[sample].hFOV = fnRightCam.horizontalFieldOfView()/iter->rightCamera[sample].fov_ratio;
					iter__->rightCamera[sample__].hFOV   = iter__->camera[sample__].hFOV;
					iter__->rightCamera[sample__].neardb = iter__->camera[sample__].neardb;
					iter__->rightCamera[sample__].fardb  = iter__->camera[sample__].fardb;

					iter__->camera[sample__].rightCam = &(iter__->rightCamera[sample__]);
					iter__->camera[sample__].leftCam  = &(iter__->leftCamera[sample__]);
				}//if( camType == "stereoRigCamera" ) 
				iter__->isStereoPass = isStereoCamera;
				iter__->aspectRatio  = liqglo.aspectRatio;
				//[refactor 12.1] end 
				//////////////////////////////////////////////////////////////////////////

				// scanScene: Determine what information to write out (RGB, alpha, zbuffer)
				//
				iter__->imageMode.clear();

				bool isOn;
				MPlug boolPlug;
				boolPlug = fnCamera.findPlug( "image" );

				boolPlug.getValue( isOn );
				if( isOn ) 
				{
					// We are writing RGB info
					//
					iter__->imageMode = "rgb";
					iter__->format = liqglo.outFormat;
				}
				boolPlug = fnCamera.findPlug( "mask" );
				boolPlug.getValue( isOn );
				if( isOn ) 
				{
					// We are writing alpha channel info
					//
					iter__->imageMode += "a";
					iter__->format = liqglo.outFormat;
				}
				boolPlug = fnCamera.findPlug( "depth" );
				boolPlug.getValue( isOn );
				if( isOn ) 
				{
					if ( !isStereoCamera  )
					{
						// We are writing z-buffer info
						//
						iter__->imageMode = "z";
						iter__->format = "zfile";
					}else
						liquidMessage( "Cannot render depth for stereo camera.", messageWarning );
				}// isOn && !isStereoCamera
	//[refactor 12] end
	return MS::kSuccess;
}
//
void liqRibTranslator::getLightData( vector<structJob>::iterator &iter__ , const int sample__)
{
	CM_TRACE_FUNC("liqRibTranslatorNew::getLightData(iter__,"<<sample__<<")");
	MStatus status;
	//[refactor 13] begin from liqRibTranslator::scanScene()
	// scanScene: doing shadow render
	//
	MDagPath lightPath;
	MFnLight   fnLight( iter__->path );
	status.clear();

	iter__->gotJobOptions = false;
	if ( liquidGetPlugValue( fnLight, "ribOptions", iter__->jobOptions, status ) ==  MS::kSuccess )
		iter__->gotJobOptions = true;
	iter__->isStereoPass = false;
	// philippe: this block is obsolete as we now get the resolution when building the job list
	//
	/* MPlug lightPlug = fnLight.findPlug( "dmapResolution" );
	float dmapSize;
	lightPlug.getValue( dmapSize );
	iter__->height = iter__->width = (int)dmapSize; */

	if( iter__->hasShadowCam ) 
	{
		// scanScene: the light uses a shadow cam
		//
		MFnCamera fnCamera( iter__->shadowCamPath );
		fnCamera.getPath(lightPath);
		MTransformationMatrix xform( lightPath.inclusiveMatrix() );

		// the camera is pointing toward negative Z
		double scale[] = { 1, 1, -1 };
		xform.setScale( scale, MSpace::kTransform );

		iter__->camera[sample__].mat         = xform.asMatrixInverse();
		iter__->camera[sample__].neardb      = fnCamera.nearClippingPlane();
		iter__->camera[sample__].fardb       = fnCamera.farClippingPlane();
		iter__->camera[sample__].isOrtho     = fnCamera.isOrtho();
		iter__->camera[sample__].orthoWidth  = fnCamera.orthoWidth();
		iter__->camera[sample__].orthoHeight = fnCamera.orthoWidth();
		iter__->camera[sample__].name        = fnCamera.fullPathName();

	} //if( iter->hasShadowCam )
	else 
	{
		// scanScene: the light does not use a shadow cam
		//

		// get the camera world matrix
		fnLight.getPath(lightPath);
		MTransformationMatrix xform( lightPath.inclusiveMatrix() );

		// the camera is pointing toward negative Z
		double scale[] = { 1, 1, -1 };
		xform.setScale( scale, MSpace::kTransform );

		if( iter__->isPoint ) 
		{
			double ninty = M_PI/2;
			if( iter__->pointDir == pPX ) { double rotation[] = { 0, -ninty, 0 }; xform.setRotation( rotation, MTransformationMatrix::kXYZ ); }
			if( iter__->pointDir == pNX ) { double rotation[] = { 0, ninty, 0 }; xform.setRotation( rotation, MTransformationMatrix::kXYZ ); }
			if( iter__->pointDir == pPY ) { double rotation[] = { ninty, 0, 0 }; xform.setRotation( rotation, MTransformationMatrix::kXYZ ); }
			if( iter__->pointDir == pNY ) { double rotation[] = { -ninty, 0, 0 }; xform.setRotation( rotation, MTransformationMatrix::kXYZ ); }
			if( iter__->pointDir == pPZ ) { double rotation[] = { 0, M_PI, 0 }; xform.setRotation( rotation, MTransformationMatrix::kXYZ ); }
			if( iter__->pointDir == pNZ ) { double rotation[] = { 0, 0, 0 }; xform.setRotation( rotation, MTransformationMatrix::kXYZ ); }
		}
		iter__->camera[sample__].mat = xform.asMatrixInverse();


		MPlug shaderConnection( fnLight.findPlug( "liquidLightShaderNode", &status ) );
		if( status == MS::kSuccess && shaderConnection.isConnected() ) 
		{
			MPlugArray LightShaderPlugArray;
			shaderConnection.connectedTo( LightShaderPlugArray, true, true );
			MFnDependencyNode fnLightShaderNode( LightShaderPlugArray[0].node() );
			fnLightShaderNode.findPlug( "nearClipPlane" ).getValue( iter__->camera[sample__].neardb );
			fnLightShaderNode.findPlug( "farClipPlane" ).getValue( iter__->camera[sample__].fardb );
		} 
		else 
		{
			iter__->camera[sample__].neardb   = 0.001;    // TODO: these values are duplicated elsewhere in this file
			iter__->camera[sample__].fardb    = 250000.0; // TODO: these values are duplicated elsewhere in this file
			liquidGetPlugValue( fnLight, "nearClipPlane", iter__->camera[sample__].neardb, status );
			liquidGetPlugValue( fnLight, "farClipPlane", iter__->camera[sample__].fardb, status );
		}

		if( fnLight.dagPath().hasFn( MFn::kDirectionalLight ) ) 
		{
			iter__->camera[sample__].isOrtho = true;
			liquidGetPlugValue( fnLight, "dmapWidthFocus", iter__->camera[sample__].orthoWidth, status );
			liquidGetPlugValue( fnLight, "dmapWidthFocus", iter__->camera[sample__].orthoHeight, status );
		} 
		else 
		{
			iter__->camera[sample__].isOrtho = false;
			iter__->camera[sample__].orthoWidth = 0.0;
		}
	}//if( iter->hasShadowCam )else

	if( iter__->shadowType == stDeep )
	{
		iter__->camera[sample__].shutter = liqglo.liqglo_shutterTime;
		iter__->camera[sample__].motionBlur = true;
	}
	else
	{
		iter__->camera[sample__].shutter = 0;
		iter__->camera[sample__].motionBlur = false;
	}
	iter__->camera[sample__].focalLength = 0;
	iter__->camera[sample__].focalDistance = 0;
	iter__->camera[sample__].fStop = 0;
	iter__->camera[sample__].name= fnLight.fullPathName();
	//doCameraMotion = 0;

	iter__->aspectRatio = 1.0;

	// The camera's fov may not match the rendered image in Maya
	// if a film-fit is used. 'fov_ratio' is used to account for
	// this.
	//
	if( iter__->hasShadowCam ) 
	{
		MFnCamera fnCamera( iter__->shadowCamPath );
		float camFov = fnCamera.horizontalFieldOfView();
		iter__->camera[sample__].hFOV = camFov;
	} 
	else 
	{
		float angle = 0, penumbra = 0;
		if ( liquidGetPlugValue( fnLight, "coneAngle", angle, status ) ==  MS::kSuccess )
		{
			liquidGetPlugValue( fnLight, "penumbraAngle", penumbra, status );
			if ( penumbra > 0 ) 
				angle += penumbra * 2;
			iter__->camera[sample__].hFOV = angle;
		} 
		else 
			iter__->camera[sample__].hFOV = 95;
	}
	/*
	// Determine what information to write out ( depth map or deep shadow )
	//
	iter__->imageMode.clear();
	if( iter__->deepShadows )
	{
		iter__->imageMode    += liqglo.liquidRenderer.dshImageMode;        //"deepopacity";
		iter__->format        =  liqglo.liquidRenderer.dshDisplayName;    //"deepshad";
	}
	else
	{
		iter__->imageMode += "z";
		iter__->format = "shadow";
	}*/
	//[refactor 13] end
}
//
// MString liqRibTranslator::getBaseShadowName(const structJob &job__)
// {
// 	MString     baseShadowName___;
// 	// build the shadow archive name for the job
//  // r773 use this section
// 	bool renderAllFrames( job__.everyFrame );
// 	long refFrame( job__.renderFrame );
// 	MString geoSet( job__.shadowObjectSet );
// 	baseShadowName___ = generateShadowArchiveName( renderAllFrames, refFrame, geoSet );
// 	baseShadowName___ = liquidGetRelativePath( liqglo.liqglo_relativeFileNames, baseShadowName___, liqglo.liqglo_ribDir );
// 
// 	return baseShadowName___;
// }
//
liqRibTranslator* liqRibTranslator::getInstancePtr()
{
	assert(mInstance);
	return mInstance;
}
//
MStatus liqRibTranslator::ribPrologue__(const structJob &currentJob)
{
	CM_TRACE_FUNC("liqRibTranslatorNew::ribPrologue__("<<currentJob.name.asChar()<<")");
	if( liqglo.m_exportReadArchive ) 
	{
		ribStatus = kRibBegin;
		return MS::kSuccess;
	}

	liquid::RendererMgr::getInstancePtr()->getRenderer()->ribPrologue_begin(currentJob);

	//user
	char* user = NULL;
#ifndef _WIN32
	uid_t userId = getuid();
	struct passwd *userPwd = getpwuid( userId );
	user = userPwd->pw_name;
#else
	user = getenv("USERNAME");
#endif

	//time
	time_t now;
	time( &now );

	liquid::RendererMgr::getInstancePtr()->getRenderer()->ribPrologue_comment(
		LIQUIDVERSION,
		(liqglo.liqglo_projectDir + liquidTransGetSceneName()).asChar(),
		user, now
		);


	liquid::RendererMgr::getInstancePtr()->getRenderer()->ribPrologue_options(currentJob);

	ribStatus = kRibBegin;
	return MS::kSuccess;
}

//
MStatus liqRibTranslator::ribEpilogue__(const structJob &currentJob)
{
	CM_TRACE_FUNC("liqRibTranslatorNew::ribEpilogue__("<<currentJob.name.asChar()<<")");
	liquid::RendererMgr::getInstancePtr()->getRenderer()->ribPrologue_end(currentJob);

	if(ribStatus == kRibBegin) 
		ribStatus = kRibOK;
	return (ribStatus == kRibOK ? MS::kSuccess : MS::kFailure);
}
//
MStatus liqRibTranslator::framePrologue__( long lframe, const structJob &currentJob)
{
	CM_TRACE_FUNC("liqRibTranslatorNew::framePrologue__("<<lframe<<","<<currentJob.name.asChar()<<")");
	LIQDEBUGPRINTF( "-> Beginning Frame Prologue__\n" );
	ribStatus = kRibFrame;

	if( liqglo.m_exportReadArchive )
	{
		return MS::kSuccess;
	}
	liquid::RendererMgr::getInstancePtr()->getRenderer()->framePrologue(lframe, currentJob);



	return MS::kSuccess;
}
//
MStatus liqRibTranslator::worldPrologue__(const structJob &currentJob)
{
	CM_TRACE_FUNC("liqRibTranslatorNew::worldPrologue__("<<currentJob.name.asChar()<<")");
	return liquid::RendererMgr::getInstancePtr()->getRenderer()->worldPrologue(currentJob);
}

//
MStatus liqRibTranslator::lightBlock__(const structJob &currentJob)
{
	CM_TRACE_FUNC("liqRibTranslatorNew::lightBlock__("<<currentJob.name.asChar()<<")");
	MStatus returnStatus = MS::kSuccess;
	LIQDEBUGPRINTF( "-> Writing lights.\n" );
	// If this is a readArchive that we are exporting then ignore this header information
	if( !liqglo.m_exportReadArchive )
	{
		RNMAP::iterator rniter;
		int nbLight = 0;
		for ( rniter = htable->RibNodeMap.begin(); rniter != htable->RibNodeMap.end(); rniter++ )
		{
			LIQ_CHECK_CANCEL_REQUEST;
			liqRibNodePtr   ribNode = (*rniter).second;

			if( ribNode->object(0)->ignore || ribNode->object(0)->type != MRT_Light )
				continue;

			//liquid::RendererMgr::getInstancePtr()->getRenderer()->exportLight(ribNode, currentJob);
			liquid::RendererInterface* ri = liquid::RendererMgr::getInstancePtr()->getRenderer();
			if( ri->writeLight_pre(ribNode, currentJob) )
			{
				ribNode->object(0)->writeObject("", currentJob, false);//call liqRibLightData::_write(...)
				ribNode->object(0)->written = 1;

				ri->writeLight_post(ribNode, currentJob);
			}

			nbLight++;
		}
	}
	return returnStatus;
}

//
MStatus liqRibTranslator::coordSysBlock__(const structJob &currentJob)
{
	CM_TRACE_FUNC("liqRibTranslatorNew::coordSysBlock__("<<currentJob.name.asChar()<<")");
	
	//MStatus returnStatus = MS::kSuccess;
	//LIQDEBUGPRINTF( "-> Writing coordinate systems.\n" );
	//RNMAP::iterator rniter;
	//for ( rniter = htable->RibNodeMap.begin(); rniter != htable->RibNodeMap.end(); rniter++ ) 
	//{
	//	LIQ_CHECK_CANCEL_REQUEST;
	//	liqRibNodePtr   ribNode = (*rniter).second;
	//	if( ribNode->object(0)->ignore || ribNode->object(0)->type != MRT_Coord ) 
	//		continue;
	//	if( liqglo.m_outputComments )
	//		RiArchiveRecord( RI_COMMENT, "Name: %s", ribNode->name.asChar(), RI_NULL );

	//	RiAttributeBegin();
	//	RiAttribute( "identifier", "name", &getLiquidRibName( ribNode->name.asChar() ), RI_NULL );

	//	liqMatrix ribMatrix;
	//	MMatrix matrix;
	//	MDagPath path;

	//	matrix = ribNode->object(0)->matrix( path.instanceNumber() );
	//	matrix.get( ribMatrix );
	//	if( liqglo.liqglo_relativeTransforms ) 
	//		RiConcatTransform( ribMatrix ); 
	//	else 
	//		RiTransform( ribMatrix );

	//	ribNode->object(0)->writeObject("", currentJob, false);
	//	ribNode->object(0)->written = 1;
	//	RiAttributeEnd();
	//}
	//return returnStatus;
	return liquid::RendererMgr::getInstancePtr()->getRenderer()->coordSysBlock__(currentJob, htable);
}

//
//
MStatus liqRibTranslator::preGeometryMel()
{
	CM_TRACE_FUNC("liqRibTranslatorNew::preGeometryMel()");
// 	// Moritz: Added Pre-Geometry RIB for insertion right before any primitives
// 	MFnDependencyNode globalsNode( liqglo.rGlobalObj );
// 	MPlug prePostplug( globalsNode.findPlug( "preGeomMel" ) );
// 	MString melcommand( prePostplug.asString() );
// 	if( m_preGeomRIB != "" || melcommand.length() )
// 	{
// 		RiArchiveRecord( RI_COMMENT,  " Pre-Geometry RIB from liquid globals");
// 		MGlobal::executeCommand( melcommand );
// 		RiArchiveRecord( RI_VERBATIM, ( char* ) m_preGeomRIB.asChar() );
// 		RiArchiveRecord( RI_VERBATIM, "\n");
// 	}
// 	return MS::kSuccess;
	return liquid::RendererMgr::getInstancePtr()->getRenderer()->preGeometryMel(m_preGeomRIB);

}
MStatus liqRibTranslator::postGeometryMel()
{
	CM_TRACE_FUNC("liqRibTranslatorNew::postGeometryMel()");
	return MS::kSuccess;
}
// retrieve the shadow set object
MObject liqRibTranslator::getShadowSetObject(const structJob &currentJob)
{
	CM_TRACE_FUNC("liqRibTranslatorNew::getShadowSetObject("<<currentJob.name.asChar()<<")");
	MStatus status;
	MObject shadowSetObj;
	if( currentJob.pass == rpShadowMap && currentJob.shadowObjectSet != "" ) 
	{
		MObject tmp;
		tmp = getNodeByName( currentJob.shadowObjectSet, &status );
		if( status == MS::kSuccess ) 
			shadowSetObj = tmp;
		else 
		{
			MString warn;
			warn += "Liquid : set " + currentJob.shadowObjectSet + " in shadow " + currentJob.name + " does not exist !";
			MGlobal:: displayWarning( warn );
			printf("%s\n",warn.asChar() );
		}
		status.clear();
	}
	return shadowSetObj;
}
MStatus liqRibTranslator::preTransformMel(const MObject &transform__)
{
	CM_TRACE_FUNC("liqRibTranslatorNew::preTransformMel");
	// Alf: preTransformMel
	MFnDagNode fnTransform( transform__ );
	MPlug prePostPlug = fnTransform.findPlug( "liqPreTransformMel" );
	m_preTransformMel = prePostPlug.asString();

	if( m_preTransformMel != "" )
		MGlobal::executeCommand( m_preTransformMel );
	
	return MS::kSuccess;
}
MStatus liqRibTranslator::postTransformMel(const MObject &transform__)
{
	CM_TRACE_FUNC("liqRibTranslatorNew::postTransformMel("<<transform__.apiTypeStr()<<")");
	// Alf: postTransformMel
	MFnDagNode fnTransform( transform__ );
	MPlug prePostPlug = fnTransform.findPlug( "liqPostTransformMel" );
	m_postTransformMel = prePostPlug.asString();
	if( m_postTransformMel != "" )
		MGlobal::executeCommand( m_postTransformMel );
	return MS::kSuccess;
}
//
MStatus liqRibTranslator::tRiIlluminate(const structJob &currentJob__, const liqRibNodePtr ribNode__)
{
	CM_TRACE_FUNC("liqRibTranslatorNew::tRiIlluminate("<<currentJob__.name.asChar()<<","<<ribNode__->name.asChar()<<")");
	// Moritz: only write out light linking if we're not in a shadow pass
#if 0
// 		MObjectArray linkLights;
// 
// 		// light linking mode - Alf
// 		// inclusive - lights are off by default and objects list included lights
// 		// exclusive - lights are on by default and objects list ignored lights
// 		// liquid Light sets - ignores the maya light linker
// 		if( m_liquidSetLightLinking )
// 			ribNode__->getSetLights( linkLights );
// 		else
// 			ribNode__->getLinkLights( linkLights, liqglo.m_illuminateByDefault );
// 
// 		for( unsigned i( 0 ); i < linkLights.length(); i++ )
// 		{
// 			MFnDagNode lightFnDag( linkLights[i] );
// 			MString nodeName = lightFnDag.fullPathName();
// 			if( htable )
// 			{
// 				//RibNode * ln = htable->find( light, MRT_Light );
// 				MDagPath nodeDagPath;
// 				lightFnDag.getPath( nodeDagPath );
// 				liqRibNodePtr  ln( htable->find( lightFnDag.fullPathName(), nodeDagPath, MRT_Light ) );
// 				if( NULL != ln )
// 				{
// 					liquid::RendererMgr::getInstancePtr()->
// 						getRenderer()->exportLightLinks(
// 						currentJob__, ribNode__, ln, liqglo.m_illuminateByDefault);
// 				}
// 			}
// 		}
#else
	MString mesh(ribNode__->name);

	MStringArray lights;
	IfMErrorWarn(MGlobal::executeCommand( "lightlink -q -set 0 -t 0 -object "+mesh, lights));
	toFullDagPath(lights);

	liquid::RendererMgr::getInstancePtr()->
		getRenderer()->exportLightLinks(
		currentJob__, ribNode__, lights);

#endif
	return MS::kSuccess;
}
//
//MStatus liqRibTranslator::MaxtrixMotionBlur(const liqRibNodePtr ribNode__, MDagPath &path__, const bool bMotionBlur)
//{
//	CM_TRACE_FUNC("liqRibTranslatorNew::MaxtrixMotionBlur("<<ribNode__->name.asChar()<<","<<path__.fullPathName().asChar()<<","<<bMotionBlur<<")");
//	MMatrix matrix;
//
//	//refactor 30 from liqRibTranslator::objectBlock()
//	if( bMotionBlur)
//	{
//		LIQDEBUGPRINTF( "-> writing matrix motion blur data\n" );
//		// Moritz: replaced RiMotionBegin call with ..V version to allow for more than five motion samples
//		if(liqglo.liqglo_relativeMotion)
//			RiMotionBeginV( liqglo.liqglo_motionSamples, liqglo.liqglo_sampleTimesOffsets );
//		else
//			RiMotionBeginV( liqglo.liqglo_motionSamples, liqglo.liqglo_sampleTimes );
//	}
//#if 1
//	liqMatrix ribMatrix;
//	matrix = ribNode__->object( 0 )->matrix( path__.instanceNumber() );
//	matrix.get( ribMatrix );
//
//	if( liqglo.liqglo_relativeTransforms ) 
//		RiConcatTransform( ribMatrix );
//	else 
//		RiTransform( ribMatrix );
//#elif 0  // Bat : a way to have double transforms :
//	double doubleTransformMatrix[4][4];
//	matrix = ribNode->object( 0 )->matrix( path.instanceNumber() );
//	matrix.get( doubleTransformMatrix );
//
//	int txIntPart = (int)(doubleTransformMatrix[3][0]);
//	float txFloatPart = doubleTransformMatrix[3][0] - txIntPart;
//
//	int tyIntPart = (int)(doubleTransformMatrix[3][1]);
//	float tyFloatPart = doubleTransformMatrix[3][1] - tyIntPart;
//
//	int tzIntPart = (int)(doubleTransformMatrix[3][2]);
//	float tzFloatPart = doubleTransformMatrix[3][2] - tzIntPart;
//
//	liqFloat floatTransformMatrixWithIntegerTranslatePart[4][4];
//	matrix.get( floatTransformMatrixWithIntegerTranslatePart );
//	liqFloat floatIdentityMatrixWithFloatingTranslatePart[4][4] = { {1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1} };
//
//	floatTransformMatrixWithIntegerTranslatePart[3][0] = txIntPart;
//	floatTransformMatrixWithIntegerTranslatePart[3][1] = tyIntPart;
//	floatTransformMatrixWithIntegerTranslatePart[3][2] = tzIntPart;
//
//	floatIdentityMatrixWithFloatingTranslatePart[3][0] = txFloatPart;
//	floatIdentityMatrixWithFloatingTranslatePart[3][1] = tyFloatPart;
//	floatIdentityMatrixWithFloatingTranslatePart[3][2] = tzFloatPart;
//
//	if( liqglo_relativeTransforms )
//	{
//		RiConcatTransform( floatIdentityMatrixWithFloatingTranslatePart );
//		RiConcatTransform( floatTransformMatrixWithIntegerTranslatePart );
//	}
//	else
//	{
//		RiTransform( floatIdentityMatrixWithFloatingTranslatePart );
//		RiConcatTransform( floatTransformMatrixWithIntegerTranslatePart );
//	}
//#endif
//	// Output the world matrices for the motionblur
//	// This will override the current transformation setting
//	if( bMotionBlur)
//	{
//		path__ = ribNode__->path();
//		liqMatrix ribMatrix;
//		for( unsigned mm( 1 ); mm < liqglo.liqglo_motionSamples; mm++ ) {
//			matrix = ribNode__->object( mm )->matrix( path__.instanceNumber() );
//			matrix.get( ribMatrix );
//			if( liqglo.liqglo_relativeTransforms ) 
//				RiConcatTransform( ribMatrix );
//			else 
//				RiTransform( ribMatrix );
//		}
//		RiMotionEnd();
//	}
//	//refactor 30 end
//	return MS::kSuccess;
//}
MStatus liqRibTranslator::preShapeMel(const MObject &transform__)
{
	CM_TRACE_FUNC("liqRibTranslatorNew::preShapeMel("<<transform__.apiTypeStr()<<")");
	// Alf: preShapeMel
	MFnDagNode fnTransform( transform__ );
	MPlug prePostPlug = fnTransform.findPlug( "liqPreShapeMel" );
	m_preShapeMel = prePostPlug.asString();
	if( m_preShapeMel != "" )
		MGlobal::executeCommand( m_preShapeMel );
	return MS::kSuccess;
}
MStatus liqRibTranslator::postShapeMel(const MObject &transform__)
{		
	CM_TRACE_FUNC("liqRibTranslatorNew::postShapeMel("<<transform__.apiTypeStr()<<")");	
	// Alf: postShapeMel
	MFnDagNode fnTransform( transform__ );
	MPlug prePostPlug = fnTransform.findPlug( "liqPostShapeMel" );
	m_postShapeMel = prePostPlug.asString();
	if( m_postShapeMel != "" )
		MGlobal::executeCommand( m_postShapeMel );

	return MS::kSuccess;
}
MStatus liqRibTranslator::checkSurfaceShader(
	const MDagPath &path__,
	const liqRibNodePtr &ribNode__,
	bool &hasSurfaceShader__,
	liqDetailShaderKind &hasCustomSurfaceShader__,
	 MString &shaderRibBox__)
{
	CM_TRACE_FUNC("liqRibTranslatorNew::checkSurfaceShader("<<path__.fullPathName().asChar()<<","<<ribNode__->name.asChar()<<",,,"<<")");
	MStatus status;

	MPlug rmanShaderPlug;
	// Check for surface shader
	status.clear();
	MFnDagNode fnDagNode( path__ );
	rmanShaderPlug = fnDagNode.findPlug( MString( "liquidSurfaceShaderNode" ), &status );
	if( ( status != MS::kSuccess ) || ( !rmanShaderPlug.isConnected() ) ) 
	{ 
		status.clear(); 
		rmanShaderPlug = ribNode__->assignedShadingGroup.findPlug( MString( "liquidSurfaceShaderNode" ), &status ); 
		MString plugname(rmanShaderPlug.name());
	}
	if( ( status != MS::kSuccess ) || ( !rmanShaderPlug.isConnected() ) ) 
	{ 
		status.clear(); 
		rmanShaderPlug = ribNode__->assignedShader.findPlug( MString( "liquidSurfaceShaderNode" ), &status ); 
		MString plugname(rmanShaderPlug.name());
	}
	if( ( status != MS::kSuccess ) || ( !rmanShaderPlug.isConnected() ) ) 
	{ 
		status.clear(); 
		rmanShaderPlug = ribNode__->assignedShadingGroup.findPlug( MString( "surfaceShader" ), &status ); 
		MString plugname(rmanShaderPlug.name());
	}
	if( status == MS::kSuccess && !rmanShaderPlug.isNull() ) 
	{
		if( rmanShaderPlug.isConnected() ) 
		{
			MPlugArray rmShaderNodeArray;
			rmanShaderPlug.connectedTo( rmShaderNodeArray, true, true );
			MObject rmShaderNodeObj;
			rmShaderNodeObj = rmShaderNodeArray[0].node();
			MFnDependencyNode shaderDepNode( rmShaderNodeObj );
			//std::cout <<"path="<<path.fullPathName()<<", shaderDepNode.typeName()="<<shaderDepNode.typeName()<<std::endl;
			// philippe : we must check the node type to avoid checking in regular maya shaders
			if( shaderDepNode.typeName() == "liquidSurface" 
				|| shaderDepNode.typeName() == "liquidShader" 
				|| shaderDepNode.typeName() == "oldBlindDataBase" ) 
			{ //cout <<"setting shader"<<endl;
				ribNode__->assignedShader.setObject( rmShaderNodeObj );
				hasSurfaceShader__ = true;
			} 
			else 
			{ // Is it a custom shading node ?
				MPxNode *mpxNode = shaderDepNode.userNode();
				liqCustomNode *customNode = NULL;
				if( mpxNode && ( customNode = dynamic_cast<liqCustomNode*>(mpxNode) ) ) 
				{ // customNode will be null if can't be casted to a liqCustomNode
					ribNode__->assignedShader.setObject( rmShaderNodeObj );
					hasSurfaceShader__ = true;
					hasCustomSurfaceShader__ = liqCustomPxShaderNode;
				} 
				else 
				{ // Try to find a liqRIBBox attribute
					MPlug ribbPlug = shaderDepNode.findPlug( MString( "liqRIBBox" ), &status );
					if( status == MS::kSuccess ) 
					{
						ribbPlug.getValue( shaderRibBox__ );
						if( shaderRibBox__.substring(0,2) == "*H*" ) 
						{
							MString parseThis = shaderRibBox__.substring(3, shaderRibBox__.length() - 1 );
							shaderRibBox__ = parseThis;
						} 
						else if( shaderRibBox__.substring(0,3) == "*SH*" ) 
						{
							MString parseThis = shaderRibBox__.substring(3, shaderRibBox__.length() - 1 );
							shaderRibBox__ = parseThis;
						}
						hasSurfaceShader__ = true;
						hasCustomSurfaceShader__ = liqRibBoxShader;
					}
					// else {
					//liquidMessage( "Do noy know how to handle " + string( shaderDepNode.typeName().asChar() ), messageError );
					//}
				}
			}
		}
	}
	return MS::kSuccess;
}
//
MStatus liqRibTranslator::checkDisplacementShader(
								const MDagPath &path__,
								const liqRibNodePtr &ribNode__,
								bool &hasDisplacementShader__ 
								//liqDetailShaderKind &hasCustomSurfaceShader__,
								//MString &shaderRibBox__
								)
{	
	CM_TRACE_FUNC("liqRibTranslatorNew::checkDisplacementShader("<<path__.fullPathName().asChar()<<","<<ribNode__->name.asChar()<<",,,"<<")");
	MStatus status;
	MPlug rmanShaderPlug;
	MFnDagNode fnDagNode( path__ );

	status.clear();
	rmanShaderPlug = fnDagNode.findPlug( MString( "liquidDispShaderNode" ), &status );
	if( ( status != MS::kSuccess ) || ( !rmanShaderPlug.isConnected() ) ) 
	{ 
		status.clear(); 
		rmanShaderPlug = ribNode__->assignedShadingGroup.findPlug( MString( "liquidDispShaderNode" ), &status ); 
	}
	if( ( status != MS::kSuccess ) || ( !rmanShaderPlug.isConnected() ) ) 
	{ 
		status.clear(); 
		rmanShaderPlug = ribNode__->assignedDisp.findPlug( MString( "liquidDispShaderNode" ), &status ); 
	}
	if( ( status != MS::kSuccess ) || ( !rmanShaderPlug.isConnected() ) ) 
	{ 
		status.clear(); 
		rmanShaderPlug = ribNode__->assignedShadingGroup.findPlug( MString( "displacementShader" ), &status ); 
	}
	if( ( status == MS::kSuccess ) && !rmanShaderPlug.isNull() && !m_ignoreDisplacements ) 
	{
		if( rmanShaderPlug.isConnected() ) 
		{
			MPlugArray rmShaderNodeArray;
			rmanShaderPlug.connectedTo( rmShaderNodeArray, true, true );
			MObject rmShaderNodeObj;
			rmShaderNodeObj = rmShaderNodeArray[0].node();
			MFnDependencyNode shaderDepNode( rmShaderNodeObj );
			// philippe : we must check the node type to avoid checking in regular maya shaders
			if( shaderDepNode.typeName() == "liquidDisplacement" 
//added in r773 || shaderDepNode.typeName() == "liquidDisplacementSwitcher"
				|| shaderDepNode.typeName() == "oldBlindDataBase" ) 
			{
				ribNode__->assignedDisp.setObject( rmShaderNodeObj );
				hasDisplacementShader__ = true;
			}
		}
	}
	return MS::kSuccess;
}
//
MStatus liqRibTranslator::checkVolumeShader(
						  const MDagPath &path__, 
						  const liqRibNodePtr &ribNode__,
						  bool &hasVolumeShader__ 
						  //liqDetailShaderKind &hasCustomSurfaceShader__,
						  //MString &shaderRibBox__
						  )
{	
	CM_TRACE_FUNC("liqRibTranslatorNew::checkVolumeShader("<<path__.fullPathName().asChar()<<","<<ribNode__->name.asChar()<<",,,"<<")");
	MStatus status;
	MPlug rmanShaderPlug;
	MFnDagNode fnDagNode( path__ );

	status.clear();
	rmanShaderPlug = fnDagNode.findPlug( MString( "liquidVolumeShaderNode" ), &status );
	if( ( status != MS::kSuccess ) || ( !rmanShaderPlug.isConnected() ) ) 
	{ 
		status.clear(); 
		rmanShaderPlug = ribNode__->assignedShadingGroup.findPlug( MString( "liquidVolumeShaderNode" ), &status ); 
	}
	if( ( status != MS::kSuccess ) || ( !rmanShaderPlug.isConnected() ) ) 
	{ 
		status.clear(); 
		rmanShaderPlug = ribNode__->assignedVolume.findPlug( MString( "liquidVolumeShaderNode" ), &status ); 
	}
	if( ( status != MS::kSuccess ) || ( !rmanShaderPlug.isConnected() ) ) 
	{ 
		status.clear(); 
		rmanShaderPlug = ribNode__->assignedShadingGroup.findPlug( MString( "volumeShader" ), &status ); 
	}
	if( ( status == MS::kSuccess ) && !rmanShaderPlug.isNull() && !m_ignoreVolumes ) 
	{
		if( rmanShaderPlug.isConnected() ) 
		{
			MPlugArray rmShaderNodeArray;
			rmanShaderPlug.connectedTo( rmShaderNodeArray, true, true );
			MObject rmShaderNodeObj;
			rmShaderNodeObj = rmShaderNodeArray[0].node();
			MFnDependencyNode shaderDepNode( rmShaderNodeObj );
			// philippe : we must check the node type to avoid checking in regular maya shaders
			if( shaderDepNode.typeName() == "liquidVolume" || shaderDepNode.typeName() == "oldBlindDataBase" ) 
			{
				ribNode__->assignedVolume.setObject( rmShaderNodeObj );
				hasVolumeShader__ = true;
			}
		}
	}
	return MS::kSuccess;
}
//
MStatus liqRibTranslator::displacementBounds(const liqRibNodePtr &ribNode__)
{
	CM_TRACE_FUNC("liqRibTranslatorNew::displacementBounds("<<ribNode__->name.asChar()<<")");
	MStatus status;
// 	displacement bounds
// 		float surfaceDisplacementBounds = 0.0;
// 		MString surfaceDisplacementBoundsSpace = "shader";
// 		MString tmpSpace = "";
// 		status.clear();
// 		if( !ribNode__->assignedShader.object().isNull() ) 
// 		{
// 			MPlug sDBPlug = ribNode__->assignedShader.findPlug( MString( "displacementBound" ), &status );
// 			if( status == MS::kSuccess ) 
// 				sDBPlug.getValue( surfaceDisplacementBounds );
// 			MPlug sDBSPlug = ribNode__->assignedShader.findPlug( MString( "displacementBoundSpace" ), &status );
// 			if( status == MS::kSuccess ) 
// 				sDBSPlug.getValue( tmpSpace );
// 			if( tmpSpace != "" ) 
// 				surfaceDisplacementBoundsSpace = tmpSpace;
// 		}
// 		float dispDisplacementBounds = 0.0;
// 		MString dispDisplacementBoundsSpace = "shader";
// 		tmpSpace = "";
// 		status.clear();
// 		if( !ribNode__->assignedDisp.object().isNull() ) 
// 		{
// 			MPlug dDBPlug = ribNode__->assignedDisp.findPlug( MString( "displacementBound" ), &status );
// 			if( status == MS::kSuccess ) 
// 				dDBPlug.getValue( dispDisplacementBounds );
// 			MPlug sDBSPlug = ribNode__->assignedDisp.findPlug( MString( "displacementBoundSpace" ), &status );
// 			if( status == MS::kSuccess ) 
// 				sDBSPlug.getValue( tmpSpace );
// 			if( tmpSpace != "" ) 
// 				dispDisplacementBoundsSpace = tmpSpace;
// 		}
// 		if( ( dispDisplacementBounds != 0.0 ) && ( dispDisplacementBounds > surfaceDisplacementBounds ) ) 
// 		{
// 			liqString coordsys( const_cast< char* >( dispDisplacementBoundsSpace.asChar() ) );
// 			RiAttribute( "displacementbound", (liqToken) "sphere", &dispDisplacementBounds, "coordinatesystem", &coordsys, RI_NULL );
// 		} 
// 		else if( ( surfaceDisplacementBounds != 0.0 ) ) 
// 		{
// 			liqString coordsys( const_cast< char* >( surfaceDisplacementBoundsSpace.asChar() ) );
// 			RiAttribute( "displacementbound", (liqToken) "sphere", &surfaceDisplacementBounds, "coordinatesystem", &coordsys, RI_NULL );
// 		}
	return MS::kSuccess;
}
//
MStatus liqRibTranslator::objectShadowAttribute(const liqRibNodePtr &ribNode__)
{
	CM_TRACE_FUNC("liqRibTranslatorNew::objectShadowAttribute("<<ribNode__->name.asChar()<<")");

	return MS::kSuccess;
}

//MStatus liqRibTranslator::objectNonShadowAttribute(const liqRibNodePtr &ribNode__)
//{
//	CM_TRACE_FUNC("liqRibTranslatorNew::objectNonShadowAttribute("<<ribNode__->name.asChar()<<")");
//
//	liqString mode;
//	liqInt off( 0 );
//	liqInt on( 1 );
//	if ( !m_skipShadingAttributes )
//	{
//	if( !ribNode__->shading.diceRasterOrient ) 
//		RiAttribute( "dice", (liqToken) "rasterorient", &off, RI_NULL );
//	if( ribNode__->shading.doubleShaded ) 
//		RiAttribute( "sides", (liqToken) "doubleshaded", &on, RI_NULL );
//	}
//	if( !m_skipRayTraceAttributes )
//	{
//		if( liqglo.liquidRenderer.supports_RAYTRACE ) 
//		{
//			if( ribNode__->trace.sampleMotion ) 
//				RiAttribute( "trace", (liqToken) "samplemotion", &on, RI_NULL );
//			if( ribNode__->trace.displacements ) 
//				RiAttribute( "trace", (liqToken) "displacements", &on, RI_NULL );
//			if( ribNode__->trace.bias != 0.01f ) 
//			{
//				liqFloat bias( ribNode__->trace.bias );
//				RiAttribute( "trace", (liqToken) "bias", &bias, RI_NULL );
//			}
//			if( ribNode__->trace.maxDiffuseDepth != 1 ) 
//			{
//				liqInt mddepth( ribNode__->trace.maxDiffuseDepth );
//				RiAttribute( "trace", (liqToken) "maxdiffusedepth", &mddepth, RI_NULL );
//			}
//			if( ribNode__->trace.maxSpecularDepth != 2 ) 
//			{
//				liqInt msdepth( ribNode__->trace.maxSpecularDepth );
//				RiAttribute( "trace", (liqToken) "maxspeculardepth", &msdepth, RI_NULL );
//			}
//		}//if( liqglo.liquidRenderer.supports_RAYTRACE ) 
//	}//if( !m_skipRayTraceAttributes )
//	if( !ribNode__->visibility.camera ) 
//		RiAttribute( "visibility", (liqToken) "camera", &off, RI_NULL );
//#ifdef GENERIC_RIBLIB      
//	extern int useAdvancedVisibilityAttributes;
//	useAdvancedVisibilityAttributes = false;
//#endif
//	// old-style raytracing visibility support
//	// philippe: if raytracing is off in the globals, trace visibility is turned off for all objects, transmission is set to TRANSPARENT for all objects
//	if ( liqglo.liquidRenderer.supports_RAYTRACE )
//	{
//		if ( !liqglo.liquidRenderer.supports_ADVANCED_VISIBILITY ) 
//		{
//		if(liqglo.rt_useRayTracing)
//		{
//			if ( ribNode__->visibility.trace ) 
//				RiAttribute( "visibility", (liqToken) "trace", &on, RI_NULL );
//			else                             
//				RiAttribute( "visibility", (liqToken) "trace", &off, RI_NULL );
//
//			if( ribNode__->visibility.transmission != liqRibNode::visibility::TRANSMISSION_TRANSPARENT ) 
//			{
//				liqString trans;
//				switch( ribNode__->visibility.transmission ) 
//				{
//				case liqRibNode::visibility::TRANSMISSION_OPAQUE:
//					trans = "opaque";
//					break;
//				case liqRibNode::visibility::TRANSMISSION_OS:
//					trans = "Os";
//					break;
//				case liqRibNode::visibility::TRANSMISSION_SHADER:
//				default:
//					trans = "shader";
//				}
//				RiAttribute( "visibility", (liqToken) "string transmission", &trans, RI_NULL );
//			}
//		}
//
//		}//if ( !liqglo.liquidRenderer.supports_ADVANCED_VISIBILITY ) 
//	else // philippe : prman 12.5 visibility support
//	{
//#ifdef GENERIC_RIBLIB         
//		useAdvancedVisibilityAttributes = true;
//#endif
//	if ( liqglo.rt_useRayTracing )
//	{
//		if( !m_skipVisibilityAttributes )
//		{
//		if(  ribNode__->visibility.diffuse ) 
//			RiAttribute( "visibility", (liqToken) "int diffuse", &on, RI_NULL );
//
//		if(  ribNode__->visibility.specular ) 
//			RiAttribute( "visibility", (liqToken) "int specular", &on, RI_NULL );
//
//		if(  ribNode__->visibility.newtransmission ) 
//			RiAttribute( "visibility", (liqToken) "int transmission", &on, RI_NULL );
//
//		//if( ribNode__->visibility.camera ) 
//		//	RiAttribute( "visibility", (liqToken) "int camera", &on, RI_NULL );
//
//		//if(  ribNode__->visibility.photon ) 
//		//	RiAttribute( "visibility", (liqToken) "int photon", &on, RI_NULL );
//
//		//if( ribNode__->visibility.midpoint ) 
//		//	RiAttribute( "visibility", (liqToken) "int midpoint", &on, RI_NULL );
//		}
//
//		if( !m_skipRayTraceAttributes )
//		{
//			if( liqglo.rt_useRayTracing && ribNode__->hitmode.diffuse != liqRibNode::hitmode::DIFFUSE_HITMODE_PRIMITIVE ) 
//			{
//				switch( ribNode__->hitmode.diffuse ) 
//				{
//				case liqRibNode::hitmode::DIFFUSE_HITMODE_SHADER:
//					mode = "shader";
//					break;
//				case liqRibNode::hitmode::DIFFUSE_HITMODE_CACHE:
//					mode = "cache";
//					break;
//				case liqRibNode::hitmode::DIFFUSE_HITMODE_PRIMITIVE:
//				default:
//					mode = "primitive";
//				}
//				RiAttribute( "shade", (liqToken) "string diffusehitmode", &mode, RI_NULL );
//			}
//
//			if( ribNode__->hitmode.specular != liqRibNode::hitmode::SPECULAR_HITMODE_SHADER ) 
//			{
//				switch( ribNode__->hitmode.specular ) 
//				{
//				case liqRibNode::hitmode::SPECULAR_HITMODE_PRIMITIVE:
//					mode = "primitive";
//					break;
//				case liqRibNode::hitmode::SPECULAR_HITMODE_CACHE:
//					mode = "cache";
//					break;
//				case liqRibNode::hitmode::SPECULAR_HITMODE_SHADER:
//				default:
//					mode = "shader";
//				}
//				RiAttribute( "shade", (liqToken) "string specularhitmode", &mode, RI_NULL );
//			}
//
//			if( ribNode__->hitmode.transmission != liqRibNode::hitmode::TRANSMISSION_HITMODE_SHADER ) 
//			{
//				switch( ribNode__->hitmode.transmission ) 
//				{
//				case liqRibNode::hitmode::TRANSMISSION_HITMODE_PRIMITIVE:
//					mode = "primitive";
//					break;
//				case liqRibNode::hitmode::TRANSMISSION_HITMODE_CACHE:
//					mode = "cache";
//					break;
//				case liqRibNode::hitmode::TRANSMISSION_HITMODE_SHADER:
//				default:
//					mode = "shader";
//				}
//				RiAttribute( "shade", (liqToken) "string transmissionhitmode", &mode, RI_NULL );
//			}
//		}//if( !m_skipRayTraceAttributes )
//	}//if ( rt_useRayTracing )
//	if( !m_skipShadingAttributes )
//	{
//		if( ribNode__->hitmode.camera != liqRibNode::hitmode::CAMERA_HITMODE_SHADER ) 
//		{
//			switch( ribNode__->hitmode.camera ) 
//			{
//			case liqRibNode::hitmode::CAMERA_HITMODE_PRIMITIVE:
//				mode = "primitive";
//				break;
//			case liqRibNode::hitmode::CAMERA_HITMODE_CACHE:
//				mode = "cache";
//				break;
//			case liqRibNode::hitmode::CAMERA_HITMODE_SHADER:
//			default:
//				mode = "shader";
//			}
//			RiAttribute( "shade", (liqToken) "string camerahitmode", &mode, RI_NULL );
//		}
//	}
//	} //else philippe : prman 12.5 visibility support 
//	// irradiance attributes
//		if( ribNode__->irradiance.shadingRate != 1.0f ) 
//		{
//			liqFloat rate = ribNode__->irradiance.shadingRate;
//			RiAttribute( "irradiance", (liqToken) "shadingrate", &rate, RI_NULL );
//		}
//
//		if( ribNode__->irradiance.nSamples != 64 ) 
//		{
//			liqInt samples = ribNode__->irradiance.nSamples;
//			RiAttribute( "irradiance", (liqToken) "nsamples", &samples, RI_NULL );
//		}
//
//		if( ribNode__->irradiance.maxError != 0.5f ) 
//		{
//			liqFloat merror = ribNode__->irradiance.maxError;
//			RiAttribute( "irradiance", (liqToken) "float maxerror", &merror, RI_NULL );
//		}
//
//		if( ribNode__->irradiance.maxPixelDist != 30.0f ) 
//		{
//			liqFloat mpd = ribNode__->irradiance.maxPixelDist;
//			RiAttribute( "irradiance", (liqToken) "float maxpixeldist", &mpd, RI_NULL );
//		}
//
//		if( ribNode__->irradiance.handle != "" ) 
//		{
//			liqString handle = const_cast< char* >( ribNode__->irradiance.handle.asChar() );
//			RiAttribute( "irradiance", (liqToken) "handle", &handle, RI_NULL );
//		}
//
//		if( ribNode__->irradiance.fileMode != liqRibNode::irradiance::FILEMODE_NONE ) 
//		{
//			liqString mode;
//			switch( ribNode__->irradiance.fileMode ) 
//			{
//			case liqRibNode::irradiance::FILEMODE_READ:
//				mode = "r";
//				break;
//			case liqRibNode::irradiance::FILEMODE_WRITE:
//				mode = "w";
//				break;
//			case liqRibNode::irradiance::FILEMODE_READ_WRITE:
//			default:
//				mode = "rw";
//			}
//			RiAttribute( "irradiance", (liqToken) "filemode", &mode, RI_NULL );
//		}
//
//		// ymesh: photon visibility support
//		if ( liqglo.rt_useRayTracing && ribNode__->visibility.photon ) 
//			RiAttribute( "visibility", (liqToken) "int photon", &on, RI_NULL );
//
//		if( ribNode__->photon.globalMap != "" ) 
//		{
//			liqString map = const_cast< char* >( ribNode__->photon.globalMap.asChar() );
//			RiAttribute( "photon", (liqToken) "globalmap", &map, RI_NULL );
//		}
//
//		if( ribNode__->photon.causticMap != "" ) 
//		{
//			liqString map = const_cast< char* >( ribNode__->photon.causticMap.asChar() );
//			RiAttribute( "photon", (liqToken) "causticmap", &map, RI_NULL );
//		}
//
//		if( ribNode__->photon.shadingModel != liqRibNode::photon::SHADINGMODEL_MATTE ) 
//		{
//			switch( ribNode__->photon.shadingModel  ) 
//			{
//			case liqRibNode::photon::SHADINGMODEL_GLASS:
//				mode = "glass";
//				break;
//			case liqRibNode::photon::SHADINGMODEL_WATER:
//				mode = "water";
//				break;
//			case liqRibNode::photon::SHADINGMODEL_CHROME:
//				mode = "chrome";
//				break;
//			case liqRibNode::photon::SHADINGMODEL_TRANSPARENT:
//				mode = "chrome";
//				break;
//			case liqRibNode::photon::SHADINGMODEL_DIALECTRIC:
//				mode = "dielectric";
//				break;
//			case liqRibNode::photon::SHADINGMODEL_MATTE:
//			default:
//				mode = "matte";
//			}
//			RiAttribute( "photon", (liqToken) "shadingmodel", &mode, RI_NULL );
//		}
//
//		if( ribNode__->photon.estimator != 100 ) 
//		{
//			liqInt estimator = ribNode__->photon.estimator;
//			RiAttribute( "photon", (liqToken) "estimator", &estimator, RI_NULL );
//		}
//
//	}
//
//	// 3Delight sss group
//	if( ribNode__->delightSSS.doScatter && liqglo.liquidRenderer.renderName == MString("3Delight") )
//	{
//#ifdef GENERIC_RIBLIB         
//		useAdvancedVisibilityAttributes = true;
//#endif			  
//		liqString groupName = const_cast< char* >( ribNode__->delightSSS.groupName.asChar() );
//		RiAttribute( "visibility", (liqToken) "string subsurface", &groupName, RI_NULL );
//
//		liqColor scattering, absorption;
//		scattering[0] = ribNode__->delightSSS.scattering.r;
//		scattering[1] = ribNode__->delightSSS.scattering.g;
//		scattering[2] = ribNode__->delightSSS.scattering.b;
//
//		absorption[0] = ribNode__->delightSSS.absorption.r;
//		absorption[1] = ribNode__->delightSSS.absorption.g;
//		absorption[2] = ribNode__->delightSSS.absorption.b;
//
//		if ( scattering[0] && scattering[1] && scattering[2] )
//			RiAttribute( "subsurface",
//			(liqToken) "scattering", &scattering,
//			(liqToken) "absorption", &absorption,
//			RI_NULL );
//
//		liqFloat refractionindex = ribNode__->delightSSS.refraction;
//		liqFloat shadingrate = ribNode__->delightSSS.shadingRate;
//		liqFloat scale = ribNode__->delightSSS.scale;
//
//		RiAttribute( "subsurface",
//			(liqToken) "refractionindex", &refractionindex,
//			(liqToken) "shadingrate", &shadingrate,
//			(liqToken) "scale", &scale, 
//			RI_NULL );
//
//		liqColor meanfreepath, reflectance;
//		meanfreepath[0] = ribNode__->delightSSS.meanfreepath.r;
//		meanfreepath[1] = ribNode__->delightSSS.meanfreepath.g;
//		meanfreepath[2] = ribNode__->delightSSS.meanfreepath.b;
//
//		reflectance[0] = ribNode__->delightSSS.reflectance.r;
//		reflectance[1] = ribNode__->delightSSS.reflectance.g;
//		reflectance[2] = ribNode__->delightSSS.reflectance.b;
//
//		if ( reflectance[0] && reflectance[1] && reflectance[2] )
//			RiAttribute( "subsurface",
//			(liqToken) "meanfreepath", &meanfreepath,
//			(liqToken) "reflectance", &reflectance, 
//			RI_NULL );
//
//		if ( ribNode__->delightSSS.referencecamera != "" )
//		{
//			liqString referenceCamera = const_cast< char* >( ribNode__->delightSSS.referencecamera.asChar() );
//			RiAttribute( "subsurface", (liqToken) "string referencecamera", &referenceCamera, RI_NULL );
//		}
//	}
//
//	//strategy
//	{
//		if(ribNode__->strategy.strategy_!="grids"){
//			liqString strategy = const_cast<char*>(ribNode__->strategy.strategy_.asChar());
//			RiAttribute("shade",(liqToken)"string strategy", &strategy, RI_NULL);
//		}
//
//		if(ribNode__->strategy.volumeIntersectionStrategy!="exclusive"){
//			liqString volumeIntersectionStrategy = const_cast<char*>(ribNode__->strategy.volumeIntersectionStrategy.asChar());
//			RiAttribute("shade",(liqToken)"string volumeintersectionstrategy", &volumeIntersectionStrategy, RI_NULL);
//		}
//
//		if(ribNode__->strategy.volumeIntersectionPriority >0.0001){
//			liqFloat volumeIntersectionPriority = ribNode__->strategy.volumeIntersectionPriority;
//			RiAttribute("shade",(liqToken)"float volumeintersectionpriority", &volumeIntersectionPriority, RI_NULL);
//		}
//	}
//
//	//trim curve 
//	{
//		liqString sense = "inside";
//
//		switch(ribNode__->trimcurve.sense){
//				case liqRibNode::trimcurve::INSIDE:  
//					{
//						sense  = "inside";
//						//RiAttribute("trimcurve",(liqToken)"string sense", &sense, RI_NULL);//default value, need not to write.
//					}break;
//				case liqRibNode::trimcurve::OUTSIDE:
//					{
//						sense  = "outside";
//						RiAttribute("trimcurve",(liqToken)"string sense", &sense, RI_NULL);	
//					}break;
//				default:
//					assert(0);
//		}
//
//	}//trim curve 
//
//	//stitch
//	{
//		if( ribNode__->stitch.enable != true ){
//			liqInt enable = (ribNode__->stitch.enable)? 1: 0;
//			RiAttribute("stitch",(liqToken)"int enable", &enable, RI_NULL);	
//		}
//		if( ribNode__->stitch.traceenable != false ){
//			liqInt traceenable = (ribNode__->stitch.traceenable)? 1: 0;
//			RiAttribute("stitch",(liqToken)"int traceenable", &traceenable, RI_NULL);	
//		}
//		if( ribNode__->stitch.newgroup != false ){
//			liqInt newgroup = (ribNode__->stitch.newgroup)? 1: 0;
//			RiAttribute("stitch",(liqToken)"int newgroup", &newgroup, RI_NULL);	
//		}	
//	}//stitch
//
//	//stochastic
//	{
//		if( ribNode__->stochastic.sigma != 0 ){
//			liqInt sigma = ribNode__->stochastic.sigma;
//			RiAttribute("stochastic",(liqToken)"int sigma", &sigma, RI_NULL);	
//		}
//		if( ribNode__->stochastic.pointfalloff != 0 ){
//			liqInt pointfalloff = ribNode__->stochastic.pointfalloff;
//			RiAttribute("stochastic",(liqToken)"int pointfalloff", &pointfalloff, RI_NULL);	
//		}
//	}//stochastic
//
//	//dice
//	{
//		if( ribNode__->dice.binary != 0 ){
//			liqInt binary = ribNode__->dice.binary;
//			RiAttribute("dice",(liqToken)"int binary", &binary, RI_NULL);
//		}
//		if( ribNode__->dice.hair != 0 ){
//			liqInt hair = ribNode__->dice.hair;
//			RiAttribute("dice",(liqToken)"int hair", &hair, RI_NULL);
//		}
//		//strategy
//		liqString strategy = "planarprojection";
//		switch(ribNode__->dice.strategy){
//				  case liqRibNode::dice::PLANAR_PROJECTION:
//					  {
//						  strategy = "planarprojection";
//						  //RiAttribute("dice",(liqToken)"string strategy", &strategy, RI_NULL); //default value
//					  }break;
//				  case liqRibNode::dice::SPHERICAL_PROJECTION:
//					  {
//						  strategy = "sphericalprojection";
//						  RiAttribute("dice",(liqToken)"string strategy", &strategy, RI_NULL);
//					  }break;
//				  default:
//					  assert(0);
//		}
//		//referencecamera
//		liqString referencecamera = "worldcamera";
//		switch(ribNode__->dice.referencecamera){
//				  case liqRibNode::dice::WORLD_CAMERA:
//					  {
//						  referencecamera = "worldcamera";
//						  //RiAttribute("dice",(liqToken)"string referencecamera", &referencecamera, RI_NULL); //default value
//					  }break;
//				  case liqRibNode::dice::FRAME_CAMERA:
//					  {
//						  referencecamera = "framecamera";
//						  RiAttribute("dice",(liqToken)"string referencecamera", &referencecamera, RI_NULL);
//					  }break;
//				  default:
//					  assert(0);
//		}
//
//	}//dice
//
//	//derivatives
//	{
//		if( ribNode__->derivatives.centered != 1 ){
//			liqInt centered = ribNode__->derivatives.centered;
//			RiAttribute("derivatives",(liqToken)"int centered", &centered, RI_NULL);
//		}
//		if( ribNode__->derivatives.extrapolate != 1 ){
//			liqInt extrapolate = ribNode__->derivatives.extrapolate;
//			RiAttribute("derivatives",(liqToken)"int extrapolate", &extrapolate, RI_NULL);
//		}
//	}//derivatives
//
//	//procedural
//	{
//		if( ribNode__->procedural.attribute.length()>0 ){
//			liqString attribute = const_cast<char*>(ribNode__->procedural.attribute.asChar());
//			RiAttribute("procedural",(liqToken)"string attribute", &attribute, RI_NULL);
//		}
//	}//procedural
//
//	if( liqglo.liqglo_doMotion && 
//		ribNode__->motion.factor != 1.0f && 
//		( ribNode__->motion.deformationBlur || ribNode__->motion.transformationBlur ) ) 
//		RiGeometricApproximation( "motionfactor", ribNode__->motion.factor );
//
//	ribNode__->writeUserAttributes();
//
//	return MS::kSuccess;
//}
//
//MStatus liqRibTranslator::writeShader(
//	const bool writeShaders__, 
//	const liqRibNodePtr &ribNode__,
//	const bool hasVolumeShader__,
//	const bool hasSurfaceShader__,
//	const bool hasCustomSurfaceShader__,
//	//const bool hasDisplacementShader__,
//	const MString &shaderRibBox__,
//	const MDagPath &path__,
//	const structJob &currentJob
//	
//	)
//{
//	const bool isShadowJob = currentJob.isShadow; 
//	const bool isDeepShadowJob = currentJob.deepShadows;
//
//		MStatus status;
//
//		if( writeShaders__ ) 
//		{
//			liqRIBMsg("[5] hasVolumeShader=%d, m_ignoreVolumes=%d", hasVolumeShader__, m_ignoreVolumes );
//			if( hasVolumeShader__ && !m_ignoreVolumes ) 
//			{
//				//liqShader& currentShader( liqGetShader( ribNode__->assignedVolume.object() ) );
//				liqShader& currentShader = liqShaderFactory::instance().getShader( ribNode__->assignedVolume.object() );
//				liqRIBMsg("[1] liqglo_currentJob.isShadow=%d, currentShader.outputInShadow=%d", isShadowJob, currentShader.outputInShadow );
//				// per shader shadow pass override
//				if( !isShadowJob || currentShader.outputInShadow )
//				{
//					currentShader.write(liqglo.liqglo_shortShaderNames, 0);
//				}
//			}
//
//			if( hasSurfaceShader__ && !m_ignoreSurfaces )
//			{
//				if( hasCustomSurfaceShader__ )
//				{
//					if( hasCustomSurfaceShader__ == liqCustomPxShaderNode )
//					{  // Just call the write method of the custom shader
//						MFnDependencyNode customShaderDepNode( ribNode__->assignedShader.object() );
//						MPxNode *mpxNode = customShaderDepNode.userNode();
//						liqCustomNode *customNode( NULL );
//						if( mpxNode && ( customNode = dynamic_cast<liqCustomNode*>( mpxNode ) ) )
//							customNode->liquidWrite();
//						else
//							;// Should never happen in theory ... but what is the way to report a problem ???
//					}
//					else
//					{ 
//						// Default : just write the contents of the rib box
//						RiArchiveRecord( RI_VERBATIM, ( char* )shaderRibBox__.asChar() );
//						RiArchiveRecord( RI_VERBATIM, "\n" );
//					}
//				}
//				else
//				{
//					//liqShader& currentShader( liqGetShader( ribNode__->assignedShader.object() ) );
//					liqShader& currentShader = liqShaderFactory::instance().getShader( ribNode__->assignedShader.object() );
//					
//					F1(ribNode__, currentShader);
//
//					liqRIBMsg("[2] liqglo_currentJob.isShadow=%d, currentShader.outputInShadow=%d", isShadowJob, currentShader.outputInShadow );
//					// per shader shadow pass override
//					if( !isShadowJob || currentShader.outputInShadow )
//					{
//						currentShader.write(liqglo.liqglo_shortShaderNames, 0);
//					}
//
//				}
//			}else{//if( hasSurfaceShader && !m_ignoreSurfaces )
//				F2(m_shaderDebug, ribNode__);
//
//				if( !m_ignoreSurfaces ) 
//				{
//					MObject shadingGroup = ribNode__->assignedShadingGroup.object();
//					MObject shader = ribNode__->findShader( shadingGroup );
//					//
//					// here we check for regular shader nodes first
//					// and assign default shader to shader-less nodes.
//					//
//					if( m_shaderDebug ) {
//						liqRIBMsg("shader debug is turned on, so the surface is constant.");
//						RiSurface( "constant", RI_NULL );
//						LIQDEBUGPRINTF("add more constant parameters here. take /RMS-1.0.1-Maya2008/lib/shaders/src/mtorBlinn.sl as an example.(?)");
//					}
//					// 					else if( shader.apiType() == MFn::kLambert ){ 
//					// 						RiSurface( "matte", RI_NULL );
//					// 						LIQDEBUGPRINTF("add more lambert parameters here. take //RMS-1.0.1-Maya2008/lib/shaders/src/mtorLambert.sl as an example.");
//					// 					}else if( shader.apiType() == MFn::kPhong ) {
//					// 						RiSurface( "plastic", RI_NULL );
//					// 						LIQDEBUGPRINTF("add more phong parameters here. take /RMS-1.0.1-Maya2008/lib/shaders/src/mtorPhong.sl as an example.");
//					// 					}
//					else if( path__.hasFn( MFn::kPfxHair ) ) 
//					{
//						// get some of the hair system parameters
//						liqFloat translucence = 0, specularPower = 0;
//						liqColor specularColor;
//
//						getPfxHairData(path__, translucence, specularPower, specularColor);
//
//						RiSurface(  "liquidpfxhair",
//							"float specularpower", &specularPower,
//							"float translucence",  &translucence,
//							"color specularcolor", &specularColor,
//							RI_NULL );
//					} 
//					else if( path__.hasFn( MFn::kPfxToon ) ) {
//						RiSurface( "liquidpfxtoon", RI_NULL );
//					}else if( path__.hasFn( MFn::kPfxGeometry ) ){
//						RiSurface( "liquidpfx", RI_NULL );
//					}else {
//						//RiSurface( "plastic", RI_NULL );//ymesh-branch r773 use this
//						MFnDependencyNode shaderFn(shader);
//						RiSurface( const_cast<char*>(shaderFn.name().asChar()), RI_NULL );
//					}
//				}
//			}//if( hasSurfaceShader && !m_ignoreSurfaces )else
//		} //if( writeShaders ) 
//		else if( isDeepShadowJob ) 
//		{
//			liqRIBMsg("[7] liqglo_currentJob[.deepShadows=%d, .isShadow=%d ], hasSurfaceShader=%d, hasCustomSurfaceShader=%d",
//				isDeepShadowJob, isShadowJob, hasSurfaceShader__, hasCustomSurfaceShader__ );
//
//			// if the current job is a deep shadow,
//			// we still want to output primitive color and opacity and surface shader
//			// In case of custom shaders, what should we do ? Stephane.
//			if( hasSurfaceShader__ && ! hasCustomSurfaceShader__ ) 
//			{
//				//liqShader & currentShader = liqGetShader( ribNode__->assignedShader.object());
//				liqShader &currentShader = liqShaderFactory::instance().getShader( ribNode__->assignedShader.object() );
//
//				F1(ribNode__, currentShader);
//
//				liqRIBMsg("[8] currentShader[.name=%s, .filename=%s, .outputInShadow=%d]", currentShader.name.c_str(), currentShader.file.c_str(), currentShader.outputInShadow );
//				if(currentShader.outputInShadow){
//					currentShader.write(liqglo.liqglo_shortShaderNames, 0);
//				}
//			} 
//			else //if( hasSurfaceShader__ && ! hasCustomSurfaceShader__ ) 
//			{
//				F2(false, ribNode__ );
//
//				if( path__.hasFn( MFn::kPfxHair ) ) 
//				{
//					// get some of the hair system parameters
//					liqFloat translucence = 0, specularPower = 0;
//					liqColor specularColor;
//
//					getPfxHairData(path__, translucence, specularPower, specularColor);
//
//					RiSurface(  "liquidPfxHair",
//						"float specularPower", &specularPower,
//						"float translucence",  &translucence,
//						"color specularColor", &specularColor,
//						RI_NULL );
//				}
//			}
//		}else {
//			RiSurface( "null", RI_NULL );
//		}
//
//
//	return MS::kSuccess;
//}
//
MStatus liqRibTranslator::objectBlock_reference(const structJob &currentJob)
{
	CM_TRACE_FUNC("liqRibTranslatorNew::objectBlock_reference("<<currentJob.name.asChar()<<")");
	MStatus returnStatus = MS::kSuccess;
	MStatus status;
	attributeDepth = 0;

	LIQDEBUGPRINTF( "-> objectBlock_reference\n" );

	//[refactor 36] begin
	liquid::RendererMgr::getInstancePtr()->getRenderer()->objectBlock_reference_begin();
// 	if( m_ignoreSurfaces && !liqglo.liqglo_skipDefaultMatte )
// 	{
// 		RiSurface( "matte", RI_NULL );
// 	}
	//[refactor 36] end

	// Moritz: Added Pre-Geometry RIB for insertion right before any primitives
	preGeometryMel();

	// retrieve the shadow set object
	MObject shadowSetObj(getShadowSetObject(currentJob));
	MFnSet shadowSet( shadowSetObj, &status );

	//MMatrix matrix;
	for ( RNMAP::iterator rniter( htable->RibNodeMap.begin() ); rniter != htable->RibNodeMap.end(); rniter++ ) 
	{
		LIQ_CHECK_CANCEL_REQUEST;

		liqRibNodePtr ribNode( rniter->second );

		if( ( !ribNode ) || ( ribNode->object(0)->type == MRT_Light ) ) 
			continue;
		if( ribNode->object(0)->type == MRT_Coord || ribNode->object(0)->type == MRT_ClipPlane ) 
			continue;
		if( ( currentJob.pass != rpShadowMap ) && ( ribNode->object(0)->ignore ) ) 
			continue;
		if( ( currentJob.pass == rpShadowMap ) && ( ribNode->object(0)->ignoreShadow ) ) 
			continue;
		// test against the set
		if( ( currentJob.pass == rpShadowMap ) 
			&& ( currentJob.shadowObjectSet != "" ) 
			&& ( !shadowSetObj.isNull() ) 
			&& ( !shadowSet.isMember( ribNode->path().transform(), &status ) ) ) 
		{
			//cout <<"SET FILTER : object "<<ribNode->name.asChar()<<" is NOT in "<<liqglo_currentJob.shadowObjectSet.asChar()<<endl;
			continue;
		}
		//
		oneObjectBlock_reference(ribNode, currentJob );

	}//for ( RNMAP::iterator rniter(...

	//[refactor 37] begin
	liquid::RendererMgr::getInstancePtr()->getRenderer()->objectBlock_reference_end();
// 	assert(attributeDepth==0);
// 	while ( attributeDepth > 0 ) 
// 	{
// 		RiAttributeEnd();
// 		attributeDepth--;
// 	}
	//[refactor 37] end

	return returnStatus;
}
//
MStatus liqRibTranslator::objectBlock_data(const structJob &currentJob)
{
	CM_TRACE_FUNC("liqRibTranslatorNew::objectBlock_data("<<currentJob.name.asChar()<<")");
	MStatus returnStatus = MS::kSuccess;
	MStatus status;
	attributeDepth = 0;

	LIQDEBUGPRINTF( "-> objectBlock_data\n" );

	// retrieve the shadow set object
	MObject shadowSetObj(getShadowSetObject(currentJob));
	MFnSet shadowSet( shadowSetObj, &status );

	//MMatrix matrix;
	for ( RNMAP::iterator rniter( htable->RibNodeMap.begin() ); rniter != htable->RibNodeMap.end(); rniter++ ) 
	{
		LIQ_CHECK_CANCEL_REQUEST;

		liqRibNodePtr ribNode( rniter->second );

		if( ( !ribNode ) || ( ribNode->object(0)->type == MRT_Light ) ) 
			continue;
		if( ribNode->object(0)->type == MRT_Coord || ribNode->object(0)->type == MRT_ClipPlane ) 
			continue;
		if( ( currentJob.pass != rpShadowMap ) && ( ribNode->object(0)->ignore ) ) 
			continue;
		if( ( currentJob.pass == rpShadowMap ) && ( ribNode->object(0)->ignoreShadow ) ) 
			continue;
		// test against the set
		if( ( currentJob.pass == rpShadowMap ) 
			&& ( currentJob.shadowObjectSet != "" ) 
			&& ( !shadowSetObj.isNull() ) 
			&& ( !shadowSet.isMember( ribNode->path().transform(), &status ) ) ) 
		{
			//cout <<"SET FILTER : object "<<ribNode->name.asChar()<<" is NOT in "<<liqglo_currentJob.shadowObjectSet.asChar()<<endl;
			continue;
		}
		//
		oneObjectBlock_data(ribNode, currentJob );

	}//for ( RNMAP::iterator rniter(...

	return returnStatus;
}
MStatus liqRibTranslator::objectBlock__(const structJob &currentJob)
{
	CM_TRACE_FUNC("liqRibTranslatorNew::objectBlock__("<<currentJob.name.asChar()<<")");

	objectBlock_data(currentJob);

	return objectBlock_reference(currentJob);
}

//
MStatus liqRibTranslator::worldEpilogue__()
{
	CM_TRACE_FUNC("liqRibTranslatorNew::worldEpilogue__()");
	return worldEpilogue();
}
//
MStatus liqRibTranslator::frameEpilogue__( long scanTime)
{
	CM_TRACE_FUNC("liqRibTranslatorNew::frameEpilogue__("<<scanTime<<")");
	if( ribStatus == kRibFrame) 
	{
		ribStatus = kRibBegin;
		liquid::RendererMgr::getInstancePtr()->getRenderer()->frameEpilogue(scanTime);
	}
	return (ribStatus == kRibBegin ? MS::kSuccess : MS::kFailure);

}
//
MStatus liqRibTranslator::_doItNewWithoutRenderScript( const MString& originalLayer )
{
	CM_TRACE_FUNC("liqRibTranslatorNew::_doItNewWithoutRenderScript(arg,"<<originalLayer.asChar()<<")");
	MStatus status;
#if (Refactoring == 0)
	MString lastRibName;
#endif
	//bool hashTableInited = false;

	if( !liqglo.liquidBin && !liqglo.m_deferredGen ) 
		liquidMessage( "Creating RIB <Press ESC To Cancel> ...", messageInfo );

	// Remember the frame the scene was at so we can restore it later.
	originalTime = MAnimControl::currentTime();

	// Set the frames-per-second global (we'll need this for
	// streak particles)
	//
	MTime oneSecond( 1, MTime::kSeconds );
	liqglo.liqglo_FPS = oneSecond.as( MTime::uiUnit() );

	// check to see if the output camera, if specified, is available. If exporting only objects, don't care about camera
	//[refactor][1.1.1 begin] from _doIt()
	//[refactor][1.1.1 end] from _doIt()

	// check to see if all the directories we are working with actually exist.
	/*if( verifyOutputDirectories() ) {
	MString err( "The output directories are not properly setup in the globals" );
	throw err;
	}*/
	// This is bollocks! Liquid defaults to system temp folders if it can't setup shit. It should always work, not breaks
	verifyOutputDirectories();

	// setup the error handler
// #if( !defined (GENERIC_RIBLIB) ) && ( defined ( AQSIS ) || ( _WIN32 && DELIGHT ) )
// #  ifdef _WIN32
// 	if( m_errorMode ) RiErrorHandler( (void(__cdecl*)(int,int,char*))liqRibTranslatorErrorHandler );
// #  else
// 	if( m_errorMode ) RiErrorHandler( (void(*)(int,int,char*))liqRibTranslatorErrorHandler );
// #  endif
// #else
// 	if( m_errorMode ) RiErrorHandler( liqRibTranslatorErrorHandler );
// #endif
	// Setup helper variables for alfred
	// 	MString alfredCleanUpCommand;// not used. [9/15/2010 yys]
	// 	if( remoteRender ) 
	// 		alfredCleanUpCommand = MString( "RemoteCmd" );
	// 	else 
	// 		alfredCleanUpCommand = MString( "Cmd" );

	// 	MString alfredRemoteTagsAndServices;// not used. [9/15/2010 yys]
	// 	if( remoteRender || useNetRman ) 
	// 	{
	// 		alfredRemoteTagsAndServices  = MString( "-service { " );
	// 		alfredRemoteTagsAndServices += m_alfredServices.asChar();
	// 		alfredRemoteTagsAndServices += MString( " } -tags { " );
	// 		alfredRemoteTagsAndServices += m_alfredTags.asChar();
	// 		alfredRemoteTagsAndServices += MString( " } " );
	// 	}
	// A seperate one for cleanup as it doesn't need a tag!
	// 	MString alfredCleanupRemoteTagsAndServices;// not used. [9/15/2010 yys]
	// 	if( remoteRender || useNetRman ) 
	// 	{
	// 		alfredCleanupRemoteTagsAndServices  = MString( "-service { " );
	// 		alfredCleanupRemoteTagsAndServices += m_alfredServices.asChar();
	// 		alfredCleanupRemoteTagsAndServices += MString( " } " );
	// 	}

	// exception handling block, this tracks liquid for any possible errors and tries to catch them
	// to avoid crashing
	try 
	{
		liqRibTranslator::m_escHandler.beginComputation();

		MString preFrameMel = parseString( m_preFrameMel );
		MString postFrameMel = parseString( m_postFrameMel );

		if( ( preFrameMel  != "" ) && !fileExists( preFrameMel ) ) 
			liquidMessage( "Cannot find pre frame MEL script file! Assuming local.", messageWarning );

		if( ( m_postFrameMel != "" ) && !fileExists( postFrameMel ) ) 
			liquidMessage( "Cannot find post frame MEL script file! Assuming local.", messageWarning );

		// build temp file names
		MString renderScriptName = generateRenderScriptName();
		liqglo.tempDefname    = generateTempMayaSceneName();

		if( liqglo.m_deferredGen ) 
		{
			MString currentFileType = MFileIO::fileType();
			MFileIO::exportAll( liqglo.tempDefname, currentFileType.asChar() );
		}


		liqRenderScript jobScript;
		// 		liqRenderScript::Job preJobInstance;
		// 		preJobInstance.title = "liquid pre-job";
		// 		preJobInstance.isInstance = true;
		tJobScriptMgr jobScriptMgr(jobScript);

		printFrameSequence("1");
		// build the frame array
		liquidMessage2(messageInfo, "liqglo.m_renderView=%d", liqglo.m_renderView );
		liquidMessage2(messageInfo, "isBatchMode()=%d", isBatchMode() );
		if( liqglo.m_renderView ) 
		{
			// if we are in renderView mode,
			// just ignore the animation range
			// and render the current frame.
			liqglo.frameNumbers.clear();
			liqglo.frameNumbers.push_back( ( int ) originalTime.as( MTime::uiUnit() ) );
		}
		//
		// start looping through the frames  +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		//
		liquidMessage( "Starting to loop through frames", messageInfo );
		printFrameSequence("2");

		unsigned frameIndex( 0 );
		for ( ; frameIndex < liqglo.frameNumbers.size(); frameIndex++ ) 
		{
			int currentBlock( 0 );
			//[refactor][1.7 begin] from _doIt() 
			const MString framePreCommand(parseString( liqglo.m_preCommand, false));
			const MString frameRibgenCommand(parseString( m_ribgenCommand, false ));
			const MString frameRenderCommand( parseString( m_renderCommand + " " + liqglo.liquidRenderer.renderCmdFlags, false ));
			const MString framePreFrameCommand(parseString( m_preFrameCommand, false ));
			const MString framePostFrameCommand(parseString( m_postFrameCommand, false ));
			//[refactor][1.7 end] from _doIt()
			//[refactor] [1.8.1 begin] to _doIt()
			liqRenderScript::Job frameScriptJob;
			//[refactor] [1.8.1 end] to _doIt()
			tFrameScriptJobMgr frameScriptJobMgr(frameScriptJob);

			//[refactor] [1.8.3.1 begin] from _doIt()
			stringstream titleStream;
			titleStream << liquidTransGetSceneName().asChar() << "Frame" << liqglo.liqglo_lframe;
			frameScriptJobMgr.setCommonParameters( titleStream.str() );
			//[refactor] [1.8.3.1 end] from _doIt()
			///////////////////////////////////////////////////
			TempControlBreak tcb = 
				processOneFrame(frameIndex, liqglo);
			PROCESS_TEMP_CONTROL_BREAK_CONTINUE_RETURN(tcb)
			///////////////////////////////////////////////////

			jobScript.addJob( frameScriptJob );


			if( ( ribStatus != kRibOK ) && !liqglo.m_deferredGen ) 
				break;

		} // frame for-loop

		LIQDEBUGPRINTF( "-> ending escape handler.\n" );
		liqRibTranslator::m_escHandler.endComputation();

		if( !liqglo.m_deferredGen ) 
			liquidMessage( "Finished creating RIB", messageInfo );
		
		structJob currentJob____ = *(jobList.rbegin());//I guess liqglo.liqglo_currentJob is jobList.rbegin()

// 		LIQDEBUGPRINTF( "-> clearing job list.\n" );
// 		jobList.clear();
// 		jobScript.clear();

		// set the attributes on the liquidGlobals for the last rib file and last alfred script name
		LIQDEBUGPRINTF( "-> setting lastAlfredScript and lastRibFile.\n" );
		MGlobal::executeCommand("if(!attributeExists(\"lastRenderScript\",\"liquidGlobals\")) { addAttr -ln \"lastRenderScript\" -dt \"string\" liquidGlobals; }");
		MFnDependencyNode rGlobalNode( liqglo.rGlobalObj );
		MPlug nPlug;
		nPlug = rGlobalNode.findPlug( "lastRenderScript" );
		nPlug.setValue( renderScriptName );
#if (Refactoring == 0)
		nPlug = rGlobalNode.findPlug( "lastRibFile" );
		nPlug.setValue( lastRibName );
#endif
		LIQDEBUGPRINTF( "-> spawning command.\n" );
		if( liqglo.launchRender ) 
		{
			{
				//[refactor][1.16 ]
				// launch renders directly
				liquidMessage( "", messageInfo ); // emit a '\n'
				//int exitstat = 0;
#ifndef _Refactor_doTexure_doShadow
				// write out make texture pass
				doTextures(txtList);

				if( liqglo.liqglo_doShadows ) 
				{
					doShadows(shadowList);
				}
#endif
				//if( !exitstat ){
				liquidMessage( "Rendering hero pass... ", messageInfo );
				cerr << "liquidBin = " <<  liqglo.liquidBin << endl << flush; 

// moved to ...
// 				if( isBatchMode() )//batch mode
// 				{
// 					std::size_t SIZE = getRibFileListSize();
// 					for(std::size_t i=0; i<SIZE; ++i)
// 					{
// 						liquidMessage2(messageInfo, "rendering frame %d ...", i);
// 						liquid::RendererMgr::getInstancePtr()->getRenderer()->renderAll_local( getRibFile(i) );
// 					}
// 				}else{//interactive mode
// 					//structJob &currentJob____ = *(jobList.rbegin());//I guess liqglo.liqglo_currentJob is jobList.rbegin()
// 					if( currentJob____.skip ) 
// 					{
// 						printf("    - skipping '%s'\n", currentJob____.ribFileName.asChar() );
// 						liquidMessage( "    - skipping '" + string( currentJob____.ribFileName.asChar() ) + "'", messageInfo );
// 					}else {
// 						liquid::RendererMgr::getInstancePtr()->getRenderer()->renderAll_local(currentJob____.ribFileName);
// 					}
// 				}//if( isBatchMode() )

				//}//if( !exitstat )

			}
		} // if( launchRender )
		
		LIQDEBUGPRINTF( "-> clearing job list.\n" );
		jobList.clear();
		jobScript.clear();

		postActions(originalLayer);

		return ( (ribStatus == kRibOK || liqglo.m_deferredGen) ? MS::kSuccess : MS::kFailure);

	} 
	catch ( MString errorMessage ) 
	{
		liquidMessage( errorMessage, messageError );
		/*if( htable && hashTableInited ) delete htable;
		freeShaders();*/
		liqRibTranslator::m_escHandler.endComputation();
		return MS::kFailure;
	} 
	catch ( ... ) 
	{
		liquidMessage( "Unknown exception thrown", messageError );
		/*if( htable && hashTableInited ) delete htable;
		freeShaders();*/
		liqRibTranslator::m_escHandler.endComputation();
		return MS::kFailure;
	}
}
//
MStatus liqRibTranslator::_doItNewWithRenderScript( const MString& originalLayer )
{
	CM_TRACE_FUNC("liqRibTranslatorNew::_doItNewWithRenderScript(args,"<<originalLayer.asChar()<<")");

	MStatus status;
#if (Refactoring == 0)
	MString lastRibName;
#endif
	//bool hashTableInited = false;

	if( !liqglo.liquidBin && !liqglo.m_deferredGen ) 
		liquidMessage( "Creating RIB <Press ESC To Cancel> ...", messageInfo );

	// Remember the frame the scene was at so we can restore it later.
	originalTime = MAnimControl::currentTime();

	// Set the frames-per-second global (we'll need this for
	// streak particles)
	//
	MTime oneSecond( 1, MTime::kSeconds );
	liqglo.liqglo_FPS = oneSecond.as( MTime::uiUnit() );

	// check to see if the output camera, if specified, is available. If exporting only objects, don't care about camera
	//[refactor][1.1.1 begin] from _doIt()
	//[refactor][1.1.1 end] from _doIt()

	// check to see if all the directories we are working with actually exist.
	/*if( verifyOutputDirectories() ) {
	MString err( "The output directories are not properly setup in the globals" );
	throw err;
	}*/
	// This is bollocks! Liquid defaults to system temp folders if it can't setup shit. It should always work, not breaks
	verifyOutputDirectories();

	// setup the error handler
// #if( !defined (GENERIC_RIBLIB) ) && ( defined ( AQSIS ) || ( _WIN32 && DELIGHT ) )
// #  ifdef _WIN32
// 	if( m_errorMode ) RiErrorHandler( (void(__cdecl*)(int,int,char*))liqRibTranslatorErrorHandler );
// #  else
// 	if( m_errorMode ) RiErrorHandler( (void(*)(int,int,char*))liqRibTranslatorErrorHandler );
// #  endif
// #else
// 	if( m_errorMode ) RiErrorHandler( liqRibTranslatorErrorHandler );
// #endif

	//[refactor][1.2 begin] from _doItNewWithRenderScript()
	// Setup helper variables for alfred
	MString alfredCleanUpCommand = ( liqglo.remoteRender ) ? MString( "RemoteCmd" ) : MString( "Cmd" );
	MString alfredRemoteTagsAndServices;
	MString alfredCleanupRemoteTagsAndServices;
	if( liqglo.remoteRender || liqglo.useNetRman ) 
	{
		alfredRemoteTagsAndServices  = MString( "-service { " );
		alfredRemoteTagsAndServices += liqglo.m_alfredServices.asChar();
		alfredRemoteTagsAndServices += MString( " } -tags { " );
		alfredRemoteTagsAndServices += liqglo.m_alfredTags.asChar();
		alfredRemoteTagsAndServices += MString( " } " );

		alfredCleanupRemoteTagsAndServices  = MString( "-service { " );
		alfredCleanupRemoteTagsAndServices += liqglo.m_alfredServices.asChar();
		alfredCleanupRemoteTagsAndServices += MString( " } " );
	}
	//[refactor][1.2 end] from _doItNewWithRenderScript()

	// exception handling block, this tracks liquid for any possible errors and tries to catch them
	// to avoid crashing
	try 
	{
		liqRibTranslator::m_escHandler.beginComputation();

		MString preFrameMel = parseString( m_preFrameMel );
		MString postFrameMel = parseString( m_postFrameMel );

		if( ( preFrameMel  != "" ) && !fileExists( preFrameMel ) ) 
			liquidMessage( "Cannot find pre frame MEL script file! Assuming local.", messageWarning );

		if( ( m_postFrameMel != "" ) && !fileExists( postFrameMel ) ) 
			liquidMessage( "Cannot find post frame MEL script file! Assuming local.", messageWarning );

		// build temp file names
		MString renderScriptName = generateRenderScriptName();
		liqglo.tempDefname    = generateTempMayaSceneName();

		if( liqglo.m_deferredGen ) 
		{
			MString currentFileType = MFileIO::fileType();
			MFileIO::exportAll( liqglo.tempDefname, currentFileType.asChar() );
		}


		liqRenderScript jobScript;
		// 		liqRenderScript::Job preJobInstance;
		// 		preJobInstance.title = "liquid pre-job";
		// 		preJobInstance.isInstance = true;
		tJobScriptMgr jobScriptMgr(jobScript);

		if( true/*useRenderScript*/ ) 
		{
			if( renderJobName == "" ) 
				renderJobName = liquidTransGetSceneName();
			
			//[refactor][1.4 begin] from _doIt()
			jobScriptMgr.setCommonParameters(
				renderJobName.asChar(), liqglo.useNetRman, m_minCPU, m_maxCPU, liqglo.m_dirmaps 
				);
			//[refactor][1.4 end] from _doIt()

			if( m_preJobCommand != MString( "" ) ) 
			{			
				//[refactor][1.5 begin] from _doIt()
				jobScriptMgr.addJob("liquid pre-job", 
					m_preJobCommand.asChar(), liqglo.remoteRender && !liqglo.useNetRman);
				//[refactor][1.5 end] from _doIt()
			}
		}
		// build the frame array
		//
		if( liqglo.m_renderView ) 
		{
			// if we are in renderView mode,
			// just ignore the animation range
			// and render the current frame.
			liqglo.frameNumbers.clear();
			liqglo.frameNumbers.push_back( ( int ) originalTime.as( MTime::uiUnit() ) );
		}
		//
		// start looping through the frames  +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		//
		liquidMessage( "Starting to loop through frames", messageInfo );


		unsigned frameIndex( 0 );
		for ( ; frameIndex < liqglo.frameNumbers.size(); frameIndex++ ) 
		{
			int currentBlock( 0 );
			//[refactor][1.7 begin] from _doIt() 
			const MString framePreCommand(parseString( liqglo.m_preCommand, false));
			const MString frameRibgenCommand(parseString( m_ribgenCommand, false ));
			const MString frameRenderCommand( parseString( m_renderCommand + " " + liqglo.liquidRenderer.renderCmdFlags, false ));
			const MString framePreFrameCommand(parseString( m_preFrameCommand, false ));
			const MString framePostFrameCommand(parseString( m_postFrameCommand, false ));
			//[refactor][1.7 end] from _doIt()
			//[refactor] [1.8.1 begin] to _doIt()
			liqRenderScript::Job frameScriptJob;
			//[refactor] [1.8.1 end] to _doIt()
			tFrameScriptJobMgr frameScriptJobMgr(frameScriptJob);

			//[refactor] [1.8.3.1 begin] from _doIt()
			stringstream titleStream;
			titleStream << liquidTransGetSceneName().asChar() << "Frame" << liqglo.liqglo_lframe;
			frameScriptJobMgr.setCommonParameters( titleStream.str() );
			//[refactor] [1.8.3.1 end] from _doIt()

			//[refactor][1.8 begin] from _doIt()
			if( true/*useRenderScript*/ ) 
			{
				if( liqglo.m_deferredGen ) 
				{
					if( ( frameIndex % liqglo.m_deferredBlockSize ) == 0 ) 
					{
						//[refactor] [1.8.1.1 begin] from _doIt()
						MString frameRangePart;
						if( liqglo.m_deferredBlockSize == 1 ) 
						{
							currentBlock = liqglo.liqglo_lframe;
							frameRangePart = MString( "-t " ) + liqglo.liqglo_lframe;
						}else {
							currentBlock++;
							// Add list of frames to process for this block
							unsigned lastGenFrame( ( frameIndex + liqglo.m_deferredBlockSize ) < liqglo.frameNumbers.size() ? frameIndex + liqglo.m_deferredBlockSize : liqglo.frameNumbers.size() );
							//frameRangePart = MString( "-sequence " ) + frameIndex + " " + lastGenFrame  + " " + "1";
							frameRangePart = MString( "-t " ) + liqglo.liqglo_lframe;
							for( unsigned outputFrame( frameIndex + 1 ); outputFrame < lastGenFrame; outputFrame++ )
							{
								frameRangePart += "," + liqglo.frameNumbers[ outputFrame ];
							}
						}
						//[refactor] [1.8.1.1 end] 

						jobScriptMgr.addDefferedJob(currentBlock, frameIndex,
							framePreCommand, frameRibgenCommand
							);
					}
				}//if( m_deferredGen )
			}
			if(true/*useRenderScript*/ && !m_justRib)
			{
				//[refactor] [1.8.3.2 begin] from _doIt()
				if( liqglo.m_deferredGen ) 
				{
					stringstream ss;
					ss << liquidTransGetSceneName().asChar() << "FrameRIBGEN" << currentBlock;
					frameScriptJobMgr.addInstanceJob(true, ss.str() );
				}
				//[refactor] [1.8.3.2 end] from _doIt()
			}//if( useRenderScript ) 
			//[refactor][1.8 end] to _doIt()

			///////////////////////////////////////////////////
			TempControlBreak tcb = 
				processOneFrame(frameIndex, liqglo);
			PROCESS_TEMP_CONTROL_BREAK_CONTINUE_RETURN(tcb)
			///////////////////////////////////////////////////
			
			//[refactor][1.10 begin] from _doIt()
			// now we re-iterate through the job list to write out the alfred file if we are using it
			if( true/*useRenderScript*/ && !m_justRib ) 
			{
				//[refactor][1.10.1 begin] from _doIt()
				bool alf_textures = false;
				bool alf_shadows = false;
				bool alf_refmaps = false;
				bool use_dirmaps = ( liqglo.m_dirmaps.length() )? 1 : 0;
				//[refactor][1.10.1 end] from _doIt()

				// write out make texture pass
				LIQDEBUGPRINTF( "-> Generating job for MakeTexture pass\n");

				frameScriptJobMgr.makeTexture(txtList, 
					alf_textures, alf_shadows, alf_refmaps);

				// write out shadows
				if( liqglo.liqglo_doShadows ) 
				{
					LIQDEBUGPRINTF( "-> writing out shadow information to alfred file.\n" );
					frameScriptJobMgr.makeShadow(shadowList, 
						alf_textures, alf_shadows, alf_refmaps, currentBlock, m_renderCommand);

				}//if( liqglo__.liqglo_doShadows ) 
				LIQDEBUGPRINTF( "-> finished writing out shadow information to render script file.\n" );

				// write out make reflection pass
				frameScriptJobMgr.makeReflection(refList,
					alf_textures, alf_shadows, alf_refmaps
					);

				LIQDEBUGPRINTF( "-> initiating hero pass information.\n" );
				structJob *frameJob = NULL;
				structJob *shadowPassJob = NULL;
				LIQDEBUGPRINTF( "-> setting hero pass.\n" );
				if( m_outputHeroPass && !m_outputShadowPass ){
					frameJob = &jobList[jobList.size() - 1];
				}
				else if( m_outputShadowPass && m_outputHeroPass ) 
				{
					frameJob = &jobList[jobList.size() - 1];
					shadowPassJob = &jobList[jobList.size() - 2];
				} 
				else if( m_outputShadowPass && !m_outputHeroPass ){
					shadowPassJob = &jobList[jobList.size() - 1];
				}

				LIQDEBUGPRINTF( "-> hero pass set.\n" );
				LIQDEBUGPRINTF( "-> writing out pre frame command information to render script file.\n" );
				frameScriptJobMgr.try_addPreFrameCommand(framePreFrameCommand.asChar());


				if( m_outputHeroPass ) 
				{
					frameScriptJobMgr.addHeroPass(
						//[refactor][1.10.5 begin] from
						frameJob->ribFileName, 
						//[refactor][1.10.5 end] from
						framePreCommand, frameRenderCommand);
				}//if( m_outputHeroPass ) 
				LIQDEBUGPRINTF( "-> finished writing out hero information to alfred file.\n" );
				if( m_outputShadowPass ) 
				{
					frameScriptJobMgr.addShadowPass(
						//[refactor][1.10.6 begin] from
						shadowPassJob->ribFileName, 
						//[refactor][1.10.6 end] from
						framePreCommand, frameRenderCommand);
				}//if( m_outputShadowPass )

//			   if( liqglo.cleanRib || ( framePostFrameCommand != MString( "" ) ) ) 
//			   {
				if( liqglo.cleanRib ) 
				{
					if( m_outputHeroPass  ) 
					{
						frameScriptJobMgr.cleanHeroPass(framePreCommand, frameJob->ribFileName);//[refactor][1.10.9]		
					}
					if( m_outputShadowPass) 
					{
						frameScriptJobMgr.cleanShadowPass(framePreCommand, shadowPassJob->ribFileName);//[refactor][1.10.10]	
					}
					if( liqglo.m_alfShadowRibGen ) 
					{
						structJob &currentJob____ = *(jobList.rbegin());//I guess liqglo.liqglo_currentJob is jobList.rbegin()
						MString     baseShadowName(getBaseShadowName(currentJob____));//[refactor][1.10.11]
						frameScriptJobMgr.cleanShadowRibGen(framePreCommand, baseShadowName);
					}
				}
				// try to add post frame command
				frameScriptJobMgr.try_addPostFrameCommand(framePostFrameCommand);

			//}//if( cleanRib || ( framePostFrameCommand != MString( "" ) ) ) 
				if( m_outputHeroPass ){
					frameScriptJobMgr.viewHeroPassImage(frameJob->imageName);
				}
				if( m_outputShadowPass ){
					frameScriptJobMgr.viewShadowPassImage(shadowPassJob->imageName);
				}
#if (Refactoring == 0)
				if( m_outputShadowPass && !m_outputHeroPass ) 
					lastRibName = liquidGetRelativePath( liqglo__.liqglo_relativeFileNames, shadowPassJob->ribFileName, liqglo__.liqglo_projectDir );
				else 
					lastRibName = liquidGetRelativePath( liqglo__.liqglo_relativeFileNames, frameJob->ribFileName, liqglo__.liqglo_projectDir );
#endif		
			}//if( useRenderScript && !m_justRib ) 

			jobScript.addJob( frameScriptJob );


			if( ( ribStatus != kRibOK ) && !liqglo.m_deferredGen ) 
				break;

		} // frame for-loop

		if( true/*useRenderScript*/ ) 
		{
			if( m_preJobCommand != MString( "" ) ) 
			{
				//[refactor][1.3 begin] from _doIt()
				liqRenderScript::Job preJobInstance;
				preJobInstance.title = "liquid pre-job";
				preJobInstance.isInstance = true;
				//[refactor][1.3 end] from _doIt()
				//[refactor][1.11 begin]from _doIt()
				jobScript.addLeafDependency( preJobInstance );
				//[refactor][1.11 end]from _doIt()
			}
		}
		if(true/*useRenderScript*/ && !m_justRib)
		{
			// clean up the alfred file in the future
			if( liqglo.m_deferredGen ) 
			{
				jobScriptMgr.cleanupDefferedJob();
			}
			if( cleanRenderScript ) 
			{
				jobScriptMgr.cleanupRenderScript(renderScriptName);
			}
			if( m_postJobCommand != MString("") )
			{
				jobScriptMgr.cleanupPostJob(m_postJobCommand);
			}
			jobScriptMgr.writeRenderScript(m_renderScriptFormat, renderScriptName);
		}
		LIQDEBUGPRINTF( "-> ending escape handler.\n" );
		liqRibTranslator::m_escHandler.endComputation();

		if( !liqglo.m_deferredGen ) 
			liquidMessage( "Finished creating RIB", messageInfo );
		LIQDEBUGPRINTF( "-> clearing job list.\n" );
		jobList.clear();
		jobScript.clear();

		// set the attributes on the liquidGlobals for the last rib file and last alfred script name
		LIQDEBUGPRINTF( "-> setting lastAlfredScript and lastRibFile.\n" );
		MGlobal::executeCommand("if(!attributeExists(\"lastRenderScript\",\"liquidGlobals\")) { addAttr -ln \"lastRenderScript\" -dt \"string\" liquidGlobals; }");
		MFnDependencyNode rGlobalNode( liqglo.rGlobalObj );
		MPlug nPlug;
		nPlug = rGlobalNode.findPlug( "lastRenderScript" );
		nPlug.setValue( renderScriptName );
#if (Refactoring == 0)
		nPlug = rGlobalNode.findPlug( "lastRibFile" );
		nPlug.setValue( lastRibName );
#endif
		LIQDEBUGPRINTF( "-> spawning command.\n" );
		if( liqglo.launchRender ) 
		{
			//[refactor][1.15 ]
			if( true/*useRenderScript*/ ) 
			{
				bool wait = false;
				// mesh: This already cheched while reading globals
				//if ( m_renderScriptCommand == "" ) 
				//  m_renderScriptCommand = "alfred";

				if( m_renderScriptFormat == NONE ) 
					liquidMessage( "No render script format specified to Liquid, and direct render execution not selected.", messageWarning );

				// mesh: this allows to debug output from custom renderScriptCommand
				if ( m_renderScriptCommand != "alfred" )
				{
					MString cmd = m_renderScriptCommand;
#ifndef _WIN32
					chdir( liqglo.liqglo_projectDir.asChar() );
					cmd += " " + renderScriptName + " " + liqglo.liqglo_projectDir + " " +( wait ? "" : "&" ); 
#else
					_chdir( liqglo.liqglo_projectDir.asChar() );
					cmd += " \"" + renderScriptName + "\"" + " \"" + liqglo.liqglo_projectDir + "\""; 
#endif          
					MString err = ">> render (" + MString( (!wait)? "no " : "" ) + "wait) "+ cmd +"\n";
					liquidMessage( err, messageInfo );
					int returnCode = ::system( cmd.asChar() );
				} else{
#ifdef _WIN32
					// Moritz: Added quotes to render script name as it may contain spaces in bloody Windoze
					// Note: also adding quotes to the path (aka project dir) breaks ShellExecute() -- cost me one hour to trace this!!!
					// Bloody, damn, asinine Windoze!!!
					printf("5.liqProcessLauncher::execute(%s, \"%s\", %s, %d);\n", m_renderScriptCommand.asChar(), renderScriptName.asChar(), liqglo.liqglo_projectDir.asChar(), false);
					liqProcessLauncher::execute( m_renderScriptCommand, "\"" + renderScriptName + "\"", liqglo.liqglo_projectDir, false );
#else
					liqProcessLauncher::execute( m_renderScriptCommand, renderScriptName, liqglo.liqglo_projectDir, false );
#endif
				}
// 				if( liqglo.m_renderView ) //omited in r772
// 				{
// 					doRenderView();
// 				}
			}
			//[refactor][1.15 ]
		} // if( launchRender )

		postActions(originalLayer);

		return ( (ribStatus == kRibOK || liqglo.m_deferredGen) ? MS::kSuccess : MS::kFailure);

	} 
	catch ( MString errorMessage ) 
	{
		liquidMessage( errorMessage, messageError );
		/*if( htable && hashTableInited ) delete htable;
		freeShaders();*/
		liqRibTranslator::m_escHandler.endComputation();
		return MS::kFailure;
	} 
	catch ( ... ) 
	{
		liquidMessage( "Unknown exception thrown", messageError );
		/*if( htable && hashTableInited ) delete htable;
		freeShaders();*/
		liqRibTranslator::m_escHandler.endComputation();
		return MS::kFailure;
	}
}
//
//void liqRibTranslator::F1(
//						  const liqRibNodePtr &ribNode__,  liqShader &currentShader
//						  )
//{
//	CM_TRACE_FUNC("liqRibTranslatorNew::F1("<<ribNode__->name.asChar()<<","<<currentShader.getName()<<")");
//	// Output color overrides or color ====>>>>>  WILL BE DONE IN liqShader::write -begin //r773 going to omit in r773
//	if(ribNode__->shading.color.r != -1.0)
//	{
//		liqColor rColor;
//		rColor[0] = ribNode__->shading.color[0];
//		rColor[1] = ribNode__->shading.color[1];
//		rColor[2] = ribNode__->shading.color[2];
//		RiArchiveRecord( RI_COMMENT, "mark10" );
//		RiColor( rColor );
//	}
//	else{
//		RiArchiveRecord( RI_COMMENT, "mark11" );
//		RiColor( currentShader.rmColor );
//	}
//
//	if(ribNode__->shading.opacity.r != -1.0)
//	{
//		liqColor rOpacity;
//		rOpacity[0] = ribNode__->shading.opacity[0];
//		rOpacity[1] = ribNode__->shading.opacity[1];
//		rOpacity[2] = ribNode__->shading.opacity[2];
//		RiOpacity( rOpacity );
//	}
//	else
//		RiOpacity( currentShader.rmOpacity );
//	//====>>>>>  WILL BE DONE IN liqShader::write  -end
//}
//
//////////////////////////////////////////
//void liqRibTranslator::F2(
//						  const bool m_shaderDebug, const liqRibNodePtr &ribNode__
//						  )
//{ 
//	CM_TRACE_FUNC("liqRibTranslatorNew::F2("<<m_shaderDebug<<","<<ribNode__->name.asChar()<<")");
//	if( m_shaderDebug ) 
//	{
//		liqColor rColor,rOpacity;
//		liqRIBMsg("shader debug is turned on, so the object is red.");
//		// shader debug on !! everything goes red and opaque !!!
//		rColor[0] = 1.;
//		rColor[1] = 0.;
//		rColor[2] = 0.;
//		RiArchiveRecord( RI_COMMENT, "mark12" );
//		RiColor( rColor );
//
//		rOpacity[0] = 1.;
//		rOpacity[1] = 1.;
//		rOpacity[2] = 1.;
//		RiOpacity( rOpacity );
//	}else{
//		if(ribNode__->shading.color.r != -1.0) 
//		{
//			liqColor rColor;
//			rColor[0] = ribNode__->shading.color[0];
//			rColor[1] = ribNode__->shading.color[1];
//			rColor[2] = ribNode__->shading.color[2];
//			RiArchiveRecord( RI_COMMENT, "mark13" );
//			RiColor( rColor );
//		} 
//		else if( ( ribNode__->color.r != AS_NotEXist )&&( ribNode__->color.r != AS_ConnectedAsDes )) 
//		{
//			liqColor rColor;
//			rColor[0] = ribNode__->color[0];
//			rColor[1] = ribNode__->color[1];
//			rColor[2] = ribNode__->color[2];
//			RiArchiveRecord( RI_COMMENT, "mark14" );
//			RiColor( rColor );
//		}
//
//		if(ribNode__->shading.opacity.r != -1.0) 
//		{
//			liqColor rOpacity;
//			rOpacity[0] = ribNode__->shading.opacity[0];
//			rOpacity[1] = ribNode__->shading.opacity[1];
//			rOpacity[2] = ribNode__->shading.opacity[2];
//			RiOpacity( rOpacity );
//		} 
//		else if( ( ribNode__->opacity.r != AS_NotEXist )&&( ribNode__->opacity.r != AS_ConnectedAsDes )) 
//		{	
//			liqColor rOpacity;
//			rOpacity[0] = ribNode__->opacity[0];
//			rOpacity[1] = ribNode__->opacity[1];
//			rOpacity[2] = ribNode__->opacity[2];
//			RiOpacity( rOpacity );
//		}
//	}
//}

////////////////////////////////////
void liqRibTranslator::getPfxHairData(const MDagPath &path__,
	liqFloat &translucence, liqFloat &specularPower, liqColor &specularColor
)
{
	CM_TRACE_FUNC("liqRibTranslatorNew::getPfxHairData("<<path__.fullPathName().asChar()<<",,,)");
	MStatus status;

	//cout <<"getting pfxHair shading params..."<<endl;
	MObject hairSystemObj;
	MFnDependencyNode pfxHairNode( path__.node() );
	MPlug plugToHairSystem = pfxHairNode.findPlug( "renderHairs", &status );
	MPlugArray hsPlugs;
	status.clear();
	if( plugToHairSystem.connectedTo( hsPlugs, true, false, &status ) ) 
		if( status == MS::kSuccess ) 
			hairSystemObj = hsPlugs[0].node();

	if( hairSystemObj != MObject::kNullObj ) 
	{
		MFnDependencyNode hairSystemNode(hairSystemObj);
		MPlug paramPlug;
		status.clear();
		paramPlug = hairSystemNode.findPlug( "translucence", &status );
		if( status == MS::kSuccess ) 
			paramPlug.getValue( translucence );
		status.clear();
		paramPlug = hairSystemNode.findPlug( "specularColor", &status );
		if( status == MS::kSuccess ) 
		{
			paramPlug.child(0).getValue( specularColor[0] );
			paramPlug.child(1).getValue( specularColor[1] );
			paramPlug.child(2).getValue( specularColor[2] );
		}
		status.clear();
		paramPlug = hairSystemNode.findPlug( "specularPower", &status );
		if( status == MS::kSuccess ) 
			paramPlug.getValue( specularPower );
	}
}
//
void liqRibTranslator::oneObjectBlock_reference(
	const liqRibNodePtr &ribNode,
	const structJob &currentJob
	)
{
	CM_TRACE_FUNC("liqRibTranslatorNew::oneObjectBlock_reference("<<ribNode->name.asChar()<<","<<currentJob.name.asChar()<<")");
	MDagPath path = ribNode->path();
	MFnDagNode dagFn;

	liquid::RendererMgr::getInstancePtr()->getRenderer()
		->oneObjectBlock_reference_attribute_begin(ribNode, currentJob);

	liquid::RendererMgr::getInstancePtr()->getRenderer()
		->oneObjectBlock_reference_attribute_block0(ribNode, currentJob);

		LIQDEBUGPRINTF( "-> object name: %s\n", ribNode->name.asChar() );
		MObject object;

		// Moritz: only write out light linking if we're not in a shadow pass
		if( currentJob.pass != rpShadowMap 
			|| (currentJob.shadowType == stDeep && m_outputLightsInDeepShadows && !m_ignoreLights) )
		{
			tRiIlluminate(currentJob, ribNode);
		}

		bool hasSurfaceShader( false );
		liqDetailShaderKind hasCustomSurfaceShader( liqRegularShaderNode );
		MString shaderRibBox( "" );
		bool hasDisplacementShader( false );
		bool hasVolumeShader( false );

		// Check for surface shader
		checkSurfaceShader(path, ribNode, hasSurfaceShader, hasCustomSurfaceShader, shaderRibBox);
		// Check for displacement shader
		checkDisplacementShader(path, ribNode, hasDisplacementShader);
		// Check for volume shader
		checkVolumeShader(path, ribNode, hasVolumeShader);

		liquid::RendererMgr::getInstancePtr()->getRenderer()
			->oneObjectBlock_reference_attribute_block1(ribNode, currentJob);
		//[refactor 33] begin
		bool writeShaders( true );
// 
		if( currentJob.pass == rpShadowMap &&
			(    (  currentJob.shadowType != stDeep && !m_outputShadersInShadows ) 
			  || (  currentJob.shadowType == stDeep && !m_outputShadersInDeepShadows ) )
		  )
		{
			writeShaders = false;
		} 
		//[refactor 33] end
		//[refactor 34] begin
// 		liqString mode;
// 		// new prman 16.x shade attributes group 
// 		if ( ribNode->shade.strategy != liqRibNode::shade::SHADE_STRATEGY_GRIDS )
// 		{
// 			mode = "vpvolumes"; 
// 			RiAttribute( "shade", (liqToken) "strategy", &mode, RI_NULL );
// 		}
// 		if ( ribNode->shade.volumeIntersectionStrategy != liqRibNode::shade::SHADE_VOLUMEINTERSECTIONSTRATEGY_EXCLUSIVE )
// 		{
// 			mode = "additive"; 
// 			RiAttribute( "shade", (liqToken) "volumeintersectionstrategy", &mode, RI_NULL );
// 		}
// 		if ( ribNode->shade.volumeIntersectionPriority != 0.0 )
// 		{
// 			liqFloat value= ribNode->shade.volumeIntersectionPriority; 
// 			RiAttribute( "shade", (liqToken) "volumeintersectionpriority", &value, RI_NULL );
// 		}
		//[refactor 34] end
// 		liqRIBMsg("[6] writeShaders=%d=%d && ((!%d&&!%d)||(%d&&!%d) ", writeShaders, 
// 			currentJob.isShadow, 
//			currentJob.deepShadows, m_outputShadersInShadows, currentJob.deepShadows, m_outputShadersInDeepShadows );

		if( currentJob.pass != rpShadowMap )
		{
			writeShader_(
				true,//writeShaders
				ribNode,
				hasVolumeShader,
				hasSurfaceShader,
				hasCustomSurfaceShader,
				//hasDisplacementShader,
				shaderRibBox,
				path,
				currentJob
				);
			liqRIBMsg("[4] hasDisplacementShader=%d, m_ignoreDisplacements=%d", hasDisplacementShader, m_ignoreDisplacements );
			if( hasDisplacementShader && !m_ignoreDisplacements ) 
			{
				//liqShader & currentShader = liqGetShader( ribNode__->assignedDisp.object() );
				liqShader &currentShader = liqShaderFactory::instance().getShader( ribNode->assignedDisp.object() );

				liqRIBMsg("[3] liqglo_currentJob.isShadow=%d, currentShader.outputInShadow=%d", currentJob.pass, currentShader.outputInShadow );
				// per shader shadow pass override
				if( true/*!currentJob.isShadow || currentShader.outputInShadow*/ )
				{
					//currentShader.write();//use ShadingGroup file reference(e.g. *.erapi/*.rmsg) instead.
				}
			}

			liquid::RendererMgr::getInstancePtr()->getRenderer()
				->oneObjectBlock_reference_attribute_block3_ShadingGroup(
					ribNode->name
				);

		}else{
			//currentJob.isShadow==true

			writeShader_forShadow(
				true,//the value is changed  in the function.
				ribNode,
				hasVolumeShader,
				hasSurfaceShader,
				hasCustomSurfaceShader,
				//hasDisplacementShader,
				shaderRibBox,
				path,
				currentJob
				);
			liqRIBMsg("[4] hasDisplacementShader=%d, m_ignoreDisplacements=%d", hasDisplacementShader, m_ignoreDisplacements );
			if( hasDisplacementShader && !m_ignoreDisplacements ) 
			{
				//liqShader & currentShader = liqGetShader( ribNode__->assignedDisp.object() );
				liqShader &currentShader = liqShaderFactory::instance().getShader( ribNode->assignedDisp.object() );

				liqRIBMsg("[3] liqglo_currentJob.isShadow=%d, currentShader.outputInShadow=%d", currentJob.pass, currentShader.outputInShadow );
				// per shader shadow pass override
				if( currentJob.pass != rpShadowMap || currentShader.outputInShadow )
				{
					currentShader.write();
				}
			}
		}


		liquid::RendererMgr::getInstancePtr()->getRenderer()->exportOneObject_reference(ribNode, currentJob);


		liquid::RendererMgr::getInstancePtr()->getRenderer()
			->oneObjectBlock_reference_attribute_end(ribNode, currentJob);

}
void liqRibTranslator::oneObjectBlock_data(
	const liqRibNodePtr &ribNode,
	const structJob &currentJob
	)
{
	CM_TRACE_FUNC("liqRibTranslatorNew::oneObjectBlock_data("<<ribNode->name.asChar()<<","<<currentJob.name.asChar()<<")");

	liquid::RendererMgr::getInstancePtr()->getRenderer()->exportOneObject_data(ribNode, currentJob);
}
//
MStatus liqRibTranslator::writeShader_(
									  const bool writeShaders__, 
									  const liqRibNodePtr &ribNode__,
									  const bool hasVolumeShader__,
									  const bool hasSurfaceShader__,
									  const bool hasCustomSurfaceShader__,
									  //const bool hasDisplacementShader__,
									  const MString &shaderRibBox__,
									  const MDagPath &path__,
									  const structJob &currentJob

									  )
{
	CM_TRACE_FUNC("liqRibTranslatorNew::writeShader_("
		<<writeShaders__<<","
		<<ribNode__->name.asChar()
		<<",v="<<hasVolumeShader__
		<<",s="<<hasSurfaceShader__
		<<",c="<<hasCustomSurfaceShader__<<","
		<<shaderRibBox__.asChar()<<","
		<<path__.fullPathName().asChar()<<","
		<<currentJob.name.asChar()<<")");
	const bool isShadowJob = currentJob.pass == rpShadowMap;//false 
	const bool isDeepShadowJob = currentJob.shadowType == stDeep;//false

	MStatus status;

	if( true ) 
	{
		liqRIBMsg("[5] hasVolumeShader=%d, m_ignoreVolumes=%d", hasVolumeShader__, m_ignoreVolumes );
		if( hasVolumeShader__ && !m_ignoreVolumes ) 
		{
			//liqShader& currentShader( liqGetShader( ribNode__->assignedVolume.object() ) );
			liqShader& currentShader = liqShaderFactory::instance().getShader( ribNode__->assignedVolume.object() );
			liqRIBMsg("[1] liqglo_currentJob.isShadow=%d, currentShader.outputInShadow=%d", isShadowJob, currentShader.outputInShadow );
			// per shader shadow pass override
			if( !isShadowJob || currentShader.outputInShadow )
			{
				//currentShader.write();//use ShadingGroup file reference(e.g. *.erapi/*.rmsg) instead.
			}
		}

		if( hasSurfaceShader__ && !m_ignoreSurfaces )
		{
			if( hasCustomSurfaceShader__ )
			{
				if( hasCustomSurfaceShader__ == liqCustomPxShaderNode )
				{  // Just call the write method of the custom shader
					MFnDependencyNode customShaderDepNode( ribNode__->assignedShader.object() );
					MPxNode *mpxNode = customShaderDepNode.userNode();
					liqCustomNode *customNode( NULL );
					if( mpxNode && ( customNode = dynamic_cast<liqCustomNode*>( mpxNode ) ) )
						customNode->liquidWrite();
					else
						;// Should never happen in theory ... but what is the way to report a problem ???
				}
				else//hasCustomSurfaceShader==liqRibBoxShader
				{ 
					// Default : just write the contents of the rib box
					liquid::RendererMgr::getInstancePtr()->getRenderer()
						->oneObjectBlock_reference_attribute_block2_writeShader_RibBox(shaderRibBox__.asChar());
				}
			}
			else//hasCustomSurfaceShader==liqRegularShaderNode
			{
				liquid::RendererMgr::getInstancePtr()->getRenderer()
					->oneObjectBlock_reference_attribute_block2_writeShader_RegularShader(ribNode__, currentJob);
			}
		}//if( hasSurfaceShader && !m_ignoreSurfaces )
		else{
			liquid::RendererMgr::getInstancePtr()->getRenderer()
				->oneObjectBlock_reference_attribute_block2_writeShader_HasNoSurfaceShaderORIngoreSurface(
					ribNode__, path__, m_shaderDebug
				);

		}//if( hasSurfaceShader && !m_ignoreSurfaces )else
	} //if( writeShaders ) 
 


	return MS::kSuccess;
}
MStatus liqRibTranslator::writeShader_forShadow(
									  /*const*/ bool writeShaders__, 
									  const liqRibNodePtr &ribNode__,
									  const bool hasVolumeShader__,
									  const bool hasSurfaceShader__,
									  const bool hasCustomSurfaceShader__,
									  //const bool hasDisplacementShader__,
									  const MString &shaderRibBox__,
									  const MDagPath &path__,
									  const structJob &currentJob

									  )
{
	CM_TRACE_FUNC("liqRibTranslatorNew::writeShader_forShadow("
		<<writeShaders__<<","
		<<ribNode__->name.asChar()
		<<",v="<<hasVolumeShader__
		<<",s="<<hasSurfaceShader__
		<<",c="<<hasCustomSurfaceShader__<<","
		<<shaderRibBox__.asChar()<<","
		<<path__.fullPathName().asChar()<<","
		<<currentJob.name.asChar()<<")");
	assert(currentJob.pass == rpShadowMap);

	//const bool isDeepShadowJob = ;
	
	if(    (  currentJob.shadowType != stDeep && !m_outputShadersInShadows ) // shadow
		|| (  currentJob.shadowType == stDeep && !m_outputShadersInDeepShadows ) )//deep shadow
	{
		writeShaders__ = false;
	}

	MStatus status;

	if( writeShaders__ ) 
	{
		liqRIBMsg("[5] hasVolumeShader=%d, m_ignoreVolumes=%d", hasVolumeShader__, m_ignoreVolumes );
		if( hasVolumeShader__ && !m_ignoreVolumes ) 
		{
			//liqShader& currentShader( liqGetShader( ribNode__->assignedVolume.object() ) );
			liqShader& currentShader = liqShaderFactory::instance().getShader( ribNode__->assignedVolume.object() );
			liqRIBMsg("[1] currentShader.outputInShadow=%d", currentShader.outputInShadow );
			// per shader shadow pass override
			if( /* !isShadowJob ||*/ currentShader.outputInShadow )
			{
				currentShader.write();
			}
		}

		if( hasSurfaceShader__ && !m_ignoreSurfaces )
		{
			if( hasCustomSurfaceShader__ )
			{
				if( hasCustomSurfaceShader__ == liqCustomPxShaderNode )
				{  // Just call the write method of the custom shader
					MFnDependencyNode customShaderDepNode( ribNode__->assignedShader.object() );
					MPxNode *mpxNode = customShaderDepNode.userNode();
					liqCustomNode *customNode( NULL );
					if( mpxNode && ( customNode = dynamic_cast<liqCustomNode*>( mpxNode ) ) )
						customNode->liquidWrite();
					else
						;// Should never happen in theory ... but what is the way to report a problem ???
				}
				else
				{ 
					
					liquid::RendererMgr::getInstancePtr()->getRenderer()->writeShader_forShadow_ribbox(shaderRibBox__.asChar());
					//refactor 37 begin
					// Default : just write the contents of the rib box
					//RiArchiveRecord( RI_VERBATIM, ( char* )shaderRibBox__.asChar() );
 					//RiArchiveRecord( RI_VERBATIM, "\n" );
					//refactor 37 end
				}
			}
			else
			{
				//liqShader& currentShader( liqGetShader( ribNode__->assignedShader.object() ) );
				liqShader& currentShader = liqShaderFactory::instance().getShader( ribNode__->assignedShader.object() );

				liquid::RendererMgr::getInstancePtr()->getRenderer()->F1(ribNode__, currentShader);

				liqRIBMsg("[2] currentShader.outputInShadow=%d", currentShader.outputInShadow );
				// per shader shadow pass override
				if( /*!isShadowJob ||*/ currentShader.outputInShadow )
				{
					currentShader.write();
				}

				//if( outputSurfaceShader )
				//{
				//	scoped_array< liqToken > tokenArray( new liqToken[ currentShader.tokenPointerArray.size() ] );
				//	scoped_array< liqPointer > pointerArray( new liqPointer[ currentShader.tokenPointerArray.size() ] );
				//	assignTokenArrays( currentShader.tokenPointerArray.size(), &currentShader.tokenPointerArray[ 0 ], tokenArray.get(), pointerArray.get() );

				//	char* shaderFileName;
				//	LIQ_GET_SHADER_FILE_NAME( shaderFileName, liqglo_shortShaderNames, currentShader );

				//	// check shader space transformation
				//	if( currentShader.shaderSpace != "" )
				//	{
				//		RiTransformBegin();
				//		RiCoordSysTransform( ( liqString )currentShader.shaderSpace.asChar() );
				//	}
				//	// output shader
				//	// its one less as the tokenPointerArray has a preset size of 1 not 0
				//	int shaderParamCount = currentShader.tokenPointerArray.size() - 1;
				//	RiSurfaceV ( shaderFileName, shaderParamCount, tokenArray.get(), pointerArray.get() );
				//	if( currentShader.shaderSpace != "" )
				//		RiTransformEnd();
				//}
			}
		}else{//if( hasSurfaceShader && !m_ignoreSurfaces )
			liquid::RendererMgr::getInstancePtr()->getRenderer()->F2(m_shaderDebug, ribNode__);

			if( !m_ignoreSurfaces ) 
			{
				//refactor 38 begin
				liquid::RendererMgr::getInstancePtr()->getRenderer()
					->writeShader_forShadow_forSpecialTypes(ribNode__, m_shaderDebug, path__);
// 				MObject shadingGroup = ribNode__->assignedShadingGroup.object();
// 				MObject shader = ribNode__->findShader();
// 				//
// 				// here we check for regular shader nodes first
// 				// and assign default shader to shader-less nodes.
// 				//
// 				if( m_shaderDebug ) {
// 					liqRIBMsg("shader debug is turned on, so the surface is constant.");
// 					RiSurface( "constant", RI_NULL );
// 					LIQDEBUGPRINTF("add more constant parameters here. take /RMS-1.0.1-Maya2008/lib/shaders/src/mtorBlinn.sl as an example.(?)");
// 				}
//  				//else if( shader.apiType() == MFn::kLambert ){ 
//  				//	RiSurface( "matte", RI_NULL );
//  				//	LIQDEBUGPRINTF("add more lambert parameters here. take //RMS-1.0.1-Maya2008/lib/shaders/src/mtorLambert.sl as an example.");
//  				//}else if( shader.apiType() == MFn::kPhong ) {
//  				//	RiSurface( "plastic", RI_NULL );
//  				//	LIQDEBUGPRINTF("add more phong parameters here. take /RMS-1.0.1-Maya2008/lib/shaders/src/mtorPhong.sl as an example.");
//  				//}
// 				else if( path__.hasFn( MFn::kPfxHair ) ) 
// 				{
// 					// get some of the hair system parameters
// 					liqFloat translucence = 0, specularPower = 0;
// 					liqColor specularColor;
// 
// 					getPfxHairData(path__, translucence, specularPower, specularColor);
// 
// 					RiSurface(  "liquidpfxhair",
// 						"float specularpower", &specularPower,
// 						"float translucence",  &translucence,
// 						"color specularcolor", &specularColor,
// 						RI_NULL );
// 				} 
// 				else if( path__.hasFn( MFn::kPfxToon ) ) {
// 					RiSurface( "liquidpfxtoon", RI_NULL );
// 				}else if( path__.hasFn( MFn::kPfxGeometry ) ){
// 					RiSurface( "liquidpfx", RI_NULL );
// 				}else {
// 					//RiSurface( "plastic", RI_NULL );//ymesh-branch r773 use this
// 					MFnDependencyNode shaderFn(shader);
// 					RiSurface( const_cast<char*>(shaderFn.name().asChar()), RI_NULL );
// 				}
 				//refactor 38 end

			}
		}//if( hasSurfaceShader && !m_ignoreSurfaces )else
	} //if( writeShaders ) 
	else if( currentJob.shadowType == stDeep ) 
	{
		liqRIBMsg("[7] liqglo_currentJob[   ], hasSurfaceShader=%d, hasCustomSurfaceShader=%d",
			hasSurfaceShader__, hasCustomSurfaceShader__ );

		// if the current job is a deep shadow,
		// we still want to output primitive color and opacity and surface shader
		// In case of custom shaders, what should we do ? Stephane.
		if( hasSurfaceShader__ && ! hasCustomSurfaceShader__ ) 
		{
			//liqShader & currentShader = liqGetShader( ribNode__->assignedShader.object());
			liqShader &currentShader = liqShaderFactory::instance().getShader( ribNode__->assignedShader.object() );

			liquid::RendererMgr::getInstancePtr()->getRenderer()->F1(ribNode__, currentShader);

			liqRIBMsg("[8] currentShader[.name=%s, .filename=%s, .outputInShadow=%d]", currentShader.getName().c_str(), currentShader.getShaderFileName().c_str(), currentShader.outputInShadow );
			if(currentShader.outputInShadow){
				currentShader.write();
			}
		} 
		else //if( hasSurfaceShader__ && ! hasCustomSurfaceShader__ ) 
		{
			liquid::RendererMgr::getInstancePtr()->getRenderer()->F2(false, ribNode__ );

			//refactor 39 begin
			liquid::RendererMgr::getInstancePtr()->getRenderer()
				->writeShader_forDeepShadow_forSpecialTypes(ribNode__, path__);
// 			if( path__.hasFn( MFn::kPfxHair ) ) 
// 			{
// 				// get some of the hair system parameters
// 				liqFloat translucence = 0, specularPower = 0;
// 				liqColor specularColor;
// 
// 				getPfxHairData(path__, translucence, specularPower, specularColor);
// 
// 				RiSurface(  "liquidPfxHair",
// 					"float specularPower", &specularPower,
// 					"float translucence",  &translucence,
// 					"color specularColor", &specularColor,
// 					RI_NULL );
// 			}
			//refactor 39 end

		}
	}else {
		//refactor 40 begin
		liquid::RendererMgr::getInstancePtr()->getRenderer()
			->writeShader_forShadow_NullShader(ribNode__, path__);
		//refactor 40 end
	}


	return MS::kSuccess;
}
//
