#ifndef _LIQ_ENUM_H_
#define _LIQ_ENUM_H_

enum PixelFilterType {
	pfBoxFilter            = 0,
	pfTriangleFilter       = 1,
	pfCatmullRomFilter     = 2,
	pfGaussianFilter       = 3,
	pfSincFilter           = 4,

	pfBlackmanHarrisFilter = 5,
	pfMitchellFilter       = 6,
	pfSepCatmullRomFilter  = 7,

	pfLanczosFilter        = 8,
	pfBesselFilter         = 9,
	pfDiskFilter           = 10
};

enum ShadowType {
	stStandart = 0,
	stMidPoint = 1,
	stMinMax   = 2,
	stDeep     = 3
};

enum ShadowHiderType {
	shNone     = 0,
	shMin      = 1,
	shMax      = 2,
	shAverage  = 3,
	shMidPoint = 4
};

typedef enum SBD_EXTRA_TAG {
	TAG_CREASE,
	TAG_HOLE,
	TAG_CORNER,
	TAG_BOUNDARY,
	TAG_STITCH,
	TAG_FACEVARYINGBOUNDARY
} SBD_EXTRA_TAG;

enum ObjectType {
	MRT_Unknown         = 0,
	MRT_Nurbs           = 1,
	MRT_Mesh            = 2,
	MRT_Light           = 3,
	MRT_Weirdo          = 4,
	MRT_NuCurve         = 5,
	MRT_Particles       = 6,
	MRT_Locator         = 7,
	MRT_RibGen          = 8,
	MRT_Shader          = 9,
	MRT_Coord           = 10,
	MRT_Subdivision     = 11,
	MRT_MayaSubdivision = 12,
	MRT_Custom          = 13,
	MRT_ClipPlane       = 14,
	MRT_PfxToon         = 15,
	MRT_PfxHair         = 16,
	MRT_PfxTube         = 17,
	MRT_PfxLeaf         = 18,
	MRT_PfxPetal        = 19,
	MRT_ImplicitSphere  = 20,
	MRT_Curves          = 21,
	MRT_Shave			  = 22, //added in maya2renderer -yaoyansi
	MRT_Camera		  = 23
};
#endif//_LIQ_ENUM_H_

