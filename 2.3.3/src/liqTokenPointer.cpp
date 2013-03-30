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
#include <liqTokenPointer.h>

//#include <liquid.h>
#include <stdio.h>
#include <string>
#include <sstream>

//extern "C" {
//#include "ri_interface.h"
//}

#include <liquid.h>

#include <string>
#include <boost/shared_array.hpp>

using namespace std;
using namespace boost;

extern int debugMode;


const string liqTokenPointer::detailType[] = {
	"uniform",
	"varying",
	"vertex",
	"constant",
	"facevarying",
	"facevertex"
};


liqTokenPointer::liqTokenPointer()
{
	//CM_TRACE_FUNC("liqTokenPointer::liqTokenPointer()");

	m_pType        = rFloat;
	m_isArray      = false;
	m_isUArray     = false;
	m_isString     = false;
	m_isFull       = false;
	m_arraySize    = 0;
	m_uArraySize   = 0;
	m_eltSize      = 0;
	m_tokenSize    = 0;
	m_stringSize   = 0;
	m_dType        = rUndefined;
}

liqTokenPointer::liqTokenPointer( const liqTokenPointer &src )
{
	//CM_TRACE_FUNC("liqTokenPointer::liqTokenPointer(src)");
	LIQDEBUGPRINTF( "-> copy constructing additional ribdata: %s\n", src.m_tokenName.c_str() );

	m_isArray       = false;
	m_isUArray      = false;
	m_isString      = false;
	m_isFull        = false;
	m_arraySize     = 0;
	m_uArraySize    = 0;
	m_eltSize       = 0;
	m_tokenSize     = 0;
	m_stringSize    = 0;

	if( src.m_isUArray )
		// Moritz: this is the baddy: src.m_arraySize wasn't divided by m_uArraySize!
		//         m_uArraySize actually is only used in the set() method to calculate
		//         m_arraySize and in getRiDeclare()
		// Todo:   Find a less Italian (non-spaghetticode) solution to this.
		set( src.m_tokenName, src.m_pType, src.m_arraySize, src.m_uArraySize );
	else
		set( src.m_tokenName, src.m_pType, src.m_arraySize );
	m_dType = src.m_dType;

	if( m_pType != rString && m_pType != rShader ) {
		//setTokenFloats( src.m_tokenFloats.get() );
		m_tokenFloats = src.m_tokenFloats; // Superfast: just increase shared_array's reference counter. However, ownership is shared now! Test if this works everywhere accross Liquid.
	} else {
		m_tokenString = src.m_tokenString;
	}

	LIQDEBUGPRINTF("-> done copy constructing additional ribdata: %s\n", src.m_tokenName.c_str() );
}

liqTokenPointer & liqTokenPointer::operator=( const liqTokenPointer &src)
{
	//CM_TRACE_FUNC("liqTokenPointer::operator=(src)");
	LIQDEBUGPRINTF("-> copying additional ribdata: %s\n", src.m_tokenName.c_str() );

	reset();
	if( src.m_isUArray )
		set( src.m_tokenName, src.m_pType, src.m_arraySize, src.m_uArraySize );
	else
		set( src.m_tokenName, src.m_pType, src.m_arraySize );
	m_dType = src.m_dType;
	if( m_pType != rString && m_pType != rShader ) {
		//setTokenFloats( src.m_tokenFloats.get() );
		m_tokenFloats = src.m_tokenFloats; // Superfast: just increase shared_array's reference counter. However, ownership is shared now! Test if this works everywhere accross Liquid.
	} else {
		m_tokenString = src.m_tokenString;
	}

	LIQDEBUGPRINTF("-> done copying additional ribdata: %s\n", src.m_tokenName.c_str() );
	return *this;
}

liqTokenPointer::~liqTokenPointer()
{
	//CM_TRACE_FUNC("liqTokenPointer::~liqTokenPointer()");
	LIQDEBUGPRINTF("-> freeing additional ribdata: %s\n ", m_tokenName.c_str() );

	//if( m_tokenFloats ) { lfree( m_tokenFloats ); m_tokenFloats = NULL; }
	//resetTokenString();

};

