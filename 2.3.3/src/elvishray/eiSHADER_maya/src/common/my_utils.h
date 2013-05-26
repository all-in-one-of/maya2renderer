#ifndef _MY_UTILS_H_
#define _MY_UTILS_H_

#include <eiAPI/ei_shaderx.h>
#include <new>

#include "../../../../liq_math.h"

// a->x<b && a->y<b && a->z <b
eiBool less_than(const eiVector *a, const scalar b);
//
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
//

//
#endif//_MY_UTILS_H_