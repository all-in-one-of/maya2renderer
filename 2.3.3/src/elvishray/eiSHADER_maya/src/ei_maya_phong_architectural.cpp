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
#include "ei_AOVMacroDef.h"
#include "common/_3delight/shading_utils.h"
#include "common/my_utils.h"
#include <new>




SURFACE(maya_phong_architectural)

	DECLARE;
	DECLARE_COLOR(	i_ambientColor,				0.0f, 0.0f, 0.0f); 
	DECLARE_COLOR(	i_color,					0.5f, 0.5f, 0.5f); //Common Material Attributes - begin
	DECLARE_SCALAR(	i_cosinePower,				20.0f);	
	DECLARE_SCALAR(	i_diffuse,					0.8f);
	DECLARE_COLOR(	i_incandescence,				0.0f, 0.0f, 0.0f);
	DECLARE_INDEX(  i_matteOpacityMode,			2);					//Matte Opacity - begin
	DECLARE_SCALAR(	i_matteOpacity,				1.0f);
	DECLARE_INT(	liq_UserDefinedNormal,		0);// use user defined normal or not, 1:yes, 0:no
	DECLARE_NORMAL(	i_normalCamera,				0.0f, 0.0f, 0.0f); //bump
	DECLARE_COLOR(	i_specularColor,				0.5f, 0.5f, 0.5f);
	DECLARE_SCALAR(	i_reflectivity,				0.5f);
	DECLARE_COLOR(	i_reflectedColor,				0.0f, 0.0f, 0.0f);
	DECLARE_INDEX(  i_reflectionLimit,				1);					//Raytrace Options - begin
	DECLARE_SCALAR(	i_translucence,				0.0f); 
	DECLARE_SCALAR(	i_translucenceDepth,			0.5f); 
	DECLARE_SCALAR(	i_translucenceFocus,			0.5f); 
	DECLARE_COLOR(	i_transparency,				0.0f, 0.0f, 0.0f); 
	DECLARE_COLOR(	o_outColor,					0.0f, 0.0f, 0.0f);	//output - begin
	DECLARE_COLOR(	o_outTransparency,			0.0f, 0.0f, 0.0f);
	DECLARE_OUT_COLOR(aov_ambient,				0.0f, 0.0f, 0.0f);
	DECLARE_OUT_COLOR(aov_diffuse,				0.0f, 0.0f, 0.0f);
	DECLARE_OUT_COLOR(aov_specular,				0.0f, 0.0f, 0.0f);
	END_DECLARE;

	static const char *u_result;

	static void init()
	{
		u_result = ei_token("result");
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

	void call_bump_shader(const eiTag shader, const scalar factor)
	{
		color	C, Cx, Cy;
		scalar	Dudx = Dx(u);
		scalar	Dudy = Dy(u);
		scalar	Dvdx = Dx(v);
		scalar	Dvdy = Dy(v);

		eiShaderOutput outputs[] = {
			{EI_TYPE_VECTOR, u_result, &C}, 
		};
		call_shader(shader, 1, outputs, NULL);

		point old_P = P;
		scalar old_u = u;
		scalar old_v = v;

		P = old_P + dPdx;
		u = old_u + Dudx;
		v = old_v + Dvdx;

		eiShaderOutput outputs_x[] = {
			{EI_TYPE_VECTOR, u_result, &Cx}, 
		};
		call_shader(shader, 1, outputs_x, NULL);

		P = old_P + dPdy;
		u = old_u + Dudy;
		v = old_v + Dvdy;

		eiShaderOutput outputs_y[] = {
			{EI_TYPE_VECTOR, u_result, &Cy}, 
		};
		call_shader(shader, 1, outputs_y, NULL);

		P = old_P;
		u = old_u;
		v = old_v;

		scalar s = average(&C);
		scalar sx = average(&Cx);
		scalar sy = average(&Cy);

		N = normalize(N + factor * (dPdx * (sx - s) + dPdy * (sy - s)));
	}

	eiFORCEINLINE vector rotate_vector(const vector & p, const vector & a, scalar rdn)
	{
	   const scalar c = fastcosfull(rdn);
	   const scalar t = 1 - c;
	   const scalar s = fastsinfull(rdn);
	   const scalar txy = t * a.x * a.y;
	   const scalar tyz = t * a.y * a.z;
	   const scalar txz = t * a.x * a.z;
	   const scalar sx = s * a.x;
	   const scalar sy = s * a.y;
	   const scalar sz = s * a.z;

	   return vector(
		   p.x * (t * a.x * a.x + c) + p.y * (txy - sz) + p.z * (txz + sy), 
		   p.x * (txy + sz) + p.y * (t * a.y * a.y + c) + p.z * (tyz - sx), 
		   p.x * (txz - sy) + p.y * (tyz + sx) + p.z * (t * a.z * a.z + c));
	}

	eiFORCEINLINE color integrate_direct_lighting(
		const color & K, 
		BSDF & R, 
		const vector & wo)
	{
		LightSampler	sampler(this, P, N, PI / 2.0f);
		color			result(0.0f);

		while (sampler.sample())
		{
			const vector wi(normalize(to_local(L)));

			if (!almost_black(K))
			{
				result += K * eval_light_sample(wo, wi, R);
			}
		}

		return result;
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

	void main(void *arg)
	{
		const color WHITE(1.0f);
		//-----------------------------------------
		const color Cs(i_color());
		color diffuse_color(1.0f, 1.0f, 1.0f);
		scalar diffuse_weight = i_diffuse();
		color specular_color(i_specularColor());
		scalar specular_weight= 0.2f;//cosinePower
		scalar roughness = 0.0f;
		int specular_mode = 1;//[0,3]
		scalar glossiness = 1.0f;
		color reflection_color(i_reflectedColor());
		scalar reflection_weight = 0.5f;
		color  refraction_color(1.0f, 1.0f, 1.0f);
		scalar refraction_weight= 0.0f;//zero will lead a dark image
		scalar refraction_glossiness = 0.5f;
		scalar refraction_thickness= 2.0f;//surfaceThickness
		color  translucency_color(i_transparency());
		scalar translucency_weight = i_translucence();
		scalar anisotropy = 1.0f;
		scalar rotation = 0.0f;
		scalar ior = 1.5f;//refractiveIndex
		bool fresnel_by_ior = eiTRUE;
		scalar fresnel_0_degree_refl = 0.2f;
		scalar fresnel_90_degree_refl = 1.0f;
		scalar fresnel_curve= 5.0f;
		bool is_metal = eiTRUE;
		int diffuse_samples = 8;
		int reflection_samples= 4;
		int refraction_samples= 4;
		scalar cutoff_threshold = LIQ_SCALAR_EPSILON;
		eiTag bump_shader = eiNULL_TAG;
		scalar bump_factor= 0.3f;

  		if( liq_UserDefinedNormal() == 0 )
  		{
  			i_normalCamera() = N;
  		}
		//-----------------------------------------
		vector In = normalize( I );
		normal Nn = normalize( i_normalCamera() );
		normal Nf = ShadingNormal(Nn);
		vector V = -In;
		//-----------------------------------------



		// specular is the percentage of specular lighting
		// limit weights in range [0, 1]
		specular_weight = clamp(specular_weight, 0.0f, 1.0f);
		refraction_weight = clamp(refraction_weight, 0.0f, 1.0f);
		translucency_weight = clamp(translucency_weight, 0.0f, 1.0f);
		// automatically compute Kd, Ks, Kt in an energy conserving way
		diffuse_weight = clamp(diffuse_weight, 0.0f, 1.0f);
		reflection_weight = clamp(reflection_weight, 0.0f, 1.0f);
		// the energy balancing between reflection and refraction is 
		// dominated by Fresnel law
		//color Kt(refraction_color * (spec * refr * (1.0f - trans)) * (is_metal?Cs:WHITE));
		
		// this is a monolithic shader which also serves as shadow shader
		if (ray_type == EI_RAY_SHADOW)
		{
			main_shadow(arg, refraction_color * (specular_weight * refraction_weight * (1.0f - translucency_weight)), 
				refraction_thickness, cutoff_threshold);
			return;
		}

		// for surface shader, we call bump shader
		eiTag shader = bump_shader;
		if (shader != eiNULL_TAG)
		{
			call_bump_shader(shader, bump_factor);
		}

		//color Kc(refraction_color * (spec * refr * trans) * (is_metal?Cs:WHITE));
		// non-reflected energy is absorbed
		//color Ks(specular_color * spec * (1.0f - refl) * (is_metal?Cs:WHITE));
		//color Kr(reflection_color * (spec * refl) * (is_metal?Cs:WHITE));
		// surface color will impact specular for metal material
		//const color Cs(surface_color);

//		const color Kd( Cs *(1.0f - spec) * diff );
//		const int spec_mode = clamp(specular_mode, 0, 3);

		computeSurface(
			i_color(),//outColor(),//out->Ci,//
			i_transparency(),//out->Oi,//
			i_matteOpacityMode(),
			i_matteOpacity(),
			o_outColor(),//out->Ci,//
			o_outTransparency()//out->Oi//
		);
		out->Ci = o_outColor();
		out->Oi = o_outTransparency();

		// apply rotation
		scalar deg = rotation;
		if (deg != 0.0f)
		{
			dPdu = rotate_vector(dPdu, N, radians(deg));
			dPdv = cross(dPdu, N);
			u_axis = normalize(dPdu);
			v_axis = normalize(dPdv);
		}

		// set the glossiness scale based on the chosen BSDF
		scalar glossiness_scale = 370.37f;
		if (specular_mode == 1)
		{
			glossiness_scale = 125.0f;
		}
		else if (specular_mode == 3)
		{
			// scale to make the same glossiness parameter 
			// results in similar lobe for different BSDFs
			glossiness_scale = 22.88f;
		}

//		scalar aniso = anisotropy;
		int refl_samples = reflection_samples;
		int refr_samples = refraction_samples;

		// prepare outgoing direction in local frame
		const vector wo(normalize(to_local(V)));
		// construct BSDFs
		OrenNayar Rd(roughness);
		scalar shiny_u = glossiness;
		if (shiny_u < eiSCALAR_EPS)
		{
			shiny_u = eiSCALAR_EPS;
			refl_samples = 1;
		}
		shiny_u = max(0.0f, glossiness_scale / shiny_u);
		scalar shiny_v = max(0.0f, shiny_u * anisotropy);
		
		scalar IOR = ior;
		eiBool fresn_by_ior = fresnel_by_ior;
		scalar fresn_0_degree_refl = fresnel_0_degree_refl;
		scalar fresn_90_degree_refl = fresnel_90_degree_refl;
		scalar fresn_curve = fresnel_curve;

		union {
			eiByte by_ior[sizeof(FresnelByIOR)];
			eiByte schlick[sizeof(FresnelSchlick)];
		} F_storage;

		union {
			eiByte by_ior[sizeof(FresnelByIOR)];
			eiByte schlick[sizeof(FresnelSchlick)];
		} invF_storage;

		Fresnel *F = NULL;
		Fresnel *invF = NULL;
		if (fresn_by_ior)
		{
			F = new (F_storage.by_ior) FresnelByIOR(IOR);
			invF = new (invF_storage.by_ior) InvFresnelByIOR(IOR);
		}
		else
		{
			F = new (F_storage.schlick) FresnelSchlick(
				fresn_0_degree_refl, 
				fresn_90_degree_refl, 
				fresn_curve);
			invF = new (invF_storage.schlick) InvFresnelSchlick(
				fresn_0_degree_refl, 
				fresn_90_degree_refl, 
				fresn_curve);
		}

		union {
			eiByte ward[sizeof(Ward)];
			eiByte phong[sizeof(StretchedPhong)];
			eiByte blinn[sizeof(Blinn)];
			eiByte cooktorrance[sizeof(CookTorrance)];
		} Rs_storage;

		BSDF *Rs = NULL;
		switch (specular_mode)
		{
		case 0:
 			Rs = new (Rs_storage.ward) Ward(F, shiny_u, shiny_v);
			break;
		case 1:
			Rs = new (Rs_storage.phong) StretchedPhong(F, shiny_u);
			break;
		case 2:
			Rs = new (Rs_storage.blinn) Blinn(F, shiny_u);
			break;
		case 3:
			Rs = new (Rs_storage.cooktorrance) CookTorrance(F, 1.0f / shiny_u);
			break;
		}

		SpecularReflection Rr(F);

		scalar refr_shiny_u = refraction_glossiness;
		if (refr_shiny_u < eiSCALAR_EPS)
		{
			refr_shiny_u = eiSCALAR_EPS;
			refr_samples = 1;
		}
		refr_shiny_u = max(0.0f, glossiness_scale / refr_shiny_u);
		scalar refr_shiny_v = max(0.0f, shiny_u * anisotropy);

		union {
			eiByte ward[sizeof(Ward)];
			eiByte phong[sizeof(StretchedPhong)];
			eiByte blinn[sizeof(Blinn)];
			eiByte cooktorrance[sizeof(CookTorrance)];
		} Rts_storage;

		BSDF *Rts = NULL;
		switch (specular_mode)
		{
		case 0:
			Rts = new (Rts_storage.ward) Ward(invF, refr_shiny_u, refr_shiny_v);
			break;
		case 1:
			Rts = new (Rts_storage.phong) StretchedPhong(invF, refr_shiny_u);
			break;
		case 2:
			Rts = new (Rts_storage.blinn) Blinn(invF, refr_shiny_u);
			break;
		case 3:
			Rts = new (Rts_storage.cooktorrance) CookTorrance(invF, 1.0f / refr_shiny_u);
			break;
		}

		scalar refr_thickness = refraction_thickness;

		// internal scale for refraction thickness, make it smaller
		BRDFtoBTDF Rt(Rts, IOR, refr_thickness * 0.1f, this);
		
		color Cdiffuse(0.0f);
		color Cspecular(0.0f);

		// don't integrate direct lighting if the ray hits the back face
 		if (dot_nd < 0.0f)
 		{
 			// integrate direct lighting from the front side
 			//out->Ci += integrate_direct_lighting(/*Kd*/diffuse(), Rd, wo);
 			//out->Ci *= i_diffuse() * getDiffuse(Nf, eiFALSE, eiFALSE);
			Cdiffuse += i_diffuse() * getDiffuse(Nf, eiFALSE, eiFALSE);

 			//out->Ci += integrate_direct_lighting(Ks, *Rs, wo);
 			//out->Ci += i_specularColor() * getPhong (Nf, V, i_cosinePower(), eiFALSE, eiFALSE);
			Cspecular += i_specularColor() * getPhong (Nf, V, i_cosinePower(), eiFALSE, eiFALSE);

		}

		// integrate for translucency from the back side
		if (!almost_black( refraction_color * (specular_weight * refraction_weight * translucency_weight)*(is_metal?Cs:WHITE) ) && //almost_black(Kc)
			(refr_thickness > 0.0f || dot_nd > 0.0f))
		{
			vector old_dPdu(dPdu);
			vector old_dPdv(dPdv);
			vector old_u_axis(u_axis);
			vector old_v_axis(v_axis);
			normal old_N(N);
			vector new_wo(wo);

			if (dot_nd < 0.0f)
			{
				dPdu = old_dPdv;
				dPdv = old_dPdu;
				u_axis = old_v_axis;
				v_axis = old_u_axis;
				N = -N;
				new_wo = vector(wo.y, wo.x, -wo.z);
			}
			
			// integrate direct lighting from the back side
			//out->Ci += Kc * integrate_direct_lighting(/*Kd*/i_diffuse(), Rd, new_wo);
			//out->Ci += i_diffuse() * getDiffuse(Nf, eiFALSE, eiFALSE);
			Cdiffuse += i_diffuse() * getDiffuse(Nf, eiFALSE, eiFALSE);

			//out->Ci += Kc * integrate_direct_lighting(Ks, *Rs, new_wo);
			//out->Ci += i_specularColor() * getPhong (Nf, V, i_cosinePower(), eiFALSE, eiFALSE);
			Cspecular += i_specularColor() * getPhong (Nf, V, i_cosinePower(), eiFALSE, eiFALSE);

			N = old_N;
			u_axis = old_u_axis;
			v_axis = old_v_axis;
			dPdu = old_dPdu;
			dPdv = old_dPdv;
		}


		scalar cutoff_thresh = cutoff_threshold;
				
		color CReflectSpecular(0.0f);
 		// integrate indirect specular lighting
		if (!almost_black( specular_color * (specular_weight * (1.0f - reflection_weight))*(is_metal?Cs:WHITE) ) && dot_nd < 0.0f)//almost_black(Ks)
 		{
 			IntegrateOptions opt;
 			opt.ray_type = EI_RAY_REFLECT_GLOSSY;
 			opt.min_samples = opt.max_samples = refl_samples;
 			opt.cutoff_threshold = cutoff_thresh;

			CReflectSpecular = integrate(wo, *Rs, opt);
  		}

		color CSpecularReflection(0.0f);
		// integrate perfect specular reflection
		if (!almost_black(reflection_color * (specular_weight * reflection_weight) * (is_metal?Cs:WHITE)) && dot_nd < 0.0f)//almost_black(Kr)
		{
			IntegrateOptions opt;
			opt.ray_type = EI_RAY_REFLECT_GLOSSY;
			opt.min_samples = opt.max_samples = 1; // force one sample for reflection
			opt.cutoff_threshold = cutoff_thresh;
			// the direct lighting of this BRDF is not accounted, 
			// so we trace lights here to compensate
			opt.trace_lights = eiTRUE;
			
			CSpecularReflection = integrate(wo, Rr, opt);
		}

		color CReflectDiffuse(0.0f);
		// integrate indirect diffuse lighting (color bleeding)
		if (!almost_black( Cs *(1.0f - specular_weight) * diffuse_weight ) && dot_nd < 0.0f)//almost_black(Kd)
		{
			IntegrateOptions opt;
			opt.ray_type = EI_RAY_REFLECT_DIFFUSE;
			opt.min_samples = opt.max_samples = diffuse_samples;
			opt.cutoff_threshold = cutoff_thresh;

			CReflectDiffuse = integrate(wo, Rd, opt);
		}

		color CRefraction(0.0f);
		// integrate refraction
		if ( !almost_black(refraction_color * specular_weight * refraction_weight * (1.0f-translucency_weight)*(is_metal?Cs:WHITE)) ) //almost_black(Kt)
		{
			IntegrateOptions opt;
			opt.ray_type = EI_RAY_REFRACT_GLOSSY;
			if (IOR == 1.0f)
			{
				opt.ray_type = EI_RAY_TRANSPARENT;
			}
			opt.min_samples = opt.max_samples = refr_samples;
			opt.cutoff_threshold = cutoff_thresh;
			// account for refractive caustics
			opt.trace_lights = eiTRUE;

			CRefraction = integrate(wo, Rt, opt);
		}

		out->Ci *= (Cdiffuse 

					+ CReflectDiffuse *
					  Cs *(1.0f - specular_weight) * diffuse_weight//Kd
					);

		out->Ci += (Cspecular 

					+ CReflectSpecular* 
					  specular_color   * specular_weight * (1.0f - reflection_weight)*(is_metal?Cs:WHITE)//Ks
					
					+ CSpecularReflection* 
					  reflection_color * specular_weight * reflection_weight * (is_metal?Cs:WHITE)//Kr
					
					+ CRefraction*
					  refraction_color * specular_weight * refraction_weight * (1.0f-translucency_weight) * (is_metal?Cs:WHITE)//Kt			
					);

#ifdef USE_AOV_aov_ambient
		aov_ambient() += ( i_ambientColor() 
							*(CReflectDiffuse *
							  Cs *(1.0f - specular_weight) * diffuse_weight//Kd
							 )
						 ) * (1.0f - o_outTransparency());
#endif
#ifdef USE_AOV_aov_diffuse
		aov_diffuse() += ( Cdiffuse * i_color() ) * (1.0f - o_outTransparency());
#endif
#ifdef USE_AOV_aov_specular
		aov_specular() += (Cspecular
							+ CReflectSpecular* 
							specular_color   * specular_weight * (1.0f - reflection_weight)*(is_metal?Cs:WHITE)//Ks

							+ CSpecularReflection* 
							reflection_color * specular_weight * reflection_weight * (is_metal?Cs:WHITE)//Kr

							+ CRefraction*
							refraction_color * specular_weight * refraction_weight * (1.0f-translucency_weight) * (is_metal?Cs:WHITE)//Kt			
							);
#endif


		if ( ! less_than( &i_transparency(), LIQ_SCALAR_EPSILON ) )
		{//transparent
			out->Ci = out->Ci * ( 1.0f - i_transparency() ) + trace_transparent() * i_transparency();
		}//else{ opacity }

		Rs->~BSDF();
		Rts->~BSDF();
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
	//
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
END(maya_phong_architectural)

const char *maya_phong_architectural::u_result = NULL;