void liqTokenPointer::reset()
{
	//CM_TRACE_FUNC("liqTokenPointer::reset()");
	//if( m_tokenFloats ) { lfree( m_tokenFloats ); m_tokenFloats = NULL; }
	m_tokenFloats.reset();
	m_tokenString.clear();
	m_isArray      = false;
	m_isUArray     = false;
	m_isString     = false;
	m_isFull       = false;
	m_arraySize    = 0;
	m_uArraySize   = 0;
	m_eltSize      = 0;
	m_tokenSize    = 0;
	m_stringSize   = 0;
	m_pType        = rFloat;
	m_tokenName.clear();
	m_dType        = rUndefined;
}

bool liqTokenPointer::set( const string& name, ParameterType ptype )
{
	//CM_TRACE_FUNC("liqTokenPointer:set("<<name<<","<<ptype<<")");
	return set( name, ptype, 0, 0 );
}

bool liqTokenPointer::set( const string& name, ParameterType ptype, unsigned int arraySize )
{
	//CM_TRACE_FUNC("liqTokenPointer:set("<<name<<","<<ptype<<","<<arrraySize<<")");
	return set( name, ptype, arraySize, 0 );
}

bool liqTokenPointer::set( const string& name, ParameterType ptype, bool asArray, bool asUArray, unsigned int arraySize )
{
	//CM_TRACE_FUNC("liqTokenPointer:set("<<name<<","<<ptype<<","<<asArray<<","<<asUArray<<","<<arrraySize<<")");
	// philippe : passing arraySize when asUArray is true fixed the float array export problem
	// TO DO : replace occurences of this function with non-obsolete ones
	//return set( name, ptype, arraySize, asUArray ? 2 : 0 );

	return set( name, ptype, asArray ? arraySize : 1, asUArray ? arraySize : 0 );
}

bool liqTokenPointer::set( const string& name, ParameterType ptype, unsigned int arraySize, unsigned int uArraySize )
{
	//CM_TRACE_FUNC("liqTokenPointer:set("<<name<<","<<ptype<<","<<arraySize<<","<<uArraySize<<")");

	setTokenName( name );
	m_pType = ptype;

	if( m_pType!=rString && m_pType!=rShader ) {

		resetTokenString();

		// define element size based on parameter type
		switch( m_pType ) {
	  case rFloat:
		  m_eltSize = 1;
		  break;
	  case rPoint:
	  case rColor:
	  case rNormal:
	  case rVector:
		  m_eltSize = 3;
		  break;
	  case rHpoint:
		  m_eltSize = 4;
		  break;
	  case rString: // Useless but prevent warning at compile time
	  case rShader: // Useless but prevent warning at compile time
		  m_eltSize = 0;
		  break;
	  case rMatrix:
		  m_eltSize = 16;
		  break;
		}

		// check how much we need if we have an array
		m_isArray = arraySize != 0;
		unsigned neededSize;
		if( m_isArray ) {
			m_arraySize  = arraySize;
			m_isUArray   = bool( uArraySize );
			m_uArraySize = uArraySize;
			neededSize = m_arraySize * m_eltSize;
			if( m_isUArray )
				neededSize *= uArraySize;
		} else {
			m_arraySize = 0;
			neededSize = m_eltSize;
		}
		// allocate whatever we need
		// Check if we already got enough space
		if( m_tokenSize < neededSize ) {
			LIQDEBUGPRINTF("-> allocating memory 1 for: %s\n", name.c_str() );
			m_tokenFloats = shared_array< liqFloat >( new liqFloat[ neededSize ] );
			if( ! m_tokenFloats ) {
				m_tokenSize = 0;
				cerr << "Error : liqTokenPointer out of memory for " << neededSize << " bytes" << endl;
				return false;
			}
			m_tokenSize = neededSize;
		} else if( neededSize ) {
			LIQDEBUGPRINTF("-> allocating memory 2 for: %s\n", name.c_str() );
			m_tokenFloats = shared_array< liqFloat >( new liqFloat[ neededSize ] );
			if( ! m_tokenFloats ) {
				cerr << "Error : liqTokenPointer out of memory for " << neededSize << " bytes" << endl;
				return false;
			}
			m_tokenSize = neededSize;
		}
		LIQDEBUGPRINTF( "-> array size: %d\n", arraySize );
		LIQDEBUGPRINTF( "-> needed %u got %d\n", neededSize, m_tokenSize );
	} else {
		// STRINGS ARE A SPECIAL CASE
		// Space is now allocated upfront

		m_tokenFloats.reset();
		resetTokenString();

		m_isUArray = false;
		m_isArray  = arraySize != 0;

		if ( m_isArray ) {

			// init array size
			m_arraySize = arraySize;
			m_tokenSize = 0;
			// init pointer array
			m_tokenString.resize( m_arraySize );

		} else {
			m_arraySize   = 0; // useless
			m_tokenSize   = 0;
			m_tokenString.resize( 1 );
		}
	}
	return true;
}

