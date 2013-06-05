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

#ifndef liqRibHierarchicalSubdivisionData_H
#define liqRibHierarchicalSubdivisionData_H

/* ______________________________________________________________________
**
** Liquid Rib Mesh Data Header File
** ______________________________________________________________________
*/
#include <common/prerequest_std.h>
//#include <liquid.h>
#include <liqRibData.h>
#include "liqtypes.h"

using namespace boost;

#ifndef liqSubdivStructs_H
#define liqSubdivStructs_H

typedef struct tagPolyEdgeIndx
{
	liqInt	vtx0;
	liqInt	vtx1;
} PolyEdgeIndx;

typedef liqInt PolyVertexIndx;
typedef liqInt PolyFaceIndx;

typedef struct tagSbdExtraTag
{
	liqFloat	value;		// hardness for creases and corners
	liqInt	length;		// number of elements
	union tagExtraData
	{
		PolyEdgeIndx	*edges;
		PolyVertexIndx	*vertices;
		PolyFaceIndx	*faces;
	} ExtraData;
} SbdExtraTag;

#endif


class liqRibHierarchicalSubdivisionData : public liqRibData
{
public: // Methods
	explicit liqRibHierarchicalSubdivisionData( MObject mesh );
	virtual ~liqRibHierarchicalSubdivisionData();

	virtual void       write(const MString &ribFileFullPath, const structJob &currentJob, const bool bReference);
	virtual bool       compare( const liqRibData & other ) const;
	virtual ObjectType type() const;
	virtual void initializeSubdivParameters();
	
	bool isEmpty()const { return (numPoints<=1); }
	MString getName()const { return name; }
	const liqFloat* getTransformationMatrixPtr() const {return &transformationMatrix[0][0]; }

//private: // Data
	//void       _write(const structJob &currentJob);
	liqInt     numFaces;
	liqInt     numPoints;
	shared_array< liqInt > nverts;
	shared_array< liqInt > verts;
	const liqFloat* vertexParam;

	DetailType uvDetail;
	bool trueFacevarying;

	MString   name;
	liqMatrix  transformationMatrix;

	int interpolateBoundary; // Now an integer from PRMan 12/3Delight 6

	std::vector <liqToken> v_tags;
	std::vector <liqInt>   v_nargs;
	std::vector <liqInt>   v_intargs;
	std::vector <liqFloat> v_floatargs;

	void checkExtraTags( MObject &mesh );
	void addExtraTags( MObject &mesh, SBD_EXTRA_TAG extraTag );
	void addExtraTags( MObject &mesh, float extraTagValue, SBD_EXTRA_TAG extraTag );

	// subdiv params
	liqToken m_subdivScheme;
	liqInt m_subdivNTags;
	liqToken *m_subdivTags;
	liqInt *m_subdivNArgs;
	int m_subdivsNInts;
	int m_subdivsNFloats;
	int m_subdivsNStrings;
	liqInt *m_subdivIntArgs;
	liqFloat *m_subdivFloatArgs;
	liqString *m_subdivStringArgs;

private:
	liqRibHierarchicalSubdivisionData(const liqRibHierarchicalSubdivisionData&);
	liqRibHierarchicalSubdivisionData& operator=(const liqRibHierarchicalSubdivisionData&);
};

#endif
