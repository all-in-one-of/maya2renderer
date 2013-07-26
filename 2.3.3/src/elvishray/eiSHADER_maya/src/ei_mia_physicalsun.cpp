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
#include <eiCORE/ei_timer.h>
#include <algorithm> // upper_bound
#include <cassert>

LIGHT(mia_physicalsun)

	DECLARE;
	DECLARE_BOOL(i_on, eiTRUE);
	DECLARE_SCALAR(i_multiplier,		1.0f);
	DECLARE_COLOR(i_rgb_unit_conversion, 0.0001f, 0.0001f, 0.0001f);
	DECLARE_SCALAR(i_haze,				0.0f);
	DECLARE_SCALAR(i_redblueshift,		0.0f);
	DECLARE_SCALAR(i_saturation,		1.0f);
	DECLARE_SCALAR(i_horizon_height,	0.0f);
	DECLARE_SCALAR(i_shadow_softness,	1.0f);
	DECLARE_INT(i_samples,				8);
	DECLARE_VECTOR(i_photon_bbox_min,	0.0f, 0.0f, 0.0f);
	DECLARE_VECTOR(i_photon_bbox_max,	0.0f, 0.0f, 0.0f);
	DECLARE_BOOL(i_automatic_photon_energy,				eiFALSE);
	DECLARE_BOOL(i_y_is_up,								eiTRUE);
	DECLARE_INT(i_illuminance_mode,						0);
	DECLARE_SCALAR(i_direct_normal_illuminance,			0.0f);
	DECLARE_TOKEN(env_shader,			NULL);
	DECLARE_INT(resolution,				1000);
	DECLARE_SCALAR(max_dist,			10000.0f);
	END_DECLARE;
		
	static const char *u_result;

	class Globals
	{
	public:
		Globals()
		{
			values = NULL;
			rows = NULL;
			cols = NULL;
		}

		~Globals()
		{
			delete [] values;
			delete [] rows;
			delete [] cols;
		}

		color		*values;	// actual map
		scalar		*rows;		// probability of choosing a given row 'y'
		scalar		*cols;		// probability of choosing a given column 'x', given that we've chosen row 'y'
		int			res;		// resolution in pixels of the precomputed table
		scalar		invres;		// 1 / resolution
		scalar		jacobian;
	};

	static void init()
	{
		u_result = ei_token("result");
	}

	static void exit()
	{
	}

	static eiFORCEINLINE vector map(const Globals *g, const scalar u, const scalar v, scalar & sintheta)
	{
		const scalar invres = g->invres;
		const scalar phi = (u * invres - 0.5f) * (2.0f * (scalar)eiPI);
		const scalar theta = (0.5f - v * invres) * (scalar)eiPI;

		const scalar costheta = cosf(theta);
		// map the range of theta from [-pi/2, pi/2] to [0, pi]
		sintheta = max(eiSCALAR_EPS, sinf(theta + (scalar)eiPI_2));

		return vector(sinf(phi) * costheta, 
			-cosf(phi) * costheta, 
			sinf(theta));
    }

	eiFORCEINLINE const color cb(const vector & dir, const eiTag shader)
	{
		color	C;

		I = dir;

		const eiUshort prev_ray_type = ray_type;
		ray_type = EI_RAY_ENVIRONMENT;

		eiShaderOutput outputs[] = {
			{EI_TYPE_VECTOR, u_result, &C}, 
		};

		call_shader(shader, 1, outputs, NULL);

		ray_type = prev_ray_type;

		return C;
	}

	void init_node()
	{
		const int MIN_RESOLUTION = 32;

		Globals *g = new Globals;
		glob = (void *)g;

		const eiTag shader = ei_find_node( env_shader().str ) ;
		if (shader == eiNULL_TAG)
		{
			printf("ERROR> mia_physicalsun, env_shader().str=\"%s\", it can't be found\n", env_shader().str);
			assert(0 && "mia_physicalsun's env_shader() node is not found");
			return;
		}

		eiTimer timer;
		ei_timer_reset(&timer);
		ei_timer_start(&timer);

		g->res = resolution();
		if (g->res < MIN_RESOLUTION) {
			g->res = MIN_RESOLUTION; // validate
		}
        g->invres = 1.0f / g->res;
        g->jacobian = g->invres * g->invres * (2.0f * (scalar)eiPI * (scalar)eiPI);
        g->values = new color[g->res * g->res];
        g->rows = new scalar[g->res];
        g->cols = new scalar[g->res * g->res];
        for (int y = 0, i = 0; y < g->res; ++y) {
            for (int x = 0; x < g->res; ++x, ++i) {
				scalar sintheta;
                g->values[i] = cb(vfrom_world(map(g, (scalar)x + 0.5f, (scalar)y + 0.5f, sintheta)), shader);
                g->cols[i] = luminance(g->values[i]) * sintheta + ((x > 0) ? g->cols[i - 1] : 0.0f);
            }
            g->rows[y] = g->cols[i - 1] + ((y > 0) ? g->rows[y - 1] : 0.0f);
            // normalize the pdf for this scanline (if it was non-zero)
			if (g->cols[i - 1] > 0) {
				for (int x = 0; x < g->res; ++x) {
                    g->cols[i - g->res + x] /= g->cols[i - 1];
				}
			}
        }
        // normalize the pdf across all scanlines
		if (g->rows[g->res - 1] > 0) {
			for (int y = 0; y < g->res; ++y) {
				g->rows[y] /= g->rows[g->res - 1];
			}
		}

		ei_timer_stop(&timer);

		eiInt hours, minutes;
		eiScalar seconds;
		ei_timer_format(&timer, &hours, &minutes, &seconds);
		ei_info("IBL: Finished pre-processing in %d hours %d minutes %f seconds.\n", hours, minutes, seconds);
	}

	void exit_node()
	{
		Globals *g = (Globals *)glob;
		delete g;
	}

	static eiFORCEINLINE int eval(const Globals *g, const vector & dir, scalar & pdf)
	{
        // map from sphere to unit-square
        const scalar u = 0.5f + atan2f(dir.x, -dir.y) * (1.0f / (2.0f * (scalar)eiPI));
		const scalar theta = asinf(clamp(dir.z, -1.0f, 1.0f)); // the range is in [-pi/2, pi/2]
        const scalar v = 0.5f - theta * (1.0f / (scalar)eiPI);
        // retrieve nearest neighbor
		const int res = g->res;
        const int x = clamp((int)(u * res), 0, res - 1);
        const int y = clamp((int)(v * res), 0, res - 1);
        const int i = y * res + x;
		const scalar *rows = g->rows;
		const scalar *cols = g->cols;
        const scalar row_pdf = rows[y] - (y > 0 ? rows[y - 1] : 0.0f);
        const scalar col_pdf = cols[i] - (x > 0 ? cols[i - 1] : 0.0f);
		// map the range of theta from [-pi/2, pi/2] to [0, pi]
		const scalar sintheta = max(eiSCALAR_EPS, sinf(theta + (scalar)eiPI_2));
        pdf = (row_pdf * col_pdf) / (g->jacobian * sintheta);
        return i;
    }

	static eiFORCEINLINE scalar sample_cdf(const Globals *g, const scalar *data, unsigned int n, const scalar x, unsigned int & idx, scalar & pdf)
	{
        idx = std::upper_bound(data, data + n, x) - data;
		idx = clamp<unsigned int>(idx, 0, g->res - 1);
        scalar scaled_sample;
        if (idx == 0) {
            pdf = data[0];
            scaled_sample = x;
        } else {
            pdf = data[idx] - data[idx - 1];
            scaled_sample = x - data[idx - 1];
        }
		pdf = max(eiSCALAR_EPS, pdf);
        return scaled_sample / pdf;
    }

    static eiFORCEINLINE int sample(const Globals *g, scalar rx, scalar ry, vector & dir, scalar & pdf)
	{
        scalar row_pdf, col_pdf;
        unsigned int x, y;
		const int res = g->res;
        ry = sample_cdf(g, g->rows, res, ry, y, row_pdf);
        rx = sample_cdf(g, g->cols + y * res, res, rx, x, col_pdf);
		scalar sintheta;
        dir = map(g, (scalar)x + rx, (scalar)y + ry, sintheta);
		pdf = (row_pdf * col_pdf) / (g->jacobian * sintheta);
        return (y * res + x);
    }

	eiBool intersect(void *arg)
	{
		scalar pdf;
		L = normalize(L);
		const Globals *g = (const Globals *)glob;
		Cl = trace_shadow(P, L * max_dist(), g->values[eval(g, vto_world(L), pdf)]);
		Lpdf = pdf;

		return eiTRUE;
	}

	void main(void *arg)
	{
		vector dir((void *)NULL);
		scalar pdf;
		const Globals *g = (const Globals *)glob;
		const eiLightSampler *sampler = light_sampler;
		const color result(g->values[sample(g, sampler->u1, sampler->u2, dir, pdf)]);
		dir = vfrom_world(dir);
		L = -dir;
		Cl = trace_shadow(P, dir * max_dist(), result);
		Lpdf = pdf;
	}

END_LIGHT(mia_physicalsun)

const char *mia_physicalsun::u_result = NULL;