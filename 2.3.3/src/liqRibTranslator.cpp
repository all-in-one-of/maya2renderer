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


#include "./common/prerequest_std.h"

// Renderman Headers
//extern "C" {
#include "ri_interface.h"
//}


// Maya headers
#include "./common/prerequest_maya.h"
#include "./common/mayacheck.h"
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
#include "renderermgr.h"
#include "shadergraph/shadermgr.h"

using namespace boost;
using namespace std;

typedef int RtError;


#if defined(_WIN32)/* && !defined(DEFINED_LIQUIDVERSION)*/
// unix build gets this from the Makefile
extern const char *LIQUIDVERSION;
#endif

//void liqRibTranslator::freeShaders( void )
//{
//  LIQDEBUGPRINTF( "-> freeing shader data.\n" );
//  m_shaders.clear();
//  LIQDEBUGPRINTF( "-> finished freeing shader data.\n" );
//}

// Hmmmmm should change magic to Liquid
MString liqRibTranslator::magic("##Liquid");

#if(Refactoring==1)
static structJob liqglo_currentJob;
#endif
MComputation liqRibTranslator::m_escHandler;
/**
* Creates a new instance of the translator.
*/
void *liqRibTranslator::creator()
{
	mInstance = new liqRibTranslator();	
	return mInstance;// will be deleted by maya
}

// void liqRibTranslator::scanExpressions( liqRibLightData *light )
// {
// 	if( light != NULL ) 
// 	{
// 		vector<liqTokenPointer>::iterator iter = light->tokenPointerArray.begin();
// 		while ( iter != (light->tokenPointerArray.end()) ) 
// 		{
// 			if( iter->getParameterType() == rString ) 
// 			{
// 				liqTokenPointer i = *iter;
// 				processExpression( &i, light );
// 			}
// 			++iter;
// 		}
// 	}
// }

//liqShader & liqRibTranslator::liqGetShader( MObject shaderObj )
//{
//  MString rmShaderStr;
//
//  MFnDependencyNode shaderNode( shaderObj );
//  MPlug rmanShaderNamePlug = shaderNode.findPlug( MString( "rmanShaderLong" ) );
//  rmanShaderNamePlug.getValue( rmShaderStr );
//
//  LIQDEBUGPRINTF( "-> Using Renderman Shader %s\n", rmShaderStr.asChar() );
//
//  vector<liqShader>::iterator iter = m_shaders.begin();
//  while ( iter != m_shaders.end() )
//  {
//    string shaderNodeName = shaderNode.name().asChar();
//    if( iter->name == shaderNodeName ) 
//    {
//      // Already got it : nothing to do
//      return *iter;
//    }
//    ++iter;
//  }
//  liqShader currentShader( shaderObj );
//  m_shaders.push_back( currentShader );
//  return m_shaders.back();
//}

//r773  Don't seem to be called
//
// MStatus liqRibTranslator::liqShaderParseVectorAttr ( liqShader & currentShader, MFnDependencyNode & shaderNode, const char * argName, ParameterType pType )
// {
// 	CM_TRACE_FUNC("liqRibTranslator::liqShaderParseVectorAttr(&currentShader,&shaderNode,"<<argName<<","<<pType<<")");
// 
// 	MStatus status = MS::kSuccess;
// 	MPlug triplePlug = shaderNode.findPlug( argName, &status );
// 	if( status == MS::kSuccess ) 
// 	{
// 		float x, y, z;
// 		currentShader.tokenPointerArray.rbegin()->set( argName, pType, false, false, 0 );
// 		triplePlug.child( 0 ).getValue( x );
// 		triplePlug.child( 1 ).getValue( y );
// 		triplePlug.child( 2 ).getValue( z );
// 		currentShader.tokenPointerArray.rbegin()->setTokenFloat( 0, x, y, z );
// 		currentShader.tokenPointerArray.push_back( liqTokenPointer() );
// 		//currentShader.numTPV++;
// 	}
// 	return status;
// }
void liqRibTranslator::printProgress( unsigned stat, unsigned numFrames, unsigned where )
// for printing the progress to the Maya Console or stdout. If alfred is being used it
// will print it in a format that causes the correct formatting for the progress meters
//
// TODO - should be able to set the progress output format somehow to cater for
// different render pipelines - with a user-specifiable string in printf format?
{
	//CM_TRACE_FUNC("liqRibTranslator::printProgress("<<stat<<","<<numFrames<<","<<where<<")");

	float statSize   = ( ( 1 / ( float )( numFrames + 1 ) ) / 4 ) * ( float )stat * 100.0;
	float progressf  = ( ( ( float )where / ( float )( numFrames + 1 ) ) * 100.0 ) + statSize;
	int progress     = ( int ) progressf;

	if( liqglo.liquidBin ) 
		printf( "ALF_PROGRESS %d%%\n", progress );
	else 
	{
		stringstream progressOutput;
		progressOutput << "Progress: frame "<< where <<", " << progress << "%" << ends;
		liquidMessage( progressOutput.str(), messageInfo );
	}
}

/**
* Class constructor.
*/
liqRibTranslator::liqRibTranslator()
{
	//CM_TRACE_FUNC("liqRibTranslator::liqRibTranslator()");

	char* envTmp;
	if( ( envTmp = getenv( "TMP" ) ) ||
		( envTmp = getenv( "TEMP" ) ) ||
		( envTmp = getenv( "TEMPDIR" ) ) ) 
	{
		m_systemTempDirectory = envTmp;
		m_systemTempDirectory = liquidSanitizePath( m_systemTempDirectory );
		LIQ_ADD_SLASH_IF_NEEDED( m_systemTempDirectory );
	}
	else 
	{
#ifndef _WIN32
		m_systemTempDirectory = "/tmp/";
#else
		m_systemTempDirectory = "%SystemRoot%/Temp/";
#endif
	}



	liqglo.liqglo_shortShaderNames = false;
	liqglo.liqglo_relativeFileNames = false;

	liqglo.frameNumbers.clear();

	m_showProgress = false;
	liqglo.m_deferredBlockSize = 1;
	liqglo.m_deferredGen = false;

	m_outputHeroPass = true;
	m_useNewTranslator = true;
	m_OutputShaderGraph = false;
	m_outputDebugMsgToRib = false;
	m_outputShadowPass = false;
	liqglo.m_illuminateByDefault = false;
	m_liquidSetLightLinking = false;
	m_ignoreLights = false;
	m_ignoreSurfaces = false;
	m_ignoreDisplacements = false;
	m_ignoreVolumes = false;
	m_renderSelected = false;
	liqglo.m_exportReadArchive = false;
	liqglo.useNetRman = false;
	liqglo.remoteRender = false;
	useRenderScript = true;
	liqglo.cleanRib = false;
	cleanRenderScript = false;
	liqglo.liqglo_doBinary = false;
	liqglo.liqglo_doCompression = false;
	initCameraParameters();


	liqglo.liqglo_doMotion = false;          // matrix motion blocks
	liqglo.liqglo_doDef = false;             // geometry motion blocks
	liqglo.liqglo_relativeMotion = false;

	liqglo.liqglo_doExtensionPadding = false;       // pad the frame number in the rib file names
	liqglo.liqglo_doShadows = true;          // render shadows
	liqglo.liqglo_shapeOnlyInShadowNames = false;
	liqglo.liqglo_noSingleFrameShadows = false;
	liqglo.liqglo_singleFrameShadowsOnly = false;
	m_justRib = false;
	cleanShadows = 0;                 // render shadows
	cleanTextures = 0;                // render shadows
	
	initOtherParameters();

	depth = 1;

	m_animation = false;
	m_useFrameExt = true;  // Use frame extensions

	liqglo.liqglo_motionSamples = 2;
	liqglo.liqglo_motionFactor = 1.0f;
	liqglo.liqglo_FPS = 24.0;
	width = 360;
	height = 243;

	liqglo.liqglo_outPadding = 0;
	ignoreFilmGate = true;
	m_lazyCompute = false;
	m_outputShadersInShadows = false;
	m_outputShadersInDeepShadows = false;
	m_outputLightsInDeepShadows = false;
	liqglo.m_alfredExpand = false;
#ifdef DEBUG
	debugMode = 1;
#else
	debugMode = 0;
#endif
	m_errorMode = 0;
//	extension = ".rib";

	initLimitsParameters();

	liqglo.liqglo_shutterTime = 0.5;
	liqglo.liqglo_shutterEfficiency = 1.0;

	m_blurTime = 1.0;
	liqglo.fullShadowRib = false;
	baseShadowName = "";
	baseSceneName = "";

	liqglo.liqglo_projectDir = m_systemTempDirectory;

	m_tmpDir = "rmantmp/";
	liqglo.liqglo_preReadArchive.clear();
	liqglo.liqglo_preRibBox.clear();
	liqglo.m_alfredTags.clear();
	liqglo.m_alfredServices.clear();
	liqglo.m_dirmaps.clear();
	liqglo.m_defGenKey.clear();
	liqglo.m_defGenService.clear();
	m_preFrameMel.clear();
	m_postFrameMel.clear();
	liqglo.m_preCommand.clear();
	m_preJobCommand.clear();
	m_postJobCommand.clear();
	m_postFrameCommand.clear();
	m_preFrameCommand.clear();
	m_preTransformMel.clear();
	m_postTransformMel.clear();
	m_preShapeMel.clear();
	m_postShapeMel.clear();
	liqglo.m_outputComments = false;
	m_shaderDebug = false;






#ifdef AIR
	m_renderCommand = "air";
#elif defined( AQSIS )
	m_renderCommand = "aqsis";
#elif defined( DELIGHT )
	m_renderCommand = "renderdl";
#elif defined( PIXIE )
	m_renderCommand = "rndr";
#elif defined( PRMAN )
#ifdef _WIN32
	m_renderCommand = "prman";
#else
	m_renderCommand = "render";
#endif
#endif
	
	 m_minCPU =  m_maxCPU = 1;

	m_ribgenCommand = "liquid";
	createOutputDirectories = true;
	liqglo.liqglo_expandShaderArrays = false;



	initStatisticsParameters();

	initHinderParameters();


	liqglo.liqglo_isShadowPass = false;



	liqglo.m_preWorldRIB.clear();
	liqglo.m_postWorldRIB.clear();
	m_preGeomRIB.clear();

	m_renderScriptFormat = XML;
	m_exportOnlyObjectBlock = false;

	liqglo.liqglo_useMtorSubdiv = false;
	liqglo.liqglo_outputMayaPolyCreases = false;
	liqglo.liqglo_renderAllCurves = false;
	liqglo.liqglo_hider = htHidden;

	liqglo.liqglo_ribDir = "rib";
	liqglo.liqglo_textureDir = "rmantex";

	liqglo.liqglo_shaderPath = "&:@:.:~:rmanshader";
	liqglo.liqglo_texturePath = "&:@:.:~:" + liqglo.liqglo_textureDir;
	liqglo.liqglo_archivePath = "&:@:.:~:" + liqglo.liqglo_ribDir;
	liqglo.liqglo_proceduralPath = "&:@:.:~";
	liqglo.renderCamera.clear();
	liqglo.liqglo_shotName.clear();
	liqglo.liqglo_shotVersion.clear();
	liqglo.liqglo_layer.clear();
	originalLayer.clear();

	// raytracing
	liqglo.rt_useRayTracing = false;
	liqglo.rt_traceBreadthFactor = 1.0;
	liqglo.rt_traceDepthFactor = 1.0;
	liqglo.rt_traceMaxDepth = 10;
	liqglo.rt_traceSpecularThreshold = 10.0;
	liqglo.rt_traceRayContinuation = 1;
	liqglo.rt_traceCacheMemory = 30720;
	liqglo.rt_traceDisplacements = false;
	liqglo.rt_traceSampleMotion = false;
	liqglo.rt_traceBias = 0.05;
	liqglo.rt_traceMaxDiffuseDepth = 2;
	liqglo.rt_traceMaxSpecularDepth = 2;

  liqglo.rt_irradianceGlobalHandle="";
  liqglo.rt_irradianceGlobalFileMode=0;

  liqglo.rt_photonGlobalHandle = "";
  liqglo.rt_causticGlobalHandle = "";
  liqglo.rt_photonShadingModel = liqRibNode::photon::SHADINGMODEL_MATTE;
  liqglo.rt_photonEstimator = 0;
	
	MString tmphome( getenv( "LIQUIDHOME" ) );
	tmphome = liquidSanitizeSearchPath( tmphome );

	if( tmphome != "" ) 
	{
		liqglo.liqglo_shaderPath += ":" + tmphome + "/lib/shaders";
		liqglo.liqglo_texturePath += ":" + tmphome + "/lib/textures";
		liqglo.liqglo_archivePath += ":" + tmphome + "/lib/rib";
	}

	liqglo.liqglo_shaderPath += ":" + tmphome + "/lib/shaders";
	liqglo.liqglo_texturePath += ":" + tmphome + "/lib/textures";
	liqglo.liqglo_archivePath += ":" + tmphome + "/lib/rib";

	m_exportSpecificList = false;
	m_exportOnlyObjectBlock = false;

	m_skipVisibilityAttributes = false;
	m_skipShadingAttributes = false;
	m_skipRayTraceAttributes = false;

	m_isStereoCamera = false;
}


/**
* Class destructor
*/
liqRibTranslator::~liqRibTranslator()
{
	//CM_TRACE_FUNC("liqRibTranslator::~liqRibTranslator()");

	//delete mInstance;
	//mInstance = NULL;
}

/**
* Error handling function.
* This gets called when the RIB library detects an error.
*/
#if defined( DELIGHT ) || defined( ENTROPY ) ||  defined( PRMAN ) || defined( AIR ) || defined( PIXIE ) || defined( GENERIC_RIBLIB ) 
void liqRibTranslatorErrorHandler( RtInt code, RtInt severity, char* message )
#else
void liqRibTranslatorErrorHandler( RtInt code, RtInt severity, const char* message )
#endif
{
	liquidMessage2(messageError, "The renderman library is reporting and error! Code: %d  Severity: %d", code, severity );
	MString error( message );
	throw error;
}

// MSyntax liqRibTranslator::syntax()
// {
// 	//CM_TRACE_FUNC("liqRibTranslator::syntax()");

// 	MSyntax syntax;

// 	syntax.addFlag("lr",    "launchRender");
// 	syntax.addFlag("nolr",  "noLaunchRender");
// 	syntax.addFlag("GL",    "useGlobals");
// 	syntax.addFlag("sel",   "selected");
// 	syntax.addFlag("ra",    "readArchive");
// 	syntax.addFlag("acv",   "allCurves");
// 	syntax.addFlag("tif",   "tiff");
// 	syntax.addFlag("dof",   "dofOn");
// 	syntax.addFlag("bin",   "doBinary");
// 	syntax.addFlag("sh",    "shadows");
// 	syntax.addFlag("nsh",   "noShadows");
// 	syntax.addFlag("zip",   "doCompression");
// 	syntax.addFlag("cln",   "cleanRib");
// 	syntax.addFlag("pro",   "progress");
// 	syntax.addFlag("mb",    "motionBlur");
// 	syntax.addFlag("rmot",  "relativeMotion");
// 	syntax.addFlag("db",    "deformationBlur");
// 	syntax.addFlag("d",     "debug");
// 	syntax.addFlag("net",   "netRender");
// 	syntax.addFlag("fsr",   "fullShadowRib");
// 	syntax.addFlag("rem",   "remote");
// 	syntax.addFlag("rs",    "renderScript");
// 	syntax.addFlag("nrs",   "noRenderScript");
// 	syntax.addFlag("err",   "errHandler");
// 	syntax.addFlag("sdb",   "shaderDebug");
// 	syntax.addFlag("n",     "sequence",         MSyntax::kLong, MSyntax::kLong, MSyntax::kLong);
// 	syntax.addFlag("fl",    "frameList",        MSyntax::kString);
// 	syntax.addFlag("m",     "mbSamples",        MSyntax::kLong);
// 	syntax.addFlag("dbs",   "defBlock");
// 	syntax.addFlag("cam",   "camera",           MSyntax::kString);
// 	syntax.addFlag("rcam",  "rotateCamera");
// 	syntax.addFlag("s",     "samples",          MSyntax::kLong);
// 	syntax.addFlag("rnm",   "ribName",          MSyntax::kString);
// 	syntax.addFlag("pd",    "projectDir",       MSyntax::kString);
// 	syntax.addFlag("rel",   "relativeDirs");
// 	syntax.addFlag("prm",   "preFrameMel",      MSyntax::kString);
// 	syntax.addFlag("pom",   "postFrameMel",     MSyntax::kString);
// 	syntax.addFlag("rid",   "ribdir",           MSyntax::kString);
// 	syntax.addFlag("txd",   "texdir",           MSyntax::kString);
// 	syntax.addFlag("tmd",   "tmpdir",           MSyntax::kString);
// 	syntax.addFlag("pid",   "picdir",           MSyntax::kString);
// 	syntax.addFlag("pec",   "preCommand",       MSyntax::kString);
// 	syntax.addFlag("poc",   "postJobCommand",   MSyntax::kString);
// 	syntax.addFlag("pof",   "postFrameCommand", MSyntax::kString);
// 	syntax.addFlag("prf",   "preFrameCommand",  MSyntax::kString);
// 	syntax.addFlag("rec",   "renderCommand",    MSyntax::kString);
// 	syntax.addFlag("rgc",   "ribgenCommand",    MSyntax::kString);
// 	syntax.addFlag("blt",   "blurTime",         MSyntax::kDouble);
// 	syntax.addFlag("sr",    "shadingRate",      MSyntax::kDouble);
// 	syntax.addFlag("bs",    "bucketSize",       MSyntax::kLong, MSyntax::kLong);
// 	syntax.addFlag("pf",    "pixelFilter",      MSyntax::kLong, MSyntax::kLong, MSyntax::kLong);
// 	syntax.addFlag("gs",    "gridSize",         MSyntax::kLong);
// 	syntax.addFlag("txm",   "texmem",           MSyntax::kLong);
// 	syntax.addFlag("es",    "eyeSplits",        MSyntax::kLong);
// 	syntax.addFlag("ar",    "aspect",           MSyntax::kDouble);
// 	syntax.addFlag("x",     "width",            MSyntax::kLong);
// 	syntax.addFlag("y",     "height",           MSyntax::kLong);
// 	syntax.addFlag("cw",    "cropWindow",       MSyntax::kLong, MSyntax::kLong, MSyntax::kLong, MSyntax::kLong);
// 	syntax.addFlag("def",   "deferred");
// 	syntax.addFlag("ndf",   "noDef");
// 	syntax.addFlag("pad",   "padding",          MSyntax::kLong);
// 	syntax.addFlag("rgo",   "ribGenOnly");
// 	syntax.addFlag("sfso",  "singleFrameShadowsOnly");
// 	syntax.addFlag("nsfs",  "noSingleFrameShadows");
// 	syntax.addFlag("rv",    "renderView");
// 	syntax.addFlag("rvl",   "renderViewlocal");
// 	syntax.addFlag("rvp",   "renderViewPort",  MSyntax::kLong);
// 	syntax.addFlag("shn",   "shotName",        MSyntax::kString);
// 	syntax.addFlag("shv",   "shotVersion",     MSyntax::kString);
// 	syntax.addFlag("lyr",   "layer",           MSyntax::kString);
// //	syntax.addFlag("lrp",   "currentLayerRifParameters",           MSyntax::kString);
// 	syntax.addFlag("obl",   "objectList", MSyntax::kString, MSyntax::kString, MSyntax::kString, MSyntax::kString, MSyntax::kString, MSyntax::kString);
// 	syntax.addFlag("oob",   "onlyObjectBlock");
// 	syntax.addFlag("igs",   "ignoreSurfaces");
// 	syntax.addFlag("no_igs","noIgnoreSurfaces");
// 	syntax.addFlag("igd",   "ignoreDisplacements");
// 	syntax.addFlag("no_igd","noIgnoreDisplacements");
// 	syntax.addFlag("igv",   "ignoreVolumes");
// 	syntax.addFlag("no_igv","noIgnoreVolumes");
// 	syntax.addFlag("no_ufe","noUseFrameExtension");
// 	syntax.addFlag("skv",   "skipVisibilityAttributes");
// 	syntax.addFlag("sks",   "skipShadingAttributes");
// 	syntax.addFlag("skr",   "skipRayTraceAttributes");

// 	syntax.addFlag("easp",   "exportAllShadersParams");
// 	syntax.addFlag("rhcn",   "ribHasCameraName");
// 	return syntax;
// }

/**
* Read the values from the command line and set the internal values.
*/
// MStatus liqRibTranslator::liquidDoArgs( MArgList args )
// {
// 	//CM_TRACE_FUNC("liqRibTranslator::liquidDoArgs(args)");
// 
// 	MStatus status;
// 	MString argValue;
// 
// 	LIQDEBUGPRINTF( "-> processing arguments\n" );
// 
// 	// Parse the arguments and set the options.
// 	if( args.length() == 0 ) 
// 	{
// 		liquidMessage( "Doing nothing, no parameters given", messageError );
// 		return MS::kFailure;
// 	}
// 	// find the activeView for previews;
// 	width        = M3dView::active3dView().portWidth();
// 	height       = M3dView::active3dView().portHeight();
// 
// 	// get the current project directory
// 	MString MELCommand = "workspace -q -rd";
// 	MString MELReturn;
// 	MGlobal::executeCommand( MELCommand, MELReturn );
// 	liqglo.liqglo_projectDir = MELReturn;
// 
// 	// get the current scene name
// 	//liqglo.liqglo_sceneName = liquidTransGetSceneName();//omitted by yaoyansi
// 
// 	// setup default animation parameters
// 	liqglo.frameNumbers.push_back( ( int )MAnimControl::currentTime().as( MTime::uiUnit() ) );
// 
// 	// check to see if the correct project directory was found
// 	/*if( !fileFullyAccessible( liqglo_projectDir ) ) {
// 	liqglo_projectDir = m_systemTempDirectory;
// 	liquidMessage( "Trying to set project directory to system temp directory '" + string( liqglo_projectDir.asChar() ) + "'.", messageWarning );
// 	}*/
// 	LIQ_ADD_SLASH_IF_NEEDED( liqglo.liqglo_projectDir );
// 	if( !fileFullyAccessible( liqglo.liqglo_projectDir ) ) 
// 	{
// 		liquidMessage( "Cannot find project directory, '" + string( liqglo.liqglo_projectDir.asChar() ) + "'. Defaulting to system temp directory!", messageWarning );
// 		liqglo.liqglo_projectDir = m_systemTempDirectory;
// 	}
// 
// 	bool GL_read( false );
// 
// 	for( unsigned i( 0 ); i < args.length(); i++ ) 
// 	{
// 		MString arg = args.asString( i, &status );
// 		MString err, err_fmt = "error in ^1s parameter";
// 		err.format( err_fmt, arg);
// 		LIQCHECKSTATUS(status, err );
// 
// 		if((arg == "-lr") || (arg == "-launchRender"))			liqglo.launchRender = true;
// 		else if((arg == "-nolr") || (arg == "-noLaunchRender")) liqglo.launchRender = false;
// 		else if((arg == "-GL") || (arg == "-useGlobals")) 
//		{
// 			//load up all the render global parameters!
// 			if( liquidInitGlobals() && !GL_read ) 
// 				liquidReadGlobals();
// 			GL_read = true;
// 		} 
// 		else if((arg == "-sel") || (arg == "-selected"))		m_renderSelected = true;
// 		else if((arg == "-ra") || (arg == "-readArchive"))		liqglo.m_exportReadArchive = true;
// 		else if((arg == "-acv") || (arg == "-allCurves"))		liqglo.liqglo_renderAllCurves = true;
// 		else if((arg == "-tif") || (arg == "-tiff"))			liqglo.outFormat = "tiff";
// 		else if((arg == "-dof") || (arg == "-dofOn"))			liqglo.doDof = true;
// 		else if((arg == "-bin") || (arg == "-doBinary"))		liqglo.liqglo_doBinary = true;
// 		else if((arg == "-sh") || (arg == "-shadows"))			liqglo.liqglo_doShadows = true;
// 		else if((arg == "-nsh") || (arg == "-noShadows"))		liqglo.liqglo_doShadows = false;
// 		else if((arg == "-zip") || (arg == "-doCompression"))	liqglo.liqglo_doCompression = true;
// 		else if((arg == "-cln") || (arg == "-cleanRib"))		liqglo.cleanRib = true;
// 		else if((arg == "-cmd") || (arg == "-createMissingDirs")) createOutputDirectories = true;
// 		else if((arg == "-pro") || (arg == "-progress"))		m_showProgress = true;
// 		else if((arg == "-mb") || (arg == "-motionBlur"))		liqglo.liqglo_doMotion = true;
// 		else if((arg == "-db") || (arg == "-deformationBlur"))	liqglo.liqglo_doDef = true;
// 		else if((arg == "-d") || (arg == "-debug"))				debugMode = 1;
// 		else if((arg == "-net") || (arg == "-netRender"))		liqglo.useNetRman = true;
// 		else if((arg == "-fsr") || (arg == "-fullShadowRib"))	liqglo.fullShadowRib = true;
// 		else if((arg == "-rem") || (arg == "-remote"))			liqglo.remoteRender = true;
// 		else if((arg == "-rs") || (arg == "-renderScript"))		useRenderScript = true;
// 		else if((arg == "-nrs") || (arg == "-noRenderScript"))	useRenderScript = false;
// 		else if((arg == "-err") || (arg == "-errHandler"))		m_errorMode = 1;
// 		else if((arg == "-sdb") || (arg == "-shaderDebug"))		m_shaderDebug = true;
// 		else if((arg == "-rmot") || (arg == "-relativeMotion"))	liqglo.liqglo_relativeMotion = true;
// 		else if((arg == "-rcam") || (arg == "-rotateCamera"))	liqglo.liqglo_rotateCamera = true;
// 		else if((arg == "-rel") || (arg == "-relativeDirs"))	liqglo.liqglo_relativeFileNames = true;
// 		else if((arg == "-def") || (arg == "-deferred"))		liqglo.m_deferredGen = true;
// 		else if((arg == "-ndf") || (arg == "-noDef"))			liqglo.m_deferredGen = false;
// 		else if((arg == "-rgo") || (arg == "-ribGenOnly"))		m_justRib = true;
// 		else if((arg == "-rv") || (arg == "-renderView"))		liqglo.m_renderView = true;
// 		else if((arg == "-rvl") || (arg == "-renderViewLocal")) liqglo.m_renderViewLocal = true;
// 		else if((arg == "-nsfs") || (arg == "-noSingleFrameShadows")) liqglo.liqglo_noSingleFrameShadows = true;
// 		else if((arg == "-sfso") || (arg == "-singleFrameShadowsOnly"))liqglo.liqglo_singleFrameShadowsOnly = true;
// 		else if((arg == "-shn") || (arg == "-shotName"))		liqglo.liqglo_shotName = args.asString( i, &status );
// 		else if((arg == "-shv") || (arg == "-shotVersion"))		liqglo.liqglo_shotVersion = args.asString( i, &status );
// 		else if((arg == "-n") || (arg == "-sequence")) 
//		{
// 			int first( argValue.asInt() );
//			LIQCHECKSTATUS(status, err);
// 			argValue = args.asString( ++i, &status );
// 			int last( argValue.asInt() );
//			LIQCHECKSTATUS(status, err);
// 			argValue = args.asString( ++i, &status );
// 			int step( argValue.asInt() );
//			LIQCHECKSTATUS(status, err);
// 			m_animation = true;
// 			if( first > last ) 
// 				step = -abs( step );
// 			// Fill our vector with frames
// 			liqglo.frameNumbers.clear();
// 			for ( int frame( first ); frame <= last; frame += step ) 
// 				liqglo.frameNumbers.push_back( frame );
// 		} 
// 		else if((arg == "-fl") || (arg == "-frameList") || (arg == "-t") || (arg == "-timeRange") || (arg == "--timerange")) 
//		{
// 			argValue = args.asString( ++i, &status );
//			LIQCHECKSTATUS(status, err);
// 			// fill our vector with frames
// 			liqglo.frameNumbers = generateFrameNumbers( string( argValue.asChar() ) );
// 			if( liqglo.frameNumbers.size() ) { 
// 				m_animation = true;
// 				//liquidMessage2(messageInfo, ">>> render frames: ");
// 				//for( unsigned i(0) ; i < frameNumbers.size() ; i++ )
// 				//  liquidMessage2(messageInfo, "%d ", frameNumbers[i] );
// 				//liquidMessage2(messageInfo, "\n");
// 			}
// 		} 
// 		else if((arg == "-m") || (arg == "-mbSamples")) 
//		{
// 			argValue = args.asString( ++i, &status );
// 			liqglo.liqglo_motionSamples = argValue.asInt();
//			LIQCHECKSTATUS(status, err);
// 		} 
// 		else if((arg == "-dbs") || (arg == "-defBlock")) 
//		{
// 			argValue = args.asString( ++i, &status );
// 			liqglo.m_deferredBlockSize = argValue.asInt();
//			LIQCHECKSTATUS(status, err);
// 		} 
// 		else if((arg == "-cam") || (arg == "-camera")) 
//		{
// 			MString parsingString = args.asString( ++i, &status );
// 			liqglo.renderCamera = parseString( parsingString );
// 			liqglo.liqglo_renderCamera = liqglo.renderCamera;
//			LIQCHECKSTATUS(status, err);
// 		} 
// 
// 		else if((arg == "-s") || (arg == "-samples")) 
//		{
// 			argValue = args.asString( ++i, &status );
// 			liqglo.pixelSamples = argValue.asInt();
//			LIQCHECKSTATUS(status, err);
// 		} 
// 		else if((arg == "-rnm") || (arg == "-ribName")) 
//		{
// 			MString parsingString = args.asString( ++i, &status );;
// 			//Note:  -ribName is set to liqglo.liqglo_sceneName originally. 
// 			//       Because we replace liqglo.liqglo_sceneName with liquidTransGetSceneName(), and delete liqglo.liqglo_sceneName,
// 			//       we use liqglo.liqglo_ribName to store the ribName which passed by commandline parameter '-ribName'
// 			//  [2/17/2012 yaoyansi]
// 			assert(0&&"warrning: ribName is store in liqglo.liqglo_ribName instead of liqglo.liqglo_sceneName.[2/17/2012 yaoyansi]");
// #if 0
// 			liqglo.liqglo_sceneName = parseString( parsingString );//liquidMaya r772 use this one.
// #else
// 			liqglo.liqglo_ribName = parseString( parsingString );
// #endif
//			LIQCHECKSTATUS(status, err);
// 		} 
// 		else if((arg == "-pd") || (arg == "-projectDir")) 
//		{
// 			MString parsingString = args.asString( ++i, &status );
// 			liqglo.liqglo_projectDir = parseString( parsingString, false );
// 			LIQ_ADD_SLASH_IF_NEEDED( liqglo.liqglo_projectDir );
// #ifdef _WIN32
// 			int dirMode = 6; // dummy arg
// 			int mkdirMode = 0;
// #else
// 			mode_t dirMode, mkdirMode;
// 			dirMode = R_OK|W_OK|X_OK|F_OK;
// 			mkdirMode = S_IRWXU|S_IRWXG|S_IRWXO;
// #endif
// 			if( createOutputDirectories ) 
// 				makeFullPath( liqglo.liqglo_projectDir.asChar(), mkdirMode );
// 			if( !fileFullyAccessible( liqglo.liqglo_projectDir ) ) 
// 			{
// 				liquidMessage( "Cannot find or access Maya project directory; defaulting to system temp directory!", messageWarning );
// 				liqglo.liqglo_projectDir = m_systemTempDirectory;
// 			}
//			LIQCHECKSTATUS(status, err);
// 		} 
// 		else if((arg == "-prm") || (arg == "-preFrameMel")) 
//		{
// 			m_preFrameMel =  args.asString( ++i, &status );
//			LIQCHECKSTATUS(status, err);
// 		} 
// 		else if((arg == "-pom") || (arg == "-postFrameMel")) 
//		{
// 			m_postFrameMel = args.asString( ++i, &status );
//			LIQCHECKSTATUS(status, err);
// 		} 
// 		else if((arg == "-rid") || (arg == "-ribdir")) 
//		{
// 			MString parsingString = args.asString( ++i, &status );
// 			liqglo.liqglo_ribDir = parseString( parsingString, false );
//			LIQCHECKSTATUS(status, err);
// 		} 
// 		else if((arg == "-txd") || (arg == "-texdir")) 
//		{
// 			MString parsingString = args.asString( ++i, &status );
// 			liqglo.liqglo_textureDir = parseString( parsingString, false );
//			LIQCHECKSTATUS(status, err);
// 		} 
// 		else if((arg == "-tmd") || (arg == "-tmpdir")) 
//		{
// 			MString parsingString = args.asString( ++i, &status );
// 			m_tmpDir = parseString( parsingString, false );
//			LIQCHECKSTATUS(status, err);
// 		} 
// 		else if((arg == "-pid") || (arg == "-picdir")) 
//		{
// 			MString parsingString = args.asString( ++i, &status );
// 			liqglo.m_pixDir = parseString( parsingString, false );
//			LIQCHECKSTATUS(status, err);
// 		} 
// 		else if((arg == "-pec") || (arg == "-preCommand")) 
//		{
// 			liqglo.m_preCommand = args.asString( ++i, &status );
//			LIQCHECKSTATUS(status, err);
// 		} 
// 		else if((arg == "-poc") || (arg == "-postJobCommand")) 
//		{
// 			MString varVal = args.asString( ++i, &status );
// 			m_postJobCommand = parseString( varVal );
//			LIQCHECKSTATUS(status, err);
// 		} 
// 		else if((arg == "-pof") || (arg == "-postFrameCommand")) 
//		{
// 			m_postFrameCommand = args.asString( ++i, &status );
//			LIQCHECKSTATUS(status, err);
// 		} 
// 		else if((arg == "-prf") || (arg == "-preFrameCommand")) 
//		{
// 			m_preFrameCommand = args.asString( ++i, &status );
//			LIQCHECKSTATUS(status, err);
// 		} 
// 		else if((arg == "-rec") || (arg == "-renderCommand")) 
//		{
// 			m_renderCommand = args.asString( ++i, &status );
//			LIQCHECKSTATUS(status, err);
// 		} 
// 		else if((arg == "-rgc") || (arg == "-ribgenCommand")) 
//		{
// 			m_ribgenCommand = args.asString( ++i, &status );
//			LIQCHECKSTATUS(status, err);
// 		} 
// 		else if((arg == "-blt") || (arg == "-blurTime")) 
//		{
// 			argValue = args.asString( ++i, &status );
// 			m_blurTime = argValue.asDouble();
//			LIQCHECKSTATUS(status, err);
// 		} 
// 		else if((arg == "-sr") || (arg == "-shadingRate")) 
//		{ 
// 			argValue = args.asString( ++i, &status );
// 			liqglo.shadingRate = argValue.asDouble();
//			LIQCHECKSTATUS(status, err);
// 		} 
// 		else if((arg == "-bs") || (arg == "-bucketSize")) 
//		{
// 			argValue = args.asString( ++i, &status );
//			liqglo.bucketSize[0] = argValue.asInt();
// 			LIQCHECKSTATUS(status, err);
// 			argValue = args.asString( ++i, &status );
// 			liqglo.bucketSize[1] = argValue.asInt();
//			LIQCHECKSTATUS(status, err);
// 		} 
// 		else if((arg == "-pf") || (arg == "-pixelFilter")) 
//		{
// 			argValue = args.asString( ++i, &status );
// 			liqglo.m_rFilter = argValue.asInt();
//			LIQCHECKSTATUS(status, err);
// 			argValue = args.asString( ++i, &status );
// 			liqglo.m_rFilterX = argValue.asInt();
//			LIQCHECKSTATUS(status, err);
// 			argValue = args.asString( ++i, &status );
// 			liqglo.m_rFilterY = argValue.asInt();
//			LIQCHECKSTATUS(status, err);
// 		} 
// 		else if((arg == "-gs") || (arg == "-gridSize")) 
//		{
// 			argValue = args.asString( ++i, &status );
// 			liqglo.gridSize = argValue.asInt();
// 			LIQCHECKSTATUS(status, err);
// 		} 
// 		else if((arg == "-txm") || (arg == "-texmem")) 
//		{
// 			argValue = args.asString( ++i, &status );
// 			liqglo.textureMemory = argValue.asInt();
// 			LIQCHECKSTATUS(status, err);
// 		} 
// 		else if ((arg == "-es") || (arg == "-eyeSplits")) 
//		{
// 			argValue = args.asString( ++i, &status );
// 			liqglo.eyeSplits = argValue.asInt();
// 			LIQCHECKSTATUS(status, err);
// 		} 
// 		else if ((arg == "-ar") || (arg == "-aspect")) 
//		{
// 			argValue = args.asString( ++i, &status );
// 			liqglo.aspectRatio = argValue.asDouble();
// 			LIQCHECKSTATUS(status, err);
// 		} 
// 		else if ((arg == "-x") || (arg == "-width")) 
//		{
// 			argValue = args.asString( ++i, &status );
// 			width = argValue.asInt();
// 			LIQCHECKSTATUS(status, err);
// 		} 
// 		else if ((arg == "-y") || (arg == "-height")) 
//		{
// 			argValue = args.asString( ++i, &status );
// 			height = argValue.asInt();
// 			LIQCHECKSTATUS(status, err);
// 		} 
// // 		else if((arg == "-def") || (arg == "-deferred")) {
// //       		LIQCHECKSTATUS(status, "error in -deferred parameter");
// //       		m_deferredGen = true;
// //     	} 
// // 		else if((arg == "-ndf") || (arg == "-noDef")) {
// //       		LIQCHECKSTATUS(status, "error in -noDef parameter");
// //       		m_deferredGen = false;
// //     	} 
// 		else if((arg == "-pad") || (arg == "-padding")) 
//		{
// 			argValue = args.asString( ++i, &status );
// 			liqglo.liqglo_outPadding = argValue.asInt();
// 			LIQCHECKSTATUS(status, err);
// 		} 
// 		else if((arg == "-rvp") || (arg == "-renderViewPort")) 
//		{
// 			argValue = args.asString( ++i, &status );
// 			liqglo.m_renderViewPort = argValue.asInt();
// 			LIQCHECKSTATUS(status, err);
// 		} 
// 		else if ((arg == "-cw") || (arg == "-cropWindow")) 
//		{
// 			argValue = args.asString( ++i, &status );
// 			liqglo.m_cropX1 = argValue.asDouble();
// 			LIQCHECKSTATUS(status, err);
// 			argValue = args.asString( ++i, &status );
// 			liqglo.m_cropX2 = argValue.asDouble();
// 			LIQCHECKSTATUS(status, err);
// 			argValue = args.asString( ++i, &status );
// 			liqglo.m_cropY1 = argValue.asDouble();
// 			LIQCHECKSTATUS(status, err);
// 			argValue = args.asString( ++i, &status );
// 			liqglo.m_cropY2 = argValue.asDouble();
// 			LIQCHECKSTATUS(status, err);
// 			if( liqglo.m_renderView ) 
// 				liqglo.m_renderViewCrop = true;
// 		}
// // 		else if ((arg == "-shn") || (arg == "-shotName")) 
// // 		{
// // 			liqglo.liqglo_shotName = args.asString( ++i, &status );
// // 			LIQCHECKSTATUS(status, err);
// // 		} 
// // 		else if ((arg == "-shv") || (arg == "-shotVersion")) 
// // 		{
// // 			liqglo.liqglo_shotVersion = args.asString( ++i, &status );
// // 			LIQCHECKSTATUS(status, err);
// // 		}
// 		else if((arg == "-obl") || (arg == "-objectList")) {
//       		LIQCHECKSTATUS(status, "error in -objectList parameter");
//       		i++;
//       		m_objectListToExport = args.asStringArray( i, &status );
//       		LIQCHECKSTATUS(status, "error in -objectList parameter 1");
//       		m_exportSpecificList = true;
//     	}
//     	else if((arg == "-oob") || (arg == "-onlyObjectBlock")) {
//       		m_exportOnlyObjectBlock = true;
//     	}
// 		else if((arg == "-igs") || (arg == "-ignoreSurfaces")) {
//       		m_ignoreSurfaces = true;
//     	}
// 		else if((arg == "-no_igs") || (arg == "-noIgnoreSurfaces")) {
// 		  m_ignoreSurfaces = false;
// 		}
// 		else if((arg == "-igd") || (arg == "-ignoreDisplacements")) {
// 		  m_ignoreDisplacements = true;
// 		}
// 		else if((arg == "-no_igd") || (arg == "-noIgnoreDisplacements")) {
// 		  m_ignoreDisplacements = false;
// 		}
// 		else if((arg == "-igv") || (arg == "-ignoreVolumes")) {
// 		  m_ignoreVolumes = true;
// 		}
// 		else if((arg == "-no_igv") || (arg == "-noIgnoreVolumes")) {
// 		  m_ignoreVolumes = false;
// 		}
// 		else if((arg == "-no_ufe") || (arg == "-noUseFrameExtension")) {
// 		  m_useFrameExt = false;
// 		}
// 		else if((arg == "-skv") || (arg == "-skipVisibilityAttributes")) {
// 		  m_skipVisibilityAttributes = true;
// 		}
// 		else if((arg == "-sks") || (arg == "-skipShadingAttributes")) {
// 		  m_skipShadingAttributes = true;
// 		}
// 		else if((arg == "-skr") || (arg == "-skipRayTraceAttributes")) {
// 		  m_skipRayTraceAttributes = true;
// 		}
// 		else if((arg == "-easp") || (arg == "-exportAllShadersParams")) {
// 		  i++;
// 		  argValue = args.asString( i, &status );
// 		  LIQCHECKSTATUS(status, "error in -exportAllShadersParams parameter : -exportAllShadersParams 1/0");
// 		  liqglo.liqglo_exportAllShadersParams = args.asInt( i, &status );
// 		  LIQCHECKSTATUS(status, "error in -exportAllShadersParams parameter (must be an integer) : -exportAllShadersParams 1/0");
// 		}
// 		else if((arg == "-rhcn") || (arg == "-ribHasCameraName")) {
// 		  i++;
// 		  argValue = args.asString( i, &status );
// 		  LIQCHECKSTATUS(status, "error in -ribHasCameraName parameter : -ribHasCameraName 1/0");
// 		  liqglo.liqglo_beautyRibHasCameraName = args.asInt( i, &status );
// 		  LIQCHECKSTATUS(status, "error in -ribHasCameraName parameter (must be an integer) :  1/0");
// 		}
// 		else if((arg == "-sdm") || (arg == "-skipDefaultMatte")) {
// 		  i++;
// 		  argValue = args.asString( i, &status );
// 		  LIQCHECKSTATUS(status, "error in -skipDefaultMatte parameter : -skipDefaultMatte 1/0");
// 		  liqglo.liqglo_skipDefaultMatte = args.asInt( i, &status );
// 		  LIQCHECKSTATUS(status, "error in -skipDefaultMatte parameter (must be an integer) :  1/0");
// 		}
// 		else {
// 			printf("[liqRibTranslator] undefined argument %d : '%s' \n", i, args.asString( i ).asChar());
// 		}
// 	}
// 	if( !m_useFrameExt )
// 	{
// 		if( m_animation )
// 		{
// 			liquidMessage( "[liqRibTranslator] useFrameExtension is false and animation was true, set animation=false", messageWarning );
// 		}
// 		m_animation = false;
// 	}
// 	liquidMessage( "Using project base path '" + string( liqglo.liqglo_projectDir.asChar() ) + "'", messageInfo );
// 	setSearchPaths();
// 	return MS::kSuccess;
// }

