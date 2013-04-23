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
	validConnectionMap.append("colorR");
	validConnectionMap.append("colorG");
	validConnectionMap.append("colorB");
	validConnectionMap.append("color");
	validConnectionMap.append("transparencyR");
	validConnectionMap.append("transparencyG");
	validConnectionMap.append("transparencyB");
	validConnectionMap.append("transparency");
	validConnectionMap.append("ambientColorR");
	validConnectionMap.append("ambientColorG");
	validConnectionMap.append("ambientColorB");
	validConnectionMap.append("ambientColor");
	validConnectionMap.append("incandescenceR");
	validConnectionMap.append("incandescenceG");
	validConnectionMap.append("incandescenceB");
	validConnectionMap.append("incandescence");
	validConnectionMap.append("normalCameraX");
	validConnectionMap.append("normalCameraY");
	validConnectionMap.append("normalCameraZ");
	validConnectionMap.append("normalCamera");
	validConnectionMap.append("diffuse");
	validConnectionMap.append("translucence");
	validConnectionMap.append("translucenceDepth");
	validConnectionMap.append("translucenceFocus");
	validConnectionMap.append("outColorR");
	validConnectionMap.append("outColorG");
	validConnectionMap.append("outColorB");
	validConnectionMap.append("outColor");
	validConnectionMap.append("outTransparencyR");
	validConnectionMap.append("outTransparencyG");
	validConnectionMap.append("outTransparencyB");
	validConnectionMap.append("outTransparency");
	validConnectionMap.end();

	//layered shader

	//blinn
	validConnectionMap.begin("blinn");
	validConnectionMap.append("ambientColorR");
	validConnectionMap.append("ambientColorG");
	validConnectionMap.append("ambientColorB");
	validConnectionMap.append("ambientColor");
	validConnectionMap.append("colorR");
	validConnectionMap.append("colorG");
	validConnectionMap.append("colorB");
	validConnectionMap.append("color");
	validConnectionMap.append("diffuse");
	validConnectionMap.append("eccentricity");
	validConnectionMap.append("incandescenceR");
	validConnectionMap.append("incandescenceG");
	validConnectionMap.append("incandescenceB");
	validConnectionMap.append("incandescence");
	validConnectionMap.append("matteOpacityMode");
	validConnectionMap.append("matteOpacity");
	validConnectionMap.append("specularColorR");
	validConnectionMap.append("specularColorG");
	validConnectionMap.append("specularColorB");
	validConnectionMap.append("specularColor");
	validConnectionMap.append("specularRollOff");
	validConnectionMap.append("reflectivity");
	validConnectionMap.append("reflectedColorR");
	validConnectionMap.append("reflectedColorG");
	validConnectionMap.append("reflectedColorB");
	validConnectionMap.append("reflectedColor");
	/* Refraction. */
	validConnectionMap.append("refractions");
	validConnectionMap.append("refractiveIndex");
	validConnectionMap.append("refractionLimit");
	validConnectionMap.append("lightAbsorbance");
	validConnectionMap.append("shadowAttenuation");
	/* Reflection. */
	validConnectionMap.append("reflectionLimit");

	validConnectionMap.append("translucence");
	validConnectionMap.append("translucenceDepth");
	validConnectionMap.append("translucenceFocus");
	validConnectionMap.append("transparencyR");
	validConnectionMap.append("transparencyG");
	validConnectionMap.append("transparencyB");
	validConnectionMap.append("transparency");
	validConnectionMap.append("normalCameraX");
	validConnectionMap.append("normalCameraY");
	validConnectionMap.append("normalCameraZ");
	validConnectionMap.append("normalCamera");
	// Outputs:
	validConnectionMap.append("outColorR");
	validConnectionMap.append("outColorG");
	validConnectionMap.append("outColorB");
	validConnectionMap.append("outColor");
	validConnectionMap.append("outTransparencyR");
	validConnectionMap.append("outTransparencyG");
	validConnectionMap.append("outTransparencyB");
	validConnectionMap.append("outTransparency");
	validConnectionMap.end();

	//oceanShader 

	//phong
	validConnectionMap.begin("phong");
	validConnectionMap.append("colorR");
	validConnectionMap.append("colorG");
	validConnectionMap.append("colorB");
	validConnectionMap.append("color");
	validConnectionMap.append("transparencyR");
	validConnectionMap.append("transparencyG");
	validConnectionMap.append("transparencyB");
	validConnectionMap.append("transparency");
	validConnectionMap.append("ambientColorR");
	validConnectionMap.append("ambientColorG");
	validConnectionMap.append("ambientColorB");
	validConnectionMap.append("ambientColor");
	validConnectionMap.append("incandescenceR");
	validConnectionMap.append("incandescenceG");
	validConnectionMap.append("incandescenceB");
	validConnectionMap.append("incandescence");
	validConnectionMap.append("normalCameraX");
	validConnectionMap.append("normalCameraY");
	validConnectionMap.append("normalCameraZ");
	validConnectionMap.append("normalCamera");
	validConnectionMap.append("diffuse");
	validConnectionMap.append("translucence");
	validConnectionMap.append("translucenceDepth");
	validConnectionMap.append("translucenceFocus");
	validConnectionMap.append("cosinePower");
	validConnectionMap.append("specularColorR");
	validConnectionMap.append("specularColorG");
	validConnectionMap.append("specularColorB");
	validConnectionMap.append("specularColor");
	validConnectionMap.append("reflectivity");
	validConnectionMap.append("reflectedColorR");
	validConnectionMap.append("reflectedColorG");
	validConnectionMap.append("reflectedColorB");
	validConnectionMap.append("reflectedColor");
	validConnectionMap.append("matteOpacityMode");
	validConnectionMap.append("matteOpacity");
	validConnectionMap.append("reflectionLimit");
	validConnectionMap.append("outColorR");
	validConnectionMap.append("outColorG");
	validConnectionMap.append("outColorB");
	validConnectionMap.append("outColor");
	validConnectionMap.append("outTransparencyR");
	validConnectionMap.append("outTransparencyG");
	validConnectionMap.append("outTransparencyB");
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
	validConnectionMap.append("color1R");
	validConnectionMap.append("color1G");
	validConnectionMap.append("color1B");
	validConnectionMap.append("color1");//color1
	validConnectionMap.append("color2R");
	validConnectionMap.append("color2G");
	validConnectionMap.append("color2B");
	validConnectionMap.append("color2");//color2
	validConnectionMap.append("colorGainR");
	validConnectionMap.append("colorGainG");
	validConnectionMap.append("colorGainB");
	validConnectionMap.append("colorGain");
	validConnectionMap.append("colorOffsetR");
	validConnectionMap.append("colorOffsetG");
	validConnectionMap.append("colorOffsetB");
	validConnectionMap.append("colorOffset");
	validConnectionMap.append("contrast");
	validConnectionMap.append("defaultColorR");
	validConnectionMap.append("defaultColorG");
	validConnectionMap.append("defaultColorB");
	validConnectionMap.append("defaultColor");
	validConnectionMap.append("filter");
	validConnectionMap.append("filterOffset");
	validConnectionMap.append("invert");
	validConnectionMap.append("uCoord");
	validConnectionMap.append("vCoord");
	validConnectionMap.append("uvCoord");//uvCoord
	validConnectionMap.append("outAlpha");
	validConnectionMap.append("outColorR");
	validConnectionMap.append("outColorG");
	validConnectionMap.append("outColorB");
	validConnectionMap.append("outColor");//outColor
	validConnectionMap.end();

	//cloth

	// file
	validConnectionMap.begin("file");
	validConnectionMap.append("alphaGain");
	validConnectionMap.append("alphaIsLuminance");
	validConnectionMap.append("alphaOffset");
	validConnectionMap.append("colorGainR");
	validConnectionMap.append("colorGainG");
	validConnectionMap.append("colorGainB");
	validConnectionMap.append("colorGain");
	validConnectionMap.append("colorOffsetR");
	validConnectionMap.append("colorOffsetG");
	validConnectionMap.append("colorOffsetB");
	validConnectionMap.append("colorOffset");
	validConnectionMap.append("defaultColorR");
	validConnectionMap.append("defaultColorG");
	validConnectionMap.append("defaultColorB");
	validConnectionMap.append("defaultColor");
	validConnectionMap.append("fileTextureName");
	validConnectionMap.append("filterType");
	validConnectionMap.append("filter");
	validConnectionMap.append("filterOffset");
	validConnectionMap.append("invert");
	validConnectionMap.append("uCoord");
	validConnectionMap.append("vCoord");
	validConnectionMap.append("uvCoord");
	validConnectionMap.append("fileHasAlpha");
	validConnectionMap.append("outAlpha");
	validConnectionMap.append("outColorR");
	validConnectionMap.append("outColorG");
	validConnectionMap.append("outColorB");
	validConnectionMap.append("outColor");
	validConnectionMap.append("outTransparencyR");
	validConnectionMap.append("outTransparencyG");
	validConnectionMap.append("outTransparencyB");
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
	validConnectionMap.append("normalCameraX");
	validConnectionMap.append("normalCameraY");
	validConnectionMap.append("normalCameraZ");
	validConnectionMap.append("normalCamera");
	validConnectionMap.append("rayDirectionX");
	validConnectionMap.append("rayDirectionY");
	validConnectionMap.append("rayDirectionZ");
	validConnectionMap.append("rayDirection");
	validConnectionMap.append("filterSizeX");
	validConnectionMap.append("filterSizeY");
	validConnectionMap.append("filterSizeZ");
	validConnectionMap.append("filterSize");
	validConnectionMap.append("uvFilterSizeX");
	validConnectionMap.append("uvFilterSizeY");
	validConnectionMap.append("uvFilterSize");
	validConnectionMap.append("imageR");
	validConnectionMap.append("imageG");
	validConnectionMap.append("imageB");
	validConnectionMap.append("image");
	validConnectionMap.append("uCoord");
	validConnectionMap.append("vCoord");
	validConnectionMap.append("uvCoord");
	validConnectionMap.append("pointCameraX");
	validConnectionMap.append("pointCameraY");
	validConnectionMap.append("pointCameraZ");
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
	validConnectionMap.append("outAlpha");
	validConnectionMap.append("outColorR");
	validConnectionMap.append("outColorG");
	validConnectionMap.append("outColorB");
	validConnectionMap.append("outColor");
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
	validConnectionMap.append("normalCameraX");
	validConnectionMap.append("normalCameraY");
	validConnectionMap.append("normalCameraZ");
	validConnectionMap.append("normalCamera");
	validConnectionMap.append("bumpNormal");
	validConnectionMap.append("outNormalX");
	validConnectionMap.append("outNormalY");
	validConnectionMap.append("outNormalZ");
	validConnectionMap.append("outNormal");
	validConnectionMap.end();

	//bump3d
	validConnectionMap.begin("bump3d");
	validConnectionMap.append("bumpValue");
	validConnectionMap.append("bumpDepth");
	validConnectionMap.append("bumpFilter");
	validConnectionMap.append("bumpFilterOffset");
	validConnectionMap.append("normalCameraX");
	validConnectionMap.append("normalCameraY");
	validConnectionMap.append("normalCameraZ");
	validConnectionMap.append("normalCamera");
	validConnectionMap.append("outNormalX");
	validConnectionMap.append("outNormalY");
	validConnectionMap.append("outNormalZ");
	validConnectionMap.append("outNormal");
	validConnectionMap.end();

	//condition

	//distanceBetween

	//heightField

	//lightInfo

	//multiplyDivide

	//place2dTexture
	validConnectionMap.begin("place2dTexture");
	validConnectionMap.append("uCoord");
	validConnectionMap.append("vCoord");
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
	validConnectionMap.append("outU");
	validConnectionMap.append("outV");
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