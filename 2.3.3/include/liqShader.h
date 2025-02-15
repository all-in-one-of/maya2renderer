/*
**
** The contents of this file are subject to the Mozilla Public License Version 1.1 (the
** "License"); you may not use this file except in compliance with the License. You may
** obtain a copy of the License at http://www.mozilla.org/MPL/
**
** Software distributed under the License is distributed on an "AS IS" basis, WITHOUT
** WARRANTY OF ANY KIND, either express or implied. See the License for the specific
** language governing rights and limitations under the License.
**
** The Original Code is the Liquid Rendering Toolkit.
**
** The Initial Developer of the Original Code is Colin Doncaster. Portions created by
** Colin Doncaster are Copyright (C) 2002. All Rights Reserved.
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
#ifndef liqShader_H
#define liqShader_H

#include <common/prerequest_std.h>
#include <common/prerequest_maya.h>

#include "liqtypes.h"//#include "ri_interface.h"

#include "liqShaderParameterType.h"
#include "liqTokenParameterType.h"

#define MR_SURFPARAMSIZE 1024

class liqTokenPointer;
class liqRibLightData;

class LIQUID_EXPORT liqShader
{
public :
    liqShader();
    liqShader( const liqShader & src );
    liqShader & operator=( const liqShader & src );
    explicit liqShader ( MObject shaderObj );
    virtual ~liqShader();
	//MStatus liqShaderParseVectorAttr( const MFnDependencyNode& shaderNode, const std::string& argName, ParameterType pType );
    //MStatus liqShaderParseVectorArrayAttr( const MFnDependencyNode& shaderNode, const std::string& argName, ParameterType pType, unsigned int arraySize );
	//MStatus liqShaderParseMatrixAttr ( const MFnDependencyNode& shaderNode, const std::string& argName, ParameterType pType );

	void appendCoShader(MObject coshader, MPlug plug);
	void write();
    //void writeAsCoShader();
	//void outputIndentation(unsigned int indentLevel);

	void buildJobs();
	void scanExpressions( liqShader & currentShader );
	void processExpression( liqTokenPointer *token, const liqRibLightData *light = NULL );

    // void freeShader( void ); -- not needed anymore. vector calls the dtors itself when going out of scope
    // int numTPV; -- handled by tokenPointerArray.size() now

	std::string getName() const;
	std::string getShaderFileName() const;

	void setShaderFileName(const std::string &file_){ file = file_; }
	
	void getValidConnection( MStringArray& connections )const;
	
	const MObject getMObject()const{ return m_mObject; }

protected:
	void init();

	std::string      name;
	std::string      file;

public:
    liqColor     rmColor;
    liqColor     rmOpacity;
    bool        hasShadingRate;
    liqFloat     shadingRate;
    bool        hasDisplacementBound;
    liqFloat     displacementBound;
    bool        outputInShadow;
    bool        hasErrors;
    VOLUME_TYPE volume_type;
    bool        useVisiblePoints; // New for PPMAN 16.x: use VP.. shader version
	
    //SHADER_TYPE shader_type;
	//use shader_type_ex instead of shader_type for all renderers
	//e.g."surface","volume","light",... for renderman
	//    "surface","volume","light","shadow",... for elvishray
	//    "surfaceShader","BRDF,"EDF",... for appleseed
	MString     shader_type_ex;
    MString     shaderSpace;
    MString     shaderHandler;
    int         evaluateAtEveryFrame;
    MObject     m_mObject;
    float		m_previewGamma;
	SHADER_TYPE forceAs;

    std::vector< liqTokenPointer	> tokenPointerArray;
    std::vector< MObject > m_coShaderArray;
};


#endif // liqShader_H