/**
* Read the values from the render globals and set internal values.
*/
// void liqRibTranslator::liquidReadGlobals()
// {
// 	//CM_TRACE_FUNC("liqRibTranslator::liquidReadGlobals()");
// 
// 	//trace log is not created, can't call CM_TRACE_FUNC("liqRibTranslator::liquidReadGlobals()");
// 	MStatus gStatus;
// 	MPlug gPlug;
// 	MFnDependencyNode rGlobalNode( liqglo.rGlobalObj );
// 	MString varVal;
// 	int var;
// 	// Display Channels - Read and store 'em !
// 	// philippe : channels are stored as structures in a vector
// 	{
// 		if( liqglo.liquidRenderer.supports_DISPLAY_CHANNELS ) 
// 		{
// 			liqglo.m_channels.clear();
// 			unsigned int nChannels = liquidGetPlugNumElements( rGlobalNode, "channelName", &gStatus );
// 
// 			for ( unsigned i( 0 ); i < nChannels; i++ ) 
// 			{
// 				StructChannel theChannel;
// 
// 				liquidGetPlugElementValue( rGlobalNode, i, "channelName", theChannel.name, gStatus );
// 				liquidGetPlugElementValue( rGlobalNode, i, "channelType", theChannel.type, gStatus );
// 				liquidGetPlugElementValue( rGlobalNode, i, "channelArraySize", theChannel.arraySize, gStatus );
// 				liquidGetPlugElementValue( rGlobalNode, i, "channelQuantize", theChannel.quantize, gStatus );
// 				liquidGetPlugElementValue( rGlobalNode, i, "channelBitDepth", theChannel.bitDepth, gStatus );
// 				liquidGetPlugElementValue( rGlobalNode, i, "channelDither", theChannel.dither, gStatus );
// 				liquidGetPlugElementValue( rGlobalNode, i, "channelFilter", theChannel.filter, gStatus );
// 				liquidGetPlugElementValue( rGlobalNode, i, "channelPixelFilter", theChannel.pixelFilter, gStatus );
// 				liquidGetPlugElementValue( rGlobalNode, i, "channelPixelFilterX", theChannel.pixelFilterX, gStatus );
// 				liquidGetPlugElementValue( rGlobalNode, i, "channelPixelFilterY", theChannel.pixelFilterY, gStatus );
// 
// 				liqglo.m_channels.push_back( theChannel );
// 			}
// 		}
// 	}
// 	// Display Driver Globals - Read 'em and store 'em !
// 	liquidGetPlugValue( rGlobalNode, "ignoreAOVDisplays", liqglo.m_ignoreAOVDisplays, gStatus );
// 	{
// 		liqglo.m_displays.clear();
// 		unsigned int nDisplays = liquidGetPlugNumElements( rGlobalNode, "ddImageName", &gStatus );
// 		//cout <<"  DD : we have "<<nDisplays<<" displays..."<<endl;
// 		for ( unsigned int i(0); i<nDisplays; i++ ) 
// 		{
// 			StructDisplay theDisplay;
// 			
// 			liquidGetPlugElementValue( rGlobalNode, i, "ddImageName", theDisplay.name, gStatus );
// 			liquidGetPlugElementValue( rGlobalNode, i, "ddImageType", theDisplay.type, gStatus );
// 			liquidGetPlugElementValue( rGlobalNode, i, "ddImageMode", theDisplay.mode, gStatus );
// 
// 			if( i==0 ) {
// 				theDisplay.enabled = true;
// 				theDisplay.doQuantize = true;
// 				theDisplay.doFilter = true;
// 			}
// 			else 
// 			{
// 				liquidGetPlugElementValue( rGlobalNode, i, "ddEnable", theDisplay.enabled, gStatus );
// 				liquidGetPlugElementValue( rGlobalNode, i, "ddQuantizeEnabled", theDisplay.doQuantize, gStatus );
// 				liquidGetPlugElementValue( rGlobalNode, i, "ddFilterEnabled", theDisplay.doFilter, gStatus );
// 			}
// 			
// 			if ( theDisplay.doQuantize )
// 			{
// 				liquidGetPlugElementValue( rGlobalNode, i, "ddBitDepth", theDisplay.bitDepth, gStatus );
// 				liquidGetPlugElementValue( rGlobalNode, i, "ddDither", theDisplay.dither, gStatus );
// 			}
// 			if ( theDisplay.doFilter )
// 			{
// 				liquidGetPlugElementValue( rGlobalNode, i, "ddPixelFilter", theDisplay.filter, gStatus );
// 				liquidGetPlugElementValue( rGlobalNode, i, "ddPixelFilterX", theDisplay.filterX, gStatus );
// 				liquidGetPlugElementValue( rGlobalNode, i, "ddPixelFilterY", theDisplay.filterY, gStatus );
// 			}
// 			
// 			// retrieve the extra parameters for this display
// 			MStringArray xtraParamsNames;
// 			MStringArray xtraParamsDatas;
// 			MIntArray    xtraParamsTypes;
// 
// 			liquidGetPlugElementValue( rGlobalNode, i, "ddXtraParamNames", xtraParamsNames, gStatus );
// 			liquidGetPlugElementValue( rGlobalNode, i, "ddXtraParamTypes", xtraParamsTypes, gStatus );
// 			liquidGetPlugElementValue( rGlobalNode, i, "ddXtraParamDatas", xtraParamsDatas, gStatus );
// 
// 			if(i==0) 
// 			{ // copy filter params from display 0
// 				liqglo.m_rFilter = theDisplay.filter;
// 				liqglo.m_rFilterX = theDisplay.filterX;
// 				liqglo.m_rFilterY = theDisplay.filterY;
// 				liqglo.quantValue = theDisplay.bitDepth;
// 				//liqglo_DDimageName = theDisplay.name; //we don't use liqglo_DDimageName, so we omit this line in r772
// 			}
// 			StructDDParam xtraDDParams;
// 			xtraDDParams.num   = xtraParamsNames.length();
// 			xtraDDParams.names = xtraParamsNames;
// 			xtraDDParams.data  = xtraParamsDatas;
// 			xtraDDParams.type  = xtraParamsTypes;
// 			theDisplay.xtraParams = xtraDDParams;
// 
// 			liqglo.m_displays.push_back( theDisplay );
// 		}
// 	}
// 	liquidGetPlugValue( rGlobalNode, "shotName", liqglo.liqglo_shotName, gStatus ); // no substitution here
// 	liquidGetPlugValue( rGlobalNode, "shotVersion", liqglo.liqglo_shotVersion, gStatus ); // no substitution here
// 	liquidGetPlugValue( rGlobalNode, "rotateCamera",  liqglo.liqglo_rotateCamera, gStatus );
// 	liquidGetPlugValue( rGlobalNode, "relativeFileNames", liqglo.liqglo_relativeFileNames, gStatus );
// 	
// 	liquidGetPlugValue( rGlobalNode, "renderScriptFileName", m_userRenderScriptFileName, gStatus );
// 	liquidGetPlugValue( rGlobalNode, "beautyRibHasCameraName", liqglo.liqglo_beautyRibHasCameraName, gStatus );
// 
// 	liquidGetPlugValue( rGlobalNode, "ribgenCommand", m_ribgenCommand, gStatus, true ); // get parsed result
// 	liquidGetPlugValue( rGlobalNode, "preJobCommand", varVal, gStatus, true );
// 	liquidGetPlugValue( rGlobalNode, "postJobCommand", varVal, gStatus, true );
// 	liquidGetPlugValue( rGlobalNode, "postFrameCommand", m_postFrameCommand, gStatus );
// 	liquidGetPlugValue( rGlobalNode, "preFrameCommand", m_preFrameCommand, gStatus );
// 	liquidGetPlugValue( rGlobalNode, "preCommand", liqglo.m_preCommand, gStatus );
// 	liquidGetPlugValue( rGlobalNode, "launchRender",  liqglo.launchRender, gStatus );
// 
// 
// 	liquidGetPlugValue( rGlobalNode, "renderCamera", liqglo.renderCamera, gStatus, true );
// 	liqglo.liqglo_renderCamera = liqglo.renderCamera;
// 	getCameraParameters(rGlobalNode);
// 
// 	liquidGetPlugValue( rGlobalNode, "ribName", liqglo.liqglo_ribName, gStatus, true );
// 	//Note:  -ribName is set to liqglo.liqglo_sceneName originally. 
// 	//       Because we replace liqglo.liqglo_sceneName with liquidTransGetSceneName(), and delete liqglo.liqglo_sceneName,
// 	//       we use liqglo.liqglo_ribName to store the ribName which passed by commandline parameter '-ribName'
// 	//  [2/17/2012 yaoyansi]
// 	//liquidMessage2(messageWarning,"warrning: ribName is store in liqglo.liqglo_ribName instead of liqglo.liqglo_sceneName.[2/17/2012 yaoyansi]");
// 
// 
// 
// 
// 	liquidGetPlugValue( rGlobalNode, "alfredTags", liqglo.m_alfredTags, gStatus, true );
// 	liquidGetPlugValue( rGlobalNode, "alfredServices", liqglo.m_alfredServices, gStatus, true );
// 	liquidGetPlugValue( rGlobalNode, "dirmaps", liqglo.m_dirmaps, gStatus, true );
// 	liquidGetPlugValue( rGlobalNode, "key", liqglo.m_defGenKey, gStatus, true );
// 	liquidGetPlugValue( rGlobalNode, "service", liqglo.m_defGenService, gStatus, true );
// 
// 	liquidGetPlugValue( rGlobalNode, "preframeMel", m_preFrameMel, gStatus );
// 	liquidGetPlugValue( rGlobalNode, "postframeMel", m_postFrameMel, gStatus );
// 
// 	getHinderParameters(rGlobalNode);
// 
// 	liquidGetPlugValue( rGlobalNode, "cropX1", liqglo.m_cropX1, gStatus );
// 	liquidGetPlugValue( rGlobalNode, "cropX2", liqglo.m_cropX2, gStatus );
// 	liquidGetPlugValue( rGlobalNode, "cropY1", liqglo.m_cropY1, gStatus );
// 	liquidGetPlugValue( rGlobalNode, "cropY2", liqglo.m_cropY2, gStatus );
// 
// 	// RAYTRACING OPTIONS:BEGIN
// 
// 	liquidGetPlugValue( rGlobalNode, "useRayTracing", liqglo.rt_useRayTracing, gStatus );
// 	liquidGetPlugValue( rGlobalNode, "traceMaxDepth", liqglo.rt_traceMaxDepth, gStatus );
// 	liquidGetPlugValue( rGlobalNode, "traceSpecularThreshold", liqglo.rt_traceSpecularThreshold, gStatus );
// 	liquidGetPlugValue( rGlobalNode, "traceBreadthFactor", liqglo.rt_traceBreadthFactor, gStatus ); 
// 	liquidGetPlugValue( rGlobalNode, "traceDepthFactor", liqglo.rt_traceDepthFactor, gStatus ); 
// 	liquidGetPlugValue( rGlobalNode, "traceRayContinuation", liqglo.rt_traceRayContinuation, gStatus ); 
// 	liquidGetPlugValue( rGlobalNode, "traceCacheMemory", liqglo.rt_traceCacheMemory, gStatus ); 
// 	liquidGetPlugValue( rGlobalNode, "traceDisplacements", liqglo.rt_traceDisplacements, gStatus ); 
// 	liquidGetPlugValue( rGlobalNode, "traceSampleMotion", liqglo.rt_traceSampleMotion, gStatus ); 
// 	liquidGetPlugValue( rGlobalNode, "traceBias", liqglo.rt_traceBias, gStatus ); 
// 	liquidGetPlugValue( rGlobalNode, "traceMaxDiffuseDepth", liqglo.rt_traceMaxDiffuseDepth, gStatus ); 
// 	liquidGetPlugValue( rGlobalNode, "traceMaxSpecularDepth", liqglo.rt_traceMaxSpecularDepth, gStatus ); 
// 	liquidGetPlugValue( rGlobalNode, "irradianceMaxError", liqglo.rt_irradianceMaxError, gStatus ); 
// 	liquidGetPlugValue( rGlobalNode, "irradianceMaxPixelDist", liqglo.rt_irradianceMaxPixelDist, gStatus ); 
// 	// RAYTRACING OPTIONS:END
// 	liquidGetPlugValue( rGlobalNode, "outputMayaPolyCreases", liqglo.liqglo_outputMayaPolyCreases, gStatus ); 
// 	liquidGetPlugValue( rGlobalNode, "useMtorSubdiv", liqglo.liqglo_useMtorSubdiv, gStatus ); 
// 	liquidGetPlugValue( rGlobalNode, "ribRelativeTransforms", liqglo.liqglo_relativeTransforms, gStatus ); 
// 	liquidGetPlugValue( rGlobalNode, "shortShaderNames", liqglo.liqglo_shortShaderNames, gStatus ); 
// 	liquidGetPlugValue( rGlobalNode, "expandAlfred", liqglo.m_alfredExpand, gStatus ); 
// 	liquidGetPlugValue( rGlobalNode, "createOutputDirectories", createOutputDirectories, gStatus ); 
// 	liquidGetPlugValue( rGlobalNode, "minCPU", m_minCPU, gStatus ); 
// 	liquidGetPlugValue( rGlobalNode, "maxCPU", m_maxCPU, gStatus );
// 	liquidGetPlugValue( rGlobalNode, "showProgress", m_showProgress, gStatus );
// 	liquidGetPlugValue( rGlobalNode, "expandShaderArrays", liqglo.liqglo_expandShaderArrays, gStatus );
// 	liquidGetPlugValue( rGlobalNode, "outputComments", liqglo.m_outputComments, gStatus );
// 	liquidGetPlugValue( rGlobalNode, "shaderDebug", m_shaderDebug, gStatus );
// 	liquidGetPlugValue( rGlobalNode, "deferredGen", liqglo.m_deferredGen, gStatus );
// 	liquidGetPlugValue( rGlobalNode, "deferredBlock", liqglo.m_deferredBlockSize, gStatus );
// 	liquidGetPlugValue( rGlobalNode, "useRenderScript", useRenderScript, gStatus );
// 	liquidGetPlugValue( rGlobalNode, "remoteRender", liqglo.remoteRender, gStatus );
// 	liquidGetPlugValue( rGlobalNode, "renderAllCurves", liqglo.liqglo_renderAllCurves, gStatus );
// 	liquidGetPlugValue( rGlobalNode, "illuminateByDefault", liqglo.m_illuminateByDefault, gStatus );
// 	liquidGetPlugValue( rGlobalNode, "liquidSetLightLinking", m_liquidSetLightLinking, gStatus );
// 	liquidGetPlugValue( rGlobalNode, "ignoreLights", m_ignoreLights, gStatus );
// 	liquidGetPlugValue( rGlobalNode, "ignoreSurfaces", m_ignoreSurfaces, gStatus );
// 	liquidGetPlugValue( rGlobalNode, "ignoreDisplacements", m_ignoreDisplacements, gStatus );
// 	liquidGetPlugValue( rGlobalNode, "ignoreVolumes", m_ignoreVolumes, gStatus );
// 
// 	liquidGetPlugValue( rGlobalNode, "useNewTranslator", m_useNewTranslator, gStatus );
// 	liquidGetPlugValue( rGlobalNode, "outputShaderGraph", m_OutputShaderGraph, gStatus );
// 	liquidGetPlugValue( rGlobalNode, "outputDebugMsgToRib", m_outputDebugMsgToRib, gStatus );
// 	liquidGetPlugValue( rGlobalNode, "outputShadowPass", m_outputShadowPass, gStatus );
// 	liquidGetPlugValue( rGlobalNode, "outputHeroPass", m_outputHeroPass, gStatus );
// 
// 	liquidGetPlugValue( rGlobalNode, "netRManRender", liqglo.useNetRman, gStatus );
// 	liquidGetPlugValue( rGlobalNode, "ignoreShadows", liqglo.liqglo_doShadows, gStatus );
// 	liqglo.liqglo_doShadows = !liqglo.liqglo_doShadows;
// 
// 	liquidGetPlugValue( rGlobalNode, "shapeOnlyInShadowNames", liqglo.liqglo_shapeOnlyInShadowNames, gStatus );
// 	liquidGetPlugValue( rGlobalNode, "fullShadowRibs", liqglo.fullShadowRib, gStatus );
// 	liquidGetPlugValue( rGlobalNode, "binaryOutput", liqglo.liqglo_doBinary, gStatus );
// 	liquidGetPlugValue( rGlobalNode, "lazyCompute", m_lazyCompute, gStatus );
// 	liquidGetPlugValue( rGlobalNode, "outputShadersInShadows", m_outputShadersInShadows, gStatus );
// 	// Moritz: added new options for light/shader output in deep shadows
// 	liquidGetPlugValue( rGlobalNode, "outputShadersInDeepShadows", m_outputShadersInDeepShadows, gStatus );
// 	liquidGetPlugValue( rGlobalNode, "outputLightsInDeepShadows", m_outputLightsInDeepShadows, gStatus );
// 
// 	liquidGetPlugValue( rGlobalNode, "outputMeshUVs", liqglo.liqglo_outputMeshUVs, gStatus );
// 	liquidGetPlugValue( rGlobalNode, "outputMeshAsRMSArrays", liqglo.liqglo_outputMeshAsRMSArrays, gStatus );
// 	liquidGetPlugValue( rGlobalNode, "compressedOutput", liqglo.liqglo_doCompression, gStatus );
// 	liquidGetPlugValue( rGlobalNode, "exportReadArchive", liqglo.m_exportReadArchive, gStatus );
// 	liquidGetPlugValue( rGlobalNode, "renderJobName", renderJobName, gStatus );
// 	liquidGetPlugValue( rGlobalNode, "doAnimation", m_animation, gStatus );
// 	if( m_animation ) 
// 	{
// 		MString frameSequence;
// 		liquidGetPlugValue( rGlobalNode, "frameSequence", frameSequence, gStatus );
// 		if( gStatus == MS::kSuccess ) 
// 			liqglo.frameNumbers = generateFrameNumbers( string( frameSequence.asChar() ) );
// 	}
// 	liquidGetPlugValue( rGlobalNode, "doPadding", liqglo.liqglo_doExtensionPadding, gStatus );
// 	if( liqglo.liqglo_doExtensionPadding ) 
// 		liquidGetPlugValue( rGlobalNode, "padding", liqglo.liqglo_outPadding, gStatus );
// 	{  
// 		int gWidth, gHeight;
// 		liquidGetPlugValue( rGlobalNode, "xResolution", gWidth, gStatus );
// 		liquidGetPlugValue( rGlobalNode, "yResolution", gHeight, gStatus );
// 		if( gWidth > 0 ) 
// 			width = gWidth;
// 		if( gHeight > 0 ) 
// 			height = gHeight;
// 	}
// 
// 
// 	liquidGetPlugValue( rGlobalNode, "transformationBlur", liqglo.liqglo_doMotion, gStatus );
// 
// 	liquidGetPlugValue( rGlobalNode, "deformationBlur", liqglo.liqglo_doDef, gStatus );
// 
// 	liquidGetPlugValue( rGlobalNode, "shutterEfficiency", liqglo.liqglo_shutterEfficiency, gStatus );
// 	liquidGetPlugValue( rGlobalNode, "motionBlurSamples", liqglo.liqglo_motionSamples, gStatus );
// 	if( liqglo.liqglo_motionSamples > LIQMAXMOTIONSAMPLES )
// 		liqglo.liqglo_motionSamples = LIQMAXMOTIONSAMPLES;
// 	liquidGetPlugValue( rGlobalNode, "relativeMotion", liqglo.liqglo_relativeMotion, gStatus );
// 
// 
// 	getOtherParameters(rGlobalNode);	
// 	getLimitsParameters(rGlobalNode);
// 
// 	liquidGetPlugValue( rGlobalNode, "cleanRib", liqglo.cleanRib, gStatus );
// 	liquidGetPlugValue( rGlobalNode, "cleanRenderScript", cleanRenderScript, gStatus );
// 	liquidGetPlugValue( rGlobalNode, "cleanTex", cleanTextures, gStatus );
// 	liquidGetPlugValue( rGlobalNode, "cleanShad", cleanShadows, gStatus );
// 	liquidGetPlugValue( rGlobalNode, "justRib", m_justRib, gStatus );
// 
// 
// 	getStatisticsParameters(rGlobalNode);
// 
// 	// Philippe : OBSOLETE ?
// 
// 
// 	// taking into account liquidRibRequest nodes and preposterous mel scripts - Alf
// 	{
// 		MStringArray requestArray;
// 		MString request;
// 
// 		liquidGetPlugValue( rGlobalNode, "preFrameBegin", request, gStatus );
// 		if( request != "" )
// 			requestArray.append( request );
// 		// add rib request node values
// 		request = parseLiquidRibRequest( requestArray, "preFrame" );
// 		if( request != "" )
// 			liqglo.m_preFrameRIB = parseString( request );
// 		requestArray.clear();
// 		request.clear();
// 
// 		liquidGetPlugValue( rGlobalNode, "preWorld", request, gStatus );
// 		if( request != "" )
// 			requestArray.append( request );
// 		request = parseLiquidRibRequest( requestArray, "preWorld" );
// 		if( request != "" )
// 			liqglo.m_preWorldRIB = parseString( request );
// 		requestArray.clear();
// 		request.clear();
// 
// 		liquidGetPlugValue( rGlobalNode, "postWorld", request, gStatus );
// 		if( request != "" )
// 			requestArray.append( request );
// 		request = parseLiquidRibRequest( requestArray, "postWorld" );
// 		if( request != "" )
// 			liqglo.m_postWorldRIB = parseString( request );
// 		requestArray.clear();
// 		request.clear();
// 
// 		liquidGetPlugValue( rGlobalNode, "preGeom", request, gStatus );
// 		if( request != "" )
// 			requestArray.append( request );
// 		request = parseLiquidRibRequest( requestArray, "preGeom" );
// 		if( request != "" )
// 			m_preGeomRIB = parseString( request );
// 	}
// 	liquidGetPlugValue( rGlobalNode, "renderScriptFormat", var, gStatus );
// 	if( gStatus == MS::kSuccess ) 
// 		m_renderScriptFormat = ( enum renderScriptFormat ) var;
// 
// 	liquidGetPlugValue( rGlobalNode, "renderScriptCommand", varVal, gStatus );
// 	m_renderScriptCommand = ( varVal != "" )? parseString( varVal, false ) : "alfred";
// 
// 	liquidGetPlugValue( rGlobalNode, "currentLayerRifParameters", liqglo.liqglo_rifParams, gStatus );
// 	
// 	setOutDirs();
// 	setSearchPaths();
// }

MString liqRibTranslator::verifyResourceDir( const char *resourceName, MString resourceDir, bool &problem )
{
	CM_TRACE_FUNC(boost::format("liqRibTranslator::verifyResourceDir(%s, %s, %d)")%resourceName %resourceDir.asChar() %problem);
#ifdef _WIN32
	int dirMode = 6; // dummy arg
	int mkdirMode = 0;
	_chdir( liqglo.liqglo_projectDir.asChar() );
#else
	mode_t dirMode,mkdirMode;
	dirMode = R_OK|W_OK|X_OK|F_OK;
	mkdirMode = S_IRWXU|S_IRWXG|S_IRWXO;
	chdir( liqglo_projectDir.asChar() );
#endif

#ifndef DIR_CREATION_WARNING
#define DIR_CREATION_WARNING(type, path) \
	liquidMessage( "Had trouble creating " + string( type ) + " directory, '" + path + "'. Defaulting to system temp directory!", messageWarning );
#endif
#ifndef DIR_MISSING_WARNING
#define DIR_MISSING_WARNING(type, path) \
	liquidMessage( string( type ) + " directory, '" + path + "', does not exist or is not accessible. Defaulting to system temp directory!", messageWarning );
#endif  

	LIQ_ADD_SLASH_IF_NEEDED( resourceDir );
	MString tmp_path( liquidSanitizePath( liquidGetRelativePath( liqglo.liqglo_relativeFileNames, resourceDir, liqglo.liqglo_projectDir ) ) );
	if ( !fileFullyAccessible( tmp_path ) ) 
	{
		if ( createOutputDirectories ) 
		{
			if ( !makeFullPath( tmp_path.asChar(), mkdirMode ) ) 
			{
				DIR_CREATION_WARNING( resourceName, tmp_path.asChar() );
				resourceDir = m_systemTempDirectory;
				problem = true;
			}
		} 
		else 
		{
			DIR_MISSING_WARNING( resourceName, tmp_path.asChar() );
			resourceDir = m_systemTempDirectory;
			problem = true;
		}
	} 
	else 
		resourceDir = tmp_path;  
	return resourceDir;
}
bool liqRibTranslator::verifyOutputDirectories()
{
	CM_TRACE_FUNC("liqRibTranslator::verifyOutputDirectories()");

	bool problem( false );

	liqglo.liqglo_ribDir = verifyResourceDir( "RIB", liqglo.liqglo_ribDir, problem );
	liqglo.liqglo_textureDir = verifyResourceDir( "Texture", liqglo.liqglo_textureDir, problem );
	liqglo.m_pixDir = verifyResourceDir( "Picture", liqglo.m_pixDir, problem );
	m_tmpDir = verifyResourceDir( "Temp Files", m_tmpDir, problem );

	return problem;
}

MString liqRibTranslator::generateRenderScriptName() const
{
	CM_TRACE_FUNC("liqRibTranslator::generateRenderScriptName()");
	MString renderScriptName;
	renderScriptName = m_tmpDir;
	if( m_userRenderScriptFileName != MString( "" ) )
		renderScriptName += m_userRenderScriptFileName;
	else 
	{
		renderScriptName += liquidTransGetSceneName();
		size_t tempSize = 0;
		char currentHostName[1024];
		short alfRand;
		gethostname( currentHostName, tempSize );
		liquidlong hashVal = liquidHash( currentHostName );

#ifndef _WIN32
		struct timeval  t_time;
		struct timezone t_zone;
		gettimeofday( &t_time, &t_zone );
		srandom( t_time.tv_usec + hashVal );
		alfRand = random();
#else
		struct tm *time;
		__time64_t long_time;
		_time64( &long_time );
		time = _localtime64( &long_time ); 
		srand( time->tm_sec + hashVal );
		alfRand = rand();
#endif
		renderScriptName += alfRand;
	}
	if(m_renderScriptFormat == ALFRED) 
		renderScriptName += ".alf";

	if(m_renderScriptFormat == XML) 
		renderScriptName += ".xml";

	return renderScriptName;
}

MString liqRibTranslator::generateTempMayaSceneName() const
{
	CM_TRACE_FUNC("liqRibTranslator::generateTempMayaSceneName()");
	MString tempDefname = m_tmpDir;
	tempDefname += liquidTransGetSceneName();
	
	size_t tempSize = 0;
	char currentHostName[1024];
	short defRand;

	gethostname( currentHostName, tempSize );
	liquidlong hashVal = liquidHash( currentHostName );

#ifndef _WIN32
	struct timeval  t_time;
	struct timezone t_zone;
	gettimeofday( &t_time, &t_zone );
	srandom( t_time.tv_usec + hashVal );
	defRand = random();
#else
	struct tm *time;
	__time64_t long_time;
	_time64( &long_time );
	time = _localtime64( &long_time ); 
	srand( time->tm_sec + hashVal );
	defRand = rand();
#endif

	tempDefname += defRand;

	MString currentFileType = MFileIO::fileType();
	if( MString( "mayaAscii" )  == currentFileType ) 
		tempDefname += ".ma";
	if( MString( "mayaBinary" ) == currentFileType ) 
		tempDefname += ".mb";
	return tempDefname;
}

 //MString liqRibTranslator::generateShadowArchiveName( bool renderAllFrames, long renderAtframe, MString geometrySet )
 //{
 //	MString baseShadowName;
 //	if( !liqglo.liqglo_shapeOnlyInShadowNames ) 
 //		baseShadowName += liquidTransGetSceneName() + "_";
 //
 //	baseShadowName += "SHADOWBODY";
 //	if( geometrySet != "" ) 
 //		baseShadowName += "." + sanitizeNodeName( geometrySet.substring(0, 99) );
 //	baseShadowName += LIQ_ANIM_EXT;
 //	baseShadowName += ".rib";
 //
 //	size_t shadowNameLength = baseShadowName.length() + 1;
 //	shadowNameLength += 10;
 //	scoped_ptr< char > baseShadowRibName( new char[ shadowNameLength ] );
 //	sprintf( baseShadowRibName.get(), baseShadowName.asChar(), liqglo.liqglo_doExtensionPadding ? liqglo.liqglo_outPadding : 0, renderAllFrames ? liqglo.liqglo_lframe : renderAtframe );
 //	baseShadowName = baseShadowRibName.get();
 //
 //	return liquidSanitizePath( baseShadowName );
 //}
string liqRibTranslator::generateImageName( MString aovName, const structJob& job, MString format )
{
	CM_TRACE_FUNC(boost::format("liqRibTranslator::generateImageName(%s, job(%s)), %s")%aovName.asChar() %job.name.asChar() %format.asChar());

	std::stringstream ss;
	ss << liqglo.m_pixDir.asChar(); 
	
	std::string ext( liqglo.outExt.asChar() );
	if ( format != "" )
	{
		if ( format == "openexr" || format == "exr") 
			ext = "exr"; 
		else if ( format == "jpeg" ) 
			ext = "jpg"; 
		else if ( format == "tga" ) 
			ext = "tga"; 
		else if ( format == "sgi" ) 
			ext = "sgi"; 
		else        
			ext = "tif"; 
	}
	if ( liqglo.m_displays[0].name == "" )//r772 uses liqglo_DDimageName
		ss << parseString( liquidTransGetSceneName(), false ).asChar();//r772 uses liqglo_sceneName
	else
		ss << parseString( liqglo.m_displays[0].name, false ).asChar();//r772 uses liqglo_DDimageName

	// TODO: add new global var liqglo_displayHasCameraName
	if ( liqglo.liqglo_beautyRibHasCameraName )
		ss << "." << sanitizeNodeName( job.name ).asChar();

	if ( aovName != "" )
		ss << "." << aovName.asChar();  

	ss << "." << setfill('0') << setw( (liqglo.liqglo_doExtensionPadding)? liqglo.liqglo_outPadding : 0 ) <<  job.renderFrame;

	ss << "." << liqglo.outExt.asChar() ;

	return liquidSanitizePath( ss.str() );  	
}
MString liqRibTranslator::generateFileName( fileGenMode mode, const structJob& job )
{
	CM_TRACE_FUNC(boost::format("liqRibTranslator::generateFileName(%d,%s)")%mode %job.name.asChar());
	MString filename;
	std::stringstream ss;

	MString debug, suffix, fileExt;
	MString geometrySet = sanitizeNodeName( job.shadowObjectSet.substring(0, 99));
	switch ( mode )
	{
	case fgm_shadow_tex:  
		ss << liqglo.liqglo_textureDir.asChar(); 
		break;

	case fgm_shadow_rib:
	case fgm_shadow_archive:
	case fgm_beauty_rib:
		ss << liqglo.liqglo_ribDir.asChar(); 
		break;

	case fgm_image:
		// ss << m_pixDir.asChar(); 
		filename = generateImageName( "", job, job.format ).c_str();
		return filename;
		break;
	}

	switch ( mode )
	{
	case fgm_shadow_tex:  
	case fgm_shadow_rib:
	case fgm_shadow_archive:
		if ( !liqglo.liqglo_shapeOnlyInShadowNames )
			ss << liquidTransGetSceneName().asChar() << "_"; //liqglo_sceneName
		break;

	case fgm_beauty_rib:
		ss << liquidTransGetSceneName().asChar(); //liqglo_sceneName
		if ( liqglo.liqglo_beautyRibHasCameraName )
			ss << "_" << sanitizeNodeName( job.name ).asChar();
		break;
	}

	switch ( mode )
	{
	case fgm_shadow_tex:  
	case fgm_shadow_rib:
		//[refactor][shadowname] begin 
		suffix = "_";
		suffix += ( job.deepShadows ? "DSH" : "SHD");
		if ( job.isPoint && ( job.deepShadows || !job.shadowAggregation ) )
		{
			switch ( job.pointDir )
			{
			case pPX: suffix += "_PX"; break;
			case pPY: suffix += "_PY"; break;
			case pPZ: suffix += "_PZ"; break;
			case pNX: suffix += "_NX"; break;
			case pNY: suffix += "_NY"; break;
			case pNZ: suffix += "_NZ"; break;
			}
		} 	
		//[refactor][shadowname] end 
		break;

	case fgm_shadow_archive:
		suffix = "SHADOWBODY";
		break;
	case fgm_scene_archive:
		suffix = "SCENE";
		break;
	}

	switch ( mode )
	{
	case fgm_shadow_tex:
		debug = "fgm_shadow_tex";
		// check if aggregate is on
		if ( job.texName != "" )
			ss << sanitizeNodeName( job.texName ).asChar();
		else
			ss << sanitizeNodeName( job.name ).asChar();
		ss << suffix.asChar();

		// only if aggregate is off
		if ( job.shadowObjectSet != "" && job.texName == "" )
			ss << "." << geometrySet.asChar();
		fileExt = liqglo.liquidRenderer.textureExtension; // ".tex";
		break;

	case fgm_shadow_rib:
		debug = "fgm_shadow_rib";
		ss << sanitizeNodeName( job.name ).asChar();
		ss << suffix.asChar();

		if ( job.shadowObjectSet != "" ) 
			ss << "." << geometrySet.asChar();
		fileExt = ".rib";//extension
		break;

	case fgm_shadow_archive:
		debug = "fgm_shadow_archive"; 
		ss << suffix.asChar();

		if ( job.shadowObjectSet != "" ) 
			ss << "." << geometrySet.asChar();
		fileExt = ".rib";//extension;
		break;

	case fgm_scene_archive:
		debug = "fgm_scene_archive";
		ss << suffix.asChar();
		fileExt = ".rib";//extension;
		break;

	case fgm_beauty_rib:
		debug = "fgm_beauty_rib";
		fileExt = ".rib";//extension;
		break;

	case fgm_image:
		debug = "fgm_image";
		if ( liqglo.m_displays[0].name == "" )//r772 use liqglo_DDimageName
			ss << parseString( liquidTransGetSceneName(), false ).asChar();//liqglo_sceneName
		else
			ss << parseString( liqglo.m_displays[0].name, false ).asChar();//r772 use liqglo_DDimageName
		fileExt = liqglo.outExt;
		break;

	default:
		liquidMessage( "liqRibTranslator::generateFileName: unknown case", messageError );
	}
	//  prepare format string "%0*d" ( boost format doesn't support * modificator )...
	if ( ( m_animation || m_useFrameExt )  ) // &&  mode != fgm_image
		ss << "." << setfill('0') << setw( (liqglo.liqglo_doExtensionPadding)? liqglo.liqglo_outPadding : 0 ) <<  job.renderFrame;

	ss << "." << fileExt.asChar() ;

	filename = liquidSanitizePath( ss.str() ).c_str();
	filename = liquidGetRelativePath( false, filename, liqglo.liqglo_projectDir );//r776

	//cerr << "liqRibTranslator::generateFileName( " << debug.asChar() << " ) -> " << filename.asChar() << endl;
	LIQDEBUGPRINTF( "liqRibTranslator::generateFileName(%s) -> %s\n", debug.asChar(), filename.asChar() );
	return filename;
}
/**
 * This method setups render layer and save originalLayer
 */
