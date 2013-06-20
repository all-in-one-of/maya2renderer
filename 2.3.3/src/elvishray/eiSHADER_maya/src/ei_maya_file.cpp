
#include <eiAPI/ei_shaderx.h>
#include <shader/_3delight/utils.h>

SURFACE(maya_file)
	DECLARE;
	DECLARE_SCALAR(i_alphaGain, 1.0f);//input begin
	DECLARE_BOOL(  i_alphaIsLuminance, eiFALSE);
	DECLARE_SCALAR(i_alphaOffset, 0.0f);
	DECLARE_COLOR( i_colorGain, 1.0f, 1.0f, 1.0f);
	DECLARE_COLOR( i_colorOffset, 0.0f, 0.0f, 0.0f);
	DECLARE_COLOR( i_defaultColor, 0.5f, 0.5f, 0.5f);
	DECLARE_TAG(   i_fileTextureName, eiNULL_TAG);
	DECLARE_INDEX( i_filterType, 3);//Quadratic
	DECLARE_SCALAR(i_filter, 1.0f);
	DECLARE_SCALAR(i_filterOffset, 0.0f);
	DECLARE_BOOL(  i_invert, eiFALSE);
	DECLARE_VECTOR(i_uvCoord, 0.0f, 0.0f, 0.0f);//only unCoord[0] and unCoord[1] is used
	DECLARE_BOOL(  i_fileHasAlpha, eiFALSE);
	DECLARE_SCALAR(o_outAlpha, 1.0f);//output begin
	DECLARE_COLOR( o_outColor, 0.0f, 0.0f, 0.0f);
	DECLARE_COLOR( o_outTransparency, 0.0f, 0.0f, 0.0f);  
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
			if (i_fileTextureName() != eiNULL_TAG)
			{
				eiTag tex = i_fileTextureName();

				scalar filterWidth = i_filterType()>0.0f ? i_filter() : 0.0f;

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
					o_outColor() = color_texture(tex, 0, i_uvCoord().x, i_uvCoord().y);
				}

				if( i_fileHasAlpha() )
				{
					o_outAlpha() = scalar_texture(tex, 3, i_uvCoord().x, i_uvCoord().y);
				}else{
					o_outAlpha() = luminance( o_outColor() );
				}
				
			}
			colorBalance(o_outColor(), 
				o_outAlpha(),
				i_alphaIsLuminance(),
				i_alphaGain(),
				i_alphaOffset(),
				i_colorGain(),
				i_colorOffset(),
				i_invert());
		}
		else
		{
			o_outColor() = i_defaultColor();
			o_outAlpha() = luminance( o_outColor() );
		}
		o_outTransparency() = 
			color(1.0f-o_outAlpha(),1.0f-o_outAlpha(),1.0f-o_outAlpha());
	}

END(maya_file)