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
** The RenderMan (R) Interface Procedures and Protocol are:** Copyright 1988, 1989, Pixar** All Rights Reserved****** RenderMan (R) is a registered trademark of Pixar
*/

#ifndef liquidRibCoordData_H
#define liquidRibCoordData_H

/* ______________________________________________________________________
**
** Liquid Rib Locator Data Header File
** ______________________________________________________________________
*/

#include <liqRibData.h>

class liqRibCoordData : public liqRibData {
public: // Methods

    explicit liqRibCoordData( MObject locator );
	virtual ~liqRibCoordData(){}

	virtual void write(
		const MString &ribFileFullPath, 
		const structJob &currentJob, 
		const bool bReference
		);
    virtual bool       compare( const liqRibData & other ) const;
    virtual ObjectType type() const;

	const MString getName() const { return name;}

private: // Data
    //void       _write(const structJob &currentJob);
	MString name;

private:
	liqRibCoordData(const liqRibCoordData&);
	liqRibCoordData& operator=(const liqRibCoordData&);
};

#endif
