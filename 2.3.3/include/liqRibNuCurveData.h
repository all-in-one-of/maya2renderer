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

#ifndef liqRibNuCurveData_H
#define liqRibNuCurveData_H

/* ______________________________________________________________________
**
** Liquid Rib Nurbs Curve Data Header File
** ______________________________________________________________________
*/

#include <boost/shared_array.hpp>
#include <liqRibData.h>

class liqRibNuCurveData : public liqRibData {
public: // Methods

    explicit liqRibNuCurveData( MObject curve );
	virtual ~liqRibNuCurveData(){}

	virtual void write(
		const MString &ribFileName, 
		const structJob &currentJob, 
		const bool bReference
		);
    virtual bool       compare( const liqRibData & other ) const;
    virtual ObjectType type() const;

//private: // Data
    //void       _write(const structJob &currentJob);
    liqInt                   ncurves;
	boost::shared_array< liqInt >   nverts;
    //boost::shared_array< liqInt >   order;
    //boost::shared_array< liqFloat > knot;
    //boost::shared_array< liqFloat > min;
    //boost::shared_array< liqFloat > max;
    boost::shared_array< liqFloat > CVs;
    boost::shared_array< liqFloat > NuCurveWidth;

private:
	liqRibNuCurveData(const liqRibNuCurveData&);
	liqRibNuCurveData& operator=(const liqRibNuCurveData&);
};

#endif
