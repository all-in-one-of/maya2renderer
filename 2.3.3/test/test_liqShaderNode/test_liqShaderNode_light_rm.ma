//Maya ASCII 2012 scene
//Name: rm_spotlight2.ma
//Last modified: Sun, Aug 19, 2012 11:46:18 PM
//Codeset: 936
requires maya "2012";
requires "liquid_2012x32d" "2.3.5 (buildtime=19:31:32.98)";
currentUnit -l centimeter -a degree -t film;
fileInfo "application" "maya";
fileInfo "product" "Maya 2012";
fileInfo "version" "2012";
fileInfo "cutIdentifier" "001200000000-796618";
fileInfo "osv" "Microsoft Windows XP Professional Service Pack 3 (Build 2600)\n";
createNode transform -s -n "persp";
	setAttr ".v" no;
	setAttr ".t" -type "double3" 38.68124635748044 17.612876498741127 14.23192754591072 ;
	setAttr ".r" -type "double3" -23.138352729604236 69.800000000000594 -4.6055187815750872e-015 ;
createNode camera -s -n "perspShape" -p "persp";
	setAttr -k off ".v" no;
	setAttr ".fl" 34.999999999999993;
	setAttr ".coi" 44.82186966202994;
	setAttr ".imn" -type "string" "persp";
	setAttr ".den" -type "string" "persp_depth";
	setAttr ".man" -type "string" "persp_mask";
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
createNode transform -n "pPlane1";
	setAttr ".t" -type "double3" 4.5583570306494359 0 1.0961500083097668 ;
createNode mesh -n "pPlaneShape1" -p "pPlane1";
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
createNode transform -n "spotLight1";
	setAttr ".t" -type "double3" 0 7.2361337810264512 0 ;
	setAttr ".r" -type "double3" -85.12197550166357 0 0 ;
createNode spotLight -n "spotLightShape1" -p "spotLight1";
	addAttr -ci true -sn "liquidLightShaderNode" -ln "liquidLightShaderNode" -at "message";
	setAttr -k off ".v";
createNode transform -n "pointLight1";
	setAttr ".t" -type "double3" -0.76678525773571327 5.8926123268187114 9.2091404126947509 ;
createNode pointLight -n "pointLightShape1" -p "pointLight1";
	setAttr -k off ".v";
	setAttr ".us" no;
createNode transform -n "spotLight2";
	setAttr ".t" -type "double3" 9.0701059653890859 6.0027472451901893 0 ;
	setAttr ".r" -type "double3" -77.870834178708222 0 0 ;
createNode spotLight -n "spotLightShape2" -p "spotLight2";
	addAttr -ci true -sn "liquidLightShaderNode" -ln "liquidLightShaderNode" -at "message";
	setAttr -k off ".v";
createNode lightLinker -s -n "lightLinker1";
	setAttr -s 8 ".lnk";
	setAttr -s 8 ".slnk";
createNode displayLayerManager -n "layerManager";
createNode displayLayer -n "defaultLayer";
createNode renderLayerManager -n "renderLayerManager";
createNode renderLayer -n "defaultRenderLayer";
	setAttr ".g" yes;
createNode script -n "liquidCleanUpNode";
	setAttr ".b" -type "string" "if ( `whatIs liquid` == \"Command\" ) liquidCreateScriptNode;";
	setAttr ".a" -type "string" "if ( `whatIs liquid` == \"Command\" ) liquidCleanUp;";
	setAttr ".st" 2;
createNode liquidGlobals -n "liquidGlobals";
	setAttr ".rc" -type "string" "perspShape";
	setAttr ".ddin[0]" -type "string" "$SCN.$CAM.$F.$RND.bmp";
	setAttr ".ddim[0]" -type "string" "rgba";
	setAttr ".ddit[0]" -type "string" "it";
	setAttr ".ddbd[0]"  8;
	setAttr ".shdd" -type "string" "generatedShader";
	setAttr ".unt" yes;
	setAttr ".osg" yes;
	setAttr ".rdc" -type "string" "prman";
	setAttr ".prv" -type "string" "prman";
	setAttr ".lrs" -type "string" "E:/MyDocuments/maya/projects/default/rmantmp/rm_spotlight2116.xml";
	setAttr ".shi" -type "string" "sloinfo";
	setAttr ".shcp" -type "string" "shader";
	setAttr ".she" -type "string" "slo";
	setAttr ".mtx" -type "string" "txmake";
	setAttr ".vtx" -type "string" "sho";
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
createNode polyPlane -n "polyPlane1";
	setAttr ".w" 14.527488363544961;
	setAttr ".h" 15.023654250837886;
	setAttr ".sw" 1;
	setAttr ".sh" 1;
	setAttr ".cuv" 2;
