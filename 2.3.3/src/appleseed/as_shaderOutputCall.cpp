#include <liqConfig.h>
#ifdef _USE_APPLESEED_

#include "as_shaderOutputCall.h"
#include <liqlog.h>
#include "../common/mayacheck.h"
#include "../shadergraph/convertShadingNetwork.h"
#include "../shadergraph/shadermgr.h"
#include "prerequest_as.h"
#include "as_renderer.h"
#include "../renderermgr.h"

namespace appleseed{
namespace call{

OutputHelper::OutputHelper()
{
}
//
OutputHelper::~OutputHelper()
{

}
//
void OutputHelper::addVariable(MString rslType, const MString& rslName,
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
			{
				//ei_shader_param_vector( rslName.asChar(), val[0], val[1], val[2]);
			}
		}else if(rslType=="string"){
			MString val;
			IfMErrorWarn(MGlobal::executeCommand("getAttr \""+plug+"\"", val));
			rslShaderBody +="\""+val+"\"";
			{
				//ei_shader_param_string( rslName.asChar(), val.asChar());
			}
		}else if(rslType=="texture"){
			MString val;
			IfMErrorWarn(MGlobal::executeCommand("getAttr \""+plug+"\"", val));
			rslShaderBody +="\""+val+"\"";
			{
				//ei_shader_param_texture(rslName.asChar(),val.asChar());
			}
		}
		else if(rslType=="int"){
			int val;
			IfMErrorWarn(MGlobal::executeCommand("getAttr \""+plug+"\"", val));
			MString sVal; sVal.set(val);
			rslShaderBody +="\""+sVal+"\"";
			{
				//ei_shader_param_int(rslName.asChar(),val);
			}
		}
		else if(rslType=="index"){
			int val;
			IfMErrorWarn(MGlobal::executeCommand("getAttr \""+plug+"\"", val));
			MString sVal; sVal.set(val);
			rslShaderBody +="\""+sVal+"\"";
			{
				//eiIndex iVal = val;
				//ei_shader_param_index(rslName.asChar(), iVal);
			}
		}
		else if(rslType=="bool"){
			int val;
			IfMErrorWarn(MGlobal::executeCommand("getAttr \""+plug+"\"", val));
			MString sVal; sVal.set(val);
			rslShaderBody +="\""+sVal+"\"";
			{
				//eiBool bVal = val;
				//ei_shader_param_bool(rslName.asChar(), bVal );
			}
		}
		else if(rslType=="tag"){
			liquidMessage2(messageError,MString(rslType+" is not implemented yet.").asChar() );
		}
		else if(rslType=="node"){
			liquidMessage2(messageError,MString(rslType+" is not implemented yet.").asChar() );
		}
		else if(rslType=="vector4"){
			liquidMessage2(messageError,MString(rslType+" is not implemented yet.").asChar() );
		}
		else if(rslType=="float"){
			if(rslTypeSize == 1){
				double val;
				IfMErrorWarn(MGlobal::executeCommand("getAttr \""+plug+"\"", val));
				//val(double) --> valStr(string)
				MString valStr;
				valStr.set(val);
				rslShaderBody += valStr;
				{
					//ei_shader_param_scalar( rslName.asChar(), val );
				}
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
				{
					//ei_shader_param_vector( rslName.asChar(), val[0], val[1], val[2]);
				}
			}
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
			//		ei_shader_param_texture((rslName+"_tex").asChar(), fileTextureName.asChar());
			//	}else{
			//		//ei_shader_param_texture((rslName+"_tex").asChar(), srcNode.asChar());
			//		ei_shader_link_param(rslName.asChar(), srcNode.asChar(), srcAttr.asChar());
			//	}
			//}
			//else//the srcNode is NOT a texture
			{
				//ei_shader_link_param( rslName.asChar(), srcNode.asChar(), srcAttr.asChar() );
			}
		}

	}//else
}
//
// void OutputHelper::addTo(const MString& code)
// {
// 	file<<code<<std::endl;
// }
//
void OutputHelper::begin (const MString& name)
{
	CM_TRACE_FUNC("OutputHelper::begin("<<name.asChar()<<")");
}
//
void OutputHelper::end ()
{
	CM_TRACE_FUNC("OutputHelper::end()");
}
//////////////////////////////////////////////////////////////////////////
Visitor::Visitor()
{
	m_assembly = NULL;
}
//
Visitor::~Visitor()
{
}
//

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
}
void  Visitor::addShaderMethodBody(const MString &currentNode,
								   const MStringArray& inputVars,
								   const MStringArray& outputVars)
{
	CM_TRACE_FUNC("Visitor::addShaderMethodBody("<<currentNode.asChar()<<","<<liqM(inputVars)<<","<<liqM(outputVars)<<")");

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
}
void Visitor::postOutput()
{
	CM_TRACE_FUNC("Visitor::postOutput()");
}
void Visitor::outputShadingGroup(const char* shadingGroupNode)
{
	CM_TRACE_FUNC("Visitor::outputShadingGroup("<<shadingGroupNode<<")");

	if( m_assembly == NULL )
	{
		Renderer* m_renderer = dynamic_cast<appleseed::Renderer*>( liquid::RendererMgr::getInstancePtr()->getRenderer() );
		assert(m_renderer != NULL );

		m_assembly = m_renderer->getAssembly().get();
		assert(m_assembly != nullptr);
	}

	createMaterial(shadingGroupNode, true);

	if( needToCreateBackfaceMaterial(shadingGroupNode) )
	{
		createMaterial(shadingGroupNode, false);
	}

}
void Visitor::createMaterial(const char* shadingGroupNode, bool front)
{
	CM_TRACE_FUNC("Visitor::createFrontfaceMaterial("<<shadingGroupNode<<","<<front<<")");

	MString cmd;

	const char *brdf_plug = front ? "liqBRDF" : "liqBRDF_back";

	MStringArray surfaceShaders;
	MStringArray liqBRDF;
	MStringArray liqEDF;
	{
		getlistConnections(shadingGroupNode, "surfaceShader",	surfaceShaders);
		getlistConnections(shadingGroupNode, brdf_plug,			liqBRDF);
		getlistConnections(shadingGroupNode, "liqEDF",			liqEDF);
	}
	if( surfaceShaders[0].length() == 0)
	{
		liquidMessage2(messageError,"surface shader not exist in \"%s\", return.", shadingGroupNode);
		return;
	}

	//cook parameters of material 
	asr::ParamArray material_params;

	MString surfaceNodeType;
	getNodeType(surfaceNodeType, surfaceShaders[0]);

	//if the surface shader is liquidShader, we add the value of liqBSDF and liqEDF to the material parameter.
	//else, the surface shader is a Maya node, and we cook the parameter differently in buildMaterialWithMayaShaderNode().
	if( surfaceNodeType == "liquidShader")
	{
		//build material with appleseed shader nodes
		material_params.insert( "surface_shader", surfaceShaders[0].asChar() );

		if( liqBRDF[0].length() != 0 ){
			material_params.insert( "bsdf", liqBRDF[0].asChar() );
		}
		if( liqEDF[0].length() != 0 ){
			material_params.insert( "edf", liqEDF[0].asChar() );
		}
	}else{
		//build material with maya shader nodes
		buildMaterialWithMayaShaderNode(material_params, surfaceShaders[0], front);
	}
	
	//create material
	const std::string material_name = front? shadingGroupNode 
									 : getBackfaceMaterial(shadingGroupNode).c_str();
	if(m_assembly->materials().get_by_name(material_name.c_str()) == nullptr)
	{
		m_assembly->materials().insert(
			asr::MaterialFactory::create(material_name.c_str(), material_params)
		);
	}
}
//
void Visitor::buildMaterialWithMayaShaderNode(asr::ParamArray& material_params, const MString& surfaceShaderNode, bool front)
{
	CM_TRACE_FUNC("Visitor::buildMaterialWithMayaShaderNode(...,"<<surfaceShaderNode.asChar()<<","<<front<<")");

	//surface shader
	std::string aoNode;
	if( hasAO(surfaceShaderNode.asChar(), aoNode) )
	{
		material_params.insert( "surface_shader", getSurfaceShaderName(aoNode.c_str()).c_str() );
	}else{
		material_params.insert( "surface_shader", getSurfaceShaderName(surfaceShaderNode.asChar()).c_str() );
	}
	
	//bsdf
	const std::string bsdf_name = front? getBSDFName(surfaceShaderNode.asChar())    //front brdf
								  :getBSDFNameBack(surfaceShaderNode.asChar());//back brdf
	material_params.insert( "bsdf", bsdf_name.c_str());

	//EDF
	if( hasEDF(surfaceShaderNode.asChar(), nullptr, nullptr, nullptr) )
	{
		material_params.insert( "edf", getEDFName(surfaceShaderNode.asChar()).c_str() );
	}

	std::string fileNode;
// 	//alpha map
// 	MVector transparency; 
// 	AlphaMapType amt = AMT_Null;
// 	if(AMT_Null != (amt=getAlphaMap(surfaceShaderNode.asChar(), &transparency.x, &transparency.y, &transparency.z, &fileNode)) )
// 	{
// 		if(AMT_Color==amt)
// 		{
// 			//It seems that the alpha color can't achieve the right effect, so I omit it.
// 			material_params.insert( "alpha_map", getTransparencyName(surfaceShaderNode.asChar()).c_str() );
// 		}else if(AMT_Texture==amt){
// 			material_params.insert( "alpha_map", getTextureInstanceName(fileNode).c_str() );
// 		}else{
// 			liquidMessage2(messageError, "\"%s\"'s alphamap type\"%d\" is unhandled",surfaceShaderNode.asChar(), amt);
// 		}
// 	}

	//normal map
	if(hasNormalMap(surfaceShaderNode.asChar(), &fileNode))
	{
		material_params.insert( "normal_map", getTextureInstanceName(fileNode).c_str() );
	}
}
bool Visitor::hasAO(const char* node, std::string &aoNode)
{	
	CM_TRACE_FUNC("Visitor::hasAO(...,"<<node<<","<<aoNode.c_str()<<")");

	bool isSurfaceShaderCreated = false;
	MString plug(MString(node) +".ambientColor");

	MStringArray nodes;
	IfMErrorWarn(MGlobal::executeCommand("listConnections -source true -plugs false \""+plug+"\"", nodes));
	
	if( nodes.length() == 0 )
		return false;

	MString srcNode(nodes[0]);
	MString srcNodeType;
	IfMErrorWarn(MGlobal::executeCommand("nodeType \""+srcNode+"\"", srcNodeType));
	if( srcNodeType == "mib_amb_occlusion" )
	{
		aoNode = srcNode.asChar();
		return true;
	}else {
		return false;
	}
}
//
bool Visitor::hasEDF(const char* node, double* outR, double* outG, double* outB)
{
	CM_TRACE_FUNC("Visitor::hasEDF("<<node<<")");

	bool ret = false;

	MStatus status;
	MObject mnode;
	getDependNodeByName(mnode, node);

	MVector incandescence;
	IfMErrorWarn(liquidGetPlugValue(mnode, "incandescence", incandescence, status));

	if( outR != nullptr ) *outR = incandescence.x;
	if( outG != nullptr ) *outG = incandescence.y;
	if( outB != nullptr ) *outB = incandescence.z;

	return !isZero(incandescence.x, incandescence.y, incandescence.z);

}
Visitor::AlphaMapType Visitor::getAlphaMap(const char* node, 
	double* outR, double* outG, double* outB, std::string *textureNode)
{
	CM_TRACE_FUNC("Visitor::getAlphaMap("<<node<<")");

	bool ret = false;

	MStringArray transparencyChannelSrcNodes;
	IfMErrorWarn(MGlobal::executeCommand("listConnections -source true -plugs false \""+MString(node)+".transparency\"", transparencyChannelSrcNodes));
	if(transparencyChannelSrcNodes.length()>0)
	{
		if( textureNode != nullptr ) 
			*textureNode = transparencyChannelSrcNodes[0].asChar();
		return AMT_Texture;
	}

	MStatus status;
	MObject mnode;
	getDependNodeByName(mnode, node);

	MVector opacity;
	IfMErrorWarn(liquidGetPlugValue(mnode, "transparency", opacity, status));

	if( outR != nullptr ) *outR = opacity.x;
	if( outG != nullptr ) *outG = opacity.y;
	if( outB != nullptr ) *outB = opacity.z;

	if( isZero(opacity.x, opacity.y, opacity.z) )
		return AMT_Null;
	else
		return AMT_Color;
}
bool Visitor::hasNormalMap(const char* node, std::string *textureNode)
{
	CM_TRACE_FUNC("Visitor::hasNormalMap("<<node<<")");

	bool ret = false;

	MStringArray bump2dNodes;
	IfMErrorWarn(MGlobal::executeCommand("listConnections -source true -plugs false \""+MString(node)+".normalCamera\"", bump2dNodes));

	if(bump2dNodes.length()==0)
		return false;

	MStringArray fileNodes;
	IfMErrorWarn(MGlobal::executeCommand("listConnections -source true -plugs false \""+bump2dNodes[0]+".bumpValue\"", fileNodes));
	
	if(fileNodes.length()==0)
		return false;
	
	if( textureNode != nullptr ) 
		*textureNode = fileNodes[0].asChar();

	return true;
}
//
}//namespace call
}//namespace appleseed
#endif//_USE_APPLESEED_