#include <liqConfig.h>
#ifdef _USE_RENDERMAN_

#include <maya/MGlobal.h>
#include <maya/MSelectionList.h>
#include <maya/MPlug.h>
#include <maya/MFnMesh.h>

#include "rm_renderer.h"
#include <liqPreviewShader.h>
#include "rm_helper.h"
#include <liqGlobalVariable.h>

namespace renderman
{
	//
	int Renderer::preview( const std::string& fileName, const liqPreviewShaderOptions& options )
	{
		CM_TRACE_FUNC("Renderer::preview("<<fileName<<","<<options.shaderNodeName<<")");

		  std::string shaderFileName;
  liqShader currentShader;
  MObject	shaderObj;
  MString shader_type_TempForRefactoring;//  [2/14/2012 yaoyansi]

  if ( options.fullShaderPath ) 
  {
	  // a full shader path was specified
	  //cout <<"[liquid] got full path for preview !"<<endl;
	  //shaderFileName = const_cast<char*>(options.shaderNodeName);

	  std::string tmp( options.shaderNodeName );
	  currentShader.setShaderFileName(tmp.substr( 0, tmp.length() -  4 ) );

	  if ( options.type == "surface" ){
		  assert(0&&"we should use currentShader.shader_type_ex = \"surface\"");
		  //currentShader.shader_type = SHADER_TYPE_SURFACE;//  [2/14/2012 yaoyansi]
		  shader_type_TempForRefactoring = "surface";
	  }else if ( options.type == "displacement" ){
		  assert(0&&"we should use currentShader.shader_type_ex = \"displacement\"");
		  //currentShader.shader_type = SHADER_TYPE_DISPLACEMENT;//  [2/14/2012 yaoyansi]
		  shader_type_TempForRefactoring = "displacement";
	  }
	  //cout <<"[liquid]   options.shaderNodeName = " << options.shaderNodeName << endl;
	  //cout <<"[liquid]   options.type = "<<options.type<<endl;
  } 
  else 
  {
	  // a shader node was specified
	  MSelectionList shaderNameList;
	  shaderNameList.add( options.shaderNodeName.c_str() );
	  shaderNameList.getDependNode( 0, shaderObj );
	  if( shaderObj == MObject::kNullObj )
	  {
		  MGlobal::displayError( std::string( "Can't find node for " + options.shaderNodeName ).c_str() );
		  RiEnd();
		  return 0;
	  }
	  currentShader = liqShader( shaderObj );
	  shader_type_TempForRefactoring = currentShader.shader_type_ex;
	  shaderFileName = currentShader.getShaderFileName();
  }
  MFnDependencyNode assignedShader( shaderObj );


  // Get the Pathes in globals node
  MObject globalObjNode;
  MString liquidShaderPath = "",liquidTexturePath = "",liquidProceduralPath = "";
  MStatus status;
  MSelectionList globalList;
	
	// get the current project directory
  MString MELCommand = "workspace -q -rd";
  MString MELReturn;
  MGlobal::executeCommand( MELCommand, MELReturn );
  MString liquidProjectDir = MELReturn;
	
  status = globalList.add( "liquidGlobals" );
  if ( globalList.length() > 0 ) 
	{
    status.clear();
    status = globalList.getDependNode( 0, globalObjNode );
    MFnDependencyNode globalNode( globalObjNode );
		liquidGetPlugValue( globalNode, "shaderPath", liquidShaderPath, status);
    liquidGetPlugValue( globalNode, "texturePath", liquidTexturePath, status);
    liquidGetPlugValue( globalNode, "proceduralPath", liquidProceduralPath, status);
  }
  if( fileName.empty() ) 
	{
    RiBegin_liq( NULL );
#ifdef DELIGHT
    //RtPointer callBack( progressCallBack );
    //RiOption( "statistics", "progresscallback", &callBack, RI_NULL );
#endif
  } 
	else {
	liquidMessage2(messageInfo,"preview rib file: %s", fileName.c_str());
    RiBegin_liq( (RtToken)fileName.c_str() );
  }

  std::string liquidHomeDir = liquidSanitizeSearchPath( getEnvironment( "LIQUIDHOME" ) );
  std::string shaderPath( "&:@:.:~:" + liquidHomeDir + "/lib/shaders" );
  std::string texturePath( "&:@:.:~:" + liquidHomeDir + "/lib/textures" );
  std::string proceduralPath( "&:@:.:~:" + liquidHomeDir + "/lib/shaders" );

  std::string projectDir = std::string( liquidSanitizeSearchPath( liquidProjectDir ).asChar() );
  if ( liquidProjectDir != "")
  {
    shaderPath += ":" + projectDir;	
    texturePath += ":" + projectDir;
    proceduralPath += ":" + projectDir;
  }
  if ( liquidShaderPath != "" )
		shaderPath += ":" + std::string( liquidSanitizeSearchPath( parseString( liquidShaderPath, false ) ).asChar());	
  if ( liquidTexturePath != "" )
		texturePath += ":" + std::string( liquidSanitizeSearchPath( parseString( liquidTexturePath, false) ).asChar());	
  if ( liquidProceduralPath != "" )
		proceduralPath += ":" + std::string( liquidSanitizeSearchPath( parseString( liquidProceduralPath, false) ).asChar());	
	
  RtString list( const_cast< RtString >( shaderPath.c_str() ) );
  RiOption( "searchpath", "shader", &list, RI_NULL );
	
  RtString texPath( const_cast< RtString >( texturePath.c_str() ) );
  if( texPath[ 0 ] )
    RiOption( "searchpath","texture", &texPath, RI_NULL );
	
  RtString procPath( const_cast< RtString >( proceduralPath.c_str() ) );
  
  if( procPath[ 0 ] )
    RiOption( "searchpath","procedural", &procPath, RI_NULL );

  RiShadingRate( ( RtFloat )options.shadingRate );
  RiPixelSamples( options.pixelSamples, options.pixelSamples );

#ifdef PRMAN
  if ( MString( "PRMan" ) == liqglo.liquidRenderer.renderName )
	RiPixelFilter( RiCatmullRomFilter, 4., 4. );
#elif defined( DELIGHT )
  if ( MString( "3Delight" ) == liqglo.liquidRenderer.renderName )
    RiPixelFilter( RiSeparableCatmullRomFilter, 4., 4. );
//    RiPixelFilter( RiMitchellFilter, 4., 4.);
#else
  RiPixelFilter( RiCatmullRomFilter, 4., 4. );
#endif

  RiFormat( ( RtInt )options.displaySize, ( RtInt )options.displaySize, 1.0 );
  if( options.backPlane ) 
	{
    RiDisplay( const_cast< RtString >( options.displayName.c_str() ),
               const_cast< RtString >( options.displayDriver.c_str() ), RI_RGB, RI_NULL );
  } 
	else 
	{ // Alpha might be useful
    RiDisplay( const_cast< RtString >( options.displayName.c_str() ),
               const_cast< RtString >( options.displayDriver.c_str() ), RI_RGBA, RI_NULL );
  }
  RtFloat fov( 22.5 );
  RiProjection( "perspective", "fov", &fov, RI_NULL );
  RiTranslate( 0, 0, 2.75 );
  RiExposure(1, currentShader.m_previewGamma);
  
  RtInt visible = 1;
  RtString transmission = "transparent";

  RiAttribute( "visibility", ( RtToken ) "camera", &visible, RI_NULL );
  RiAttribute( "visibility",  ( RtToken ) "trace", &visible, RI_NULL );
  // RiAttribute( "visibility", ( RtToken ) "transmission", ( RtPointer ) &transmission, RI_NULL );
  
  RiWorldBegin();
  RiReverseOrientation();
  RiTransformBegin();
  RiRotate( -90., 1., 0., 0. );
  RiCoordinateSystem( "_environment" );
  RiTransformEnd();
  RtLightHandle ambientLightH, directionalLightH;
  RtFloat intensity;
  intensity = 0.05 * (RtFloat)options.previewIntensity;
  ambientLightH = RiLightSource( "ambientlight", "intensity", &intensity, RI_NULL );
  intensity = 0.9 * (RtFloat)options.previewIntensity;
  RtPoint from;
  RtPoint to;
  from[0] = -1.; from[1] = 1.5; from[2] = -1.;
  to[0] = 0.; to[1] = 0.; to[2] = 0.;
  RiTransformBegin();
    RiRotate( 55.,  1, 0, 0 );
    RiRotate( 30.,  0, 1, 0 );
    directionalLightH = RiLightSource( "liquiddistant", "intensity", &intensity, RI_NULL );
  RiTransformEnd();
  intensity = 0.2f * (RtFloat)options.previewIntensity;
  from[0] = 1.3f; from[1] = -1.2f; from[2] = -1.;
  RiTransformBegin();
    RiRotate( -50.,  1, 0, 0 );
    RiRotate( -40.,  0, 1, 0 );
    directionalLightH = RiLightSource( "liquiddistant", "intensity", &intensity, RI_NULL );
  RiTransformEnd();

  RiAttributeBegin();


  //ymesh omit this section, because liqShader::writeRibAttributes() do this work in that branch
  //I don't use liqShader::writeRibAttributes(), so I use this section. -yayansi
  float displacementBounds = 0.;
	liquidGetPlugValue( assignedShader, "displacementBound", displacementBounds, status);
  
  if ( displacementBounds > 0. ) 
	{
    RtString coordsys = "shader";
	RiArchiveRecord( RI_COMMENT, "ymesh omit this section, because liqShader::writeRibAttributes do this work in that branch" );
    RiAttribute( "displacementbound", "coordinatesystem", &coordsys, RI_NULL );	
    RiAttribute( "displacementbound", "sphere", &displacementBounds, "coordinatesystem", &coordsys, RI_NULL );
  }

  //LIQ_GET_SHADER_FILE_NAME( shaderFileName, options.shortShaderName, currentShader );

	// output shader space
  MString shadingSpace;
	liquidGetPlugValue( assignedShader, "shaderSpace", shadingSpace, status);
  
  if ( shadingSpace != "" ) 
	{
    RiTransformBegin();
    RiCoordSysTransform( (char*) shadingSpace.asChar() );
  }

  RiTransformBegin();
  // Rotate shader space to make the preview more interesting
  RiRotate( 60., 1., 0., 0. );
  RiRotate( 60., 0., 1., 0. );
  RtFloat scale( 1.f / ( RtFloat )options.objectScale );
  RiScale( scale, scale, scale );

  if ( shader_type_TempForRefactoring=="surface" || shader_type_TempForRefactoring=="shader"/*currentShader.shader_type == SHADER_TYPE_SURFACE || currentShader.shader_type == SHADER_TYPE_SHADER */ ) //  [2/14/2012 yaoyansi]
	{
		RiColor( currentShader.rmColor );
		RiOpacity( currentShader.rmOpacity );
		//cout << "Shader: " << shaderFileName << endl;
		if ( options.fullShaderPath ) 
				RiSurface( (RtToken)shaderFileName.c_str(), RI_NULL );
		else
		{
			liqShader liqAssignedShader( shaderObj );
			liqAssignedShader.forceAs = SHADER_TYPE_SURFACE;
			liqAssignedShader.write();
		}
  } 
	else if ( shader_type_TempForRefactoring=="displacement"/*currentShader.shader_type == SHADER_TYPE_DISPLACEMENT*/ ) //  [2/14/2012 yaoyansi]
	{
		RtToken Kd( "Kd" );
		RtFloat KdValue( 1. );
#ifdef GENERIC_RIBLIB    
    // !!! current ribLib has wrong interpretation of RiSurface parameters 
    RiSurface( "plastic", Kd, &KdValue, RI_NULL );
#else
		RiSurface( "plastic", &Kd, &KdValue, RI_NULL );
#endif    
		if ( options.fullShaderPath ) 
			RiDisplacement( (RtToken)shaderFileName.c_str(), RI_NULL );
		else 
		{
			liqShader liqAssignedShader( shaderObj );
			liqAssignedShader.forceAs = SHADER_TYPE_DISPLACEMENT;
			liqAssignedShader.write();
		}
  }
  RiTransformEnd();
  if ( shadingSpace != "" ) 
		RiTransformEnd();

 switch( options.primitiveType ) 
 {
    case CYLINDER: 
		{
      RiReverseOrientation();
      RiScale( 0.95, 0.95, 0.95 );
      RiRotate( 60., 1., 0., 0. );
      RiTranslate( 0., 0., -0.05 );
      RiCylinder( 0.5, -0.3, 0.3, 360., RI_NULL );
      RiTranslate( 0., 0., 0.3f );
      RiTorus( 0.485, 0.015, 0., 90., 360., RI_NULL );
      RiDisk( 0.015, 0.485, 360., RI_NULL );
      RiTranslate( 0., 0., -0.6 );
      RiTorus( 0.485, 0.015, 270., 360., 360., RI_NULL );
      RiReverseOrientation();
      RiDisk( -0.015, 0.485, 360., RI_NULL );
      break;
    }
    case TORUS: 
		{
      RiRotate( 45., 1., 0., 0. );
      RiTranslate( 0., 0., -0.05 );
      RiReverseOrientation();
      RiTorus( 0.3f, 0.2f, 0., 360., 360., RI_NULL );
      break;
    }
    case PLANE: 
		{
      RiScale( 0.5, 0.5, 0.5 );
      RiReverseOrientation();
      static RtPoint plane[4] = {
        { -1.,  1.,  0. },
        {  1.,  1.,  0. },
        { -1., -1.,  0. },
        {  1., -1.,  0. }
      };
      RiPatch( RI_BILINEAR, RI_P, (RtPointer) plane, RI_NULL );
      break;
    }
    case TEAPOT: 
		{
      RiTranslate( 0.06f, -0.18f, 0. );
      RiRotate( -120., 1., 0., 0. );
      RiRotate( 130., 0., 0., 1. );
      RiScale( 0.2f, 0.2f, 0.2f );
			RiArchiveRecord( RI_VERBATIM, "Geometry \"teapot\"" );
      break;
    }
    case CUBE: 
		{
      /* Lovely cube with rounded corners and edges */
      RiScale( 0.35f, 0.35f, 0.35f );
      RiRotate( 60., 1., 0., 0. );
      RiRotate( 60., 0., 0., 1. );

      RiTranslate( 0.11f, 0., -0.08f );

      RiReverseOrientation();

      static RtPoint top[ 4 ] = { { -0.95, 0.95, -1. }, { 0.95, 0.95, -1. }, { -0.95, -0.95, -1. },  { 0.95, -0.95, -1. } };
      RiPatch( RI_BILINEAR, RI_P, ( RtPointer ) top, RI_NULL );

      static RtPoint bottom[ 4 ] = { { 0.95, 0.95, 1. }, { -0.95, 0.95, 1. }, { 0.95, -0.95, 1. }, { -0.95, -0.95, 1. } };
      RiPatch( RI_BILINEAR, RI_P, ( RtPointer ) bottom, RI_NULL );

      static RtPoint right[ 4 ] = { { -0.95, -1., -0.95 }, { 0.95, -1., -0.95 }, { -0.95, -1., 0.95 }, { 0.95, -1., 0.95 } };
      RiPatch( RI_BILINEAR, RI_P, ( RtPointer ) right, RI_NULL );

      static RtPoint left[ 4 ] = { { 0.95, 1., -0.95 }, { -0.95, 1., -0.95 }, { 0.95, 1., 0.95 }, { -0.95, 1., 0.95 } };
      RiPatch( RI_BILINEAR, RI_P, ( RtPointer ) left, RI_NULL );

      static RtPoint front[ 4 ] = { {-1., 0.95, -0.95 }, { -1., -0.95, -0.95 }, { -1., 0.95, 0.95 }, { -1., -0.95, 0.95 } };
      RiPatch( RI_BILINEAR, RI_P, ( RtPointer ) front, RI_NULL );

      static RtPoint back[ 4 ] = { { 1., -0.95, -0.95 }, { 1., 0.95, -0.95 }, { 1., -0.95, 0.95 }, { 1., 0.95, 0.95 } };
      RiPatch( RI_BILINEAR, RI_P, ( RtPointer ) back, RI_NULL );

      RiTransformBegin();
      RiTranslate( 0.95, 0.95, 0. );
      RiCylinder( 0.05, -0.95, 0.95, 90., RI_NULL );
      RiTransformEnd();

      RiTransformBegin();
      RiTranslate( 0.95, -0.95, 0. );
      RiRotate( -90., 0., 0., 1. );
      RiCylinder( 0.05, -0.95, 0.95, 90., RI_NULL );
      RiTransformEnd();

      RiTransformBegin();
      RiTranslate( -0.95, 0.95, 0. );
      RiRotate( 90., 0., 0., 1. );
      RiCylinder( 0.05, -0.95, 0.95, 90., RI_NULL );
      RiTransformEnd();

      RiTransformBegin();
      RiTranslate( -0.95, -0.95, 0. );
      RiRotate( 180., 0., 0., 1. );
      RiCylinder( 0.05, -0.95, 0.95, 90., RI_NULL );
      RiTransformEnd();

      RiTransformBegin();
      RiTranslate( 0., 0., 0.95 );

      RiTransformBegin();

      RiTransformBegin();
      RiTranslate( 0.95, 0.95, 0. );
      RiSphere( 0.05, 0., 0.05, 90., RI_NULL );
      RiTransformEnd();

      RiTransformBegin();
      RiTranslate( 0.95, -0.95, 0. );
      RiRotate( -90., 0., 0., 1. );
      RiSphere( 0.05, 0., 0.05, 90., RI_NULL );
      RiTransformEnd();

      RiRotate( 180., 0., 0., 1. );

      RiTransformBegin();
      RiTranslate( 0.95, 0.95, 0. );
      RiSphere( 0.05, 0., 0.05, 90., RI_NULL );
      RiTransformEnd();

      RiTransformBegin();
      RiTranslate( 0.95, -0.95, 0. );
      RiRotate( -90., 0., 0., 1. );
      RiSphere( 0.05, 0., 0.05, 90., RI_NULL );
      RiTransformEnd();

      RiTransformEnd();

      RiRotate( 90., 1., 0., 0. );

      RiTransformBegin();
      RiTranslate( 0.95, 0., 0. );
      RiCylinder( 0.05, -0.95, 0.95, 90., RI_NULL );
      RiTransformEnd();

      RiTransformBegin();
      RiTranslate( -0.95, 0., 0. );
      RiRotate( 90., 0., 0., 1. );
      RiCylinder( 0.05, -0.95, 0.95, 90., RI_NULL );
      RiTransformEnd();

      RiRotate( 90., 0., 1., 0. );

      RiTransformBegin();
      RiTranslate( 0.95, 0.,  0. );
      RiCylinder( 0.05, -0.95, 0.95, 90., RI_NULL );
      RiTransformEnd();

      RiTransformBegin();
      RiTranslate( -0.95, 0., 0. );
      RiRotate( 90., 0., 0., 1. );
      RiCylinder( 0.05, -0.95, 0.95, 90., RI_NULL );
      RiTransformEnd();

      RiTransformEnd();

      RiTransformBegin();
      RiTranslate( 0., 0., -0.95 );

      RiTransformBegin();

      RiTransformBegin();
      RiTranslate( 0.95, 0.95, 0. );
      RiSphere( 0.05, -0.05, 0., 90., RI_NULL );
      RiTransformEnd();

      RiTransformBegin();
      RiTranslate( 0.95, -0.95, 0. );
      RiRotate( -90., 0., 0., 1. );
      RiSphere( 0.05, -0.05, 0., 90., RI_NULL );
      RiTransformEnd();

      RiRotate( 180., 0., 0., 1. );

      RiTransformBegin();
      RiTranslate( 0.95, 0.95, 0. );
      RiSphere( 0.05, -0.05, 0., 90., RI_NULL );
      RiTransformEnd();

      RiTransformBegin();
      RiTranslate( 0.95, -0.95, 0. );
      RiRotate( -90., 0., 0., 1. );
      RiSphere( 0.05, -0.05, 0., 90., RI_NULL );
      RiTransformEnd();

      RiTransformEnd();

      RiRotate( 90., 1., 0., 0. );

      RiTransformBegin();
      RiTranslate( -0.95, 0.,  0. );
      RiRotate( 180., 0., 0., 1. );
      RiCylinder( 0.05, -0.95, 0.95, 90., RI_NULL );
      RiTransformEnd();

      RiTransformBegin();
      RiTranslate( 0.95, 0.,  0. );
      RiRotate( -90., 0., 0., 1. );
      RiCylinder( 0.05, -0.95, 0.95, 90., RI_NULL );
      RiTransformEnd();

      RiRotate( 90., 0., 1., 0. );

      RiTransformBegin();
      RiTranslate( 0.95, 0.,  0. );
      RiRotate( -90., 0., 0., 1. );
      RiCylinder( 0.05, -0.95, 0.95, 90., RI_NULL );
      RiTransformEnd();

      RiTransformBegin();
      RiTranslate( -0.95, 0.,  0. );
      RiRotate( 180., 0., 0., 1. );
      RiCylinder( 0.05, -0.95, 0.95, 90., RI_NULL );
      RiTransformEnd();

      RiTransformEnd();

      break;
    }
    case CUSTOM: 
		{
      //cout <<"custom : "<<options.customRibFile<<endl;
      if ( fileExists( options.customRibFile.c_str() ) ) 
			{
        RiReadArchive( const_cast< RtToken >( options.customRibFile.c_str() ), NULL, RI_NULL );
      }
      break;
    }
    case SPHERE:
    default: 
		{
      RiRotate( 60., 1., 0., 0. );
      RiReverseOrientation();
      RiSphere( 0.5, -0.5, 0.5, 360., RI_NULL );
      break;
    }
  }

  RiAttributeEnd();
  /*
   * Backplane
   */
  if( options.backPlane ) 
	{
    if ( options.customBackplane.empty() ) 
		{
      RiAttributeBegin();
      RiScale( 0.91, 0.91, 0.91 );
      if( MString("displacement")==shader_type_TempForRefactoring/*SHADER_TYPE_DISPLACEMENT == currentShader.shader_type*/ ) //  [2/14/2012 yaoyansi]
			{
        RtColor bg = { 0.698, 0.698, 0. };
        RiColor( bg );
      } else 
        RiSurface( const_cast< RtToken >( options.backPlaneShader.c_str() ), RI_NULL );




      static RtPoint backplane[4] = {
        { -1.,  1.,  2. },
        {  1.,  1.,  2. },
        { -1., -1.,  2. },
        {  1., -1.,  2. }
      };
      RiPatch( RI_BILINEAR, RI_P, (RtPointer) backplane, RI_NULL );
      RiAttributeEnd();
    } 
		else 
		{
      if ( fileExists( options.customBackplane.c_str() ) ) 
			{
        RiAttributeBegin();
          RiScale( 1., 1., -1. );
          RiReadArchive( const_cast< RtString >( options.customBackplane.c_str() ), NULL, RI_NULL );
        RiAttributeEnd();
      }
    }
  }

  RiWorldEnd();

/* this caused maya to hang up under windoof - Alf
#ifdef _WIN32
//	Wait until the renderer is done
	while( !fileFullyAccessible( options.displayName.c_str() ) );
#endif
*/
  RiEnd();

  fflush( NULL );
	}

}//namespace renderman

#endif//_USE_RENDERMAN_