createNode script -n "uiConfigurationScriptNode";
	setAttr ".b" -type "string" (
		"// Maya Mel UI Configuration File.\n//\n//  This script is machine generated.  Edit at your own risk.\n//\n//\n\nglobal string $gMainPane;\nif (`paneLayout -exists $gMainPane`) {\n\n\tglobal int $gUseScenePanelConfig;\n\tint    $useSceneConfig = $gUseScenePanelConfig;\n\tint    $menusOkayInPanels = `optionVar -q allowMenusInPanels`;\tint    $nVisPanes = `paneLayout -q -nvp $gMainPane`;\n\tint    $nPanes = 0;\n\tstring $editorName;\n\tstring $panelName;\n\tstring $itemFilterName;\n\tstring $panelConfig;\n\n\t//\n\t//  get current state of the UI\n\t//\n\tsceneUIReplacement -update $gMainPane;\n\n\t$panelName = `sceneUIReplacement -getNextPanel \"modelPanel\" (localizedPanelLabel(\"Top View\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `modelPanel -unParent -l (localizedPanelLabel(\"Top View\")) -mbv $menusOkayInPanels `;\n\t\t\t$editorName = $panelName;\n            modelEditor -e \n                -camera \"top\" \n                -useInteractiveMode 0\n                -displayLights \"default\" \n                -displayAppearance \"wireframe\" \n"
		+ "                -activeOnly 0\n                -ignorePanZoom 0\n                -wireframeOnShaded 0\n                -headsUpDisplay 1\n                -selectionHiliteDisplay 1\n                -useDefaultMaterial 0\n                -bufferMode \"double\" \n                -twoSidedLighting 1\n                -backfaceCulling 0\n                -xray 0\n                -jointXray 0\n                -activeComponentsXray 0\n                -displayTextures 0\n                -smoothWireframe 0\n                -lineWidth 1\n                -textureAnisotropic 0\n                -textureHilight 1\n                -textureSampling 2\n                -textureDisplay \"modulate\" \n                -textureMaxSize 8192\n                -fogging 0\n                -fogSource \"fragment\" \n                -fogMode \"linear\" \n                -fogStart 0\n                -fogEnd 100\n                -fogDensity 0.1\n                -fogColor 0.5 0.5 0.5 1 \n                -maxConstantTransparency 1\n                -rendererName \"base_OpenGL_Renderer\" \n"
		+ "                -colorResolution 256 256 \n                -bumpResolution 512 512 \n                -textureCompression 0\n                -transparencyAlgorithm \"frontAndBackCull\" \n                -transpInShadows 0\n                -cullingOverride \"none\" \n                -lowQualityLighting 0\n                -maximumNumHardwareLights 1\n                -occlusionCulling 0\n                -shadingModel 0\n                -useBaseRenderer 0\n                -useReducedRenderer 0\n                -smallObjectCulling 0\n                -smallObjectThreshold -1 \n                -interactiveDisableShadows 0\n                -interactiveBackFaceCull 0\n                -sortTransparent 1\n                -nurbsCurves 1\n                -nurbsSurfaces 1\n                -polymeshes 1\n                -subdivSurfaces 1\n                -planes 1\n                -lights 1\n                -cameras 1\n                -controlVertices 1\n                -hulls 1\n                -grid 1\n                -joints 1\n                -ikHandles 1\n"
		+ "                -deformers 1\n                -dynamics 1\n                -fluids 1\n                -hairSystems 1\n                -follicles 1\n                -nCloths 1\n                -nParticles 1\n                -nRigids 1\n                -dynamicConstraints 1\n                -locators 1\n                -manipulators 1\n                -dimensions 1\n                -handles 1\n                -pivots 1\n                -textures 1\n                -strokes 1\n                -motionTrails 1\n                -shadows 0\n                $editorName;\nmodelEditor -e -viewSelected 0 $editorName;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tmodelPanel -edit -l (localizedPanelLabel(\"Top View\")) -mbv $menusOkayInPanels  $panelName;\n\t\t$editorName = $panelName;\n        modelEditor -e \n            -camera \"top\" \n            -useInteractiveMode 0\n            -displayLights \"default\" \n            -displayAppearance \"wireframe\" \n            -activeOnly 0\n            -ignorePanZoom 0\n            -wireframeOnShaded 0\n"
		+ "            -headsUpDisplay 1\n            -selectionHiliteDisplay 1\n            -useDefaultMaterial 0\n            -bufferMode \"double\" \n            -twoSidedLighting 1\n            -backfaceCulling 0\n            -xray 0\n            -jointXray 0\n            -activeComponentsXray 0\n            -displayTextures 0\n            -smoothWireframe 0\n            -lineWidth 1\n            -textureAnisotropic 0\n            -textureHilight 1\n            -textureSampling 2\n            -textureDisplay \"modulate\" \n            -textureMaxSize 8192\n            -fogging 0\n            -fogSource \"fragment\" \n            -fogMode \"linear\" \n            -fogStart 0\n            -fogEnd 100\n            -fogDensity 0.1\n            -fogColor 0.5 0.5 0.5 1 \n            -maxConstantTransparency 1\n            -rendererName \"base_OpenGL_Renderer\" \n            -colorResolution 256 256 \n            -bumpResolution 512 512 \n            -textureCompression 0\n            -transparencyAlgorithm \"frontAndBackCull\" \n            -transpInShadows 0\n            -cullingOverride \"none\" \n"
		+ "            -lowQualityLighting 0\n            -maximumNumHardwareLights 1\n            -occlusionCulling 0\n            -shadingModel 0\n            -useBaseRenderer 0\n            -useReducedRenderer 0\n            -smallObjectCulling 0\n            -smallObjectThreshold -1 \n            -interactiveDisableShadows 0\n            -interactiveBackFaceCull 0\n            -sortTransparent 1\n            -nurbsCurves 1\n            -nurbsSurfaces 1\n            -polymeshes 1\n            -subdivSurfaces 1\n            -planes 1\n            -lights 1\n            -cameras 1\n            -controlVertices 1\n            -hulls 1\n            -grid 1\n            -joints 1\n            -ikHandles 1\n            -deformers 1\n            -dynamics 1\n            -fluids 1\n            -hairSystems 1\n            -follicles 1\n            -nCloths 1\n            -nParticles 1\n            -nRigids 1\n            -dynamicConstraints 1\n            -locators 1\n            -manipulators 1\n            -dimensions 1\n            -handles 1\n            -pivots 1\n"
		+ "            -textures 1\n            -strokes 1\n            -motionTrails 1\n            -shadows 0\n            $editorName;\nmodelEditor -e -viewSelected 0 $editorName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextPanel \"modelPanel\" (localizedPanelLabel(\"Side View\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `modelPanel -unParent -l (localizedPanelLabel(\"Side View\")) -mbv $menusOkayInPanels `;\n\t\t\t$editorName = $panelName;\n            modelEditor -e \n                -camera \"side\" \n                -useInteractiveMode 0\n                -displayLights \"default\" \n                -displayAppearance \"wireframe\" \n                -activeOnly 0\n                -ignorePanZoom 0\n                -wireframeOnShaded 0\n                -headsUpDisplay 1\n                -selectionHiliteDisplay 1\n                -useDefaultMaterial 0\n                -bufferMode \"double\" \n                -twoSidedLighting 1\n                -backfaceCulling 0\n"
		+ "                -xray 0\n                -jointXray 0\n                -activeComponentsXray 0\n                -displayTextures 0\n                -smoothWireframe 0\n                -lineWidth 1\n                -textureAnisotropic 0\n                -textureHilight 1\n                -textureSampling 2\n                -textureDisplay \"modulate\" \n                -textureMaxSize 8192\n                -fogging 0\n                -fogSource \"fragment\" \n                -fogMode \"linear\" \n                -fogStart 0\n                -fogEnd 100\n                -fogDensity 0.1\n                -fogColor 0.5 0.5 0.5 1 \n                -maxConstantTransparency 1\n                -rendererName \"base_OpenGL_Renderer\" \n                -colorResolution 256 256 \n                -bumpResolution 512 512 \n                -textureCompression 0\n                -transparencyAlgorithm \"frontAndBackCull\" \n                -transpInShadows 0\n                -cullingOverride \"none\" \n                -lowQualityLighting 0\n                -maximumNumHardwareLights 1\n"
		+ "                -occlusionCulling 0\n                -shadingModel 0\n                -useBaseRenderer 0\n                -useReducedRenderer 0\n                -smallObjectCulling 0\n                -smallObjectThreshold -1 \n                -interactiveDisableShadows 0\n                -interactiveBackFaceCull 0\n                -sortTransparent 1\n                -nurbsCurves 1\n                -nurbsSurfaces 1\n                -polymeshes 1\n                -subdivSurfaces 1\n                -planes 1\n                -lights 1\n                -cameras 1\n                -controlVertices 1\n                -hulls 1\n                -grid 1\n                -joints 1\n                -ikHandles 1\n                -deformers 1\n                -dynamics 1\n                -fluids 1\n                -hairSystems 1\n                -follicles 1\n                -nCloths 1\n                -nParticles 1\n                -nRigids 1\n                -dynamicConstraints 1\n                -locators 1\n                -manipulators 1\n                -dimensions 1\n"
		+ "                -handles 1\n                -pivots 1\n                -textures 1\n                -strokes 1\n                -motionTrails 1\n                -shadows 0\n                $editorName;\nmodelEditor -e -viewSelected 0 $editorName;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tmodelPanel -edit -l (localizedPanelLabel(\"Side View\")) -mbv $menusOkayInPanels  $panelName;\n\t\t$editorName = $panelName;\n        modelEditor -e \n            -camera \"side\" \n            -useInteractiveMode 0\n            -displayLights \"default\" \n            -displayAppearance \"wireframe\" \n            -activeOnly 0\n            -ignorePanZoom 0\n            -wireframeOnShaded 0\n            -headsUpDisplay 1\n            -selectionHiliteDisplay 1\n            -useDefaultMaterial 0\n            -bufferMode \"double\" \n            -twoSidedLighting 1\n            -backfaceCulling 0\n            -xray 0\n            -jointXray 0\n            -activeComponentsXray 0\n            -displayTextures 0\n            -smoothWireframe 0\n            -lineWidth 1\n"
		+ "            -textureAnisotropic 0\n            -textureHilight 1\n            -textureSampling 2\n            -textureDisplay \"modulate\" \n            -textureMaxSize 8192\n            -fogging 0\n            -fogSource \"fragment\" \n            -fogMode \"linear\" \n            -fogStart 0\n            -fogEnd 100\n            -fogDensity 0.1\n            -fogColor 0.5 0.5 0.5 1 \n            -maxConstantTransparency 1\n            -rendererName \"base_OpenGL_Renderer\" \n            -colorResolution 256 256 \n            -bumpResolution 512 512 \n            -textureCompression 0\n            -transparencyAlgorithm \"frontAndBackCull\" \n            -transpInShadows 0\n            -cullingOverride \"none\" \n            -lowQualityLighting 0\n            -maximumNumHardwareLights 1\n            -occlusionCulling 0\n            -shadingModel 0\n            -useBaseRenderer 0\n            -useReducedRenderer 0\n            -smallObjectCulling 0\n            -smallObjectThreshold -1 \n            -interactiveDisableShadows 0\n            -interactiveBackFaceCull 0\n"
		+ "            -sortTransparent 1\n            -nurbsCurves 1\n            -nurbsSurfaces 1\n            -polymeshes 1\n            -subdivSurfaces 1\n            -planes 1\n            -lights 1\n            -cameras 1\n            -controlVertices 1\n            -hulls 1\n            -grid 1\n            -joints 1\n            -ikHandles 1\n            -deformers 1\n            -dynamics 1\n            -fluids 1\n            -hairSystems 1\n            -follicles 1\n            -nCloths 1\n            -nParticles 1\n            -nRigids 1\n            -dynamicConstraints 1\n            -locators 1\n            -manipulators 1\n            -dimensions 1\n            -handles 1\n            -pivots 1\n            -textures 1\n            -strokes 1\n            -motionTrails 1\n            -shadows 0\n            $editorName;\nmodelEditor -e -viewSelected 0 $editorName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextPanel \"modelPanel\" (localizedPanelLabel(\"Front View\")) `;\n\tif (\"\" == $panelName) {\n"
		+ "\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `modelPanel -unParent -l (localizedPanelLabel(\"Front View\")) -mbv $menusOkayInPanels `;\n\t\t\t$editorName = $panelName;\n            modelEditor -e \n                -camera \"front\" \n                -useInteractiveMode 0\n                -displayLights \"default\" \n                -displayAppearance \"wireframe\" \n                -activeOnly 0\n                -ignorePanZoom 0\n                -wireframeOnShaded 0\n                -headsUpDisplay 1\n                -selectionHiliteDisplay 1\n                -useDefaultMaterial 0\n                -bufferMode \"double\" \n                -twoSidedLighting 1\n                -backfaceCulling 0\n                -xray 0\n                -jointXray 0\n                -activeComponentsXray 0\n                -displayTextures 0\n                -smoothWireframe 0\n                -lineWidth 1\n                -textureAnisotropic 0\n                -textureHilight 1\n                -textureSampling 2\n                -textureDisplay \"modulate\" \n                -textureMaxSize 8192\n"
		+ "                -fogging 0\n                -fogSource \"fragment\" \n                -fogMode \"linear\" \n                -fogStart 0\n                -fogEnd 100\n                -fogDensity 0.1\n                -fogColor 0.5 0.5 0.5 1 \n                -maxConstantTransparency 1\n                -rendererName \"base_OpenGL_Renderer\" \n                -colorResolution 256 256 \n                -bumpResolution 512 512 \n                -textureCompression 0\n                -transparencyAlgorithm \"frontAndBackCull\" \n                -transpInShadows 0\n                -cullingOverride \"none\" \n                -lowQualityLighting 0\n                -maximumNumHardwareLights 1\n                -occlusionCulling 0\n                -shadingModel 0\n                -useBaseRenderer 0\n                -useReducedRenderer 0\n                -smallObjectCulling 0\n                -smallObjectThreshold -1 \n                -interactiveDisableShadows 0\n                -interactiveBackFaceCull 0\n                -sortTransparent 1\n                -nurbsCurves 1\n"
		+ "                -nurbsSurfaces 1\n                -polymeshes 1\n                -subdivSurfaces 1\n                -planes 1\n                -lights 1\n                -cameras 1\n                -controlVertices 1\n                -hulls 1\n                -grid 1\n                -joints 1\n                -ikHandles 1\n                -deformers 1\n                -dynamics 1\n                -fluids 1\n                -hairSystems 1\n                -follicles 1\n                -nCloths 1\n                -nParticles 1\n                -nRigids 1\n                -dynamicConstraints 1\n                -locators 1\n                -manipulators 1\n                -dimensions 1\n                -handles 1\n                -pivots 1\n                -textures 1\n                -strokes 1\n                -motionTrails 1\n                -shadows 0\n                $editorName;\nmodelEditor -e -viewSelected 0 $editorName;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tmodelPanel -edit -l (localizedPanelLabel(\"Front View\")) -mbv $menusOkayInPanels  $panelName;\n"
		+ "\t\t$editorName = $panelName;\n        modelEditor -e \n            -camera \"front\" \n            -useInteractiveMode 0\n            -displayLights \"default\" \n            -displayAppearance \"wireframe\" \n            -activeOnly 0\n            -ignorePanZoom 0\n            -wireframeOnShaded 0\n            -headsUpDisplay 1\n            -selectionHiliteDisplay 1\n            -useDefaultMaterial 0\n            -bufferMode \"double\" \n            -twoSidedLighting 1\n            -backfaceCulling 0\n            -xray 0\n            -jointXray 0\n            -activeComponentsXray 0\n            -displayTextures 0\n            -smoothWireframe 0\n            -lineWidth 1\n            -textureAnisotropic 0\n            -textureHilight 1\n            -textureSampling 2\n            -textureDisplay \"modulate\" \n            -textureMaxSize 8192\n            -fogging 0\n            -fogSource \"fragment\" \n            -fogMode \"linear\" \n            -fogStart 0\n            -fogEnd 100\n            -fogDensity 0.1\n            -fogColor 0.5 0.5 0.5 1 \n            -maxConstantTransparency 1\n"
		+ "            -rendererName \"base_OpenGL_Renderer\" \n            -colorResolution 256 256 \n            -bumpResolution 512 512 \n            -textureCompression 0\n            -transparencyAlgorithm \"frontAndBackCull\" \n            -transpInShadows 0\n            -cullingOverride \"none\" \n            -lowQualityLighting 0\n            -maximumNumHardwareLights 1\n            -occlusionCulling 0\n            -shadingModel 0\n            -useBaseRenderer 0\n            -useReducedRenderer 0\n            -smallObjectCulling 0\n            -smallObjectThreshold -1 \n            -interactiveDisableShadows 0\n            -interactiveBackFaceCull 0\n            -sortTransparent 1\n            -nurbsCurves 1\n            -nurbsSurfaces 1\n            -polymeshes 1\n            -subdivSurfaces 1\n            -planes 1\n            -lights 1\n            -cameras 1\n            -controlVertices 1\n            -hulls 1\n            -grid 1\n            -joints 1\n            -ikHandles 1\n            -deformers 1\n            -dynamics 1\n            -fluids 1\n"
		+ "            -hairSystems 1\n            -follicles 1\n            -nCloths 1\n            -nParticles 1\n            -nRigids 1\n            -dynamicConstraints 1\n            -locators 1\n            -manipulators 1\n            -dimensions 1\n            -handles 1\n            -pivots 1\n            -textures 1\n            -strokes 1\n            -motionTrails 1\n            -shadows 0\n            $editorName;\nmodelEditor -e -viewSelected 0 $editorName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextPanel \"modelPanel\" (localizedPanelLabel(\"Persp View\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `modelPanel -unParent -l (localizedPanelLabel(\"Persp View\")) -mbv $menusOkayInPanels `;\n\t\t\t$editorName = $panelName;\n            modelEditor -e \n                -camera \"persp\" \n                -useInteractiveMode 0\n                -displayLights \"default\" \n                -displayAppearance \"wireframe\" \n                -activeOnly 0\n                -ignorePanZoom 0\n"
		+ "                -wireframeOnShaded 0\n                -headsUpDisplay 1\n                -selectionHiliteDisplay 1\n                -useDefaultMaterial 0\n                -bufferMode \"double\" \n                -twoSidedLighting 1\n                -backfaceCulling 0\n                -xray 0\n                -jointXray 0\n                -activeComponentsXray 0\n                -displayTextures 0\n                -smoothWireframe 0\n                -lineWidth 1\n                -textureAnisotropic 0\n                -textureHilight 1\n                -textureSampling 2\n                -textureDisplay \"modulate\" \n                -textureMaxSize 8192\n                -fogging 0\n                -fogSource \"fragment\" \n                -fogMode \"linear\" \n                -fogStart 0\n                -fogEnd 100\n                -fogDensity 0.1\n                -fogColor 0.5 0.5 0.5 1 \n                -maxConstantTransparency 1\n                -rendererName \"base_OpenGL_Renderer\" \n                -colorResolution 256 256 \n                -bumpResolution 512 512 \n"
		+ "                -textureCompression 0\n                -transparencyAlgorithm \"frontAndBackCull\" \n                -transpInShadows 0\n                -cullingOverride \"none\" \n                -lowQualityLighting 0\n                -maximumNumHardwareLights 1\n                -occlusionCulling 0\n                -shadingModel 0\n                -useBaseRenderer 0\n                -useReducedRenderer 0\n                -smallObjectCulling 0\n                -smallObjectThreshold -1 \n                -interactiveDisableShadows 0\n                -interactiveBackFaceCull 0\n                -sortTransparent 1\n                -nurbsCurves 1\n                -nurbsSurfaces 1\n                -polymeshes 1\n                -subdivSurfaces 1\n                -planes 1\n                -lights 1\n                -cameras 1\n                -controlVertices 1\n                -hulls 1\n                -grid 1\n                -joints 1\n                -ikHandles 1\n                -deformers 1\n                -dynamics 1\n                -fluids 1\n"
		+ "                -hairSystems 1\n                -follicles 1\n                -nCloths 1\n                -nParticles 1\n                -nRigids 1\n                -dynamicConstraints 1\n                -locators 1\n                -manipulators 1\n                -dimensions 1\n                -handles 1\n                -pivots 1\n                -textures 1\n                -strokes 1\n                -motionTrails 1\n                -shadows 0\n                $editorName;\nmodelEditor -e -viewSelected 0 $editorName;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tmodelPanel -edit -l (localizedPanelLabel(\"Persp View\")) -mbv $menusOkayInPanels  $panelName;\n\t\t$editorName = $panelName;\n        modelEditor -e \n            -camera \"persp\" \n            -useInteractiveMode 0\n            -displayLights \"default\" \n            -displayAppearance \"wireframe\" \n            -activeOnly 0\n            -ignorePanZoom 0\n            -wireframeOnShaded 0\n            -headsUpDisplay 1\n            -selectionHiliteDisplay 1\n            -useDefaultMaterial 0\n"
		+ "            -bufferMode \"double\" \n            -twoSidedLighting 1\n            -backfaceCulling 0\n            -xray 0\n            -jointXray 0\n            -activeComponentsXray 0\n            -displayTextures 0\n            -smoothWireframe 0\n            -lineWidth 1\n            -textureAnisotropic 0\n            -textureHilight 1\n            -textureSampling 2\n            -textureDisplay \"modulate\" \n            -textureMaxSize 8192\n            -fogging 0\n            -fogSource \"fragment\" \n            -fogMode \"linear\" \n            -fogStart 0\n            -fogEnd 100\n            -fogDensity 0.1\n            -fogColor 0.5 0.5 0.5 1 \n            -maxConstantTransparency 1\n            -rendererName \"base_OpenGL_Renderer\" \n            -colorResolution 256 256 \n            -bumpResolution 512 512 \n            -textureCompression 0\n            -transparencyAlgorithm \"frontAndBackCull\" \n            -transpInShadows 0\n            -cullingOverride \"none\" \n            -lowQualityLighting 0\n            -maximumNumHardwareLights 1\n"
		+ "            -occlusionCulling 0\n            -shadingModel 0\n            -useBaseRenderer 0\n            -useReducedRenderer 0\n            -smallObjectCulling 0\n            -smallObjectThreshold -1 \n            -interactiveDisableShadows 0\n            -interactiveBackFaceCull 0\n            -sortTransparent 1\n            -nurbsCurves 1\n            -nurbsSurfaces 1\n            -polymeshes 1\n            -subdivSurfaces 1\n            -planes 1\n            -lights 1\n            -cameras 1\n            -controlVertices 1\n            -hulls 1\n            -grid 1\n            -joints 1\n            -ikHandles 1\n            -deformers 1\n            -dynamics 1\n            -fluids 1\n            -hairSystems 1\n            -follicles 1\n            -nCloths 1\n            -nParticles 1\n            -nRigids 1\n            -dynamicConstraints 1\n            -locators 1\n            -manipulators 1\n            -dimensions 1\n            -handles 1\n            -pivots 1\n            -textures 1\n            -strokes 1\n            -motionTrails 1\n"
		+ "            -shadows 0\n            $editorName;\nmodelEditor -e -viewSelected 0 $editorName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextPanel \"outlinerPanel\" (localizedPanelLabel(\"Outliner\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `outlinerPanel -unParent -l (localizedPanelLabel(\"Outliner\")) -mbv $menusOkayInPanels `;\n\t\t\t$editorName = $panelName;\n            outlinerEditor -e \n                -showShapes 0\n                -showAttributes 0\n                -showConnected 0\n                -showAnimCurvesOnly 0\n                -showMuteInfo 0\n                -organizeByLayer 1\n                -showAnimLayerWeight 1\n                -autoExpandLayers 1\n                -autoExpand 0\n                -showDagOnly 1\n                -showAssets 1\n                -showContainedOnly 1\n                -showPublishedAsConnected 0\n                -showContainerContents 1\n                -ignoreDagHierarchy 0\n                -expandConnections 0\n"
		+ "                -showUpstreamCurves 1\n                -showUnitlessCurves 1\n                -showCompounds 1\n                -showLeafs 1\n                -showNumericAttrsOnly 0\n                -highlightActive 1\n                -autoSelectNewObjects 0\n                -doNotSelectNewObjects 0\n                -dropIsParent 1\n                -transmitFilters 0\n                -setFilter \"defaultSetFilter\" \n                -showSetMembers 1\n                -allowMultiSelection 1\n                -alwaysToggleSelect 0\n                -directSelect 0\n                -displayMode \"DAG\" \n                -expandObjects 0\n                -setsIgnoreFilters 1\n                -containersIgnoreFilters 0\n                -editAttrName 0\n                -showAttrValues 0\n                -highlightSecondary 0\n                -showUVAttrsOnly 0\n                -showTextureNodesOnly 0\n                -attrAlphaOrder \"default\" \n                -animLayerFilterOptions \"allAffecting\" \n                -sortOrder \"none\" \n                -longNames 0\n"
		+ "                -niceNames 1\n                -showNamespace 1\n                -showPinIcons 0\n                -mapMotionTrails 0\n                $editorName;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\toutlinerPanel -edit -l (localizedPanelLabel(\"Outliner\")) -mbv $menusOkayInPanels  $panelName;\n\t\t$editorName = $panelName;\n        outlinerEditor -e \n            -showShapes 0\n            -showAttributes 0\n            -showConnected 0\n            -showAnimCurvesOnly 0\n            -showMuteInfo 0\n            -organizeByLayer 1\n            -showAnimLayerWeight 1\n            -autoExpandLayers 1\n            -autoExpand 0\n            -showDagOnly 1\n            -showAssets 1\n            -showContainedOnly 1\n            -showPublishedAsConnected 0\n            -showContainerContents 1\n            -ignoreDagHierarchy 0\n            -expandConnections 0\n            -showUpstreamCurves 1\n            -showUnitlessCurves 1\n            -showCompounds 1\n            -showLeafs 1\n            -showNumericAttrsOnly 0\n            -highlightActive 1\n"
		+ "            -autoSelectNewObjects 0\n            -doNotSelectNewObjects 0\n            -dropIsParent 1\n            -transmitFilters 0\n            -setFilter \"defaultSetFilter\" \n            -showSetMembers 1\n            -allowMultiSelection 1\n            -alwaysToggleSelect 0\n            -directSelect 0\n            -displayMode \"DAG\" \n            -expandObjects 0\n            -setsIgnoreFilters 1\n            -containersIgnoreFilters 0\n            -editAttrName 0\n            -showAttrValues 0\n            -highlightSecondary 0\n            -showUVAttrsOnly 0\n            -showTextureNodesOnly 0\n            -attrAlphaOrder \"default\" \n            -animLayerFilterOptions \"allAffecting\" \n            -sortOrder \"none\" \n            -longNames 0\n            -niceNames 1\n            -showNamespace 1\n            -showPinIcons 0\n            -mapMotionTrails 0\n            $editorName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"graphEditor\" (localizedPanelLabel(\"Graph Editor\")) `;\n"
		+ "\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `scriptedPanel -unParent  -type \"graphEditor\" -l (localizedPanelLabel(\"Graph Editor\")) -mbv $menusOkayInPanels `;\n\n\t\t\t$editorName = ($panelName+\"OutlineEd\");\n            outlinerEditor -e \n                -showShapes 1\n                -showAttributes 1\n                -showConnected 1\n                -showAnimCurvesOnly 1\n                -showMuteInfo 0\n                -organizeByLayer 1\n                -showAnimLayerWeight 1\n                -autoExpandLayers 1\n                -autoExpand 1\n                -showDagOnly 0\n                -showAssets 1\n                -showContainedOnly 0\n                -showPublishedAsConnected 0\n                -showContainerContents 0\n                -ignoreDagHierarchy 0\n                -expandConnections 1\n                -showUpstreamCurves 1\n                -showUnitlessCurves 1\n                -showCompounds 0\n                -showLeafs 1\n                -showNumericAttrsOnly 1\n                -highlightActive 0\n"
		+ "                -autoSelectNewObjects 1\n                -doNotSelectNewObjects 0\n                -dropIsParent 1\n                -transmitFilters 1\n                -setFilter \"0\" \n                -showSetMembers 0\n                -allowMultiSelection 1\n                -alwaysToggleSelect 0\n                -directSelect 0\n                -displayMode \"DAG\" \n                -expandObjects 0\n                -setsIgnoreFilters 1\n                -containersIgnoreFilters 0\n                -editAttrName 0\n                -showAttrValues 0\n                -highlightSecondary 0\n                -showUVAttrsOnly 0\n                -showTextureNodesOnly 0\n                -attrAlphaOrder \"default\" \n                -animLayerFilterOptions \"allAffecting\" \n                -sortOrder \"none\" \n                -longNames 0\n                -niceNames 1\n                -showNamespace 1\n                -showPinIcons 1\n                -mapMotionTrails 1\n                $editorName;\n\n\t\t\t$editorName = ($panelName+\"GraphEd\");\n            animCurveEditor -e \n"
		+ "                -displayKeys 1\n                -displayTangents 0\n                -displayActiveKeys 0\n                -displayActiveKeyTangents 1\n                -displayInfinities 0\n                -autoFit 0\n                -snapTime \"integer\" \n                -snapValue \"none\" \n                -showResults \"off\" \n                -showBufferCurves \"off\" \n                -smoothness \"fine\" \n                -resultSamples 1\n                -resultScreenSamples 0\n                -resultUpdate \"delayed\" \n                -showUpstreamCurves 1\n                -stackedCurves 0\n                -stackedCurvesMin -1\n                -stackedCurvesMax 1\n                -stackedCurvesSpace 0.2\n                -displayNormalized 0\n                -preSelectionHighlight 0\n                -constrainDrag 0\n                -classicMode 1\n                $editorName;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Graph Editor\")) -mbv $menusOkayInPanels  $panelName;\n\n\t\t\t$editorName = ($panelName+\"OutlineEd\");\n"
		+ "            outlinerEditor -e \n                -showShapes 1\n                -showAttributes 1\n                -showConnected 1\n                -showAnimCurvesOnly 1\n                -showMuteInfo 0\n                -organizeByLayer 1\n                -showAnimLayerWeight 1\n                -autoExpandLayers 1\n                -autoExpand 1\n                -showDagOnly 0\n                -showAssets 1\n                -showContainedOnly 0\n                -showPublishedAsConnected 0\n                -showContainerContents 0\n                -ignoreDagHierarchy 0\n                -expandConnections 1\n                -showUpstreamCurves 1\n                -showUnitlessCurves 1\n                -showCompounds 0\n                -showLeafs 1\n                -showNumericAttrsOnly 1\n                -highlightActive 0\n                -autoSelectNewObjects 1\n                -doNotSelectNewObjects 0\n                -dropIsParent 1\n                -transmitFilters 1\n                -setFilter \"0\" \n                -showSetMembers 0\n                -allowMultiSelection 1\n"
		+ "                -alwaysToggleSelect 0\n                -directSelect 0\n                -displayMode \"DAG\" \n                -expandObjects 0\n                -setsIgnoreFilters 1\n                -containersIgnoreFilters 0\n                -editAttrName 0\n                -showAttrValues 0\n                -highlightSecondary 0\n                -showUVAttrsOnly 0\n                -showTextureNodesOnly 0\n                -attrAlphaOrder \"default\" \n                -animLayerFilterOptions \"allAffecting\" \n                -sortOrder \"none\" \n                -longNames 0\n                -niceNames 1\n                -showNamespace 1\n                -showPinIcons 1\n                -mapMotionTrails 1\n                $editorName;\n\n\t\t\t$editorName = ($panelName+\"GraphEd\");\n            animCurveEditor -e \n                -displayKeys 1\n                -displayTangents 0\n                -displayActiveKeys 0\n                -displayActiveKeyTangents 1\n                -displayInfinities 0\n                -autoFit 0\n                -snapTime \"integer\" \n"
		+ "                -snapValue \"none\" \n                -showResults \"off\" \n                -showBufferCurves \"off\" \n                -smoothness \"fine\" \n                -resultSamples 1\n                -resultScreenSamples 0\n                -resultUpdate \"delayed\" \n                -showUpstreamCurves 1\n                -stackedCurves 0\n                -stackedCurvesMin -1\n                -stackedCurvesMax 1\n                -stackedCurvesSpace 0.2\n                -displayNormalized 0\n                -preSelectionHighlight 0\n                -constrainDrag 0\n                -classicMode 1\n                $editorName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"dopeSheetPanel\" (localizedPanelLabel(\"Dope Sheet\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `scriptedPanel -unParent  -type \"dopeSheetPanel\" -l (localizedPanelLabel(\"Dope Sheet\")) -mbv $menusOkayInPanels `;\n\n\t\t\t$editorName = ($panelName+\"OutlineEd\");\n            outlinerEditor -e \n"
		+ "                -showShapes 1\n                -showAttributes 1\n                -showConnected 1\n                -showAnimCurvesOnly 1\n                -showMuteInfo 0\n                -organizeByLayer 1\n                -showAnimLayerWeight 1\n                -autoExpandLayers 1\n                -autoExpand 0\n                -showDagOnly 0\n                -showAssets 1\n                -showContainedOnly 0\n                -showPublishedAsConnected 0\n                -showContainerContents 0\n                -ignoreDagHierarchy 0\n                -expandConnections 1\n                -showUpstreamCurves 1\n                -showUnitlessCurves 0\n                -showCompounds 1\n                -showLeafs 1\n                -showNumericAttrsOnly 1\n                -highlightActive 0\n                -autoSelectNewObjects 0\n                -doNotSelectNewObjects 1\n                -dropIsParent 1\n                -transmitFilters 0\n                -setFilter \"0\" \n                -showSetMembers 0\n                -allowMultiSelection 1\n"
		+ "                -alwaysToggleSelect 0\n                -directSelect 0\n                -displayMode \"DAG\" \n                -expandObjects 0\n                -setsIgnoreFilters 1\n                -containersIgnoreFilters 0\n                -editAttrName 0\n                -showAttrValues 0\n                -highlightSecondary 0\n                -showUVAttrsOnly 0\n                -showTextureNodesOnly 0\n                -attrAlphaOrder \"default\" \n                -animLayerFilterOptions \"allAffecting\" \n                -sortOrder \"none\" \n                -longNames 0\n                -niceNames 1\n                -showNamespace 1\n                -showPinIcons 0\n                -mapMotionTrails 1\n                $editorName;\n\n\t\t\t$editorName = ($panelName+\"DopeSheetEd\");\n            dopeSheetEditor -e \n                -displayKeys 1\n                -displayTangents 0\n                -displayActiveKeys 0\n                -displayActiveKeyTangents 0\n                -displayInfinities 0\n                -autoFit 0\n                -snapTime \"integer\" \n"
		+ "                -snapValue \"none\" \n                -outliner \"dopeSheetPanel1OutlineEd\" \n                -showSummary 1\n                -showScene 0\n                -hierarchyBelow 0\n                -showTicks 1\n                -selectionWindow 0 0 0 0 \n                $editorName;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Dope Sheet\")) -mbv $menusOkayInPanels  $panelName;\n\n\t\t\t$editorName = ($panelName+\"OutlineEd\");\n            outlinerEditor -e \n                -showShapes 1\n                -showAttributes 1\n                -showConnected 1\n                -showAnimCurvesOnly 1\n                -showMuteInfo 0\n                -organizeByLayer 1\n                -showAnimLayerWeight 1\n                -autoExpandLayers 1\n                -autoExpand 0\n                -showDagOnly 0\n                -showAssets 1\n                -showContainedOnly 0\n                -showPublishedAsConnected 0\n                -showContainerContents 0\n                -ignoreDagHierarchy 0\n"
		+ "                -expandConnections 1\n                -showUpstreamCurves 1\n                -showUnitlessCurves 0\n                -showCompounds 1\n                -showLeafs 1\n                -showNumericAttrsOnly 1\n                -highlightActive 0\n                -autoSelectNewObjects 0\n                -doNotSelectNewObjects 1\n                -dropIsParent 1\n                -transmitFilters 0\n                -setFilter \"0\" \n                -showSetMembers 0\n                -allowMultiSelection 1\n                -alwaysToggleSelect 0\n                -directSelect 0\n                -displayMode \"DAG\" \n                -expandObjects 0\n                -setsIgnoreFilters 1\n                -containersIgnoreFilters 0\n                -editAttrName 0\n                -showAttrValues 0\n                -highlightSecondary 0\n                -showUVAttrsOnly 0\n                -showTextureNodesOnly 0\n                -attrAlphaOrder \"default\" \n                -animLayerFilterOptions \"allAffecting\" \n                -sortOrder \"none\" \n"
		+ "                -longNames 0\n                -niceNames 1\n                -showNamespace 1\n                -showPinIcons 0\n                -mapMotionTrails 1\n                $editorName;\n\n\t\t\t$editorName = ($panelName+\"DopeSheetEd\");\n            dopeSheetEditor -e \n                -displayKeys 1\n                -displayTangents 0\n                -displayActiveKeys 0\n                -displayActiveKeyTangents 0\n                -displayInfinities 0\n                -autoFit 0\n                -snapTime \"integer\" \n                -snapValue \"none\" \n                -outliner \"dopeSheetPanel1OutlineEd\" \n                -showSummary 1\n                -showScene 0\n                -hierarchyBelow 0\n                -showTicks 1\n                -selectionWindow 0 0 0 0 \n                $editorName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"clipEditorPanel\" (localizedPanelLabel(\"Trax Editor\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n"
		+ "\t\t\t$panelName = `scriptedPanel -unParent  -type \"clipEditorPanel\" -l (localizedPanelLabel(\"Trax Editor\")) -mbv $menusOkayInPanels `;\n\n\t\t\t$editorName = clipEditorNameFromPanel($panelName);\n            clipEditor -e \n                -displayKeys 0\n                -displayTangents 0\n                -displayActiveKeys 0\n                -displayActiveKeyTangents 0\n                -displayInfinities 0\n                -autoFit 0\n                -snapTime \"none\" \n                -snapValue \"none\" \n                -manageSequencer 0 \n                $editorName;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Trax Editor\")) -mbv $menusOkayInPanels  $panelName;\n\n\t\t\t$editorName = clipEditorNameFromPanel($panelName);\n            clipEditor -e \n                -displayKeys 0\n                -displayTangents 0\n                -displayActiveKeys 0\n                -displayActiveKeyTangents 0\n                -displayInfinities 0\n                -autoFit 0\n                -snapTime \"none\" \n"
		+ "                -snapValue \"none\" \n                -manageSequencer 0 \n                $editorName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"sequenceEditorPanel\" (localizedPanelLabel(\"Camera Sequencer\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `scriptedPanel -unParent  -type \"sequenceEditorPanel\" -l (localizedPanelLabel(\"Camera Sequencer\")) -mbv $menusOkayInPanels `;\n\n\t\t\t$editorName = sequenceEditorNameFromPanel($panelName);\n            clipEditor -e \n                -displayKeys 0\n                -displayTangents 0\n                -displayActiveKeys 0\n                -displayActiveKeyTangents 0\n                -displayInfinities 0\n                -autoFit 0\n                -snapTime \"none\" \n                -snapValue \"none\" \n                -manageSequencer 1 \n                $editorName;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Camera Sequencer\")) -mbv $menusOkayInPanels  $panelName;\n"
		+ "\t\t\t$editorName = sequenceEditorNameFromPanel($panelName);\n            clipEditor -e \n                -displayKeys 0\n                -displayTangents 0\n                -displayActiveKeys 0\n                -displayActiveKeyTangents 0\n                -displayInfinities 0\n                -autoFit 0\n                -snapTime \"none\" \n                -snapValue \"none\" \n                -manageSequencer 1 \n                $editorName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"hyperGraphPanel\" (localizedPanelLabel(\"Hypergraph Hierarchy\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `scriptedPanel -unParent  -type \"hyperGraphPanel\" -l (localizedPanelLabel(\"Hypergraph Hierarchy\")) -mbv $menusOkayInPanels `;\n\n\t\t\t$editorName = ($panelName+\"HyperGraphEd\");\n            hyperGraph -e \n                -graphLayoutStyle \"hierarchicalLayout\" \n                -orientation \"horiz\" \n                -mergeConnections 0\n                -zoom 1\n"
		+ "                -animateTransition 0\n                -showRelationships 1\n                -showShapes 0\n                -showDeformers 0\n                -showExpressions 0\n                -showConstraints 0\n                -showUnderworld 0\n                -showInvisible 0\n                -transitionFrames 1\n                -opaqueContainers 0\n                -freeform 0\n                -imagePosition 0 0 \n                -imageScale 1\n                -imageEnabled 0\n                -graphType \"DAG\" \n                -heatMapDisplay 0\n                -updateSelection 1\n                -updateNodeAdded 1\n                -useDrawOverrideColor 0\n                -limitGraphTraversal -1\n                -range 0 0 \n                -iconSize \"smallIcons\" \n                -showCachedConnections 0\n                $editorName;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Hypergraph Hierarchy\")) -mbv $menusOkayInPanels  $panelName;\n\n\t\t\t$editorName = ($panelName+\"HyperGraphEd\");\n"
		+ "            hyperGraph -e \n                -graphLayoutStyle \"hierarchicalLayout\" \n                -orientation \"horiz\" \n                -mergeConnections 0\n                -zoom 1\n                -animateTransition 0\n                -showRelationships 1\n                -showShapes 0\n                -showDeformers 0\n                -showExpressions 0\n                -showConstraints 0\n                -showUnderworld 0\n                -showInvisible 0\n                -transitionFrames 1\n                -opaqueContainers 0\n                -freeform 0\n                -imagePosition 0 0 \n                -imageScale 1\n                -imageEnabled 0\n                -graphType \"DAG\" \n                -heatMapDisplay 0\n                -updateSelection 1\n                -updateNodeAdded 1\n                -useDrawOverrideColor 0\n                -limitGraphTraversal -1\n                -range 0 0 \n                -iconSize \"smallIcons\" \n                -showCachedConnections 0\n                $editorName;\n\t\tif (!$useSceneConfig) {\n"
		+ "\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"hyperShadePanel\" (localizedPanelLabel(\"Hypershade\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `scriptedPanel -unParent  -type \"hyperShadePanel\" -l (localizedPanelLabel(\"Hypershade\")) -mbv $menusOkayInPanels `;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Hypershade\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\tif ($useSceneConfig) {\n\t\tscriptedPanel -e -to $panelName;\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"visorPanel\" (localizedPanelLabel(\"Visor\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `scriptedPanel -unParent  -type \"visorPanel\" -l (localizedPanelLabel(\"Visor\")) -mbv $menusOkayInPanels `;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Visor\")) -mbv $menusOkayInPanels  $panelName;\n"
		+ "\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"createNodePanel\" (localizedPanelLabel(\"Create Node\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `scriptedPanel -unParent  -type \"createNodePanel\" -l (localizedPanelLabel(\"Create Node\")) -mbv $menusOkayInPanels `;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Create Node\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"polyTexturePlacementPanel\" (localizedPanelLabel(\"UV Texture Editor\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `scriptedPanel -unParent  -type \"polyTexturePlacementPanel\" -l (localizedPanelLabel(\"UV Texture Editor\")) -mbv $menusOkayInPanels `;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"UV Texture Editor\")) -mbv $menusOkayInPanels  $panelName;\n"
		+ "\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"renderWindowPanel\" (localizedPanelLabel(\"Render View\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `scriptedPanel -unParent  -type \"renderWindowPanel\" -l (localizedPanelLabel(\"Render View\")) -mbv $menusOkayInPanels `;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Render View\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\tif ($useSceneConfig) {\n\t\tscriptedPanel -e -to $panelName;\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextPanel \"blendShapePanel\" (localizedPanelLabel(\"Blend Shape\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\tblendShapePanel -unParent -l (localizedPanelLabel(\"Blend Shape\")) -mbv $menusOkayInPanels ;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tblendShapePanel -edit -l (localizedPanelLabel(\"Blend Shape\")) -mbv $menusOkayInPanels  $panelName;\n"
		+ "\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"dynRelEdPanel\" (localizedPanelLabel(\"Dynamic Relationships\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `scriptedPanel -unParent  -type \"dynRelEdPanel\" -l (localizedPanelLabel(\"Dynamic Relationships\")) -mbv $menusOkayInPanels `;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Dynamic Relationships\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"relationshipPanel\" (localizedPanelLabel(\"Relationship Editor\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `scriptedPanel -unParent  -type \"relationshipPanel\" -l (localizedPanelLabel(\"Relationship Editor\")) -mbv $menusOkayInPanels `;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Relationship Editor\")) -mbv $menusOkayInPanels  $panelName;\n"
		+ "\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"referenceEditorPanel\" (localizedPanelLabel(\"Reference Editor\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `scriptedPanel -unParent  -type \"referenceEditorPanel\" -l (localizedPanelLabel(\"Reference Editor\")) -mbv $menusOkayInPanels `;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Reference Editor\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"componentEditorPanel\" (localizedPanelLabel(\"Component Editor\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `scriptedPanel -unParent  -type \"componentEditorPanel\" -l (localizedPanelLabel(\"Component Editor\")) -mbv $menusOkayInPanels `;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Component Editor\")) -mbv $menusOkayInPanels  $panelName;\n"
		+ "\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"dynPaintScriptedPanelType\" (localizedPanelLabel(\"Paint Effects\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `scriptedPanel -unParent  -type \"dynPaintScriptedPanelType\" -l (localizedPanelLabel(\"Paint Effects\")) -mbv $menusOkayInPanels `;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Paint Effects\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"scriptEditorPanel\" (localizedPanelLabel(\"Script Editor\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `scriptedPanel -unParent  -type \"scriptEditorPanel\" -l (localizedPanelLabel(\"Script Editor\")) -mbv $menusOkayInPanels `;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Script Editor\")) -mbv $menusOkayInPanels  $panelName;\n"
		+ "\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\tif ($useSceneConfig) {\n\t\tscriptedPanel -e -to $panelName;\n\t}\n\n\n\tif ($useSceneConfig) {\n        string $configName = `getPanel -cwl (localizedPanelLabel(\"Current Layout\"))`;\n        if (\"\" != $configName) {\n\t\t\tpanelConfiguration -edit -label (localizedPanelLabel(\"Current Layout\")) \n\t\t\t\t-defaultImage \"\"\n\t\t\t\t-image \"\"\n\t\t\t\t-sc false\n\t\t\t\t-configString \"global string $gMainPane; paneLayout -e -cn \\\"single\\\" -ps 1 100 100 $gMainPane;\"\n\t\t\t\t-removeAllPanels\n\t\t\t\t-ap false\n\t\t\t\t\t(localizedPanelLabel(\"Persp View\")) \n\t\t\t\t\t\"modelPanel\"\n"
		+ "\t\t\t\t\t\"$panelName = `modelPanel -unParent -l (localizedPanelLabel(\\\"Persp View\\\")) -mbv $menusOkayInPanels `;\\n$editorName = $panelName;\\nmodelEditor -e \\n    -cam `findStartUpCamera persp` \\n    -useInteractiveMode 0\\n    -displayLights \\\"default\\\" \\n    -displayAppearance \\\"wireframe\\\" \\n    -activeOnly 0\\n    -ignorePanZoom 0\\n    -wireframeOnShaded 0\\n    -headsUpDisplay 1\\n    -selectionHiliteDisplay 1\\n    -useDefaultMaterial 0\\n    -bufferMode \\\"double\\\" \\n    -twoSidedLighting 1\\n    -backfaceCulling 0\\n    -xray 0\\n    -jointXray 0\\n    -activeComponentsXray 0\\n    -displayTextures 0\\n    -smoothWireframe 0\\n    -lineWidth 1\\n    -textureAnisotropic 0\\n    -textureHilight 1\\n    -textureSampling 2\\n    -textureDisplay \\\"modulate\\\" \\n    -textureMaxSize 8192\\n    -fogging 0\\n    -fogSource \\\"fragment\\\" \\n    -fogMode \\\"linear\\\" \\n    -fogStart 0\\n    -fogEnd 100\\n    -fogDensity 0.1\\n    -fogColor 0.5 0.5 0.5 1 \\n    -maxConstantTransparency 1\\n    -rendererName \\\"base_OpenGL_Renderer\\\" \\n    -colorResolution 256 256 \\n    -bumpResolution 512 512 \\n    -textureCompression 0\\n    -transparencyAlgorithm \\\"frontAndBackCull\\\" \\n    -transpInShadows 0\\n    -cullingOverride \\\"none\\\" \\n    -lowQualityLighting 0\\n    -maximumNumHardwareLights 1\\n    -occlusionCulling 0\\n    -shadingModel 0\\n    -useBaseRenderer 0\\n    -useReducedRenderer 0\\n    -smallObjectCulling 0\\n    -smallObjectThreshold -1 \\n    -interactiveDisableShadows 0\\n    -interactiveBackFaceCull 0\\n    -sortTransparent 1\\n    -nurbsCurves 1\\n    -nurbsSurfaces 1\\n    -polymeshes 1\\n    -subdivSurfaces 1\\n    -planes 1\\n    -lights 1\\n    -cameras 1\\n    -controlVertices 1\\n    -hulls 1\\n    -grid 1\\n    -joints 1\\n    -ikHandles 1\\n    -deformers 1\\n    -dynamics 1\\n    -fluids 1\\n    -hairSystems 1\\n    -follicles 1\\n    -nCloths 1\\n    -nParticles 1\\n    -nRigids 1\\n    -dynamicConstraints 1\\n    -locators 1\\n    -manipulators 1\\n    -dimensions 1\\n    -handles 1\\n    -pivots 1\\n    -textures 1\\n    -strokes 1\\n    -motionTrails 1\\n    -shadows 0\\n    $editorName;\\nmodelEditor -e -viewSelected 0 $editorName\"\n"
		+ "\t\t\t\t\t\"modelPanel -edit -l (localizedPanelLabel(\\\"Persp View\\\")) -mbv $menusOkayInPanels  $panelName;\\n$editorName = $panelName;\\nmodelEditor -e \\n    -cam `findStartUpCamera persp` \\n    -useInteractiveMode 0\\n    -displayLights \\\"default\\\" \\n    -displayAppearance \\\"wireframe\\\" \\n    -activeOnly 0\\n    -ignorePanZoom 0\\n    -wireframeOnShaded 0\\n    -headsUpDisplay 1\\n    -selectionHiliteDisplay 1\\n    -useDefaultMaterial 0\\n    -bufferMode \\\"double\\\" \\n    -twoSidedLighting 1\\n    -backfaceCulling 0\\n    -xray 0\\n    -jointXray 0\\n    -activeComponentsXray 0\\n    -displayTextures 0\\n    -smoothWireframe 0\\n    -lineWidth 1\\n    -textureAnisotropic 0\\n    -textureHilight 1\\n    -textureSampling 2\\n    -textureDisplay \\\"modulate\\\" \\n    -textureMaxSize 8192\\n    -fogging 0\\n    -fogSource \\\"fragment\\\" \\n    -fogMode \\\"linear\\\" \\n    -fogStart 0\\n    -fogEnd 100\\n    -fogDensity 0.1\\n    -fogColor 0.5 0.5 0.5 1 \\n    -maxConstantTransparency 1\\n    -rendererName \\\"base_OpenGL_Renderer\\\" \\n    -colorResolution 256 256 \\n    -bumpResolution 512 512 \\n    -textureCompression 0\\n    -transparencyAlgorithm \\\"frontAndBackCull\\\" \\n    -transpInShadows 0\\n    -cullingOverride \\\"none\\\" \\n    -lowQualityLighting 0\\n    -maximumNumHardwareLights 1\\n    -occlusionCulling 0\\n    -shadingModel 0\\n    -useBaseRenderer 0\\n    -useReducedRenderer 0\\n    -smallObjectCulling 0\\n    -smallObjectThreshold -1 \\n    -interactiveDisableShadows 0\\n    -interactiveBackFaceCull 0\\n    -sortTransparent 1\\n    -nurbsCurves 1\\n    -nurbsSurfaces 1\\n    -polymeshes 1\\n    -subdivSurfaces 1\\n    -planes 1\\n    -lights 1\\n    -cameras 1\\n    -controlVertices 1\\n    -hulls 1\\n    -grid 1\\n    -joints 1\\n    -ikHandles 1\\n    -deformers 1\\n    -dynamics 1\\n    -fluids 1\\n    -hairSystems 1\\n    -follicles 1\\n    -nCloths 1\\n    -nParticles 1\\n    -nRigids 1\\n    -dynamicConstraints 1\\n    -locators 1\\n    -manipulators 1\\n    -dimensions 1\\n    -handles 1\\n    -pivots 1\\n    -textures 1\\n    -strokes 1\\n    -motionTrails 1\\n    -shadows 0\\n    $editorName;\\nmodelEditor -e -viewSelected 0 $editorName\"\n"
		+ "\t\t\t\t$configName;\n\n            setNamedPanelLayout (localizedPanelLabel(\"Current Layout\"));\n        }\n\n        panelHistory -e -clear mainPanelHistory;\n        setFocus `paneLayout -q -p1 $gMainPane`;\n        sceneUIReplacement -deleteRemaining;\n        sceneUIReplacement -clear;\n\t}\n\n\ngrid -spacing 5 -size 12 -divisions 5 -displayAxes yes -displayGridLines yes -displayDivisionLines yes -displayPerspectiveLabels no -displayOrthographicLabels no -displayAxesBold yes -perspectiveLabelPosition axis -orthographicLabelPosition edge;\nviewManip -drawCompass 0 -compassAngle 0 -frontParameters \"\" -homeParameters \"\" -selectionLockParameters \"\";\n}\n");
	setAttr ".st" 3;
