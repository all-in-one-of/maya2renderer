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
#include "common/_3delight/utils.h"

SURFACE(maya_checker)
	DECLARE;
	DECLARE_SCALAR(alphaGain,		1.0f);	// Inputs - begin
	DECLARE_BOOL(alphaIsLuminance,	eiFALSE);
	DECLARE_SCALAR(alphaOffset,		0.0f);
	DECLARE_COLOR(color1,			1.0f, 1.0f, 1.0f);
	DECLARE_COLOR(color2,			0.0f, 0.0f, 0.0f);
	DECLARE_COLOR(colorGain,		1.0f, 1.0f, 1.0f);
	DECLARE_COLOR(colorOffset,		0.0f, 0.0f, 0.0f);
	DECLARE_SCALAR(contrast,		1.0f);
	DECLARE_COLOR(defaultColor,		0.5, 0.5f, 0.5f);
	DECLARE_SCALAR(filter,			1.0f);
	DECLARE_SCALAR(filterOffset,	0.0f);
	DECLARE_BOOL(invert,			eiFALSE);
	DECLARE_VECTOR(uvCoord_,			0.0f, 0.0f, 0.0f);//only uvCoord[0],uvCoord[1] are used.
	DECLARE_SCALAR(outAlpha,		0.5f);// Outputs - begin
	DECLARE_COLOR(outColor,			0.5f, 0.5f, 0.5f);
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
		scalar alpha = outAlpha();

		float ss = uvCoord().x;
		float tt = uvCoord().y;

		if(ISUVDEFINED(ss, tt))
		{
			ss = fmodf(ss, WRAPMAX);
			tt = fmodf(tt, WRAPMAX);

			/* compute 'ss' and 'tt' filter widths */
			vector dUVdu, dUVdv;
			Duv(vector(ss, tt, 0.0f), dUVdu, dUVdv);
			float dss = abs(dUVdu.x * du()) + abs(dUVdv.x * dv());
			float dtt = abs(dUVdu.y * du()) + abs(dUVdv.y * dv());


			/* Add in "Effects" filter values. We multiplie the i_filterOffset
			variable by 2 to match Maya's look */ 
			dss = dss * filter() + filterOffset()*2.0f; 
			dtt = dtt * filter() + filterOffset()*2.0f; 

			/* compute separation: 0 for half the squares, 1 for the others. */
			float f = 0.5f - 2.0f *
				(filteredpulsetrain(0.5f, 1.0f, ss, dss) - 0.5f) *
				(filteredpulsetrain(0.5f, 1.0f, tt, dtt) - 0.5f);

			/* contrast interpolates the separation from 0.5 to its normal value. */
			f = 0.5f + (f - 0.5f) * contrast();

			/* Compute final values. */
			alpha = 1.0f - f;
			Ci() = color1() + (color2() - color1()) * f;

			colorBalance(Ci(), 
				alpha,
				alphaIsLuminance(),
				alphaGain(),
				alphaOffset(),
				colorGain(),
				colorOffset(),
				invert() );
		}
		else
		{
			Ci() = defaultColor();
			alpha = luminance( defaultColor() );
		}

		// set output
		outColor() = Ci();
		outAlpha() = alpha;
	}


END(maya_checker)

SURFACE(maya_checker_uv)

	PARAM(color, color1);
	PARAM(color, color2);
	PARAM(vector,uvCoord);
	PARAM(color, outColor);

	void parameters(int pid)
	{
		DECLARE_COLOR(color1, 0.0f, 0.0f, 0.0f);
		DECLARE_COLOR(color2, 1.0f, 1.0f, 1.0f);
		DECLARE_VECTOR(uvCoord, 2.0f, 2.0f, 0.0f);
		DECLARE_COLOR(outColor, 0.0f, 0.0f, 0.0f);
	}

	void init()
	{
	}

	void exit()
	{
	}

	void main(void *arg)
	{
		scalar	x, y;

		//1
		//x = uvCoord().x;
		//y = uvCoord().y;
		//2
		//x = fmodf( u() * uvCoord().x, 1.0f);
		//y = fmodf( v() * uvCoord().y, 1.0f);
		//3
		x =  u() * uvCoord().x;
		y =  v() * uvCoord().y;

		//if ( floor( x * 2 ) == floor( y * 2 ) )
		if ((((eiInt)x + (eiInt)y) % 2) == 0)
		{
			outColor() = color1();
		}
		else
		{
			outColor() = color2();
		}
	}

END(maya_checker_uv)

SURFACE(maya_simple_texture)

	PARAM(eiTag, texture);
	PARAM(color, outColor);

	void parameters(int pid)
	{
		DECLARE_TAG(texture, eiNULL_TAG);
		DECLARE_COLOR(outColor, 0.0f, 0.0f, 0.0f);
	}

	void init()
	{
	}

	void exit()
	{
	}

	void main(void *arg)
	{
		outColor() = color(0.0f, 0.0f, 0.0f);

		eiTag tex = texture();

		if (tex != eiNULL_TAG)
		{
			outColor() = color_texture(tex, 0, get_state()->bary.x, get_state()->bary.y);
		}
	}

END(maya_simple_texture)
