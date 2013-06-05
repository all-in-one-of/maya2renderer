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
#include <liqShader.h>

#include <liqlog.h>
#include <liqGlobalHelpers.h>
#include <liqMayaNodeIds.h>
#include <liqShaderFactory.h>
#include <liqRibLightData.h>
#include <liqRibTranslator.h>
#include <liqExpression.h>
//#include <liqTokenPointer.h>
#include <liqGetSloInfo.h>
#include "renderermgr.h"
#include "common/mayacheck.h"
//using namespace std;
//using namespace boost;


liqShader::liqShader()
{
	CM_TRACE_FUNC("liqShader::liqShader()");

  //numTPV                = 0;
  name                  = "";
  file                  = "";
  rmColor[0]            = 1.0;
  rmColor[1]            = 1.0;
  rmColor[2]            = 1.0;
  rmOpacity[0]          = 1.0;
  rmOpacity[1]          = 1.0;
  rmOpacity[2]          = 1.0;
  hasShadingRate        = false;
  shadingRate           = 1.0;
  hasDisplacementBound  = false;
  displacementBound     = 0.0;
  outputInShadow        = false;
  hasErrors             = false;
//  shader_type           = SHADER_TYPE_UNKNOWN;
  shader_type_ex        = "";
  volume_type           = VOLUME_TYPE_ATMOSPHERE;
  useVisiblePoints      = false;  // New for PPMAN 16.x: use VP.. shader version
  shaderSpace           = "";
  evaluateAtEveryFrame  = 0;
  tokenPointerArray.push_back( liqTokenPointer() ); // ENsure we have a 0 element
  shaderHandler         = liqShaderFactory::instance().getUniqueShaderHandler();
  m_previewGamma		= 1.0f;
  forceAs				= SHADER_TYPE_UNKNOWN;
}

liqShader::liqShader( const liqShader& src )
{
	CM_TRACE_FUNC("liqShader::liqShader("<<src.name<<")");
  //numTPV               = src.numTPV;
  tokenPointerArray    = src.tokenPointerArray;
  name                 = src.name;
  file                 = src.file;
  rmColor[0]           = src.rmColor[0];
  rmColor[1]           = src.rmColor[1];
  rmColor[2]           = src.rmColor[2];
  rmOpacity[0]         = src.rmOpacity[0];
  rmOpacity[1]         = src.rmOpacity[1];
  rmOpacity[2]         = src.rmOpacity[2];
  hasShadingRate       = src.hasShadingRate;
  shadingRate          = src.shadingRate;
  hasDisplacementBound = src.hasDisplacementBound;
  displacementBound    = src.displacementBound;
  outputInShadow       = src.outputInShadow;
  hasErrors            = src.hasErrors;
//  shader_type        = src.shader_type;
  shader_type_ex       = src.shader_type_ex;
  volume_type          = src.volume_type;
  useVisiblePoints     = src.useVisiblePoints;
  shaderSpace          = src.shaderSpace;
  evaluateAtEveryFrame = src.evaluateAtEveryFrame;
  shaderHandler        = src.shaderHandler;
  m_mObject            = src.m_mObject;
  m_previewGamma       = src.m_previewGamma;
  forceAs			   = src.forceAs;
}

liqShader & liqShader::operator=( const liqShader & src )
{
	//CM_TRACE_FUNC("liqShader::operator=(src)");
  //numTPV = src.numTPV;
  tokenPointerArray     = src.tokenPointerArray;
  name                  = src.name;
  file                  = src.file;
  rmColor[0]            = src.rmColor[0];
  rmColor[1]            = src.rmColor[1];
  rmColor[2]            = src.rmColor[2];
  rmOpacity[0]          = src.rmOpacity[0];
  rmOpacity[1]          = src.rmOpacity[1];
  rmOpacity[2]          = src.rmOpacity[2];
  hasShadingRate        = src.hasShadingRate;
  shadingRate           = src.shadingRate;
  hasDisplacementBound  = src.hasDisplacementBound;
  displacementBound     = src.displacementBound;
  outputInShadow        = src.outputInShadow;
  hasErrors             = src.hasErrors;
//  shader_type         = src.shader_type;
  shader_type_ex        = src.shader_type_ex;
  volume_type           = src.volume_type;
  useVisiblePoints     = src.useVisiblePoints;
  shaderSpace           = src.shaderSpace;
  evaluateAtEveryFrame = src.evaluateAtEveryFrame;
  shaderHandler        = src.shaderHandler;
  m_mObject            = src.m_mObject;
  m_previewGamma       = src.m_previewGamma;
  forceAs			   = src.forceAs;
  return *this;
}


