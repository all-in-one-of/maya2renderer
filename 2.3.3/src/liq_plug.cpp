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

/* ______________________________________________________________________
**
** Liquid Plug-In
** ______________________________________________________________________
*/
#include <liq_plug.h>

#define MNoVersionString
#define MNoPluginEntry

#include <common/prerequest_maya.h>
#include <common/mayacheck.h>
#include <maya/MFnPlugin.h>

#include <liquid.h>
#include <liqRibTranslator.h>
#include <liqGetAttr.h>
#include <liqAttachPrefAttribute.h>
#include <liqPreviewShader.h>
#include <liqWriteArchive.h>
#include <liqNodeSwatch.h>
#include <liqSurfaceNode.h>
#include <liqDisplacementNode.h>
#include <liqVolumeNode.h>
#include <liqLightNode.h>
#include <liqShaderNode.h>
#include <liqLightNodeBehavior.h>
#include <liqRibboxNode.h>
#include <liqRibRequestNode.h>
#include <liqCoordSysNode.h>
#include <liqGlobalHelpers.h>
#include <liqMayaRenderView.h>
#include <liqGlobalsNode.h>
#include "liqGlobalNodeRenderer.h"
//#include <liqJobList.h>
#include <liqBoundingBoxLocator.h>
#include <liqCoShaderNode.h>
#include <liqShaderFactory.h>
#include <liqGlobalVariable.h>
#include <liqGetSloInfo.h>
#include <liqParseString.h>
#include <liqIPRNodeMessageCmd.h>
#include "liq_classification.h"
#include "liqMessageCallback.h"

#define LIQVENDOR "http://liquidmaya.sourceforge.net/"

#if defined(_WIN32) /*&& !defined(DEFINED_LIQUIDVERSION)*/
// unix build gets this from the Makefile
const char * LIQUIDVERSION =
#include "../liquid.version"
	;
#define DEFINED_LIQUIDVERSION
#endif


