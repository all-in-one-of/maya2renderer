
#include <eiAPI/ei_shaderx.h>
#include "common/_3delight/utils.h"

SURFACE(maya_file)
	DECLARE;
	DECLARE_SCALAR(alphaGain, 1.0f);//input begin
	DECLARE_BOOL(alphaIsLuminance, eiFALSE);
	DECLARE_SCALAR(alphaOffset, 0.0f);
	DECLARE_COLOR(colorGain, 1.0f, 1.0f, 1.0f);
	DECLARE_COLOR(colorOffset, 0.0f, 0.0f, 0.0f);
	DECLARE_COLOR(defaultColor, 0.5f, 0.5f, 0.5f);
	DECLARE_TAG(fileTextureName, eiNULL_TAG);
	DECLARE_INDEX(filterType, 3);//Quadratic
	DECLARE_SCALAR(filter, 1.0f);
	DECLARE_SCALAR(filterOffset, 0.0f);
	DECLARE_BOOL(invert, eiFALSE);
	DECLARE_VECTOR(uvCoord, 0.0f, 0.0f, 0.0f);//only unCoord[0] and unCoord[1] is used
	DECLARE_BOOL(fileHasAlpha, eiFALSE);
	DECLARE_SCALAR(outAlpha, 1.0f);//output begin
	DECLARE_COLOR(outColor, 0.0f, 0.0f, 0.0f);
	DECLARE_COLOR(outTransparency, 0.0f, 0.0f, 0.0f);  
	END_DECLARE;
	//DECLARE_INDEX(num_channels, 3);//added from 3delight

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
		if(1)//ISUVDEFINED(uvCoord().x, uvCoord().y)
		{
			if (fileTextureName() != eiNULL_TAG)
			{
				eiTag tex = fileTextureName();

				scalar filterWidth = filterType()>0.0f ? filter() : 0.0f;

				// we ingore single-channel case
				//if( num_channels() == 1 )
				//{
				//	/* We must read single-channel files as float or only the red
				//	channel will be filled. */
				//	scalar r = scalar_texture(tex, 0, uvCoord().x, uvCoord().y)
				//	outColor() = color(r, 0.0f, 0.0f);
				//}
				//else
				{
					outColor() = color_texture(tex, 0, uvCoord().x, uvCoord().y);
				}

				if( fileHasAlpha() )
				{
					outAlpha() = scalar_texture(tex, 3, uvCoord().x, uvCoord().y);
				}else{
					outAlpha() = luminance( outColor() );
				}
				
			}
		}
		else
		{
			outColor() = defaultColor();
			outAlpha() = luminance( outColor() );
		}
		outTransparency() = 
			color(1.0f-outAlpha(),1.0f-outAlpha(),1.0f-outAlpha());
	}

END(maya_file)