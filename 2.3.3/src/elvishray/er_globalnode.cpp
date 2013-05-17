#include "er_globalnode.h"

#include <liquid.h>
#include "er_nodeId.h"

namespace elvishray
{
const MTypeId GlobalNode::typeId(TI_GLOBAL_NODE);
const MString GlobalNode::typeName(TN_GLOBAL_NODE);
//dev
MObject GlobalNode::aoutput_call;
MObject GlobalNode::aoutput_esa;
MObject GlobalNode::aoutput_ess;
//verose&link
MObject GlobalNode::averbose;
MObject GlobalNode::alink_eiIMG;
MObject GlobalNode::alink_eiSHADER;
MObject GlobalNode::alink_eiSHADER_maya;
//options
MObject GlobalNode::acontrast;
MObject GlobalNode::asamples;
MObject GlobalNode::afilterType;
MObject GlobalNode::afilterSize;
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
MObject GlobalNode::aapprox_args;
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


GlobalNode::GlobalNode()
{
}

GlobalNode::~GlobalNode()
{
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
	MStatus status, sstat;

	MFnStringData stringData;

	//dev
	CREATE_BOOL( nAttr,  aoutput_call,	"output_call",	"cal",    true  );
	CREATE_BOOL( nAttr,  aoutput_esa,	"output_esa",	"esa",    false );
	CREATE_BOOL( nAttr,  aoutput_ess,	"output_ess",	"ess",    false );
	//verose&link
	CREATE_INT(  nAttr,  averbose,				"verbose",		"vrb",	6);
	CREATE_BOOL( nAttr,  alink_eiIMG,			"eiIMG",		"li",	true );
	CREATE_BOOL( nAttr,  alink_eiSHADER,		"eiSHADER",		"ls",	true );
	CREATE_BOOL( nAttr,  alink_eiSHADER_maya,	"eiSHADER_maya","lsm",	true );

	//MObject GlobalNode::alink;
	////options
	//MObject GlobalNode::acontrast;
	//MObject GlobalNode::asamples;
	//MObject GlobalNode::afilterType;
	//MObject GlobalNode::afilterSize;
	////motion
	////trace_depth
	//MObject GlobalNode::atrace_depth_transp;
	//MObject GlobalNode::atrace_depth_glossy_reflect;
	//MObject GlobalNode::atrace_depth_diffuse_reflect;
	//MObject GlobalNode::atrace_depth_glossy_refract;
	//MObject GlobalNode::atrace_depth_diffuse_refract;
	//MObject GlobalNode::atrace_depth_sum;

	//MObject GlobalNode::adisplace;
	//MObject GlobalNode::amax_displace;
	//MObject GlobalNode::aface;
	////approx
	//MObject GlobalNode::aapprox_method;
	//MObject GlobalNode::aapprox_any;
	//MObject GlobalNode::aapprox_view_dep;
	//MObject GlobalNode::aapprox_args;
	//MObject GlobalNode::aapprox_sharp;
	//MObject GlobalNode::aapprox_min_subdiv;
	//MObject GlobalNode::aapprox_max_subdiv;
	//MObject GlobalNode::aapprox_max_grid_size;
	//MObject GlobalNode::aapprox_motion_factor;


	return MS::kSuccess;
}
}//namespace elvishray




