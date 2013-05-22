
#include <eiAPI/ei_shaderx.h>
#include "common/_3delight/utils.h"

SURFACE(maya_place2dTexture)
	DECLARE;
	DECLARE_VECTOR(i_uvCoord, 0.0f, 0.0f, 0.0f);//float2,
	DECLARE_SCALAR(i_coverageU, 1.0f);
	DECLARE_SCALAR(i_coverageV, 1.0f);
	DECLARE_BOOL(i_mirrorU, eiFALSE);
	DECLARE_BOOL(i_mirrorV, eiFALSE);
	DECLARE_SCALAR(i_noiseU, 0.0f);
	DECLARE_SCALAR(i_noiseV, 0.0f);
	DECLARE_SCALAR(i_offsetU, 0.0f);
	DECLARE_SCALAR(i_offsetV, 0.0f);
	DECLARE_SCALAR(i_repeatU, 1.0f);
	DECLARE_SCALAR(i_repeatV, 1.0f);
	DECLARE_SCALAR(i_rotateFrame, 0.0f);//angle(deg)
	DECLARE_SCALAR(i_rotateUV, 0.0f);//angle(deg)
	DECLARE_BOOL(i_stagger, eiFALSE);
	DECLARE_SCALAR(i_translateFrameU, 0.0f);
	DECLARE_SCALAR(i_translateFrameV, 0.0f);
	DECLARE_BOOL(i_wrapU, eiTRUE);
	DECLARE_BOOL(i_wrapV, eiTRUE);
	DECLARE_VECTOR(o_outUV,    0.0f, 0.0f, -1.0f); //float2
	DECLARE_INT(liq_UserDefinedU, 0);//0: use elvishray internal u.   1: use user defined u.
	DECLARE_INT(liq_UserDefinedV, 0);//0: use elvishray internal v.   1: use user defined v.
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
		main_er0(arg);
		//main_3delight(arg);
	}
	void main_er0(void *arg)
	{
		if( liq_UserDefinedU() == 0 ){
			i_uvCoord().x = u;//elvishray generates u internally.
		}else{
			//u is connected in
		}
		if( liq_UserDefinedV() == 0 ){
			i_uvCoord().y = v;//elvishray generates v internally.
		}else{
			//v is connected in
		}

		o_outUV().x = fmodf( i_uvCoord().x * i_repeatU(), 1.0f );
		o_outUV().y = fmodf( i_uvCoord().y * i_repeatV(), 1.0f );
 		o_outUV().y = 1.0f - o_outUV().y;//adjust v for elvishray
	}
	void main_3delight(void *arg)
	{
		float ss = u;
		float tt = 1.0f - v;//adjust v for elvishray

		float outU = ss;
		float outV = tt;

		if(i_noiseU() > 0.0f)
		{
			outU += 1.25f * i_noiseU() * (	pnoise( point(ss * 18.0f, tt * 18.0f, 0.0f), 
													point(18.0f, 18.0f, 18.0f) 
											) 
											* 2.0f - 1.0f
										);
		}

		if(i_noiseV() > 0.0f)
		{
			outV += 1.25f * i_noiseV() * (	pnoise(	point(ss * 18.0f, tt * 18.0f, 1.0f), 
													point(18.0f, 18.0f, 18.0f)
											) 
											* 2.0f - 1.0f
										);
		}

		if(i_rotateFrame() != 0.0f)
		{
			point Q = point(outU, outV, 0.0);
			point P1 = point(0.5, 0.5, 0.0);
			point P2 = point(0.5, 0.5, 1.0);

			matrix rotMatrix = rotate(i_rotateFrame(), P2-P1 ); //Q = rotate(Q, radians(rotateFrame()), P1, P2 );
			Q = Q * rotMatrix;

			outU = Q.x;
			outV = Q.y;
		}

		outU -= i_translateFrameU();
		outV -= i_translateFrameV();

		if( fmodf(outU, WRAPMAX) - i_coverageU() > eiSCALAR_EPS || 
			fmodf(outV, WRAPMAX) - i_coverageV() > eiSCALAR_EPS ||
			(i_wrapU() == eiFALSE && (outU < 0.0f || (outU * i_repeatU()) - i_repeatU() > eiSCALAR_EPS)) ||
			(i_wrapV() == eiFALSE && (outV < 0.0f || (outV * i_repeatV()) - i_repeatV() > eiSCALAR_EPS)))
		{
			o_outUV().x = UNDEFINED_UV;
			o_outUV().y = UNDEFINED_UV;
		}
		else
		{
			if(i_coverageU() < 1.0f)
			{
				outU = fmodf(outU, 1.0f);
			}

			if(i_coverageV() < 1.0f)
			{
				outV = fmodf(outV, 1.0f);
			}

			outU = outU * i_repeatU() / i_coverageU() + i_offsetU();
			outV = outV * i_repeatV() / i_coverageV() + i_offsetV();	

			if(fmodf(outV, 2.0f) >= 1.0f)
			{
				if(i_stagger() != eiFALSE)
				{
					outU += 0.5f;
				}

				if(i_mirrorV() != eiFALSE)
				{
					scalar axis = floor(outV) + 0.5f;
					outV = axis - (outV - axis);
				}
			}

			if(i_mirrorU() != eiFALSE && fmodf(outU, 2.0f) >= 1.0f)
			{
				scalar axis = floor(outU) + 0.5f;
				outU = axis - (outU - axis);
			}

			if(i_rotateUV() != 0.0f)
			{
				point Q = point(outU, outV, 0.0f);
				point P1 = point(0.5f, 0.5f, 0.0f);
				point P2 = point(0.5f, 0.5f, 1.0f);

				matrix rotMatrix = rotate(i_rotateFrame(), P2-P1 );//Q = rotate(Q, radians(rotateUV()), P1, P2 ); 
				Q = Q * rotMatrix;

				outU = fmodf(Q.x, i_repeatU());
				outV = fmodf(Q.y, i_repeatV());
			}

			o_outUV().x = outU;
			o_outUV().y = outV;
		}
	}

END(maya_place2dTexture)