liqShader::liqShader( MObject shaderObj )
{
	CM_TRACE_FUNC("liqShader::liqShader("<<MFnDependencyNode(shaderObj).name().asChar()<<")");

	forceAs = SHADER_TYPE_UNKNOWN;

    bool outputAllParameters = true;
	
	MString rmShaderStr;
	MStatus status;
	MFnDependencyNode shaderNode( shaderObj );
	MPlug rmanShaderNamePlug = shaderNode.findPlug( MString( "rmanShaderLong" ) );
	rmanShaderNamePlug.getValue( rmShaderStr );
	shaderHandler = liqShaderFactory::instance().getUniqueShaderHandler();

	LIQDEBUGPRINTF( "-> Using Renderman Shader %s. \n", rmShaderStr.asChar() );

	unsigned numArgs;
	//numTPV = 0;
	hasShadingRate = false;
	hasDisplacementBound = false;
	outputInShadow = false;
	hasErrors = false;
	tokenPointerArray.push_back( liqTokenPointer() );
	m_mObject = shaderObj;

	// if this shader instance isn't currently used already then load it into the
	// lookup set it as my slo lookup
	name = shaderNode.name().asChar();
	file = rmShaderStr.substring( 0, rmShaderStr.length() - 5 ).asChar();

	rmColor[0]            = 1.0;
	rmColor[1]            = 1.0;
	rmColor[2]            = 1.0;
	rmOpacity[0]          = 1.0;
	rmOpacity[1]          = 1.0;
	rmOpacity[2]          = 1.0;

	liqGetSloInfo shaderInfo;


// commented out for it generates errors - Alf
	int success = ( shaderInfo.setShaderNode( shaderNode ) );
	if ( !success )
	{
		liquidMessage( "Problem using shader '" + shaderNode.name() + "'", messageError );
		rmColor[0] = 1.0;
		rmColor[1] = 0.0;
		rmColor[2] = 0.0;
		name = "plastic";
//		numTPV = 0;
		hasErrors = true;
	}
	else
	{
		/* Used to handling shading rates set in the surface shader,
		this is a useful way for shader writers to ensure that their
		shaders are always rendered as they were designed.  This value
		overrides the global shading rate but gets overridden with the
		node specific shading rate. */

		shader_type_ex = shaderInfo.getTypeStr2();
		// Set RiColor and RiOpacity
		status.clear();
		MPlug colorPlug = shaderNode.findPlug( "color" );
		if ( MS::kSuccess == status )
		{
			colorPlug.child(0).getValue( rmColor[0] );
			colorPlug.child(1).getValue( rmColor[1] );
			colorPlug.child(2).getValue( rmColor[2] );
		}

		status.clear();
		MPlug opacityPlug( shaderNode.findPlug( "opacity" ) );
		// Moritz: changed opacity from float to color in MEL
		if ( MS::kSuccess == status )
		{
		  opacityPlug.child(0).getValue( rmOpacity[0] );
		  opacityPlug.child(1).getValue( rmOpacity[1] );
		  opacityPlug.child(2).getValue( rmOpacity[2] );
		}
		int volumeType = volume_type; 
		liquidGetPlugValue( shaderNode, "volumeType", volumeType, status );
		if ( MS::kSuccess == status ) volume_type = (VOLUME_TYPE)volumeType;

		liquidGetPlugValue( shaderNode, "useVisiblePoints", useVisiblePoints, status );

		liquidGetPlugValue( shaderNode, "shaderSpace", shaderSpace, status );
		liquidGetPlugValue( shaderNode, "outputInShadow", outputInShadow, status );
		liquidGetPlugValue( shaderNode, "evaluateAtEveryFrame", evaluateAtEveryFrame, status );
		liquidGetPlugValue( shaderNode, "previewGamma", m_previewGamma, status );

		// find the parameter details and declare them in the rib stream
		numArgs = shaderInfo.getNumParam();
		for (unsigned int i( 0 ); i < numArgs; i++ )
		{
			MString paramName = shaderInfo.getArgName(i);
			int arraySize = shaderInfo.getArgArraySize(i);
			SHADER_TYPE shaderParameterType = shaderInfo.getArgType(i);
			SHADER_DETAIL shaderDetail = shaderInfo.getArgDetail(i);
			MString shaderAccept = shaderInfo.getArgAccept(i);
			if( shaderParameterType == SHADER_TYPE_STRING )
			{
				// check if a string must be used as a shader
				if( shaderAccept != "" )
				{
					shaderParameterType = SHADER_TYPE_SHADER;
				}
			}

			bool skipToken = false;
			if ( paramName == "liquidShadingRate" )
			{
				// BUGFIX: Monday 6th August - fixed shading rate bug where it only accepted the default value
				MPlug floatPlug = shaderNode.findPlug( paramName, &status );
				if ( MS::kSuccess == status )
				{
					float floatPlugVal;
					floatPlug.getValue( floatPlugVal );
					shadingRate = floatPlugVal;
				}
				else
					shadingRate = shaderInfo.getArgFloatDefault( i, 0 );
				
				hasShadingRate = true;
				continue;
			}

			if( shaderInfo.isOutputParameter(i) && !outputAllParameters )   // throw output parameters
			{
				continue;		
			}
			else if( shaderInfo.isOutputParameter(i) && outputAllParameters )
			{
				if( arraySize == -1 )    // single value
				{
					switch ( shaderParameterType )
					{
						case SHADER_TYPE_SHADER:
						{
							ParameterType parameterType = rShader;
							MString s = shaderInfo.getArgStringDefault( i, 0 );
							tokenPointerArray.rbegin()->set( paramName.asChar(), parameterType );
							tokenPointerArray.rbegin()->setTokenString( 0, s.asChar() );
							break;
						}
						case SHADER_TYPE_STRING:
						{
							ParameterType parameterType = rString;
							MString s = shaderInfo.getArgStringDefault( i, 0 );
							tokenPointerArray.rbegin()->set( paramName.asChar(), parameterType );
							tokenPointerArray.rbegin()->setTokenString( 0, s.asChar() );
							break;
						}
						case SHADER_TYPE_SCALAR:
						{
							ParameterType parameterType = rFloat;
							float x = shaderInfo.getArgFloatDefault( i, 0 );
							tokenPointerArray.rbegin()->set( paramName.asChar(), parameterType );
							tokenPointerArray.rbegin()->setTokenFloat( 0, x );
							break;
						}
						case SHADER_TYPE_INT:
						{
							ParameterType parameterType = rInt;
							int x = shaderInfo.getArgIntDefault( i, 0 );
							tokenPointerArray.rbegin()->set( paramName.asChar(), parameterType );
							tokenPointerArray.rbegin()->setTokenFloat( 0, x );//setTokenInt()
							break;
						}
						case SHADER_TYPE_BOOL:
							{
								ParameterType parameterType = rBool;
								bool x = shaderInfo.getArgBoolDefault( i, 0 );
								tokenPointerArray.rbegin()->set( paramName.asChar(), parameterType );
								tokenPointerArray.rbegin()->setTokenFloat( 0, x );//setTokenBool()
								break;
							}
						case SHADER_TYPE_COLOR:
						case SHADER_TYPE_POINT:
						case SHADER_TYPE_VECTOR:
						case SHADER_TYPE_NORMAL:
						{
							ParameterType parameterType;
							if( shaderParameterType==SHADER_TYPE_COLOR )
							{
								parameterType = rColor;
							}
							else if(shaderParameterType==SHADER_TYPE_POINT)
							{
								parameterType = rPoint;
							}
							else if(shaderParameterType==SHADER_TYPE_VECTOR)
							{
								parameterType = rVector;
							}
							else if(shaderParameterType==SHADER_TYPE_NORMAL)
							{
								parameterType = rNormal;
							}
							float x = shaderInfo.getArgFloatDefault( i, 0 );
							float y = shaderInfo.getArgFloatDefault( i, 1 );
							float z = shaderInfo.getArgFloatDefault( i, 2 );
							tokenPointerArray.rbegin()->set( paramName.asChar(), parameterType );
							tokenPointerArray.rbegin()->setTokenFloat( 0, x, y, z );
							break;
						}
						case SHADER_TYPE_MATRIX:
						{
							liquidMessage2(messageError,"[liqShader] warning cannot write output matrix parameters yet. skip param [%s] on [%s]", paramName.asChar(), shaderNode.name().asChar() );
							continue;
						}
						default:
						{
							liquidMessage2(messageError,"[liqShader] warning unhandled parameters type. skip param [%s] on [%s]", paramName.asChar(), shaderNode.name().asChar() );
							continue;
						}
					}//if( arraySize == -1 ){ switch ( shaderParameterType )
				}//if( arraySize == -1 )
				else
				{
					liquidMessage2(messageError,"[liqShader] warning cannot write output array parameters yet. skip param [%s] on [%s]", paramName.asChar(), shaderNode.name().asChar() );
					continue;
				}
			}
			else//shaderInfo.isOutputParameter(i) is false
			{
			switch ( shaderParameterType )
			{
				case SHADER_TYPE_SHADER:
				{
					ParameterType parameterType = rShader;  // rString
					MPlug coShaderPlug = shaderNode.findPlug( paramName, &status );
					if ( MS::kSuccess != status )
					{
						skipToken = true;
						liquidMessage2(messageError,"[liqShader] error while building shader param [%s] on [%s] ...", paramName.asChar(), shaderNode.name().asChar() );
					}
					else
					{
						if( arraySize==0 )    // dynamic array
						{
							MIntArray indices;
							coShaderPlug.getExistingArrayAttributeIndices(indices);
							if( indices.length() == 0 )
							{
								skipToken = true;
							}
							else//indices.length() != 0
							{
								int maxIndex = 0;
								for(unsigned int kk( 0 ); kk<indices.length(); kk++)
								{
									if( indices[kk]>maxIndex )
									{
										maxIndex = indices[kk];
									}
								}
								arraySize = maxIndex + 1;								
								tokenPointerArray.rbegin()->set( paramName.asChar(), parameterType, arraySize );
								for( unsigned int kk( 0 ); kk < (unsigned int)arraySize; kk++ )
								{
									bool existingIndex = false;
									for(unsigned int kkk( 0 ); kkk<indices.length(); kkk++)
									{
										if(kk==indices[kkk])
										{
											existingIndex = true;
											continue;
										}
									}
									if( existingIndex )  // get plug value
									{
										MPlug argNameElement = coShaderPlug.elementByLogicalIndex(kk);
										MString coShaderHandler;
											
										if( argNameElement.isConnected() )
										{
											bool asSrc = 0;
											bool asDst = 1;
											MPlugArray connectedPlugArray;
											argNameElement.connectedTo( connectedPlugArray, asDst, asSrc );
												
											MObject coshader = connectedPlugArray[0].node();
											appendCoShader(coshader, connectedPlugArray[0]);
#if 0
											coShaderHandler = liqShaderFactory::instance().getShaderId(coshader);
#else
											MFnDependencyNode fnObject(coshader);
											coShaderHandler = fnObject.name();
											liquidMessage2(messageWarning, "I use shader name [%s] for coShaderHandler in plug [%s]", coShaderHandler.asChar(), coShaderPlug.name().asChar());
#endif
										}
										else
										{
											coShaderHandler = "";
										}									
										tokenPointerArray.rbegin()->setTokenString( kk, coShaderHandler.asChar() );
									}//if( existingIndex )
									else  // don't mind about value
									{
										tokenPointerArray.rbegin()->setTokenString( kk, "" );
									}
								}//for( kk
							}//indices.length() == 0
						}//if( arraySize==0 )
						else if ( arraySize > 0 )    // static array
						{
							bool isArrayAttr( coShaderPlug.isArray( &status ) );
							if ( isArrayAttr )
							{
							std::vector<MString> coShaderHandlers;

							for( unsigned int kk( 0 ); kk < (unsigned int)arraySize; kk++ )
							{
								MPlug argNameElement = coShaderPlug.elementByLogicalIndex(kk);
								MString coShaderHandler;
								if( argNameElement.isConnected() )
								{
									bool asSrc = 0;
									bool asDst = 1;
									MPlugArray connectedPlugArray;
									argNameElement.connectedTo( connectedPlugArray, asDst, asSrc );
										
									MObject coshader = connectedPlugArray[0].node();
									appendCoShader(coshader, connectedPlugArray[0]);
#if 0
									coShaderHandler = liqShaderFactory::instance().getShaderId(coshader);
#else
									MFnDependencyNode fnObject(coshader);
									coShaderHandler = fnObject.name();
									liquidMessage2(messageWarning, "I use shader name [%s] for coShaderHandler in plug [%s]", coShaderHandler.asChar(), coShaderPlug.name().asChar());
#endif
								}
								else
								{
									coShaderHandler = "";
								}
								coShaderHandlers.push_back(coShaderHandler);
							}
								
							int isDefault = 1;
							for( unsigned int kk( 0 ); kk < (unsigned int)arraySize; kk++ )
							{
								if( coShaderHandlers[kk] != "" )
								{
									isDefault = 0;
									continue;
								}
							}
							if( isDefault && !outputAllParameters )  // skip default
							{
								skipToken = true;
							}
							else  // build non default param
							{
								tokenPointerArray.rbegin()->set( paramName.asChar(), parameterType, arraySize );
								for( unsigned int kk( 0 ); kk < (unsigned int)arraySize; kk++ )
								{
									tokenPointerArray.rbegin()->setTokenString( kk, coShaderHandlers[kk].asChar() );
								}
							}
							}//if ( isArrayAttr )
							else
							{
								liquidMessage2(messageError, "[liqShader] error while building coshader param [%s] assumed as an array but wasn't...", coShaderPlug.name().asChar() );
							}
						}//if ( arraySize > 0 )
						else if ( arraySize == -1 )    // single value
						{
							MPlugArray connectionArray;
							bool asSrc = 0;
							bool asDst = 1;
							coShaderPlug.connectedTo(connectionArray, asDst, asSrc);
							if( connectionArray.length() == 0 )
							{
								skipToken = true;
								liquidMessage2(messageWarning, "[liqShader] warning:  coshader param [%s] is not connected in.", coShaderPlug.name().asChar() );
							}
							else
							{
								MPlug connectedPlug = connectionArray[0];
								MObject coshader = connectedPlug.node();
								appendCoShader(coshader, coShaderPlug);
#if 0
								MString coShaderId = liqShaderFactory::instance().getShaderId(coshader);
#else
								MFnDependencyNode fnObject(coshader);
								MString coShaderId(fnObject.name());
								liquidMessage2(messageWarning, "I use shader name [%s] for coShaderId in plug [%s]", coShaderId.asChar(), coShaderPlug.name().asChar());
#endif
								if( coShaderId == "" )
								{
									skipToken = true;
									liquidMessage2(messageWarning, "[liqShader] warning:  coshader param [%s]'s source is empty.", coShaderPlug.name().asChar() );
								}
								else
								{
									tokenPointerArray.rbegin()->set( paramName.asChar(), parameterType );
									tokenPointerArray.rbegin()->setTokenString( 0, coShaderId.asChar() );
								}
							}

						}//if ( arraySize == -1 )
						else    // unknown type
						{
							skipToken = true;
							liquidMessage2(messageError,"[liqShader] error while building shader param [%s] on [%s] : undefined array size [%d] ", paramName.asChar(), shaderNode.name().asChar(), arraySize );
						}
					}
					break;				
				}
				case SHADER_TYPE_STRING:
				{
					MPlug stringPlug = shaderNode.findPlug( paramName, &status );
					if ( MS::kSuccess != status )
					{
						skipToken = true;
						liquidMessage2(messageError,"[liqShader] error while building string param [%s] on [%s] ...", paramName.asChar(), shaderNode.name().asChar() );
					}
					else
					{
						if( arraySize == 0 )     // dynamic array
						{
							MIntArray indices;
							stringPlug.getExistingArrayAttributeIndices(indices);
							if( indices.length() == 0 )
							{
								skipToken = true;
							}
							else
							{
								int maxIndex = 0;
								for(unsigned int kk( 0 ); kk<indices.length(); kk++)
								{
									if( indices[kk]>maxIndex )
									{
										maxIndex = indices[kk];
									}
								}
								arraySize = maxIndex + 1;

								tokenPointerArray.rbegin()->set( paramName.asChar(), rString, arraySize );
								for( unsigned int kk( 0 ); kk < (unsigned int)arraySize; kk++ )
								{
									bool existingIndex = false;
									for(unsigned int kkk( 0 ); kkk<indices.length(); kkk++)
									{
										if(kk==indices[kkk])
										{
											existingIndex = true;
											continue;
										}
									}
									if( existingIndex )  // get plug value
									{
										MPlug argNameElement = stringPlug.elementByLogicalIndex(kk);
										MString stringPlugVal;
										argNameElement.getValue( stringPlugVal );
										MString stringVal = parseString( stringPlugVal );
										tokenPointerArray.rbegin()->setTokenString( kk, stringVal.asChar() );
									}
									else  // don't mind about value
									{
										tokenPointerArray.rbegin()->setTokenString( kk, "" );
									}
								}//for( kk
							}	
						}//if( arraySize == 0 )
						else if( arraySize > 0 )     // static array
						{
							bool isArrayAttr( stringPlug.isArray( &status ) );
							if ( isArrayAttr )
							{
								MPlug plugObj;
								// check default
								int isDefault = 1;
								for( unsigned int kk( 0 ); kk < (unsigned int)arraySize; kk++ )
								{
									plugObj = stringPlug.elementByLogicalIndex( kk, &status );
									MString stringDefault( shaderInfo.getArgStringDefault( i, kk ) );
									if( plugObj.asString() != stringDefault )
									{
										isDefault = 0;
										continue;
									}
								}
								if( isDefault && !outputAllParameters )  // skip default
								{
									skipToken = true;
								}
								else  // build non default param
								{
								tokenPointerArray.rbegin()->set( paramName.asChar(), rString, arraySize );
								for( unsigned int kk( 0 ); kk < (unsigned int)arraySize; kk++ )
								{
									plugObj = stringPlug.elementByLogicalIndex( kk, &status );
									if ( MS::kSuccess == status )
									{
										MString stringPlugVal;
										plugObj.getValue( stringPlugVal );
										MString stringVal = parseString( stringPlugVal );
										tokenPointerArray.rbegin()->setTokenString( kk, stringVal.asChar() );
									}
									else
									{
										liquidMessage2(messageError, "[liqShader] error while building param [%d] : [%s] ", kk, stringPlug.name().asChar() );
									}
								}
								}
							}
							else
							{
								liquidMessage2(messageError, "[liqShader] error while building string param [%s] assumed as an array but wasn't...", stringPlug.name().asChar() );
							}
						}//if( arraySize > 0 )
						else if( arraySize == -1 )    // single value
						{
							MString stringPlugVal;
							stringPlug.getValue( stringPlugVal );

							stringPlugVal = evaluateTheTextureNameValue(stringPlug.partialName(), stringPlugVal);
							
							MString stringDefault( shaderInfo.getArgStringDefault( i, 0 ) );
							if( stringPlugVal == stringDefault && !outputAllParameters )  // skip default
							{
								skipToken = true;
							}
							else  // build non default param
							{
								MString stringVal( parseString( stringPlugVal ) );
								liquidMessage2(messageInfo,"[liqShader::liqShader] parsed string for param [%s] = [%s] ", paramName.asChar(), stringVal.asChar() );
								tokenPointerArray.rbegin()->set( paramName.asChar(), rString );
								tokenPointerArray.rbegin()->setTokenString( 0, stringVal.asChar() );
							}
						}//if( arraySize == -1 )
						else    // unknown type
						{
							skipToken = true;
							liquidMessage2(messageError,"[liqShader] error while building string param [%s] on [%s] : undefined array size %d ", paramName.asChar(), shaderNode.name().asChar(), arraySize );
						}
					}//status == MS::kSuccess
					break;
				}
				case SHADER_TYPE_SCALAR:
				{
					MPlug floatPlug( shaderNode.findPlug( paramName, &status ) );
					if ( MS::kSuccess != status )
					{
						skipToken = true;
						liquidMessage2(messageError,"[liqShader] error while building float param [%s] on [%s] ...", paramName.asChar(), shaderNode.name().asChar() );
					}
					else
					{
						if( arraySize == 0 )    // dynamic array
						{
							MIntArray indices;
							floatPlug.getExistingArrayAttributeIndices(indices);
							if( indices.length() == 0 )
							{
								skipToken = true;
							}
							else
							{
								int maxIndex = 0;
								for(unsigned int kk( 0 ); kk<indices.length(); kk++)
								{
									if( indices[kk]>maxIndex )
									{
										maxIndex = indices[kk];
									}
								}
								arraySize = maxIndex + 1;

								tokenPointerArray.rbegin()->set( paramName.asChar(), rFloat, false, true, arraySize );
								for( unsigned int kk( 0 ); kk < (unsigned int)arraySize; kk++ )
								{
									bool existingIndex = false;
									for(unsigned int kkk( 0 ); kkk<indices.length(); kkk++)
									{
										if(kk==indices[kkk])
										{
											existingIndex = true;
											continue;
										}
									}
									if( existingIndex )  // get plug value
									{
										MPlug argNameElement = floatPlug.elementByLogicalIndex(kk);
										float value = argNameElement.asFloat();
										tokenPointerArray.rbegin()->setTokenFloat( kk, value );
									}
									else  // don't mind about value
									{
										tokenPointerArray.rbegin()->setTokenFloat( kk, 0 );
									}
								}
							}
						}//if( arraySize == 0 ) 
						else if( arraySize > 0 )    // static array
						{
							bool isArrayAttr( floatPlug.isArray( &status ) );
							if ( isArrayAttr )
							{
								MPlug plugObj;
								// check default
								int isDefault = 1;
								for( unsigned int kk( 0 ); kk < (unsigned int)arraySize; kk++ )
								{
									plugObj = floatPlug.elementByLogicalIndex( kk, &status );
									float floatDefault = shaderInfo.getArgFloatDefault( i, kk );
									if( plugObj.asFloat() != floatDefault )
									{
										isDefault = 0;
										continue;
									}
								}
								if( isDefault && !outputAllParameters ) // skip default
								{
									skipToken = true;
								}
								else  // build non default param
								{
								tokenPointerArray.rbegin()->set( paramName.asChar(), rFloat, false, true, arraySize );
								for( unsigned int kk( 0 ); kk < (unsigned int)arraySize; kk++ )
								{
									plugObj = floatPlug.elementByLogicalIndex( kk, &status );
									if ( MS::kSuccess == status )
									{
										float x;
										plugObj.getValue( x );
										tokenPointerArray.rbegin()->setTokenFloat( kk, x );
									}
								}
								}
							}
						}//if( arraySize > 0 )
						else if( arraySize == -1 )    // single value
						{
							float floatPlugVal;
							floatPlug.getValue( floatPlugVal );
							float floatDefault( shaderInfo.getArgFloatDefault( i, 0 ) );
							if( floatPlugVal == floatDefault && !outputAllParameters )  // skip default
							{
								skipToken = true;
							}
							else  // build non default param
							{
							tokenPointerArray.rbegin()->set( paramName.asChar(), rFloat );
							tokenPointerArray.rbegin()->setTokenFloat( 0, floatPlugVal );
							}
						}//if( arraySize == -1 ) 
						else    // unknown type
						{
							skipToken = true;
							liquidMessage2(messageError,"[liqShader] error while building float param [%s] on [%s] : undefined array size [%d] ", paramName.asChar(), shaderNode.name().asChar(), arraySize );
						}
					}
					break;
				}
				case SHADER_TYPE_INT:
				{
					MPlug intPlug( shaderNode.findPlug( paramName, &status ) );
					if ( MS::kSuccess != status )
					{
						skipToken = true;
						liquidMessage2(messageError,"[liqShader] error while building int param [%s] on [%s] ...", paramName.asChar(), shaderNode.name().asChar() );
					}
					else
					{
						if( arraySize == 0 )    // dynamic array
						{
							MIntArray indices;
							intPlug.getExistingArrayAttributeIndices(indices);
							if( indices.length() == 0 )
							{
								skipToken = true;
							}
							else
							{
								int maxIndex = 0;
								for(unsigned int kk( 0 ); kk<indices.length(); kk++)
								{
									if( indices[kk]>maxIndex )
									{
										maxIndex = indices[kk];
									}
								}
								arraySize = maxIndex + 1;

								tokenPointerArray.rbegin()->set( paramName.asChar(), rInt, false, true, arraySize );
								for( unsigned int kk( 0 ); kk < (unsigned int)arraySize; kk++ )
								{
									bool existingIndex = false;
									for(unsigned int kkk( 0 ); kkk<indices.length(); kkk++)
									{
										if(kk==indices[kkk])
										{
											existingIndex = true;
											continue;
										}
									}
									if( existingIndex )  // get plug value
									{
										MPlug argNameElement = intPlug.elementByLogicalIndex(kk);
										int value = argNameElement.asInt();
										tokenPointerArray.rbegin()->setTokenFloat( kk, value );//setTokenInt()
									}
									else  // don't mind about value
									{
										tokenPointerArray.rbegin()->setTokenFloat( kk, 0 );//setTokenInt()
									}
								}
							}
						}//if( arraySize == 0 ) 
						else if( arraySize > 0 )    // static array
						{
							bool isArrayAttr( intPlug.isArray( &status ) );
							if ( isArrayAttr )
							{
								MPlug plugObj;
								// check default
								int isDefault = 1;
								for( unsigned int kk( 0 ); kk < (unsigned int)arraySize; kk++ )
								{
									plugObj = intPlug.elementByLogicalIndex( kk, &status );
									int intDefault = shaderInfo.getArgIntDefault( i, kk );
									if( plugObj.asInt() != intDefault )
									{
										isDefault = 0;
										continue;
									}
								}
								if( isDefault && !outputAllParameters ) // skip default
								{
									skipToken = true;
								}
								else  // build non default param
								{
									tokenPointerArray.rbegin()->set( paramName.asChar(), rInt, false, true, arraySize );
									for( unsigned int kk( 0 ); kk < (unsigned int)arraySize; kk++ )
									{
										plugObj = intPlug.elementByLogicalIndex( kk, &status );
										if ( MS::kSuccess == status )
										{
											int x;
											plugObj.getValue( x );
											tokenPointerArray.rbegin()->setTokenFloat( kk, x );//setTokenInt()
										}
									}
								}
							}
						}//if( arraySize > 0 )
						else if( arraySize == -1 )    // single value
						{
							int intPlugVal;
							intPlug.getValue( intPlugVal );
							int intDefault( shaderInfo.getArgIntDefault( i, 0 ) );
							if( intPlugVal == intDefault && !outputAllParameters )  // skip default
							{
								skipToken = true;
							}
							else  // build non default param
							{
								tokenPointerArray.rbegin()->set( paramName.asChar(), rInt );
								tokenPointerArray.rbegin()->setTokenFloat( 0, intPlugVal );//setTokenInt()
							}
						}//if( arraySize == -1 ) 
						else    // unknown type
						{
							skipToken = true;
							liquidMessage2(messageError,"[liqShader] error while building int param [%s] on [%s] : undefined array size [%d] ", paramName.asChar(), shaderNode.name().asChar(), arraySize );
						}
					}
					break;
				}
				//////////////////////////////////////////////////////////////////////////
				case SHADER_TYPE_BOOL:
					{
						MPlug boolPlug( shaderNode.findPlug( paramName, &status ) );
						if ( MS::kSuccess != status )
						{
							skipToken = true;
							liquidMessage2(messageError,"[liqShader] error while building bool param [%s] on [%s] ...", paramName.asChar(), shaderNode.name().asChar() );
						}
						else
						{
							if( arraySize == 0 )    // dynamic array
							{
								MIntArray indices;
								boolPlug.getExistingArrayAttributeIndices(indices);
								if( indices.length() == 0 )
								{
									skipToken = true;
								}
								else
								{
									int maxIndex = 0;
									for(unsigned int kk( 0 ); kk<indices.length(); kk++)
									{
										if( indices[kk]>maxIndex )
										{
											maxIndex = indices[kk];
										}
									}
									arraySize = maxIndex + 1;

									tokenPointerArray.rbegin()->set( paramName.asChar(), rBool, false, true, arraySize );
									for( unsigned int kk( 0 ); kk < (unsigned int)arraySize; kk++ )
									{
										bool existingIndex = false;
										for(unsigned int kkk( 0 ); kkk<indices.length(); kkk++)
										{
											if(kk==indices[kkk])
											{
												existingIndex = true;
												continue;
											}
										}
										if( existingIndex )  // get plug value
										{
											MPlug argNameElement = boolPlug.elementByLogicalIndex(kk);
											bool value = argNameElement.asBool();
											tokenPointerArray.rbegin()->setTokenFloat( kk, value );//setTokenBool()
										}
										else  // don't mind about value
										{
											tokenPointerArray.rbegin()->setTokenFloat( kk, 0 );//setTokenBool()
										}
									}
								}
							}//if( arraySize == 0 ) 
							else if( arraySize > 0 )    // static array
							{
								bool isArrayAttr( boolPlug.isArray( &status ) );
								if ( isArrayAttr )
								{
									MPlug plugObj;
									// check default
									int isDefault = 1;
									for( unsigned int kk( 0 ); kk < (unsigned int)arraySize; kk++ )
									{
										plugObj = boolPlug.elementByLogicalIndex( kk, &status );
										bool boolDefault = shaderInfo.getArgBoolDefault( i, kk );
										if( plugObj.asBool() != boolDefault )
										{
											isDefault = 0;
											continue;
										}
									}
									if( isDefault && !outputAllParameters ) // skip default
									{
										skipToken = true;
									}
									else  // build non default param
									{
										tokenPointerArray.rbegin()->set( paramName.asChar(), rBool, false, true, arraySize );
										for( unsigned int kk( 0 ); kk < (unsigned int)arraySize; kk++ )
										{
											plugObj = boolPlug.elementByLogicalIndex( kk, &status );
											if ( MS::kSuccess == status )
											{
												bool x;
												plugObj.getValue( x );
												tokenPointerArray.rbegin()->setTokenFloat( kk, x );//setTokenBool()
											}
										}
									}
								}
							}//if( arraySize > 0 )
							else if( arraySize == -1 )    // single value
							{
								bool boolPlugVal;
								boolPlug.getValue( boolPlugVal );
								bool boolDefault( shaderInfo.getArgBoolDefault( i, 0 ) );
								if( boolPlugVal == boolDefault && !outputAllParameters )  // skip default
								{
									skipToken = true;
								}
								else  // build non default param
								{
									tokenPointerArray.rbegin()->set( paramName.asChar(), rBool );
									tokenPointerArray.rbegin()->setTokenFloat( 0, boolPlugVal );//setTokenBool()
								}
							}//if( arraySize == -1 ) 
							else    // unknown type
							{
								skipToken = true;
								liquidMessage2(messageError,"[liqShader] error while building bool param [%s] on [%s] : undefined array size [%d] ", paramName.asChar(), shaderNode.name().asChar(), arraySize );
							}
						}
						break;
					}				
				//////////////////////////////////////////////////////////////////////////
				
				case SHADER_TYPE_COLOR:
				case SHADER_TYPE_POINT:
				case SHADER_TYPE_VECTOR:
				case SHADER_TYPE_NORMAL:
				{
					ParameterType parameterType;
					if( shaderParameterType==SHADER_TYPE_COLOR )
					{
						parameterType = rColor;
					}
					else if(shaderParameterType==SHADER_TYPE_POINT)
					{
						parameterType = rPoint;
					}
					else if(shaderParameterType==SHADER_TYPE_VECTOR)
					{
						parameterType = rVector;
					}
					else if(shaderParameterType==SHADER_TYPE_NORMAL)
					{
						parameterType = rNormal;
					}

					MPlug triplePlug( shaderNode.findPlug( paramName, true, &status ) );
					if( status != MS::kSuccess )
					{
						skipToken = true;
						liquidMessage2(messageError,"[liqShader] error while building float[3] param [%s] on [%s] ...", paramName.asChar(), shaderNode.name().asChar() );
					}
					else
					{
					if( arraySize==0 )    // dynamic array
					{
						MIntArray indices;
						triplePlug.getExistingArrayAttributeIndices(indices);
						if( indices.length() == 0 )
						{
							skipToken = true;
						}
						else
						{
							int maxIndex = 0;
							for(unsigned int kk( 0 ); kk<indices.length(); kk++)
							{
								if( indices[kk]>maxIndex )
								{
									maxIndex = indices[kk];
								}
							}
							arraySize = maxIndex + 1;

							tokenPointerArray.rbegin()->set( paramName.asChar(), parameterType, false, true, arraySize );
							for( unsigned int kk( 0 ); kk < (unsigned int)arraySize; kk++ )
							{
								bool existingIndex = false;
								for(unsigned int kkk( 0 ); kkk<indices.length(); kkk++)
								{
									if(kk==indices[kkk])
									{
										existingIndex = true;
										continue;
									}
								}
								if( existingIndex )  // get plug value
								{
									MPlug argNameElement = triplePlug.elementByLogicalIndex(kk);
									float x, y, z;
									argNameElement.child( 0 ).getValue( x );
									argNameElement.child( 1 ).getValue( y );
									argNameElement.child( 2 ).getValue( z );
									tokenPointerArray.rbegin()->setTokenFloat( kk, x, y, z );
								}
								else  // don't mind about value
								{
									tokenPointerArray.rbegin()->setTokenFloat( kk, 0, 0, 0 );
								}
							}
						}
					}//if( arraySize == 0 )
					else if ( arraySize > 0 )    // static array
					{
						// check default
						int isDefault = 1;
						for( unsigned int kk( 0 ); kk < (unsigned int)arraySize; kk++ )
						{
							MPlug argNameElement( triplePlug.elementByLogicalIndex( kk ) );
							float x, y, z;
							argNameElement.child( 0 ).getValue( x );
							argNameElement.child( 1 ).getValue( y );
							argNameElement.child( 2 ).getValue( z );
							float xDefault, yDefault, zDefault;
							xDefault = shaderInfo.getArgFloatDefault(i, (kk*3)+0);
							yDefault = shaderInfo.getArgFloatDefault(i, (kk*3)+1);
							zDefault = shaderInfo.getArgFloatDefault(i, (kk*3)+2);
							if( x!=xDefault || y!=yDefault || z!=zDefault )
							{
								isDefault = 0;
								continue;
							}
						}
						if( isDefault && !outputAllParameters ) // skip default
						{
							skipToken = true;
						}
						else  // build non default param
						{
							tokenPointerArray.rbegin()->set( paramName.asChar(), parameterType, false, true, arraySize );
							for( unsigned int kk( 0 ); kk < (unsigned int)arraySize; kk++ )
							{
								MPlug argNameElement( triplePlug.elementByLogicalIndex( kk ) );
								float x, y, z;
								argNameElement.child( 0 ).getValue( x );
								argNameElement.child( 1 ).getValue( y );
								argNameElement.child( 2 ).getValue( z );
								tokenPointerArray.rbegin()->setTokenFloat( kk, x, y, z );
							}
						}
					}//if ( arraySize > 0 )
					else if ( arraySize == -1 )     // single value
					{
						// check default
						float x, y, z;
						triplePlug.child( 0 ).getValue( x );
						triplePlug.child( 1 ).getValue( y );
						triplePlug.child( 2 ).getValue( z );
						float xDefault, yDefault, zDefault;
						xDefault = shaderInfo.getArgFloatDefault(i, 0);
						yDefault = shaderInfo.getArgFloatDefault(i, 1);
						zDefault = shaderInfo.getArgFloatDefault(i, 2);

						if( (x==xDefault && y==yDefault && z==zDefault) && !outputAllParameters ) // skip default
						{
							skipToken = true;
						}
						else  // build non default param
						{
							tokenPointerArray.rbegin()->set( paramName.asChar(), parameterType );
							tokenPointerArray.rbegin()->setTokenFloat( 0, x, y, z );
						}
					}//if ( arraySize == -1 )
					else    // unknown type//arraySize
					{
						skipToken = true;
						liquidMessage2(messageError,"[liqShader] error while building float[3] param [%s] on [%s] : undefined array size [%d] ", paramName.asChar(), shaderNode.name().asChar(), arraySize );
					}
					}//if( status
					break;
				}
				case SHADER_TYPE_MATRIX:
				{
					liquidMessage2(messageInfo,  "[liqShader]  [%s.%s] arraySize=[%d]", shaderNode.name().asChar(), paramName.asChar(), arraySize );
					MPlug matrixPlug( shaderNode.findPlug( paramName, &status ) );
					if ( MS::kSuccess != status )
					{
						skipToken = true;
						liquidMessage2(messageError,"[liqShader] error while building float[16] param [%s] on [%s] ...", paramName.asChar(), shaderNode.name().asChar() );
					}
					else
					{
						if( arraySize == 0 )    // dynamic array
						{
							MIntArray indices;
							matrixPlug.getExistingArrayAttributeIndices(indices);
							if( indices.length() == 0 )
							{
								skipToken = true;
							}
							else// indices.length() != 0 
							{
								int maxIndex = 0;
								for(unsigned int kk( 0 ); kk<indices.length(); kk++)
								{
									if( indices[kk]>maxIndex )
									{
										maxIndex = indices[kk];
									}
								}
								arraySize = maxIndex + 1;

								tokenPointerArray.rbegin()->set( paramName.asChar(), rMatrix, false, true, arraySize );
								for( unsigned int kk( 0 ); kk < (unsigned int)arraySize; kk++ )
								{
									bool existingIndex = false;
									for(unsigned int kkk( 0 ); kkk<indices.length(); kkk++)
									{
										if(kk==indices[kkk])
										{
											existingIndex = true;
											continue;
										}
									}
									if( existingIndex )  // get plug value
									{
										MPlug argNameElement = matrixPlug.elementByLogicalIndex(kk);
										MObject matrixObject = argNameElement.asMObject(MDGContext::fsNormal, &status);
										MFnMatrixData matrixData(matrixObject, &status);
										if(status!=MS::kSuccess)
										{
											skipToken = true;
											liquidMessage2(messageError,"[liqShader] error while initializing MFnMatrixData on param[?] [%s] on shader [%s] ...", paramName.asChar(), shaderNode.name().asChar() );
											continue;
										}
										else
										{
											MMatrix matrix = matrixData.matrix();
											float x1, y1, z1, w1;
											float x2, y2, z2, w2;
											float x3, y3, z3, w3;
											float x4, y4, z4, w4;
											x1 = matrix(0, 0);
											y1 = matrix(0, 1);
											z1 = matrix(0, 2);
											w1 = matrix(0, 3);
											x2 = matrix(1, 0);
											y2 = matrix(1, 1);
											z2 = matrix(1, 2);
											w2 = matrix(1, 3);
											x3 = matrix(2, 0);
											y3 = matrix(2, 1);
											z3 = matrix(2, 2);
											w3 = matrix(2, 3);
											x4 = matrix(3, 0);
											y4 = matrix(3, 1);
											z4 = matrix(3, 2);
											w4 = matrix(3, 3);
											tokenPointerArray.rbegin()->setTokenFloat( kk, x1, y1, z1, w1, x2, y2, z2, w2, x3, y3, z3, w3, x4, y4, z4, w4 );
										}
									}
									else  // don't mind about value
									{
										tokenPointerArray.rbegin()->setTokenFloat( kk, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 );
									}//if( existingIndex )else
								}
							}// indices.length() != 0 
						}
						else  if ( arraySize > 0 )    // static array
						{
							// check default
							int isDefault = 1;
							for( unsigned int kk( 0 ); kk < (unsigned int)arraySize; kk++ )
							{
								MPlug argNameElement( matrixPlug.elementByLogicalIndex( kk ) );
								MObject matrixObject = argNameElement.asMObject(MDGContext::fsNormal, &status);
								MFnMatrixData matrixData(matrixObject, &status);
								if(status!=MS::kSuccess)
								{
									skipToken = true;
									liquidMessage2(messageError,"[liqShader] error while initializing MFnMatrixData on param[] [%s] on shader [%s] ...", paramName.asChar(), shaderNode.name().asChar() );
								}
								else
								{
									MMatrix matrix = matrixData.matrix();
									MMatrix defaultMatrix;
									defaultMatrix(0, 0) = shaderInfo.getArgFloatDefault(i, (kk*16)+0);
									defaultMatrix(0, 1) = shaderInfo.getArgFloatDefault(i, (kk*16)+1);
									defaultMatrix(0, 2) = shaderInfo.getArgFloatDefault(i, (kk*16)+2);
									defaultMatrix(0, 3) = shaderInfo.getArgFloatDefault(i, (kk*16)+3);
									defaultMatrix(1, 0) = shaderInfo.getArgFloatDefault(i, (kk*16)+4);
									defaultMatrix(1, 1) = shaderInfo.getArgFloatDefault(i, (kk*16)+5);
									defaultMatrix(1, 2) = shaderInfo.getArgFloatDefault(i, (kk*16)+6);
									defaultMatrix(1, 3) = shaderInfo.getArgFloatDefault(i, (kk*16)+7);
									defaultMatrix(2, 0) = shaderInfo.getArgFloatDefault(i, (kk*16)+8);
									defaultMatrix(2, 1) = shaderInfo.getArgFloatDefault(i, (kk*16)+9);
									defaultMatrix(2, 2) = shaderInfo.getArgFloatDefault(i, (kk*16)+10);
									defaultMatrix(2, 3) = shaderInfo.getArgFloatDefault(i, (kk*16)+11);
									defaultMatrix(3, 0) = shaderInfo.getArgFloatDefault(i, (kk*16)+12);
									defaultMatrix(3, 1) = shaderInfo.getArgFloatDefault(i, (kk*16)+13);
									defaultMatrix(3, 2) = shaderInfo.getArgFloatDefault(i, (kk*16)+14);
									defaultMatrix(3, 3) = shaderInfo.getArgFloatDefault(i, (kk*16)+15);
									if( matrix != defaultMatrix )
									{
										isDefault = 0;
										continue;
									}
								}
							}
							if( isDefault && !outputAllParameters ) // skip default
							{
								skipToken = true;
							}
							else  // build non default param
							{
								tokenPointerArray.rbegin()->set( paramName.asChar(), rMatrix, false, true, arraySize );
								for( unsigned int kk( 0 ); kk < (unsigned int)arraySize; kk++ )
								{
									MPlug argNameElement( matrixPlug.elementByLogicalIndex( kk ) );
									MObject matrixObject = argNameElement.asMObject(MDGContext::fsNormal, &status);
									MFnMatrixData matrixData(matrixObject, &status);
									MMatrix matrix = matrixData.matrix();
									float x1, y1, z1, w1;
									float x2, y2, z2, w2;
									float x3, y3, z3, w3;
									float x4, y4, z4, w4;
									x1 = matrix(0, 0);
									y1 = matrix(0, 1);
									z1 = matrix(0, 2);
									w1 = matrix(0, 3);
									x2 = matrix(1, 0);
									y2 = matrix(1, 1);
									z2 = matrix(1, 2);
									w2 = matrix(1, 3);
									x3 = matrix(2, 0);
									y3 = matrix(2, 1);
									z3 = matrix(2, 2);
									w3 = matrix(2, 3);
									x4 = matrix(3, 0);
									y4 = matrix(3, 1);
									z4 = matrix(3, 2);
									w4 = matrix(3, 3);
									tokenPointerArray.rbegin()->setTokenFloat( kk, x1, y1, z1, w1, x2, y2, z2, w2, x3, y3, z3, w3, x4, y4, z4, w4 );
								}
							}	
						} 
						else if( arraySize == -1 )    // single value
						{
							// check default
							MObject matrixObject = matrixPlug.asMObject(MDGContext::fsNormal, &status);
							MFnMatrixData matrixData(matrixObject, &status);
							if(status!=MS::kSuccess)
							{
								skipToken = true;
								liquidMessage2(messageError,"[liqShader] error while initializing MFnMatrixData on param [%s] on shader [%s] ...", paramName.asChar(), shaderNode.name().asChar() );
							}
							else
							{
								MMatrix matrix = matrixData.matrix();
								MMatrix defaultMatrix;
								defaultMatrix(0, 0) = shaderInfo.getArgFloatDefault(i, 0);
								defaultMatrix(0, 1) = shaderInfo.getArgFloatDefault(i, 1);
								defaultMatrix(0, 2) = shaderInfo.getArgFloatDefault(i, 2);
								defaultMatrix(0, 3) = shaderInfo.getArgFloatDefault(i, 3);
								defaultMatrix(1, 0) = shaderInfo.getArgFloatDefault(i, 4);
								defaultMatrix(1, 1) = shaderInfo.getArgFloatDefault(i, 5);
								defaultMatrix(1, 2) = shaderInfo.getArgFloatDefault(i, 6);
								defaultMatrix(1, 3) = shaderInfo.getArgFloatDefault(i, 7);
								defaultMatrix(2, 0) = shaderInfo.getArgFloatDefault(i, 8);
								defaultMatrix(2, 1) = shaderInfo.getArgFloatDefault(i, 9);
								defaultMatrix(2, 2) = shaderInfo.getArgFloatDefault(i, 10);
								defaultMatrix(2, 3) = shaderInfo.getArgFloatDefault(i, 11);
								defaultMatrix(3, 0) = shaderInfo.getArgFloatDefault(i, 12);
								defaultMatrix(3, 1) = shaderInfo.getArgFloatDefault(i, 13);
								defaultMatrix(3, 2) = shaderInfo.getArgFloatDefault(i, 14);
								defaultMatrix(3, 3) = shaderInfo.getArgFloatDefault(i, 15);
								if( matrix == defaultMatrix && !outputAllParameters )  // skip default
								{
									skipToken = true;
								}
								else  // build non default param
								{
									float x1, y1, z1, w1;
									float x2, y2, z2, w2;
									float x3, y3, z3, w3;
									float x4, y4, z4, w4;
									x1 = matrix(0, 0);
									y1 = matrix(0, 1);
									z1 = matrix(0, 2);
									w1 = matrix(0, 3);
									x2 = matrix(1, 0);
									y2 = matrix(1, 1);
									z2 = matrix(1, 2);
									w2 = matrix(1, 3);
									x3 = matrix(2, 0);
									y3 = matrix(2, 1);
									z3 = matrix(2, 2);
									w3 = matrix(2, 3);
									x4 = matrix(3, 0);
									y4 = matrix(3, 1);
									z4 = matrix(3, 2);
									w4 = matrix(3, 3);
									tokenPointerArray.rbegin()->set( paramName.asChar(), rMatrix );
									//printf("SET MATRIX : \n %f %f %f %f \n %f %f %f %f \n %f %f %f %f \n %f %f %f %f \n", x1, y1, z1, w1, x2, y2, z2, w2, x3, y3, z3, w3, x4, y4, z4, w4);
									tokenPointerArray.rbegin()->setTokenFloat( 0, x1, y1, z1, w1, x2, y2, z2, w2, x3, y3, z3, w3, x4, y4, z4, w4 );
								}
							}
						}
						else    // unknown type//arraySize
						{
							skipToken = true;
							liquidMessage2(messageError,"[liqShader] error while building float[16] param [%s] on [%s] : undefined array size [%d] ", paramName.asChar(), shaderNode.name().asChar(), arraySize );
						}
				    }
					break;
				}
				case SHADER_TYPE_UNKNOWN :
				default:
					liquidMessage( "Unknown shader type", messageError );
					skipToken = true;
					break;
			}//switch ( shaderParameterType )
			}//else//shaderInfo.isOutputParameter(i) is false


			if( !skipToken )
			{
				// set token type
				switch ( shaderDetail )
				{
					case SHADER_DETAIL_UNIFORM:
					{
						tokenPointerArray.rbegin()->setDetailType( rUniform );
						break;
					}
					case SHADER_DETAIL_VARYING:
					{
						tokenPointerArray.rbegin()->setDetailType( rVarying);
						break;
					}
					case SHADER_DETAIL_UNKNOWN:
						tokenPointerArray.rbegin()->setDetailType( rUniform);
						break;
				}
				// create next token
				tokenPointerArray.push_back( liqTokenPointer() );
			}
			else
			{
				// skip parameter : parameter will not be written inside rib
				if( outputAllParameters )
				{
					liquidMessage2(messageError,"[liqShader] skipping shader parameter [%s] on [%s] (probably an empty dynamic array)", paramName.asChar(), shaderNode.name().asChar() );
				}
			}
		}//for
	}//else//success is true;
	shaderInfo.resetIt();
}


