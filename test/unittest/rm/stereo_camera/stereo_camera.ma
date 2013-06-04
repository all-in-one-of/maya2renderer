//Maya ASCII 2012 scene
//Name: stereo_camera.ma
//Last modified: Tue, Jun 04, 2013 02:50:48 PM
//Codeset: 936
requires maya "2012";
requires "liquid_2012x32d" "2.3.5";
requires "stereoCamera" "10.0";
requires "renderman" "0.0.1";
currentUnit -l centimeter -a degree -t film;
fileInfo "application" "maya";
fileInfo "product" "Maya 2012";
fileInfo "version" "2012";
fileInfo "cutIdentifier" "001200000000-796618";
fileInfo "osv" "Microsoft Windows 7 Ultimate Edition, 64-bit Windows 7 Service Pack 1 (Build 7601)\n";
createNode transform -s -n "persp";
	setAttr ".v" no;
	setAttr ".t" -type "double3" 1.1246871360654112 2.4650743034296188 35.788124956197791 ;
	setAttr ".r" -type "double3" -3.9383527296064602 1.8000000000001286 0 ;
createNode camera -s -n "perspShape" -p "persp";
	setAttr -k off ".v" no;
	setAttr ".rnd" no;
	setAttr ".fl" 34.999999999999993;
	setAttr ".coi" 35.890547507049192;
	setAttr ".imn" -type "string" "persp";
	setAttr ".den" -type "string" "persp_depth";
	setAttr ".man" -type "string" "persp_mask";
	setAttr ".hc" -type "string" "viewSet -p %camera";
createNode transform -s -n "top";
	setAttr ".v" no;
	setAttr ".t" -type "double3" 10.538279672913509 100.09999999999992 262.148600155042 ;
	setAttr ".r" -type "double3" -89.999999999999986 0 0 ;
createNode camera -s -n "topShape" -p "top";
	setAttr -k off ".v" no;
	setAttr ".rnd" no;
	setAttr ".coi" 100.1;
	setAttr ".ow" 1440.1626020788867;
	setAttr ".imn" -type "string" "top";
	setAttr ".den" -type "string" "top_depth";
	setAttr ".man" -type "string" "top_mask";
	setAttr ".tp" -type "double3" -6.5028913838008116 0 -0.9580362523877044 ;
	setAttr ".hc" -type "string" "viewSet -t %camera";
	setAttr ".o" yes;
createNode transform -s -n "front";
	setAttr ".v" no;
	setAttr ".t" -type "double3" 0 69.661081224550216 100.1 ;
createNode camera -s -n "frontShape" -p "front";
	setAttr -k off ".v" no;
	setAttr ".rnd" no;
	setAttr ".coi" 100.1;
	setAttr ".ow" 553.63899243764843;
	setAttr ".imn" -type "string" "front";
	setAttr ".den" -type "string" "front_depth";
	setAttr ".man" -type "string" "front_mask";
	setAttr ".hc" -type "string" "viewSet -f %camera";
	setAttr ".o" yes;
createNode transform -s -n "side";
	setAttr ".v" no;
	setAttr ".t" -type "double3" 100.1 0 0 ;
	setAttr ".r" -type "double3" 0 89.999999999999986 0 ;
createNode camera -s -n "sideShape" -p "side";
	setAttr -k off ".v" no;
	setAttr ".rnd" no;
	setAttr ".coi" 100.1;
	setAttr ".ow" 30;
	setAttr ".imn" -type "string" "side";
	setAttr ".den" -type "string" "side_depth";
	setAttr ".man" -type "string" "side_mask";
	setAttr ".hc" -type "string" "viewSet -s %camera";
	setAttr ".o" yes;
createNode transform -n "pCube1";
	setAttr ".t" -type "double3" -95.867946975383674 0 -67.693612828745671 ;
	setAttr ".r" -type "double3" 2.9051091852431346 -0.34250960788051871 -0.36932200614451044 ;
	setAttr ".s" -type "double3" 4.3722405263656112 10.045462998261689 6.5890484741555913 ;
createNode mesh -n "pCubeShape1" -p "pCube1";
	addAttr -ci true -sn "mso" -ln "miShadingSamplesOverride" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "msh" -ln "miShadingSamples" -min 0 -smx 8 -at "float";
	addAttr -ci true -sn "mdo" -ln "miMaxDisplaceOverride" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "mmd" -ln "miMaxDisplace" -min 0 -smx 1 -at "float";
	addAttr -ci true -sn "shaderFullPath" -ln "shaderFullPath" -dt "string";
	addAttr -ci true -sn "liquidSurfaceShaderNode" -ln "liquidSurfaceShaderNode" -at "message";
	setAttr -k off ".v";
	setAttr ".vir" yes;
	setAttr ".vif" yes;
	setAttr ".uvst[0].uvsn" -type "string" "map1";
	setAttr ".cuvs" -type "string" "map1";
	setAttr ".dcc" -type "string" "Ambient+Diffuse";
	setAttr ".covm[0]"  0 1 1;
	setAttr ".cdvm[0]"  0 1 1;
	setAttr ".bnr" 0;
	setAttr ".shaderFullPath" -type "string" "E:\\MyDocuments\\maya\\projects\\default\\generatedShader\\lambert1.slo";
createNode transform -n "pointLight1";
	setAttr ".t" -type "double3" -101.94970134135926 79.381387212196131 37.346807741166629 ;
	setAttr ".r" -type "double3" 2.9051091852431346 -0.34250960788051871 -0.36932200614451044 ;
createNode pointLight -n "pointLightShape1" -p "pointLight1";
	addAttr -ci true -sn "elvishrayShader" -ln "elvishrayShader" -dt "string";
	addAttr -ci true -sn "elvishrayUseOverrideShader" -ln "elvishrayUseOverrideShader" 
		-min 0 -max 1 -at "bool";
	addAttr -ci true -sn "elvishrayShadowSpatialQualityX" -ln "elvishrayShadowSpatialQualityX" 
		-dv 2 -min 1 -max 16 -smx 16 -at "long";
	addAttr -ci true -sn "elvishrayShadowSpatialQualityY" -ln "elvishrayShadowSpatialQualityY" 
		-dv 2 -min 1 -max 16 -smx 16 -at "long";
	addAttr -ci true -sn "elvishrayVolumeShadows" -ln "elvishrayVolumeShadows" -min 
		0 -max 1 -at "bool";
	addAttr -ci true -sn "elvishrayVolumeShadowZChannels" -ln "elvishrayVolumeShadowZChannels" 
		-dv 3 -min 2 -max 64 -smx 8 -at "long";
	addAttr -ci true -sn "elvishrayVolumeShadowOpaqueWidth" -ln "elvishrayVolumeShadowOpaqueWidth" 
		-dv 0.05 -min 0 -max 10 -smx 1 -at "float";
	addAttr -ci true -sn "elvishrayShadowGeomSet" -ln "elvishrayShadowGeomSet" -dt "string";
	addAttr -ci true -sn "elvishrayUseOcclusion" -ln "elvishrayUseOcclusion" -min 0 
		-max 1 -at "bool";
	addAttr -ci true -sn "elvishrayEmitSubsurface" -ln "elvishrayEmitSubsurface" -dv 
		1 -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "elvishrayUseCaustics" -ln "elvishrayUseCaustics" -min 0 -max 
		1 -at "bool";
	addAttr -ci true -sn "elvishrayCausticsMaxError" -ln "elvishrayCausticsMaxError" 
		-dv 0.25 -min 0 -max 1 -smx 1 -at "float";
	addAttr -ci true -sn "elvishrayCausticsMaxPixelDist" -ln "elvishrayCausticsMaxPixelDist" 
		-dv 20 -min 0 -max 1000 -smx 50 -at "float";
	addAttr -ci true -sn "elvishrayCausticsMinPhotons" -ln "elvishrayCausticsMinPhotons" 
		-dv 10 -min 0 -max 10000 -smx 100 -at "float";
	addAttr -ci true -sn "elvishrayCausticsIntensity" -ln "elvishrayCausticsIntensity" 
		-dv 1 -min 0 -max 100 -smx 1 -at "float";
	addAttr -ci true -sn "elvishrayCausticsEnvName" -ln "elvishrayCausticsEnvName" -dt "string";
	addAttr -ci true -sn "elvishrayCausticsDistribution" -ln "elvishrayCausticsDistribution" 
		-dt "string";
	addAttr -ci true -sn "elvishrayCausticsJitter" -ln "elvishrayCausticsJitter" -min 
		0 -max 1 -at "bool";
	addAttr -ci true -sn "elvishrayCausticsPhotonsPerPoint" -ln "elvishrayCausticsPhotonsPerPoint" 
		-dv 1 -min 1 -max 256 -smx 16 -at "float";
	setAttr -k off ".v";
	setAttr ".in" 3.4959349632263184;
	setAttr ".us" no;
	setAttr ".elvishrayShader" -type "string" "";
	setAttr ".elvishrayShadowGeomSet" -type "string" "";
	setAttr ".elvishrayCausticsEnvName" -type "string" "reflection";
	setAttr ".elvishrayCausticsDistribution" -type "string" "omni";
createNode transform -n "camera1";
	setAttr ".t" -type "double3" -1447.076950631988 1437.473130135761 1512.9829865727747 ;
	setAttr ".r" -type "double3" -34.79999999999999 -43.6 0 ;
createNode camera -n "cameraShape1" -p "camera1";
	setAttr -k off ".v";
	setAttr ".rnd" no;
	setAttr ".cap" -type "double2" 1.41732 0.94488 ;
	setAttr ".ff" 0;
	setAttr ".ncp" 0.01;
	setAttr ".coi" 2518.7295545961001;
	setAttr ".ow" 30;
	setAttr ".imn" -type "string" "camera1";
	setAttr ".den" -type "string" "camera1_depth";
	setAttr ".man" -type "string" "camera1_mask";
createNode transform -n "pointLight2";
	setAttr ".t" -type "double3" 2.1404379079374714 75.497773341998197 168.21900170303627 ;
createNode pointLight -n "pointLightShape2" -p "pointLight2";
	setAttr -k off ".v";
	setAttr ".in" 3.4959349632263184;
	setAttr ".us" no;
createNode transform -n "pCube2";
	setAttr ".t" -type "double3" 51.468243531947778 4.4807518740855228 38.788855129481476 ;
	setAttr ".r" -type "double3" 2.9051229668571179 -0.34464758796798256 -0.36932208878653633 ;
	setAttr ".s" -type "double3" 24.119118567610705 6.6551364636648938 25.433792981422577 ;
createNode mesh -n "pCubeShape2" -p "pCube2";
	addAttr -ci true -sn "mso" -ln "miShadingSamplesOverride" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "msh" -ln "miShadingSamples" -min 0 -smx 8 -at "float";
	addAttr -ci true -sn "mdo" -ln "miMaxDisplaceOverride" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "mmd" -ln "miMaxDisplace" -min 0 -smx 1 -at "float";
	setAttr -k off ".v";
	setAttr ".vir" yes;
	setAttr ".vif" yes;
	setAttr ".uvst[0].uvsn" -type "string" "map1";
	setAttr ".cuvs" -type "string" "map1";
	setAttr ".dcc" -type "string" "Ambient+Diffuse";
	setAttr ".covm[0]"  0 1 1;
	setAttr ".cdvm[0]"  0 1 1;
	setAttr ".bnr" 0;
createNode transform -n "pCube3";
	setAttr ".t" -type "double3" -23.677691132639652 5.2536513179261268 -2.0233064277540791 ;
	setAttr ".r" -type "double3" 2.9051091852431346 -0.34250960788051871 -0.36932200614451044 ;
	setAttr ".s" -type "double3" 5.514575928502957 6.592800544709446 13.578308431251619 ;
createNode mesh -n "pCubeShape3" -p "pCube3";
	addAttr -ci true -sn "mso" -ln "miShadingSamplesOverride" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "msh" -ln "miShadingSamples" -min 0 -smx 8 -at "float";
	addAttr -ci true -sn "mdo" -ln "miMaxDisplaceOverride" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "mmd" -ln "miMaxDisplace" -min 0 -smx 1 -at "float";
	setAttr -k off ".v";
	setAttr ".vir" yes;
	setAttr ".vif" yes;
	setAttr ".uvst[0].uvsn" -type "string" "map1";
	setAttr ".cuvs" -type "string" "map1";
	setAttr ".dcc" -type "string" "Ambient+Diffuse";
	setAttr ".covm[0]"  0 1 1;
	setAttr ".cdvm[0]"  0 1 1;
	setAttr ".bnr" 0;
createNode stereoRigTransform -n "stereoCamera";
	addAttr -ci true -sn "stereoRigType" -ln "stereoRigType" -dt "string";
	addAttr -ci true -sn "centerCam" -ln "centerCam" -at "message";
	addAttr -ci true -sn "leftCam" -ln "leftCam" -at "message";
	addAttr -ci true -sn "rightCam" -ln "rightCam" -at "message";
	setAttr ".t" -type "double3" 12.041771104042311 85.648593213476659 262.98447880679333 ;
	setAttr ".r" -type "double3" -14.233956413772333 0 0 ;
	setAttr ".stereoRigType" -type "string" "StereoCamera";
createNode stereoRigCamera -n "stereoCameraCenterCamShape" -p "stereoCamera";
	setAttr -k off ".v";
	setAttr ".rnd" no;
	setAttr ".sm" 2;
	setAttr ".zpp" yes;
	setAttr ".svv" yes;
createNode stereoRigFrustum -n "stereoCameraFrustum" -p "stereoCamera";
	setAttr -k off ".v";
	setAttr -cb off ".lpx";
	setAttr -cb off ".lpy";
	setAttr -cb off ".lpz";
	setAttr -cb off ".lsx";
	setAttr -cb off ".lsy";
	setAttr -cb off ".lsz";
createNode transform -n "stereoCameraLeft" -p "stereoCamera";
	setAttr -k off ".v";
	setAttr -l on ".t";
	setAttr -l on ".r";
	setAttr -k off ".sx";
	setAttr -k off ".sy";
	setAttr -k off ".sz";
createNode camera -n "stereoCameraLeftShape" -p "stereoCameraLeft";
	setAttr -k off ".v";
	setAttr -k off ".hfa";
	setAttr -k off ".vfa";
	setAttr -k off ".fl";
	setAttr -k off ".lsr";
	setAttr -av ".ncp";
	setAttr -av ".fcp";
	setAttr -k off ".fs";
	setAttr -k off ".fd";
	setAttr -k off ".sa";
	setAttr -k off ".coi";
	setAttr ".imn" -type "string" "camera2";
	setAttr ".den" -type "string" "camera2_depth";
	setAttr ".man" -type "string" "camera2_mask";
createNode transform -n "stereoCameraRight" -p "stereoCamera";
	setAttr -k off ".v";
	setAttr -l on ".t";
	setAttr -l on ".r";
	setAttr -k off ".sx";
	setAttr -k off ".sy";
	setAttr -k off ".sz";
createNode camera -n "stereoCameraRightShape" -p "stereoCameraRight";
	setAttr -k off ".v";
	setAttr -k off ".hfa";
	setAttr -k off ".vfa";
	setAttr -k off ".fl";
	setAttr -k off ".lsr";
	setAttr -av ".ncp";
	setAttr -av ".fcp";
	setAttr -k off ".fs";
	setAttr -k off ".fd";
	setAttr -k off ".sa";
	setAttr -k off ".coi";
	setAttr ".imn" -type "string" "camera2";
	setAttr ".den" -type "string" "camera2_depth";
	setAttr ".man" -type "string" "camera2_mask";
createNode lightLinker -s -n "lightLinker1";
	setAttr -s 4 ".lnk";
	setAttr -s 2 ".ign";
	setAttr -s 4 ".slnk";
createNode displayLayerManager -n "layerManager";
createNode displayLayer -n "defaultLayer";
createNode renderLayerManager -n "renderLayerManager";
createNode renderLayer -n "defaultRenderLayer";
	setAttr ".g" yes;
createNode polyCube -n "polyCube1";
	setAttr ".w" 12.835782894353837;
	setAttr ".h" 5.8991033835649471;
	setAttr ".d" 8.5912510407797846;
	setAttr ".cuv" 4;
createNode polyTriangulate -n "polyTriangulate1";
	setAttr ".ics" -type "componentList" 1 "f[*]";