createNode script -n "sceneConfigurationScriptNode";
	setAttr ".b" -type "string" "playbackOptions -min 1 -max 24 -ast 1 -aet 48 ";
	setAttr ".st" 6;
createNode shadingEngine -n "liquidLight1SG";
	setAttr ".ihi" 0;
	setAttr ".ro" yes;
createNode materialInfo -n "materialInfo1";
createNode shadingEngine -n "liquidLight2SG";
	setAttr ".ihi" 0;
	setAttr ".ro" yes;
createNode materialInfo -n "materialInfo2";
createNode liquidLight -n "liquidLight3";
	addAttr -ci true -sn "liqAssignedObjects" -ln "liqAssignedObjects" -at "message";
	addAttr -ci true -h true -sn "intensity" -ln "intensity" -at "double";
	addAttr -ci true -uac -h true -k true -sn "lightcolor" -ln "lightcolor" -at "float3" 
		-nc 3;
	addAttr -ci true -h true -k true -sn "lightcolora" -ln "lightcolora" -at "float" 
		-p "lightcolor";
	addAttr -ci true -h true -k true -sn "lightcolorb" -ln "lightcolorb" -at "float" 
		-p "lightcolor";
	addAttr -ci true -h true -k true -sn "lightcolorc" -ln "lightcolorc" -at "float" 
		-p "lightcolor";
	addAttr -ci true -h true -sn "coneangle" -ln "coneangle" -at "double";
	addAttr -ci true -h true -sn "penumbraangle" -ln "penumbraangle" -at "double";
	addAttr -ci true -h true -sn "dropoff" -ln "dropoff" -at "double";
	addAttr -ci true -h true -sn "decay" -ln "decay" -at "double";
	addAttr -ci true -h true -sn "barndoors" -ln "barndoors" -at "double";
	addAttr -ci true -h true -sn "leftbarndoor" -ln "leftbarndoor" -at "double";
	addAttr -ci true -h true -sn "rightbarndoor" -ln "rightbarndoor" -at "double";
	addAttr -ci true -h true -sn "topbarndoor" -ln "topbarndoor" -at "double";
	addAttr -ci true -h true -sn "bottombarndoor" -ln "bottombarndoor" -at "double";
	addAttr -ci true -h true -sn "decayRegions" -ln "decayRegions" -at "double";
	addAttr -ci true -h true -sn "startDistance1" -ln "startDistance1" -at "double";
	addAttr -ci true -h true -sn "endDistance1" -ln "endDistance1" -at "double";
	addAttr -ci true -h true -sn "startDistance2" -ln "startDistance2" -at "double";
	addAttr -ci true -h true -sn "endDistance2" -ln "endDistance2" -at "double";
	addAttr -ci true -h true -sn "startDistance3" -ln "startDistance3" -at "double";
	addAttr -ci true -h true -sn "endDistance3" -ln "endDistance3" -at "double";
	addAttr -ci true -h true -sn "startDistanceIntensity1" -ln "startDistanceIntensity1" 
		-at "double";
	addAttr -ci true -h true -sn "endDistanceIntensity1" -ln "endDistanceIntensity1" 
		-at "double";
	addAttr -ci true -h true -sn "startDistanceIntensity2" -ln "startDistanceIntensity2" 
		-at "double";
	addAttr -ci true -h true -sn "endDistanceIntensity2" -ln "endDistanceIntensity2" 
		-at "double";
	addAttr -ci true -h true -sn "startDistanceIntensity3" -ln "startDistanceIntensity3" 
		-at "double";
	addAttr -ci true -h true -sn "endDistanceIntensity3" -ln "endDistanceIntensity3" 
		-at "double";
	addAttr -ci true -h true -sn "shadowname" -ln "shadowname" -dt "string";
	addAttr -ci true -h true -sn "shadowbias" -ln "shadowbias" -at "double";
	addAttr -ci true -h true -sn "shadowblur" -ln "shadowblur" -at "double";
	addAttr -ci true -h true -sn "shadowsamples" -ln "shadowsamples" -at "double";
	addAttr -ci true -h true -sn "shadowfiltersize" -ln "shadowfiltersize" -at "double";
	addAttr -ci true -uac -h true -k true -sn "shadowcolor" -ln "shadowcolor" -at "float3" 
		-nc 3;
	addAttr -ci true -h true -k true -sn "shadowcolora" -ln "shadowcolora" -at "float" 
		-p "shadowcolor";
	addAttr -ci true -h true -k true -sn "shadowcolorb" -ln "shadowcolorb" -at "float" 
		-p "shadowcolor";
	addAttr -ci true -h true -k true -sn "shadowcolorc" -ln "shadowcolorc" -at "float" 
		-p "shadowcolor";
	addAttr -ci true -uac -h true -k true -sn "shadowcolorSurf" -ln "shadowcolorSurf" 
		-at "float3" -nc 3;
	addAttr -ci true -h true -k true -sn "shadowcolorSurfa" -ln "shadowcolorSurfa" -at "float" 
		-p "shadowcolorSurf";
	addAttr -ci true -h true -k true -sn "shadowcolorSurfb" -ln "shadowcolorSurfb" -at "float" 
		-p "shadowcolorSurf";
	addAttr -ci true -h true -k true -sn "shadowcolorSurfc" -ln "shadowcolorSurfc" -at "float" 
		-p "shadowcolorSurf";
	addAttr -ci true -h true -sn "shadowcolorMix" -ln "shadowcolorMix" -at "double";
	addAttr -ci true -h true -sn "lightID" -ln "lightID" -at "double";
	addAttr -ci true -h true -sn "__category" -ln "__category" -dt "string";
	addAttr -ci true -uac -h true -k true -sn "__shadowC" -ln "__shadowC" -at "float3" 
		-nc 3;
	addAttr -ci true -h true -k true -sn "__shadowCa" -ln "__shadowCa" -at "float" -p "__shadowC";
	addAttr -ci true -h true -k true -sn "__shadowCb" -ln "__shadowCb" -at "float" -p "__shadowC";
	addAttr -ci true -h true -k true -sn "__shadowCc" -ln "__shadowCc" -at "float" -p "__shadowC";
	addAttr -ci true -h true -sn "__shadowF" -ln "__shadowF" -at "double";
	addAttr -ci true -h true -sn "__barn" -ln "__barn" -at "double";
	addAttr -ci true -uac -h true -k true -sn "__unshadowed_Cl" -ln "__unshadowed_Cl" 
		-at "float3" -nc 3;
	addAttr -ci true -h true -k true -sn "__unshadowed_Cla" -ln "__unshadowed_Cla" -at "float" 
		-p "__unshadowed_Cl";
	addAttr -ci true -h true -k true -sn "__unshadowed_Clb" -ln "__unshadowed_Clb" -at "float" 
		-p "__unshadowed_Cl";
	addAttr -ci true -h true -k true -sn "__unshadowed_Clc" -ln "__unshadowed_Clc" -at "float" 
		-p "__unshadowed_Cl";
	addAttr -ci true -h true -sn "__nondiffuse" -ln "__nondiffuse" -at "double";
	addAttr -ci true -h true -sn "__nonspecular" -ln "__nonspecular" -at "double";
	setAttr ".rms" -type "string" "liquidspot";
	setAttr ".rst" -type "string" "light";
	setAttr ".rml" -type "string" "E:/dev/Autodesk/maya/myplugin/project/liquid_/2.3.3/lib/shaders/liquidspot.slo";
	setAttr ".rpr" -type "stringArray" 40 "intensity" "lightcolor" "coneangle" "penumbraangle" "dropoff" "decay" "barndoors" "leftbarndoor" "rightbarndoor" "topbarndoor" "bottombarndoor" "decayRegions" "startDistance1" "endDistance1" "startDistance2" "endDistance2" "startDistance3" "endDistance3" "startDistanceIntensity1" "endDistanceIntensity1" "startDistanceIntensity2" "endDistanceIntensity2" "startDistanceIntensity3" "endDistanceIntensity3" "shadowname" "shadowbias" "shadowblur" "shadowsamples" "shadowfiltersize" "shadowcolor" "shadowcolorSurf" "shadowcolorMix" "lightID" "__category" "__shadowC" "__shadowF" "__barn" "__unshadowed_Cl" "__nondiffuse" "__nonspecular"  ;
	setAttr ".rdt" -type "stringArray" 40 "uniform" "uniform" "uniform" "uniform" "uniform" "uniform" "uniform" "uniform" "uniform" "uniform" "uniform" "uniform" "uniform" "uniform" "uniform" "uniform" "uniform" "uniform" "uniform" "uniform" "uniform" "uniform" "uniform" "uniform" "uniform" "uniform" "uniform" "uniform" "uniform" "uniform" "varying" "uniform" "uniform" "uniform" "varying" "varying" "varying" "varying" "varying" "varying"  ;
	setAttr ".rty" -type "stringArray" 40 "float" "color" "float" "float" "float" "float" "float" "float" "float" "float" "float" "float" "float" "float" "float" "float" "float" "float" "float" "float" "float" "float" "float" "float" "string" "float" "float" "float" "float" "color" "color" "float" "float" "string" "color" "float" "float" "color" "float" "float"  ;
	setAttr ".rdf" -type "stringArray" 40 "1.0" "1.0000001.0000001.000000" "0.698132" "0.0" "0.0" "0.0" "0.0" "10.0" "10.0" "10.0" "10.0" "0.0" "1.0" "2.0" "3.0" "6.0" "8.0" "10.0" "1.0" "1.0" "1.0" "1.0" "1.0" "1.0" "" "0.01" "0.0" "32.0" "1.0" "0.0000000.0000000.000000" "0.0000000.0000000.000000" "-1.0" "0.0" "" "0.0000000.0000000.000000" "0.0" "0.0" "0.0000000.0000000.000000" "0.0" "0.0"  ;
	setAttr ".ras" -type "Int32Array" 40 -1 -1 -1 -1 -1 -1
		 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1
		 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1
		 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 ;
	setAttr ".rlc" -type "stringArray" 40 "liquidAE_LifFloat( \"intensity\", -1, \"float\", \"intensity\", {\"0\", \"0\"}, \"\", 14 );" "liquidAE_LifColor( \"lightcolor\", -1, \"color\", \"lightcolor\", {\"0\", \"0\"}, \"\", 14 );" "liquidAE_LifFloat( \"coneangle\", -1, \"float\", \"coneangle\", {\"0\", \"0\"}, \"\", 14 );" "liquidAE_LifFloat( \"penumbraangle\", -1, \"float\", \"penumbraangle\", {\"0\", \"0\"}, \"\", 14 );" "liquidAE_LifFloat( \"dropoff\", -1, \"float\", \"dropoff\", {\"0\", \"0\"}, \"\", 14 );" "liquidAE_LifFloat( \"decay\", -1, \"float\", \"decay\", {\"0\", \"0\"}, \"\", 14 );" "liquidAE_LifFloat( \"barndoors\", -1, \"float\", \"barndoors\", {\"0\", \"0\"}, \"\", 14 );" "liquidAE_LifFloat( \"leftbarndoor\", -1, \"float\", \"leftbarndoor\", {\"0\", \"0\"}, \"\", 14 );" "liquidAE_LifFloat( \"rightbarndoor\", -1, \"float\", \"rightbarndoor\", {\"0\", \"0\"}, \"\", 14 );" "liquidAE_LifFloat( \"topbarndoor\", -1, \"float\", \"topbarndoor\", {\"0\", \"0\"}, \"\", 14 );" "liquidAE_LifFloat( \"bottombarndoor\", -1, \"float\", \"bottombarndoor\", {\"0\", \"0\"}, \"\", 14 );" "liquidAE_LifFloat( \"decayRegions\", -1, \"float\", \"decayRegions\", {\"0\", \"0\"}, \"\", 14 );" "liquidAE_LifFloat( \"startDistance1\", -1, \"float\", \"startDistance1\", {\"0\", \"0\"}, \"\", 14 );" "liquidAE_LifFloat( \"endDistance1\", -1, \"float\", \"endDistance1\", {\"0\", \"0\"}, \"\", 14 );" "liquidAE_LifFloat( \"startDistance2\", -1, \"float\", \"startDistance2\", {\"0\", \"0\"}, \"\", 14 );" "liquidAE_LifFloat( \"endDistance2\", -1, \"float\", \"endDistance2\", {\"0\", \"0\"}, \"\", 14 );" "liquidAE_LifFloat( \"startDistance3\", -1, \"float\", \"startDistance3\", {\"0\", \"0\"}, \"\", 14 );" "liquidAE_LifFloat( \"endDistance3\", -1, \"float\", \"endDistance3\", {\"0\", \"0\"}, \"\", 14 );" "liquidAE_LifFloat( \"startDistanceIntensity1\", -1, \"float\", \"startDistanceIntensity1\", {\"0\", \"0\"}, \"\", 14 );" "liquidAE_LifFloat( \"endDistanceIntensity1\", -1, \"float\", \"endDistanceIntensity1\", {\"0\", \"0\"}, \"\", 14 );" "liquidAE_LifFloat( \"startDistanceIntensity2\", -1, \"float\", \"startDistanceIntensity2\", {\"0\", \"0\"}, \"\", 14 );" "liquidAE_LifFloat( \"endDistanceIntensity2\", -1, \"float\", \"endDistanceIntensity2\", {\"0\", \"0\"}, \"\", 14 );" "liquidAE_LifFloat( \"startDistanceIntensity3\", -1, \"float\", \"startDistanceIntensity3\", {\"0\", \"0\"}, \"\", 14 );" "liquidAE_LifFloat( \"endDistanceIntensity3\", -1, \"float\", \"endDistanceIntensity3\", {\"0\", \"0\"}, \"\", 14 );" "liquidAE_LifString( \"shadowname\", -1, \"string\", \"shadowname\", {\"0\", \"0\"}, \"\", 14 );" "liquidAE_LifFloat( \"shadowbias\", -1, \"float\", \"shadowbias\", {\"0\", \"0\"}, \"\", 14 );" "liquidAE_LifFloat( \"shadowblur\", -1, \"float\", \"shadowblur\", {\"0\", \"0\"}, \"\", 14 );" "liquidAE_LifFloat( \"shadowsamples\", -1, \"float\", \"shadowsamples\", {\"0\", \"0\"}, \"\", 14 );" "liquidAE_LifFloat( \"shadowfiltersize\", -1, \"float\", \"shadowfiltersize\", {\"0\", \"0\"}, \"\", 14 );" "liquidAE_LifColor( \"shadowcolor\", -1, \"color\", \"shadowcolor\", {\"0\", \"0\"}, \"\", 14 );" "liquidAE_LifColor( \"shadowcolorSurf\", -1, \"color\", \"shadowcolorSurf\", {\"0\", \"0\"}, \"\", 14 );" "liquidAE_LifFloat( \"shadowcolorMix\", -1, \"float\", \"shadowcolorMix\", {\"0\", \"0\"}, \"\", 14 );" "liquidAE_LifFloat( \"lightID\", -1, \"float\", \"lightID\", {\"0\", \"0\"}, \"\", 14 );" "liquidAE_LifString( \"__category\", -1, \"string\", \"__category\", {\"0\", \"0\"}, \"\", 14 );" "liquidAE_LifColor( \"__shadowC\", -1, \"color\", \"__shadowC\", {\"0\", \"0\"}, \"\", 14 );" "liquidAE_LifFloat( \"__shadowF\", -1, \"float\", \"__shadowF\", {\"0\", \"0\"}, \"\", 14 );" "liquidAE_LifFloat( \"__barn\", -1, \"float\", \"__barn\", {\"0\", \"0\"}, \"\", 14 );" "liquidAE_LifColor( \"__unshadowed_Cl\", -1, \"color\", \"__unshadowed_Cl\", {\"0\", \"0\"}, \"\", 14 );" "liquidAE_LifFloat( \"__nondiffuse\", -1, \"float\", \"__nondiffuse\", {\"0\", \"0\"}, \"\", 14 );" "liquidAE_LifFloat( \"__nonspecular\", -1, \"float\", \"__nonspecular\", {\"0\", \"0\"}, \"\", 14 );"  ;
	setAttr ".rmt" -type "stringArray" 1 "light"  ;
	setAttr ".rio" -type "Int32Array" 40 0 0 0 0 0 0
		 0 0 0 0 0 0 0 0 0 0 0 0
		 0 0 0 0 0 0 0 0 0 0 0 0
		 0 0 0 0 1 1 1 1 1 1 ;
	setAttr ".intensity" 1;
	setAttr -k on ".lightcolor" -type "float3" 1 0 0 ;
	setAttr ".coneangle" 0.698132;
	setAttr ".leftbarndoor" 10;
	setAttr ".rightbarndoor" 10;
	setAttr ".topbarndoor" 10;
	setAttr ".bottombarndoor" 10;
	setAttr ".startDistance1" 1;
	setAttr ".endDistance1" 2;
	setAttr ".startDistance2" 3;
	setAttr ".endDistance2" 6;
	setAttr ".startDistance3" 8;
	setAttr ".endDistance3" 10;
	setAttr ".startDistanceIntensity1" 1;
	setAttr ".endDistanceIntensity1" 1;
	setAttr ".startDistanceIntensity2" 1;
	setAttr ".endDistanceIntensity2" 1;
	setAttr ".startDistanceIntensity3" 1;
	setAttr ".endDistanceIntensity3" 1;
	setAttr ".shadowname" -type "string" "";
	setAttr ".shadowbias" 0.01;
	setAttr ".shadowsamples" 32;
	setAttr ".shadowfiltersize" 1;
	setAttr ".shadowcolorMix" -1;
	setAttr ".__category" -type "string" "";