int liqTokenPointer::reserve( unsigned int size )
{
	//CM_TRACE_FUNC("liqTokenPointer:reserve("<<size<<")");

	if( m_pType != rString && m_pType != rShader ) {

		if( m_arraySize != size )
		{
			// Only augment allocated memory if needed, do not reduce it
			unsigned long neededSize( size * m_eltSize );
			if( m_isUArray )
				neededSize *= m_uArraySize;
			if( m_tokenSize < neededSize ) {
				shared_array< liqFloat > tmp( new liqFloat[ neededSize ] );
				memcpy( tmp.get(), m_tokenFloats.get(), m_tokenSize );
				m_tokenFloats = tmp;
				m_tokenSize = neededSize;
			}
		}
	} else {
		m_tokenString.resize( size );
	}
	m_arraySize = size;
	return m_arraySize;
}

void liqTokenPointer::setDetailType( DetailType dType )
{
	//CM_TRACE_FUNC("liqTokenPointer:setDetailType("<<dType<<")");

	m_dType = dType;
}

DetailType liqTokenPointer::getDetailType() const
{
	//CM_TRACE_FUNC("liqTokenPointer:getDetailType()");

	return m_dType;
}

ParameterType liqTokenPointer::getParameterType() const
{
	//CM_TRACE_FUNC("liqTokenPointer:getParameterType()");
	return m_pType;
}

void liqTokenPointer::setTokenFloat( unsigned int i, liqFloat val )
{
	//CM_TRACE_FUNC("liqTokenPointer:setTokenFloat(<<i<<","<<val<<)");
	assert( m_tokenSize > i );
	m_tokenFloats[i] = val;
}

void liqTokenPointer::setTokenFloat( unsigned int i, unsigned int uIndex, liqFloat val )
{
	//CM_TRACE_FUNC("liqTokenPointer:setTokenFloat(<<i<<","<<uIndex<<","<<val<<)");
	assert( m_tokenSize > ( i * m_uArraySize + uIndex ) );
	setTokenFloat( i * m_uArraySize + uIndex, val );
}

void liqTokenPointer::setTokenFloat( unsigned int i, liqFloat x, liqFloat y , liqFloat z )
{
	//CM_TRACE_FUNC("liqTokenPointer:setTokenFloat(<<i<<","<<x<<","<<y<<","<<z<<")");
	assert( m_tokenSize > ( m_eltSize * i + 2 ) );
	m_tokenFloats[ m_eltSize * i + 0 ] = x;
	m_tokenFloats[ m_eltSize * i + 1 ] = y;
	m_tokenFloats[ m_eltSize * i + 2 ] = z;
}



void liqTokenPointer::setTokenFloats( const liqFloat* vals )
{
	//CM_TRACE_FUNC("liqTokenPointer:setTokenFloats(const liqFloat* vals)");
	LIQDEBUGPRINTF( "-> copying data\n" );
	if( m_isArray || m_isUArray ) {
		//m_tokenFloats = shared_array< liqFloat >( new liqFloat[ m_tokenSize ] );
		memcpy( m_tokenFloats.get(), vals, m_tokenSize * sizeof( liqFloat) );
	} else {
		//m_tokenFloats = shared_array< liqFloat >( new liqFloat[ m_eltSize ] );
		memcpy( m_tokenFloats.get(), vals, m_eltSize * sizeof( liqFloat) );
	}
}

void liqTokenPointer::setTokenFloats( const shared_array< liqFloat > vals )
{
	//CM_TRACE_FUNC("liqTokenPointer:setTokenFloats(const shared_array< liqFloat > vals)");

	m_tokenFloats = vals;
}

const liqFloat* liqTokenPointer::getTokenFloatArray() const
{
	//CM_TRACE_FUNC("liqTokenPointer:getTokenFloatArray()");

	return m_tokenFloats.get();
}