createNode script -n "uiConfigurationScriptNode";
	setAttr ".b" -type "string" (
		"// Maya Mel UI Configuration File.\n//\n//  This script is machine generated.  Edit at your own risk.\n//\n//\n\nglobal string $gMainPane;\nif (`paneLayout -exists $gMainPane`) {\n\n\tglobal int $gUseScenePanelConfig;\n\tint    $useSceneConfig = $gUseScenePanelConfig;\n\tint    $menusOkayInPanels = `optionVar -q allowMenusInPanels`;\tint    $nVisPanes = `paneLayout -q -nvp $gMainPane`;\n\tint    $nPanes = 0;\n\tstring $editorName;\n\tstring $panelName;\n\tstring $itemFilterName;\n\tstring $panelConfig;\n\n\t//\n\t//  get current state of the UI\n\t//\n\tsceneUIReplacement -update $gMainPane;\n\n\t$panelName = `sceneUIReplacement -getNextPanel \"modelPanel\" (localizedPanelLabel(\"Top View\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `modelPanel -unParent -l (localizedPanelLabel(\"Top View\")) -mbv $menusOkayInPanels `;\n\t\t\t$editorName = $panelName;\n            modelEditor -e \n                -camera \"stereoCameraCenterCamShape\" \n                -useInteractiveMode 0\n                -displayLights \"default\" \n                -displayAppearance \"wireframe\" \n"
		+ "                -activeOnly 0\n                -ignorePanZoom 0\n                -wireframeOnShaded 0\n                -headsUpDisplay 1\n                -selectionHiliteDisplay 1\n                -useDefaultMaterial 0\n                -bufferMode \"double\" \n                -twoSidedLighting 1\n                -backfaceCulling 0\n                -xray 0\n                -jointXray 0\n                -activeComponentsXray 0\n                -displayTextures 0\n                -smoothWireframe 0\n                -lineWidth 1\n                -textureAnisotropic 0\n                -textureHilight 1\n                -textureSampling 2\n                -textureDisplay \"modulate\" \n                -textureMaxSize 16384\n                -fogging 0\n                -fogSource \"fragment\" \n                -fogMode \"linear\" \n                -fogStart 0\n                -fogEnd 100\n                -fogDensity 0.1\n                -fogColor 0.5 0.5 0.5 1 \n                -maxConstantTransparency 1\n                -rendererName \"base_OpenGL_Renderer\" \n"
		+ "                -colorResolution 256 256 \n                -bumpResolution 512 512 \n                -textureCompression 0\n                -transparencyAlgorithm \"frontAndBackCull\" \n                -transpInShadows 0\n                -cullingOverride \"none\" \n                -lowQualityLighting 0\n                -maximumNumHardwareLights 1\n                -occlusionCulling 0\n                -shadingModel 0\n                -useBaseRenderer 0\n                -useReducedRenderer 0\n                -smallObjectCulling 0\n                -smallObjectThreshold -1 \n                -interactiveDisableShadows 0\n                -interactiveBackFaceCull 0\n                -sortTransparent 1\n                -nurbsCurves 1\n                -nurbsSurfaces 1\n                -polymeshes 1\n                -subdivSurfaces 1\n                -planes 1\n                -lights 1\n                -cameras 1\n                -controlVertices 1\n                -hulls 1\n                -grid 1\n                -joints 1\n                -ikHandles 1\n"
		+ "                -deformers 1\n                -dynamics 1\n                -fluids 1\n                -hairSystems 1\n                -follicles 1\n                -nCloths 1\n                -nParticles 1\n                -nRigids 1\n                -dynamicConstraints 1\n                -locators 1\n                -manipulators 1\n                -dimensions 1\n                -handles 1\n                -pivots 1\n                -textures 1\n                -strokes 1\n                -motionTrails 1\n                -shadows 0\n                $editorName;\nmodelEditor -e -viewSelected 0 $editorName;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tmodelPanel -edit -l (localizedPanelLabel(\"Top View\")) -mbv $menusOkayInPanels  $panelName;\n\t\t$editorName = $panelName;\n        modelEditor -e \n            -camera \"stereoCameraCenterCamShape\" \n            -useInteractiveMode 0\n            -displayLights \"default\" \n            -displayAppearance \"wireframe\" \n            -activeOnly 0\n            -ignorePanZoom 0\n            -wireframeOnShaded 0\n"
		+ "            -headsUpDisplay 1\n            -selectionHiliteDisplay 1\n            -useDefaultMaterial 0\n            -bufferMode \"double\" \n            -twoSidedLighting 1\n            -backfaceCulling 0\n            -xray 0\n            -jointXray 0\n            -activeComponentsXray 0\n            -displayTextures 0\n            -smoothWireframe 0\n            -lineWidth 1\n            -textureAnisotropic 0\n            -textureHilight 1\n            -textureSampling 2\n            -textureDisplay \"modulate\" \n            -textureMaxSize 16384\n            -fogging 0\n            -fogSource \"fragment\" \n            -fogMode \"linear\" \n            -fogStart 0\n            -fogEnd 100\n            -fogDensity 0.1\n            -fogColor 0.5 0.5 0.5 1 \n            -maxConstantTransparency 1\n            -rendererName \"base_OpenGL_Renderer\" \n            -colorResolution 256 256 \n            -bumpResolution 512 512 \n            -textureCompression 0\n            -transparencyAlgorithm \"frontAndBackCull\" \n            -transpInShadows 0\n            -cullingOverride \"none\" \n"
		+ "            -lowQualityLighting 0\n            -maximumNumHardwareLights 1\n            -occlusionCulling 0\n            -shadingModel 0\n            -useBaseRenderer 0\n            -useReducedRenderer 0\n            -smallObjectCulling 0\n            -smallObjectThreshold -1 \n            -interactiveDisableShadows 0\n            -interactiveBackFaceCull 0\n            -sortTransparent 1\n            -nurbsCurves 1\n            -nurbsSurfaces 1\n            -polymeshes 1\n            -subdivSurfaces 1\n            -planes 1\n            -lights 1\n            -cameras 1\n            -controlVertices 1\n            -hulls 1\n            -grid 1\n            -joints 1\n            -ikHandles 1\n            -deformers 1\n            -dynamics 1\n            -fluids 1\n            -hairSystems 1\n            -follicles 1\n            -nCloths 1\n            -nParticles 1\n            -nRigids 1\n            -dynamicConstraints 1\n            -locators 1\n            -manipulators 1\n            -dimensions 1\n            -handles 1\n            -pivots 1\n"
		+ "            -textures 1\n            -strokes 1\n            -motionTrails 1\n            -shadows 0\n            $editorName;\nmodelEditor -e -viewSelected 0 $editorName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextPanel \"modelPanel\" (localizedPanelLabel(\"Side View\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `modelPanel -unParent -l (localizedPanelLabel(\"Side View\")) -mbv $menusOkayInPanels `;\n\t\t\t$editorName = $panelName;\n            modelEditor -e \n                -camera \"camera1\" \n                -useInteractiveMode 0\n                -displayLights \"default\" \n                -displayAppearance \"smoothShaded\" \n                -activeOnly 0\n                -ignorePanZoom 0\n                -wireframeOnShaded 0\n                -headsUpDisplay 1\n                -selectionHiliteDisplay 1\n                -useDefaultMaterial 0\n                -bufferMode \"double\" \n                -twoSidedLighting 1\n                -backfaceCulling 0\n"
		+ "                -xray 0\n                -jointXray 0\n                -activeComponentsXray 0\n                -displayTextures 0\n                -smoothWireframe 0\n                -lineWidth 1\n                -textureAnisotropic 0\n                -textureHilight 1\n                -textureSampling 2\n                -textureDisplay \"modulate\" \n                -textureMaxSize 16384\n                -fogging 0\n                -fogSource \"fragment\" \n                -fogMode \"linear\" \n                -fogStart 0\n                -fogEnd 100\n                -fogDensity 0.1\n                -fogColor 0.5 0.5 0.5 1 \n                -maxConstantTransparency 1\n                -rendererName \"hwRender_OpenGL_Renderer\" \n                -colorResolution 256 256 \n                -bumpResolution 512 512 \n                -textureCompression 0\n                -transparencyAlgorithm \"frontAndBackCull\" \n                -transpInShadows 0\n                -cullingOverride \"none\" \n                -lowQualityLighting 0\n                -maximumNumHardwareLights 1\n"
		+ "                -occlusionCulling 0\n                -shadingModel 0\n                -useBaseRenderer 0\n                -useReducedRenderer 0\n                -smallObjectCulling 0\n                -smallObjectThreshold -1 \n                -interactiveDisableShadows 0\n                -interactiveBackFaceCull 0\n                -sortTransparent 1\n                -nurbsCurves 1\n                -nurbsSurfaces 1\n                -polymeshes 1\n                -subdivSurfaces 1\n                -planes 1\n                -lights 1\n                -cameras 1\n                -controlVertices 1\n                -hulls 1\n                -grid 1\n                -joints 1\n                -ikHandles 1\n                -deformers 1\n                -dynamics 1\n                -fluids 1\n                -hairSystems 1\n                -follicles 1\n                -nCloths 1\n                -nParticles 1\n                -nRigids 1\n                -dynamicConstraints 1\n                -locators 1\n                -manipulators 1\n                -dimensions 1\n"
		+ "                -handles 1\n                -pivots 1\n                -textures 1\n                -strokes 1\n                -motionTrails 1\n                -shadows 0\n                $editorName;\nmodelEditor -e -viewSelected 0 $editorName;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tmodelPanel -edit -l (localizedPanelLabel(\"Side View\")) -mbv $menusOkayInPanels  $panelName;\n\t\t$editorName = $panelName;\n        modelEditor -e \n            -camera \"camera1\" \n            -useInteractiveMode 0\n            -displayLights \"default\" \n            -displayAppearance \"smoothShaded\" \n            -activeOnly 0\n            -ignorePanZoom 0\n            -wireframeOnShaded 0\n            -headsUpDisplay 1\n            -selectionHiliteDisplay 1\n            -useDefaultMaterial 0\n            -bufferMode \"double\" \n            -twoSidedLighting 1\n            -backfaceCulling 0\n            -xray 0\n            -jointXray 0\n            -activeComponentsXray 0\n            -displayTextures 0\n            -smoothWireframe 0\n            -lineWidth 1\n"
		+ "            -textureAnisotropic 0\n            -textureHilight 1\n            -textureSampling 2\n            -textureDisplay \"modulate\" \n            -textureMaxSize 16384\n            -fogging 0\n            -fogSource \"fragment\" \n            -fogMode \"linear\" \n            -fogStart 0\n            -fogEnd 100\n            -fogDensity 0.1\n            -fogColor 0.5 0.5 0.5 1 \n            -maxConstantTransparency 1\n            -rendererName \"hwRender_OpenGL_Renderer\" \n            -colorResolution 256 256 \n            -bumpResolution 512 512 \n            -textureCompression 0\n            -transparencyAlgorithm \"frontAndBackCull\" \n            -transpInShadows 0\n            -cullingOverride \"none\" \n            -lowQualityLighting 0\n            -maximumNumHardwareLights 1\n            -occlusionCulling 0\n            -shadingModel 0\n            -useBaseRenderer 0\n            -useReducedRenderer 0\n            -smallObjectCulling 0\n            -smallObjectThreshold -1 \n            -interactiveDisableShadows 0\n            -interactiveBackFaceCull 0\n"
		+ "            -sortTransparent 1\n            -nurbsCurves 1\n            -nurbsSurfaces 1\n            -polymeshes 1\n            -subdivSurfaces 1\n            -planes 1\n            -lights 1\n            -cameras 1\n            -controlVertices 1\n            -hulls 1\n            -grid 1\n            -joints 1\n            -ikHandles 1\n            -deformers 1\n            -dynamics 1\n            -fluids 1\n            -hairSystems 1\n            -follicles 1\n            -nCloths 1\n            -nParticles 1\n            -nRigids 1\n            -dynamicConstraints 1\n            -locators 1\n            -manipulators 1\n            -dimensions 1\n            -handles 1\n            -pivots 1\n            -textures 1\n            -strokes 1\n            -motionTrails 1\n            -shadows 0\n            $editorName;\nmodelEditor -e -viewSelected 0 $editorName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextPanel \"modelPanel\" (localizedPanelLabel(\"Front View\")) `;\n\tif (\"\" == $panelName) {\n"
		+ "\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `modelPanel -unParent -l (localizedPanelLabel(\"Front View\")) -mbv $menusOkayInPanels `;\n\t\t\t$editorName = $panelName;\n            modelEditor -e \n                -camera \"front\" \n                -useInteractiveMode 0\n                -displayLights \"default\" \n                -displayAppearance \"wireframe\" \n                -activeOnly 0\n                -ignorePanZoom 0\n                -wireframeOnShaded 0\n                -headsUpDisplay 1\n                -selectionHiliteDisplay 1\n                -useDefaultMaterial 0\n                -bufferMode \"double\" \n                -twoSidedLighting 1\n                -backfaceCulling 0\n                -xray 0\n                -jointXray 0\n                -activeComponentsXray 0\n                -displayTextures 0\n                -smoothWireframe 0\n                -lineWidth 1\n                -textureAnisotropic 0\n                -textureHilight 1\n                -textureSampling 2\n                -textureDisplay \"modulate\" \n                -textureMaxSize 16384\n"
		+ "                -fogging 0\n                -fogSource \"fragment\" \n                -fogMode \"linear\" \n                -fogStart 0\n                -fogEnd 100\n                -fogDensity 0.1\n                -fogColor 0.5 0.5 0.5 1 \n                -maxConstantTransparency 1\n                -rendererName \"base_OpenGL_Renderer\" \n                -colorResolution 256 256 \n                -bumpResolution 512 512 \n                -textureCompression 0\n                -transparencyAlgorithm \"frontAndBackCull\" \n                -transpInShadows 0\n                -cullingOverride \"none\" \n                -lowQualityLighting 0\n                -maximumNumHardwareLights 1\n                -occlusionCulling 0\n                -shadingModel 0\n                -useBaseRenderer 0\n                -useReducedRenderer 0\n                -smallObjectCulling 0\n                -smallObjectThreshold -1 \n                -interactiveDisableShadows 0\n                -interactiveBackFaceCull 0\n                -sortTransparent 1\n                -nurbsCurves 1\n"
		+ "                -nurbsSurfaces 1\n                -polymeshes 1\n                -subdivSurfaces 1\n                -planes 1\n                -lights 1\n                -cameras 1\n                -controlVertices 1\n                -hulls 1\n                -grid 1\n                -joints 1\n                -ikHandles 1\n                -deformers 1\n                -dynamics 1\n                -fluids 1\n                -hairSystems 1\n                -follicles 1\n                -nCloths 1\n                -nParticles 1\n                -nRigids 1\n                -dynamicConstraints 1\n                -locators 1\n                -manipulators 1\n                -dimensions 1\n                -handles 1\n                -pivots 1\n                -textures 1\n                -strokes 1\n                -motionTrails 1\n                -shadows 0\n                $editorName;\nmodelEditor -e -viewSelected 0 $editorName;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tmodelPanel -edit -l (localizedPanelLabel(\"Front View\")) -mbv $menusOkayInPanels  $panelName;\n"
		+ "\t\t$editorName = $panelName;\n        modelEditor -e \n            -camera \"front\" \n            -useInteractiveMode 0\n            -displayLights \"default\" \n            -displayAppearance \"wireframe\" \n            -activeOnly 0\n            -ignorePanZoom 0\n            -wireframeOnShaded 0\n            -headsUpDisplay 1\n            -selectionHiliteDisplay 1\n            -useDefaultMaterial 0\n            -bufferMode \"double\" \n            -twoSidedLighting 1\n            -backfaceCulling 0\n            -xray 0\n            -jointXray 0\n            -activeComponentsXray 0\n            -displayTextures 0\n            -smoothWireframe 0\n            -lineWidth 1\n            -textureAnisotropic 0\n            -textureHilight 1\n            -textureSampling 2\n            -textureDisplay \"modulate\" \n            -textureMaxSize 16384\n            -fogging 0\n            -fogSource \"fragment\" \n            -fogMode \"linear\" \n            -fogStart 0\n            -fogEnd 100\n            -fogDensity 0.1\n            -fogColor 0.5 0.5 0.5 1 \n            -maxConstantTransparency 1\n"
		+ "            -rendererName \"base_OpenGL_Renderer\" \n            -colorResolution 256 256 \n            -bumpResolution 512 512 \n            -textureCompression 0\n            -transparencyAlgorithm \"frontAndBackCull\" \n            -transpInShadows 0\n            -cullingOverride \"none\" \n            -lowQualityLighting 0\n            -maximumNumHardwareLights 1\n            -occlusionCulling 0\n            -shadingModel 0\n            -useBaseRenderer 0\n            -useReducedRenderer 0\n            -smallObjectCulling 0\n            -smallObjectThreshold -1 \n            -interactiveDisableShadows 0\n            -interactiveBackFaceCull 0\n            -sortTransparent 1\n            -nurbsCurves 1\n            -nurbsSurfaces 1\n            -polymeshes 1\n            -subdivSurfaces 1\n            -planes 1\n            -lights 1\n            -cameras 1\n            -controlVertices 1\n            -hulls 1\n            -grid 1\n            -joints 1\n            -ikHandles 1\n            -deformers 1\n            -dynamics 1\n            -fluids 1\n"
		+ "            -hairSystems 1\n            -follicles 1\n            -nCloths 1\n            -nParticles 1\n            -nRigids 1\n            -dynamicConstraints 1\n            -locators 1\n            -manipulators 1\n            -dimensions 1\n            -handles 1\n            -pivots 1\n            -textures 1\n            -strokes 1\n            -motionTrails 1\n            -shadows 0\n            $editorName;\nmodelEditor -e -viewSelected 0 $editorName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextPanel \"modelPanel\" (localizedPanelLabel(\"Persp View\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `modelPanel -unParent -l (localizedPanelLabel(\"Persp View\")) -mbv $menusOkayInPanels `;\n\t\t\t$editorName = $panelName;\n            modelEditor -e \n                -camera \"camera1\" \n                -useInteractiveMode 0\n                -displayLights \"default\" \n                -displayAppearance \"wireframe\" \n                -activeOnly 0\n                -ignorePanZoom 0\n"
		+ "                -wireframeOnShaded 0\n                -headsUpDisplay 1\n                -selectionHiliteDisplay 1\n                -useDefaultMaterial 0\n                -bufferMode \"double\" \n                -twoSidedLighting 1\n                -backfaceCulling 0\n                -xray 0\n                -jointXray 0\n                -activeComponentsXray 0\n                -displayTextures 0\n                -smoothWireframe 0\n                -lineWidth 1\n                -textureAnisotropic 0\n                -textureHilight 1\n                -textureSampling 2\n                -textureDisplay \"modulate\" \n                -textureMaxSize 16384\n                -fogging 0\n                -fogSource \"fragment\" \n                -fogMode \"linear\" \n                -fogStart 0\n                -fogEnd 100\n                -fogDensity 0.1\n                -fogColor 0.5 0.5 0.5 1 \n                -maxConstantTransparency 1\n                -rendererName \"base_OpenGL_Renderer\" \n                -colorResolution 256 256 \n                -bumpResolution 512 512 \n"
		+ "                -textureCompression 0\n                -transparencyAlgorithm \"frontAndBackCull\" \n                -transpInShadows 0\n                -cullingOverride \"none\" \n                -lowQualityLighting 0\n                -maximumNumHardwareLights 1\n                -occlusionCulling 0\n                -shadingModel 0\n                -useBaseRenderer 0\n                -useReducedRenderer 0\n                -smallObjectCulling 0\n                -smallObjectThreshold -1 \n                -interactiveDisableShadows 0\n                -interactiveBackFaceCull 0\n                -sortTransparent 1\n                -nurbsCurves 1\n                -nurbsSurfaces 1\n                -polymeshes 1\n                -subdivSurfaces 1\n                -planes 1\n                -lights 1\n                -cameras 1\n                -controlVertices 1\n                -hulls 1\n                -grid 1\n                -joints 1\n                -ikHandles 1\n                -deformers 1\n                -dynamics 1\n                -fluids 1\n"
		+ "                -hairSystems 1\n                -follicles 1\n                -nCloths 1\n                -nParticles 1\n                -nRigids 1\n                -dynamicConstraints 1\n                -locators 1\n                -manipulators 1\n                -dimensions 1\n                -handles 1\n                -pivots 1\n                -textures 1\n                -strokes 1\n                -motionTrails 1\n                -shadows 0\n                $editorName;\nmodelEditor -e -viewSelected 0 $editorName;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tmodelPanel -edit -l (localizedPanelLabel(\"Persp View\")) -mbv $menusOkayInPanels  $panelName;\n\t\t$editorName = $panelName;\n        modelEditor -e \n            -camera \"camera1\" \n            -useInteractiveMode 0\n            -displayLights \"default\" \n            -displayAppearance \"wireframe\" \n            -activeOnly 0\n            -ignorePanZoom 0\n            -wireframeOnShaded 0\n            -headsUpDisplay 1\n            -selectionHiliteDisplay 1\n            -useDefaultMaterial 0\n"
		+ "            -bufferMode \"double\" \n            -twoSidedLighting 1\n            -backfaceCulling 0\n            -xray 0\n            -jointXray 0\n            -activeComponentsXray 0\n            -displayTextures 0\n            -smoothWireframe 0\n            -lineWidth 1\n            -textureAnisotropic 0\n            -textureHilight 1\n            -textureSampling 2\n            -textureDisplay \"modulate\" \n            -textureMaxSize 16384\n            -fogging 0\n            -fogSource \"fragment\" \n            -fogMode \"linear\" \n            -fogStart 0\n            -fogEnd 100\n            -fogDensity 0.1\n            -fogColor 0.5 0.5 0.5 1 \n            -maxConstantTransparency 1\n            -rendererName \"base_OpenGL_Renderer\" \n            -colorResolution 256 256 \n            -bumpResolution 512 512 \n            -textureCompression 0\n            -transparencyAlgorithm \"frontAndBackCull\" \n            -transpInShadows 0\n            -cullingOverride \"none\" \n            -lowQualityLighting 0\n            -maximumNumHardwareLights 1\n"
		+ "            -occlusionCulling 0\n            -shadingModel 0\n            -useBaseRenderer 0\n            -useReducedRenderer 0\n            -smallObjectCulling 0\n            -smallObjectThreshold -1 \n            -interactiveDisableShadows 0\n            -interactiveBackFaceCull 0\n            -sortTransparent 1\n            -nurbsCurves 1\n            -nurbsSurfaces 1\n            -polymeshes 1\n            -subdivSurfaces 1\n            -planes 1\n            -lights 1\n            -cameras 1\n            -controlVertices 1\n            -hulls 1\n            -grid 1\n            -joints 1\n            -ikHandles 1\n            -deformers 1\n            -dynamics 1\n            -fluids 1\n            -hairSystems 1\n            -follicles 1\n            -nCloths 1\n            -nParticles 1\n            -nRigids 1\n            -dynamicConstraints 1\n            -locators 1\n            -manipulators 1\n            -dimensions 1\n            -handles 1\n            -pivots 1\n            -textures 1\n            -strokes 1\n            -motionTrails 1\n"
		+ "            -shadows 0\n            $editorName;\nmodelEditor -e -viewSelected 0 $editorName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextPanel \"outlinerPanel\" (localizedPanelLabel(\"Outliner\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `outlinerPanel -unParent -l (localizedPanelLabel(\"Outliner\")) -mbv $menusOkayInPanels `;\n\t\t\t$editorName = $panelName;\n            outlinerEditor -e \n                -docTag \"isolOutln_fromSeln\" \n                -showShapes 1\n                -showAttributes 0\n                -showConnected 0\n                -showAnimCurvesOnly 0\n                -showMuteInfo 0\n                -organizeByLayer 1\n                -showAnimLayerWeight 1\n                -autoExpandLayers 1\n                -autoExpand 0\n                -showDagOnly 0\n                -showAssets 1\n                -showContainedOnly 1\n                -showPublishedAsConnected 0\n                -showContainerContents 1\n                -ignoreDagHierarchy 0\n"
		+ "                -expandConnections 0\n                -showUpstreamCurves 1\n                -showUnitlessCurves 1\n                -showCompounds 1\n                -showLeafs 1\n                -showNumericAttrsOnly 0\n                -highlightActive 1\n                -autoSelectNewObjects 0\n                -doNotSelectNewObjects 0\n                -dropIsParent 1\n                -transmitFilters 0\n                -setFilter \"defaultSetFilter\" \n                -showSetMembers 1\n                -allowMultiSelection 1\n                -alwaysToggleSelect 0\n                -directSelect 0\n                -displayMode \"DAG\" \n                -expandObjects 0\n                -setsIgnoreFilters 1\n                -containersIgnoreFilters 0\n                -editAttrName 0\n                -showAttrValues 0\n                -highlightSecondary 0\n                -showUVAttrsOnly 0\n                -showTextureNodesOnly 0\n                -attrAlphaOrder \"default\" \n                -animLayerFilterOptions \"allAffecting\" \n                -sortOrder \"none\" \n"
		+ "                -longNames 0\n                -niceNames 1\n                -showNamespace 1\n                -showPinIcons 0\n                -mapMotionTrails 0\n                $editorName;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\toutlinerPanel -edit -l (localizedPanelLabel(\"Outliner\")) -mbv $menusOkayInPanels  $panelName;\n\t\t$editorName = $panelName;\n        outlinerEditor -e \n            -docTag \"isolOutln_fromSeln\" \n            -showShapes 1\n            -showAttributes 0\n            -showConnected 0\n            -showAnimCurvesOnly 0\n            -showMuteInfo 0\n            -organizeByLayer 1\n            -showAnimLayerWeight 1\n            -autoExpandLayers 1\n            -autoExpand 0\n            -showDagOnly 0\n            -showAssets 1\n            -showContainedOnly 1\n            -showPublishedAsConnected 0\n            -showContainerContents 1\n            -ignoreDagHierarchy 0\n            -expandConnections 0\n            -showUpstreamCurves 1\n            -showUnitlessCurves 1\n            -showCompounds 1\n"
		+ "            -showLeafs 1\n            -showNumericAttrsOnly 0\n            -highlightActive 1\n            -autoSelectNewObjects 0\n            -doNotSelectNewObjects 0\n            -dropIsParent 1\n            -transmitFilters 0\n            -setFilter \"defaultSetFilter\" \n            -showSetMembers 1\n            -allowMultiSelection 1\n            -alwaysToggleSelect 0\n            -directSelect 0\n            -displayMode \"DAG\" \n            -expandObjects 0\n            -setsIgnoreFilters 1\n            -containersIgnoreFilters 0\n            -editAttrName 0\n            -showAttrValues 0\n            -highlightSecondary 0\n            -showUVAttrsOnly 0\n            -showTextureNodesOnly 0\n            -attrAlphaOrder \"default\" \n            -animLayerFilterOptions \"allAffecting\" \n            -sortOrder \"none\" \n            -longNames 0\n            -niceNames 1\n            -showNamespace 1\n            -showPinIcons 0\n            -mapMotionTrails 0\n            $editorName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n"
		+ "\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"graphEditor\" (localizedPanelLabel(\"Graph Editor\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `scriptedPanel -unParent  -type \"graphEditor\" -l (localizedPanelLabel(\"Graph Editor\")) -mbv $menusOkayInPanels `;\n\n\t\t\t$editorName = ($panelName+\"OutlineEd\");\n            outlinerEditor -e \n                -showShapes 1\n                -showAttributes 1\n                -showConnected 1\n                -showAnimCurvesOnly 1\n                -showMuteInfo 0\n                -organizeByLayer 1\n                -showAnimLayerWeight 1\n                -autoExpandLayers 1\n                -autoExpand 1\n                -showDagOnly 0\n                -showAssets 1\n                -showContainedOnly 0\n                -showPublishedAsConnected 0\n                -showContainerContents 0\n                -ignoreDagHierarchy 0\n                -expandConnections 1\n                -showUpstreamCurves 1\n                -showUnitlessCurves 1\n                -showCompounds 0\n"
		+ "                -showLeafs 1\n                -showNumericAttrsOnly 1\n                -highlightActive 0\n                -autoSelectNewObjects 1\n                -doNotSelectNewObjects 0\n                -dropIsParent 1\n                -transmitFilters 1\n                -setFilter \"0\" \n                -showSetMembers 0\n                -allowMultiSelection 1\n                -alwaysToggleSelect 0\n                -directSelect 0\n                -displayMode \"DAG\" \n                -expandObjects 0\n                -setsIgnoreFilters 1\n                -containersIgnoreFilters 0\n                -editAttrName 0\n                -showAttrValues 0\n                -highlightSecondary 0\n                -showUVAttrsOnly 0\n                -showTextureNodesOnly 0\n                -attrAlphaOrder \"default\" \n                -animLayerFilterOptions \"allAffecting\" \n                -sortOrder \"none\" \n                -longNames 0\n                -niceNames 1\n                -showNamespace 1\n                -showPinIcons 1\n                -mapMotionTrails 1\n"
		+ "                $editorName;\n\n\t\t\t$editorName = ($panelName+\"GraphEd\");\n            animCurveEditor -e \n                -displayKeys 1\n                -displayTangents 0\n                -displayActiveKeys 0\n                -displayActiveKeyTangents 1\n                -displayInfinities 0\n                -autoFit 0\n                -snapTime \"integer\" \n                -snapValue \"none\" \n                -showResults \"off\" \n                -showBufferCurves \"off\" \n                -smoothness \"fine\" \n                -resultSamples 1\n                -resultScreenSamples 0\n                -resultUpdate \"delayed\" \n                -showUpstreamCurves 1\n                -stackedCurves 0\n                -stackedCurvesMin -1\n                -stackedCurvesMax 1\n                -stackedCurvesSpace 0.2\n                -displayNormalized 0\n                -preSelectionHighlight 0\n                -constrainDrag 0\n                -classicMode 1\n                $editorName;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Graph Editor\")) -mbv $menusOkayInPanels  $panelName;\n"
		+ "\t\t\t$editorName = ($panelName+\"OutlineEd\");\n            outlinerEditor -e \n                -showShapes 1\n                -showAttributes 1\n                -showConnected 1\n                -showAnimCurvesOnly 1\n                -showMuteInfo 0\n                -organizeByLayer 1\n                -showAnimLayerWeight 1\n                -autoExpandLayers 1\n                -autoExpand 1\n                -showDagOnly 0\n                -showAssets 1\n                -showContainedOnly 0\n                -showPublishedAsConnected 0\n                -showContainerContents 0\n                -ignoreDagHierarchy 0\n                -expandConnections 1\n                -showUpstreamCurves 1\n                -showUnitlessCurves 1\n                -showCompounds 0\n                -showLeafs 1\n                -showNumericAttrsOnly 1\n                -highlightActive 0\n                -autoSelectNewObjects 1\n                -doNotSelectNewObjects 0\n                -dropIsParent 1\n                -transmitFilters 1\n                -setFilter \"0\" \n"
		+ "                -showSetMembers 0\n                -allowMultiSelection 1\n                -alwaysToggleSelect 0\n                -directSelect 0\n                -displayMode \"DAG\" \n                -expandObjects 0\n                -setsIgnoreFilters 1\n                -containersIgnoreFilters 0\n                -editAttrName 0\n                -showAttrValues 0\n                -highlightSecondary 0\n                -showUVAttrsOnly 0\n                -showTextureNodesOnly 0\n                -attrAlphaOrder \"default\" \n                -animLayerFilterOptions \"allAffecting\" \n                -sortOrder \"none\" \n                -longNames 0\n                -niceNames 1\n                -showNamespace 1\n                -showPinIcons 1\n                -mapMotionTrails 1\n                $editorName;\n\n\t\t\t$editorName = ($panelName+\"GraphEd\");\n            animCurveEditor -e \n                -displayKeys 1\n                -displayTangents 0\n                -displayActiveKeys 0\n                -displayActiveKeyTangents 1\n                -displayInfinities 0\n"
		+ "                -autoFit 0\n                -snapTime \"integer\" \n                -snapValue \"none\" \n                -showResults \"off\" \n                -showBufferCurves \"off\" \n                -smoothness \"fine\" \n                -resultSamples 1\n                -resultScreenSamples 0\n                -resultUpdate \"delayed\" \n                -showUpstreamCurves 1\n                -stackedCurves 0\n                -stackedCurvesMin -1\n                -stackedCurvesMax 1\n                -stackedCurvesSpace 0.2\n                -displayNormalized 0\n                -preSelectionHighlight 0\n                -constrainDrag 0\n                -classicMode 1\n                $editorName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"dopeSheetPanel\" (localizedPanelLabel(\"Dope Sheet\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `scriptedPanel -unParent  -type \"dopeSheetPanel\" -l (localizedPanelLabel(\"Dope Sheet\")) -mbv $menusOkayInPanels `;\n"
		+ "\t\t\t$editorName = ($panelName+\"OutlineEd\");\n            outlinerEditor -e \n                -showShapes 1\n                -showAttributes 1\n                -showConnected 1\n                -showAnimCurvesOnly 1\n                -showMuteInfo 0\n                -organizeByLayer 1\n                -showAnimLayerWeight 1\n                -autoExpandLayers 1\n                -autoExpand 0\n                -showDagOnly 0\n                -showAssets 1\n                -showContainedOnly 0\n                -showPublishedAsConnected 0\n                -showContainerContents 0\n                -ignoreDagHierarchy 0\n                -expandConnections 1\n                -showUpstreamCurves 1\n                -showUnitlessCurves 0\n                -showCompounds 1\n                -showLeafs 1\n                -showNumericAttrsOnly 1\n                -highlightActive 0\n                -autoSelectNewObjects 0\n                -doNotSelectNewObjects 1\n                -dropIsParent 1\n                -transmitFilters 0\n                -setFilter \"0\" \n"
		+ "                -showSetMembers 0\n                -allowMultiSelection 1\n                -alwaysToggleSelect 0\n                -directSelect 0\n                -displayMode \"DAG\" \n                -expandObjects 0\n                -setsIgnoreFilters 1\n                -containersIgnoreFilters 0\n                -editAttrName 0\n                -showAttrValues 0\n                -highlightSecondary 0\n                -showUVAttrsOnly 0\n                -showTextureNodesOnly 0\n                -attrAlphaOrder \"default\" \n                -animLayerFilterOptions \"allAffecting\" \n                -sortOrder \"none\" \n                -longNames 0\n                -niceNames 1\n                -showNamespace 1\n                -showPinIcons 0\n                -mapMotionTrails 1\n                $editorName;\n\n\t\t\t$editorName = ($panelName+\"DopeSheetEd\");\n            dopeSheetEditor -e \n                -displayKeys 1\n                -displayTangents 0\n                -displayActiveKeys 0\n                -displayActiveKeyTangents 0\n                -displayInfinities 0\n"
		+ "                -autoFit 0\n                -snapTime \"integer\" \n                -snapValue \"none\" \n                -outliner \"dopeSheetPanel1OutlineEd\" \n                -showSummary 1\n                -showScene 0\n                -hierarchyBelow 0\n                -showTicks 1\n                -selectionWindow 0 0 0 0 \n                $editorName;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Dope Sheet\")) -mbv $menusOkayInPanels  $panelName;\n\n\t\t\t$editorName = ($panelName+\"OutlineEd\");\n            outlinerEditor -e \n                -showShapes 1\n                -showAttributes 1\n                -showConnected 1\n                -showAnimCurvesOnly 1\n                -showMuteInfo 0\n                -organizeByLayer 1\n                -showAnimLayerWeight 1\n                -autoExpandLayers 1\n                -autoExpand 0\n                -showDagOnly 0\n                -showAssets 1\n                -showContainedOnly 0\n                -showPublishedAsConnected 0\n                -showContainerContents 0\n"
		+ "                -ignoreDagHierarchy 0\n                -expandConnections 1\n                -showUpstreamCurves 1\n                -showUnitlessCurves 0\n                -showCompounds 1\n                -showLeafs 1\n                -showNumericAttrsOnly 1\n                -highlightActive 0\n                -autoSelectNewObjects 0\n                -doNotSelectNewObjects 1\n                -dropIsParent 1\n                -transmitFilters 0\n                -setFilter \"0\" \n                -showSetMembers 0\n                -allowMultiSelection 1\n                -alwaysToggleSelect 0\n                -directSelect 0\n                -displayMode \"DAG\" \n                -expandObjects 0\n                -setsIgnoreFilters 1\n                -containersIgnoreFilters 0\n                -editAttrName 0\n                -showAttrValues 0\n                -highlightSecondary 0\n                -showUVAttrsOnly 0\n                -showTextureNodesOnly 0\n                -attrAlphaOrder \"default\" \n                -animLayerFilterOptions \"allAffecting\" \n"
		+ "                -sortOrder \"none\" \n                -longNames 0\n                -niceNames 1\n                -showNamespace 1\n                -showPinIcons 0\n                -mapMotionTrails 1\n                $editorName;\n\n\t\t\t$editorName = ($panelName+\"DopeSheetEd\");\n            dopeSheetEditor -e \n                -displayKeys 1\n                -displayTangents 0\n                -displayActiveKeys 0\n                -displayActiveKeyTangents 0\n                -displayInfinities 0\n                -autoFit 0\n                -snapTime \"integer\" \n                -snapValue \"none\" \n                -outliner \"dopeSheetPanel1OutlineEd\" \n                -showSummary 1\n                -showScene 0\n                -hierarchyBelow 0\n                -showTicks 1\n                -selectionWindow 0 0 0 0 \n                $editorName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"clipEditorPanel\" (localizedPanelLabel(\"Trax Editor\")) `;\n\tif (\"\" == $panelName) {\n"
		+ "\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `scriptedPanel -unParent  -type \"clipEditorPanel\" -l (localizedPanelLabel(\"Trax Editor\")) -mbv $menusOkayInPanels `;\n\n\t\t\t$editorName = clipEditorNameFromPanel($panelName);\n            clipEditor -e \n                -displayKeys 0\n                -displayTangents 0\n                -displayActiveKeys 0\n                -displayActiveKeyTangents 0\n                -displayInfinities 0\n                -autoFit 0\n                -snapTime \"none\" \n                -snapValue \"none\" \n                -manageSequencer 0 \n                $editorName;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Trax Editor\")) -mbv $menusOkayInPanels  $panelName;\n\n\t\t\t$editorName = clipEditorNameFromPanel($panelName);\n            clipEditor -e \n                -displayKeys 0\n                -displayTangents 0\n                -displayActiveKeys 0\n                -displayActiveKeyTangents 0\n                -displayInfinities 0\n                -autoFit 0\n"
		+ "                -snapTime \"none\" \n                -snapValue \"none\" \n                -manageSequencer 0 \n                $editorName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"sequenceEditorPanel\" (localizedPanelLabel(\"Camera Sequencer\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `scriptedPanel -unParent  -type \"sequenceEditorPanel\" -l (localizedPanelLabel(\"Camera Sequencer\")) -mbv $menusOkayInPanels `;\n\n\t\t\t$editorName = sequenceEditorNameFromPanel($panelName);\n            clipEditor -e \n                -displayKeys 0\n                -displayTangents 0\n                -displayActiveKeys 0\n                -displayActiveKeyTangents 0\n                -displayInfinities 0\n                -autoFit 0\n                -snapTime \"none\" \n                -snapValue \"none\" \n                -manageSequencer 1 \n                $editorName;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Camera Sequencer\")) -mbv $menusOkayInPanels  $panelName;\n"
		+ "\t\t\t$editorName = sequenceEditorNameFromPanel($panelName);\n            clipEditor -e \n                -displayKeys 0\n                -displayTangents 0\n                -displayActiveKeys 0\n                -displayActiveKeyTangents 0\n                -displayInfinities 0\n                -autoFit 0\n                -snapTime \"none\" \n                -snapValue \"none\" \n                -manageSequencer 1 \n                $editorName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"hyperGraphPanel\" (localizedPanelLabel(\"Hypergraph Hierarchy\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `scriptedPanel -unParent  -type \"hyperGraphPanel\" -l (localizedPanelLabel(\"Hypergraph Hierarchy\")) -mbv $menusOkayInPanels `;\n\n\t\t\t$editorName = ($panelName+\"HyperGraphEd\");\n            hyperGraph -e \n                -graphLayoutStyle \"hierarchicalLayout\" \n                -orientation \"horiz\" \n                -mergeConnections 0\n                -zoom 1\n"
		+ "                -animateTransition 0\n                -showRelationships 1\n                -showShapes 0\n                -showDeformers 0\n                -showExpressions 0\n                -showConstraints 0\n                -showUnderworld 0\n                -showInvisible 0\n                -transitionFrames 1\n                -opaqueContainers 0\n                -freeform 0\n                -imagePosition 0 0 \n                -imageScale 1\n                -imageEnabled 0\n                -graphType \"DAG\" \n                -heatMapDisplay 0\n                -updateSelection 1\n                -updateNodeAdded 1\n                -useDrawOverrideColor 0\n                -limitGraphTraversal -1\n                -range 0 0 \n                -iconSize \"smallIcons\" \n                -showCachedConnections 0\n                $editorName;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Hypergraph Hierarchy\")) -mbv $menusOkayInPanels  $panelName;\n\n\t\t\t$editorName = ($panelName+\"HyperGraphEd\");\n"
		+ "            hyperGraph -e \n                -graphLayoutStyle \"hierarchicalLayout\" \n                -orientation \"horiz\" \n                -mergeConnections 0\n                -zoom 1\n                -animateTransition 0\n                -showRelationships 1\n                -showShapes 0\n                -showDeformers 0\n                -showExpressions 0\n                -showConstraints 0\n                -showUnderworld 0\n                -showInvisible 0\n                -transitionFrames 1\n                -opaqueContainers 0\n                -freeform 0\n                -imagePosition 0 0 \n                -imageScale 1\n                -imageEnabled 0\n                -graphType \"DAG\" \n                -heatMapDisplay 0\n                -updateSelection 1\n                -updateNodeAdded 1\n                -useDrawOverrideColor 0\n                -limitGraphTraversal -1\n                -range 0 0 \n                -iconSize \"smallIcons\" \n                -showCachedConnections 0\n                $editorName;\n\t\tif (!$useSceneConfig) {\n"
		+ "\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"hyperShadePanel\" (localizedPanelLabel(\"Hypershade\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `scriptedPanel -unParent  -type \"hyperShadePanel\" -l (localizedPanelLabel(\"Hypershade\")) -mbv $menusOkayInPanels `;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Hypershade\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"visorPanel\" (localizedPanelLabel(\"Visor\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `scriptedPanel -unParent  -type \"visorPanel\" -l (localizedPanelLabel(\"Visor\")) -mbv $menusOkayInPanels `;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Visor\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n"
		+ "\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"createNodePanel\" (localizedPanelLabel(\"Create Node\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `scriptedPanel -unParent  -type \"createNodePanel\" -l (localizedPanelLabel(\"Create Node\")) -mbv $menusOkayInPanels `;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Create Node\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"polyTexturePlacementPanel\" (localizedPanelLabel(\"UV Texture Editor\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `scriptedPanel -unParent  -type \"polyTexturePlacementPanel\" -l (localizedPanelLabel(\"UV Texture Editor\")) -mbv $menusOkayInPanels `;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"UV Texture Editor\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n"
		+ "\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"renderWindowPanel\" (localizedPanelLabel(\"Render View\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `scriptedPanel -unParent  -type \"renderWindowPanel\" -l (localizedPanelLabel(\"Render View\")) -mbv $menusOkayInPanels `;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Render View\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextPanel \"blendShapePanel\" (localizedPanelLabel(\"Blend Shape\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\tblendShapePanel -unParent -l (localizedPanelLabel(\"Blend Shape\")) -mbv $menusOkayInPanels ;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tblendShapePanel -edit -l (localizedPanelLabel(\"Blend Shape\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n"
		+ "\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"dynRelEdPanel\" (localizedPanelLabel(\"Dynamic Relationships\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `scriptedPanel -unParent  -type \"dynRelEdPanel\" -l (localizedPanelLabel(\"Dynamic Relationships\")) -mbv $menusOkayInPanels `;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Dynamic Relationships\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"relationshipPanel\" (localizedPanelLabel(\"Relationship Editor\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `scriptedPanel -unParent  -type \"relationshipPanel\" -l (localizedPanelLabel(\"Relationship Editor\")) -mbv $menusOkayInPanels `;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Relationship Editor\")) -mbv $menusOkayInPanels  $panelName;\n"
		+ "\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"referenceEditorPanel\" (localizedPanelLabel(\"Reference Editor\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `scriptedPanel -unParent  -type \"referenceEditorPanel\" -l (localizedPanelLabel(\"Reference Editor\")) -mbv $menusOkayInPanels `;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Reference Editor\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"componentEditorPanel\" (localizedPanelLabel(\"Component Editor\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `scriptedPanel -unParent  -type \"componentEditorPanel\" -l (localizedPanelLabel(\"Component Editor\")) -mbv $menusOkayInPanels `;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Component Editor\")) -mbv $menusOkayInPanels  $panelName;\n"
		+ "\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"dynPaintScriptedPanelType\" (localizedPanelLabel(\"Paint Effects\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `scriptedPanel -unParent  -type \"dynPaintScriptedPanelType\" -l (localizedPanelLabel(\"Paint Effects\")) -mbv $menusOkayInPanels `;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Paint Effects\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"scriptEditorPanel\" (localizedPanelLabel(\"Script Editor\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `scriptedPanel -unParent  -type \"scriptEditorPanel\" -l (localizedPanelLabel(\"Script Editor\")) -mbv $menusOkayInPanels `;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Script Editor\")) -mbv $menusOkayInPanels  $panelName;\n"
		+ "\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"Stereo\" (localizedPanelLabel(\"Stereo\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `scriptedPanel -unParent  -type \"Stereo\" -l (localizedPanelLabel(\"Stereo\")) -mbv $menusOkayInPanels `;\nstring $editorName = ($panelName+\"Editor\");\n            stereoCameraView -e \n                -editorChanged \"updateModelPanelBar\" \n                -camera \"stereoCameraCenterCamShape\" \n                -useInteractiveMode 0\n                -displayLights \"default\" \n                -displayAppearance \"wireframe\" \n                -activeOnly 0\n                -ignorePanZoom 0\n                -wireframeOnShaded 0\n                -headsUpDisplay 1\n                -selectionHiliteDisplay 1\n                -useDefaultMaterial 0\n                -bufferMode \"double\" \n                -twoSidedLighting 1\n                -backfaceCulling 0\n                -xray 0\n                -jointXray 0\n"
		+ "                -activeComponentsXray 0\n                -displayTextures 0\n                -smoothWireframe 0\n                -lineWidth 1\n                -textureAnisotropic 0\n                -textureHilight 1\n                -textureSampling 2\n                -textureDisplay \"modulate\" \n                -textureMaxSize 16384\n                -fogging 0\n                -fogSource \"fragment\" \n                -fogMode \"linear\" \n                -fogStart 0\n                -fogEnd 100\n                -fogDensity 0.1\n                -fogColor 0.5 0.5 0.5 1 \n                -maxConstantTransparency 1\n                -rendererName \"base_OpenGL_Renderer\" \n                -colorResolution 256 256 \n                -bumpResolution 512 512 \n                -textureCompression 0\n                -transparencyAlgorithm \"frontAndBackCull\" \n                -transpInShadows 0\n                -cullingOverride \"none\" \n                -lowQualityLighting 0\n                -maximumNumHardwareLights 1\n                -occlusionCulling 0\n"
		+ "                -shadingModel 0\n                -useBaseRenderer 0\n                -useReducedRenderer 0\n                -smallObjectCulling 0\n                -smallObjectThreshold -1 \n                -interactiveDisableShadows 0\n                -interactiveBackFaceCull 0\n                -sortTransparent 1\n                -nurbsCurves 1\n                -nurbsSurfaces 1\n                -polymeshes 1\n                -subdivSurfaces 1\n                -planes 1\n                -lights 1\n                -cameras 1\n                -controlVertices 1\n                -hulls 1\n                -grid 1\n                -joints 1\n                -ikHandles 1\n                -deformers 1\n                -dynamics 1\n                -fluids 1\n                -hairSystems 1\n                -follicles 1\n                -nCloths 1\n                -nParticles 1\n                -nRigids 1\n                -dynamicConstraints 1\n                -locators 1\n                -manipulators 1\n                -dimensions 1\n                -handles 1\n"
		+ "                -pivots 1\n                -textures 1\n                -strokes 1\n                -motionTrails 1\n                -shadows 0\n                -displayMode \"anaglyphLum\" \n                -rigRoot \"|stereoCamera\" \n                -viewColor 0 0 0 1 \n                $editorName;\nstereoCameraView -e -viewSelected 0 $editorName;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Stereo\")) -mbv $menusOkayInPanels  $panelName;\nstring $editorName = ($panelName+\"Editor\");\n            stereoCameraView -e \n                -editorChanged \"updateModelPanelBar\" \n                -camera \"stereoCameraCenterCamShape\" \n                -useInteractiveMode 0\n                -displayLights \"default\" \n                -displayAppearance \"wireframe\" \n                -activeOnly 0\n                -ignorePanZoom 0\n                -wireframeOnShaded 0\n                -headsUpDisplay 1\n                -selectionHiliteDisplay 1\n                -useDefaultMaterial 0\n                -bufferMode \"double\" \n"
		+ "                -twoSidedLighting 1\n                -backfaceCulling 0\n                -xray 0\n                -jointXray 0\n                -activeComponentsXray 0\n                -displayTextures 0\n                -smoothWireframe 0\n                -lineWidth 1\n                -textureAnisotropic 0\n                -textureHilight 1\n                -textureSampling 2\n                -textureDisplay \"modulate\" \n                -textureMaxSize 16384\n                -fogging 0\n                -fogSource \"fragment\" \n                -fogMode \"linear\" \n                -fogStart 0\n                -fogEnd 100\n                -fogDensity 0.1\n                -fogColor 0.5 0.5 0.5 1 \n                -maxConstantTransparency 1\n                -rendererName \"base_OpenGL_Renderer\" \n                -colorResolution 256 256 \n                -bumpResolution 512 512 \n                -textureCompression 0\n                -transparencyAlgorithm \"frontAndBackCull\" \n                -transpInShadows 0\n                -cullingOverride \"none\" \n"
		+ "                -lowQualityLighting 0\n                -maximumNumHardwareLights 1\n                -occlusionCulling 0\n                -shadingModel 0\n                -useBaseRenderer 0\n                -useReducedRenderer 0\n                -smallObjectCulling 0\n                -smallObjectThreshold -1 \n                -interactiveDisableShadows 0\n                -interactiveBackFaceCull 0\n                -sortTransparent 1\n                -nurbsCurves 1\n                -nurbsSurfaces 1\n                -polymeshes 1\n                -subdivSurfaces 1\n                -planes 1\n                -lights 1\n                -cameras 1\n                -controlVertices 1\n                -hulls 1\n                -grid 1\n                -joints 1\n                -ikHandles 1\n                -deformers 1\n                -dynamics 1\n                -fluids 1\n                -hairSystems 1\n                -follicles 1\n                -nCloths 1\n                -nParticles 1\n                -nRigids 1\n                -dynamicConstraints 1\n"
		+ "                -locators 1\n                -manipulators 1\n                -dimensions 1\n                -handles 1\n                -pivots 1\n                -textures 1\n                -strokes 1\n                -motionTrails 1\n                -shadows 0\n                -displayMode \"anaglyphLum\" \n                -rigRoot \"|stereoCamera\" \n                -viewColor 0 0 0 1 \n                $editorName;\nstereoCameraView -e -viewSelected 0 $editorName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\tif ($useSceneConfig) {\n        string $configName = `getPanel -cwl (localizedPanelLabel(\"Current Layout\"))`;\n        if (\"\" != $configName) {\n\t\t\tpanelConfiguration -edit -label (localizedPanelLabel(\"Current Layout\")) \n\t\t\t\t-defaultImage \"\"\n\t\t\t\t-image \"\"\n\t\t\t\t-sc false\n\t\t\t\t-configString \"global string $gMainPane; paneLayout -e -cn \\\"quad\\\" -ps 1 50 50 -ps 2 50 50 -ps 3 50 50 -ps 4 50 50 $gMainPane;\"\n\t\t\t\t-removeAllPanels\n\t\t\t\t-ap false\n\t\t\t\t\t(localizedPanelLabel(\"Stereo\")) \n\t\t\t\t\t\"scriptedPanel\"\n"
		+ "\t\t\t\t\t\"$panelName = `scriptedPanel -unParent  -type \\\"Stereo\\\" -l (localizedPanelLabel(\\\"Stereo\\\")) -mbv $menusOkayInPanels `;\\nstring $editorName = ($panelName+\\\"Editor\\\");\\n            stereoCameraView -e \\n                -editorChanged \\\"updateModelPanelBar\\\" \\n                -camera \\\"stereoCameraCenterCamShape\\\" \\n                -useInteractiveMode 0\\n                -displayLights \\\"default\\\" \\n                -displayAppearance \\\"wireframe\\\" \\n                -activeOnly 0\\n                -ignorePanZoom 0\\n                -wireframeOnShaded 0\\n                -headsUpDisplay 1\\n                -selectionHiliteDisplay 1\\n                -useDefaultMaterial 0\\n                -bufferMode \\\"double\\\" \\n                -twoSidedLighting 1\\n                -backfaceCulling 0\\n                -xray 0\\n                -jointXray 0\\n                -activeComponentsXray 0\\n                -displayTextures 0\\n                -smoothWireframe 0\\n                -lineWidth 1\\n                -textureAnisotropic 0\\n                -textureHilight 1\\n                -textureSampling 2\\n                -textureDisplay \\\"modulate\\\" \\n                -textureMaxSize 16384\\n                -fogging 0\\n                -fogSource \\\"fragment\\\" \\n                -fogMode \\\"linear\\\" \\n                -fogStart 0\\n                -fogEnd 100\\n                -fogDensity 0.1\\n                -fogColor 0.5 0.5 0.5 1 \\n                -maxConstantTransparency 1\\n                -rendererName \\\"base_OpenGL_Renderer\\\" \\n                -colorResolution 256 256 \\n                -bumpResolution 512 512 \\n                -textureCompression 0\\n                -transparencyAlgorithm \\\"frontAndBackCull\\\" \\n                -transpInShadows 0\\n                -cullingOverride \\\"none\\\" \\n                -lowQualityLighting 0\\n                -maximumNumHardwareLights 1\\n                -occlusionCulling 0\\n                -shadingModel 0\\n                -useBaseRenderer 0\\n                -useReducedRenderer 0\\n                -smallObjectCulling 0\\n                -smallObjectThreshold -1 \\n                -interactiveDisableShadows 0\\n                -interactiveBackFaceCull 0\\n                -sortTransparent 1\\n                -nurbsCurves 1\\n                -nurbsSurfaces 1\\n                -polymeshes 1\\n                -subdivSurfaces 1\\n                -planes 1\\n                -lights 1\\n                -cameras 1\\n                -controlVertices 1\\n                -hulls 1\\n                -grid 1\\n                -joints 1\\n                -ikHandles 1\\n                -deformers 1\\n                -dynamics 1\\n                -fluids 1\\n                -hairSystems 1\\n                -follicles 1\\n                -nCloths 1\\n                -nParticles 1\\n                -nRigids 1\\n                -dynamicConstraints 1\\n                -locators 1\\n                -manipulators 1\\n                -dimensions 1\\n                -handles 1\\n                -pivots 1\\n                -textures 1\\n                -strokes 1\\n                -motionTrails 1\\n                -shadows 0\\n                -displayMode \\\"anaglyphLum\\\" \\n                -rigRoot \\\"|stereoCamera\\\" \\n                -viewColor 0 0 0 1 \\n                $editorName;\\nstereoCameraView -e -viewSelected 0 $editorName\"\n"
		+ "\t\t\t\t\t\"scriptedPanel -edit -l (localizedPanelLabel(\\\"Stereo\\\")) -mbv $menusOkayInPanels  $panelName;\\nstring $editorName = ($panelName+\\\"Editor\\\");\\n            stereoCameraView -e \\n                -editorChanged \\\"updateModelPanelBar\\\" \\n                -camera \\\"stereoCameraCenterCamShape\\\" \\n                -useInteractiveMode 0\\n                -displayLights \\\"default\\\" \\n                -displayAppearance \\\"wireframe\\\" \\n                -activeOnly 0\\n                -ignorePanZoom 0\\n                -wireframeOnShaded 0\\n                -headsUpDisplay 1\\n                -selectionHiliteDisplay 1\\n                -useDefaultMaterial 0\\n                -bufferMode \\\"double\\\" \\n                -twoSidedLighting 1\\n                -backfaceCulling 0\\n                -xray 0\\n                -jointXray 0\\n                -activeComponentsXray 0\\n                -displayTextures 0\\n                -smoothWireframe 0\\n                -lineWidth 1\\n                -textureAnisotropic 0\\n                -textureHilight 1\\n                -textureSampling 2\\n                -textureDisplay \\\"modulate\\\" \\n                -textureMaxSize 16384\\n                -fogging 0\\n                -fogSource \\\"fragment\\\" \\n                -fogMode \\\"linear\\\" \\n                -fogStart 0\\n                -fogEnd 100\\n                -fogDensity 0.1\\n                -fogColor 0.5 0.5 0.5 1 \\n                -maxConstantTransparency 1\\n                -rendererName \\\"base_OpenGL_Renderer\\\" \\n                -colorResolution 256 256 \\n                -bumpResolution 512 512 \\n                -textureCompression 0\\n                -transparencyAlgorithm \\\"frontAndBackCull\\\" \\n                -transpInShadows 0\\n                -cullingOverride \\\"none\\\" \\n                -lowQualityLighting 0\\n                -maximumNumHardwareLights 1\\n                -occlusionCulling 0\\n                -shadingModel 0\\n                -useBaseRenderer 0\\n                -useReducedRenderer 0\\n                -smallObjectCulling 0\\n                -smallObjectThreshold -1 \\n                -interactiveDisableShadows 0\\n                -interactiveBackFaceCull 0\\n                -sortTransparent 1\\n                -nurbsCurves 1\\n                -nurbsSurfaces 1\\n                -polymeshes 1\\n                -subdivSurfaces 1\\n                -planes 1\\n                -lights 1\\n                -cameras 1\\n                -controlVertices 1\\n                -hulls 1\\n                -grid 1\\n                -joints 1\\n                -ikHandles 1\\n                -deformers 1\\n                -dynamics 1\\n                -fluids 1\\n                -hairSystems 1\\n                -follicles 1\\n                -nCloths 1\\n                -nParticles 1\\n                -nRigids 1\\n                -dynamicConstraints 1\\n                -locators 1\\n                -manipulators 1\\n                -dimensions 1\\n                -handles 1\\n                -pivots 1\\n                -textures 1\\n                -strokes 1\\n                -motionTrails 1\\n                -shadows 0\\n                -displayMode \\\"anaglyphLum\\\" \\n                -rigRoot \\\"|stereoCamera\\\" \\n                -viewColor 0 0 0 1 \\n                $editorName;\\nstereoCameraView -e -viewSelected 0 $editorName\"\n"
		+ "\t\t\t\t-ap false\n\t\t\t\t\t(localizedPanelLabel(\"Side View\")) \n\t\t\t\t\t\"modelPanel\"\n"
		+ "\t\t\t\t\t\"$panelName = `modelPanel -unParent -l (localizedPanelLabel(\\\"Side View\\\")) -mbv $menusOkayInPanels `;\\n$editorName = $panelName;\\nmodelEditor -e \\n    -camera \\\"camera1\\\" \\n    -useInteractiveMode 0\\n    -displayLights \\\"default\\\" \\n    -displayAppearance \\\"smoothShaded\\\" \\n    -activeOnly 0\\n    -ignorePanZoom 0\\n    -wireframeOnShaded 0\\n    -headsUpDisplay 1\\n    -selectionHiliteDisplay 1\\n    -useDefaultMaterial 0\\n    -bufferMode \\\"double\\\" \\n    -twoSidedLighting 1\\n    -backfaceCulling 0\\n    -xray 0\\n    -jointXray 0\\n    -activeComponentsXray 0\\n    -displayTextures 0\\n    -smoothWireframe 0\\n    -lineWidth 1\\n    -textureAnisotropic 0\\n    -textureHilight 1\\n    -textureSampling 2\\n    -textureDisplay \\\"modulate\\\" \\n    -textureMaxSize 16384\\n    -fogging 0\\n    -fogSource \\\"fragment\\\" \\n    -fogMode \\\"linear\\\" \\n    -fogStart 0\\n    -fogEnd 100\\n    -fogDensity 0.1\\n    -fogColor 0.5 0.5 0.5 1 \\n    -maxConstantTransparency 1\\n    -rendererName \\\"hwRender_OpenGL_Renderer\\\" \\n    -colorResolution 256 256 \\n    -bumpResolution 512 512 \\n    -textureCompression 0\\n    -transparencyAlgorithm \\\"frontAndBackCull\\\" \\n    -transpInShadows 0\\n    -cullingOverride \\\"none\\\" \\n    -lowQualityLighting 0\\n    -maximumNumHardwareLights 1\\n    -occlusionCulling 0\\n    -shadingModel 0\\n    -useBaseRenderer 0\\n    -useReducedRenderer 0\\n    -smallObjectCulling 0\\n    -smallObjectThreshold -1 \\n    -interactiveDisableShadows 0\\n    -interactiveBackFaceCull 0\\n    -sortTransparent 1\\n    -nurbsCurves 1\\n    -nurbsSurfaces 1\\n    -polymeshes 1\\n    -subdivSurfaces 1\\n    -planes 1\\n    -lights 1\\n    -cameras 1\\n    -controlVertices 1\\n    -hulls 1\\n    -grid 1\\n    -joints 1\\n    -ikHandles 1\\n    -deformers 1\\n    -dynamics 1\\n    -fluids 1\\n    -hairSystems 1\\n    -follicles 1\\n    -nCloths 1\\n    -nParticles 1\\n    -nRigids 1\\n    -dynamicConstraints 1\\n    -locators 1\\n    -manipulators 1\\n    -dimensions 1\\n    -handles 1\\n    -pivots 1\\n    -textures 1\\n    -strokes 1\\n    -motionTrails 1\\n    -shadows 0\\n    $editorName;\\nmodelEditor -e -viewSelected 0 $editorName\"\n"
		+ "\t\t\t\t\t\"modelPanel -edit -l (localizedPanelLabel(\\\"Side View\\\")) -mbv $menusOkayInPanels  $panelName;\\n$editorName = $panelName;\\nmodelEditor -e \\n    -camera \\\"camera1\\\" \\n    -useInteractiveMode 0\\n    -displayLights \\\"default\\\" \\n    -displayAppearance \\\"smoothShaded\\\" \\n    -activeOnly 0\\n    -ignorePanZoom 0\\n    -wireframeOnShaded 0\\n    -headsUpDisplay 1\\n    -selectionHiliteDisplay 1\\n    -useDefaultMaterial 0\\n    -bufferMode \\\"double\\\" \\n    -twoSidedLighting 1\\n    -backfaceCulling 0\\n    -xray 0\\n    -jointXray 0\\n    -activeComponentsXray 0\\n    -displayTextures 0\\n    -smoothWireframe 0\\n    -lineWidth 1\\n    -textureAnisotropic 0\\n    -textureHilight 1\\n    -textureSampling 2\\n    -textureDisplay \\\"modulate\\\" \\n    -textureMaxSize 16384\\n    -fogging 0\\n    -fogSource \\\"fragment\\\" \\n    -fogMode \\\"linear\\\" \\n    -fogStart 0\\n    -fogEnd 100\\n    -fogDensity 0.1\\n    -fogColor 0.5 0.5 0.5 1 \\n    -maxConstantTransparency 1\\n    -rendererName \\\"hwRender_OpenGL_Renderer\\\" \\n    -colorResolution 256 256 \\n    -bumpResolution 512 512 \\n    -textureCompression 0\\n    -transparencyAlgorithm \\\"frontAndBackCull\\\" \\n    -transpInShadows 0\\n    -cullingOverride \\\"none\\\" \\n    -lowQualityLighting 0\\n    -maximumNumHardwareLights 1\\n    -occlusionCulling 0\\n    -shadingModel 0\\n    -useBaseRenderer 0\\n    -useReducedRenderer 0\\n    -smallObjectCulling 0\\n    -smallObjectThreshold -1 \\n    -interactiveDisableShadows 0\\n    -interactiveBackFaceCull 0\\n    -sortTransparent 1\\n    -nurbsCurves 1\\n    -nurbsSurfaces 1\\n    -polymeshes 1\\n    -subdivSurfaces 1\\n    -planes 1\\n    -lights 1\\n    -cameras 1\\n    -controlVertices 1\\n    -hulls 1\\n    -grid 1\\n    -joints 1\\n    -ikHandles 1\\n    -deformers 1\\n    -dynamics 1\\n    -fluids 1\\n    -hairSystems 1\\n    -follicles 1\\n    -nCloths 1\\n    -nParticles 1\\n    -nRigids 1\\n    -dynamicConstraints 1\\n    -locators 1\\n    -manipulators 1\\n    -dimensions 1\\n    -handles 1\\n    -pivots 1\\n    -textures 1\\n    -strokes 1\\n    -motionTrails 1\\n    -shadows 0\\n    $editorName;\\nmodelEditor -e -viewSelected 0 $editorName\"\n"
		+ "\t\t\t\t-ap false\n\t\t\t\t\t(localizedPanelLabel(\"Front View\")) \n\t\t\t\t\t\"modelPanel\"\n"
		+ "\t\t\t\t\t\"$panelName = `modelPanel -unParent -l (localizedPanelLabel(\\\"Front View\\\")) -mbv $menusOkayInPanels `;\\n$editorName = $panelName;\\nmodelEditor -e \\n    -cam `findStartUpCamera front` \\n    -useInteractiveMode 0\\n    -displayLights \\\"default\\\" \\n    -displayAppearance \\\"wireframe\\\" \\n    -activeOnly 0\\n    -ignorePanZoom 0\\n    -wireframeOnShaded 0\\n    -headsUpDisplay 1\\n    -selectionHiliteDisplay 1\\n    -useDefaultMaterial 0\\n    -bufferMode \\\"double\\\" \\n    -twoSidedLighting 1\\n    -backfaceCulling 0\\n    -xray 0\\n    -jointXray 0\\n    -activeComponentsXray 0\\n    -displayTextures 0\\n    -smoothWireframe 0\\n    -lineWidth 1\\n    -textureAnisotropic 0\\n    -textureHilight 1\\n    -textureSampling 2\\n    -textureDisplay \\\"modulate\\\" \\n    -textureMaxSize 16384\\n    -fogging 0\\n    -fogSource \\\"fragment\\\" \\n    -fogMode \\\"linear\\\" \\n    -fogStart 0\\n    -fogEnd 100\\n    -fogDensity 0.1\\n    -fogColor 0.5 0.5 0.5 1 \\n    -maxConstantTransparency 1\\n    -rendererName \\\"base_OpenGL_Renderer\\\" \\n    -colorResolution 256 256 \\n    -bumpResolution 512 512 \\n    -textureCompression 0\\n    -transparencyAlgorithm \\\"frontAndBackCull\\\" \\n    -transpInShadows 0\\n    -cullingOverride \\\"none\\\" \\n    -lowQualityLighting 0\\n    -maximumNumHardwareLights 1\\n    -occlusionCulling 0\\n    -shadingModel 0\\n    -useBaseRenderer 0\\n    -useReducedRenderer 0\\n    -smallObjectCulling 0\\n    -smallObjectThreshold -1 \\n    -interactiveDisableShadows 0\\n    -interactiveBackFaceCull 0\\n    -sortTransparent 1\\n    -nurbsCurves 1\\n    -nurbsSurfaces 1\\n    -polymeshes 1\\n    -subdivSurfaces 1\\n    -planes 1\\n    -lights 1\\n    -cameras 1\\n    -controlVertices 1\\n    -hulls 1\\n    -grid 1\\n    -joints 1\\n    -ikHandles 1\\n    -deformers 1\\n    -dynamics 1\\n    -fluids 1\\n    -hairSystems 1\\n    -follicles 1\\n    -nCloths 1\\n    -nParticles 1\\n    -nRigids 1\\n    -dynamicConstraints 1\\n    -locators 1\\n    -manipulators 1\\n    -dimensions 1\\n    -handles 1\\n    -pivots 1\\n    -textures 1\\n    -strokes 1\\n    -motionTrails 1\\n    -shadows 0\\n    $editorName;\\nmodelEditor -e -viewSelected 0 $editorName\"\n"
		+ "\t\t\t\t\t\"modelPanel -edit -l (localizedPanelLabel(\\\"Front View\\\")) -mbv $menusOkayInPanels  $panelName;\\n$editorName = $panelName;\\nmodelEditor -e \\n    -cam `findStartUpCamera front` \\n    -useInteractiveMode 0\\n    -displayLights \\\"default\\\" \\n    -displayAppearance \\\"wireframe\\\" \\n    -activeOnly 0\\n    -ignorePanZoom 0\\n    -wireframeOnShaded 0\\n    -headsUpDisplay 1\\n    -selectionHiliteDisplay 1\\n    -useDefaultMaterial 0\\n    -bufferMode \\\"double\\\" \\n    -twoSidedLighting 1\\n    -backfaceCulling 0\\n    -xray 0\\n    -jointXray 0\\n    -activeComponentsXray 0\\n    -displayTextures 0\\n    -smoothWireframe 0\\n    -lineWidth 1\\n    -textureAnisotropic 0\\n    -textureHilight 1\\n    -textureSampling 2\\n    -textureDisplay \\\"modulate\\\" \\n    -textureMaxSize 16384\\n    -fogging 0\\n    -fogSource \\\"fragment\\\" \\n    -fogMode \\\"linear\\\" \\n    -fogStart 0\\n    -fogEnd 100\\n    -fogDensity 0.1\\n    -fogColor 0.5 0.5 0.5 1 \\n    -maxConstantTransparency 1\\n    -rendererName \\\"base_OpenGL_Renderer\\\" \\n    -colorResolution 256 256 \\n    -bumpResolution 512 512 \\n    -textureCompression 0\\n    -transparencyAlgorithm \\\"frontAndBackCull\\\" \\n    -transpInShadows 0\\n    -cullingOverride \\\"none\\\" \\n    -lowQualityLighting 0\\n    -maximumNumHardwareLights 1\\n    -occlusionCulling 0\\n    -shadingModel 0\\n    -useBaseRenderer 0\\n    -useReducedRenderer 0\\n    -smallObjectCulling 0\\n    -smallObjectThreshold -1 \\n    -interactiveDisableShadows 0\\n    -interactiveBackFaceCull 0\\n    -sortTransparent 1\\n    -nurbsCurves 1\\n    -nurbsSurfaces 1\\n    -polymeshes 1\\n    -subdivSurfaces 1\\n    -planes 1\\n    -lights 1\\n    -cameras 1\\n    -controlVertices 1\\n    -hulls 1\\n    -grid 1\\n    -joints 1\\n    -ikHandles 1\\n    -deformers 1\\n    -dynamics 1\\n    -fluids 1\\n    -hairSystems 1\\n    -follicles 1\\n    -nCloths 1\\n    -nParticles 1\\n    -nRigids 1\\n    -dynamicConstraints 1\\n    -locators 1\\n    -manipulators 1\\n    -dimensions 1\\n    -handles 1\\n    -pivots 1\\n    -textures 1\\n    -strokes 1\\n    -motionTrails 1\\n    -shadows 0\\n    $editorName;\\nmodelEditor -e -viewSelected 0 $editorName\"\n"
		+ "\t\t\t\t-ap false\n\t\t\t\t\t(localizedPanelLabel(\"Top View\")) \n\t\t\t\t\t\"modelPanel\"\n"
		+ "\t\t\t\t\t\"$panelName = `modelPanel -unParent -l (localizedPanelLabel(\\\"Top View\\\")) -mbv $menusOkayInPanels `;\\n$editorName = $panelName;\\nmodelEditor -e \\n    -camera \\\"stereoCameraCenterCamShape\\\" \\n    -useInteractiveMode 0\\n    -displayLights \\\"default\\\" \\n    -displayAppearance \\\"wireframe\\\" \\n    -activeOnly 0\\n    -ignorePanZoom 0\\n    -wireframeOnShaded 0\\n    -headsUpDisplay 1\\n    -selectionHiliteDisplay 1\\n    -useDefaultMaterial 0\\n    -bufferMode \\\"double\\\" \\n    -twoSidedLighting 1\\n    -backfaceCulling 0\\n    -xray 0\\n    -jointXray 0\\n    -activeComponentsXray 0\\n    -displayTextures 0\\n    -smoothWireframe 0\\n    -lineWidth 1\\n    -textureAnisotropic 0\\n    -textureHilight 1\\n    -textureSampling 2\\n    -textureDisplay \\\"modulate\\\" \\n    -textureMaxSize 16384\\n    -fogging 0\\n    -fogSource \\\"fragment\\\" \\n    -fogMode \\\"linear\\\" \\n    -fogStart 0\\n    -fogEnd 100\\n    -fogDensity 0.1\\n    -fogColor 0.5 0.5 0.5 1 \\n    -maxConstantTransparency 1\\n    -rendererName \\\"base_OpenGL_Renderer\\\" \\n    -colorResolution 256 256 \\n    -bumpResolution 512 512 \\n    -textureCompression 0\\n    -transparencyAlgorithm \\\"frontAndBackCull\\\" \\n    -transpInShadows 0\\n    -cullingOverride \\\"none\\\" \\n    -lowQualityLighting 0\\n    -maximumNumHardwareLights 1\\n    -occlusionCulling 0\\n    -shadingModel 0\\n    -useBaseRenderer 0\\n    -useReducedRenderer 0\\n    -smallObjectCulling 0\\n    -smallObjectThreshold -1 \\n    -interactiveDisableShadows 0\\n    -interactiveBackFaceCull 0\\n    -sortTransparent 1\\n    -nurbsCurves 1\\n    -nurbsSurfaces 1\\n    -polymeshes 1\\n    -subdivSurfaces 1\\n    -planes 1\\n    -lights 1\\n    -cameras 1\\n    -controlVertices 1\\n    -hulls 1\\n    -grid 1\\n    -joints 1\\n    -ikHandles 1\\n    -deformers 1\\n    -dynamics 1\\n    -fluids 1\\n    -hairSystems 1\\n    -follicles 1\\n    -nCloths 1\\n    -nParticles 1\\n    -nRigids 1\\n    -dynamicConstraints 1\\n    -locators 1\\n    -manipulators 1\\n    -dimensions 1\\n    -handles 1\\n    -pivots 1\\n    -textures 1\\n    -strokes 1\\n    -motionTrails 1\\n    -shadows 0\\n    $editorName;\\nmodelEditor -e -viewSelected 0 $editorName\"\n"
		+ "\t\t\t\t\t\"modelPanel -edit -l (localizedPanelLabel(\\\"Top View\\\")) -mbv $menusOkayInPanels  $panelName;\\n$editorName = $panelName;\\nmodelEditor -e \\n    -camera \\\"stereoCameraCenterCamShape\\\" \\n    -useInteractiveMode 0\\n    -displayLights \\\"default\\\" \\n    -displayAppearance \\\"wireframe\\\" \\n    -activeOnly 0\\n    -ignorePanZoom 0\\n    -wireframeOnShaded 0\\n    -headsUpDisplay 1\\n    -selectionHiliteDisplay 1\\n    -useDefaultMaterial 0\\n    -bufferMode \\\"double\\\" \\n    -twoSidedLighting 1\\n    -backfaceCulling 0\\n    -xray 0\\n    -jointXray 0\\n    -activeComponentsXray 0\\n    -displayTextures 0\\n    -smoothWireframe 0\\n    -lineWidth 1\\n    -textureAnisotropic 0\\n    -textureHilight 1\\n    -textureSampling 2\\n    -textureDisplay \\\"modulate\\\" \\n    -textureMaxSize 16384\\n    -fogging 0\\n    -fogSource \\\"fragment\\\" \\n    -fogMode \\\"linear\\\" \\n    -fogStart 0\\n    -fogEnd 100\\n    -fogDensity 0.1\\n    -fogColor 0.5 0.5 0.5 1 \\n    -maxConstantTransparency 1\\n    -rendererName \\\"base_OpenGL_Renderer\\\" \\n    -colorResolution 256 256 \\n    -bumpResolution 512 512 \\n    -textureCompression 0\\n    -transparencyAlgorithm \\\"frontAndBackCull\\\" \\n    -transpInShadows 0\\n    -cullingOverride \\\"none\\\" \\n    -lowQualityLighting 0\\n    -maximumNumHardwareLights 1\\n    -occlusionCulling 0\\n    -shadingModel 0\\n    -useBaseRenderer 0\\n    -useReducedRenderer 0\\n    -smallObjectCulling 0\\n    -smallObjectThreshold -1 \\n    -interactiveDisableShadows 0\\n    -interactiveBackFaceCull 0\\n    -sortTransparent 1\\n    -nurbsCurves 1\\n    -nurbsSurfaces 1\\n    -polymeshes 1\\n    -subdivSurfaces 1\\n    -planes 1\\n    -lights 1\\n    -cameras 1\\n    -controlVertices 1\\n    -hulls 1\\n    -grid 1\\n    -joints 1\\n    -ikHandles 1\\n    -deformers 1\\n    -dynamics 1\\n    -fluids 1\\n    -hairSystems 1\\n    -follicles 1\\n    -nCloths 1\\n    -nParticles 1\\n    -nRigids 1\\n    -dynamicConstraints 1\\n    -locators 1\\n    -manipulators 1\\n    -dimensions 1\\n    -handles 1\\n    -pivots 1\\n    -textures 1\\n    -strokes 1\\n    -motionTrails 1\\n    -shadows 0\\n    $editorName;\\nmodelEditor -e -viewSelected 0 $editorName\"\n"
		+ "\t\t\t\t$configName;\n\n            setNamedPanelLayout (localizedPanelLabel(\"Current Layout\"));\n        }\n\n        panelHistory -e -clear mainPanelHistory;\n        setFocus `paneLayout -q -p1 $gMainPane`;\n        sceneUIReplacement -deleteRemaining;\n        sceneUIReplacement -clear;\n\t}\n\n\ngrid -spacing 5 -size 12 -divisions 5 -displayAxes yes -displayGridLines yes -displayDivisionLines yes -displayPerspectiveLabels no -displayOrthographicLabels no -displayAxesBold yes -perspectiveLabelPosition axis -orthographicLabelPosition edge;\nviewManip -drawCompass 0 -compassAngle 0 -frontParameters \"\" -homeParameters \"\" -selectionLockParameters \"\";\n}\n");
	setAttr ".st" 3;