createNode shadingEngine -n "liquidLight3SG";
	setAttr ".ihi" 0;
	setAttr ".ro" yes;
createNode materialInfo -n "materialInfo3";
createNode shadingEngine -n "liquidShader1SG";
	setAttr ".ihi" 0;
	setAttr ".ro" yes;
createNode materialInfo -n "materialInfo4";
createNode liquidShader -n "liquidShader2";
	addAttr -ci true -sn "liqAssignedObjects" -ln "liqAssignedObjects" -at "message";
	addAttr -ci true -h true -sn "intensity" -ln "intensity" -at "double";
	addAttr -ci true -uac -h true -k true -sn "lightcolor" -ln "lightcolor" -at "float3" 
		-nc 3;
	addAttr -ci true -h true -k true -sn "lightcolora" -ln "lightcolora" -at "float" 
		-p "lightcolor";
	addAttr -ci true -h true -k true -sn "lightcolorb" -ln "lightcolorb" -at "float" 
		-p "lightcolor";
	addAttr -ci true -h true -k true -sn "lightcolorc" -ln "lightcolorc" -at "float" 
		-p "lightcolor";
	addAttr -ci true -h true -sn "coneangle" -ln "coneangle" -at "double";
	addAttr -ci true -h true -sn "penumbraangle" -ln "penumbraangle" -at "double";
	addAttr -ci true -h true -sn "dropoff" -ln "dropoff" -at "double";
	addAttr -ci true -h true -sn "decay" -ln "decay" -at "double";
	addAttr -ci true -h true -sn "barndoors" -ln "barndoors" -at "double";
	addAttr -ci true -h true -sn "leftbarndoor" -ln "leftbarndoor" -at "double";
	addAttr -ci true -h true -sn "rightbarndoor" -ln "rightbarndoor" -at "double";
	addAttr -ci true -h true -sn "topbarndoor" -ln "topbarndoor" -at "double";
	addAttr -ci true -h true -sn "bottombarndoor" -ln "bottombarndoor" -at "double";
	addAttr -ci true -h true -sn "decayRegions" -ln "decayRegions" -at "double";
	addAttr -ci true -h true -sn "startDistance1" -ln "startDistance1" -at "double";
	addAttr -ci true -h true -sn "endDistance1" -ln "endDistance1" -at "double";
	addAttr -ci true -h true -sn "startDistance2" -ln "startDistance2" -at "double";
	addAttr -ci true -h true -sn "endDistance2" -ln "endDistance2" -at "double";
	addAttr -ci true -h true -sn "startDistance3" -ln "startDistance3" -at "double";
	addAttr -ci true -h true -sn "endDistance3" -ln "endDistance3" -at "double";
	addAttr -ci true -h true -sn "startDistanceIntensity1" -ln "startDistanceIntensity1" 
		-at "double";
	addAttr -ci true -h true -sn "endDistanceIntensity1" -ln "endDistanceIntensity1" 
		-at "double";
	addAttr -ci true -h true -sn "startDistanceIntensity2" -ln "startDistanceIntensity2" 
		-at "double";
	addAttr -ci true -h true -sn "endDistanceIntensity2" -ln "endDistanceIntensity2" 
		-at "double";
	addAttr -ci true -h true -sn "startDistanceIntensity3" -ln "startDistanceIntensity3" 
		-at "double";
	addAttr -ci true -h true -sn "endDistanceIntensity3" -ln "endDistanceIntensity3" 
		-at "double";
	addAttr -ci true -h true -sn "shadowname" -ln "shadowname" -dt "string";
	addAttr -ci true -h true -sn "shadowbias" -ln "shadowbias" -at "double";
	addAttr -ci true -h true -sn "shadowblur" -ln "shadowblur" -at "double";
	addAttr -ci true -h true -sn "shadowsamples" -ln "shadowsamples" -at "double";
	addAttr -ci true -h true -sn "shadowfiltersize" -ln "shadowfiltersize" -at "double";
	addAttr -ci true -uac -h true -k true -sn "shadowcolor" -ln "shadowcolor" -at "float3" 
		-nc 3;
	addAttr -ci true -h true -k true -sn "shadowcolora" -ln "shadowcolora" -at "float" 
		-p "shadowcolor";
	addAttr -ci true -h true -k true -sn "shadowcolorb" -ln "shadowcolorb" -at "float" 
		-p "shadowcolor";
	addAttr -ci true -h true -k true -sn "shadowcolorc" -ln "shadowcolorc" -at "float" 
		-p "shadowcolor";
	addAttr -ci true -uac -h true -k true -sn "shadowcolorSurf" -ln "shadowcolorSurf" 
		-at "float3" -nc 3;
	addAttr -ci true -h true -k true -sn "shadowcolorSurfa" -ln "shadowcolorSurfa" -at "float" 
		-p "shadowcolorSurf";
	addAttr -ci true -h true -k true -sn "shadowcolorSurfb" -ln "shadowcolorSurfb" -at "float" 
		-p "shadowcolorSurf";
	addAttr -ci true -h true -k true -sn "shadowcolorSurfc" -ln "shadowcolorSurfc" -at "float" 
		-p "shadowcolorSurf";
	addAttr -ci true -h true -sn "shadowcolorMix" -ln "shadowcolorMix" -at "double";
	addAttr -ci true -h true -sn "lightID" -ln "lightID" -at "double";
	addAttr -ci true -h true -sn "__category" -ln "__category" -dt "string";
	addAttr -ci true -uac -h true -k true -sn "__shadowC" -ln "__shadowC" -at "float3" 
		-nc 3;
	addAttr -ci true -h true -k true -sn "__shadowCa" -ln "__shadowCa" -at "float" -p "__shadowC";
	addAttr -ci true -h true -k true -sn "__shadowCb" -ln "__shadowCb" -at "float" -p "__shadowC";
	addAttr -ci true -h true -k true -sn "__shadowCc" -ln "__shadowCc" -at "float" -p "__shadowC";
	addAttr -ci true -h true -sn "__shadowF" -ln "__shadowF" -at "double";
	addAttr -ci true -h true -sn "__barn" -ln "__barn" -at "double";
	addAttr -ci true -uac -h true -k true -sn "__unshadowed_Cl" -ln "__unshadowed_Cl" 
		-at "float3" -nc 3;
	addAttr -ci true -h true -k true -sn "__unshadowed_Cla" -ln "__unshadowed_Cla" -at "float" 
		-p "__unshadowed_Cl";
	addAttr -ci true -h true -k true -sn "__unshadowed_Clb" -ln "__unshadowed_Clb" -at "float" 
		-p "__unshadowed_Cl";
	addAttr -ci true -h true -k true -sn "__unshadowed_Clc" -ln "__unshadowed_Clc" -at "float" 
		-p "__unshadowed_Cl";
	addAttr -ci true -h true -sn "__nondiffuse" -ln "__nondiffuse" -at "double";
	addAttr -ci true -h true -sn "__nonspecular" -ln "__nonspecular" -at "double";
	setAttr ".rms" -type "string" "liquidspot";
	setAttr ".rst" -type "string" "light";
	setAttr ".rml" -type "string" "E:/dev/Autodesk/maya/myplugin/project/liquid_/2.3.3/lib/shaders/liquidspot.slo";
	setAttr ".rpr" -type "stringArray" 40 "intensity" "lightcolor" "coneangle" "penumbraangle" "dropoff" "decay" "barndoors" "leftbarndoor" "rightbarndoor" "topbarndoor" "bottombarndoor" "decayRegions" "startDistance1" "endDistance1" "startDistance2" "endDistance2" "startDistance3" "endDistance3" "startDistanceIntensity1" "endDistanceIntensity1" "startDistanceIntensity2" "endDistanceIntensity2" "startDistanceIntensity3" "endDistanceIntensity3" "shadowname" "shadowbias" "shadowblur" "shadowsamples" "shadowfiltersize" "shadowcolor" "shadowcolorSurf" "shadowcolorMix" "lightID" "__category" "__shadowC" "__shadowF" "__barn" "__unshadowed_Cl" "__nondiffuse" "__nonspecular"  ;
	setAttr ".rdt" -type "stringArray" 40 "uniform" "uniform" "uniform" "uniform" "uniform" "uniform" "uniform" "uniform" "uniform" "uniform" "uniform" "uniform" "uniform" "uniform" "uniform" "uniform" "uniform" "uniform" "uniform" "uniform" "uniform" "uniform" "uniform" "uniform" "uniform" "uniform" "uniform" "uniform" "uniform" "uniform" "varying" "uniform" "uniform" "uniform" "varying" "varying" "varying" "varying" "varying" "varying"  ;
	setAttr ".rty" -type "stringArray" 40 "float" "color" "float" "float" "float" "float" "float" "float" "float" "float" "float" "float" "float" "float" "float" "float" "float" "float" "float" "float" "float" "float" "float" "float" "string" "float" "float" "float" "float" "color" "color" "float" "float" "string" "color" "float" "float" "color" "float" "float"  ;
	setAttr ".rdf" -type "stringArray" 40 "1.0" "1.0000001.0000001.000000" "0.698132" "0.0" "0.0" "0.0" "0.0" "10.0" "10.0" "10.0" "10.0" "0.0" "1.0" "2.0" "3.0" "6.0" "8.0" "10.0" "1.0" "1.0" "1.0" "1.0" "1.0" "1.0" "" "0.01" "0.0" "32.0" "1.0" "0.0000000.0000000.000000" "0.0000000.0000000.000000" "-1.0" "0.0" "" "0.0000000.0000000.000000" "0.0" "0.0" "0.0000000.0000000.000000" "0.0" "0.0"  ;
	setAttr ".ras" -type "Int32Array" 40 -1 -1 -1 -1 -1 -1
		 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1
		 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1
		 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 ;
	setAttr ".rlc" -type "stringArray" 40 "liquidAE_LifFloat( \"intensity\", -1, \"float\", \"intensity\", {\"0\", \"0\"}, \"\", 14 );" "liquidAE_LifColor( \"lightcolor\", -1, \"color\", \"lightcolor\", {\"0\", \"0\"}, \"\", 14 );" "liquidAE_LifFloat( \"coneangle\", -1, \"float\", \"coneangle\", {\"0\", \"0\"}, \"\", 14 );" "liquidAE_LifFloat( \"penumbraangle\", -1, \"float\", \"penumbraangle\", {\"0\", \"0\"}, \"\", 14 );" "liquidAE_LifFloat( \"dropoff\", -1, \"float\", \"dropoff\", {\"0\", \"0\"}, \"\", 14 );" "liquidAE_LifFloat( \"decay\", -1, \"float\", \"decay\", {\"0\", \"0\"}, \"\", 14 );" "liquidAE_LifFloat( \"barndoors\", -1, \"float\", \"barndoors\", {\"0\", \"0\"}, \"\", 14 );" "liquidAE_LifFloat( \"leftbarndoor\", -1, \"float\", \"leftbarndoor\", {\"0\", \"0\"}, \"\", 14 );" "liquidAE_LifFloat( \"rightbarndoor\", -1, \"float\", \"rightbarndoor\", {\"0\", \"0\"}, \"\", 14 );" "liquidAE_LifFloat( \"topbarndoor\", -1, \"float\", \"topbarndoor\", {\"0\", \"0\"}, \"\", 14 );" "liquidAE_LifFloat( \"bottombarndoor\", -1, \"float\", \"bottombarndoor\", {\"0\", \"0\"}, \"\", 14 );" "liquidAE_LifFloat( \"decayRegions\", -1, \"float\", \"decayRegions\", {\"0\", \"0\"}, \"\", 14 );" "liquidAE_LifFloat( \"startDistance1\", -1, \"float\", \"startDistance1\", {\"0\", \"0\"}, \"\", 14 );" "liquidAE_LifFloat( \"endDistance1\", -1, \"float\", \"endDistance1\", {\"0\", \"0\"}, \"\", 14 );" "liquidAE_LifFloat( \"startDistance2\", -1, \"float\", \"startDistance2\", {\"0\", \"0\"}, \"\", 14 );" "liquidAE_LifFloat( \"endDistance2\", -1, \"float\", \"endDistance2\", {\"0\", \"0\"}, \"\", 14 );" "liquidAE_LifFloat( \"startDistance3\", -1, \"float\", \"startDistance3\", {\"0\", \"0\"}, \"\", 14 );" "liquidAE_LifFloat( \"endDistance3\", -1, \"float\", \"endDistance3\", {\"0\", \"0\"}, \"\", 14 );" "liquidAE_LifFloat( \"startDistanceIntensity1\", -1, \"float\", \"startDistanceIntensity1\", {\"0\", \"0\"}, \"\", 14 );" "liquidAE_LifFloat( \"endDistanceIntensity1\", -1, \"float\", \"endDistanceIntensity1\", {\"0\", \"0\"}, \"\", 14 );" "liquidAE_LifFloat( \"startDistanceIntensity2\", -1, \"float\", \"startDistanceIntensity2\", {\"0\", \"0\"}, \"\", 14 );" "liquidAE_LifFloat( \"endDistanceIntensity2\", -1, \"float\", \"endDistanceIntensity2\", {\"0\", \"0\"}, \"\", 14 );" "liquidAE_LifFloat( \"startDistanceIntensity3\", -1, \"float\", \"startDistanceIntensity3\", {\"0\", \"0\"}, \"\", 14 );" "liquidAE_LifFloat( \"endDistanceIntensity3\", -1, \"float\", \"endDistanceIntensity3\", {\"0\", \"0\"}, \"\", 14 );" "liquidAE_LifString( \"shadowname\", -1, \"string\", \"shadowname\", {\"0\", \"0\"}, \"\", 14 );" "liquidAE_LifFloat( \"shadowbias\", -1, \"float\", \"shadowbias\", {\"0\", \"0\"}, \"\", 14 );" "liquidAE_LifFloat( \"shadowblur\", -1, \"float\", \"shadowblur\", {\"0\", \"0\"}, \"\", 14 );" "liquidAE_LifFloat( \"shadowsamples\", -1, \"float\", \"shadowsamples\", {\"0\", \"0\"}, \"\", 14 );" "liquidAE_LifFloat( \"shadowfiltersize\", -1, \"float\", \"shadowfiltersize\", {\"0\", \"0\"}, \"\", 14 );" "liquidAE_LifColor( \"shadowcolor\", -1, \"color\", \"shadowcolor\", {\"0\", \"0\"}, \"\", 14 );" "liquidAE_LifColor( \"shadowcolorSurf\", -1, \"color\", \"shadowcolorSurf\", {\"0\", \"0\"}, \"\", 14 );" "liquidAE_LifFloat( \"shadowcolorMix\", -1, \"float\", \"shadowcolorMix\", {\"0\", \"0\"}, \"\", 14 );" "liquidAE_LifFloat( \"lightID\", -1, \"float\", \"lightID\", {\"0\", \"0\"}, \"\", 14 );" "liquidAE_LifString( \"__category\", -1, \"string\", \"__category\", {\"0\", \"0\"}, \"\", 14 );" "liquidAE_LifColor( \"__shadowC\", -1, \"color\", \"__shadowC\", {\"0\", \"0\"}, \"\", 14 );" "liquidAE_LifFloat( \"__shadowF\", -1, \"float\", \"__shadowF\", {\"0\", \"0\"}, \"\", 14 );" "liquidAE_LifFloat( \"__barn\", -1, \"float\", \"__barn\", {\"0\", \"0\"}, \"\", 14 );" "liquidAE_LifColor( \"__unshadowed_Cl\", -1, \"color\", \"__unshadowed_Cl\", {\"0\", \"0\"}, \"\", 14 );" "liquidAE_LifFloat( \"__nondiffuse\", -1, \"float\", \"__nondiffuse\", {\"0\", \"0\"}, \"\", 14 );" "liquidAE_LifFloat( \"__nonspecular\", -1, \"float\", \"__nonspecular\", {\"0\", \"0\"}, \"\", 14 );"  ;
	setAttr ".rmt" -type "stringArray" 1 "light"  ;
	setAttr ".rio" -type "Int32Array" 40 0 0 0 0 0 0
		 0 0 0 0 0 0 0 0 0 0 0 0
		 0 0 0 0 0 0 0 0 0 0 0 0
		 0 0 0 0 1 1 1 1 1 1 ;
	setAttr ".intensity" 1;
	setAttr -k on ".lightcolor" -type "float3" 0 1 0 ;
	setAttr ".coneangle" 0.698132;
	setAttr ".leftbarndoor" 10;
	setAttr ".rightbarndoor" 10;
	setAttr ".topbarndoor" 10;
	setAttr ".bottombarndoor" 10;
	setAttr ".startDistance1" 1;
	setAttr ".endDistance1" 2;
	setAttr ".startDistance2" 3;
	setAttr ".endDistance2" 6;
	setAttr ".startDistance3" 8;
	setAttr ".endDistance3" 10;
	setAttr ".startDistanceIntensity1" 1;
	setAttr ".endDistanceIntensity1" 1;
	setAttr ".startDistanceIntensity2" 1;
	setAttr ".endDistanceIntensity2" 1;
	setAttr ".startDistanceIntensity3" 1;
	setAttr ".endDistanceIntensity3" 1;
	setAttr ".shadowname" -type "string" "";
	setAttr ".shadowbias" 0.01;
	setAttr ".shadowsamples" 32;
	setAttr ".shadowfiltersize" 1;
	setAttr ".shadowcolorMix" -1;
	setAttr ".__category" -type "string" "";
