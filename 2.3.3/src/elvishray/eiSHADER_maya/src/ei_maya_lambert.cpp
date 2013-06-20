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
#include <shader/_3delight/shading_utils.h>
#include <shader/_3delight/utils.h>
#include <shader/my_utils.h>


SURFACE(maya_lambert)
	DECLARE;
	DECLARE_COLOR(	i_ambientColor,				0.0f, 0.0f, 0.0f); //Common Material Attributes - begin
	DECLARE_COLOR(	i_color,					0.5f, 0.5f, 0.5f); 
	DECLARE_SCALAR(	i_diffuse,					0.8f);	
	DECLARE_COLOR(	i_incandescence,			0.0f, 0.0f, 0.0f); 
	DECLARE_INDEX(  i_matteOpacityMode,			2);	//Matte Opacity - begin
	DECLARE_SCALAR(	i_matteOpacity,				1.0f);
	DECLARE_BOOL(	i_refractions,				0);/* Refraction. */
	DECLARE_SCALAR(	i_refractiveIndex,			1.0f);
	DECLARE_INDEX(	i_refractionLimit,			6);
	DECLARE_SCALAR(	i_lightAbsorbance,			0.0f);
	DECLARE_SCALAR(	i_shadowAttenuation,		0.5f);
	DECLARE_INT(	liq_UserDefinedNormal,		0);// use user defined normal or not, 1:yes, 0:no
	DECLARE_NORMAL(	i_normalCamera,				0.0f, 0.0f, 1.0f); //bump
	DECLARE_COLOR(	i_transparency,				0.0f, 0.0f, 0.0f); 
	DECLARE_SCALAR(	i_translucence,				0.0f); 
	DECLARE_SCALAR(	i_translucenceDepth,		0.5f); 
	DECLARE_SCALAR(	i_translucenceFocus,		0.5f); 
	DECLARE_COLOR(	o_outColor,					0.0f, 0.0f, 0.0f);	//output - begin
	DECLARE_COLOR(	o_outTransparency,			0.0f, 0.0f, 0.0f);
	DECLARE_OUT_COLOR(aov_ambient,				0.0f, 0.0f, 0.0f);  //AOVs
	DECLARE_OUT_COLOR(aov_diffuse,				0.0f, 0.0f, 0.0f);
	DECLARE_OUT_COLOR(aov_specular,				0.0f, 0.0f, 0.0f);
	END_DECLARE;

	static void init()	{}
	static void exit()	{}

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
// 		if (almost_equal(raster.x, 149.0f, 0.5f) 
// 			&& almost_equal(raster.y,  51.0f, 0.5f))
// 		{
// 			bool debugnow = true;
// 		}
// 
// 		if (almost_equal(raster.x, 140.0f, 0.5f) 
// 			&& almost_equal(raster.y,  67.0f, 0.5f))
// 		{
// 			bool debugnow = true;
// 		}

		// prepare outgoing direction in local frame
		const vector V(-normalize(I));
		const vector wo(normalize(to_local(V)));

		scalar fresn_0_degree_refl = 0.2f;//fresnel_0_degree_refl();//
		scalar fresn_90_degree_refl = 1.0f;//fresnel_90_degree_refl();//
		scalar fresn_curve = 5.0f;//fresnel_curve();//

		union {
			eiByte by_ior[sizeof(FresnelByIOR)];
			eiByte schlick[sizeof(FresnelSchlick)];
		} F_storage;
		//
