//Maya ASCII 2012 scene
//Name: architectural.ma
//Last modified: Wed, Jun 19, 2013 07:49:08 PM
//Codeset: 936
requires maya "2012";
requires "liquidDLL" "2.3.5";
requires "ElvishRender" "0.0.1";
requires "Mayatomr" "2012.0m - 3.9.1.36 ";
requires "3delight_for_maya2012" "6.0.17";
requires "stereoCamera" "10.0";
requires "elvishray" "0.0.1";
currentUnit -l centimeter -a degree -t film;
fileInfo "application" "maya";
fileInfo "product" "Maya 2012";
fileInfo "version" "2012";
fileInfo "cutIdentifier" "001200000000-796618";
fileInfo "osv" "Microsoft Windows 7 Ultimate Edition, 64-bit Windows 7 Service Pack 1 (Build 7601)\n";
createNode transform -s -n "persp";
	setAttr ".t" -type "double3" -13.918249728560719 11.038588956929047 5.3596708503637727 ;
	setAttr ".r" -type "double3" -21.338352729610751 -64.200000000002035 1.096160644098256e-014 ;
	setAttr ".rp" -type "double3" 1.1102230246251565e-015 -8.8817841970012523e-016 1.7763568394002505e-015 ;
	setAttr ".rpt" -type "double3" -1.5787257499291787e-015 2.4071621844002335e-016 
		3.5394773620273538e-016 ;
createNode camera -s -n "perspShape" -p "persp";
	addAttr -ci true -sn "usedBy3dfm" -ln "usedBy3dfm" -at "message";
	setAttr -k off ".v";
	setAttr ".rnd" no;
	setAttr ".fl" 34.999999999999979;
	setAttr ".coi" 13.53097099936727;
	setAttr ".imn" -type "string" "persp";
	setAttr ".den" -type "string" "persp_depth";
	setAttr ".man" -type "string" "persp_mask";
	setAttr ".tp" -type "double3" 1.7275068799482369 5.4530970362090923 -3.379527054926168 ;
	setAttr ".hc" -type "string" "viewSet -p %camera";
createNode transform -s -n "top";
	setAttr ".v" no;
	setAttr ".t" -type "double3" 0 100.1 0 ;
	setAttr ".r" -type "double3" -89.999999999999986 0 0 ;
createNode camera -s -n "topShape" -p "top";
	setAttr -k off ".v" no;
	setAttr ".rnd" no;
	setAttr ".coi" 100.1;
	setAttr ".ow" 30;
	setAttr ".imn" -type "string" "top";
	setAttr ".den" -type "string" "top_depth";
	setAttr ".man" -type "string" "top_mask";
	setAttr ".hc" -type "string" "viewSet -t %camera";
	setAttr ".o" yes;
createNode transform -s -n "front";
	setAttr ".v" no;
	setAttr ".t" -type "double3" 0 0 100.1 ;
createNode camera -s -n "frontShape" -p "front";
	setAttr -k off ".v" no;
	setAttr ".rnd" no;
	setAttr ".coi" 100.1;
	setAttr ".ow" 30;
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
createNode transform -n "camera1";
	setAttr ".t" -type "double3" 0 0 20.212519669168824 ;
createNode camera -n "cameraShape1" -p "camera1";
	setAttr -k off ".v";
	setAttr ".cap" -type "double2" 1.41732 0.94488 ;
	setAttr ".ff" 0;
	setAttr ".ncp" 0.01;
	setAttr ".ow" 30;
	setAttr ".imn" -type "string" "camera1";
	setAttr ".den" -type "string" "camera1_depth";
	setAttr ".man" -type "string" "camera1_mask";
createNode transform -n "pPlane1";
	setAttr ".t" -type "double3" 0 4 2.4434389140271477 ;
	setAttr ".r" -type "double3" 0 -17.70206018629748 -0.16786276871553737 ;
createNode mesh -n "pPlaneShape1" -p "pPlane1";
	setAttr -k off ".v";
	setAttr ".vir" yes;
	setAttr ".vif" yes;
	setAttr ".uvst[0].uvsn" -type "string" "map1";
	setAttr ".cuvs" -type "string" "map1";
	setAttr ".dcc" -type "string" "Ambient+Diffuse";
	setAttr ".covm[0]"  0 1 1;
	setAttr ".cdvm[0]"  0 1 1;
createNode transform -n "pointLight3";
	setAttr ".t" -type "double3" 0.24724668182794574 7.504683558292359 -1.5860857561508244 ;
createNode pointLight -n "pointLightShape3" -p "pointLight3";
	setAttr -k off ".v";
	setAttr ".us" no;
createNode transform -n "pPlane3";
	setAttr ".t" -type "double3" 0.10855890731087237 5.3649041570765243 -2.2381591955153621 ;
	setAttr ".r" -type "double3" 89.309289583166844 0 0 ;
createNode mesh -n "pPlaneShape3" -p "pPlane3";
	setAttr -k off ".v";
	setAttr ".vir" yes;
	setAttr ".vif" yes;
	setAttr ".uvst[0].uvsn" -type "string" "map1";
	setAttr ".cuvs" -type "string" "map1";
	setAttr ".dcc" -type "string" "Ambient+Diffuse";
	setAttr ".covm[0]"  0 1 1;
	setAttr ".cdvm[0]"  0 1 1;
createNode transform -n "pCube1";
	setAttr ".t" -type "double3" 0.75977170836543717 2.7974990373834214 -1.502225491801672 ;
createNode mesh -n "pCubeShape1" -p "pCube1";
	setAttr -k off ".v";
	setAttr ".vir" yes;
	setAttr ".vif" yes;
	setAttr ".uvst[0].uvsn" -type "string" "map1";
	setAttr ".cuvs" -type "string" "map1";
	setAttr ".dcc" -type "string" "Ambient+Diffuse";
	setAttr ".covm[0]"  0 1 1;
	setAttr ".cdvm[0]"  0 1 1;
createNode lightLinker -s -n "lightLinker1";
	setAttr -s 9 ".lnk";
	setAttr -s 9 ".slnk";
createNode displayLayerManager -n "layerManager";
createNode displayLayer -n "defaultLayer";
createNode renderLayerManager -n "renderLayerManager";
createNode renderLayer -n "defaultRenderLayer";
	setAttr ".g" yes;