const shared_array< liqFloat > liqTokenPointer::getTokenFloatSharedArray() const
{
	//CM_TRACE_FUNC("liqTokenPointer:getTokenFloatSharedArray()");

	return m_tokenFloats;
}

void liqTokenPointer::setTokenFloat( unsigned int i, liqFloat x, liqFloat y , liqFloat z, liqFloat w )
{
	//CM_TRACE_FUNC("liqTokenPointer:setTokenFloat(<<i<<","<<x<<","<<y<<","<<z<<","<<w<<")");

	assert( m_tokenSize > ( m_eltSize * i + 3 ) );
	m_tokenFloats[ m_eltSize * i + 0 ] = x;
	m_tokenFloats[ m_eltSize * i + 1 ] = y;
	m_tokenFloats[ m_eltSize * i + 2 ] = z;
	m_tokenFloats[ m_eltSize * i + 3 ] = w;
}

void liqTokenPointer::setTokenFloat( unsigned int i, liqFloat x1, liqFloat y1 , liqFloat z1, liqFloat w1, liqFloat x2, liqFloat y2 , liqFloat z2, liqFloat w2, liqFloat x3, liqFloat y3 , liqFloat z3, liqFloat w3, liqFloat x4, liqFloat y4 , liqFloat z4, liqFloat w4 )
{
	//CM_TRACE_FUNC("liqTokenPointer:setTokenFloat(<<i<<","<<x1<<","<<y1<<","<<z1<<"...)");
	assert( m_tokenSize > ( m_eltSize * i + 15 ) );
	m_tokenFloats[ m_eltSize * i + 0 ] = x1;
	m_tokenFloats[ m_eltSize * i + 1 ] = y1;
	m_tokenFloats[ m_eltSize * i + 2 ] = z1;
	m_tokenFloats[ m_eltSize * i + 3 ] = w1;

	m_tokenFloats[ m_eltSize * i + 4 ] = x2;
	m_tokenFloats[ m_eltSize * i + 5 ] = y2;
	m_tokenFloats[ m_eltSize * i + 6 ] = z2;
	m_tokenFloats[ m_eltSize * i + 7 ] = w2;

	m_tokenFloats[ m_eltSize * i + 8 ] = x3;
	m_tokenFloats[ m_eltSize * i + 9 ] = y3;
	m_tokenFloats[ m_eltSize * i + 10 ] = z3;
	m_tokenFloats[ m_eltSize * i + 11 ] = w3;

	m_tokenFloats[ m_eltSize * i + 12 ] = x4;
	m_tokenFloats[ m_eltSize * i + 13 ] = y4;
	m_tokenFloats[ m_eltSize * i + 14 ] = z4;
	m_tokenFloats[ m_eltSize * i + 15 ] = w4;
}
string liqTokenPointer::getTokenString() const
{
	//CM_TRACE_FUNC("liqTokenPointer:getTokenString()");

	return m_tokenString[0];
}

void liqTokenPointer::setTokenString( unsigned int i, const string& str )
{
	//CM_TRACE_FUNC("liqTokenPointer:setTokenString("<<i<<","<<str<<")");

	if( m_isArray ){//as for array, the array allocate memroy first, then fill the data. so 'i' will less than 'm_arraySize'.
	}else{
		assert( i >= m_arraySize );
	}

	m_tokenString[ i ] = str;
}

// void liqTokenPointer::setTokenInt( unsigned int i, liqInt val )
// {
// 	//CM_TRACE_FUNC("liqTokenPointer:setTokenInt(<<i<<","<<val<<")");
// 
// 	assert( m_tokenSize > i );
// 	m_tokenFloats[i] = val;
// }

void liqTokenPointer::resetTokenString()
{
	//CM_TRACE_FUNC("liqTokenPointer:resetTokenString()");

	m_tokenString.clear();
	m_tokenStringArray.reset();
}

void liqTokenPointer::setTokenName( const string& name )
{
	//CM_TRACE_FUNC("liqTokenPointer:setTokenName("<<name<<")");
	m_tokenName = name;
}

string liqTokenPointer::getTokenName() const
{
	//CM_TRACE_FUNC("liqTokenPointer:getTokenName()");
	// Hmmmm should we handle token without name ?
	return m_tokenName;
}