createNode script -n "sceneConfigurationScriptNode";
	setAttr ".b" -type "string" "playbackOptions -min 1 -max 24 -ast 1 -aet 48 ";
	setAttr ".st" 6;
createNode script -n "liquidCleanUpNode";
	setAttr ".b" -type "string" "if ( `whatIs liquid` == \"Command\" ) liquidCreateScriptNode;";
	setAttr ".a" -type "string" "if ( `whatIs liquid` == \"Command\" ) liquidCleanUp;";
	setAttr ".st" 2;
createNode liquidGlobals -n "liquidGlobals";
	addAttr -ci true -sn "displayList" -ln "displayList" -dt "stringArray";
	setAttr ".rc" -type "string" "stereoCameraCenterCamShape";
	setAttr ".ddin[0]" -type "string" "$SCN.$CAM.$F.$RND.tif";
	setAttr ".ddim[0]" -type "string" "rgb";
	setAttr ".ddit[0]" -type "string" "bmp";
	setAttr ".ddbd[0]"  8;
	setAttr ".shdd" -type "string" "generatedShader";
	setAttr ".unt" yes;
	setAttr ".osg" yes;
	setAttr ".lzt" -type "float3" 1 1 1 ;
	setAttr ".rdc" -type "string" "renderdl";
	setAttr ".prv" -type "string" "renderdl";
	setAttr ".lrs" -type "string" "R:/MyDocuments/maya/projects/default/rmantmp/stereo_camera31461.xml";
	setAttr ".shi" -type "string" "shaderinfo";
	setAttr ".shcp" -type "string" "shaderdl";
	setAttr ".she" -type "string" "sdl";
	setAttr ".mtx" -type "string" "tdlmake";
	setAttr ".vtx" -type "string" "i-display";
	setAttr ".txe" -type "string" "tdl";
	setAttr ".dsdn" -type "string" "deepshad";
	setAttr ".dsim" -type "string" "deepopacity";
	setAttr ".Photon" yes;
	setAttr ".ZBuffer" no;
	setAttr ".DepthMask" yes;
	setAttr ".Blackman_Harris" yes;
	setAttr ".Mitchell" yes;
	setAttr ".SeparableCatmull_Rom" yes;
	setAttr ".Blobbies" yes;
	setAttr ".Points" yes;
	setAttr ".Raytracing" yes;
	setAttr ".AdvancedVisibility" yes;
	setAttr ".rnd" -type "string" "renderman";
	setAttr ".displayList" -type "stringArray" 18 "bmp" "cineon" "dsm" "eps" "exr" "framebuffer" "idisplay" "iff" "jpeg" "liqmaya-" "liqmaya" "memory" "pic" "png" "psd" "radiance" "tiff" "zfile"  ;
