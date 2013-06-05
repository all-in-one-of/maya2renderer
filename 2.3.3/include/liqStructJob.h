#ifndef _LIQ_STRUCT_JOB_H_
#define _LIQ_STRUCT_JOB_H_

#include <common/prerequest_maya.h>
#include "liqEnum.h"

#define LIQMAXMOTIONSAMPLES 16

enum VolumeInterpretation {
	viNone       = 0, // renderer doesn't support DSMs
	viDiscrete   = 1,
	viContinuous = 2
};

enum PointLightDirection {
	pPX     = 0,
	pPY     = 1,
	pPZ     = 2,
	pNX     = 3,
	pNY     = 4,
	pNZ     = 5
};

typedef struct structCamera
{
	MMatrix  mat;   // camera inverse matrix
	double  neardb;
	double  fardb;
	double  hFOV;
	int     isOrtho;
	double  orthoWidth;
	double  orthoHeight;
	MString name;
	bool    motionBlur;
	double  shutter;
	double  fStop;
	double  focalDistance;
	double  focalLength;
	double  horizontalFilmOffset;
	double  verticalFilmOffset;
	int width;
	int height;
	double fov_ratio;
	structCamera *rightCam;
	structCamera *leftCam;
} structCamera;

 enum RenderPass {
   rpNone        = -1,
   rpHeroPass    = 0,
   rpShadowPass  = 1, // special shadow pass for compositing purpose
   rpShadowMap   = 2,
   rpReflectMap  = 3,
   rpEnvMap      = 4,
   rpMakeTexture = 5
 };

struct structJob 
{
	MString name;
	MString renderName;

	MString ribFileName;

	MString texName;

	MString imageName;
	MString imageMode;
	MString format;

	int     width, height;
	float   aspectRatio;

	short   samples;
	float   shadingRate;
	float   shadingRateFactor;

	RenderPass  			pass;

	bool                  isShadowPass;
	bool                  isStereoPass;//r773

	int					renderFrame;

	// shadows specific job options
	//bool     			isShadow;
	bool                  everyFrame;

	MString               shadowObjectSet;
	bool                  shadowArchiveRibDone;

	ShadowType            shadowType;
	ShadowHiderType       shadowHiderType;

	bool                  hasShadowCam;
	bool                  shadowAggregation;

	// MidPoint shadows specific job options
	//bool                isMidPointShadow;
	float                 midPointRatio;

	// MinMax shadows specific job options
	//bool                  isMinMaxShadow;

	// Deep shadows specific job options
	//bool                  deepShadows;
	//int                   shadowPixelSamples;
	VolumeInterpretation  volume;
	//int                   shadowVolumeInterpretation;
	MString               deepShadowOption; // deep shadows display driver option

	// pointlight shadow job options
	bool                  isPoint;
	PointLightDirection   pointDir;

	// camera job options
	structCamera          camera[ LIQMAXMOTIONSAMPLES ];
	structCamera          leftCamera[ LIQMAXMOTIONSAMPLES ];    // stereo cam
	structCamera          rightCamera[ LIQMAXMOTIONSAMPLES ];   // stereo cam

	MDagPath              path;
	MDagPath              shadowCamPath;

	MString               jobOptions;
	bool                  gotJobOptions;

	MString               jobFrameRib;
	bool                  gotJobFrameRib;

	bool                  skip;
};

#endif//_LIQ_STRUCT_JOB_H_