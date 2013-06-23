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
#include "er_architectural_node.h"

#include <common/mayacheck.h>
#include <liqlog.h>
#include <liqNodeSwatch.h>
#include <liqIOStream.h>
#include <liqNodeSwatch.h>
#include "../er_user_nodeId.h"

namespace elvishray
{

// static data
// Attributes
MObject ArchitecturalNode::aRmanShader;
MObject ArchitecturalNode::aRmanShaderType;
MObject ArchitecturalNode::aRmanShaderLong;
MObject ArchitecturalNode::aRmanShaderLif;
MObject ArchitecturalNode::aRmanParams;
MObject ArchitecturalNode::aRmanDetails;
MObject ArchitecturalNode::aRmanTypes;
MObject ArchitecturalNode::aRmanDefaults;
MObject ArchitecturalNode::aRmanArraySizes;
MObject ArchitecturalNode::aRmanLifCmds;
MObject ArchitecturalNode::aRmanMethods;
MObject ArchitecturalNode::aRmanIsOutput;  
MObject ArchitecturalNode::aRmanAccept;

MObject ArchitecturalNode::aOutColor;
MObject ArchitecturalNode::aOutTransparency;
//MObject ArchitecturalNode::aAssignedObjects;

MObject ArchitecturalNode::asurface_color;
MObject ArchitecturalNode::adiffuse_color;  
MObject ArchitecturalNode::adiffuse_weight;
MObject ArchitecturalNode::aspecular_color;
MObject ArchitecturalNode::aspecular_weight;
MObject ArchitecturalNode::aroughness;
MObject ArchitecturalNode::aspecular_mode;
MObject ArchitecturalNode::aglossiness;
MObject ArchitecturalNode::areflection_color;
MObject ArchitecturalNode::areflection_weight;
MObject ArchitecturalNode::arefraction_color;
MObject ArchitecturalNode::arefraction_weight;
MObject ArchitecturalNode::arefraction_glossiness;
MObject ArchitecturalNode::arefraction_thickness;
MObject ArchitecturalNode::atranslucency_color;
MObject ArchitecturalNode::atranslucency_weight;
MObject ArchitecturalNode::atranslucency_mode;
MObject ArchitecturalNode::asss_color;
MObject ArchitecturalNode::asss_scale_color;
MObject ArchitecturalNode::asss_scale_weight;
MObject ArchitecturalNode::asss_phase;
MObject ArchitecturalNode::aanisotropy;
MObject ArchitecturalNode::arotation;
MObject ArchitecturalNode::aior;
MObject ArchitecturalNode::afresnel_by_ior;
MObject ArchitecturalNode::afresnel_0_degree_refl;
MObject ArchitecturalNode::afresnel_90_degree_refl;
MObject ArchitecturalNode::afresnel_curve;
MObject ArchitecturalNode::ais_metal;
MObject ArchitecturalNode::adiffuse_samples;
MObject ArchitecturalNode::areflection_samples;
MObject ArchitecturalNode::arefraction_samples;
MObject ArchitecturalNode::asss_samples;
MObject ArchitecturalNode::acutoff_threshold;
MObject ArchitecturalNode::abump_shader;
MObject ArchitecturalNode::abump_factor;
MObject ArchitecturalNode::adisplace_shader;
MObject ArchitecturalNode::adisplace_factor;
//MObject ArchitecturalNode::aliq_bump_shader_token;
//MObject ArchitecturalNode::aliq_displace_shader_token;

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

void ArchitecturalNode::postConstructor( )
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

ArchitecturalNode::ArchitecturalNode()
{
	swatchInit = false;
	renderSwatch = NULL;
}

ArchitecturalNode::~ArchitecturalNode()
{
	if (renderSwatch != NULL) 
		delete renderSwatch;
}

const MTypeId ArchitecturalNode::getTypeId()
{ 
	return UTI_Architectural_NODE;
}

const MString ArchitecturalNode::getTypeName()
{ 
	return ("er_architectural");
}

void* ArchitecturalNode::creator()
{
	return new ArchitecturalNode();
}

MStatus ArchitecturalNode::initialize()
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