// 		union {
// 			eiByte by_ior[sizeof(FresnelByIOR)];
// 			eiByte schlick[sizeof(FresnelSchlick)];
// 		} invF_storage;

		Fresnel *F = new (F_storage.schlick) FresnelSchlick(
			fresn_0_degree_refl, 
			fresn_90_degree_refl, 
			fresn_curve);

		const scalar cutoff_thresh = 0.01f;//cutoff_threshold();//

		//main1(arg);
		//main1_3(arg);
		main2(arg, wo, F, cutoff_thresh);

	}
	//
	normal ShadingNormal(const normal& i_N)
	{
		normal Nf = i_N;

		//const int sides = 2;
		if( true/*sides == 2*/ )
		{
			Nf = faceforward(Nf, I);
		}
		return Nf;
	}
	//
	color getDiffuse(
		LightSampler &sampler, 
		const normal& i_N,
		const eiBool keyLightsOnly,
		const eiBool unshadowed )
	{
		eiBool isKeyLight = eiTRUE;
		color C = 0.0f;

		//LightSampler	sampler(this, P, i_N, PI/2.0f );
		while (sampler.sample())
		{
			//if( keyLightsOnly != eiFALSE )
			//{
			//	isKeyLight = eiFALSE;
			//	lightsource( "iskeylight", isKeyLight );
			//}
			if( isKeyLight != eiFALSE )
			{
				float nondiffuse = 0.0f;
				//lightsource( "__nondiffuse", nondiffuse );

				if( nondiffuse < 1.0f )
				{
					/*varying */color cur_cl;
					GET_CL(cur_cl);
					C += cur_cl * (normalize(L) % i_N) * (1.0f-nondiffuse);
				}
			}
		}

		return C;
	}
	//
	color getTranslucence(const normal& i_N, 
		const float i_translucence,
		const float i_translucenceDepth,
		const float i_translucenceFocus)
	{
		/*
		A translucence focus of 1 leads to a division by zero and an effective
		focus of 0. Clamping it like this yields about the same result as maya.
		*/
		float focus = min( i_translucenceFocus, 0.99999f );
		color C = 0.0f;

		LightSampler	sampler(this, P );

		if( i_translucence > 0.0f )
		{
			while (sampler.sample())
			{
				float nondiffuse = 0.0f;
				//lightsource( "__nondiffuse", nondiffuse );

				if( nondiffuse < 1.0f )
				{
					float costheta = normalize(L) % normalize(I);
					float a = (1.0f + costheta) * 0.5f;
					float trs = pow( pow(a, focus), 1.0f/(1.0f-focus) );
					C += Cl * trs * (1.0f-nondiffuse);
				}
			}
		}

		return C * i_translucence;
	}
	//
	void main2(void *arg, const vector &wo, Fresnel *F, const scalar cutoff_thresh)
	{
		o_outColor() = i_color() * out->Ci;

  		if( liq_UserDefinedNormal() == 0 )
  		{
  			i_normalCamera() = N;
  		}

		normal Nn = normalize( i_normalCamera() );
		normal Nf = ShadingNormal(Nn);

		// TODO: Only need to build this sampler when either i_diffuse or i_specularColor is non-zero
		LightSampler	sampler(this, P, Nf, PI/2.0f );

		color Ia = i_ambientColor() + getAmbient(Nf);
		color Id = i_diffuse() * getDiffuse(sampler, Nf, eiFALSE, eiFALSE);
		color Itr = getTranslucence(
			Nf, i_translucence(), i_translucenceDepth(), i_translucenceFocus() );
		
		computeSurface(
			i_color(),
			i_transparency(),
			i_matteOpacityMode(),
			i_matteOpacity(),
			o_outColor(),
			o_outTransparency()
		);

		//Id = getGlobalIllumination(Id, o_outTransparency);

		//extern color __transparency;

#ifdef USE_AOV_aov_ambient
		aov_ambient() += /*__transparency **/ Ia * i_color() * (1.0f - o_outTransparency());
#endif
#ifdef USE_AOV_aov_diffuse
		aov_diffuse() += /*__transparency **/ Id * i_color() * (1.0f - o_outTransparency());
#endif

		color refraction(0.0f);

// 		/*varying*/ float tir = doRefraction(
// 			Nn(),
// 			I(),
// 			i_refractions,
// 			i_refractiveIndex,
// 			i_refractionLimit,
// 			i_lightAbsorbance,
// 			i_shadowAttenuation,
// 			o_outTransparency(),
// 			refraction );



		o_outColor() = o_outColor() * (Ia + Id + Itr) + i_incandescence() + refraction;

		if ( ! less_than( &i_transparency(), LIQ_SCALAR_EPSILON ) )
		{//transparent
			o_outColor() *= ( 1.0f - i_transparency() ) + trace_transparent() * i_transparency();
		}//else{ opacity }

//		computeShadowPass(Nf);

//		__transparency *= o_outTransparency;

		setOutputForMaya();
	}
	void setOutputForMaya()
	{
		out->Ci = o_outColor();
		out->Oi = o_outTransparency();
	}
	void main_shadow(void *arg, const color & Kt, 
		const scalar &refraction_thickness, 
		const scalar &cutoff_threshold)
	{
		// the light has falled off to almost black, 
		// no need to trace
		if (almost_black(Cl))
		{
			return;
		}

		scalar transp = average(&Kt);

		// we are less transparent if the surface is treated as two sided
		if (refraction_thickness > 0.0f)
		{
			transp = transp * transp;
		}

		// the same with opaque shader
		if (almost_zero(transp, cutoff_threshold))
		{
			Cl = color(0.0f);
			Ol = color(1.0f);
			return;
		}

		// reduce the length of ray direction to limit the maximum distance
		Cl = trace_shadow(P, I - (P - E), Cl) * transp;
		Ol = (1.0f - transp);
	}
END(maya_lambert)