MStatus liqRibTranslator::setRenderLayer( const MArgList& args )
{
//	CM_TRACE_FUNC(boost::format("liqRibTranslator::setRenderLayer(arg)") );

  MStatus status = MS::kSuccess;
  // check if we need to switch to a specific render layer
  // we do that here because we need to switch to the chosen layer first
  // to be able to read overriden gloabsl and stuff...
  unsigned int argIndex = args.flagIndex( "lyr", "layer" );
  
  if ( argIndex != MArgList::kInvalidArgIndex ) 
    liqglo.liqglo_layer = args.asString( argIndex + 1, &status );

  // get the name of the current render layer
  
  if ( MGlobal::executeCommand( "editRenderLayerGlobals -q -currentRenderLayer;", originalLayer, false, false ) == MS::kFailure ) 
  {
    liquidMessage( "Could not get the current render layer name! ABORTING.", messageError );
    return MS::kFailure;
  }

  // switch to the specified render layer
  if ( liqglo.liqglo_layer != "" ) 
  {
    MString cmd = "if( `editRenderLayerGlobals -q -currentRenderLayer` != \"" + liqglo.liqglo_layer + "\" ) ";
    cmd += "editRenderLayerGlobals( \"-currentRenderLayer\", \"" + liqglo.liqglo_layer + "\");";
    if (  MGlobal::executeCommand( cmd, false, false ) == MS::kFailure ) 
    {
      liquidMessage( "Could not switch to render layer '" + std::string( liqglo.liqglo_layer.asChar() ) + "'! ABORTING.", messageError );
      return MS::kFailure;
    }
  } 
  else 
  {
    // we fill liqglo_layer with current layer name
    // to be able to substitute $LYR in strings.
    liqglo.liqglo_layer = originalLayer;
  }
  return status;
}
//
// Rib output.
//
MStatus liqRibTranslator::ribOutput( long scanTime, MString ribName, bool world_only, bool out_lightBlock, MString archiveName )
{
	CM_TRACE_FUNC(boost::format("liqRibTranslator::ribOutput(%d, %s, %d, %d, %s)")%scanTime %ribName.asChar() %world_only %out_lightBlock %archiveName.asChar());

	MStatus status = MS::kSuccess;

	// Rib client file creation options MUST be set before RiBegin
	LIQDEBUGPRINTF( "-> setting RiOptions\n" );
	//[refactor][1.9.2.3 begin] to setRenderScriptFormatAndCompress() --> rm::_RiOption_format_compress()
	//NOTE: this has already been set before, see:
	//liqRibTranslator::_doIt()    --> getRenderer()->setRenderScriptFormatAndCompress(liqglo.liqglo_doBinary, liqglo.liqglo_doCompression)
	//liqRibTranslator::_doItNew() --> getRenderer()->setRenderScriptFormatAndCompress(liqglo.liqglo_doBinary, liqglo.liqglo_doCompression)
#if defined(PRMAN) || defined(DELIGHT)
	/* THERE IS A RIBLIB BUG WHICH PREVENTS THIS WORKING */

	RtString format[ 2 ] = { "ascii", "binary" };
	if ( liqglo.liqglo_doBinary )
	{
		LIQDEBUGPRINTF( "-> setting binary option\n" );
		RiOption( "rib", "format", ( RtPointer )&format[1], RI_NULL );
	}
	else
	{
		LIQDEBUGPRINTF( "-> setting ascii option\n" );
		RiOption( "rib", "format", ( RtPointer )&format[0], RI_NULL );
		RtString style = "indented";
		RiOption( "rib", "string asciistyle", &style, RI_NULL );
	}
#endif // PRMAN || DELIGHT
#if defined(PRMAN) || defined(DELIGHT) || defined(GENERIC_RIBLIB)
	LIQDEBUGPRINTF( "-> setting compression option\n" );
	if ( liqglo.liqglo_doCompression ) 
	{
		RtString comp[ 1 ] = { "gzip" };
		RiOption( "rib", "compression", ( RtPointer )comp, RI_NULL );
	}
#endif // PRMAN || DELIGHT || GENERIC_RIBLIB
	//[refactor][1.9.2.3 end] to getRenderer()::setRenderScriptFormatAndCompress() --> rm::_RiOption_format_compress()




	liquidMessage( "Beginning RIB output to " + string( ribName.asChar() ), messageInfo );
	//[refactor][1.9.2.4 begin] to rm::renderer::BaseShadowBegin(ribName)
#ifndef RENDER_PIPE
	RiBegin_liq( const_cast< RtToken >( ribName.asChar() ) );
#else
	liqglo_ribFP = fopen( ribName.asChar(), "w" );
	if ( liqglo_ribFP ) 
	{
		LIQDEBUGPRINTF( "-> setting pipe option\n" );
		RtInt ribFD( fileno( liqglo_ribFP ) );
		RiOption( "rib", "pipe", &ribFD, RI_NULL );
	}
	else
		liquidMessage( "Error opening RIB -- writing to stdout.", messageError );

	liquidMessage( "Beginning RI output directly to renderer", messageInfo );

	RiBegin_liq( RI_NULL );
#endif
	//[refactor][1.9.2.4 end] to rm::renderer::BaseShadowBegin()

	//cerr << ">> writng RIB " << ribName.asChar() << endl;
	if ( !world_only )
	{
		//[refactor][1.9.2.5 begin] to tHeroRibWriterMgr::write()/tShadowRibWriterMgr.write()
		//cerr << "writng ribPrologue()" << endl;
		if ( ribPrologue() != MS::kSuccess ) 
			return MS::kFailure;
		//cerr << "writng framePrologue()" << endl;
		if ( framePrologue( scanTime ) != MS::kSuccess ) 
			return MS::kFailure;
		//[refactor][1.9.2.5 end] to tHeroRibWriterMgr::write()/tShadowRibWriterMgr.write()
	}    
	if ( archiveName != "" )
	{
		// reference the correct shadow/scene archive
		//[refactor][1.9.2.8 begin] to rm::Renderer::readBaseShadow()
		liquidMessage( "Writng archiveName " + string( archiveName.asChar() ), messageInfo ); 
		RiArchiveRecord( RI_COMMENT, "Read Archive Data:\n" );
		RiReadArchive( const_cast< RtToken >( archiveName.asChar() ), NULL, RI_NULL );
		//[refactor][1.9.2.8 end] to rm::Renderer::readBaseShadow()
	}
	else
	{
		// full beauty/shadow rib generation
		//[refactor][1.9.2.6 begin] to tHeroRibWriterMgr::write()/tShadowRibWriterMgr.write()
		//cerr << "writng worldPrologue()" << endl;
		if ( worldPrologue() != MS::kSuccess ) 
			return MS::kFailure;
		//cerr << "writng lightBlock()" << endl;
		if ( out_lightBlock && lightBlock() != MS::kSuccess ) 
			return MS::kFailure;
		//cerr << "writng coordSysBlock()" << endl;    
		if ( coordSysBlock() != MS::kSuccess ) 
			return MS::kFailure;
		//cerr << "writng objectBlock()" << endl;     
		if ( objectBlock() != MS::kSuccess ) 
			return MS::kFailure;
		//cerr << "writng worldEpilogue()" << endl;      
		if ( worldEpilogue() != MS::kSuccess ) 
			return MS::kFailure;
		//[refactor][1.9.2.6 end] to tHeroRibWriterMgr::write()/tShadowRibWriterMgr.write()
	}

	if ( !world_only )
	{
		//[refactor][1.9.2.7 begin] to tHeroRibWriterMgr::write()/tShadowRibWriterMgr.write()
		//cerr << "writng frameEpilogue()" << endl;
		if ( frameEpilogue( scanTime ) != MS::kSuccess ) 
			return MS::kFailure;  
		//cerr << "writng ribEpilogue()" << endl;
		if ( ribEpilogue() != MS::kSuccess )
			return MS::kFailure;
		//[refactor][1.9.2.7 end] to tHeroRibWriterMgr::write()/tShadowRibWriterMgr.write()
	}
	RiEnd();
	// output info when done with the rib - Alf
	liquidMessage( "Finished RIB generation " + string( ribName.asChar()), messageInfo ); 

#ifdef RENDER_PIPE  
	fclose( liqglo.liqglo_ribFP );
#endif
	liqglo.liqglo_ribFP = NULL;
	return status;
}
/**
* This method actually does the renderman output.
*/
MStatus liqRibTranslator::doIt( const MArgList& args )
{
	MStatus status;

	status = setRenderLayer( args );
	if( status != MS::kSuccess ) 
		return MS::kFailure;

	if( MS::kSuccess!=liqglo.liquidRenderer.setRenderer() ){
		return MS::kFailure;
	}
	m_renderCommand = liqglo.liquidRenderer.renderCommand;

	status = liquidDoArgs( args );
	if( !status ) 
		return MS::kFailure;

	{
		CM_TRACE_OPEN(getFunctionTraceLogFileName().c_str());
		CM_TRACE_FUNC("liqRibTranslator::doIt()-->if(checkSettings()==true)");
		{//set renderer
			MFnDependencyNode rGlobalNode( liqglo.rGlobalObj );
			MString renderer;
			liquidGetPlugValue( rGlobalNode, "renderer", renderer, status );
			liquid::RendererMgr::getInstancePtr()->createFactory(renderer.asChar());
			liquid::RendererMgr::getInstancePtr()->install();
			liquid::RendererMgr::getInstancePtr()->prologue();
		}

		if( canExport() )
		{
			if(m_useNewTranslator){
				liquidMessage("_doItNew()", messageInfo);
				status = _doItNew(args, originalLayer);// new doIt() process
			}else{
				liquidMessage("_doIt()", messageInfo);
				status = _doIt(args, originalLayer);//original doIt() process
			}
		}

		{//
			liquid::RendererMgr::getInstancePtr()->test();
			liquid::RendererMgr::getInstancePtr()->epilogue();
			liquid::RendererMgr::getInstancePtr()->uninstall();
			liquid::RendererMgr::getInstancePtr()->deleteFactory();
		}
		CM_TRACE_CLOSE();
	}


	return status;
}
//
MStatus liqRibTranslator::_doIt( const MArgList& args , const MString& originalLayer )
{
	//liqAssert("test liqAssert");
	//liqAssert("tile", "msg", "yes");
	//liqAssert("tile", "msg", "yes","no");
	CM_TRACE_FUNC(boost::format("liqRibTranslator::_doIt(args,%s)")%originalLayer.asChar());
	//[refactor][1 begin] to _doItNewWithoutRenderScript()/_doItNewWithRenderScript()
	MStatus status;
	MString lastRibName;
	bool hashTableInited = false;

	if( !liqglo.liquidBin && !liqglo.m_deferredGen ) 
		liquidMessage( "Creating RIB <Press ESC To Cancel> ...", messageInfo );

	// Remember the frame the scene was at so we can restore it later.
	MTime originalTime = MAnimControl::currentTime();

	// Set the frames-per-second global (we'll need this for
	// streak particles)
	//
	MTime oneSecond( 1, MTime::kSeconds );
	liqglo.liqglo_FPS = oneSecond.as( MTime::uiUnit() );

	// append the progress flag for render job feedback
	//[refactor][1.1 begin] to _doItNew()
// 	if( useRenderScript ) // ommited in liquidMaya r772
// 		if( ( m_renderCommand == MString( "render" ) ) || ( m_renderCommand == MString( "prman" ) ) || ( m_renderCommand == MString( "renderdl" ) ) ) 
// 			m_renderCommand = m_renderCommand + " -Progress";
	//[refactor][1.1 end] to _doItNew()

	// check to see if the output camera, if specified, is available. If exporting only objects, don't care about camera
	//[refactor][1.1.1 begin] to _doItNewWithRenderScript()/_doItNewWithoutRenderScript()
	if ( !m_exportOnlyObjectBlock )
	{
		MStatus camStatus;
		// check to see if the output camera, if specified, is available
		if( liqglo.liquidBin && ( liqglo.renderCamera == MString("") ) ) 
		{
			liquidMessage( "No render camera specified!", messageError );
			return MS::kFailure;
		}
		if( liqglo.renderCamera != MString("") ) 
		{
			MStatus selectionStatus;
			MSelectionList camList;
			selectionStatus = camList.add( liqglo.renderCamera );
			if( selectionStatus != MS::kSuccess ) 
			{
				liquidMessage( "Invalid render camera!", messageError );
				return MS::kFailure;
			}
			camList.getDagPath(0, m_camDagPath);
		}
		else{
			m_activeView.getCamera( m_camDagPath );
		}
		// check stereo camera
		MFnCamera fnCamera( m_camDagPath, &camStatus );
		if ( camStatus != MS::kSuccess )
		{
			liquidMessage( "Cannot create FN for render camera!", messageError );
			return MS::kFailure;
		}
		MString camType = fnCamera.typeName();
		if ( camType == "stereoRigCamera" )
			m_isStereoCamera = true;
	}
	else
	{
		liqglo.liqglo_renderCamera = "";
		liqglo.liqglo_beautyRibHasCameraName = 0;
	}
	//[refactor][1.1.1 end] to _doItNewWithRenderScript()/_doItNewWithoutRenderScript()

	// check to see if all the directories we are working with actually exist.
	/*if( verifyOutputDirectories() ) {
	MString err( "The output directories are not properly setup in the globals" );
	throw err;
	}*/
	// This is bollocks! Liquid defaults to system temp folders if it can't setup shit. It should always work, not breaks
	verifyOutputDirectories();

	// setup the error handler
#if( !defined (GENERIC_RIBLIB) ) && ( defined ( AQSIS ) || ( _WIN32 && DELIGHT ) )
#  ifdef _WIN32
	if( m_errorMode ) RiErrorHandler( (void(__cdecl*)(int,int,char*))liqRibTranslatorErrorHandler );
#  else
	if( m_errorMode ) RiErrorHandler( (void(*)(int,int,char*))liqRibTranslatorErrorHandler );
#  endif
#else
	if( m_errorMode ) RiErrorHandler( liqRibTranslatorErrorHandler );
#endif
	//[refactor][1.2 begin] to _doItNewWithRenderScript()
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
	//[refactor][1.2 end] to _doItNewWithRenderScript()

	// exception handling block, this tracks liquid for any possible errors and tries to catch them
	// to avoid crashing
	try 
	{
		m_escHandler.beginComputation();

		MString preFrameMel = parseString( m_preFrameMel );
		MString postFrameMel = parseString( m_postFrameMel );

		if( ( preFrameMel  != "" ) && !fileExists( preFrameMel ) ) 
			liquidMessage( "Cannot find pre frame MEL script file! Assuming local.", messageWarning );

		if( ( m_postFrameMel != "" ) && !fileExists( postFrameMel ) ) 
			liquidMessage( "Cannot find post frame MEL script file! Assuming local.", messageWarning );

		// build temp file names
		MString renderScriptName = generateRenderScriptName();
		MString tempDefname    = generateTempMayaSceneName();

		if( liqglo.m_deferredGen ) 
		{
			MString currentFileType = MFileIO::fileType();
			MFileIO::exportAll( tempDefname, currentFileType.asChar() );
		}

		if( !liqglo.m_deferredGen && m_justRib ) 
			useRenderScript = false;

		liqRenderScript jobScript;
		//[refactor][1.3 begin] to _doItNewWithRenderScript(), before "jobScript.addLeafDependency( preJobInstance );"
 		liqRenderScript::Job preJobInstance;
 		preJobInstance.title = "liquid pre-job";
 		preJobInstance.isInstance = true;
		//[refactor][1.3 end] to _doItNewWithRenderScript(),

		if( useRenderScript ) 
		{
			if( renderJobName == "" ) 
				renderJobName = liquidTransGetSceneName();

			//[refactor][1.4 begin] to tJobScriptMgr::setCommonParameters()
			jobScript.title = renderJobName.asChar();

			if( liqglo.useNetRman ) 
			{
				jobScript.minServers = m_minCPU;
				jobScript.maxServers = m_maxCPU;
			} 
			else 
			{
				jobScript.minServers = 1;
				jobScript.maxServers = 1;
			}

			if ( liqglo.m_dirmaps.length() )//added in ymesh
				jobScript.dirmaps = liqglo.m_dirmaps.asChar();
			//[refactor][1.4 end] to tJobScriptMgr::setCommonParameters()

			if( m_preJobCommand != MString( "" ) ) 
			{
				//[refactor][1.5 begin] to tJobScriptMgr::addJob()
				liqRenderScript::Job preJob;
				preJob.title = "liquid pre-job";
				liqRenderScript::Cmd jobCommand( m_preJobCommand.asChar(), ( liqglo.remoteRender && !liqglo.useNetRman ) );
				jobCommand.alfredServices = liqglo.m_alfredServices.asChar();
				jobCommand.alfredTags = liqglo.m_alfredTags.asChar();  
				preJob.commands.push_back( jobCommand );
				jobScript.addJob( preJob );
				//[refactor][1.5 end] to tJobScriptMgr::addJob()
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

		int currentBlock( 0 );
		unsigned frameIndex( 0 );
		for ( ; frameIndex < liqglo.frameNumbers.size(); frameIndex++ ) 
		{
			//[refactor][1.6 begin] to processOneFrame()
			liqShaderFactory::instance().clearShaders();

			liqglo.liqglo_lframe = liqglo.frameNumbers[ frameIndex ];

			if( m_showProgress ) 
				printProgress( 1, liqglo.frameNumbers.size(), frameIndex );

			liqRenderScript::Job frameScriptJob;

			liqglo.m_alfShadowRibGen = false;
			liqglo.liqglo_preReadArchive.clear();
			liqglo.liqglo_preRibBox.clear();
			liqglo.liqglo_preReadArchiveShadow.clear();
			liqglo.liqglo_preRibBoxShadow.clear();
			//[refactor][1.6 end] to processOneFrame()

			//[refactor][1.7 begin] to _doItNewWithRenderScript()/_doItNewWithoutRenderScript()/processOneFrame()
			// make sure all the global strings are parsed for this frame
			
			//[refactor][1.7.1 begin] +to tFrameScriptJobMgr::makeShadow
			MString frameRenderCommand    = parseString( m_renderCommand + " " + liqglo.liquidRenderer.renderCmdFlags, false );
			//[refactor][1.7.1 end]  +to tFrameScriptJobMgr::makeShadow

			MString frameRibgenCommand    = parseString( m_ribgenCommand, false );

			//[refactor][1.7.2 begin] +to tFrameScriptJobMgr::makeShadow
			MString framePreCommand       = parseString( liqglo.m_preCommand, false );
			//[refactor][1.7.2 end] +to tFrameScriptJobMgr::makeShadow

			MString framePreFrameCommand  = parseString( m_preFrameCommand, false );
			MString framePostFrameCommand = parseString( m_postFrameCommand, false );
			//[refactor][1.7 end] to _doItNewWithRenderScript()/_doItNewWithoutRenderScript()/processOneFrame()	
			
			//[refactor][1.8 begin] to _doItNewWithRenderScript()
			if( useRenderScript ) 
			{
				if( liqglo.m_deferredGen ) 
				{
					//[refactor] [1.8.1 begin] to _doItNewWithRenderScript()
					liqRenderScript::Job deferredJob;
					//[refactor] [1.8.1 end] to _doItNewWithRenderScript()

					if( ( frameIndex % liqglo.m_deferredBlockSize ) == 0 ) 
					{
						//[refactor] [1.8.1.1 begin] to _doItNewWithRenderScript()
						MString frameRangePart;
						if( liqglo.m_deferredBlockSize == 1 ) 
						{
							currentBlock = liqglo.liqglo_lframe;
							frameRangePart = MString( "-t " ) + liqglo.liqglo_lframe;
						}
						else 
						{
							currentBlock++;
							// Add list of frames to process for this block
							unsigned lastGenFrame( ( frameIndex + liqglo.m_deferredBlockSize ) < liqglo.frameNumbers.size() ? frameIndex + liqglo.m_deferredBlockSize : liqglo.frameNumbers.size() );
							//frameRangePart = MString( "-sequence " ) + frameIndex + " " + lastGenFrame  + " " + "1";
							frameRangePart = MString( "-t " ) + liqglo.liqglo_lframe;
							for( unsigned outputFrame( frameIndex + 1 ); outputFrame < lastGenFrame; outputFrame++ )
							{
									frameRangePart += "," + liqglo.frameNumbers[ outputFrame ];
							}
						}//[refactor] [1.8.1.1 end]

						//[refactor] [1.8.2 begin] to tJobScriptMgr::addDefferedJob()
						stringstream ribGenExtras;
						//Note:  -ribName is set to liqglo.liqglo_sceneName originally. 
						//       Because we replace liqglo.liqglo_sceneName with liquidTransGetSceneName(), and delete liqglo.liqglo_sceneName,
						//       we use liqglo.liqglo_ribName to store the ribName which passed by commandline parameter '-ribName'
						//  [2/17/2012 yaoyansi]
						//---------------------------------------------------
#if 1					//---ymesh r775
						assert(0&&"warrning: ribName is store in liqglo.liqglo_ribName instead of liqglo.liqglo_sceneName.[2/17/2012 yaoyansi]");
						// ribGenExtras << " -progress -noDef -nop -noalfred -projectDir " << liqglo_projectDir.asChar() << " -ribName " << liqglo.liqglo_ribName.asChar() << " -mf " << tempDefname.asChar() << " -t ";
            			if ( debugMode ) ribGenExtras << " -debug";
            			ribGenExtras << " -progress -noDef -ribGenOnly -noLaunchRender";
						ribGenExtras << " -projectDir \"" << liqglo.liqglo_projectDir.asChar() << "\" -ribName \"" << liqglo.liqglo_ribName.asChar() << "\" -fl ";

						unsigned lastGenFrame( ( frameIndex + liqglo.m_deferredBlockSize ) < liqglo.frameNumbers.size() ? frameIndex + liqglo.m_deferredBlockSize : liqglo.frameNumbers.size() );

						//liquidMessage2(messageInfo, ">>> frameIndex = %d m_deferredBlockSize = %d frameNumbers.size = %d lastGenFrame = %d\n",frameIndex,m_deferredBlockSize,frameNumbers.size(),lastGenFrame );

						for( unsigned outputFrame( frameIndex ); outputFrame < lastGenFrame; outputFrame++ )
						{  
							ribGenExtras << liqglo.frameNumbers[ outputFrame ];
							ribGenExtras << (( outputFrame != ( lastGenFrame - 1 ) )? ", " : " ");
							// liquidMessage2(messageInfo, "\t outputFrame = %d\n", outputFrame );
						}
#else					// r773
						//ribGenExtras << " -progress -noDef -nop -noalfred -projectDir " << liqglo_projectDir.asChar() << " -ribName " << liqglo_sceneName.asChar() << " -mf " << tempDefname.asChar() << " -t ";
						// Ensure deferred rib gen and use render script are off
						// Project dir argument must be first, in case -GL uses paths relative to the current project

						//string preFrameCmd = "if(exists(\"userSetup\")){source \"userSetup\";}";
						ribGenExtras << "-projectDir " << liqglo_projectDir.asChar() << " -GL -noDef -noLaunchRender -noRenderScript -ribGenOnly -ribdir "<< liqglo_ribDir.asChar() <<" -ribName " << liqglo_sceneName.asChar() << " " << frameRangePart.asChar() << " " << tempDefname.asChar();
#endif					//-----------------------------------------------------
						stringstream titleStream;
						titleStream << liquidTransGetSceneName().asChar() << "FrameRIBGEN" << currentBlock;
						deferredJob.title = titleStream.str();

						stringstream ss;
						ss << framePreCommand.asChar() << " " << frameRibgenCommand.asChar() << ribGenExtras.str() << " " << tempDefname.asChar();
						liqRenderScript::Cmd cmd( ss.str(), liqglo.remoteRender );
						cmd.alfredServices = liqglo.m_defGenService.asChar();
						cmd.alfredTags     = liqglo.m_defGenKey.asChar();
						if( liqglo.m_alfredExpand ) 
							cmd.alfredExpand = true;

						deferredJob.commands.push_back(cmd);
						//[refactor] [1.8.2 end] to tJobScriptMgr::addDefferedJob()

						jobScript.addJob(deferredJob);
					}
				}//if( m_deferredGen )
				
				//[refactor] [1.8.3 begin] to _doItNewWithRenderScript()
				if( !m_justRib ) 
				{
					//[refactor] [1.8.3.1 begin] to _doItNewWithRenderScript()/_doItNewWithoutRenderScript()
					stringstream titleStream;
					titleStream << liquidTransGetSceneName().asChar() << "Frame" << liqglo.liqglo_lframe;
					frameScriptJob.title = titleStream.str();
					//[refactor] [1.8.3.1 end] to _doItNewWithRenderScript()/_doItNewWithoutRenderScript()

					//[refactor] [1.8.3.2 begin] to _doItNewWithRenderScript()
					if( liqglo.m_deferredGen ) 
					{
						stringstream ss;
						ss << liquidTransGetSceneName().asChar() << "FrameRIBGEN" << currentBlock;
						liqRenderScript::Job instanceJob;
						instanceJob.isInstance = true;
						instanceJob.title = ss.str();
						frameScriptJob.childJobs.push_back(instanceJob);
					}
					//[refactor] [1.8.3.2 end] to _doItNewWithRenderScript()
				}
				//[refactor] [1.8.3 end] to _doItNewWithRenderScript()

			}//if( useRenderScript ) 
			//[refactor][1.8 begin] to _doItNewWithRenderScript()

			//[refactor][1.9 begin] to processOneFrame()
			LIQDEBUGPRINTF( "-> building jobs\n" );
			if( buildJobs() != MS::kSuccess ) // Hmmmmmm not really clean ....
				break;

			if( !liqglo.m_deferredGen ) 
			{
				if( m_showProgress ) 
					printProgress( 2, liqglo.frameNumbers.size(), frameIndex );

				long lastScannedFrame = -100000;
				long scanTime = liqglo.liqglo_lframe;
				hashTableInited = false;
				//
				// start iterating through the job list   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-
				//
				if( jobList.size() == 0 ) 
				{
					liquidMessage( "Nothing to render!", messageWarning );
					return MS::kSuccess;
				}
				//cout <<"Job iteration start -------------------------------------"<<endl;
				//cout <<"    nsfs:"<<liqglo_noSingleFrameShadows<<"  sfso:"<<liqglo_singleFrameShadowsOnly<<endl;
				vector< structJob >::iterator iter( jobList.begin() );
				for (; iter != jobList.end(); ++iter ) 
				{
					m_currentMatteMode = false;
					//[refactor] [1.9.1 begin] to processOneFrame()
					liqglo_currentJob = *iter;

					if( liqglo_currentJob.skip ) 
						continue;

					//cout <<">> outputing job ["<<liqglo_lframe<<"] ->"<<liqglo_currentJob.name.asChar()<<" -> "<<liqglo_currentJob.imageName.asChar()<<endl;
					// set the scan time based on the job's render frame
					//
					scanTime = liqglo_currentJob.renderFrame;
					// if we changed the frame to calculate a shadow at a different time,
					// we need to rescan the scene, otherwise not.
					//
					if( lastScannedFrame != scanTime ) 
					{
						LIQDEBUGPRINTF( "Scanning at time: %u \n", scanTime );
						// hash table handling
						//
						/*if( hashTableInited && htable ) {
						//cout <<"delete old table... "<<flush;
						//delete htable;
						htable.reset();
						//freeShaders();
						}*/

						htable = boost::shared_ptr< liqRibHT >( new liqRibHT() );
						hashTableInited = true;
						LIQDEBUGPRINTF( "Created hash table...\n" );

						//  calculate sampling time
						//[refactor][1.9.1.1 begin] to calaculateSamplingTime()
						float sampleinc( ( liqglo.liqglo_shutterTime * m_blurTime ) / ( liqglo.liqglo_motionSamples - 1 ) );
						for ( unsigned msampleOn( 0 ); msampleOn < liqglo.liqglo_motionSamples; msampleOn++ ) 
						{
							float subframe;
							switch( liqglo.shutterConfig ) 
							{
							case OPEN_ON_FRAME:
							default:
								subframe = scanTime + ( msampleOn * sampleinc );
								break;
							case CENTER_ON_FRAME:
								subframe = ( scanTime - ( liqglo.liqglo_shutterTime * m_blurTime * 0.5 ) ) + msampleOn * sampleinc;
								break;
							case CENTER_BETWEEN_FRAMES:
								subframe = scanTime + ( 0.5 * ( 1 - ( liqglo.liqglo_shutterTime * m_blurTime ) ) ) + ( msampleOn * sampleinc );
								break;
							case CLOSE_ON_NEXT_FRAME:
								subframe = scanTime + ( 1 - ( liqglo.liqglo_shutterTime * m_blurTime ) ) + ( msampleOn * sampleinc );
								break;
							}
							liqglo.liqglo_sampleTimes[ msampleOn ] = subframe;
							liqglo.liqglo_sampleTimesOffsets[ msampleOn ] = msampleOn * sampleinc;
						}
						//[refactor][1.9.1.1 end] to calaculateSamplingTime()
						//cout <<"about to scan... "<<endl;
						// scan the scene
						//
						if( liqglo.doCameraMotion || liqglo.liqglo_doMotion || liqglo.liqglo_doDef ) 
						{
							for ( int msampleOn = 0; msampleOn < liqglo.liqglo_motionSamples; msampleOn++ ) 
								scanScene( liqglo.liqglo_sampleTimes[ msampleOn ] , msampleOn );
						} else {
							liqglo.liqglo_sampleTimes[ 0 ] = scanTime;
							liqglo.liqglo_sampleTimesOffsets[ 0 ] = 0;
							scanScene( scanTime, 0 );
						}
						//cout <<"    + scene scan done !"<<endl;

						// mark the frame as already scanned
						lastScannedFrame = scanTime;
						liqglo_currentJob = *iter;
					}
					//
					// start scene parsing ------------------------------------------------------------------
					//
					liqglo.liqglo_isShadowPass = liqglo_currentJob.isShadowPass;

					//[refactor][1.9.1.2] begin to getBaseShadowName()
					// build the shadow archive name for the job
					// r773 use this section
					//bool renderAllFrames( liqglo_currentJob.everyFrame );
					//long refFrame( liqglo_currentJob.renderFrame );
					//MString geoSet( liqglo_currentJob.shadowObjectSet );
					//baseShadowName = generateShadowArchiveName( renderAllFrames, refFrame, geoSet );
					//baseShadowName = liquidGetRelativePath( liqglo.liqglo_relativeFileNames, baseShadowName, liqglo.liqglo_ribDir );
					//r775 ymesh use this section
					baseShadowName = generateFileName( fgm_shadow_archive, liqglo_currentJob); 
					baseShadowName = liquidGetRelativePath( liqglo.liqglo_relativeFileNames, baseShadowName, liqglo.liqglo_ribDir );
					//[refactor][1.9.1.2] end to getBaseShadowName()			
					bool out_lightBlock = false;
					


					LIQDEBUGPRINTF( "-> setting RiOptions\n" );


					liquid::RendererMgr::getInstancePtr()->
						getRenderer()->setRenderScriptFormatAndCompress(liqglo.liqglo_doBinary, liqglo.liqglo_doCompression);


					// world RiReadArchives and Rib Boxes ************************************************
					//!liqglo_currentJob.shadowArchiveRibDone is refactored to isBaseShadowReady()
					if( liqglo_currentJob.isShadow && !liqglo.fullShadowRib ) 
					{
						//[refactor][1.9.2 begin] to tShadowRibWriterMgr::write()
						//[refactor][1.9.2.1 begin] to isBaseShadowReady()
						if( !liqglo_currentJob.shadowArchiveRibDone )
						//[refactor][1.9.2.1 end] to isBaseShadowReady()
						{
							//
							//  create the read-archive shadow files
							//  world_only = true
							//  out_lightBlock = (liqglo_currentJob.deepShadows && m_outputLightsInDeepShadows )
							//  archiveName = ""
							//[refactor][1.9.2.2 begin] to isBaseShadowReady()
							out_lightBlock = (liqglo_currentJob.deepShadows && m_outputLightsInDeepShadows );
							//[refactor][1.9.2.2 end] to isBaseShadowReady()
							if ( ribOutput( scanTime, baseShadowName, true, out_lightBlock, MString("") ) != MS::kSuccess )
								break;

							//[refactor][1.9.2.3 begin] to isBaseShadowReady()
							// mark all other jobs with the same set as done
							vector<structJob>::iterator iterCheck = jobList.begin();
							while ( iterCheck != jobList.end() ) 
							{
								if( iterCheck->shadowObjectSet == liqglo_currentJob.shadowObjectSet &&
									iterCheck->everyFrame == liqglo_currentJob.everyFrame &&
									iterCheck->renderFrame == liqglo_currentJob.renderFrame
									)
									iterCheck->shadowArchiveRibDone = true;
								++iterCheck;
							}
							liqglo.m_alfShadowRibGen = true;
							//[refactor][1.9.2.3 end] to isBaseShadowReady()
						}
						//[refactor][1.9.2 end] to tShadowRibWriterMgr::write()

					}//if( liqglo_currentJob.isShadow && !fullShadowRib ) 

					//  create beauty/shadow rib files
					//  world_only = false
					MString archiveName = "";

					if( liqglo_currentJob.isShadow && !liqglo.fullShadowRib ) 
						archiveName = baseShadowName;

					out_lightBlock = (!liqglo_currentJob.isShadow || ( liqglo_currentJob.isShadow && liqglo_currentJob.deepShadows && m_outputLightsInDeepShadows) );

					if ( ribOutput( scanTime, liqglo_currentJob.ribFileName, false, out_lightBlock, archiveName ) != MS::kSuccess )
						break;

					if ( m_showProgress ) 
						printProgress( 3, liqglo.frameNumbers.size(), frameIndex );
				}//for (; iter != jobList.end(); ++iter ) 
				
				//[refactor][1.9.3 begin] to processOneFrame()
				// set the rib file for the 'view last rib' menu command
				// NOTE: this may be overridden later on in certain code paths
				lastRibName = liqglo_currentJob.ribFileName;
				//[refactor][1.9.3 end] to processOneFrame()

			}//if( !m_deferredGen ) 
			//[refactor][1.9 end] to processOneFrame()

			//[refactor][1.10 begin] to _doItNewWithRenderScript()
			// now we re-iterate through the job list to write out the alfred file if we are using it
			if( useRenderScript && !m_justRib ) 
			{
				//[refactor][1.10.1 begin] to _doItNewWithRenderScript()
				bool alf_textures = false;
				bool alf_shadows = false;
				bool alf_refmaps = false;
				bool use_dirmaps = ( liqglo.m_dirmaps.length() )? 1 : 0;
				//[refactor][1.10.1 end] to _doItNewWithRenderScript()

				// write out make texture pass
				LIQDEBUGPRINTF( "-> Generating job for MakeTexture pass\n");
				
				//[refactor][1.10.2 begin] to tFrameScriptJobMgr::makeTexturePass()
				vector<structJob>::iterator iter = txtList.begin();
				if( txtList.size() ) 
				{
					//[refactor][1.10.2.1 begin]
					alf_textures = true;
					liqRenderScript::Job textureJob;
					stringstream ts;
					ts << "Textures." << liqglo.liqglo_lframe;
					textureJob.title = ts.str();
					//[refactor][1.10.2.1 end]

					while ( iter != txtList.end() ) 
					{
						liqRenderScript::Job textureSubtask;
						stringstream ts;
						ts << textureJob.title << " " << iter->imageName.asChar();
						textureSubtask.title = ts.str();
						if( liqglo.m_deferredGen ) {

						}
						stringstream ss;
						ss << iter->renderName.asChar() << " " << iter->ribFileName.asChar();
						liqRenderScript::Cmd cmd( ss.str(), ( liqglo.remoteRender && !liqglo.useNetRman ) );

						if( liqglo.m_alfredExpand ) 
							cmd.alfredExpand = true;

						cmd.alfredServices = liqglo.m_alfredServices.asChar();
						cmd.alfredTags     = liqglo.m_alfredTags.asChar();
						textureSubtask.commands.push_back( cmd );
						textureSubtask.chaserCommand = ( string( "sho \"" ) + liqglo.liqglo_textureDir.asChar() + " " + iter->imageName.asChar() + "\"" );
						++iter;
						textureJob.childJobs.push_back( textureSubtask );
					}
					//[refactor][1.10.2.2 begin] to tFrameScriptJobMgr::makeTexture(
					frameScriptJob.childJobs.push_back( textureJob );
					//[refactor][1.10.2.2 end] to tFrameScriptJobMgr::makeTexture(
				}//if( txtList.size() )
				//[refactor][1.10.2 end] to tFrameScriptJobMgr::makeTexturePass()

				// write out shadows
				if( liqglo.liqglo_doShadows ) 
				{
					LIQDEBUGPRINTF( "-> writing out shadow information to alfred file.\n" );
					vector< structJob >::iterator iter = shadowList.begin();
					//[refactor][1.10.3 begin]
					if( shadowList.size() ) 
					{
						alf_shadows = true;
						liqRenderScript::Job shadowJob;
						stringstream ts;
						ts << "Shadows." << liqglo.liqglo_lframe;
						shadowJob.title = ts.str();
						while ( iter != shadowList.end() ) 
						{
							alf_shadows = true;
							liqRenderScript::Job shadowSubtask;
							shadowSubtask.title = iter->name.asChar();
							if( alf_textures ) 
							{
								stringstream ss;
								ss << "Textures." << liqglo.liqglo_lframe;
								liqRenderScript::Job instanceJob;
								instanceJob.isInstance = true;
								instanceJob.title = ss.str();
								shadowSubtask.childJobs.push_back(instanceJob);
							}
							if( liqglo.m_deferredGen ) 
							{
								stringstream ss;
								ss << liquidTransGetSceneName().asChar() << ".FrameRIBGEN." << currentBlock;
								liqRenderScript::Job instanceJob;
								instanceJob.isInstance = true;
								instanceJob.title = ss.str();
								shadowSubtask.childJobs.push_back(instanceJob);
							}
							stringstream ss;
							if ( liqglo.useNetRman ) 
								ss << framePreCommand.asChar() << " netrender %H ";
							else
								ss << framePreCommand.asChar() << " " << frameRenderCommand.asChar() << " ";
							ss << "-Progress ";

							if ( use_dirmaps ) 
								ss << "%D(" << iter->ribFileName.asChar() << ")";
							else
#ifdef _WIN32
								ss << "\"" << iter->ribFileName.asChar() << "\"";
#else
								ss << iter->ribFileName.asChar();
#endif
							liqRenderScript::Cmd cmd(ss.str(), (liqglo.remoteRender && !liqglo.useNetRman));
							if( liqglo.m_alfredExpand ) 
								cmd.alfredExpand = true;

							cmd.alfredServices = liqglo.m_alfredServices.asChar();
							cmd.alfredTags     = liqglo.m_alfredTags.asChar();
							shadowSubtask.commands.push_back(cmd);

							if(liqglo.cleanRib)  
							{
								stringstream ss;
								ss << framePreCommand.asChar() << " " << RM_CMD << " ";
#ifdef _WIN32
								ss << "\"" << iter->ribFileName.asChar() << "\"";
#else
								ss << iter->ribFileName.asChar();
#endif
								liqRenderScript::Cmd jobShdCommand( ss.str(), liqglo.remoteRender );
								jobShdCommand.alfredServices = liqglo.m_alfredServices.asChar();
								jobShdCommand.alfredTags     = liqglo.m_alfredTags.asChar();
								shadowSubtask.cleanupCommands.push_back( jobShdCommand );
							}
							shadowSubtask.chaserCommand = ( string( "sho \"" ) + iter->imageName.asChar() + "\"" );
							++iter;
							if( !liqglo.m_alfShadowRibGen && !liqglo.fullShadowRib ) 
								liqglo.m_alfShadowRibGen = true;
							shadowJob.childJobs.push_back( shadowSubtask );
						}
						frameScriptJob.childJobs.push_back( shadowJob );
					}
				}//if( liqglo.liqglo_doShadows ) 
				//[refactor][1.10.3 end]
				LIQDEBUGPRINTF( "-> finished writing out shadow information to render script file.\n" );

				// write out make reflection pass
				//[refactor][1.10.4 begin]
				if( refList.size() ) 
				{
					LIQDEBUGPRINTF( "-> Generating job for ReflectionMap pass\n" );
					vector<structJob>::iterator iter = refList.begin();

					alf_refmaps = true;
					liqRenderScript::Job reflectJob;
					stringstream ts;
					ts << "Reflections." << liqglo.liqglo_lframe;
					reflectJob.title = ts.str();

					while ( iter != refList.end() ) 
					{
						liqRenderScript::Job reflectSubtask;
						stringstream ts;
						ts << reflectJob.title << " " << iter->imageName.asChar();
						reflectSubtask.title = ts.str();
						if( liqglo.m_deferredGen ) {

						}
						if( alf_textures ) 
						{
							stringstream ss;
							ss << "Textures." << liqglo.liqglo_lframe;
							liqRenderScript::Job instanceJob;
							instanceJob.isInstance = true;
							instanceJob.title = ss.str();
							reflectJob.childJobs.push_back( instanceJob );
						}
						if( alf_shadows ) 
						{
							stringstream ss;
							ss << "Shadows." << liqglo.liqglo_lframe;
							liqRenderScript::Job instanceJob;
							instanceJob.isInstance = true;
							instanceJob.title = ss.str();
							reflectJob.childJobs.push_back( instanceJob );
						}

						stringstream ss;
						ss << iter->renderName.asChar() << " " << iter->ribFileName.asChar();
						liqRenderScript::Cmd cmd( ss.str(), (liqglo.remoteRender && !liqglo.useNetRman) );

						if( liqglo.m_alfredExpand ) 
							cmd.alfredExpand = true;

						cmd.alfredServices = liqglo.m_alfredServices.asChar();
						cmd.alfredTags     = liqglo.m_alfredTags.asChar();
						reflectSubtask.commands.push_back( cmd );
						reflectSubtask.chaserCommand = ( string( "sho \"" ) + liqglo.liqglo_textureDir.asChar() + " " + iter->imageName.asChar() + "\"" );
						++iter;
						reflectJob.childJobs.push_back( reflectSubtask );
					}
					frameScriptJob.childJobs.push_back( reflectJob );
				}
				//[refactor][1.10.4 end]

				LIQDEBUGPRINTF( "-> initiating hero pass information.\n" );
				structJob *frameJob = NULL;
				structJob *shadowPassJob = NULL;
				LIQDEBUGPRINTF( "-> setting hero pass.\n" );
				if( m_outputHeroPass && !m_outputShadowPass ) 
					frameJob = &jobList[jobList.size() - 1];
				else if( m_outputShadowPass && m_outputHeroPass ) 
				{
					frameJob = &jobList[jobList.size() - 1];
					shadowPassJob = &jobList[jobList.size() - 2];
				} 
				else if( m_outputShadowPass && !m_outputHeroPass ) 
					shadowPassJob = &jobList[jobList.size() - 1];

				LIQDEBUGPRINTF( "-> hero pass set.\n" );
				LIQDEBUGPRINTF( "-> writing out pre frame command information to render script file.\n" );
				//[refactor][1.10.4 begin] to tFrameScriptJobMgr::try_addPreFrameCommand(
				if( framePreFrameCommand != MString("") ) 
				{
					liqRenderScript::Cmd cmd(framePreFrameCommand.asChar(), (liqglo.remoteRender && !liqglo.useNetRman));
					cmd.alfredServices = liqglo.m_alfredServices.asChar();
					cmd.alfredTags     = liqglo.m_alfredTags.asChar();
					frameScriptJob.commands.push_back(cmd);
				}
				//[refactor][1.10.4 end] to tFrameScriptJobMgr::try_addPreFrameCommand(

				if( m_outputHeroPass || m_outputShadowPass ) 
				{
					std::stringstream ss;
					std::string ribFileName;
					if( m_outputHeroPass ){
						//[refactor][1.10.5 begin] to
						ribFileName = std::string( frameJob->ribFileName.asChar() );
						//[refactor][1.10.5 end]to
					}else{
						//[refactor][1.10.6 begin]to
						ribFileName = std::string( shadowPassJob->ribFileName.asChar() );
						//[refactor][1.10.6 end]to
					}
						
					//[refactor][1.10.7 begin]to tFrameScriptJobMgr::addHeroPass()/tFrameScriptJobMgr::addShadowPass()
					if ( liqglo.useNetRman ) 
						ss << framePreCommand.asChar() << " netrender %H ";
					else
						ss << framePreCommand.asChar() << " " << frameRenderCommand.asChar();
					ss << " -Progress ";

					if ( use_dirmaps ) 
						ss << "%D("  << ribFileName << ")";
					else
#ifdef _WIN32            
						ss << "\"" << ribFileName << "\"";
#else           
						ss << ribFileName;
#endif
					//[refactor][1.10.7 end]to tFrameScriptJobMgr::addHeroPass()/tFrameScriptJobMgr::addShadowPass()

					liqRenderScript::Cmd cmd(ss.str(), (liqglo.remoteRender && !liqglo.useNetRman));
					if( liqglo.m_alfredExpand ) 
						cmd.alfredExpand = true;

					cmd.alfredServices = liqglo.m_alfredServices.asChar();
					cmd.alfredTags     = liqglo.m_alfredTags.asChar();
					frameScriptJob.commands.push_back(cmd);
				}//if( m_outputHeroPass  || m_outputShadowPass) 

				LIQDEBUGPRINTF( "-> finished writing out hero information to alfred file.\n" );
#if 0
				if( m_outputShadowPass ) 
				{
					//[refactor][1.10.8 begin] to tFrameScriptJobMgr::addShadowPass(
					stringstream ss;
					if( liqglo.useNetRman ) 
					{
#ifdef _WIN32
						ss << framePreCommand.asChar() << " netrender %H -Progress \"" << shadowPassJob->ribFileName.asChar() << "\"";
#else
						ss << framePreCommand.asChar() << " netrender %H -Progress " << shadowPassJob->ribFileName.asChar();
#endif
					} else {
#ifdef _WIN32
						ss << framePreCommand.asChar() << " " << frameRenderCommand.asChar() << " \"" << shadowPassJob->ribFileName.asChar() << "\"";
#else
						ss << framePreCommand.asChar() << " " << frameRenderCommand.asChar() << " " << shadowPassJob->ribFileName.asChar();
#endif
					}
					liqRenderScript::Cmd cmd(ss.str(), (liqglo.remoteRender && !liqglo.useNetRman));
					if( liqglo.m_alfredExpand ) 
						cmd.alfredExpand = true;

					cmd.alfredServices = liqglo.m_alfredServices.asChar();
					cmd.alfredTags     = liqglo.m_alfredTags.asChar();
					frameScriptJob.commands.push_back(cmd);
					//[refactor][1.10.8 end] to tFrameScriptJobMgr::addShadowPass(
				}//if( m_outputShadowPass )
#endif
				if( liqglo.cleanRib || ( framePostFrameCommand != MString( "" ) ) ) 
				{
					if( liqglo.cleanRib & ( m_outputHeroPass ||  m_outputShadowPass || liqglo.m_alfShadowRibGen )) 
					{
						std::stringstream ss;
						std::string  ribFileName;

						if( m_outputHeroPass )
							ribFileName = string( frameJob->ribFileName.asChar() );//[refactor][1.10.9]
						else if ( m_outputShadowPass) 
							ribFileName = string( shadowPassJob->ribFileName.asChar() );//[refactor][1.10.10]
						else
							ribFileName = string( baseShadowName.asChar() );//[refactor][1.10.11]
						
						//[refactor][1.10.12 begin]to
						ss << framePreCommand.asChar() << " " << RM_CMD << " ";
						if ( use_dirmaps ) 
							ss << "%D(" << ribFileName << ")";
						else
#ifdef _WIN32
							ss << "\"" << ribFileName << "\"";
#else
							ss << ribFileName;
#endif

						// frameScriptJob.cleanupCommands.push_back(liqRenderScript::Cmd(ss.str(), remoteRender));
						liqRenderScript::Cmd jobCleanCommand( ss.str(),  liqglo.remoteRender );
						jobCleanCommand.alfredServices =  liqglo.m_alfredServices.asChar();
						jobCleanCommand.alfredTags =  liqglo.m_alfredTags.asChar();
						frameScriptJob.cleanupCommands.push_back( jobCleanCommand );
						//[refactor][1.10.12 end]to
					}

					//[refactor][1.10.13 begin]to
					if( framePostFrameCommand != MString("") ) 
					{
						// liqRenderScript::Cmd cmd(framePostFrameCommand.asChar(), (remoteRender && !useNetRman));
						liqRenderScript::Cmd cmd( framePostFrameCommand.asChar(), ( liqglo.remoteRender && ! liqglo.useNetRman) );
						cmd.alfredServices =  liqglo.m_alfredServices.asChar();
						cmd.alfredTags =  liqglo.m_alfredTags.asChar();
						frameScriptJob.cleanupCommands.push_back(cmd);
					}
					//[refactor][1.10.13 end]to

				}//if( cleanRib || ( framePostFrameCommand != MString( "" ) ) ) 
				if( m_outputHeroPass ) 
					frameScriptJob.chaserCommand = (string( "sho \"" ) + frameJob->imageName.asChar() + "\"" );
				if( m_outputShadowPass ) 
					frameScriptJob.chaserCommand = (string( "sho \"" ) + shadowPassJob->imageName.asChar() + "\"" );
				
				if( m_outputShadowPass && !m_outputHeroPass ) 
					lastRibName = liquidGetRelativePath( liqglo.liqglo_relativeFileNames, shadowPassJob->ribFileName, liqglo.liqglo_projectDir );
				else 
					lastRibName = liquidGetRelativePath( liqglo.liqglo_relativeFileNames, frameJob->ribFileName, liqglo.liqglo_projectDir );
			}//if( useRenderScript && !m_justRib ) 

			jobScript.addJob( frameScriptJob );
			if( ( ribStatus != kRibOK ) && !liqglo.m_deferredGen ) 
				break;
		} // frame for-loop

		if( useRenderScript ) 
		{
			if( m_preJobCommand != MString( "" ) ) 
			{
				//[refactor][1.11 begin]to
				jobScript.addLeafDependency( preJobInstance );
				//[refactor][1.11 end]to
			}
			// clean up the alfred file in the future
			if( !m_justRib ) 
			{
				if( liqglo.m_deferredGen ) 
				{
					//[refactor][1.12 begin]to
					string cmd = RM_CMD  + string( MString( " \""  + tempDefname + "\"" ).asChar() );
					liqRenderScript::Cmd jobCleanCmd( cmd, 0 );
					jobCleanCmd.alfredServices =  liqglo.m_alfredServices.asChar();
					jobCleanCmd.alfredTags =  liqglo.m_alfredTags.asChar();
					jobScript.cleanupCommands.push_back( jobCleanCmd );
					//[refactor][1.12 end]to
				}
				if( cleanRenderScript ) 
				{
					//[refactor][1.13 ]
					string cmd = RM_CMD  + string( MString( " \""  + renderScriptName + "\"" ).asChar() );
					liqRenderScript::Cmd jobCleanCmd( cmd, 0 );
					jobCleanCmd.alfredServices =  liqglo.m_alfredServices.asChar();
					jobCleanCmd.alfredTags =  liqglo.m_alfredTags.asChar();
					jobScript.cleanupCommands.push_back( jobCleanCmd );
					//[refactor][1.13 ]
				}
				if( m_postJobCommand != MString("") ) 
				{
					//[refactor][1.14 ]
					string cmd = m_postJobCommand.asChar();

					liqRenderScript::Cmd jobCleanCmd( cmd, 0 );
					jobCleanCmd.alfredServices =  liqglo.m_alfredServices.asChar();
					jobCleanCmd.alfredTags =  liqglo.m_alfredTags.asChar();
					jobScript.cleanupCommands.push_back( jobCleanCmd );
					//[refactor][1.14 ]
				}
			}
			if( m_renderScriptFormat == ALFRED ) 
				jobScript.writeALF( liquidGetRelativePath( liqglo.liqglo_relativeFileNames, renderScriptName, liqglo.liqglo_projectDir ).asChar() );
			else if( m_renderScriptFormat == XML ) 
				jobScript.writeXML( liquidGetRelativePath( liqglo.liqglo_relativeFileNames, renderScriptName, liqglo.liqglo_projectDir ).asChar() );
		}
		LIQDEBUGPRINTF( "-> ending escape handler.\n" );
		m_escHandler.endComputation();

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
		nPlug = rGlobalNode.findPlug( "lastRibFile" );
		nPlug.setValue( lastRibName );
		LIQDEBUGPRINTF( "-> spawning command.\n" );
		if( liqglo.launchRender ) 
		{
			//[refactor][1.15 ]
			if( useRenderScript ) 
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
				  stringstream err;
				  err << ">> render (" << ( (!wait)? "no " : "" ) << "wait) "<< cmd.asChar() << endl << ends;
				  liquidMessage( err.str(), messageInfo );
				  int returnCode = ::system( cmd.asChar() );
				} else{
#ifdef _WIN32
					// Moritz: Added quotes to render script name as it may contain spaces in bloody Windoze
					// Note: also adding quotes to the path (aka project dir) breaks ShellExecute() -- cost me one hour to trace this!!!
					// Bloody, damn, asinine Windoze!!!
					printf("2.liqProcessLauncher::execute(%s, \"%s\" ,%s, %d)\n", m_renderScriptCommand.asChar(), renderScriptName.asChar(), liqglo.liqglo_projectDir.asChar(), false);
					liqProcessLauncher::execute( m_renderScriptCommand, "\"" + renderScriptName + "\"", liqglo.liqglo_projectDir, false );
#else
					liqProcessLauncher::execute( m_renderScriptCommand, renderScriptName, liqglo.liqglo_projectDir, false );
#endif
				}

			} 
			//[refactor][1.15 ]
			else 
			{
				//[refactor][1.16 ]
				// launch renders directly
				liquidMessage( string(), messageInfo ); // emit a '\n'
        		// int exitstat = 0; ???
				
				//[refactor][1.17 ]
				// write out make texture pass
				vector<structJob>::iterator iter = txtList.begin();
				while ( iter != txtList.end() ) 
				{
					liquidMessage( "Making textures '" + string( iter->imageName.asChar() ) + "'", messageInfo );
					liqProcessLauncher::execute( iter->renderName, 
#ifdef _WIN32
						(" -progress \"" + iter->ribFileName + "\""), 
#else
						(" -progress " + iter->ribFileName), 
#endif
						liqglo.liqglo_projectDir, true 
					);
					++iter;
				}
				//[refactor][1.17 ]
				//
				// write out shadows
				//
				if( liqglo.liqglo_doShadows ) 
				{
					//[refactor][1.18 ]
					liquidMessage( "Rendering shadow maps... ", messageInfo );
					vector<structJob>::iterator iter = shadowList.begin();
					while ( iter != shadowList.end() ) 
					{
						if( iter->skip ) 
						{
							liquidMessage( "    - skipping '" + string( iter->ribFileName.asChar() ) + "'", messageInfo );
							++iter;
							continue;
						}
						liquidMessage( "    + '" + string( iter->ribFileName.asChar() ) + "'", messageInfo );
						if ( 
							!liqProcessLauncher::execute(  liqglo.liquidRenderer.renderCommand,  liqglo.liquidRenderer.renderCmdFlags + " " +
#ifdef _WIN32
							"\"" + iter->ribFileName + "\"", 
#else
							iter->ribFileName, 
#endif
							 liqglo.liqglo_projectDir, true 
							) 
						) break;
						++iter;
					} // while ( iter != shadowList.end() )
					//[refactor][1.18 ]
				}
				//
				// write out hero pass
				//
				{
					liquidMessage( "Rendering hero pass... ", messageInfo );
					cerr << "liquidBin = " <<  liqglo.liquidBin << endl << flush; 
					if( liqglo_currentJob.skip ) 
					{
						//[refactor][1.19 ]
						liquidMessage( "    - skipping '" + string( liqglo_currentJob.ribFileName.asChar() ) + "'", messageInfo );
						//[refactor][1.19 ]
					} 
					else 
					{
						//[refactor][1.20 ]
						liquidMessage( "    + '" + string( liqglo_currentJob.ribFileName.asChar() ) + "'", messageInfo );
						liqProcessLauncher::execute( 
							liqglo.liquidRenderer.renderCommand, 
#ifdef _WIN32
							" "+liqglo.liqglo_rifParams+" "+ liqglo.liquidRenderer.renderCmdFlags + " \"" + liqglo_currentJob.ribFileName + "\"", 
							"\"" + liqglo.liqglo_projectDir + "\"", 
#else
							" "+liqglo.liqglo_rifParams+" "+ liqglo.liquidRenderer.renderCmdFlags + " " + liqglo_currentJob.ribFileName, 
							liqglo.liqglo_projectDir,
#endif
							false
						);
						//[refactor][1.20 ]
					}
				}//if( !exitstat ) 
			}//if( useRenderScript ) else
			//
			//  philippe: here we launch the liquidRenderView command which will listen to the liqmaya display driver
			//  to display buckets in the renderview.
			if ( liqglo.m_renderView ) 
			{
				//[refactor][1.21 ]
				stringstream displayCmd;
				displayCmd << "liquidRenderView -c " << liqglo.renderCamera.asChar();
				displayCmd << " -l " << ( ( liqglo.m_renderViewLocal )? "1":"0" );
				displayCmd << " -port " << liqglo.m_renderViewPort;
				displayCmd << " -timeout " << liqglo.m_renderViewTimeOut;
				if ( liqglo.m_renderViewCrop ) 
					displayCmd << " -doRegion";
				displayCmd << ";liquidSaveRenderViewImage();";
				MGlobal::executeCommand( MString( displayCmd.str().c_str() ) );
				//[refactor][1.21 ]
			} 
		} // if( launchRender )
		
		//[refactor][1.22 ]
		// return to the frame we were at before we ran the animation
		LIQDEBUGPRINTF( "-> setting frame to current frame.\n" );
		MGlobal::viewFrame (originalTime);

		if( originalLayer != "" ) 
		{
			MString cmd;
			cmd = "if( `editRenderLayerGlobals -q -currentRenderLayer` != \"" + originalLayer + "\" ) editRenderLayerGlobals -currentRenderLayer \"" + originalLayer + "\";";
			if(  MGlobal::executeCommand( cmd, false, false ) == MS::kFailure ) 
			{
				MString err;
				err = "Liquid : could not switch back to render layer \"" + originalLayer + "\" !";
				throw err;
			}
		}
		//[refactor][1.22 ]

		return ( (ribStatus == kRibOK || liqglo.m_deferredGen) ? MS::kSuccess : MS::kFailure);

	} 
	catch ( MString errorMessage ) 
	{
		liquidMessage( errorMessage.asChar(), messageError );
		/*if( htable && hashTableInited ) delete htable;
		freeShaders();*/
		m_escHandler.endComputation();
		return MS::kFailure;
	} 
	catch ( ... ) 
	{
		liquidMessage( "Unknown exception thrown", messageError );
		/*if( htable && hashTableInited ) delete htable;
		freeShaders();*/
		m_escHandler.endComputation();
		return MS::kFailure;
	}
	//[refactor][1 end] _doItNewWithoutRenderScript()/_doItNewWithRenderScript()
}

