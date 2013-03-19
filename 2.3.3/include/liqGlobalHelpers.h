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

#ifndef liqGlobalHelpers_H
#define liqGlobalHelpers_H

/* ______________________________________________________________________
**
** Liquid Liquid Global Helpers Header File
** ______________________________________________________________________
*/

#include <string>
#include <vector>

#include <maya/MString.h>
#include <maya/MFnDependencyNode.h>
#include <maya/MObject.h>
#include <maya/MDagPath.h>
#include <maya/MVector.h>
#include <maya/MIntArray.h>

#include <liquid.h>
#include <liqTokenPointer.h>

typedef enum liquidVerbosityType {
  verbosityOff = 0,
  verbosityErrors = 1,
  messageError = 1,
  verbosityErrorsWarnings = 2,
  messageWarning = 2,
  verbosityAll = 3,
  messageInfo = 3
} liquidVerbosityType;

// Moritz: basename() is missing in Windoze, we define our own in liqGlobalHelpers.cpp
#ifdef _WIN32
char* basename( const char* filename );
#else
#  include <libgen.h> // for basename()
#endif

#define LIQ_GET_SHADER_FILE_NAME(a, b, c) if( b ) a = basename( const_cast<char *>(c.getShaderFileName().c_str())); else a = const_cast<char *>(c.getShaderFileName().c_str());


LIQUID_EXPORT MStringArray findAttributesByPrefix(const char* pPrefix, MFnDependencyNode& NodeFn );
LIQUID_EXPORT bool isObjectTwoSided( const MDagPath & path );
LIQUID_EXPORT bool isObjectReversed( const MDagPath & path );
LIQUID_EXPORT bool isObjectVisible( const MDagPath & path );
LIQUID_EXPORT bool isObjectPrimaryVisible( const MDagPath & path );
LIQUID_EXPORT bool isObjectTemplated( const MDagPath & path );
LIQUID_EXPORT bool isObjectCastsShadows( const MDagPath & path );
LIQUID_EXPORT bool isObjectReceivesShadows( const MDagPath & path );
LIQUID_EXPORT bool isObjectMotionBlur( const MDagPath & path );
LIQUID_EXPORT bool areObjectAndParentsVisible( const MDagPath & path );
LIQUID_EXPORT bool areObjectAndParentsTemplated( const MDagPath & path );
LIQUID_EXPORT void assignTokenArrays( unsigned numTokens, const liqTokenPointer tokenPointerArray[], liqToken tokens[], liqPointer pointers[] );
LIQUID_EXPORT void assignTokenArraysV( const std::vector<liqTokenPointer>& tokenPointerArray, liqToken tokens[], liqPointer pointers[] );
LIQUID_EXPORT void assignIthTokenArraysV( const std::vector<liqTokenPointer>& tokenPointerArray, liqToken tokens[], liqPointer pointers[], unsigned int primi );
LIQUID_EXPORT MObject findFacetShader( MObject mesh, int polygonIndex );
LIQUID_EXPORT bool fileExists( const MString & filename );
LIQUID_EXPORT bool fileIsNewer( const MString & file1, const MString & file2 );
LIQUID_EXPORT bool fileFullyAccessible( const MString& path );
LIQUID_EXPORT MString getFullPathFromRelative ( const MString & filename );
LIQUID_EXPORT MString getFileName( const MString & fullpath );
LIQUID_EXPORT MString getFileNameExtention( const MString& fullpath );
LIQUID_EXPORT MString parseString( const MString & inputString, bool doEscaped = true );
LIQUID_EXPORT MString parseCommandString( const MString & inputString );
LIQUID_EXPORT MString liquidTransGetSceneName();
LIQUID_EXPORT MString liquidTransGetFullSceneName();
LIQUID_EXPORT void liquidGetGlobal( MString globalName, double &value, MStatus &returnStatus );
LIQUID_EXPORT liquidlong liquidHash( const char *str );
LIQUID_EXPORT MString liquidSanitizePath( const MString& inputString );
LIQUID_EXPORT std::string liquidSanitizePath( const std::string& inputString );
LIQUID_EXPORT MString liquidSanitizeSearchPath( const MString& inputString );
LIQUID_EXPORT std::string liquidSanitizeSearchPath( const std::string& inputString );
LIQUID_EXPORT std::string liquidGetRelativePath( bool relative, const std::string& name, const std::string& dir );
LIQUID_EXPORT MString liquidGetRelativePath( bool relative, const MString& name, const MString& dir );
LIQUID_EXPORT MString removeEscapes( const MString & inputString );
LIQUID_EXPORT MObject getNodeByName( MString name, MStatus *returnStatus );
LIQUID_EXPORT std::string getEnvironment( const std::string& envVar );
LIQUID_EXPORT std::vector< int > generateFrameNumbers( const std::string& mayaSeq );
LIQUID_EXPORT bool makeFullPath( const std::string& name, int mode );
LIQUID_EXPORT std::string sanitizeNodeName( const std::string& name );
LIQUID_EXPORT MString sanitizeNodeName( const MString& name );
//replace ':' with '='
LIQUID_EXPORT std::string sanitizeNodeName2( const std::string& name );
//replace ':' with '=', and '|' with '/'
LIQUID_EXPORT std::string sanitizeNodeName2_ToFileSystemPath(const std::string &name);
LIQUID_EXPORT liqString& getLiquidRibName( const std::string& name );
LIQUID_EXPORT void liquidMessage( const MString &msg, liquidVerbosityType type );
LIQUID_EXPORT MString parseLiquidRibRequest( MStringArray requestArray, MString attr );

