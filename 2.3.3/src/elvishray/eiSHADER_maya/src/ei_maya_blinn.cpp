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


SURFACE(maya_blinn)
	DECLARE;
	DECLARE_COLOR(	i_ambientColor,				0.0f, 0.0f, 0.0f); //Common Material Attributes - begin
	DECLARE_COLOR(	i_color,					0.5f, 0.5f, 0.5f); 
	DECLARE_SCALAR(	i_diffuse,					0.8f);	
	DECLARE_SCALAR(	i_eccentricity,				0.3f);
	DECLARE_COLOR(	i_incandescence,			0.0f, 0.0f, 0.0f); 
	DECLARE_INDEX( i_matteOpacityMode,			2);	//Matte Opacity - begin
	DECLARE_SCALAR(	i_matteOpacity,				1.0f);
	DECLARE_COLOR(	i_specularColor,			0.5f, 0.5f, 0.5f);
	DECLARE_SCALAR(	i_specularRollOff,			0.7f);
	DECLARE_SCALAR(	i_reflectivity,				0.5f);
	DECLARE_COLOR(	i_reflectedColor,			0.0f, 0.0f, 0.0f);
	DECLARE_BOOL(	i_refractions,				0);/* Refraction. */
	DECLARE_SCALAR(	i_refractiveIndex,			1.0f);
	DECLARE_INDEX(	i_refractionLimit,			6);
	DECLARE_SCALAR(	i_lightAbsorbance,			0.0f);
	DECLARE_SCALAR(	i_shadowAttenuation,		0.5f);
	DECLARE_INDEX(	i_reflectionLimit,			1);/* Reflection. */
	DECLARE_SCALAR(	i_translucence,				0.0f); 
	DECLARE_SCALAR(	i_translucenceDepth,		0.5f); 
	DECLARE_SCALAR(	i_translucenceFocus,		0.5f); 
	DECLARE_COLOR(	i_transparency,				0.0f, 0.0f, 0.0f); 
	DECLARE_INT(	liq_UserDefinedNormal,		0);// use user defined normal or not, 1:yes, 0:no
	DECLARE_NORMAL(	i_normalCamera,				0.0f, 0.0f, 1.0f); //bump
	DECLARE_SCALAR(	i_reflectionMaxDistance,	0.0f);//3delight defined parameter
	DECLARE_SCALAR(	i_reflectionSamples,		0.5f);//3delight defined parameter
	DECLARE_SCALAR(	i_reflectionBlur,			0.5f);//3delight defined parameter
	DECLARE_SCALAR(	i_reflectionNoiseAmplitude,	0.5f);//3delight defined parameter
	DECLARE_SCALAR(	i_reflectionNoiseFrequency,	0.5f);//3delight defined parameter
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
		// prepare outgoing direction in local frame
		const vector V(-normalize(I));
		const vector wo(to_local(V));

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
		const normal& i_N,
		const eiBool keyLightsOnly,
		const eiBool unshadowed )
	{
		eiBool isKeyLight = eiTRUE;
		color C = 0.0f;

		LightSampler	sampler(this, P, i_N, PI/2.0f );
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
	color getBlinn(
		const normal& i_Nf, 
		const float i_eccentricity,
		const float i_specularRollOff, 
		const eiBool i_keyLightsOnly,
		const eiBool unshadowed)
	{
		float E;
		color C = 0.0f;
		vector H, Ln, V, Nn;
		float NH, NH2, NHSQR, Dd, Gg, VN, VH, LN, Ff, tmp;

		if( !almost_equal(i_eccentricity, 1.0f, LIQ_SCALAR_ALMOST_ZERO ) )
			E = 1.0f / (SQR(i_eccentricity) - 1.0f);
		else
			E = -1.0 * 100000.0f;

		V = normalize(-I);
		VN = V % i_Nf;

		//vector R = reflect( normalize(i_V), normalize(i_N) );
		LightSampler	sampler(this, P, i_Nf, PI/2.0f );
		while (sampler.sample())
		{
			eiBool isKeyLight = eiTRUE;
			//if( i_keyLightsOnly != 0 )
			//{
			//	lightsource( "iskeylight", isKeyLight );
			//}
			if( isKeyLight != eiFALSE )
			{
				const float nonspecular = 0.0f;
				//lightsource( "__nonspecular", nonspecular );
				if( nonspecular < 1.0f )
				{
					Ln = normalize(L);
					H = normalize(Ln + V);
					NH = i_Nf % H;
					NHSQR = SQR(NH);
					NH2 = NH * 2.0f;
					Dd = (E + 1.0f) / (NHSQR + E);
					Dd *= Dd;
					VH = V % H;
					LN = Ln % i_Nf;
					if(VN < LN)
					{
						if(VN * NH2 < VH)
							Gg = NH2 / VH;
						else
							Gg = 1.0f / VN;
					}
					else
					{
						if(LN * NH2 < VH)
							Gg = (LN * NH2) / (VH * VN);
						else
							Gg = 1.0f / VN;
					}
					/* poor man's Fresnel */
					tmp = pow((1.0f - VH), 3.0f);
					Ff = tmp + (1.0f - tmp) * i_specularRollOff;

					/*varying */color cur_cl;
					GET_CL(cur_cl);
					C += cur_cl * Dd * Gg * Ff * (1.0f - nonspecular);
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
	color getReflection(
		const normal& i_N,
		const vector& i_I,
		const color& i_specularColor,
		const float i_reflectivity,
		const color& i_reflectedColor,
		//const float i_maxDistance, const float i_samples, const float i_blur, const float i_noiseAmp, const float i_noiseFreq,
		const eiIndex i_reflectionLimit,
		const vector &wo, Fresnel *F, const scalar cutoff_thresh)
	{
		color ray_coloration = i_specularColor * i_reflectivity;
		color reflected = i_reflectedColor;

// 		if( /*ray_coloration != color(0)*/!less_than(&ray_coloration, LIQ_SCALAR_ALMOST_ZERO) 
// 			&&
// 			/*raySpecularDepth() < i_reflectionLimit*/eiTRUE )
// 		{
//			vector R = reflect( i_I, i_N );

			//if( i_noiseAmp != 0 && i_noiseFreq != 0)
			//{
			//	point Pobj = transform("object", P);
			//	R = mix( R, R * noise( Pobj * i_noiseFreq ), i_noiseAmp );
			//}
			//color rc = trace_probe(P(), R,i_maxDistance);
// 			color rc = trace_reflection(R);
// 			color trs = trace_transmission(R);
// 
// 			reflected *= trs;
// 			reflected += rc;

			//color Kr(i_reflectedColor * (/*spec*/1.0f * i_reflectivity));
			//color Kr(ray_coloration);

			SpecularReflection Rr(F);

			// integrate perfect specular reflection
			if (dot_nd < 0.0f)
			{
				IntegrateOptions opt;
				opt.ray_type = EI_RAY_REFLECT_GLOSSY;
				opt.min_samples = opt.max_samples = 1; // force one sample for reflection
				opt.cutoff_threshold = cutoff_thresh;
				// the direct lighting of this BRDF is not accounted, 
				// so we trace lights here to compensate
				opt.trace_lights = eiTRUE;

				reflected += integrate(wo, Rr, opt);
			}
		//}
		return reflected * ray_coloration;
	}
	void main2(void *arg, const vector &wo, Fresnel *F, const scalar cutoff_thresh)
	{
		normal Nf, Nn;
		color Ia, Id, Itr, Is, Ir, Isr;

		o_outColor() = i_color() * out->Ci;


  		if( liq_UserDefinedNormal() == 0 )
  		{
  			i_normalCamera() = N;
  		}

		vector In = normalize( I );
		Nn = normalize( i_normalCamera() );
		Nf = ShadingNormal(Nn);
		Ia = i_ambientColor() + getAmbient(Nf);
		Id = i_diffuse() * getDiffuse(Nf, eiFALSE, eiFALSE);
		Itr = getTranslucence(
			Nf, i_translucence(), i_translucenceDepth(), i_translucenceFocus() );
		Is = i_specularColor() * getBlinn(Nf, i_eccentricity(), i_specularRollOff(), eiFALSE, eiFALSE);
		
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
#ifdef USE_AOV_aov_specular
		aov_specular() += /*__transparency **/ Is;
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

// 		if( tir > 0 )
// 		{
// 			Ir = getReflection(
//				Nf, I, 
//				1, // i_specularColor
//				1, // i_relfectivity
//				0, // i_reflectedColor
//				i_reflectionMaxDistance, i_reflectionSamples, i_reflectionBlur,
//				i_reflectionNoiseAmplitude, i_reflectionNoiseFrequency,
//				i_refractionLimit );
// 		}
// 		else
 		{
 			Ir = getReflection(
 				Nf, I, i_specularColor(),
 				i_reflectivity() * i_specularRollOff(), i_reflectedColor(),
 				//i_reflectionMaxDistance, i_reflectionSamples, i_reflectionBlur, i_reflectionNoiseAmplitude, i_reflectionNoiseFrequency,
 				i_reflectionLimit(),
 				wo, F, cutoff_thresh);
 		}



		o_outColor() *= (Ia + Id + Itr);
		o_outColor() += Ir + Is + i_incandescence() + refraction;

		if ( ! less_than( &i_transparency(), LIQ_SCALAR_ALMOST_ZERO ) )
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
END(maya_blinn)
