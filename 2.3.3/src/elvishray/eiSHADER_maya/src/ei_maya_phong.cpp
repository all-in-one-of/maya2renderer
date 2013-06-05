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

#define SAMPLE_LIGHT_2(T, name, default_value, cmd)	\
	T last_##name = default_value;					\
	int num_samples_##name = 0;						\
	const int sample_slot = 4;						\
	while( sample_light() ){						\
		cmd;										\
		++ num_samples_##name;						\
		if ((num_samples_##name % sample_slot) == 0){		\
			const color current =							\
				name * (1.0f / (scalar)num_samples_##name);	\
			if (converged(current, last_##name))	\
				break;								\
			last_##name = current;					\
		}											\
	}												\
	name *= (1.0f / (scalar)num_samples_##name);



SURFACE(maya_phong)
	DECLARE;
	DECLARE_COLOR(	i_ambientColor,				0.0f, 0.0f, 0.0f); 	
	DECLARE_COLOR(	i_color,					0.5f, 0.5f, 0.5f); //Common Material Attributes - begin
	DECLARE_SCALAR(	i_cosinePower,				20.0f);				//Specular Shading - begin
	DECLARE_SCALAR(	i_diffuse,					0.8f);
	DECLARE_COLOR(	i_incandescence,			0.0f, 0.0f, 0.0f); 
	DECLARE_INDEX(  i_matteOpacityMode,			2);					//Matte Opacity - begin
	DECLARE_SCALAR(	i_matteOpacity,				1.0f);
	DECLARE_INT(	liq_UserDefinedNormal,		0);// use user defined normal or not, 1:yes, 0:no
	DECLARE_NORMAL(	i_normalCamera,				0.0f, 0.0f, 1.0f); //bump
	DECLARE_COLOR(	i_specularColor,			0.5f, 0.5f, 0.5f);
	DECLARE_SCALAR(	i_reflectivity,				0.5f);
	DECLARE_COLOR(	i_reflectedColor,			0.0f, 0.0f, 0.0f);
	DECLARE_INDEX(  i_reflectionLimit,			1);	//Raytrace Options - begin
	DECLARE_SCALAR(	i_translucence,				0.0f); 
	DECLARE_SCALAR(	i_translucenceDepth,		0.5f); 
	DECLARE_SCALAR(	i_translucenceFocus,		0.5f); 
	DECLARE_COLOR(	i_transparency,				0.0f, 0.0f, 0.0f); 
	DECLARE_COLOR(	o_outColor,					0.0f, 0.0f, 0.0f);	//output - begin
	DECLARE_COLOR(	o_outTransparency,			0.0f, 0.0f, 0.0f);
	DECLARE_OUT_COLOR(aov_ambient,				0.0f, 0.0f, 0.0f);
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
	//void main1(void *arg)
	//{
	//	color Kd = color(diffuse(), diffuse(), diffuse());

	//	normal Nf = faceforward(normalize(N()), I());
	//	vector V = -normalize(I());

	//	while (illuminance(P(), Nf, PI / 2.0f))
	//	{
	//		color	C = 0.0f;
	//		color	last = 0.0f;
	//		int		num_samples = 0;

	//		while (sample_light())
	//		{
	//			C += Cl() * (
	//				color_() * Kd * (normalize(L()) % Nf) 
	//				+ cosinePower() * specularColor() * specularbrdf(normalize(L()), Nf, V, 0.1f/*roughness()*/)
	//				);

	//			++ num_samples;

	//			if ((num_samples % 4) == 0)
	//			{
	//				color	current = C * (1.0f / (scalar)num_samples);

	//				if (converged(current, last)){
	//					break;
	//				}
	//				last = current;
	//			}
	//		}

	//		C *= (1.0f / (scalar)num_samples);
	//		Ci() += C;
	//	}

	//	if ( ! less_than( &transparency(), LIQ_SCALAR_EPSILON ) )
	//	{//transparent
	//		Ci() = Ci() * ( 1.0f - transparency() ) + trace_transparent() * transparency();
	//	}//else{ opacity }
	//	setOutputForMaya();
	//}
	//void main1_3(void *arg)
	//{
	//	color Kd = color(diffuse(), diffuse(), diffuse());

	//	normal Nf = faceforward(normalize(N()), I());
	//	vector V = -normalize(I());

	//	while (illuminance(P(), Nf, PI / 2.0f))
	//	{
	//		color C = 0.0f;
	//		SAMPLE_LIGHT_2(color, C, 0.0f,
	//			C += Cl() * (
	//				color_() * Kd * (normalize(L()) % Nf) 
	//				+ cosinePower() * specularColor() * specularbrdf(normalize(L()), Nf, V, 0.1f/*roughness()*/)
	//				)
	//			);

	//		Ci() += C;
	//	}

	//	if ( ! less_than( &transparency(), LIQ_SCALAR_EPSILON ) )
	//	{//transparent
	//		Ci() = Ci() * ( 1.0f - transparency() ) + trace_transparent() * transparency();
	//	}//else{ opacity }
	//	setOutputForMaya();
	//}
	//////////////////////////////////////////////////////////////////////////
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
	color getDiffuse(
		const normal& i_N,
		const eiBool keyLightsOnly,
		const eiBool unshadowed )
	{
		eiBool isKeyLight = eiTRUE;
		color C = 0.0f;

		LightSampler	sampler(this, P, i_N, PI/2.0f );


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
					//SAMPLE_LIGHT_2(color, C, 0.0f,
					//	C += Cl * (normalize(L) % i_N) * (1.0f-nondiffuse)						
					//);
					while (sampler.sample())
					{
						C += Cl * (normalize(L) % i_N) * (1.0f-nondiffuse);
					}
				}
			}

		return C;
	}


	color getPhong(
		const normal& i_N, const vector& i_V, const float cosinePower, 
		const eiBool i_keyLightsOnly, const eiBool unshadowed)
	{
		color C = 0.0f;
		vector R = reflect( normalize(i_V), normalize(i_N) );
		LightSampler	sampler(this, P, i_N, PI/2.0f );


			float isKeyLight = 1;
			//if( i_keyLightsOnly != 0 )
			//{
			//	lightsource( "iskeylight", isKeyLight );
			//}
			if( isKeyLight != 0 )
			{
				const float nonspecular = 0.0f;
				//lightsource( "__nonspecular", nonspecular );
				if( nonspecular < 1 )
				{
					//SAMPLE_LIGHT_2(color, C, 0.0f,
					//	C += Cl()*pow(max<float>(0.0f,R%Ln),cosinePower)*(1.0f-nonspecular);
					//);
					while (sampler.sample())
					{
						vector Ln = normalize(L);
						C += Cl*pow(max<float>(0.0f,R%Ln),cosinePower)*(1.0f-nonspecular);
					}
				}
			}

		return C;
	}
	//color getPhong2(
	//	const vector& i_N, const vector& i_V, const float cosinePower, 
	//	const eiBool i_keyLightsOnly, const eiBool unshadowed)
	//{
	//	color C  = 0.0f;
	//	vector R = reflect( normalize(i_V), normalize(i_N) );
	//	vector H = normalize(normalize(L()) + i_V);

	//	while( illuminance( P(), i_N, PI/2.0f ) )
	//	{
	//		const eiBool isKeyLight = eiTRUE;
	//		//if( i_keyLightsOnly != 0 )
	//		//{
	//		//	lightsource( "iskeylight", isKeyLight );
	//		//}
	//		if( isKeyLight != eiFALSE )
	//		{
	//			const float nonspecular = 0.0f;
	//			//lightsource( "__nonspecular", nonspecular );
	//			if( nonspecular < 1.0f )
	//			{
	//				vector Ln = normalize(L());
	//				SAMPLE_LIGHT_2(color, C, 0.0f,
	//					C += Cl()*pow(max<float>(0.0f,i_N%H),cosinePower)*(1.0f-nonspecular);
	//				);
	//			}
	//		}
	//	}
	//	return C;
	//}
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
				float nondiffuse = 0.0f;
				//lightsource( "__nondiffuse", nondiffuse );

				if( nondiffuse < 1.0f )
				{
					//SAMPLE_LIGHT_2(color, C, 0.0f,
					//	float costheta = normalize(L()) % normalize(I());
					//	float a = (1.0f + costheta) * 0.5f;
					//	float trs = pow( pow(a, focus), 1.0f/(1.0f-focus) );
					//	C += Cl() * trs * (1.0f-nondiffuse);
					//);
					while (sampler.sample())
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

// 		if( /*ray_coloration != color(0)*/!less_than(&ray_coloration, LIQ_SCALAR_EPSILON) 
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
// 		color refraction_color(1.0f);
// 		const scalar diffuse_weight = diffuse();
// 		const scalar specular_weight = 0.2f;
// 		const scalar reflection_weight = reflectivity();//0.0f for er
// 		const scalar refraction_weight = 0.0f;
// 		const scalar translucency_weight = 0.0f;
// 		const scalar diff = clamp(diffuse_weight, 0.0f, 1.0f);
// 		const scalar spec = clamp(specular_weight, 0.0f, 1.0f);
// 		const scalar refl = clamp(reflection_weight, 0.0f, 1.0f);
// 		const scalar refr = clamp(refraction_weight, 0.0f, 1.0f);
// 		const scalar trans = clamp(translucency_weight, 0.0f, 1.0f);
// 		const scalar refraction_thickness = 0.0f;
// 		// the energy balancing between reflection and refraction is 
// 		// dominated by Fresnel law
// 		color Kt(refraction_color * (spec * refr * (1.0f - trans)));
// 		color Kc(refraction_color * (spec * refr * trans));
// 		// non-reflected energy is absorbed
// 		color Ks(specularColor() * (spec * (1.0f - refl)));
// 		color Kr(reflectedColor() * (spec * refl));
		//if (ray_type == EI_RAY_SHADOW)
		//{
		//	main_shadow(arg, Kt, refraction_thickness, cutoff_thresh);
		//	return;
		//}

		o_outColor() = i_color() * out->Ci;


  		if( liq_UserDefinedNormal() == 0 )
  		{
  			i_normalCamera() = N;
  		}

		vector In = normalize( I );
		normal Nn = normalize( i_normalCamera() );
		normal Nf = ShadingNormal(Nn);
		
		vector V = -In;



		color Cdiffuse = i_diffuse() * getDiffuse(Nf, eiFALSE, eiFALSE);
		color Cambient = i_ambientColor() + getAmbient(Nf);
		color Cspecular = i_specularColor() * getPhong (Nf, V, i_cosinePower(), eiFALSE, eiFALSE);
		//color Cspecular   = specularColor() * getPhong2(Nf, V, cosinePower(), eiFALSE, eiFALSE);
		
		color Ctransl = getTranslucence(Nf, i_translucence(), i_translucenceDepth(), i_translucenceFocus());


		//out->Ci = color_();
		//out->Oi = transparency();	
		computeSurface(
			i_color(),//outColor(),//out->Ci,//
			i_transparency(),//out->Oi,//
			i_matteOpacityMode(),
			i_matteOpacity(),
			o_outColor(),//out->Ci,//
			o_outTransparency()//out->Oi//
		);

#ifdef USE_AOV_aov_ambient
		aov_ambient() += Cambient * i_color() * (1.0f - o_outTransparency());
#endif
#ifdef USE_AOV_aov_diffuse
		aov_diffuse() += Cdiffuse * i_color() * (1.0f - o_outTransparency());
#endif
#ifdef USE_AOV_aov_specular
		aov_specular() += Cspecular;
#endif

// 		if (true/*is_metal()*/)
// 		{
// 			Ks *= i_color();
// 			Kr *= i_color();
// 			Kt *= i_color();
// 			Kc *= i_color();
// 		}
// 		color Kd = i_color() *((1.0f - spec) * diff);


		color Creflect(0.0f);
// 		if( !almost_black(Kr) )
		{
			Creflect = /*Kr **/ getReflection(
			Nf, In, i_specularColor(), i_reflectivity(), i_reflectedColor(),
			//i_reflectionMaxDistance, i_reflectionSamples, i_reflectionBlur, i_reflectionNoiseAmplitude, i_reflectionNoiseFrequency,
			i_reflectionLimit(),
			wo, F, cutoff_thresh);
		}

		color refraction(0.0f);
// 		doRefraction(
// 			Nn(),
// 			I(),
// 			refractions,
// 			refractiveIndex,
// 			refractionLimit,
// 			lightAbsorbance,
// 			shadowAttenuation,
// 			outTransparency(),
// 			refraction );

		//Ci() += Kd * irradiance();
		o_outColor() *= (Cdiffuse + Cambient + Ctransl);
		o_outColor() += Creflect +  Cspecular + i_incandescence() + refraction;

		if ( ! less_than( &i_transparency(), LIQ_SCALAR_EPSILON ) )
		{//transparent
			o_outColor() = o_outColor() * ( 1.0f - i_transparency() ) + trace_transparent() * i_transparency();
		}//else{ opacity }

		//computeShadowPass(Nf);

		setOutputForMaya();
	}
	void setOutputForMaya()
	{
		//o_outColor() = out->Ci;
		//o_outTransparency() = out->Oi;
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
END(maya_phong)