createNode shadingEngine -n "liquidShader2SG";
	setAttr ".ihi" 0;
	setAttr ".ro" yes;
createNode materialInfo -n "materialInfo5";
createNode liquidShader -n "liquidShader3";
	addAttr -ci true -h true -sn "intensity" -ln "intensity" -at "double";
	addAttr -ci true -uac -h true -k true -sn "lightcolor" -ln "lightcolor" -at "float3" 
		-nc 3;
	addAttr -ci true -h true -k true -sn "lightcolora" -ln "lightcolora" -at "float" 
		-p "lightcolor";
	addAttr -ci true -h true -k true -sn "lightcolorb" -ln "lightcolorb" -at "float" 
		-p "lightcolor";
	addAttr -ci true -h true -k true -sn "lightcolorc" -ln "lightcolorc" -at "float" 
		-p "lightcolor";
	addAttr -ci true -h true -sn "coneangle" -ln "coneangle" -at "double";
	addAttr -ci true -h true -sn "penumbraangle" -ln "penumbraangle" -at "double";
	addAttr -ci true -h true -sn "dropoff" -ln "dropoff" -at "double";
	addAttr -ci true -h true -sn "decay" -ln "decay" -at "double";
	addAttr -ci true -h true -sn "barndoors" -ln "barndoors" -at "double";
	addAttr -ci true -h true -sn "leftbarndoor" -ln "leftbarndoor" -at "double";
	addAttr -ci true -h true -sn "rightbarndoor" -ln "rightbarndoor" -at "double";
	addAttr -ci true -h true -sn "topbarndoor" -ln "topbarndoor" -at "double";
	addAttr -ci true -h true -sn "bottombarndoor" -ln "bottombarndoor" -at "double";
	addAttr -ci true -h true -sn "decayRegions" -ln "decayRegions" -at "double";
	addAttr -ci true -h true -sn "startDistance1" -ln "startDistance1" -at "double";
	addAttr -ci true -h true -sn "endDistance1" -ln "endDistance1" -at "double";
	addAttr -ci true -h true -sn "startDistance2" -ln "startDistance2" -at "double";
	addAttr -ci true -h true -sn "endDistance2" -ln "endDistance2" -at "double";
	addAttr -ci true -h true -sn "startDistance3" -ln "startDistance3" -at "double";
	addAttr -ci true -h true -sn "endDistance3" -ln "endDistance3" -at "double";
	addAttr -ci true -h true -sn "startDistanceIntensity1" -ln "startDistanceIntensity1" 
		-at "double";
	addAttr -ci true -h true -sn "endDistanceIntensity1" -ln "endDistanceIntensity1" 
		-at "double";
	addAttr -ci true -h true -sn "startDistanceIntensity2" -ln "startDistanceIntensity2" 
		-at "double";
	addAttr -ci true -h true -sn "endDistanceIntensity2" -ln "endDistanceIntensity2" 
		-at "double";
	addAttr -ci true -h true -sn "startDistanceIntensity3" -ln "startDistanceIntensity3" 
		-at "double";
	addAttr -ci true -h true -sn "endDistanceIntensity3" -ln "endDistanceIntensity3" 
		-at "double";
	addAttr -ci true -h true -sn "shadowname" -ln "shadowname" -dt "string";
	addAttr -ci true -h true -sn "shadowbias" -ln "shadowbias" -at "double";
	addAttr -ci true -h true -sn "shadowblur" -ln "shadowblur" -at "double";
	addAttr -ci true -h true -sn "shadowsamples" -ln "shadowsamples" -at "double";
	addAttr -ci true -h true -sn "shadowfiltersize" -ln "shadowfiltersize" -at "double";
	addAttr -ci true -uac -h true -k true -sn "shadowcolor" -ln "shadowcolor" -at "float3" 
		-nc 3;
	addAttr -ci true -h true -k true -sn "shadowcolora" -ln "shadowcolora" -at "float" 
		-p "shadowcolor";
	addAttr -ci true -h true -k true -sn "shadowcolorb" -ln "shadowcolorb" -at "float" 
		-p "shadowcolor";
	addAttr -ci true -h true -k true -sn "shadowcolorc" -ln "shadowcolorc" -at "float" 
		-p "shadowcolor";
	addAttr -ci true -uac -h true -k true -sn "shadowcolorSurf" -ln "shadowcolorSurf" 
		-at "float3" -nc 3;
	addAttr -ci true -h true -k true -sn "shadowcolorSurfa" -ln "shadowcolorSurfa" -at "float" 
		-p "shadowcolorSurf";
	addAttr -ci true -h true -k true -sn "shadowcolorSurfb" -ln "shadowcolorSurfb" -at "float" 
		-p "shadowcolorSurf";
	addAttr -ci true -h true -k true -sn "shadowcolorSurfc" -ln "shadowcolorSurfc" -at "float" 
		-p "shadowcolorSurf";
	addAttr -ci true -h true -sn "shadowcolorMix" -ln "shadowcolorMix" -at "double";
	addAttr -ci true -h true -sn "lightID" -ln "lightID" -at "double";
	addAttr -ci true -h true -sn "__category" -ln "__category" -dt "string";
	addAttr -ci true -uac -h true -k true -sn "__shadowC" -ln "__shadowC" -at "float3" 
		-nc 3;
	addAttr -ci true -h true -k true -sn "__shadowCa" -ln "__shadowCa" -at "float" -p "__shadowC";
	addAttr -ci true -h true -k true -sn "__shadowCb" -ln "__shadowCb" -at "float" -p "__shadowC";
	addAttr -ci true -h true -k true -sn "__shadowCc" -ln "__shadowCc" -at "float" -p "__shadowC";
	addAttr -ci true -h true -sn "__shadowF" -ln "__shadowF" -at "double";
	addAttr -ci true -h true -sn "__barn" -ln "__barn" -at "double";
	addAttr -ci true -uac -h true -k true -sn "__unshadowed_Cl" -ln "__unshadowed_Cl" 
		-at "float3" -nc 3;
	addAttr -ci true -h true -k true -sn "__unshadowed_Cla" -ln "__unshadowed_Cla" -at "float" 
		-p "__unshadowed_Cl";
	addAttr -ci true -h true -k true -sn "__unshadowed_Clb" -ln "__unshadowed_Clb" -at "float" 
		-p "__unshadowed_Cl";
	addAttr -ci true -h true -k true -sn "__unshadowed_Clc" -ln "__unshadowed_Clc" -at "float" 
		-p "__unshadowed_Cl";
	addAttr -ci true -h true -sn "__nondiffuse" -ln "__nondiffuse" -at "double";
	addAttr -ci true -h true -sn "__nonspecular" -ln "__nonspecular" -at "double";
	addAttr -ci true -sn "liqAssignedObjects" -ln "liqAssignedObjects" -at "message";
	setAttr ".rms" -type "string" "liquidspot";
	setAttr ".rst" -type "string" "light";
	setAttr ".rml" -type "string" "E:/dev/Autodesk/maya/myplugin/project/liquid_2/2.3.3/lib/shaders/liquidspot.slo";
	setAttr ".rpr" -type "stringArray" 40 "intensity" "lightcolor" "coneangle" "penumbraangle" "dropoff" "decay" "barndoors" "leftbarndoor" "rightbarndoor" "topbarndoor" "bottombarndoor" "decayRegions" "startDistance1" "endDistance1" "startDistance2" "endDistance2" "startDistance3" "endDistance3" "startDistanceIntensity1" "endDistanceIntensity1" "startDistanceIntensity2" "endDistanceIntensity2" "startDistanceIntensity3" "endDistanceIntensity3" "shadowname" "shadowbias" "shadowblur" "shadowsamples" "shadowfiltersize" "shadowcolor" "shadowcolorSurf" "shadowcolorMix" "lightID" "__category" "__shadowC" "__shadowF" "__barn" "__unshadowed_Cl" "__nondiffuse" "__nonspecular"  ;
	setAttr ".rdt" -type "stringArray" 40 "uniform" "uniform" "uniform" "uniform" "uniform" "uniform" "uniform" "uniform" "uniform" "uniform" "uniform" "uniform" "uniform" "uniform" "uniform" "uniform" "uniform" "uniform" "uniform" "uniform" "uniform" "uniform" "uniform" "uniform" "uniform" "uniform" "uniform" "uniform" "uniform" "uniform" "varying" "uniform" "uniform" "uniform" "varying" "varying" "varying" "varying" "uniform" "uniform"  ;
	setAttr ".rty" -type "stringArray" 40 "float" "color" "float" "float" "float" "float" "float" "float" "float" "float" "float" "float" "float" "float" "float" "float" "float" "float" "float" "float" "float" "float" "float" "float" "string" "float" "float" "float" "float" "color" "color" "float" "float" "string" "color" "float" "float" "color" "float" "float"  ;
	setAttr ".rdf" -type "stringArray" 40 "1.0" "1.0000001.0000001.000000" "0.698132" "0.0" "0.0" "0.0" "0.0" "10.0" "10.0" "10.0" "10.0" "0.0" "1.0" "2.0" "3.0" "6.0" "8.0" "10.0" "1.0" "1.0" "1.0" "1.0" "1.0" "1.0" "" "0.01" "0.0" "32.0" "1.0" "0.0000000.0000000.000000" "0.0000000.0000000.000000" "-1.0" "0.0" "" "0.0000000.0000000.000000" "0.0" "0.0" "0.0000000.0000000.000000" "0.0" "0.0"  ;
	setAttr ".ras" -type "Int32Array" 40 -1 -1 -1 -1 -1 -1
		 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1
		 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1
		 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 ;
	setAttr ".rlc" -type "stringArray" 40 "liquidAE_LifFloat( \"intensity\", -1, \"float\", \"intensity\", {\"0\", \"0\"}, \"\", 14 );" "liquidAE_LifColor( \"lightcolor\", -1, \"color\", \"lightcolor\", {\"0\", \"0\"}, \"\", 14 );" "liquidAE_LifFloat( \"coneangle\", -1, \"float\", \"coneangle\", {\"0\", \"0\"}, \"\", 14 );" "liquidAE_LifFloat( \"penumbraangle\", -1, \"float\", \"penumbraangle\", {\"0\", \"0\"}, \"\", 14 );" "liquidAE_LifFloat( \"dropoff\", -1, \"float\", \"dropoff\", {\"0\", \"0\"}, \"\", 14 );" "liquidAE_LifFloat( \"decay\", -1, \"float\", \"decay\", {\"0\", \"0\"}, \"\", 14 );" "liquidAE_LifFloat( \"barndoors\", -1, \"float\", \"barndoors\", {\"0\", \"0\"}, \"\", 14 );" "liquidAE_LifFloat( \"leftbarndoor\", -1, \"float\", \"leftbarndoor\", {\"0\", \"0\"}, \"\", 14 );" "liquidAE_LifFloat( \"rightbarndoor\", -1, \"float\", \"rightbarndoor\", {\"0\", \"0\"}, \"\", 14 );" "liquidAE_LifFloat( \"topbarndoor\", -1, \"float\", \"topbarndoor\", {\"0\", \"0\"}, \"\", 14 );" "liquidAE_LifFloat( \"bottombarndoor\", -1, \"float\", \"bottombarndoor\", {\"0\", \"0\"}, \"\", 14 );" "liquidAE_LifFloat( \"decayRegions\", -1, \"float\", \"decayRegions\", {\"0\", \"0\"}, \"\", 14 );" "liquidAE_LifFloat( \"startDistance1\", -1, \"float\", \"startDistance1\", {\"0\", \"0\"}, \"\", 14 );" "liquidAE_LifFloat( \"endDistance1\", -1, \"float\", \"endDistance1\", {\"0\", \"0\"}, \"\", 14 );" "liquidAE_LifFloat( \"startDistance2\", -1, \"float\", \"startDistance2\", {\"0\", \"0\"}, \"\", 14 );" "liquidAE_LifFloat( \"endDistance2\", -1, \"float\", \"endDistance2\", {\"0\", \"0\"}, \"\", 14 );" "liquidAE_LifFloat( \"startDistance3\", -1, \"float\", \"startDistance3\", {\"0\", \"0\"}, \"\", 14 );" "liquidAE_LifFloat( \"endDistance3\", -1, \"float\", \"endDistance3\", {\"0\", \"0\"}, \"\", 14 );" "liquidAE_LifFloat( \"startDistanceIntensity1\", -1, \"float\", \"startDistanceIntensity1\", {\"0\", \"0\"}, \"\", 14 );" "liquidAE_LifFloat( \"endDistanceIntensity1\", -1, \"float\", \"endDistanceIntensity1\", {\"0\", \"0\"}, \"\", 14 );" "liquidAE_LifFloat( \"startDistanceIntensity2\", -1, \"float\", \"startDistanceIntensity2\", {\"0\", \"0\"}, \"\", 14 );" "liquidAE_LifFloat( \"endDistanceIntensity2\", -1, \"float\", \"endDistanceIntensity2\", {\"0\", \"0\"}, \"\", 14 );" "liquidAE_LifFloat( \"startDistanceIntensity3\", -1, \"float\", \"startDistanceIntensity3\", {\"0\", \"0\"}, \"\", 14 );" "liquidAE_LifFloat( \"endDistanceIntensity3\", -1, \"float\", \"endDistanceIntensity3\", {\"0\", \"0\"}, \"\", 14 );" "liquidAE_LifString( \"shadowname\", -1, \"string\", \"shadowname\", {\"0\", \"0\"}, \"\", 14 );" "liquidAE_LifFloat( \"shadowbias\", -1, \"float\", \"shadowbias\", {\"0\", \"0\"}, \"\", 14 );" "liquidAE_LifFloat( \"shadowblur\", -1, \"float\", \"shadowblur\", {\"0\", \"0\"}, \"\", 14 );" "liquidAE_LifFloat( \"shadowsamples\", -1, \"float\", \"shadowsamples\", {\"0\", \"0\"}, \"\", 14 );" "liquidAE_LifFloat( \"shadowfiltersize\", -1, \"float\", \"shadowfiltersize\", {\"0\", \"0\"}, \"\", 14 );" "liquidAE_LifColor( \"shadowcolor\", -1, \"color\", \"shadowcolor\", {\"0\", \"0\"}, \"\", 14 );" "liquidAE_LifColor( \"shadowcolorSurf\", -1, \"color\", \"shadowcolorSurf\", {\"0\", \"0\"}, \"\", 14 );" "liquidAE_LifFloat( \"shadowcolorMix\", -1, \"float\", \"shadowcolorMix\", {\"0\", \"0\"}, \"\", 14 );" "liquidAE_LifFloat( \"lightID\", -1, \"float\", \"lightID\", {\"0\", \"0\"}, \"\", 14 );" "liquidAE_LifString( \"__category\", -1, \"string\", \"__category\", {\"0\", \"0\"}, \"\", 14 );" "liquidAE_LifColor( \"__shadowC\", -1, \"color\", \"__shadowC\", {\"0\", \"0\"}, \"\", 14 );" "liquidAE_LifFloat( \"__shadowF\", -1, \"float\", \"__shadowF\", {\"0\", \"0\"}, \"\", 14 );" "liquidAE_LifFloat( \"__barn\", -1, \"float\", \"__barn\", {\"0\", \"0\"}, \"\", 14 );" "liquidAE_LifColor( \"__unshadowed_Cl\", -1, \"color\", \"__unshadowed_Cl\", {\"0\", \"0\"}, \"\", 14 );" "liquidAE_LifFloat( \"__nondiffuse\", -1, \"float\", \"__nondiffuse\", {\"0\", \"0\"}, \"\", 14 );" "liquidAE_LifFloat( \"__nonspecular\", -1, \"float\", \"__nonspecular\", {\"0\", \"0\"}, \"\", 14 );"  ;
	setAttr ".rmt" -type "stringArray" 1 "light"  ;
	setAttr ".rio" -type "Int32Array" 40 0 0 0 0 0 0
		 0 0 0 0 0 0 0 0 0 0 0 0
		 0 0 0 0 0 0 0 0 0 0 0 0
		 0 0 0 0 1 1 1 1 1 1 ;
	setAttr ".intensity" 1;
	setAttr -k on ".lightcolor" -type "float3" 0 0 1 ;
	setAttr ".coneangle" 0.698132;
	setAttr ".leftbarndoor" 10;
	setAttr ".rightbarndoor" 10;
	setAttr ".topbarndoor" 10;
	setAttr ".bottombarndoor" 10;
	setAttr ".startDistance1" 1;
	setAttr ".endDistance1" 2;
	setAttr ".startDistance2" 3;
	setAttr ".endDistance2" 6;
	setAttr ".startDistance3" 8;
	setAttr ".endDistance3" 10;
	setAttr ".startDistanceIntensity1" 1;
	setAttr ".endDistanceIntensity1" 1;
	setAttr ".startDistanceIntensity2" 1;
	setAttr ".endDistanceIntensity2" 1;
	setAttr ".startDistanceIntensity3" 1;
	setAttr ".endDistanceIntensity3" 1;
	setAttr ".shadowname" -type "string" "";
	setAttr ".shadowbias" 0.01;
	setAttr ".shadowsamples" 32;
	setAttr ".shadowfiltersize" 1;
	setAttr ".shadowcolorMix" -1;
	setAttr ".__category" -type "string" "";