createNode script -n "uiConfigurationScriptNode";
	setAttr ".b" -type "string" (
		"// Maya Mel UI Configuration File.\n//\n//  This script is machine generated.  Edit at your own risk.\n//\n//\n\nglobal string $gMainPane;\nif (`paneLayout -exists $gMainPane`) {\n\n\tglobal int $gUseScenePanelConfig;\n\tint    $useSceneConfig = $gUseScenePanelConfig;\n\tint    $menusOkayInPanels = `optionVar -q allowMenusInPanels`;\tint    $nVisPanes = `paneLayout -q -nvp $gMainPane`;\n\tint    $nPanes = 0;\n\tstring $editorName;\n\tstring $panelName;\n\tstring $itemFilterName;\n\tstring $panelConfig;\n\n\t//\n\t//  get current state of the UI\n\t//\n\tsceneUIReplacement -update $gMainPane;\n\n\t$panelName = `sceneUIReplacement -getNextPanel \"modelPanel\" (localizedPanelLabel(\"Top View\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `modelPanel -unParent -l (localizedPanelLabel(\"Top View\")) -mbv $menusOkayInPanels `;\n\t\t\t$editorName = $panelName;\n            modelEditor -e \n                -camera \"top\" \n                -useInteractiveMode 0\n                -displayLights \"default\" \n                -displayAppearance \"wireframe\" \n"
		+ "                -activeOnly 0\n                -ignorePanZoom 0\n                -wireframeOnShaded 0\n                -headsUpDisplay 1\n                -selectionHiliteDisplay 1\n                -useDefaultMaterial 0\n                -bufferMode \"double\" \n                -twoSidedLighting 1\n                -backfaceCulling 0\n                -xray 0\n                -jointXray 0\n                -activeComponentsXray 0\n                -displayTextures 0\n                -smoothWireframe 0\n                -lineWidth 1\n                -textureAnisotropic 0\n                -textureHilight 1\n                -textureSampling 2\n                -textureDisplay \"modulate\" \n                -textureMaxSize 16384\n                -fogging 0\n                -fogSource \"fragment\" \n                -fogMode \"linear\" \n                -fogStart 0\n                -fogEnd 100\n                -fogDensity 0.1\n                -fogColor 0.5 0.5 0.5 1 \n                -maxConstantTransparency 1\n                -rendererName \"base_OpenGL_Renderer\" \n"
		+ "                -colorResolution 256 256 \n                -bumpResolution 512 512 \n                -textureCompression 0\n                -transparencyAlgorithm \"frontAndBackCull\" \n                -transpInShadows 0\n                -cullingOverride \"none\" \n                -lowQualityLighting 0\n                -maximumNumHardwareLights 1\n                -occlusionCulling 0\n                -shadingModel 0\n                -useBaseRenderer 0\n                -useReducedRenderer 0\n                -smallObjectCulling 0\n                -smallObjectThreshold -1 \n                -interactiveDisableShadows 0\n                -interactiveBackFaceCull 0\n                -sortTransparent 1\n                -nurbsCurves 1\n                -nurbsSurfaces 1\n                -polymeshes 1\n                -subdivSurfaces 1\n                -planes 1\n                -lights 1\n                -cameras 1\n                -controlVertices 1\n                -hulls 1\n                -grid 1\n                -joints 1\n                -ikHandles 1\n"
		+ "                -deformers 1\n                -dynamics 1\n                -fluids 1\n                -hairSystems 1\n                -follicles 1\n                -nCloths 1\n                -nParticles 1\n                -nRigids 1\n                -dynamicConstraints 1\n                -locators 1\n                -manipulators 1\n                -dimensions 1\n                -handles 1\n                -pivots 1\n                -textures 1\n                -strokes 1\n                -motionTrails 1\n                -shadows 0\n                $editorName;\nmodelEditor -e -viewSelected 0 $editorName;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tmodelPanel -edit -l (localizedPanelLabel(\"Top View\")) -mbv $menusOkayInPanels  $panelName;\n\t\t$editorName = $panelName;\n        modelEditor -e \n            -camera \"top\" \n            -useInteractiveMode 0\n            -displayLights \"default\" \n            -displayAppearance \"wireframe\" \n            -activeOnly 0\n            -ignorePanZoom 0\n            -wireframeOnShaded 0\n"
		+ "            -headsUpDisplay 1\n            -selectionHiliteDisplay 1\n            -useDefaultMaterial 0\n            -bufferMode \"double\" \n            -twoSidedLighting 1\n            -backfaceCulling 0\n            -xray 0\n            -jointXray 0\n            -activeComponentsXray 0\n            -displayTextures 0\n            -smoothWireframe 0\n            -lineWidth 1\n            -textureAnisotropic 0\n            -textureHilight 1\n            -textureSampling 2\n            -textureDisplay \"modulate\" \n            -textureMaxSize 16384\n            -fogging 0\n            -fogSource \"fragment\" \n            -fogMode \"linear\" \n            -fogStart 0\n            -fogEnd 100\n            -fogDensity 0.1\n            -fogColor 0.5 0.5 0.5 1 \n            -maxConstantTransparency 1\n            -rendererName \"base_OpenGL_Renderer\" \n            -colorResolution 256 256 \n            -bumpResolution 512 512 \n            -textureCompression 0\n            -transparencyAlgorithm \"frontAndBackCull\" \n            -transpInShadows 0\n            -cullingOverride \"none\" \n"
		+ "            -lowQualityLighting 0\n            -maximumNumHardwareLights 1\n            -occlusionCulling 0\n            -shadingModel 0\n            -useBaseRenderer 0\n            -useReducedRenderer 0\n            -smallObjectCulling 0\n            -smallObjectThreshold -1 \n            -interactiveDisableShadows 0\n            -interactiveBackFaceCull 0\n            -sortTransparent 1\n            -nurbsCurves 1\n            -nurbsSurfaces 1\n            -polymeshes 1\n            -subdivSurfaces 1\n            -planes 1\n            -lights 1\n            -cameras 1\n            -controlVertices 1\n            -hulls 1\n            -grid 1\n            -joints 1\n            -ikHandles 1\n            -deformers 1\n            -dynamics 1\n            -fluids 1\n            -hairSystems 1\n            -follicles 1\n            -nCloths 1\n            -nParticles 1\n            -nRigids 1\n            -dynamicConstraints 1\n            -locators 1\n            -manipulators 1\n            -dimensions 1\n            -handles 1\n            -pivots 1\n"
		+ "            -textures 1\n            -strokes 1\n            -motionTrails 1\n            -shadows 0\n            $editorName;\nmodelEditor -e -viewSelected 0 $editorName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextPanel \"modelPanel\" (localizedPanelLabel(\"Side View\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `modelPanel -unParent -l (localizedPanelLabel(\"Side View\")) -mbv $menusOkayInPanels `;\n\t\t\t$editorName = $panelName;\n            modelEditor -e \n                -camera \"side\" \n                -useInteractiveMode 0\n                -displayLights \"default\" \n                -displayAppearance \"wireframe\" \n                -activeOnly 0\n                -ignorePanZoom 0\n                -wireframeOnShaded 0\n                -headsUpDisplay 1\n                -selectionHiliteDisplay 1\n                -useDefaultMaterial 0\n                -bufferMode \"double\" \n                -twoSidedLighting 1\n                -backfaceCulling 0\n"
		+ "                -xray 0\n                -jointXray 0\n                -activeComponentsXray 0\n                -displayTextures 0\n                -smoothWireframe 0\n                -lineWidth 1\n                -textureAnisotropic 0\n                -textureHilight 1\n                -textureSampling 2\n                -textureDisplay \"modulate\" \n                -textureMaxSize 16384\n                -fogging 0\n                -fogSource \"fragment\" \n                -fogMode \"linear\" \n                -fogStart 0\n                -fogEnd 100\n                -fogDensity 0.1\n                -fogColor 0.5 0.5 0.5 1 \n                -maxConstantTransparency 1\n                -rendererName \"base_OpenGL_Renderer\" \n                -colorResolution 256 256 \n                -bumpResolution 512 512 \n                -textureCompression 0\n                -transparencyAlgorithm \"frontAndBackCull\" \n                -transpInShadows 0\n                -cullingOverride \"none\" \n                -lowQualityLighting 0\n                -maximumNumHardwareLights 1\n"
		+ "                -occlusionCulling 0\n                -shadingModel 0\n                -useBaseRenderer 0\n                -useReducedRenderer 0\n                -smallObjectCulling 0\n                -smallObjectThreshold -1 \n                -interactiveDisableShadows 0\n                -interactiveBackFaceCull 0\n                -sortTransparent 1\n                -nurbsCurves 1\n                -nurbsSurfaces 1\n                -polymeshes 1\n                -subdivSurfaces 1\n                -planes 1\n                -lights 1\n                -cameras 1\n                -controlVertices 1\n                -hulls 1\n                -grid 1\n                -joints 1\n                -ikHandles 1\n                -deformers 1\n                -dynamics 1\n                -fluids 1\n                -hairSystems 1\n                -follicles 1\n                -nCloths 1\n                -nParticles 1\n                -nRigids 1\n                -dynamicConstraints 1\n                -locators 1\n                -manipulators 1\n                -dimensions 1\n"
		+ "                -handles 1\n                -pivots 1\n                -textures 1\n                -strokes 1\n                -motionTrails 1\n                -shadows 0\n                $editorName;\nmodelEditor -e -viewSelected 0 $editorName;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tmodelPanel -edit -l (localizedPanelLabel(\"Side View\")) -mbv $menusOkayInPanels  $panelName;\n\t\t$editorName = $panelName;\n        modelEditor -e \n            -camera \"side\" \n            -useInteractiveMode 0\n            -displayLights \"default\" \n            -displayAppearance \"wireframe\" \n            -activeOnly 0\n            -ignorePanZoom 0\n            -wireframeOnShaded 0\n            -headsUpDisplay 1\n            -selectionHiliteDisplay 1\n            -useDefaultMaterial 0\n            -bufferMode \"double\" \n            -twoSidedLighting 1\n            -backfaceCulling 0\n            -xray 0\n            -jointXray 0\n            -activeComponentsXray 0\n            -displayTextures 0\n            -smoothWireframe 0\n            -lineWidth 1\n"
		+ "            -textureAnisotropic 0\n            -textureHilight 1\n            -textureSampling 2\n            -textureDisplay \"modulate\" \n            -textureMaxSize 16384\n            -fogging 0\n            -fogSource \"fragment\" \n            -fogMode \"linear\" \n            -fogStart 0\n            -fogEnd 100\n            -fogDensity 0.1\n            -fogColor 0.5 0.5 0.5 1 \n            -maxConstantTransparency 1\n            -rendererName \"base_OpenGL_Renderer\" \n            -colorResolution 256 256 \n            -bumpResolution 512 512 \n            -textureCompression 0\n            -transparencyAlgorithm \"frontAndBackCull\" \n            -transpInShadows 0\n            -cullingOverride \"none\" \n            -lowQualityLighting 0\n            -maximumNumHardwareLights 1\n            -occlusionCulling 0\n            -shadingModel 0\n            -useBaseRenderer 0\n            -useReducedRenderer 0\n            -smallObjectCulling 0\n            -smallObjectThreshold -1 \n            -interactiveDisableShadows 0\n            -interactiveBackFaceCull 0\n"
		+ "            -sortTransparent 1\n            -nurbsCurves 1\n            -nurbsSurfaces 1\n            -polymeshes 1\n            -subdivSurfaces 1\n            -planes 1\n            -lights 1\n            -cameras 1\n            -controlVertices 1\n            -hulls 1\n            -grid 1\n            -joints 1\n            -ikHandles 1\n            -deformers 1\n            -dynamics 1\n            -fluids 1\n            -hairSystems 1\n            -follicles 1\n            -nCloths 1\n            -nParticles 1\n            -nRigids 1\n            -dynamicConstraints 1\n            -locators 1\n            -manipulators 1\n            -dimensions 1\n            -handles 1\n            -pivots 1\n            -textures 1\n            -strokes 1\n            -motionTrails 1\n            -shadows 0\n            $editorName;\nmodelEditor -e -viewSelected 0 $editorName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextPanel \"modelPanel\" (localizedPanelLabel(\"Front View\")) `;\n\tif (\"\" == $panelName) {\n"
		+ "\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `modelPanel -unParent -l (localizedPanelLabel(\"Front View\")) -mbv $menusOkayInPanels `;\n\t\t\t$editorName = $panelName;\n            modelEditor -e \n                -camera \"front\" \n                -useInteractiveMode 0\n                -displayLights \"default\" \n                -displayAppearance \"wireframe\" \n                -activeOnly 0\n                -ignorePanZoom 0\n                -wireframeOnShaded 0\n                -headsUpDisplay 1\n                -selectionHiliteDisplay 1\n                -useDefaultMaterial 0\n                -bufferMode \"double\" \n                -twoSidedLighting 1\n                -backfaceCulling 0\n                -xray 0\n                -jointXray 0\n                -activeComponentsXray 0\n                -displayTextures 0\n                -smoothWireframe 0\n                -lineWidth 1\n                -textureAnisotropic 0\n                -textureHilight 1\n                -textureSampling 2\n                -textureDisplay \"modulate\" \n                -textureMaxSize 16384\n"
		+ "                -fogging 0\n                -fogSource \"fragment\" \n                -fogMode \"linear\" \n                -fogStart 0\n                -fogEnd 100\n                -fogDensity 0.1\n                -fogColor 0.5 0.5 0.5 1 \n                -maxConstantTransparency 1\n                -rendererName \"base_OpenGL_Renderer\" \n                -colorResolution 256 256 \n                -bumpResolution 512 512 \n                -textureCompression 0\n                -transparencyAlgorithm \"frontAndBackCull\" \n                -transpInShadows 0\n                -cullingOverride \"none\" \n                -lowQualityLighting 0\n                -maximumNumHardwareLights 1\n                -occlusionCulling 0\n                -shadingModel 0\n                -useBaseRenderer 0\n                -useReducedRenderer 0\n                -smallObjectCulling 0\n                -smallObjectThreshold -1 \n                -interactiveDisableShadows 0\n                -interactiveBackFaceCull 0\n                -sortTransparent 1\n                -nurbsCurves 1\n"
		+ "                -nurbsSurfaces 1\n                -polymeshes 1\n                -subdivSurfaces 1\n                -planes 1\n                -lights 1\n                -cameras 1\n                -controlVertices 1\n                -hulls 1\n                -grid 1\n                -joints 1\n                -ikHandles 1\n                -deformers 1\n                -dynamics 1\n                -fluids 1\n                -hairSystems 1\n                -follicles 1\n                -nCloths 1\n                -nParticles 1\n                -nRigids 1\n                -dynamicConstraints 1\n                -locators 1\n                -manipulators 1\n                -dimensions 1\n                -handles 1\n                -pivots 1\n                -textures 1\n                -strokes 1\n                -motionTrails 1\n                -shadows 0\n                $editorName;\nmodelEditor -e -viewSelected 0 $editorName;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tmodelPanel -edit -l (localizedPanelLabel(\"Front View\")) -mbv $menusOkayInPanels  $panelName;\n"
		+ "\t\t$editorName = $panelName;\n        modelEditor -e \n            -camera \"front\" \n            -useInteractiveMode 0\n            -displayLights \"default\" \n            -displayAppearance \"wireframe\" \n            -activeOnly 0\n            -ignorePanZoom 0\n            -wireframeOnShaded 0\n            -headsUpDisplay 1\n            -selectionHiliteDisplay 1\n            -useDefaultMaterial 0\n            -bufferMode \"double\" \n            -twoSidedLighting 1\n            -backfaceCulling 0\n            -xray 0\n            -jointXray 0\n            -activeComponentsXray 0\n            -displayTextures 0\n            -smoothWireframe 0\n            -lineWidth 1\n            -textureAnisotropic 0\n            -textureHilight 1\n            -textureSampling 2\n            -textureDisplay \"modulate\" \n            -textureMaxSize 16384\n            -fogging 0\n            -fogSource \"fragment\" \n            -fogMode \"linear\" \n            -fogStart 0\n            -fogEnd 100\n            -fogDensity 0.1\n            -fogColor 0.5 0.5 0.5 1 \n            -maxConstantTransparency 1\n"
		+ "            -rendererName \"base_OpenGL_Renderer\" \n            -colorResolution 256 256 \n            -bumpResolution 512 512 \n            -textureCompression 0\n            -transparencyAlgorithm \"frontAndBackCull\" \n            -transpInShadows 0\n            -cullingOverride \"none\" \n            -lowQualityLighting 0\n            -maximumNumHardwareLights 1\n            -occlusionCulling 0\n            -shadingModel 0\n            -useBaseRenderer 0\n            -useReducedRenderer 0\n            -smallObjectCulling 0\n            -smallObjectThreshold -1 \n            -interactiveDisableShadows 0\n            -interactiveBackFaceCull 0\n            -sortTransparent 1\n            -nurbsCurves 1\n            -nurbsSurfaces 1\n            -polymeshes 1\n            -subdivSurfaces 1\n            -planes 1\n            -lights 1\n            -cameras 1\n            -controlVertices 1\n            -hulls 1\n            -grid 1\n            -joints 1\n            -ikHandles 1\n            -deformers 1\n            -dynamics 1\n            -fluids 1\n"
		+ "            -hairSystems 1\n            -follicles 1\n            -nCloths 1\n            -nParticles 1\n            -nRigids 1\n            -dynamicConstraints 1\n            -locators 1\n            -manipulators 1\n            -dimensions 1\n            -handles 1\n            -pivots 1\n            -textures 1\n            -strokes 1\n            -motionTrails 1\n            -shadows 0\n            $editorName;\nmodelEditor -e -viewSelected 0 $editorName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextPanel \"modelPanel\" (localizedPanelLabel(\"Persp View\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `modelPanel -unParent -l (localizedPanelLabel(\"Persp View\")) -mbv $menusOkayInPanels `;\n\t\t\t$editorName = $panelName;\n            modelEditor -e \n                -camera \"persp\" \n                -useInteractiveMode 0\n                -displayLights \"default\" \n                -displayAppearance \"wireframe\" \n                -activeOnly 0\n                -ignorePanZoom 0\n"
		+ "                -wireframeOnShaded 0\n                -headsUpDisplay 1\n                -selectionHiliteDisplay 1\n                -useDefaultMaterial 0\n                -bufferMode \"double\" \n                -twoSidedLighting 1\n                -backfaceCulling 0\n                -xray 0\n                -jointXray 0\n                -activeComponentsXray 0\n                -displayTextures 0\n                -smoothWireframe 0\n                -lineWidth 1\n                -textureAnisotropic 0\n                -textureHilight 1\n                -textureSampling 2\n                -textureDisplay \"modulate\" \n                -textureMaxSize 16384\n                -fogging 0\n                -fogSource \"fragment\" \n                -fogMode \"linear\" \n                -fogStart 0\n                -fogEnd 100\n                -fogDensity 0.1\n                -fogColor 0.5 0.5 0.5 1 \n                -maxConstantTransparency 1\n                -rendererName \"base_OpenGL_Renderer\" \n                -colorResolution 256 256 \n                -bumpResolution 512 512 \n"
		+ "                -textureCompression 0\n                -transparencyAlgorithm \"frontAndBackCull\" \n                -transpInShadows 0\n                -cullingOverride \"none\" \n                -lowQualityLighting 0\n                -maximumNumHardwareLights 1\n                -occlusionCulling 0\n                -shadingModel 0\n                -useBaseRenderer 0\n                -useReducedRenderer 0\n                -smallObjectCulling 0\n                -smallObjectThreshold -1 \n                -interactiveDisableShadows 0\n                -interactiveBackFaceCull 0\n                -sortTransparent 1\n                -nurbsCurves 1\n                -nurbsSurfaces 1\n                -polymeshes 1\n                -subdivSurfaces 1\n                -planes 1\n                -lights 1\n                -cameras 1\n                -controlVertices 1\n                -hulls 1\n                -grid 1\n                -joints 1\n                -ikHandles 1\n                -deformers 1\n                -dynamics 1\n                -fluids 1\n"
		+ "                -hairSystems 1\n                -follicles 1\n                -nCloths 1\n                -nParticles 1\n                -nRigids 1\n                -dynamicConstraints 1\n                -locators 1\n                -manipulators 1\n                -dimensions 1\n                -handles 1\n                -pivots 1\n                -textures 1\n                -strokes 1\n                -motionTrails 1\n                -shadows 0\n                $editorName;\nmodelEditor -e -viewSelected 0 $editorName;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tmodelPanel -edit -l (localizedPanelLabel(\"Persp View\")) -mbv $menusOkayInPanels  $panelName;\n\t\t$editorName = $panelName;\n        modelEditor -e \n            -camera \"persp\" \n            -useInteractiveMode 0\n            -displayLights \"default\" \n            -displayAppearance \"wireframe\" \n            -activeOnly 0\n            -ignorePanZoom 0\n            -wireframeOnShaded 0\n            -headsUpDisplay 1\n            -selectionHiliteDisplay 1\n            -useDefaultMaterial 0\n"
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
		+ "\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"Stereo\" (localizedPanelLabel(\"Stereo\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `scriptedPanel -unParent  -type \"Stereo\" -l (localizedPanelLabel(\"Stereo\")) -mbv $menusOkayInPanels `;\nstring $editorName = ($panelName+\"Editor\");\n            stereoCameraView -e \n                -editorChanged \"updateModelPanelBar\" \n                -camera \"persp\" \n                -useInteractiveMode 0\n                -displayLights \"default\" \n                -displayAppearance \"wireframe\" \n                -activeOnly 0\n                -ignorePanZoom 0\n                -wireframeOnShaded 0\n                -headsUpDisplay 1\n                -selectionHiliteDisplay 1\n                -useDefaultMaterial 0\n                -bufferMode \"double\" \n                -twoSidedLighting 1\n                -backfaceCulling 0\n                -xray 0\n                -jointXray 0\n                -activeComponentsXray 0\n"
		+ "                -displayTextures 0\n                -smoothWireframe 0\n                -lineWidth 1\n                -textureAnisotropic 0\n                -textureHilight 1\n                -textureSampling 2\n                -textureDisplay \"modulate\" \n                -textureMaxSize 16384\n                -fogging 0\n                -fogSource \"fragment\" \n                -fogMode \"linear\" \n                -fogStart 0\n                -fogEnd 100\n                -fogDensity 0.1\n                -fogColor 0.5 0.5 0.5 1 \n                -maxConstantTransparency 1\n                -colorResolution 4 4 \n                -bumpResolution 4 4 \n                -textureCompression 0\n                -transparencyAlgorithm \"frontAndBackCull\" \n                -transpInShadows 0\n                -cullingOverride \"none\" \n                -lowQualityLighting 0\n                -maximumNumHardwareLights 0\n                -occlusionCulling 0\n                -shadingModel 0\n                -useBaseRenderer 0\n                -useReducedRenderer 0\n"
		+ "                -smallObjectCulling 0\n                -smallObjectThreshold -1 \n                -interactiveDisableShadows 0\n                -interactiveBackFaceCull 0\n                -sortTransparent 1\n                -nurbsCurves 1\n                -nurbsSurfaces 1\n                -polymeshes 1\n                -subdivSurfaces 1\n                -planes 1\n                -lights 1\n                -cameras 1\n                -controlVertices 1\n                -hulls 1\n                -grid 1\n                -joints 1\n                -ikHandles 1\n                -deformers 1\n                -dynamics 1\n                -fluids 1\n                -hairSystems 1\n                -follicles 1\n                -nCloths 1\n                -nParticles 1\n                -nRigids 1\n                -dynamicConstraints 1\n                -locators 1\n                -manipulators 1\n                -dimensions 1\n                -handles 1\n                -pivots 1\n                -textures 1\n                -strokes 1\n                -motionTrails 1\n"
		+ "                -shadows 0\n                -displayMode \"centerEye\" \n                -viewColor 0 0 0 1 \n                $editorName;\nstereoCameraView -e -viewSelected 0 $editorName;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Stereo\")) -mbv $menusOkayInPanels  $panelName;\nstring $editorName = ($panelName+\"Editor\");\n            stereoCameraView -e \n                -editorChanged \"updateModelPanelBar\" \n                -camera \"persp\" \n                -useInteractiveMode 0\n                -displayLights \"default\" \n                -displayAppearance \"wireframe\" \n                -activeOnly 0\n                -ignorePanZoom 0\n                -wireframeOnShaded 0\n                -headsUpDisplay 1\n                -selectionHiliteDisplay 1\n                -useDefaultMaterial 0\n                -bufferMode \"double\" \n                -twoSidedLighting 1\n                -backfaceCulling 0\n                -xray 0\n                -jointXray 0\n                -activeComponentsXray 0\n"
		+ "                -displayTextures 0\n                -smoothWireframe 0\n                -lineWidth 1\n                -textureAnisotropic 0\n                -textureHilight 1\n                -textureSampling 2\n                -textureDisplay \"modulate\" \n                -textureMaxSize 16384\n                -fogging 0\n                -fogSource \"fragment\" \n                -fogMode \"linear\" \n                -fogStart 0\n                -fogEnd 100\n                -fogDensity 0.1\n                -fogColor 0.5 0.5 0.5 1 \n                -maxConstantTransparency 1\n                -colorResolution 4 4 \n                -bumpResolution 4 4 \n                -textureCompression 0\n                -transparencyAlgorithm \"frontAndBackCull\" \n                -transpInShadows 0\n                -cullingOverride \"none\" \n                -lowQualityLighting 0\n                -maximumNumHardwareLights 0\n                -occlusionCulling 0\n                -shadingModel 0\n                -useBaseRenderer 0\n                -useReducedRenderer 0\n"
		+ "                -smallObjectCulling 0\n                -smallObjectThreshold -1 \n                -interactiveDisableShadows 0\n                -interactiveBackFaceCull 0\n                -sortTransparent 1\n                -nurbsCurves 1\n                -nurbsSurfaces 1\n                -polymeshes 1\n                -subdivSurfaces 1\n                -planes 1\n                -lights 1\n                -cameras 1\n                -controlVertices 1\n                -hulls 1\n                -grid 1\n                -joints 1\n                -ikHandles 1\n                -deformers 1\n                -dynamics 1\n                -fluids 1\n                -hairSystems 1\n                -follicles 1\n                -nCloths 1\n                -nParticles 1\n                -nRigids 1\n                -dynamicConstraints 1\n                -locators 1\n                -manipulators 1\n                -dimensions 1\n                -handles 1\n                -pivots 1\n                -textures 1\n                -strokes 1\n                -motionTrails 1\n"
		+ "                -shadows 0\n                -displayMode \"centerEye\" \n                -viewColor 0 0 0 1 \n                $editorName;\nstereoCameraView -e -viewSelected 0 $editorName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"multiListerPanel\" (localizedPanelLabel(\"Multilister\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `scriptedPanel -unParent  -type \"multiListerPanel\" -l (localizedPanelLabel(\"Multilister\")) -mbv $menusOkayInPanels `;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Multilister\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextPanel \"devicePanel\" (localizedPanelLabel(\"Devices\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\tdevicePanel -unParent -l (localizedPanelLabel(\"Devices\")) -mbv $menusOkayInPanels ;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n"
		+ "\t\tdevicePanel -edit -l (localizedPanelLabel(\"Devices\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"webBrowserPanel\" (localizedPanelLabel(\"Web Browser\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `scriptedPanel -unParent  -type \"webBrowserPanel\" -l (localizedPanelLabel(\"Web Browser\")) -mbv $menusOkayInPanels `;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Web Browser\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"delightShaderAssignmentPanel\" (localizedPanelLabel(\"3Delight Assignment\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `scriptedPanel -unParent  -type \"delightShaderAssignmentPanel\" -l (localizedPanelLabel(\"3Delight Assignment\")) -mbv $menusOkayInPanels `;\n\t\t}\n"
		+ "\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"3Delight Assignment\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"delightExplorerPanel\" (localizedPanelLabel(\"3Delight Relationship Editor\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `scriptedPanel -unParent  -type \"delightExplorerPanel\" -l (localizedPanelLabel(\"3Delight Relationship Editor\")) -mbv $menusOkayInPanels `;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"3Delight Relationship Editor\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\tif ($useSceneConfig) {\n        string $configName = `getPanel -cwl (localizedPanelLabel(\"Current Layout\"))`;\n        if (\"\" != $configName) {\n\t\t\tpanelConfiguration -edit -label (localizedPanelLabel(\"Current Layout\")) \n"
		+ "\t\t\t\t-defaultImage \"\"\n\t\t\t\t-image \"\"\n\t\t\t\t-sc false\n\t\t\t\t-configString \"global string $gMainPane; paneLayout -e -cn \\\"single\\\" -ps 1 100 100 $gMainPane;\"\n\t\t\t\t-removeAllPanels\n\t\t\t\t-ap false\n\t\t\t\t\t(localizedPanelLabel(\"Persp View\")) \n\t\t\t\t\t\"modelPanel\"\n"
		+ "\t\t\t\t\t\"$panelName = `modelPanel -unParent -l (localizedPanelLabel(\\\"Persp View\\\")) -mbv $menusOkayInPanels `;\\n$editorName = $panelName;\\nmodelEditor -e \\n    -cam `findStartUpCamera persp` \\n    -useInteractiveMode 0\\n    -displayLights \\\"default\\\" \\n    -displayAppearance \\\"wireframe\\\" \\n    -activeOnly 0\\n    -ignorePanZoom 0\\n    -wireframeOnShaded 0\\n    -headsUpDisplay 1\\n    -selectionHiliteDisplay 1\\n    -useDefaultMaterial 0\\n    -bufferMode \\\"double\\\" \\n    -twoSidedLighting 1\\n    -backfaceCulling 0\\n    -xray 0\\n    -jointXray 0\\n    -activeComponentsXray 0\\n    -displayTextures 0\\n    -smoothWireframe 0\\n    -lineWidth 1\\n    -textureAnisotropic 0\\n    -textureHilight 1\\n    -textureSampling 2\\n    -textureDisplay \\\"modulate\\\" \\n    -textureMaxSize 16384\\n    -fogging 0\\n    -fogSource \\\"fragment\\\" \\n    -fogMode \\\"linear\\\" \\n    -fogStart 0\\n    -fogEnd 100\\n    -fogDensity 0.1\\n    -fogColor 0.5 0.5 0.5 1 \\n    -maxConstantTransparency 1\\n    -rendererName \\\"base_OpenGL_Renderer\\\" \\n    -colorResolution 256 256 \\n    -bumpResolution 512 512 \\n    -textureCompression 0\\n    -transparencyAlgorithm \\\"frontAndBackCull\\\" \\n    -transpInShadows 0\\n    -cullingOverride \\\"none\\\" \\n    -lowQualityLighting 0\\n    -maximumNumHardwareLights 1\\n    -occlusionCulling 0\\n    -shadingModel 0\\n    -useBaseRenderer 0\\n    -useReducedRenderer 0\\n    -smallObjectCulling 0\\n    -smallObjectThreshold -1 \\n    -interactiveDisableShadows 0\\n    -interactiveBackFaceCull 0\\n    -sortTransparent 1\\n    -nurbsCurves 1\\n    -nurbsSurfaces 1\\n    -polymeshes 1\\n    -subdivSurfaces 1\\n    -planes 1\\n    -lights 1\\n    -cameras 1\\n    -controlVertices 1\\n    -hulls 1\\n    -grid 1\\n    -joints 1\\n    -ikHandles 1\\n    -deformers 1\\n    -dynamics 1\\n    -fluids 1\\n    -hairSystems 1\\n    -follicles 1\\n    -nCloths 1\\n    -nParticles 1\\n    -nRigids 1\\n    -dynamicConstraints 1\\n    -locators 1\\n    -manipulators 1\\n    -dimensions 1\\n    -handles 1\\n    -pivots 1\\n    -textures 1\\n    -strokes 1\\n    -motionTrails 1\\n    -shadows 0\\n    $editorName;\\nmodelEditor -e -viewSelected 0 $editorName\"\n"
		+ "\t\t\t\t\t\"modelPanel -edit -l (localizedPanelLabel(\\\"Persp View\\\")) -mbv $menusOkayInPanels  $panelName;\\n$editorName = $panelName;\\nmodelEditor -e \\n    -cam `findStartUpCamera persp` \\n    -useInteractiveMode 0\\n    -displayLights \\\"default\\\" \\n    -displayAppearance \\\"wireframe\\\" \\n    -activeOnly 0\\n    -ignorePanZoom 0\\n    -wireframeOnShaded 0\\n    -headsUpDisplay 1\\n    -selectionHiliteDisplay 1\\n    -useDefaultMaterial 0\\n    -bufferMode \\\"double\\\" \\n    -twoSidedLighting 1\\n    -backfaceCulling 0\\n    -xray 0\\n    -jointXray 0\\n    -activeComponentsXray 0\\n    -displayTextures 0\\n    -smoothWireframe 0\\n    -lineWidth 1\\n    -textureAnisotropic 0\\n    -textureHilight 1\\n    -textureSampling 2\\n    -textureDisplay \\\"modulate\\\" \\n    -textureMaxSize 16384\\n    -fogging 0\\n    -fogSource \\\"fragment\\\" \\n    -fogMode \\\"linear\\\" \\n    -fogStart 0\\n    -fogEnd 100\\n    -fogDensity 0.1\\n    -fogColor 0.5 0.5 0.5 1 \\n    -maxConstantTransparency 1\\n    -rendererName \\\"base_OpenGL_Renderer\\\" \\n    -colorResolution 256 256 \\n    -bumpResolution 512 512 \\n    -textureCompression 0\\n    -transparencyAlgorithm \\\"frontAndBackCull\\\" \\n    -transpInShadows 0\\n    -cullingOverride \\\"none\\\" \\n    -lowQualityLighting 0\\n    -maximumNumHardwareLights 1\\n    -occlusionCulling 0\\n    -shadingModel 0\\n    -useBaseRenderer 0\\n    -useReducedRenderer 0\\n    -smallObjectCulling 0\\n    -smallObjectThreshold -1 \\n    -interactiveDisableShadows 0\\n    -interactiveBackFaceCull 0\\n    -sortTransparent 1\\n    -nurbsCurves 1\\n    -nurbsSurfaces 1\\n    -polymeshes 1\\n    -subdivSurfaces 1\\n    -planes 1\\n    -lights 1\\n    -cameras 1\\n    -controlVertices 1\\n    -hulls 1\\n    -grid 1\\n    -joints 1\\n    -ikHandles 1\\n    -deformers 1\\n    -dynamics 1\\n    -fluids 1\\n    -hairSystems 1\\n    -follicles 1\\n    -nCloths 1\\n    -nParticles 1\\n    -nRigids 1\\n    -dynamicConstraints 1\\n    -locators 1\\n    -manipulators 1\\n    -dimensions 1\\n    -handles 1\\n    -pivots 1\\n    -textures 1\\n    -strokes 1\\n    -motionTrails 1\\n    -shadows 0\\n    $editorName;\\nmodelEditor -e -viewSelected 0 $editorName\"\n"
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
	setAttr ".rc" -type "string" "perspShape";
	setAttr ".ddin[0]" -type "string" "$SCN.$CAM.$F.$RND.bmp";
	setAttr ".ddim[0]" -type "string" "rgb";
	setAttr ".ddit[0]" -type "string" "bmp";
	setAttr ".ddbd[0]"  8;
	setAttr ".shdd" -type "string" "generatedShader";
	setAttr ".unt" yes;
	setAttr ".osg" yes;
	setAttr ".xres" 200;
	setAttr ".yres" 150;
	setAttr ".rdc" -type "string" "renderdl";
	setAttr ".prv" -type "string" "renderdl";
	setAttr ".lrs" -type "string" "R:/MyDocuments/maya/projects/default/rmantmp/architectural182.xml";
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
	setAttr ".rnd" -type "string" "ElvishRender";
	setAttr ".displayList" -type "stringArray" 16 "framebuffer" "alias" "cineon" "mayaiff" "openexr" "photoshop" "picio" "rib" "sgif" "softimage" "targa" "tiff" "wavefrontobj" "it" "combiner" "slim"  ;
createNode shadingEngine -n "phong1SG";
	setAttr ".ihi" 0;
	setAttr ".ro" yes;
createNode materialInfo -n "materialInfo1";
createNode polyPlane -n "polyPlane1";
	setAttr ".w" 14.113074275956471;
	setAttr ".h" 11.547068058246108;
	setAttr ".sw" 1;
	setAttr ".sh" 1;
	setAttr ".cuv" 2;
createNode shadingEngine -n "phong2SG";
	setAttr ".ihi" 0;
	setAttr ".ro" yes;
createNode materialInfo -n "materialInfo2";
createNode polyPlane -n "polyPlane3";
	setAttr ".w" 15.086398473362213;
	setAttr ".h" 10.822601614050285;
	setAttr ".sw" 1;
	setAttr ".sh" 1;
	setAttr ".cuv" 2;
createNode shadingEngine -n "phong3SG";
	setAttr ".ihi" 0;
	setAttr ".ro" yes;
createNode materialInfo -n "materialInfo3";
createNode shadingEngine -n "phong4SG";
	setAttr ".ihi" 0;
	setAttr ".ro" yes;
createNode materialInfo -n "materialInfo4";
createNode delightRenderPass -n "fullRenderPass";
	addAttr -ci true -sn "version" -ln "version" -dt "string";
	addAttr -ci true -sn "camera" -ln "camera" -at "message";
	addAttr -ci true -sn "connectToRenderGlobals" -ln "connectToRenderGlobals" -dv 1 
		-min 0 -max 1 -at "bool";
	addAttr -ci true -sn "resolution" -ln "resolution" -at "long2" -nc 2;
	addAttr -ci true -sn "resolutionX" -ln "resolutionX" -dv 720 -at "long" -p "resolution";
	addAttr -ci true -sn "resolutionY" -ln "resolutionY" -dv 486 -at "long" -p "resolution";
	addAttr -ci true -sn "resolutionMultiplier" -ln "resolutionMultiplier" -min 0 -max 
		3 -en "Full:Half:Quarter:Eighth" -at "enum";
	addAttr -ci true -sn "pixelAspectRatio" -ln "pixelAspectRatio" -dv 1 -min 0.01 -at "float";
	addAttr -ci true -sn "animation" -ln "animation" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "frameRange" -ln "frameRange" -at "float2" -nc 2;
	addAttr -ci true -sn "startFrame" -ln "startFrame" -dv 1 -at "float" -p "frameRange";
	addAttr -ci true -sn "endFrame" -ln "endFrame" -dv 1 -at "float" -p "frameRange";
	addAttr -ci true -sn "increment" -ln "increment" -dv 1 -min 0.0001 -at "float";
	addAttr -ci true -sn "useCropWindow" -ln "useCropWindow" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "cropMin" -ln "cropMin" -at "float2" -nc 2;
	addAttr -ci true -sn "cropMinX" -ln "cropMinX" -min 0 -max 1 -at "float" -p "cropMin";
	addAttr -ci true -sn "cropMinY" -ln "cropMinY" -min 0 -max 1 -at "float" -p "cropMin";
	addAttr -ci true -sn "cropMax" -ln "cropMax" -at "float2" -nc 2;
	addAttr -ci true -sn "cropMaxX" -ln "cropMaxX" -dv 1 -min 0 -max 1 -at "float" -p "cropMax";
	addAttr -ci true -sn "cropMaxY" -ln "cropMaxY" -dv 1 -min 0 -max 1 -at "float" -p "cropMax";
	addAttr -ci true -sn "bucketOrder" -ln "bucketOrder" -min 0 -max 4 -en "Horizontal:Vertical:Zigzag:Spiral:Circle" 
		-at "enum";
	addAttr -ci true -sn "useImagerShaders" -ln "useImagerShaders" -dv 1 -min 0 -max 
		1 -at "bool";
	addAttr -ci true -sn "renderMode" -ln "renderMode" -min 0 -max 3 -en "Render:Save RIB:RIB Archive:Background Render" 
		-at "enum";
	addAttr -ci true -sn "ribFilename" -ln "ribFilename" -dt "string";
	addAttr -ci true -sn "binaryRib" -ln "binaryRib" -dv 1 -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "compressedRib" -ln "compressedRib" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "backgroundRenderParams" -ln "backgroundRenderParams" -dt "string";
	addAttr -ci true -sn "renderLogFilename" -ln "renderLogFilename" -dt "string";
	addAttr -ci true -sn "shadingRate" -ln "shadingRate" -dv 1 -min 0.001 -at "float";
	addAttr -ci true -sn "pixelSamples" -ln "pixelSamples" -at "long2" -nc 2;
	addAttr -ci true -sn "pixelSamplesX" -ln "pixelSamplesX" -dv 3 -min 1 -at "long" 
		-p "pixelSamples";
	addAttr -ci true -sn "pixelSamplesY" -ln "pixelSamplesY" -dv 3 -min 1 -at "long" 
		-p "pixelSamples";
	addAttr -ci true -sn "pixelFilter" -ln "pixelFilter" -dv 5 -min 0 -max 8 -en "box:triangle:gaussian:catmull-rom:bessel:sinc:mitchell:zmin:zmax" 
		-at "enum";
	addAttr -ci true -sn "filterWidth" -ln "filterWidth" -at "float2" -nc 2;
	addAttr -ci true -sn "filterWidthX" -ln "filterWidthX" -dv 4 -min 0.001 -at "float" 
		-p "filterWidth";
	addAttr -ci true -sn "filterWidthY" -ln "filterWidthY" -dv 4 -min 0.001 -at "float" 
		-p "filterWidth";
	addAttr -ci true -sn "raytraceMaxSpecularDepth" -ln "raytraceMaxSpecularDepth" -dv 
		1 -min 0 -smx 16 -at "long";
	addAttr -ci true -sn "raytraceMaxDiffuseDepth" -ln "raytraceMaxDiffuseDepth" -dv 
		1 -min 0 -smx 16 -at "long";
	addAttr -ci true -sn "raytraceMaxDepth" -ln "raytraceMaxDepth" -dv 1 -min 0 -smx 
		16 -at "long";
	addAttr -ci true -sn "polygonalAperture" -ln "polygonalAperture" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "apertureNumberOfBlades" -ln "apertureNumberOfBlades" -dv 8 
		-min 3 -smx 20 -at "long";
	addAttr -ci true -sn "apertureRotation" -ln "apertureRotation" -min 0 -smx 360 -at "float";
	addAttr -ci true -sn "cameraBlur" -ln "cameraBlur" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "cameraBlurSamples" -ln "cameraBlurSamples" -dv 2 -min 2 -max 
		20 -at "long";
	addAttr -ci true -sn "transformationBlur" -ln "transformationBlur" -min 0 -max 1 
		-at "bool";
	addAttr -ci true -sn "transformationSamples" -ln "transformationSamples" -dv 2 -min 
		2 -max 20 -at "long";
	addAttr -ci true -sn "deformationBlur" -ln "deformationBlur" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "deformationSamples" -ln "deformationSamples" -dv 2 -min 2 
		-max 20 -at "long";
	addAttr -ci true -sn "motionBlurPosition" -ln "motionBlurPosition" -dv 1 -min 0 
		-max 2 -en "end on frame:centred on frame:start on frame" -at "enum";
	addAttr -ci true -sn "sampleMotionBlurOnIntegerFrames" -ln "sampleMotionBlurOnIntegerFrames" 
		-min 0 -max 1 -at "bool";
	addAttr -ci true -sn "shutterEfficiency" -ln "shutterEfficiency" -at "float2" -nc 
		2;
	addAttr -ci true -sn "shutterEfficiencyA" -ln "shutterEfficiencyA" -dv 1 -min 0 
		-max 1 -at "float" -p "shutterEfficiency";
	addAttr -ci true -sn "shutterEfficiencyB" -ln "shutterEfficiencyB" -dv 1 -min 0 
		-max 1 -at "float" -p "shutterEfficiency";
	addAttr -ci true -sn "shutterAngleScale" -ln "shutterAngleScale" -dv 1 -min 0.01 
		-at "float";
	addAttr -ci true -sn "sampleMotion" -ln "sampleMotion" -dv 1 -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "extremeMotionDepthOfField" -ln "extremeMotionDepthOfField" 
		-min 0 -max 1 -at "bool";
	addAttr -ci true -sn "renderSecondaryDisplays" -ln "renderSecondaryDisplays" -dv 
		1 -min 0 -max 1 -at "bool";
	addAttr -ci true -m -sn "displayRenderables" -ln "displayRenderables" -min 0 -max 
		1 -at "bool";
	addAttr -ci true -m -sn "displayFilenames" -ln "displayFilenames" -dt "string";
	addAttr -ci true -m -sn "displayDrivers" -ln "displayDrivers" -dt "string";
	addAttr -ci true -m -sn "displayOutputVariables" -ln "displayOutputVariables" -dt "string";
	addAttr -ci true -m -sn "displayCameras" -ln "displayCameras" -at "message";
	addAttr -ci true -m -sn "displayOverridePixelFilters" -ln "displayOverridePixelFilters" 
		-min 0 -max 1 -at "bool";
	addAttr -ci true -h true -m -sn "displayPixelFilters" -ln "displayPixelFilters" 
		-dv 5 -min 0 -max 8 -en "box:triangle:gaussian:catmull-rom:bessel:sinc:mitchell:zmin:zmax" 
		-at "enum";
	addAttr -ci true -m -sn "displayFilterWidthsX" -ln "displayFilterWidthsX" -dv 4 
		-min 0.001 -at "float";
	addAttr -ci true -m -sn "displayFilterWidthsY" -ln "displayFilterWidthsY" -dv 4 
		-min 0.001 -at "float";
	addAttr -ci true -m -sn "displayQuantizeZeros" -ln "displayQuantizeZeros" -at "long";
	addAttr -ci true -m -sn "displayQuantizeOnes" -ln "displayQuantizeOnes" -dv 255 
		-at "long";
	addAttr -ci true -m -sn "displayQuantizeMins" -ln "displayQuantizeMins" -at "long";
	addAttr -ci true -m -sn "displayQuantizeMaxs" -ln "displayQuantizeMaxs" -dv 255 
		-at "long";
	addAttr -ci true -m -sn "displayQuantizeDithers" -ln "displayQuantizeDithers" -dv 
		0.5 -at "float";
	addAttr -ci true -m -sn "displayHalfFloats" -ln "displayHalfFloats" -min 0 -max 
		1 -at "bool";
	addAttr -ci true -m -sn "displayGains" -ln "displayGains" -dv 1 -at "float";
	addAttr -ci true -m -sn "displayGammas" -ln "displayGammas" -dv 1 -at "float";
	addAttr -ci true -m -sn "displayMattes" -ln "displayMattes" -dv 1 -at "float";
	addAttr -ci true -m -sn "displayExclusives" -ln "displayExclusives" -at "float";
	addAttr -ci true -m -sn "displayAssociateAlphas" -ln "displayAssociateAlphas" -dv 
		1 -at "float";
	addAttr -ci true -m -sn "displayComputeAlphas" -ln "displayComputeAlphas" -at "float";
	addAttr -ci true -m -sn "displayEdgeEnables" -ln "displayEdgeEnables" -min 0 -max 
		1 -at "bool";
	addAttr -ci true -m -sn "displayEdgeVarNames" -ln "displayEdgeVarNames" -dt "string";
	addAttr -ci true -m -sn "displayEdgeThresholds" -ln "displayEdgeThresholds" -at "double";
	addAttr -ci true -uac -m -sn "displayEdgeColors" -ln "displayEdgeColors" -at "float3" 
		-nc 3;
	addAttr -ci true -sn "displayEdgeColorsR" -ln "displayEdgeColorsR" -at "float" -p "displayEdgeColors";
	addAttr -ci true -sn "displayEdgeColorsG" -ln "displayEdgeColorsG" -at "float" -p "displayEdgeColors";
	addAttr -ci true -sn "displayEdgeColorsB" -ln "displayEdgeColorsB" -at "float" -p "displayEdgeColors";
	addAttr -ci true -m -sn "displayEdgeFilterWidths" -ln "displayEdgeFilterWidths" 
		-at "double";
	addAttr -ci true -h true -m -sn "displayEdgeFilterWidthInterps" -ln "displayEdgeFilterWidthInterps" 
		-min 0 -max 1 -en "Pixels:% Of Frame Width" -at "enum";
	addAttr -ci true -m -sn "displayEdgeDepthEnables" -ln "displayEdgeDepthEnables" 
		-min 0 -max 1 -at "bool";
	addAttr -ci true -m -sn "displayEdgeDepthZMins" -ln "displayEdgeDepthZMins" -at "double";
	addAttr -ci true -m -sn "displayEdgeDepthZMaxs" -ln "displayEdgeDepthZMaxs" -at "double";
	addAttr -ci true -m -sn "displayEdgeDepthMinFilters" -ln "displayEdgeDepthMinFilters" 
		-at "double";
	addAttr -ci true -h true -m -sn "displayFrameCollapseStates" -ln "displayFrameCollapseStates" 
		-min 0 -max 1 -at "bool";
	addAttr -ci true -h true -m -sn "displayAdvancedFrameCollapseStates" -ln "displayAdvancedFrameCollapseStates" 
		-min 0 -max 1 -at "bool";
	addAttr -ci true -h true -m -sn "displayEdgeFrameCollapseStates" -ln "displayEdgeFrameCollapseStates" 
		-min 0 -max 1 -at "bool";
	addAttr -ci true -h true -sn "displaysAdvanceCollapseString" -ln "displaysAdvanceCollapseString" 
		-dt "string";
	addAttr -ci true -m -im false -sn "displaySubsetSets_0" -ln "displaySubsetSets_0" 
		-at "message";
	addAttr -ci true -sn "renderShadowMaps" -ln "renderShadowMaps" -dv 1 -min 0 -max 
		1 -at "bool";
	addAttr -ci true -sn "useDisplacementShadersInShadows" -ln "useDisplacementShadersInShadows" 
		-dv 1 -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "useSurfaceShadersInShadows" -ln "useSurfaceShadersInShadows" 
		-min 0 -max 1 -at "bool";
	addAttr -ci true -sn "useAtmosphereShadersInShadows" -ln "useAtmosphereShadersInShadows" 
		-min 0 -max 1 -at "bool";
	addAttr -ci true -sn "useInteriorShadersInShadows" -ln "useInteriorShadersInShadows" 
		-min 0 -max 1 -at "bool";
	addAttr -ci true -sn "shadowMapZThreshold" -ln "shadowMapZThreshold" -at "float3" 
		-nc 3;
	addAttr -ci true -sn "shadowMapZThresholdR" -ln "shadowMapZThresholdR" -dv 1 -at "float" 
		-p "shadowMapZThreshold";
	addAttr -ci true -sn "shadowMapZThresholdG" -ln "shadowMapZThresholdG" -dv 1 -at "float" 
		-p "shadowMapZThreshold";
	addAttr -ci true -sn "shadowMapZThresholdB" -ln "shadowMapZThresholdB" -dv 1 -at "float" 
		-p "shadowMapZThreshold";
	addAttr -ci true -sn "depthFilter" -ln "depthFilter" -min 0 -max 3 -en "min:max:average:midpoint" 
		-at "enum";
	addAttr -ci true -sn "volumeInterpretation" -ln "volumeInterpretation" -min 0 -max 
		2 -en "Discrete:Continuous:Distance Inside" -at "enum";
	addAttr -ci true -sn "giEffect" -ln "giEffect" -min 0 -max 4 -en "None:Occlusion:Fast Image Based Lighting:Full Global Illumination:Point-Based Full Global Illumination" 
		-at "enum";
	addAttr -ci true -sn "giAddToFinalMix" -ln "giAddToFinalMix" -dv 1 -min 0 -max 1 
		-at "bool";
	addAttr -ci true -sn "giColorBleedingIntensity" -ln "giColorBleedingIntensity" -dv 
		1 -min 0 -max 1e+020 -smn 0 -smx 1 -at "float";
	addAttr -ci true -sn "giCastShadowsFromEnvironment" -ln "giCastShadowsFromEnvironment" 
		-min 0 -max 1 -at "bool";
	addAttr -ci true -uac -sn "giBlackPoint" -ln "giBlackPoint" -at "float3" -nc 3;
	addAttr -ci true -sn "giBlackPointR" -ln "giBlackPointR" -at "float" -p "giBlackPoint";
	addAttr -ci true -sn "giBlackPointG" -ln "giBlackPointG" -at "float" -p "giBlackPoint";
	addAttr -ci true -sn "giBlackPointB" -ln "giBlackPointB" -at "float" -p "giBlackPoint";
	addAttr -ci true -uac -sn "giWhitePoint" -ln "giWhitePoint" -at "float3" -nc 3;
	addAttr -ci true -sn "giWhitePointR" -ln "giWhitePointR" -dv 1 -at "float" -p "giWhitePoint";
	addAttr -ci true -sn "giWhitePointG" -ln "giWhitePointG" -dv 1 -at "float" -p "giWhitePoint";
	addAttr -ci true -sn "giWhitePointB" -ln "giWhitePointB" -dv 1 -at "float" -p "giWhitePoint";
	addAttr -ci true -sn "giSamples" -ln "giSamples" -dv 64 -min 0 -max 1e+020 -smn 
		0 -smx 128 -at "long";
	addAttr -ci true -sn "giMaxDistance" -ln "giMaxDistance" -dv 1e+020 -min 0 -max 
		1e+020 -at "double";
	addAttr -ci true -sn "giFalloff" -ln "giFalloff" -min 0 -max 2 -en "None:Exponential:Polynomial" 
		-at "enum";
	addAttr -ci true -sn "giFalloffExponent" -ln "giFalloffExponent" -dv 2 -min 1 -max 
		10 -at "float";
	addAttr -ci true -sn "giCoordinateSystem" -ln "giCoordinateSystem" -at "message";
	addAttr -ci true -sn "giEnvironmentUseAsBackground" -ln "giEnvironmentUseAsBackground" 
		-min 0 -max 1 -at "bool";
	addAttr -ci true -sn "giEnvironmentIntensity" -ln "giEnvironmentIntensity" -dv 1 
		-min 0 -max 1e+020 -smn 0 -smx 1 -at "float";
	addAttr -ci true -sn "giEnvironmentSpecularity" -ln "giEnvironmentSpecularity" -dv 
		1 -min 0 -max 1 -at "float";
	addAttr -ci true -uac -sn "giEnvironmentColorGain" -ln "giEnvironmentColorGain" 
		-at "float3" -nc 3;
	addAttr -ci true -sn "giEnvironmentColorGainR" -ln "giEnvironmentColorGainR" -dv 
		1 -at "float" -p "giEnvironmentColorGain";
	addAttr -ci true -sn "giEnvironmentColorGainG" -ln "giEnvironmentColorGainG" -dv 
		1 -at "float" -p "giEnvironmentColorGain";
	addAttr -ci true -sn "giEnvironmentColorGainB" -ln "giEnvironmentColorGainB" -dv 
		1 -at "float" -p "giEnvironmentColorGain";
	addAttr -ci true -uac -sn "giEnvironmentColorOffset" -ln "giEnvironmentColorOffset" 
		-at "float3" -nc 3;
	addAttr -ci true -sn "giEnvironmentColorOffsetR" -ln "giEnvironmentColorOffsetR" 
		-at "float" -p "giEnvironmentColorOffset";
	addAttr -ci true -sn "giEnvironmentColorOffsetG" -ln "giEnvironmentColorOffsetG" 
		-at "float" -p "giEnvironmentColorOffset";
	addAttr -ci true -sn "giEnvironmentColorOffsetB" -ln "giEnvironmentColorOffsetB" 
		-at "float" -p "giEnvironmentColorOffset";
	addAttr -ci true -sn "renderPhotonMaps" -ln "renderPhotonMaps" -dv 2 -min 0 -max 
		2 -en "Never:Always:Automatic" -at "enum";
	addAttr -ci true -sn "photonMapsWriteMode" -ln "photonMapsWriteMode" -nn "Write Mode" 
		-dv 1 -min 0 -max 1 -en "Reuse existing photon maps:Overwrite existing photon maps" 
		-at "enum";
	addAttr -ci true -sn "numberOfPhotons" -ln "numberOfPhotons" -dv 50000 -min 0 -at "long";
	addAttr -ci true -sn "numberOfBounces" -ln "numberOfBounces" -dv 3 -min 1 -smx 16 
		-at "long";
	addAttr -ci true -sn "globalMap" -ln "globalMap" -dt "string";
	addAttr -ci true -sn "causticMap" -ln "causticMap" -dt "string";
	addAttr -ci true -sn "pointCloudCamera" -ln "pointCloudCamera" -at "message";
	addAttr -ci true -sn "pointCloudFile" -ln "pointCloudFile" -dt "string";
	addAttr -ci true -sn "pointCloudWriteMode" -ln "pointCloudWriteMode" -dv 1 -min 
		0 -max 1 -en "Reuse existing point cloud:Overwrite existing point cloud" -at "enum";
	addAttr -ci true -sn "pointCloudShadingRateMultiplier" -ln "pointCloudShadingRateMultiplier" 
		-dv 1 -min 0.0001 -smn 0.001 -smx 10 -at "float";
	addAttr -ci true -sn "pointCloudMaxSolidAngle" -ln "pointCloudMaxSolidAngle" -dv 
		0.1 -min 0 -max 1 -at "float";
	addAttr -ci true -sn "pointCloudBias" -ln "pointCloudBias" -dv 0.001 -min 0 -at "float";
	addAttr -ci true -sn "shaderCollection" -ln "shaderCollection" -at "message";
	addAttr -ci true -sn "objectsToRender" -ln "objectsToRender" -at "message";
	addAttr -ci true -sn "lightsToRender" -ln "lightsToRender" -at "message";
	addAttr -ci true -sn "clippingPlanesToRender" -ln "clippingPlanesToRender" -at "message";
	addAttr -ci true -sn "layerToRender" -ln "layerToRender" -at "message";
	addAttr -ci true -sn "generateRIBArchives" -ln "generateRIBArchives" -dv 1 -min 
		0 -max 1 -at "bool";
	addAttr -ci true -sn "ignoreArchivedObjects" -ln "ignoreArchivedObjects" -dv 1 -min 
		0 -max 1 -at "bool";
	addAttr -ci true -sn "archiveLighting" -ln "archiveLighting" -dv 2 -min 0 -max 2 
		-en "No Lighting:Light Linking:Light Sources & Light Linking" -at "enum";
	addAttr -ci true -sn "archiveWriteMode" -ln "archiveWriteMode" -dv 1 -min 0 -max 
		1 -en "Reuse existing archive:Overwrite existing archive" -at "enum";
	addAttr -ci true -sn "archiveTransforms" -ln "archiveTransforms" -dv 1 -min 0 -max 
		1 -at "bool";
	addAttr -ci true -sn "concatenateGeoTransforms" -ln "concatenateGeoTransforms" -dv 
		1 -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "concatenateLightTransforms" -ln "concatenateLightTransforms" 
		-dv 1 -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "archiveGeometryShaders" -ln "archiveGeometryShaders" -dv 1 
		-min 0 -max 1 -at "bool";
	addAttr -ci true -sn "archiveGeometryAttributes" -ln "archiveGeometryAttributes" 
		-dv 1 -min 0 -max 1 -at "bool";
	addAttr -ci true -m -sn "fragmentObjectSets" -ln "fragmentObjectSets" -at "message";
	addAttr -ci true -m -sn "fragmentFilenames" -ln "fragmentFilenames" -dt "string";
	addAttr -ci true -m -sn "fragmentUseStates" -ln "fragmentUseStates" -dv 1 -min 0 
		-max 1 -at "bool";
	addAttr -ci true -m -sn "fragmentWriteStates" -ln "fragmentWriteStates" -dv 1 -min 
		0 -max 1 -at "bool";
	addAttr -ci true -h true -m -sn "fragmentCollapseStates" -ln "fragmentCollapseStates" 
		-min 0 -max 1 -at "bool";
	addAttr -ci true -sn "fragmentBinaryRib" -ln "fragmentBinaryRib" -dv 1 -min 0 -max 
		1 -at "bool";
	addAttr -ci true -sn "fragmentCompressedRib" -ln "fragmentCompressedRib" -min 0 
		-max 1 -at "bool";
	addAttr -ci true -sn "fragmentWriteMode" -ln "fragmentWriteMode" -dv 1 -min 0 -max 
		1 -en "Reuse existing fragments:Overwrite existing fragments" -at "enum";
	addAttr -ci true -m -sn "riFilterPluginNames" -ln "riFilterPluginNames" -dt "string";
	addAttr -ci true -m -sn "riFilterPluginParams" -ln "riFilterPluginParams" -dt "string";
	addAttr -ci true -m -sn "riFilterPluginLoadStates" -ln "riFilterPluginLoadStates" 
		-min 0 -max 1 -at "bool";
	addAttr -ci true -h true -m -sn "riFilterPluginCollapseStates" -ln "riFilterPluginCollapseStates" 
		-min 0 -max 1 -at "bool";
	addAttr -ci true -sn "shaderPath" -ln "shaderPath" -dt "string";
	addAttr -ci true -sn "texturePath" -ln "texturePath" -dt "string";
	addAttr -ci true -sn "proceduralPath" -ln "proceduralPath" -dt "string";
	addAttr -ci true -sn "archivePath" -ln "archivePath" -dt "string";
	addAttr -ci true -sn "preRenderMEL" -ln "preRenderMEL" -dt "string";
	addAttr -ci true -sn "postRenderMEL" -ln "postRenderMEL" -dt "string";
	addAttr -ci true -sn "preFrameMEL" -ln "preFrameMEL" -dt "string";
	addAttr -ci true -sn "postFrameMEL" -ln "postFrameMEL" -dt "string";
	addAttr -ci true -sn "postOptionMEL" -ln "postOptionMEL" -dt "string";
	addAttr -ci true -sn "preWorldMEL" -ln "preWorldMEL" -dt "string";
	addAttr -ci true -sn "postWorldMEL" -ln "postWorldMEL" -dt "string";
	addAttr -ci true -sn "hider" -ln "hider" -min 0 -max 3 -en "hidden:raytrace:photon:progressive raytrace" 
		-at "enum";
	addAttr -ci true -sn "bucketSize" -ln "bucketSize" -at "long2" -nc 2;
	addAttr -ci true -sn "bucketSizeX" -ln "bucketSizeX" -dv 16 -min 2 -at "long" -p "bucketSize";
	addAttr -ci true -sn "bucketSizeY" -ln "bucketSizeY" -dv 16 -min 2 -at "long" -p "bucketSize";
	addAttr -ci true -sn "gridSize" -ln "gridSize" -dv 256 -min 16 -at "long";
	addAttr -ci true -sn "textureMemory" -ln "textureMemory" -dv 131072 -min 128 -at "long";
	addAttr -ci true -sn "eyeSplits" -ln "eyeSplits" -dv 10 -min 0 -at "long";
	addAttr -ci true -sn "opacityThreshold" -ln "opacityThreshold" -at "float3" -nc 
		3;
	addAttr -ci true -sn "opacityThresholdR" -ln "opacityThresholdR" -dv 0.99608 -at "float" 
		-p "opacityThreshold";
	addAttr -ci true -sn "opacityThresholdG" -ln "opacityThresholdG" -dv 0.99608 -at "float" 
		-p "opacityThreshold";
	addAttr -ci true -sn "opacityThresholdB" -ln "opacityThresholdB" -dv 0.99608 -at "float" 
		-p "opacityThreshold";
	addAttr -ci true -sn "imageZThreshold" -ln "imageZThreshold" -at "float3" -nc 3;
	addAttr -ci true -sn "imageZThresholdR" -ln "imageZThresholdR" -dv 0.99608 -at "float" 
		-p "imageZThreshold";
	addAttr -ci true -sn "imageZThresholdG" -ln "imageZThresholdG" -dv 0.99608 -at "float" 
		-p "imageZThreshold";
	addAttr -ci true -sn "imageZThresholdB" -ln "imageZThresholdB" -dv 0.99608 -at "float" 
		-p "imageZThreshold";
	addAttr -ci true -sn "numberOfCPUs" -ln "numberOfCPUs" -at "long";
	addAttr -ci true -sn "translateMayaShaders" -ln "translateMayaShaders" -dv 1 -min 
		0 -max 1 -at "bool";
	addAttr -ci true -sn "useMayaShaders" -ln "useMayaShaders" -dv 1 -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "translatedShadersAOVs" -ln "translatedShadersAOVs" -min 0 
		-max 2 -en "Default AOVs for Render Mode:Defined AOVs Only:All AOVs" -at "enum";
	addAttr -ci true -sn "standardAtmosphere" -ln "standardAtmosphere" -min 0 -max 1 
		-at "bool";
	addAttr -ci true -sn "volumeShadingRate" -ln "volumeShadingRate" -dv 1 -min 0.001 
		-at "float";
	addAttr -ci true -sn "useNetCache" -ln "useNetCache" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "netCacheDir" -ln "netCacheDir" -dt "string";
	addAttr -ci true -sn "netCacheSize" -ln "netCacheSize" -dv 100 -min 100 -at "long";
	addAttr -ci true -sn "outputRenderProgress" -ln "outputRenderProgress" -min 0 -max 
		1 -at "bool";
	addAttr -ci true -sn "statisticsLevel" -ln "statisticsLevel" -min 0 -max 3 -at "long";
	addAttr -ci true -sn "statisticsFile" -ln "statisticsFile" -dt "string";
	addAttr -ci true -h true -m -sn "displaySubsetTypes" -ln "displaySubsetTypes" -at "double";
	setAttr ".version" -type "string" "6.0.14";
	setAttr ".ribFilename" -type "string" "3delight/<scene>/rib/<scene>_<pass>_#.<ext>";
	setAttr ".backgroundRenderParams" -type "string" "";
	setAttr ".renderLogFilename" -type "string" "3delight/<scene>/rib/<scene>_<pass>_#.<ext>";
	setAttr ".displayRenderables[0]" yes;
	setAttr ".displayFilenames[0]" -type "string" "3delight/<scene>/image/<scene>_<pass>_<aov>_#.<ext>";
	setAttr ".displayDrivers[0]" -type "string" "maya_render_view";
	setAttr ".displayOutputVariables[0]" -type "string" "rgba";
	setAttr ".displayEdgeVarNames[0]" -type "string" "Ci";
	setAttr ".displayEdgeThresholds[0]"  0.1;
	setAttr ".displayEdgeColors[0]" -type "float3"  1 1 1;
	setAttr ".displayEdgeFilterWidths[0]"  2;
	setAttr ".displayEdgeDepthZMaxs[0]"  1000;
	setAttr ".displayEdgeDepthMinFilters[0]"  1;
	setAttr ".displayAdvancedFrameCollapseStates[0]" yes;
	setAttr ".displayEdgeFrameCollapseStates[0]" yes;
	setAttr ".globalMap" -type "string" "<scene>_<pass>_global_#.pmap";
	setAttr ".causticMap" -type "string" "<scene>_<pass>_caustic_#.pmap";
	setAttr ".pointCloudFile" -type "string" "<scene>_<pass>_GI_#.<ext>";
	setAttr ".shaderPath" -type "string" "@";
	setAttr ".texturePath" -type "string" "@";
	setAttr ".proceduralPath" -type "string" "@";
	setAttr ".archivePath" -type "string" "@";
	setAttr ".preRenderMEL" -type "string" "";
	setAttr ".postRenderMEL" -type "string" "";
	setAttr ".preFrameMEL" -type "string" "";
	setAttr ".postFrameMEL" -type "string" "";
	setAttr ".postOptionMEL" -type "string" "";
	setAttr ".preWorldMEL" -type "string" "";
	setAttr ".postWorldMEL" -type "string" "";
	setAttr ".netCacheDir" -type "string" "";
	setAttr ".statisticsFile" -type "string" "";
createNode expression -n "fullRenderPass_pixelRatioExpr";
	setAttr -k on ".nds";
	setAttr -s 3 ".in";
	setAttr -s 3 ".in";
	setAttr ".ixp" -type "string" ".O[0] = .I[0] / .I[1] * .I[2]";
	setAttr ".ani" 0;
createNode delightRenderGlobals -n "delightRenderGlobals1";
	addAttr -ci true -sn "version" -ln "version" -dt "string";
	addAttr -ci true -sn "renderPass" -ln "renderPass" -at "message";
	setAttr ".version" -type "string" "6.0.14";
createNode liquidShader -n "liquidShader1";
	addAttr -ci true -uac -h true -sn "surface_color" -ln "surface_color" -at "float3" 
		-nc 3;
	addAttr -ci true -h true -sn "surface_colorR" -ln "surface_colorR" -at "float" -p "surface_color";
	addAttr -ci true -h true -sn "surface_colorG" -ln "surface_colorG" -at "float" -p "surface_color";
	addAttr -ci true -h true -sn "surface_colorB" -ln "surface_colorB" -at "float" -p "surface_color";
	addAttr -ci true -uac -h true -sn "diffuse_color" -ln "diffuse_color" -at "float3" 
		-nc 3;
	addAttr -ci true -h true -sn "diffuse_colorR" -ln "diffuse_colorR" -at "float" -p "diffuse_color";
	addAttr -ci true -h true -sn "diffuse_colorG" -ln "diffuse_colorG" -at "float" -p "diffuse_color";
	addAttr -ci true -h true -sn "diffuse_colorB" -ln "diffuse_colorB" -at "float" -p "diffuse_color";
	addAttr -ci true -h true -sn "diffuse_weight" -ln "diffuse_weight" -at "double";
	addAttr -ci true -uac -h true -sn "specular_color" -ln "specular_color" -at "float3" 
		-nc 3;
	addAttr -ci true -h true -sn "specular_colorR" -ln "specular_colorR" -at "float" 
		-p "specular_color";
	addAttr -ci true -h true -sn "specular_colorG" -ln "specular_colorG" -at "float" 
		-p "specular_color";
	addAttr -ci true -h true -sn "specular_colorB" -ln "specular_colorB" -at "float" 
		-p "specular_color";
	addAttr -ci true -h true -sn "specular_weight" -ln "specular_weight" -at "double";
	addAttr -ci true -h true -sn "roughness" -ln "roughness" -at "double";
	addAttr -ci true -h true -sn "specular_mode" -ln "specular_mode" -at "double";
	addAttr -ci true -h true -sn "glossiness" -ln "glossiness" -at "double";
	addAttr -ci true -uac -h true -sn "reflection_color" -ln "reflection_color" -at "float3" 
		-nc 3;
	addAttr -ci true -h true -sn "reflection_colorR" -ln "reflection_colorR" -at "float" 
		-p "reflection_color";
	addAttr -ci true -h true -sn "reflection_colorG" -ln "reflection_colorG" -at "float" 
		-p "reflection_color";
	addAttr -ci true -h true -sn "reflection_colorB" -ln "reflection_colorB" -at "float" 
		-p "reflection_color";
	addAttr -ci true -h true -sn "reflection_weight" -ln "reflection_weight" -at "double";
	addAttr -ci true -uac -h true -sn "refraction_color" -ln "refraction_color" -at "float3" 
		-nc 3;
	addAttr -ci true -h true -sn "refraction_colorR" -ln "refraction_colorR" -at "float" 
		-p "refraction_color";
	addAttr -ci true -h true -sn "refraction_colorG" -ln "refraction_colorG" -at "float" 
		-p "refraction_color";
	addAttr -ci true -h true -sn "refraction_colorB" -ln "refraction_colorB" -at "float" 
		-p "refraction_color";
	addAttr -ci true -h true -sn "refraction_weight" -ln "refraction_weight" -at "double";
	addAttr -ci true -h true -sn "refraction_glossiness" -ln "refraction_glossiness" 
		-at "double";
	addAttr -ci true -h true -sn "refraction_thickness" -ln "refraction_thickness" -at "double";
	addAttr -ci true -uac -h true -sn "translucency_color" -ln "translucency_color" 
		-at "float3" -nc 3;
	addAttr -ci true -h true -sn "translucency_colorR" -ln "translucency_colorR" -at "float" 
		-p "translucency_color";
	addAttr -ci true -h true -sn "translucency_colorG" -ln "translucency_colorG" -at "float" 
		-p "translucency_color";
	addAttr -ci true -h true -sn "translucency_colorB" -ln "translucency_colorB" -at "float" 
		-p "translucency_color";
	addAttr -ci true -h true -sn "translucency_weight" -ln "translucency_weight" -at "double";
	addAttr -ci true -h true -sn "anisotropy" -ln "anisotropy" -at "double";
	addAttr -ci true -h true -sn "rotation" -ln "rotation" -at "double";
	addAttr -ci true -h true -sn "ior" -ln "ior" -at "double";
	addAttr -ci true -h true -sn "fresnel_by_ior" -ln "fresnel_by_ior" -at "double";
	addAttr -ci true -h true -sn "fresnel_0_degree_refl" -ln "fresnel_0_degree_refl" 
		-at "double";
	addAttr -ci true -h true -sn "fresnel_90_degree_refl" -ln "fresnel_90_degree_refl" 
		-at "double";
	addAttr -ci true -h true -sn "fresnel_curve" -ln "fresnel_curve" -at "double";
	addAttr -ci true -h true -sn "is_metal" -ln "is_metal" -at "double";
	addAttr -ci true -h true -sn "diffuse_samples" -ln "diffuse_samples" -at "double";
	addAttr -ci true -h true -sn "reflection_samples" -ln "reflection_samples" -at "double";
	addAttr -ci true -h true -sn "refraction_samples" -ln "refraction_samples" -at "double";
	addAttr -ci true -h true -sn "cutoff_threshold" -ln "cutoff_threshold" -at "double";
	addAttr -ci true -h true -sn "rmanParamLifCmdId" -ln "rmanParamLifCmdId" -dt "Int32Array";
	addAttr -ci true -h true -sn "rmanGroups" -ln "rmanGroups" -dt "stringArray";
	addAttr -ci true -h true -sn "rmanGroupLifCmdId" -ln "rmanGroupLifCmdId" -dt "Int32Array";
	addAttr -ci true -sn "rmanLifCtrls" -ln "rmanLifCtrls" -dt "stringArray";
	addAttr -ci true -sn "liqAssignedObjects" -ln "liqAssignedObjects" -at "message";
	addAttr -ci true -h true -k true -sn "bump_factor" -ln "bump_factor" -at "double";
	setAttr ".rms" -type "string" "architectural";
	setAttr ".rst" -type "string" "surface";
	setAttr ".rml" -type "string" "E:\\dev\\Autodesk\\maya\\myplugin\\project\\liquid_\\test\\unittest\\er\\surface_shader/architectural.dll";
	setAttr ".rpr" -type "stringArray" 29 "surface_color" "diffuse_color" "diffuse_weight" "specular_color" "specular_weight" "roughness" "specular_mode" "glossiness" "reflection_color" "reflection_weight" "refraction_color" "refraction_weight" "refraction_glossiness" "refraction_thickness" "translucency_color" "translucency_weight" "anisotropy" "rotation" "ior" "fresnel_by_ior" "fresnel_0_degree_refl" "fresnel_90_degree_refl" "fresnel_curve" "is_metal" "diffuse_samples" "reflection_samples" "refraction_samples" "cutoff_threshold" "bump_factor"  ;
	setAttr ".rdt" -type "stringArray" 29 "uniform" "uniform" "uniform" "uniform" "uniform" "uniform" "uniform" "uniform" "uniform" "uniform" "uniform" "uniform" "uniform" "uniform" "uniform" "uniform" "uniform" "uniform" "uniform" "uniform" "uniform" "uniform" "uniform" "uniform" "uniform" "uniform" "uniform" "uniform" "uniform"  ;
	setAttr ".rty" -type "stringArray" 29 "color" "color" "float" "color" "float" "float" "int" "float" "color" "float" "color" "float" "float" "float" "color" "float" "float" "float" "float" "bool" "float" "float" "float" "bool" "int" "int" "int" "float" "float"  ;
	setAttr ".rdf" -type "stringArray" 29 "1.0000001.0000001.000000" "1.0000001.0000001.000000" "1.0" "1.0000001.0000001.000000" "0.2" "0.0" "0" "1.0" "1.0000001.0000001.000000" "0.0" "1.0000001.0000001.000000" "0.0" "0.0" "0.0" "1.0000001.0000001.000000" "0.0" "1.0" "0.0" "1.5" "0" "0.2" "1.0" "5.0" "1" "8" "4" "4" "0.01" "0.3"  ;
	setAttr ".ras" -type "Int32Array" 29 -1 -1 -1 -1 -1 -1
		 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1
		 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 ;
	setAttr ".rlc" -type "stringArray" 28 "liquidAE_LifParameter( \"surface_color\", \"-1\", \"color\", \"color\", \"surface_color\", {\"\"}, \"\", 14 );" "liquidAE_LifParameter( \"diffuse_color\", \"-1\", \"color\", \"color\", \"diffuse_color\", {\"\"}, \"\", 14 );" "liquidAE_LifParameter( \"diffuse_weight\", \"-1\", \"float\", \"slider\", \"diffuse_weight\", {\"0\", \"1\"}, \"\", 14 );" "liquidAE_LifParameter( \"specular_color\", \"-1\", \"color\", \"color\", \"specular_color\", {\"\"}, \"\", 14 );" "liquidAE_LifParameter( \"specular_weight\", \"-1\", \"float\", \"slider\", \"specular_weight\", {\"0\", \"1\"}, \"\", 14 );" "liquidAE_LifParameter( \"roughness\", \"-1\", \"float\", \"float\", \"roughness\", {\"\"}, \"\", 14 );" "liquidAE_LifParameter( \"specular_mode\", \"-1\", \"int\", \"selector\", \"specular_mode\", {\"Ward\", \"0\", \"StretchedPhong\", \"1\", \"Blinn\", \"2\", \"CookTorrance\", \"3\"}, \"\", 14 );" "liquidAE_LifParameter( \"glossiness\", \"-1\", \"float\", \"float\", \"glossiness\", {\"\"}, \"\", 14 );" "liquidAE_LifParameter( \"reflection_color\", \"-1\", \"color\", \"color\", \"reflection_color\", {\"\"}, \"\", 14 );" "liquidAE_LifParameter( \"reflection_weight\", \"-1\", \"float\", \"slider\", \"reflection_weight\", {\"0\", \"1\"}, \"\", 14 );" "liquidAE_LifParameter( \"refraction_color\", \"-1\", \"color\", \"color\", \"refraction_color\", {\"\"}, \"\", 14 );" "liquidAE_LifParameter( \"refraction_weight\", \"-1\", \"float\", \"slider\", \"refraction_weight\", {\"0\", \"1\"}, \"\", 14 );" "liquidAE_LifParameter( \"refraction_glossiness\", \"-1\", \"float\", \"float\", \"refraction_glossiness\", {\"\"}, \"\", 14 );" "liquidAE_LifParameter( \"refraction_thickness\", \"-1\", \"float\", \"float\", \"refraction_thickness\", {\"\"}, \"\", 14 );" "liquidAE_LifParameter( \"translucency_color\", \"-1\", \"color\", \"color\", \"translucency_color\", {\"\"}, \"\", 14 );" "liquidAE_LifParameter( \"translucency_weight\", \"-1\", \"float\", \"slider\", \"translucency_weight\", {\"0\", \"1\"}, \"\", 14 );" "liquidAE_LifParameter( \"anisotropy\", \"-1\", \"float\", \"float\", \"anisotropy\", {\"\"}, \"\", 14 );" "liquidAE_LifParameter( \"rotation\", \"-1\", \"float\", \"float\", \"rotation\", {\"\"}, \"\", 14 );" "liquidAE_LifParameter( \"ior\", \"-1\", \"float\", \"float\", \"ior\", {\"\"}, \"\", 14 );" "liquidAE_LifParameter( \"fresnel_by_ior\", \"-1\", \"bool\", \"bool\", \"fresnel_by_ior\", {\"\"}, \"\", 14 );" "liquidAE_LifParameter( \"fresnel_0_degree_refl\", \"-1\", \"float\", \"slider\", \"fresnel_0_degree_refl\", {\"0\", \"1\"}, \"\", 14 );" "liquidAE_LifParameter( \"fresnel_90_degree_refl\", \"-1\", \"float\", \"slider\", \"fresnel_90_degree_refl\", {\"0\", \"1\"}, \"\", 14 );" "liquidAE_LifParameter( \"fresnel_curve\", \"-1\", \"float\", \"float\", \"fresnel_curve\", {\"\"}, \"\", 14 );" "liquidAE_LifParameter( \"is_metal\", \"-1\", \"bool\", \"bool\", \"is_metal\", {\"\"}, \"\", 14 );" "liquidAE_LifParameter( \"diffuse_samples\", \"-1\", \"int\", \"int\", \"diffuse_samples\", {\"\"}, \"\", 14 );" "liquidAE_LifParameter( \"reflection_samples\", \"-1\", \"int\", \"int\", \"reflection_samples\", {\"\"}, \"\", 14 );" "liquidAE_LifParameter( \"refraction_samples\", \"-1\", \"int\", \"int\", \"refraction_samples\", {\"\"}, \"\", 14 );" "liquidAE_LifParameter( \"cutoff_threshold\", \"-1\", \"float\", \"float\", \"cutoff_threshold\", {\"\"}, \"\", 14 );"  ;
	setAttr ".rmt" -type "stringArray" 0  ;
	setAttr ".rio" -type "Int32Array" 29 0 0 0 0 0 0
		 0 0 0 0 0 0 0 0 0 0 0 0
		 0 0 0 0 0 0 0 0 0 0 0 ;
	setAttr ".rma" -type "stringArray" 29 "" "" "" "" "" "" "" "" "" "" "" "" "" "" "" "" "" "" "" "" "" "" "" "" "" "" "" "" ""  ;
	setAttr ".surface_color" -type "float3" 1 0 0 ;
	setAttr ".diffuse_color" -type "float3" 1 1 1 ;
	setAttr ".diffuse_weight" 1;
	setAttr ".specular_color" -type "float3" 1 1 1 ;
	setAttr ".specular_weight" 0.2;
	setAttr ".glossiness" 1;
	setAttr ".reflection_color" -type "float3" 1 1 1 ;
	setAttr ".refraction_color" -type "float3" 1 1 1 ;
	setAttr ".refraction_thickness" 1;
	setAttr ".translucency_color" -type "float3" 1 1 1 ;
	setAttr ".anisotropy" 1;
	setAttr ".ior" 1.5;
	setAttr ".fresnel_by_ior" 1;
	setAttr ".fresnel_0_degree_refl" 0.2;
	setAttr ".fresnel_90_degree_refl" 1;
	setAttr ".fresnel_curve" 5;
	setAttr ".is_metal" 1;
	setAttr ".diffuse_samples" 8;
	setAttr ".reflection_samples" 4;
	setAttr ".refraction_samples" 4;
	setAttr ".cutoff_threshold" 0.01;
	setAttr ".rmanParamLifCmdId" -type "Int32Array" 29 0 1 2 3
		 4 5 6 7 8 9 10 11 12 13 14 15
		 16 17 18 19 20 21 22 23 24 25 26 27
		 0 ;
	setAttr ".rmanGroups" -type "stringArray" 0  ;
	setAttr ".rmanGroupLifCmdId" -type "Int32Array" 0 ;
	setAttr -k on ".bump_factor" 0.3;
createNode shadingEngine -n "liquidShader1SG";
	setAttr ".ihi" 0;
	setAttr ".ro" yes;
createNode materialInfo -n "materialInfo5";
createNode liquidSurface -n "liquidSurface1";
	addAttr -ci true -uac -h true -sn "surface_color" -ln "surface_color" -at "float3" 
		-nc 3;
	addAttr -ci true -h true -sn "surface_colorR" -ln "surface_colorR" -at "float" -p "surface_color";
	addAttr -ci true -h true -sn "surface_colorG" -ln "surface_colorG" -at "float" -p "surface_color";
	addAttr -ci true -h true -sn "surface_colorB" -ln "surface_colorB" -at "float" -p "surface_color";
	addAttr -ci true -uac -h true -sn "diffuse_color" -ln "diffuse_color" -at "float3" 
		-nc 3;
	addAttr -ci true -h true -sn "diffuse_colorR" -ln "diffuse_colorR" -at "float" -p "diffuse_color";
	addAttr -ci true -h true -sn "diffuse_colorG" -ln "diffuse_colorG" -at "float" -p "diffuse_color";
	addAttr -ci true -h true -sn "diffuse_colorB" -ln "diffuse_colorB" -at "float" -p "diffuse_color";
	addAttr -ci true -h true -sn "diffuse_weight" -ln "diffuse_weight" -at "double";
	addAttr -ci true -uac -h true -sn "specular_color" -ln "specular_color" -at "float3" 
		-nc 3;
	addAttr -ci true -h true -sn "specular_colorR" -ln "specular_colorR" -at "float" 
		-p "specular_color";
	addAttr -ci true -h true -sn "specular_colorG" -ln "specular_colorG" -at "float" 
		-p "specular_color";
	addAttr -ci true -h true -sn "specular_colorB" -ln "specular_colorB" -at "float" 
		-p "specular_color";
	addAttr -ci true -h true -sn "specular_weight" -ln "specular_weight" -at "double";
	addAttr -ci true -h true -sn "roughness" -ln "roughness" -at "double";
	addAttr -ci true -h true -sn "specular_mode" -ln "specular_mode" -at "double";
	addAttr -ci true -h true -sn "glossiness" -ln "glossiness" -at "double";
	addAttr -ci true -uac -h true -sn "reflection_color" -ln "reflection_color" -at "float3" 
		-nc 3;
	addAttr -ci true -h true -sn "reflection_colorR" -ln "reflection_colorR" -at "float" 
		-p "reflection_color";
	addAttr -ci true -h true -sn "reflection_colorG" -ln "reflection_colorG" -at "float" 
		-p "reflection_color";
	addAttr -ci true -h true -sn "reflection_colorB" -ln "reflection_colorB" -at "float" 
		-p "reflection_color";
	addAttr -ci true -h true -sn "reflection_weight" -ln "reflection_weight" -at "double";
	addAttr -ci true -uac -h true -sn "refraction_color" -ln "refraction_color" -at "float3" 
		-nc 3;
	addAttr -ci true -h true -sn "refraction_colorR" -ln "refraction_colorR" -at "float" 
		-p "refraction_color";
	addAttr -ci true -h true -sn "refraction_colorG" -ln "refraction_colorG" -at "float" 
		-p "refraction_color";
	addAttr -ci true -h true -sn "refraction_colorB" -ln "refraction_colorB" -at "float" 
		-p "refraction_color";
	addAttr -ci true -h true -sn "refraction_weight" -ln "refraction_weight" -at "double";
	addAttr -ci true -h true -sn "refraction_glossiness" -ln "refraction_glossiness" 
		-at "double";
	addAttr -ci true -h true -sn "refraction_thickness" -ln "refraction_thickness" -at "double";
	addAttr -ci true -uac -h true -sn "translucency_color" -ln "translucency_color" 
		-at "float3" -nc 3;
	addAttr -ci true -h true -sn "translucency_colorR" -ln "translucency_colorR" -at "float" 
		-p "translucency_color";
	addAttr -ci true -h true -sn "translucency_colorG" -ln "translucency_colorG" -at "float" 
		-p "translucency_color";
	addAttr -ci true -h true -sn "translucency_colorB" -ln "translucency_colorB" -at "float" 
		-p "translucency_color";
	addAttr -ci true -h true -sn "translucency_weight" -ln "translucency_weight" -at "double";
	addAttr -ci true -h true -sn "anisotropy" -ln "anisotropy" -at "double";
	addAttr -ci true -h true -sn "rotation" -ln "rotation" -at "double";
	addAttr -ci true -h true -sn "ior" -ln "ior" -at "double";
	addAttr -ci true -h true -sn "fresnel_by_ior" -ln "fresnel_by_ior" -at "double";
	addAttr -ci true -h true -sn "fresnel_0_degree_refl" -ln "fresnel_0_degree_refl" 
		-at "double";
	addAttr -ci true -h true -sn "fresnel_90_degree_refl" -ln "fresnel_90_degree_refl" 
		-at "double";
	addAttr -ci true -h true -sn "fresnel_curve" -ln "fresnel_curve" -at "double";
	addAttr -ci true -h true -sn "is_metal" -ln "is_metal" -at "double";
	addAttr -ci true -h true -sn "diffuse_samples" -ln "diffuse_samples" -at "double";
	addAttr -ci true -h true -sn "reflection_samples" -ln "reflection_samples" -at "double";
	addAttr -ci true -h true -sn "refraction_samples" -ln "refraction_samples" -at "double";
	addAttr -ci true -h true -sn "cutoff_threshold" -ln "cutoff_threshold" -at "double";
	addAttr -ci true -h true -sn "rmanParamLifCmdId" -ln "rmanParamLifCmdId" -dt "Int32Array";
	addAttr -ci true -h true -sn "rmanGroups" -ln "rmanGroups" -dt "stringArray";
	addAttr -ci true -h true -sn "rmanGroupLifCmdId" -ln "rmanGroupLifCmdId" -dt "Int32Array";
	addAttr -ci true -sn "rmanLifCtrls" -ln "rmanLifCtrls" -dt "stringArray";
	addAttr -ci true -sn "liqAssignedObjects" -ln "liqAssignedObjects" -at "message";
	addAttr -ci true -h true -k true -sn "bump_factor" -ln "bump_factor" -at "double";
	setAttr ".rms" -type "string" "architectural";
	setAttr ".rst" -type "string" "surface";
	setAttr ".rml" -type "string" "E:\\dev\\Autodesk\\maya\\myplugin\\project\\liquid_\\test\\unittest\\er\\surface_shader/architectural.dll";
	setAttr ".rpr" -type "stringArray" 29 "surface_color" "diffuse_color" "diffuse_weight" "specular_color" "specular_weight" "roughness" "specular_mode" "glossiness" "reflection_color" "reflection_weight" "refraction_color" "refraction_weight" "refraction_glossiness" "refraction_thickness" "translucency_color" "translucency_weight" "anisotropy" "rotation" "ior" "fresnel_by_ior" "fresnel_0_degree_refl" "fresnel_90_degree_refl" "fresnel_curve" "is_metal" "diffuse_samples" "reflection_samples" "refraction_samples" "cutoff_threshold" "bump_factor"  ;
	setAttr ".rdt" -type "stringArray" 29 "uniform" "uniform" "uniform" "uniform" "uniform" "uniform" "uniform" "uniform" "uniform" "uniform" "uniform" "uniform" "uniform" "uniform" "uniform" "uniform" "uniform" "uniform" "uniform" "uniform" "uniform" "uniform" "uniform" "uniform" "uniform" "uniform" "uniform" "uniform" "uniform"  ;
	setAttr ".rty" -type "stringArray" 29 "color" "color" "float" "color" "float" "float" "int" "float" "color" "float" "color" "float" "float" "float" "color" "float" "float" "float" "float" "bool" "float" "float" "float" "bool" "int" "int" "int" "float" "float"  ;
	setAttr ".rdf" -type "stringArray" 29 "1.0000001.0000001.000000" "1.0000001.0000001.000000" "1.0" "1.0000001.0000001.000000" "0.2" "0.0" "0" "1.0" "1.0000001.0000001.000000" "0.0" "1.0000001.0000001.000000" "0.0" "0.0" "0.0" "1.0000001.0000001.000000" "0.0" "1.0" "0.0" "1.5" "0" "0.2" "1.0" "5.0" "1" "8" "4" "4" "0.01" "0.3"  ;
	setAttr ".ras" -type "Int32Array" 29 -1 -1 -1 -1 -1 -1
		 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1
		 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 ;
	setAttr ".rlc" -type "stringArray" 28 "liquidAE_LifParameter( \"surface_color\", \"-1\", \"color\", \"color\", \"surface_color\", {\"\"}, \"\", 14 );" "liquidAE_LifParameter( \"diffuse_color\", \"-1\", \"color\", \"color\", \"diffuse_color\", {\"\"}, \"\", 14 );" "liquidAE_LifParameter( \"diffuse_weight\", \"-1\", \"float\", \"slider\", \"diffuse_weight\", {\"0\", \"1\"}, \"\", 14 );" "liquidAE_LifParameter( \"specular_color\", \"-1\", \"color\", \"color\", \"specular_color\", {\"\"}, \"\", 14 );" "liquidAE_LifParameter( \"specular_weight\", \"-1\", \"float\", \"slider\", \"specular_weight\", {\"0\", \"1\"}, \"\", 14 );" "liquidAE_LifParameter( \"roughness\", \"-1\", \"float\", \"float\", \"roughness\", {\"\"}, \"\", 14 );" "liquidAE_LifParameter( \"specular_mode\", \"-1\", \"int\", \"selector\", \"specular_mode\", {\"Ward\", \"0\", \"StretchedPhong\", \"1\", \"Blinn\", \"2\", \"CookTorrance\", \"3\"}, \"\", 14 );" "liquidAE_LifParameter( \"glossiness\", \"-1\", \"float\", \"float\", \"glossiness\", {\"\"}, \"\", 14 );" "liquidAE_LifParameter( \"reflection_color\", \"-1\", \"color\", \"color\", \"reflection_color\", {\"\"}, \"\", 14 );" "liquidAE_LifParameter( \"reflection_weight\", \"-1\", \"float\", \"slider\", \"reflection_weight\", {\"0\", \"1\"}, \"\", 14 );" "liquidAE_LifParameter( \"refraction_color\", \"-1\", \"color\", \"color\", \"refraction_color\", {\"\"}, \"\", 14 );" "liquidAE_LifParameter( \"refraction_weight\", \"-1\", \"float\", \"slider\", \"refraction_weight\", {\"0\", \"1\"}, \"\", 14 );" "liquidAE_LifParameter( \"refraction_glossiness\", \"-1\", \"float\", \"float\", \"refraction_glossiness\", {\"\"}, \"\", 14 );" "liquidAE_LifParameter( \"refraction_thickness\", \"-1\", \"float\", \"float\", \"refraction_thickness\", {\"\"}, \"\", 14 );" "liquidAE_LifParameter( \"translucency_color\", \"-1\", \"color\", \"color\", \"translucency_color\", {\"\"}, \"\", 14 );" "liquidAE_LifParameter( \"translucency_weight\", \"-1\", \"float\", \"slider\", \"translucency_weight\", {\"0\", \"1\"}, \"\", 14 );" "liquidAE_LifParameter( \"anisotropy\", \"-1\", \"float\", \"float\", \"anisotropy\", {\"\"}, \"\", 14 );" "liquidAE_LifParameter( \"rotation\", \"-1\", \"float\", \"float\", \"rotation\", {\"\"}, \"\", 14 );" "liquidAE_LifParameter( \"ior\", \"-1\", \"float\", \"float\", \"ior\", {\"\"}, \"\", 14 );" "liquidAE_LifParameter( \"fresnel_by_ior\", \"-1\", \"bool\", \"bool\", \"fresnel_by_ior\", {\"\"}, \"\", 14 );" "liquidAE_LifParameter( \"fresnel_0_degree_refl\", \"-1\", \"float\", \"slider\", \"fresnel_0_degree_refl\", {\"0\", \"1\"}, \"\", 14 );" "liquidAE_LifParameter( \"fresnel_90_degree_refl\", \"-1\", \"float\", \"slider\", \"fresnel_90_degree_refl\", {\"0\", \"1\"}, \"\", 14 );" "liquidAE_LifParameter( \"fresnel_curve\", \"-1\", \"float\", \"float\", \"fresnel_curve\", {\"\"}, \"\", 14 );" "liquidAE_LifParameter( \"is_metal\", \"-1\", \"bool\", \"bool\", \"is_metal\", {\"\"}, \"\", 14 );" "liquidAE_LifParameter( \"diffuse_samples\", \"-1\", \"int\", \"int\", \"diffuse_samples\", {\"\"}, \"\", 14 );" "liquidAE_LifParameter( \"reflection_samples\", \"-1\", \"int\", \"int\", \"reflection_samples\", {\"\"}, \"\", 14 );" "liquidAE_LifParameter( \"refraction_samples\", \"-1\", \"int\", \"int\", \"refraction_samples\", {\"\"}, \"\", 14 );" "liquidAE_LifParameter( \"cutoff_threshold\", \"-1\", \"float\", \"float\", \"cutoff_threshold\", {\"\"}, \"\", 14 );"  ;
	setAttr ".rmt" -type "stringArray" 0  ;
	setAttr ".rio" -type "Int32Array" 29 0 0 0 0 0 0
		 0 0 0 0 0 0 0 0 0 0 0 0
		 0 0 0 0 0 0 0 0 0 0 0 ;
	setAttr ".rma" -type "stringArray" 29 "" "" "" "" "" "" "" "" "" "" "" "" "" "" "" "" "" "" "" "" "" "" "" "" "" "" "" "" ""  ;
	setAttr ".surface_color" -type "float3" 1 1 0 ;
	setAttr ".diffuse_color" -type "float3" 1 1 1 ;
	setAttr ".diffuse_weight" 1;
	setAttr ".specular_color" -type "float3" 1 1 1 ;
	setAttr ".specular_weight" 0.2;
	setAttr ".glossiness" 1;
	setAttr ".reflection_color" -type "float3" 1 1 1 ;
	setAttr ".refraction_color" -type "float3" 1 1 1 ;
	setAttr ".translucency_color" -type "float3" 1 1 1 ;
	setAttr ".anisotropy" 1;
	setAttr ".ior" 1.5;
	setAttr ".fresnel_by_ior" 1;
	setAttr ".fresnel_0_degree_refl" 0.2;
	setAttr ".fresnel_90_degree_refl" 1;
	setAttr ".fresnel_curve" 5;
	setAttr ".is_metal" 1;
	setAttr ".diffuse_samples" 8;
	setAttr ".reflection_samples" 4;
	setAttr ".refraction_samples" 4;
	setAttr ".cutoff_threshold" 0.01;
	setAttr ".rmanParamLifCmdId" -type "Int32Array" 29 0 1 2 3
		 4 5 6 7 8 9 10 11 12 13 14 15
		 16 17 18 19 20 21 22 23 24 25 26 27
		 0 ;
	setAttr ".rmanGroups" -type "stringArray" 0  ;
	setAttr ".rmanGroupLifCmdId" -type "Int32Array" 0 ;
	setAttr -k on ".bump_factor" 0.3;
createNode shadingEngine -n "liquidSurface1SG";
	setAttr ".ihi" 0;
	setAttr ".ro" yes;
createNode materialInfo -n "materialInfo6";
createNode polyCube -n "polyCube1";
	setAttr ".w" 4.1433415618246254;
	setAttr ".h" 5.5949980747668429;
	setAttr ".d" 7.3627088982102036;
	setAttr ".cuv" 4;
createNode liquidShader -n "liquidShader2";
	addAttr -ci true -uac -h true -sn "surface_color" -ln "surface_color" -at "float3" 
		-nc 3;
	addAttr -ci true -h true -sn "surface_colorR" -ln "surface_colorR" -at "float" -p "surface_color";
	addAttr -ci true -h true -sn "surface_colorG" -ln "surface_colorG" -at "float" -p "surface_color";
	addAttr -ci true -h true -sn "surface_colorB" -ln "surface_colorB" -at "float" -p "surface_color";
	addAttr -ci true -uac -h true -sn "diffuse_color" -ln "diffuse_color" -at "float3" 
		-nc 3;
	addAttr -ci true -h true -sn "diffuse_colorR" -ln "diffuse_colorR" -at "float" -p "diffuse_color";
	addAttr -ci true -h true -sn "diffuse_colorG" -ln "diffuse_colorG" -at "float" -p "diffuse_color";
	addAttr -ci true -h true -sn "diffuse_colorB" -ln "diffuse_colorB" -at "float" -p "diffuse_color";
	addAttr -ci true -h true -sn "diffuse_weight" -ln "diffuse_weight" -at "double";
	addAttr -ci true -uac -h true -sn "specular_color" -ln "specular_color" -at "float3" 
		-nc 3;
	addAttr -ci true -h true -sn "specular_colorR" -ln "specular_colorR" -at "float" 
		-p "specular_color";
	addAttr -ci true -h true -sn "specular_colorG" -ln "specular_colorG" -at "float" 
		-p "specular_color";
	addAttr -ci true -h true -sn "specular_colorB" -ln "specular_colorB" -at "float" 
		-p "specular_color";
	addAttr -ci true -h true -sn "specular_weight" -ln "specular_weight" -at "double";
	addAttr -ci true -h true -sn "roughness" -ln "roughness" -at "double";
	addAttr -ci true -h true -sn "specular_mode" -ln "specular_mode" -at "double";
	addAttr -ci true -h true -sn "glossiness" -ln "glossiness" -at "double";
	addAttr -ci true -uac -h true -sn "reflection_color" -ln "reflection_color" -at "float3" 
		-nc 3;
	addAttr -ci true -h true -sn "reflection_colorR" -ln "reflection_colorR" -at "float" 
		-p "reflection_color";
	addAttr -ci true -h true -sn "reflection_colorG" -ln "reflection_colorG" -at "float" 
		-p "reflection_color";
	addAttr -ci true -h true -sn "reflection_colorB" -ln "reflection_colorB" -at "float" 
		-p "reflection_color";
	addAttr -ci true -h true -sn "reflection_weight" -ln "reflection_weight" -at "double";
	addAttr -ci true -uac -h true -sn "refraction_color" -ln "refraction_color" -at "float3" 
		-nc 3;
	addAttr -ci true -h true -sn "refraction_colorR" -ln "refraction_colorR" -at "float" 
		-p "refraction_color";
	addAttr -ci true -h true -sn "refraction_colorG" -ln "refraction_colorG" -at "float" 
		-p "refraction_color";
	addAttr -ci true -h true -sn "refraction_colorB" -ln "refraction_colorB" -at "float" 
		-p "refraction_color";
	addAttr -ci true -h true -sn "refraction_weight" -ln "refraction_weight" -at "double";
	addAttr -ci true -h true -sn "refraction_glossiness" -ln "refraction_glossiness" 
		-at "double";
	addAttr -ci true -h true -sn "refraction_thickness" -ln "refraction_thickness" -at "double";
	addAttr -ci true -uac -h true -sn "translucency_color" -ln "translucency_color" 
		-at "float3" -nc 3;
	addAttr -ci true -h true -sn "translucency_colorR" -ln "translucency_colorR" -at "float" 
		-p "translucency_color";
	addAttr -ci true -h true -sn "translucency_colorG" -ln "translucency_colorG" -at "float" 
		-p "translucency_color";
	addAttr -ci true -h true -sn "translucency_colorB" -ln "translucency_colorB" -at "float" 
		-p "translucency_color";
	addAttr -ci true -h true -sn "translucency_weight" -ln "translucency_weight" -at "double";
	addAttr -ci true -h true -sn "anisotropy" -ln "anisotropy" -at "double";
	addAttr -ci true -h true -sn "rotation" -ln "rotation" -at "double";
	addAttr -ci true -h true -sn "ior" -ln "ior" -at "double";
	addAttr -ci true -h true -sn "fresnel_by_ior" -ln "fresnel_by_ior" -at "double";
	addAttr -ci true -h true -sn "fresnel_0_degree_refl" -ln "fresnel_0_degree_refl" 
		-at "double";
	addAttr -ci true -h true -sn "fresnel_90_degree_refl" -ln "fresnel_90_degree_refl" 
		-at "double";
	addAttr -ci true -h true -sn "fresnel_curve" -ln "fresnel_curve" -at "double";
	addAttr -ci true -h true -sn "is_metal" -ln "is_metal" -at "double";
	addAttr -ci true -h true -sn "diffuse_samples" -ln "diffuse_samples" -at "double";
	addAttr -ci true -h true -sn "reflection_samples" -ln "reflection_samples" -at "double";
	addAttr -ci true -h true -sn "refraction_samples" -ln "refraction_samples" -at "double";
	addAttr -ci true -h true -sn "cutoff_threshold" -ln "cutoff_threshold" -at "double";
	addAttr -ci true -h true -k true -sn "bump_factor" -ln "bump_factor" -at "double";
	addAttr -ci true -h true -sn "rmanParamLifCmdId" -ln "rmanParamLifCmdId" -dt "Int32Array";
	addAttr -ci true -h true -sn "rmanGroups" -ln "rmanGroups" -dt "stringArray";
	addAttr -ci true -h true -sn "rmanGroupLifCmdId" -ln "rmanGroupLifCmdId" -dt "Int32Array";
	addAttr -ci true -sn "rmanLifCtrls" -ln "rmanLifCtrls" -dt "stringArray";
	addAttr -ci true -sn "liqAssignedObjects" -ln "liqAssignedObjects" -at "message";
	setAttr ".rms" -type "string" "architectural";
	setAttr ".rst" -type "string" "surface";
	setAttr ".rml" -type "string" "E:\\dev\\Autodesk\\maya\\myplugin\\project\\liquid_\\test\\unittest\\er\\surface_shader/architectural.dll";
	setAttr ".rpr" -type "stringArray" 29 "surface_color" "diffuse_color" "diffuse_weight" "specular_color" "specular_weight" "roughness" "specular_mode" "glossiness" "reflection_color" "reflection_weight" "refraction_color" "refraction_weight" "refraction_glossiness" "refraction_thickness" "translucency_color" "translucency_weight" "anisotropy" "rotation" "ior" "fresnel_by_ior" "fresnel_0_degree_refl" "fresnel_90_degree_refl" "fresnel_curve" "is_metal" "diffuse_samples" "reflection_samples" "refraction_samples" "cutoff_threshold" "bump_factor"  ;
	setAttr ".rdt" -type "stringArray" 29 "uniform" "uniform" "uniform" "uniform" "uniform" "uniform" "uniform" "uniform" "uniform" "uniform" "uniform" "uniform" "uniform" "uniform" "uniform" "uniform" "uniform" "uniform" "uniform" "uniform" "uniform" "uniform" "uniform" "uniform" "uniform" "uniform" "uniform" "uniform" "uniform"  ;
	setAttr ".rty" -type "stringArray" 29 "color" "color" "float" "color" "float" "float" "int" "float" "color" "float" "color" "float" "float" "float" "color" "float" "float" "float" "float" "bool" "float" "float" "float" "bool" "int" "int" "int" "float" "float"  ;
	setAttr ".rdf" -type "stringArray" 29 "1.0000001.0000001.000000" "1.0000001.0000001.000000" "1.0" "1.0000001.0000001.000000" "0.2" "0.0" "0" "1.0" "1.0000001.0000001.000000" "0.0" "1.0000001.0000001.000000" "0.0" "0.0" "0.0" "1.0000001.0000001.000000" "0.0" "1.0" "0.0" "1.5" "0" "0.2" "1.0" "5.0" "1" "8" "4" "4" "0.01" "0.3"  ;
	setAttr ".ras" -type "Int32Array" 29 -1 -1 -1 -1 -1 -1
		 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1
		 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 ;
	setAttr ".rlc" -type "stringArray" 28 "liquidAE_LifParameter( \"surface_color\", \"-1\", \"color\", \"color\", \"surface_color\", {\"\"}, \"\", 14 );" "liquidAE_LifParameter( \"diffuse_color\", \"-1\", \"color\", \"color\", \"diffuse_color\", {\"\"}, \"\", 14 );" "liquidAE_LifParameter( \"diffuse_weight\", \"-1\", \"float\", \"slider\", \"diffuse_weight\", {\"0\", \"1\"}, \"\", 14 );" "liquidAE_LifParameter( \"specular_color\", \"-1\", \"color\", \"color\", \"specular_color\", {\"\"}, \"\", 14 );" "liquidAE_LifParameter( \"specular_weight\", \"-1\", \"float\", \"slider\", \"specular_weight\", {\"0\", \"1\"}, \"\", 14 );" "liquidAE_LifParameter( \"roughness\", \"-1\", \"float\", \"float\", \"roughness\", {\"\"}, \"\", 14 );" "liquidAE_LifParameter( \"specular_mode\", \"-1\", \"int\", \"selector\", \"specular_mode\", {\"Ward\", \"0\", \"StretchedPhong\", \"1\", \"Blinn\", \"2\", \"CookTorrance\", \"3\"}, \"\", 14 );" "liquidAE_LifParameter( \"glossiness\", \"-1\", \"float\", \"float\", \"glossiness\", {\"\"}, \"\", 14 );" "liquidAE_LifParameter( \"reflection_color\", \"-1\", \"color\", \"color\", \"reflection_color\", {\"\"}, \"\", 14 );" "liquidAE_LifParameter( \"reflection_weight\", \"-1\", \"float\", \"slider\", \"reflection_weight\", {\"0\", \"1\"}, \"\", 14 );" "liquidAE_LifParameter( \"refraction_color\", \"-1\", \"color\", \"color\", \"refraction_color\", {\"\"}, \"\", 14 );" "liquidAE_LifParameter( \"refraction_weight\", \"-1\", \"float\", \"slider\", \"refraction_weight\", {\"0\", \"1\"}, \"\", 14 );" "liquidAE_LifParameter( \"refraction_glossiness\", \"-1\", \"float\", \"float\", \"refraction_glossiness\", {\"\"}, \"\", 14 );" "liquidAE_LifParameter( \"refraction_thickness\", \"-1\", \"float\", \"float\", \"refraction_thickness\", {\"\"}, \"\", 14 );" "liquidAE_LifParameter( \"translucency_color\", \"-1\", \"color\", \"color\", \"translucency_color\", {\"\"}, \"\", 14 );" "liquidAE_LifParameter( \"translucency_weight\", \"-1\", \"float\", \"slider\", \"translucency_weight\", {\"0\", \"1\"}, \"\", 14 );" "liquidAE_LifParameter( \"anisotropy\", \"-1\", \"float\", \"float\", \"anisotropy\", {\"\"}, \"\", 14 );" "liquidAE_LifParameter( \"rotation\", \"-1\", \"float\", \"float\", \"rotation\", {\"\"}, \"\", 14 );" "liquidAE_LifParameter( \"ior\", \"-1\", \"float\", \"float\", \"ior\", {\"\"}, \"\", 14 );" "liquidAE_LifParameter( \"fresnel_by_ior\", \"-1\", \"bool\", \"bool\", \"fresnel_by_ior\", {\"\"}, \"\", 14 );" "liquidAE_LifParameter( \"fresnel_0_degree_refl\", \"-1\", \"float\", \"slider\", \"fresnel_0_degree_refl\", {\"0\", \"1\"}, \"\", 14 );" "liquidAE_LifParameter( \"fresnel_90_degree_refl\", \"-1\", \"float\", \"slider\", \"fresnel_90_degree_refl\", {\"0\", \"1\"}, \"\", 14 );" "liquidAE_LifParameter( \"fresnel_curve\", \"-1\", \"float\", \"float\", \"fresnel_curve\", {\"\"}, \"\", 14 );" "liquidAE_LifParameter( \"is_metal\", \"-1\", \"bool\", \"bool\", \"is_metal\", {\"\"}, \"\", 14 );" "liquidAE_LifParameter( \"diffuse_samples\", \"-1\", \"int\", \"int\", \"diffuse_samples\", {\"\"}, \"\", 14 );" "liquidAE_LifParameter( \"reflection_samples\", \"-1\", \"int\", \"int\", \"reflection_samples\", {\"\"}, \"\", 14 );" "liquidAE_LifParameter( \"refraction_samples\", \"-1\", \"int\", \"int\", \"refraction_samples\", {\"\"}, \"\", 14 );" "liquidAE_LifParameter( \"cutoff_threshold\", \"-1\", \"float\", \"float\", \"cutoff_threshold\", {\"\"}, \"\", 14 );"  ;
	setAttr ".rmt" -type "stringArray" 0  ;
	setAttr ".rio" -type "Int32Array" 29 0 0 0 0 0 0
		 0 0 0 0 0 0 0 0 0 0 0 0
		 0 0 0 0 0 0 0 0 0 0 0 ;
	setAttr ".rma" -type "stringArray" 29 "" "" "" "" "" "" "" "" "" "" "" "" "" "" "" "" "" "" "" "" "" "" "" "" "" "" "" "" ""  ;
	setAttr ".surface_color" -type "float3" 0 1 0.96257091 ;
	setAttr ".diffuse_color" -type "float3" 1 1 1 ;
	setAttr ".diffuse_weight" 1;
	setAttr ".specular_color" -type "float3" 1 1 1 ;
	setAttr ".specular_weight" 0.2;
	setAttr ".glossiness" 1;
	setAttr ".reflection_color" -type "float3" 1 1 1 ;
	setAttr ".refraction_color" -type "float3" 1 1 1 ;
	setAttr ".refraction_thickness" 2;
	setAttr ".translucency_color" -type "float3" 1 1 1 ;
	setAttr ".anisotropy" 1;
	setAttr ".ior" 1.5;
	setAttr ".fresnel_by_ior" 1;
	setAttr ".fresnel_0_degree_refl" 0.2;
	setAttr ".fresnel_90_degree_refl" 1;
	setAttr ".fresnel_curve" 5;
	setAttr ".is_metal" 1;
	setAttr ".diffuse_samples" 8;
	setAttr ".reflection_samples" 4;
	setAttr ".refraction_samples" 4;
	setAttr ".cutoff_threshold" 0.01;
	setAttr -k on ".bump_factor" 0.3;
	setAttr ".rmanParamLifCmdId" -type "Int32Array" 29 0 1 2 3
		 4 5 6 7 8 9 10 11 12 13 14 15
		 16 17 18 19 20 21 22 23 24 25 26 27
		 0 ;
	setAttr ".rmanGroups" -type "stringArray" 0  ;
	setAttr ".rmanGroupLifCmdId" -type "Int32Array" 0 ;
createNode shadingEngine -n "liquidShader2SG";
	setAttr ".ihi" 0;
	setAttr ".ro" yes;
createNode materialInfo -n "materialInfo7";
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
createNode liqGlobalsNodeRenderer -n "liqGlobalsNodeRenderer_elvishray";
	addAttr -ci true -h true -sn "verbose" -ln "verbose" -dv 6 -at "long";
	addAttr -ci true -h true -sn "link" -ln "link" -dt "string";
	addAttr -ci true -h true -sn "contrast" -ln "contrast" -dt "string";
	addAttr -ci true -h true -sn "samples" -ln "samples" -dt "string";
	addAttr -ci true -h true -sn "filterType" -ln "filterType" -dv 4 -at "long";
	addAttr -ci true -h true -sn "filterSize" -ln "filterSize" -dv 3 -at "float";
	addAttr -ci true -h true -sn "trace_depth_transp" -ln "trace_depth_transp" -dv 4 
		-at "long";
	addAttr -ci true -h true -sn "trace_depth_glossy_reflect" -ln "trace_depth_glossy_reflect" 
		-dv 4 -at "long";
	addAttr -ci true -h true -sn "trace_depth_diffuse_reflect" -ln "trace_depth_diffuse_reflect" 
		-dv 4 -at "long";
	addAttr -ci true -h true -sn "trace_depth_glossy_refract" -ln "trace_depth_glossy_refract" 
		-dv 4 -at "long";
	addAttr -ci true -h true -sn "trace_depth_diffuse_refract" -ln "trace_depth_diffuse_refract" 
		-dv 4 -at "long";
	addAttr -ci true -h true -sn "trace_depth_sum" -ln "trace_depth_sum" -dv 4 -at "long";
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
		65536 -at "long";
	addAttr -ci true -h true -sn "approx_motion_factor" -ln "approx_motion_factor" -dv 
		16 -at "float";
	setAttr ".link" -type "string" "eiIMG|eiSHADER|eiSHADER_maya";
	setAttr ".contrast" -type "string" "0.05|0.05|0.05|0.05";
	setAttr ".samples" -type "string" "0|2";
	setAttr ".approx_args" -type "string" "0|0|0|0";
createNode elvishrayGlobals -n "elvishrayGlobals1";
	setAttr ".esa" 1;
	setAttr ".ess" 1;
createNode ElvishRenderGlobals -n "ElvishRenderGlobals1";
	setAttr ".esa" yes;
	setAttr ".ess" yes;
select -ne :time1;
	setAttr -av -k on ".cch";
	setAttr -cb on ".ihi";
	setAttr -k on ".nds";
	setAttr -cb on ".bnm";
	setAttr -k on ".o" 1;
	setAttr -av ".unw" 1;
select -ne :renderPartition;
	setAttr -k on ".cch";
	setAttr -cb on ".ihi";
	setAttr -k on ".nds";
	setAttr -cb on ".bnm";
	setAttr -s 9 ".st";
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
	setAttr -cb on ".mimt";
	setAttr -cb on ".miop";
	setAttr -cb on ".mise";
	setAttr -cb on ".mism";
	setAttr -cb on ".mice";
	setAttr -av ".micc";
	setAttr -cb on ".mica";
	setAttr -cb on ".micw";
	setAttr -cb on ".mirw";
select -ne :initialParticleSE;
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
	setAttr -cb on ".mimt";
	setAttr -cb on ".miop";
	setAttr -cb on ".mise";
	setAttr -cb on ".mism";
	setAttr -cb on ".mice";
	setAttr -cb on ".micc";
	setAttr -cb on ".mica";
	setAttr -cb on ".micw";
	setAttr -cb on ".mirw";
select -ne :defaultShaderList1;
	setAttr -k on ".cch";
	setAttr -cb on ".ihi";
	setAttr -k on ".nds";
	setAttr -cb on ".bnm";
	setAttr -s 5 ".s";
select -ne :lightList1;
	setAttr -k on ".cch";
	setAttr -cb on ".ihi";
	setAttr -k on ".nds";
	setAttr -cb on ".bnm";
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
	setAttr -cb on ".ihi";
	setAttr -k on ".nds";
	setAttr -cb on ".bnm";
	setAttr -k on ".macc";
	setAttr -k on ".macd";
	setAttr -k on ".macq";
	setAttr -cb on ".ifg";
	setAttr -k on ".clip";
	setAttr -k on ".edm";
	setAttr -k on ".edl";
	setAttr ".ren" -type "string" "liquid";
	setAttr -av -k on ".esr";
	setAttr -k on ".ors";
	setAttr -cb on ".sdf";
	setAttr -k on ".outf" 20;
	setAttr -k on ".gama";
	setAttr -cb on ".ar";
	setAttr -av ".bfs";
	setAttr -cb on ".me";
	setAttr -cb on ".se";
	setAttr -k on ".be";
	setAttr -cb on ".ep";
	setAttr -k on ".fec";
	setAttr -k on ".ofc";
	setAttr -cb on ".ofe";
	setAttr -cb on ".efe";
	setAttr -cb on ".oft";
	setAttr -cb on ".umfn";
	setAttr -cb on ".ufe";
	setAttr -cb on ".peie";
	setAttr ".ifp" -type "string" "test";
	setAttr -k on ".comp";
	setAttr -k on ".cth";
	setAttr -k on ".soll";
	setAttr -k on ".rd" 3;
	setAttr -k on ".lp";
	setAttr -k on ".sp";
	setAttr -k on ".shs";
	setAttr -k on ".lpr";
	setAttr -cb on ".gv";
	setAttr -cb on ".sv";
	setAttr -k on ".mm";
	setAttr -k on ".npu";
	setAttr -k on ".itf";
	setAttr -k on ".shp";
	setAttr -cb on ".isp";
	setAttr -k on ".uf";
	setAttr -k on ".oi";
	setAttr -k on ".rut";
	setAttr -av -k on ".mbf";
	setAttr -k on ".afp";
	setAttr -k on ".pfb";
	setAttr -cb on ".prm";
	setAttr -cb on ".pom";
	setAttr -cb on ".pfrm";
	setAttr -cb on ".pfom";
	setAttr -av -k on ".bll";
	setAttr -k on ".bls";
	setAttr -k on ".smv";
	setAttr -k on ".ubc";
	setAttr -k on ".mbc";
	setAttr -cb on ".mbt";
	setAttr -k on ".udbx";
	setAttr -k on ".smc";
	setAttr -k on ".kmv";
	setAttr -cb on ".isl";
	setAttr -cb on ".ism";
	setAttr -cb on ".imb";
	setAttr -k on ".rlen";
	setAttr -av -k on ".frts";
	setAttr -k on ".tlwd";
	setAttr -k on ".tlht";
	setAttr -k on ".jfc";
	setAttr -cb on ".rsb";
	setAttr -k on ".ope";
	setAttr -k on ".oppf";
	setAttr -cb on ".hbl";
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
select -ne :defaultRenderQuality;
	setAttr ".ert" yes;
select -ne :defaultResolution;
	setAttr -av -k on ".cch";
	setAttr -k on ".ihi";
	setAttr -av -k on ".nds";
	setAttr -k on ".bnm";
	setAttr ".w" 200;
	setAttr ".h" 150;
	setAttr -av ".pa" 1;
	setAttr -av -k on ".al";
	setAttr -av -k on ".ldar";
	setAttr -k on ".dpi";
	setAttr -av -k on ".off";
	setAttr -av -k on ".fld";
	setAttr -av -k on ".zsl";
	setAttr -k on ".isu";
	setAttr -k on ".pdu";
select -ne :defaultLightSet;
	setAttr -k on ".cch";
	setAttr -k on ".ihi";
	setAttr -k on ".nds";
	setAttr -k on ".bnm";
	setAttr -k on ".mwc";
	setAttr -k on ".an";
	setAttr -k on ".il";
	setAttr -k on ".vo";
	setAttr -k on ".eo";
	setAttr -k on ".fo";
	setAttr -k on ".epo";
	setAttr ".ro" yes;
select -ne :defaultObjectSet;
	setAttr ".ro" yes;
select -ne :hardwareRenderGlobals;
	setAttr -k on ".cch";
	setAttr -cb on ".ihi";
	setAttr -k on ".nds";
	setAttr -cb on ".bnm";
	setAttr ".ctrs" 256;
	setAttr ".btrs" 512;
	setAttr -k off ".fbfm";
	setAttr -k off -cb on ".ehql";
	setAttr -k off -cb on ".eams";
	setAttr -k off -cb on ".eeaa";
	setAttr -k off -cb on ".engm";
	setAttr -k off -cb on ".mes";
	setAttr -k off -cb on ".emb";
	setAttr -av -k off -cb on ".mbbf";
	setAttr -k off -cb on ".mbs";
	setAttr -k off -cb on ".trm";
	setAttr -k off -cb on ".tshc";
	setAttr -k off ".enpt";
	setAttr -k off -cb on ".clmt";
	setAttr -k off -cb on ".tcov";
	setAttr -k off -cb on ".lith";
	setAttr -k off -cb on ".sobc";
	setAttr -k off -cb on ".cuth";
	setAttr -k off -cb on ".hgcd";
	setAttr -k off -cb on ".hgci";
	setAttr -k off -cb on ".mgcs";
	setAttr -k off -cb on ".twa";
	setAttr -k off -cb on ".twz";
	setAttr -k on ".hwcc";
	setAttr -k on ".hwdp";
	setAttr -k on ".hwql";
	setAttr -k on ".hwfr";
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
	setAttr -av -k on ".es";
	setAttr -av -k on ".ef";
	setAttr -av -k on ".bf";
	setAttr -k on ".fii";
	setAttr -av -k on ".sf";
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
connectAttr "polyPlane1.out" "pPlaneShape1.i";
connectAttr "polyPlane3.out" "pPlaneShape3.i";
connectAttr "polyCube1.out" "pCubeShape1.i";
relationship "link" ":lightLinker1" ":initialShadingGroup.message" ":defaultLightSet.message";
relationship "link" ":lightLinker1" ":initialParticleSE.message" ":defaultLightSet.message";
relationship "link" ":lightLinker1" "phong1SG.message" ":defaultLightSet.message";
relationship "link" ":lightLinker1" "phong2SG.message" ":defaultLightSet.message";
relationship "link" ":lightLinker1" "phong3SG.message" ":defaultLightSet.message";
relationship "link" ":lightLinker1" "phong4SG.message" ":defaultLightSet.message";
relationship "link" ":lightLinker1" "liquidShader1SG.message" ":defaultLightSet.message";
relationship "link" ":lightLinker1" "liquidSurface1SG.message" ":defaultLightSet.message";
relationship "link" ":lightLinker1" "liquidShader2SG.message" ":defaultLightSet.message";
relationship "shadowLink" ":lightLinker1" ":initialShadingGroup.message" ":defaultLightSet.message";
relationship "shadowLink" ":lightLinker1" ":initialParticleSE.message" ":defaultLightSet.message";
relationship "shadowLink" ":lightLinker1" "phong1SG.message" ":defaultLightSet.message";
relationship "shadowLink" ":lightLinker1" "phong2SG.message" ":defaultLightSet.message";
relationship "shadowLink" ":lightLinker1" "phong3SG.message" ":defaultLightSet.message";
relationship "shadowLink" ":lightLinker1" "phong4SG.message" ":defaultLightSet.message";
relationship "shadowLink" ":lightLinker1" "liquidShader1SG.message" ":defaultLightSet.message";
relationship "shadowLink" ":lightLinker1" "liquidSurface1SG.message" ":defaultLightSet.message";
relationship "shadowLink" ":lightLinker1" "liquidShader2SG.message" ":defaultLightSet.message";
connectAttr "layerManager.dli[0]" "defaultLayer.id";
connectAttr "renderLayerManager.rlmi[0]" "defaultRenderLayer.rlid";
connectAttr "phong1SG.msg" "materialInfo1.sg";
connectAttr "phong2SG.msg" "materialInfo2.sg";
connectAttr "phong3SG.msg" "materialInfo3.sg";
connectAttr "phong4SG.msg" "materialInfo4.sg";
connectAttr ":perspShape.usedBy3dfm" "fullRenderPass.camera";
connectAttr ":defaultResolution.w" "fullRenderPass.resolutionX";
connectAttr ":defaultResolution.h" "fullRenderPass.resolutionY";
connectAttr "fullRenderPass_pixelRatioExpr.out[0]" "fullRenderPass.pixelAspectRatio"
		;
connectAttr ":defaultResolution.h" "fullRenderPass_pixelRatioExpr.in[0]";
connectAttr ":defaultResolution.w" "fullRenderPass_pixelRatioExpr.in[1]";
connectAttr ":defaultResolution.dar" "fullRenderPass_pixelRatioExpr.in[2]";
connectAttr "liquidShader1.oc" "liquidShader1SG.ss";
connectAttr "pPlaneShape1.iog" "liquidShader1SG.dsm" -na;
connectAttr "liquidShader1SG.msg" "materialInfo5.sg";
connectAttr "liquidShader1.msg" "materialInfo5.m";
connectAttr "liquidShader1.msg" "materialInfo5.t" -na;
connectAttr "liquidSurface1.oc" "liquidSurface1SG.ss";
connectAttr "pPlaneShape3.iog" "liquidSurface1SG.dsm" -na;
connectAttr "liquidSurface1SG.msg" "materialInfo6.sg";
connectAttr "liquidSurface1.msg" "materialInfo6.m";
connectAttr "liquidSurface1.msg" "materialInfo6.t" -na;
connectAttr "liquidShader2.oc" "liquidShader2SG.ss";
connectAttr "pCubeShape1.iog" "liquidShader2SG.dsm" -na;
connectAttr "liquidShader2SG.msg" "materialInfo7.sg";
connectAttr "liquidShader2.msg" "materialInfo7.m";
connectAttr "liquidShader2.msg" "materialInfo7.t" -na;
connectAttr "phong1SG.pa" ":renderPartition.st" -na;
connectAttr "phong2SG.pa" ":renderPartition.st" -na;
connectAttr "phong3SG.pa" ":renderPartition.st" -na;
connectAttr "phong4SG.pa" ":renderPartition.st" -na;
connectAttr "liquidShader1SG.pa" ":renderPartition.st" -na;
connectAttr "liquidSurface1SG.pa" ":renderPartition.st" -na;
connectAttr "liquidShader2SG.pa" ":renderPartition.st" -na;
connectAttr "liquidShader1.msg" ":defaultShaderList1.s" -na;
connectAttr "liquidSurface1.msg" ":defaultShaderList1.s" -na;
connectAttr "liquidShader2.msg" ":defaultShaderList1.s" -na;
connectAttr "pointLightShape3.ltd" ":lightList1.l" -na;
connectAttr "defaultRenderLayer.msg" ":defaultRenderingList1.r" -na;
connectAttr "pointLight3.iog" ":defaultLightSet.dsm" -na;
// End of architectural.ma