const string& liqTokenPointer::getDetailedTokenName()
{
	//CM_TRACE_FUNC("liqTokenPointer:getDetailedTokenName()");
	// Hmmmm should we handle token without name ?
#ifdef PRMAN
	// Philippe : in PRMAN, declaring P as a vertex point is not necessary and it make riCurves generation fail.
	// so when the token is P, we just skip the type declaration.
	if ( "P" != m_tokenName )
	{
		detailedTokenName = getRiDeclare() + " ";
	}
	else
		detailedTokenName = "";
#else
	detailedTokenName = getRiDeclare() + " ";
#endif
	detailedTokenName += m_tokenName;
	return detailedTokenName;
}

const liqPointer liqTokenPointer::getRtPointer()
{
	//CM_TRACE_FUNC("liqTokenPointer:getRtPointer()");
	if( m_pType == rString || m_pType == rShader ) {
		m_tokenStringArray = shared_array< liqString >( new liqString[ m_tokenString.size() ] );

		for( unsigned i( 0 ); i < m_tokenString.size(); i++ ) {
			m_tokenStringArray[ i ] = const_cast< liqString >( m_tokenString[ i ].c_str() );
		}
		return ( liqPointer )m_tokenStringArray.get();
	} else {
		return ( liqPointer )m_tokenFloats.get();
	}
}


// Return a liqPointer for a token corresponding to the ith element of a primitive
const liqPointer liqTokenPointer::getIthRtPointer( unsigned int i )
{
	//CM_TRACE_FUNC("liqTokenPointer:getIthRtPointer("<<i<<")");
	if( m_pType == rString || m_pType == rShader ) {
		assert( m_arraySize > i );
		m_tokenStringArray = shared_array< liqString >( new liqString[ 1 ] );
		m_tokenStringArray[ 0 ] = const_cast< liqString >( m_tokenString[ i ].c_str() );
		return ( liqPointer )m_tokenStringArray.get();
	} else {
		if( m_isArray || m_isUArray ) {
			assert( m_tokenSize > ( m_eltSize * i ) );
			return ( liqPointer ) ( m_tokenFloats.get() + m_eltSize * i);
		} else {
			return ( liqPointer )m_tokenFloats.get(); // uniform ...
		}
	}
}

string liqTokenPointer::getRiDeclare() const
{
	//CM_TRACE_FUNC("liqTokenPointer:getRiDeclare()");
	string type;
	switch ( m_pType )
	{
	case rString:
		type = "string";
		break;
	case rShader:
		type = "shader";
		break;
	case rMatrix:
		type = "matrix";
		break;
	case rFloat:
		type = "float";
		break;
	case rHpoint:
		type = "hpoint";
		break;
	case rPoint:
		type = "point";
		break;
	case rVector:
		type = "vector";
		break;
	case rNormal:
		type = "normal";
		break;
	case rColor:
		type = "color";
		break;
	}
	LIQDEBUGPRINTF("liqTokenPointer :: TYPE=%s _ m_isUArray=%d _ m_isArray=%d _ m_uArraySize=%d _ m_arraySize=%d \n", type.c_str(), m_isUArray, m_isArray, m_uArraySize, m_arraySize);
	if( m_pType==rString || m_pType==rShader )
	{
		if( m_isArray )
		{
			stringstream declare;
			declare << "[" << m_arraySize << "]";
			type += declare.str();
		}
	}
	else
	{
		if( m_isUArray )
		{	
			stringstream declare;
			declare << "[" << m_uArraySize << "]";
			type += declare.str();
		}
	}
	if( rUndefined != m_dType )
	{
		type = detailType[ m_dType ] + " " + type;
	}
	return type;
}

liqTokenPointer::operator bool() const
{
	//CM_TRACE_FUNC("liqTokenPointer:operator bool()");
	return m_tokenFloats;
}

bool liqTokenPointer::empty() const
{
	//CM_TRACE_FUNC("liqTokenPointer:empty()");
	return !m_tokenFloats;
}

bool liqTokenPointer::isBasicST() const
{
	//CM_TRACE_FUNC("liqTokenPointer:isBasicST()");
	// Not st or, if it is, face varying
	if( m_tokenName[0] != 's' || m_tokenName[1] != 't' || m_tokenName.length() != 2 || m_dType == rFaceVarying || m_dType == rFaceVertex ) {
		return false;
	}
	return true;
}

