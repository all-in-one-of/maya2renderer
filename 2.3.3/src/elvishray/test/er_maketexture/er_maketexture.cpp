#include <cassert>
#include <iostream>
#include <string>
#include <boost/algorithm/string/replace.hpp>
#include "eiAPI/ei.h"
#include <eiCORE/ei_data_table.h>

void er_maketexture(const std::string &source_image_dir, 
					const std::string &source_image_name, 
					const std::string &source_image_ext);

int main(int argc,char *argv[])
{

	if( argc <= 1 ){
		printf("argc=%d, no source image parameter\n", argc);
		return -1;
	}
	const std::string source_image_path(argv[1]);

	std::size_t last_slash = source_image_path.find_last_of("/\\");
	if( last_slash == std::string::npos )
	{
		printf("argv[1]=%s, no source image name\n", argv[1]);
		return -1;	
	}

	const std::string source_image_dir(source_image_path.substr(0, last_slash));
	const std::string source_image_name(source_image_path.substr(last_slash+1));

	std::size_t last_dot = source_image_name.find_last_of('.');
	if( last_dot == std::string::npos )
	{
		printf(" source image<%s> has no extension\n", source_image_name.c_str());
		return -1;	
	}
	const std::string source_image_ext(source_image_name.substr(last_dot+1));

	er_maketexture(source_image_dir, source_image_name, source_image_ext);
	
	return 0;
}
void er_maketexture(const std::string &source_image_dir, 
					const std::string &source_image_name, 
					const std::string &source_image_ext)
{
	char	tmp[ EI_MAX_FILE_NAME_LEN ];
	ei_get_current_directory(tmp);
	std::string cur_dir(tmp);
	cur_dir = boost::replace_all_copy( cur_dir, "\\", "/" );

	char	output_filename[ EI_MAX_FILE_NAME_LEN ];



	ei_context();
	ei_verbose( 6 );
	ei_link( "eiIMG" );
	ei_link( "eiSHADER" );
	ei_link( "eiSHADER_maya" );

	//############################### option #;
	ei_options("perspShape_option");
	ei_contrast(0.05);
	ei_samples(0,2);
	ei_filter(4,3);
	//transform motion=0, deform motion=0;
	ei_motion(0);
	ei_trace_depth(4,4,4,4,4,4);
	ei_displace(0);
	ei_face(3);
	ei_end_options();
	// maya settings: focal=35, aperture = 46.8, aspect=1.33333;
	//Depth of Field on camera "|persp|perspShape" is turned off in Maya;

	//############################### camera #;
	ei_camera("|persp|perspShape_object");
	//beauty channel;
	ei_output( (cur_dir+"/er_maketexture_output.bmp").c_str(), "bmp",1);
	printf("rendering result:%s\n",(cur_dir+"/er_maketexture_output.bmp").c_str());
	//mode=rgb;
	ei_output_variable("color",11);
	ei_end_output();
	// AOV;
	// AOV end;
	ei_focal(35);
	ei_aperture(36);
	ei_aspect(1.33333);
	ei_resolution(800,600);
	ei_clip(0.1,10000);
	ei_end_camera();
	//----------------------------------;
	ei_instance("|persp|perspShape");
	ei_element("|persp|perspShape_object");
	ei_transform(0.999945,0,-0.0104718,0,   -0.0104706,0.015038,-0.999832,0,   0.000157475,0.999887,0.0150372,0,   -2.6803,10.6466,-0.386112,1);
	ei_end_instance();
	//;
	ei_shader("maya_place2dTexture", "place2dTexture1");
	ei_shader_param_int("liq_UserDefinedU",0);
	ei_shader_param_int("liq_UserDefinedV",0);
	ei_shader_param_vector("i_uvCoord",0,0,0);
	ei_shader_param_scalar("i_coverageU", 1);
	ei_shader_param_scalar("i_coverageV", 1);
	ei_shader_param_bool("i_mirrorU", 0);
	ei_shader_param_bool("i_mirrorV", 0);
	ei_shader_param_scalar("i_noiseU", 0);
	ei_shader_param_scalar("i_noiseV", 0);
	ei_shader_param_scalar("i_offsetU", 0);
	ei_shader_param_scalar("i_offsetV", 0);
	ei_shader_param_scalar("i_repeatU", 1);
	ei_shader_param_scalar("i_repeatV", 1);
	ei_shader_param_scalar("i_rotateFrame", 0);
	ei_shader_param_scalar("i_rotateUV", 0);
	ei_shader_param_bool("i_stagger", 0);
	ei_shader_param_scalar("i_translateFrameU", 0);
	ei_shader_param_scalar("i_translateFrameV", 0);
	ei_shader_param_bool("i_wrapU", 1);
	ei_shader_param_bool("i_wrapV", 1);
	ei_shader_param_vector("o_outUV", 0, 0, 0);
	ei_end_shader();

	//make texture
	const std::string src_image_fullpath(source_image_dir+"/"+source_image_name);
	std::string des_image_fullpath(cur_dir+"/"+source_image_name);

	if(source_image_ext != "tex")
	{
		des_image_fullpath += ".tex";
		ei_make_texture( 
			src_image_fullpath.c_str(),
			des_image_fullpath.c_str(),
			EI_TEX_WRAP_CLAMP, EI_TEX_WRAP_CLAMP, EI_FILTER_BOX, 1.0f, 1.0f);
	}
	printf("des image:%s\n", des_image_fullpath.c_str());

	ei_texture( src_image_fullpath.c_str() );
	ei_file_texture( des_image_fullpath.c_str(), eiFALSE);
	ei_end_texture();


	ei_shader("maya_file", "file1");
	ei_shader_param_scalar("i_alphaGain", 1);
	ei_shader_param_bool("i_alphaIsLuminance", 0);
	ei_shader_param_scalar("i_alphaOffset", 0);
	ei_shader_param_vector("i_colorGain", 1, 1, 1);
	ei_shader_param_vector("i_colorOffset", 0, 0, 0);
	ei_shader_param_vector("i_defaultColor", 0.5, 0.5, 0.5);
	ei_shader_link_param("i_uvCoord", "place2dTexture1", "o_outUV");
	ei_shader_param_texture("i_fileTextureName", src_image_fullpath.c_str() );
	ei_shader_param_index("i_filterType", 3);
	ei_shader_param_scalar("i_filter", 1);
	ei_shader_param_scalar("i_filterOffset", 0);
	ei_shader_param_bool("i_invert", 0);
	ei_shader_param_bool("i_fileHasAlpha", 0);
	ei_shader_param_scalar("o_outAlpha", 1);
	ei_shader_param_vector("o_outColor", 0, 0, 0);
	ei_shader_param_vector("o_outTransparency", 0, 0, 0);
	ei_end_shader();

	ei_shader("maya_phong", "phong1");
	ei_shader_param_vector("i_ambientColor", 0, 0, 0);
	ei_shader_link_param("i_color", "file1", "o_outColor");
	ei_shader_param_vector("i_transparency", 0, 0, 0);
	ei_shader_param_vector("i_incandescence", 0, 0, 0);
	ei_shader_param_int("liq_UserDefinedNormal", 0);
	ei_shader_param_vector("i_normalCamera", 1, 1, 1);
	ei_shader_param_scalar("i_diffuse", 0.8);
	ei_shader_param_scalar("i_translucence", 0);
	ei_shader_param_scalar("i_translucenceDepth", 0.5);
	ei_shader_param_scalar("i_translucenceFocus", 0.5);
	ei_shader_param_scalar("i_cosinePower", 20);
	ei_shader_param_vector("i_specularColor", 0.5, 0.5, 0.5);
	ei_shader_param_scalar("i_reflectivity", 0.5);
	ei_shader_param_vector("i_reflectedColor", 0, 0, 0);
	ei_shader_param_index("i_matteOpacityMode", 2);
	ei_shader_param_scalar("i_matteOpacity", 1);
	ei_shader_param_index("i_reflectionLimit", 1);
	ei_shader_param_vector("o_outColor", 0, 0, 0);
	ei_shader_param_vector("o_outTransparency", 0, 0, 0);
	ei_end_shader();

	ei_material("phong1SG");
	ei_surface_shader("phong1");
	ei_end_material();

	//worldPrologue;

	// Renderer::exportLight("_pointLight1_pointLightShape1");;

	// Renderer::exportPointLight();
	ei_shader("pointlight", "|pointLight1|pointLightShape1_shader");
	ei_shader_param_vector("lightcolor",1,1,1);
	ei_shader_param_scalar("intensity",1);
	ei_end_shader();
	ei_light("|pointLight1|pointLightShape1_object");
	ei_light_shader("|pointLight1|pointLightShape1_shader" );
	ei_origin(0,0,0);
	ei_end_light();
	ei_instance("|pointLight1|pointLightShape1");
	ei_element("|pointLight1|pointLightShape1_object");
	ei_transform(1,0,0,0,   0,1,0,0,   0,0,1,0,   0,5.23026,0,1);
	{;
	eiInt tag = 0;
	ei_declare("|pPlane2_lightgroup",0,4, &tag);
	};
	ei_end_instance();

	// Renderer::exportLight("_pointLight2_pointLightShape2");;

	// Renderer::exportPointLight();
	ei_shader("pointlight", "|pointLight2|pointLightShape2_shader");
	ei_shader_param_vector("lightcolor",1,1,1);
	ei_shader_param_scalar("intensity",1);
	ei_end_shader();
	ei_light("|pointLight2|pointLightShape2_object");
	ei_light_shader("|pointLight2|pointLightShape2_shader" );
	ei_origin(0,0,0);
	ei_end_light();
	ei_instance("|pointLight2|pointLightShape2");
	ei_element("|pointLight2|pointLightShape2_object");
	ei_transform(1,0,0,0,   0,1,0,0,   0,0,1,0,   -5.18346,1.22941,0,1);
	{;
	eiInt tag = 0;
	ei_declare("|pPlane2_lightgroup",0,4, &tag);
	};
	ei_end_instance();

	// Renderer::exportOneGeometry_Mesh(|pPlane2|pPlaneShape11,0,0);

	//############################### mesh #;
	//shape full path name=|pPlane2|pPlaneShape11;
	ei_object("poly","|pPlane2|pPlaneShape11");
	{;
	eiTag tag;
	//### vertex positions, fnMesh.numVertices()=9;
	tag = ei_tab(EI_TYPE_VECTOR, 1, 1024);
	ei_pos_list(tag);
	ei_tab_add_vector(-0.5,-1.11022e-016,0.5);
	ei_tab_add_vector(0,-1.11022e-016,0.5);
	ei_tab_add_vector(0.5,-1.11022e-016,0.5);
	ei_tab_add_vector(-0.5,0,0);
	ei_tab_add_vector(0,0,0);
	ei_tab_add_vector(0.5,0,0);
	ei_tab_add_vector(-0.5,1.11022e-016,-0.5);
	ei_tab_add_vector(0,1.11022e-016,-0.5);
	ei_tab_add_vector(0.5,1.11022e-016,-0.5);
	ei_end_tab();
	//### N ###;
	tag = eiNULL_TAG;
	ei_declare("N",2,6, &tag);
	tag = ei_tab(EI_TYPE_VECTOR, 1, 1024);
	ei_variable("N", &tag);
	ei_tab_add_vector(0,1,2.22045e-016);
	ei_tab_add_vector(0,1,2.22045e-016);
	ei_tab_add_vector(0,1,2.22045e-016);
	ei_tab_add_vector(0,1,2.22045e-016);
	ei_tab_add_vector(0,1,2.22045e-016);
	ei_tab_add_vector(0,1,2.22045e-016);
	ei_tab_add_vector(0,1,2.22045e-016);
	ei_tab_add_vector(0,1,2.22045e-016);
	ei_tab_add_vector(0,1,2.22045e-016);
	ei_end_tab();
	//### N ### end;
	//### UV(map1), size=9;
	tag = eiNULL_TAG;
	ei_declare("uv",2,6, &tag);
	tag = ei_tab(EI_TYPE_VECTOR2, 1, 1024);
	ei_variable("uv", &tag);
	ei_tab_add_vector2(0,0);
	ei_tab_add_vector2(0.5,0);
	ei_tab_add_vector2(1,0);
	ei_tab_add_vector2(0,0.5);
	ei_tab_add_vector2(0.5,0.5);
	ei_tab_add_vector2(1,0.5);
	ei_tab_add_vector2(0,1);
	ei_tab_add_vector2(0.5,1);
	ei_tab_add_vector2(1,1);
	ei_end_tab();
	//### triangles, size=4;
	tag = ei_tab(EI_TYPE_INDEX, 1, 1024);
	ei_triangle_list(tag);
	ei_tab_add_index(0);
	ei_tab_add_index(1);
	ei_tab_add_index(3);
	ei_tab_add_index(3);
	ei_tab_add_index(1);
	ei_tab_add_index(4);
	ei_tab_add_index(1);
	ei_tab_add_index(2);
	ei_tab_add_index(4);
	ei_tab_add_index(4);
	ei_tab_add_index(2);
	ei_tab_add_index(5);
	ei_tab_add_index(3);
	ei_tab_add_index(4);
	ei_tab_add_index(6);
	ei_tab_add_index(6);
	ei_tab_add_index(4);
	ei_tab_add_index(7);
	ei_tab_add_index(4);
	ei_tab_add_index(5);
	ei_tab_add_index(7);
	ei_tab_add_index(7);
	ei_tab_add_index(5);
	ei_tab_add_index(8);
	ei_end_tab();
	}//|pPlane2|pPlaneShape11;
	ei_end_object();
	//--------------------------;
	//ribNode->name=|pPlane2|pPlaneShape11;
	//ribNode's transform node=|pPlane2;
	//ribNode->object(0)->getDataPtr()->getFullPathName()=|pPlane2|pPlaneShape11;
	ei_instance("|pPlane2");
	ei_mtl("phong1SG");
	//shape name=|pPlane2|pPlaneShape11;
	//shape full path name=|pPlane2|pPlaneShape11;
	ei_element("|pPlane2|pPlaneShape11");
	ei_transform(9.6133,0,0,0,   0,1,0,0,   0,0,8.02763,0,   -2.76716,0,-0.566055,1);
	//ribNode->doDef=205, ribNode->doMotion=205;
	ei_motion(1);
	{//light group(light-link group);
	const char *tag = NULL;
	ei_declare("lightgroup",0,1, &tag);
	tag = ei_token("|pPlane2_lightgroup");;
	ei_variable("lightgroup", &tag);
	};
	ei_end_instance();
	//;

	//Renderer::worldEpilogue();
	ei_instgroup("perspShape");
	//camera;
	ei_add_instance("|persp|perspShape");
	//light(s);
	ei_add_instance("|pointLight1|pointLightShape1");
	ei_add_instance("|pointLight2|pointLightShape2");
	//mesh(s);
	ei_add_instance("|pPlane2");
	ei_end_instgroup( );
	//ei_set_connection( &(MayaConnection::getInstance()->connection.base );
	ei_render("perspShape", "|persp|perspShape", "perspShape_option");
	ei_end_context();

}