createNode polyCube -n "polyCube2";
	setAttr ".w" 3.8971424025785835;
	setAttr ".h" 8.9615037481710456;
	setAttr ".d" 4.9894411418467683;
	setAttr ".cuv" 4;
createNode lambert -n "lambert3";
	setAttr ".c" -type "float3" 0.5 0.24725001 0 ;
createNode shadingEngine -n "lambert3SG";
	setAttr ".ihi" 0;
	setAttr ".ro" yes;
createNode materialInfo -n "materialInfo1";
createNode polyCube -n "polyCube3";
	setAttr ".w" 5.9874780199633166;
	setAttr ".h" 10.507302635852254;
	setAttr ".d" 6.3548219716341805;
	setAttr ".cuv" 4;
createNode lambert -n "lambert4";
	setAttr ".c" -type "float3" 0 0.5 0.015750051 ;
createNode shadingEngine -n "lambert4SG";
	setAttr ".ihi" 0;
	setAttr ".ro" yes;
createNode materialInfo -n "materialInfo2";
createNode liqGlobalsNodeRenderer -n "liqGlobalsNodeRenderer_elvishray";
	addAttr -ci true -h true -sn "verbose" -ln "verbose" -dv 6 -at "long";
	addAttr -ci true -h true -sn "link" -ln "link" -dt "string";
	addAttr -ci true -h true -sn "contrast" -ln "contrast" -dt "string";
	addAttr -ci true -h true -sn "samples" -ln "samples" -dt "string";
	addAttr -ci true -h true -sn "filterType" -ln "filterType" -dv 4 -at "long";
	addAttr -ci true -h true -sn "filterSize" -ln "filterSize" -dv 3 -at "float";
	addAttr -ci true -h true -sn "trace_depth" -ln "trace_depth" -dt "string";
	addAttr -ci true -h true -sn "displace" -ln "displace" -min 0 -max 1 -at "bool";
	addAttr -ci true -h true -sn "max_displace" -ln "max_displace" -at "float";
	addAttr -ci true -h true -sn "face" -ln "face" -dv 3 -at "long";
	addAttr -ci true -h true -sn "approx_method" -ln "approx_method" -dv 1 -at "long";
	addAttr -ci true -h true -sn "approx_any" -ln "approx_any" -at "long";
	addAttr -ci true -h true -sn "approx_view_dep" -ln "approx_view_dep" -at "long";
	addAttr -ci true -h true -sn "approx_args" -ln "approx_args" -dt "string";
	addAttr -ci true -h true -sn "approx_sharp" -ln "approx_sharp" -at "float";
	addAttr -ci true -h true -sn "approx_min_subdiv" -ln "approx_min_subdiv" -at "long";
	addAttr -ci true -h true -sn "approx_max_subdiv" -ln "approx_max_subdiv" -dv 5 -at "long";
	addAttr -ci true -h true -sn "approx_max_grid_size" -ln "approx_max_grid_size" -dv 
		4096 -at "long";
	addAttr -ci true -h true -sn "approx_motion_factor" -ln "approx_motion_factor" -dv 
		16 -at "float";
	setAttr ".link" -type "string" "eiIMG|eiSHADER|eiSHADER_maya";
	setAttr ".contrast" -type "string" "0.01|0.01|0.01|0.01";
	setAttr ".samples" -type "string" "0|16";
	setAttr ".trace_depth" -type "string" "4|4|4";
	setAttr ".approx_args" -type "string" "0|0|0|0";
