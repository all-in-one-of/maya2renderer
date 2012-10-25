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
** Liquid Rib Light Data Source
** ______________________________________________________________________
*/
#include <liqRibLightData.h>
// Standard/Boost headers
#include <boost/scoped_array.hpp>
// Renderman headers
//extern "C" {
#include "ri_interface.h"
//}

// Maya headers
#include <maya/MFnDependencyNode.h>
#include <maya/MFnLight.h>
#include <maya/MPlug.h>
#include <maya/MPlugArray.h>
#include <maya/MSelectionList.h>
#include <maya/MFnDoubleArrayData.h>
#include <maya/MDoubleArray.h>
#include <maya/MTransformationMatrix.h>
#include <maya/MFnSpotLight.h>
#include <maya/MFnAreaLight.h>
#include <maya/MColor.h>
#include <maya/MString.h>
#include <maya/MStringArray.h>

// Liquid headers
#include <liqGlobalHelpers.h>
//#include <liqGetSloInfo.h>
#include <liqRenderer.h>
#include <liqShaderFactory.h>
#include <liqGlobalVariable.h>
#include "renderermgr.h"
#include "renderman/rm_helper.h"
#ifdef Refactoring
#include "liqRibTranslator.h"
#endif
using namespace std;

/** Create a RIB compatible representation of a Maya light.
 */
