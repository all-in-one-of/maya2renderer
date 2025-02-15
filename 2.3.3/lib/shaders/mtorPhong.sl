/* $Revision: #2 $ */
/*-______________________________________________________________________
**
** Copyright (c) 1998 PIXAR.  All rights reserved.  This program or
** documentation contains proprietary confidential information and trade
** secrets of PIXAR.  Reverse engineering of object code is prohibited.
** Use of copyright notice is precautionary and does not imply
** publication.
**
**                    RESTRICTED RIGHTS NOTICE
**
** Use, duplication, or disclosure by the Government is subject to the
** following restrictions:  For civilian agencies, subparagraphs (a) through
** (d) of the Commercial Computer Software--Restricted Rights clause at
** 52.227-19 of the FAR; and, for units of the Department of Defense, DoD
** Supplement to the FAR, clause 52.227-7013 (c)(1)(ii), Rights in
** Technical Data and Computer Software.
**
** Pixar
** 1001 W Cutting Blvd
** Richmond, CA  94804
**
** ______________________________________________________________________
*/
#include "mtorPhong.impl"

surface
mtorPhong(  uniform color ambientColor = color(0);
            float diffuseCoeff = .8;
            float cosPower = 15;
            color incandescence = color(0);
            float translucenceCoeff = 0;
            color specularColor = color(1);
            float reflectivity = 1;
            float refractiveIndex = 1; /* unsupported */
            color refractedColor = color(1); /* unsupported */
            float glowIntensity = 0; /* unsupported */
            )
{
    mtorPhong(  ambientColor,
            diffuseCoeff,
            cosPower ,
            incandescence ,
            translucenceCoeff ,
            specularColor ,
            reflectivity ,
            refractiveIndex , /* unsupported */
            refractedColor , /* unsupported */
            glowIntensity /* unsupported */
            );
}
