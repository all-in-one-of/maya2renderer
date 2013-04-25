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
#include "common/my_math.h"
#include "common/my_utils.h"

SURFACE(maya_bump2d)

	DECLARE;
	DECLARE_SCALAR(i_bumpValue,			0.0f);	// Inputs - begin
	DECLARE_SCALAR(i_bumpDepth,			1.0f);
	DECLARE_INDEX(i_bumpInterp,			0);
	DECLARE_SCALAR(i_bumpFilter,		1.0f);
	DECLARE_SCALAR(i_bumpFilterOffset,	0.0f);
	DECLARE_NORMAL(i_normalCamera,		0.0f, 0.0f, 1.0f);
	DECLARE_NORMAL(i_bumpNormal,		-1.0f, -1.0f, -1.0f);
	DECLARE_NORMAL(o_outNormal,			0.0f, 0.0f, 1.0f);	// Outputs		
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

	void do_bump_map(
		//const eiScalar i_bumpValue,
		//const eiScalar i_bumpDepth,
		const normal& i_normalCamera,
		normal& o_outNormal )
	{
		eiScalar LOW = -1.0f, HEIGHT=1.0f;
		eiScalar offset = clamp(i_bumpValue(), LOW, HEIGHT) * abs(i_bumpDepth());

		//Du_offset=d(offset)/du, Dv_offset=d(offset)/dv
		eiScalar Du_offset, Dv_offset;
		eiScalar DbumpValue_du, DbumpValue_dv;
		eiScalar DbumpDepth_du, DbumpDepth_dv;		
		DbumpValue_du = Du(i_bumpValue);
		DbumpValue_dv = Dv(i_bumpValue);
		DbumpDepth_du = Du(i_bumpDepth);
		DbumpDepth_dv = Dv(i_bumpDepth);
		Du_offset = D_clamp(i_bumpValue(), LOW,HEIGHT) * DbumpValue_du      * abs(i_bumpDepth()) 
			        + clamp(i_bumpValue(), LOW,HEIGHT) * D_abs(i_bumpDepth()) * DbumpDepth_du;
		Dv_offset = D_clamp(i_bumpValue(), LOW,HEIGHT) * DbumpValue_dv      * abs(i_bumpDepth()) 
			        + clamp(i_bumpValue(), LOW,HEIGHT) * D_abs(i_bumpDepth()) * DbumpDepth_dv;
		/*
		These scale factors are a bit experimental. The constant is to roughly
		match maya's bump mapping. The part about dPdu/dPdv ensures that the
		bump's scale does not depend on the underlying parametrization of the
		patch (the use of Du and Dv below introduce that) and that it happens
		in object space. Note that maya's handling of object space appears to
		be slightly broken since an enlarged sphere will have the same bump as
		a sphere with its control points moved outwards by a scale, somehow.
		*/
		eiScalar uscale = 1.0f / len( &(vto_object(dPdu)) ) * 6.0f;
		eiScalar vscale = 1.0f / len( &(vto_object(dPdv)) ) * 6.0f;

		vector gu = vector(1.0f, 0.0f, Du_offset * uscale);
		vector gv = vector(0.0f, 1.0f, Dv_offset * vscale);
		normal n = normal(gu ^ gv);

		vector basisz = normalize(i_normalCamera);
		vector basisx = normalize((basisz ^ dPdu) ^ basisz);
		vector basisy = normalize((basisz ^ dPdv) ^ basisz);

		o_outNormal = normal(
			n.x * basisx + n.y * basisy + n.z * basisz);

		o_outNormal = normalize(o_outNormal);
	}
	void main_3delight(void *arg)
	{
		if( i_bumpInterp() == 0 )
		{
			normal n = o_outNormal();
			/* Bump. */
			do_bump_map(
				//bumpValue(),
				//bumpDepth(),
				i_normalCamera(),
				n );
			o_outNormal() = n;
		}
		else if( i_bumpInterp() == 1 )
		{
			///* Tangent Space Normals. */
			//vector udir, vdir;

			//if( stangent != vector(0) )
			//{
			//	vdir = stangent ^ i_normalCamera;
			//}
			//else
			//{
			//	vdir = Du(tt) * dPdu + Dv(tt) * dPdv;
			//	vdir = i_normalCamera ^ (vdir ^ i_normalCamera);
			//}

			//udir = i_normalCamera ^ vdir;
			//vector uorient = Du(ss) * dPdu + Dv(ss) * dPdv;
			//if( udir.uorient < 0 )
			//{
			//	udir = -udir;
			//}

			//vector basisx = normalize(udir);
			//vector basisy = normalize(vdir);
			//vector basisz = normalize(i_normalCamera);

			//outNormal() = normal(
			//	(bumpNormal().x - 0.5f) * basisx +
			//	(bumpNormal().y - 0.5f) * basisy +
			//	(bumpNormal().z - 0.5f) * basisz );

			//outNormal() = normalize(outNormal());
		}
		else
		{
			/* Object Space Normals. This needs some work. */
			//outNormal() = ntransform( "object", "current", bumpNormal() - 0.5f );
			//outNormal() = normalize(outNormal());
			vector tmp = nto_object(i_bumpNormal() - normal(0.5f,0.5f,0.5f));
			o_outNormal() = normalize(tmp);
		}
	}
	void main_hack(void *arg)
	{
		//outNormal() = normalCamera();
		o_outNormal() = i_bumpNormal();
	}
	void main(void *arg)
	{
		//main_3delight(arg);
		//main_hack(arg);
		main_3delight_10_0_50(arg);
	}
	void main_3delight_10_0_50(void *arg)
	{
		//extern varying vector dPdu, dPdv, I;
		vector stangent(0.0f);
		float ss = u;
		float tt = 1.0f - v;
  
		if( i_bumpInterp() <= 1 )
		{
			normal bumpNormal;
			if( i_bumpInterp() == 0 )
			{
				/* 2D bump map. */
				float depth = abs( i_bumpDepth() );
				float offset = clamp( i_bumpValue() * i_bumpDepth(), -depth, depth );
				point pb = point( ss, tt, 0.04f * offset );
				bumpNormal = normalize( calculatenormal( pb ) );

				/*
					Depending on the orientation of texture coordinates and
					geometry, calculatenormal might return a normal which points
					the wrong way.
				*/
				bumpNormal = -bumpNormal;
				if( bumpNormal.z < 0.0f )
				{
					bumpNormal.z = -bumpNormal.z;
				}
				else
				{
					/*uniform */float backfacing = 0.0f;
					//attribute( "geometry:backfacing", backfacing );
					backfacing = 1.0f - 2.0f * backfacing;

					float flip = -sign( i_normalCamera()%I ) * backfacing;
					bumpNormal.x = flip * bumpNormal.x;
					bumpNormal.y = flip * bumpNormal.y;
				}
			}
			else
			{
				/* Tangent Space Normals. */
				bumpNormal = normal(i_bumpNormal() - vector(0.5f));
			}

			vector udir, vdir;

			if( ! less_than(&stangent, LIQ_SCALAR_ALMOST_ZERO) )
			{
				vdir = stangent ^ i_normalCamera();
			}
			else
			{
				float Dtt_Du =  0.0f;//Du(tt) = Du(1.0-v) = -Du(v); //; //-Du(v);
				float Dtt_Dv = -1.0f;//Dv(tt) = Dv(1.0-v) = -Dv(v); //; //-Dv(v);
				vdir = Dtt_Du * dPdu + Dtt_Dv * dPdv;
				vdir = i_normalCamera() ^ (vdir ^ i_normalCamera());
			}

			udir = i_normalCamera() ^ vdir;
			float Dss_Du = 1.0f;//Du(ss) = Du(u) = Du(u); //; //Du(u);
			float Dss_Dv = 0.0f;//Dv(ss) = Dv(u) = Dv(u); //; //Dv(u);
			vector uorient = Dss_Du * dPdu + Dss_Dv * dPdv;
			if( udir % uorient < 0.0f )
			{
				udir = -udir;
			}

			vector basisx = normalize(udir);
			vector basisy = normalize(vdir);
			vector basisz = normalize(i_normalCamera());

			o_outNormal() = normal(
				bumpNormal.x * basisx +
				bumpNormal.y * basisy +
				bumpNormal.z * basisz );

			o_outNormal() = normalize(o_outNormal());
		}
		else
		{
			/* Object Space Normals. This needs some work. */
			o_outNormal() = nto_object(i_bumpNormal() - normal(0.5f));
			o_outNormal() = normalize(o_outNormal());
		}
	}
END(maya_bump2d)