liqRibLightData::liqRibLightData( const MDagPath & light )
{
	CM_TRACE_FUNC("liqRibLightData::liqRibLightData("<<light.fullPathName().asChar()<<")");

	// Init
	lightType = MRLT_Unknown;
	color[0] = 0;
	color[1] = 0;
	color[2] = 0;
	decay = 0;
	intensity = 0;
	coneAngle = 0;
	penumbraAngle = 0;
	dropOff = 0;
	shadowBlur = 0;
	// spot lights
	barnDoors = 0;
	leftBarnDoor = 0;
	rightBarnDoor = 0;
	topBarnDoor = 0;
	bottomBarnDoor = 0;
	decayRegions = 0;
	startDistance1 = 0;
	endDistance1 = 0;
	startDistance2 = 0;
	endDistance2 = 0;
	startDistance3 = 0;
	endDistance3 = 0;
	startDistanceIntensity1 = 0;
	endDistanceIntensity1 = 0;
	startDistanceIntensity2 = 0;
	endDistanceIntensity2 = 0;
	startDistanceIntensity3 = 0;
	endDistanceIntensity3 = 0;
	// Area Lights
	lightMap = "";
	lightMapSaturation = 0;
  
  // 3Delight light attributes
  delightLight.emitPhotons = false;
	delightLight.autoShadows = false;
  delightLight.autoShadowsSamples = 1;
  delightLight.SamplingStrategy = delightLight::SAMPLING_STRATEGY_NONE;
	// General
	nonDiffuse = 0;
	nonSpecular = 0;
	RtMatrix transformationMatrixTmp = {{1, 0, 0, 0},  {0, 1, 0, 0},  {0, 0, 1, 0},  {0, 0, 0, 1}};
  memcpy( ( void *)transformationMatrix, ( void *)transformationMatrixTmp, sizeof(RtMatrix) );
//	bcopy(transformationMatrixTmp, transformationMatrix, sizeof(RtMatrix));
	handle = NULL;
	usingShadow = false;
	deepShadows = 0;
	rayTraced = false;
	raySamples = 16;
	shadowRadius = 0;
	excludeFromRib = false;
	bothSidesEmit = 0;
	userShadowName = "";
	lightName = "";
	shadowType = stStandart;
	shadowHiderType = shMin;
	everyFrame = true;
	renderAtFrame = 0;
	geometrySet = "";
	shadowName = "";
	shadowNamePx = "";
	shadowNameNx = "";
	shadowNamePy = "";
	shadowNameNy = "";
	shadowNamePz = "";
	shadowNameNz = "";
	shadowBias = 0;
	shadowFilterSize = 0;
	shadowSamples = 0;
	shadowColor[0] = 0;
	shadowColor[1] = 0;
	shadowColor[2] = 0;
	lightCategory = "NULL";
	lightID = 0;
	hitmode = 0;
	
	rmanLightShader = NULL;

  MStatus status;
  LIQDEBUGPRINTF( "[liqRibLightData] creating light\n" );
  rmanLight = false;
  MFnDependencyNode lightDepNode( light.node() );
  MFnDependencyNode lightMainDepNode( light.node() );
  MFnLight fnLight( light );
  lightName = fnLight.name();

  // philippe : why this liquidExcludeFromRib attr ? Shouldn't LiqInvisible do the trick ?
  // not in the mel gui either.
  status.clear();
  liquidGetPlugValue( fnLight, "liquidExcludeFromRib", excludeFromRib, status );
 
  // check if the light should be in the shadow pass - deep shadows only.
  //status.clear();
  //MPlug outputInShadowPlug = fnLight.findPlug( "outputInShadow", &status );
  //if ( MS::kSuccess == status ) {
  //  outputInShadowPlug.getValue( outputLightInShadow );
  //}

  // check to see if the light is using raytraced shadows
  if ( liqglo.liquidRenderer.supports_RAYTRACE ) 
  {
    rayTraced = fnLight.useRayTraceShadows();
    if( rayTraced ) 
    {
      usingShadow = true;
      liquidGetPlugValue( lightDepNode, "shadowRays", raySamples, status );
    }
  }

   if ( liquidRenderer.renderName == MString("3Delight") )
   {
 	  liquidGetPlugValue( lightDepNode, "liqDelightLightEmitphotons", delightLight.emitPhotons, status );
 	  liquidGetPlugValue( lightDepNode, "liqDelightLightShadows", delightLight.autoShadows, status );
 	  liquidGetPlugValue( lightDepNode, "liqDelightLightSamples", delightLight.autoShadowsSamples, status );
 	  liquidGetPlugValue( lightDepNode, "liqDelightLightSamplingStrategy", (int &)delightLight.SamplingStrategy, status );
   }
  if( !rayTraced ) 
  {
    if ( liquidGetPlugValue( fnLight, "liquidShadowName", userShadowName, status ) ==  MS::kSuccess )
      userShadowName = parseString( userShadowName, false );
    
    if ( liquidGetPlugValue( fnLight, "useDepthMapShadows", usingShadow, status ) ==  MS::kSuccess )
    { 
      if ( usingShadow ) 
      {
        liquidGetPlugValue( lightDepNode, "deepShadows", deepShadows, status );
        if ( deepShadows ) 
            shadowType = stDeep;
        
        liquidGetPlugValue( lightDepNode, "everyFrame", everyFrame, status );
        
        float tmp;
        if ( liquidGetPlugValue( lightDepNode, "renderAtFrame", tmp, status ) == MS::kSuccess )
          renderAtFrame = (int) tmp;
        
        liquidGetPlugValue( lightDepNode, "geometrySet", geometrySet, status );
      }
    }
  }
  
  MPlug rmanLightPlug = lightDepNode.findPlug( MString( "liquidLightShaderNode" ), &status );
#if 1
  if ( MS::kSuccess == status && rmanLightPlug.isConnected() ) 
  {
    //MString liquidShaderNodeName;
    MPlugArray rmanLightPlugs;
    rmanLightPlug.connectedTo( rmanLightPlugs, true, true );
    MObject liquidShaderNodeDep = rmanLightPlugs[0].node();
    //lightDepNode.setObject( liquidShaderNodeDep );
    
    
    
//    MPlug rmanShaderPlug = lightDepNode.findPlug( "rmanShaderLong", &status );

//    if ( MS::kSuccess == status ) 
//    {
      liqShader &lightShader = liqShaderFactory::instance().getShader( liquidShaderNodeDep );
      if ( lightShader.hasErrors )
      {
	    liquidMessage( "[liqRibLightData] Reading shader '" + std::string( assignedRManShader.asChar() ) + "' failed", messageError );
        rmanLight = false; 
      }
	  else 
	  { 
		rmanLight = true;
		rmanLightShader = &lightShader;
		LIQDEBUGPRINTF( "[liqRibLightData] created shader %s\n", rmanLightShader->getName().c_str() );      
	  }
//	  }
  } // if ( MS::kSuccess == status && rmanLightPlug.isConnected() )  

#else
  rmanLight = false;
#endif
  addAdditionalSurfaceParameters( fnLight.object() );

  MColor colorVal = fnLight.color();
  color[ 0 ]  = colorVal.r;
  color[ 1 ]  = colorVal.g;
  color[ 2 ]  = colorVal.b;

  intensity = fnLight.intensity();

  // get the light transform and flip it as maya's light work in the opposite direction
  // this seems to work correctly!
  RtMatrix rLightFix = {{ 1.0, 0.0,  0.0, 0.0},
                        { 0.0, 1.0,  0.0, 0.0},
                        { 0.0, 0.0, -1.0, 0.0},
                        { 0.0, 0.0,  0.0, 1.0}};

  MMatrix lightFix( rLightFix );

  /*
    philippe : why not use fnLight.lightDiffuse() and fnLight.lightSpecular() ?
    answer   : when decay regions are enabled on a spotlight,
               suddenly these calls return 0 and the light goes black.
               On the other hand, the attributes still have the correct value,
               that's why we use them.
  */
  fnLight.findPlug( "emitDiffuse"  ).getValue( nonDiffuse );
  fnLight.findPlug( "emitSpecular" ).getValue( nonSpecular );
  nonDiffuse  = 1 - nonDiffuse;
  nonSpecular = 1 - nonSpecular;

  colorVal       = fnLight.shadowColor();
  shadowColor[ 0 ]  = colorVal.r;
  shadowColor[ 1 ]  = colorVal.g;
  shadowColor[ 2 ]  = colorVal.b;

#if 0 // [3/3/2012 yaoyansi] I can't find "liquidUseLightScale" plug in liquid, so I ommit this section.
  /* added the ablility to use the lights scale - Dan Kripac - 01/03/06 */
  bool useLightScale( false );
  MPlug useLightScalePlug( fnLight.findPlug( "liquidUseLightScale", &status ) );
  if ( status == MS::kSuccess ) //has "liquidUseLightScale" plug
  {
    useLightScalePlug.getValue( useLightScale );
	
	MTransformationMatrix worldMatrix = light.inclusiveMatrix();
	if ( ! useLightScalePlug ) 
	{
		double scale[] = { 1.0, 1.0, -1.0 };
		worldMatrix.setScale( scale, MSpace::kTransform );
		MMatrix worldMatrixM( worldMatrix.asMatrix() );
		worldMatrixM.get( transformationMatrix );
	} else 
		worldMatrix.asMatrix().get( transformationMatrix );
  }
  else //no "liquidUseLightScale" plug
#endif
  {
	MTransformationMatrix worldMatrix = light.inclusiveMatrix();
	worldMatrix.asMatrix().get( transformationMatrix );
  }
  


  if ( rmanLight ) 
  {
	  lightType  = MRLT_Rman;
  }else if ( light.hasFn( MFn::kAmbientLight ) )// DIRECT MAYA LIGHTS SUPPORT
    {
        lightType = MRLT_Ambient;
    }
    else if ( light.hasFn( MFn::kDirectionalLight ) )
    {
        MFnNonExtendedLight fnDistantLight( light );
        lightType = MRLT_Distant;
        
        if ( liqglo.liqglo_doShadows && usingShadow )
        {
			shadowSamples = fnDistantLight.numShadowSamples( &status );
            if ( !rayTraced )
            {
                if ( ( shadowName == "" ) || ( shadowName.substring( 0, 9 ).toLowerCase() == "autoshadow" ))
                {
                    shadowName = autoShadowName();
                }
				liquidGetPlugValue(lightDepNode, "liqShadowMapSamples", shadowSamples, status);
            }
            else
            {
                shadowName = "raytrace";
            }
            
			liquidGetPlugValue(lightDepNode, "liqShadowBlur", shadowBlur, status);
            shadowFilterSize = fnDistantLight.depthMapFilterSize( &status );
            shadowBias       = fnDistantLight.depthMapBias( &status );
            // Philippe : on a distant light, it seems that shadow radius always returns 0.
            shadowRadius     = fnDistantLight.shadowRadius( &status );
        }
		if ( liquidGetPlugValue(lightDepNode, "__category", lightCategory, status) != MS::kSuccess )
			lightCategory = "";
		if ( liquidGetPlugValue(lightDepNode, "lightID", lightID, status) != MS::kSuccess )
			lightID = 0;
    }
	else if ( light.hasFn( MFn::kPointLight ) )
	{
        MFnNonExtendedLight fnPointLight( light );
        lightType = MRLT_Point;
        decay = fnPointLight.decayRate();
        
        if ( liqglo.liqglo_doShadows && usingShadow )
        {
            shadowFilterSize = fnPointLight.depthMapFilterSize( &status );
            shadowBias       = fnPointLight.depthMapBias( &status );
            shadowRadius     = fnPointLight.shadowRadius( &status );
			shadowSamples    = fnPointLight.numShadowSamples( &status );
            if ( !rayTraced )
            {
				liquidGetPlugValue(lightDepNode, "liqShadowMapSamples", shadowSamples, status);
            }
            liquidGetPlugValue(lightDepNode, "liqShadowBlur", shadowBlur, status);
        }
		if ( liquidGetPlugValue(lightDepNode, "__category", lightCategory, status) != MS::kSuccess )
			lightCategory = "";
		if ( liquidGetPlugValue(lightDepNode, "lightID", lightID, status) != MS::kSuccess )
			lightID = 0;
    }
    else if( light.hasFn( MFn::kSpotLight ) )
    {
        MFnSpotLight fnSpotLight( light );
        lightType         = MRLT_Spot;
        decay             = fnSpotLight.decayRate();
        coneAngle         = fnSpotLight.coneAngle();
        penumbraAngle     = fnSpotLight.penumbraAngle();
        dropOff           = fnSpotLight.dropOff();
        barnDoors         = fnSpotLight.barnDoors();
        leftBarnDoor      = fnSpotLight.barnDoorAngle( MFnSpotLight::kLeft   );
        rightBarnDoor     = fnSpotLight.barnDoorAngle( MFnSpotLight::kRight  );
        topBarnDoor       = fnSpotLight.barnDoorAngle( MFnSpotLight::kTop    );
        bottomBarnDoor    = fnSpotLight.barnDoorAngle( MFnSpotLight::kBottom );
        decayRegions      = fnSpotLight.useDecayRegions();
        startDistance1    = fnSpotLight.startDistance( MFnSpotLight::kFirst  );
        endDistance1      = fnSpotLight.endDistance(   MFnSpotLight::kFirst  );
        startDistance2    = fnSpotLight.startDistance( MFnSpotLight::kSecond );
        endDistance2      = fnSpotLight.endDistance(   MFnSpotLight::kSecond );
        startDistance3    = fnSpotLight.startDistance( MFnSpotLight::kThird  );
        endDistance3      = fnSpotLight.endDistance(   MFnSpotLight::kThird  );

		if ( liquidGetPlugValue(lightDepNode,"startDistanceIntensity1",startDistanceIntensity1, status) != MS::kSuccess )
			startDistanceIntensity1 = 1.0;
		if ( liquidGetPlugValue(lightDepNode,"endDistanceIntensity1",endDistanceIntensity1, status) != MS::kSuccess )
			endDistanceIntensity1 = 1.0;
		if ( liquidGetPlugValue(lightDepNode,"startDistanceIntensity2",startDistanceIntensity2, status) != MS::kSuccess )
			startDistanceIntensity2 = 1.0;
		if ( liquidGetPlugValue(lightDepNode,"endDistanceIntensity2",endDistanceIntensity2, status) != MS::kSuccess )
			endDistanceIntensity2 = 1.0;
		if ( liquidGetPlugValue(lightDepNode,"startDistanceIntensity3",startDistanceIntensity3, status) != MS::kSuccess )
			startDistanceIntensity3 = 1.0;
		if ( liquidGetPlugValue(lightDepNode,"endDistanceIntensity3",endDistanceIntensity3, status) != MS::kSuccess )
			endDistanceIntensity3 = 1.0;

        if ( liqglo.liqglo_doShadows && usingShadow )
        {
			shadowSamples = fnSpotLight.numShadowSamples( &status );
            if ( !rayTraced )
            {
                if ( ( shadowName == "" ) || ( shadowName.substring( 0, 9 ).toLowerCase() == "autoshadow" ) )
                {
                    shadowName = autoShadowName();
                }
                liquidGetPlugValue(lightDepNode, "liqShadowMapSamples", shadowSamples, status);
            }
            else
            {
                shadowName = "raytrace";
            }
            
			liquidGetPlugValue(lightDepNode, "liqShadowBlur", shadowBlur, status);
            shadowFilterSize = fnSpotLight.depthMapFilterSize( &status );
            shadowBias       = fnSpotLight.depthMapBias( &status );
            shadowRadius     = fnSpotLight.shadowRadius( &status );
        }
            
		if ( liquidGetPlugValue(lightDepNode, "__category", lightCategory, status) != MS::kSuccess )
			lightCategory = "";
		if ( liquidGetPlugValue(lightDepNode, "lightID", lightID, status) != MS::kSuccess )
			lightID = 0;
    }
    else if ( light.hasFn( MFn::kAreaLight ) )
    {
      MFnAreaLight fnAreaLight( light );
      lightType      = MRLT_Area;
      decay          = fnAreaLight.decayRate();
      shadowSamples  = 64.0f;
      if ( liqglo.liqglo_doShadows && usingShadow ) 
	  {
        if ( !rayTraced ) 
		{
          if ( ( shadowName == "" ) || ( shadowName.substring( 0, 9 ).toLowerCase() == "autoshadow" ) ) {
            shadowName   = autoShadowName();
          }
        } else {
          shadowName = "raytrace";
        }

        shadowFilterSize = fnAreaLight.depthMapFilterSize( &status );
        shadowBias       = fnAreaLight.depthMapBias( &status );
        shadowSamples    = fnAreaLight.numShadowSamples( &status );
        shadowRadius     = fnAreaLight.shadowRadius( &status );
      }
	  bool bothsides = false;
	  liquidGetPlugValue(lightDepNode, "liqBothSidesEmit", bothsides, status);
	  bothSidesEmit = ( bothsides == true ) ? 1.0 : 0.0;

	  if ( liquidGetPlugValue(lightDepNode, "__category", lightCategory, status) != MS::kSuccess )
		  lightCategory = "";
	  if ( liquidGetPlugValue(lightDepNode, "lightID", lightID, status) != MS::kSuccess )
		  lightID = 0;
	  if ( liquidGetPlugValue(lightDepNode, "liqAreaHitmode", hitmode, status) != MS::kSuccess )
		  hitmode = 1;

	  if ( liquidGetPlugValue(lightDepNode, "liqLightMap", lightMap, status) == MS::kSuccess )
		  lightMap = parseString( lightMap, false );  
	  else 
		  lightMap = "";

	  if ( liquidGetPlugValue(lightDepNode, "liqLightMapSaturation", lightMapSaturation, status) != MS::kSuccess )
		  lightMapSaturation = 1.0;
    }
  //}//if ( rmanLight ) else
}