	IfMErrorWarn( addAttribute( aOutColor ) );
	IfMErrorWarn( addAttribute( aOutTransparency ) );

	initialize_shader_parameters();

	return MS::kSuccess;
}

MStatus ArchitecturalNode::initialize_shader_parameters()
{
	//CM_TRACE_FUNC("ArchitecturalNode::initialize_shader_parameters()");
	MFnTypedAttribute   tAttr;
	MFnStringData       tDefault;
	MFnNumericAttribute nAttr;
	MFnEnumAttribute    eAttr;
	MFnLightDataAttribute lAttr;
	MStatus status;

	asurface_color = nAttr.createColor("surface_color", "sfc");
	nAttr.setDefault(1.0f, 1.0f, 1.0f);
	nAttr.setDisconnectBehavior( MFnAttribute::kReset );
	MAKE_INPUT(nAttr);

	adiffuse_color = nAttr.createColor("diffuse_color", "dfc");
	nAttr.setDefault( 1.0, 1.0, 1.0 );
	nAttr.setDisconnectBehavior( MFnAttribute::kReset );
	MAKE_INPUT(nAttr);

	adiffuse_weight = nAttr.create("diffuse_weight", "dfw", MFnNumericData::kDouble, 1.0, &status);
	MAKE_INPUT(nAttr);

	aspecular_color = nAttr.createColor("specular_color", "spc");
	nAttr.setDefault( 1.0, 1.0, 1.0 );
	nAttr.setDisconnectBehavior( MFnAttribute::kReset );
	MAKE_INPUT(nAttr);

	aspecular_weight = nAttr.create("specular_weight", "spw", MFnNumericData::kDouble, 0.2, &status);
	MAKE_INPUT(nAttr);

	aroughness = nAttr.create("roughness", "rgh", MFnNumericData::kDouble, 0.0, &status);
	MAKE_INPUT(nAttr);

	aspecular_mode = nAttr.create("specular_mode", "spm", MFnNumericData::kInt, 0, &status);
	MAKE_INPUT(nAttr);

	aglossiness = nAttr.create("glossiness", "gls", MFnNumericData::kDouble, 1.0, &status);
	MAKE_INPUT(nAttr);

	areflection_color = nAttr.createColor("reflection_color", "flc");
	nAttr.setDefault( 1.0, 1.0, 1.0 );
	nAttr.setDisconnectBehavior( MFnAttribute::kReset );
	MAKE_INPUT(nAttr);

	areflection_weight = nAttr.create("reflection_weight", "flw", MFnNumericData::kDouble, 0.0, &status);
	MAKE_INPUT(nAttr);

	arefraction_color = nAttr.createColor("refraction_color", "frc");
	nAttr.setDefault( 1.0, 1.0, 1.0 );
	nAttr.setDisconnectBehavior( MFnAttribute::kReset );
	MAKE_INPUT(nAttr);

	arefraction_weight = nAttr.create("refraction_weight", "frw", MFnNumericData::kDouble, 0.0, &status);
	MAKE_INPUT(nAttr);

	arefraction_glossiness = nAttr.create("refraction_glossiness", "frg", MFnNumericData::kDouble, 0.0, &status);
	MAKE_INPUT(nAttr);

	arefraction_thickness = nAttr.create("refraction_thickness", "frt", MFnNumericData::kDouble, 0.0, &status);
	MAKE_INPUT(nAttr);

	atranslucency_color = nAttr.createColor("translucency_color", "trc");
	nAttr.setDefault( 1.0, 1.0, 1.0 );
	nAttr.setDisconnectBehavior( MFnAttribute::kReset );
	MAKE_INPUT(nAttr);

	atranslucency_weight = nAttr.create("translucency_weight", "trw", MFnNumericData::kDouble, 0.0, &status);
	MAKE_INPUT(nAttr);

	atranslucency_mode = nAttr.create("translucency_mode", "trm", MFnNumericData::kInt, 1, &status);
	MAKE_INPUT(nAttr);

	asss_color = nAttr.createColor("sss_color", "ssc");
	nAttr.setDefault( 0.8, 0.8, 0.8 );
	nAttr.setDisconnectBehavior( MFnAttribute::kReset );
	MAKE_INPUT(nAttr);

	asss_scale_color = nAttr.createColor("sss_scale_color", "sss");
	nAttr.setDefault( 0.5f, 0.5f, 0.5f );
	nAttr.setDisconnectBehavior( MFnAttribute::kReset );
	MAKE_INPUT(nAttr);

	asss_scale_weight = nAttr.create("sss_scale_weight", "ssw", MFnNumericData::kDouble, 10.0, &status);
	MAKE_INPUT(nAttr);

	asss_phase = nAttr.create("sss_phase", "ssp", MFnNumericData::kDouble, 0.0, &status);
	MAKE_INPUT(nAttr);

	aanisotropy = nAttr.create("anisotropy", "ani", MFnNumericData::kDouble, 1.0, &status);
	MAKE_INPUT(nAttr);

	arotation = nAttr.create("rotation", "rot", MFnNumericData::kDouble, 0.0, &status);
	MAKE_INPUT(nAttr);

	aior = nAttr.create("ior", "ior", MFnNumericData::kDouble, 1.5, &status);
	MAKE_INPUT(nAttr);

	afresnel_by_ior = nAttr.create("fresnel_by_ior", "fbi", MFnNumericData::kBoolean, 0, &status);
	MAKE_INPUT(nAttr);

	afresnel_0_degree_refl = nAttr.create("fresnel_0_degree_refl", "f0d", MFnNumericData::kDouble, 0.2, &status);
	MAKE_INPUT(nAttr);

	afresnel_90_degree_refl = nAttr.create("fresnel_90_degree_refl", "f9d", MFnNumericData::kDouble, 1.0, &status);
	MAKE_INPUT(nAttr);

	afresnel_curve = nAttr.create("fresnel_curve", "fc", MFnNumericData::kDouble, 5.0, &status);
	MAKE_INPUT(nAttr);

	ais_metal = nAttr.create("is_metal", "met", MFnNumericData::kBoolean, 1, &status);
	MAKE_INPUT(nAttr);

	adiffuse_samples = nAttr.create("diffuse_samples", "dfs", MFnNumericData::kInt, 8, &status);
	MAKE_INPUT(nAttr);

	areflection_samples = nAttr.create("reflection_samples", "fls", MFnNumericData::kInt, 4, &status);
	MAKE_INPUT(nAttr);

	arefraction_samples = nAttr.create("refraction_samples", "frs", MFnNumericData::kInt, 4, &status);
	MAKE_INPUT(nAttr);

	asss_samples = nAttr.create("sss_samples", "s3s", MFnNumericData::kInt, 64, &status);
	MAKE_INPUT(nAttr);

	acutoff_threshold = nAttr.create("cutoff_threshold", "cot", MFnNumericData::kDouble, 0.01, &status);
	MAKE_INPUT(nAttr);

	abump_shader = nAttr.createColor("bump_shader", "bum");
	nAttr.setDefault( 0.0f, 0.0f, 0.0f );
	nAttr.setUsedAsColor( true );
	nAttr.setDisconnectBehavior( MFnAttribute::kReset );
	MAKE_INPUT(nAttr);

	abump_factor = nAttr.create("bump_factor", "buf", MFnNumericData::kDouble, 0.3, &status);
	MAKE_INPUT(nAttr);

	adisplace_shader = nAttr.createColor("displace_shader", "dss");
	nAttr.setDefault( 0.0f, 0.0f, 0.0f );
	nAttr.setUsedAsColor( true );
	nAttr.setDisconnectBehavior( MFnAttribute::kReset );
	MAKE_INPUT(nAttr);

	adisplace_factor = nAttr.create("displace_factor", "dsp", MFnNumericData::kDouble, 1.0, &status);
	MAKE_INPUT(nAttr);

	//aliq_bump_shader_token = tAttr.create( MString("liq_bump_shader_token"), MString("bto"), MFnData::kString, aliq_bump_shader_token, &status );
	//MAKE_INPUT(tAttr);
	//aliq_displace_shader_token = tAttr.create( MString("liq_displace_shader_token"), MString("dto"), MFnData::kString, aliq_displace_shader_token, &status );
	//MAKE_INPUT(tAttr);

	IfMErrorWarn( addAttribute( asurface_color ) );
	IfMErrorWarn( addAttribute( adiffuse_color ) );
	IfMErrorWarn( addAttribute( adiffuse_weight ) );
	IfMErrorWarn( addAttribute( aspecular_color ) );
	IfMErrorWarn( addAttribute( aspecular_weight ) );
	IfMErrorWarn( addAttribute( aroughness ) );
	IfMErrorWarn( addAttribute( aspecular_mode ) );
	IfMErrorWarn( addAttribute( aglossiness ) );
	IfMErrorWarn( addAttribute( areflection_color ) );
	IfMErrorWarn( addAttribute( areflection_weight ) );
	IfMErrorWarn( addAttribute( arefraction_color ) );
	IfMErrorWarn( addAttribute( arefraction_weight ) );
	IfMErrorWarn( addAttribute( arefraction_glossiness ) );
	IfMErrorWarn( addAttribute( arefraction_thickness ) );
	IfMErrorWarn( addAttribute( atranslucency_color ) );
	IfMErrorWarn( addAttribute( atranslucency_weight ) );
	IfMErrorWarn( addAttribute( atranslucency_mode ) );
	IfMErrorWarn( addAttribute( asss_color  ) );
	IfMErrorWarn( addAttribute( asss_scale_color ) );
	IfMErrorWarn( addAttribute( asss_scale_weight ) );
	IfMErrorWarn( addAttribute( asss_phase ) );
	IfMErrorWarn( addAttribute( aanisotropy ) );
	IfMErrorWarn( addAttribute( arotation ) );
	IfMErrorWarn( addAttribute( aior ) );
	IfMErrorWarn( addAttribute( afresnel_by_ior ) );
	IfMErrorWarn( addAttribute( afresnel_0_degree_refl ) );
	IfMErrorWarn( addAttribute( afresnel_90_degree_refl ) );
	IfMErrorWarn( addAttribute( afresnel_curve ) );
	IfMErrorWarn( addAttribute( ais_metal ) );
	IfMErrorWarn( addAttribute( adiffuse_samples ) );
	IfMErrorWarn( addAttribute( areflection_samples ) );
	IfMErrorWarn( addAttribute( arefraction_samples ) );
	IfMErrorWarn( addAttribute( asss_samples ) );
	IfMErrorWarn( addAttribute( acutoff_threshold  ) );
	IfMErrorWarn( addAttribute(abump_shader  ) );
	IfMErrorWarn( addAttribute(abump_factor  ) );
	IfMErrorWarn( addAttribute( adisplace_shader ) );
	IfMErrorWarn( addAttribute( adisplace_factor ) );
	//IfMErrorWarn( addAttribute( aliq_bump_shader_token ) );
	//IfMErrorWarn( addAttribute( aliq_displace_shader_token ) );

	IfMErrorWarn( attributeAffects( asurface_color,			aOutColor ) );
	IfMErrorWarn( attributeAffects( adiffuse_color,			aOutColor ) );
	IfMErrorWarn( attributeAffects( adiffuse_weight,		aOutColor ) );
	IfMErrorWarn( attributeAffects( aspecular_color,		aOutColor ) );
	IfMErrorWarn( attributeAffects( aspecular_weight,		aOutColor ) );
	IfMErrorWarn( attributeAffects( aroughness,				aOutColor ) );
	IfMErrorWarn( attributeAffects( aspecular_mode,			aOutColor ) );
	IfMErrorWarn( attributeAffects( aglossiness,			aOutColor ) );
	IfMErrorWarn( attributeAffects( areflection_color,		aOutColor ) );
	IfMErrorWarn( attributeAffects( areflection_weight,		aOutColor ) );
	IfMErrorWarn( attributeAffects( arefraction_color,		aOutTransparency ) );
	IfMErrorWarn( attributeAffects( arefraction_weight,		aOutTransparency ) );
	IfMErrorWarn( attributeAffects( arefraction_glossiness,	aOutTransparency ) );
	IfMErrorWarn( attributeAffects( arefraction_thickness,	aOutTransparency ) );
	IfMErrorWarn( attributeAffects( atranslucency_color,	aOutTransparency ) );
	IfMErrorWarn( attributeAffects( atranslucency_weight,	aOutTransparency ) );
	IfMErrorWarn( attributeAffects( atranslucency_mode,		aOutTransparency ) );
	IfMErrorWarn( attributeAffects( asss_color,				aOutColor ) );
	IfMErrorWarn( attributeAffects( asss_scale_color,		aOutColor ) );
	IfMErrorWarn( attributeAffects( asss_scale_weight,		aOutColor ) );
	IfMErrorWarn( attributeAffects( asss_phase,				aOutColor ) );
	IfMErrorWarn( attributeAffects( aanisotropy,			aOutColor ) );
	IfMErrorWarn( attributeAffects( arotation,				aOutColor ) );
	IfMErrorWarn( attributeAffects( aior,					aOutColor ) );
	IfMErrorWarn( attributeAffects( afresnel_by_ior,		aOutColor ) );
	IfMErrorWarn( attributeAffects( afresnel_0_degree_refl,	aOutColor ) );
	IfMErrorWarn( attributeAffects( afresnel_90_degree_refl,aOutColor ) );
	IfMErrorWarn( attributeAffects( afresnel_curve,			aOutColor ) );
	IfMErrorWarn( attributeAffects( ais_metal,				aOutColor ) );
	IfMErrorWarn( attributeAffects( adiffuse_samples,		aOutColor ) );
	IfMErrorWarn( attributeAffects( areflection_samples,	aOutColor ) );
	IfMErrorWarn( attributeAffects( arefraction_samples,	aOutColor ) );
	IfMErrorWarn( attributeAffects( asss_samples,			aOutColor ) );
	IfMErrorWarn( attributeAffects( acutoff_threshold,		aOutColor ) );
	IfMErrorWarn( attributeAffects( abump_shader,			aOutColor ) );
	IfMErrorWarn( attributeAffects( abump_factor,			aOutColor ) );
	IfMErrorWarn( attributeAffects( adisplace_shader,		aOutColor ) );
	IfMErrorWarn( attributeAffects( adisplace_factor,		aOutColor ) );
	//IfMErrorWarn( attributeAffects( aliq_bump_shader_token,		aOutColor ) );
	//IfMErrorWarn( attributeAffects( aliq_displace_shader_token,		aOutColor ) );

	return MS::kSuccess;
}

MStatus ArchitecturalNode::compute( const MPlug& plug, MDataBlock& block )
{
	//CM_TRACE_FUNC("SkyLightNode::compute(job="<<plug.name()<<",block)");

	// outColor or individual R, G, B channel
	if( (plug == aOutColor)			|| (plug.parent() == aOutColor)		  ||
		(plug == aOutTransparency)	|| (plug.parent() == aOutTransparency)
		) 
	{

		// init shader
		MStatus status;
		MFloatVector& surface_color  = block.inputValue(asurface_color).asFloatVector();


		//rendering begin (maya software)
		MFloatVector resultColor;
		resultColor = surface_color;

		//rendering end

		// set ouput color attribute
		MDataHandle outColorHandle = block.outputValue( aOutColor, &status );
		IfMErrorWarn(status);
		outColorHandle.asFloatVector() = resultColor;
		outColorHandle.setClean();

		MDataHandle outTransHandle = block.outputValue( aOutTransparency, &status );
		IfMErrorWarn(status);
		outTransHandle.asFloatVector() = MFloatVector(0.0, 0.0, 0.0);
		outTransHandle.setClean();
	} 
	else {
		return MS::kUnknownParameter;
	}

	return MS::kSuccess;
}
}//namespace elvishray



