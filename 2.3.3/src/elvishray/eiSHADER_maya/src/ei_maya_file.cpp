
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
	//DECLARE_INT(frameExtension, 0);
	//DECLARE_INT(frameOffset, 0);
	//DECLARE_BOOL(useFrameExtension, eiFALSE);
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
// The following code is added in 3delight 10.0.50, 
// but this code should be move to getFileNodeImageName() to cook the "fileName" parameter.
// 		uniform string fileName = i_fileName;
// 
// 		// If there is a "spriteNumPP" primitive variable, file node must use this as
// 		// an additional offset, and automatically wrap if the resulting image index
// 		// is larger than the frame sequence length.
// 		//
// 		uniform float spriteNumPP = 0;	
// 		float has_spriteNumPP = getvar( null, "spriteNumPP", spriteNumPP );
// 
// 		// ugly capitalization but fits with what is produced by the "sprite wizard"
// 		uniform float spriteCycleLength = 0;	
// 		float has_spriteCycleLength = 
// 			getvar( null, "SpriteCycleLength", spriteCycleLength );
// 
// 		uniform float spriteAnimation = 1;
// 		getvar( null, "SpriteAnimation", spriteAnimation );
// 
// 		if( i_useFrameExtension > 0 )
// 		{
// 			float curr_texture_frame = i_frameExtension + i_frameOffset;
// 
// 			// SpriteNumPP overrides the current frame (and discards the frame offset)
// 			// This is the Maya behaviour.
// 			//
// 			if( spriteAnimation > 0 )
// 			{
// 				if( has_spriteNumPP > 0 )
// 				{
// 					curr_texture_frame = floor( spriteNumPP );
// 				}
// 
// 				if( has_spriteCycleLength > 0 && spriteCycleLength > 0 )
// 				{
// 					curr_texture_frame = 
// 						1 + mod( curr_texture_frame, floor(spriteCycleLength) - 1 );
// 				}
// 			}
// 
// 			fileName = format(i_fileName, curr_texture_frame);
// 		}
		if(1)//ISUVDEFINED(uvCoord().x, uvCoord().y)
		{
			if (fileTextureName() != eiNULL_TAG)
			{
				eiTag tex = fileTextureName();

				scalar filterWidth = filterType()>0.0f ? filter() : 0.0f;

				// liquid ingore single-channel case
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
			colorBalance(outColor(), 
				outAlpha(),
				alphaIsLuminance(),
				alphaGain(),
				alphaOffset(),
				colorGain(),
				colorOffset(),
				invert());
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