/** Write the RIB for this light.
 */
void liqRibLightData::_write(const structJob &currentJob)
{
	CM_TRACE_FUNC("liqRibLightData::_write(job="<<currentJob.name.asChar()<<")");

  if ( !excludeFromRib ) 
  {
    LIQDEBUGPRINTF( "-> writing light %s \n", lightName.asChar());

	//RiConcatTransform( * const_cast< RtMatrix* >( &transformationMatrix ) );
    if ( liqglo.liqglo_isShadowPass ) 
    {
      if ( usingShadow ) 
      {
        RtString sName( const_cast< char* >( shadowName.asChar() ) );
        // Hmmmmm got to set a LIQUIDHOME env var and use it ...
        // May be set relative name shadowPassLight and resolve path with RIB searchpath
        // Moritz: solved through default shader searchpath in liqRibTranslator
		handle = liquid::RendererMgr::getInstancePtr()->
			getRenderer()->exportShadowPassLight("??", lightName.asChar(), sName, transformationMatrix);
      }
    } 
    else 
    {
      RtString cat( const_cast< char* >( lightCategory.asChar() ) );

      switch ( lightType ) 
      {
        case MRLT_Ambient:
			handle = liquid::RendererMgr::getInstancePtr()->
				getRenderer()->exportAmbientLight("???", lightName.asChar(), intensity, color, transformationMatrix);
          break;
        
        case MRLT_Distant:
			{
				liqFloat    i_intensity = 1;
				liqColor i_lightcolor;	setColor(i_lightcolor, 1.0);
				assert(i_lightcolor[0] == 1.0f);
				assert(i_lightcolor[1] == 1.0f);
				assert(i_lightcolor[2] == 1.0f);
				liqString i_shadowname = "";  /* shadow map name or "raytrace" for traced shadows */
				liqFloat i_shadowbias = 0.01;
				liqFloat i_shadowblur = 0.0;
				liqFloat i_shadowsamples = 16; /* samples or rays */
				liqFloat i_shadowfiltersize = 1;
				liqColor i_shadowcolor;	setColor(i_shadowcolor, 0.0);
				liqFloat  i_lightID=0;
				liqString o_category="";
				liqFloat o_shadowF = 0;
				liqColor o_shadowC;	setColor(o_shadowC, 0.0);
				liqColor o_unshadowed_Cl;	setColor(o_unshadowed_Cl, 0.0);
				liqFloat o_nondiffuse = 0;  /* set to 1 to exclude from diffuse light */
				liqFloat o_nonspecular = 0; /* set to 1 to exclude from highlights */

				if ( liqglo.liqglo_doShadows && usingShadow ) 
				{
					i_intensity = intensity;
					setColor(i_lightcolor, color);
					i_shadowname  = const_cast< char* >( shadowName.asChar() );
					i_shadowfiltersize = rayTraced ? shadowRadius : shadowFilterSize;
					i_shadowbias = shadowBias;
					i_shadowsamples = shadowSamples;
					i_shadowblur = shadowBlur;
					setColor(i_shadowcolor, shadowColor);
					o_nondiffuse = nonDiffuse;
					o_nonspecular = nonSpecular;
					o_category = cat;
					i_lightID = lightID;
				}else{
					i_intensity = intensity;
					setColor(i_lightcolor, color);
					setColor(i_shadowcolor, shadowColor);
					o_nondiffuse = nonDiffuse;
					o_nonspecular = nonSpecular;
					o_category = cat;
					i_lightID = lightID;
				}
				handle = liquid::RendererMgr::getInstancePtr()->
					getRenderer()->exportDistantLight(
					"distantlight", 
					lightName.asChar(),  
					i_intensity,
					i_lightcolor,
					i_shadowname,
					i_shadowbias,
					i_shadowblur,
					i_shadowsamples,
					i_shadowfiltersize,
					i_shadowcolor,
					i_lightID,
					o_category,
					o_shadowF,
					o_shadowC,
					o_unshadowed_Cl,
					o_nondiffuse,
					o_nonspecular,
					transformationMatrix
					);
			}
          break;
        
        case MRLT_Point:
			{
					liqFloat    i_intensity = 1;
					liqColor i_lightcolor;	setColor(i_lightcolor, 1);
					liqFloat i_decay = 0;
					liqString i_shadownamepx = "";
					liqString i_shadownamenx = "";
					liqString i_shadownamepy = "";
					liqString i_shadownameny = "";
					liqString i_shadownamepz = "";
					liqString i_shadownamenz = "";
					liqFloat i_shadowbias = 0.01;
					liqFloat i_shadowblur = 0.0;
					liqFloat i_shadowsamples = 16; /* samples or rays */
					liqFloat i_shadowfiltersize = 1.0;
					liqColor i_shadowcolor;	setColor(i_shadowcolor, 0.0);
					liqFloat  i_lightID=0;
					liqString o_category="";
					liqFloat	o_shadowF = 0;
					liqColor	o_shadowC;setColor(o_shadowC, 1.0);
					liqColor	o_unshadowed_Cl;setColor(o_unshadowed_Cl, 0.0);
					liqFloat o_nondiffuse = 0;  /* set to 1 to exclude from diffuse light */
					liqFloat o_nonspecular = 0; /* set to 1 to exclude from highlights */

			  if ( liqglo.liqglo_doShadows && usingShadow ) 
			  {
				  liqFloat    i_intensity = intensity;
				  setColor(i_lightcolor, color);
				  i_decay = decay;
				  {
					  MString	px = rayTraced ? "raytrace" : autoShadowName( pPX );
					  MString	nx = rayTraced ? "raytrace" : autoShadowName( pNX );
					  MString	py = rayTraced ? "raytrace" : autoShadowName( pPY );
					  MString	ny = rayTraced ? "raytrace" : autoShadowName( pNY );
					  MString	pz = rayTraced ? "raytrace" : autoShadowName( pPZ );
					  MString	nz = rayTraced ? "raytrace" : autoShadowName( pNZ );
					  i_shadownamepx = const_cast<char*>( px.asChar() );
					  i_shadownamenx = const_cast<char*>( nx.asChar() );
					  i_shadownamepy = const_cast<char*>( py.asChar() );
					  i_shadownameny = const_cast<char*>( ny.asChar() );
					  i_shadownamepz = const_cast<char*>( pz.asChar() );
					  i_shadownamenz = const_cast<char*>( nz.asChar() );
				  }
				  i_shadowbias = shadowBias;
				  i_shadowblur = shadowBlur;
				  i_shadowsamples = shadowSamples; /* samples or rays */
				  i_shadowfiltersize = rayTraced ? shadowRadius : shadowFilterSize;
				  setColor(i_shadowcolor, shadowColor);
				  i_lightID = lightID;
				  o_category=cat;
				  o_nondiffuse = nonDiffuse;  /* set to 1 to exclude from diffuse light */
				  o_nonspecular = nonSpecular; /* set to 1 to exclude from highlights */
			  }else{
					i_intensity = intensity;
					setColor(i_lightcolor, color);
					i_decay = decay;
					setColor(i_shadowcolor, shadowColor);
					i_lightID = lightID;					
					o_category=cat;
					o_nondiffuse = nonDiffuse;  /* set to 1 to exclude from diffuse light */
					o_nonspecular = nonSpecular; /* set to 1 to exclude from highlights */
			  }
			  handle = liquid::RendererMgr::getInstancePtr()->
				  getRenderer()->exportPointLight(
				  "pointlight", 
				  lightName.asChar(),  
				  i_intensity,
				  i_lightcolor,
				  i_decay,
				  i_shadownamepx,
				  i_shadownamenx,
				  i_shadownamepy,
				  i_shadownameny,
				  i_shadownamepz,
				  i_shadownamenz,
				  i_shadowbias,
				  i_shadowblur,
				  i_shadowsamples,
				  i_shadowfiltersize,
				  i_shadowcolor,
				  i_lightID,
				  o_category,
				  o_shadowF,
				  o_shadowC,
				  o_unshadowed_Cl,
				  o_nondiffuse,
				  o_nonspecular,
				  transformationMatrix
				  );
			}
          break;
        case MRLT_Spot:
			{
				liqFloat i_intensity = 1;
				liqColor i_lightcolor;	setColor(i_lightcolor, 1.0);
				liqFloat i_coneangle               = 40;
				liqFloat i_penumbraangle           = 0;
				liqFloat i_dropoff                 = 0;
				liqFloat i_decay                   = 0;

				liqFloat i_barndoors               = 0;
				liqFloat i_leftbarndoor            = 10;
				liqFloat i_rightbarndoor           = 10;
				liqFloat i_topbarndoor             = 10;
				liqFloat i_bottombarndoor          = 10;

				liqFloat i_decayRegions            = 0;
				liqFloat i_startDistance1          = 1.0;
				liqFloat i_endDistance1            = 2.0;
				liqFloat i_startDistance2          = 3.0;
				liqFloat i_endDistance2            = 6.0;
				liqFloat i_startDistance3          = 8.0;
				liqFloat i_endDistance3            = 10.0;
				liqFloat i_startDistanceIntensity1 = 1.0;
				liqFloat i_endDistanceIntensity1   = 1.0;
				liqFloat i_startDistanceIntensity2 = 1.0;
				liqFloat i_endDistanceIntensity2   = 1.0;
				liqFloat i_startDistanceIntensity3 = 1.0;
				liqFloat i_endDistanceIntensity3   = 1.0;

				liqString i_shadowname       = "";
				liqFloat  i_shadowbias       = 0.01;
				liqFloat  i_shadowblur       = 0.0;
				liqFloat  i_shadowsamples    = 32;
				liqFloat  i_shadowfiltersize = 1;
				liqColor  i_shadowcolor;setColor(i_shadowcolor, 0.0f);
				liqColor  i_shadowcolorsurf;setColor(i_shadowcolorsurf, 0.0f);
				liqFloat  i_shadowcolormix  = -1;

				liqFloat  i_lightID          = 0;
				liqString i_category       = "";

				liqColor o_shadowC;setColor(o_shadowC, 0.0f);
				liqFloat o_shadowF        = 0;
				liqFloat o_barn           = 0;
				liqColor o_unshadowed_Cl;setColor(o_unshadowed_Cl, 0.0f);
				liqFloat o_nondiffuse     = 0;
				liqFloat o_nonspecular    = 0;

				  if (liqglo.liqglo_doShadows && usingShadow) 
				  {
					/* if ( ( shadowName == "" ) || ( shadowName.substring( 0, 9 ) == "autoshadow" ) ) {
					  shadowName = liqglo_texDir + autoShadowName();
					} */
					i_intensity = intensity;
					setColor(i_lightcolor, color);
					i_coneangle = coneAngle;
					i_penumbraangle = penumbraAngle;
					i_dropoff = dropOff;
					i_decay = decay;
					i_barndoors = barnDoors;
					i_leftbarndoor = leftBarnDoor;
					i_rightbarndoor = rightBarnDoor;
					i_topbarndoor = topBarnDoor;
					i_bottombarndoor = bottomBarnDoor;
					i_decayRegions = decayRegions;
					i_startDistance1 = startDistance1;
					i_endDistance1   = endDistance1;
					i_startDistance2 = startDistance2;
					i_endDistance2   = endDistance2;
					i_startDistance3 = startDistance3;
					i_endDistance3   = endDistance3;
					i_startDistanceIntensity1 = startDistanceIntensity1;
					i_endDistanceIntensity1   = endDistanceIntensity1;
					i_startDistanceIntensity2 = startDistanceIntensity2;
					i_endDistanceIntensity2   = endDistanceIntensity2;
					i_startDistanceIntensity3 = startDistanceIntensity3;
					i_endDistanceIntensity3   = endDistanceIntensity3;
					i_shadowname = const_cast< char* >( shadowName.asChar() );
					i_shadowfiltersize = rayTraced ? shadowRadius : shadowFilterSize;
					i_shadowbias = shadowBias;
					i_shadowsamples = shadowSamples;
					i_shadowblur = shadowBlur;
					setColor(i_shadowcolor,shadowColor);
					o_nondiffuse = nonDiffuse;
					o_nonspecular = nonSpecular;
					i_category = cat;
					i_lightID = lightID;
				} 
				else 
				{
						i_intensity = intensity;
						setColor(i_lightcolor, color);
						setColor(i_shadowcolor, shadowColor);
						i_coneangle = coneAngle;
						i_penumbraangle = penumbraAngle;
						i_dropoff = dropOff;
						i_decay = decay;
						i_barndoors = barnDoors;
						i_leftbarndoor = leftBarnDoor;
						i_rightbarndoor = rightBarnDoor;
						i_topbarndoor = topBarnDoor;
						i_bottombarndoor = bottomBarnDoor;
						i_decayRegions = decayRegions;
						i_startDistance1 = startDistance1;
						i_endDistance1   = endDistance1;
						i_startDistance2 = startDistance2;
						i_endDistance2   = endDistance2;
						i_startDistance3 = startDistance3;
						i_endDistance3   = endDistance3;
						i_startDistanceIntensity1 = startDistanceIntensity1;
						i_endDistanceIntensity1   = endDistanceIntensity1;
						i_startDistanceIntensity2 = startDistanceIntensity2;
						i_endDistanceIntensity2   = endDistanceIntensity2;
						i_startDistanceIntensity3 = startDistanceIntensity3;
						i_endDistanceIntensity3   = endDistanceIntensity3;
						i_shadowname = const_cast< char* >( shadowName.asChar() );
						i_shadowbias = shadowBias;
						i_shadowsamples = shadowSamples;
						o_nondiffuse = nonDiffuse;
						o_nonspecular = nonSpecular;
						i_category = cat;
						i_lightID = lightID;
				  }
				  handle = liquid::RendererMgr::getInstancePtr()->
					  getRenderer()->exportSpotLight(
					  "spotlight", 
					  lightName.asChar(),
					  i_intensity,
					  i_lightcolor,
					  i_coneangle,
					  i_penumbraangle,
					  i_dropoff,
					  i_decay,

					  i_barndoors,
					  i_leftbarndoor,
					  i_rightbarndoor,
					  i_topbarndoor,
					  i_bottombarndoor,

					  i_decayRegions,
					  i_startDistance1,
					  i_endDistance1,
					  i_startDistance2,
					  i_endDistance2,
					  i_startDistance3,
					  i_endDistance3,
					  i_startDistanceIntensity1,
					  i_endDistanceIntensity1,
					  i_startDistanceIntensity2,
					  i_endDistanceIntensity2,
					  i_startDistanceIntensity3,
					  i_endDistanceIntensity3,

					  i_shadowname,
					  i_shadowbias,
					  i_shadowblur,
					  i_shadowsamples,
					  i_shadowfiltersize,
					  i_shadowcolor,
					  i_shadowcolorsurf,
					  i_shadowcolormix,

					  i_lightID,
					  i_category,

					  o_shadowC,
					  o_shadowF,
					  o_barn,
					  o_unshadowed_Cl,
					  o_nondiffuse,
					  o_nonspecular,
					  transformationMatrix
					  );

				  
			}
          break;
        case MRLT_Rman: 
        {
          /*
          unsigned numTokens( tokenPointerArray.size() );
          scoped_array< RtToken > tokenArray( new RtToken[ numTokens ] );
          scoped_array< RtPointer > pointerArray( new RtPointer[ numTokens ] );
          assignTokenArraysV( tokenPointerArray, tokenArray.get(), pointerArray.get() );

          if ( liqglo_shortShaderNames ) 
            assignedRManShader = basename( const_cast< char* >( assignedRManShader.asChar() ) );
         
          RtString shaderName = const_cast< RtString >( assignedRManShader.asChar() );
          handle = RiLightSourceV( shaderName, numTokens, tokenArray.get(), pointerArray.get() );
          */
			liqMatrix transformationMatrixScaledZ;
			liqRibLightData::scaleZ_forRenderman(
				transformationMatrixScaledZ, transformationMatrix
				);
			RiConcatTransform( * const_cast< RtMatrix* >( &transformationMatrixScaledZ ) );
          rmanLightShader->write();
 		  #ifdef RIBLIB_AQSIS
 		  handle = reinterpret_cast<RtLightHandle>(static_cast<ptrdiff_t>(rmanLightShader->shaderHandler.asInt()));
 		  #else
		  /* !!!! In Generic libRib light handle is unsigned int */
		  LIQDEBUGPRINTF( "-> assigning light handle: " );
		  handle = (const RtLightHandle)(const void *)( rmanLightShader->shaderHandler.asUnsigned() );
		  LIQDEBUGPRINTF( "%u\n", (unsigned int)(long)(const void *)handle );
          #endif
          break;
        }
        case MRLT_Area: 
        {
			liqFloat   i_intensity     = 1.0;
			liqColor   i_lightcolor;	setColor(i_lightcolor, 1.0);
			liqFloat   i_decay         = 2;
			liqString  i_coordsys      = "";
			liqFloat   i_lightsamples  = 32;
			liqFloat   i_doublesided   = 0;
			liqString  i_shadowname    = "";
			liqColor   i_shadowcolor;	setColor(i_lightcolor, 0.0);
			liqString  i_hitmode       = "primitive";

			liqString  i_lightmap      = "";
			liqFloat   i_lightmapsaturation  = 2.0;

			liqFloat  i_lightID        = 0;
			liqString i_category       = "";

			liqFloat o_nonspecular          = 1;
			liqFloat o_shadowF              = 0;
			liqColor o_shadowC;	setColor(o_shadowC, 0.0);
			liqColor o_unshadowed_Cl;	setColor(o_unshadowed_Cl, 0.0);
			liqFloat o_arealightIntensity   = 0;
			liqColor o_arealightColor;	setColor(o_arealightColor, 0.0);



		  i_coordsys = const_cast< char* >(MString(lightName+"CoordSys").asChar());
          RtString areacoordsys = i_coordsys;

//           MString areashader( getenv("LIQUIDHOME") );
//           areashader += "/lib/shaders/liquidarea";

          RtString rt_hitmode;
          switch( hitmode ) 
		  {
            case 1:
              rt_hitmode = const_cast< char* >( "primitive" );
              break;
            case 2:
              rt_hitmode = const_cast< char* >( "shader" );
              break;
            default:
              rt_hitmode = const_cast< char* >( "default" );
              break;
          }


          // if raytraced shadows are off, we get a negative value, so we correct it here.
          RtString rt_lightmap( const_cast< char* >( lightMap.asChar() ) );
		 
		  {
			  i_intensity = intensity;
			  setColor(i_lightcolor, color);
			  i_decay = decay;
			  i_coordsys = areacoordsys;
			  i_lightsamples = shadowSamples;
			  i_doublesided = bothSidesEmit;
			  i_shadowname = const_cast< char* >( shadowName.asChar() );
			  setColor(i_shadowcolor, shadowColor);
			  i_lightmap = rt_lightmap;
			  i_lightID = lightID;
			  i_hitmode = rt_hitmode;
			  i_category = cat;
		  }

		  handle = liquid::RendererMgr::getInstancePtr()->
			  getRenderer()->exportAreaLight(
			  "arealight", 
			  lightName.asChar(),
			  i_intensity,
			  i_lightcolor,
			  i_decay,
			  i_coordsys,
			  i_lightsamples,
			  i_doublesided,
			  i_shadowname,
			  i_shadowcolor,
			  i_hitmode,
			  i_lightmap,
			  i_lightmapsaturation,
			  i_lightID,
			  i_category,

			  o_nonspecular,
			  o_shadowF, 
			  o_shadowC,
			  o_unshadowed_Cl,
			  o_arealightIntensity,
			  o_arealightColor,
			  transformationMatrix
			  );
          break;
        }
        case MRLT_Unknown: {
			RiConcatTransform( * const_cast< RtMatrix* >( &transformationMatrix ) );
          break;
        }
      }
    }
  }
}