createNode shadingEngine -n "liquidShader3SG";
	setAttr ".ihi" 0;
	setAttr ".ro" yes;
createNode materialInfo -n "materialInfo6";
select -ne :time1;
	setAttr ".o" 1;
	setAttr ".unw" 1;
select -ne :renderPartition;
	setAttr -s 8 ".st";
select -ne :initialShadingGroup;
	setAttr ".ro" yes;
select -ne :initialParticleSE;
	setAttr ".ro" yes;
select -ne :defaultShaderList1;
	setAttr -s 5 ".s";
select -ne :lightList1;
	setAttr -s 3 ".l";
select -ne :postProcessList1;
	setAttr -s 2 ".p";
select -ne :defaultRenderingList1;
select -ne :renderGlobalsList1;
select -ne :defaultRenderGlobals;
	setAttr ".ren" -type "string" "liquid";
select -ne :defaultResolution;
	setAttr ".pa" 1;
select -ne :defaultLightSet;
	setAttr -s 3 ".dsm";
select -ne :hardwareRenderGlobals;
	setAttr ".ctrs" 256;
	setAttr ".btrs" 512;
select -ne :defaultHardwareRenderGlobals;
	setAttr ".fn" -type "string" "im";
	setAttr ".res" -type "string" "ntsc_4d 646 485 1.333";
