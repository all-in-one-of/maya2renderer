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

#include "liq_plug.h"

// Renderman Headers
//extern "C" {
//#include "ri_interface.h"
//}

#include <common/prerequest_maya.h>
#include <maya/MFnPlugin.h>
#include <common/mayacheck.h>
#include <liqNodeSwatch.h>
#include "er_.h"
#include "../renderermgr.h"
#include "../elvishray/er_factory.h"
#include "er_globalnode.h"
#include "er_shader_node.h"
#include "er_classification.h"
#include "er_NodeAddCallback.h"

////////////////////// EXPORTS /////////////////////////////////////////////////////////
PLUGIN_EXPORT MStatus initializePlugin(MObject obj)
//  Description:
//      Register the command when the plug-in is loaded
{
	MStatus status;
	MFnPlugin plugin( obj, "https://github.com/maya2renderer/maya2renderer", "0.0.1", "Any");
	//_initializePlugin(obj);

	//
	liquid::RendererMgr::registFactory(elvishray::RENDER_NAME.asChar(), new elvishray::Factory());
	//
	elvishray::NodeAddCallback::registCallback();
	//
	// register the Globals node
	assert( elvishray::GlobalNode::getTypeName().length() );
	status = plugin.registerNode( 
		elvishray::GlobalNode::getTypeName(), 
		elvishray::GlobalNode::getTypeId(), 
		elvishray::GlobalNode::creator, elvishray::GlobalNode::initialize, MPxNode::kDependNode );
	IfMErrorMsgReturnIt( status, "Can't register "+elvishray::GlobalNode::getTypeName()+" node" );
	status.clear();
	//
	//
	status = plugin.registerNode( 
		elvishray::PhysicalskyNode::getTypeName(), 
		elvishray::PhysicalskyNode::getTypeId(), 
		elvishray::PhysicalskyNode::creator, elvishray::PhysicalskyNode::initialize, MPxNode::kDependNode,
		&elvishray::env_classification);
	IfMErrorMsgReturnIt( status, "Can't register "+elvishray::PhysicalskyNode::getTypeName()+" node" );
	status.clear();
	status = MSwatchRenderRegister::registerSwatchRender( elvishray::PhysicalskyNode::getTypeName()+"Swatch", liqNodeSwatch::creator );
	IfMErrorMsgReturnIt( status, "Can't register "+elvishray::PhysicalskyNode::getTypeName()+"Swatch" );
	//
	status = plugin.registerNode( 
		elvishray::SkyLightNode::getTypeName(), 
		elvishray::SkyLightNode::getTypeId(), 
		elvishray::SkyLightNode::creator, elvishray::SkyLightNode::initialize, MPxNode::kDependNode,
		&elvishray::light_classification);
	IfMErrorMsgReturnIt( status, "Can't register "+elvishray::SkyLightNode::getTypeName()+" node" );
	status.clear();
	status = MSwatchRenderRegister::registerSwatchRender( elvishray::SkyLightNode::getTypeName()+"Swatch", liqNodeSwatch::creator );
	IfMErrorMsgReturnIt( status, "Can't register "+elvishray::SkyLightNode::getTypeName()+"Swatch" );
	//
	status = plugin.registerNode( 
		elvishray::FlatColorNode::getTypeName(), 
		elvishray::FlatColorNode::getTypeId(), 
		elvishray::FlatColorNode::creator, elvishray::FlatColorNode::initialize, MPxNode::kDependNode,
		&elvishray::surface_classification);
	IfMErrorMsgReturnIt( status, "Can't register "+elvishray::FlatColorNode::getTypeName()+" node" );
	status.clear();
	status = MSwatchRenderRegister::registerSwatchRender( elvishray::FlatColorNode::getTypeName()+"Swatch", liqNodeSwatch::creator );
	IfMErrorMsgReturnIt( status, "Can't register "+elvishray::FlatColorNode::getTypeName()+"Swatch" );
	//
	elvishray::CheckerNodeVisitor::regist(plugin);

	return MS::kSuccess;
}

PLUGIN_EXPORT MStatus uninitializePlugin(MObject obj)
//  Description:
//      Deregister the command when the plug-in is unloaded
{
	MStatus status;
	MFnPlugin plugin(obj);

	//
	elvishray::CheckerNodeVisitor::unregist(plugin);
	//
	status = MSwatchRenderRegister::unregisterSwatchRender(elvishray::FlatColorNode::getTypeName()+"Swatch");
	IfMErrorMsgReturnIt( status, "Can't deregister "+elvishray::FlatColorNode::getTypeName()+"Swatch generator" );
	status = plugin.deregisterNode( elvishray::FlatColorNode::getTypeId() );
	IfMErrorMsgReturnIt( status, "Can't deregister "+elvishray::FlatColorNode::getTypeName()+" node" );
	//
	status = MSwatchRenderRegister::unregisterSwatchRender(elvishray::SkyLightNode::getTypeName()+"Swatch");
	IfMErrorMsgReturnIt( status, "Can't deregister "+elvishray::SkyLightNode::getTypeName()+"Swatch generator" );
	status = plugin.deregisterNode( elvishray::SkyLightNode::getTypeId() );
	IfMErrorMsgReturnIt( status, "Can't deregister "+elvishray::SkyLightNode::getTypeName()+" node" );
	//
	status = MSwatchRenderRegister::unregisterSwatchRender(elvishray::PhysicalskyNode::getTypeName()+"Swatch");
	IfMErrorMsgReturnIt( status, "Can't deregister "+elvishray::PhysicalskyNode::getTypeName()+"Swatch generator" );
	status = plugin.deregisterNode( elvishray::PhysicalskyNode::getTypeId() );
	IfMErrorMsgReturnIt( status, "Can't deregister "+elvishray::PhysicalskyNode::getTypeName()+" node" );
	//
	//
	status = plugin.deregisterNode( elvishray::GlobalNode::getTypeId() );
	IfMErrorMsgReturnIt( status, "Can't deregister "+elvishray::GlobalNode::getTypeName()+" node" );
	//
	elvishray::NodeAddCallback::removeCallback();
	//
	liquid::RendererMgr::unregistFactory(elvishray::RENDER_NAME.asChar());
	//
	//_uninitializePlugin(obj);

  return MS::kSuccess;
}
