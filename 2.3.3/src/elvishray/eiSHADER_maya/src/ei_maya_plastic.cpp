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
#include "ei_AOVMacroDef.h"
#include "common/_3delight/shading_utils.h"
#include "common/_3delight/utils.h"
#include "common/my_utils.h"

//SURFACE(maya_lambert)
//	DECLARE;
//	DECLARE_COLOR(Cs, 1.0f, 1.0f, 1.0f);
//	DECLARE_COLOR(Kd, 1.0f, 1.0f, 1.0f);
//	DECLARE_SCALAR(Ks, 0.5f);
//	DECLARE_SCALAR(roughness, 0.1f);
//	DECLARE_COLOR(specularcolor, 1.0f, 1.0f, 1.0f);
//	DECLARE_TAG(Cs_tex, eiNULL_TAG);
//	END_DECLARE;
//
//	static void init()
//	{
//	}
//
//	static void exit()
//	{
//	}
//
//	void init_node()
//	{
//	}
//
//	void exit_node()
//	{
//	}
//
//	color specularbrdf(const vector & vL, const normal & vN, const vector & V, scalar roughness)
//	{
//		vector	H = normalize(vL + V);
//		scalar	dotNH = vN % H;
//		return powf(max(eiSCALAR_EPS, dotNH), 1.0f / roughness);
//	}
//
//	void main(void *arg)
//	{
//		vector V = -normalize(I);
//
//		out->Ci = 0.0f;
//
//		eiTag tex = Cs_tex();
//
//		if (tex != eiNULL_TAG)
//		{
//			Cs() = color_texture(tex, 0, bary.x, bary.y);
//		}
//
//		color kd = Cs() * Kd();
//		color ks = Ks() * specularcolor();
//		scalar rough = roughness();
//
//		LightSampler	sampler(this, P, N, PI / 2.0f);
//
//
//			color	sum = 0.0f;
//			
//			while (sampler.sample())
//			{
//				sum += Cl * (
//					kd * (normalize(L) % N) 
//					+ ks * specularbrdf(normalize(L), N, V, rough));
//			}
//
//			out->Ci += sum;
//
//
//		out->Oi = color(1.0f);
//	}
//
//END(maya_lambert)

SURFACE(maya_lambert_uv)
	DECLARE;
	DECLARE_COLOR(Cs, 1.0f, 1.0f, 1.0f);
	DECLARE_COLOR(Kd, 1.0f, 1.0f, 1.0f);
	DECLARE_SCALAR(Ks, 0.5f);
	DECLARE_SCALAR(roughness, 0.1f);
	DECLARE_COLOR(specularcolor, 1.0f, 1.0f, 1.0f);
	DECLARE_TAG(Cs_tex, eiNULL_TAG);
	END_DECLARE;

	color specularbrdf(const vector & vL, const normal & vN, const vector & V, scalar roughness)
	{
		vector	H = normalize(vL + V);
		scalar	dotNH = vN % H;
		return powf(max(eiSCALAR_EPS, dotNH), 1.0f / roughness);
	}

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

	void main(void *arg)
	{
		vector V = -normalize(I);

		out->Ci = 0.0f;

		eiTag tex = Cs_tex();

		if (tex != eiNULL_TAG)
		{
			const scalar tiling = 4.0f;

			scalar s = fmod1(u * tiling);
			scalar t = fmod1(v * tiling);
			scalar dsdx = Dx(u);
			scalar dsdy = Dy(u);
			scalar dtdx = Dx(v);
			scalar dtdy = Dy(v);
			dsdx = dsdx * tiling;
			dsdy = dsdy * tiling;
			dtdx = dtdx * tiling;
			dtdy = dtdy * tiling;
			//Cs() = color_texture(tex, 0, s, t);
			Cs() = color_texture(tex, 0, s, t, dsdx, dsdy, dtdx, dtdy, 8.0f);
			
			// visualize ray differentials
			/*if (dsdy > 0.0f)
			{
				Cs() = color(absf(dsdy), 0.0f, 0.0f);
			}
			else
			{
				Cs() = color(0.0f, 0.0f, absf(dsdy));
			}*/
		}

		color kd = Cs() * Kd();
		color ks = Ks() * specularcolor();
		scalar rough = roughness();

		LightSampler	sampler(this, P, N, PI / 2.0f);


			color	sum = 0.0f;
			
			while (sampler.sample())
			{
				sum += Cl * (
					kd * (normalize(L) % N) 
					+ ks * specularbrdf(normalize(L), N, V, rough));
			}

			out->Ci += sum;


		out->Oi = color(1.0f);
	}

END(maya_lambert_uv)

