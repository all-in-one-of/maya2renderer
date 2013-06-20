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
#include "er_checker_node.h"

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
MObject CheckerNode::aRmanShader;
MObject CheckerNode::aRmanShaderType;
MObject CheckerNode::aRmanShaderLong;
MObject CheckerNode::aRmanShaderLif;
MObject CheckerNode::aRmanParams;
MObject CheckerNode::aRmanDetails;
MObject CheckerNode::aRmanTypes;
MObject CheckerNode::aRmanDefaults;
MObject CheckerNode::aRmanArraySizes;
MObject CheckerNode::aRmanLifCmds;
MObject CheckerNode::aRmanMethods;
MObject CheckerNode::aRmanIsOutput;  
MObject CheckerNode::aRmanAccept;

MObject CheckerNode::aOutColor;
MObject CheckerNode::aOutTransparency;
//MObject CheckerNode::aAssignedObjects;

MObject CheckerNode::acolor1;
MObject CheckerNode::acolor2;  
MObject CheckerNode::axscale;
MObject CheckerNode::ayscale;
MObject CheckerNode::aresult;

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

void CheckerNode::postConstructor( )
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

CheckerNode::CheckerNode()
{
	swatchInit = false;
	renderSwatch = NULL;
}

CheckerNode::~CheckerNode()
{
	if (renderSwatch != NULL) 
		delete renderSwatch;
}

const MTypeId CheckerNode::getTypeId()
{ 
	return TI_Checker_NODE;
}

const MString CheckerNode::getTypeName()
{ 
	return ("er_checker");
}

void* CheckerNode::creator()
{
	return new CheckerNode();
}

MStatus CheckerNode::initialize()
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

MStatus CheckerNode::initialize_shader_parameters()
{
	//CM_TRACE_FUNC("CheckerNode::initialize_shader_parameters()");
	MFnTypedAttribute   tAttr;
	MFnStringData       tDefault;
	MFnNumericAttribute nAttr;
	MFnEnumAttribute    eAttr;
	MFnLightDataAttribute lAttr;
	MStatus status;

	acolor1 = nAttr.createColor("color1", "c1");
	nAttr.setDefault( 0.0, 0.0, 0.0 );
	nAttr.setDisconnectBehavior( MFnAttribute::kReset );
	MAKE_INPUT(nAttr);

	acolor2 = nAttr.createColor("color2", "c2");
	nAttr.setDefault( 1.0, 1.0, 1.0 );
	nAttr.setDisconnectBehavior( MFnAttribute::kReset );
	MAKE_INPUT(nAttr);

	axscale = nAttr.create("xscale", "xs", MFnNumericData::kDouble, 2.0, &status);
	MAKE_INPUT(nAttr);
	CHECK_MSTATUS(nAttr.setConnectable(false ) );

	ayscale = nAttr.create("yscale", "ys", MFnNumericData::kDouble, 2.0, &status);
	MAKE_INPUT(nAttr);
	CHECK_MSTATUS(nAttr.setConnectable(false ) );

	aresult = nAttr.createColor("result", "res");
	nAttr.setDefault( 0.0, 0.0, 0.0 );
	nAttr.setDisconnectBehavior( MFnAttribute::kReset );
	MAKE_OUTPUT(nAttr);

	IfMErrorWarn( addAttribute( acolor1 ) );
	IfMErrorWarn( addAttribute( acolor2 ) );
	IfMErrorWarn( addAttribute( axscale ) );
	IfMErrorWarn( addAttribute( ayscale ) );
	IfMErrorWarn( addAttribute( aresult ) );

	IfMErrorWarn( attributeAffects( acolor1,			aOutColor ) );
	IfMErrorWarn( attributeAffects( acolor2,			aOutColor ) );
	IfMErrorWarn( attributeAffects( axscale,			aOutColor ) );
	IfMErrorWarn( attributeAffects( ayscale,			aOutColor ) );

	IfMErrorWarn( attributeAffects( acolor1,			aresult ) );
	IfMErrorWarn( attributeAffects( acolor2,			aresult ) );
	IfMErrorWarn( attributeAffects( axscale,			aresult ) );
	IfMErrorWarn( attributeAffects( ayscale,			aresult ) );
	return MS::kSuccess;
}

MStatus CheckerNode::compute( const MPlug& plug, MDataBlock& block )
{
	//CM_TRACE_FUNC("SkyLightNode::compute(job="<<plug.name()<<",block)");

	// outColor or individual R, G, B channel
	if( (plug == aOutColor)			|| (plug.parent() == aOutColor)		  ||
		(plug == aOutTransparency)	|| (plug.parent() == aOutTransparency)||
		(plug == aresult)			|| (plug.parent() == aresult) 
		) 
	{

		// init shader
		MStatus status;
		MFloatVector& color1  = block.inputValue(acolor1).asFloatVector();
		MFloatVector& color2  = block.inputValue(acolor2).asFloatVector();
		MFloatVector& xscale  = block.inputValue(axscale).asFloatVector();
		MFloatVector& yscale  = block.inputValue(ayscale).asFloatVector();


		//rendering begin (maya software)
		MFloatVector resultColor;
		if (1)
		{
			resultColor = color1;
		} else {
			resultColor = color2;
		}
		//rendering end

		// set ouput color attribute
		MDataHandle outResultHandle = block.outputValue( aresult, &status );
		IfMErrorWarn(status);
		outResultHandle.asFloatVector() = resultColor;
		outResultHandle.setClean();

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
//
//MStatus registerCheckerNode( MFnPlugin &plugin )
//{
//	MStatus status;
//	status = plugin.registerNode( 
//		elvishray::CheckerNode::getTypeName(), 
//		elvishray::CheckerNode::getTypeId(), 
//		elvishray::CheckerNode::creator, elvishray::CheckerNode::initialize, MPxNode::kDependNode,
//		&elvishray::surface_classification);
//	LIQCHECKSTATUS( status, "Can't register "+elvishray::CheckerNode::getTypeName()+" node" );
//	status.clear();
//	elvishray::CheckerNodeVisitor::regist();
//}
//MStatus deregisterCheckerNode( MFnPlugin &plugin )
//{
//	MStatus status;
//	elvishray::CheckerNodeVisitor::unregist();
//	status = plugin.deregisterNode( elvishray::CheckerNode::getTypeId() );
//	LIQCHECKSTATUS( status, "Can't deregister "+elvishray::CheckerNode::getTypeName()+" node" );
//
//}
}//namespace elvishray



