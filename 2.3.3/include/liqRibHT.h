/*
**
** The contents of this file are subject to the Mozilla Public License Version
** 1.1 (the "License"); you may not use this file except in compliance with
** the License. You may obtain a copy of the License at
** http://www.mozilla.org/MPL/
**
** Software distributed under the License is distributed on an "AS IS" basis,
** WITHOUT WARRANTY OF ANY KIND, either express or implied. See the License
** for the specific language governing rights and limitations under the
** License.
**
** The Original Code is the Liquid Rendering Toolkit.
**
** The Initial Developer of the Original Code is Colin Doncaster. Portions
** created by Colin Doncaster are Copyright (C) 2002. All Rights Reserved.
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
**
*/

#ifndef liqRibHT_H
#define liqRibHT_H

/* ______________________________________________________________________
**
** Liquid Rib Hash Table Header File
** ______________________________________________________________________
*/

#include <common/prerequest_std.h>
#include <liqtypes.h>
#include <liqRibNode.h>
#include <liqRefactoring.h>

	typedef std::multimap< ulong, liqRibNodePtr > RNMAP;
	typedef std::vector< MString > str_Vector;
	typedef std::vector< ObjectType > type_Vector;

class LIQUID_EXPORT liqRibHT {

public:
	liqRibHT();
	virtual ~liqRibHT();

    int           insert( MDagPath &, double, int,
                          ObjectType objType,int CountID,
                          MMatrix *matrix = NULL,
                          const MString instanceStr = "",
                          int particleId = -1 );
	/*RibNode*	    find( const MObject &, ObjectType objType );*/
	liqRibNodePtr find( MString nodeName, MDagPath  path, ObjectType objType);

private:
	str_Vector RibHashVec;
	type_Vector objTypeVec;
#ifdef Refactoring 
public: 
#endif
	RNMAP	RibNodeMap;
	ulong	hash( const char*, int ID );
	friend class liqRibTranslator;

private:
	liqRibHT(const liqRibHT&);
	liqRibHT& operator=(const liqRibHT&);
};

//static const uint MR_HASHSIZE = 65536;

#endif
