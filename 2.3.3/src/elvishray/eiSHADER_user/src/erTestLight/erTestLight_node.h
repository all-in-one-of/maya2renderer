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

#ifndef _ER_TEST_LIGHT_NODE_H_
#define _ER_TEST_LIGHT_NODE_H_

#include <common/prerequest_maya.h>
//#include <liqShaderNode.h>
class liqNodeSwatch;

namespace elvishray
{

class TestLightNode : public MPxNode
{
  public:
                      TestLightNode();
    virtual          ~TestLightNode();

    virtual MStatus   compute( const MPlug&, MDataBlock& );
    virtual void      postConstructor();

    static  void *    creator();
    static  MStatus   initialize();
    static  MStatus   initialize_shader_parameters();


	static  const MTypeId   getTypeId();
	static  const MString   getTypeName();
	static  const MString getShaderClasscification();

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
	  static MObject alightcolor;
	  static MObject aintensity;

	bool    swatchInit;

private:
	TestLightNode(const TestLightNode&);
	TestLightNode& operator=(const TestLightNode&);
};
//
}//namespace

#endif//_ER_TEST_LIGHT_NODE_H_
