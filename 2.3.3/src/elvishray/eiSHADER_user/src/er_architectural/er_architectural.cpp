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
#include <new>


class FresnelByIOR : public Fresnel {
public:
	FresnelByIOR(const scalar ior)
	{
		m_ior = ior;
	}

	virtual scalar eval(const scalar WOdotWh)
	{
		return fresnel_dielectric(WOdotWh, 1.0f, m_ior);
	}

private:
	scalar		m_ior;
};

class FresnelSchlick : public Fresnel {
public:
	FresnelSchlick(
		const scalar fresnel_0_degree_refl, 
		const scalar fresnel_90_degree_refl, 
		const scalar fresnel_curve)
	{
		m_fresnel_r1 = clamp(fresnel_0_degree_refl, 0.0f, 1.0f);
		m_fresnel_r2 = clamp(fresnel_90_degree_refl - fresnel_0_degree_refl, 0.0f, 1.0f);
		m_fresnel_curve = max(eiSCALAR_EPS, fresnel_curve);
	}

	virtual scalar eval(const scalar WOdotWh)
	{
		// a modified Schlick fresnel approximation is used here
		return (m_fresnel_r1 + m_fresnel_r2 * fastpow(1.0f - WOdotWh, m_fresnel_curve));
	}

private:
	scalar			m_fresnel_r1;
	scalar			m_fresnel_r2;
	scalar			m_fresnel_curve;
};

class InvFresnelByIOR : public Fresnel {
public:
	InvFresnelByIOR(const scalar ior)
	{
		m_ior = ior;
	}

	virtual scalar eval(const scalar WOdotWh)
	{
		return 1.0f - fresnel_dielectric(WOdotWh, 1.0f, m_ior);
	}

private:
	scalar		m_ior;
};

class InvFresnelSchlick : public Fresnel {
public:
	InvFresnelSchlick(
		const scalar fresnel_0_degree_refl, 
		const scalar fresnel_90_degree_refl, 
		const scalar fresnel_curve)
	{
		m_fresnel_r1 = clamp(fresnel_0_degree_refl, 0.0f, 1.0f);
		m_fresnel_r2 = clamp(fresnel_90_degree_refl - fresnel_0_degree_refl, 0.0f, 1.0f);
		m_fresnel_curve = max(eiSCALAR_EPS, fresnel_curve);
	}

	virtual scalar eval(const scalar WOdotWh)
	{
		// a modified Schlick fresnel approximation is used here
		return 1.0f - (m_fresnel_r1 + m_fresnel_r2 * fastpow(1.0f - WOdotWh, m_fresnel_curve));
	}

private:
	scalar			m_fresnel_r1;
	scalar			m_fresnel_r2;
	scalar			m_fresnel_curve;
};

class BRDFtoBTDF : public BSDF {
public:
	BRDFtoBTDF(
		BSDF *brdf, 
		const scalar ior, 
		const scalar thickness, 
		eiState *state)
		: BSDF(NULL)
	{
		m_brdf = brdf;
		m_ior = ior;
		m_thickness = thickness;
		m_state = state;
		// record original P
		m_originalP = m_state->P;
	}

	virtual ~BRDFtoBTDF()
	{
		// restore the P
		m_state->P = m_originalP;
	}

	virtual color bsdf(const vector & wo, const vector & wi)
	{
		// reflect wo about H
		const vector R(reflect(wo, m_cached_halfvector));

		return m_brdf->bsdf(wo, R);
	}

	virtual eiBool sample_bsdf(const vector & wo, vector & wi, scalar u1, scalar u2)
	{
		// BRDFs usually guarantee that the sideness of wi is the same as wo
		eiBool result = m_brdf->sample_bsdf(wo, wi, u1, u2);

		const vector H(normalize(wo + wi));

		// figure out the relative IOR
		scalar ior = m_ior;
		if (wo.z > 0.0f) // if entering
		{
			ior = 1.0f / ior;
		}

		// refract wo about H
		wi = refract(wo, H, ior);

		// cache the half vector because we don't know how to compute 
		// fresnel for BTDF just from wo, wi
		m_cached_halfvector = H;

		// handle two-sided mode
		if (m_thickness > eiSCALAR_EPS)
		{
			vector D((void *)NULL);
			ei_from_local((eiVector *)(&D), &wi, m_state);

			// move the ray origin
			m_state->P = m_originalP + D * (m_thickness / absf(dot(m_state->N, D)));
			// the ray direction does not change
			wi = -wo;
		}

		return result;
	}

