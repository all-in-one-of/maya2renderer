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

// Maya's Headers
#include <maya/MFnPlugin.h>
#include <maya/MGlobal.h>
#include <maya/MPxCommand.h>
#include <maya/MSwatchRenderRegister.h>

#include "../renderermgr.h"
#include "../elvishray/er_factory.h"
#include "er_globalnode.h"


////////////////////// EXPORTS /////////////////////////////////////////////////////////
PLUGIN_EXPORT MStatus initializePlugin(MObject obj)
//  Description:
//      Register the command when the plug-in is loaded
{
	MStatus status;
	MFnPlugin plugin( obj, "https://github.com/maya2renderer/maya2renderer", "0.0.1", "Any");
	//_initializePlugin(obj);

	// register the Globals node
	status = plugin.registerNode( 
		elvishray::GlobalNode::typeName, 
		elvishray::GlobalNode::typeId, 
		elvishray::GlobalNode::creator, elvishray::GlobalNode::initialize, MPxNode::kDependNode );
	LIQCHECKSTATUS( status, "Can't register elvishray::GlobalNode node" );
	status.clear();

  liquid::RendererMgr::registFactory("elvishray", new elvishray::Factory());
  
  return MS::kSuccess;
}

PLUGIN_EXPORT MStatus uninitializePlugin(MObject obj)
//  Description:
//      Deregister the command when the plug-in is unloaded
{
	MStatus status;
	MFnPlugin plugin(obj);
	liquid::RendererMgr::unregistFactory("elvishray");

	status = plugin.deregisterNode( elvishray::GlobalNode::typeId );
	LIQCHECKSTATUS( status, "Can't deregister elvishray::GlobalNode node" );

	//_uninitializePlugin(obj);

  return MS::kSuccess;
}
