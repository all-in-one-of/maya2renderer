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

#ifndef liquid_H
#define liquid_H

/* ______________________________________________________________________
**
** Liquid Header File
** ______________________________________________________________________
*/

#if defined(WIN32) && !defined(DEBUG)
  // Disable double -> float conversion and signed <> unsigned mismatch warnings
  #pragma warning( disable : 4244 4305 4018 )
#endif




//#ifndef WIN32
//#include <sys/times.h>
//#include <sys/types.h>
//#endif


#include <common/prerequest_std.h>
#include <common/prerequest_maya.h>
////////////////////////
// Macros and Defines //
////////////////////////
// #ifndef debugMode
// extern int debugMode;
// #endif


#include "liqDebug.h"
#include "liqDefine.h"
#include "liqEnum.h"
#include "liqlog.h"
#include "liqMath.h"
#include "liqRefactoring.h"
#include "liqStructJob.h"
#include "liqtypes.h"
//#define HERE  cout<<"at line "<<__LINE__<<" in "<<__FUNCTION__<<endl<<flush;

//#define TIMER_START       struct tms t,u;long r1,r2;r1 = times(&t);
//#define TIMER_STOP(msg)   r2 = times(&u); cout <<"[liquid timer] "<<msg<<" :"<<endl<<"\t  user time = "<<((float)(u.tms_utime-t.tms_utime))/(HZ)<<endl<<"\tsystem time = "<<((float)(u.tms_stime-t.tms_stime))/(HZ)<<endl;






// Equivalence test for floats.  Equality tests are dangerous for floating
// point values
//


// Specifies how the start/end frame is set
//
//#define USE_TIMESLIDER 1
//MM_TO_INCH is moved to liqMath.h



///////////
// Enums //
///////////

//ObjectType is moved to liqRibObj.h

//LightType is moved to liqRibLightData.h


// enum RendererType {
//   PRMan   = 0,
//   BMRT    = 1,
//   RDC     = 2
// };




//enum HiderType {
//  htHidden    = 0,
//  htPhoton    = 1,
//  htZbuffer   = 2,
//  htRaytrace  = 3,
//  htOpenGL    = 4,
//  htDepthMask = 5
//};



// enum TransmissionType { // shadow cast attribute
//   trNone        = 0,  // not set
//   trTransparent = 1,
//   trOpaque      = 2,
//   trOs          = 3,
//   trShader      = 4
// };
//
//enum fileGenMode {
//	fgm_shadow_tex,
//	fgm_shadow_rib,
//	fgm_shadow_archive,
//	fgm_scene_archive,
//	fgm_hero_rib,
//	fgm_image
//};
//
//enum ShadowType {
//  stStandart = 0,
//  stMidPoint = 1,
//  stMinMax   = 2,
//  stDeep     = 3
//};
//
//enum ShadowHiderType {
//  shNone     = 0,
//  shMin      = 1,
//  shMax      = 2,
//  shAverage  = 3,
//  shMidPoint = 4
//};
//
//struct structCamera {
//  MMatrix  mat;   // camera inverse matrix
//  double  neardb;
//  double  fardb;
//  double  hFOV;
//  int     isOrtho;
//  double  orthoWidth;
//  double  orthoHeight;
//  MString name;
//  bool    motionBlur;
//  double  shutter;
//  double  fStop;
//  double  focalDistance;
//  double  focalLength;
//  double  horizontalFilmOffset;
//  double  verticalFilmOffset;
//  int width;
//  int height;
//  double fov_ratio;
//  structCamera *rightCam;
//  structCamera *leftCam;
//};


//
//struct structJob {
//  MString name;
//  MString renderName;
//  
//  MString ribFileName;
//
//  MString texName;
//  
//  MString imageName;
//  MString imageMode;
//  MString format;
//
//  int     width, height;
//  float   aspectRatio;
//
//  short   samples;
//  float   shadingRate;
//  float   shadingRateFactor;
//
//  RenderPass  			pass;
//
//  bool                  isShadowPass;
//  bool                  isStereoPass;//r773
//  
//  int					renderFrame;
//
//  // shadows specific job options
//  //bool     			isShadow;
//  bool                  everyFrame;
//
//  MString               shadowObjectSet;
//  bool                  shadowArchiveRibDone;
//  
//  ShadowType            shadowType;
//  ShadowHiderType       shadowHiderType;
//
//  bool                  hasShadowCam;
//  bool                  shadowAggregation;
//  
//  // MidPoint shadows specific job options
//  //bool                isMidPointShadow;
//  float                 midPointRatio;
//
//  // MinMax shadows specific job options
//  //bool                  isMinMaxShadow;
//
//  // Deep shadows specific job options
//  //bool                  deepShadows;
//  //int                   shadowPixelSamples;
//  VolumeInterpretation  volume;
//  //int                   shadowVolumeInterpretation;
//  MString               deepShadowOption; // deep shadows display driver option
//  
//  // pointlight shadow job options
//  bool                  isPoint;
//  PointLightDirection   pointDir;
//
//  // camera job options
//  structCamera          camera[ LIQMAXMOTIONSAMPLES ];
//  structCamera          leftCamera[ LIQMAXMOTIONSAMPLES ];    // stereo cam
//  structCamera          rightCamera[ LIQMAXMOTIONSAMPLES ];   // stereo cam
//
//  MDagPath              path;
//  MDagPath              shadowCamPath;
//
//  MString               jobOptions;
//  bool                  gotJobOptions;
//
//  MString               jobFrameRib;
//  bool                  gotJobFrameRib;
//
//  bool                  skip;
//};

//typedef enum SBD_EXTRA_TAG {
//  TAG_CREASE,
//  TAG_HOLE,
//  TAG_CORNER,
//  TAG_BOUNDARY,
//  TAG_STITCH,
//  TAG_FACEVARYINGBOUNDARY
//} SBD_EXTRA_TAG;








#endif//liquid_H