createNode liqGlobalsNodeRenderer -n "liqGlobalsNodeRenderer_renderman";
	addAttr -ci true -h true -sn "testBool0" -ln "testBool0" -min 0 -max 1 -at "bool";
	addAttr -ci true -h true -sn "testBool1" -ln "testBool1" -min 0 -max 1 -at "bool";
	addAttr -ci true -h true -sn "testString0" -ln "testString0" -dt "string";
	addAttr -ci true -h true -sn "testString1" -ln "testString1" -dt "string";
	addAttr -ci true -uac -h true -sn "testColor0" -ln "testColor0" -at "float3" -nc 
		3;
	addAttr -ci true -sn "testColor0R" -ln "testColor0R" -at "float" -p "testColor0";
	addAttr -ci true -sn "testColor0G" -ln "testColor0G" -at "float" -p "testColor0";
	addAttr -ci true -sn "testColor0B" -ln "testColor0B" -at "float" -p "testColor0";
	addAttr -ci true -uac -h true -sn "testColor1" -ln "testColor1" -at "float3" -nc 
		3;
	addAttr -ci true -sn "testColor1R" -ln "testColor1R" -at "float" -p "testColor1";
	addAttr -ci true -sn "testColor1G" -ln "testColor1G" -at "float" -p "testColor1";
	addAttr -ci true -sn "testColor1B" -ln "testColor1B" -at "float" -p "testColor1";
	addAttr -ci true -h true -sn "testInt0" -ln "testInt0" -at "long";
	addAttr -ci true -h true -sn "testInt1" -ln "testInt1" -dv 10 -at "long";
	addAttr -ci true -h true -sn "testFloat0" -ln "testFloat0" -at "float";
	addAttr -ci true -h true -sn "testFloat1" -ln "testFloat1" -dv 10 -at "float";
	addAttr -ci true -h true -sn "testIntMenu0" -ln "testIntMenu0" -at "long";
	addAttr -ci true -h true -sn "testIntMenu1" -ln "testIntMenu1" -dv 1 -at "long";
	setAttr ".testBool0" yes;
	setAttr ".testString0" -type "string" "string_test_value0renderman";
	setAttr ".testString1" -type "string" "string_test_value1renderman";
	setAttr ".testColor0" -type "float3" 1 1 0 ;
	setAttr ".testColor1" -type "float3" 1.9 9 2 ;