//
void liqRibLightData::write(const MString &ribFileName, const structJob &currentJob, const bool bReference)
{
	CM_TRACE_FUNC("liqRibLightData::write("<<ribFileName.asChar()<<",job="<<currentJob.name.asChar()<<","<<bReference<<")");

	//assert(liqglo.m_ribFileOpen&&"liqRibLightData.cpp");//er also goes here , refactoring is needed.

	if( !bReference ){//write data at first time
		assert(m_ribFileFullPath.length()==0&&"liqRibLightData.cpp");
		this->setRibFileFullPath(ribFileName);

		renderman::Helper o;
		o.RiBeginRef(m_ribFileFullPath.asChar());
		_write(currentJob);
		o.RiEndRef();

	}else{
		//write the reference
		assert(m_ribFileFullPath == ribFileName);
		RiReadArchive( const_cast< RtToken >( m_ribFileFullPath.asChar() ), NULL, RI_NULL );
	}
}
/** Light comparisons are not supported in this version.
 */
bool liqRibLightData::compare( const liqRibData & otherObj ) const
{
	CM_TRACE_FUNC("liqRibLightData::compare("<<otherObj.getFullPathName()<<")");

  otherObj.type(); // reference it to avoid unused param compiler warning
  LIQDEBUGPRINTF( "-> comparing light\n" );
  return true;
}


