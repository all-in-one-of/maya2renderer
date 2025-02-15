/*
**
** The contents of this file are subject to the Mozilla Public License Version 1.1 (the
** "License"); you may not use this file except in compliance with the License. You may
** obtain a copy of the License at http://www.mozilla.org/MPL/
**
** Software distributed under the License is distributed on an "AS IS" basis, WITHOUT
** WARRANTY OF ANY KIND, either express or implied. See the License for the specific
** language governing rights and limitations under the License.
**
** The Original Code is the Liquid Rendering Toolkit.
**
** The Initial Developer of the Original Code is Colin Doncaster. Portions created by
** Colin Doncaster are Copyright (C) 2002. All Rights Reserved.
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
#include <liqRenderer.h>
//#include <liquid.h>
#include <liqGlobalHelpers.h>


extern MObject rGlobalObj;

liqRenderer::liqRenderer()
{
    pixelFilterNames.clear();
    pixelFilterNames.append("box");
    pixelFilterNames.append("triangle");
    pixelFilterNames.append("catmull-rom");
    pixelFilterNames.append("gaussian");
    pixelFilterNames.append("sinc");
    pixelFilterNames.append("blackman-harris");
    pixelFilterNames.append("mitchell");
    pixelFilterNames.append("separable-catmull-rom");
    pixelFilterNames.append("lanczos");
    pixelFilterNames.append("bessel");
    pixelFilterNames.append("disk");
}

liqRenderer::~liqRenderer()
{
}

MStatus liqRenderer::setRenderer()
{
	//CM_TRACE_FUNC("liqRenderer::setRenderer()");

  MStatus status;
  MFnDependencyNode globalsNode( initGlobals() );
  MPlug gPlug;

  liquidGetPlugValue( globalsNode, "renderCommand", renderCommand, status );
  liquidGetPlugValue( globalsNode, "previewer", renderPreview, status );
  liquidGetPlugValue( globalsNode, "renderCmdFlags", renderCmdFlags, status );
  renderCmdFlags = parseString( renderCmdFlags );
  liquidGetPlugValue( globalsNode, "shaderExt", shaderExtension, status );
  liquidGetPlugValue( globalsNode, "shaderInfo", shaderInfo, status );
  liquidGetPlugValue( globalsNode, "shaderComp", shaderCompiler, status );
  liquidGetPlugValue( globalsNode, "makeTexture", textureMaker, status );
  liquidGetPlugValue( globalsNode, "viewTexture", textureViewer, status );
  liquidGetPlugValue( globalsNode, "textureExt", textureExtension, status );

  {
    // get enabled features from the globals.
    // the bits_features attribute is a compound attribute with boolean children attributes.
    // retrieving the list of children gives you the name of the feature.
    // retrieving the children's value tells you if the feature is supported.

    status.clear();
    gPlug = globalsNode.findPlug( "bits_features", &status );
    if ( status == MS::kSuccess ) {
      unsigned int nFeatures = gPlug.numChildren( &status );
      for ( unsigned int i(0); i<nFeatures; i++ ) {
        MPlug featurePlug;
        featurePlug = gPlug.child( i, &status );
        if ( status == MS::kSuccess ) {
          MString feature;
          bool enabled =  false;
          feature = featurePlug.partialName( false, false, false, false, false, false, &status );
          featurePlug.getValue( enabled );
          feature = feature.toLowerCase();

		      if ( feature == "blobbies" )            supports_BLOBBIES             = enabled;
		      if ( feature == "points" )              supports_POINTS               = enabled;
		      if ( feature == "eyesplits" )           supports_EYESPLITS            = enabled;
		      if ( feature == "raytracing" )          supports_RAYTRACE             = enabled;
		      if ( feature == "depthoffield" )        supports_DOF                  = enabled;
		      if ( feature == "advancedvisibility" )  supports_ADVANCED_VISIBILITY  = enabled;
		      if ( feature == "displaychannels" )     supports_DISPLAY_CHANNELS     = enabled;
        }
      }
    }
  }

  {
    status.clear();
    gPlug = globalsNode.findPlug( "bits_filters", &status );
    if ( status == MS::kSuccess ) {
      unsigned int nFeatures = gPlug.numChildren( &status );
      for ( unsigned int i=0; i<nFeatures; i++ ) {
        MPlug filterPlug;
        filterPlug = gPlug.child( i, &status );
        if ( status == MS::kSuccess ) {
          MString filter;
          bool enabled =  false;
          filter = filterPlug.partialName( false, false, false, false, false, false, &status );
          filterPlug.getValue( enabled );
          filter = filter.toLowerCase();

          if ( filter == "box" )                   pixelfilter_BOX             = enabled;
          if ( filter == "triangle" )              pixelfilter_TRIANGLE        = enabled;
          if ( filter == "catmull_rom" )           pixelfilter_CATMULLROM      = enabled;
          if ( filter == "gaussian" )              pixelfilter_GAUSSIAN        = enabled;
          if ( filter == "sinc" )                  pixelfilter_SINC            = enabled;
          if ( filter == "blackman_harris" )       pixelfilter_BLACKMANHARRIS  = enabled;
          if ( filter == "mitchell" )              pixelfilter_MITCHELL        = enabled;
          if ( filter == "separablecatmull_rom" )  pixelfilter_SEPCATMULLROM   = enabled;
          if ( filter == "lanczos" )               pixelfilter_LANCZOS         = enabled;
          if ( filter == "bessel" )                pixelfilter_BESSEL          = enabled;
          if ( filter == "disk" )                  pixelfilter_DISK            = enabled;
        }
      }
    }
  }

  {
    status.clear();
    gPlug = globalsNode.findPlug( "bits_hiders", &status );
    if ( status == MS::kSuccess ) {
      unsigned int nFeatures = gPlug.numChildren( &status );
      for ( unsigned int i=0; i<nFeatures; i++ ) {
        MPlug hiderPlug;
        hiderPlug = gPlug.child( i, &status );
        if ( status == MS::kSuccess ) {
          MString hider;
          bool enabled =  false;
          hider = hiderPlug.partialName( false, false, false, false, false, false,&status );
          hiderPlug.getValue( enabled );
          hider = hider.toLowerCase();

          if ( hider == "hidden" )      hider_HIDDEN    = enabled;
          if ( hider == "photon" )      hider_PHOTON    = enabled;
          if ( hider == "zbuffer" )     hider_ZBUFFER   = enabled;
          if ( hider == "raytrace" )    hider_RAYTRACE  = enabled;
          if ( hider == "opengl" )      hider_OPENGL    = enabled;
          if ( hider == "depthmask" )   hider_DEPTHMASK = enabled;
        }
      }
    }
  }

  {
    status.clear();
    gPlug = globalsNode.findPlug( "bits_required", &status );
    if ( status == MS::kSuccess ) {
      unsigned int nFeatures = gPlug.numChildren( &status );
      for ( unsigned int i=0; i<nFeatures; i++ ) {
        MPlug requiredPlug;
        requiredPlug = gPlug.child( i, &status );
        if ( status == MS::kSuccess ) {
          MString required;
          bool enabled =  false;
          required = requiredPlug.partialName( false, false, false, false, false, false, &status );
          requiredPlug.getValue( enabled );
          required = required.toLowerCase();

          if ( required == "swap_uv" )     requires_SWAPPED_UVS  = enabled;
          if ( required == "__pref" )      requires__PREF        = enabled;
          if ( required == "makeshadow" )  requires_MAKESHADOW   = enabled;
        }
      }
    }
  }

  liquidGetPlugValue( globalsNode, "dshDisplayName", dshDisplayName, status );
  liquidGetPlugValue( globalsNode, "dshImageMode", dshImageMode, status );

  if ( renderCommand == "" ) {
    MGlobal::displayError( "Liquid : The render command is not defined !!\n" );
	return MS::kInvalidParameter;
  }else 
  {
    int lastSlash = renderCommand.rindex( '/' );
    MString tmp, envvar, homeDir;
    if ( lastSlash < 0 ) 
      tmp = renderCommand;
    else 
    {
      int len = renderCommand.length();
      tmp = renderCommand.substring( lastSlash+1, len-1 );
    }
    if ( tmp == "prman" ) 
    {
      renderName = "PRMan";
      envvar = "RMANTREE";
    } 
    else if ( tmp == "rndr" ) 
    {
      renderName = "Pixie";
      envvar = "PIXIEHOME";
    } 
    else if ( tmp == "renderdl" ) 
    {
      renderName = "3Delight";
      envvar = "DELIGHT";
    } 
    else if ( tmp == "aqsis" ) 
    {
      renderName = "Aqsis";
      envvar = "AQSISHOME";
    } 
    else if ( tmp == "air" ) 
    {
      renderName = "Air";
      envvar = "AIRHOME";
    }
    renderHome = getenv( envvar.asChar() );
	if ( renderHome == "" ) {
      MGlobal::displayError( "Liquid : The " + envvar + " environment variable is not defined !!\n" );
	}
  }

  //cout << *this <<endl;
  return MS::kSuccess;
}

MObject liqRenderer::initGlobals()
{
//	CM_TRACE_FUNC("liqRenderer::initGlobals()");

  MStatus status;
  MObject renderGlobalsObj;
  MSelectionList rGlobalList;

  status = rGlobalList.add( "liquidGlobals" );

  if ( rGlobalList.isEmpty() ) 
  {
    MGlobal::executeCommand( "liquidCreateGlobals()", false, false );
    status.clear();
    rGlobalList.clear();
    status = rGlobalList.add( "liquidGlobals" );
  }

  if ( rGlobalList.length() > 0 ) 
  {
    status.clear();
    status = rGlobalList.getDependNode( 0, renderGlobalsObj );
    if ( status == MS::kSuccess ) 
      return renderGlobalsObj;
  }

  return MObject::kNullObj;
}
//
std::ostream& liqRenderer::Print(std::ostream& o) const
{
	o <<"\n";
	o <<"liqRenderer ---------------------------------"<<endl;
	o <<"  renderName : "<<renderName.asChar()<<endl;
	o <<"  renderCommand   : "<<renderCommand.asChar()<<endl;
	o <<"  renderPreview   : "<<renderPreview.asChar()<<endl;
	o <<"  renderCmdFlags  : "<<renderCmdFlags.asChar()<<endl;
	o <<"  shaderExtension : "<<shaderExtension.asChar()<<endl;
	o <<"  shaderInfo      : "<<shaderInfo.asChar()<<endl;
	o <<"  shaderCompiler  : "<<shaderCompiler.asChar()<<endl;
	o <<"  supports_BLOBBIES            : "<<supports_BLOBBIES<<endl;
	o <<"  supports_POINTS              : "<<supports_POINTS<<endl;
	o <<"  supports_EYESPLITS           : "<<supports_EYESPLITS<<endl;
	o <<"  supports_RAYTRACE            : "<<supports_RAYTRACE<<endl;
	o <<"  supports_DOF                 : "<<supports_DOF<<endl;
	o <<"  supports_ADVANCED_VISIBILITY : "<<supports_ADVANCED_VISIBILITY<<endl;
	o <<"  supports_DISPLAY_CHANNELS    : "<<supports_DISPLAY_CHANNELS<<endl;
	o <<"  pixelfilter_BOX            : "<<pixelfilter_BOX<<endl;
	o <<"  pixelfilter_TRIANGLE       : "<<pixelfilter_TRIANGLE<<endl;
	o <<"  pixelfilter_CATMULLROM     : "<<pixelfilter_CATMULLROM<<endl;
	o <<"  pixelfilter_GAUSSIAN       : "<<pixelfilter_GAUSSIAN<<endl;
	o <<"  pixelfilter_SINC           : "<<pixelfilter_SINC<<endl;
	o <<"  pixelfilter_BLACKMANHARRIS : "<<pixelfilter_BLACKMANHARRIS<<endl;
	o <<"  pixelfilter_MITCHELL       : "<<pixelfilter_MITCHELL<<endl;
	o <<"  pixelfilter_SEPCATMULLROM  : "<<pixelfilter_SEPCATMULLROM<<endl;
	o <<"  pixelfilter_LANCZOS        : "<<pixelfilter_LANCZOS<<endl;
	o <<"  pixelfilter_BESSEL         : "<<pixelfilter_BESSEL<<endl;
	o <<"  pixelfilter_DISK           : "<<pixelfilter_DISK<<endl;
	o <<"  hider_HIDDEN    : "<<hider_HIDDEN<<endl;
	o <<"  hider_PHOTON    : "<<hider_PHOTON<<endl;
	o <<"  hider_ZBUFFER   : "<<hider_ZBUFFER<<endl;
	o <<"  hider_RAYTRACE  : "<<hider_RAYTRACE<<endl;
	o <<"  hider_OPENGL    : "<<hider_OPENGL<<endl;
	o <<"  hider_DEPTHMASK : "<<hider_DEPTHMASK<<endl;
	o <<"  requires_SWAPPED_UVS : "<<requires_SWAPPED_UVS<<endl;
	o <<"  requires__PREF       : "<<requires__PREF<<endl;
	o <<"  requires_MAKESHADOW  : "<<requires_MAKESHADOW<<endl;
	o <<"  dshDisplayName : "<<dshDisplayName.asChar()<<endl;
	o <<"  dshImageMode   : "<<dshImageMode.asChar()<<endl; 

	return o;
}
//
std::ostream& operator<<(std::ostream& o, const liqRenderer& renderer)
{
    return renderer.Print(o);
}