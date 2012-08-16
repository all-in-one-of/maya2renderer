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
** Contributor(s): Philippe Leprince.
**
**
** The RenderMan (R) Interface Procedures and Protocol are:
** Copyright 1988, 1989, Pixar
** All Rights Reserved
**
**
** RenderMan (R) is a registered trademark of Pixar
*/

/* ______________________________________________________________________
**
** Liquid Area Light Shader Source
** ______________________________________________________________________
*/



#include "liquidarea.impl"

/*
 * Simple implementation of a rectangular "pseudo area light".
 * This is an analytic solution to the problem.
 * We use stratified sampling to illuminate the surfaces.
 * Shadows are computed using the transmission shadeop, so
 * shadow-casting objects should be visible to transmission
 * rays. This is potentially expensive.
 * The light uses a named coordinate system to calculate
 * it's geometry. This coordinate system can also be used to
 * render specular reflections in surface shaders using
 * message passing.
 */

light
liquidarea(
      uniform float   intensity     = 1;
      uniform color   lightcolor    = 1;
      uniform float   decay         = 2;
      uniform string  coordsys      = "";
      uniform float   lightsamples  = 32;
      uniform float   doublesided   = 0;
      uniform string  shadowname    = "";
      uniform color   shadowcolor   = 0;
      uniform string  hitmode       = "primitive";

      uniform string  lightmap      = "";
      uniform float   lightmapsaturation  = 2.0;

      uniform float  lightID        = 0;
      uniform string __category     = "";

      output uniform float __nonspecular          = 1;
      output varying float __shadowF              = 0;
      output varying color __shadowC              = 0;
      output varying color __unshadowed_Cl        = 0;
      output uniform float __arealightIntensity   = 0;
      output uniform color __arealightColor       = 0;
)
{
    liquidarea(
      intensity,
      lightcolor,
      decay,
      coordsys,
      lightsamples,
      doublesided,
      shadowname,
      shadowcolor,
      hitmode,

      lightmap,
      lightmapsaturation,

      lightID,
      __category,

      __nonspecular,
      __shadowF,
      __shadowC,
      __unshadowed_Cl,
      __arealightIntensity,
      __arealightColor
    );
}
