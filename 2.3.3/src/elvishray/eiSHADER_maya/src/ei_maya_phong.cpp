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
	DECLARE_COLOR(	color_,						0.5f, 0.5f, 0.5f); //Common Material Attributes - begin
	DECLARE_COLOR(	transparency,				0.0f, 0.0f, 0.0f); 
	DECLARE_COLOR(	ambientColor,				0.0f, 0.0f, 0.0f); 
	DECLARE_COLOR(	incandescence,				0.0f, 0.0f, 0.0f); 
	DECLARE_NORMAL(	normalCamera,				0.0f, 0.0f, 1.0f); //bump
	DECLARE_SCALAR(	diffuse,					0.8f);
	DECLARE_SCALAR(	translucence,				0.0f); 
	DECLARE_SCALAR(	translucenceDepth,			0.5f); 
	DECLARE_SCALAR(	translucenceFocus,			0.5f); 
	DECLARE_SCALAR(	cosinePower,				20.0f);				//Specular Shading - begin
	DECLARE_COLOR(	specularColor,				0.5f, 0.5f, 0.5f);
	DECLARE_SCALAR(	reflectivity,				0.5f);
	DECLARE_COLOR(	reflectedColor,				0.0f, 0.0f, 0.0f);
	DECLARE_INDEX( matteOpacityMode,			2);					//Matte Opacity - begin
	DECLARE_SCALAR(	matteOpacity,				1.0f);
	DECLARE_INDEX( reflectionLimit,				1);					//Raytrace Options - begin
	DECLARE_COLOR(	outColor,					0.0f, 0.0f, 0.0f);	//output - begin
	DECLARE_COLOR(	outTransparency,			0.0f, 0.0f, 0.0f);
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

	//	if ( ! less_than( &transparency(), LIQ_SCALAR_ALMOST_ZERO ) )
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

	//	if ( ! less_than( &transparency(), LIQ_SCALAR_ALMOST_ZERO ) )
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

		outColor() = color_() * out->Ci;



		vector In = normalize( I );
		normal Nn = normalize( N );
		normal Nf = ShadingNormal(Nn);
		
		vector V = -In;



		color Cdiffuse = diffuse() * getDiffuse(Nf, eiFALSE, eiFALSE);
		color Cambient = ambientColor() + getAmbient(Nf);
		color Cspecular = specularColor() * getPhong (Nf, V, cosinePower(), eiFALSE, eiFALSE);
		//color Cspecular   = specularColor() * getPhong2(Nf, V, cosinePower(), eiFALSE, eiFALSE);
		
		color Ctransl = getTranslucence(Nf, translucence(), translucenceDepth(), translucenceFocus());


		//out->Ci = color_();
		//out->Oi = transparency();	
		computeSurface(
			color_(),//outColor(),//out->Ci,//
			transparency(),//out->Oi,//
			matteOpacityMode(),
			matteOpacity(),
			outColor(),//out->Ci,//
			outTransparency()//out->Oi//
		);

// 		if (true/*is_metal()*/)
// 		{
// 			Ks *= color_();
// 			Kr *= color_();
// 			Kt *= color_();
// 			Kc *= color_();
// 		}
// 		color Kd = color_() *((1.0f - spec) * diff);


		color Creflect(0.0f);
// 		if( !almost_black(Kr) )
		{
			Creflect = /*Kr **/ getReflection(
			Nf, In, specularColor(), reflectivity(), reflectedColor(),
			//i_reflectionMaxDistance, i_reflectionSamples, i_reflectionBlur, i_reflectionNoiseAmplitude, i_reflectionNoiseFrequency,
			reflectionLimit(),
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
		outColor() *= (Cdiffuse + Cambient + Ctransl);
		outColor() += Creflect +  Cspecular + incandescence() + refraction;

		if ( ! less_than( &transparency(), LIQ_SCALAR_ALMOST_ZERO ) )
		{//transparent
			outColor() = outColor() * ( 1.0f - transparency() ) + trace_transparent() * transparency();
		}//else{ opacity }

		//computeShadowPass(Nf);

		setOutputForMaya();
	}
	void setOutputForMaya()
	{
		//outColor() = out->Ci;
		//outTransparency() = out->Oi;
		out->Ci = outColor();
		out->Oi = outTransparency();
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
