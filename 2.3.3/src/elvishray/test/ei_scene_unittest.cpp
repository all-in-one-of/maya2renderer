/**************************************************************************
 * Copyright (C) 2011 Elvic Liang<len3dev@gmail.com>
 * All rights reserved.
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *************************************************************************/
 

#include <eiAPI/ei.h>


void ei_scene_unittest(bool useIBL)
{
	char	cur_dir[ EI_MAX_FILE_NAME_LEN ];
	char	output_filename[ EI_MAX_FILE_NAME_LEN ];

	ei_get_current_directory(cur_dir);

	ei_context();

	ei_verbose(EI_VERBOSE_ALL);
	ei_link("eiIMG");
	ei_link("eiSHADER");

	ei_options("opt");
		ei_samples(0, 16);
		ei_contrast(0.01f);
		ei_filter(EI_FILTER_GAUSSIAN, 3.0f);
	ei_end_options();

// 	if (useIBL)
// 	{
// 		ei_shader("physicalsky", "sky_shader");
// 		ei_end_shader();
// 	}

	ei_camera("cam1");
// 		if (useIBL)
// 		{
// 			ei_append_filename(output_filename, cur_dir, "scene_unittest_frame_IBL01.bmp");
// 		}
// 		else
		{
			ei_append_filename(output_filename, cur_dir, "scene_unittest_frame01.bmp");
		}
		ei_output(output_filename, "bmp", EI_IMG_DATA_RGB);
			ei_output_variable("color", EI_TYPE_VECTOR);
		ei_end_output();

		if (1)
		{		
			char	output_diffuse_filename[ EI_MAX_FILE_NAME_LEN ];
			char	output_specular_filename[ EI_MAX_FILE_NAME_LEN ];
			ei_append_filename(output_diffuse_filename, cur_dir, "scene_unittest_frame01_diffuse.bmp");
			ei_append_filename(output_specular_filename, cur_dir, "scene_unittest_frame01_specular.bmp");

			ei_output(output_diffuse_filename, "bmp", EI_IMG_DATA_RGB);
			ei_output_variable("aov_diffuse", EI_TYPE_VECTOR);
			ei_end_output();
			ei_output(output_specular_filename, "bmp", EI_IMG_DATA_RGB);
			ei_output_variable("aov_specular", EI_TYPE_VECTOR);
			ei_end_output();
		}

		ei_focal(100.0f);
		ei_aperture(144.724029f);
		ei_aspect(800.0f / 600.0f);
		ei_resolution(800, 600);
		ei_window(0, 600, 0, 400);
		//ei_add_environment("sky_shader");
	ei_end_camera();

	ei_instance("caminst1");
		ei_element("cam1");
	ei_end_instance();

// 	if (useIBL)
// 	{
// 		ei_shader("skylight", "point_light_shader");
// 			ei_shader_param_node("env_shader", "sky_shader");
// 	}
// 	else
	{
		ei_shader("pointlight", "point_light_shader");
			ei_shader_param_scalar("intensity", 5.0f);
			ei_shader_param_vector("lightcolor", 1.0f, 1.0f, 1.0f);
	}
	ei_end_shader();

	ei_light("light1");
		ei_light_shader("point_light_shader");
// 		if (useIBL)
// 		{
// 			// no origin
// 		}
// 		else
		{
			ei_origin(141.375732f, 83.116005f, 35.619434f);
		}
		ei_area_samples(16);
		ei_adaptive(eiFALSE);
	ei_end_light();

	ei_instance("lightinst1");
		ei_element("light1");
	ei_end_instance();

// 	if (useIBL)
// 	{
// 		ei_shader("architectural", "phong_shader");
// 			ei_shader_param_vector("specular", 0.0f, 0.0f, 0.0f);
// 	}
// 	else
	{
		ei_shader("plastic", "phong_shader");
			ei_shader_param_vector("Cs", 0.0f, 1.0f, 0.0f);
			ei_shader_param_vector("Kd", 0.7f, 1.0f, 1.0f);
			ei_shader_param_scalar("Ks", 1.0f);
			ei_shader_param_scalar("roughness", 0.2f);
	}
	ei_end_shader();

	ei_material("mtl");
		ei_surface_shader("phong_shader");
	ei_end_material();

	ei_object("poly", "obj1");
		ei_pos_list(ei_tab(EI_TYPE_VECTOR, 1, 1024));
			ei_tab_add_vector(-7.068787f, -4.155799f, -22.885710f);
			ei_tab_add_vector(-0.179573f, -7.973234f, -16.724060f);
			ei_tab_add_vector(-7.068787f, 4.344949f, -17.619093f);
			ei_tab_add_vector(-0.179573f, 0.527515f, -11.457443f);
			ei_tab_add_vector(0.179573f, -0.527514f, -28.742058f);
			ei_tab_add_vector(7.068787f, -4.344948f, -22.580408f);
			ei_tab_add_vector(0.179573f, 7.973235f, -23.475441f);
			ei_tab_add_vector(7.068787f, 4.155800f, -17.313791f);
		ei_end_tab();
		ei_triangle_list(ei_tab(EI_TYPE_INDEX, 1, 1024));
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
		ei_mtl("mtl");
		ei_element("obj1");
	ei_end_instance();

	ei_instgroup("world");
		ei_add_instance("caminst1");
		ei_add_instance("lightinst1");
		ei_add_instance("inst1");
	ei_end_instgroup();

	// render frame 01
	ei_render("world", "caminst1", "opt");

	//-frame2--------------------------------------------------
// 	ei_camera("cam1");
// 		ei_append_filename(output_filename, cur_dir, "scene_unittest_frame02.bmp");
// 		ei_output(output_filename, "bmp", EI_IMG_DATA_RGB);
// 			ei_output_variable("color", EI_TYPE_VECTOR);
// 		ei_end_output();
// 	ei_end_camera();
// 
// //	if (!useIBL)
// 	{
// // 		ei_shader("pointlight", "point_light_shader");
// // 			ei_shader_param_vector("lightcolor", 1.0f, 0.5f, 1.0f);
// // 		ei_end_shader();
// 		ei_shader("plastic", "phong_shader");
// 			ei_shader_param_vector("Cs", 0.0f, 0.0f, 1.0f);
// 		ei_shader_param_scalar("roughness", 0.2f);
// 	}
// 
// 		// render frame 02
// 		ei_render("world", "caminst1", "opt");

	ei_end_context();
}
