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

/** \brief Physical Sky & Sun
 * \file ei_physicalsky.cpp
 * \author Bo Zhou
 */

#include <eiAPI/ei_shaderx.h>

static scalar get_sun_intensity(
	const vector &ray_dir,
	const vector &sun_dir,
	scalar   haze,
	scalar   sun_disk_size,
	scalar   sun_disk_intensity,
	scalar   sun_glow_size,
	scalar   sun_glow_intensity,
	scalar   sun_glow_falloff)
{
	sun_disk_size *= 0.0465f;
	sun_glow_size *= 0.0465f;
	scalar sun_total_size = sun_disk_size + sun_glow_size;

	scalar intensity = 1.0f;
	scalar gamma = acosf(dot(ray_dir, sun_dir));
	if (gamma < sun_total_size)
	{
		scalar r = 1.0f - gamma / sun_total_size;
		intensity += sun_disk_intensity * smoothstep(0.0f, haze * 2.0f, r) + sun_glow_intensity * fastpow(r, sun_glow_falloff * haze);
		if (gamma < sun_disk_size)
		{
			intensity += sun_disk_intensity;
		}
	}
	return intensity;
}

/**
 * According to "A Practical Analytic Model for Daylight".
 */
static scalar get_sky_luminance(
	const vector &ray_dir,
	const vector &sun_dir,
	scalar   t)
{
	scalar cos_gamma = dot(ray_dir, sun_dir);
	if(cos_gamma < 0.0f)
	{
		cos_gamma = 0.0f;
	}
	if(cos_gamma > 1.0f)
	{
		cos_gamma = 2.0f - cos_gamma;
	}
	scalar gamma         = acosf(cos_gamma);
	scalar cos_theta     = ray_dir.y;
	scalar cos_theta_sun = sun_dir.y;
	scalar theta_sun     = acosf(cos_theta_sun);

	scalar a =   0.178721f * t - 1.463037f;
	scalar b = - 0.355402f * t + 0.427494f;
	scalar c = - 0.022669f * t + 5.325056f;
	scalar d =   0.120647f * t - 2.577052f;
	scalar e = - 0.066967f * t + 0.370275f;
	scalar ratio = ((1.0f + a * fastexp(b / cos_theta)) * ((1.0f + c * fastexp(d * gamma)) + (e * cos_gamma) * cos_gamma)) / ((1.0f + a * fastexp(b)) * ((1.0f + c * fastexp(d * theta_sun)) + (e * cos_theta_sun) * cos_theta_sun));
	return ratio;
}

