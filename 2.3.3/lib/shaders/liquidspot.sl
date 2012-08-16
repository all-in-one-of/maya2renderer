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
** Liquid Spot Light Shader Source
** ______________________________________________________________________
*/

#include "liquidspot.impl"

light
liquidspot(
      uniform float intensity               = 1;
      uniform color lightcolor              = 1;
      uniform float coneangle               = radians( 40 );
      uniform float penumbraangle           = radians( 0 );
      uniform float dropoff                 = 0;
      uniform float decay                   = 0;

      uniform float barndoors               = 0;
      uniform float leftbarndoor            = 10;
      uniform float rightbarndoor           = 10;
      uniform float topbarndoor             = 10;
      uniform float bottombarndoor          = 10;

      uniform float decayRegions            = 0;
      uniform float startDistance1          = 1.0;
      uniform float endDistance1            = 2.0;
      uniform float startDistance2          = 3.0;
      uniform float endDistance2            = 6.0;
      uniform float startDistance3          = 8.0;
      uniform float endDistance3            = 10.0;
      uniform float startDistanceIntensity1 = 1.0;
      uniform float endDistanceIntensity1   = 1.0;
      uniform float startDistanceIntensity2 = 1.0;
      uniform float endDistanceIntensity2   = 1.0;
      uniform float startDistanceIntensity3 = 1.0;
      uniform float endDistanceIntensity3   = 1.0;

      uniform string shadowname       = "";
      uniform float  shadowbias       = 0.01;
      uniform float  shadowblur       = 0.0;
      uniform float  shadowsamples    = 32;
      uniform float  shadowfiltersize = 1;
      uniform color  shadowcolor      = 0;
      varying color  shadowcolorSurf  = 0;
      uniform float  shadowcolorMix  = -1;

      uniform float  lightID          = 0;
      uniform string __category       = "";

      output varying color __shadowC        = 0;
      output varying float __shadowF        = 0;
      output varying float __barn           = 0;
      output varying color __unshadowed_Cl  = 0;
      output varying float __nondiffuse     = 0;
      output varying float __nonspecular    = 0;
)
{
    liquidspot(
      intensity               ,
      lightcolor              ,
      coneangle               ,
      penumbraangle           ,
      dropoff                 ,
      decay                   ,

      barndoors               ,
      leftbarndoor            ,
      rightbarndoor           ,
      topbarndoor             ,
      bottombarndoor          ,

      decayRegions            ,
      startDistance1          ,
      endDistance1            ,
      startDistance2          ,
      endDistance2            ,
      startDistance3          ,
      endDistance3            ,
      startDistanceIntensity1 ,
      endDistanceIntensity1   ,
      startDistanceIntensity2 ,
      endDistanceIntensity2   ,
      startDistanceIntensity3 ,
      endDistanceIntensity3   ,

      shadowname       ,
      shadowbias       ,
      shadowblur       ,
      shadowsamples    ,
      shadowfiltersize ,
      shadowcolor      ,
      shadowcolorSurf  ,
      shadowcolorMix  ,

      lightID          ,
      __category       ,

      __shadowC        ,
      __shadowF        ,
      __barn           ,
      __unshadowed_Cl  ,
      __nondiffuse     ,
      __nonspecular
   );
}