////////////////////// EXPORTS /////////////////////////////////////////////////////////
MStatus _initializePlugin(MObject obj)
	//  Description:
	//      Register the command when the plug-in is loaded
{
	liqglo.liquidBin = false;

	MStatus status;

	MFnPlugin plugin( obj, LIQVENDOR, LIQUIDVERSION, "Any");

	MGlobal::displayInfo(MString("Initializing Liquid v") + LIQUIDVERSION);
	MGlobal::displayInfo("Initial Liquid code by Colin Doncaster");

	if( ! isCommandRegistered(liqRibTranslator::getName()) ){
		status = plugin.registerCommand(liqRibTranslator::getName(), liqRibTranslator::creator, liqRibTranslator::syntax );
		IfMErrorMsgReturnIt( status, "Can't register command:"+liqRibTranslator::getName() );
	}else{
		displayInfo(liqRibTranslator::getName()+" already registered. skip");
	}

	if( ! isCommandRegistered(liqAttachPrefAttribute::getName()) ){
		// register the liquidAttachPrefAttribute command
		status = plugin.registerCommand( liqAttachPrefAttribute::getName(), liqAttachPrefAttribute::creator, liqAttachPrefAttribute::syntax );
		IfMErrorMsgReturnIt( status, "Can't register command:"+liqAttachPrefAttribute::getName() );
	}else{
		displayInfo(liqAttachPrefAttribute::getName()+" already registered. skip");
	}

	if( ! isCommandRegistered(liqPreviewShader::getName()) ){
		// register the liquidPreviewShader command
		status = plugin.registerCommand( liqPreviewShader::getName(), liqPreviewShader::creator, liqPreviewShader::syntax );
		IfMErrorMsgReturnIt( status, "Can't register command:"+liqPreviewShader::getName() );
	}else{
		displayInfo(liqPreviewShader::getName()+" already registered. skip");
	}

	if( ! isCommandRegistered(liqGetSloInfo::getCommandName()) ){
		// register the liqGetSloInfo command
		status = plugin.registerCommand( liqGetSloInfo::getCommandName(), liqGetSloInfo::creator );
		IfMErrorMsgReturnIt( status, "Can't register command:"+liqGetSloInfo::getCommandName() );
	}else{
		displayInfo(liqGetSloInfo::getCommandName()+" already registered. skip");
	}

	if( ! isCommandRegistered(liqGetAttr::getName()) ){
		// register the liquidGetAttr command
		status = plugin.registerCommand( liqGetAttr::getName(), liqGetAttr::creator );
		IfMErrorMsgReturnIt( status, "Can't register command:"+liqGetAttr::getName() );
	}else{
		displayInfo(liqGetAttr::getName()+" already registered. skip");
	}

	//if( ! isCommandRegistered("liquidWriteArchive") ){
	// register the liquidWriteArchive command
	//   status = plugin.registerCommand( "liquidWriteArchive", liqWriteArchive::creator, liqWriteArchive::syntax );
	//   IfMErrorMsgReturnIt( status, "Can't register liquidWriteArchive command" );
	//}else{
	//	displayInfo("liquidPreviewShader already registered. skip")
	//}

	if( ! isCommandRegistered(liqMayaRenderCmd::getName()) ){
		// register the liquidRenderViewCmd command
		status = plugin.registerCommand( liqMayaRenderCmd::getName(), liqMayaRenderCmd::creator ,liqMayaRenderCmd::newSyntax);
		IfMErrorMsgReturnIt( status, "Can't register command:"+liqMayaRenderCmd::getName() );
	}else{
		displayInfo(liqMayaRenderCmd::getName()+" already registered. skip");
	}

	//if( ! isCommandRegistered(liqJobList::getName()) ){
	//	// register the liquidJobList command
	//	status = plugin.registerCommand( liqJobList::getName(), liqJobList::creator ,liqJobList::syntax);
	//	IfMErrorMsgReturnIt( status, "Can't register command:"+liqJobList::getName() );
	//}else{
	//	displayInfo(liqJobList::getName()+" already registered. skip");
	//}

	//#ifndef NO_RICMD
	//  // register the RIArchiveBegin command
	//  status = plugin.registerCommand( "RIArchiveBegin", RIArchiveBegin::creator, RIArchiveBegin::newSyntax );
	//  IfMErrorMsgReturnIt( status, "Can't register RIArchiveBegin command" );
	//
	//  // register the RIArchiveEnd command
	//  status = plugin.registerCommand( "RIArchiveEnd", RIArchiveEnd::creator, RIArchiveEnd::newSyntax );
	//  IfMErrorMsgReturnIt( status, "Can't register RIArchiveEnd command" );
	//
	//  // register the RIArchiveRecord command
	//  status = plugin.registerCommand( "RIArchiveRecord", RIArchiveRecord::creator, RIArchiveRecord::newSyntax );
	//  IfMErrorMsgReturnIt( status, "Can't register RIArchiveRecord command" );
	//
	//  // register the RIAtmosphere command
	//  status = plugin.registerCommand( "RIAtmosphere", RIAtmosphere::creator, RIAtmosphere::newSyntax );
	//  IfMErrorMsgReturnIt( status, "Can't register RIAtmosphere command" );
	//
	//  // register the RIAttribute command
	//  status = plugin.registerCommand( "RIAttribute", RIAttribute::creator, RIAttribute::newSyntax );
	//  IfMErrorMsgReturnIt( status, "Can't register RIAttribute command" );
	//
	//  // register the RIAttributeBegin command
	//  status = plugin.registerCommand( "RIAttributeBegin", RIAttributeBegin::creator, RIAttributeBegin::newSyntax );
	//  IfMErrorMsgReturnIt( status, "Can't register RIAttributeBegin command" );
	//
	//  // register the RIAttributeEnd command
	//  status = plugin.registerCommand( "RIAttributeEnd", RIAttributeEnd::creator, RIAttributeEnd::newSyntax );
	//  IfMErrorMsgReturnIt( status, "Can't register RIAttributeEnd command" );
	//
	//  // register the RIClipping command
	//  status = plugin.registerCommand( "RIClipping", RIClipping::creator, RIClipping::newSyntax );
	//  IfMErrorMsgReturnIt( status, "Can't register RIClipping command" );
	//
	//  // register the RIColor command
	//  status = plugin.registerCommand( "RIColor", RIColor::creator, RIColor::newSyntax );
	//  IfMErrorMsgReturnIt( status, "Can't register RIColor command" );
	//
	//  // register the RIConcatTransform command
	//  status = plugin.registerCommand( "RIConcatTransform", RIConcatTransform::creator, RIConcatTransform::newSyntax );
	//  IfMErrorMsgReturnIt( status, "Can't register RIConcatTransform command" );
	//
	//  // register the RICropWindow command
	//  status = plugin.registerCommand( "RICropWindow", RICropWindow::creator, RICropWindow::newSyntax );
	//  IfMErrorMsgReturnIt( status, "Can't register RICropWindow command" );
	//
	//  // register the RIDepthOfField command
	//  status = plugin.registerCommand( "RIDepthOfField", RIDepthOfField::creator, RIDepthOfField::newSyntax );
	//  IfMErrorMsgReturnIt( status, "Can't register RIDepthOfField command" );
	//
	//  // register the RIDetail command
	//  status = plugin.registerCommand( "RIDetail", RIDetail::creator, RIDetail::newSyntax );
	//  IfMErrorMsgReturnIt( status, "Can't register RIDetail command" );
	//
	//  // register the RIDetailRange command
	//  status = plugin.registerCommand( "RIDetailRange", RIDetailRange::creator, RIDetailRange::newSyntax );
	//  IfMErrorMsgReturnIt( status, "Can't register RIDetailRange command" );
	//
	//  // register the RIDisplacement command
	//  status = plugin.registerCommand( "RIDisplacement", RIDisplacement::creator, RIDisplacement::newSyntax );
	//  IfMErrorMsgReturnIt( status, "Can't register RIDisplacement command" );
	//
	//  // register the RIDisplay command
	//  status = plugin.registerCommand( "RIDisplay", RIDisplay::creator, RIDisplay::newSyntax );
	//  IfMErrorMsgReturnIt( status, "Can't register RIDisplay command" );
	//
	//  // register the RIDisplayChannel command
	//  status = plugin.registerCommand( "RIDisplayChannel", RIDisplayChannel::creator, RIDisplayChannel::newSyntax );
	//  IfMErrorMsgReturnIt( status, "Can't register RIDisplayChannel command" );
	//
	//  // register the RIExterior command
	//  status = plugin.registerCommand( "RIExterior", RIExterior::creator, RIExterior::newSyntax );
	//  IfMErrorMsgReturnIt( status, "Can't register RIExterior command" );
	//
	//  // register the RIFormat command
	//  status = plugin.registerCommand( "RIFormat", RIFormat::creator, RIFormat::newSyntax );
	//  IfMErrorMsgReturnIt( status, "Can't register RIFormat command" );
	//
	//  // register the RIFrameBegin command
	//  status = plugin.registerCommand( "RIFrameBegin", RIFrameBegin::creator, RIFrameBegin::newSyntax );
	//  IfMErrorMsgReturnIt( status, "Can't register RIFrameBegin command" );
	//
	//  // register the RIFrameEnd command
	//  status = plugin.registerCommand( "RIFrameEnd", RIFrameEnd::creator, RIFrameEnd::newSyntax );
	//  IfMErrorMsgReturnIt( status, "Can't register RIFrameEnd command" );
	//
	//  // register the RIGeometricApproximation command
	//  status = plugin.registerCommand( "RIGeometricApproximation", RIGeometricApproximation::creator, RIGeometricApproximation::newSyntax );
	//  IfMErrorMsgReturnIt( status, "Can't register RIGeometricApproximation command" );
	//
	//  // register the RIGeometry command
	//  status = plugin.registerCommand( "RIGeometry", RIGeometry::creator, RIGeometry::newSyntax );
	//  IfMErrorMsgReturnIt( status, "Can't register RIGeometry command" );
	//
	//  // register the RIHider command
	//  status = plugin.registerCommand( "RIHider", RIHider::creator, RIHider::newSyntax );
	//  IfMErrorMsgReturnIt( status, "Can't register RIHider command" );
	//
	//  // register the RIIdentity command
	//  status = plugin.registerCommand( "RIIdentity", RIIdentity::creator, RIIdentity::newSyntax );
	//  IfMErrorMsgReturnIt( status, "Can't register RIIdentity command" );
	//
	//  // register the RIIlluminate command
	//  status = plugin.registerCommand( "RIIlluminate", RIIlluminate::creator, RIIlluminate::newSyntax );
	//  IfMErrorMsgReturnIt( status, "Can't register RIIlluminate command" );
	//
	//  // register the RIInterior command
	//  status = plugin.registerCommand( "RIInterior", RIInterior::creator, RIInterior::newSyntax );
	//  IfMErrorMsgReturnIt( status, "Can't register RIInterior command" );
	//
	//  // register the RILightSource command
	//  status = plugin.registerCommand( "RILightSource", RILightSource::creator, RILightSource::newSyntax );
	//  IfMErrorMsgReturnIt( status, "Can't register RILightSource command" );
	//
	//  // register the RIMatte command
	//  status = plugin.registerCommand( "RIMatte", RIMatte::creator, RIMatte::newSyntax );
	//  IfMErrorMsgReturnIt( status, "Can't register RIMatte command" );
	//
	//  // register the RIMotionBegin command
	//  status = plugin.registerCommand( "RIMotionBegin", RIMotionBegin::creator, RIMotionBegin::newSyntax );
	//  IfMErrorMsgReturnIt( status, "Can't register RIMotionBegin command" );
	//
	//  // register the RIMotionEnd command
	//  status = plugin.registerCommand( "RIMotionEnd", RIMotionEnd::creator, RIMotionEnd::newSyntax );
	//  IfMErrorMsgReturnIt( status, "Can't register RIMotionEnd command" );
	//
	//  // register the RIObjectBegin command
	//  status = plugin.registerCommand( "RIObjectBegin", RIObjectBegin::creator, RIObjectBegin::newSyntax );
	//  IfMErrorMsgReturnIt( status, "Can't register RIObjectBegin command" );
	//
	//  // register the RIObjectEnd command
	//  status = plugin.registerCommand( "RIObjectEnd", RIObjectEnd::creator, RIObjectEnd::newSyntax );
	//  IfMErrorMsgReturnIt( status, "Can't register RIObjectEnd command" );
	//
	//  // register the RIOpacity command
	//  status = plugin.registerCommand( "RIOpacity", RIOpacity::creator, RIOpacity::newSyntax );
	//  IfMErrorMsgReturnIt( status, "Can't register RIOpacity command" );
	//
	//  // register the RIOption command
	//  status = plugin.registerCommand( "RIOption", RIOption::creator, RIOption::newSyntax );
	//  IfMErrorMsgReturnIt( status, "Can't register RIOption command" );
	//
	//  // register the RIOrientation command
	//  status = plugin.registerCommand( "RIOrientation", RIOrientation::creator, RIOrientation::newSyntax );
	//  IfMErrorMsgReturnIt( status, "Can't register RIOrientation command" );
	//
	//  // register the RIPixelSamples command
	//  status = plugin.registerCommand( "RIPixelSamples", RIPixelSamples::creator, RIPixelSamples::newSyntax );
	//  IfMErrorMsgReturnIt( status, "Can't register RIPixelSamples command" );
	//
	//  // register the RIProcedural command
	//  status = plugin.registerCommand( "RIProcedural", RIProcedural::creator, RIProcedural::newSyntax );
	//  IfMErrorMsgReturnIt( status, "Can't register RIProcedural command" );
	//
	//  // register the RIProjection command
	//  status = plugin.registerCommand( "RIProjection", RIProjection::creator, RIProjection::newSyntax );
	//  IfMErrorMsgReturnIt( status, "Can't register RIProjection command" );
	//
	//  // register the RIReadArchive command
	//  status = plugin.registerCommand( "RIReadArchive", RIReadArchive::creator, RIReadArchive::newSyntax );
	//  IfMErrorMsgReturnIt( status, "Can't register RIReadArchive command" );
	//
	//  // register the RIRelativeDetail command
	//  status = plugin.registerCommand( "RIRelativeDetail", RIRelativeDetail::creator, RIRelativeDetail::newSyntax );
	//  IfMErrorMsgReturnIt( status, "Can't register RIRelativeDetail command" );
	//
	//  // register the RIResource command
	//  status = plugin.registerCommand( "RIResource", RIResource::creator, RIResource::newSyntax );
	//  IfMErrorMsgReturnIt( status, "Can't register RIResource command" );
	//
	//  // register the RIResourceBegin command
	//  status = plugin.registerCommand( "RIResourceBegin", RIResourceBegin::creator, RIResourceBegin::newSyntax );
	//  IfMErrorMsgReturnIt( status, "Can't register RIResourceBegin command" );
	//
	//  // register the RIResourceEnd command
	//  status = plugin.registerCommand( "RIResourceEnd", RIResourceEnd::creator, RIResourceEnd::newSyntax );
	//  IfMErrorMsgReturnIt( status, "Can't register RIResourceEnd command" );
	//
	//  // register the RIReverseOrientation command
	//  status = plugin.registerCommand( "RIReverseOrientation", RIReverseOrientation::creator, RIReverseOrientation::newSyntax );
	//  IfMErrorMsgReturnIt( status, "Can't register RIReverseOrientation command" );
	//
	//  // register the RIRotate command
	//  status = plugin.registerCommand( "RIRotate", RIRotate::creator, RIRotate::newSyntax );
	//  IfMErrorMsgReturnIt( status, "Can't register RIRotate command" );
	//
	//  // register the RIScale command
	//  status = plugin.registerCommand( "RIScale", RIScale::creator, RIScale::newSyntax );
	//  IfMErrorMsgReturnIt( status, "Can't register RIScale command" );
	//
	//  // register the RIScreenWindow command
	//  status = plugin.registerCommand( "RIScreenWindow", RIScreenWindow::creator, RIScreenWindow::newSyntax );
	//  IfMErrorMsgReturnIt( status, "Can't register RIScreenWindow command" );
	//
	//  // register the RIShadingInterpolation command
	//  status = plugin.registerCommand( "RIShadingInterpolation", RIShadingInterpolation::creator, RIShadingInterpolation::newSyntax );
	//  IfMErrorMsgReturnIt( status, "Can't register RIShadingInterpolation command" );
	//
	//  // register the RIShadingRate command
	//  status = plugin.registerCommand( "RIShadingRate", RIShadingRate::creator, RIShadingRate::newSyntax );
	//  IfMErrorMsgReturnIt( status, "Can't register RIShadingRate command" );
	//
	//  // register the RIShutter command
	//  status = plugin.registerCommand( "RIShutter", RIShutter::creator, RIShutter::newSyntax );
	//  IfMErrorMsgReturnIt( status, "Can't register RIShutter command" );
	//
	//  // register the RISides command
	//  status = plugin.registerCommand( "RISides", RISides::creator, RISides::newSyntax );
	//  IfMErrorMsgReturnIt( status, "Can't register RISides command" );
	//
	//  // register the RISkew command
	//  status = plugin.registerCommand( "RISkew", RISkew::creator, RISkew::newSyntax );
	//  IfMErrorMsgReturnIt( status, "Can't register RISkew command" );
	//
	//  // register the RISolidBegin command
	//  status = plugin.registerCommand( "RISolidBegin", RISolidBegin::creator, RISolidBegin::newSyntax );
	//  IfMErrorMsgReturnIt( status, "Can't register RISolidBegin command" );
	//
	//  // register the RISolidEnd command
	//  status = plugin.registerCommand( "RISolidEnd", RISolidEnd::creator, RISolidEnd::newSyntax );
	//  IfMErrorMsgReturnIt( status, "Can't register RISolidEnd command" );
	//
	//  // register the RISphere command
	//  status = plugin.registerCommand( "RISphere", RISphere::creator, RISphere::newSyntax );
	//  IfMErrorMsgReturnIt( status, "Can't register RISphere command" );
	//
	//  // register the RISurface command
	//  status = plugin.registerCommand( "RISurface", RISurface::creator, RISurface::newSyntax );
	//  IfMErrorMsgReturnIt( status, "Can't register RISurface command" );
	//
	//  // register the RITransformBegin command
	//  status = plugin.registerCommand( "RITransformBegin", RITransformBegin::creator, RITransformBegin::newSyntax );
	//  IfMErrorMsgReturnIt( status, "Can't register RITransformBegin command" );
	//
	//  // register the RITransformEnd command
	//  status = plugin.registerCommand( "RITransformEnd", RITransformEnd::creator, RITransformEnd::newSyntax );
	//  IfMErrorMsgReturnIt( status, "Can't register RITransformEnd command" );
	//
	//  // register the RITranslate command
	//  status = plugin.registerCommand( "RITranslate", RITranslate::creator, RITranslate::newSyntax );
	//  IfMErrorMsgReturnIt( status, "Can't register RITranslate command" );
	//
	//  // register the RIWorldBegin command
	//  status = plugin.registerCommand( "RIWorldBegin", RIWorldBegin::creator, RIWorldBegin::newSyntax );
	//  IfMErrorMsgReturnIt( status, "Can't register RIWorldBegin command" );
	//
	//  // register the RIWorldEnd command
	//  status = plugin.registerCommand( "RIWorldEnd", RIWorldEnd::creator, RIWorldEnd::newSyntax );
	//  IfMErrorMsgReturnIt( status, "Can't register RIWorldEnd command" );
	//
	//#endif

	// register the liquidShader node
	//const MString UserClassify( "rendernode/liquid/shader/surface:shader/surface:swatch/liqSurfSwatch" );
	if( ! isNodeRegistered(liqSurfaceNode::getTypeName()) ){
		status = plugin.registerNode( liqSurfaceNode::getTypeName(), liqSurfaceNode::id, liqSurfaceNode::creator, liqSurfaceNode::initialize, MPxNode::kDependNode, &liquid::surface_classification );
		IfMErrorMsgReturnIt( status, "Can't register "+liqSurfaceNode::getTypeName() );
		status.clear();
		status = MSwatchRenderRegister::registerSwatchRender( "liqSurfSwatch", liqNodeSwatch::creator );
		IfMErrorMsgReturnIt( status, "Can't register liquidSurface swatch" );
	}else{
		displayInfo(liqSurfaceNode::getTypeName()+" already registered. skip");
	}

	if( ! isNodeRegistered(liqDisplacementNode::getTypeName()) ){
		// register the liquidDisplacement node
		//const MString UserClassify1( "rendernode/liquid/shader/displacement:shader/displacement:swatch/liqDispSwatch" );
		status = plugin.registerNode( liqDisplacementNode::getTypeName(), liqDisplacementNode::id, liqDisplacementNode::creator, liqDisplacementNode::initialize, MPxNode::kDependNode, &liquid::displacement_classification );
		IfMErrorMsgReturnIt( status, "Can't register "+liqDisplacementNode::getTypeName() );
		status.clear();
		status = MSwatchRenderRegister::registerSwatchRender( "liqDispSwatch", liqNodeSwatch::creator );
		IfMErrorMsgReturnIt( status, "Can't register liquidDisplacement swatch" );
	}else{
		displayInfo(liqDisplacementNode::getTypeName()+" already registered. skip");
	}

	if( ! isNodeRegistered(liqVolumeNode::getTypeName()) ){
		// register the liquidVolume node
		//const MString UserClassify2( "rendernode/liquid/shader/volume:shader/volume:swatch/liqVolSwatch" );
		status = plugin.registerNode( liqVolumeNode::getTypeName(), liqVolumeNode::id, liqVolumeNode::creator, liqVolumeNode::initialize, MPxNode::kDependNode, &liquid::volume_classification );
		IfMErrorMsgReturnIt( status, "Can't register "+liqVolumeNode::getTypeName() );
		status.clear();
		status = MSwatchRenderRegister::registerSwatchRender( "liqVolSwatch", liqNodeSwatch::creator );
		IfMErrorMsgReturnIt( status, "Can't register liquidVolume swatch" );
	}else{
		displayInfo(liqVolumeNode::getTypeName()+" already registered. skip");
	}

	if( ! isNodeRegistered(liqLightNode::getTypeName()) ){
		// register the liquidLight node
		//  const MString UserClassify3( "rendernode/liquid/light:light:swatch/liqLightSwatch" );
		//const MString UserClassify3( "rendernode/liquid/shader/surface:shader/surface:swatch/liqLightSwatch" );
		status = plugin.registerNode( liqLightNode::getTypeName(), liqLightNode::id, liqLightNode::creator, liqLightNode::initialize, MPxNode::kDependNode, &liquid::light_classification );
		IfMErrorMsgReturnIt( status, "Can't register "+liqLightNode::getTypeName() );
		status.clear();
		status = MSwatchRenderRegister::registerSwatchRender( "liqLightSwatch", liqNodeSwatch::creator );
		IfMErrorMsgReturnIt( status, "Can't register liqLightSwatch swatch" );
		status = plugin.registerDragAndDropBehavior( "liquidLightBehavior", liqLightNodeBehavior::creator);
		IfMErrorMsgReturnIt( status, "Can't register liquidLight behavior" );
	}else{
		displayInfo(liqLightNode::getTypeName()+" already registered. skip");
	}

	if( ! isNodeRegistered( liqRibboxNode::getTypeName()) ){
		// register the liquidRibbox node
		//const MString UserClassify4( "rendernode/liquid/utility/general:utility/general:swatch/liqRibSwatch" );
		status = plugin.registerNode( liqRibboxNode::getTypeName(), liqRibboxNode::id, liqRibboxNode::creator, liqRibboxNode::initialize, MPxNode::kDependNode, &liquid::utility_general_classification );
		IfMErrorMsgReturnIt( status, "Can't register "+ liqRibboxNode::getTypeName() );
		status.clear();
		status = MSwatchRenderRegister::registerSwatchRender( "liqRibSwatch", liqNodeSwatch::creator );
		IfMErrorMsgReturnIt( status, "Can't register liquidRibbox swatch" );
	}else{
		displayInfo( liqRibboxNode::getTypeName()+" already registered. skip");
	}

	if( ! isNodeRegistered(liqRibRequestNode::getTypeName()) ){
		// register the liquidRequest node
		status = plugin.registerNode( liqRibRequestNode::getTypeName(), liqRibRequestNode::id, liqRibRequestNode::creator, liqRibRequestNode::initialize, MPxNode::kDependNode );
		IfMErrorMsgReturnIt( status, "Can't register "+liqRibRequestNode::getTypeName() );
		status.clear();
	}else{
		displayInfo(liqRibRequestNode::getTypeName()+" already registered. skip");
	}

	if( ! isNodeRegistered(liqCoordSysNode::getTypeName()) ){
		// register the liquidCoordSys node
		status = plugin.registerNode( liqCoordSysNode::getTypeName(), liqCoordSysNode::id, liqCoordSysNode::creator, liqCoordSysNode::initialize, MPxNode::kLocatorNode );
		IfMErrorMsgReturnIt( status, "Can't register "+liqCoordSysNode::getTypeName() );
		status.clear();
	}else{
		displayInfo(liqCoordSysNode::getTypeName()+" already registered. skip");
	}

	if( ! isNodeRegistered(liqBoundingBoxLocator::getTypeName()) ){
		// register the liqBoundingBoxLocator node
		status = plugin.registerNode( liqBoundingBoxLocator::getTypeName(), liqBoundingBoxLocator::id, liqBoundingBoxLocator::creator, liqBoundingBoxLocator::initialize, MPxNode::kLocatorNode );
		IfMErrorMsgReturnIt( status, "Can't register "+liqBoundingBoxLocator::getTypeName() );
		status.clear();
	}else{
		displayInfo(liqBoundingBoxLocator::getTypeName()+" already registered. skip");
	}

	if( ! isNodeRegistered(liqGlobalsNode::getTypeName()) ){
		// register the liquidGlobals node
		status = plugin.registerNode( liqGlobalsNode::getTypeName(), liqGlobalsNode::id, liqGlobalsNode::creator, liqGlobalsNode::initialize, MPxNode::kDependNode );
		IfMErrorMsgReturnIt( status, "Can't register "+liqGlobalsNode::getTypeName() );
		status.clear();
	}else{
		displayInfo(liqGlobalsNode::getTypeName()+" already registered. skip");
	}

	if( ! isNodeRegistered(liqCoShaderNode::getTypeName()) ){
		// register the liquidCoShader node
		//const MString UserClassifyCoShader( "rendernode/liquid/utility/general:utility/general:swatch/liqCoShaderSwatch" );
		status = plugin.registerNode( liqCoShaderNode::getTypeName(), liqCoShaderNode::id, liqCoShaderNode::creator, liqCoShaderNode::initialize, MPxNode::kDependNode, &liquid::utility_general_classification );
		IfMErrorMsgReturnIt( status, "Can't register "+liqCoShaderNode::getTypeName() );
		status.clear();
		status = MSwatchRenderRegister::registerSwatchRender( "liqCoShaderSwatch", liqNodeSwatch::creator );
		IfMErrorMsgReturnIt( status, "Can't register liquidCoShader swatch" );
	}else{
		displayInfo(liqCoShaderNode::getTypeName()+" already registered. skip");
	}
	//  status = plugin.registerCommand("liqRibShaderTranslator", liqRibShaderTranslator::creator );
	//  IfMErrorMsgReturnIt( status, "Can't register liquid translator command" );

	if( ! isNodeRegistered(liqGlobalsNodeRenderer::getTypeName()) ){
		// register the liqGlobalsNodeRenderer node
		status = plugin.registerNode( liqGlobalsNodeRenderer::getTypeName(), liqGlobalsNodeRenderer::id, liqGlobalsNodeRenderer::creator, liqGlobalsNodeRenderer::initialize, MPxNode::kDependNode );
		IfMErrorMsgReturnIt( status, "Can't register "+liqGlobalsNodeRenderer::getTypeName() );
		status.clear();
	}else{
		displayInfo(liqGlobalsNodeRenderer::getTypeName()+" already registered. skip");
	}

	if( ! isNodeRegistered(liqShaderNode::getTypeName()) ){
		// register the liquidShader node
		//const MString UserClassifyShader( "rendernode/liquid/utility/color:utility/color:swatch/liqShaderSwatch" );
		status = plugin.registerNode( liqShaderNode::getTypeName(), liqShaderNode::id, liqShaderNode::creator, liqShaderNode::initialize, MPxNode::kDependNode, &liquid::utility_color_classification );
		IfMErrorMsgReturnIt( status, "Can't register "+liqShaderNode::getTypeName());
		status.clear();
		status = MSwatchRenderRegister::registerSwatchRender( "liqShaderSwatch", liqNodeSwatch::creator );
		IfMErrorMsgReturnIt( status, "Can't register liquidShader swatch" );
	}else{
		displayInfo(liqShaderNode::getTypeName()+" already registered. skip");
	}

	if( ! isCommandRegistered(liqParseString::getName()) ){
		status = plugin.registerCommand(liqParseString::getName(), liqParseString::creator );
		IfMErrorMsgReturnIt( status, "Can't register command"+liqParseString::getName() );
	}else{
		displayInfo(liqParseString::getName()+" already registered. skip");
	}

	if( ! isCommandRegistered(liqIPRNodeMessage::getName()) ){
		status = plugin.registerCommand( liqIPRNodeMessage::getName(), liqIPRNodeMessage::creator );
		IfMErrorMsgReturnIt( status, "Can't register command:"+liqIPRNodeMessage::getName() );
	}else{
		displayInfo(liqIPRNodeMessage::getName()+" already registered. skip");
	}

	// setup all of the base liquid interface
	MString sourceLine;
	char *tmphomeChar;
	if( (tmphomeChar = getenv( "LIQUIDHOME" )) == 0 ){
		liquidMessage2(messageError, "env LIQUIDHOME is not found, it is set in liquidDLL.mod by default. make sure liquidDLL.mod is copied to your <MyDocuments>/maya/<version>/modules");
		return MS::kFailure;
	}
	for (unsigned k( 0 ); k < strlen(tmphomeChar); k++ ) 
	{
		if ( tmphomeChar[ k ] == '\\' )
			tmphomeChar[ k ] = '/';
	}
	sourceLine += "source \""+MString(tmphomeChar)+"/mel/liquidLoadScripts.mel\";";
	sourceLine += "source \""+MString(tmphomeChar)+"/mel/liquidStartup.mel\"; ";
	status = MGlobal::executeCommand(sourceLine);
	IfMErrorMsgReturnIt( status, "executeCommand() fails: "+sourceLine );

	status = MGlobal::executeCommand("liquidStartupNoUI();");
	IfMErrorMsgReturnIt( status, "liquidStartupNoUI() fails");

	status = plugin.registerUI("liquidStartupUI", "liquidShutdownUI");
	IfMErrorMsgReturnIt( status, "Can't register liquidStartupUI and liquidShutdownUI interface scripts" );
	printf("Liquid %s registered\n", LIQUIDVERSION);

	liquid::liqMessageCallback::registCallback();
	//  liquid::RendererMgr::registFactory("renderman", new renderman::Factory());
	//  liquid::RendererMgr::registFactory("elvishray", new elvishray::Factory());
	//  liquid::RendererMgr::registFactory("appleseed", new appleseed::Factory());

	return MS::kSuccess;
}

