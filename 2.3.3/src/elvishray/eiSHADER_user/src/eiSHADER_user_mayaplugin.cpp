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
#include <liqDefine.h>
#include "../../er_.h"
#include "../../../renderermgr.h"
#include "elvishray/er_factory.h"
#include "../../er_globalnode.h"
#include "er_user_nodeId.h"
#include "erChecker/erChecker_node_visitor.h"
#include "er_checker2/er_checker2_node_visitor.h"
#include "er_architectural/er_architectural_node_visitor.h"
#include "erTestLight/erTestLight_node_visitor.h"
#include "erFlatColor/erFlatColor_node_visitor.h"
#include "erSkylight/erSkylight_node_visitor.h"

MString getShaderPluginName()
{
	return "eiSHADER_user";
}

////////////////////// EXPORTS /////////////////////////////////////////////////////////
PLUGIN_EXPORT MStatus initializePlugin(MObject obj)
//  Description:
//      Register the command when the plug-in is loaded
{
	MStatus status;
	MFnPlugin plugin( obj, "https://github.com/maya2renderer/maya2renderer", "0.0.1", "Any");
	
	//_initializePlugin(obj);
	
	elvishray::CheckerNodeVisitor::regist(plugin);
	elvishray::Checker2NodeVisitor::regist(plugin);
	elvishray::ArchitecturalNodeVisitor::regist(plugin);
	elvishray::TestLightNodeVisitor::regist(plugin);
	elvishray::FlatColorNodeVisitor::regist(plugin);
	elvishray::erSkylightNodeVisitor::regist(plugin);

	MGlobal::executeCommand("erAddShaderPlugin(\""+getShaderPluginName()+"\")");

	return MS::kSuccess;
}

PLUGIN_EXPORT MStatus uninitializePlugin(MObject obj)
//  Description:
//      Deregister the command when the plug-in is unloaded
{
	MStatus status;
	MFnPlugin plugin(obj);

	MGlobal::executeCommand("erDelShaderPlugin(\""+getShaderPluginName()+"\")");

	elvishray::erSkylightNodeVisitor::unregist(plugin);	
	elvishray::FlatColorNodeVisitor::unregist(plugin);
	elvishray::TestLightNodeVisitor::unregist(plugin);
	elvishray::ArchitecturalNodeVisitor::unregist(plugin);
	elvishray::Checker2NodeVisitor::unregist(plugin);
	elvishray::CheckerNodeVisitor::unregist(plugin);
	
	//_uninitializePlugin(obj);

  return MS::kSuccess;
}
