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
** Contributor(s): Alf Kraus.
**
**
** The RenderMan (R) Interface Procedures and Protocol are:
** Copyright 1988, 1989, Pixar
** All Rights Reserved
**
**
** RenderMan (R) is a registered trademark of Pixar
*/
#ifndef liqBoundingBoxLocator_H
#define liqBoundingBoxLocator_H

#include <common/prerequest_std.h>
#include <common/prerequest_maya.h>

class liqBoundingBoxLocator : public MPxLocatorNode
{
public:
	liqBoundingBoxLocator(){}
	virtual ~liqBoundingBoxLocator(){}

	virtual void draw( M3dView & view, const MDagPath & path, M3dView::DisplayStyle style, M3dView::DisplayStatus status );
	static void *creator();
    static MStatus initialize();

    static const MTypeId id;
    static const MString typeName;

	boost::shared_array< MPoint > pts;

	static MObject aDrawBox;

private:
	liqBoundingBoxLocator(const liqBoundingBoxLocator&);
	liqBoundingBoxLocator& operator=(const liqBoundingBoxLocator&);
};

#endif//liqBoundingBoxLocator_H