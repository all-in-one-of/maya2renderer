#include <liqConfig.h>
#ifdef _USE_ELVISHRAY_

#include "shaderOutputER.h"
#include <liqlog.h>
#include "../common/mayacheck.h"
#include "../shadergraph/convertShadingNetwork.h"
#include "../shadergraph/shadermgr.h"
#include "er_renderer.h"
#include "er_helper.h"

namespace ER
{

OutputHelper::OutputHelper(std::ofstream& file_)
:file(elvishray::Renderer::m_log.get())
{
	assert(file.is_open());
}
//
OutputHelper::~OutputHelper()
{

}
//
void OutputHelper::addRSLVariable(MString rslType, const MString& rslName,
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
	liquidmaya::ConnectionType  connected = liquidmaya::ShaderMgr::getSingletonPtr()->convertibleConnection(plug.asChar());

	// If there are no convertible connections, then we have to
	// write out the variable into the shader's body.
	if( connected == liquidmaya::CT_None )
	{
		//rslTypeSize(int) --> rslTypeSizeStr(string)
		MString rslTypeSizeStr;
		rslTypeSizeStr.set(rslTypeSize);
		// Write out the description of the variable.
		rslShaderBody += (" " + rslType + " " + rslName);
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
			file<<"ei_shader_param_vector(\""<< rslName.asChar()<<"\", "<<val[0]<<", "<<val[1]<<", "<<val[2]<<");"<<endl;
		}else if(rslType=="string"){
			MString val;
			IfMErrorWarn(MGlobal::executeCommand("getAttr \""+plug+"\"", val));
			rslShaderBody +="\""+val+"\"";
			file<<"ei_shader_param_string(\""<<rslName.asChar()<<"\", \""<<val.asChar()<<"\");"<<endl;
		}else if(rslType=="shader"){
			MStringArray srcNode;
			//we only care about the input of this plug
			IfMErrorWarn(MGlobal::executeCommand("listConnections -source true -destination off -plugs off \""+plug+"\"", srcNode));
			assert(srcNode.length()==1);
			rslShaderBody +="\""+srcNode[0]+"\"";
			file<<"ei_shader_param_token(\""<<rslName.asChar()<<"\", ei_token(\""<<srcNode[0].asChar()<<"\") );"<<endl;
		}else if(rslType=="texture"){
			MString val;
			IfMErrorWarn(MGlobal::executeCommand("getAttr \""+plug+"\"", val));
			rslShaderBody +="\""+val+"\"";
			file<<"ei_shader_param_texture(\""<<rslName<<"\", \""<<val<<"\");"<<endl;
		}
		else if(rslType=="int"){
			int val;
			IfMErrorWarn(MGlobal::executeCommand("getAttr \""+plug+"\"", val));
			MString sVal; sVal.set(val);
			rslShaderBody +="\""+sVal+"\"";
			file<<"ei_shader_param_int(\""<<rslName.asChar()<<"\", "<<val<<");"<<endl;
		}
		else if(rslType=="index"){
			int val;
			IfMErrorWarn(MGlobal::executeCommand("getAttr \""+plug+"\"", val));
			MString sVal; sVal.set(val);
			rslShaderBody +="\""+sVal+"\"";
			file<<"ei_shader_param_index(\""<<rslName.asChar()<<"\", "<<val<<");"<<endl;
		}
		else if(rslType=="bool"){
			int val;
			IfMErrorWarn(MGlobal::executeCommand("getAttr \""+plug+"\"", val));
			MString sVal; sVal.set(val);
			rslShaderBody +="\""+sVal+"\"";
			file<<"ei_shader_param_bool(\""<<rslName.asChar()<<"\", "<<val<<");"<<endl;
		}
		else if(rslType=="tag"){
			file<<"//ei_shader_param_tag(\""<<rslName.asChar()<<"\", val); not implemented yet."<<endl;
		}
		else if(rslType=="node"){
			file<<"//ei_shader_param_node(\""<<rslName.asChar()<<"\", val); not implemented yet."<<endl;
		}
		else if(rslType=="vector4"){
			file<<"//ei_shader_param_vector4(\""<<rslName.asChar()<<"\", val); not implemented yet."<<endl;
		}
		else if(rslType=="float"){
			if(rslTypeSize == 1){
				double val;
				IfMErrorWarn(MGlobal::executeCommand("getAttr \""+plug+"\"", val));
				//val(double) --> valStr(string)
				MString valStr;
				valStr.set(val);
				rslShaderBody += valStr;
				file<<"ei_shader_param_scalar(\""<<rslName.asChar()<<"\", "<<val<<");"<<endl;
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
		}//if(rslType=="float")
		rslShaderBody += ";\n";
	}//if( $connected == liquidmaya::CT_None )
	// Otherwise, we have a convertible connection, so we'll be
	// adding the variable to the block's header.
	else{
		rslShaderHeader += " ";

		// Note if it's connected as an output.
		if(connected == liquidmaya::CT_Out)
		{
			rslShaderHeader += "output ";
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
				file<<"ei_shader_param_vector(\""<< rslName.asChar()<<"\", "<<val[0]<<", "<<val[1]<<", "<<val[2]<<");"<<endl;
			}else if(rslType=="string"){
				MString val;
				IfMErrorWarn(MGlobal::executeCommand("getAttr \""+plug+"\"", val));
				rslShaderBody +="\""+val+"\"";
				file<<"ei_shader_param_string(\""<<rslName.asChar()<<"\", \""<<val.asChar()<<"\");"<<endl;
			}else if(rslType=="shader"){
				MStringArray srcNode;
				//we only care about the input of this plug
				IfMErrorWarn(MGlobal::executeCommand("listConnections -source true -destination off -plugs off \""+plug+"\"", srcNode));
				assert(srcNode.length()==1);
				rslShaderBody +="\""+srcNode[0]+"\"";
				file<<"ei_shader_param_token(\""<<rslName.asChar()<<"\", ei_token(\""<<srcNode[0].asChar()<<"\") );"<<endl;
			}else if(rslType=="texture"){
				MString val;
				IfMErrorWarn(MGlobal::executeCommand("getAttr \""+plug+"\"", val));
				rslShaderBody +="\""+val+"\"";
				file<<"ei_shader_param_texture(\""<<rslName<<"\", \""<<val<<"\");"<<endl;
			}
			else if(rslType=="int"){
				int val;
				IfMErrorWarn(MGlobal::executeCommand("getAttr \""+plug+"\"", val));
				MString sVal; sVal.set(val);
				rslShaderBody +="\""+sVal+"\"";
				file<<"ei_shader_param_int(\""<<rslName.asChar()<<"\", "<<val<<");"<<endl;
			}
			else if(rslType=="index"){
				int val;
				IfMErrorWarn(MGlobal::executeCommand("getAttr \""+plug+"\"", val));
				MString sVal; sVal.set(val);
				rslShaderBody +="\""+sVal+"\"";
				file<<"ei_shader_param_index(\""<<rslName.asChar()<<"\", "<<val<<");"<<endl;
			}
			else if(rslType=="bool"){
				int val;
				IfMErrorWarn(MGlobal::executeCommand("getAttr \""+plug+"\"", val));
				MString sVal; sVal.set(val);
				rslShaderBody +="\""+sVal+"\"";
				file<<"ei_shader_param_bool(\""<<rslName.asChar()<<"\", "<<val<<");"<<endl;
			}
			else if(rslType=="tag"){
				file<<"//ei_shader_param_tag(\""<<rslName.asChar()<<"\", val); not implemented yet."<<endl;
			}
			else if(rslType=="node"){
				file<<"//ei_shader_param_node(\""<<rslName.asChar()<<"\", val); not implemented yet."<<endl;
			}
			else if(rslType=="vector4"){
				file<<"//ei_shader_param_vector4(\""<<rslName.asChar()<<"\", val); not implemented yet."<<endl;
			}
			else if(rslType=="float"){
				if(rslTypeSize == 1){
					double val;
					IfMErrorWarn(MGlobal::executeCommand("getAttr \""+plug+"\"", val));
					//val(double) --> valStr(string)
					MString valStr;
					valStr.set(val);
					rslShaderBody += valStr;
					file<<"ei_shader_param_scalar(\""<<rslName.asChar()<<"\", "<<val<<");"<<endl;
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
			}//if(rslType=="float")
			rslShaderBody += ";\n";
		}//if(connected == liquidmaya::CT_Out)

		// Write out the description.
		rslShaderHeader += ( rslType + " " + rslName );
		if( rslTypeSize != 1 )
		{
			rslShaderHeader += "[]";
		}
		rslShaderHeader += ";\n";

		//
		if(connected == liquidmaya::CT_In || connected == liquidmaya::CT_InOut)
		{
			MStringArray srcPlug;
			//we only care about the input of this plug
			IfMErrorWarn(MGlobal::executeCommand("listConnections -source true -destination off -plugs true \""+plug+"\"", srcPlug));
			assert(srcPlug.length()==1);
			MStringArray src;
			srcPlug[0].split('.',src);
			MString srcNode(src[0]);
			MString srcAttr(src[1]);
			rslShaderBody +="//"+plug+" <-- "+srcPlug[0]+"\n";

			//// if the srcNode is a texture
			//if( is2DTexture(srcNode) || is3DTexture(srcNode) ){
			//	if( is2DFileTexture(srcNode) ){
			//		MString fileTextureName;
			//		IfMErrorWarn(MGlobal::executeCommand("getAttr \""+srcNode+".fileTextureName\"", fileTextureName));
			//		file<<"ei_shader_param_texture(\""<<rslName<<"_tex\", \""<<fileTextureName<<"\");"<<std::endl;
			//	}else{
			//		//file<<"ei_shader_param_texture(\""<<rslName<<"_tex\", \""<<srcNode<<"\");"<<std::endl;
			//		file<<"ei_shader_link_param(\""<<rslName<<"\", \""<<srcNode<<"\", \""<<srcAttr<<"\");"<<std::endl;
			//	}
			//}
			//else//the srcNode is NOT a texture
			{
				file<<"ei_shader_link_param(\""<<rslName.asChar()<<"\", \""<<srcNode.asChar()<<"\", \""<<srcAttr.asChar()<<"\");"<<std::endl;
			}
		}

	}//else
}
//
void OutputHelper::addToRSL(const MString& code)
{
	file<<code.asChar()<<std::endl;
}
//
void OutputHelper::beginRSL (const MString &type_name, const MString &instance_name)
{
	CM_TRACE_FUNC("OutputHelper::beginRSL("<<type_name.asChar()<<","<<instance_name.asChar()<<")");

	file<< "ei_shader(\""<<type_name.asChar()<<"\", \""<<instance_name.asChar()<<"\");"<<std::endl;
}
//
void OutputHelper::endRSL ()
{
	CM_TRACE_FUNC("OutputHelper::endRSL()");

	file<< "ei_end_shader();"<< std::endl;
	file<<std::endl;
}
//
void OutputHelper::add_liq_UserDefinedNormal(const char* node)
{
	CM_TRACE_FUNC("OutputHelper::add_liq_UserDefinedNormal("<<node<<")");

	MString s; s.set( elvishray::isBumpMapConnected(node) );
	file<<"ei_shader_param_int(\"liq_UserDefinedNormal\", "<<s.asChar()<<");"<<endl;
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

}
void Visitor::preOutput(const char* shaderNodeName)
{
	CM_TRACE_FUNC("Visitor::preOutput("<<shaderNodeName<<")");
}
void Visitor::outputBegin(const char* startingNode)
{
	CM_TRACE_FUNC("Visitor::outputBegin("<<startingNode<<")");
	//file.open( (getShaderDirectory()+startingNode+".erapi").asChar() );
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

// 	file << "void " << shaderName << "()\n{\n";
// 	file << shaderMethodVariavles;
// 	file << "\n";
// 	file << shaderMethodBody;
// 	file << "}\n";
}
void  Visitor::addShaderMethodBody(const MString &currentNode,
								   const MStringArray& inputSrc,
								   const MStringArray& outputSrc)
{
	CM_TRACE_FUNC("Visitor::addShaderMethodBody("<<currentNode.asChar()<<","<<liqM(inputSrc)<<","<<liqM(outputSrc)<<")");

}
void Visitor::addShaderMethodVariavles(
	const MString &node,
	const MString &plug,
	const MString &type_,
	const int arraysize,
	const MString &details)
{
	CM_TRACE_FUNC("Visitor::addShaderMethodVariavles("<<node.asChar()<<","<<plug.asChar()<<","<<type_.asChar()<<","<<arraysize<<","<<details.asChar()<<")");

}
void Visitor::outputEnd()
{
	CM_TRACE_FUNC("Visitor::outputEnd()");
	//file.close();
}
void Visitor::postOutput()
{
	CM_TRACE_FUNC("Visitor::postOutput()");
}
void Visitor::outputShadingGroup(const char* shadingGroupNode)
{
	CM_TRACE_FUNC("Visitor::outputShadingGroup("<<shadingGroupNode<<")");

	MString cmd;

	MStringArray surfaceShaders;
	MStringArray volumeShaders;
	MStringArray displacementShaders;
	MStringArray shadowShaders;
	MStringArray environmentShaders;
	{
		getlistConnections(shadingGroupNode, "surfaceShader", surfaceShaders);

		getlistConnections(shadingGroupNode, "volumeShader", volumeShaders);

		getlistConnections(shadingGroupNode, "displacementShader", displacementShaders);

		getlistConnections(shadingGroupNode, "liqShadowShader", shadowShaders);

		getlistConnections(shadingGroupNode, "liqEnvironmentShader", environmentShaders);
	}

	// Work out where to put it & make sure the directory exists
	MString shadingGroupFileName(getShaderDirectory()+shadingGroupNode);

// 	std::ofstream shadingGroupFile;
// 	shadingGroupFile.open((shadingGroupFileName+".erapi").asChar());
	std::ofstream &shadingGroupFile=elvishray::Renderer::m_log.get();
	assert(shadingGroupFile.is_open());

	shadingGroupFile<<"ei_material(\""<<shadingGroupNode<<"\");"<<std::endl;
	if( surfaceShaders[0].length() != 0 ){
		shadingGroupFile<<"ei_surface_shader(\""<<surfaceShaders[0].asChar()<<"\");"<<std::endl;
	}
	if( volumeShaders[0].length() != 0 ){
		shadingGroupFile<<"ei_volume_shader(\""<<volumeShaders[0].asChar()<<"\");"<<std::endl;
	}
	if( displacementShaders[0].length() != 0 ){
		shadingGroupFile<<"ei_displace_shader(\""<<displacementShaders[0].asChar()<<"\");"<<std::endl;
	}
	if( shadowShaders[0].length() != 0 ){
		shadingGroupFile<<"ei_shadow_shader(\""<<shadowShaders[0].asChar()<<"\");"<<std::endl;
	}
	if( environmentShaders[0].length() != 0 ){
		shadingGroupFile<<"ei_environment_shader(\""<<environmentShaders[0].asChar()<<"\");"<<std::endl;
	}
	shadingGroupFile<<"ei_end_material();"<<std::endl;
//	shadingGroupFile.close();

	
}
//
}//namespace ER
#endif//_USE_ELVISHRAY_