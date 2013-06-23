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
** ElvishRender Checker Shader Node Header
** ______________________________________________________________________
*/

#ifndef _ER_ARCHITECTURAL_NODE_H_
#define _ER_ARCHITECTURAL_NODE_H_

#include <common/prerequest_maya.h>
//#include <liqShaderNode.h>
class liqNodeSwatch;

namespace elvishray
{

class ArchitecturalNode : public MPxNode
{
  public:
                      ArchitecturalNode();
    virtual          ~ArchitecturalNode();

    virtual MStatus   compute( const MPlug&, MDataBlock& );
    virtual void      postConstructor();

    static  void *    creator();
    static  MStatus   initialize();
    static  MStatus   initialize_shader_parameters();


	static  const MTypeId   getTypeId();
	static  const MString   getTypeName();

    liqNodeSwatch*    renderSwatch;

  private:
	  // Input attributes
	  static MObject aRmanShader;
	  static MObject aRmanShaderType;//for liqShader::shader_type_ex
	  static MObject aRmanShaderLong;
	  static MObject aRmanShaderLif;
	  static MObject aRmanParams;
	  static MObject aRmanDetails;
	  static MObject aRmanTypes;
	  static MObject aRmanDefaults;
	  static MObject aRmanArraySizes;
	  static MObject aRmanLifCmds;
	  static MObject aRmanMethods;
	  static MObject aRmanIsOutput;
	  static MObject aRmanAccept;


	  static MObject aOutColor;
	  static MObject aOutTransparency;
//	  static MObject aAssignedObjects;

	  //shader parameters
	  static MObject asurface_color;
	  static MObject adiffuse_color;
	  static MObject adiffuse_weight;
	  static MObject aspecular_color;
	  static MObject aspecular_weight;
	  static MObject aroughness;
	  static MObject aspecular_mode;
	  static MObject aglossiness;
	  static MObject areflection_color;
	  static MObject areflection_weight;
	  static MObject arefraction_color;
	  static MObject arefraction_weight;
	  static MObject arefraction_glossiness;
	  static MObject arefraction_thickness;
	  static MObject atranslucency_color;
	  static MObject atranslucency_weight;
	  static MObject atranslucency_mode;
	  static MObject asss_color;
	  static MObject asss_scale_color;
	  static MObject asss_scale_weight;
	  static MObject asss_phase;
	  static MObject aanisotropy;
	  static MObject arotation;
	  static MObject aior;
	  static MObject afresnel_by_ior;
	  static MObject afresnel_0_degree_refl;
	  static MObject afresnel_90_degree_refl;
	  static MObject afresnel_curve;
	  static MObject ais_metal;
	  static MObject adiffuse_samples;
	  static MObject areflection_samples;
	  static MObject arefraction_samples;
	  static MObject asss_samples;
	  static MObject acutoff_threshold;
	  static MObject abump_shader;
	  static MObject abump_factor;
	  static MObject adisplace_shader;
	  static MObject adisplace_factor;
	  //static MObject aliq_bump_shader_token;
	  //static MObject aliq_displace_shader_token;

	bool    swatchInit;

private:
	ArchitecturalNode(const ArchitecturalNode&);
	ArchitecturalNode& operator=(const ArchitecturalNode&);
};
//
}//namespace

#endif//_ER_ARCHITECTURAL_NODE_H_
