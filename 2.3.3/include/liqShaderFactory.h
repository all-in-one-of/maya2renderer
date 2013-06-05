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
** Contributor(s): Baptiste Sansierra.
**
**
** The RenderMan (R) Interface Procedures and Protocol are:
** Copyright 1988, 1989, Pixar
** All Rights Reserved
**
**
** RenderMan (R) is a registered trademark of Pixar
*/

#ifndef liqShaderFactory_H
#define liqShaderFactory_H

#include <common/prerequest_std.h>
#include <common/prerequest_maya.h>
#include "liqtypes.h"

class liqShader;//#include <liqShader.h>

class LIQUID_EXPORT liqShaderFactory
{
public:
    inline static liqShaderFactory &instance()
    {
        if( !_instance )
            _instance = new liqShaderFactory();
        return *_instance;
    }
    inline static void deleteInstance()
    {
        if( _instance )
            delete _instance;
    }
    
	virtual ~liqShaderFactory();

	liqShader &getShader(MObject shaderObj);
	liqShader &getShader(const char* shaderNodeName);
//	MString getShaderId( MObject shaderObj );

	MString getUniqueShaderHandler();

	void clearShaders();

private:
	liqShaderFactory();
	static liqShaderFactory *_instance;
	int shaderHandlerId;
	std::vector<liqShader*> m_shaders;

private:
	liqShaderFactory(const liqShaderFactory&);
	liqShaderFactory& operator=(const liqShaderFactory&);
};

#endif//liqShaderFactory_H