/**
* Calculate the port field of view for the camera.
*/
void liqRibTranslator::portFieldOfView( int port_width, int port_height,
									   double& horizontal,
									   double& vertical,
									   const MFnCamera& fnCamera )
{
	CM_TRACE_FUNC("liqRibTranslator::portFieldOfView("<<port_width<<","<<port_height<<",&horizontal,&vertical,&fnCamera)");
	// note : works well - api tested
	double left, right, bottom, top;
	double aspect = (double) port_width / port_height;
	computeViewingFrustum(aspect,left,right,bottom,top,fnCamera);

	double neardb = fnCamera.nearClippingPlane();
	horizontal    = atan( ( ( right - left ) * 0.5 ) / neardb ) * 2.0;
	vertical      = atan( ( ( top - bottom ) * 0.5 ) / neardb ) * 2.0;
}

/**
* Calculate the viewing frustrum for the camera.
*/
void liqRibTranslator::computeViewingFrustum ( double     window_aspect,
											  double&    left,
											  double&    right,
											  double&    bottom,
											  double&    top,
											  const MFnCamera& cam )
{
	CM_TRACE_FUNC("liqRibTranslator::computeViewingFrustum("<<window_aspect<<","<<",&left,&right,&bottom,&top,"<<cam.fullPathName().asChar()<<")");

	double film_aspect   = cam.aspectRatio();
	double aperture_x    = cam.horizontalFilmAperture();
	double aperture_y    = cam.verticalFilmAperture();
	double offset_x      = cam.horizontalFilmOffset();
	double offset_y      = cam.verticalFilmOffset();
	double focal_to_near = cam.nearClippingPlane() / (cam.focalLength() * MM_TO_INCH);

	focal_to_near *= cam.cameraScale();

	double scale_x = 1.0;
	double scale_y = 1.0;
	double translate_x = 0.0;
	double translate_y = 0.0;

	switch ( cam.filmFit() ) 
	{
	case MFnCamera::kFillFilmFit:
		if( window_aspect < film_aspect ) 
			scale_x = window_aspect / film_aspect;
		else 
			scale_y = film_aspect / window_aspect;
		break;
	case MFnCamera::kHorizontalFilmFit:
		scale_y = film_aspect / window_aspect;
		if( scale_y > 1.0 ) 
			translate_y = cam.filmFitOffset() * ( aperture_y - ( aperture_y * scale_y ) ) / 2.0;
		break;
	case MFnCamera::kVerticalFilmFit:
		scale_x = window_aspect / film_aspect;
		if(scale_x > 1.0 ) 
			translate_x = cam.filmFitOffset() * ( aperture_x - ( aperture_x * scale_x ) ) / 2.0;
		break;
	case MFnCamera::kOverscanFilmFit:
		if( window_aspect < film_aspect ) 
			scale_y = film_aspect / window_aspect;
		else 
			scale_x = window_aspect / film_aspect;
		break;
	case MFnCamera::kInvalid:
		break;
	}

	left   = focal_to_near * (-.5 * aperture_x * scale_x + offset_x + translate_x );
	right  = focal_to_near * ( .5 * aperture_x * scale_x + offset_x + translate_x );
	bottom = focal_to_near * (-.5 * aperture_y * scale_y + offset_y + translate_y );
	top    = focal_to_near * ( .5 * aperture_y * scale_y + offset_y + translate_y );

	// NOTE :
	//      all the code above could be replaced by :
	//
	//      cam.getRenderingFrustum( window_aspect, left, right, bottom, top );
	//
	//      should we keep this for educationnal purposes or use the API call ??
}
void liqRibTranslator::exportJobCamera(const structJob &job, const structCamera camera[])
{
	CM_TRACE_FUNC("liqRibTranslator::exportJobCamera("<<job.name.asChar()<<","<<",)");

	if ( camera[0].isOrtho )
	{
		RtFloat frameWidth, frameHeight;
		// the whole frame width has to be scaled according to the UI Unit
		frameWidth  = camera[0].orthoWidth  * 0.5 ;
		frameHeight = camera[0].orthoHeight * 0.5 ;
		RiProjection( "orthographic", RI_NULL );
		// if we are describing a shadow map camera,
		// we need to set the screenwindow to the default,
		// as shadow maps are always square.
		if( job.isShadow == true ) RiScreenWindow( -frameWidth, frameWidth, -frameHeight, frameHeight );
		else 			                 RiScreenWindow( -1.0, 1.0, -1.0, 1.0 );
	}
	else
	{
		RtFloat fieldOfView = camera[0].hFOV * 180.0 / M_PI ;
		if ( job.isShadow && job.isPoint ) fieldOfView = job.camera[0].hFOV;

		RiProjection( "perspective", RI_FOV, &fieldOfView, RI_NULL );

		// if we are describing a shadow map camera,
		// we need to set the screenwindow to the default,
		// as shadow maps are always square.

		if ( job.isShadow == false )
		{
			double ratio = (double)job.width / (double)job.height;
			double left, right, bottom, top;
			if ( ratio <= 0 )
			{
				left    = -1 + camera[0].horizontalFilmOffset;
				right   =  1 + camera[0].horizontalFilmOffset;
				bottom  = -1 / ratio + camera[0].verticalFilmOffset;
				top     =  1 / ratio + camera[0].verticalFilmOffset;
			}
			else
			{
				left    = -ratio + camera[0].horizontalFilmOffset;
				right   =  ratio + camera[0].horizontalFilmOffset;
				bottom  = -1 + camera[0].verticalFilmOffset;
				top     =  1 + camera[0].verticalFilmOffset;
			}
			RiScreenWindow( left, right, bottom, top );
		}
		else
		{
			RiScreenWindow( -1.0, 1.0, -1.0, 1.0 );
		}
	}
	RiClipping( camera[0].neardb, camera[0].fardb );
	if ( liqglo.doDof && !job.isShadow ) RiDepthOfField( camera[0].fStop, camera[0].focalLength, camera[0].focalDistance );
	// if we motion-blur the cam, open the motion block
	//
	if ( liqglo.doCameraMotion && ( !job.isShadow || job.deepShadows) )
	{
		if ( liqglo.liqglo_relativeMotion ) 
			RiMotionBeginV( liqglo.liqglo_motionSamples, liqglo.liqglo_sampleTimesOffsets );
		else 		                     
			RiMotionBeginV( liqglo.liqglo_motionSamples, liqglo.liqglo_sampleTimes );
	}

	// write the camera transform
	//
	RtMatrix cameraMatrix;
	camera[0].mat.get( cameraMatrix );
	RiTransform( cameraMatrix );

	// if we motion-blur the cam, write the subsequent motion samples and close the motion block
	//
	if ( liqglo.doCameraMotion && ( !job.isShadow || job.deepShadows ) )
	{
		int mm = 1;
		while ( mm < liqglo.liqglo_motionSamples )
		{
			camera[mm].mat.get( cameraMatrix );
			RiTransform( cameraMatrix );
			++mm;
		}
		RiMotionEnd();
	}
}
/**
 * getCameraTransform
 */
MStatus liqRibTranslator::getCameraTransform(const MFnCamera& cam, structCamera &camStruct )
{
	CM_TRACE_FUNC("liqRibTranslator::getCameraTransform("<<cam.fullPathName().asChar()<<",)");
	MStatus status;
	MDagPath cameraPath;
	cam.getPath(cameraPath);
	MTransformationMatrix xform( cameraPath.inclusiveMatrix(&status) );
	if ( status != MS::kSuccess ) // error ?!... set identity...
	{
		char errorMsg[512];
		sprintf(errorMsg, "Cannot get transfo matrix for camera '%s' \n", cam.name().asChar());
		//liquidMessage(errorMsg, messageError );
		printf(errorMsg);
		MMatrix id;
		camStruct.mat = id;
		return MS::kFailure;
	}
	// MMatrix mxform = xform.asMatrix();
	// printf("CAM MATRIX '%s' : \n", cam.name().asChar() );
	// printf("%f %f %f %f \n", mxform(0, 0), mxform(0, 1), mxform(0, 2), mxform(0, 3));
	// printf("%f %f %f %f \n", mxform(1, 0), mxform(1, 1), mxform(1, 2), mxform(1, 3));
	// printf("%f %f %f %f \n", mxform(2, 0), mxform(2, 1), mxform(2, 2), mxform(2, 3));
	// printf("%f %f %f %f \n", mxform(3, 0), mxform(3, 1), mxform(3, 2), mxform(3, 3));

	// the camera is pointing toward negative Z
	double scale[] = { 1, 1, -1 };
	xform.setScale( scale, MSpace::kTransform );

	// scanScene:
	// philippe : rotate the main camera 90 degrees around Z-axis if necessary
	// ( only in main camera )
	MMatrix camRotMatrix;
	if ( liqglo.liqglo_rotateCamera == true )
	{
		float crm[4][4] =	{	{  0.0,  1.0,  0.0,  0.0 },
								{ -1.0,  0.0,  0.0,  0.0 },
								{  0.0,  0.0,  1.0,  0.0 },
								{  0.0,  0.0,  0.0,  1.0 }	};
		camRotMatrix = crm;
	}
	camStruct.mat = xform.asMatrixInverse() * camRotMatrix;
	return MS::kSuccess;
}
/**
 * getCameraFilmOffset
 */
void liqRibTranslator::getCameraFilmOffset(const MFnCamera& cam, structCamera &camStruct )
{
	CM_TRACE_FUNC("liqRibTranslator::getCameraFilmOffset("<<cam.fullPathName().asChar()<<",)");
	// film back offsets
	double hSize, vSize, hOffset, vOffset;
	cam.getFilmFrustum( cam.focalLength(), hSize, vSize, hOffset, vOffset );

	double imr = ((float)camStruct.width / (float)camStruct.height);
	double fbr = hSize / vSize;
	double ho, vo;

	// convert inches to mm !
	hOffset *= 25.4;
	vOffset *= 25.4;
	switch( cam.filmFit() )
	{
	case MFnCamera::kVerticalFilmFit:
	case MFnCamera::kFillFilmFit:
		{
			ho = hOffset / vSize * 2.0;
			vo = vOffset / vSize * 2.0;
			break;
		}
	case MFnCamera::kHorizontalFilmFit:
	case MFnCamera::kOverscanFilmFit:
		{
			ho = hOffset / ( vSize * fbr / imr ) * 2.0;
			vo = vOffset / ( vSize * fbr / imr ) * 2.0;
			break;
		}
	default:
		{
			ho = 0;
			vo = 0;
			break;
		}
	}
	camStruct.horizontalFilmOffset = ho;
	camStruct.verticalFilmOffset   = vo;
}
/**
* Get information about the given camera.
*/
void liqRibTranslator::getCameraInfo(const MFnCamera& cam, structCamera &camStruct )
{
	CM_TRACE_FUNC("liqRibTranslator::getCameraInfo("<<cam.fullPathName().asChar()<<",)");

	// Resolution can change if camera film-gate clips image
	// so we must keep camera width/height separate from render
	// globals width/height.
	//
	camStruct.width  = width;
	camStruct.height = height;

	// If we are using a film-gate then we may need to
	// adjust the resolution to simulate the 'letter-boxed'
	// effect.
	if( cam.filmFit() == MFnCamera::kHorizontalFilmFit ) 
	{
		if( !ignoreFilmGate ) 
		{
			double new_height = camStruct.width / ( cam.horizontalFilmAperture() / cam.verticalFilmAperture() );
			if( new_height < camStruct.height ) 
				camStruct.height = ( int )new_height;
		}

		double hfov, vfov;
		portFieldOfView( camStruct.width, camStruct.height, hfov, vfov, cam );
		camStruct.fov_ratio = hfov / vfov;
	}
	else if( cam.filmFit() == MFnCamera::kVerticalFilmFit ) 
	{
		double new_width = camStruct.height / ( cam.verticalFilmAperture() / cam.horizontalFilmAperture() );
		double hfov, vfov;

		// case 1 : film-gate smaller than resolution
		//         film-gate on
		if( ( new_width < camStruct.width ) && ( !ignoreFilmGate ) ) 
		{
			camStruct.width = ( int )new_width;
			camStruct.fov_ratio = 1.0;
		}
		// case 2 : film-gate smaller than resolution
		//         film-gate off
		else if( ( new_width < camStruct.width ) && ( ignoreFilmGate ) ) 
		{
			portFieldOfView( ( int )new_width, camStruct.height, hfov, vfov, cam );
			camStruct.fov_ratio = hfov / vfov;
		}
		// case 3 : film-gate larger than resolution
		//         film-gate on
		else if( !ignoreFilmGate ) 
		{
			portFieldOfView( ( int )new_width, camStruct.height, hfov, vfov, cam );
			camStruct.fov_ratio = hfov / vfov;
		}
		// case 4 : film-gate larger than resolution
		//         film-gate off
		else if( ignoreFilmGate ) 
		{
			portFieldOfView( ( int )new_width, camStruct.height, hfov, vfov, cam );
			camStruct.fov_ratio = hfov / vfov;
		}
	}
	else if( cam.filmFit() == MFnCamera::kOverscanFilmFit ) 
	{
		double new_height = camStruct.width / ( cam.horizontalFilmAperture() / cam.verticalFilmAperture() );
		double new_width = camStruct.height / ( cam.verticalFilmAperture() / cam.horizontalFilmAperture() );

		if( new_width < camStruct.width ) 
		{
			if( !ignoreFilmGate ) {
				camStruct.width = ( int ) new_width;
				camStruct.fov_ratio = 1.0;
			}
			else 
			{
				double hfov, vfov;
				portFieldOfView( ( int )new_width, camStruct.height, hfov, vfov, cam );
				camStruct.fov_ratio = hfov / vfov;
			}
		}
		else {
			if( !ignoreFilmGate )
				camStruct.height = ( int ) new_height;
			double hfov, vfov;
			portFieldOfView( camStruct.width, camStruct.height, hfov, vfov, cam );
			camStruct.fov_ratio = hfov / vfov;
		}
	}
	else if( cam.filmFit() == MFnCamera::kFillFilmFit ) 
	{
		double new_width = camStruct.height / ( cam.verticalFilmAperture() / cam.horizontalFilmAperture() );
		double hfov, vfov;
		if( new_width >= camStruct.width ) 
		{
			portFieldOfView( ( int )new_width, camStruct.height, hfov, vfov, cam );
			camStruct.fov_ratio = hfov / vfov;
		}
		else 
		{
			portFieldOfView( camStruct.width, camStruct.height, hfov, vfov, cam );
			camStruct.fov_ratio = hfov / vfov;
		}
	}
}

