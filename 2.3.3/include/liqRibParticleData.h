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

#ifndef liqRibParticleData_H
#define liqRibParticleData_H

/* ______________________________________________________________________
**
** Liquid Rib Nurbs Curve Data Header File
** ______________________________________________________________________
*/
#include <liqRibData.h>

class liqRibParticleData : public liqRibData {
public:

  explicit liqRibParticleData( MObject curve );
  virtual ~liqRibParticleData(){}

  virtual void write(
	  const MString &ribFileName, 
	  const structJob &currentJob, 
	  const bool bReference
	  );
  virtual unsigned granularity() const;
//  virtual bool writeNextGrain(const structJob &currentJob);
  virtual bool	compare( const liqRibData & other ) const;
  virtual ObjectType	type() const;

  void addAdditionalParticleParameters( MObject node );

  void addAdditionalFloatParameters ( MFnDependencyNode &nodeFn);
  //void addAdditionalPointParameters ( MFnDependencyNode nodeFn);
  void addAdditionalVectorParameters( MFnDependencyNode &nodeFn, const std::string& prefix, ParameterType type );
  //void addAdditionalColorParameters ( MFnDependencyNode nodeFn);

  // pType data type, these values corrospond to the types of
  // particleRenderType in maya!
  enum pType {
	MPTMultiPoint = 0,
	MPTMultiStreak = 1,
    MPTNumeric = 2,
    MPTPoints = 3,
    MPTSpheres = 4,
    MPTSprites = 5,
    MPTStreak = 6,
    MPTBlobbies = 7,
    MPTCloudy = 8,
    MPTTube = 9
  };

  pType particleType;

//private:
  //void	_write(const structJob &currentJob);
  unsigned grain;

  // Data storage for blobby particles
  std::vector< liqInt > m_codeArray;
  std::vector< liqFloat > m_floatArray;
  std::vector< std::string > m_stringArray;

  MIntArray m_validParticles;

  unsigned  m_numParticles;
  unsigned  m_numValidParticles;
  short     m_multiCount;  // Support for multi-point and multi-streak.

private:
	liqRibParticleData(const liqRibParticleData&);
	liqRibParticleData& operator=(const liqRibParticleData&);
};

#endif