liqShader::~liqShader()
{
//	CM_TRACE_FUNC("liqShader::~liqShader()");
}


void liqShader::appendCoShader(MObject coshader, MPlug plug)
{
	CM_TRACE_FUNC("liqShader::appendCoShader("<<MFnDependencyNode(coshader).name().asChar()<<","<<plug.name().asChar()<<")");

	// test if it's really a co-shader
	int isLiquidShader = 0;
	MFn::Type objectType = coshader.apiType();
	if( objectType == MFn::kPluginDependNode )
	{
		MFnDependencyNode fnObject(coshader);
		MTypeId depNodeId = fnObject.typeId();
		//liquidMessage2(messageInfo, "liqSurfaceNodeId=[%d]  liqDisplacementNodeId=[%d]  liqVolumeNodeId=[%d]  liqCoShaderNodeId=[%d]\n", liqSurfaceNodeId, liqDisplacementNodeId, liqVolumeNodeId, liqCoShaderNodeId);
		if( depNodeId==liqSurfaceNodeId || 
		  depNodeId==liqDisplacementNodeId || 
		  depNodeId==liqVolumeNodeId || 
		  depNodeId==liqCoShaderNodeId ||
		  depNodeId==liqShaderNodeId 
		  )
		{
			isLiquidShader = 1;
		}else{
			liquidMessage2(messageError,"[%s]'s coshader is not a liquid shader", getName().c_str() );
		}
	}else{
		liquidMessage2(messageError,"[%s]'s coshader is not a MFn::kPluginDependNode", getName().c_str() );
	}
	if( isLiquidShader )
	{
		MFnDependencyNode fnObject(coshader);
		m_coShaderArray.push_back(coshader);
	}
	else
	{
		liquidMessage2(messageError, "[liqShader::appendCoShader] Error unsupported connection in plug [%s], abort co-shading for this plug", plug.name().asChar());
	}
}