static color get_haze_driven_sky_color(
	const vector &ray_dir,
	const vector &sun_dir,
	scalar   t)
{
	scalar cos_gamma = dot(ray_dir, sun_dir);
	if(cos_gamma < 0.0f)
	{
		cos_gamma = 0.0f;
	}
	if(cos_gamma > 1.0f)
	{
		cos_gamma = 2.0f - cos_gamma;
	}
	scalar gamma = acosf(cos_gamma);

	scalar cos_theta     = ray_dir.y;
	scalar cos_theta_sun = sun_dir.y;

	scalar t2 = t * t;

	scalar theta_sun  = acosf(cos_theta_sun);
	scalar theta_sun2 = theta_sun * theta_sun;
	scalar theta_sun3 = theta_sun * theta_sun2;

	scalar zenith_x = ((((0.001650f * theta_sun3 - 0.003742f * theta_sun2) + 0.002088f * theta_sun) + 0) * t2 + (((-0.029028f * theta_sun3 + 0.063773f * theta_sun2) - 0.032020f * theta_sun) + 0.003948f) * t) + (((+0.116936f * theta_sun3 - 0.211960f * theta_sun2) + 0.060523f * theta_sun) + 0.258852f);
	scalar zenith_y = ((((0.002759f * theta_sun3 - 0.006105f * theta_sun2) + 0.003162f * theta_sun) + 0) * t2 + (((-0.042149f * theta_sun3 + 0.089701f * theta_sun2) - 0.041536f * theta_sun) + 0.005158f) * t) + (((+0.153467f * theta_sun3 - 0.267568f * theta_sun2) + 0.066698f * theta_sun) + 0.266881f);

	scalar chi = (4.0f / 9.0f - t / 120.0f) * (static_cast<scalar>(eiPI) - 2.0f * theta_sun);
	scalar zenith_Y = ((1000.0f * (4.0453f * t - 4.9710f)) * fastertanfull(chi) - 0.2155f * t) + 2.4192f;
	zenith_Y *= get_sky_luminance(ray_dir, sun_dir, t);

	//
	scalar a, b, c, d, e;
	a = - 0.019257f * t - (0.29f - Fast_sqrt(cos_theta_sun) * 0.09f);
	b = - 0.066513f * t + 0.000818f;
	c = - 0.000417f * t + 0.212479f;
	d = - 0.064097f * t - 0.898875f;
	e = - 0.003251f * t + 0.045178f;
	scalar ratio_x = ((1.0f + a * fastexp(b / cos_theta)) * (1.0f + c * fastexp(d * gamma) + e * cos_gamma * cos_gamma)) /
	                  ((1.0f + a * fastexp(b)) * (1.0f + c * fastexp(d * theta_sun) + e * cos_theta_sun * cos_theta_sun));

	a = - 0.016698f * t - 0.260787f;
	b = - 0.094958f * t + 0.009213f;
	c = - 0.007928f * t + 0.210230f;
	d = - 0.044050f * t - 1.653694f;
	e = - 0.010922f * t + 0.052919f;
	scalar ratio_y = ((1.0f + a * fastexp(b / cos_theta)) * (1.0f + c * fastexp(d * gamma) + e * cos_gamma * cos_gamma)) /
	                  ((1.0f + a * fastexp(b)) * (1.0f + c * fastexp(d * theta_sun) + e * cos_theta_sun * cos_theta_sun));
	scalar chromaticity_x = zenith_x * ratio_x;
	scalar chromaticity_y = zenith_y * ratio_y;

	//
	color cie_XYZ;
	cie_XYZ.g = zenith_Y;
	cie_XYZ.r = chromaticity_x / chromaticity_y * cie_XYZ.g;
	cie_XYZ.b = (1.0f - chromaticity_x - chromaticity_y) / chromaticity_y * cie_XYZ.g;

	color linear_RGB;
	linear_RGB.r =   3.241f * cie_XYZ.r - 1.537f * cie_XYZ.g - 0.499f * cie_XYZ.b;
	linear_RGB.g = - 0.969f * cie_XYZ.r + 1.876f * cie_XYZ.g + 0.042f * cie_XYZ.b;
	linear_RGB.b =   0.056f * cie_XYZ.r - 0.204f * cie_XYZ.g + 1.057f * cie_XYZ.b;
	return linear_RGB;
}

/**
 * According to "Spatial distribution of daylight CIE standard general sky".
 */
static color get_cie_standard_sky_color(
	const vector &ray_dir,
	const vector &sun_dir,
	const color  &zenith_color,
	scalar   a,
	scalar   b,
	scalar   c,
	scalar   d,
	scalar   e)
{
	eiVector2 ray_pos;
	ray_pos.x = acosf(ray_dir.y);
	ray_pos.y = atan2f(ray_dir.z, ray_dir.x);

	eiVector2 sun_pos;
	sun_pos.x = acosf(sun_dir.y);
	sun_pos.y = atan2f(sun_dir.z, sun_dir.x);

	scalar cos_z_sun = fastercosfull(sun_pos.x);
	scalar cos_z = fastercosfull(ray_pos.x);
	scalar sin_z_sun = fastersinfull(sun_pos.x);
	scalar sin_z = fastersinfull(ray_pos.x);

	scalar chi = acosf(cos_z_sun * cos_z + sin_z_sun * sin_z * fastercosfull(absf(ray_pos.y - sun_pos.y)));
	scalar cos_chi = fastercosfull(chi);

	const scalar pi_over_2 = static_cast<scalar>(eiPI / 2.0f);
	scalar f_chi   = 1.0f + c * (fastexp(d * chi) - fastexp(d * pi_over_2)) + e * cos_chi * cos_chi;
	scalar phi_z   = 1.0f + a * fastexp(b / cos_z);
	scalar f_z_sun = 1.0f + c * (fastexp(d * sun_pos.x) - fastexp(d * pi_over_2)) + e * cos_z_sun * cos_z_sun;
	scalar phi_0   = 1.0f + a * fastexp(b);
	scalar ratio   = (f_chi * phi_z) / (f_z_sun * phi_0);

	return zenith_color * ratio;
}