	virtual scalar pdf(const vector & wo, const vector & wi)
	{
		// reflect wo about H
		const vector R(reflect(wo, m_cached_halfvector));

		return m_brdf->pdf(wo, R);
	}

private:
	BSDF			*m_brdf;
	eiState			*m_state;
	scalar			m_ior;
	scalar			m_thickness;
	point			m_originalP;
	vector			m_cached_halfvector;
};

SURFACE(er_architectural)

	DECLARE;
	DECLARE_COLOR(surface_color, 1.0f, 1.0f, 1.0f);
	DECLARE_COLOR(diffuse_color, 1.0f, 1.0f, 1.0f);
	DECLARE_SCALAR(diffuse_weight, 1.0f);
	DECLARE_COLOR(specular_color, 1.0f, 1.0f, 1.0f);
	DECLARE_SCALAR(specular_weight, 0.2f);
	DECLARE_SCALAR(roughness, 0.0f);
	DECLARE_INT(specular_mode, 0);
	DECLARE_SCALAR(glossiness, 1.0f);
	DECLARE_COLOR(reflection_color, 1.0f, 1.0f, 1.0f);
	DECLARE_SCALAR(reflection_weight, 0.0f);
	DECLARE_COLOR(refraction_color, 1.0f, 1.0f, 1.0f);
	DECLARE_SCALAR(refraction_weight, 0.0f);
	DECLARE_SCALAR(refraction_glossiness, 0.0f);
	DECLARE_SCALAR(refraction_thickness, 0.0f);
	DECLARE_COLOR(translucency_color, 1.0f, 1.0f, 1.0f);
	DECLARE_SCALAR(translucency_weight, 0.0f);
	DECLARE_INT(translucency_mode, 1);
	DECLARE_COLOR(sss_color, 0.8f, 0.8f, 0.8f);
	DECLARE_COLOR(sss_scale_color, 0.5f, 0.5f, 0.5f);
	DECLARE_SCALAR(sss_scale_weight, 10.0f);
	DECLARE_SCALAR(sss_phase, 0.0f);
	DECLARE_SCALAR(anisotropy, 1.0f);
	DECLARE_SCALAR(rotation, 0.0f);
	DECLARE_SCALAR(ior, 1.5f);
	DECLARE_BOOL(fresnel_by_ior, eiFALSE);
	DECLARE_SCALAR(fresnel_0_degree_refl, 0.2f);
	DECLARE_SCALAR(fresnel_90_degree_refl, 1.0f);
	DECLARE_SCALAR(fresnel_curve, 5.0f);
	DECLARE_BOOL(is_metal, eiTRUE);
	DECLARE_INT(diffuse_samples, 8);
	DECLARE_INT(reflection_samples, 4);
	DECLARE_INT(refraction_samples, 4);
	DECLARE_INT(sss_samples, 64);
	DECLARE_SCALAR(cutoff_threshold, 0.01f);
	DECLARE_TAG(bump_shader, eiNULL_TAG);
	DECLARE_SCALAR(bump_factor, 0.3f);
	DECLARE_TAG(displace_shader, eiNULL_TAG);
	DECLARE_SCALAR(displace_factor, 1.0f);
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
		const normal & Nf, 
		const color & Kd, 
		const color & Ks, 
		BSDF & Rd, 
		BSDF & Rs, 
		const vector & wo)
	{
		LightSampler	sampler(this, P, Nf, PI * 0.5f);
		color			result(0.0f);

		while (sampler.sample())
		{
			const vector wi(to_local(L));

			if (!almost_black(Kd) && affect_diffuse())
			{
				result += Kd * eval_light_sample(wo, wi, Rd);
			}

			if (!almost_black(Ks) && affect_specular())
			{
				result += Ks * eval_light_sample(wo, wi, Rs);
			}
		}

		return result;
	}

	void main_shadow(void *arg, const color & Kt)
	{
		// the light has falled off to almost black, 
		// no need to trace
		if (almost_black(Cl))
		{
			return;
		}

		scalar transp = average(&Kt);

		// we are less transparent if the surface is treated as two sided
		if (refraction_thickness() > 0.0f)
		{
			transp = transp * transp;
		}

		// the same with opaque shader
		if (almost_zero(transp, cutoff_threshold()))
		{
			Cl = color(0.0f);
			Ol = color(1.0f);
			return;
		}

		// reduce the length of ray direction to limit the maximum distance
		Cl = trace_shadow(P, I - (P - E), Cl) * transp;
		Ol = (1.0f - transp);
	}

	void main_displace(void *arg)
	{
		eiTag shader = displace_shader();
		if (shader != eiNULL_TAG)
		{
			color	C;

			eiShaderOutput outputs[] = {
				{EI_TYPE_VECTOR, u_result, &C}, 
			};

			call_shader(shader, 1, outputs, NULL);

			scalar disp_factor = displace_factor();
			scalar offset = average(&C);
			
			P = P + disp_factor * offset * N;
		}
	}

	void main(void *arg)
	{
		if (ray_type == EI_RAY_DISPLACE)
		{
			main_displace(arg);
			return;
		}

		// specular is the percentage of specular lighting
		// limit weights in range [0, 1]
		const scalar spec = clamp(specular_weight(), 0.0f, 1.0f);
		const scalar refr = clamp(refraction_weight(), 0.0f, 1.0f);
		const scalar trans = clamp(translucency_weight(), 0.0f, 1.0f);
		const scalar kr = spec;
		const scalar kt_notrans = refr * (1.0f - trans);
		const scalar kt = (1.0f - spec) * kt_notrans;
		const scalar ktr = kr + kt;
		// the energy balancing between reflection and refraction is 
		// dominated by Fresnel law
		const scalar kt_final = (kt_notrans == 0.0f) ? 0.0f : ktr;
		color Kt(refraction_color() * kt_final);
		
		// this is a monolithic shader which also serves as shadow shader
		if (ray_type == EI_RAY_SHADOW)
		{
			main_shadow(arg, Kt);
			return;
		}

		// automatically compute Kd, Ks, Kt in an energy conserving way
		const scalar diff = clamp(diffuse_weight(), 0.0f, 1.0f);
		const scalar refl = clamp(reflection_weight(), 0.0f, 1.0f);

		// for surface shader, we call bump shader
		eiTag shader = bump_shader();
		if (shader != eiNULL_TAG)
		{
			call_bump_shader(shader, bump_factor());
		}

		const vector V(-normalize(I));

		// the refraction thickness is also used as general thickness
		const scalar refr_thickness = refraction_thickness();
		normal Nf(N);
		if (refr_thickness > 0.0f)
		{
			Nf = faceforward(N, I);
		}

		color Kc(translucency_color() * ((1.0f - spec) * refr * trans));
		// non-reflected energy is absorbed
		const scalar kr_final = (spec == 0.0f) ? 0.0f : ktr;
		color Ks(specular_color() * (kr_final * (1.0f - refl)));
		color Kr(reflection_color() * (kr_final * refl));
		// surface color will impact specular for metal material
		const color Cs(surface_color());
		if (is_metal())
		{
			Ks *= Cs;
			Kr *= Cs;
			Kt *= Cs;
			Kc *= Cs;
		}
		const color Kd(Cs * ((1.0f - spec) * (1.0f - refr) * diff));
		const int spec_mode = clamp(specular_mode(), 0, 3);

		out->Ci = 0.0f;

		// apply rotation
		scalar deg = rotation();
		if (deg != 0.0f)
		{
			dPdu = rotate_vector(dPdu, N, radians(deg));
			dPdv = cross(N, dPdu);
			u_axis = normalize(dPdu);
			v_axis = normalize(dPdv);
		}

		// set the glossiness scale based on the chosen BSDF
		scalar glossiness_scale = 370.37f;
		if (spec_mode == 1)
		{
			glossiness_scale = 125.0f;
		}
		else if (spec_mode == 3)
		{
			// scale to make the same glossiness parameter 
			// results in similar lobe for different BSDFs
			glossiness_scale = 22.88f;
		}

		scalar aniso = anisotropy();
		int refl_samples = reflection_samples();
		int refr_samples = refraction_samples();

		// prepare outgoing direction in local frame
		const vector wo(to_local(V));
		// construct BSDFs
		OrenNayar Rd(roughness());
		scalar shiny_u = glossiness();
		if (shiny_u < eiSCALAR_EPS)
		{
			shiny_u = eiSCALAR_EPS;
			refl_samples = 1;
		}
		shiny_u = max(0.0f, glossiness_scale / shiny_u);
		scalar shiny_v = max(0.0f, shiny_u * aniso);
		
		scalar IOR = ior();
		eiBool fresn_by_ior = fresnel_by_ior();
		scalar fresn_0_degree_refl = fresnel_0_degree_refl();
		scalar fresn_90_degree_refl = fresnel_90_degree_refl();
		scalar fresn_curve = fresnel_curve();

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
		switch (spec_mode)
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

		scalar refr_shiny_u = refraction_glossiness();
		if (refr_shiny_u < eiSCALAR_EPS)
		{
			refr_shiny_u = eiSCALAR_EPS;
			refr_samples = 1;
		}
		refr_shiny_u = max(0.0f, glossiness_scale / refr_shiny_u);
		scalar refr_shiny_v = max(0.0f, shiny_u * aniso);

		union {
			eiByte ward[sizeof(Ward)];
			eiByte phong[sizeof(StretchedPhong)];
			eiByte blinn[sizeof(Blinn)];
			eiByte cooktorrance[sizeof(CookTorrance)];
		} Rts_storage;

		BSDF *Rts = NULL;
		switch (spec_mode)
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

		// internal scale for refraction thickness, make it smaller
		BRDFtoBTDF Rt(Rts, IOR, refr_thickness * 0.1f, this);
		
		// don't integrate direct lighting if the ray hits the back face
		if (dot_nd < 0.0f || refr_thickness > 0.0f)
		{
			// integrate direct lighting from the front side
			out->Ci += integrate_direct_lighting(Nf, Kd, Ks, Rd, *Rs, wo);
		}

		scalar cutoff_thresh = cutoff_threshold();

		// integrate translucency
		if (!almost_black(Kc))
		{
			const int trans_mode = translucency_mode();

			if (trans_mode == 0)
			{
				// integrate for translucency from the back side
				if (refr_thickness > 0.0f || dot_nd > 0.0f)
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
					out->Ci += Kc * integrate_direct_lighting(N, Kd, Ks, Rd, *Rs, new_wo);
				
					N = old_N;
					u_axis = old_u_axis;
					v_axis = old_v_axis;
					dPdu = old_dPdu;
					dPdv = old_dPdv;
				}
			}
			else if (dot_nd < 0.0f)
			{
				// for sub-surface scattering, we only integrate from the front side
				BSSRDFIntegrateOptions opt;
				opt.min_samples = opt.max_samples = sss_samples();
				opt.min_indirect_samples = opt.max_indirect_samples = diffuse_samples();
				opt.cutoff_threshold = cutoff_thresh;

				const color rd(sss_color());
				const color sigma_tr(1.0f / (sss_scale_color() * sss_scale_weight()));
				const scalar g = sss_phase();
				const color mfp(
					sqrtf(3.0f * (1.0f - rd.r)) / sigma_tr.r, 
					sqrtf(3.0f * (1.0f - rd.g)) / sigma_tr.g, 
					sqrtf(3.0f * (1.0f - rd.b)) / sigma_tr.b);

				out->Ci += Kc * trace_single_scatter(rd, mfp, g, IOR, opt);
			}
		}

		// integrate indirect specular lighting
		if (!almost_black(Ks) && dot_nd < 0.0f)
		{
			IntegrateOptions opt;
			opt.ray_type = EI_RAY_REFLECT_GLOSSY;
			opt.min_samples = opt.max_samples = refl_samples;
			opt.cutoff_threshold = cutoff_thresh;

			out->Ci += Ks * integrate(wo, *Rs, opt);
		}

		// integrate perfect specular reflection
		if (!almost_black(Kr) && dot_nd < 0.0f)
		{
			IntegrateOptions opt;
			opt.ray_type = EI_RAY_REFLECT_GLOSSY;
			opt.min_samples = opt.max_samples = 1; // force one sample for reflection
			opt.cutoff_threshold = cutoff_thresh;
			// the direct lighting of this BRDF is not accounted, 
			// so we trace lights here to compensate
			opt.trace_lights = eiTRUE;

			out->Ci += Kr * integrate(wo, Rr, opt);
		}

		// integrate indirect diffuse lighting
		if (!almost_black(Kd) && dot_nd < 0.0f)
		{
			IntegrateOptions opt;
			opt.ray_type = EI_RAY_REFLECT_DIFFUSE;
			opt.min_samples = opt.max_samples = diffuse_samples();
			opt.cutoff_threshold = cutoff_thresh;

			out->Ci += Kd * integrate(wo, Rd, opt);
		}

		// integrate refraction
		if (!almost_black(Kt))
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

			out->Ci += Kt * integrate(wo, Rt, opt);
		}

		out->Oi = color(1.0f);

		Rs->~BSDF();
		Rts->~BSDF();
	}

END(er_architectural)

const char *er_architectural::u_result = NULL;