/** Return the object type.
 */
ObjectType liqRibLightData::type() const
{
  LIQDEBUGPRINTF( "-> returning light type\n" );
  return MRT_Light;
}

RtLightHandle liqRibLightData::lightHandle() const
{
	LIQDEBUGPRINTF( "-> returning light handle: " );
	LIQDEBUGPRINTF( "%u\n", (unsigned int)(long)(const void *)handle );
  return handle;
}

MString liqRibLightData::autoShadowName( int PointLightDir ) const
{
	CM_TRACE_FUNC("liqRibLightData::autoShadowName("<<PointLightDir<<")");

  MString frame;
  MString shadowName;

  if( userShadowName.length() ) 
    shadowName = userShadowName;
  else 
  {
    shadowName = liquidGetRelativePath( liqglo.liqglo_relativeFileNames, liqglo.liqglo_textureDir, liqglo.liqglo_projectDir );
    if ( !liqglo.liqglo_shapeOnlyInShadowNames ) 
    {
      shadowName += liquidTransGetSceneName();
      shadowName = parseString( shadowName, false );
      shadowName += "_";
    }
    shadowName += sanitizeNodeName( lightName );
	//[refactor][shadowname] begin 
	shadowName += "_";
    shadowName += ( shadowType == stDeep )? "DSH": "SHD";

    if ( PointLightDir != -1 ) 
    {
      switch ( PointLightDir ) 
      {
        case pPX:
          shadowName += "_PX";
          break;
        case pPY:
          shadowName += "_PY";
          break;
        case pPZ:
          shadowName += "_PZ";
          break;
        case pNX:
          shadowName += "_NX";
          break;
        case pNY:
          shadowName += "_NY";
          break;
        case pNZ:
          shadowName += "_NZ";
          break;
      }
    }
	//[refactor][shadowname] end 
    shadowName += ".";

    if ( geometrySet != "" ) 
      shadowName += geometrySet + ".";

    if ( everyFrame ) 
      frame += (int) liqglo.liqglo_lframe;
    else 
      frame += (int) renderAtFrame;


    if ( liqglo.liqglo_doExtensionPadding ) 
      while( frame.length() < liqglo.liqglo_outPadding ) 
        frame = "0" + frame;
    
    shadowName += frame;
    shadowName += ".tex";
  }
  //cout <<"liqRibLightData::autoShadowName : "<<shadowName.asChar()<<"  ( "<<liquidTransGetSceneName().asChar()<<" )"<<endl;
  LIQDEBUGPRINTF( "[liqRibLightData::autoShadowName] : %s scene = %s\n", shadowName.asChar(), liquidTransGetSceneName().asChar() );// we replace liqglo.liqglo_sceneName with liquidTransGetSceneName()

  return shadowName;

}


