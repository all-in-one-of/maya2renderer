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
#include "rm_flat_node.h"

#include <common/mayacheck.h>
#include <liqlog.h>
#include <liqNodeSwatch.h>
#include <liqIOStream.h>
#include <liqNodeSwatch.h>
#include "../rm_user_nodeId.h"

namespace renderman
{

// static data
// Attributes
MObject FlatNode::aRmanShader;
MObject FlatNode::aRmanShaderType;
MObject FlatNode::aRmanShaderLong;
MObject FlatNode::aRmanShaderLif;
MObject FlatNode::aRmanParams;
MObject FlatNode::aRmanDetails;
MObject FlatNode::aRmanTypes;
MObject FlatNode::aRmanDefaults;
MObject FlatNode::aRmanArraySizes;
MObject FlatNode::aRmanLifCmds;
MObject FlatNode::aRmanMethods;
MObject FlatNode::aRmanIsOutput;  
MObject FlatNode::aRmanAccept;

MObject FlatNode::aOutColor;
MObject FlatNode::aOutTransparency;
//MObject FlatNode::aAssignedObjects;

MObject FlatNode::aicolor;
MObject FlatNode::aiopacity;

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

void FlatNode::postConstructor( )
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

FlatNode::FlatNode()
{
	swatchInit = false;
	renderSwatch = NULL;
}

FlatNode::~FlatNode()
{
	if (renderSwatch != NULL) 
		delete renderSwatch;
}

const MTypeId FlatNode::getTypeId()
{ 
	return UTI_Flat_NODE;
}

const MString FlatNode::getTypeName()
{ 
	return ("rmFlat");
}

void* FlatNode::creator()
{
	return new FlatNode();
}

MStatus FlatNode::initialize()
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

MStatus FlatNode::initialize_shader_parameters()
{
	//CM_TRACE_FUNC("FlatNode::initialize_shader_parameters()");
	MFnTypedAttribute   tAttr;
	MFnStringData       tDefault;
	MFnNumericAttribute nAttr;
	MFnEnumAttribute    eAttr;
	MFnLightDataAttribute lAttr;
	MStatus status;

	aicolor = nAttr.createColor("icolor", "ic");
	nAttr.setDefault( 0.8, 0.8, 0.8 );
	nAttr.setDisconnectBehavior( MFnAttribute::kReset );
	MAKE_INPUT(nAttr);

	aiopacity = nAttr.createColor("iopacity", "io");
	nAttr.setDefault( 0.0, 0.0, 0.0 );
	nAttr.setDisconnectBehavior( MFnAttribute::kReset );
	MAKE_INPUT(nAttr);

	IfMErrorWarn( addAttribute( aicolor ) );
	IfMErrorWarn( addAttribute( aiopacity ) );

	IfMErrorWarn( attributeAffects( aicolor,			aOutColor ) );
	IfMErrorWarn( attributeAffects( aiopacity,			aOutColor ) );

	return MS::kSuccess;
}

MStatus FlatNode::compute( const MPlug& plug, MDataBlock& block )
{
	//CM_TRACE_FUNC("SkyLightNode::compute(job="<<plug.name()<<",block)");

	// outColor or individual R, G, B channel
	if( (plug == aOutColor)			|| (plug.parent() == aOutColor)		  ||
		(plug == aOutTransparency)	|| (plug.parent() == aOutTransparency)
		) 
	{

		// init shader
		MStatus status;
		MFloatVector& icolor  = block.inputValue(aicolor).asFloatVector();


		//rendering begin (maya software)
		MFloatVector resultColor;
		resultColor = icolor;
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
}//namespace renderman