SURFACE(simple_hair)
	DECLARE;
	DECLARE_COLOR(Cs, 1.0f, 1.0f, 1.0f);
	DECLARE_COLOR(Kd, 1.0f, 1.0f, 1.0f);
	DECLARE_SCALAR(Ks, 0.5f);
	DECLARE_COLOR(specularcolor, 1.0f, 1.0f, 1.0f);
	DECLARE_SCALAR(specularpower, 60.0f);
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

	void main(void *arg)
	{
		vector V = -normalize(I);

		out->Ci = 0.0f;
		vector tangent = normalize(dPdu);
		scalar spec_n = specularpower();
		color kd = Kd();
		color ks = Ks() * specularcolor();

		LightSampler	sampler(this, P, N, PI);


			color	sum = 0.0f;
			
			while (sampler.sample())
			{
				vector lightVector = normalize(L);
				scalar diff = sinf(acosf(tangent % lightVector));
				scalar spec = powf((tangent % lightVector) * (tangent % V) 
					+ sinf(acosf(lightVector % tangent)) * sinf(acosf(tangent % V)), spec_n);

				sum += Cl * (kd * diff + ks * spec);
			}

			out->Ci += sum;


		out->Ci *= Cs();
		out->Oi = color(1.0f);
	}

END(simple_hair)

DISPLACEMENT(simple_displace)
	DECLARE;
	DECLARE_TAG(disp_tex, eiNULL_TAG);
	DECLARE_SCALAR(disp_dist, 1.0f);
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

	void main(void *arg)
	{
		eiTag tex = disp_tex();
		scalar dist = disp_dist();
		//scalar offset = 0.0f;

		//if (tex != eiNULL_TAG)
		//{
		//offset = scalar_texture(tex, 0, bary.x, bary.y);
		//}

		//P = P + dist * offset * N;
		P = P + dist * noise(P);
	}

END(simple_displace)


SURFACE(maya_lambert_transparent)
	DECLARE;
	DECLARE_COLOR(Cs, 1.0f, 1.0f, 1.0f);
	DECLARE_COLOR(Kd, 1.0f, 1.0f, 1.0f);
	DECLARE_SCALAR(Ks, 0.5f);
	DECLARE_SCALAR(roughness, 0.1f);
	DECLARE_COLOR(specularcolor, 1.0f, 1.0f, 1.0f);
	DECLARE_COLOR(	transparency,				0.0f, 0.0f, 0.0f); 
	DECLARE_COLOR(	ambientColor,				0.0f, 0.0f, 0.0f); 
	DECLARE_COLOR(	incandescence,				0.0f, 0.0f, 0.0f); 
	DECLARE_SCALAR(diffusion, 0.8f);
	DECLARE_TAG(Cs_tex, eiNULL_TAG);
	DECLARE_COLOR(	outColor,					0.0f, 0.0f, 0.0f); 
	DECLARE_COLOR(	outTransparency,			0.0f, 0.0f, 0.0f); 
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

	color specularbrdf(const vector & vL, const normal & vN, const vector & V, scalar roughness)
	{
		vector	H = normalize(vL + V);
		scalar	dotNH = vN % H;
		return pow(max(eiSCALAR_EPS, dotNH), 1.0f / roughness);
	}

	void main(void *arg)
	{
		vector V = -normalize(I);

		out->Ci = 0.0f;

		eiTag tex = Cs_tex();

		if (tex != eiNULL_TAG)
		{
			const scalar tiling = 4.0f;

			scalar s = fmod1(u * tiling);
			scalar t = fmod1(v * tiling);
			scalar dsdx = Dx(u);
			scalar dsdy = Dy(u);
			scalar dtdx = Dx(v);
			scalar dtdy = Dy(v);
			dsdx = dsdx * tiling;
			dsdy = dsdy * tiling;
			dtdx = dtdx * tiling;
			dtdy = dtdy * tiling;
			//Cs() = color_texture(tex, 0, s, t);
			Cs() = color_texture(tex, 0, s, t, dsdx, dsdy, dtdx, dtdy, 8.0f);

			// visualize ray differentials
			/*if (dsdy > 0.0f)
			{
			Cs() = color(absf(dsdy), 0.0f, 0.0f);
			}
			else
			{
			Cs() = color(0.0f, 0.0f, absf(dsdy));
			}*/
		}

		color kd = Cs() * Kd();
		color ks = Ks() * specularcolor();
		scalar rough = roughness();
		LightSampler	sampler(this, P, N, PI / 2.0f);


			color	sum = 0.0f;

			while (sampler.sample())
			{
				sum += Cl * (
					kd * (normalize(L) % N) 
					+ ks * specularbrdf(normalize(L), N, V, rough));
			}

			out->Ci += sum;


		if ( ! less_than( &transparency(), LIQ_SCALAR_ALMOST_ZERO ) )
		{
			out->Ci = out->Ci * ( 1.0f - transparency() ) + trace_transparent() * transparency();
		}

		out->Oi = color(1.0f);
	}

END(maya_lambert_transparent)