MString liqRibLightData::extraShadowName( const MFnDependencyNode & lightShaderNode, const int & index ) const
{
	CM_TRACE_FUNC("liqRibLightData::extraShadowName("<<lightShaderNode.name().asChar()<<","<<index<<")");

  MString frame;
  MString shadowName        = "";
  MStatus status;

  MString shdCamName        = "";
  bool shdCamDeepShadows    = false;
  bool shdCamEveryFrame     = true;
  int shdCamRenderAtFrame   = 0;
  MString shdCamGeometrySet = "";

  status.clear();
  MPlug shadowCamerasPlug = lightShaderNode.findPlug( "shadowCameras", &status );
  if ( MS::kSuccess == status ) 
  {
    MPlug theShadowCamPlug = shadowCamerasPlug.elementByPhysicalIndex( index, &status );
    MPlugArray shadowCamPlugArray;
    if ( MS::kSuccess == status && theShadowCamPlug.connectedTo( shadowCamPlugArray, true, false ) ) {
      MFnDependencyNode shadowCamDepNode = shadowCamPlugArray[0].node();
      shdCamName = shadowCamDepNode.name();
      MPlug shadowCamParamPlug = shadowCamDepNode.findPlug( "liqDeepShadows", &status );
      if ( MS::kSuccess == status ) shadowCamParamPlug.getValue( shdCamDeepShadows );
      status.clear();
      shadowCamParamPlug = shadowCamDepNode.findPlug( "liqEveryFrame", &status );
      if ( MS::kSuccess == status ) shadowCamParamPlug.getValue( shdCamEveryFrame );
      status.clear();
      shadowCamParamPlug = shadowCamDepNode.findPlug( "liqRenderAtFrame", &status );
      if ( MS::kSuccess == status ) shadowCamParamPlug.getValue( shdCamRenderAtFrame );
      status.clear();
      shadowCamParamPlug = shadowCamDepNode.findPlug( "liqGeometrySet", &status );
      if ( MS::kSuccess == status ) shadowCamParamPlug.getValue( shdCamGeometrySet );

      shadowName += liquidTransGetSceneName();
      shadowName =  parseString( shadowName, false );
      shadowName += "_";
      shadowName += sanitizeNodeName( shdCamName );
      shadowName += ( shdCamDeepShadows )? "DSH": "SHD";
      shadowName += ".";
      if ( shdCamGeometrySet != "" ) 
      {
        shadowName += shdCamGeometrySet + ".";
      }
      if ( shdCamEveryFrame ) 
        frame += (int) liqglo.liqglo_lframe;
      else 
        frame += (int) shdCamRenderAtFrame;
      if ( liqglo.liqglo_doExtensionPadding ) 
        while( frame.length() < liqglo.liqglo_outPadding ) 
          frame = "0" + frame;
      shadowName += frame;
      shadowName += ".tex";
    } 
    else 
    {
      //error message here !!
     string err = "could not evaluate shadow camera connected to " + string( lightShaderNode.name().asChar() );
      liquidMessage( err, messageError );
    }
  } else {
    //error message here !!
    string err = "Could not find a shadowCameras attribute on " + string( lightShaderNode.name().asChar() );
    liquidMessage( err, messageError );
  }

  //cout <<"liqRibLightData::extraShadowName : "<<shadowName.asChar()<<"  ( "<<liquidTransGetSceneName().asChar()<<" )"<<endl;
  LIQDEBUGPRINTF( "[liqRibLightData::extraShadowName] : %s scene = %s\n", shadowName.asChar(), liquidTransGetSceneName().asChar() );// we replace liqglo.liqglo_sceneName with liquidTransGetSceneName()

  return shadowName;
}
#ifdef Refactoring
void liqRibLightData::scaleZ_forRenderman(liqMatrix& desMatrix, const liqMatrix& srcMatrix)
{
	liqRIBMsg("scale z in transform matrix");
	MMatrix tmpM(srcMatrix);
	MTransformationMatrix worldMatrix = tmpM;
	double scale[] = { 1.0, 1.0, -1.0 };
	worldMatrix.setScale( scale, MSpace::kTransform );
	MMatrix worldMatrixM( worldMatrix.asMatrix() );
	worldMatrixM.get( desMatrix );
}
#endif