// MStatus liqShader::liqShaderParseVectorAttr ( const MFnDependencyNode& shaderNode, const std::string& argName, ParameterType pType )
// {
// 	CM_TRACE_FUNC("liqShader::liqShaderParseVectorAttr("<<shaderNode.name().asChar()<<","<<argName<<","<<pType<<")");
// 
// 	MStatus status( MS::kSuccess );
// 
// 	MPlug triplePlug( shaderNode.findPlug( argName.c_str(), &status ) );
// 
// 	if ( MS::kSuccess == status )
// 	{
// 		float x, y, z;
// 		tokenPointerArray.rbegin()->set( argName.c_str(), pType );
// 		triplePlug.child( 0 ).getValue( x );
// 		triplePlug.child( 1 ).getValue( y );
// 		triplePlug.child( 2 ).getValue( z );
// 		tokenPointerArray.rbegin()->setTokenFloat( 0, x, y, z );
// 		//tokenPointerArray.push_back( liqTokenPointer() );
// 	}
//   return status;
// }

// philippe : multi attr support
// MStatus liqShader::liqShaderParseVectorArrayAttr ( const MFnDependencyNode& shaderNode, const std::string& argName, ParameterType pType, unsigned int arraySize )
// {
// 	CM_TRACE_FUNC("liqShader::liqShaderParseVectorArrayAttr("<<shaderNode.name().asChar()<<","<<argName<<","<<pType<<","<<arraySize<<")");
// 
//   MStatus status( MS::kSuccess );
// 
//   MPlug triplePlug( shaderNode.findPlug( argName.c_str(), true, &status ) );
// 
//   if ( MS::kSuccess == status ) {
//     tokenPointerArray.rbegin()->set( argName, pType, false, true, arraySize );
//     for( unsigned int kk( 0 ); kk < arraySize; kk++ ) {
//       MPlug argNameElement( triplePlug.elementByLogicalIndex( kk ) );
// 
//       if ( MS::kSuccess == status ) {
//         float x, y, z;
//         argNameElement.child( 0 ).getValue( x );
//         argNameElement.child( 1 ).getValue( y );
//         argNameElement.child( 2 ).getValue( z );
//         tokenPointerArray.rbegin()->setTokenFloat( kk, x, y, z );
//       }
//     }
//     //tokenPointerArray.push_back( liqTokenPointer() );
//   }
// 
//   return status;
// }


