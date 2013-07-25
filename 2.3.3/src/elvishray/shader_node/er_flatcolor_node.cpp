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
** Contributor(s): Philippe Leprince.
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
** Liquid Surface Shader Node Source
** ______________________________________________________________________
*/
#include "er_flatcolor_node.h"

#include <common/mayacheck.h>
#include <liqlog.h>
#include <liqNodeSwatch.h>
#include <liqIOStream.h>
#include <liqNodeSwatch.h>
#include "../er_nodeId.h"

namespace elvishray
{

// static data
// Attributes
MObject FlatColorNode::aRmanShader;
MObject FlatColorNode::aRmanShaderType;
MObject FlatColorNode::aRmanShaderLong;
MObject FlatColorNode::aRmanShaderLif;
MObject FlatColorNode::aRmanParams;
MObject FlatColorNode::aRmanDetails;
MObject FlatColorNode::aRmanTypes;
MObject FlatColorNode::aRmanDefaults;
MObject FlatColorNode::aRmanArraySizes;
MObject FlatColorNode::aRmanLifCmds;
MObject FlatColorNode::aRmanMethods;
MObject FlatColorNode::aRmanIsOutput;  
MObject FlatColorNode::aRmanAccept;

// MObject FlatColorNode::aPreviewPrimitive;
// MObject FlatColorNode::aPreviewCustomPrimitive;
// MObject FlatColorNode::aPreviewCustomBackplane;
// MObject FlatColorNode::aPreviewCustomLightRig;
// MObject FlatColorNode::aColor;
// MObject FlatColorNode::aTransparency;
// MObject FlatColorNode::aOpacity;
// MObject FlatColorNode::aShaderSpace;
// MObject FlatColorNode::aDisplacementBound;
// MObject FlatColorNode::aDisplacementBoundSpace;
// MObject FlatColorNode::aOutputInShadow;
// 
// //MObject FlatColorNode::aResolution;
// MObject FlatColorNode::aRefreshPreview;
// MObject FlatColorNode::aPreviewObjectSize;
// MObject FlatColorNode::aPreviewShadingRate;
// MObject FlatColorNode::aPreviewBackplane;
// MObject FlatColorNode::aPreviewIntensity;
// MObject FlatColorNode::aGLPreviewTexture;
// //MObject FlatColorNode::aCi;
// //MObject FlatColorNode::aOi;
// 
// MObject FlatColorNode::aMayaIgnoreLights;
// MObject FlatColorNode::aMayaKa;
// MObject FlatColorNode::aMayaKd;
// MObject FlatColorNode::aNormalCameraX;
// MObject FlatColorNode::aNormalCameraY;
// MObject FlatColorNode::aNormalCameraZ;
// MObject FlatColorNode::aNormalCamera;
// MObject FlatColorNode::aLightDirectionX;
// MObject FlatColorNode::aLightDirectionY;
// MObject FlatColorNode::aLightDirectionZ;
// MObject FlatColorNode::aLightDirection;
// MObject FlatColorNode::aLightIntensityR;
// MObject FlatColorNode::aLightIntensityG;
// MObject FlatColorNode::aLightIntensityB;
// MObject FlatColorNode::aLightIntensity;
// MObject FlatColorNode::aLightAmbient;
// MObject FlatColorNode::aLightDiffuse;
// MObject FlatColorNode::aLightSpecular;
// MObject FlatColorNode::aLightShadowFraction;
// MObject FlatColorNode::aPreShadowIntensity;
// MObject FlatColorNode::aLightBlindData;
// MObject FlatColorNode::aLightData;
// 
// MObject FlatColorNode::aEvaluateAtEveryFrame;
// MObject FlatColorNode::aPreviewGamma;
// 
MObject FlatColorNode::aOutColor;
MObject FlatColorNode::aOutTransparency;

MObject FlatColorNode::acolor;
MObject FlatColorNode::atransparency;

#define MAKE_INPUT(attr)		\
	IfMErrorWarn(attr.setKeyable(true ) ); 		\
	IfMErrorWarn(attr.setStorable(true ) );		\
	IfMErrorWarn(attr.setReadable(true ) ); 		\
	IfMErrorWarn(attr.setWritable(true ) );

#define MAKE_NONKEYABLE_INPUT(attr)		\
	IfMErrorWarn(attr.setKeyable(false ) ); 		\
	IfMErrorWarn(attr.setStorable(true ) );		\
	IfMErrorWarn(attr.setReadable(true ) ); 		\
	IfMErrorWarn(attr.setWritable(true ) );

#define MAKE_OUTPUT(attr)		\
	IfMErrorWarn(attr.setKeyable(false ) ); 		\
	IfMErrorWarn(attr.setStorable(false ) );		\
	IfMErrorWarn(attr.setReadable(true ) ); 		\
	IfMErrorWarn(attr.setWritable(false ) );

void FlatColorNode::postConstructor( )
{
	setMPSafe(true);

	// init swatch
	if ( swatchInit != true ) {
		MObject obj = MPxNode::thisMObject();
		renderSwatch = new liqNodeSwatch( obj, obj, 128 );
		swatchInit = true;
	}

	MGlobal::executeCommandOnIdle( "liquidCheckGlobals()", false );
}

FlatColorNode::FlatColorNode()
{
	swatchInit = false;
	renderSwatch = NULL;
}

FlatColorNode::~FlatColorNode()
{
	if (renderSwatch != NULL) 
		delete renderSwatch;
}

const MTypeId FlatColorNode::getTypeId()
{ 
	return TI_FlatColor_NODE;
}

const MString FlatColorNode::getTypeName()
{ 
	return ("er_flatcolor");
}

void* FlatColorNode::creator()
{
	return new FlatColorNode();
}

MStatus FlatColorNode::initialize()
{
	MFnTypedAttribute   tAttr;
	MFnStringData       tDefault;
	MFnNumericAttribute nAttr;
	MFnEnumAttribute    eAttr;
	MFnLightDataAttribute lAttr;
	MStatus status;

	// Create input attributes

	aRmanShader = tAttr.create( MString("rmanShader"), MString("rms"), MFnData::kString, aRmanShader, &status );
	MAKE_INPUT(tAttr);

	aRmanShaderType = tAttr.create( MString("rmanShaderType"), MString("rst"), MFnData::kString, aRmanShaderType, &status );
	MAKE_INPUT(tAttr);

	aRmanShaderLong = tAttr.create( MString("rmanShaderLong"), MString("rml"), MFnData::kString, aRmanShaderLong, &status );
	MAKE_INPUT(tAttr);

	aRmanShaderLif = tAttr.create(  MString("rmanShaderLif"),  MString("lif"), MFnData::kString, aRmanShaderLif, &status );
	MAKE_INPUT(tAttr);

	aRmanParams = tAttr.create(  MString("rmanParams"),  MString("rpr"), MFnData::kStringArray, aRmanParams, &status );
	MAKE_INPUT(tAttr);

	aRmanDetails = tAttr.create(  MString("rmanDetails"),  MString("rdt"), MFnData::kStringArray, aRmanDetails, &status );
	MAKE_INPUT(tAttr);

	aRmanTypes = tAttr.create(  MString("rmanTypes"),  MString("rty"), MFnData::kStringArray, aRmanTypes, &status );
	MAKE_INPUT(tAttr);

	aRmanDefaults = tAttr.create(  MString("rmanDefaults"),  MString("rdf"), MFnData::kStringArray, aRmanDefaults, &status );
	MAKE_INPUT(tAttr);

	aRmanArraySizes = tAttr.create(  MString("rmanArraySizes"),  MString("ras"), MFnData::kIntArray, aRmanArraySizes, &status );
	MAKE_INPUT(tAttr);

	aRmanLifCmds = tAttr.create(  MString("rmanLifCmds"),  MString("rlc"), MFnData::kStringArray, aRmanLifCmds, &status );
	MAKE_INPUT(tAttr);

	aRmanMethods = tAttr.create(  MString("rmanMethods"),  MString("rmt"), MFnData::kStringArray, aRmanMethods, &status );
	MAKE_INPUT(tAttr);

	aRmanIsOutput = tAttr.create(  MString("rmanIsOutput"),  MString("rio"), MFnData::kIntArray, aRmanIsOutput, &status );
	MAKE_INPUT(tAttr);

	aRmanAccept = tAttr.create(  MString("rmanAccept"),  MString("rma"), MFnData::kStringArray, aRmanAccept, &status );
	MAKE_INPUT(tAttr);

// 	aPreviewPrimitive = eAttr.create( "previewPrimitive", "pvp", 7, &status );
// 	eAttr.addField( "Sphere",   0 );
// 	eAttr.addField( "Cube",     1 );
// 	eAttr.addField( "Cylinder", 2 );
// 	eAttr.addField( "Torus",    3 );
// 	eAttr.addField( "Plane",    4 );
// 	eAttr.addField( "Teapot",   5 );
// 	eAttr.addField( "Custom",   6 );
// 	eAttr.addField( "(globals)",7 );
// 	MAKE_NONKEYABLE_INPUT(eAttr);
// 	IfMErrorWarn(eAttr.setConnectable(false ) );
// 
// 	aPreviewCustomPrimitive = tAttr.create(  MString("previewCustomPrimitive"),  MString("pcp"), MFnData::kString, aPreviewCustomPrimitive, &status );
// 	MAKE_INPUT(tAttr);
// 
// 	aPreviewCustomBackplane = tAttr.create(  MString("previewCustomBackplane"),  MString("pcb"), MFnData::kString, aPreviewCustomBackplane, &status );
// 	MAKE_INPUT(tAttr);
// 
// 	aPreviewCustomLightRig = tAttr.create(  MString("previewCustomLights"),  MString("pcl"), MFnData::kString, aPreviewCustomLightRig, &status );
// 	MAKE_INPUT(tAttr);

// 	aPreviewObjectSize = nAttr.create("previewObjectSize", "pos", MFnNumericData::kDouble, 1.0, &status);
// 	MAKE_NONKEYABLE_INPUT(nAttr);
// 	IfMErrorWarn(nAttr.setConnectable(false ) );
// 
// 	aPreviewShadingRate = nAttr.create("previewShadingRate", "psr", MFnNumericData::kDouble, 1.0, &status);
// 	MAKE_NONKEYABLE_INPUT(nAttr);
// 	IfMErrorWarn(nAttr.setConnectable(false ) );
// 
// 	aPreviewBackplane = nAttr.create("previewBackplane", "pbp", MFnNumericData::kBoolean, true, &status);
// 	MAKE_NONKEYABLE_INPUT(nAttr);
// 	IfMErrorWarn(nAttr.setConnectable(false ) );
// 
// 	aPreviewIntensity = nAttr.create("previewIntensity", "pi", MFnNumericData::kDouble, 1.0, &status);
// 	MAKE_NONKEYABLE_INPUT(nAttr);
// 	IfMErrorWarn(nAttr.setConnectable(false ) );
// 
// 	aGLPreviewTexture = nAttr.createColor("GLPreviewTexture", "gpt");
// 	nAttr.setDefault( -1.0, -1.0, -1.0 );
// 	nAttr.setDisconnectBehavior( MFnAttribute::kReset );
// 	MAKE_INPUT(nAttr);
// 
// 	aColor = nAttr.createColor("liqcolor", "cs");
// 	nAttr.setDefault( 1.0, 1.0, 1.0 );
// 	nAttr.setDisconnectBehavior( MFnAttribute::kReset );
// 	MAKE_INPUT(nAttr);
//	
// 	aOpacity = nAttr.createColor("liqopacity", "os");
// 	nAttr.setDefault( 1.0, 1.0, 1.0 );
// 	MAKE_INPUT(nAttr);
// 
// 	aTransparency = nAttr.createColor("transparency", "ts"); // Needed by Maya for Open Gl preview in "5" mode, invert opacity in compute
// 	nAttr.setDefault( 0.0, 0.0, 0.0 );
// 	MAKE_INPUT(nAttr);
// 
// 	aShaderSpace = tAttr.create( MString("shaderSpace"), MString("ssp"), MFnData::kString, aShaderSpace, &status );
// 	MAKE_INPUT(tAttr);
// 
// 	aDisplacementBound = nAttr.create("displacementBound", "db", MFnNumericData::kDouble, 0.0, &status);
// 	MAKE_INPUT(nAttr);
// 
// 	MObject defaultSpaceObj = tDefault.create( MString("shader"), &status);
// 	aDisplacementBoundSpace = tAttr.create( MString("displacementBoundSpace"), MString("dbs"), MFnData::kString, defaultSpaceObj, &status );
// 	MAKE_INPUT(tAttr);
// 
// 	aOutputInShadow = nAttr.create("outputInShadow", "ois",  MFnNumericData::kBoolean, 0.0, &status);
// 	MAKE_NONKEYABLE_INPUT(nAttr);

// 	// resolution attribute for maya's hardware renderer
// 	aResolution = nAttr.create("resolution", "res",  MFnNumericData::kInt, 32, &status);
// 	IfMErrorWarn(nAttr.setStorable( true ) );
// 	IfMErrorWarn(nAttr.setReadable( true ) );
// 	IfMErrorWarn(nAttr.setWritable( true ) );
// 	IfMErrorWarn(nAttr.setHidden( true ) );



//	// refreshPreview must be true to allow refresh
//	aRefreshPreview = nAttr.create("refreshPreview", "rfp",  MFnNumericData::kBoolean, 0.0, &status);
//	MAKE_NONKEYABLE_INPUT(nAttr);
//	IfMErrorWarn(nAttr.setHidden(true ) );

	// dynamic shader attr
	//aCi = nAttr.createColor("Ci", "ci");
	//nAttr.setDefault( 1.0, 1.0, 1.0 );
	//MAKE_INPUT(nAttr);

	//aOi = nAttr.createColor("Oi", "oi");
	//nAttr.setDefault( 1.0, 1.0, 1.0 );
	//MAKE_INPUT(nAttr);


	// create attributes for maya renderer

	// lambertian control
// 	aMayaIgnoreLights = nAttr.create("mayaIgnoreLights", "mil",  MFnNumericData::kBoolean, 0.0, &status);
// 	MAKE_INPUT(nAttr);
// 	aMayaKa = nAttr.create("mayaKa", "mka", MFnNumericData::kFloat, 0.2, &status);
// 	MAKE_INPUT(nAttr);
// 	aMayaKd = nAttr.create("mayaKd", "mkd", MFnNumericData::kFloat, 0.8, &status);
// 	MAKE_INPUT(nAttr);

	// Camera Normals
// 	aNormalCameraX = nAttr.create( "normalCameraX", "nx", MFnNumericData::kFloat, 0, &status );
// 	IfMErrorWarn( status );
// 	IfMErrorWarn( nAttr.setStorable( false ) );
// 	IfMErrorWarn( nAttr.setDefault( 1.0f ) );
// 	aNormalCameraY = nAttr.create( "normalCameraY", "ny", MFnNumericData::kFloat, 0, &status );
// 	IfMErrorWarn( status );
// 	IfMErrorWarn( nAttr.setStorable( false ) );
// 	IfMErrorWarn( nAttr.setDefault( 1.0f ) );
// 	aNormalCameraZ = nAttr.create( "normalCameraZ", "nz", MFnNumericData::kFloat, 0, &status );
// 	IfMErrorWarn( status );
// 	IfMErrorWarn( nAttr.setStorable( false ) );
// 	IfMErrorWarn( nAttr.setDefault( 1.0f ) );
// 	aNormalCamera = nAttr.create( "normalCamera","n", aNormalCameraX, aNormalCameraY, aNormalCameraZ, &status );
// 	IfMErrorWarn( status );
// 	IfMErrorWarn( nAttr.setStorable( false ) );
// 	IfMErrorWarn( nAttr.setDefault( 1.0f, 1.0f, 1.0f ) );
// 	IfMErrorWarn( nAttr.setHidden( true ) );

	// Light Direction
// 	aLightDirectionX = nAttr.create( "lightDirectionX", "ldx", MFnNumericData::kFloat, 0, &status );
// 	IfMErrorWarn( status );
// 	IfMErrorWarn( nAttr.setStorable( false ) );
// 	IfMErrorWarn( nAttr.setHidden( true ) );
// 	IfMErrorWarn( nAttr.setReadable( false ) );
// 	IfMErrorWarn( nAttr.setDefault( 1.0f ) );
// 	aLightDirectionY = nAttr.create( "lightDirectionY", "ldy", MFnNumericData::kFloat, 0, &status );
// 	IfMErrorWarn( status );
// 	IfMErrorWarn( nAttr.setStorable( false ) );
// 	IfMErrorWarn( nAttr.setHidden( true ) );
// 	IfMErrorWarn( nAttr.setReadable( false ) );
// 	IfMErrorWarn( nAttr.setDefault( 1.0f ) );
// 	aLightDirectionZ = nAttr.create( "lightDirectionZ", "ldz", MFnNumericData::kFloat, 0, &status );
// 	IfMErrorWarn( status );
// 	IfMErrorWarn( nAttr.setStorable( false ) );
// 	IfMErrorWarn( nAttr.setHidden( true ) );
// 	IfMErrorWarn( nAttr.setReadable( false ) );
// 	IfMErrorWarn( nAttr.setDefault( 1.0f ) );
// 	aLightDirection = nAttr.create( "lightDirection", "ld", aLightDirectionX, aLightDirectionY, aLightDirectionZ, &status );
// 	IfMErrorWarn( status );
// 	IfMErrorWarn( nAttr.setStorable( false ) );
// 	IfMErrorWarn( nAttr.setHidden( true ) );
// 	IfMErrorWarn( nAttr.setReadable( false ) );
// 	IfMErrorWarn( nAttr.setDefault( 1.0f, 1.0f, 1.0f ) );

	// Light Intensity
// 	aLightIntensityR = nAttr.create( "lightIntensityR", "lir", MFnNumericData::kFloat, 0, &status );
// 	IfMErrorWarn( status );
// 	IfMErrorWarn( nAttr.setStorable( false ) );
// 	IfMErrorWarn( nAttr.setHidden( true ) );
// 	IfMErrorWarn( nAttr.setReadable( false ) );
// 	IfMErrorWarn( nAttr.setDefault( 1.0f ) );
// 	aLightIntensityG = nAttr.create( "lightIntensityG", "lig", MFnNumericData::kFloat, 0, &status );
// 	IfMErrorWarn( status );
// 	IfMErrorWarn( nAttr.setStorable( false ) );
// 	IfMErrorWarn( nAttr.setHidden( true ) );
// 	IfMErrorWarn( nAttr.setReadable( false ) );
// 	IfMErrorWarn( nAttr.setDefault( 1.0f ) );
// 	aLightIntensityB = nAttr.create( "lightIntensityB", "lib", MFnNumericData::kFloat, 0, &status );
// 	IfMErrorWarn( status );
// 	IfMErrorWarn( nAttr.setStorable( false ) );
// 	IfMErrorWarn( nAttr.setHidden( true ) );
// 	IfMErrorWarn( nAttr.setReadable( false ) );
// 	IfMErrorWarn( nAttr.setDefault( 1.0f ) );
// 	aLightIntensity = nAttr.create( "lightIntensity", "li", aLightIntensityR, aLightIntensityG, aLightIntensityB, &status );
// 	IfMErrorWarn( status );
// 	IfMErrorWarn( nAttr.setStorable( false ) );
// 	IfMErrorWarn( nAttr.setHidden( true ) );
// 	IfMErrorWarn( nAttr.setReadable( false ) );
// 	IfMErrorWarn( nAttr.setDefault( 1.0f, 1.0f, 1.0f ) );

	// Light
// 	aLightAmbient = nAttr.create( "lightAmbient", "la", MFnNumericData::kBoolean, 0, &status );
// 	IfMErrorWarn( status );
// 	IfMErrorWarn( nAttr.setStorable( false ) );
// 	IfMErrorWarn( nAttr.setHidden( true ) );
// 	IfMErrorWarn( nAttr.setReadable( false ) );
// 	IfMErrorWarn( nAttr.setDefault( true ) );
// 	aLightDiffuse = nAttr.create( "lightDiffuse", "ldf", MFnNumericData::kBoolean, 0, &status );
// 	IfMErrorWarn( status );
// 	IfMErrorWarn( nAttr.setStorable( false ) );
// 	IfMErrorWarn( nAttr.setHidden( true ) );
// 	IfMErrorWarn( nAttr.setReadable( false ) );
// 	IfMErrorWarn( nAttr.setDefault( true ) );
// 	aLightSpecular = nAttr.create( "lightSpecular", "ls", MFnNumericData::kBoolean, 0, &status );
// 	IfMErrorWarn( status );
// 	IfMErrorWarn( nAttr.setStorable( false ) );
// 	IfMErrorWarn( nAttr.setHidden( true ) );
// 	IfMErrorWarn( nAttr.setReadable( false ) );
// 	IfMErrorWarn( nAttr.setDefault( false ) );
// 	aLightShadowFraction = nAttr.create( "lightShadowFraction", "lsf", MFnNumericData::kFloat, 0, &status );
// 	IfMErrorWarn( status );
// 	IfMErrorWarn( nAttr.setStorable( false ) );
// 	IfMErrorWarn( nAttr.setReadable( true ) );
// 	IfMErrorWarn( nAttr.setWritable( true ) );
// 	IfMErrorWarn( nAttr.setDefault( 1.0f ) );
// 	aPreShadowIntensity = nAttr.create( "preShadowIntensity", "psi", MFnNumericData::kFloat, 0, &status );
// 	IfMErrorWarn( status );
// 	IfMErrorWarn( nAttr.setStorable( false ) );
// 	IfMErrorWarn( nAttr.setHidden( true ) );
// 	IfMErrorWarn( nAttr.setReadable( false ) );
// 	IfMErrorWarn( nAttr.setDefault( 1.0f ) );
// 
// #if MAYA_API_VERSION >= 800
// 	aLightBlindData = nAttr.createAddr( "lightBlindData", "lbld", 0, &status );
// 	IfMErrorWarn( status );
// 	IfMErrorWarn( nAttr.setStorable( false ) );
// 	IfMErrorWarn( nAttr.setHidden( true ) );
// 	IfMErrorWarn( nAttr.setReadable( false ) );
// 	IfMErrorWarn( nAttr.setDefault( (void*) 0 ) );
// #else
// 	aLightBlindData = nAttr.create( "lightBlindData", "lbld", MFnNumericData::kLong, 0, &status );
// 	IfMErrorWarn( status );
// 	IfMErrorWarn( nAttr.setStorable( false ) );
// 	IfMErrorWarn( nAttr.setHidden( true ) );
// 	IfMErrorWarn( nAttr.setReadable( false ) );
// 	IfMErrorWarn( nAttr.setDefault( 0 ) );
// #endif
// 	aLightData = lAttr.create( "lightDataArray", "ltd", aLightDirection,
// 		aLightIntensity, aLightAmbient, aLightDiffuse, aLightSpecular,
// 		aLightShadowFraction, aPreShadowIntensity, aLightBlindData,
// 		&status );
// 	IfMErrorWarn( status );
// 	IfMErrorWarn( lAttr.setArray( true ) );
// 	IfMErrorWarn( lAttr.setStorable( false ) );
// 	IfMErrorWarn( lAttr.setHidden( true ) );
// 	IfMErrorWarn( lAttr.setDefault( 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, true, true, false, 1.0f, 1.0f, 0 ) );
// 
// 	aEvaluateAtEveryFrame = nAttr.create("evaluateAtEveryFrame", "def",  MFnNumericData::kBoolean, 0.0, &status);
// 	MAKE_NONKEYABLE_INPUT(nAttr);
// 
// 	aPreviewGamma = nAttr.create( "previewGamma", "pg", MFnNumericData::kFloat, 1, &status );
// 	IfMErrorWarn( status );
// 	IfMErrorWarn( nAttr.setStorable( true ) );
// 	IfMErrorWarn( nAttr.setHidden( true ) );
// 	IfMErrorWarn( nAttr.setReadable( true ) );
// 	IfMErrorWarn( nAttr.setDefault( 1.0f ) );

	// Create output attributes
	aOutColor = nAttr.createColor("outColor", "oc");
	MAKE_OUTPUT(nAttr);
	aOutTransparency = nAttr.createColor("outTransparency", "ot");
	MAKE_OUTPUT(nAttr);

	IfMErrorWarn( addAttribute( aRmanShader ) );
	IfMErrorWarn( addAttribute( aRmanShaderType ) );
	IfMErrorWarn( addAttribute( aRmanShaderLong ) );
	IfMErrorWarn( addAttribute( aRmanShaderLif ) );
	IfMErrorWarn( addAttribute( aRmanParams ) );
	IfMErrorWarn( addAttribute( aRmanDetails ) );
	IfMErrorWarn( addAttribute( aRmanTypes ) );
	IfMErrorWarn( addAttribute( aRmanDefaults ) );
	IfMErrorWarn( addAttribute( aRmanArraySizes ) );
	IfMErrorWarn( addAttribute( aRmanLifCmds ) );
	IfMErrorWarn( addAttribute( aRmanMethods) );
	IfMErrorWarn( addAttribute( aRmanIsOutput) );
	IfMErrorWarn( addAttribute( aRmanAccept) );

// 	IfMErrorWarn( addAttribute( aPreviewPrimitive ) );
// 	IfMErrorWarn( addAttribute( aPreviewCustomPrimitive ) );
// 	IfMErrorWarn( addAttribute( aPreviewCustomBackplane ) );
// 	IfMErrorWarn( addAttribute( aPreviewCustomLightRig ) );
// 	IfMErrorWarn( addAttribute( aPreviewObjectSize ) );
// 	IfMErrorWarn( addAttribute( aPreviewShadingRate ) );
// 	IfMErrorWarn( addAttribute( aPreviewBackplane ) );
// 	IfMErrorWarn( addAttribute( aPreviewIntensity ) );
// 	IfMErrorWarn(addAttribute(aGLPreviewTexture));
	//IfMErrorWarn( addAttribute( aCi ) );
	//IfMErrorWarn( addAttribute( aOi ) );

// 	IfMErrorWarn( addAttribute( aColor ) );
// 	IfMErrorWarn( addAttribute( aTransparency ) );
// 	IfMErrorWarn( addAttribute( aOpacity ) );
// 	IfMErrorWarn( addAttribute( aShaderSpace ) );
// 	IfMErrorWarn( addAttribute( aDisplacementBound ) );
// 	IfMErrorWarn( addAttribute( aDisplacementBoundSpace ) );
// 	IfMErrorWarn( addAttribute( aOutputInShadow ) );

//	IfMErrorWarn( addAttribute( aResolution ) );
// 	IfMErrorWarn( addAttribute( aRefreshPreview ) );
// 	IfMErrorWarn( addAttribute( aMayaIgnoreLights ) );
// 	IfMErrorWarn( addAttribute( aMayaKa ) );
// 	IfMErrorWarn( addAttribute( aMayaKd ) );
// 	IfMErrorWarn( addAttribute( aNormalCamera ) );
// 	IfMErrorWarn( addAttribute( aLightData ) );

//	IfMErrorWarn( addAttribute( aEvaluateAtEveryFrame ) );
//	IfMErrorWarn( addAttribute( aPreviewGamma ) );

	IfMErrorWarn( addAttribute( aOutColor ) );
	IfMErrorWarn( addAttribute( aOutTransparency ) );
// 	IfMErrorWarn(attributeAffects( aGLPreviewTexture,    aOutColor ));
// 	IfMErrorWarn(attributeAffects( aColor,               aOutColor ) );
// 	IfMErrorWarn(attributeAffects( aOpacity,             aOutColor ) );
// 	IfMErrorWarn(attributeAffects( aMayaIgnoreLights,    aOutColor ) );
// 	IfMErrorWarn(attributeAffects( aMayaKa,              aOutColor ) );
// 	IfMErrorWarn(attributeAffects( aMayaKd,              aOutColor ) );
// 	IfMErrorWarn(attributeAffects( aLightIntensityR,     aOutColor ) );
// 	IfMErrorWarn(attributeAffects( aLightIntensityB,     aOutColor ) );
// 	IfMErrorWarn(attributeAffects( aLightIntensityG,     aOutColor ) );
// 	IfMErrorWarn(attributeAffects( aLightIntensity,      aOutColor ) );
// 	IfMErrorWarn(attributeAffects( aNormalCameraX,       aOutColor ) );
// 	IfMErrorWarn(attributeAffects( aNormalCameraY,       aOutColor ) );
// 	IfMErrorWarn(attributeAffects( aNormalCameraZ,       aOutColor ) );
// 	IfMErrorWarn(attributeAffects( aNormalCamera,        aOutColor ) );
// 	IfMErrorWarn(attributeAffects( aLightDirectionX,     aOutColor ) );
// 	IfMErrorWarn(attributeAffects( aLightDirectionY,     aOutColor ) );
// 	IfMErrorWarn(attributeAffects( aLightDirectionZ,     aOutColor ) );
// 	IfMErrorWarn(attributeAffects( aLightDirection,      aOutColor ) );
// 	IfMErrorWarn(attributeAffects( aLightAmbient,        aOutColor ) );
// 	IfMErrorWarn(attributeAffects( aLightSpecular,       aOutColor ) );
// 	IfMErrorWarn(attributeAffects( aLightDiffuse,        aOutColor ) );
// 	IfMErrorWarn(attributeAffects( aLightShadowFraction, aOutColor ) );
// 	IfMErrorWarn(attributeAffects( aPreShadowIntensity,  aOutColor ) );
// 	IfMErrorWarn(attributeAffects( aLightBlindData,      aOutColor ) );
// 	IfMErrorWarn(attributeAffects( aLightData,           aOutColor ) );

//	IfMErrorWarn(attributeAffects( aOpacity,           aOutTransparency ) );

	initialize_shader_parameters();

	return MS::kSuccess;
}

MStatus FlatColorNode::initialize_shader_parameters()
{
	//CM_TRACE_FUNC("FlatColorNode::initialize_shader_parameters()");
	MFnTypedAttribute   tAttr;
	MFnStringData       tDefault;
	MFnNumericAttribute nAttr;
	MFnEnumAttribute    eAttr;
	MFnLightDataAttribute lAttr;
	MStatus status;

	acolor = nAttr.createColor("color", "ic");
	nAttr.setDefault( 1.0, 1.0, 1.0 );
	nAttr.setDisconnectBehavior( MFnAttribute::kReset );
	MAKE_INPUT(nAttr);

	atransparency = nAttr.createColor("transparency", "it");
	nAttr.setDefault( 1.0, 1.0, 1.0 );
	nAttr.setDisconnectBehavior( MFnAttribute::kReset );
	MAKE_INPUT(nAttr);


	IfMErrorWarn( addAttribute( acolor ) );
	IfMErrorWarn( addAttribute( atransparency ) );

	IfMErrorWarn( attributeAffects( acolor,			aOutColor ) );
	IfMErrorWarn( attributeAffects( atransparency,	aOutTransparency ) );

	return MS::kSuccess;
}

MStatus FlatColorNode::compute( const MPlug& plug, MDataBlock& block )
{
	//CM_TRACE_FUNC("SkyLightNode::compute(job="<<plug.name()<<",block)");

	// outColor or individual R, G, B channel
	if( (plug == aOutColor) || (plug.parent() == aOutColor)||
		(plug == aOutTransparency) || (plug.parent() == aOutTransparency) ) 
	{

		// init shader
		MStatus status;
		MFloatVector& cColor  = block.inputValue(acolor).asFloatVector();
		MFloatVector& cTrans  = block.inputValue(atransparency).asFloatVector();

		//rendering begin (maya software)
		MFloatVector resultColor( cColor );
		MFloatVector resultTrans( cTrans );

		resultColor[0] = resultTrans[0] * cColor[0];
		resultColor[1] = resultTrans[1] * cColor[1];
		resultColor[2] = resultTrans[2] * cColor[2];
		//rendering end

		// set ouput color attribute
		MDataHandle outColorHandle = block.outputValue( aOutColor, &status );
		IfMErrorWarn(status);
		outColorHandle.asFloatVector() = resultColor;
		outColorHandle.setClean();

		MDataHandle outTransHandle = block.outputValue( aOutTransparency, &status );
		IfMErrorWarn(status);
		outTransHandle.asFloatVector() = resultTrans;
		outTransHandle.setClean();
	} 
	else {
		return MS::kUnknownParameter;
	}

	return MS::kSuccess;
}

}//namespace elvishray