createNode rendermanGlobals -n "rendermanGlobals1";
select -ne :time1;
	setAttr -k on ".cch";
	setAttr -cb on ".ihi";
	setAttr -k on ".nds";
	setAttr -cb on ".bnm";
	setAttr ".o" 1;
	setAttr ".unw" 1;
select -ne :renderPartition;
	setAttr -k on ".cch";
	setAttr -cb on ".ihi";
	setAttr -k on ".nds";
	setAttr -cb on ".bnm";
	setAttr -s 4 ".st";
	setAttr -cb on ".an";
	setAttr -cb on ".pt";
select -ne :initialShadingGroup;
	setAttr -k on ".cch";
	setAttr -cb on ".ihi";
	setAttr -av -k on ".nds";
	setAttr -cb on ".bnm";
	setAttr -k on ".mwc";
	setAttr -cb on ".an";
	setAttr -cb on ".il";
	setAttr -cb on ".vo";
	setAttr -cb on ".eo";
	setAttr -cb on ".fo";
	setAttr -cb on ".epo";
	setAttr ".ro" yes;
select -ne :initialParticleSE;
	setAttr -k on ".cch";
	setAttr -cb on ".ihi";
	setAttr -k on ".nds";
	setAttr -cb on ".bnm";
	setAttr -k on ".mwc";
	setAttr -cb on ".an";
	setAttr -cb on ".il";
	setAttr -cb on ".vo";
	setAttr -cb on ".eo";
	setAttr -cb on ".fo";
	setAttr -cb on ".epo";
	setAttr ".ro" yes;
select -ne :defaultShaderList1;
	setAttr -k on ".cch";
	setAttr -cb on ".ihi";
	setAttr -k on ".nds";
	setAttr -cb on ".bnm";
	setAttr -s 4 ".s";
select -ne :lightList1;
	setAttr -k on ".cch";
	setAttr -cb on ".ihi";
	setAttr -k on ".nds";
	setAttr -cb on ".bnm";
	setAttr -s 2 ".l";
select -ne :lambert1;
	addAttr -ci true -sn "elvishrayShader" -ln "elvishrayShader" -dt "string";
	addAttr -ci true -sn "elvishrayUseOverrideShader" -ln "elvishrayUseOverrideShader" 
		-min 0 -max 1 -at "bool";
	addAttr -ci true -sn "elvishrayReflectionAngle" -ln "elvishrayReflectionAngle" -min 
		0 -max 180 -smx 10 -at "float";
	addAttr -ci true -sn "elvishrayReflectionSamples" -ln "elvishrayReflectionSamples" 
		-dv 1 -min 1 -max 1024 -smx 16 -at "long";
	addAttr -ci true -sn "elvishrayRefractionAngle" -ln "elvishrayRefractionAngle" -min 
		0 -max 180 -smx 10 -at "float";
	addAttr -ci true -sn "elvishrayRefractionSamples" -ln "elvishrayRefractionSamples" 
		-dv 1 -min 1 -max 1024 -smx 16 -at "long";
	addAttr -ci true -uac -sn "elvishrayBacklighting" -ln "elvishrayBacklighting" -at "float3" 
		-nc 3;
	addAttr -ci true -sn "elvishrayBacklightingR" -ln "elvishrayBacklightingR" -at "float" 
		-p "elvishrayBacklighting";
	addAttr -ci true -sn "elvishrayBacklightingG" -ln "elvishrayBacklightingG" -at "float" 
		-p "elvishrayBacklighting";
	addAttr -ci true -sn "elvishrayBacklightingB" -ln "elvishrayBacklightingB" -at "float" 
		-p "elvishrayBacklighting";
	addAttr -ci true -sn "elvishraySubsurfaceScattering" -ln "elvishraySubsurfaceScattering" 
		-min 0 -max 1 -at "bool";
	addAttr -ci true -sn "elvishraySubsurfaceIntensity" -ln "elvishraySubsurfaceIntensity" 
		-dv 1 -min 0 -max 1000 -smx 1 -at "float";
	addAttr -ci true -sn "elvishraySubsurfaceDiffuseFile" -ln "elvishraySubsurfaceDiffuseFile" 
		-dt "string";
	addAttr -ci true -uac -sn "elvishraySubsurfaceAlbedo" -ln "elvishraySubsurfaceAlbedo" 
		-at "float3" -nc 3;
	addAttr -ci true -sn "elvishraySubsurfaceAlbedoR" -ln "elvishraySubsurfaceAlbedoR" 
		-dv 0.83 -at "float" -p "elvishraySubsurfaceAlbedo";
	addAttr -ci true -sn "elvishraySubsurfaceAlbedoG" -ln "elvishraySubsurfaceAlbedoG" 
		-dv 0.79 -at "float" -p "elvishraySubsurfaceAlbedo";
	addAttr -ci true -sn "elvishraySubsurfaceAlbedoB" -ln "elvishraySubsurfaceAlbedoB" 
		-dv 0.75 -at "float" -p "elvishraySubsurfaceAlbedo";
	addAttr -ci true -uac -sn "elvishraySubsurfaceMeanFreePath" -ln "elvishraySubsurfaceMeanFreePath" 
		-at "float3" -nc 3;
	addAttr -ci true -sn "elvishraySubsurfaceMeanFreePathR" -ln "elvishraySubsurfaceMeanFreePathR" 
		-dv 8.51 -at "float" -p "elvishraySubsurfaceMeanFreePath";
	addAttr -ci true -sn "elvishraySubsurfaceMeanFreePathG" -ln "elvishraySubsurfaceMeanFreePathG" 
		-dv 5.57 -at "float" -p "elvishraySubsurfaceMeanFreePath";
	addAttr -ci true -sn "elvishraySubsurfaceMeanFreePathB" -ln "elvishraySubsurfaceMeanFreePathB" 
		-dv 3.95 -at "float" -p "elvishraySubsurfaceMeanFreePath";
	addAttr -ci true -sn "elvishraySubsurfaceEta" -ln "elvishraySubsurfaceEta" -dv 1.5 
		-min 1 -max 10 -smx 3 -at "float";
	addAttr -ci true -sn "elvishraySubsurfaceMaxSolidAngle" -ln "elvishraySubsurfaceMaxSolidAngle" 
		-dv 1 -min 0 -max 10 -smx 2 -at "float";
	addAttr -ci true -sn "elvishraySubsurfaceScale" -ln "elvishraySubsurfaceScale" -dv 
		1 -min 0 -max 1000 -smx 20 -at "float";
	addAttr -ci true -sn "elvishraySubsurfaceSpace" -ln "elvishraySubsurfaceSpace" -dt "string";
	addAttr -ci true -sn "elvishraySubsurfaceTwoSided" -ln "elvishraySubsurfaceTwoSided" 
		-min 0 -max 1 -at "bool";
	addAttr -ci true -sn "elvishrayCaustics" -ln "elvishrayCaustics" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "elvishrayCausticKr" -ln "elvishrayCausticKr" -min 0 -max 10 
		-smx 1 -at "float";
	addAttr -ci true -sn "elvishrayCausticKt" -ln "elvishrayCausticKt" -min 0 -max 10 
		-smx 1 -at "float";
	addAttr -ci true -sn "elvishrayCausticEta" -ln "elvishrayCausticEta" -dv 1.5 -min 
		1 -max 10 -smx 3 -at "float";
	addAttr -ci true -uac -sn "elvishrayCausticSpecularColor" -ln "elvishrayCausticSpecularColor" 
		-at "float3" -nc 3;
	addAttr -ci true -sn "elvishrayCausticSpecularColorR" -ln "elvishrayCausticSpecularColorR" 
		-dv 1 -at "float" -p "elvishrayCausticSpecularColor";
	addAttr -ci true -sn "elvishrayCausticSpecularColorG" -ln "elvishrayCausticSpecularColorG" 
		-dv 1 -at "float" -p "elvishrayCausticSpecularColor";
	addAttr -ci true -sn "elvishrayCausticSpecularColorB" -ln "elvishrayCausticSpecularColorB" 
		-dv 1 -at "float" -p "elvishrayCausticSpecularColor";
	addAttr -ci true -uac -sn "elvishrayCausticTransmitColor" -ln "elvishrayCausticTransmitColor" 
		-at "float3" -nc 3;
	addAttr -ci true -sn "elvishrayCausticTransmitColorR" -ln "elvishrayCausticTransmitColorR" 
		-dv 1 -at "float" -p "elvishrayCausticTransmitColor";
	addAttr -ci true -sn "elvishrayCausticTransmitColorG" -ln "elvishrayCausticTransmitColorG" 
		-dv 1 -at "float" -p "elvishrayCausticTransmitColor";
	addAttr -ci true -sn "elvishrayCausticTransmitColorB" -ln "elvishrayCausticTransmitColorB" 
		-dv 1 -at "float" -p "elvishrayCausticTransmitColor";
	addAttr -ci true -sn "elvishrayCausticPhotonFile" -ln "elvishrayCausticPhotonFile" 
		-dt "string";
	addAttr -ci true -sn "elvishrayCausticGeometrySet" -ln "elvishrayCausticGeometrySet" 
		-dt "string";
	addAttr -ci true -sn "elvishrayOverrideOpaqueShadows" -ln "elvishrayOverrideOpaqueShadows" 
		-min 0 -max 1 -at "bool";
	addAttr -ci true -sn "elvishrayOpaqueShadows" -ln "elvishrayOpaqueShadows" -dv 1 
		-min 0 -max 1 -at "bool";
	setAttr ".c" -type "float3" 0 0.003000021 0.5 ;
	setAttr ".elvishrayShader" -type "string" "";
	setAttr ".elvishraySubsurfaceDiffuseFile" -type "string" "diffuse.sdb";
	setAttr ".elvishraySubsurfaceSpace" -type "string" "world";
	setAttr ".elvishrayCausticPhotonFile" -type "string" "caustics.sdb";
	setAttr ".elvishrayCausticGeometrySet" -type "string" "reflection";
