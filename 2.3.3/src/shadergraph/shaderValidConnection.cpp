#include "shaderValidConnection.h"
#include <liqlog.h>

namespace liquidmaya
{
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
	validConnection.clear();
	validConnection.append("");
	validConnectionMap.insert(std::make_pair("null", validConnection));	

	// MATERIAL -----------------------------------
	/// surface ///
	//anisotropic

	//hairTubeShader

	//lambert
	validConnection.clear();
	validConnection.append("color");
	validConnection.append("transparency");
	validConnection.append("ambientColor");
	validConnection.append("incandescence");
	validConnection.append("diffuse");
	validConnection.append("outColor");
	validConnection.append("outTransparency");
	validConnectionMap.insert(std::make_pair("lambert", validConnection));	

	//layered shader

	//blinn
	validConnection.clear();
	validConnection.append("color");
	validConnection.append("transparency");
	validConnection.append("ambientColor");
	validConnection.append("incandescence");
	validConnection.append("diffuse");
	validConnection.append("eccentricity");
	validConnection.append("specularRollOff");
	validConnection.append("specularColor");
	validConnection.append("reflectivity");
	validConnection.append("reflectedColor");
	validConnection.append("outColor");
	validConnectionMap.insert(std::make_pair("blinn", validConnection));	

	//oceanShader 

	//phong
	validConnection.clear();
	validConnection.append("color");
	validConnection.append("transparency");
	validConnection.append("ambientColor");
	validConnection.append("incandescence");
	validConnection.append("normalCamera");
	validConnection.append("diffuse");
	validConnection.append("translucence");
	validConnection.append("translucenceDepth");
	validConnection.append("translucenceFocus");
	validConnection.append("cosinePower");
	validConnection.append("specularColor");
	validConnection.append("reflectivity");
	validConnection.append("reflectedColor");
	validConnection.append("matteOpacityMode");
	validConnection.append("matteOpacity");
	validConnection.append("reflectionLimit");
	validConnection.append("outColor");
	validConnection.append("outTransparency");
	validConnectionMap.insert(std::make_pair("phong", validConnection));	

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
	validConnection.clear();
	validConnection.append("alphaGain");
	validConnection.append("alphaIsLuminance");
	validConnection.append("alphaOffset");
	validConnection.append("color1");//color1
	validConnection.append("color1R");
	validConnection.append("color1G");
	validConnection.append("color1B");
	validConnection.append("color2");//color2
	validConnection.append("color2R");
	validConnection.append("color2G");
	validConnection.append("color2B");
	validConnection.append("colorGain");
	validConnection.append("colorOffset");
	validConnection.append("contrast");
	validConnection.append("defaultColor");
	validConnection.append("filter");
	validConnection.append("filterOffset");
	validConnection.append("invert");
	validConnection.append("uvCoord");//uvCoord
	validConnection.append("uCoord");
	validConnection.append("vCoord");
	validConnection.append("outAlpha");
	validConnection.append("outColor");//outColor
	validConnection.append("outColorR");
	validConnection.append("outColorG");
	validConnection.append("outColorB");
	validConnectionMap.insert(std::make_pair("checker", validConnection));	

	//cloth

	// file
	validConnection.clear();
	validConnection.append("alphaGain");
	validConnection.append("alphaIsLuminance");
	validConnection.append("alphaOffset");
	validConnection.append("colorGain");
	validConnection.append("colorOffset");
	validConnection.append("defaultColor");
	validConnection.append("fileTextureName");
	validConnection.append("filterType");
	validConnection.append("filter");
	validConnection.append("filterOffset");
	validConnection.append("invert");
	validConnection.append("uvCoord");
	validConnection.append("fileHasAlpha");
	validConnection.append("outAlpha");
	validConnection.append("outColor");
	validConnection.append("outTransparency");
	validConnectionMap.insert(std::make_pair("file", validConnection));	

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
	validConnection.clear();
	validConnection.append("normalCamera");
	validConnection.append("rayDirection");
	validConnection.append("filterSize");
	validConnection.append("uvFilterSize");
	validConnection.append("image");
	validConnection.append("uvCoord");
	validConnection.append("outAlpha");
	validConnection.append("outColor");
	validConnection.append("pointCamera");
	validConnection.append("inclination");
	validConnection.append("elevation");
	validConnection.append("skyRadius");
	validConnection.append("bottom");
	validConnection.append("top");
	validConnection.append("left");
	validConnection.append("right");
	validConnection.append("front");
	validConnection.append("back");
	validConnection.append("reflect");
	validConnection.append("eyeSpace");
	validConnection.append("infoBits");
	validConnectionMap.insert(std::make_pair("envBall", validConnection));	
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
	validConnection.clear();
	validConnection.append("bumpValue");
	validConnection.append("bumpDepth");
	validConnection.append("bumpInterp");
	validConnection.append("bumpFilter");
	validConnection.append("bumpFilterOffset");
	validConnection.append("normalCamera");
	validConnection.append("bumpNormal");
	validConnection.append("outNormal");
	validConnectionMap.insert(std::make_pair("bump2d", validConnection));

	//bump3d
	validConnection.clear();
	validConnection.append("bumpValue");
	validConnection.append("bumpDepth");
	validConnection.append("bumpFilter");
	validConnection.append("bumpFilterOffset");
	validConnection.append("normalCamera");
	validConnection.append("outNormal");
	validConnectionMap.insert(std::make_pair("bump3d", validConnection));

	//condition

	//distanceBetween

	//heightField

	//lightInfo

	//multiplyDivide

	//place2dTexture
	validConnection.clear();
	validConnection.append("uvCoord");
	validConnection.append("coverageU");
	validConnection.append("coverageV");
	validConnection.append("coverage");
	validConnection.append("mirrorU");
	validConnection.append("mirrorV");
	validConnection.append("noiseU");
	validConnection.append("noiseV");
	validConnection.append("noiseUV");
	validConnection.append("offsetU");
	validConnection.append("offsetV");
	validConnection.append("offset");
	validConnection.append("repeatU");
	validConnection.append("repeatV");
	validConnection.append("repeatUV");
	validConnection.append("rotateFrame");
	validConnection.append("rotateUV");
	validConnection.append("stagger");
	validConnection.append("translateFrameU");
	validConnection.append("translateFrameV");
	validConnection.append("translateFrame");
	validConnection.append("wrapU");
	validConnection.append("wrapV");
	validConnection.append("outUV");
	validConnectionMap.insert(std::make_pair("place2dTexture", validConnection));	

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

	///  liquidSurface  ///
	validConnection.clear();
	validConnection.append("");
	validConnectionMap.insert(std::make_pair("liquidSurface", validConnection));	


}
//
bool ShaderValidConnection::hasShaderType(const char* shadertype)const
{
	return (validConnectionMap.find(shadertype) != validConnectionMap.end());
}
//
const MStringArray& 
ShaderValidConnection::getValidConnectionRef(const char* shadertype) const
{
	if( hasShaderType(shadertype) ){
		if(strcmp("liquidSurface",shadertype)==0)
		{
			liquidMessage2(messageError, "getValidConnectionRef() of shader type \"%s\" is not implemented.", shadertype);
			return validConnectionMap.find(shadertype)->second;
		}else{
			return validConnectionMap.find(shadertype)->second;
		}
	}else{
		liquidMessage2(messageError, "shader type \"%s\" is not supported.", shadertype);
		assert(0&&"shader type is not supported.");
		return validConnectionMap.find("null")->second;
	}
}

}//namespace liquidmaya