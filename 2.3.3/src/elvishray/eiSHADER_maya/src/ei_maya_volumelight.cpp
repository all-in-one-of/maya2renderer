/**************************************************************************
 * Copyright (C) 2011 Elvic Liang<len3dev@gmail.com>
 * All rights reserved.
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *************************************************************************/

#include <eiAPI/ei_shaderx.h>

LIGHT(spherelight)

	DECLARE;
	DECLARE_SCALAR(intensity, 1.0f);
	DECLARE_COLOR(lightcolor, 1.0f, 1.0f, 1.0f);
	DECLARE_SCALAR(radius, 1.0f);
	END_DECLARE;

	static void init()
	{
	}

	static void exit()
	{
	}

	void init_node()
	{
	}

	void exit_node()
	{
	}

	// assume dir is normalized before calling this function.
	// get the nearest positive intersection.
	// R2 is the square of sphere radius.
	static eiFORCEINLINE eiBool intersectSphere(
		const point & org, const vector & dir, 
		const point & center, const scalar R2, 
		scalar & t)
	{
		vector oc = org - center;
		scalar B = 2.0f * dot(dir, oc);
		scalar C = dot(oc, oc) - R2;
		scalar disc = B * B - 4.0f * C;
		if (disc < 0.0f)
		{
			return eiFALSE;
		}
		scalar d = sqrtf(disc);
		scalar t0 = (-B - d) * 0.5f;
		if (t0 > 0.0f)
		{
			t = t0;
			return eiTRUE;
		}
		scalar t1 = (-B + d) * 0.5f;
		if (t1 > 0.0f)
		{
			t = t1;
			return eiTRUE;
		}
		return eiFALSE;
	}

	eiBool intersect(void *arg)
	{
		scalar t = eiMAX_SCALAR;
		scalar R = radius();
		scalar R2 = MAX(eiSCALAR_EPS, R * R);

		// we need L to be normalized
		L = normalize(L);

		if (intersectSphere(P, L, E, R2, t))
		{
			scalar dist2 = distance_squared(P, E);

			// set to black if point is inside the sphere
			if (dist2 - R2 < eiSCALAR_EPS)
			{
				Cl = 0.0f;
				return eiFALSE;
			}

			vector lightNormal = normalize((P + L * t) - E);
			scalar cosLightNormal = dot(lightNormal, -L);

			if (cosLightNormal <= 0.0f)
			{
				Cl = 0.0f;
				return eiFALSE;
			}

			// sample sphere uniformly inside subtended cone
			scalar cosThetaMax = sqrtf(MAX(0.0f, 1.0f - R2 / distance_squared(P, E)));
			Lpdf = ei_uniform_cone_pdf(cosThetaMax);

			// L() * t is the maximum distance limit for shadow ray.
			// by our definition, lights won't block each other, 
			// they can only be blocked by scene geometry, so we 
			// only need to intersect with the scene here.
			// if you really need a light to be blocked by another light, 
			// just place a geometry there to workaround.
			scalar inv_decay = length_squared(L * t) / (4.0f * PI * R2);
			Cl = inv_decay * trace_shadow(P, L * t, intensity() * lightcolor());
			return eiTRUE;
		}
		else
		{
			Cl = 0.0f;
			return eiFALSE;
		}
	}

	void main(void *arg)
	{
		point Lp;
		scalar R = radius();
		scalar R2 = MAX(eiSCALAR_EPS, R * R);
		vector wc = normalize(E - P);
		scalar dist2 = distance_squared(P, E);
		// set to black if point is inside the sphere
		if (dist2 - R2 < eiSCALAR_EPS)
		{
			Cl = 0.0f;
			return;
		}

		const eiLightSampler *sampler = light_sampler;

		// sample sphere uniformly inside subtended cone
		scalar cosThetaMax = sqrtf(MAX(0.0f, 1.0f - R2 / distance_squared(P, E)));
		vector ray_dir;
		ei_uniform_sample_cone(&ray_dir, sampler->u1, sampler->u2, cosThetaMax);
		vector wcX, wcY;
		ortho_basis(&wc, &wcX, &wcY);
		ray_dir = wcX * ray_dir.x + wcY * ray_dir.y + wc * ray_dir.z;
		scalar t = eiMAX_SCALAR;
		if (!intersectSphere(P, ray_dir, E, R2, t))
		{
			Lp = E - R * wc;
		}
		else
		{
			Lp = P + ray_dir * t;
		}
		vector lightNormal = normalize(Lp - E);
		Lpdf = ei_uniform_cone_pdf(cosThetaMax);

		// use epsilon for the spread angle to avoid NaN
		if (illuminate(Lp, lightNormal, eiCOS_DEG_89_9)) // cos(PI / 2) = 0
		{
			scalar inv_decay = length_squared(L) / (4.0f * PI * R2);
			Cl = inv_decay * trace_shadow(P, -L, intensity() * lightcolor());
		}
	}

END_LIGHT(spherelight)
