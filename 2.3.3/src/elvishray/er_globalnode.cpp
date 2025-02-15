#include "er_globalnode.h"

#include <liquid.h>
#include <liqCreatePlug.h>
#include "er_.h"
#include "er_nodeId.h"

namespace elvishray
{
//dev
MObject GlobalNode::aoutput_call;
MObject GlobalNode::aoutput_esa;
MObject GlobalNode::aoutput_ess;
//verose&link
MObject GlobalNode::averbose;
MObject GlobalNode::alink_eiIMG;
MObject GlobalNode::alink_eiSHADER;
MObject GlobalNode::alink_eiSHADER_maya;
MObject GlobalNode::aotherlinks;
//options
MObject GlobalNode::acontrast;
MObject GlobalNode::asamples_min;
MObject GlobalNode::asamples_max;
MObject GlobalNode::afilterType;
MObject GlobalNode::afilterSize;
MObject GlobalNode::aprogressive;
//motion
//trace_depth
MObject GlobalNode::atrace_depth_transp;
MObject GlobalNode::atrace_depth_glossy_reflect;
MObject GlobalNode::atrace_depth_diffuse_reflect;
MObject GlobalNode::atrace_depth_glossy_refract;
MObject GlobalNode::atrace_depth_diffuse_refract;
MObject GlobalNode::atrace_depth_sum;

MObject GlobalNode::adisplace;
MObject GlobalNode::amax_displace;
MObject GlobalNode::aface;
//approx
MObject GlobalNode::aapprox_method;
MObject GlobalNode::aapprox_any;
MObject GlobalNode::aapprox_view_dep;
MObject GlobalNode::aapprox_regular_usubdiv;
MObject GlobalNode::aapprox_regular_vsubdiv;
MObject GlobalNode::aapprox_length_edgelength;
//MObject GlobalNode::aapprox_args0;
//MObject GlobalNode::aapprox_args1;
//MObject GlobalNode::aapprox_args2;
//MObject GlobalNode::aapprox_args3;
MObject GlobalNode::aapprox_sharp;
MObject GlobalNode::aapprox_min_subdiv;
MObject GlobalNode::aapprox_max_subdiv;
MObject GlobalNode::aapprox_max_grid_size;
MObject GlobalNode::aapprox_motion_factor;

#define LIQ_GLOBALS_HIDE_ATTRIBUTES false

#define CREATE_BOOL(attr, obj, name, shortName, default)    \
    obj = attr.create( name, shortName, MFnNumericData::kBoolean, default, &status); \
    CHECK_MSTATUS(attr.setKeyable(true));     \
    CHECK_MSTATUS(attr.setStorable(true));    \
    CHECK_MSTATUS(attr.setReadable(true));    \
    CHECK_MSTATUS(attr.setWritable(true));    \
    CHECK_MSTATUS(attr.setHidden(LIQ_GLOBALS_HIDE_ATTRIBUTES));      \
    CHECK_MSTATUS(addAttribute(obj));

#define CREATE_MULTI_BOOL(attr, obj, name, shortName, default)    \
    obj = attr.create( name, shortName, MFnNumericData::kBoolean, default, &status); \
    CHECK_MSTATUS(attr.setArray(true));       \
    CHECK_MSTATUS(attr.setKeyable(true));     \
    CHECK_MSTATUS(attr.setStorable(true));    \
    CHECK_MSTATUS(attr.setReadable(true));    \
    CHECK_MSTATUS(attr.setWritable(true));    \
    CHECK_MSTATUS(attr.setHidden(LIQ_GLOBALS_HIDE_ATTRIBUTES));      \
    CHECK_MSTATUS(addAttribute(obj));

#define CREATE_INT(attr, obj, name, shortName, default)    \
    obj = attr.create( name, shortName, MFnNumericData::kInt, default, &status); \
    CHECK_MSTATUS(attr.setKeyable(true));     \
    CHECK_MSTATUS(attr.setStorable(true));    \
    CHECK_MSTATUS(attr.setReadable(true));    \
    CHECK_MSTATUS(attr.setWritable(true));    \
    CHECK_MSTATUS(attr.setHidden(LIQ_GLOBALS_HIDE_ATTRIBUTES));      \
    CHECK_MSTATUS(addAttribute(obj));

#define CREATE_MULTI_INT(attr, obj, name, shortName, default)    \
    obj = attr.create( name, shortName, MFnNumericData::kInt, default, &status); \
    CHECK_MSTATUS(attr.setArray(true));       \
    CHECK_MSTATUS(attr.setKeyable(true));     \
    CHECK_MSTATUS(attr.setStorable(true));    \
    CHECK_MSTATUS(attr.setReadable(true));    \
    CHECK_MSTATUS(attr.setWritable(true));    \
    CHECK_MSTATUS(attr.setHidden(LIQ_GLOBALS_HIDE_ATTRIBUTES));      \
    CHECK_MSTATUS(addAttribute(obj));

#define CREATE_LONG(attr, obj, name, shortName, default)    \
    obj = attr.create( name, shortName, MFnNumericData::kLong, default, &status); \
    CHECK_MSTATUS(attr.setKeyable(true));     \
    CHECK_MSTATUS(attr.setStorable(true));    \
    CHECK_MSTATUS(attr.setReadable(true));    \
    CHECK_MSTATUS(attr.setWritable(true));    \
    CHECK_MSTATUS(attr.setHidden(LIQ_GLOBALS_HIDE_ATTRIBUTES));      \
    CHECK_MSTATUS(addAttribute(obj));

#define CREATE_MULTI_LONG(attr, obj, name, shortName, default)    \
    obj = attr.create( name, shortName, MFnNumericData::kLong, default, &status); \
    CHECK_MSTATUS(attr.setArray(true));       \
    CHECK_MSTATUS(attr.setKeyable(true));     \
    CHECK_MSTATUS(attr.setStorable(true));    \
    CHECK_MSTATUS(attr.setReadable(true));    \
    CHECK_MSTATUS(attr.setWritable(true));    \
    CHECK_MSTATUS(attr.setHidden(LIQ_GLOBALS_HIDE_ATTRIBUTES));      \
    CHECK_MSTATUS(addAttribute(obj));

#define CREATE_FLOAT(attr, obj, name, shortName, default)    \
    obj = attr.create( name, shortName, MFnNumericData::kFloat, default, &status); \
    CHECK_MSTATUS(attr.setKeyable(true));     \
    CHECK_MSTATUS(attr.setStorable(true));    \
    CHECK_MSTATUS(attr.setReadable(true));    \
    CHECK_MSTATUS(attr.setWritable(true));    \
    CHECK_MSTATUS(attr.setHidden(LIQ_GLOBALS_HIDE_ATTRIBUTES));      \
    CHECK_MSTATUS(addAttribute(obj));

#define CREATE_MULTI_FLOAT(attr, obj, name, shortName, default)    \
    obj = attr.create( name, shortName, MFnNumericData::kFloat, default, &status); \
    CHECK_MSTATUS(attr.setArray(true));       \
    CHECK_MSTATUS(attr.setKeyable(true));     \
    CHECK_MSTATUS(attr.setStorable(true));    \
    CHECK_MSTATUS(attr.setReadable(true));    \
    CHECK_MSTATUS(attr.setWritable(true));    \
    CHECK_MSTATUS(attr.setHidden(LIQ_GLOBALS_HIDE_ATTRIBUTES));      \
    CHECK_MSTATUS(addAttribute(obj));

#define CREATE_STRING(attr, obj, name, shortName, default)    \
    obj = attr.create( name, shortName, MFnData::kString, obj, &status); \
    CHECK_MSTATUS(attr.setKeyable(true));     \
    CHECK_MSTATUS(attr.setStorable(true));    \
    CHECK_MSTATUS(attr.setReadable(true));    \
    CHECK_MSTATUS(attr.setWritable(true));    \
    CHECK_MSTATUS(attr.setHidden(LIQ_GLOBALS_HIDE_ATTRIBUTES));     \
	CHECK_MSTATUS(attr.setDefault( stringData.create( MString( default ), &sstat ) ) ); \
	CHECK_MSTATUS( sstat );					  \
    CHECK_MSTATUS(addAttribute(obj));

#define CREATE_MULTI_STRING(attr, obj, name, shortName, default)    \
	obj = attr.create( name, shortName, MFnData::kString, obj, &status); \
	CHECK_MSTATUS(attr.setArray(true));       \
	CHECK_MSTATUS(attr.setKeyable(true));     \
	CHECK_MSTATUS(attr.setStorable(true));    \
	CHECK_MSTATUS(attr.setReadable(true));    \
	CHECK_MSTATUS(attr.setWritable(true));    \
	CHECK_MSTATUS(attr.setHidden(LIQ_GLOBALS_HIDE_ATTRIBUTES));      \
	CHECK_MSTATUS(addAttribute(obj));

#define CREATE_COMP(attr, obj, name, shortName)    \
    obj = attr.create( name, shortName, &status); \
    CHECK_MSTATUS(attr.setStorable(true));    \
    CHECK_MSTATUS(attr.setReadable(true));    \
    CHECK_MSTATUS(attr.setWritable(true));    \
    CHECK_MSTATUS(attr.setHidden(LIQ_GLOBALS_HIDE_ATTRIBUTES));      \
    CHECK_MSTATUS(addAttribute(obj));

#define CREATE_MULTI_STR_ARRAY(attr, obj, name, shortName)    \
    obj = attr.create( name, shortName, MFnData::kStringArray, obj, &status); \
    CHECK_MSTATUS(attr.setArray(true));       \
    CHECK_MSTATUS(attr.setKeyable(true));     \
    CHECK_MSTATUS(attr.setStorable(true));    \
    CHECK_MSTATUS(attr.setReadable(true));    \
    CHECK_MSTATUS(attr.setWritable(true));    \
    CHECK_MSTATUS(attr.setHidden(LIQ_GLOBALS_HIDE_ATTRIBUTES));      \
    CHECK_MSTATUS(addAttribute(obj));

#define CREATE_MULTI_INT_ARRAY(attr, obj, name, shortName)    \
    obj = attr.create( name, shortName, MFnData::kIntArray, obj, &status); \
    CHECK_MSTATUS(attr.setArray(true));       \
    CHECK_MSTATUS(attr.setKeyable(true));     \
    CHECK_MSTATUS(attr.setStorable(true));    \
    CHECK_MSTATUS(attr.setReadable(true));    \
    CHECK_MSTATUS(attr.setWritable(true));    \
    CHECK_MSTATUS(attr.setHidden(LIQ_GLOBALS_HIDE_ATTRIBUTES));      \
    CHECK_MSTATUS(addAttribute(obj));

#define CREATE_COLOR( attr, obj, name, shortName, default1, default2, default3 )    \
	obj = attr.createColor( name, shortName, &status ); \
	CHECK_MSTATUS(attr.setDefault( default1, default2, default3 )); \
	CHECK_MSTATUS(attr.setKeyable(true));     \
	CHECK_MSTATUS(attr.setStorable(true));    \
	CHECK_MSTATUS(attr.setReadable(true));    \
	CHECK_MSTATUS(attr.setWritable(true));    \
	CHECK_MSTATUS(attr.setHidden(LIQ_GLOBALS_HIDE_ATTRIBUTES));      \
	CHECK_MSTATUS(addAttribute(obj));

#define CREATE_ENUM(attr, obj, name, shortName, default, values, N)    \
    obj = attr.create( name, shortName, default, &status);  \
	for(int i=0; i<N; ++i){									\
		CHECK_MSTATUS(attr.addField( values[i], i ));\
	}														\
	CHECK_MSTATUS(attr.setKeyable(true));					\
	CHECK_MSTATUS(attr.setStorable(true));					\
	CHECK_MSTATUS(attr.setReadable(true));					\
	CHECK_MSTATUS(attr.setWritable(true));					\
	CHECK_MSTATUS(attr.setHidden(LIQ_GLOBALS_HIDE_ATTRIBUTES));\
	CHECK_MSTATUS(addAttribute( obj ));	


GlobalNode::GlobalNode()
{
}

GlobalNode::~GlobalNode()
{
}
const MTypeId GlobalNode::getTypeId()
{ 
	return TI_GLOBAL_NODE;
}
const MString GlobalNode::getTypeName()
{ 
	return (RENDER_NAME+"Globals");
}

void* GlobalNode::creator()
{
    return new GlobalNode();
}

MStatus GlobalNode::initialize()
{
	MFnTypedAttribute     tAttr;
	MFnNumericAttribute   nAttr;
	MFnCompoundAttribute  cAttr;
	MFnEnumAttribute    eAttr;
	MStatus status, sstat;

	MFnStringData stringData;

	//dev
	CREATE_BOOL( nAttr,  aoutput_call,	"output_call",	"cal",    true  );
	CREATE_BOOL( nAttr,  aoutput_esa,	"output_esa",	"esa",    false );
	CREATE_BOOL( nAttr,  aoutput_ess,	"output_ess",	"ess",    false );
	//verose&link
	CREATE_ENUM_BEG(eAttr, averbose,	"verbose",		"vrb", 6);
		eAttr.addField("None",	0);
		eAttr.addField("Fatal",	1);
		eAttr.addField("Error",	2);
		eAttr.addField("Warning",3);
		eAttr.addField("Info",	4);
		eAttr.addField("Debug",	5);
		eAttr.addField("All",	6);
	CREATE_ENUM_END(eAttr, averbose);

	CREATE_BOOL( nAttr,  alink_eiIMG,			"eiIMG",		"li",	true );
	CREATE_BOOL( nAttr,  alink_eiSHADER,		"eiSHADER",		"ls",	true );
	CREATE_BOOL( nAttr,  alink_eiSHADER_maya,	"eiSHADER_maya","lsm",	true );
	
	//CREATE_MULTI_STRING(tAttr, alinks,		"eiOtherLinks",	"lnk",	"");
	aotherlinks = tAttr.create( "shader_plugin"/*"eiOtherLinks"*/,	"sp"/*lnk*/, MFnData::kString, aotherlinks, &status);
		CHECK_MSTATUS(tAttr.setArray(true)); 
		CHECK_MSTATUS(tAttr.setKeyable(true));
		CHECK_MSTATUS(tAttr.setStorable(true));
		CHECK_MSTATUS(tAttr.setReadable(true));
		CHECK_MSTATUS(tAttr.setWritable(true));
		CHECK_MSTATUS(tAttr.setHidden(LIQ_GLOBALS_HIDE_ATTRIBUTES));
		CHECK_MSTATUS(addAttribute(aotherlinks));

	//options
	CREATE_FLOAT( nAttr,	acontrast,	"contrast",	"ctr",	0.05f);
	CREATE_INT( nAttr,	asamples_min,	"samples_min",	"smn",	0);
	CREATE_INT( nAttr,	asamples_max,	"samples_max",	"smx",	16);
	CREATE_ENUM_BEG(eAttr, afilterType,	"filterType",	"ft",	4);
		eAttr.addField("None",		0);
		eAttr.addField("Box",		1);
		eAttr.addField("Triangle",	2);
		eAttr.addField("Catmull",	3);
		eAttr.addField("Gaussian",	4);
		eAttr.addField("Sinc",		5);
	CREATE_ENUM_END(eAttr, afilterType);
	CREATE_FLOAT( nAttr,afilterSize,	"filterSize",	"fs",	3.0f);
	CREATE_ENUM_BEG(eAttr, aface,	"face",		"fac", 3);
		eAttr.addField("None",	0);
		eAttr.addField("Front",	1);
		eAttr.addField("Back",	2);
		eAttr.addField("Both",	3);
	CREATE_ENUM_END(eAttr, aface);
	CREATE_BOOL( nAttr,  aprogressive,	"progressive",	"prg",	true );

	////motion
	////trace_depth
	CREATE_INT( nAttr,	atrace_depth_transp,			"trace_depth_transp",			"trn",	4);
	CREATE_INT( nAttr,	atrace_depth_glossy_reflect,	"trace_depth_glossy_reflect",	"grl",	4);
	CREATE_INT( nAttr,	atrace_depth_diffuse_reflect,	"trace_depth_diffuse_reflect",	"drl",	4);
	CREATE_INT( nAttr,	atrace_depth_glossy_refract,	"trace_depth_glossy_refract",	"grr",	4);
	CREATE_INT( nAttr,	atrace_depth_diffuse_refract,	"trace_depth_diffuse_refract",	"drr",	4);
	CREATE_INT( nAttr,	atrace_depth_sum,				"trace_depth_sum",				"sum",	4);

	CREATE_BOOL( nAttr,  adisplace,			"displace",			"dis",	false);
	CREATE_FLOAT( nAttr, amax_displace,		"max_displace",		"mdi",	false);

	////approx
	const char* const values[3]={"None", "Regular", "Length"};
	CREATE_ENUM(  eAttr,	aapprox_method,		"approx_method",	"amd",	1, values, 3);
	CREATE_BOOL(  nAttr,	aapprox_any,		"approx_any",	"aay",	0);
	CREATE_BOOL(  nAttr,	aapprox_view_dep,	"approx_view_dep",	"avd",	0);
	CREATE_INT(  nAttr,	aapprox_regular_usubdiv,		"approx_regular_usubdiv",	"usd",	0);
	CREATE_INT(  nAttr,	aapprox_regular_vsubdiv,		"approx_regular_vsubdiv",	"vsd",	0);
	CREATE_FLOAT(  nAttr,	aapprox_length_edgelength,	"approx_length_edgelength",	"eln",	0.0f);
	//MObject GlobalNode::aapprox_args;
	//CREATE_INT(  nAttr,	aapprox_args0,		"approx_args0",	"ag0",	0);
	//CREATE_INT(  nAttr,	aapprox_args1,		"approx_args1",	"ag1",	0);
	//CREATE_INT(  nAttr,	aapprox_args2,		"approx_args2",	"ag2",	0);
	//CREATE_INT(  nAttr,	aapprox_args3,		"approx_args3",	"ag3",	0);
	CREATE_FLOAT( nAttr, aapprox_sharp,		"approx_sharp",		"ash",	0.0f);
	CREATE_INT(  nAttr,	aapprox_min_subdiv,		"approx_min_subdiv",	"ais",	0);
	CREATE_INT(  nAttr,	aapprox_max_subdiv,		"approx_max_subdiv",	"aas",	5);
	CREATE_INT(  nAttr,	aapprox_max_grid_size,	"approx_max_grid_size",	"aag",	65536);
	CREATE_FLOAT( nAttr, aapprox_motion_factor,	"approx_motion_factor",	"amf",	16.0f);


	return MS::kSuccess;
}
}//namespace elvishray