MStatus _uninitializePlugin(MObject obj)
	//  Description:
	//      Deregister the command when the plug-in is unloaded
{
	//	liquid::RendererMgr::unregistFactory("appleseed");
	//	liquid::RendererMgr::unregistFactory("elvishray");
	//	liquid::RendererMgr::unregistFactory("renderman");

	MStatus status;
	MFnPlugin plugin(obj);

	liquid::liqMessageCallback::removeCallback();

	if( isCommandRegistered(liqRibTranslator::getName()) ){
		status = plugin.deregisterCommand(liqRibTranslator::getName());
		IfMErrorMsgReturnIt( status, "Can't deregister command:"+liqRibTranslator::getName() );
	}else{
		displayInfo(liqRibTranslator::getName()+" is deregisted. skip");
	}
		
	if( isCommandRegistered(liqAttachPrefAttribute::getName()) ){
		status = plugin.deregisterCommand(liqAttachPrefAttribute::getName());
		IfMErrorMsgReturnIt( status, "Can't deregister command:"+liqAttachPrefAttribute::getName() );
	}else{
		displayInfo(liqAttachPrefAttribute::getName()+" is deregisted. skip");
	}

	if( isCommandRegistered(liqPreviewShader::getName()) ){
		status = plugin.deregisterCommand(liqPreviewShader::getName());
		IfMErrorMsgReturnIt( status, "Can't deregister command:"+liqPreviewShader::getName() );
	}else{
		displayInfo(liqPreviewShader::getName()+" is deregisted. skip");
	}

	if( isCommandRegistered(liqGetSloInfo::getCommandName()) ){
		status = plugin.deregisterCommand(liqGetSloInfo::getCommandName());
		IfMErrorMsgReturnIt( status, "Can't deregister command:"+liqGetSloInfo::getCommandName() );
	}else{
		displayInfo(liqGetSloInfo::getCommandName()+" is deregisted. skip");
	}

	if( isCommandRegistered(liqGetAttr::getName()) ){
		status = plugin.deregisterCommand(liqGetAttr::getName());
		IfMErrorMsgReturnIt( status, "Can't deregister command:"+liqGetAttr::getName() );
	}else{
		displayInfo(liqGetAttr::getName()+" is deregisted. skip");
	}

	//if( isCommandRegistered("liquidWriteArchive") ){
	//	status = plugin.deregisterCommand("liquidWriteArchive");
	//	IfMErrorMsgReturnIt( status, "Can't deregister liquidWriteArchive command" );
	//}else{
	//	displayInfo("liquidWriteArchive is deregisted. skip");
	//}

	if( isCommandRegistered(liqMayaRenderCmd::getName()) ){
		status = plugin.deregisterCommand(liqMayaRenderCmd::getName());
		IfMErrorMsgReturnIt( status, "Can't deregister command:"+liqMayaRenderCmd::getName() );
	}else{
		displayInfo(liqMayaRenderCmd::getName()+" is deregisted. skip");
	}

	//if( isCommandRegistered(liqJobList::getName()) ){
	//	status = plugin.deregisterCommand(liqJobList::getName());
	//	IfMErrorMsgReturnIt( status, "Can't deregister command:"+liqJobList::getName() );
	//}else{
	//	displayInfo(liqJobList::getName()+" is deregisted. skip");
	//}

	//#ifndef NO_RICMD
	//  status = plugin.deregisterCommand("RIArchiveBegin");
	//  IfMErrorMsgReturnIt( status, "Can't deregister RIArchiveBegin command" );
	//
	//  status = plugin.deregisterCommand("RIArchiveEnd");
	//  IfMErrorMsgReturnIt( status, "Can't deregister RIArchiveEnd command" );
	//
	//  status = plugin.deregisterCommand("RIArchiveRecord");
	//  IfMErrorMsgReturnIt( status, "Can't deregister RIArchiveRecord command" );
	//
	//  status = plugin.deregisterCommand("RIAtmosphere");
	//  IfMErrorMsgReturnIt( status, "Can't deregister RIAtmosphere command" );
	//
	//  status = plugin.deregisterCommand("RIAttribute");
	//  IfMErrorMsgReturnIt( status, "Can't deregister RIAttribute command" );
	//
	//  status = plugin.deregisterCommand("RIAttributeBegin");
	//  IfMErrorMsgReturnIt( status, "Can't deregister RIAttributeBegin command" );
	//
	//  status = plugin.deregisterCommand("RIAttributeEnd");
	//  IfMErrorMsgReturnIt( status, "Can't deregister RIAttributeEnd command" );
	//
	//  status = plugin.deregisterCommand("RIClipping");
	//  IfMErrorMsgReturnIt( status, "Can't deregister RIClipping command" );
	//
	//  status = plugin.deregisterCommand("RIColor");
	//  IfMErrorMsgReturnIt( status, "Can't deregister RIColor command" );
	//
	//  status = plugin.deregisterCommand("RIConcatTransform");
	//  IfMErrorMsgReturnIt( status, "Can't deregister RIConcatTransform command" );
	//
	//  status = plugin.deregisterCommand("RICropWindow");
	//  IfMErrorMsgReturnIt( status, "Can't deregister RICropWindow command" );
	//
	//  status = plugin.deregisterCommand("RIDepthOfField");
	//  IfMErrorMsgReturnIt( status, "Can't deregister RIDepthOfField command" );
	//
	//  status = plugin.deregisterCommand("RIDetail");
	//  IfMErrorMsgReturnIt( status, "Can't deregister RIDetail command" );
	//
	//  status = plugin.deregisterCommand("RIDetailRange");
	//  IfMErrorMsgReturnIt( status, "Can't deregister RIDetailRange command" );
	//  
	//  status = plugin.deregisterCommand("RIDisplacement");
	//  IfMErrorMsgReturnIt( status, "Can't deregister RIDisplacement command" );
	//
	//  status = plugin.deregisterCommand("RIDisplay");
	//  IfMErrorMsgReturnIt( status, "Can't deregister RIDisplay command" );
	//
	//  status = plugin.deregisterCommand("RIDisplayChannel");
	//  IfMErrorMsgReturnIt( status, "Can't deregister RIDisplayChannel command" );
	//
	//  status = plugin.deregisterCommand("RIExterior");
	//  IfMErrorMsgReturnIt( status, "Can't deregister RIExterior command" );
	//
	//  status = plugin.deregisterCommand("RIFormat");
	//  IfMErrorMsgReturnIt( status, "Can't deregister RIFormat command" );
	//
	//  status = plugin.deregisterCommand("RIFrameBegin");
	//  IfMErrorMsgReturnIt( status, "Can't deregister RIFrameBegin command" );
	//
	//  status = plugin.deregisterCommand("RIFrameEnd");
	//  IfMErrorMsgReturnIt( status, "Can't deregister RIFrameEnd command" );
	//
	//  status = plugin.deregisterCommand("RIGeometricApproximation");
	//  IfMErrorMsgReturnIt( status, "Can't deregister RIGeometricApproximation command" );
	//
	//  status = plugin.deregisterCommand("RIGeometry");
	//  IfMErrorMsgReturnIt( status, "Can't deregister RIGeometry command" );
	//
	//  status = plugin.deregisterCommand("RIHider");
	//  IfMErrorMsgReturnIt( status, "Can't deregister RIHider command" );
	//
	//  status = plugin.deregisterCommand("RIIdentity");
	//  IfMErrorMsgReturnIt( status, "Can't deregister RIIdentity command" );
	//
	//  status = plugin.deregisterCommand("RIIlluminate");
	//  IfMErrorMsgReturnIt( status, "Can't deregister RIIlluminate command" );
	//
	//  status = plugin.deregisterCommand("RIInterior");
	//  IfMErrorMsgReturnIt( status, "Can't deregister RIInterior command" );
	//
	//  status = plugin.deregisterCommand("RILightSource");
	//  IfMErrorMsgReturnIt( status, "Can't deregister RILightSource command" );
	//
	//  status = plugin.deregisterCommand("RIMatte");
	//  IfMErrorMsgReturnIt( status, "Can't deregister RIMatte command" );
	//
	//  status = plugin.deregisterCommand("RIMotionBegin");
	//  IfMErrorMsgReturnIt( status, "Can't deregister RIMotionBegin command" );
	//
	//  status = plugin.deregisterCommand("RIMotionEnd");
	//  IfMErrorMsgReturnIt( status, "Can't deregister RIMotionEnd command" );
	//
	//  status = plugin.deregisterCommand("RIObjectBegin");
	//  IfMErrorMsgReturnIt( status, "Can't deregister RIObjectBegin command" );
	//
	//  status = plugin.deregisterCommand("RIObjectEnd");
	//  IfMErrorMsgReturnIt( status, "Can't deregister RIObjectEnd command" );
	//
	//  status = plugin.deregisterCommand("RIOpacity");
	//  IfMErrorMsgReturnIt( status, "Can't deregister RIOpacity command" );
	//
	//  status = plugin.deregisterCommand("RIOption");
	//  IfMErrorMsgReturnIt( status, "Can't deregister RIOption command" );
	//
	//  status = plugin.deregisterCommand("RIOrientation");
	//  IfMErrorMsgReturnIt( status, "Can't deregister RIOrientation command" );
	//
	//  status = plugin.deregisterCommand("RIPixelSamples");
	//  IfMErrorMsgReturnIt( status, "Can't deregister RIPixelSamples command" );
	//
	//  status = plugin.deregisterCommand("RIProcedural");
	//  IfMErrorMsgReturnIt( status, "Can't deregister RIProcedural command" );
	//
	//  status = plugin.deregisterCommand("RIProjection");
	//  IfMErrorMsgReturnIt( status, "Can't deregister RIProjection command" );
	//
	//  status = plugin.deregisterCommand("RIReadArchive");
	//  IfMErrorMsgReturnIt( status, "Can't deregister RIReadArchive command" );
	//
	//  status = plugin.deregisterCommand("RIRelativeDetail");
	//  IfMErrorMsgReturnIt( status, "Can't deregister RIRelativeDetail command" );
	//
	//  status = plugin.deregisterCommand("RIResource");
	//  IfMErrorMsgReturnIt( status, "Can't deregister RIResource command" );
	//
	//  status = plugin.deregisterCommand("RIResourceBegin");
	//  IfMErrorMsgReturnIt( status, "Can't deregister RIResourceBegin command" );
	//
	//  status = plugin.deregisterCommand("RIResourceEnd");
	//  IfMErrorMsgReturnIt( status, "Can't deregister RIResourceEnd command" );
	//
	//  status = plugin.deregisterCommand("RIReverseOrientation");
	//  IfMErrorMsgReturnIt( status, "Can't deregister RIReverseOrientation command" );
	//
	//  status = plugin.deregisterCommand("RIRotate");
	//  IfMErrorMsgReturnIt( status, "Can't deregister RIRotate command" );
	//
	//  status = plugin.deregisterCommand("RIScale");
	//  IfMErrorMsgReturnIt( status, "Can't deregister RIScale command" );
	//
	//  status = plugin.deregisterCommand("RIScreenWindow");
	//  IfMErrorMsgReturnIt( status, "Can't deregister RIScreenWindow command" );
	//
	//  status = plugin.deregisterCommand("RIShadingInterpolation");
	//  IfMErrorMsgReturnIt( status, "Can't deregister RIShadingInterpolation command" );
	//
	//  status = plugin.deregisterCommand("RIShadingRate");
	//  IfMErrorMsgReturnIt( status, "Can't deregister RIShadingRate command" );
	//
	//  status = plugin.deregisterCommand("RIShutter");
	//  IfMErrorMsgReturnIt( status, "Can't deregister RIShutter command" );
	//
	//  status = plugin.deregisterCommand("RISides");
	//  IfMErrorMsgReturnIt( status, "Can't deregister RISides command" );
	//
	//  status = plugin.deregisterCommand("RISkew");
	//  IfMErrorMsgReturnIt( status, "Can't deregister RISkew command" );
	//
	//  status = plugin.deregisterCommand("RISolidBegin");
	//  IfMErrorMsgReturnIt( status, "Can't deregister RISolidBegin command" );
	//
	//  status = plugin.deregisterCommand("RISolidEnd");
	//  IfMErrorMsgReturnIt( status, "Can't deregister RISolidEnd command" );
	//
	//  status = plugin.deregisterCommand("RISphere");
	//  IfMErrorMsgReturnIt( status, "Can't deregister RISphere command" );
	//
	//  status = plugin.deregisterCommand("RISurface");
	//  IfMErrorMsgReturnIt( status, "Can't deregister RISurface command" );
	//
	//  status = plugin.deregisterCommand("RITransformBegin");
	//  IfMErrorMsgReturnIt( status, "Can't deregister RITransformBegin command" );
	//
	//  status = plugin.deregisterCommand("RITransformEnd");
	//  IfMErrorMsgReturnIt( status, "Can't deregister RITransformEnd command" );
	//
	//  status = plugin.deregisterCommand("RITranslate");
	//  IfMErrorMsgReturnIt( status, "Can't deregister RITranslate command" );
	//
	//  status = plugin.deregisterCommand("RIWorldBegin");
	//  IfMErrorMsgReturnIt( status, "Can't deregister RIWorldBegin command" );
	//
	//  status = plugin.deregisterCommand("RIWorldEnd");
	//  IfMErrorMsgReturnIt( status, "Can't deregister RIWorldEnd command" );
	//  
	//#endif    
	if( isNodeRegistered(liqShaderNode::getTypeName()) ){
		status = plugin.deregisterNode( liqShaderNode::id );
		IfMErrorMsgReturnIt( status, "Can't deregister node:"+liqShaderNode::getTypeName() );
		status = MSwatchRenderRegister::unregisterSwatchRender( "liqShaderSwatch" );
		IfMErrorMsgReturnIt( status, "Can't deregister liquidShader swatch generator" );
	}else{
		displayInfo(liqShaderNode::getTypeName()+" is deregisted. skip");
	}

	if( isNodeRegistered(liqSurfaceNode::getTypeName()) ){
		status = plugin.deregisterNode( liqSurfaceNode::id );
		IfMErrorMsgReturnIt( status, "Can't deregister  node:"+liqSurfaceNode::getTypeName() );
		status = MSwatchRenderRegister::unregisterSwatchRender( "liqSurfSwatch" );
		IfMErrorMsgReturnIt( status, "Can't deregister liquidSurface swatch generator" );
	}else{
		displayInfo(liqSurfaceNode::getTypeName()+" is deregisted. skip");
	}

	if( isNodeRegistered(liqDisplacementNode::getTypeName()) ){
		status = plugin.deregisterNode( liqDisplacementNode::id );
		IfMErrorMsgReturnIt( status, "Can't deregister node:"+liqDisplacementNode::getTypeName() );
		status = MSwatchRenderRegister::unregisterSwatchRender( "liqDispSwatch" );
		IfMErrorMsgReturnIt( status, "Can't deregister liquidDisp swatch generator" );
	}else{
		displayInfo(liqDisplacementNode::getTypeName()+" is deregisted. skip");
	}

	if( isNodeRegistered(liqVolumeNode::getTypeName()) ){
		status = plugin.deregisterNode( liqVolumeNode::id );
		IfMErrorMsgReturnIt( status, "Can't deregister node:"+liqVolumeNode::getTypeName() );
		status = MSwatchRenderRegister::unregisterSwatchRender( "liqVolSwatch" );
		IfMErrorMsgReturnIt( status, "Can't deregister liquidVolume swatch generator" );
	}else{
		displayInfo(liqVolumeNode::getTypeName()+" is deregisted. skip");
	}

	if( isNodeRegistered(liqLightNode::getTypeName()) ){
		status = plugin.deregisterNode( liqLightNode::id );
		IfMErrorMsgReturnIt( status, "Can't deregister node:"+liqLightNode::getTypeName() );
		status = MSwatchRenderRegister::unregisterSwatchRender( "liqLightSwatch" );
		IfMErrorMsgReturnIt( status, "Can't deregister liquidLight swatch generator" );
		status = plugin.deregisterDragAndDropBehavior( "liquidLightBehavior" );
		IfMErrorMsgReturnIt( status, "Can't deregister liquidLight behavior" );
	}else{
		displayInfo(liqLightNode::getTypeName()+" is deregisted. skip");
	}

	if( isNodeRegistered(liqRibboxNode::getTypeName()) ){
		status = plugin.deregisterNode( liqRibboxNode::id );
		IfMErrorMsgReturnIt( status, "Can't deregister node:"+liqRibboxNode::getTypeName() );
		status = MSwatchRenderRegister::unregisterSwatchRender( "liqRibSwatch" );
		IfMErrorMsgReturnIt( status, "Can't deregister liquidRibbox swatch generator" );
	}else{
		displayInfo(liqRibboxNode::getTypeName()+" is deregisted. skip");
	}

	if( isNodeRegistered(liqRibRequestNode::getTypeName()) ){
		status = plugin.deregisterNode( liqRibRequestNode::id );
		IfMErrorMsgReturnIt( status, "Can't deregister node:"+liqRibRequestNode::getTypeName() );
	}else{
		displayInfo(liqRibRequestNode::getTypeName()+" is deregisted. skip");
	}

	if( isNodeRegistered(liqCoordSysNode::getTypeName()) ){
		status = plugin.deregisterNode( liqCoordSysNode::id );
		IfMErrorMsgReturnIt( status, "Can't deregister node:"+liqCoordSysNode::getTypeName() );
	}else{
		displayInfo(liqCoordSysNode::getTypeName()+" is deregisted. skip");
	}

	if( isNodeRegistered(liqBoundingBoxLocator::getTypeName()) ){
		status = plugin.deregisterNode( liqBoundingBoxLocator::id );
		IfMErrorMsgReturnIt( status, "Can't deregister node:"+liqBoundingBoxLocator::getTypeName() );
	}else{
		displayInfo(liqBoundingBoxLocator::getTypeName()+" is deregisted. skip");
	}

	if( isNodeRegistered(liqGlobalsNodeRenderer::getTypeName()) ){
		status = plugin.deregisterNode( liqGlobalsNodeRenderer::id );
		IfMErrorMsgReturnIt( status, "Can't deregister node:"+liqGlobalsNodeRenderer::getTypeName() );
	}else{
		displayInfo(liqGlobalsNodeRenderer::getTypeName()+" is deregisted. skip");
	}

	if( isNodeRegistered(liqGlobalsNode::getTypeName()) ){
		status = plugin.deregisterNode( liqGlobalsNode::id );
		IfMErrorMsgReturnIt( status, "Can't deregister node:"+liqGlobalsNode::getTypeName() );
	}else{
		displayInfo(liqGlobalsNode::getTypeName()+" is deregisted. skip");
	
	}
	if( isNodeRegistered(liqCoShaderNode::getTypeName()) ){
		status = plugin.deregisterNode( liqCoShaderNode::id );
		IfMErrorMsgReturnIt( status, "Can't deregister node:"+liqCoShaderNode::getTypeName() );
		status = MSwatchRenderRegister::unregisterSwatchRender( "liqCoShaderSwatch" );
		IfMErrorMsgReturnIt( status, "Can't deregister liquidCoShader swatch generator" );
	}else{
		displayInfo(liqCoShaderNode::getTypeName()+" is deregisted. skip");
	}

	//if( isNodeRegistered("liqRibShaderTranslator") ){
	//  status = plugin.deregisterCommand("liqRibShaderTranslator");
	//  IfMErrorMsgReturnIt( status, "Can't deregister liqRibShaderTranslator command" );
	//}else{
	//	displayInfo("liqRibShaderTranslator is deregisted. skip");
	//}

	if( isCommandRegistered(liqParseString::getName()) ){
		status = plugin.deregisterCommand(liqParseString::getName());
		IfMErrorMsgReturnIt( status, "Can't deregister command:"+liqParseString::getName() );
	}else{
		displayInfo(liqParseString::getName()+" is deregisted. skip");
	}

	if( isCommandRegistered(liqIPRNodeMessage::getName()) ){
		status = plugin.deregisterCommand( liqIPRNodeMessage::getName() );
		IfMErrorMsgReturnIt( status, "Can't deregister command:"+liqIPRNodeMessage::getName() );
	}else{
		displayInfo(liqIPRNodeMessage::getName()+" is deregisted. skip");
	}

	printf("Liquid %s unregistered\n\n", LIQUIDVERSION );

	liqShaderFactory::deleteInstance();

	return MS::kSuccess;
}
