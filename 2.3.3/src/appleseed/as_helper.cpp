#include <liqConfig.h>
#ifdef _USE_APPLESEED_


#include"as_helper.h"

#include "../common/prerequest_std.h"
#include "../common/prerequest_maya.h"
#include "../common/mayacheck.h"
//#undef min
//#undef max
#include <openexr/ImfRgbaFile.h>
#include <openexr/ImfArray.h>
#include <openexr/half.h>
#include <liqGlobalHelpers.h>
#include <liqlog.h>

namespace appleseed
{
	void makeTexture(const char* srcImagePath, const char* desImagePath)
	{
		CM_TRACE_FUNC("makeTexture("<<srcImagePath<<","<<desImagePath<<")");
#if 0
		MImage srcImage;
		
		float* src = NULL;	
		if(src==NULL){
			IfMErrorWarn( srcImage.readFromFile(srcImagePath, MImage::kFloat) );
			src = srcImage.floatPixels();
		}
		if(src==NULL){
			IfMErrorWarn( srcImage.readFromFile(srcImagePath, MImage::kByte) );
			src = srcImage.floatPixels();
		}
		if(src==NULL){
			IfMErrorWarn( srcImage.readFromFile(srcImagePath, MImage::kUnknown) );
			src = srcImage.floatPixels();
		}
		assert( src != NULL );
		
		unsigned int width, height;
		IfMErrorWarn( srcImage.getSize(width, height) );

		srcImage.verticalFlip();
		int fChannels = srcImage.isRGBA() ? 4 : 3;

		Imf::Array<Imf::Rgba> pixels( width * height );


		assert( src !=NULL );
        Imf::Rgba* dest = pixels;
        for( int y = 0; y < height; y++)
		{
			for( int x = 0; x < width; x++)
			{
				dest->r = *src++;
				dest->g = *src++;
				dest->b = *src++;
				if( fChannels == 4)
					dest->a = *src++;
				dest++;
			}
		}

		Imf::RgbaOutputFile file(desImagePath, width, height, Imf::WRITE_RGBA);
		file.setFrameBuffer (pixels, 1, width);
		file.writePixels (height);
#else
		MString cmd;
		cmd = 
		"import os\n"
		"import sys\n"
		"import subprocess\n"
		"def convertTexToExr2(file_path, dest_file):\n"
		"	imf_copy_path = os.path.join(os.path.split(sys.path[0])[0], 'bin', 'imf_copy')\n"
		"	p = subprocess.Popen([imf_copy_path, file_path, dest_file])\n\n"
		"convertTexToExr2(\""+MString(srcImagePath)+"\", \""+MString(desImagePath)+"\")";

		IfMErrorMsgWarn(MGlobal::executePythonCommand(cmd, true, false), 
			"convertTexToExr2("+MString(srcImagePath)+", "+MString(desImagePath)+")");
#endif
	}

}//namespace appleseed
#endif//_USE_APPLESEED_