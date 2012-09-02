#include <liqConfig.h>
#ifdef _USE_APPLESEED_


#include"as_helper.h"

#include "../common/prerequest_std.h"
#include "../common/prerequest_maya.h"
#include "../common/mayacheck.h"
#if USE_OPENEXR_CPP_INTERFACE
#include <openexr/ImfRgbaFile.h>
#include <openexr/ImfArray.h>
#include <openexr/half.h>
#endif
#include <liqGlobalHelpers.h>
#include <liqlog.h>
#include "as_GlobalNodeHelper.h"

namespace appleseed
{
	void makeTexture(const char* srcImagePath, const char* desImagePath)
	{
		CM_TRACE_FUNC("makeTexture("<<srcImagePath<<","<<desImagePath<<")");
#if USE_OPENEXR_CPP_INTERFACE
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
	//
	std::string getTextureInstanceName(const std::string& texturename)
	{
		return texturename + "_inst";
	}
	//
	const char const* frame_renderer[]={"generic", "progressive"};
	const char const* tile_renderer[] ={"generic", "blank", "debug", "ewatest"};
	const char const* sample_renderer[] ={"generic"};
	const char const* sample_generator[] ={"generic", "lighttracing"};
	const char const* lighting_engine[] ={"dr", "pt"};
	const char const* sample_filter_type[] ={"mitchell", "box"};
	std::string getConfiguration(GlobalNodeHelper *node, const std::string &category, const std::string &name)
	{
		assert( node != NULL );
		MString plugname((category+"_"+name).c_str());

		std::string ret(("unknown name:"+plugname).asChar());

		if(name == "frame_renderer"){
			ret = frame_renderer[ node->getInt(plugname) ];
		}	
		else if(name == "tile_renderer"){
			ret = tile_renderer[ node->getInt(plugname) ];
		}	
		else if(name == "sample_renderer"){
			ret = sample_renderer[ node->getInt(plugname) ];
		}	
		else if(name == "sample_generator"){
			ret = sample_generator[ node->getInt(plugname) ];
		}	
		else if(name == "lighting_engine"){
			ret = lighting_engine[ node->getInt(plugname) ];
		}	
		else if(name == "min_samples"){
			MString str;
			str.set( node->getInt(plugname) );
			ret = str.asChar();
		}
		else if(name == "max_samples"){
			MString str;
			str.set( node->getInt(plugname) );
			ret = str.asChar();
		}
		else if(name == "sample_filter_size"){
			MString str;
			str.set( node->getInt(plugname) );
			ret = str.asChar();
		}
		else if(name == "sample_filter_type"){
			ret = sample_filter_type[ node->getInt(plugname) ];
		}
		return ret;
	}
	//
	std::string as_get_export_mesh_to()
	{
		return "E:/MyDocuments/maya/projects/default/as_mesh";
	}
	std::string get_filesys_fullPathName(const std::string &meshFullPathName)
	{
		return (as_get_export_mesh_to() + sanitizeNodeName2_ToFileSystemPath(meshFullPathName) +".obj");
	}
	std::string get_mesh_dirname(const std::string& meshFullPathName)
	{
		const std::string fullPathName(get_filesys_fullPathName(meshFullPathName));
		std::size_t found = fullPathName.find_last_of('/');
		return  fullPathName.substr(0,found);
	}
	std::string get_mesh_basename(const std::string& meshFullPathName)
	{
		const std::string fullPathName(get_filesys_fullPathName(meshFullPathName));
		std::size_t found = fullPathName.find_last_of('/');
		return  fullPathName.substr(found+1);
	}

	//
	static const std::string BSDF_NAME_SEPERATOR("_");
	static const std::string EDF_NAME_SEPERATOR("_");
	static const std::string SURFACE_SHADER_NAME_SEPERATOR("_");
	std::string getBSDFName(const std::string& node, const std::string & bsdf_model)
	{
		return node + BSDF_NAME_SEPERATOR + bsdf_model;
	}
	std::string getEDFName(const std::string& node, const std::string & edf_model)
	{
		return node + EDF_NAME_SEPERATOR + edf_model;
	}
	std::string getSurfaceShaderName(const std::string& node, const std::string & surface_shader_model)
	{
		return node + SURFACE_SHADER_NAME_SEPERATOR + surface_shader_model;
	}
	std::string getBSDFName(const std::string& node)
	{
		return node + BSDF_NAME_SEPERATOR + "BSDF";
	}
	std::string getEDFName(const std::string& node)
	{
		return node + EDF_NAME_SEPERATOR + "EDF";
	}
	std::string getSurfaceShaderName(const std::string& node)
	{
		return node + SURFACE_SHADER_NAME_SEPERATOR + "SurfaceShader";
	}
	std::string getAlphaMapName(const std::string& node)
	{
		return node + SURFACE_SHADER_NAME_SEPERATOR + "AlphaMap";
	}
	std::string getNormalMapName(const std::string& node)
	{
		return node + SURFACE_SHADER_NAME_SEPERATOR + "NormalMap";
	}
	//
	bool createColor3(asr::ColorContainer& colors, const char* name, const float r, const float g, const float b)
	{
		if( colors.get_by_name(name) == nullptr)//color not exist
		{
			const float color[] = { r, g, b };
			colors.insert(
				asr::ColorEntityFactory::create(
				name,
				asr::ParamArray().insert("color_space", "srgb"),
				asr::ColorValueArray(3, color)
				)
			);
			return true;
		}else{
			return false;
		}
	}
	bool createColor3(asr::ColorContainer& colors, const char* name, const float multiplier, const float r, const float g, const float b)
	{
		if( colors.get_by_name(name) == nullptr)//color not exist
		{
			MString strMultiplier;
			strMultiplier.set(multiplier);

			const float color[] = { r, g, b };
			colors.insert(
				asr::ColorEntityFactory::create(
				name,
				asr::ParamArray()
				.insert("color_space", "srgb")
				.insert("multiplier",  strMultiplier.asChar() ),
				asr::ColorValueArray(3, color)
				)
			);
			return true;
		}else{
			return false;
		}
	}
	bool createColor4(asr::ColorContainer& colors, const char* name, const float r, const float g, const float b, const float a)
	{
		if( colors.get_by_name(name) == nullptr)//color not exist
		{
			const float color[] = { r, g, b, a };
			colors.insert(
				asr::ColorEntityFactory::create(
				name,
				asr::ParamArray().insert("color_space", "srgb"), 
				asr::ColorValueArray(4, color),
				asr::ColorValueArray(1, color+3)
				)
			);
			return true;
		}else{
			return false;
		}
	}
	bool createColor4(asr::ColorContainer& colors, const char* name, const float multiplier, const float r, const float g, const float b, const float a)
	{
		if( colors.get_by_name(name) == nullptr)//color not exist
		{
			MString strMultiplier;
			strMultiplier.set(multiplier);

			const float color[] = { r, g, b, a };
			colors.insert(
				asr::ColorEntityFactory::create(
				name,
				asr::ParamArray()
					.insert("color_space", "srgb")
					.insert("multiplier",  strMultiplier.asChar() ), 
				asr::ColorValueArray(4, color),
				asr::ColorValueArray(1, color+3)
				)
			);
			return true;
		}else{
			return false;
		}
	}

}//namespace appleseed
#endif//_USE_APPLESEED_