void liqShader::write(/*, */)
{
	CM_TRACE_FUNC("liqShader::write()");

	//bool shortShaderNames  = ;
	//unsigned int indentLevel = 0;

	MFnDependencyNode node(m_mObject);
	if( hasErrors )
	{
		liquidMessage2(messageError,"[liqShader::write] Erros occured while initializing shader [%s], won't export shader", node.name().asChar());
		return;
	}

	liquid::RendererMgr::getInstancePtr()->getRenderer()->shader_UserDefinedShader(this);

}

// 
// void liqShader::outputIndentation(unsigned int indentLevel)
// {
// 	for(unsigned int i=0; i<indentLevel; ++i)
// 	{
// 		RiArchiveRecord(RI_VERBATIM, "\t");
// 	}
// }

//MStatus liqShader::liqShaderParseMatrixAttr ( const MFnDependencyNode& shaderNode, const std::string& argName, ParameterType pType )
// {
// 	CM_TRACE_FUNC("liqShader::liqShaderParseMatrixAttr("<<shaderNode.name().asChar()<<","<<argName<<","<<pType<<")");
// 	MStatus status( MS::kSuccess );
// 
// 	MPlug matrixPlug( shaderNode.findPlug( argName.c_str(), &status ) );
// 
// 	if ( MS::kSuccess == status )
// 	{
//  		tokenPointerArray.rbegin()->set( argName.c_str(), pType );
// 		
// 		RtFloat matrixBuff[16];
// 
// 		//get matrix value
// 		MObject oMatrix;
// 		matrixPlug.getValue(oMatrix);
// 		MFnMatrixData fndMatrix(oMatrix, &status);
// 		LIQCHECKSTATUS(status,"liqShader::liqShaderParseMatrixAttr(...) fndMatrix");
// 		MMatrix matrix(fndMatrix.matrix(&status));
// 		LIQCHECKSTATUS(status,"liqShader::liqShaderParseMatrixAttr(...) mmatrix");
// 		RtFloat mmatrixBuff[4][4];
// 		matrix.get(mmatrixBuff);
// 		matrixBuff[0 ] = mmatrixBuff[0][0];  matrixBuff[1 ] = mmatrixBuff[0][1];  matrixBuff[2 ] = mmatrixBuff[0][2];  matrixBuff[3 ] = mmatrixBuff[0][3];
// 		matrixBuff[4 ] = mmatrixBuff[1][0];  matrixBuff[5 ] = mmatrixBuff[1][1];  matrixBuff[6 ] = mmatrixBuff[1][2];  matrixBuff[7 ] = mmatrixBuff[1][3];
// 		matrixBuff[8 ] = mmatrixBuff[2][0];  matrixBuff[9 ] = mmatrixBuff[2][1];  matrixBuff[10] = mmatrixBuff[2][2];  matrixBuff[11] = mmatrixBuff[2][3];
// 		matrixBuff[12] = mmatrixBuff[3][0];  matrixBuff[13] = mmatrixBuff[3][1];  matrixBuff[14] = mmatrixBuff[3][2];  matrixBuff[15] = mmatrixBuff[3][3];
//       
// 		//you can alose get the matrix value by mel
// // 		MDoubleArray matrix;
// // 		MString cmd;
// // 		cmd += "getAttr "+shaderNode.name()+"."+(MString)(argName.c_str());
// // 		MGlobal::executeCommand(cmd, matrix);
// // 		liquidMessage2(messageInfo, "m[][]={%f, %f, %f, %f, ...}",matrix[0] , matrix[1], matrix[2], matrix[3]);
// //		matrix.get(matrixBuff);
// 
// 		tokenPointerArray.rbegin()->setTokenFloats( matrixBuff );			
// 		//tokenPointerArray.push_back( liqTokenPointer() );
// 	}
// 	return status;
// }
//
void liqShader::buildJobs()
{
	CM_TRACE_FUNC("liqShader::buildJobs()");

	scanExpressions(*this);
}
// check shaders to see if "string" parameters are expression
// replace expression with calculated values
void liqShader::scanExpressions( liqShader & currentShader )
{
	CM_TRACE_FUNC("liqShader::scanExpressions("<<currentShader.name<<")");

	for ( std::vector< liqTokenPointer >::iterator it( currentShader.tokenPointerArray.begin() ); 
		it != currentShader.tokenPointerArray.end(); 
		it++ ) 
	{
		if( it->getParameterType() == rString )
			processExpression( &( *it ) );
	}
}
//
const std::string removeRedundentExtsion(std::string const& texturepath)//texturepath=e:/a.b\c.d.bmp.tex
{
	CM_TRACE_FUNC("removeRedundentExtsion("<<texturepath<<")");

	if(texturepath.empty())
		return "";

	std::string texpath(texturepath);
	std::replace(texpath.begin(), texpath.end(), '\\', '/');//texpath=e:/a.b/c.d.bmp.tex

	std::string texname(texpath.substr(texpath.find_last_of('/')+1));//texname=c.d.bmp.tex

	std::string basepath(texpath.substr(0,texpath.find_last_of('/')));//basepath=e:/a.b

	std::size_t i_last_dot = texname.find_last_of('.');
	if( i_last_dot == std::string::npos ){
		liquidMessage2(messageError,"removeRedundentExtsion():[%s] has no extention!", texturepath.c_str());
		return texturepath;
	}

	std::string const texname2(texname.substr(0, i_last_dot));//texname2=c.d.bmp

	std::size_t i_last2_dot = texname2.find_last_of('.');
	if( i_last2_dot == std::string::npos ){
		liquidMessage2(messageInfo, "[%s] is a texture, no need to convert.", texturepath.c_str());
		return texturepath;
	}else{
		std::string imgext(texname2.substr(i_last2_dot+1));//imgext=bmp
		std::transform(imgext.begin(),imgext.end(),imgext.begin(),tolower);
		if( imgext!="bmp" && imgext!="jpg" && imgext!="png" ){
			liquidMessage2(messageWarning,"[%s]'type [%s] maybe not supported.", texname2.c_str(), imgext.c_str());
		}
		return basepath+"/"+texname2;//return "e:/a.b/c.d.bmp"
	}
}
//
void liqShader::processExpression( liqTokenPointer *token, const liqRibLightData *light )
{
	CM_TRACE_FUNC("liqShader::processExpression("<<token->getTokenName()<<",*light)");

	if( token != NULL ) 
	{
		std::string strValue( token->getTokenString() );
		strValue = removeRedundentExtsion(strValue);
		LIQDEBUGPRINTF( "-> Expression:(%s)\n", token->getTokenName().c_str() );
		// NOTE:
		// For convenience, we use the image path instead of [MakeTexture ...],
		// e.g. we use c:/test.bmp for  [MakeTexture c:/test.bmp]
		if( !IS_EXPRESSION(strValue.c_str()) )
		{
			LIQDEBUGPRINTF("texturePathPoint=%s, strValue=%s\n", strValue.c_str(), strValue.c_str());
			strValue = "[MakeTexture "+strValue+"]";
		}

		liqExpression expr( strValue );
		if( expr.type != exp_None && expr.isValid ) 
		{ // we've got expression here
			expr.CalcValue(); // calculate value;
			switch ( expr.type ) 
			{
			case exp_CoordSys:
				LIQDEBUGPRINTF( "-> CoordSys Expression: %s\n", expr.GetValue().asChar() );
				token->setTokenString( 0, expr.GetValue().asChar() );
				break;

			case exp_MakeTexture:
				{
					token->setTokenString( 0, expr.GetValue().asChar() );
					if( !expr.destExists || !expr.destIsNewer ) {
						LIQDEBUGPRINTF( "-> Making Texture: %s\n", liqglo.liquidRenderer.textureMaker.asChar() );
						LIQDEBUGPRINTF( "-> MakeTexture Command: %s\n", expr.GetCmd().asChar() );

						structJob thisJob;
						thisJob.pass = rpMakeTexture;
						thisJob.renderName = liqglo.liquidRenderer.textureMaker;
						thisJob.ribFileName = expr.GetCmd();
						thisJob.imageName = expr.GetValue(); // destination file name

						//skip
						std::size_t loc = strValue.find_last_of('.');
						if( loc == std::string::npos )
							liquidMessage2(messageError,"liqShader::processExpression():[%s] has no extention!", strValue.c_str());
						std::string extention;
						if( *(strValue.rbegin())==']' ){//if the strValue end with ']', extention should exclude the ']'
							extention = strValue.substr(loc+1, strValue.size()-loc-2);
							printf("extention = %s\n", extention.c_str() );
						}else{
							extention = strValue.substr(loc+1);
						}
						thisJob.skip = (getTextureExt().asChar()==extention);

						std::vector<structJob>::iterator iter = liqRibTranslator::getInstancePtr()->txtList.begin();
						while ( iter != liqRibTranslator::getInstancePtr()->txtList.end() ) {
							if( iter->imageName == thisJob.imageName )
								break; // already have this job
							++iter;
						}
						liqRibTranslator::getInstancePtr()->txtList.push_back( thisJob );

					}
				}
				break;

			case exp_ReflectMap:
				LIQDEBUGPRINTF( "-> ReflectMap Expression: %s\n", expr.GetValue().asChar());
				token->setTokenString( 0, expr.GetValue().asChar() );
				break;

			case exp_Shadow:
			case exp_PointShadow:
				{
					MString shadowName = liqglo.liqglo_textureDir + light->autoShadowName();
					token->setTokenString( 0, shadowName.asChar() );
				}
				break;

			case exp_EnvMap:
			case exp_CubeEnvMap:
			case exp_None:
			default:
				break;
			}
		}
	}//if( token != NULL ) 
}

std::string liqShader::getName() const
{
	CM_TRACE_FUNC("liqShader::getName()");
	return name;
}

std::string liqShader::getShaderFileName() const
{
	CM_TRACE_FUNC("liqShader::getShaderFileName()");

	return liqglo.liqglo_shortShaderNames ? 
		basename( file.c_str() ) : file.c_str();

}
void liqShader::getValidConnection( MStringArray& connections ) const
{
	CM_TRACE_FUNC("liqShader::getValidConnection(...)");
	IfMErrorWarn(MGlobal::executeCommand( ("getAttr "+MString(getName().c_str())+".rmanParams"), connections));
}