ENVIRONMENT(erPhysicalsky)
	DECLARE;
	DECLARE_VECTOR(i_sun_dir, 0.577f, 0.577f, 0.577f);
	DECLARE_SCALAR(i_sun_disk_size, 2.0f);
	DECLARE_SCALAR(i_sun_disk_intensity, 7.0f);
	DECLARE_SCALAR(i_sun_glow_size, 1.0f);
	DECLARE_SCALAR(i_sun_glow_intensity, 1.0f);
	DECLARE_SCALAR(i_sun_glow_falloff, 5.0f);
	DECLARE_COLOR(i_ground_color, 0.2f, 0.2f, 0.2f);
	DECLARE_SCALAR(i_ground_blur, 0.01f);
	DECLARE_INT(i_type, 0);
	DECLARE_SCALAR(i_haze, 5.0f);
	DECLARE_COLOR(i_zenith_color, 0.109f, 0.109f, 0.109f);
	DECLARE_SCALAR(i_a, -1.0f);
	DECLARE_SCALAR(i_b, -0.32f);
	DECLARE_SCALAR(i_c, 10.0f);
	DECLARE_SCALAR(i_d, -3.0f);
	DECLARE_SCALAR(i_e, 0.45f);
	DECLARE_SCALAR(i_intensity, 0.5f);
	DECLARE_OUT_COLOR(o_result, 0.0f, 0.0f, 0.0f);//update with liq_skylight::init() ei_token("o_result")
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

	color physicalsky_color(const vector & ray_dir)
	{
		scalar haze_val = max(i_haze() + 2.0f, 2.0f);
		vector sun_dir_val(i_sun_dir());
		color sky_color(0.0f);
		switch (i_type())
		{
		case 0:
			{
				sky_color = 0.0001f * get_haze_driven_sky_color(ray_dir, sun_dir_val, haze_val);
			}
			break;
		case 1:
			{
				sky_color = get_cie_standard_sky_color(ray_dir, sun_dir_val, i_zenith_color(), i_a(), i_b(), i_c(), i_d(), i_e());
			}
			break;
		default:
			{
				return color(0.0f);
			}
		}
		
		return i_intensity() * sky_color * get_sun_intensity(ray_dir, sun_dir_val, haze_val, i_sun_disk_size(), i_sun_disk_intensity(), i_sun_glow_size(), i_sun_glow_intensity(), i_sun_glow_falloff());
	}

	void main(void *arg)
	{
		// the shading space of physical sky should be in world space
		vector Iw(normalize(vto_world(I)));
		vector ray_dir = Iw;
		// we should always enable ground blur for Max material editor. 
		// when refraction is enabled, and IOR is 1.0, because in that 
		// case, the ray direction will be coplanar with X-Y plane 
		// (Y is 0.0), and after some transformations between internal 
		// space and local frame, the precision will lose and result 
		// in either +epsilon or -epsilon for Y component.
		scalar blur = i_ground_blur();
		if (ray_dir.y >= blur)
		{
			o_result() = physicalsky_color(ray_dir);
		}
		else if (ray_dir.y <= 0.0f)
		{
			o_result() = i_ground_color();
		}
		else
		{
			color sky_c(physicalsky_color(ray_dir));
			color ground_c(i_ground_color());

			scalar factor = curve(ray_dir.y / blur);
			o_result() = ground_c * (1.0f - factor) + sky_c * factor;
		}
		out->Ci = o_result();
		out->Oi = color(0.0f);
	}

END(erPhysicalsky)
