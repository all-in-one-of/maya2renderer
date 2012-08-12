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
** Liquid Surface Shader Node Header
** ______________________________________________________________________
*/

#ifndef liqShaderNode_H
#define liqShaderNode_H

#include <maya/MPxNode.h>

class liqNodeSwatch;

class liqShaderNode : public MPxNode
{
  public:
                      liqShaderNode();
    virtual          ~liqShaderNode();

    virtual MStatus   compute( const MPlug&, MDataBlock& );
    virtual void      postConstructor();

    static  void *    creator();
    static  MStatus   initialize();

    //  Id tag for use with binary file format
    static  MTypeId   id;
    liqNodeSwatch*    renderSwatch;

  private:

    // Input attributes
    static MObject aRmanShader;
    static MObject aRmanShaderType;
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

    static MObject aPreviewPrimitive;
    static MObject aPreviewCustomPrimitive;
    static MObject aPreviewCustomBackplane;
    static MObject aPreviewCustomLightRig;
    static MObject aPreviewObjectSize;
    static MObject aPreviewPixelSamples;
    static MObject aPreviewShadingRate;
    static MObject aPreviewBackplane;
    static MObject aPreviewIntensity;
    static MObject aGLPreviewTexture;

    static MObject aColor;
    static MObject aOpacity;
    static MObject aDisplacementBound;
    static MObject aDisplacementBoundSpace;
    static MObject aOutputInShadow;
    static MObject aShaderSpace;

    static MObject aResolution;
    static MObject aRefreshPreview;

    static MObject aCi;
    static MObject aOi;

    static MObject aMayaIgnoreLights;
    static MObject aMayaKa;
    static MObject aMayaKd;
    static MObject aNormalCameraX;
    static MObject aNormalCameraY;
    static MObject aNormalCameraZ;
    static MObject aNormalCamera;
    static MObject aLightDirectionX;
    static MObject aLightDirectionY;
    static MObject aLightDirectionZ;
    static MObject aLightDirection;
    static MObject aLightIntensityR;
    static MObject aLightIntensityG;
    static MObject aLightIntensityB;
    static MObject aLightIntensity;
    static MObject aLightAmbient;
    static MObject aLightDiffuse;
    static MObject aLightSpecular;
    static MObject aLightShadowFraction;
    static MObject aPreShadowIntensity;
    static MObject aLightBlindData;
    static MObject aLightData;

    static MObject aEvaluateAtEveryFrame;

    // Output attributes
    static MObject aOutColor;
    static MObject aOutTransparency;
    static MObject aAssignedObjects;

	bool    swatchInit;

private:
	liqShaderNode(const liqShaderNode&);
	liqShaderNode& operator=(const liqShaderNode&);
};




#endif