/**
* Set up data for the current job.
*/
MStatus liqRibTranslator::buildJobs()
{
	CM_TRACE_FUNC("liqRibTranslator::buildJobs()");

	LIQDEBUGPRINTF( "-> beginning to build job list\n" );
	MStatus returnStatus = MS::kSuccess;
	MStatus status;
	MObject cameraNode;
	MDagPath lightPath;
	jobList.clear();
	shadowList.clear();
	txtList.clear();
	structJob thisJob;

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
		//[refactor 2] begin
		//[refactor 2.1] begin
		MItDag dagIterator( MItDag::kDepthFirst, MFn::kLight, &returnStatus );
		for (; !dagIterator.isDone(); dagIterator.next()) 
		{
			if( !dagIterator.getPath( lightPath ) ) 
				continue;
			bool usesDepthMap = false;
			MFnLight fnLightNode( lightPath );
			liquidGetPlugValue( fnLightNode, "useDepthMapShadows", usesDepthMap, status );
			if( usesDepthMap && areObjectAndParentsVisible( lightPath ) ) 
			{
				// philippe : this is the default and can be overriden
				// by the everyFrame/renderAtFrame attributes.
				//
				thisJob.renderFrame           = liqglo.liqglo_lframe;
				thisJob.everyFrame            = true;
				thisJob.shadowObjectSet       = "";
				thisJob.shadowArchiveRibDone  = false;
				thisJob.skip                  = false;
				//
				// We have a shadow job, so find out if we need to use deep shadows,
				// and the pixel sample count
				//
				thisJob.deepShadows                 = false;
				thisJob.shadowPixelSamples          = 1;
				thisJob.shadowVolumeInterpretation  = 1;
				thisJob.shadingRateFactor           = 1.0;
				thisJob.shadowAggregation			= 0;

				// philippe : we grab the job's resolution now instead of in the output phase
				// that way , we can make sure one light can generate many shadow maps
				// with different resolutions
				thisJob.aspectRatio = 1.0;
				liquidGetPlugValue( fnLightNode, "dmapResolution", thisJob.width, status );
				thisJob.height = thisJob.width;

				// Get to our shader node.
				//
				MPlug liquidLightShaderNodeConnection;
				MStatus liquidLightShaderStatus;
				liquidLightShaderNodeConnection = fnLightNode.findPlug( "liquidLightShaderNode", &liquidLightShaderStatus );
				if( liquidLightShaderStatus == MS::kSuccess && liquidLightShaderNodeConnection.isConnected() )
				{
					MPlugArray liquidLightShaderNodePlugArray;
					liquidLightShaderNodeConnection.connectedTo( liquidLightShaderNodePlugArray, true, true );
					MFnDependencyNode fnLightShaderNode( liquidLightShaderNodePlugArray[0].node() );

					// Now grab the parameters.
					//
					liquidGetPlugValue( fnLightShaderNode, "deepShadows", thisJob.deepShadows, status );

					// Only use the pixel samples and volume interpretation with deep shadows.
					//
					if( thisJob.deepShadows )
					{
						liquidGetPlugValue( fnLightShaderNode, "pixelSamples", thisJob.shadowPixelSamples, status );
						liquidGetPlugValue( fnLightShaderNode, "volumeInterpretation", thisJob.shadowVolumeInterpretation, status );
						
						thisJob.imageMode    = liqglo.liquidRenderer.dshImageMode;        //"deepopacity";
						thisJob.format       = liqglo.liquidRenderer.dshDisplayName;    //"deepshad";

						int displayImageMode = 0; // 0 = default
						liquidGetPlugValue( fnLightShaderNode, "liqDeepShadowsDisplayMode", displayImageMode, status );
						if ( displayImageMode ) thisJob.imageMode = MString( "deepprevdisttotal" );

					}

					// philippe : check the shadow rendering frequency
					//
					liquidGetPlugValue( fnLightShaderNode, "everyFrame", thisJob.everyFrame, status );

					// philippe : this is crucial, as we rely on the renderFrame to check
					// which frame the scene should be scanned for that job.
					// If the job is a shadow rendering once only at a given frame, we take the
					// renderAtFrame attribute, otherwise, the current time.
					//
					if( !thisJob.everyFrame ) 
						liquidGetPlugValue( fnLightShaderNode, "renderAtFrame", thisJob.renderFrame, status );  

					// Check if the shadow aggregation option is used
					liquidGetPlugValue( fnLightShaderNode, "aggregateShadowMaps", thisJob.shadowAggregation, status );  

					// philippe : check the shadow geometry set
					//
					liquidGetPlugValue( fnLightShaderNode, "geometrySet", thisJob.shadowObjectSet, status );
					liquidGetPlugValue( fnLightShaderNode, "shadingRateFactor", thisJob.shadingRateFactor, status );
				} 
				else 
				{
					/* Here we support the same options as those found on light shader nodes
					but we look for dynamic attributes, so we need a bit more error checking.
					*/
					//MPlug paramPlug = fnLightNode.findPlug( "deepShadows", &status );
					liquidGetPlugValue( fnLightNode, "deepShadows", thisJob.deepShadows, status );
					if( thisJob.deepShadows ) 
					{
						liquidGetPlugValue( fnLightNode, "pixelSamples", thisJob.shadowPixelSamples, status );
						liquidGetPlugValue( fnLightNode, "volumeInterpretation", thisJob.shadowVolumeInterpretation, status );
					}
					liquidGetPlugValue( fnLightNode, "everyFrame", thisJob.everyFrame, status );
					if( !thisJob.everyFrame ) 
						liquidGetPlugValue( fnLightNode, "renderAtFrame", thisJob.renderFrame, status );  

					liquidGetPlugValue( fnLightNode, "geometrySet", thisJob.shadowObjectSet, status );  
					liquidGetPlugValue( fnLightNode, "shadingRateFactor", thisJob.shadingRateFactor, status ); 
				}
				// this will store the shadow camera path and the test's result
				bool lightHasShadowCam = false;//1
				MDagPathArray shadowCamPath;

				if( lightPath.hasFn( MFn::kSpotLight ) || lightPath.hasFn( MFn::kDirectionalLight ) ) 
				{
					//[refactor 2.1.1] begin
					bool computeShadow = true;
					thisJob.hasShadowCam = false;
					MPlug liquidLightShaderNodeConnection;
					MStatus liquidLightShaderStatus;
					liquidLightShaderNodeConnection = fnLightNode.findPlug( "liquidLightShaderNode", &liquidLightShaderStatus );

					if( liquidLightShaderStatus == MS::kSuccess && liquidLightShaderNodeConnection.isConnected() ) 
					{
						// a shader is connected to the light !
						MPlugArray liquidLightShaderNodePlugArray;
						liquidLightShaderNodeConnection.connectedTo( liquidLightShaderNodePlugArray, true, true );
						MFnDependencyNode fnLightShaderNode( liquidLightShaderNodePlugArray[0].node() );

						// has the main shadow been disabled ?
						liquidGetPlugValue( fnLightShaderNode, "generateMainShadow", computeShadow, status ); 

						// look for shadow cameras...
						MStatus stat;
						//[refactor 2.1.1.1] begin//added in liquidMaya r772   begin
						// at first, check if shadow main camera is specified
						// cerr << ">> at first, check if shadow main camera is specified for "  << lightPath.fullPathName().asChar() << endl;

						MString camName;
						liquidGetPlugValue( fnLightShaderNode, "shadowMainCamera", camName, status ); 
						if ( status == MS::kSuccess && camName != "" )
						{
							// cerr << ">> Light node has main shadow camera : " << camName.asChar() << endl;
							MDagPath cameraPath;
							MSelectionList camList;
							camList.add( camName );
							camList.getDagPath( 0, cameraPath );
							if ( cameraPath.hasFn( MFn::kCamera ) )
							{
								// cerr << ">> cameraPath : "<< cameraPath.fullPathName().asChar() << endl;
								thisJob.hasShadowCam = true;
								thisJob.shadowCamPath = cameraPath;
							}
							else
							{
								// cerr << ">> Invalid camera name " << endl;
								string err = "Invalid main shadow camera name " + string( camName.asChar() ) + " for light " + string( lightPath.fullPathName().asChar() );
								liquidMessage( err, messageError );
							}
						}
						// now we're looking for extra cameras 
						//[refactor 2.1.1.1] end//added in liquidMaya r772   end

						MPlug shadowCamPlug = fnLightShaderNode.findPlug( "shadowCameras", &stat );
						// find the multi message attribute...
						if( stat == MS::kSuccess ) 
						{
							int numShadowCams = shadowCamPlug.numElements();
							//cout <<">> got "<<numShadowCams<<" shadow cameras"<<endl;
							// iterate through existing array elements
							for ( unsigned int i=0; i<numShadowCams; i++ ) 
							{
								stat.clear();
								MPlug camPlug = shadowCamPlug.elementByPhysicalIndex( i, &stat );
								if( stat != MS::kSuccess ) 
									continue;
								MPlugArray shadowCamPlugArray;

								// if the element is connected, keep going...
								if( camPlug.connectedTo( shadowCamPlugArray, true, false ) ) 
								{
									MFnDependencyNode shadowCamDepNode = shadowCamPlugArray[0].node();
									//cout <<"shadow camera plug "<<i<<" is connected to "<<shadowCamDepNode.name()<<endl;

									MDagPath cameraPath;
									cameraPath.getAPathTo( shadowCamPlugArray[0].node(), cameraPath);
									//cout <<"cameraPath : "<<cameraPath.fullPathName()<<endl;
									shadowCamPath.append( cameraPath );
									lightHasShadowCam = true;//2
								}
							}
						}
					}
					thisJob.path = lightPath;
					thisJob.name = fnLightNode.name();
					thisJob.texName = "";
					thisJob.isShadow = true;
					thisJob.isPoint = false;
					thisJob.isShadowPass = false;

					// check to see if the minmax shadow option is used
					thisJob.isMinMaxShadow = false;
					liquidGetPlugValue( fnLightNode, "liquidMinMaxShadow", thisJob.isMinMaxShadow, status ); 
					// check to see if the midpoint shadow option is used
					thisJob.isMidPointShadow = false;
					liquidGetPlugValue( fnLightNode, "useMidDistDmap", thisJob.isMidPointShadow, status ); 
					// in lazy compute mode, we check if the map is already on disk first.
					if( m_lazyCompute && computeShadow ) 
					{
						MString fileName( generateFileName( (fileGenMode) fgm_shadow_tex, thisJob ) );
						if( fileExists( fileName ) ) 
							computeShadow = false;
					}
					//
					// store the main shadow map    *****************************
					//
					if( computeShadow )
						jobList.push_back( thisJob );
					// We have to handle point lights differently as they need 6 shadow maps!
					//[refactor 2.1.1] end
				} 
				else if( lightPath.hasFn(MFn::kPointLight) ) 
				{
					//[refactor 2.1.2] begin
					for ( unsigned dirOn( 0 ); dirOn < 6; dirOn++ ) 
					{
						thisJob.hasShadowCam = false;
						thisJob.path = lightPath;
						thisJob.name = fnLightNode.name();
						thisJob.isShadow = true;
						thisJob.isShadowPass = false;
						thisJob.isPoint = true;
						thisJob.pointDir = ( PointLightDirection )dirOn;

						// check to see if the midpoint shadow option is used
						thisJob.isMidPointShadow = false;
						liquidGetPlugValue( fnLightNode, "useMidDistDmap", thisJob.isMidPointShadow, status );

						bool computeShadow = true;
						MStatus liquidLightShaderStatus;
						MPlug liquidLightShaderNodeConnection( fnLightNode.findPlug( "liquidLightShaderNode", &liquidLightShaderStatus ) );

						if( liquidLightShaderStatus == MS::kSuccess && liquidLightShaderNodeConnection.isConnected() ) 
						{
							// a shader is connected to the light !
							MPlugArray liquidLightShaderNodePlugArray;
							liquidLightShaderNodeConnection.connectedTo( liquidLightShaderNodePlugArray, true, true );
							MFnDependencyNode fnLightShaderNode( liquidLightShaderNodePlugArray[0].node() );

							// has the main shadow been disabled ?
							liquidGetPlugValue( fnLightShaderNode, "generateMainShadow", computeShadow, status );

							// look for shadow cameras...
							MStatus stat;
							MPlug shadowCamPlug( fnLightShaderNode.findPlug( "shadowCameras", &stat ) );

							// find the multi message attribute...
							if( stat == MS::kSuccess ) 
							{
								int numShadowCams = shadowCamPlug.numElements();
								//cout <<">> got "<<numShadowCams<<" shadow cameras"<<endl;
								// iterate through existing array elements
								for ( unsigned i( 0 ); i < numShadowCams; i++ ) 
								{
									stat.clear();
									MPlug camPlug = shadowCamPlug.elementByPhysicalIndex( i, &stat );
									if( stat != MS::kSuccess ) 
										continue;
									MPlugArray shadowCamPlugArray;

									// if the element is connected, keep going...
									if( camPlug.connectedTo( shadowCamPlugArray, true, false ) ) 
									{
										MFnDependencyNode shadowCamDepNode = shadowCamPlugArray[0].node();
										//cout <<"shadow camera plug "<<i<<" is connected to "<<shadowCamDepNode.name()<<endl;

										MDagPath cameraPath;
										cameraPath.getAPathTo( shadowCamPlugArray[0].node(), cameraPath);
										//cout <<"cameraPath : "<<cameraPath.fullPathName()<<endl;
										shadowCamPath.append( cameraPath );
										lightHasShadowCam = true;//3
									}
								}
							}
						}
						if( m_lazyCompute ) {
							MString fileName( generateFileName( ( fileGenMode )fgm_shadow_tex, thisJob ) );
							if( fileExists( fileName ) ) 
								computeShadow = false;
						}
						if( computeShadow ) 
							jobList.push_back( thisJob );
					}
				}
				//[refactor 2.1.2] end

				//[refactor][2.1.3 begin] to buildShadowJob()
				// if the job has shadow cameras, we will store them here
				//
				if( lightHasShadowCam )
				{
					int isAggregate = thisJob.shadowAggregation;
					for( unsigned i( 0 ); i < shadowCamPath.length(); i++ )
					{
						if( !i && isAggregate )
							thisJob.shadowAggregation = 0;
						else if ( isAggregate )
							thisJob.shadowAggregation = 1;
						else
							thisJob.shadowAggregation = 0;
						thisJob.shadowCamPath = shadowCamPath[ i ];
						thisJob.hasShadowCam = true;

						MFnDependencyNode shadowCamDepNode( shadowCamPath[ i ].node() );
						thisJob.name = shadowCamDepNode.name();
						if( isAggregate )
							thisJob.texName = fnLightNode.name(); //MFnDependencyNode( shadowCamPath[ i ].node() ).name();
						//					else
						//						thisJob.texName = "";
						//						thisJob.name = shadowCamDepNode.name();
						if( liquidGetPlugValue( shadowCamDepNode, "liqShadowResolution", thisJob.width, status ) == MS::kSuccess )
							thisJob.height = thisJob.width;
						liquidGetPlugValue( shadowCamDepNode, "liqMidPointShadow", thisJob.isMidPointShadow, status );
						thisJob.midPointRatio = 0;
						liquidGetPlugValue( shadowCamDepNode, "liqMidPointRatio", thisJob.midPointRatio, status );
						liquidGetPlugValue( shadowCamDepNode, "liqDeepShadows", thisJob.deepShadows, status );
						liquidGetPlugValue( shadowCamDepNode, "liqPixelSamples", thisJob.shadowPixelSamples, status );
						liquidGetPlugValue( shadowCamDepNode, "liqVolumeInterpretation", thisJob.shadowVolumeInterpretation, status );
            int displayImageMode = 0; // 0 = default
            liquidGetPlugValue( shadowCamDepNode, "liqDeepShadowsDisplayMode", displayImageMode, status );
            if ( displayImageMode ) thisJob.imageMode = MString( "deepprevdisttotal" );
						liquidGetPlugValue( shadowCamDepNode, "liqEveryFrame", thisJob.everyFrame, status );
						// as previously : this is important as thisJob.renderFrame corresponds to the
						// scene scanning time.
						if( thisJob.everyFrame )
							thisJob.renderFrame = liqglo.liqglo_lframe;
						else
							liquidGetPlugValue( shadowCamDepNode, "liqRenderAtFrame", thisJob.renderFrame, status );
						liquidGetPlugValue( shadowCamDepNode, "liqGeometrySet", thisJob.shadowObjectSet, status );
						liquidGetPlugValue( shadowCamDepNode, "liqShadingRateFactor", thisJob.shadingRateFactor, status );
						// test if the file is already on disk...
						if( m_lazyCompute )
						{
							MString fileName( generateFileName( ( fileGenMode )fgm_shadow_tex, thisJob ) );
							if( fileExists( fileName ) )
								continue;
						}
						jobList.push_back( thisJob );
					}
					//[refactor][2.1.3 end] to buildShadowJob()
				}
			} // useDepthMap
			//cout <<thisJob.name.asChar()<<" -> shd:"<<thisJob.isShadow<<" ef:"<<thisJob.everyFrame<<" raf:"<<thisJob.renderFrame<<" set:"<<thisJob.shadowObjectSet.asChar()<<endl;
		} // light dagIterator

		//[refactor][2.1.4 begin] to buildShadowCameraJob()
		MDagPath cameraPath;
		MItDag dagCameraIterator( MItDag::kDepthFirst, MFn::kCamera, &returnStatus );
		for ( ; !dagCameraIterator.isDone(); dagCameraIterator.next() ) 
		{
			if( !dagCameraIterator.getPath(cameraPath) ) 
				continue;
			bool usesDepthMap( false );
			MFnCamera fnCameraNode( cameraPath );
			liquidGetPlugValue( fnCameraNode, "useDepthMapShadows", usesDepthMap, status );
			if( usesDepthMap && areObjectAndParentsVisible( cameraPath ) ) 
			{
				//
				// We have a shadow job, so find out if we need to use deep shadows,
				// and the pixel sample count
				//
				thisJob.deepShadows = false;
				thisJob.shadowPixelSamples = 1;
				thisJob.shadowVolumeInterpretation = 1;
				fnCameraNode.findPlug( "deepShadows" ).getValue( thisJob.deepShadows );
				// Only use the pixel samples and volume interpretation with deep shadows.
				//
				if( thisJob.deepShadows )
				{
					fnCameraNode.findPlug( "pixelSamples" ).getValue( thisJob.shadowPixelSamples );
					fnCameraNode.findPlug( "volumeInterpretation" ).getValue( thisJob.shadowVolumeInterpretation );
					
					  thisJob.imageMode    = liqglo.liquidRenderer.dshImageMode;        //"deepopacity";
					  thisJob.format       = liqglo.liquidRenderer.dshDisplayName;    //"deepshad";

					  int displayImageMode = 0; // 0 = default
					  fnCameraNode.findPlug( "liqDeepShadowsDisplayMode" ).getValue( displayImageMode );
					  if ( displayImageMode )
						  thisJob.imageMode = MString("deepprevdisttotal");
				}

				thisJob.hasShadowCam = true;
				thisJob.shadowCamPath = cameraPath;
				thisJob.path = cameraPath;
				thisJob.name = fnCameraNode.name();
				thisJob.isShadow = true;
				thisJob.isPoint = false;
				thisJob.isShadowPass = false;

				// check to see if the minmax shadow option is used
				thisJob.isMinMaxShadow = false;
				status.clear();
				MPlug liquidMinMaxShadow = fnCameraNode.findPlug( "liquidMinMaxShadow", &status );
				if( status == MS::kSuccess ) 
					liquidMinMaxShadow.getValue( thisJob.isMinMaxShadow );

				// check to see if the midpoint shadow option is used
				thisJob.isMidPointShadow = false;
				status.clear();
				MPlug liquidMidPointShadow = fnCameraNode.findPlug( "useMidDistDmap", &status );
				if( status == MS::kSuccess ) 
					liquidMidPointShadow.getValue( thisJob.isMidPointShadow );

				bool computeShadow( true );
				if( m_lazyCompute ) 
				{
					MString fileName( generateFileName( ( fileGenMode )fgm_shadow_tex, thisJob ) );
					if( fileExists( fileName ) ) continue;
				}

				if( computeShadow ) 
					jobList.push_back( thisJob );
			}
			//[refactor][2.1.4 end] to buildShadowCameraJob()
		} // camera dagIterator
		//[refactor 2] end
	} // liqglo_doShadows

	//[refactor 3] begin to tRibCameraMgr::gatherDataForJob()
	// Determine which cameras to render
	// it will either traverse the dag and find all the renderable cameras or
	// grab the current view and render that as a camera - both get added to the
	// end of the renderable camera array
	MFnCamera fnCameraNode( m_camDagPath );
	thisJob.renderFrame   = liqglo.liqglo_lframe;
	thisJob.everyFrame    = true;
	thisJob.isPoint       = false;
	thisJob.path          = m_camDagPath;
	thisJob.isShadow      = false;
	thisJob.skip          = false;

	if( m_outputShadowPass )
	{ 
		thisJob.name          = fnCameraNode.name() + "SHADOWPASS";
		thisJob.isShadowPass  = true;
		jobList.push_back( thisJob );
	}
	if( m_outputHeroPass ) 
	{
		thisJob.name          = fnCameraNode.name();
		thisJob.isShadowPass  = false;
		jobList.push_back( thisJob );
	}

	liqglo.liqglo_shutterTime    = fnCameraNode.shutterAngle() * 0.5 / M_PI;
	//[refactor 3] end to tRibCameraMgr::gatherDataForJob()

	
	//[refactor 4] begin to buildJobs__()
	// If we didn't find a renderable camera then give up
	if( jobList.size() == 0 ) 
	{
		MString cError("No Renderable Camera Found!\n");
		throw( cError );
		return MS::kFailure;
	}

	// step through the jobs and setup their names
	vector<structJob>::iterator iter = jobList.begin();
	while ( iter != jobList.end() )
	{
		LIQ_CHECK_CANCEL_REQUEST;
		thisJob = *iter;

		MString frameFileName;
		frameFileName = generateFileName( ( thisJob.isShadow )? fgm_shadow_rib : fgm_beauty_rib, thisJob );

		iter->ribFileName = frameFileName;

		// set the skip flag for the job
		iter->skip   = false;
		thisJob.skip = false;

		if( thisJob.isShadow )
		{
			if( !liqglo.liqglo_doShadows )
			{
				// shadow generation disabled
				iter->skip   = true;
				thisJob.skip = true;
			}
			else if( !thisJob.everyFrame && ( liqglo.liqglo_noSingleFrameShadows || liqglo.liqglo_lframe > liqglo.frameNumbers[ 0 ] && thisJob.renderFrame != liqglo.liqglo_lframe ) )
			{
				// noSingleFrameShadows or rendering past the first frame of the sequence
				iter->skip   = true;
				thisJob.skip = true;
			}
			else if( thisJob.everyFrame && liqglo.liqglo_singleFrameShadowsOnly )
			{
				// singleFrameShadowsOnly on regular shadows
				iter->skip   = true;
				thisJob.skip = true;
			}
		}
		else if( liqglo.liqglo_singleFrameShadowsOnly )
		{
			// singleFrameShadowsOnly on hero pass
			iter->skip   = true;
			thisJob.skip = true;
		}

		MString outFileFmtString;

		if( thisJob.isShadow ) 
		{
			MString varVal;
			MString userShadowName;
			MFnDagNode lightNode( thisJob.path );

			if( liquidGetPlugValue( lightNode, "liquidShadowName", varVal, status ) == MS::kSuccess ) 
				userShadowName = parseString( varVal, false );
			//outFileFmtString = liqglo_textureDir;

			MString outName = ( userShadowName.length() ) ? userShadowName 
				: generateFileName( ( fileGenMode )fgm_shadow_tex, thisJob );

			iter->imageName = outName;
			thisJob = *iter;
			if( thisJob.isShadow ) 
				shadowList.push_back( thisJob );
		} 
		else 
		{
			MString outName;
			outName = generateFileName( ( fileGenMode )fgm_image, thisJob );
			iter->imageName = outName;
		}
		++iter;
	}

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
	//[refactor 4] end to buildJobs__()
}

/**
* Write the prologue for the RIB file.
* This includes all RI options but not the camera transformation.
*/
MStatus liqRibTranslator::ribPrologue()
{
	CM_TRACE_FUNC("liqRibTranslator::ribPrologue()");

	if( !liqglo.m_exportReadArchive ) 
	{
		LIQDEBUGPRINTF( "-> beginning to write prologue\n" );

		//[refactor 5] begin 
		// general info for traceability
		//
		RiArchiveRecord( RI_COMMENT, "    Generated by Liquid v%s", LIQUIDVERSION );
		RiArchiveRecord( RI_COMMENT, "    Scene : %s", (liqglo.liqglo_projectDir + liquidTransGetSceneName()).asChar() );
#ifndef _WIN32
		uid_t userId = getuid();
		struct passwd *userPwd = getpwuid( userId );
		RiArchiveRecord( RI_COMMENT, "    User  : %s", userPwd->pw_name );
#else
		char* user = getenv("USERNAME");
		if( user )
			RiArchiveRecord( RI_COMMENT, "    User  : %s", user );
#endif
		time_t now;
		time( &now );
		char* theTime = ctime(&now);
		RiArchiveRecord( RI_COMMENT, "    Time  : %s", theTime );
		//[refactor 5] end

		// set any rib options
		//
		writeStatisticsOptions();

		//limits options
		writeLimitsOptions();

		// set search paths
		//
		//[refactor 6] begin to rm::Renderer::ribPrologue_writeSearthPath ()
		if ( liqglo.m_dirmaps.length() )
		{
			using namespace std;
			using namespace boost;

			const string str( liqglo.m_dirmaps.asChar() );
			stringstream ss;
			vector< string > names;
			typedef tokenizer< char_separator< char > > tokenizer;
			char_separator< char > sep("{ }");
			tokenizer tokens(str, sep);
			for ( tokenizer::iterator tok_iter = tokens.begin(); tok_iter != tokens.end(); ++tok_iter )
				names.push_back( *tok_iter );
			for ( unsigned i( 0 ); i < names.size(); i+= 3 )
			{
				// names.at(i+2)  - zone
				// names.at(i)    - from
				// names.at(i+1)  - to
				// [\"UNC\" \"/from_path/\" \"//comp/to_path/\"]
				ss << "[\\\"" << names.at(i+2) << "\\\" \\\"" << names.at(i) << "\\\" \\\"" << names.at(i+1) << "\\\"] ";
			}
			printf("%s\n", ss.str().c_str() );
			string dirmapsPath ( ss.str() );
			RtString list = const_cast< char* > ( dirmapsPath.c_str() );
			RiOption( "searchpath", "dirmap", &list, RI_NULL );
		}
		RtString list = const_cast< char* > ( liqglo.liqglo_shaderPath.asChar() );
		RiOption( "searchpath", "shader", &list, RI_NULL );

		MString texturePath = liqglo.liqglo_texturePath + ":" + liquidSanitizeSearchPath( liqglo.liqglo_textureDir );
		list = const_cast< char* > ( texturePath.asChar() );
		RiOption( "searchpath", "texture", &list, RI_NULL );

		MString archivePath = liqglo.liqglo_archivePath + ":" + liquidSanitizeSearchPath( liqglo.liqglo_ribDir );
		list = const_cast< char* > ( archivePath.asChar() );
		RiOption( "searchpath", "archive", &list, RI_NULL );

		list = const_cast< char* > ( liqglo.liqglo_proceduralPath.asChar() );
		RiOption( "searchpath", "procedural", &list, RI_NULL );

		// if rendering to the renderview, add a path to the liqmaya display driver
		if( liqglo.m_renderView ) 
		{
			MString home( getenv( "LIQUIDHOME" ) );
			MString displaySearchPath;
			if( (liqglo.liquidRenderer.renderName == MString("Pixie")) || (liqglo.liquidRenderer.renderName == MString("Air")) || (liqglo.liquidRenderer.renderName == MString("3Delight")) )
				displaySearchPath = ".:@::" + liqglo.liquidRenderer.renderHome + "/displays:" + liquidSanitizePath( home ) + "/displayDrivers/" + liqglo.liquidRenderer.renderName + "/";
			else 
				displaySearchPath = ".:@:" + liqglo.liquidRenderer.renderHome + "/etc:" + liquidSanitizePath( home ) +  "/displayDrivers/" + liqglo.liquidRenderer.renderName + "/";

			list = const_cast< char* > ( displaySearchPath.asChar() );
			RiArchiveRecord( RI_VERBATIM, "Option \"searchpath\" \"display\" [\"%s\"]\n", list );
		}
		//[refactor 6] end to rm::Renderer::ribPrologue_writeSearthPath ()

		//RiOrientation( RI_RH ); // Right-hand coordinates
		if( liqglo_currentJob.isShadow ) 
		{
			//[refactor 7] 
			RiPixelSamples( liqglo_currentJob.shadowPixelSamples, liqglo_currentJob.shadowPixelSamples );
			RiShadingRate( liqglo_currentJob.shadingRateFactor );
			// Need to use Box filter for deep shadows.
			RiPixelFilter( RiBoxFilter, 1, 1 );
			RtString option;
			if( liqglo_currentJob.deepShadows ) 
				option = "deepshadow";
			else 
				option = "shadow";
			RiOption( "user", "string pass", ( RtPointer )&option, RI_NULL );
			//[refactor 7] 
		} 
		else 
		{
						//[refactor 8] 
			RtString hiderName;
			switch( liqglo.liqglo_hider ) 
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

			RiPixelSamples( liqglo.pixelSamples, liqglo.pixelSamples );

			RiShadingRate( liqglo.shadingRate );

			if( liqglo.m_rFilterX > 1 || liqglo.m_rFilterY > 1 ) 
			{
				switch (liqglo.m_rFilter) 
				{
				case pfBoxFilter:
					RiPixelFilter( RiBoxFilter, liqglo.m_rFilterX, liqglo.m_rFilterY );
					break;
				case pfTriangleFilter:
					RiPixelFilter( RiTriangleFilter, liqglo.m_rFilterX, liqglo.m_rFilterY );
					break;
				case pfCatmullRomFilter:
					RiPixelFilter( RiCatmullRomFilter, liqglo.m_rFilterX, liqglo.m_rFilterY );
					break;
				case pfGaussianFilter:
					RiPixelFilter( RiGaussianFilter, liqglo.m_rFilterX, liqglo.m_rFilterY );
					break;
				case pfSincFilter:
					RiPixelFilter( RiSincFilter, liqglo.m_rFilterX, liqglo.m_rFilterY );
					break;
#if defined ( DELIGHT ) || defined ( PRMAN ) || defined ( GENERIC_RIBLIB )
				case pfBlackmanHarrisFilter:
					RiArchiveRecord( RI_VERBATIM, "PixelFilter \"blackman-harris\" %g %g\n", liqglo.m_rFilterX, liqglo.m_rFilterY);
					break;
				case pfMitchellFilter:
					RiArchiveRecord( RI_VERBATIM, "PixelFilter \"mitchell\" %g %g\n", liqglo.m_rFilterX, liqglo.m_rFilterY);
					break;
				case pfSepCatmullRomFilter:
					RiArchiveRecord( RI_VERBATIM, "PixelFilter \"separable-catmull-rom\" %g %g\n", liqglo.m_rFilterX, liqglo.m_rFilterY);
					break;
				case pfBesselFilter:
					RiArchiveRecord( RI_VERBATIM, "PixelFilter \"bessel\" %g %g\n", liqglo.m_rFilterX, liqglo.m_rFilterY);
					break;
#endif
#if defined ( PRMAN ) || defined ( GENERIC_RIBLIB )
				case pfLanczosFilter:
					RiArchiveRecord( RI_VERBATIM, "PixelFilter \"lanczos\" %g %g\n", liqglo.m_rFilterX, liqglo.m_rFilterY);
					break;
				case pfDiskFilter:
					RiArchiveRecord( RI_VERBATIM, "PixelFilter \"disk\" %g %g\n", liqglo.m_rFilterX, liqglo.m_rFilterY);
					break;
#endif
				default:
					RiArchiveRecord( RI_COMMENT, "Unknown Pixel Filter selected" );
					break;
				}
			}
			//
			RtString option( "beauty" );
			RiOption( "user", "string pass", ( RtPointer )&option, RI_NULL );
		}

		// RAYTRACING OPTIONS
		if( liqglo.liquidRenderer.supports_RAYTRACE && liqglo.rt_useRayTracing ) 
		{
			RiArchiveRecord( RI_COMMENT, "Ray Tracing : ON" );
			RiOption( "trace",   "int maxdepth",                ( RtPointer ) &liqglo.rt_traceMaxDepth,            RI_NULL );
#if defined ( DELIGHT ) || defined ( PRMAN ) || defined ( GENERIC_RIBLIB )
			RiOption( "trace",   "float specularthreshold",     ( RtPointer ) &liqglo.rt_traceSpecularThreshold, RI_NULL );
			RiOption( "trace",   "int continuationbydefault",   ( RtPointer ) &liqglo.rt_traceRayContinuation,   RI_NULL );
			RiOption( "limits",  "int geocachememory",          ( RtPointer ) &liqglo.rt_traceCacheMemory,       RI_NULL );
			RiOption( "user",    "float traceBreadthFactor",    ( RtPointer ) &liqglo.rt_traceBreadthFactor,     RI_NULL );
			RiOption( "user",    "float traceDepthFactor",      ( RtPointer ) &liqglo.rt_traceDepthFactor,       RI_NULL );
#endif
		} 
		else 
		{
			if( !liqglo.liquidRenderer.supports_RAYTRACE ) 
				RiArchiveRecord( RI_COMMENT, "Ray Tracing : NOT SUPPORTED" );
			else 
			{
				RiArchiveRecord( RI_COMMENT, "Ray Tracing : OFF" );
				RtInt maxDepth = 0;
				RiOption( "trace",   "int maxdepth",                ( RtPointer ) &maxDepth,                  RI_NULL );
			}
		}

		// CUSTOM OPTIONS
		MFnDependencyNode globalsNode( liqglo.rGlobalObj );
		MPlug prePostplug( globalsNode.findPlug( "preFrameBeginMel" ) );
		MString melcommand( prePostplug.asString() );
		if( liqglo.m_preFrameRIB != "" || melcommand.length() )
		{
			RiArchiveRecord(RI_COMMENT,  " Pre-FrameBegin RIB from liquid globals" );
			MGlobal::executeCommand( melcommand );
			RiArchiveRecord(RI_VERBATIM, ( char* )liqglo.m_preFrameRIB.asChar() );
			RiArchiveRecord(RI_VERBATIM, "\n");
		}
		if( liqglo.m_bakeNonRasterOrient || liqglo.m_bakeNoCullHidden || liqglo.m_bakeNoCullBackface ) 
		{
			RiArchiveRecord( RI_COMMENT, "Bake Attributes" );
			RtInt zero( 0 );
			if( liqglo.m_bakeNonRasterOrient )
				RiAttribute( "dice","int rasterorient", &zero, RI_NULL );
			if( liqglo.m_bakeNoCullBackface )
				RiAttribute( "cull","int backfacing", &zero, RI_NULL );
			if( liqglo.m_bakeNoCullHidden )
				RiAttribute( "cull","int hidden", &zero, RI_NULL );
		}
	}
	ribStatus = kRibBegin;
	return MS::kSuccess;
}

/**
* Write the epilogue for the RIB file.
*/
MStatus liqRibTranslator::ribEpilogue()
{
	CM_TRACE_FUNC("liqRibTranslator::ribEpilogue()");

	if(ribStatus == kRibBegin) 
		ribStatus = kRibOK;
	return (ribStatus == kRibOK ? MS::kSuccess : MS::kFailure);
}

/**
* Scan the DAG at the given frame number and record information about the scene for writing.
*/
MStatus liqRibTranslator::scanSceneNodes( MObject &currentNode, MDagPath &path, float lframe, int sample, int &count, MStatus& returnStatus ) 
{
	CM_TRACE_FUNC("liqRibTranslator::scanSceneNodes(&currentNode, &path,"<<lframe<<","<<sample<<",&count)");

	MFnDagNode dagNode;
	returnStatus = dagNode.setObject( currentNode );
	if(MS::kSuccess != returnStatus)
		return returnStatus;

	LIQ_CHECK_CANCEL_REQUEST;
	// scanScene: check for a rib generator
	MStatus plugStatus;
	MPlug ribGenPlug = dagNode.findPlug( "liquidRibGen", &plugStatus );
	if( plugStatus == MS::kSuccess )
	{
		// scanScene: check the node to make sure it's not using the old ribGen assignment method, this is for backwards
		// compatibility.  If it's a kTypedAttribute that it's more than likely going to be a string!
		if( ribGenPlug.attribute().apiType() == MFn::kTypedAttribute )
		{
			MString ribGenNode;
			ribGenPlug.getValue( ribGenNode );
			MSelectionList ribGenList;
			MStatus ribGenAddStatus = ribGenList.add( ribGenNode );
			if( ribGenAddStatus == MS::kSuccess )
				htable->insert( path, lframe, sample, MRT_RibGen, count++ );
		}
		else
		{
			if( ribGenPlug.isConnected() )
				htable->insert( path, lframe, sample, MRT_RibGen, count++ );
		}

	}
// 	liquidMessage2(messageInfo,"%s, %s,\t\t\t\t %d%d%d %d%d %d%d %d%d%d%d"
// 		,path.fullPathName().asChar(), currentNode.apiTypeStr()
// 		,currentNode.hasFn( MFn::kNurbsSurface )
// 		,currentNode.hasFn( MFn::kMesh )
// 		,currentNode.hasFn( MFn::kParticle )
// 
// 		,currentNode.hasFn( MFn::kLocator )
// 		,currentNode.hasFn( MFn::kSubdiv )
// 
// 		,currentNode.hasFn( MFn::kPfxHair )
// 		,currentNode.hasFn( MFn::kPfxGeometry )
// 
// 		,currentNode.hasFn( MFn::kPfxToon )
// 		,currentNode.hasFn( MFn::kImplicitSphere )
// 		,currentNode.hasFn( MFn::kPluginShape )
// 		,currentNode.hasFn( MFn::kNurbsCurve ) 
	
	if(  currentNode.hasFn( MFn::kNurbsSurface )
		|| currentNode.hasFn( MFn::kMesh )
		|| currentNode.hasFn( MFn::kParticle )
		|| currentNode.hasFn( MFn::kNParticle)
		|| currentNode.hasFn( MFn::kLocator )
		|| currentNode.hasFn( MFn::kSubdiv )
		|| currentNode.hasFn( MFn::kPfxHair )
		|| currentNode.hasFn( MFn::kPfxToon )
		|| currentNode.hasFn( MFn::kImplicitSphere )
		|| currentNode.hasFn( MFn::kPluginShape ) ) // include plugin shapes as placeholders
	{
		LIQDEBUGPRINTF( "==> inserting obj to htable %s\n", path.fullPathName().asChar() );
		if( ( sample > 0 ) && isObjectMotionBlur( path ))
			htable->insert( path, lframe, sample, MRT_Unknown, count++ );
		else
			htable->insert( path, lframe, 0, MRT_Unknown, count++ );
		LIQDEBUGPRINTF( "==> %s inserted\n", path.fullPathName().asChar() );
	}
	// Alf: treat PFX as three separate entities so a separate shading group can
	// be assigned to each part
	if( currentNode.hasFn( MFn::kPfxGeometry ) )
	{
		LIQDEBUGPRINTF( "==> inserting kPfxGeometry\n" );

		MStatus status;
		MRenderLineArray tube, leaf, petal;
		MFnPfxGeometry pfx( path, &status );
		if( status == MS::kSuccess )
			pfx.getLineData( tube, leaf, petal,1,0,0,0,0,0,0,0,0 );

		if( tube.length() )
		{
			if( ( sample > 0 ) && isObjectMotionBlur( path ))
				htable->insert( path, lframe, sample, MRT_PfxTube, count++ );
			else
				htable->insert( path, lframe, 0, MRT_PfxTube, count++ );
		}
		if( leaf.length() )
		{
			if( ( sample > 0 ) && isObjectMotionBlur( path ))
				htable->insert( path, lframe, sample, MRT_PfxLeaf, count++ );
			else
				htable->insert( path, lframe, 0, MRT_PfxLeaf, count++ );
		}
		if( petal.length() )
		{
			if( ( sample > 0 ) && isObjectMotionBlur( path ))
				htable->insert( path, lframe, sample, MRT_PfxPetal, count++ );
			else
				htable->insert( path, lframe, 0, MRT_PfxPetal, count++ );
		}
		tube.deleteArray(); 
		leaf.deleteArray(); 
		petal.deleteArray();
	}
	if( currentNode.hasFn( MFn::kNurbsCurve ) )
	{
		LIQDEBUGPRINTF( "==> inserting kNurbsCurve\n" );

		MStatus plugStatus;
		MPlug renderCurvePlug = dagNode.findPlug( "liquidCurve", &plugStatus );
		if( liqglo.liqglo_renderAllCurves && ( plugStatus == MS::kSuccess ) )// ymesh use ||, r773 use &&
		{
			bool renderCurve( false );
			renderCurvePlug.getValue( renderCurve );
			if( renderCurve )
			{
				if( ( sample > 0 ) && isObjectMotionBlur( path ))
					htable->insert( path, lframe, sample, MRT_NuCurve, count++ );
				else
					htable->insert( path, lframe, 0, MRT_NuCurve, count++ );
			}
		}
	}
	return returnStatus;
}


