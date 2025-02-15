
//////////////////////////////////////////////////////////////////////////
/// renderman shader
// void lambert3 (
// 			   output vector outColor;
// )
// {
// 	vector inColor = vector ( 0.8, 0.8, 0.8 );
// 	vector transparency = vector ( 0, 0, 0 );
// 	vector ambientColor = vector ( 0, 0, 0 );
// 	vector incandescence = vector ( 0, 0, 0 );
// 	float diffusion = float 0.8;
// 	vector outTransparency = vector ( 0, 0, 0 );
// 	extern normal N;
// 	normal Nn = normalize( N );
// 	outTransparency = transparency;
// 	Oi = Os * color ( 1 - outTransparency );
// 	outColor = incandescence +
// 		( inColor * ( diffusion * 
// 		vector diffuse( Nn ) +
// 		ambientColor ) );
// 	Ci = Cs * Oi * color outColor;
// }
// 
// void lambert1 (
// 			   vector transparency;
// )
// {
// 	vector inColor = vector ( 0.5, 0.5, 0.5 );
// 	vector ambientColor = vector ( 0, 0, 0 );
// 	vector incandescence = vector ( 0, 0, 0 );
// 	float diffusion = float 0.8;
// 	vector outColor = vector ( 0, 0, 0 );
// 	vector outTransparency = vector ( 0, 0, 0 );
// 	extern normal N;
// 	normal Nn = normalize( N );
// 	outTransparency = transparency;
// 	Oi = Os * color ( 1 - outTransparency );
// 	outColor = incandescence +
// 		( inColor * ( diffusion * 
// 		vector diffuse( Nn ) +
// 		ambientColor ) );
// 	Ci = Cs * Oi * color outColor;
// }
// 
// surface lambert1()
// {
// 	vector lambert3_outColor;
// 
// 	lambert3(lambert3_outColor);
// 	lambert1(lambert3_outColor);
// }

//////////////////////////////////////////////////////////////////////////

#include "eiAPI/ei.h"

// void lambert3(
// )
// {
// 	ei_shader("lambert3");
// 		ei_shader_param_string("desc", "plastic");
// 		ei_shader_param_vector("Cs", 0.8, 0.8, 0.8);
// 		ei_shader_param_vector("Kd", 1.0f, 1.0f, 1.0f);//?
// 		ei_shader_param_scalar("Ks", 0.0f);
// 		ei_shader_param_scalar("roughness", 0.2f);//?
// 	ei_end_shader();
// }

