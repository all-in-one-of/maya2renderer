#include <liqConfig.h>
#ifdef _USE_RENDERMAN_

#include "shaderOutputRSL.h"
#include <liqlog.h>
#include <liqShader.h>
#include <liqShaderFactory.h>
#include <liqGlobalVariable.h>
#include "../common/mayacheck.h"
#include "../shadergraph/convertShadingNetwork.h"
#include "../shadergraph/shadermgr.h"
#include "ri_interface.h"
#include "rm_helper.h"

namespace RSL
{
	void connectMStringArray(MString& des, const MStringArray& src)
	{
		des.clear();
		for(std::size_t i=0; i<src.length(); ++i){
			des += src[ i ] + ",";
		}
	}
//////////////////////////////////////////////////////////////////////////
OutputHelper::OutputHelper(std::ofstream& RSLfile)
:RSLfileRef(RSLfile)
{
	assert(RSLfileRef.is_open());
}
//
OutputHelper::~OutputHelper()
{

}
//
void OutputHelper::addRSLVariable(const MString& inputQualifier, MString rslType, const MString& rslName,
					const MString& mayaName, const MString& mayaNode)
{
	MString cmd;

	// If the user specified that the type was an array of floats
	// (eg "float2"), extract the size and set the type to float.
	int rslTypeSize = 1;

	MString matchedStr;
	IfMErrorWarn(MGlobal::executeCommand("match(\"float[0-9]+$\",\""+rslType+"\")", matchedStr));
	if(matchedStr != "")
	{
		IfMErrorWarn(MGlobal::executeCommand("match(\"[0-9]+$\",\""+rslType+"\")", matchedStr));
		rslTypeSize = matchedStr.asInt();
		rslType = "float";
	}

	// Create the plug's name, and check for convertible connections.
	MString plug(mayaNode+"."+mayaName);
	int connected = liquidmaya::ShaderMgr::getSingletonPtr()->convertibleConnection(plug.asChar());

	// If there are no convertible connections, then we have to
	// write out the variable into the shader's body.
	if( connected == 0 )
	{
		//rslTypeSize(int) --> rslTypeSizeStr(string)
		MString rslTypeSizeStr;
		rslTypeSizeStr.set(rslTypeSize);
		// Write out the description of the variable.
		rslShaderBody += (" "+inputQualifier + " " + rslType + " " + rslName);
		rslShaderBody += ( rslTypeSize != 1 )?
							 ( "[" + rslTypeSizeStr + "] = " )
							:( " = " + rslType + " " );

		// Write out the value of the variable.
		if(   rslType=="color"
			||rslType=="point"
			||rslType=="normal"
			||rslType=="vector")
		{
			MDoubleArray val; val.setLength(3);
			IfMErrorWarn(MGlobal::executeCommand("getAttr \""+plug+"\"", val));
			//val(double) --> valStr(string)
			MStringArray valStr; valStr.setLength(3);
			valStr[0].set(val[0]);
			valStr[1].set(val[1]);
			valStr[2].set(val[2]);
			rslShaderBody +="("+valStr[0]+","+valStr[1]+","+valStr[2]+")";
		}else if(rslType=="string"){
			MString val;
			IfMErrorWarn(MGlobal::executeCommand("getAttr \""+plug+"\"", val));
			rslShaderBody +="\""+val+"\"";
		}else if(rslType=="float"){
			if(rslTypeSize == 1){
				double val;
				IfMErrorWarn(MGlobal::executeCommand("getAttr \""+plug+"\"", val));
				//val(double) --> valStr(string)
				MString valStr;
				valStr.set(val);
				rslShaderBody += valStr;
			}else{
				rslShaderBody += "{ ";
				MDoubleArray val; val.setLength(rslTypeSize);
				IfMErrorWarn(MGlobal::executeCommand("getAttr \""+plug+"\"", val));
				for(int i=0; i<rslTypeSize; ++i){
					if( i != 0 ){
						rslShaderBody += ", ";
					}
					//val[i](double) --> valStr(string)
					MString valStr;
					valStr.set(val[i]);
					rslShaderBody += valStr;
				}
				rslShaderBody += " }";
			}
		}
		rslShaderBody += ";\n";
	}//if( $connected == 0 )
	// Otherwise, we have a convertible connection, so we'll be
	// adding the variable to the block's header.
	else{
		rslShaderHeader += " ";

		// Note if it's connected as an output.
		if(connected == 2){
			rslShaderHeader += "output ";
		}

		// Write out the description.
		rslShaderHeader += ( rslType + " " + rslName );
		if( rslTypeSize != 1 )
		{
			rslShaderHeader += "[]";
		}
		rslShaderHeader += ";\n";

		//
		if(connected == 1)
		{
			MStringArray srcPlug;
			IfMErrorWarn(MGlobal::executeCommand("listConnections -source true -plugs true \""+plug+"\"", srcPlug));
			assert(srcPlug.length()==1);
			rslShaderBody +="//"+plug+" <-- "+srcPlug[0]+"\n";
		}

	}//else
}
//
void OutputHelper::addToRSL(const MString& code)
{
	rslShaderBody += (" "+code+"\n");
}
//
void OutputHelper::beginRSL (const MString& name)
{
	CM_TRACE_FUNC("OutputHelper::beginRSL("<<name.asChar()<<")");

	// "Open" the header and body.
	//
	rslShaderHeader  = ( "//shader name: " + name + "\n" );
	rslShaderHeader += ( "void " + getShaderName(name) + " (\n" );
	rslShaderBody = "{\n";
}
//
void OutputHelper::endRSL ()
{
	CM_TRACE_FUNC("OutputHelper::endRSL()");

	// "Close" the header and body.
	//
	rslShaderHeader += ")\n";
	rslShaderBody += "}\n";

	//include files
	std::set<std::string>::iterator i = includedFiles.begin();
	std::set<std::string>::iterator e = includedFiles.end();
	for(; i!=e; ++i){
		RSLfileRef << "#include \""<< *i<<"\"\n";//"" is better than <>
	}
	//
	RSLfileRef << (rslShaderHeader + rslShaderBody + "\n").asChar() ;
}
void OutputHelper::addInclude(const MString& file)
{
	CM_TRACE_FUNC("OutputHelper::addInclude("<<file.asChar()<<")");
	includedFiles.insert(file.asChar());
}
bool OutputHelper::isOutput(const MString& plugName)
{
	CM_TRACE_FUNC("OutputHelper::isOutput("<<plugName.asChar()<<")");
	
	return 	(2 == liquidmaya::ShaderMgr::getSingletonPtr()
		->convertibleConnection(plugName.asChar()));
}
//////////////////////////////////////////////////////////////////////////
Visitor::Visitor()
{

}
//
Visitor::~Visitor()
{

}
//
void Visitor::initShaderData(const MString& startingNode, const MString &mayaplug)
{
	CM_TRACE_FUNC("Visitor::initShaderData("<<startingNode.asChar()<<","<<mayaplug.asChar()<<")");

	shaderType = getRSLShaderType(mayaplug);

	shaderData.setLength(3);
	shaderData[SHADER_METHOD_VARIAVLES_I] = "";// shader method variables
	shaderData[SHADER_METHOD_BODY_I]      = "";// shader method body
	shaderData[SHADER_NAME_I]             = startingNode;// shader name

}
void Visitor::preOutput(const char* shaderNodeName)
{
	CM_TRACE_FUNC("Visitor::preOutput("<<shaderNodeName<<")");

	this->shaderNodeName = shaderNodeName;
}
void Visitor::outputBegin(const char* shaderNodeName)
{
	CM_TRACE_FUNC("Visitor::outputBegin("<<shaderNodeName<<"), open shader file");
	RSLfile.open( renderman::getShaderFilePath_SRC(shaderNodeName).asChar() );

	RSLfile << "#include <"<<getAOVMacroDefineFileName()<<">" <<std::endl;

	RSLfile << "//surface shader name: " << shaderNodeName << std::endl;
	RSLfile << shaderType.asChar()<<" "<< getShaderName(shaderNodeName).asChar() << std::endl;
	
	//shader parameters
	RSLfile << "(" << std::endl;
	RSLfile << getSurfaceShaderAOVOutputParametersString() << std::endl;
	RSLfile << ")"<< std::endl;

	//shader body begin
	RSLfile << "{"<< std::endl;

	defineAOVVariables();

	RSLfile << "\n\n\n// define the shaders --------------------------------------------------------------------------\n";
}
void Visitor::outputUpstreamShader(const char* shaderNodeName)
{
	CM_TRACE_FUNC("Visitor::outputUpstreamShader("<<shaderNodeName<<")");

	MString nodetype;
	IfMErrorWarn(MGlobal::executeCommand( ("nodeType \""+MString(shaderNodeName)+"\""), nodetype));

	_outputUpstreamShader(shaderNodeName, nodetype.asChar());
}
void Visitor::outputShaderMethod()
{
	CM_TRACE_FUNC("Visitor::outputShaderMethod()");
	
	RSLfile << "\n\n\n// call the shaders ----------------------------------------------------------------------------\n";
	RSLfile << shaderData[SHADER_METHOD_VARIAVLES_I].asChar();
	RSLfile << "\n";
	RSLfile << shaderData[SHADER_METHOD_BODY_I].asChar();
	RSLfile << "}\n";
}
void  Visitor::addShaderMethodBody(
						 const MString &currentNode,
						 const MStringArray& inputVars,
						 const MStringArray& outputVars)
{
	CM_TRACE_FUNC("Visitor::addShaderMethodBody("<<currentNode.asChar()<<","<<liqM(inputVars)<<","<<liqM(outputVars)<<")");

	MString varString;
	{
		MStringArray vars;
		for(std::size_t i=0; i<inputVars.length(); ++i){
			vars.append( getVariableName(inputVars[i]) );
		}
		for(std::size_t i=0; i<outputVars.length(); ++i){
			vars.append( getVariableName(outputVars[i]) );
		}

		for(std::size_t index=0; index<vars.length(); ++index){
			varString += (index == vars.length() - 1)?(vars[index]):(vars[index]+", ");
		}
	}

	// Add the current node method to the shader body
	shaderData[ SHADER_METHOD_BODY_I ] += " //" + currentNode +"\n";
	shaderData[ SHADER_METHOD_BODY_I ] += " " + getShaderName(currentNode) +"("+varString+");\n";
	
	// test the input and output of currentNode
	{	
		MString inputVarsStr; 
		MString outputVarsStr;
		connectMStringArray(inputVarsStr, inputVars);
		connectMStringArray(outputVarsStr, outputVars);
		shaderData[ SHADER_METHOD_BODY_I ] += "//input: " + inputVarsStr +"\n";
		shaderData[ SHADER_METHOD_BODY_I ] += "//output:" + outputVarsStr +"\n\n";
	}
}
void Visitor::addShaderMethodVariavles(
	const MString &node,
	const MString &plug,
	const MString &type_,
	const int arraysize,
	const MString &details)
{
	CM_TRACE_FUNC("Visitor::addShaderMethodVariavles("<<node.asChar()<<","<<plug.asChar()<<","<<type_.asChar()<<","<<arraysize<<","<<details.asChar()<<")");

	if(arraysize == -1)
	{
		shaderData[SHADER_METHOD_VARIAVLES_I] += " "+type_+" "+ getVariableName(node, plug)+";\n";
	}else{
		MString strArraySize;
		strArraySize.set(arraysize);
		shaderData[SHADER_METHOD_VARIAVLES_I] += " "+type_+" "+ getVariableName(node, plug)+"["+strArraySize+"];\n";
	}
}
void Visitor::outputEnd()
{
	CM_TRACE_FUNC("Visitor::outputEnd(), close shader file");
	RSLfile.close();
}
void Visitor::postOutput()
{
	CM_TRACE_FUNC("Visitor::postOutput()");

	//compile the shader
	MString shaderdir(getShaderDirectory());
	MString outSLO(renderman::getShaderFilePath_SLO(shaderNodeName.c_str()));
	MString srcSL (renderman::getShaderFilePath_SRC(shaderNodeName.c_str()));

	MString result;
	//NOTE:
	//     the include directory can't contain '.', so I move _3delight to %LIQUID_ROOT%\dependence
	//"shader.exe -o \"outSLO\" -I\"%LIQUID_ROOT%\dependence\_3delight" \"srcSL\""
	IfMErrorWarn(MGlobal::executeCommand("system(\"shader -o \\\""+outSLO+"\\\" -I\\\"%LIQUID_ROOT%/dependence/_3delight\\\" \\\""+srcSL+"\\\"\")", result, true));

	//show the error if there is.
	std::string strRes(result.toLowerCase().asChar());
	if(strRes.find("error") != std::string::npos)
	{
		liqAssert(strRes.c_str());
	}


}
void Visitor::outputShadingGroup(const char* shadingGroupNode)
{
	CM_TRACE_FUNC("Visitor::outputShadingGroup("<<shadingGroupNode<<")");
	
	outputShadingGroup(shadingGroupNode, false);
}
void Visitor::outputShadingGroup(const char* shadingGroupNode, const bool bReference)
{
	CM_TRACE_FUNC("Visitor::outputShadingGroup("<<shadingGroupNode<<","<<bReference<<")");
	
	if(liqglo.m_writeDataIntoMainRibFile)
	{
		if( bReference ){
			//here is the right place where shading group data should be written into the main rib file.
			_outputShadingGroup(shadingGroupNode);
		}else{
			//do nothing
		}
	}else{
		if( !bReference ){
			renderman::Helper o;
			o.RiBeginRef(renderman::getShadingGroupFilePath(shadingGroupNode).asChar());
			_outputShadingGroup(shadingGroupNode);
			o.RiEndRef();
		}else{
			RiArchiveRecord( RI_COMMENT, "use Shading Group reference:" );
			RiReadArchive( const_cast< RtToken >( renderman::getShadingGroupFilePath(shadingGroupNode).asChar() ), NULL, RI_NULL );
		}
	}
}

void _outputShadingGroup(const char* shadingGroupNode)
{
	CM_TRACE_FUNC("_outputShadingGroup("<<shadingGroupNode<<")");
	//
	MStringArray surfaceShaders;
	MStringArray volumeShaders;
	MStringArray displacementShaders;
	{
		getlistConnections(shadingGroupNode, "surfaceShader", surfaceShaders);
		getlistConnections(shadingGroupNode, "volumeShader", volumeShaders);
		getlistConnections(shadingGroupNode, "displacementShader", displacementShaders);
	}
	//
	RiArchiveRecord(RI_COMMENT, "shading group: %s", shadingGroupNode);
	//surface shader
	if( surfaceShaders[0].length() != 0 ){
		MString nodetype;
		getNodeType(nodetype, surfaceShaders[0]);
		if( nodetype == "liquidSurface" ){
			liqShader& currentShader = 
				liqShaderFactory::instance().getShader( surfaceShaders[0].asChar() );
			currentShader.write();
		}else{
			RiSurface( const_cast<char *>(renderman::getShaderFilePath_NoExt(surfaceShaders[0]).asChar()), RI_NULL );
		}
	}else{
		RiArchiveRecord(RI_COMMENT, "no surface shader.");
	}
	//volume shader
	if( volumeShaders[0].length() != 0 ){
		MString nodetype;
		getNodeType(nodetype, volumeShaders[0]);
		if( nodetype == "liquidVolume" ){
			liqShader& currentShader = 
				liqShaderFactory::instance().getShader( volumeShaders[0].asChar() );
			currentShader.write();
		}else{
			RiArchiveRecord(RI_COMMENT, "I'm not sure which one should be used for the volume shader, RiAtmosphere(), RiInterior(), or RiExterior().");
			RiAtmosphere( const_cast<char *>(renderman::getShaderFilePath_NoExt(volumeShaders[0]).asChar()), RI_NULL );
		}
	}else{
		RiArchiveRecord(RI_COMMENT, "no volume shader.");
	}
	//displacement shader
	if( displacementShaders[0].length() != 0 ){
		MString nodetype;
		getNodeType(nodetype, displacementShaders[0]);
		if( nodetype == "liquidDisplacement" ){
			liqShader& currentShader = 
				liqShaderFactory::instance().getShader( displacementShaders[0].asChar() );
			currentShader.write();
		}else{
			RiArchiveRecord(RI_COMMENT, "I'm not sure which one should be used for the displacement shader, RiDeformation(), or RiDisplacement().");
			RiDisplacement( const_cast<char *>(renderman::getShaderFilePath_NoExt(displacementShaders[0]).asChar()), RI_NULL );
		}
	}else{
		RiArchiveRecord(RI_COMMENT, "no displacement shader.");
	}
}
MString Visitor::getRSLShaderType(const MString &mayaplug)
{
	MString shaderType;

	if(mayaplug=="surfaceShader"){
		shaderType = "surface";
	}else if(mayaplug=="displacementShader"){
		shaderType = "displacement";
	}else if(mayaplug=="volumeShader"){
		shaderType = "volume";
	}else{
		liquidMessage2(messageError,"unkown shader type for plug %s", mayaplug.asChar());
	}
	return shaderType;
}
void Visitor::defineAOVVariables()
{
	CM_TRACE_FUNC("Visitor::defineAOVVariables("<<shaderNodeName<<")");

	RSLfile << "\n\n\n// define some extern variables which are used in 3delight shaders -----------------------------\n";
	RSLfile << " color __transparency = color (1,1,1);" << std::endl;

	std::stringstream aov_params;
	aov_params 
	//begin shader_extra_parameters aov_ambient
	<<"#ifdef USE_AOV_aov_ambient"	<<std::endl
	<<"		color aov_ambient = 0;"	<<std::endl
	<<"#endif"						<<std::endl
	//end shader_extra_parameters

	//begin shader_extra_parameters aov_diffuse
	<<"#ifdef USE_AOV_aov_diffuse"	<<std::endl
	<<"		color aov_diffuse = 0;"	<<std::endl
	<<"#endif"						<<std::endl
	//end shader_extra_parameters

	//begin shader_extra_parameters aov_specular
	<<"#ifdef USE_AOV_aov_specular"		<<std::endl
	<<"		color aov_specular = 0;"	<<std::endl
	<<"#endif"							<<std::endl
	//end shader_extra_parameters

	//begin shader_extra_parameters aov_reflection
	<<"#ifdef USE_AOV_aov_reflection"	<<std::endl
	<<"		color aov_reflection = 0;"	<<std::endl
	<<"#endif"							<<std::endl
	//end shader_extra_parameters

	//begin shader_extra_parameters aov_rt_reflection
	<<"#ifdef USE_AOV_aov_rt_reflection"	<<std::endl
	<<"		color aov_rt_reflection = 0;"	<<std::endl
	<<"#endif"								<<std::endl
	//end shader_extra_parameters

	//begin shader_extra_parameters aov_rt_reflection_alpha
	<<"#ifdef USE_AOV_aov_rt_reflection_alpha"	<<std::endl
	<<"		float aov_rt_reflection_alpha = 0;"	<<std::endl
	<<"#endif"									<<std::endl
	//end shader_extra_parameters

	//begin shader_extra_parameters aov_env_reflection
	<<"#ifdef USE_AOV_aov_env_reflection"	<<std::endl
	<<"		color aov_env_reflection = 0;"	<<std::endl
	<<"#endif"								<<std::endl
	//end shader_extra_parameters

	//begin shader_extra_parameters aov_refraction
	<<"#ifdef USE_AOV_aov_refraction"	<<std::endl
	<<"		color aov_refraction = 0;"	<<std::endl
	<<"#endif"							<<std::endl
	//end shader_extra_parameters

	//begin shader_extra_parameters aov_shadow
	<<"#ifdef USE_AOV_aov_shadow"	<<std::endl
	<<"		float aov_shadow = 0;"	<<std::endl
	<<"#endif"						<<std::endl
	//end shader_extra_parameters 

	//begin shader_extra_parameters aov_incandescence
	<<"#ifdef USE_AOV_aov_incandescence"	<<std::endl
	<<"		color aov_incandescence = 0;"	<<std::endl
	<<"#endif"								<<std::endl
	//end shader_extra_parameters

	//begin shader_extra_parameters aov_translucence
	<<"#ifdef USE_AOV_aov_translucence"		<<std::endl
	<<"		color aov_translucence = 0;"	<<std::endl
	<<"#endif"								<<std::endl
	//end shader_extra_parameters

	//begin shader_extra_parameters aov_ambient_no_shadow
	<<"#ifdef USE_AOV_aov_ambient_no_shadow"	<<std::endl
	<<"		color aov_ambient_no_shadow = 0;"	<<std::endl
	<<"#endif"									<<std::endl
	//end shader_extra_parameters

	//begin shader_extra_parameters aov_diffuse_no_shadow
	<<"#ifdef USE_AOV_aov_diffuse_no_shadow"	<<std::endl
	<<"		color aov_diffuse_no_shadow = 0;"	<<std::endl
	<<"#endif"									<<std::endl
	//end shader_extra_parameters

	//begin shader_extra_parameters aov_diffuse_intensity
	<<"#ifdef USE_AOV_aov_diffuse_intensity"	<<std::endl
	<<"		color aov_diffuse_intensity = 0;"	<<std::endl
	<<"#endif"									<<std::endl
	//end shader_extra_parameters

	//begin shader_extra_parameters aov_diffuse_intensity_no_shadow
	<<"#ifdef USE_AOV_aov_diffuse_intensity_no_shadow"	<<std::endl
	<<"		color aov_diffuse_intensity_no_shadow = 0;"	<<std::endl
	<<"#endif"											<<std::endl
	//end shader_extra_parameters

	//begin shader_extra_parameters aov_key_lights_diffuse_intensity
	<<"#ifdef USE_AOV_aov_key_lights_diffuse_intensity"		<<std::endl
	<<"		color aov_key_lights_diffuse_intensity = 0;"	<<std::endl
	<<"#endif"												<<std::endl
	//end shader_extra_parameters

	//begin shader_extra_parameters aov_key_lights_diffuse_intensity_no_shadow
	<<"#ifdef USE_AOV_aov_key_lights_diffuse_intensity_no_shadow"	<<std::endl
	<<"		color aov_key_lights_diffuse_intensity_no_shadow = 0;"	<<std::endl
	<<"#endif"														<<std::endl
	//end shader_extra_parameters

	//begin shader_extra_parameters aov_luminance_depth
	<<"#ifdef USE_AOV_aov_luminance_depth"	<<std::endl
	<<"		color aov_luminance_depth = 0;"	<<std::endl
	<<"#endif"								<<std::endl
	//end shader_extra_parameters

	//begin shader_extra_parameters aov_specular_no_shadow
	<<"#ifdef USE_AOV_aov_specular_no_shadow"	<<std::endl
	<<"		color aov_specular_no_shadow = 0;"	<<std::endl
	<<"#endif"									<<std::endl
	//end shader_extra_parameters

	//begin shader_extra_parameters aov_specular_intensity
	<<"#ifdef USE_AOV_aov_specular_intensity"	<<std::endl
	<<"		color aov_specular_intensity = 0;"	<<std::endl
	<<"#endif"									<<std::endl
	//end shader_extra_parameters

	//begin shader_extra_parameters aov_specular_intensity_no_shadow
	<<"#ifdef USE_AOV_aov_specular_intensity_no_shadow"		<<std::endl
	<<"		color aov_specular_intensity_no_shadow = 0;"	<<std::endl
	<<"#endif"												<<std::endl
	//end shader_extra_parameters

	//begin shader_extra_parameters aov_key_lights_specular_intensity
	<<"#ifdef USE_AOV_aov_key_lights_specular_intensity"	<<std::endl
	<<"		color aov_key_lights_specular_intensity = 0;"	<<std::endl
	<<"#endif"												<<std::endl
	//end shader_extra_parameters

	//begin shader_extra_parameters aov_key_lights_specular_intensity_no_shadow
	<<"#ifdef USE_AOV_aov_key_lights_specular_intensity_no_shadow"	<<std::endl
	<<"		color aov_key_lights_specular_intensity_no_shadow = 0;"	<<std::endl
	<<"#endif"														<<std::endl
	//end shader_extra_parameters

	//begin shader_extra_parameters aov_color_no_shadow
	<<"#ifdef USE_AOV_aov_color_no_shadow"	<<std::endl
	<<"		color aov_color_no_shadow = 0;"	<<std::endl
	<<"#endif"								<<std::endl
	//end shader_extra_parameters

	//begin shader_extra_parameters aov_surface_color
	<<"#ifdef USE_AOV_aov_surface_color"	<<std::endl
	<<"		color aov_surface_color = 0;"	<<std::endl
	<<"#endif"								<<std::endl
	//end shader_extra_parameters

	//begin shader_extra_parameters aov_normal
	<<"#ifdef USE_AOV_aov_camera_space_normal"		<<std::endl
	<<"		normal aov_camera_space_normal = 0;"	<<std::endl
	<<"#endif"										<<std::endl
	//end shader_extra_parameters

	//begin shader_extra_parameters aov_facing_ratio
	<<"#ifdef USE_AOV_aov_facing_ratio"				<<std::endl
	<<"		float aov_facing_ratio = 0;"			<<std::endl
	<<"#endif"										<<std::endl
	//end shader_extra_parameters
	;
	//RSLfile << aov_params.str();
}
std::string Visitor::getSurfaceShaderAOVOutputParametersString()const
{
	CM_TRACE_FUNC("Visitor::getSurfaceShaderAOVOutputParametersString()");

	std::stringstream ret;
	ret 
//begin shader_extra_parameters aov_ambient
<<"#ifdef USE_AOV_aov_ambient"					<<std::endl
<<"		output varying color aov_ambient = 0;"	<<std::endl
<<"#endif"										<<std::endl
//end shader_extra_parameters

//begin shader_extra_parameters aov_diffuse
<<"#ifdef USE_AOV_aov_diffuse"					<<std::endl
<<"		output varying color aov_diffuse = 0;"	<<std::endl
<<"#endif"										<<std::endl
//end shader_extra_parameters

//begin shader_extra_parameters aov_specular
<<"#ifdef USE_AOV_aov_specular"					<<std::endl
<<"		output varying color aov_specular = 0;"	<<std::endl
<<"#endif"										<<std::endl
//end shader_extra_parameters

//begin shader_extra_parameters aov_reflection
<<"#ifdef USE_AOV_aov_reflection"					<<std::endl
<<"		output varying color aov_reflection = 0;"	<<std::endl
<<"#endif"											<<std::endl
//end shader_extra_parameters

//begin shader_extra_parameters aov_rt_reflection
<<"#ifdef USE_AOV_aov_rt_reflection"					<<std::endl
<<"		output varying color aov_rt_reflection = 0;"	<<std::endl
<<"#endif"												<<std::endl
//end shader_extra_parameters

//begin shader_extra_parameters aov_rt_reflection_alpha
<<"#ifdef USE_AOV_aov_rt_reflection_alpha"					<<std::endl
<<"		output varying float aov_rt_reflection_alpha = 0;"	<<std::endl
<<"#endif"													<<std::endl
//end shader_extra_parameters

//begin shader_extra_parameters aov_env_reflection
<<"#ifdef USE_AOV_aov_env_reflection"					<<std::endl
<<"		output varying color aov_env_reflection = 0;"	<<std::endl
<<"#endif"												<<std::endl
//end shader_extra_parameters

//begin shader_extra_parameters aov_refraction
<<"#ifdef USE_AOV_aov_refraction"					<<std::endl
<<"		output varying color aov_refraction = 0;"	<<std::endl
<<"#endif"											<<std::endl
//end shader_extra_parameters

//begin shader_extra_parameters aov_shadow
<<"#ifdef USE_AOV_aov_shadow"					<<std::endl
<<"		output varying float aov_shadow = 0;"	<<std::endl
<<"#endif"										<<std::endl
//end shader_extra_parameters 

//begin shader_extra_parameters aov_incandescence
<<"#ifdef USE_AOV_aov_incandescence"					<<std::endl
<<"		output varying color aov_incandescence = 0;"	<<std::endl
<<"#endif"												<<std::endl
//end shader_extra_parameters

//begin shader_extra_parameters aov_translucence
<<"#ifdef USE_AOV_aov_translucence"					<<std::endl
<<"		output varying color aov_translucence = 0;"	<<std::endl
<<"#endif"											<<std::endl
//end shader_extra_parameters

//begin shader_extra_parameters aov_ambient_no_shadow
<<"#ifdef USE_AOV_aov_ambient_no_shadow"					<<std::endl
<<"		output varying color aov_ambient_no_shadow = 0;"	<<std::endl
<<"#endif"													<<std::endl
//end shader_extra_parameters

//begin shader_extra_parameters aov_diffuse_no_shadow
<<"#ifdef USE_AOV_aov_diffuse_no_shadow"					<<std::endl
<<"		output varying color aov_diffuse_no_shadow = 0;"	<<std::endl
<<"#endif"													<<std::endl
//end shader_extra_parameters

//begin shader_extra_parameters aov_diffuse_intensity
<<"#ifdef USE_AOV_aov_diffuse_intensity"					<<std::endl
<<"		output varying color aov_diffuse_intensity = 0;"	<<std::endl
<<"#endif"													<<std::endl
//end shader_extra_parameters

//begin shader_extra_parameters aov_diffuse_intensity_no_shadow
<<"#ifdef USE_AOV_aov_diffuse_intensity_no_shadow"					<<std::endl
<<"		output varying color aov_diffuse_intensity_no_shadow = 0;"	<<std::endl
<<"#endif"															<<std::endl
//end shader_extra_parameters

//begin shader_extra_parameters aov_key_lights_diffuse_intensity
<<"#ifdef USE_AOV_aov_key_lights_diffuse_intensity"					<<std::endl
<<"		output varying color aov_key_lights_diffuse_intensity = 0;"	<<std::endl
<<"#endif"															<<std::endl
//end shader_extra_parameters

//begin shader_extra_parameters aov_key_lights_diffuse_intensity_no_shadow
<<"#ifdef USE_AOV_aov_key_lights_diffuse_intensity_no_shadow"					<<std::endl
<<"		output varying color aov_key_lights_diffuse_intensity_no_shadow = 0;"	<<std::endl
<<"#endif"																		<<std::endl
//end shader_extra_parameters

//begin shader_extra_parameters aov_luminance_depth
<<"#ifdef USE_AOV_aov_luminance_depth"					<<std::endl
<<"		output varying color aov_luminance_depth = 0;"	<<std::endl
<<"#endif"												<<std::endl
//end shader_extra_parameters

//begin shader_extra_parameters aov_specular_no_shadow
<<"#ifdef USE_AOV_aov_specular_no_shadow"					<<std::endl
<<"		output varying color aov_specular_no_shadow = 0;"	<<std::endl
<<"#endif"													<<std::endl
//end shader_extra_parameters

//begin shader_extra_parameters aov_specular_intensity
<<"#ifdef USE_AOV_aov_specular_intensity"					<<std::endl
<<"		output varying color aov_specular_intensity = 0;"	<<std::endl
<<"#endif"													<<std::endl
//end shader_extra_parameters

//begin shader_extra_parameters aov_specular_intensity_no_shadow
<<"#ifdef USE_AOV_aov_specular_intensity_no_shadow"					<<std::endl
<<"		output varying color aov_specular_intensity_no_shadow = 0;"	<<std::endl
<<"#endif"															<<std::endl
//end shader_extra_parameters

//begin shader_extra_parameters aov_key_lights_specular_intensity
<<"#ifdef USE_AOV_aov_key_lights_specular_intensity"					<<std::endl
<<"		output varying color aov_key_lights_specular_intensity = 0;"	<<std::endl
<<"#endif"																<<std::endl
//end shader_extra_parameters

//begin shader_extra_parameters aov_key_lights_specular_intensity_no_shadow
<<"#ifdef USE_AOV_aov_key_lights_specular_intensity_no_shadow"					<<std::endl
<<"		output varying color aov_key_lights_specular_intensity_no_shadow = 0;"	<<std::endl
<<"#endif"																		<<std::endl
//end shader_extra_parameters

//begin shader_extra_parameters aov_color_no_shadow
<<"#ifdef USE_AOV_aov_color_no_shadow"					<<std::endl
<<"		output varying color aov_color_no_shadow = 0;"	<<std::endl
<<"#endif"												<<std::endl
//end shader_extra_parameters

//begin shader_extra_parameters aov_surface_color
<<"#ifdef USE_AOV_aov_surface_color"					<<std::endl
<<"		output varying color aov_surface_color = 0;"	<<std::endl
<<"#endif"												<<std::endl
//end shader_extra_parameters

//begin shader_extra_parameters aov_normal
<<"#ifdef USE_AOV_aov_camera_space_normal"					<<std::endl
<<"		output varying normal aov_camera_space_normal = 0;"	<<std::endl
<<"#endif"													<<std::endl
//end shader_extra_parameters

//begin shader_extra_parameters aov_facing_ratio
<<"#ifdef USE_AOV_aov_facing_ratio"							<<std::endl
<<"		output varying float aov_facing_ratio = 0;"			<<std::endl
<<"#endif"													<<std::endl
//end shader_extra_parameters
;
		return ret.str();
}
std::string Visitor::getAOVMacroDefineFileName()const
{
	return "liquidAOVMacroDef.h";
}
//
}//namespace RSL

#endif//_USE_RENDERMAN_