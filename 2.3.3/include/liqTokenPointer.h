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
#ifndef liqTokenPointer_H
#define liqTokenPointer_H

#include "../src/common/prerequest_std.h"
//extern "C" {
#include "liqtypes.h"//#include "ri_interface.h"//
//}

#include "liqTokenParameterType.h"

class LIQUID_EXPORT liqTokenPointer
{
  public:
    liqTokenPointer();
    liqTokenPointer(const liqTokenPointer& src);
    liqTokenPointer& operator=( const liqTokenPointer& src);
	                 operator bool() const;
    virtual ~liqTokenPointer();

    void           setTokenName( const std::string& name );
    bool           set( const std::string& name, ParameterType ptype );
    bool           set( const std::string& name, ParameterType ptype, unsigned int arraySize );
    bool           set( const std::string& name, ParameterType ptype, unsigned int arraySize, unsigned int uArraySize );
    // Deprecated: use one of the above three set() functions instead!
    bool           set( const std::string& name, ParameterType ptype, bool asArray, bool asUArray, unsigned int arraySize );
    // -----------
    int            reserve( unsigned int size );
    void           setDetailType( DetailType dType );
    void           setTokenFloat( unsigned int i, liqFloat val );
    void           setTokenFloat( unsigned int i, unsigned int uIndex, liqFloat val );
    void           setTokenFloat( unsigned int i, liqFloat x, liqFloat y , liqFloat z );
    void           setTokenFloat( unsigned int i, liqFloat x, liqFloat y , liqFloat z, liqFloat w );
	void           setTokenFloat( unsigned int i, liqFloat x1, liqFloat y1 , liqFloat z1, liqFloat w1, liqFloat x2, liqFloat y2 , liqFloat z2, liqFloat w2, liqFloat x3, liqFloat y3 , liqFloat z3, liqFloat w3, liqFloat x4, liqFloat y4 , liqFloat z4, liqFloat w4 );
	void           setTokenFloats( const boost::shared_array< liqFloat > floatVals ); // Warning! This method assumes ownership of the pointer is transferred to the TokenPointer!!!
    void           setTokenFloats( const liqFloat* floatVals ); // Use this one to copy the data
    void           setTokenString( unsigned int i, const std::string& str );
//	void           setTokenInt( unsigned int i, liqInt val );

    const std::string&						getDetailedTokenName();
    DetailType								getDetailType() const;

    const liqFloat*							getTokenFloatArray() const;
	const std::size_t						getTokenFloatArraySize()const;
	const boost::shared_array< liqFloat >	getTokenFloatSharedArray() const;
    std::string								getTokenString() const;
	std::string								getTokenName() const;

    ParameterType							getParameterType() const;

    const liqPointer							getRtPointer();
	const liqPointer							getIthRtPointer( unsigned int i );
    std::string								getRiDeclare() const;

    bool           empty() const;
    bool           isBasicST() const;
    void           resetTokenString();
    void           reset();

    typedef std::vector< liqTokenPointer > array;
  private:
	boost::shared_array< liqFloat >	m_tokenFloats;
    std::vector< std::string >		m_tokenString;
	boost::shared_array< liqString > m_tokenStringArray; // Holds pointers for getRtPointer();
    ParameterType	m_pType;
    DetailType		m_dType;
    std::string		m_tokenName;
    std::string		detailedTokenName; // This needs to be a member or else the getTokenArrays...() stuff fails as it uses c_str() on a temporary string.
    unsigned m_arraySize;
    unsigned m_uArraySize;
    unsigned m_eltSize;
    bool m_isArray;
    bool m_isUArray;
    bool m_isString;
    bool m_isFull;
    static const std::string detailType[];
    int m_stringSize;
    int m_tokenSize;
};


#endif //liquidTokenPointer_H