// void lambert1(
// 	const char *param_name, const char *src_shader_name, const char *src_param_name
// )
// {
// 	ei_shader("lambert1");
// 		ei_shader_param_string("desc", "maya_lambert");
// 		ei_shader_param_vector("Cs",  0.5, 0.5, 0.5 );
// 		if(param_name && src_shader_name && src_param_name){
// 			ei_shader_link_param(param_name, src_shader_name, src_param_name);//
// 		}
// 		ei_shader_param_vector("Kd", 1.0f, 1.0f, 1.0f);
// 		ei_shader_param_scalar("Ks", 0.0);
// 		ei_shader_param_scalar("roughness", 0.2f);//?
// 	ei_end_shader();
// }
// void surface_lambert1()
// {
// // 	lambert3();
// // 	lambert1("Os", "lambert3", "result");
// }
//
////////////////////////////////////////////////////////
void er_shader_transparent_test( )
{
	char	cur_dir[ EI_MAX_FILE_NAME_LEN ];
	char	filename[ EI_MAX_FILE_NAME_LEN ];
	char	output_filename[ EI_MAX_FILE_NAME_LEN ];
	char	picturename[ EI_MAX_FILE_NAME_LEN ];
	char	texturename1[ EI_MAX_FILE_NAME_LEN ];
	char	texturename2[ EI_MAX_FILE_NAME_LEN ];

	ei_get_current_directory(cur_dir);

	ei_context(ei_create_context());

	ei_verbose(EI_VERBOSE_ALL);
	ei_link("eiIMG");
	ei_link("eiSHADER");
	ei_link("eiSHADER_maya");

	ei_options("opt");
		ei_samples(0, 2);
		ei_contrast(0.05f, 0.05f, 0.05f, 0.05f);
		ei_filter(EI_FILTER_GAUSSIAN, 3.0f);
	ei_end_options();


	ei_camera("cam1");
		ei_append_filename(output_filename, cur_dir, "frame_shader_transparent_01.bmp");
		ei_output(output_filename, "bmp", EI_IMG_DATA_RGB);
		ei_output_variable("color", EI_DATA_TYPE_VECTOR);
		ei_end_output();
		ei_focal(100.0f);
		ei_aperture(144.724029f);
		ei_aspect(800.0f / 600.0f);
		ei_resolution(800, 600);
	ei_end_camera();

	ei_instance("caminst1");
		ei_element("cam1");
	ei_end_instance();


	ei_shader("point_light_shader");
		ei_shader_param_string("desc", "pointlight");
		ei_shader_param_scalar("intensity", 1.0f);
		ei_shader_param_vector("lightcolor", 1.0f, 1.0f, 1.0f);
	ei_end_shader();

	ei_light("light1");
		ei_add_light("point_light_shader");
		ei_origin(141.375732f, 83.116005f, 35.619434f);
	ei_end_light();

	ei_instance("lightinst1");
		ei_element("light1");
	ei_end_instance();


	//surface_lambert1();
	ei_shader("lambert1");
		ei_shader_param_string("desc", "maya_lambert_transparent"); //"" //plastic
		ei_shader_param_vector("Cs", 0.8, 0.0, 0.8);
		ei_shader_param_vector("Kd", 1.0f, 1.0f, 1.0f);//?
		ei_shader_param_scalar("Ks", 0.0f);
		ei_shader_param_scalar("roughness", 0.2f);//?
		ei_shader_param_scalar("opacity", 0.8f);
	ei_end_shader();

	ei_shader("opaque_shadow");
		ei_shader_param_string("desc", "transparent"); // //"opaque"
	ei_end_shader();

	ei_material("mtl");
		ei_add_surface("lambert1");
		//ei_add_shadow("opaque_shadow");
	ei_end_material();

	ei_object("obj1", "poly");
	ei_pos_list(ei_tab(EI_DATA_TYPE_VECTOR, 1024));
	ei_tab_add_vector(-7.068787f, -4.155799f, -22.885710f);
	ei_tab_add_vector(-0.179573f, -7.973234f, -16.724060f);
	ei_tab_add_vector(-7.068787f, 4.344949f, -17.619093f);
	ei_tab_add_vector(-0.179573f, 0.527515f, -11.457443f);
	ei_tab_add_vector(0.179573f, -0.527514f, -28.742058f);
	ei_tab_add_vector(7.068787f, -4.344948f, -22.580408f);
	ei_tab_add_vector(0.179573f, 7.973235f, -23.475441f);
	ei_tab_add_vector(7.068787f, 4.155800f, -17.313791f);
	ei_end_tab();
	ei_triangle_list(ei_tab(EI_DATA_TYPE_INDEX, 1024));
	ei_tab_add_index(0); ei_tab_add_index(1); ei_tab_add_index(3);
	ei_tab_add_index(0); ei_tab_add_index(3); ei_tab_add_index(2);
	ei_tab_add_index(1); ei_tab_add_index(5); ei_tab_add_index(7);
	ei_tab_add_index(1); ei_tab_add_index(7); ei_tab_add_index(3);
	ei_tab_add_index(5); ei_tab_add_index(4); ei_tab_add_index(6);
	ei_tab_add_index(5); ei_tab_add_index(6); ei_tab_add_index(7);
	ei_tab_add_index(4); ei_tab_add_index(0); ei_tab_add_index(2);
	ei_tab_add_index(4); ei_tab_add_index(2); ei_tab_add_index(6);
	ei_tab_add_index(4); ei_tab_add_index(5); ei_tab_add_index(1);
	ei_tab_add_index(4); ei_tab_add_index(1); ei_tab_add_index(0);
	ei_tab_add_index(2); ei_tab_add_index(3); ei_tab_add_index(7);
	ei_tab_add_index(2); ei_tab_add_index(7); ei_tab_add_index(6);
	ei_end_tab();
	ei_end_object();

	ei_instance("inst1");
	ei_add_material("mtl");
	ei_element("obj1");
	ei_end_instance();

	ei_instance("inst2");
	ei_add_material("mtl");
	ei_element("obj1");
	ei_transform(
		1.0, 0.0, 0.0, 0.0,
		0.0, 1.0, 0.0, 0.0,
		0.0, 0.0, 1.0, 0.0,
		3.0, 3.0, 3.0, 1.0
		);
	ei_end_instance();

	ei_instgroup("world");
	ei_add_instance("caminst1");
	ei_add_instance("lightinst1");
	ei_add_instance("inst1");
	ei_add_instance("inst2");
	ei_end_instgroup();

	// render frame 01
	ei_render("world", "caminst1", "opt");

	ei_delete_context(ei_context(NULL));
}