LIQUID_EXPORT void initalizeShaderHandlerGenerator();
LIQUID_EXPORT std::string getUniqueShaderHandler();

LIQUID_EXPORT MStatus liquidGetPlugValue( MFnDependencyNode node, const char *name, bool &value, MStatus &status );
LIQUID_EXPORT MStatus liquidGetPlugValue( MFnDependencyNode node, const char *name, int &value, MStatus &status );
LIQUID_EXPORT MStatus liquidGetPlugValue( MFnDependencyNode node, const char *name, liqFloat &value, MStatus &status );
LIQUID_EXPORT MStatus liquidGetPlugValue( MFnDependencyNode node, const char *name, double &value, MStatus &status );
LIQUID_EXPORT MStatus liquidGetPlugValue( MFnDependencyNode node, const char *name, MString &value, MStatus &status, bool parsed = false );
LIQUID_EXPORT MStatus liquidGetPlugValue( MFnDependencyNode node, const char *name, MVector &value, MStatus &status );
LIQUID_EXPORT unsigned int liquidGetPlugNumElements( MFnDependencyNode node, const char *name, MStatus *status );
LIQUID_EXPORT MStatus liquidGetPlugElementValue( MFnDependencyNode node, unsigned int ind, const char *name, MString &value, MStatus &status );
LIQUID_EXPORT MStatus liquidGetPlugElementValue( MFnDependencyNode node, unsigned int ind, const char *name, int &value, MStatus &status );
LIQUID_EXPORT MStatus liquidGetPlugElementValue( MFnDependencyNode node, unsigned int ind, const char *name, bool &value, MStatus &status );
LIQUID_EXPORT MStatus liquidGetPlugElementValue( MFnDependencyNode node, unsigned int ind, const char *name, float &value, MStatus &status );
LIQUID_EXPORT MStatus liquidGetPlugElementValue( MFnDependencyNode node, unsigned int ind, const char *name, MStringArray &array, MStatus &status );
LIQUID_EXPORT MStatus liquidGetPlugElementValue( MFnDependencyNode node, unsigned int ind, const char *name, MIntArray &array, MStatus &status );

class arrayDeleter // needed to free a shared/scoped/weak_ptr to an array
{
  public:
    template< typename T >
    void operator()( T* t ) {
      delete[] t;
    }
};
LIQUID_EXPORT const MString replaceAll(const MString& str, const char from, const char to);
LIQUID_EXPORT MString generateShadowArchiveName( bool renderAllFrames, long renderAtframe, MString geometrySet );
LIQUID_EXPORT MString getBaseShadowName(const structJob &job__);

LIQUID_EXPORT void getDagPathByName(MDagPath& dagPath, char const* name);
LIQUID_EXPORT void getDependNodeByName(MObject& depNode, char const* name);
LIQUID_EXPORT void getNodeType(MString& type,  MString const& node);
LIQUID_EXPORT MString getNodeType(MString const& node);
LIQUID_EXPORT MString getWorkspaceDirectory();
LIQUID_EXPORT MString getShaderDirectory();
LIQUID_EXPORT bool is2DFileTexture(const MString& node);
LIQUID_EXPORT bool is2DTexture(const MString& node);
LIQUID_EXPORT bool is3DTexture(const MString& node);
LIQUID_EXPORT bool isMiTexture(const MString& node);
LIQUID_EXPORT bool isBatchMode();

LIQUID_EXPORT int find_first_of(const MString& search, const MStringArray& array);

LIQUID_EXPORT std::string toFullDagPath(const std::string& name);
LIQUID_EXPORT MString toFullDagPath(const MString& name);
LIQUID_EXPORT void toFullDagPath(MStringArray& array);

LIQUID_EXPORT std::string getObjectName(const char *ribDataName);
LIQUID_EXPORT std::string getProceduralObjectName(const char *ribDataName);

LIQUID_EXPORT void printFrameSequence(const char* prefix);

LIQUID_EXPORT void getlistConnections(const MString& shadingGroupNode,
						const MString& plug, 
						MStringArray& connections );
LIQUID_EXPORT bool doesPlugExist(const MString& node, const MString& plug);
//
LIQUID_EXPORT MString getLightGroupName(const MString& meshShapeNodes);
//
LIQUID_EXPORT bool liqAssert(const MString & msg);
LIQUID_EXPORT bool liqAssert(const MString &title, const MString & msg, const MString &bYes);
LIQUID_EXPORT bool liqAssert(const MString &title, const MString & msg, const MString &bYes, const MString &bNo);
//
LIQUID_EXPORT MString getFileNodeImageName(const MString &node);

LIQUID_EXPORT MString liqMerge(const MStringArray& sa, const char split);
#define liqM(msa) liqMerge(msa,'|').asChar()
//
LIQUID_EXPORT bool isZero(const float r, const float g, const float b);
//
LIQUID_EXPORT MString getShaderName(const MString &shaderName);
LIQUID_EXPORT MString getVariableName(const MString &varName);
LIQUID_EXPORT MString getVariableName(const MString &node,const MString &plug);
//
LIQUID_EXPORT MString getTextureExt();
LIQUID_EXPORT bool isTextureTypeSupported(const MString &textureType);
//
LIQUID_EXPORT MString getLogFileDir();
LIQUID_EXPORT MString getLogFileName();
//
LIQUID_EXPORT MString evaluateTheTextureNameValue(const MString &plugName, const MString &plugValue);
#endif