connectAttr "polyPlane1.out" "pPlaneShape1.i";
connectAttr "liquidLight3.liqAssignedObjects" "spotLightShape1.liquidLightShaderNode"
		;
connectAttr "liquidShader2.liqAssignedObjects" "spotLightShape2.liquidLightShaderNode"
		;
relationship "link" ":lightLinker1" ":initialShadingGroup.message" ":defaultLightSet.message";
relationship "link" ":lightLinker1" ":initialParticleSE.message" ":defaultLightSet.message";
relationship "link" ":lightLinker1" "liquidLight1SG.message" ":defaultLightSet.message";
relationship "link" ":lightLinker1" "liquidLight2SG.message" ":defaultLightSet.message";
relationship "link" ":lightLinker1" "liquidLight3SG.message" ":defaultLightSet.message";
relationship "link" ":lightLinker1" "liquidShader1SG.message" ":defaultLightSet.message";
relationship "link" ":lightLinker1" "liquidShader2SG.message" ":defaultLightSet.message";
relationship "link" ":lightLinker1" "liquidShader3SG.message" ":defaultLightSet.message";
relationship "shadowLink" ":lightLinker1" ":initialShadingGroup.message" ":defaultLightSet.message";
relationship "shadowLink" ":lightLinker1" ":initialParticleSE.message" ":defaultLightSet.message";
relationship "shadowLink" ":lightLinker1" "liquidLight1SG.message" ":defaultLightSet.message";
relationship "shadowLink" ":lightLinker1" "liquidLight2SG.message" ":defaultLightSet.message";
relationship "shadowLink" ":lightLinker1" "liquidLight3SG.message" ":defaultLightSet.message";
relationship "shadowLink" ":lightLinker1" "liquidShader1SG.message" ":defaultLightSet.message";
relationship "shadowLink" ":lightLinker1" "liquidShader2SG.message" ":defaultLightSet.message";
relationship "shadowLink" ":lightLinker1" "liquidShader3SG.message" ":defaultLightSet.message";
connectAttr "layerManager.dli[0]" "defaultLayer.id";
connectAttr "renderLayerManager.rlmi[0]" "defaultRenderLayer.rlid";
connectAttr "liquidLight1SG.msg" "materialInfo1.sg";
connectAttr "liquidLight2SG.msg" "materialInfo2.sg";
connectAttr "liquidLight3.oc" "liquidLight3SG.ss";
connectAttr "liquidLight3SG.msg" "materialInfo3.sg";
connectAttr "liquidLight3.msg" "materialInfo3.m";
connectAttr "liquidLight3.msg" "materialInfo3.t" -na;
connectAttr "liquidShader1SG.msg" "materialInfo4.sg";
connectAttr "liquidShader2.oc" "liquidShader2SG.ss";
connectAttr "liquidShader2SG.msg" "materialInfo5.sg";
connectAttr "liquidShader2.msg" "materialInfo5.m";
connectAttr "liquidShader2.msg" "materialInfo5.t" -na;
connectAttr "liquidShader3.oc" "liquidShader3SG.ss";
connectAttr "liquidShader3SG.msg" "materialInfo6.sg";
connectAttr "liquidShader3.msg" "materialInfo6.m";
connectAttr "liquidShader3.msg" "materialInfo6.t" -na;
connectAttr "liquidLight1SG.pa" ":renderPartition.st" -na;
connectAttr "liquidLight2SG.pa" ":renderPartition.st" -na;
connectAttr "liquidLight3SG.pa" ":renderPartition.st" -na;
connectAttr "liquidShader1SG.pa" ":renderPartition.st" -na;
connectAttr "liquidShader2SG.pa" ":renderPartition.st" -na;
connectAttr "liquidShader3SG.pa" ":renderPartition.st" -na;
connectAttr "pPlaneShape1.iog" ":initialShadingGroup.dsm" -na;
connectAttr "liquidLight3.msg" ":defaultShaderList1.s" -na;
connectAttr "liquidShader2.msg" ":defaultShaderList1.s" -na;
connectAttr "liquidShader3.msg" ":defaultShaderList1.s" -na;
connectAttr "spotLightShape1.ltd" ":lightList1.l" -na;
connectAttr "pointLightShape1.ltd" ":lightList1.l" -na;
connectAttr "spotLightShape2.ltd" ":lightList1.l" -na;
connectAttr "defaultRenderLayer.msg" ":defaultRenderingList1.r" -na;
connectAttr "spotLight1.iog" ":defaultLightSet.dsm" -na;
connectAttr "pointLight1.iog" ":defaultLightSet.dsm" -na;
connectAttr "spotLight2.iog" ":defaultLightSet.dsm" -na;
// End of rm_spotlight2.ma
