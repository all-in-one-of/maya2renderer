#ifndef _LIQ_CREATE_PLUG_H_
#define _LIQ_CREATE_PLUG_H_

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

#endif//_LIQ_CREATE_PLUG_H_