/**
* Scan the DAG at the given frame number and record information about the scene for writing.
*/
MStatus liqRibTranslator::scanScene(float lframe, int sample )
{
	CM_TRACE_FUNC("liqRibTranslator::scanScene("<<lframe<<","<<sample<<")");

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
				if( MS::kSuccess == MGlobal::executeCommand( preFrameMel, false, false ) ) 
					liquidMessage("Liquid -> pre-frame script executed successfully.", messageInfo);
				else 
					liquidMessage("Liquid -> ERROR :pre-frame script failed.", messageError);
			}
		}

		MStatus returnStatus;
		// scanScene: Scan the scene for lights
		{
			//[refactor 9] begin to tLightMgr::scanScene()
			MItDag dagLightIterator( MItDag::kDepthFirst, MFn::kLight, &returnStatus);

			for (; !dagLightIterator.isDone(); dagLightIterator.next()) 
			{
				LIQ_CHECK_CANCEL_REQUEST;
				MDagPath path;
				MObject currentNode;
				currentNode = dagLightIterator.item();
				MFnDagNode dagNode;
				dagLightIterator.getPath( path );
				if(MS::kSuccess != returnStatus) 
					continue;
				if(!currentNode.hasFn(MFn::kDagNode)) 
					continue;
				returnStatus = dagNode.setObject( currentNode );
				if(MS::kSuccess != returnStatus) 
					continue;

				// scanScene: if it's a light then insert it into the hash table
				if( currentNode.hasFn( MFn::kLight ) ) 
				{
					if( currentNode.hasFn( MFn::kAreaLight ) ) 
					{
						// add a coordSys node if necessary
						MStatus status;
						bool coordsysExists = false;
						// get the coordsys name
						MFnDependencyNode areaLightDep( currentNode );
						MString coordsysName = areaLightDep.name()+"CoordSys";
						// get the transform
						MObject transform = path.transform();
						// check the coordsys does not exist yet under the transform
						MFnDagNode transformDag( transform );
						int numChildren = transformDag.childCount();
						if( numChildren > 1 ) 
						{
							for ( unsigned int i=0; i<numChildren; i++ ) 
							{
								MObject childObj = transformDag.child( i, &status );
								if( status == MS::kSuccess && childObj.hasFn( MFn::kLocator ) ) 
								{
									MFnDependencyNode test(childObj);
									if( test.name() == coordsysName ) 
										coordsysExists = true;
								}
							}
						}
						if( !coordsysExists ) 
						{
							// create the coordsys
							MDagModifier coordsysNode;
							MObject coordsysObj  = coordsysNode.createNode( "liquidCoordSys", transform, &status );
							if( status == MS::kSuccess ) 
							{
								// rename node to match light name
								coordsysNode.doIt();
								if( status == MS::kSuccess ) 
								{
									MFnDependencyNode coordsysDep( coordsysObj );
									coordsysDep.setName( coordsysName );
								}
							}
						}
					}

					if( ( sample > 0 ) && isObjectMotionBlur( path )) 
						htable->insert( path, lframe, sample, MRT_Light,	count++ );
					else 
						htable->insert( path, lframe, 0, MRT_Light, count++ );
					continue;
				}
			}
		}
		//[refactor 9] end to tLightMgr::scanScene()

		//[refactor 10] beg to tLocatorMgr::scanScene()
		{
			MItDag dagCoordSysIterator( MItDag::kDepthFirst, MFn::kLocator, &returnStatus);

			for (; !dagCoordSysIterator.isDone(); dagCoordSysIterator.next()) 
			{
				LIQ_CHECK_CANCEL_REQUEST;
				MDagPath path;
				MObject currentNode;
				currentNode = dagCoordSysIterator.item();
				MFnDagNode dagNode;
				dagCoordSysIterator.getPath( path );
				if(MS::kSuccess != returnStatus) 
					continue;
				if(!currentNode.hasFn(MFn::kDagNode)) 
					continue;
				returnStatus = dagNode.setObject( currentNode );
				if(MS::kSuccess != returnStatus) 
					continue;

				// scanScene: if it's a coordinate system then insert it into the hash table
				if( dagNode.typeName() == "liquidCoordSys" ) 
				{
					int coordType = 0;
					MPlug typePlug = dagNode.findPlug( "type", &returnStatus );
					if( MS::kSuccess == returnStatus ) 
						typePlug.getValue( coordType );

					if( ( sample > 0 ) && isObjectMotionBlur( path )) 
					{
						// philippe : should I store a motion-blurred clipping plane ?
						if( coordType == 5 ) 
							htable->insert(path, lframe, sample, MRT_ClipPlane,count++ );
						else 
							htable->insert(path, lframe, sample, MRT_Coord,count++ );
					} 
					else 
					{
						if( coordType == 5 ) 
							htable->insert(path, lframe, 0, MRT_ClipPlane,count++ );
						htable->insert(path, lframe, 0, MRT_Coord,count++ );
					}
					continue;
				}
			}
		}	//[refactor 10] end to tLocatorMgr::scanScene()
	
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
			//[refactor 11.1] begin
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
				if( ( sample > 0 ) && isObjectMotionBlur( path ) )
					htable->insert( path, lframe, sample, MRT_Unknown,count++, &instanceMatrix, instanceStr, instancerIter.particleId() );
				else
					htable->insert( path, lframe, 0, MRT_Unknown,count++, &instanceMatrix, instanceStr, instancerIter.particleId() );
				instancerIter.next();
			}
			//[refactor 11.1] end

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
			MItDag dagIterator( MItDag::kDepthFirst, MFn::kInvalid, &returnStatus);
			for( ; !selIterator.isDone(); selIterator.next() )
			{
				MDagPath objectPath;
				selIterator.getDagPath( objectPath );
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
			//[refactor 11.1] begin
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

				if( ( sample > 0 ) && isObjectMotionBlur( path ))
					htable->insert( path, lframe, sample, MRT_Unknown,count++, &instanceMatrix, instanceStr, instancerIter.particleId() );
				else
					htable->insert( path, lframe, 0, MRT_Unknown,count++, &instanceMatrix, instanceStr, instancerIter.particleId() );
				instancerIter.next();
			}
			//[refactor 11.1] end
		}

		vector<structJob>::iterator iter = jobList.begin();
		while ( iter != jobList.end() ) 
		{
			LIQ_CHECK_CANCEL_REQUEST;
			// scanScene: Get the camera/light info for this job at this frame
			MStatus status;

			if( !iter->isShadow ) 
			{
				//[refactor 12] begin to liqRibTranslator::getCameraData()
				MDagPath path;
				MFnCamera   fnCamera( iter->path );
				iter->gotJobOptions = false;
				status.clear();
				MPlug cPlug = fnCamera.findPlug( MString( "ribOptions" ), &status );
				if( status == MS::kSuccess ) 
				{
					cPlug.getValue( iter->jobOptions );
					iter->gotJobOptions = true;
				}
				getCameraInfo( fnCamera, iter->camera[sample] );
				iter->width = iter->camera[sample].width;
				iter->height = iter->camera[sample].height;
				// scanScene: Renderman specifies shutter by time open
				// so we need to convert shutterAngle to time.
				// To do this convert shutterAngle to degrees and
				// divide by 360.
				//
				iter->camera[sample].shutter = fnCamera.shutterAngle() * 0.5 / M_PI;
				liqglo.liqglo_shutterTime = iter->camera[sample].shutter;
				iter->camera[sample].motionBlur     = fnCamera.isMotionBlur();				

				// scanScene: The camera's fov may not match the rendered image in Maya
				// if a film-fit is used. 'fov_ratio' is used to account for
				// this.
				//
				iter->camera[sample].hFOV = fnCamera.horizontalFieldOfView()/iter->camera[sample].fov_ratio;

				if ( fnCamera.isClippingPlanes() )
				{
					iter->camera[sample].neardb    = fnCamera.nearClippingPlane();
					iter->camera[sample].fardb    = fnCamera.farClippingPlane();
				}
				else
				{
					iter->camera[sample].neardb    = 0.001;    // TODO: these values are duplicated elsewhere in this file
					iter->camera[sample].fardb    = 250000.0; // TODO: these values are duplicated elsewhere in this file
				}

				iter->camera[sample].orthoWidth     = fnCamera.orthoWidth();
				iter->camera[sample].orthoHeight    = fnCamera.orthoWidth() * ((float)iter->camera[sample].height / (float)iter->camera[sample].width);

				iter->camera[sample].focalLength    = fnCamera.focalLength();
				iter->camera[sample].focalDistance  = fnCamera.focusDistance();
				iter->camera[sample].fStop          = fnCamera.fStop();
				iter->camera[sample].isOrtho		= fnCamera.isOrtho();
				//iter->camera[sample].name           = fnCamera.fullPathName();//r773 ommit this?
				getCameraFilmOffset( fnCamera, iter->camera[sample] );

				// convert focal length to scene units
				MDistance flenDist(iter->camera[sample].focalLength,MDistance::kMillimeters);
				iter->camera[sample].focalLength = flenDist.as(MDistance::uiUnit());
				getCameraTransform( fnCamera, iter->camera[sample] );
				//////////////////////////////////////////////////////////////////////////
				//[refactor 12.1] begin to liqRibTranslator::getCameraData()
				// check stereo
				MString camType = fnCamera.typeName();
				bool isStereoCamera = false;
				if( camType == "stereoRigCamera" )
				{
					isStereoCamera = true;
					structCamera centralCameraPath = iter->camera[sample];
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

					getCameraInfo( fnLeftCam, iter->leftCamera[sample] );
					iter->leftCamera[sample].orthoWidth     = fnLeftCam.orthoWidth();
					iter->leftCamera[sample].orthoHeight    = fnLeftCam.orthoWidth() * ((float)iter->camera[sample].height / (float)iter->camera[sample].width);
					iter->leftCamera[sample].focalLength    = fnLeftCam.focalLength();
					iter->leftCamera[sample].focalDistance  = fnLeftCam.focusDistance();
					iter->leftCamera[sample].fStop          = fnLeftCam.fStop();
					iter->leftCamera[sample].isOrtho		= fnLeftCam.isOrtho();
					iter->leftCamera[sample].name			= fnLeftCam.name();
					getCameraFilmOffset( fnLeftCam, iter->leftCamera[sample] );
					// convert focal length to scene units
					MDistance flenLDist(iter->leftCamera[sample].focalLength, MDistance::kMillimeters);
					iter->leftCamera[sample].focalLength = flenLDist.as(MDistance::uiUnit());
					getCameraTransform( fnLeftCam, iter->leftCamera[sample] );
					// scanScene: The camera's fov may not match the rendered image in Maya
					// if a film-fit is used. 'fov_ratio' is used to account for
					// this.
					//
					//iter->leftCamera[sample].hFOV = fnLeftCam.horizontalFieldOfView()/iter->leftCamera[sample].fov_ratio;
					iter->leftCamera[sample].hFOV = iter->camera[sample].hFOV;
					iter->leftCamera[sample].neardb = iter->camera[sample].neardb;
					iter->leftCamera[sample].fardb = iter->camera[sample].fardb;

					getCameraInfo( fnRightCam, iter->rightCamera[sample] );
					iter->rightCamera[sample].orthoWidth	= fnRightCam.orthoWidth();
					iter->rightCamera[sample].orthoHeight	= fnRightCam.orthoWidth() * ((float)iter->camera[sample].height / (float)iter->camera[sample].width);
					iter->rightCamera[sample].focalLength	= fnRightCam.focalLength();
					iter->rightCamera[sample].focalDistance	= fnRightCam.focusDistance();
					iter->rightCamera[sample].fStop			= fnRightCam.fStop();
					iter->rightCamera[sample].isOrtho		= fnRightCam.isOrtho();
					iter->rightCamera[sample].name			= fnRightCam.name();
					getCameraFilmOffset( fnRightCam, iter->rightCamera[sample] );
					// convert focal length to scene units
					MDistance flenRDist(iter->rightCamera[sample].focalLength, MDistance::kMillimeters);
					iter->rightCamera[sample].focalLength = flenRDist.as(MDistance::uiUnit());
					getCameraTransform( fnRightCam, iter->rightCamera[sample] );
					// scanScene: The camera's fov may not match the rendered image in Maya
					// if a film-fit is used. 'fov_ratio' is used to account for
					// this.
					//
					//iter->rightCamera[sample].hFOV = fnRightCam.horizontalFieldOfView()/iter->rightCamera[sample].fov_ratio;
					iter->rightCamera[sample].hFOV = iter->camera[sample].hFOV;
					iter->rightCamera[sample].neardb = iter->camera[sample].neardb;
					iter->rightCamera[sample].fardb = iter->camera[sample].fardb;

					iter->camera[sample].rightCam = &(iter->rightCamera[sample]);
					iter->camera[sample].leftCam = &(iter->leftCamera[sample]);
				}
				iter->isStereoPass = isStereoCamera;
				iter->aspectRatio = liqglo.aspectRatio;
				//[refactor 12.1] end
				//////////////////////////////////////////////////////////////////////////
				
				// scanScene: Determine what information to write out (RGB, alpha, zbuffer)
				//
				iter->imageMode.clear();

				bool isOn;
				MPlug boolPlug;
				boolPlug = fnCamera.findPlug( "image" );

				boolPlug.getValue( isOn );
				if( isOn ) 
				{
					// We are writing RGB info
					//
					iter->imageMode = "rgb";
					iter->format = liqglo.outFormat;
				}
				boolPlug = fnCamera.findPlug( "mask" );
				boolPlug.getValue( isOn );
				if( isOn ) 
				{
					// We are writing alpha channel info
					//
					iter->imageMode += "a";
					iter->format = liqglo.outFormat;
				}
				boolPlug = fnCamera.findPlug( "depth" );
				boolPlug.getValue( isOn );
				if( isOn ) 
				{
					if ( !isStereoCamera  )
					{
						// We are writing z-buffer info
						//
						iter->imageMode = "z";
						iter->format = "zfile";
					}else
						liquidMessage( "Cannot render depth for stereo camera.", messageWarning );
				} // isOn && !isStereoCamera
				//[refactor 12] end
			} 
			else 
			{
				//[refactor 13] begin to liqRibTranslator::getLightData()
				// scanScene: doing shadow render
				//
				MDagPath lightPath;
				MFnLight   fnLight( iter->path );
				status.clear();

				iter->gotJobOptions = false;
				if ( liquidGetPlugValue( fnLight, "ribOptions", iter->jobOptions, status ) ==  MS::kSuccess )
					iter->gotJobOptions = true;  
				iter->isStereoPass = false;
				// philippe: this block is obsolete as we now get the resolution when building the job list
				//
				/* MPlug lightPlug = fnLight.findPlug( "dmapResolution" );
				float dmapSize;
				lightPlug.getValue( dmapSize );
				iter->height = iter->width = (int)dmapSize; */

				if( iter->hasShadowCam ) 
				{
					// scanScene: the light uses a shadow cam
					//
					MFnCamera fnCamera( iter->shadowCamPath );
					fnCamera.getPath(lightPath);
					MTransformationMatrix xform( lightPath.inclusiveMatrix() );
					
					cerr << "dbg> scanScene: the light uses a shadow cam " <<  lightPath.fullPathName() << endl;

					// the camera is pointing toward negative Z
					double scale[] = { 1, 1, -1 };
					xform.setScale( scale, MSpace::kTransform );

					iter->camera[sample].mat         = xform.asMatrixInverse();
					iter->camera[sample].neardb      = fnCamera.nearClippingPlane();
					iter->camera[sample].fardb       = fnCamera.farClippingPlane();
					iter->camera[sample].isOrtho     = fnCamera.isOrtho();
					iter->camera[sample].orthoWidth  = fnCamera.orthoWidth();
					iter->camera[sample].orthoHeight = fnCamera.orthoWidth();
					iter->camera[sample].name        = fnCamera.fullPathName();
				} 
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

					if( iter->isPoint ) 
					{
						double ninty = M_PI/2;
						if( iter->pointDir == pPX ) { double rotation[] = { 0, -ninty, 0 }; xform.setRotation( rotation, MTransformationMatrix::kXYZ ); }
						if( iter->pointDir == pNX ) { double rotation[] = { 0, ninty, 0 }; xform.setRotation( rotation, MTransformationMatrix::kXYZ ); }
						if( iter->pointDir == pPY ) { double rotation[] = { ninty, 0, 0 }; xform.setRotation( rotation, MTransformationMatrix::kXYZ ); }
						if( iter->pointDir == pNY ) { double rotation[] = { -ninty, 0, 0 }; xform.setRotation( rotation, MTransformationMatrix::kXYZ ); }
						if( iter->pointDir == pPZ ) { double rotation[] = { 0, M_PI, 0 }; xform.setRotation( rotation, MTransformationMatrix::kXYZ ); }
						if( iter->pointDir == pNZ ) { double rotation[] = { 0, 0, 0 }; xform.setRotation( rotation, MTransformationMatrix::kXYZ ); }
					}
					iter->camera[sample].mat = xform.asMatrixInverse();

					MPlug shaderConnection( fnLight.findPlug( "liquidLightShaderNode", &status ) );
					if( status == MS::kSuccess && shaderConnection.isConnected() ) 
					{
						MPlugArray LightShaderPlugArray;
						shaderConnection.connectedTo( LightShaderPlugArray, true, true );
						MFnDependencyNode fnLightShaderNode( LightShaderPlugArray[0].node() );
						fnLightShaderNode.findPlug( "nearClipPlane" ).getValue( iter->camera[sample].neardb );
						fnLightShaderNode.findPlug( "farClipPlane" ).getValue( iter->camera[sample].fardb );
					} 
					else 
					{
						iter->camera[sample].neardb   = 0.001;    // TODO: these values are duplicated elsewhere in this file
						iter->camera[sample].fardb    = 250000.0; // TODO: these values are duplicated elsewhere in this file
						liquidGetPlugValue( fnLight, "nearClipPlane", iter->camera[sample].neardb, status );
						liquidGetPlugValue( fnLight, "farClipPlane", iter->camera[sample].fardb, status );
					}

					if( fnLight.dagPath().hasFn( MFn::kDirectionalLight ) ) 
					{
						iter->camera[sample].isOrtho = true;
						liquidGetPlugValue( fnLight, "dmapWidthFocus", iter->camera[sample].orthoWidth, status );
						liquidGetPlugValue( fnLight, "dmapWidthFocus", iter->camera[sample].orthoHeight, status );
					} 
					else 
					{
						iter->camera[sample].isOrtho = false;
						iter->camera[sample].orthoWidth = 0.0;
					}
				}

				if( iter->deepShadows )
				{
					iter->camera[sample].shutter = liqglo.liqglo_shutterTime;
					iter->camera[sample].motionBlur = true;
				}
				else
				{
					iter->camera[sample].shutter = 0;
					iter->camera[sample].motionBlur = false;
				}
				iter->camera[sample].focalLength = 0;
				iter->camera[sample].focalDistance = 0;
				iter->camera[sample].fStop = 0;
				iter->camera[sample].name  = fnLight.fullPathName();
				//doCameraMotion = 0;

				iter->aspectRatio = 1.0;

				// The camera's fov may not match the rendered image in Maya
				// if a film-fit is used. 'fov_ratio' is used to account for
				// this.
				//
				if( iter->hasShadowCam ) 
				{
					MFnCamera fnCamera( iter->shadowCamPath );
					float camFov = fnCamera.horizontalFieldOfView();
					iter->camera[sample].hFOV = camFov;
				} 
				else 
				{
					float angle = 0, penumbra = 0;
					if ( liquidGetPlugValue( fnLight, "coneAngle", angle, status ) ==  MS::kSuccess )
					{
						liquidGetPlugValue( fnLight, "penumbraAngle", penumbra, status );
						if ( penumbra > 0 ) 
							angle += penumbra * 2;
						iter->camera[sample].hFOV = angle;
					} 
					else 
						iter->camera[sample].hFOV = 95;
				}

				// Determine what information to write out ( depth map or deep shadow )
				//
				iter->imageMode.clear();
				if( iter->deepShadows )
				{
					iter->imageMode    += liqglo.liquidRenderer.dshImageMode;        //"deepopacity";
					iter->format        =  liqglo.liquidRenderer.dshDisplayName;    //"deepshad";
				}
				else
				{
					iter->imageMode += "z";
					iter->format = "shadow";
				}
				//[refactor 13] end
			}
			++iter;
		}
		// post-frame script execution
		if( m_postFrameMel != "" ) 
		{
			MString postFrameMel( parseString( m_postFrameMel, false ) );
			if( fileExists( postFrameMel  ) ) 
				MGlobal::sourceFile( postFrameMel );
			else 
				if( MS::kSuccess == MGlobal::executeCommand( postFrameMel, false, false ) ) 
					liquidMessage("Liquid -> post-frame script executed successfully.", messageInfo);
				else 
					liquidMessage("Liquid -> ERROR :post-frame script failed.",messageError);
		}
		return MS::kSuccess;
	}
	return MS::kFailure;
}

/**
* This method takes care of the blocking together of objects and their children in the DAG.
* This method compares two DAG paths and figures out how many attribute levels to push and/or pop.
* The intention seems clear but this method is not currently used -- anyone cares to comment? --Moritz.
*/
void liqRibTranslator::doAttributeBlocking( const MDagPath& newPath, const MDagPath& previousPath )
{
	CM_TRACE_FUNC("liqRibTranslator::doAttributeBlocking("<<MFnDagNode(newPath).fullPathName().asChar()<<","<<previousPath.fullPathName().asChar()<<")");

	int newDepth = newPath.length();
	int prevDepth = 0;
	MFnDagNode dagFn( newPath );
	MDagPath npath = newPath;
	MDagPath ppath = previousPath;

	if( previousPath.isValid() ) 
	{
		// Recursive base case
		// If the paths are the same, then we don't have to write
		// any start/end attribute blocks.  So, just return
		//
		if( newPath == previousPath ) 
			return;
		prevDepth = previousPath.length();
		// End attribute block if necessary
		//
		if( newDepth <= prevDepth ) 
		{
			// Write an attribute block end
			//
			RiAttributeEnd();
			attributeDepth--;
			if( prevDepth > 1 ) 
				ppath.pop();
		}
	}
	if( ( newDepth >= prevDepth ) && ( newDepth > 1 ) )
		npath.pop();
	// Recurse and process parents
	//
	if( ( prevDepth > 1 ) || ( newDepth > 1 ) ) 
		doAttributeBlocking( npath, ppath ); // Recurse
	// Write open for new attribute block if necessary
	//
	if( newDepth >= prevDepth ) 
	{
		MString name( dagFn.name() );

		RiAttributeBegin();
		RiAttribute( "identifier", "name", &getLiquidRibName( name.asChar() ), RI_NULL );

		if( newPath.hasFn( MFn::kTransform ) ) 
		{
			// We have a transform, so write out the info
			//
			RtMatrix ribMatrix;
			MObject transform = newPath.node();
			MFnTransform transFn( transform );
			MTransformationMatrix localTransformMatrix = transFn.transformation();
			MMatrix localMatrix = localTransformMatrix.asMatrix();
			localMatrix.get( ribMatrix );
			RiConcatTransform( ribMatrix );
		}
		attributeDepth++;
	}
}

/**
* Write out the frame prologue.
* This includes all pass-dependant options like shading interpolation, hider,
* display driver and the camera transformation.
*/
MStatus liqRibTranslator::framePrologue( long lframe )
{
	CM_TRACE_FUNC("liqRibTranslator::framePrologue("<<lframe<<")");

	LIQDEBUGPRINTF( "-> Beginning Frame Prologue\n" );
	ribStatus = kRibFrame;

	if( !liqglo.m_exportReadArchive )
	{
		RiFrameBegin( lframe );

		//refactor 14-1 begin to tShadowRibWriterMgr::framePrologue_display()/tHeroRibWriterMgr::framePrologue_display()
		if( liqglo_currentJob.isShadow == false && liqglo.liqglo_rotateCamera  == true )
		{
			// philippe : Rotated Camera Case
			RiFormat( liqglo_currentJob.height, liqglo_currentJob.width, liqglo_currentJob.aspectRatio );
		}
		else
		{
			RiFormat( liqglo_currentJob.width, liqglo_currentJob.height, liqglo_currentJob.aspectRatio );
		}
		//refactor 14-1 end

		if( !liqglo_currentJob.isShadow )
		{
			//refactor 14 begin to tHeroRibWriterMgr::framePrologue_display()
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
		}
		if( liqglo_currentJob.isShadow &&
			( !liqglo_currentJob.deepShadows ||
			liqglo_currentJob.shadowPixelSamples == 1 ) )
		{
			//refactor 15
			if( liqglo.liquidRenderer.renderName == MString("Pixie") )
			{
				RtFloat zero = 0;
				RiHider( "hidden", "jitter", &zero, RI_NULL );
			}
			else
			{
				RtInt zero = 0;
				RiHider( "hidden", "int jitter", &zero, RI_NULL );
			}
			//refactor 15
		}
		if( liqglo_currentJob.isShadow && liqglo_currentJob.isMidPointShadow && !liqglo_currentJob.deepShadows )
		{//refactor 16
			RtString midPoint = "midpoint";
			RtFloat midRatio = liqglo_currentJob.midPointRatio;

			RiHider( "hidden", "depthfilter", &midPoint, RI_NULL );

			if ( liqglo_currentJob.midPointRatio != 0 )
				RiHider( "hidden", "midpointratio", &midRatio, RI_NULL ); // Output to rib jami
		//refactor 16
		}

		LIQDEBUGPRINTF( "-> Setting Display Options\n" );
		if( liqglo_currentJob.isShadow )
		{
			//MString relativeShadowName( liquidSanitizePath( liquidGetRelativePath( liqglo_relativeFileNames, liqglo_currentJob.imageName, liqglo_projectDir ) ) );
			//refactor 17 begin
			if( !liqglo_currentJob.isMinMaxShadow )
			{
				if( liqglo_currentJob.deepShadows )
				{
					// RiDeclare( "volumeinterpretation", "string" );
					RtString viContinuous = "continuous";
					RtString viDiscrete   = "discrete";

					if( liqglo.liquidRenderer.renderName == MString("3Delight") )
					{
						RiArchiveRecord( RI_COMMENT, "Display 1");
						RiDisplay( const_cast< char* >( liqglo_currentJob.imageName.asChar()),
							const_cast< char* >( liqglo_currentJob.format.asChar() ),
							(RtToken)liqglo_currentJob.imageMode.asChar(),
							"string volumeinterpretation",
							( liqglo_currentJob.shadowVolumeInterpretation == 1 ? &viContinuous : &viDiscrete ),
							RI_NULL );
					}
					else
					{
						// Deep shadows cannot be the primary output driver in PRMan & co.
						// We need to create a null output zfile first, and use the deep
						// shadows as a secondary output.
						//
						if( liqglo.liquidRenderer.renderName != MString("Pixie") )
						{
							RiArchiveRecord( RI_COMMENT, "Display 2");
							RiDisplay( "null", "null", "z", RI_NULL );
						}

						MString deepFileImageName = "+" + liqglo_currentJob.imageName;

						RiArchiveRecord( RI_COMMENT, "Display 3");
						RiDisplay( const_cast< char* >( deepFileImageName.asChar() ),
							const_cast< char* >( liqglo_currentJob.format.asChar() ),
							(RtToken)liqglo_currentJob.imageMode.asChar(),
							"string volumeinterpretation",
							( liqglo_currentJob.shadowVolumeInterpretation == 1 ? &viContinuous : &viDiscrete ),
							RI_NULL );
					}
				}
				else
				{
					RiArchiveRecord( RI_COMMENT, "Display 4");
					RtInt aggregate( liqglo_currentJob.shadowAggregation );
					RiDisplay( const_cast< char* >( liqglo_currentJob.imageName.asChar() ),
						const_cast< char* >( liqglo_currentJob.format.asChar() ),
						(RtToken)liqglo_currentJob.imageMode.asChar(),
						"int aggregate", &aggregate,
						RI_NULL );
				}
			}//if( !liqglo.liqglo_currentJob.isMinMaxShadow )
			else
			{
				RiArchiveRecord( RI_COMMENT, "Display 5");
				RiArchiveRecord( RI_COMMENT, "Display Driver:" );
				RtInt minmax = 1;
				RiDisplay( const_cast< char* >( (liqglo_currentJob.imageName+(int)liqglo.liqglo_lframe).asChar() ),//const_cast< char* >( parseString(liqglo_currentJob.imageName).asChar() ),
					const_cast< char* >(liqglo_currentJob.format.asChar()),
					(RtToken)liqglo_currentJob.imageMode.asChar(),
					"minmax", &minmax,
					RI_NULL );
			}
			//r775 exportJobCamera( liqglo_currentJob, liqglo_currentJob.camera );
			//refactor 17 end
		}
		else
		{
			//refactor 18 begin
			if( ( liqglo.m_cropX1 != 0.0 ) || ( liqglo.m_cropY1 != 0.0 ) || ( liqglo.m_cropX2 != 1.0 ) || ( liqglo.m_cropY2 != 1.0 ) ) 
			{
				// philippe : handle the rotated camera case
				if( liqglo.liqglo_rotateCamera == true ) 
					RiCropWindow( liqglo.m_cropY2, liqglo.m_cropY1, 1 - liqglo.m_cropX1, 1 - liqglo.m_cropX2 );
				else 
					RiCropWindow( liqglo.m_cropX1, liqglo.m_cropX2, liqglo.m_cropY1, liqglo.m_cropY2 );
			}

			if ( !liqglo_currentJob.isStereoPass ) 
				exportJobCamera( liqglo_currentJob, liqglo_currentJob.camera );
			else
			{
				// export right camera
				RiTransformBegin();
				exportJobCamera( liqglo_currentJob, liqglo_currentJob.rightCamera );
				RiArchiveRecord( RI_VERBATIM, "Camera \"%s\"", "right" );//RiCameraV( "right", 0, (char**)RI_NULL, (void**)RI_NULL );
				RiTransformEnd();
				// export left camera
				exportJobCamera( liqglo_currentJob, liqglo_currentJob.leftCamera );
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
					RtToken   tokens[5];
					RtPointer values[5];

					stringstream channel;
					char* filter;
					int quantize[4];
					float filterwidth[2];
					float dither;

// #if defined( GENERIC_RIBLIB )          
					stringstream quantize_str;
					stringstream dither_str;
					stringstream filter_str;
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
					if( channel == "color Ci" )
					{
						isCiDeclared = 1;
					}
					else if( channel == "float a" )
					{
						isADeclared = 1;
					}
#else
					// || defined( GENERIC_RIBLIB )
					RiArchiveRecord( RI_VERBATIM, "DisplayChannel \"%s\" %s %s %s",
						const_cast< char* >( channel.str().c_str() ),  quantize_str.asChar(), dither_str.asChar(), filter_str.asChar() );

#endif
					m_channels_iterator++;
				}
#if defined ( DELIGHT ) || defined ( GENERIC_RIBLIB ) || defined ( PRMAN ) || defined (PIXIE)
				if ( m_isStereoCamera && !liqglo_currentJob.isShadow )
				{
					RtToken   *emptyTokens = NULL;
					RtPointer *emptyValues = NULL;
					if ( !isCiDeclared ) 
						RiDisplayChannelV( ( RtToken )"color Ci", 0, emptyTokens, emptyValues );
					if ( !isADeclared ) 
						RiDisplayChannelV( ( RtToken )"float a", 0, emptyTokens, emptyValues );
				}
#endif
			}
			// output display drivers
			RiArchiveRecord( RI_COMMENT, "Display Drivers:" );
			RiArchiveRecord( RI_COMMENT, "Display 6");

			std::vector<StructDisplay>::iterator m_displays_iterator;
			m_displays_iterator = liqglo.m_displays.begin();

			// create right display for stereo
			if( m_isStereoCamera )
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

			string paramType[] = { "string ", "float ",  "int " };

			//refactor 19
			while ( m_displays_iterator != liqglo.m_displays.end() ) 
			{
				stringstream parameterString;
				stringstream imageName;
				std::string imageType;
				std::string imageMode;
				stringstream quantizer;
				stringstream dither;
				stringstream filter;
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
					//imageName << generateImageName( "", liqglo_currentJob, liqglo_currentJob.format );

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
						//I use liqglo.m_displays[ 0 ].name for maya2renderer - yaoyansi
						imageName << generateImageName( "", liqglo_currentJob, liqglo_currentJob.format );  
					}else{
						imageName << "+" << generateImageName( (*m_displays_iterator).name, liqglo_currentJob, imageType.c_str() ) ;
					}

					// get display type ( tiff, openexr, etc )
					if( !isBatchMode() ){
						imageType = ((*m_displays_iterator).type == "")? 
							"framebuffer" : (*m_displays_iterator).type.asChar();
					}else {// if in batch mode, we always use "file" -yaoyansi
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
						else {
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
				}//while ( m_displays_iterator != m_displays.end() )
				m_displays_iterator++;
			}
			//refactor 19
			//refactor 18 end
		}

		//refactor 20 begin to tRibCameraMgr::framePrologue_camera()
		LIQDEBUGPRINTF( "-> Setting Resolution\n" );
//moved to above
// 		// philippe : Rotated Camera Case
// 		if( liqglo_currentJob.isShadow == false && liqglo.liqglo_rotateCamera  == true ) 
// 			RiFormat( liqglo_currentJob.height, liqglo_currentJob.width, liqglo_currentJob.aspectRatio );
// 		else 
// 			RiFormat( liqglo_currentJob.width, liqglo_currentJob.height, liqglo_currentJob.aspectRatio );

//omitted in r773
// 		if( liqglo_currentJob.camera[0].isOrtho )
// 		{
// 			RtFloat frameWidth, frameHeight;
// 			// the whole frame width has to be scaled according to the UI Unit
// 			frameWidth  = liqglo_currentJob.camera[0].orthoWidth  * 0.5 ;
// 			frameHeight = liqglo_currentJob.camera[0].orthoHeight * 0.5 ;
// 			RiProjection( "orthographic", RI_NULL );
// 
// 			// if we are describing a shadow map camera,
// 			// we need to set the screenwindow to the default,
// 			// as shadow maps are always square.
// 			if( liqglo_currentJob.isShadow == true )
// 				RiScreenWindow( -frameWidth, frameWidth, -frameHeight, frameHeight );
// 			else
// 				RiScreenWindow( -frameWidth, frameWidth, -frameHeight, frameHeight );
// 		} 
// 		else 
// 		{
// 			RtFloat fieldOfView = liqglo_currentJob.camera[0].hFOV * 180.0 / M_PI ;
// 			if( liqglo_currentJob.isShadow && liqglo_currentJob.isPoint ) 
// 				fieldOfView = liqglo_currentJob.camera[0].hFOV;
// 
// 			RiProjection( "perspective", RI_FOV, &fieldOfView, RI_NULL );
// 
// 			// if we are describing a shadow map camera,
// 			// we need to set the screenwindow to the default,
// 			// as shadow maps are always square.
// 			if( liqglo_currentJob.isShadow == false )
// 			{
// 				double ratio = (double)liqglo_currentJob.width / (double)liqglo_currentJob.height;
// 				double left, right, bottom, top;
// 				if( ratio <= 0 ) 
// 				{
// 					left    = -1 + liqglo_currentJob.camera[0].horizontalFilmOffset;
// 					right   =  1 + liqglo_currentJob.camera[0].horizontalFilmOffset;
// 					bottom  = -1 / ratio + liqglo_currentJob.camera[0].verticalFilmOffset;
// 					top     =  1 / ratio + liqglo_currentJob.camera[0].verticalFilmOffset;
// 				} 
// 				else 
// 				{
// 					left    = -ratio + liqglo_currentJob.camera[0].horizontalFilmOffset;
// 					right   =  ratio + liqglo_currentJob.camera[0].horizontalFilmOffset;
// 					bottom  = -1 + liqglo_currentJob.camera[0].verticalFilmOffset;
// 					top     =  1 + liqglo_currentJob.camera[0].verticalFilmOffset;
// 				}
// 				RiScreenWindow( left, right, bottom, top );
// 			} else 
// 				RiScreenWindow( -1.0, 1.0, -1.0, 1.0 );
// 		}
// 
// 		RiClipping( liqglo_currentJob.camera[0].neardb, liqglo_currentJob.camera[0].fardb );
// 		if( liqglo.doDof && !liqglo_currentJob.isShadow ) 
// 			RiDepthOfField( liqglo_currentJob.camera[0].fStop, liqglo_currentJob.camera[0].focalLength, liqglo_currentJob.camera[0].focalDistance );

		// Set up for camera motion blur
		/* doCameraMotion = liqglo_currentJob.camera[0].motionBlur && liqglo_doMotion; */
		float frameOffset = 0;
		if( liqglo.doCameraMotion || liqglo.liqglo_doMotion || liqglo.liqglo_doDef ) 
		{
			switch( liqglo.shutterConfig ) 
			{
			case OPEN_ON_FRAME:
			default:
				if(liqglo.liqglo_relativeMotion)
					RiShutter( 0, liqglo_currentJob.camera[0].shutter );
				else
					RiShutter( lframe, lframe + liqglo_currentJob.camera[0].shutter );
				frameOffset = lframe;
				break;
			case CENTER_ON_FRAME:
				if(liqglo.liqglo_relativeMotion)
					RiShutter(  - ( liqglo_currentJob.camera[0].shutter * 0.5 ),  ( liqglo_currentJob.camera[0].shutter * 0.5 ) );
				else
					RiShutter( ( lframe - ( liqglo_currentJob.camera[0].shutter * 0.5 ) ), ( lframe + ( liqglo_currentJob.camera[0].shutter * 0.5 ) ) );
				frameOffset = lframe - ( liqglo_currentJob.camera[0].shutter * 0.5 );
				break;
			case CENTER_BETWEEN_FRAMES:
				if(liqglo.liqglo_relativeMotion)
					RiShutter( + ( 0.5 * ( 1 - liqglo_currentJob.camera[0].shutter ) ), + liqglo_currentJob.camera[0].shutter + ( 0.5 * ( 1 - liqglo_currentJob.camera[0].shutter ) ) );
				else
					RiShutter( lframe + ( 0.5 * ( 1 - liqglo_currentJob.camera[0].shutter ) ), lframe + liqglo_currentJob.camera[0].shutter + ( 0.5 * ( 1 -liqglo_currentJob.camera[0].shutter ) ) );
				frameOffset = lframe + ( 0.5 * ( 1 - liqglo_currentJob.camera[0].shutter ) );
				break;
			case CLOSE_ON_NEXT_FRAME:
				if(liqglo.liqglo_relativeMotion)
					RiShutter( + ( 1 - liqglo_currentJob.camera[0].shutter ),  1 );
				else
					RiShutter( lframe + ( 1 - liqglo_currentJob.camera[0].shutter ), lframe + 1 );
				frameOffset = lframe + ( 1 - liqglo_currentJob.camera[0].shutter );
				break;
			}
		} 
		else 
		{
			if(liqglo.liqglo_relativeMotion)
				RiShutter( 0, 0);
			else
				RiShutter( lframe, lframe );
			frameOffset = lframe;
		}
		// relative motion
		if(liqglo.liqglo_relativeMotion)    
			RiOption( "shutter", "offset", &frameOffset, RI_NULL);

#ifdef DELIGHT
		RiOption( "shutter", "efficiency", &liqglo.liqglo_shutterEfficiency, RI_NULL );
#endif
		if( liqglo_currentJob.gotJobOptions ) 
			RiArchiveRecord( RI_COMMENT, "jobOptions: \n%s", liqglo_currentJob.jobOptions.asChar() );

		if( ( liqglo.liqglo_preRibBox.length() > 0 ) && !liqglo_currentJob.isShadow ) 
			for ( unsigned ii(0); ii < liqglo.liqglo_preRibBox.length(); ii++ ) 
				RiArchiveRecord( RI_COMMENT, "Additional Rib:\n%s", liqglo.liqglo_preRibBox[ii].asChar() );

		if( ( liqglo.liqglo_preReadArchive.length() > 0 ) && !liqglo_currentJob.isShadow ) 
			for ( unsigned ii(0); ii < liqglo.liqglo_preReadArchive.length(); ii++ ) 
				RiArchiveRecord( RI_COMMENT, "Read Archive Data: \nReadArchive \"%s\"", liqglo.liqglo_preReadArchive[ii].asChar() );

		if( ( liqglo.liqglo_preRibBoxShadow.length() > 0 ) && !liqglo_currentJob.isShadow ) 
			for ( unsigned ii(0); ii < liqglo.liqglo_preRibBoxShadow.length(); ii++ ) 
				RiArchiveRecord( RI_COMMENT, "Additional Rib:\n%s", liqglo.liqglo_preRibBoxShadow[ii].asChar() );

		if( ( liqglo.liqglo_preReadArchiveShadow.length() > 0 ) && liqglo_currentJob.isShadow ) 
			for ( unsigned ii(0); ii < liqglo.liqglo_preReadArchiveShadow.length(); ii++ ) 
				RiArchiveRecord( RI_COMMENT, "Read Archive Data: \nReadArchive \"%s\"", liqglo.liqglo_preReadArchiveShadow[ii].asChar() );
//omitted in r773
// 		// if we motion-blur the cam, open the motion block
// 		//
// 		if( liqglo.doCameraMotion && ( !liqglo_currentJob.isShadow || liqglo_currentJob.deepShadows) ) 
// 			if(liqglo.liqglo_relativeMotion)
// 				RiMotionBeginV( liqglo.liqglo_motionSamples, liqglo.liqglo_sampleTimesOffsets );
// 			else
// 				RiMotionBeginV( liqglo.liqglo_motionSamples, liqglo.liqglo_sampleTimes );
// 
// 		// write the camera transform
// 		//
// 		RtMatrix cameraMatrix;
// 		liqglo_currentJob.camera[0].mat.get( cameraMatrix );
// 		RiTransform( cameraMatrix );
// 
// 		// if we motion-blur the cam, write the subsequent motion samples and close the motion block
// 		//
// 		if( liqglo.doCameraMotion && ( !liqglo_currentJob.isShadow || liqglo_currentJob.deepShadows ) ) 
// 		{
// 			int mm = 1;
// 			while ( mm < liqglo.liqglo_motionSamples ) 
// 			{
// 				liqglo_currentJob.camera[mm].mat.get( cameraMatrix );
// 				RiTransform( cameraMatrix );
// 				++mm;
// 			}
// 			RiMotionEnd();
// 		}
		//refactor 20 end
	}
	
	return MS::kSuccess;
}