select -ne :postProcessList1;
	setAttr -k on ".cch";
	setAttr -cb on ".ihi";
	setAttr -k on ".nds";
	setAttr -cb on ".bnm";
	setAttr -s 2 ".p";
select -ne :defaultRenderingList1;
select -ne :renderGlobalsList1;
	setAttr -k on ".cch";
	setAttr -cb on ".ihi";
	setAttr -k on ".nds";
	setAttr -cb on ".bnm";
select -ne :defaultRenderGlobals;
	addAttr -ci true -sn "inputPath" -ln "inputPath" -dt "string";
	addAttr -ci true -sn "shaderPath" -ln "shaderPath" -dt "string";
	addAttr -ci true -sn "texturePath" -ln "texturePath" -dt "string";
	addAttr -ci true -sn "generatorPath" -ln "generatorPath" -dt "string";
	addAttr -ci true -sn "imageioPath" -ln "imageioPath" -dt "string";
	addAttr -ci true -sn "elvishrayViewer" -ln "elvishrayViewer" -dt "string";
	addAttr -ci true -sn "elvishrayVerbosity" -ln "elvishrayVerbosity" -dv 1 -min 1 
		-max 5 -smx 5 -at "long";
	addAttr -ci true -sn "elvishrayUserScript" -ln "elvishrayUserScript" -dt "string";
	addAttr -ci true -sn "elvishrayBinary" -ln "elvishrayBinary" -dv 1 -min 0 -max 1 
		-at "bool";
	addAttr -ci true -sn "elvishrayGenerateSceneScriptOnly" -ln "elvishrayGenerateSceneScriptOnly" 
		-min 0 -max 1 -at "bool";
	addAttr -ci true -sn "elvishrayFullPathNames" -ln "elvishrayFullPathNames" -min 
		0 -max 1 -at "bool";
	addAttr -ci true -sn "elvishrayCleanupSceneScript" -ln "elvishrayCleanupSceneScript" 
		-dv 1 -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "elvishrayNetRender" -ln "elvishrayNetRender" -min 0 -max 1 
		-at "bool";
	addAttr -ci true -sn "elvishrayNetRenderList" -ln "elvishrayNetRenderList" -dt "string";
	addAttr -ci true -sn "elvishrayRenderCmd" -ln "elvishrayRenderCmd" -dt "string";
	addAttr -ci true -sn "elvishrayMocha" -ln "elvishrayMocha" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "elvishrayWireframe" -ln "elvishrayWireframe" -min 0 -max 1 
		-at "bool";
	addAttr -ci true -sn "elvishrayProgressive" -ln "elvishrayProgressive" -min 0 -max 
		1 -at "bool";
	addAttr -ci true -sn "elvishrayPreview" -ln "elvishrayPreview" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "elvishrayPreviewSpatialQuality" -ln "elvishrayPreviewSpatialQuality" 
		-dv 0.1 -min 0 -max 1 -smx 1 -at "float";
	addAttr -ci true -sn "elvishraySpatialQuality" -ln "elvishraySpatialQuality" -at "long2" 
		-nc 2;
	addAttr -ci true -sn "elvishraySpatialQualityX" -ln "elvishraySpatialQualityX" -dv 
		4 -min 1 -max 16 -at "long" -p "elvishraySpatialQuality";
	addAttr -ci true -sn "elvishraySpatialQualityY" -ln "elvishraySpatialQualityY" -dv 
		4 -min 1 -max 16 -at "long" -p "elvishraySpatialQuality";
	addAttr -ci true -sn "elvishrayDOFQuality" -ln "elvishrayDOFQuality" -dv 16 -at "long";
	addAttr -ci true -sn "elvishrayPixelFilter" -ln "elvishrayPixelFilter" -dt "string";
	addAttr -ci true -sn "elvishrayPixelFilterWidth" -ln "elvishrayPixelFilterWidth" 
		-at "float2" -nc 2;
	addAttr -ci true -sn "elvishrayPixelFilterWidthX" -ln "elvishrayPixelFilterWidthX" 
		-dv 2 -min 0 -max 64 -at "float" -p "elvishrayPixelFilterWidth";
	addAttr -ci true -sn "elvishrayPixelFilterWidthY" -ln "elvishrayPixelFilterWidthY" 
		-dv 2 -min 0 -max 64 -at "float" -p "elvishrayPixelFilterWidth";
	addAttr -ci true -sn "elvishrayTemporalQuality" -ln "elvishrayTemporalQuality" -dv 
		16 -min 1 -max 1000 -smx 100 -at "long";
	addAttr -ci true -sn "elvishrayUseCameraShutter" -ln "elvishrayUseCameraShutter" 
		-dv 1 -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "elvishrayShutterAngle" -ln "elvishrayShutterAngle" -dv 144 
		-min 0 -max 360 -smx 360 -at "float";
	addAttr -ci true -sn "elvishrayShutterStart" -ln "elvishrayShutterStart" -dv -72 
		-min -360 -max 360 -smx 360 -at "float";
	addAttr -ci true -sn "elvishrayRayTraceMotion" -ln "elvishrayRayTraceMotion" -min 
		0 -max 1 -at "bool";
	addAttr -ci true -sn "elvishrayGain" -ln "elvishrayGain" -dv 1 -at "float";
	addAttr -ci true -sn "elvishrayDither" -ln "elvishrayDither" -dv 0.5 -at "float";
	addAttr -ci true -sn "elvishrayBucketSize" -ln "elvishrayBucketSize" -at "long2" 
		-nc 2;
	addAttr -ci true -sn "elvishrayBucketSizeX" -ln "elvishrayBucketSizeX" -dv 32 -at "long" 
		-p "elvishrayBucketSize";
	addAttr -ci true -sn "elvishrayBucketSizeY" -ln "elvishrayBucketSizeY" -dv 32 -at "long" 
		-p "elvishrayBucketSize";
	addAttr -ci true -sn "elvishrayBucketOrder" -ln "elvishrayBucketOrder" -dt "string";
	addAttr -ci true -sn "elvishrayGridSize" -ln "elvishrayGridSize" -dv 256 -at "long";
	addAttr -ci true -sn "elvishrayTextureMemory" -ln "elvishrayTextureMemory" -dv 20480 
		-at "long";
	addAttr -ci true -sn "elvishrayTrimCurveQuality" -ln "elvishrayTrimCurveQuality" 
		-dv 1 -at "float";
	addAttr -ci true -sn "elvishrayTrimMemory" -ln "elvishrayTrimMemory" -dv 10240 -at "long";
	addAttr -ci true -sn "elvishrayThreads" -ln "elvishrayThreads" -dv 1 -min 1 -max 
		100 -smx 8 -at "long";
	addAttr -ci true -sn "elvishrayImageFormat" -ln "elvishrayImageFormat" -min 0 -max 
		1000 -smx 1000 -at "long";
	addAttr -ci true -sn "elvishrayUseOverrideSurfaceShader" -ln "elvishrayUseOverrideSurfaceShader" 
		-min 0 -max 1 -at "bool";
	addAttr -ci true -sn "elvishrayOverrideSurfaceShader" -ln "elvishrayOverrideSurfaceShader" 
		-dt "string";
	addAttr -ci true -sn "elvishrayOcclusionDBName" -ln "elvishrayOcclusionDBName" -dt "string";
	addAttr -ci true -sn "elvishrayOcclusionEnable" -ln "elvishrayOcclusionEnable" -dv 
		1 -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "elvishrayOcclusionSampleRays" -ln "elvishrayOcclusionSampleRays" 
		-dv 64 -min 16 -max 16384 -smx 2048 -at "long";
	addAttr -ci true -sn "elvishrayOcclusionAdaptive" -ln "elvishrayOcclusionAdaptive" 
		-min 0 -max 4 -smx 1 -at "float";
	addAttr -ci true -sn "elvishrayOcclusionMinSamples" -ln "elvishrayOcclusionMinSamples" 
		-dv 3 -min 1 -max 10 -smx 10 -at "long";
	addAttr -ci true -sn "elvishrayOcclusionMaxErr" -ln "elvishrayOcclusionMaxErr" -dv 
		0.25 -min 0 -max 1 -smx 1 -at "float";
	addAttr -ci true -sn "elvishrayOcclusionMaxPixelDist" -ln "elvishrayOcclusionMaxPixelDist" 
		-dv 20 -min 0 -max 100 -smx 30 -at "float";
	addAttr -ci true -sn "elvishrayOcclusionMaxHitDist" -ln "elvishrayOcclusionMaxHitDist" 
		-dv 10000 -min 0 -max 1000000 -smx 10000 -at "float";
	addAttr -ci true -sn "elvishrayOcclusionBias" -ln "elvishrayOcclusionBias" -dv 0.01 
		-min 0 -max 100 -smx 10 -at "float";
	addAttr -ci true -sn "elvishrayOcclusionFalloff" -ln "elvishrayOcclusionFalloff" 
		-min 0 -max 100 -smx 10 -at "float";
	addAttr -ci true -sn "elvishrayOcclusionFalloffMode" -ln "elvishrayOcclusionFalloffMode" 
		-min 0 -max 1 -en "Exponential:Polynomial" -at "enum";
	addAttr -ci true -sn "elvishrayOcclusionGain" -ln "elvishrayOcclusionGain" -dv 1 
		-min 0 -max 50 -smx 10 -at "float";
	addAttr -ci true -sn "elvishrayOcclusionGamma" -ln "elvishrayOcclusionGamma" -dv 
		1 -min 0.1 -max 10 -smx 10 -at "float";
	addAttr -ci true -sn "elvishrayOcclusionTwosided" -ln "elvishrayOcclusionTwosided" 
		-dv 1 -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "elvishrayReflOcclusionAngle" -ln "elvishrayReflOcclusionAngle" 
		-dv 0.01 -min 0 -max 3.14159 -smx 0.1 -at "float";
	addAttr -ci true -sn "elvishrayReflOcclusionSampleRays" -ln "elvishrayReflOcclusionSampleRays" 
		-dv 4 -min 1 -max 256 -smx 64 -at "long";
	addAttr -ci true -sn "elvishrayOverrideIndirect" -ln "elvishrayOverrideIndirect" 
		-min 0 -max 1 -at "bool";
	addAttr -ci true -sn "elvishrayIndirectDBName" -ln "elvishrayIndirectDBName" -dt "string";
	addAttr -ci true -sn "elvishrayIndirectIntensity" -ln "elvishrayIndirectIntensity" 
		-dv 1 -min 0 -max 1000 -smx 1 -at "float";
	addAttr -ci true -sn "elvishrayIndirectSampleRays" -ln "elvishrayIndirectSampleRays" 
		-dv 64 -min 16 -max 16384 -smx 1024 -at "long";
	addAttr -ci true -sn "elvishrayIndirectMinSamples" -ln "elvishrayIndirectMinSamples" 
		-dv 3 -min 1 -max 10 -smx 10 -at "long";
	addAttr -ci true -sn "elvishrayIndirectMaxErr" -ln "elvishrayIndirectMaxErr" -dv 
		0.25 -min 0 -max 1 -smx 1 -at "float";
	addAttr -ci true -sn "elvishrayIndirectMaxPixelDist" -ln "elvishrayIndirectMaxPixelDist" 
		-dv 20 -min 0 -max 100 -smx 30 -at "float";
	addAttr -ci true -sn "elvishrayIndirectMaxHitDist" -ln "elvishrayIndirectMaxHitDist" 
		-dv 1000000 -min 0 -max 1000000 -smx 10000 -at "float";
	addAttr -ci true -sn "elvishrayIndirectBias" -ln "elvishrayIndirectBias" -dv 0.01 
		-min 0 -max 100 -smx 10 -at "float";
	addAttr -ci true -sn "elvishrayIndirectFalloff" -ln "elvishrayIndirectFalloff" -min 
		0 -max 100 -smx 10 -at "float";
	addAttr -ci true -sn "elvishrayIndirectFalloffMode" -ln "elvishrayIndirectFalloffMode" 
		-min 0 -max 1 -en "Exponential:Polynomial" -at "enum";
	addAttr -ci true -sn "elvishraySubsurfaceScattering" -ln "elvishraySubsurfaceScattering" 
		-dv 1 -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "elvishraySubsurfaceBakeDiffuseShader" -ln "elvishraySubsurfaceBakeDiffuseShader" 
		-dt "string";
	addAttr -ci true -sn "elvishrayRerenderDepthMaps" -ln "elvishrayRerenderDepthMaps" 
		-dv 1 -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "elvishrayLiveUpdates" -ln "elvishrayLiveUpdates" -min 0 -max 
		1 -at "bool";
	addAttr -ci true -sn "elvishrayShadowMapUpdates" -ln "elvishrayShadowMapUpdates" 
		-dv 1 -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "elvishraySorbettoPreview" -ln "elvishraySorbettoPreview" -min 
		0 -max 1 -at "bool";
	addAttr -ci true -sn "elvishrayReshadeRays" -ln "elvishrayReshadeRays" -min 0 -max 
		1 -at "bool";
	addAttr -ci true -sn "elvishrayDefaultMeshInterpolation" -ln "elvishrayDefaultMeshInterpolation" 
		-min 0 -max 1 -en "Linear:Catmull-Clark" -at "enum";
	addAttr -ci true -sn "elvishrayOverrideMisc" -ln "elvishrayOverrideMisc" -dv 1 -min 
		0 -max 1 -at "bool";
	addAttr -ci true -sn "elvishrayShadingQuality" -ln "elvishrayShadingQuality" -dv 
		1 -min 0 -max 10 -smx 4 -at "float";
	addAttr -ci true -sn "elvishrayMaxRadius" -ln "elvishrayMaxRadius" -min 0 -max 1000000 
		-smx 100 -at "float";
	addAttr -ci true -sn "elvishrayOpaqueShadows" -ln "elvishrayOpaqueShadows" -dv 1 
		-min 0 -max 1 -at "bool";
	addAttr -ci true -sn "elvishrayTraceDisplacements" -ln "elvishrayTraceDisplacements" 
		-min 0 -max 1 -at "bool";
	addAttr -ci true -sn "elvishrayOverrideDicing" -ln "elvishrayOverrideDicing" -dv 
		1 -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "elvishrayBinaryDicing" -ln "elvishrayBinaryDicing" -min 0 
		-max 1 -at "bool";
	addAttr -ci true -sn "elvishrayDicingCurvature" -ln "elvishrayDicingCurvature" -dv 
		10 -min 0 -max 180 -smx 180 -at "float";
	addAttr -ci true -sn "elvishrayDicingHighCurvature" -ln "elvishrayDicingHighCurvature" 
		-dv 120 -min 0 -max 180 -smx 180 -at "float";
	addAttr -ci true -sn "elvishrayMotionFactor" -ln "elvishrayMotionFactor" -dv 1 -min 
		0 -max 10 -smx 10 -at "float";
	addAttr -ci true -sn "elvishrayKeepCreases" -ln "elvishrayKeepCreases" -min 0 -max 
		1 -at "bool";
	addAttr -ci true -sn "elvishrayRasterOrient" -ln "elvishrayRasterOrient" -dv 1 -min 
		0 -max 1 -at "bool";
	addAttr -ci true -sn "elvishrayFixedDicing" -ln "elvishrayFixedDicing" -at "long2" 
		-nc 2;
	addAttr -ci true -sn "elvishrayFixedDicingX" -ln "elvishrayFixedDicingX" -min 0 
		-max 10000 -at "long" -p "elvishrayFixedDicing";
	addAttr -ci true -sn "elvishrayFixedDicingY" -ln "elvishrayFixedDicingY" -min 0 
		-max 10000 -at "long" -p "elvishrayFixedDicing";
	addAttr -ci true -sn "elvishrayStereo" -ln "elvishrayStereo" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "elvishrayStereoSeparation" -ln "elvishrayStereoSeparation" 
		-min 0 -max 1000 -smx 10 -at "float";
	addAttr -ci true -sn "elvishrayStereoConvergence" -ln "elvishrayStereoConvergence" 
		-min 0 -max 1000000 -smx 1000 -at "float";
	addAttr -ci true -sn "elvishrayStereoShade" -ln "elvishrayStereoShade" -min 0 -max 
		2 -en "Center:Left:Right" -at "enum";
	addAttr -ci true -sn "elvishrayStereoProjection" -ln "elvishrayStereoProjection" 
		-min 0 -max 2 -en "Off-axis:Parallel:Toe-in" -at "enum";
	addAttr -ci true -sn "elvishrayVolumeLightDensity" -ln "elvishrayVolumeLightDensity" 
		-dv 0.005 -min 0 -max 1000 -smx 1 -at "float";
	addAttr -ci true -sn "elvishrayVolumeOpacityDensity" -ln "elvishrayVolumeOpacityDensity" 
		-min 0 -max 1000 -smx 1 -at "float";
	addAttr -ci true -sn "elvishrayVolumeStepsize" -ln "elvishrayVolumeStepsize" -dv 
		0.1 -min 0 -max 1000 -smx 1 -at "float";
	addAttr -ci true -sn "elvishrayVolumeMinsteps" -ln "elvishrayVolumeMinsteps" -dv 
		3 -min 0 -max 1000 -smx 1 -at "long";
	addAttr -ci true -sn "elvishrayVolumeMaxsteps" -ln "elvishrayVolumeMaxsteps" -dv 
		100 -min 0 -max 10000 -smx 1000 -at "long";
	addAttr -ci true -sn "elvishrayVolumeIntegStart" -ln "elvishrayVolumeIntegStart" 
		-min 0 -max 10000 -smx 1000 -at "float";
	addAttr -ci true -sn "elvishrayVolumeIntegEnd" -ln "elvishrayVolumeIntegEnd" -dv 
		10000 -min 0 -max 100000 -smx 10000 -at "float";
	addAttr -ci true -sn "elvishrayBakeFixedDicing" -ln "elvishrayBakeFixedDicing" -dv 
		1 -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "elvishrayOutputBeauty" -ln "elvishrayOutputBeauty" -dv 1 -min 
		0 -max 1 -at "bool";
	addAttr -ci true -sn "elvishrayOutputDiffuse" -ln "elvishrayOutputDiffuse" -min 
		0 -max 1 -at "bool";
	addAttr -ci true -sn "elvishrayOutputSpecular" -ln "elvishrayOutputSpecular" -min 
		0 -max 1 -at "bool";
	addAttr -ci true -sn "elvishrayOutputReflect" -ln "elvishrayOutputReflect" -min 
		0 -max 1 -at "bool";
	addAttr -ci true -sn "elvishrayOutputRefract" -ln "elvishrayOutputRefract" -min 
		0 -max 1 -at "bool";
	addAttr -ci true -sn "elvishrayOutputOcclusion" -ln "elvishrayOutputOcclusion" -min 
		0 -max 1 -at "bool";
	addAttr -ci true -sn "elvishrayOutputReflectionOcclusion" -ln "elvishrayOutputReflectionOcclusion" 
		-min 0 -max 1 -at "bool";
	addAttr -ci true -sn "elvishrayOutputSubsurface" -ln "elvishrayOutputSubsurface" 
		-min 0 -max 1 -at "bool";
	addAttr -ci true -sn "elvishrayOutputTranslucence" -ln "elvishrayOutputTranslucence" 
		-min 0 -max 1 -at "bool";
	addAttr -ci true -sn "elvishrayOutputIncandescence" -ln "elvishrayOutputIncandescence" 
		-min 0 -max 1 -at "bool";
	addAttr -ci true -sn "elvishrayOutputGlow" -ln "elvishrayOutputGlow" -min 0 -max 
		1 -at "bool";
	addAttr -ci true -sn "elvishrayOutputUnlit" -ln "elvishrayOutputUnlit" -min 0 -max 
		1 -at "bool";
	addAttr -ci true -sn "elvishrayOutputShadowed" -ln "elvishrayOutputShadowed" -min 
		0 -max 1 -at "bool";
	addAttr -ci true -sn "elvishrayOutputUnshadowedDiffuse" -ln "elvishrayOutputUnshadowedDiffuse" 
		-min 0 -max 1 -at "bool";
	addAttr -ci true -sn "elvishrayOutputDepth" -ln "elvishrayOutputDepth" -min 0 -max 
		1 -at "bool";
	setAttr -k on ".cch";
	setAttr -k on ".nds";
	setAttr -k on ".clip";
	setAttr -k on ".edm";
	setAttr -k on ".edl";
	setAttr ".ren" -type "string" "liquid";
	setAttr -av -k on ".esr";
	setAttr -k on ".ors";
	setAttr -k on ".outf" 20;
	setAttr -k on ".gama";
	setAttr -av ".bfs";
	setAttr -k on ".be";
	setAttr -k on ".fec";
	setAttr -k on ".ofc";
	setAttr ".ifp" -type "string" "test";
	setAttr -k on ".comp";
	setAttr -k on ".cth";
	setAttr -k on ".soll";
	setAttr -k on ".rd" 3;
	setAttr -k on ".lp";
	setAttr -k on ".sp";
	setAttr -k on ".shs";
	setAttr -k on ".lpr";
	setAttr -k on ".mm";
	setAttr -k on ".npu";
	setAttr -k on ".itf";
	setAttr -k on ".shp";
	setAttr -k on ".uf";
	setAttr -k on ".oi";
	setAttr -k on ".rut";
	setAttr -av -k on ".mbf";
	setAttr -k on ".afp";
	setAttr -k on ".pfb";
	setAttr -av -k on ".bll";
	setAttr -k on ".bls";
	setAttr -k on ".smv";
	setAttr -k on ".ubc";
	setAttr -k on ".mbc";
	setAttr -k on ".udbx";
	setAttr -k on ".smc";
	setAttr -k on ".kmv";
	setAttr -k on ".rlen";
	setAttr -av -k on ".frts";
	setAttr -k on ".tlwd";
	setAttr -k on ".tlht";
	setAttr -k on ".jfc";
	setAttr ".inputPath" -type "string" "$ER_HOME/inputs:$LUNA_HOME/inputs";
	setAttr ".shaderPath" -type "string" "$ER_HOME/shaders:$LUNA_HOME/shaders";
	setAttr ".texturePath" -type "string" "$ER_HOME/textures:$LUNA_HOME/textures";
	setAttr ".generatorPath" -type "string" "$ER_HOME/lib:$LUNA_HOME/lib";
	setAttr ".imageioPath" -type "string" "$ER_HOME/lib:$LUNA_HOME/lib";
	setAttr ".elvishrayViewer" -type "string" "iv";
	setAttr ".elvishrayUserScript" -type "string" "";
	setAttr ".elvishrayNetRenderList" -type "string" "";
	setAttr ".elvishrayRenderCmd" -type "string" "elvishray";
	setAttr ".elvishrayPixelFilter" -type "string" "gaussian";
	setAttr ".elvishrayBucketOrder" -type "string" "horizontal";
	setAttr ".elvishrayOverrideSurfaceShader" -type "string" "";
	setAttr ".elvishrayOcclusionDBName" -type "string" "occlusion.sdb";
	setAttr ".elvishrayIndirectDBName" -type "string" "indirect.sdb";
	setAttr ".elvishraySubsurfaceBakeDiffuseShader" -type "string" "bakediffuse";
