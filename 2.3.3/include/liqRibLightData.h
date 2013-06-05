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

#ifndef liqRibLightData_H
#define liqRibLightData_H

/* ______________________________________________________________________
**
** Liquid Rib Light Data Header File
** ______________________________________________________________________
*/
#include <common/prerequest_maya.h>
#include <liqRibData.h>
//#include <liqShader.h>
#include "liqtypes.h"
#include <liqRefactoring.h>

enum LightType {
	MRLT_Unknown  = 0,
	MRLT_Ambient  = 1,
	MRLT_Distant  = 2,
	MRLT_Point    = 3,
	MRLT_Spot     = 4,
	MRLT_Rman     = 5,//user defined light shader
	MRLT_Area     = 6,
	MRLT_Volume	= 7
};

class liqShader;

class LIQUID_EXPORT liqRibLightData : public liqRibData {
public:

  explicit liqRibLightData( const MDagPath & light );
  virtual ~liqRibLightData(){}

  virtual void write(
	  const MString &ribFileName, 
	  const structJob &currentJob, 
	  const bool bReference
	  );
  virtual bool       compare( const liqRibData & other ) const;
  virtual ObjectType type() const;

  liqLightHandle lightHandle() const;
  bool          rmanLight;
  MString       assignedRManShader;

  MString       autoShadowName( int PointLightDir = -1 ) const;
  const char* getName()const{ return lightName.asChar(); }

#ifdef Refactoring
  static void scaleZ_forRenderman(liqMatrix& desMatrix, const liqMatrix& srcMatrix);
#endif

//private:
  //void       _write(const structJob &currentJob);
  MString       extraShadowName( const MFnDependencyNode & lightShaderNode, const int & index ) const;

  LightType     lightType;
  liqFloat       color[3];
  liqFloat       decay;
  liqFloat       intensity, coneAngle, penumbraAngle, dropOff;

  liqFloat       shadowBlur;

  // spot lights
  liqFloat       barnDoors;
  liqFloat       leftBarnDoor;
  liqFloat       rightBarnDoor;
  liqFloat       topBarnDoor;
  liqFloat       bottomBarnDoor;

  liqFloat       decayRegions;
  liqFloat       startDistance1;
  liqFloat       endDistance1;
  liqFloat       startDistance2;
  liqFloat       endDistance2;
  liqFloat       startDistance3;
  liqFloat       endDistance3;
  liqFloat       startDistanceIntensity1;
  liqFloat       endDistanceIntensity1;
  liqFloat       startDistanceIntensity2;
  liqFloat       endDistanceIntensity2;
  liqFloat       startDistanceIntensity3;
  liqFloat       endDistanceIntensity3;

  // Area Lights
  MString       lightMap;
  liqFloat       lightMapSaturation;

  // 3Delight light attributes
  struct delightLight {
	  bool emitPhotons;
	  bool autoShadows;
	  int  autoShadowsSamples;
	  enum {
		  SAMPLING_STRATEGY_NONE = -1,
		  SAMPLING_STRATEGY_LIGHTSOURCE = 0, // lightsource
		  SAMPLING_STRATEGY_ILLUMINANCE = 1 // illuminance
	  } SamplingStrategy;
  } delightLight;

  liqFloat       nonDiffuse;
  liqFloat       nonSpecular;
  //RtPoint       from, to;
  liqMatrix      transformationMatrix;

  liqLightHandle handle;
  bool          usingShadow;
  bool          deepShadows;
  bool          rayTraced;
  liqInt         raySamples;
  liqFloat       shadowRadius;
  bool          excludeFromRib;
  liqFloat       bothSidesEmit;
  //bool          outputLightInShadow;
  MString       userShadowName;
  MString       lightName;

  ShadowType       shadowType;
  ShadowHiderType  shadowHiderType;

  bool          everyFrame;
  long          renderAtFrame;
  MString       geometrySet;

  MString       shadowName;
  MString       shadowNamePx;
  MString       shadowNameNx;
  MString       shadowNamePy;
  MString       shadowNameNy;
  MString       shadowNamePz;
  MString       shadowNameNz;
  liqFloat       shadowBias;
  liqFloat       shadowFilterSize;
  liqFloat       shadowSamples;
  liqColor       shadowColor;
  MString       lightCategory;
  liqFloat       lightID;
  int           hitmode;
  
  liqShader     *rmanLightShader;

private:
	liqRibLightData(const liqRibLightData&);
	liqRibLightData& operator=(const liqRibLightData&);
};

typedef boost::shared_ptr< liqRibLightData > liqRibLightDataPtr;


#endif
