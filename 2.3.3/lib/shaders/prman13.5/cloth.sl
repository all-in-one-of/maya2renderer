/* $Id: //depot/branches/rmanprod/rman-13.5/shaders/cloth.sl#1 $  (Pixar - RenderMan Division)  $Date: 2007/07/06 $ */
/*
** Copyright (c) 1999 PIXAR.  All rights reserved.  This program or
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
** 1001 West Cutting Blvd.
** Richmond, CA  94804
*/
/*---------------------------------------------------------------------------
 * cloth - displacement shader for a perpendicular weave pattern.
 *	Aliases pretty fiercely, but so does real cloth.
 *
 * freq - "frequency" of the weave
 * depth - displacement depth (displacement goes from -depth to +depth)
 *-------------------------------------------------------------------------*/
displacement
cloth(float freq=500, depth=0.02)
{
    float d;
    normal sN;
    point PP;

    d = noise(freq*s) + noise(freq*t) - 1; /* from -1 to 1 */
    PP = transform("shader",P);
    sN = normalize(ntransform("shader",N));
    P = transform("shader","current",PP + sN*depth*d);
    N = calculatenormal(P);
}
