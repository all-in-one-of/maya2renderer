/* 
 * Copyright 2010 elvish render Team
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at

 * http://www.apache.org/licenses/LICENSE-2.0

 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <eiAPI/ei_shaderx.h>
#include <shader/_3delight/shading_utils.h>
#include <shader/_3delight/utils.h>

SURFACE(maya_mib_amb_occlusion)
	DECLARE;
	DECLARE_INDEX(	i_samples,			16); 
	DECLARE_COLOR(	i_bright,			1.0f, 1.0f, 1.0f); 
	DECLARE_COLOR(	i_dark,				0.0f, 0.0f, 0.0f); 
	DECLARE_SCALAR(	i_spread,			0.8f); 
	DECLARE_SCALAR(	i_max_distance,		0.0f);
	DECLARE_BOOL(	i_reflective,		eiFALSE); 
	DECLARE_COLOR(	o_outValue,			0.0f, 0.0f, 0.0f);
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

	scalar occlusion(const point& p, const normal& N,
		const eiIndex samples, 
		const scalar max_distance,
		const scalar spread)
	{
		uint num_rays = max<eiIndex>(samples, 1);
		int num_hit = 0;

		UniformSampler<2> sampler(this, num_rays);

		while (sampler.sample())
		{
			vector dir = sample_cosine_hemisphere(point(0.0f), N, 1.0f, sampler[0], sampler[1]);
			dir = normalize((1.0f - spread) * N + spread * dir);

			if ( trace_probe(p, dir, max_distance ) )
			{
				++ num_hit;
			}
		}

		return (scalar)num_hit / (scalar)num_rays;
	}

	void main_er(void *arg)
	{
		if(i_max_distance() < eiSCALAR_EPS)//If it is zero, the entire scene is sampled
			i_max_distance() = eiMAX_SCALAR;

		normal Nf = faceforward(normalize(N), I);

		uint num_rays = max<int>(i_samples(), 1);
		int num_misses = 0;

		UniformSampler<2> sampler(this, num_rays);

		while (sampler.sample())
		{
			vector dir = sample_cosine_hemisphere(point(0.0f), Nf, 1.0f, sampler[0], sampler[1]);
			if (!trace_probe(P, dir, i_max_distance()))
			{
				++ num_misses;
			}
		}

		o_outValue() = color( (scalar)num_misses / (scalar)num_rays );
		
		//
		out->Ci = o_outValue();
		out->Oi = color(1.0f);

	}
	void main_3delight(void *arg)
	{	
		if(i_max_distance() < eiSCALAR_EPS)//If it is zero, the entire scene is sampled
			i_max_distance() = eiMAX_SCALAR;

		normal Nf = faceforward(normalize(N), I);
		normal Nn = normalize(Nf);

		if(i_reflective() != eiFALSE)
			Nn = reflect( -I, Nn );

		const scalar occ = 1.0f - occlusion(P, Nn, 
									i_samples(),
									i_max_distance(),
									i_spread()
								);

		o_outValue() = mix( i_dark(), i_bright(), occ );
		//
		out->Ci = o_outValue();
		out->Oi = color(1.0f);
	}

	void main(void *arg)
	{
		//main_er(arg);
		main_3delight(arg);
	}

END(maya_mib_amb_occlusion)