/**
* Write out the frame epilogue.
* This basically calls RiFrameEnd().
*/
MStatus liqRibTranslator::frameEpilogue( long )
{
	CM_TRACE_FUNC("liqRibTranslator::frameEpilogue()");

	if(ribStatus == kRibFrame) 
	{
		ribStatus = kRibBegin;
		if( !liqglo.m_exportReadArchive ) 
			RiFrameEnd();
	}
	return (ribStatus == kRibBegin ? MS::kSuccess : MS::kFailure);
}

/**
* Write out the body of the frame.
* This is a dump of the DAG to RIB with flattened transforms (MtoR-style).
*/
MStatus liqRibTranslator::objectBlock()
{
	CM_TRACE_FUNC("liqRibTranslator::objectBlock()");

	MStatus returnStatus = MS::kSuccess;
	MStatus status;
	attributeDepth = 0;

	LIQDEBUGPRINTF( "-> objectBlock\n" );

	if( m_ignoreSurfaces ) 
		RiSurface( "matte", RI_NULL );

	// Moritz: Added Pre-Geometry RIB for insertion right before any primitives
	MFnDependencyNode globalsNode( liqglo.rGlobalObj );
	MPlug prePostplug( globalsNode.findPlug( "preGeomMel" ) );
	MString melcommand( prePostplug.asString() );
	if( m_preGeomRIB != "" || melcommand.length() )
	{
		RiArchiveRecord( RI_COMMENT,  " Pre-Geometry RIB from liquid globals");
		MGlobal::executeCommand( melcommand );
		RiArchiveRecord( RI_VERBATIM, ( char* ) m_preGeomRIB.asChar() );
		RiArchiveRecord( RI_VERBATIM, "\n");
	}

	// retrieve the shadow set object
	MObject shadowSetObj;
	if( liqglo_currentJob.isShadow && liqglo_currentJob.shadowObjectSet != "" ) 
	{
		MObject tmp;
		tmp = getNodeByName( liqglo_currentJob.shadowObjectSet, &status );
		if( status == MS::kSuccess ) 
			shadowSetObj = tmp;
		else 
		{
			std::string warn = "Liquid : set " + string(liqglo_currentJob.shadowObjectSet.asChar()) + " in shadow " + string(liqglo_currentJob.name.asChar()) + " does not exist !";
			liquidMessage( warn, messageWarning );
		}
		status.clear();
	}
	MFnSet shadowSet( shadowSetObj, &status );

	MMatrix matrix;
	MDagPath path;
	MObject transform;
	MFnDagNode dagFn;

	for ( RNMAP::iterator rniter( htable->RibNodeMap.begin() ); rniter != htable->RibNodeMap.end(); rniter++ ) 
	{
		LIQ_CHECK_CANCEL_REQUEST;

		liqRibNodePtr ribNode( rniter->second );
		path = ribNode->path();
		transform = path.transform();

		if( ( !ribNode ) || ( ribNode->object(0)->type == MRT_Light ) ) 
			continue;
		if( ribNode->object(0)->type == MRT_Coord || ribNode->object(0)->type == MRT_ClipPlane ) 
			continue;
		if( ( !liqglo_currentJob.isShadow ) && ( ribNode->object(0)->ignore ) ) 
			continue;
		if( ( liqglo_currentJob.isShadow ) && ( ribNode->object(0)->ignoreShadow ) ) 
			continue;
		// test against the set
		if( ( liqglo_currentJob.isShadow ) && ( liqglo_currentJob.shadowObjectSet != "" ) && ( !shadowSetObj.isNull() ) && ( !shadowSet.isMember( transform, &status ) ) ) 
		{
			//cout <<"SET FILTER : object "<<ribNode->name.asChar()<<" is NOT in "<<liqglo_currentJob.shadowObjectSet.asChar()<<endl;
			continue;
		}

		if( liqglo.m_outputComments ) 
			RiArchiveRecord( RI_COMMENT, "Name: %s", ribNode->name.asChar(), RI_NULL );

		RiAttributeBegin();
		RiAttribute( "identifier", "name", &getLiquidRibName( ribNode->name.asChar() ), RI_NULL );

		// Alf: preTransformMel
		MFnDagNode fnTransform( transform );
		MPlug prePostPlug = fnTransform.findPlug( "liqPreTransformMel" );
		m_preTransformMel = prePostPlug.asString();

		if( m_preTransformMel != "" )
			MGlobal::executeCommand( m_preTransformMel );

		if( !ribNode->grouping.membership.empty() ) 
		{
			RtString members( const_cast< char* >( ribNode->grouping.membership.c_str() ) );
			RiAttribute( "grouping", "membership", &members, RI_NULL );
		}

		if( ribNode->shading.matte || ribNode->mayaMatteMode ) 
			RiMatte( RI_TRUE );

		// If this is a single sided object, then turn that on (RMan default is Sides 2)
		if( !ribNode->doubleSided ) 
			RiSides( 1 );

		if( ribNode->reversedNormals ) 
			RiReverseOrientation();

		LIQDEBUGPRINTF( "-> object name: %s\n", ribNode->name.asChar() );
		MObject object;

		// Moritz: only write out light linking if we're not in a shadow pass
		if( !liqglo_currentJob.isShadow || liqglo_currentJob.deepShadows && m_outputLightsInDeepShadows && !m_ignoreLights )
		{
			MObjectArray linkLights;

			// light linking mode - Alf
			// inclusive - lights are off by default and objects list included lights
			// exclusive - lights are on by default and objects list ignored lights
			// liquid Light sets - ignores the maya light linker
			if( m_liquidSetLightLinking )
				ribNode->getSetLights( linkLights );
			else
				ribNode->getLinkLights( linkLights, liqglo.m_illuminateByDefault );

			for( unsigned i( 0 ); i < linkLights.length(); i++ )
			{
				MFnDagNode lightFnDag( linkLights[i] );
				MString nodeName = lightFnDag.fullPathName();
				if( htable )
				{
					//RibNode * ln = htable->find( light, MRT_Light );
					MDagPath nodeDagPath;
					lightFnDag.getPath( nodeDagPath );
					liqRibNodePtr  ln( htable->find( lightFnDag.fullPathName(), nodeDagPath, MRT_Light ) );
					if( NULL != ln )
					{
						if( liqglo.m_illuminateByDefault )
							RiIlluminate( ln->object(0)->lightHandle(), RI_FALSE );
						else
							RiIlluminate( ln->object(0)->lightHandle(), RI_TRUE );
					}
				}
			}
		}
		//refactor 30 to liqRibTranslator::MaxtrixMotionBlur()
		if( liqglo.liqglo_doMotion &&
			ribNode->motion.transformationBlur &&
			( ribNode->object( 1 ) ) &&
			//( ribNode->object(0)->type != MRT_Locator ) && // Why the fuck do we not allow motion blur for locators?
			( !liqglo_currentJob.isShadow || liqglo_currentJob.deepShadows ) )
		{
			LIQDEBUGPRINTF( "-> writing matrix motion blur data\n" );
			// Moritz: replaced RiMotionBegin call with ..V version to allow for more than five motion samples
			if(liqglo.liqglo_relativeMotion)
				RiMotionBeginV( liqglo.liqglo_motionSamples, liqglo.liqglo_sampleTimesOffsets );
			else
				RiMotionBeginV( liqglo.liqglo_motionSamples, liqglo.liqglo_sampleTimes );
		}
#if 1
		RtMatrix ribMatrix;
		matrix = ribNode->object( 0 )->matrix( path.instanceNumber() );
		matrix.get( ribMatrix );

		if( liqglo.liqglo_relativeTransforms ) 
			RiConcatTransform( ribMatrix );
		else 
			RiTransform( ribMatrix );
#elif 0  // Bat : a way to have double transforms :
		double doubleTransformMatrix[4][4];
		matrix = ribNode->object( 0 )->matrix( path.instanceNumber() );
		matrix.get( doubleTransformMatrix );

		int txIntPart = (int)(doubleTransformMatrix[3][0]);
		float txFloatPart = doubleTransformMatrix[3][0] - txIntPart;

		int tyIntPart = (int)(doubleTransformMatrix[3][1]);
		float tyFloatPart = doubleTransformMatrix[3][1] - tyIntPart;

		int tzIntPart = (int)(doubleTransformMatrix[3][2]);
		float tzFloatPart = doubleTransformMatrix[3][2] - tzIntPart;

		RtFloat floatTransformMatrixWithIntegerTranslatePart[4][4];
		matrix.get( floatTransformMatrixWithIntegerTranslatePart );
		RtFloat floatIdentityMatrixWithFloatingTranslatePart[4][4] = { {1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1} };

		floatTransformMatrixWithIntegerTranslatePart[3][0] = txIntPart;
		floatTransformMatrixWithIntegerTranslatePart[3][1] = tyIntPart;
		floatTransformMatrixWithIntegerTranslatePart[3][2] = tzIntPart;

		floatIdentityMatrixWithFloatingTranslatePart[3][0] = txFloatPart;
		floatIdentityMatrixWithFloatingTranslatePart[3][1] = tyFloatPart;
		floatIdentityMatrixWithFloatingTranslatePart[3][2] = tzFloatPart;

		if( liqglo_relativeTransforms )
		{
			RiConcatTransform( floatIdentityMatrixWithFloatingTranslatePart );
			RiConcatTransform( floatTransformMatrixWithIntegerTranslatePart );
		}
		else
		{
			RiTransform( floatIdentityMatrixWithFloatingTranslatePart );
			RiConcatTransform( floatTransformMatrixWithIntegerTranslatePart );
		}
#endif
		// Output the world matrices for the motionblur
		// This will override the current transformation setting
		if( liqglo.liqglo_doMotion &&
			ribNode->motion.transformationBlur &&
			( ribNode->object( 1 ) ) &&
			//( ribNode->object( 0 )->type != MRT_Locator ) && // Why the fuck do we not allow motion blur for locators?
			( !liqglo_currentJob.isShadow || liqglo_currentJob.deepShadows ) )
		{
			path = ribNode->path();
			RtMatrix ribMatrix;
			for( unsigned mm( 1 ); mm < liqglo.liqglo_motionSamples; mm++ ) {
				matrix = ribNode->object( mm )->matrix( path.instanceNumber() );
				matrix.get( ribMatrix );
				if( liqglo.liqglo_relativeTransforms ) 
					RiConcatTransform( ribMatrix ); 
				else 
					RiTransform( ribMatrix );
			}
			RiMotionEnd();
		}
		//refactor 30 end

		// Alf: postTransformMel
		prePostPlug = fnTransform.findPlug( "liqPostTransformMel" );
		m_postTransformMel = prePostPlug.asString();
		if( m_postTransformMel != "" )
			MGlobal::executeCommand( m_postTransformMel );

		bool hasSurfaceShader( false );
		typedef enum {
			liqRegularShaderNode = 0,     // A regular Liquid node, keep it 0 to evaluate to false in conditions
			liqCustomPxShaderNode = 1,     // A custom MPxNode inheriting from liqCustomNode
			liqRibBoxShader = 2          // A rib box attached to the shader
		} liqDetailShaderKind;

		liqDetailShaderKind hasCustomSurfaceShader( liqRegularShaderNode );
		MString shaderRibBox( "" );
		bool hasDisplacementShader( false );
		bool hasVolumeShader( false );

		MPlug rmanShaderPlug;
		// Check for surface shader
		status.clear();
		MFnDagNode fnDagNode( path );
		rmanShaderPlug = fnDagNode.findPlug( MString( "liquidSurfaceShaderNode" ), &status );
		if( ( status != MS::kSuccess ) || ( !rmanShaderPlug.isConnected() ) ) 
		{ 
			status.clear(); 
			rmanShaderPlug = ribNode->assignedShadingGroup.findPlug( MString( "liquidSurfaceShaderNode" ), &status ); 
		}
		if( ( status != MS::kSuccess ) || ( !rmanShaderPlug.isConnected() ) ) 
		{ 
			status.clear(); 
			rmanShaderPlug = ribNode->assignedShader.findPlug( MString( "liquidSurfaceShaderNode" ), &status ); 
		}
		if( ( status != MS::kSuccess ) || ( !rmanShaderPlug.isConnected() ) ) 
		{ 
			status.clear(); 
			rmanShaderPlug = ribNode->assignedShadingGroup.findPlug( MString( "surfaceShader" ), &status ); 
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
				    //added in r773 || shaderDepNode.typeName() == "liquidSurfaceSwitcher"
					|| shaderDepNode.typeName() == "liquidShader" 
					|| shaderDepNode.typeName() == "oldBlindDataBase" ) 
				{ //cout <<"setting shader"<<endl;
					ribNode->assignedShader.setObject( rmShaderNodeObj );
					hasSurfaceShader = true;
				} 
				else 
				{ // Is it a custom shading node ?
					MPxNode *mpxNode = shaderDepNode.userNode();
					liqCustomNode *customNode = NULL;
					if( mpxNode && ( customNode = dynamic_cast<liqCustomNode*>(mpxNode) ) ) 
					{ // customNode will be null if can't be casted to a liqCustomNode
						ribNode->assignedShader.setObject( rmShaderNodeObj );
						hasSurfaceShader = true;
						hasCustomSurfaceShader = liqCustomPxShaderNode;
					} 
					else 
					{ // Try to find a liqRIBBox attribute
						MPlug ribbPlug = shaderDepNode.findPlug( MString( "liqRIBBox" ), &status );
						if( status == MS::kSuccess ) 
						{
							ribbPlug.getValue( shaderRibBox );
							if( shaderRibBox.substring(0,2) == "*H*" ) 
							{
								MString parseThis = shaderRibBox.substring(3, shaderRibBox.length() - 1 );
								shaderRibBox = parseThis;
							} 
							else if( shaderRibBox.substring(0,3) == "*SH*" ) 
							{
								MString parseThis = shaderRibBox.substring(3, shaderRibBox.length() - 1 );
								shaderRibBox = parseThis;
							}
							hasSurfaceShader = true;
							hasCustomSurfaceShader = liqRibBoxShader;
						}
						// else {
						//liquidMessage( "Do noy know how to handle " + string( shaderDepNode.typeName().asChar() ), messageError );
						//}
					}
				}
			}
		}
		// Check for displacement shader
		status.clear();
		rmanShaderPlug = fnDagNode.findPlug( MString( "liquidDispShaderNode" ), &status );
		if( ( status != MS::kSuccess ) || ( !rmanShaderPlug.isConnected() ) ) 
		{ 
			status.clear(); 
			rmanShaderPlug = ribNode->assignedShadingGroup.findPlug( MString( "liquidDispShaderNode" ), &status ); 
		}
		if( ( status != MS::kSuccess ) || ( !rmanShaderPlug.isConnected() ) ) 
		{ 
			status.clear(); 
			rmanShaderPlug = ribNode->assignedDisp.findPlug( MString( "liquidDispShaderNode" ), &status ); 
		}
		if( ( status != MS::kSuccess ) || ( !rmanShaderPlug.isConnected() ) ) 
		{ 
			status.clear(); 
			rmanShaderPlug = ribNode->assignedShadingGroup.findPlug( MString( "displacementShader" ), &status ); 
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
					ribNode->assignedDisp.setObject( rmShaderNodeObj );
					hasDisplacementShader = true;
				}
			}
		}
		// Check for volume shader
		status.clear();
		rmanShaderPlug = fnDagNode.findPlug( MString( "liquidVolumeShaderNode" ), &status );
		if( ( status != MS::kSuccess ) || ( !rmanShaderPlug.isConnected() ) ) 
		{ 
			status.clear(); 
			rmanShaderPlug = ribNode->assignedShadingGroup.findPlug( MString( "liquidVolumeShaderNode" ), &status ); 
		}
		if( ( status != MS::kSuccess ) || ( !rmanShaderPlug.isConnected() ) ) 
		{ 
			status.clear(); 
			rmanShaderPlug = ribNode->assignedVolume.findPlug( MString( "liquidVolumeShaderNode" ), &status ); 
		}
		if( ( status != MS::kSuccess ) || ( !rmanShaderPlug.isConnected() ) ) 
		{ 
			status.clear(); 
			rmanShaderPlug = ribNode->assignedShadingGroup.findPlug( MString( "volumeShader" ), &status ); 
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
					ribNode->assignedVolume.setObject( rmShaderNodeObj );
					hasVolumeShader = true;
				}
			}
		}
// 		// displacement bounds
// 		float surfaceDisplacementBounds = 0.0;
// 		MString surfaceDisplacementBoundsSpace = "shader";
// 		MString tmpSpace = "";
// 		status.clear();
// 		if( !ribNode->assignedShader.object().isNull() ) 
// 		{
// 			MPlug sDBPlug = ribNode->assignedShader.findPlug( MString( "displacementBound" ), &status );
// 			if( status == MS::kSuccess ) 
// 				sDBPlug.getValue( surfaceDisplacementBounds );
// 			MPlug sDBSPlug = ribNode->assignedShader.findPlug( MString( "displacementBoundSpace" ), &status );
// 			if( status == MS::kSuccess ) 
// 				sDBSPlug.getValue( tmpSpace );
// 			if( tmpSpace != "" ) 
// 				surfaceDisplacementBoundsSpace = tmpSpace;
// 		}
// 		float dispDisplacementBounds = 0.0;
// 		MString dispDisplacementBoundsSpace = "shader";
// 		tmpSpace = "";
// 		status.clear();
// 		if( !ribNode->assignedDisp.object().isNull() ) 
// 		{
// 			MPlug dDBPlug = ribNode->assignedDisp.findPlug( MString( "displacementBound" ), &status );
// 			if( status == MS::kSuccess ) 
// 				dDBPlug.getValue( dispDisplacementBounds );
// 			MPlug sDBSPlug = ribNode->assignedDisp.findPlug( MString( "displacementBoundSpace" ), &status );
// 			if( status == MS::kSuccess ) 
// 				sDBSPlug.getValue( tmpSpace );
// 			if( tmpSpace != "" ) 
// 				dispDisplacementBoundsSpace = tmpSpace;
// 		}
// 		if( ( dispDisplacementBounds != 0.0 ) && ( dispDisplacementBounds > surfaceDisplacementBounds ) ) 
// 		{
// 			RtString coordsys( const_cast< char* >( dispDisplacementBoundsSpace.asChar() ) );
// 			RiAttribute( "displacementbound", (RtToken) "sphere", &dispDisplacementBounds, "coordinatesystem", &coordsys, RI_NULL );
// 		} 
// 		else if( ( surfaceDisplacementBounds != 0.0 ) ) 
// 		{
// 			RtString coordsys( const_cast< char* >( surfaceDisplacementBoundsSpace.asChar() ) );
// 			RiAttribute( "displacementbound", (RtToken) "sphere", &surfaceDisplacementBounds, "coordinatesystem", &coordsys, RI_NULL );
// 		}
		LIQDEBUGPRINTF( "-> writing node attributes\n" );
		// if the node's shading rate == -1,
		// it means it hasn't been overriden by a liqShadingRate attribute.
		// No need to output it then.
		if( ribNode->shading.shadingRate > 0 )
			RiShadingRate ( ribNode->shading.shadingRate );
		if( !liqglo_currentJob.isShadow ) 
		{
			RtInt off( 0 );
			RtInt on( 1 );
			if( !ribNode->shading.diceRasterOrient ) 
				RiAttribute( "dice", (RtToken) "rasterorient", &off, RI_NULL );
			if( ribNode->shading.doubleShaded ) 
				RiAttribute( "sides", (RtToken) "doubleshaded", &on, RI_NULL );
		if( !m_skipRayTraceAttributes )
		{
			if( liqglo.liquidRenderer.supports_RAYTRACE ) 
			{
				if( ribNode->trace.sampleMotion ) 
					RiAttribute( "trace", (RtToken) "samplemotion", &on, RI_NULL );
				if( ribNode->trace.displacements ) 
					RiAttribute( "trace", (RtToken) "displacements", &on, RI_NULL );
				if( ribNode->trace.bias != 0.01f ) 
				{
					RtFloat bias( ribNode->trace.bias );
					RiAttribute( "trace", (RtToken) "bias", &bias, RI_NULL );
				}
				if( ribNode->trace.maxDiffuseDepth != 1 ) 
				{
					RtInt mddepth( ribNode->trace.maxDiffuseDepth );
					RiAttribute( "trace", (RtToken) "maxdiffusedepth", &mddepth, RI_NULL );
				}
				if( ribNode->trace.maxSpecularDepth != 2 ) 
				{
					RtInt msdepth( ribNode->trace.maxSpecularDepth );
					RiAttribute( "trace", (RtToken) "maxspeculardepth", &msdepth, RI_NULL );
				}
			}
		}
			if( !ribNode->visibility.camera ) 
				RiAttribute( "visibility", (RtToken) "camera", &off, RI_NULL );

			// old-style raytracing visibility support
			// philippe: if raytracing is off in the globals, trace visibility is turned off for all objects, transmission is set to TRANSPARENT for all objects
			if( liqglo.liquidRenderer.supports_RAYTRACE && !liqglo.liquidRenderer.supports_ADVANCED_VISIBILITY ) 
			{
				if( liqglo.rt_useRayTracing && ribNode->visibility.trace ) {
					RtInt on( 1 );
					RiAttribute( "visibility", (RtToken) "trace", &on, RI_NULL );
				}
				if( liqglo.rt_useRayTracing && ribNode->visibility.transmission != liqRibNode::visibility::TRANSMISSION_TRANSPARENT ) 
				{
					RtString trans;
					switch( ribNode->visibility.transmission ) 
					{
					case liqRibNode::visibility::TRANSMISSION_OPAQUE:
						trans = "opaque";
						break;
					case liqRibNode::visibility::TRANSMISSION_OS:
						trans = "Os";
						break;
					case liqRibNode::visibility::TRANSMISSION_SHADER:
					default:
						trans = "shader";
					}
					RiAttribute( "visibility", (RtToken) "string transmission", &trans, RI_NULL );
				}
			}
			// philippe : prman 12.5 visibility support
#ifdef GENERIC_RIBLIB      
			extern int useAdvancedVisibilityAttributes;
			useAdvancedVisibilityAttributes = false;
#endif
			if( liqglo.liquidRenderer.supports_RAYTRACE && liqglo.liquidRenderer.supports_ADVANCED_VISIBILITY ) 
			{
				RtInt on( 1 );
				RtString mode;
#ifdef GENERIC_RIBLIB         
				useAdvancedVisibilityAttributes = true;
#endif
				if( !m_skipVisibilityAttributes )
				{
				if( liqglo.rt_useRayTracing && ribNode->visibility.diffuse ) 
					RiAttribute( "visibility", (RtToken) "int diffuse", &on, RI_NULL );

				if( liqglo.rt_useRayTracing && ribNode->visibility.specular ) 
					RiAttribute( "visibility", (RtToken) "int specular", &on, RI_NULL );

				if( liqglo.rt_useRayTracing && ribNode->visibility.newtransmission ) 
					RiAttribute( "visibility", (RtToken) "int transmission", &on, RI_NULL );

				if( liqglo.rt_useRayTracing && ribNode->visibility.camera ) 
					RiAttribute( "visibility", (RtToken) "int camera", &on, RI_NULL );

				if( liqglo.rt_useRayTracing && ribNode->visibility.photon ) 
					RiAttribute( "visibility", (RtToken) "int photon", &on, RI_NULL );

				if( liqglo.rt_useRayTracing && ribNode->visibility.midpoint ) 
					RiAttribute( "visibility", (RtToken) "int midpoint", &on, RI_NULL );
				}
				
				if( !m_skipRayTraceAttributes )
				{	
				if( liqglo.rt_useRayTracing && ribNode->hitmode.diffuse != liqRibNode::hitmode::DIFFUSE_HITMODE_PRIMITIVE ) 
				{
					switch( ribNode->hitmode.diffuse ) 
					{
					case liqRibNode::hitmode::DIFFUSE_HITMODE_SHADER:
						mode = "shader";
						break;
					case liqRibNode::hitmode::DIFFUSE_HITMODE_PRIMITIVE:
					default:
						mode = "primitive";
					}
					RiAttribute( "shade", (RtToken) "string diffusehitmode", &mode, RI_NULL );
				}

				if( liqglo.rt_useRayTracing && ribNode->hitmode.specular != liqRibNode::hitmode::SPECULAR_HITMODE_SHADER ) 
				{
					switch( ribNode->hitmode.specular ) 
					{
					case liqRibNode::hitmode::SPECULAR_HITMODE_PRIMITIVE:
						mode = "primitive";
						break;
					case liqRibNode::hitmode::SPECULAR_HITMODE_SHADER:
					default:
						mode = "shader";
					}
					RiAttribute( "shade", (RtToken) "string specularhitmode", &mode, RI_NULL );
				}

				if( liqglo.rt_useRayTracing && ribNode->hitmode.transmission != liqRibNode::hitmode::TRANSMISSION_HITMODE_SHADER ) 
				{
					switch( ribNode->hitmode.transmission ) 
					{
					case liqRibNode::hitmode::TRANSMISSION_HITMODE_PRIMITIVE:
						mode = "primitive";
						break;
					case liqRibNode::hitmode::TRANSMISSION_HITMODE_SHADER:
					default:
						mode = "shader";
					}
					RiAttribute( "shade", (RtToken) "string transmissionhitmode", &mode, RI_NULL );
				}
				}
				if( !m_skipShadingAttributes )
				{
				if( ribNode->hitmode.camera != liqRibNode::hitmode::CAMERA_HITMODE_SHADER ) 
				{
					switch( ribNode->hitmode.camera ) 
					{
					case liqRibNode::hitmode::CAMERA_HITMODE_PRIMITIVE:
						mode = "primitive";
						break;
					case liqRibNode::hitmode::CAMERA_HITMODE_SHADER:
					default:
						mode = "shader";
					}
					RiAttribute( "shade", (RtToken) "string camerahitmode", &mode, RI_NULL );
				}
				}
			}

			if( liqglo.liquidRenderer.supports_RAYTRACE ) 
			{
				if( ribNode->irradiance.shadingRate != 1.0f ) 
				{
					RtFloat rate = ribNode->irradiance.shadingRate;
					RiAttribute( "irradiance", (RtToken) "shadingrate", &rate, RI_NULL );
				}

				if( ribNode->irradiance.nSamples != 64 ) 
				{
					RtInt samples = ribNode->irradiance.nSamples;
					RiAttribute( "irradiance", (RtToken) "nsamples", &samples, RI_NULL );
				}

				if( ribNode->irradiance.maxError != 0.5f ) 
				{
					RtFloat merror = ribNode->irradiance.maxError;
					RiAttribute( "irradiance", (RtToken) "float maxerror", &merror, RI_NULL );
				}

				if( ribNode->irradiance.maxPixelDist != 30.0f ) 
				{
					RtFloat mpd = ribNode->irradiance.maxPixelDist;
					RiAttribute( "irradiance", (RtToken) "float maxpixeldist", &mpd, RI_NULL );
				}

				if( ribNode->irradiance.handle != "" ) 
				{
					RtString handle = const_cast< char* >( ribNode->irradiance.handle.asChar() );
					RiAttribute( "irradiance", (RtToken) "handle", &handle, RI_NULL );
				}

				if( ribNode->irradiance.fileMode != liqRibNode::irradiance::FILEMODE_NONE ) 
				{
					RtString mode;
					switch( ribNode->irradiance.fileMode ) 
					{
					case liqRibNode::irradiance::FILEMODE_READ:
						mode = "r";
						break;
					case liqRibNode::irradiance::FILEMODE_WRITE:
						mode = "w";
						break;
					case liqRibNode::irradiance::FILEMODE_READ_WRITE:
					default:
						mode = "rw";
					}
					RiAttribute( "irradiance", (RtToken) "filemode", &mode, RI_NULL );
				}

				if( ribNode->photon.globalMap != "" ) 
				{
					RtString map = const_cast< char* >( ribNode->photon.globalMap.asChar() );
					RiAttribute( "photon", (RtToken) "globalmap", &map, RI_NULL );
				}

				if( ribNode->photon.causticMap != "" ) 
				{
					RtString map = const_cast< char* >( ribNode->photon.causticMap.asChar() );
					RiAttribute( "photon", (RtToken) "causticmap", &map, RI_NULL );
				}

				if( ribNode->photon.shadingModel != liqRibNode::photon::SHADINGMODEL_MATTE ) 
				{
					RtString model;
					switch( ribNode->photon.shadingModel  ) {
			case liqRibNode::photon::SHADINGMODEL_GLASS:
				model = "glass";
				break;
			case liqRibNode::photon::SHADINGMODEL_WATER:
				model = "water";
				break;
			case liqRibNode::photon::SHADINGMODEL_CHROME:
				model = "chrome";
				break;
			case liqRibNode::photon::SHADINGMODEL_TRANSPARENT:
				model = "chrome";
				break;
			case liqRibNode::photon::SHADINGMODEL_DIALECTRIC:
				model = "dielectric";
				break;
			case liqRibNode::photon::SHADINGMODEL_MATTE:
			default:
				model = "matte";
					}
					RiAttribute( "photon", (RtToken) "shadingmodel", &model, RI_NULL );
				}

				if( ribNode->photon.estimator != 100 ) 
				{
					RtInt estimator = ribNode->photon.estimator;
					RiAttribute( "photon", (RtToken) "estimator", &estimator, RI_NULL );
				}

			}

			// 3Delight sss group
			if( ribNode->delightSSS.doScatter && liqglo.liquidRenderer.renderName == MString("3Delight") )
			{
#ifdef GENERIC_RIBLIB         
				useAdvancedVisibilityAttributes = true;
#endif			  
				RtString groupName = const_cast< char* >( ribNode->delightSSS.groupName.asChar() );
				RiAttribute( "visibility", (RtToken) "string subsurface", &groupName, RI_NULL );

				RtColor scattering, absorption;
				scattering[0] = ribNode->delightSSS.scattering.r;
				scattering[1] = ribNode->delightSSS.scattering.g;
				scattering[2] = ribNode->delightSSS.scattering.b;

				absorption[0] = ribNode->delightSSS.absorption.r;
				absorption[1] = ribNode->delightSSS.absorption.g;
				absorption[2] = ribNode->delightSSS.absorption.b;

				if ( scattering[0] && scattering[1] && scattering[2] )
					RiAttribute( "subsurface",
					(RtToken) "scattering", &scattering,
					(RtToken) "absorption", &absorption,
					RI_NULL );

				RtFloat refractionindex = ribNode->delightSSS.refraction;
				RtFloat shadingrate = ribNode->delightSSS.shadingRate;
				RtFloat scale = ribNode->delightSSS.scale;

				RiAttribute( "subsurface",
					(RtToken) "refractionindex", &refractionindex,
					(RtToken) "shadingrate", &shadingrate,
					(RtToken) "scale", &scale, 
					RI_NULL );

				RtColor meanfreepath, reflectance;
				meanfreepath[0] = ribNode->delightSSS.meanfreepath.r;
				meanfreepath[1] = ribNode->delightSSS.meanfreepath.g;
				meanfreepath[2] = ribNode->delightSSS.meanfreepath.b;

				reflectance[0] = ribNode->delightSSS.reflectance.r;
				reflectance[1] = ribNode->delightSSS.reflectance.g;
				reflectance[2] = ribNode->delightSSS.reflectance.b;

				if ( reflectance[0] && reflectance[1] && reflectance[2] )
					RiAttribute( "subsurface",
					(RtToken) "meanfreepath", &meanfreepath,
					(RtToken) "reflectance", &reflectance, 
					RI_NULL );

				if ( ribNode->delightSSS.referencecamera != "" )
				{
					RtString referenceCamera = const_cast< char* >( ribNode->delightSSS.referencecamera.asChar() );
					RiAttribute( "subsurface", (RtToken) "string referencecamera", &referenceCamera, RI_NULL );
				}
			}

			//strategy
			{
				if(ribNode->strategy.strategy_!="grids"){
					RtString strategy = const_cast<char*>(ribNode->strategy.strategy_.asChar());
					RiAttribute("shade",(RtToken)"string strategy", &strategy, RI_NULL);
				}

				if(ribNode->strategy.volumeIntersectionStrategy!="exclusive"){
					RtString volumeIntersectionStrategy = const_cast<char*>(ribNode->strategy.volumeIntersectionStrategy.asChar());
					RiAttribute("shade",(RtToken)"string volumeintersectionstrategy", &volumeIntersectionStrategy, RI_NULL);
				}

				if(ribNode->strategy.volumeIntersectionPriority >0.0001){
					RtFloat volumeIntersectionPriority = ribNode->strategy.volumeIntersectionPriority;
					RiAttribute("shade",(RtToken)"float volumeintersectionpriority", &volumeIntersectionPriority, RI_NULL);
				}
			}

			//trim curve 
			{
				RtString sense = "inside";

				switch(ribNode->trimcurve.sense){
				case liqRibNode::trimcurve::INSIDE:  
					{
						sense  = "inside";
						//RiAttribute("trimcurve",(RtToken)"string sense", &sense, RI_NULL);//default value, need not to write.
					}break;
				case liqRibNode::trimcurve::OUTSIDE:
					{
						sense  = "outside";
						RiAttribute("trimcurve",(RtToken)"string sense", &sense, RI_NULL);	
					}break;
				default:
					assert(0);
				}

			}//trim curve 

			//stitch
			{
				if( ribNode->stitch.enable != true ){
					RtInt enable = (ribNode->stitch.enable)? 1: 0;
					RiAttribute("stitch",(RtToken)"int enable", &enable, RI_NULL);	
				}
				if( ribNode->stitch.traceenable != false ){
					RtInt traceenable = (ribNode->stitch.traceenable)? 1: 0;
					RiAttribute("stitch",(RtToken)"int traceenable", &traceenable, RI_NULL);	
				}
				if( ribNode->stitch.newgroup != false ){
					RtInt newgroup = (ribNode->stitch.newgroup)? 1: 0;
					RiAttribute("stitch",(RtToken)"int newgroup", &newgroup, RI_NULL);	
				}	
			}//stitch

			//stochastic
			{
				if( ribNode->stochastic.sigma != 0 ){
					RtInt sigma = ribNode->stochastic.sigma;
					RiAttribute("stochastic",(RtToken)"int sigma", &sigma, RI_NULL);	
				}
				if( ribNode->stochastic.pointfalloff != 0 ){
					RtInt pointfalloff = ribNode->stochastic.pointfalloff;
					RiAttribute("stochastic",(RtToken)"int pointfalloff", &pointfalloff, RI_NULL);	
				}
			}//stochastic

			//dice
			{
				if( ribNode->dice.binary != 0 ){
					RtInt binary = ribNode->dice.binary;
					RiAttribute("dice",(RtToken)"int binary", &binary, RI_NULL);
				}
				if( ribNode->dice.hair != 0 ){
					RtInt hair = ribNode->dice.hair;
					RiAttribute("dice",(RtToken)"int hair", &hair, RI_NULL);
				}
				//strategy
				RtString strategy = "planarprojection";
				switch(ribNode->dice.strategy){
				  case liqRibNode::dice::PLANAR_PROJECTION:
					  {
						  strategy = "planarprojection";
						  //RiAttribute("dice",(RtToken)"string strategy", &strategy, RI_NULL); //default value
					  }break;
				  case liqRibNode::dice::SPHERICAL_PROJECTION:
					  {
						  strategy = "sphericalprojection";
						  RiAttribute("dice",(RtToken)"string strategy", &strategy, RI_NULL);
					  }break;
				  default:
					  assert(0);
				}
				//referencecamera
				RtString referencecamera = "worldcamera";
				switch(ribNode->dice.referencecamera){
				  case liqRibNode::dice::WORLD_CAMERA:
					  {
						  referencecamera = "worldcamera";
						  //RiAttribute("dice",(RtToken)"string referencecamera", &referencecamera, RI_NULL); //default value
					  }break;
				  case liqRibNode::dice::FRAME_CAMERA:
					  {
						  referencecamera = "framecamera";
						  RiAttribute("dice",(RtToken)"string referencecamera", &referencecamera, RI_NULL);
					  }break;
				  default:
					  assert(0);
				}

			}//dice

			//derivatives
			{
				if( ribNode->derivatives.centered != 1 ){
					RtInt centered = ribNode->derivatives.centered;
					RiAttribute("derivatives",(RtToken)"int centered", &centered, RI_NULL);
				}
				if( ribNode->derivatives.extrapolate != 1 ){
					RtInt extrapolate = ribNode->derivatives.extrapolate;
					RiAttribute("derivatives",(RtToken)"int extrapolate", &extrapolate, RI_NULL);
				}
			}//derivatives

			//procedural
			{
				if( ribNode->procedural.attribute.length()>0 ){
					RtString attribute = const_cast<char*>(ribNode->procedural.attribute.asChar());
					RiAttribute("procedural",(RtToken)"string attribute", &attribute, RI_NULL);
				}
			}//procedural

			if( ribNode->motion.deformationBlur || ribNode->motion.transformationBlur && ribNode->motion.factor != 1.0f ) 
				RiGeometricApproximation( "motionfactor", ribNode->motion.factor );

			ribNode->writeUserAttributes();
		}

		bool writeShaders( true );

		if( liqglo_currentJob.isShadow &&
			( ( !liqglo_currentJob.deepShadows && !m_outputShadersInShadows ) || ( liqglo_currentJob.deepShadows && !m_outputShadersInDeepShadows ) )
			)
			writeShaders = false;
		
		liqRIBMsg("[6] writeShaders=%d=%d && ((!%d&&!%d)||(%d&&!%d) ", writeShaders, 
			liqglo_currentJob.isShadow, 
			liqglo_currentJob.deepShadows, m_outputShadersInShadows, liqglo_currentJob.deepShadows, m_outputShadersInDeepShadows );
		if( writeShaders ) 
		{
			liqRIBMsg("[5] hasVolumeShader=%d, m_ignoreVolumes=%d", hasVolumeShader, m_ignoreVolumes );
			if( hasVolumeShader && !m_ignoreVolumes ) 
			{
				//liqShader& currentShader( liqGetShader( ribNode->assignedVolume.object() ) );
				liqShader& currentShader = liqShaderFactory::instance().getShader( ribNode->assignedVolume.object() );
				liqRIBMsg("[1] liqglo_currentJob.isShadow=%d, currentShader.outputInShadow=%d", liqglo_currentJob.isShadow, currentShader.outputInShadow );
				// per shader shadow pass override
				if( !liqglo_currentJob.isShadow || currentShader.outputInShadow )
				{
					currentShader.write( );
				}
			}
			if( hasSurfaceShader && !m_ignoreSurfaces )
			{
				if( hasCustomSurfaceShader )
				{
					if( hasCustomSurfaceShader == liqCustomPxShaderNode )
					{  // Just call the write method of the custom shader
						MFnDependencyNode customShaderDepNode( ribNode->assignedShader.object() );
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
						RiArchiveRecord( RI_VERBATIM, ( char* )shaderRibBox.asChar() );
						RiArchiveRecord( RI_VERBATIM, "\n" );
					}
				}
				else//hasCustomSurfaceShader==liqRegularShaderNode
				{
					//liqShader& currentShader( liqGetShader( ribNode->assignedShader.object() ) );
					liqShader& currentShader = liqShaderFactory::instance().getShader( ribNode->assignedShader.object() );

					// Output color overrides or color ====>>>>>  WILL BE DONE IN liqShader::write -begin //r773 going to omit in r773
					if(ribNode->shading.color.r != -1.0)
					{
						RtColor rColor;
						rColor[0] = ribNode->shading.color[0];
						rColor[1] = ribNode->shading.color[1];
						rColor[2] = ribNode->shading.color[2];
						RiArchiveRecord( RI_COMMENT, "mark1" );
						RiColor( rColor );
					}
					else{
						RiArchiveRecord( RI_COMMENT, "mark2" );
						RiColor( currentShader.rmColor );
					}

					if(ribNode->shading.opacity.r != -1.0)
					{
						RtColor rOpacity;
						rOpacity[0] = ribNode->shading.opacity[0];
						rOpacity[1] = ribNode->shading.opacity[1];
						rOpacity[2] = ribNode->shading.opacity[2];
						RiOpacity( rOpacity );
					}
					else
						RiOpacity( currentShader.rmOpacity );
					liqRIBMsg("[2] liqglo_currentJob.isShadow=%d, currentShader.outputInShadow=%d", liqglo_currentJob.isShadow, currentShader.outputInShadow );
					//====>>>>>  WILL BE DONE IN liqShader::write  -end
					
					// per shader shadow pass override
					if( !liqglo_currentJob.isShadow || currentShader.outputInShadow )
					{
						currentShader.write();
					}
				}
			} 
			else //if( hasSurfaceShader && !m_ignoreSurfaces )
			{
				RtColor rColor,rOpacity;
				if( m_shaderDebug ) 
				{
					liqRIBMsg("shader debug is turned on, so the object is red.");
					// shader debug on !! everything goes red and opaque !!!
					rColor[0] = 1.;
					rColor[1] = 0.;
					rColor[2] = 0.;
					RiArchiveRecord( RI_COMMENT, "mark3" );
					RiColor( rColor );

					rOpacity[0] = 1.;
					rOpacity[1] = 1.;
					rOpacity[2] = 1.;
					RiOpacity( rOpacity );
				} 
				else 
				{
					if(ribNode->shading.color.r != -1.0) 
					{
						rColor[0] = ribNode->shading.color[0];
						rColor[1] = ribNode->shading.color[1];
						rColor[2] = ribNode->shading.color[2];
						RiArchiveRecord( RI_COMMENT, "mark4" );
						RiColor( rColor );
					} 
					else if( ( ribNode->color.r != -1.0 ) ) 
					{
						rColor[0] = ribNode->color[0];
						rColor[1] = ribNode->color[1];
						rColor[2] = ribNode->color[2];
						RiArchiveRecord( RI_COMMENT, "mark5" );
						RiColor( rColor );
					}
					if(ribNode->shading.opacity.r != -1.0) 
					{
						rOpacity[0] = ribNode->shading.opacity[0];
						rOpacity[1] = ribNode->shading.opacity[1];
						rOpacity[2] = ribNode->shading.opacity[2];
						RiOpacity( rOpacity );
					} 
					else if( ( ribNode->opacity.r != -1.0 ) ) 
					{
						rOpacity[0] = ribNode->opacity[0];
						rOpacity[1] = ribNode->opacity[1];
						rOpacity[2] = ribNode->opacity[2];
						RiOpacity( rOpacity );
					}
				}

				if( !m_ignoreSurfaces ) 
				{
					MObject shadingGroup = ribNode->assignedShadingGroup.object();
					MObject shader = ribNode->findShader();
					//
					// here we check for regular shader nodes first
					// and assign default shader to shader-less nodes.
					//
					if( m_shaderDebug ) {
						liqRIBMsg("shader debug is turned on, so the surface is constant.");
						RiSurface( "constant", RI_NULL );
						LIQDEBUGPRINTF("add more constant parameters here. take /RMS-1.0.1-Maya2008/lib/shaders/src/mtorBlinn.sl as an example.(?)");
					}
// 					else if( shader.apiType() == MFn::kLambert ){ 
// 						RiSurface( "matte", RI_NULL );
// 						LIQDEBUGPRINTF("add more lambert parameters here. take //RMS-1.0.1-Maya2008/lib/shaders/src/mtorLambert.sl as an example.");
// 					}else if( shader.apiType() == MFn::kPhong ) {
// 						RiSurface( "plastic", RI_NULL );
// 						LIQDEBUGPRINTF("add more phong parameters here. take /RMS-1.0.1-Maya2008/lib/shaders/src/mtorPhong.sl as an example.");
// 					}
					else if( path.hasFn( MFn::kPfxHair ) ) 
					{
						// get some of the hair system parameters
						RtFloat translucence = 0, specularPower = 0;
						RtColor specularColor;
						//cout <<"getting pfxHair shading params..."<<endl;
						MObject hairSystemObj;
						MFnDependencyNode pfxHairNode( path.node() );
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
						RiSurface(  "liquidpfxhair",
							"float specularpower", &specularPower,
							"float translucence",  &translucence,
							"color specularcolor", &specularColor,
							RI_NULL );
					} 
					else if( path.hasFn( MFn::kPfxToon ) ) {
						RiSurface( "liquidpfxtoon", RI_NULL );
					}else if( path.hasFn( MFn::kPfxGeometry ) ){
						RiSurface( "liquidpfx", RI_NULL );
					}else {
						//RiSurface( "plastic", RI_NULL );
						MFnDependencyNode shaderFn(shader);
						RiSurface( const_cast<char*>(shaderFn.name().asChar()), RI_NULL );
					}
				}
			}//if( hasSurfaceShader && !m_ignoreSurfaces )else
		} //if( writeShaders ) 
		else if( liqglo_currentJob.deepShadows ) 
		{
			liqRIBMsg("[7] liqglo_currentJob[.deepShadows=%d, .isShadow=%d ], hasSurfaceShader=%d, hasCustomSurfaceShader=%d",
				liqglo_currentJob.deepShadows, liqglo_currentJob.isShadow, hasSurfaceShader, hasCustomSurfaceShader );

			// if the current job is a deep shadow,
			// we still want to output primitive color and opacity and surface shader
			// In case of custom shaders, what should we do ? Stephane.
			if( hasSurfaceShader && ! hasCustomSurfaceShader ) 
			{
				//liqShader & currentShader = liqGetShader( ribNode->assignedShader.object());
				liqShader &currentShader = liqShaderFactory::instance().getShader( ribNode->assignedShader.object() );

				// Output color overrides or color
				if(ribNode->shading.color.r != -1.0) 
				{
					RtColor rColor;
					rColor[0] = ribNode->shading.color[0];
					rColor[1] = ribNode->shading.color[1];
					rColor[2] = ribNode->shading.color[2];
					RiArchiveRecord( RI_COMMENT, "mark6" );
					RiColor( rColor );
				} 
				else {
					RiArchiveRecord( RI_COMMENT, "mark7" );
					RiColor( currentShader.rmColor );
				}

				if(ribNode->shading.opacity.r != -1.0) 
				{
					RtColor rOpacity;
					rOpacity[0] = ribNode->shading.opacity[0];
					rOpacity[1] = ribNode->shading.opacity[1];
					rOpacity[2] = ribNode->shading.opacity[2];
					RiOpacity( rOpacity );
				} 
				else 
					RiOpacity( currentShader.rmOpacity );
				
				liqRIBMsg("[8] currentShader[.name=%s, .filename=%s, .outputInShadow=%d]", currentShader.getName().c_str(), currentShader.getShaderFileName().c_str(), currentShader.outputInShadow );
				if(currentShader.outputInShadow){
					currentShader.write();
				}

			} 
			else 
			{
				RtColor rColor,rOpacity;

				if(ribNode->shading.color.r != -1.0) 
				{
					rColor[0] = ribNode->shading.color[0];
					rColor[1] = ribNode->shading.color[1];
					rColor[2] = ribNode->shading.color[2];
					RiArchiveRecord( RI_COMMENT, "mark8" );
					RiColor( rColor );
				} 
				else if( ( ribNode->color.r != -1.0 ) ) 
				{
					rColor[0] = ribNode->color[0];
					rColor[1] = ribNode->color[1];
					rColor[2] = ribNode->color[2];
					RiArchiveRecord( RI_COMMENT, "mark9" );
					RiColor( rColor );
				}
				if(ribNode->shading.opacity.r != -1.0) 
				{
					rOpacity[0] = ribNode->shading.opacity[0];
					rOpacity[1] = ribNode->shading.opacity[1];
					rOpacity[2] = ribNode->shading.opacity[2];
					RiOpacity( rOpacity );
				} 
				else if( ( ribNode->opacity.r != -1.0 ) ) 
				{
					rOpacity[0] = ribNode->opacity[0];
					rOpacity[1] = ribNode->opacity[1];
					rOpacity[2] = ribNode->opacity[2];
					RiOpacity( rOpacity );
				}
				if( path.hasFn( MFn::kPfxHair ) ) 
				{
					// get some of the hair system parameters
					float translucence = 0, specularPower = 0;
					float specularColor[3];
					//cout <<"getting pfxHair shading params..."<<endl;
					MObject hairSystemObj;
					MFnDependencyNode pfxHairNode( path.node() );
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
							//cout <<"specularColor = "<<specularColor<<endl;
						}
						status.clear();
						paramPlug = hairSystemNode.findPlug( "specularPower", &status );
						if( status == MS::kSuccess ) 
							paramPlug.getValue( specularPower );
					}
					RiSurface(  "liquidPfxHair",
						"float specularPower", &specularPower,
						"float translucence",  &translucence,
						"color specularColor", &specularColor,
						RI_NULL );
				}
			}
		} 
		else 
			RiSurface( "null", RI_NULL );

		liqRIBMsg("[4] hasDisplacementShader=%d, m_ignoreDisplacements=%d", hasDisplacementShader, m_ignoreDisplacements );
		if( hasDisplacementShader && !m_ignoreDisplacements ) 
		{
			//liqShader & currentShader = liqGetShader( ribNode->assignedDisp.object() );
			liqShader &currentShader = liqShaderFactory::instance().getShader( ribNode->assignedDisp.object() );
			
			liqRIBMsg("[3] liqglo_currentJob.isShadow=%d, currentShader.outputInShadow=%d", liqglo_currentJob.isShadow, currentShader.outputInShadow );
			// per shader shadow pass override
			if( !liqglo_currentJob.isShadow || currentShader.outputInShadow )
			{
				currentShader.write();
			}
		}
		if( ribNode->rib.hasBox() ) 
			RiArchiveRecord( RI_COMMENT, " RIB Box:\n%s", ribNode->rib.box.asChar() );
		if( ribNode->rib.hasGenerator() ){
			MGlobal::executeCommand( ribNode->rib.generator, false, false );
		}
		if( ribNode->rib.hasReadArchive() ) 
			RiArchiveRecord( RI_VERBATIM, " ReadArchive \"%s\" \n", ribNode->rib.readArchive.asChar() );
		if( ribNode->rib.hasDelayedReadArchive() ) 
		{
			RiArchiveRecord( RI_VERBATIM, " Procedural \"DelayedReadArchive\" [ \"%s\" ] [ %f %f %f %f %f %f ] \n", ribNode->rib.delayedReadArchive.asChar(), ribNode->bound[0],ribNode->bound[3],ribNode->bound[1],ribNode->bound[4],ribNode->bound[2],ribNode->bound[5] );
			// should be using the bounding box node - Alf
			/* {
			// this is a visual display of the archive's bounding box
			RiAttributeBegin();
			RtColor debug;
			debug[0] = debug[1] = 1;
			debug[2] = 0;
			RiColor( debug );
			debug[0] = debug[1] = debug[2] = 0.250;
			RiOpacity( debug );
			RiSurface( "defaultsurface", RI_NULL );
			RiArchiveRecord( RI_VERBATIM, "Attribute \"visibility\" \"int diffuse\" [0]\n" );
			RiArchiveRecord( RI_VERBATIM, "Attribute \"visibility\" \"int specular\" [0]\n" );
			RiArchiveRecord( RI_VERBATIM, "Attribute \"visibility\" \"int transmission\" [0]\n" );
			float xmin = ribNode->bound[0];
			float ymin = ribNode->bound[1];
			float zmin = ribNode->bound[2];
			float xmax = ribNode->bound[3];
			float ymax = ribNode->bound[4];
			float zmax = ribNode->bound[5];
			RiSides( 2 );
			RiArchiveRecord( RI_VERBATIM, "Polygon \"P\" [ %f %f %f  %f %f %f  %f %f %f  %f %f %f ]\n", xmin,ymax,zmin, xmax,ymax,zmin, xmax,ymax,zmax, xmin,ymax,zmax );
			RiArchiveRecord( RI_VERBATIM, "Polygon \"P\" [ %f %f %f  %f %f %f  %f %f %f  %f %f %f ]\n", xmin,ymin,zmin, xmax,ymin,zmin, xmax,ymin,zmax, xmin,ymin,zmax );
			RiArchiveRecord( RI_VERBATIM, "Polygon \"P\" [ %f %f %f  %f %f %f  %f %f %f  %f %f %f ]\n", xmin,ymax,zmax, xmax,ymax,zmax, xmax,ymin,zmax, xmin,ymin,zmax );
			RiArchiveRecord( RI_VERBATIM, "Polygon \"P\" [ %f %f %f  %f %f %f  %f %f %f  %f %f %f ]\n", xmin,ymax,zmin, xmax,ymax,zmin, xmax,ymin,zmin, xmin,ymin,zmin );
			RiAttributeEnd();
			} */
		}
		// Alf: preShapeMel
		prePostPlug = fnTransform.findPlug( "liqPreShapeMel" );
		m_preShapeMel = prePostPlug.asString();
		if( m_preShapeMel != "" )
			MGlobal::executeCommand( m_preShapeMel );

		//refactor 31 begin to renderman::Renderer::exportOneObject_reference()

		// receive shadows ?   =>   Attribute "user" "int receivesShadows" [0/1]
 		//if( !ribNode->object(0)->receiveShadow )
		{
			int receiveShadows = ribNode->object(0)->receiveShadow;
			RiAttribute("user", (RtToken)"int receivesShadows", &receiveShadows, RI_NULL);
		}
		//refactor 31 end

		if( !ribNode->ignoreShapes ) 
		{
			liqRIBMsg("ribNode->object(0)->type= %d, path=%s",ribNode->object(0)->type, ribNode->path().fullPathName().asChar() );
			// check to see if we are writing a curve to set the proper basis
			if( ribNode->object(0)->type == MRT_NuCurve
				|| ribNode->object(0)->type == MRT_PfxHair
				|| ribNode->object(0)->type == MRT_PfxTube
				|| ribNode->object(0)->type == MRT_PfxLeaf
				|| ribNode->object(0)->type == MRT_PfxPetal 
				|| ribNode->object(0)->type == MRT_Curves )

			{
				RiBasis( RiBSplineBasis, 1, RiBSplineBasis, 1 );
			} 

			bool doMotion ( liqglo.liqglo_doDef && ribNode->motion.deformationBlur &&
				( ribNode->object(1) ) &&
				( ribNode->object(0)->type != MRT_RibGen ) &&
				//( ribNode->object(0)->type != MRT_Locator ) &&
				( !liqglo_currentJob.isShadow || liqglo_currentJob.deepShadows ) );

			if( doMotion )
			{
				// For each grain, open a new motion block...
				for( unsigned i( 0 ); i < ribNode->object( 0 )->granularity(); i++ ) //granularity() is always 1.
				{
					if( ribNode->object( 0 )->isNextObjectGrainAnimated() ) //isNextObjectGrainAnimated() is always true.
					{
						if(liqglo.liqglo_relativeMotion)
							RiMotionBeginV( liqglo.liqglo_motionSamples, liqglo.liqglo_sampleTimesOffsets );
						else
							RiMotionBeginV( liqglo.liqglo_motionSamples, liqglo.liqglo_sampleTimes );

						for ( unsigned msampleOn( 0 ); msampleOn < liqglo.liqglo_motionSamples; msampleOn++ ){ 
							MString geometryRibFile( liquidGetRelativePath( false, getLiquidRibName( ribNode->name.asChar() ), liqglo.liqglo_ribDir ) +"."+(int)liqglo.liqglo_lframe+".m"+(int)msampleOn+".rib" );
							ribNode->object( msampleOn )->writeObject(geometryRibFile, liqglo_currentJob, false);
						}

						RiMotionEnd();
					} 
					else {
						RiArchiveRecord( RI_COMMENT, "the the next object grain is not animated" );

						MString geometryRibFile( liquidGetRelativePath( false, getLiquidRibName( ribNode->name.asChar() ), liqglo.liqglo_ribDir ) +"."+(int)liqglo.liqglo_lframe+".rib" );
						ribNode->object( 0 )->writeObject(geometryRibFile, liqglo_currentJob, false);
					}
				}
			} 
			else {
				if(    ribNode->rib.hasGenerator()
					|| ribNode->rib.hasReadArchive()  
					|| ribNode->rib.hasDelayedReadArchive() 
					)
				{
					//if ribNode is tagged as readArchive or delayedReadArchive, we do not output its geometry data.
					//return;
				}else{
					//ribNode->object( 0 )->writeObject();
					_writeObject(ribNode, liqglo_currentJob, false, 0);
				}

			}

			// Alf: postShapeMel
			prePostPlug = fnTransform.findPlug( "liqPostShapeMel" );
			m_postShapeMel = prePostPlug.asString();
			if( m_postShapeMel != "" )
				MGlobal::executeCommand( m_postShapeMel );

		} // else RiArchiveRecord( RI_COMMENT, " Shapes Ignored !!" );
		RiAttributeEnd();
	}
	while ( attributeDepth > 0 ) 
	{
		RiAttributeEnd();
		attributeDepth--;
	}
	return returnStatus;
}

