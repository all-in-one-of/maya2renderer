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
*/

#ifndef liqRibObject_H
#define liqRibObject_H

/* ______________________________________________________________________
**
** Liquid Rib Object Header File
** ______________________________________________________________________
*/
#include <common/prerequest_std.h>
#include <common/prerequest_maya.h>
// Renderman Headers
//extern "C" {
#include "liqtypes.h"//#include "ri_interface.h"
//}
#include "liqStructJob.h"
#include "liqEnum.h"

enum AnimType {
	MRX_Const         = 0,
	MRX_Animated      = 1,
	MRX_Incompatible  = 2
};



enum ObjectType;
enum AnimType;
class liqRibObj;
class liqRibData;
typedef boost::shared_ptr< liqRibObj > liqRibObjPtr;
typedef boost::shared_ptr< liqRibData > liqRibDataPtr;

//struct structJob;

class LIQUID_EXPORT liqRibObj {
public:
    explicit liqRibObj( const MDagPath &, ObjectType objType );
	virtual ~liqRibObj(){}

    AnimType compareMatrix( const liqRibObjPtr, int instance ) const;
    AnimType compareBody( const liqRibObjPtr ) const;
    void     writeObject(const MString& ribFileFullPath, const structJob &currentJob, const bool bReference) const; // write geometry directly
    unsigned granularity() const; // get granularity
//    void     writeNextObjectGrain(const MString& ribFileFullPath, const structJob &currentJob) const; // write next geometry grain directly
    bool     isNextObjectGrainAnimated() const; // whether the next grain needs to be in a motion block

    ObjectType      type;
    int      written;
    bool     ignore;
	bool     ignoreShadow;
	bool     receiveShadow;
    bool     ignoreShapes;

    MMatrix  matrix( int instance ) const;
    void     setMatrix( int instance, MMatrix matrix );
    void     ref();
    void     unref();

    void     setHandle( liqObjectHandle handle );

    liqObjectHandle handle() const;
    liqLightHandle  lightHandle() const;
    //char         **lightSources; // unused?
	const liqRibDataPtr getDataPtr() const{ return data; }

private:
	std::vector< MMatrix > instanceMatrices; // Matrices for all instances of this object
    liqObjectHandle    objectHandle;     // Handle used by RenderMan to refer to defined geometry
    int               referenceCount;   // Object's reference count
    liqRibDataPtr     data;             // Geometry or light data
private:
	liqRibObj(const liqRibObj&);
	liqRibObj& operator=(const liqRibObj&);
};

#endif
