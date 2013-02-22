/* $Id: //depot/branches/rmanprod/rman-13.5/shaders/causticlight.sl#1 $  (Pixar - RenderMan Division)  $Date: 2007/07/06 $ */
/*
** Copyright (c) 2002 PIXAR.  All rights reserved.  This program or
** documentation contains proprietary confidential information and trade
** secrets of PIXAR.  Reverse engineering of object code is prohibited.
** Use of copyright notice is precautionary and does not imply
** publication.
**
**                      RESTRICTED RIGHTS NOTICE
**
** Use, duplication, or disclosure by the Government is subject to the
** following restrictions:  For civilian agencies, subparagraphs (a) through
** (d) of the Commercial Computer Software--Restricted Rights clause at
** 52.227-19 of the FAR; and, for units of the Department of Defense, DoD
** Supplement to the FAR, clause 52.227-7013 (c)(1)(ii), Rights in
** Technical Data and Computer Software.
**
** Pixar
** 1200 Park Avenue
** Emeryville, CA 94608
*/

#include "normals.h"

light
causticlight(
    color filter = color(1);
    output float __nonspecular = 1;)
{
    normal Ns = shadingnormal(N);

    /* Compute "caustic illumination" */
    illuminate (Ps + Ns) {  /* force execution independent of light location */
	Cl = filter * caustic(Ps, Ns);
    }
}