/**
* Write the world prologue.
* This includes the pre- and post-world begin RIB boxes and the definition of
* any default coordinate systems.
*/
MStatus liqRibTranslator::worldPrologue()
{
	CM_TRACE_FUNC("liqRibTranslator::worldPrologue()");

	MStatus returnStatus = MS::kSuccess;
	LIQDEBUGPRINTF( "-> Writing world prologue.\n" );
	// if this is a readArchive that we are exporting then ingore this header information
	if( !liqglo.m_exportReadArchive )
	{
		MFnDependencyNode globalsNode( liqglo.rGlobalObj );
		MPlug prePostplug( globalsNode.findPlug( "preWorldMel" ) );
		MString melcommand( prePostplug.asString() );
		// put in pre-worldbegin statements
		if(liqglo.m_preWorldRIB != "" || melcommand.length() )
		{
			RiArchiveRecord(RI_COMMENT,  " Pre-WorldBegin RIB from liquid globals");
			MGlobal::executeCommand( melcommand );
			RiArchiveRecord(RI_VERBATIM, ( char* )liqglo.m_preWorldRIB.asChar());
			RiArchiveRecord(RI_VERBATIM, "\n");
		}
		// output the arbitrary clipping planes here /////////////
		//
		{
			for ( RNMAP::iterator rniter( htable->RibNodeMap.begin() ); rniter != htable->RibNodeMap.end(); rniter++ ) 
			{
				LIQ_CHECK_CANCEL_REQUEST;
				liqRibNodePtr   ribNode = (*rniter).second;
				if( ribNode->object(0)->ignore || ribNode->object(0)->type != MRT_ClipPlane ) 
					continue;
				RiTransformBegin();
				if( liqglo.m_outputComments )
					RiArchiveRecord( RI_COMMENT, "Clipping Plane: %s", ribNode->name.asChar(), RI_NULL );
				RtMatrix ribMatrix;
				MMatrix matrix;
				MDagPath path;

				matrix = ribNode->object(0)->matrix( path.instanceNumber() );
				matrix.get( ribMatrix );
				RiConcatTransform( ribMatrix );

				ribNode->object(0)->writeObject("", liqglo_currentJob, false);
				ribNode->object(0)->written = 1;

				RiTransformEnd();
			}
		}
		RiWorldBegin();
		// set attributes from the globals
		if( liqglo.rt_useRayTracing )
		{
			RiArchiveRecord(RI_COMMENT,  " Ray-Tracing Attributes from liquid globals");
			RtInt on( 1 );
			RiAttribute("visibility", "int trace", &on, RI_NULL );
			if( liqglo.rt_traceDisplacements )
				RiAttribute("trace", "int displacements", &on, RI_NULL );
			if( liqglo.rt_traceSampleMotion )
				RiAttribute("trace", "int samplemotion", &on, RI_NULL );
			if( liqglo.rt_traceBias != 0 )
				RiAttribute("trace", "float bias", &liqglo.rt_traceBias, RI_NULL );
			RiAttribute("trace", "int maxdiffusedepth", &liqglo.rt_traceMaxDiffuseDepth, RI_NULL);
			RiAttribute("trace", "int maxspeculardepth", &liqglo.rt_traceMaxSpecularDepth, RI_NULL);
			if( liqglo.rt_irradianceMaxError != -1.0 )
				RiAttribute( "irradiance", (RtToken) "float maxerror", &liqglo.rt_irradianceMaxError, RI_NULL );
			if( liqglo.rt_irradianceMaxPixelDist != -1.0 )
				RiAttribute( "irradiance", (RtToken) "float maxpixeldist", &liqglo.rt_irradianceMaxPixelDist, RI_NULL );
 
		// ymesh: add photon/caustic map attribites
		  if (  liqglo.rt_photonGlobalHandle != "" || liqglo.rt_causticGlobalHandle != "") 
		  {
			MString parsedName = parseString( liqglo.rt_photonGlobalHandle, false );  //  doEscaped = false

			RtString photon_map = const_cast< char* >( parsedName.asChar() );
			RiAttribute( "photon", (RtToken) "globalmap", &photon_map, RI_NULL );
        
			parsedName = parseString( liqglo.rt_causticGlobalHandle, false );  //  doEscaped = false
			RtString caustic_map = const_cast< char* >( parsedName.asChar() );
			RiAttribute( "photon", (RtToken) "causticmap", &caustic_map, RI_NULL );
      
			RtString model;
			switch ( liqglo.rt_photonShadingModel  ) 
			{
			  case liqRibNode::photon::SHADINGMODEL_GLASS:
				model = "glass";
				break;
			  case liqRibNode::photon::SHADINGMODEL_WATER:
				model = "water";
				break;
			  case liqRibNode::photon::SHADINGMODEL_CHROME:
				model = "chrome";
				break;
			  case liqRibNode::photon::SHADINGMODEL_TRANSPARENT:
				model = "chrome";
				break;
			  case liqRibNode::photon::SHADINGMODEL_DIALECTRIC:
				model = "dielectric";
				break;
			  case liqRibNode::photon::SHADINGMODEL_MATTE:
			  default:
				model = "matte";
			}
			RiAttribute( "photon", (RtToken) "shadingmodel", &model, RI_NULL );
      
			RtInt estimator = liqglo.rt_photonEstimator;
			RiAttribute( "photon", (RtToken) "estimator", &estimator, RI_NULL );
		  }
		}
		// put in post-worldbegin statements
		prePostplug = globalsNode.findPlug( "postWorldMel" );
		melcommand = prePostplug.asString();
		if(liqglo.m_postWorldRIB != "" || melcommand.length() )
		{
			RiArchiveRecord( RI_COMMENT,  " Post-WorldBegin RIB from liquid globals" );
			MGlobal::executeCommand( melcommand );
			RiArchiveRecord( RI_VERBATIM, ( char* )liqglo.m_postWorldRIB.asChar() );
			RiArchiveRecord( RI_VERBATIM, "\n");
		}
		RiTransformBegin();
		RiCoordinateSystem( "worldspace" );
		RiTransformEnd();

		RiTransformBegin();
		RiRotate( -90., 1., 0., 0. );
		RiCoordinateSystem( "_environment" );
		RiTransformEnd();
	}
	//RiReverseOrientation();//???
	return returnStatus;
}

/**
* Write the world epilogue.
* This basically calls RiWorldEnd().
*/
MStatus liqRibTranslator::worldEpilogue()
{
	CM_TRACE_FUNC("liqRibTranslator::worldEpilogue()");

	return liquid::RendererMgr::getInstancePtr()->getRenderer()->worldEpilogue();
}

/**
* Write all coordinate systems.
* This writes all user-defined coordinate systems as well as those required
* for environment/reflection map lookup and texture projection.
*/
MStatus liqRibTranslator::coordSysBlock()
{
	CM_TRACE_FUNC("liqRibTranslator::coordSysBlock()");

	MStatus returnStatus = MS::kSuccess;
	LIQDEBUGPRINTF( "-> Writing coordinate systems.\n" );
	RNMAP::iterator rniter;
	for ( rniter = htable->RibNodeMap.begin(); rniter != htable->RibNodeMap.end(); rniter++ ) 
	{
		LIQ_CHECK_CANCEL_REQUEST;
		liqRibNodePtr   ribNode = (*rniter).second;
		if( ribNode->object(0)->ignore || ribNode->object(0)->type != MRT_Coord ) 
			continue;
		if( liqglo.m_outputComments )
			RiArchiveRecord( RI_COMMENT, "Name: %s", ribNode->name.asChar(), RI_NULL );

		RiAttributeBegin();
		RiAttribute( "identifier", "name", &getLiquidRibName( ribNode->name.asChar() ), RI_NULL );

		RtMatrix ribMatrix;
		MMatrix matrix;
		MDagPath path;

		matrix = ribNode->object(0)->matrix( path.instanceNumber() );
		matrix.get( ribMatrix );
		if( liqglo.liqglo_relativeTransforms ) 
			RiConcatTransform( ribMatrix ); 
		else 
			RiTransform( ribMatrix );

		ribNode->object(0)->writeObject("", liqglo_currentJob, false);
		ribNode->object(0)->written = 1;
		RiAttributeEnd();
	}
	return returnStatus;
}

/**
* Write all lights.
* This writes all lightsource shaders with their attributes and switches them
* on afterwards.
*/
MStatus liqRibTranslator::lightBlock()
{
	CM_TRACE_FUNC("liqRibTranslator::lightBlock()");

	MStatus returnStatus = MS::kSuccess;
	LIQDEBUGPRINTF( "-> Writing lights.\n" );
	// If this is a readArchive that we are exporting then ignore this header information
	if( !liqglo.m_exportReadArchive )
	{
		RNMAP::iterator rniter;
		int nbLight = 0;
		for ( rniter = htable->RibNodeMap.begin(); rniter != htable->RibNodeMap.end(); rniter++ )
		{
			RtInt on( 1 );
			LIQ_CHECK_CANCEL_REQUEST;
			liqRibNodePtr   ribNode = (*rniter).second;

			if( ribNode->object(0)->ignore || ribNode->object(0)->type != MRT_Light )
				continue;
			// We need to enclose lights in attribute blocks because of the
			// new added attribute support
			RiAttributeBegin();

			// All this stuff below should be handled by a new attribute class
			LIQDEBUGPRINTF( "-> RibNodeName " );
			RtString RibNodeName = getLiquidRibName( ribNode->name.asChar() );
			LIQDEBUGPRINTF( "= %s.\n", (char *)RibNodeName  );
			RiAttribute( "identifier", "name", &RibNodeName, RI_NULL );
			if( ribNode->trace.sampleMotion )
				RiAttribute( "trace", (RtToken) "samplemotion", &on, RI_NULL );
			if( ribNode->trace.displacements )
				RiAttribute( "trace", (RtToken) "displacements", &on, RI_NULL );
			if( ribNode->trace.bias != 0.01f )
			{
				RtFloat bias = ribNode->trace.bias;
				RiAttribute( "trace", (RtToken) "bias", &bias, RI_NULL );
			}
			if( ribNode->trace.maxDiffuseDepth != 1 )
			{
				RtInt mddepth = ribNode->trace.maxDiffuseDepth;
				RiAttribute( "trace", (RtToken) "maxdiffusedepth", &mddepth, RI_NULL );
			}
			if( ribNode->trace.maxSpecularDepth != 2 )
			{
				RtInt msdepth = ribNode->trace.maxSpecularDepth;
				RiAttribute( "trace", (RtToken) "maxspeculardepth", &msdepth, RI_NULL );
			}
			ribNode->object(0)->writeObject("", liqglo_currentJob, false);
			ribNode->object(0)->written = 1;
			// The next line pops the light...
			RiAttributeEnd();
			// ...so we have to switch it on again explicitly
			// if exclusive Lightlinking is set
			if( liqglo.m_illuminateByDefault )
				RiIlluminate( ribNode->object(0)->lightHandle(), 1 );
			else
				RiIlluminate( ribNode->object(0)->lightHandle(), 0 );
			nbLight++;
		}
	}
	return returnStatus;
}

void liqRibTranslator::setOutDirs()
{
//	CM_TRACE_FUNC("liqRibTranslator::setOutDirs()");

	MStatus gStatus;
    MString varVal;

	MFnDependencyNode rGlobalNode( liqglo.rGlobalObj );

	liquidGetPlugValue( rGlobalNode, "ribDirectory", varVal, gStatus );
	if ( gStatus == MS::kSuccess ) 
		if ( varVal != "" ) 
			liqglo.liqglo_ribDir = removeEscapes( parseString( varVal, false ) );  

	liquidGetPlugValue( rGlobalNode, "textureDirectory", varVal, gStatus );
	if ( gStatus == MS::kSuccess ) 
		if ( varVal != "" ) 
			liqglo.liqglo_textureDir = removeEscapes( parseString( varVal, false ) );  

	liquidGetPlugValue( rGlobalNode, "pictureDirectory", varVal, gStatus );
	if ( gStatus == MS::kSuccess ) 
		if ( varVal != "" ) 
			liqglo.m_pixDir = removeEscapes( parseString( varVal, false ) );  
}


void liqRibTranslator::setSearchPaths()
{
//	CM_TRACE_FUNC("liqRibTranslator::setSearchPaths()");

	//trace log is not created, can't call CM_TRACE_FUNC()
	liqglo.liqglo_shaderPath = "&:@:.:~:rmanshader";
	liqglo.liqglo_texturePath = "&:@:.:~"; // :rmantex
	liqglo.liqglo_archivePath = "&:@:.:~"; // :rib
	liqglo.liqglo_proceduralPath = "&:@:.:~";

	MString tmphome( getenv( "LIQUIDHOME" ) );
	tmphome = liquidSanitizeSearchPath( tmphome );

	if( tmphome != "" ) 
	{
		liqglo.liqglo_shaderPath += ":" + tmphome + "/lib/shaders";
		liqglo.liqglo_texturePath += ":" + tmphome + "/lib/textures";
		liqglo.liqglo_archivePath += ":" + tmphome + "/lib/rib";
		liqglo.liqglo_proceduralPath += ":" + tmphome + "/lib/plugins";
	}

	liqglo.liqglo_shaderPath += ":" + liqglo.liqglo_projectDir;
	liqglo.liqglo_texturePath += ":" + liqglo.liqglo_projectDir;
	liqglo.liqglo_archivePath += ":" + liqglo.liqglo_projectDir;
	liqglo.liqglo_proceduralPath += ":" + liqglo.liqglo_projectDir;

	MStatus gStatus;
    MString varVal;
	MFnDependencyNode rGlobalNode( liqglo.rGlobalObj );

	{
		liquidGetPlugValue( rGlobalNode, "shaderPath", varVal, gStatus );
		if ( gStatus == MS::kSuccess ) 
			if ( varVal != "" ) 
			{
				tmphome += "/shaders";
				if( varVal.index( ':' ) == 0 )
					liqglo.liqglo_shaderPath += ":" + removeEscapes( parseString( varVal, false ) );
				else if( varVal.rindex( ':' ) == 0 )
					liqglo.liqglo_shaderPath = removeEscapes( parseString( varVal, false ) ) + liqglo.liqglo_shaderPath;
				else
					liqglo.liqglo_shaderPath = removeEscapes( parseString( varVal, false ) ) + ":" + tmphome;
			}
	}
	{
		liquidGetPlugValue( rGlobalNode, "texturePath", varVal, gStatus );
		if ( gStatus == MS::kSuccess ) 
			if ( varVal != "" ) 
			{
				if( varVal.index( ':' ) == 0 )
					liqglo.liqglo_texturePath += ":" + removeEscapes( parseString( varVal, false ) );
				else if( varVal.rindex( ':' ) == 0 )
					liqglo.liqglo_texturePath = removeEscapes( parseString( varVal, false ) ) + liqglo.liqglo_texturePath;
				else
					liqglo.liqglo_texturePath = removeEscapes( parseString( varVal, false ) );
			}
	}
	{
		liquidGetPlugValue( rGlobalNode, "archivePath", varVal, gStatus );
		if ( gStatus == MS::kSuccess ) 
			if ( varVal != "" ) 
			{
				if( varVal.index( ':' ) == 0 )
					liqglo.liqglo_archivePath += ":" + removeEscapes( parseString( varVal, false ) );
				else if( varVal.rindex( ':' ) == 0 )
					liqglo.liqglo_archivePath = removeEscapes( parseString( varVal, false ) ) + liqglo.liqglo_archivePath;
				else
					liqglo.liqglo_archivePath = removeEscapes( parseString( varVal, false ) );
			}
	}
	{
		liquidGetPlugValue( rGlobalNode, "proceduralPath", varVal, gStatus );
		if ( gStatus == MS::kSuccess ) 
			if ( varVal != "" ) 
			{
				if( varVal.index( ':' ) == 0 )
					liqglo.liqglo_proceduralPath += ":" + removeEscapes( parseString( varVal, false ) );
				else if( varVal.rindex( ':' ) == 0 )
					liqglo.liqglo_proceduralPath = removeEscapes( parseString( varVal, false ) ) + liqglo.liqglo_proceduralPath;
				else
					liqglo.liqglo_proceduralPath = removeEscapes( parseString( varVal, false ) );
			}
	}
}

bool liqRibTranslator::renderFrameSort( const structJob& a, const structJob& b )
{
	CM_TRACE_FUNC("liqRibTranslator::renderFrameSort("<<a.name.asChar()<<","<<b.name.asChar()<<")");

	long v1 = ( a.isShadow )? a.renderFrame : 100000000;
	long v2 = ( b.isShadow )? b.renderFrame : 100000000;
	return v1 < v2;
}

void liqRibTranslator::_writeObject(
	const liqRibNodePtr& ribNode, 
	const structJob &currentJob,
	const bool bGeometryMotionBlur,
	const unsigned int msampleOn
	)
{
	CM_TRACE_FUNC("liqRibTranslator::_writeObject("<<ribNode->name.asChar()<<","<<currentJob.name.asChar()<<","<<bGeometryMotionBlur<<","<<msampleOn<<")");

// 	MString frame; 
// 	frame.set(liqglo.liqglo_lframe);

	MString MotionPostfix;
	unsigned int sample;
	if( bGeometryMotionBlur ){
		MString MSampleOn;
		MSampleOn.set((int)msampleOn);
		MotionPostfix = ".m"+MSampleOn;
		sample        = msampleOn;
	}else{
		MotionPostfix = "";
		sample        = 0;
	}
    
	MString geometryRibFile( 
				liquidGetRelativePath( 
					false, 
					getLiquidRibName( ribNode->name.asChar() ), 
					liqglo.liqglo_ribDir 
				) +"."+(int)liqglo.liqglo_lframe+MotionPostfix+".rib" 
			);
	ribNode->object( sample )->writeObject(geometryRibFile, currentJob, false);
}
//
bool liqRibTranslator::canExport()
{
	//CM_TRACE_FUNC("liqRibTranslator::canExport()");
	//
	if( liqglo.m_displays[0].name.length()==0 )
	{
		assert(0&&"liqglo.m_displays[ 0 ].name is empty. Please set the output image and render the scene again.");
		liquidMessage2(messageError,"liqglo.m_displays[ 0 ].name is empty. Please set the output image and render the scene again.");
		return false;
	}
	//check liquid required project directories.
	int checkDirecoties=0;
	IfMErrorWarn(MGlobal::executeCommand( "source liquidCheck.mel; liquidCheckProjectDirectories()", checkDirecoties));
	if(0==checkDirecoties){
		liquidMessage2(messageError,"liquidCheckProjectDirectories() fails, see script editor for more details.");
		return false;
	}
	//
	return liquid::RendererMgr::getInstancePtr()->getRenderer()->canExport();
}

std::string liqRibTranslator::getFunctionTraceLogFileName() const
{
	//CM_TRACE_FUNC("liqRibTranslator::getFunctionTraceLogFileName()");

	std::stringstream sslogFileName;
	{
		//fullpath name
#ifndef _DEBUG
		liquidMessage2(messageError, "liqglo.liqglo_lframe is not initilized in Release mode. so when the program goes here liqglo.liqglo_lframe is random value, I set it to 0 manully. You can ingore this error.");
		liqglo.liqglo_lframe = 0;
#endif
		MString imageName( liqglo.liqglo_projectDir+liqglo.m_pixDir );
		imageName += parseString( liqglo.m_displays[ 0 ].name, false );


		sslogFileName << boost::format("%s.log")%imageName.asChar();
	}
	return sslogFileName.str();
}