select -ne :defaultResolution;
	setAttr -k on ".cch";
	setAttr -k on ".nds";
	setAttr ".pa" 1;
	setAttr -k on ".al";
	setAttr -k on ".off";
	setAttr -k on ".fld";
	setAttr -k on ".zsl";
select -ne :defaultLightSet;
	setAttr -k on ".cch";
	setAttr -k on ".nds";
	setAttr -s 2 ".dsm";
	setAttr -k on ".mwc";
	setAttr ".ro" yes;
select -ne :hardwareRenderGlobals;
	setAttr -k on ".cch";
	setAttr -cb on ".ihi";
	setAttr -k on ".nds";
	setAttr -cb on ".bnm";
	setAttr ".ctrs" 256;
	setAttr ".btrs" 512;
	setAttr -k off -cb on ".ehql";
	setAttr -k off -cb on ".eams";
	setAttr -k off ".eeaa";
	setAttr -k off ".engm";
	setAttr -k off ".mes";
	setAttr -k off ".emb";
	setAttr -k off ".mbbf";
	setAttr -k off ".mbs";
	setAttr -k off ".trm";
	setAttr -k off -cb on ".tshc";
	setAttr -k off ".clmt";
	setAttr -k off -cb on ".tcov";
	setAttr -k off -cb on ".lith";
	setAttr -k off -cb on ".sobc";
	setAttr -k off -cb on ".cuth";
	setAttr -k off -cb on ".mgcs";
	setAttr -k off ".twa";
	setAttr -k off ".twz";
	setAttr -k on ".hwcc";
	setAttr -k on ".hwdp";
	setAttr -k on ".hwql";
select -ne :defaultHardwareRenderGlobals;
	setAttr -k on ".cch";
	setAttr -cb on ".ihi";
	setAttr -k on ".nds";
	setAttr -cb on ".bnm";
	setAttr -k on ".rp";
	setAttr -k on ".cai";
	setAttr -k on ".coi";
	setAttr -cb on ".bc";
	setAttr -av -k on ".bcb";
	setAttr -av -k on ".bcg";
	setAttr -av -k on ".bcr";
	setAttr -k on ".ei";
	setAttr -k on ".ex";
	setAttr -k on ".es";
	setAttr -av ".ef";
	setAttr -k on ".bf";
	setAttr -k on ".fii";
	setAttr -k on ".sf";
	setAttr -k on ".gr";
	setAttr -k on ".li";
	setAttr -k on ".ls";
	setAttr -k on ".mb";
	setAttr -k on ".ti";
	setAttr -k on ".txt";
	setAttr -k on ".mpr";
	setAttr -k on ".wzd";
	setAttr ".fn" -type "string" "im";
	setAttr -k on ".if";
	setAttr ".res" -type "string" "ntsc_4d 646 485 1.333";
	setAttr -k on ".as";
	setAttr -k on ".ds";
	setAttr -k on ".lm";
	setAttr -k on ".fir";
	setAttr -k on ".aap";
	setAttr -k on ".gh";
	setAttr -cb on ".sd";
select -ne :ikSystem;
	setAttr -s 4 ".sol";
connectAttr "polyTriangulate1.out" "pCubeShape1.i";
connectAttr "polyCube2.out" "pCubeShape2.i";
connectAttr "polyCube3.out" "pCubeShape3.i";
connectAttr "stereoCameraCenterCamShape.sm" "stereoCamera.sm";
connectAttr "stereoCameraCenterCamShape.isep" "stereoCamera.isep";
connectAttr "stereoCameraCenterCamShape.zp" "stereoCamera.zp";
connectAttr "stereoCameraCenterCamShape.tia" "stereoCamera.tia";
connectAttr "stereoCameraCenterCamShape.fofr" "stereoCamera.fofr";
connectAttr "stereoCameraCenterCamShape.fofl" "stereoCamera.fofl";
connectAttr "stereoCameraCenterCamShape.fl" "stereoCamera.fli";
connectAttr "stereoCamera.msg" "stereoCamera.centerCam";
connectAttr "stereoCameraLeft.msg" "stereoCamera.leftCam";
connectAttr "stereoCameraRight.msg" "stereoCamera.rightCam";
connectAttr "stereoCameraCenterCamShape.dncp" "stereoCameraFrustum.dnc";
connectAttr "stereoCameraCenterCamShape.dfcp" "stereoCameraFrustum.dfc";
connectAttr "stereoCameraCenterCamShape.df" "stereoCameraFrustum.df";
connectAttr "stereoCameraCenterCamShape.zpp" "stereoCameraFrustum.zpp";
connectAttr "stereoCameraCenterCamShape.zpt" "stereoCameraFrustum.zpt";
connectAttr "stereoCameraCenterCamShape.zpc" "stereoCameraFrustum.zpc";
connectAttr "stereoCameraCenterCamShape.svv" "stereoCameraFrustum.svv";
connectAttr "stereoCameraCenterCamShape.svt" "stereoCameraFrustum.svt";
connectAttr "stereoCameraCenterCamShape.svc" "stereoCameraFrustum.svc";
connectAttr "stereoCameraCenterCamShape.ss" "stereoCameraFrustum.ss";
connectAttr "stereoCameraCenterCamShape.zp" "stereoCameraFrustum.zp";
connectAttr "stereoCameraLeft.msg" "stereoCameraFrustum.lc";
connectAttr "stereoCameraRight.msg" "stereoCameraFrustum.rc";
connectAttr "stereoCameraCenterCamShape.msg" "stereoCameraFrustum.cc";
connectAttr "stereoCamera.slo" "stereoCameraLeft.tx" -l on;
connectAttr "stereoCamera.sla" "stereoCameraLeft.ry" -l on;
connectAttr "stereoCameraCenterCamShape.hfa" "stereoCameraLeftShape.hfa";
connectAttr "stereoCameraCenterCamShape.vfa" "stereoCameraLeftShape.vfa";
connectAttr "stereoCameraCenterCamShape.fl" "stereoCameraLeftShape.fl";
connectAttr "stereoCameraCenterCamShape.lsr" "stereoCameraLeftShape.lsr";
connectAttr "stereoCameraCenterCamShape.ncp" "stereoCameraLeftShape.ncp";
connectAttr "stereoCameraCenterCamShape.fcp" "stereoCameraLeftShape.fcp";
connectAttr "stereoCameraCenterCamShape.fs" "stereoCameraLeftShape.fs";
connectAttr "stereoCameraCenterCamShape.fd" "stereoCameraLeftShape.fd";
connectAttr "stereoCameraCenterCamShape.sa" "stereoCameraLeftShape.sa";
connectAttr "stereoCameraCenterCamShape.cpt" "stereoCameraLeftShape.cpt";
connectAttr "stereoCameraCenterCamShape.ff" "stereoCameraLeftShape.ff";
connectAttr "stereoCameraCenterCamShape.dfg" "stereoCameraLeftShape.dfg";
connectAttr "stereoCameraCenterCamShape.dr" "stereoCameraLeftShape.dr";
connectAttr "stereoCamera.fbol" "stereoCameraLeftShape.hfo";
connectAttr "stereoCamera.sro" "stereoCameraRight.tx" -l on;
connectAttr "stereoCamera.sra" "stereoCameraRight.ry" -l on;
connectAttr "stereoCameraCenterCamShape.hfa" "stereoCameraRightShape.hfa";
connectAttr "stereoCameraCenterCamShape.vfa" "stereoCameraRightShape.vfa";
connectAttr "stereoCameraCenterCamShape.fl" "stereoCameraRightShape.fl";
connectAttr "stereoCameraCenterCamShape.lsr" "stereoCameraRightShape.lsr";
connectAttr "stereoCameraCenterCamShape.ncp" "stereoCameraRightShape.ncp";
connectAttr "stereoCameraCenterCamShape.fcp" "stereoCameraRightShape.fcp";
connectAttr "stereoCameraCenterCamShape.fs" "stereoCameraRightShape.fs";
connectAttr "stereoCameraCenterCamShape.fd" "stereoCameraRightShape.fd";
connectAttr "stereoCameraCenterCamShape.sa" "stereoCameraRightShape.sa";
connectAttr "stereoCameraCenterCamShape.cpt" "stereoCameraRightShape.cpt";
connectAttr "stereoCameraCenterCamShape.ff" "stereoCameraRightShape.ff";
connectAttr "stereoCameraCenterCamShape.dfg" "stereoCameraRightShape.dfg";
connectAttr "stereoCameraCenterCamShape.dr" "stereoCameraRightShape.dr";
connectAttr "stereoCamera.fbor" "stereoCameraRightShape.hfo";
relationship "link" ":lightLinker1" ":initialShadingGroup.message" ":defaultLightSet.message";
relationship "link" ":lightLinker1" ":initialParticleSE.message" ":defaultLightSet.message";
relationship "link" ":lightLinker1" "lambert3SG.message" ":defaultLightSet.message";
relationship "link" ":lightLinker1" "lambert4SG.message" ":defaultLightSet.message";
relationship "shadowLink" ":lightLinker1" ":initialShadingGroup.message" ":defaultLightSet.message";
relationship "shadowLink" ":lightLinker1" ":initialParticleSE.message" ":defaultLightSet.message";
relationship "shadowLink" ":lightLinker1" "lambert3SG.message" ":defaultLightSet.message";
relationship "shadowLink" ":lightLinker1" "lambert4SG.message" ":defaultLightSet.message";
relationship "ignore" ":lightLinker1" "pCubeShape1.message" "pointLightShape2.message";
relationship "ignore" ":lightLinker1" "pCubeShape2.message" "pointLightShape1.message";
connectAttr "layerManager.dli[0]" "defaultLayer.id";
connectAttr "renderLayerManager.rlmi[0]" "defaultRenderLayer.rlid";
connectAttr "polyCube1.out" "polyTriangulate1.ip";
connectAttr "lambert3.oc" "lambert3SG.ss";
connectAttr "pCubeShape2.iog" "lambert3SG.dsm" -na;
connectAttr "lambert3SG.msg" "materialInfo1.sg";
connectAttr "lambert3.msg" "materialInfo1.m";
connectAttr "lambert4.oc" "lambert4SG.ss";
connectAttr "pCubeShape3.iog" "lambert4SG.dsm" -na;
connectAttr "lambert4SG.msg" "materialInfo2.sg";
connectAttr "lambert4.msg" "materialInfo2.m";
connectAttr "lambert3SG.pa" ":renderPartition.st" -na;
connectAttr "lambert4SG.pa" ":renderPartition.st" -na;
connectAttr "pCubeShape1.iog" ":initialShadingGroup.dsm" -na;
connectAttr "lambert3.msg" ":defaultShaderList1.s" -na;
connectAttr "lambert4.msg" ":defaultShaderList1.s" -na;
connectAttr "pointLightShape1.ltd" ":lightList1.l" -na;
connectAttr "pointLightShape2.ltd" ":lightList1.l" -na;
connectAttr "defaultRenderLayer.msg" ":defaultRenderingList1.r" -na;
connectAttr "pointLight1.iog" ":defaultLightSet.dsm" -na;
connectAttr "pointLight2.iog" ":defaultLightSet.dsm" -na;
// End of stereo_camera.ma
