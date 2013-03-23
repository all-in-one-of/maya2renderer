#include "shaderValidConnection.h"
#include <liqlog.h>
#include "../common/mayacheck.h"
#include <liqShader.h>
#include <liqShaderFactory.h>

namespace liquidmaya
{
	ShaderConnectionMap::ShaderConnectionMap()
	{

	}
	ShaderConnectionMap::~ShaderConnectionMap()
	{

	}
	void ShaderConnectionMap::begin(const MString &shadertype)
	{
		currentShaderType = shadertype;
		validConnection.clear();
	}
	void ShaderConnectionMap::append(const MString &plug)
	{
		validConnection.append(plug);
	}
	void ShaderConnectionMap::end()
	{
		validConnectionMap.insert(std::make_pair(currentShaderType.asChar(), validConnection));	
	}
	bool ShaderConnectionMap::hasShaderType(const char* shadertype)const
	{
		return (validConnectionMap.find(shadertype) != validConnectionMap.end());
	}
	void ShaderConnectionMap::getValidConnection(const char* nodename, MStringArray& connections) const
	{
		MString shadertype;
		IfMErrorWarn(MGlobal::executeCommand( ("nodeType \""+MString(nodename)+"\""), shadertype));

		if( hasShaderType(shadertype.asChar()) ){
			if(strcmp("liquidShader",shadertype.asChar())==0)
			{
				liqShader& liqshader = liqShaderFactory::instance().getShader( nodename );
				liqshader.getValidConnection(connections);
			}else{
				connections = validConnectionMap.find(shadertype.asChar())->second;
			}
		}else{
			liquidMessage2(messageError, "shader type \"%s\" is not supported.", shadertype.asChar());
			assert(0&&"shader type is not supported.");
			connections = validConnectionMap.find("null")->second;
		}
	}
	//////////////////////////////////////////////////////////////////////////
ShaderValidConnection::ShaderValidConnection()
{
	setValidConnection();
}
//
ShaderValidConnection::~ShaderValidConnection()
{

}
//
void ShaderValidConnection::setValidConnection()
{
	MStringArray validConnection;
	
	//null (dummy shader)
	validConnectionMap.begin("null");
	validConnectionMap.append("");
	validConnectionMap.end();	

	// MATERIAL -----------------------------------
	/// surface ///
	//anisotropic

	//hairTubeShader

	//lambert
	validConnectionMap.begin("lambert");
	validConnectionMap.append("color");
	validConnectionMap.append("transparency");
	validConnectionMap.append("ambientColor");
	validConnectionMap.append("incandescence");
	validConnectionMap.append("normalCamera");
	validConnectionMap.append("diffuse");
	validConnectionMap.append("translucence");
	validConnectionMap.append("translucenceDepth");
	validConnectionMap.append("translucenceFocus");
	validConnectionMap.append("outColor");
	validConnectionMap.append("outTransparency");
	validConnectionMap.end();

	//layered shader

	//blinn
	validConnectionMap.begin("blinn");
	validConnectionMap.append("color");
	validConnectionMap.append("transparency");
	validConnectionMap.append("ambientColor");
	validConnectionMap.append("incandescence");
	validConnectionMap.append("normalCamera");
	validConnectionMap.append("diffuse");
	validConnectionMap.append("translucence");
	validConnectionMap.append("translucenceDepth");
	validConnectionMap.append("translucenceFocus");
	validConnectionMap.append("eccentricity");
	validConnectionMap.append("specularRollOff");
	validConnectionMap.append("specularColor");
	validConnectionMap.append("reflectivity");
	validConnectionMap.append("reflectedColor");
	validConnectionMap.append("outColor");
	validConnectionMap.append("outTransparency");
	validConnectionMap.end();

	//oceanShader 

	//phong
	validConnectionMap.begin("phong");
	validConnectionMap.append("colorR");
	validConnectionMap.append("colorG");
	validConnectionMap.append("colorB");
	validConnectionMap.append("color");
	validConnectionMap.append("transparency");
	validConnectionMap.append("ambientColor");
	validConnectionMap.append("incandescence");
	validConnectionMap.append("normalCamera");
	validConnectionMap.append("diffuse");
	validConnectionMap.append("translucence");
	validConnectionMap.append("translucenceDepth");
	validConnectionMap.append("translucenceFocus");
	validConnectionMap.append("cosinePower");
	validConnectionMap.append("specularColor");
	validConnectionMap.append("reflectivity");
	validConnectionMap.append("reflectedColor");
	validConnectionMap.append("matteOpacityMode");
	validConnectionMap.append("matteOpacity");
	validConnectionMap.append("reflectionLimit");
	validConnectionMap.append("outColor");
	validConnectionMap.append("outTransparency");
	validConnectionMap.end();

	//phongE

	//rampShader

	//shadingMap

	//surfaceShader

	//useBackground

	/// Volumetric ///
	//envFog

	//fluidShape

	//lightFog

	//particleCloud

	//volumeFog

	//volumeShader

	/// DISPLACEMENT ///
	//displacement

	/// 2D Texture (normal)///
	//bulge

	//checker
	validConnectionMap.begin("checker");
	validConnectionMap.append("alphaGain");
	validConnectionMap.append("alphaIsLuminance");
	validConnectionMap.append("alphaOffset");
	validConnectionMap.append("color1");//color1
	validConnectionMap.append("color1R");
	validConnectionMap.append("color1G");
	validConnectionMap.append("color1B");
	validConnectionMap.append("color2");//color2
	validConnectionMap.append("color2R");
	validConnectionMap.append("color2G");
	validConnectionMap.append("color2B");
	validConnectionMap.append("colorGain");
	validConnectionMap.append("colorOffset");
	validConnectionMap.append("contrast");
	validConnectionMap.append("defaultColor");
	validConnectionMap.append("filter");
	validConnectionMap.append("filterOffset");
	validConnectionMap.append("invert");
	validConnectionMap.append("uvCoord");//uvCoord
	validConnectionMap.append("uCoord");
	validConnectionMap.append("vCoord");
	validConnectionMap.append("outAlpha");
	validConnectionMap.append("outColor");//outColor
	validConnectionMap.append("outColorR");
	validConnectionMap.append("outColorG");
	validConnectionMap.append("outColorB");
	validConnectionMap.end();

	//cloth

	// file
	validConnectionMap.begin("file");
	validConnectionMap.append("alphaGain");
	validConnectionMap.append("alphaIsLuminance");
	validConnectionMap.append("alphaOffset");
	validConnectionMap.append("colorGain");
	validConnectionMap.append("colorOffset");
	validConnectionMap.append("defaultColor");
	validConnectionMap.append("fileTextureName");
	validConnectionMap.append("filterType");
	validConnectionMap.append("filter");
	validConnectionMap.append("filterOffset");
	validConnectionMap.append("invert");
	validConnectionMap.append("uvCoord");
	validConnectionMap.append("fileHasAlpha");
	validConnectionMap.append("outAlpha");
	validConnectionMap.append("outColor");
	validConnectionMap.append("outTransparency");
	validConnectionMap.end();

	//fluidTexture2D

	//fractal

	//grid

	//mountain

	//movie

	//noise

	//ocean

	//psdFileTex

	//ramp

	//water

	/// 3D Textures ///
	//brownian

	//cloud

	//crater

	//fluidTexture3D

	//granite

	//leather

	//marble

	//rock

	//snow

	//solidFractal

	//stucco

	//volumeNoise

	//wood

	/// Env Textures ///
	//envBall
	validConnectionMap.begin("envBall");
	validConnectionMap.append("normalCamera");
	validConnectionMap.append("rayDirection");
	validConnectionMap.append("filterSize");
	validConnectionMap.append("uvFilterSize");
	validConnectionMap.append("image");
	validConnectionMap.append("uvCoord");
	validConnectionMap.append("outAlpha");
	validConnectionMap.append("outColor");
	validConnectionMap.append("pointCamera");
	validConnectionMap.append("inclination");
	validConnectionMap.append("elevation");
	validConnectionMap.append("skyRadius");
	validConnectionMap.append("bottom");
	validConnectionMap.append("top");
	validConnectionMap.append("left");
	validConnectionMap.append("right");
	validConnectionMap.append("front");
	validConnectionMap.append("back");
	validConnectionMap.append("reflect");
	validConnectionMap.append("eyeSpace");
	validConnectionMap.append("infoBits");
	validConnectionMap.end();
	//envChrome

	//envCube

	//envSky 

	//envSphere

	/// other textures ///
	//layeredTexture

	/// Lights ///
	//ambientLight

	//areaLight

	//directionalLight

	//pointLight

	//spotLight

	//volumeLight

	/// General utilities ///
	//arrayMapper

	//bump2d
	validConnectionMap.begin("bump2d");
	validConnectionMap.append("bumpValue");
	validConnectionMap.append("bumpDepth");
	validConnectionMap.append("bumpInterp");
	validConnectionMap.append("bumpFilter");
	validConnectionMap.append("bumpFilterOffset");
	validConnectionMap.append("normalCamera");
	validConnectionMap.append("bumpNormal");
	validConnectionMap.append("outNormal");
	validConnectionMap.end();

	//bump3d
	validConnectionMap.begin("bump3d");
	validConnectionMap.append("bumpValue");
	validConnectionMap.append("bumpDepth");
	validConnectionMap.append("bumpFilter");
	validConnectionMap.append("bumpFilterOffset");
	validConnectionMap.append("normalCamera");
	validConnectionMap.append("outNormal");
	validConnectionMap.end();

	//condition

	//distanceBetween

	//heightField

	//lightInfo

	//multiplyDivide

	//place2dTexture
	validConnectionMap.begin("place2dTexture");
	validConnectionMap.append("uvCoord");
	validConnectionMap.append("coverageU");
	validConnectionMap.append("coverageV");
	validConnectionMap.append("coverage");
	validConnectionMap.append("mirrorU");
	validConnectionMap.append("mirrorV");
	validConnectionMap.append("noiseU");
	validConnectionMap.append("noiseV");
	validConnectionMap.append("noiseUV");
	validConnectionMap.append("offsetU");
	validConnectionMap.append("offsetV");
	validConnectionMap.append("offset");
	validConnectionMap.append("repeatU");
	validConnectionMap.append("repeatV");
	validConnectionMap.append("repeatUV");
	validConnectionMap.append("rotateFrame");
	validConnectionMap.append("rotateUV");
	validConnectionMap.append("stagger");
	validConnectionMap.append("translateFrameU");
	validConnectionMap.append("translateFrameV");
	validConnectionMap.append("translateFrame");
	validConnectionMap.append("wrapU");
	validConnectionMap.append("wrapV");
	validConnectionMap.append("outUV");
	validConnectionMap.end();	

	//place3dTexture

	//plusMinusAverage

	//projection

	//reverse

	//samplerInfo

	//setRange

	//stencil

	//studioClearCoat

	//uvChooser

	//vectorProduct

	/// color utilities ///
	//blendColors

	//clamp

	//contrast

	//gammaCorrect

	//hsvToRgb

	//luminance

	//remapColor

	//remapHsv

	//remapValue

	//rgbToHsv

	//smear

	//surfaceLuminance

	/// switch utilities ///
	//doubleShadingSwitch

	//quadShadingSwitch

	//singleShadingSwitch

	//tripleShadingSwitch

	/// particle utilities ///
	//particleSamplerInfo

	/// image planes ///
	//imagePlane

	/// glow ///
	//opticalFX

	setValidConnection_mi();

	///  liquidShader  ///
	validConnectionMap.begin("liquidShader");
	validConnectionMap.append("");
	validConnectionMap.end();	


}
//
bool ShaderValidConnection::hasShaderType(const char* shadertype)const
{
	return validConnectionMap.hasShaderType(shadertype);
}
//
void ShaderValidConnection::getValidConnection(const char* nodename, MStringArray& connections) const
{
	return validConnectionMap.getValidConnection(nodename, connections);
}

}//namespace liquidmaya