#include <liqConfig.h>
#ifdef _USE_ELVISHRAY_

#ifndef _ER_OUTPUT_BASE_H_
#define _ER_OUTPUT_BASE_H_
#include "../../common/prerequest_std.h"
#include <eiAPI/ei.h>

namespace elvishray
{	
	class OutputBase
	{
	public:
		OutputBase();
		virtual ~OutputBase();

	protected:
		OutputBase(const OutputBase &o);
		OutputBase& operator=(const OutputBase &o);


		//----------------------------------------------------
		// ER API interfaces
		//----------------------------------------------------
	public:
		virtual void annotation(const std::string &msg) = 0;

		virtual void ei_context() = 0;
		virtual void ei_end_context() = 0;

		//	Client application connection:
		virtual void ei_connection( eiConnection *con ) = 0;

		//	Shader Linking:
		virtual void ei_link( const char *filename ) = 0;


		//	Commands:
		virtual void ei_verbose( int level ) = 0;
		virtual void ei_delete( const char *element_name ) = 0;
		virtual void ei_render( const char *root_instgroup, const char *camera_inst, const char *options ) = 0;

		virtual eiTag ei_tab(const eiInt type, const eiInt items_per_slot) = 0;
		//virtual void ei_tab_add(const void *value) = 0;
		//virtual void ei_tab_add_int(const eiInt value) = 0;
		virtual void ei_tab_add_scalar(const eiScalar value) = 0;
		virtual void ei_tab_add_vector(const eiScalar x, const eiScalar y, const eiScalar z) = 0;
		virtual void ei_tab_add_vector2(const eiScalar x, const eiScalar y) = 0;
		//virtual void ei_tab_add_vector4(const eiScalar x, const eiScalar y, const eiScalar z, const eiScalar w) = 0;
		//virtual void ei_tab_add_tag(const eiTag value) = 0;
		virtual void ei_tab_add_index(const eiIndex value) = 0;
		//virtual void ei_tab_add_bool(const eiBool value) = 0;
		virtual void ei_end_tab() = 0;

		//	Options:
		virtual void ei_options( const char *name ) = 0;

		//	Sampling Quality:
		virtual void ei_contrast( eiScalar value ) = 0;
		virtual void ei_samples( int _min, int _max ) = 0;
		virtual void ei_bucket_size( int size ) = 0;
		virtual void ei_filter( int filter, float size ) = 0;

		//	Tessellation Quality:
		// virtual void ei_shading_rate( float rate ) = 0;
		// virtual void ei_motion_factor( float factor ) = 0;
		virtual void ei_max_displace( float dist ) = 0;

		//	Motion Blur:
		virtual void ei_shutter( float open, float close ) = 0;
		virtual void ei_motion( int type ) = 0;

		//	Trace Depth:
		virtual void ei_trace_depth( int reflect, int refract, int sum ) = 0;

		//	Shadow:
		virtual void ei_shadow( int type ) = 0;

		//	Feature Disabling:
		virtual void ei_lens( int type ) = 0;
		virtual void ei_volume( int type ) = 0;
		virtual void ei_geometry( int type ) = 0;
		virtual void ei_displace( int type ) = 0;
		virtual void ei_imager( int type ) = 0;

		//	Frame Buffer Control:
		virtual void ei_exposure( float gain, float gamma ) = 0;
		virtual void ei_quantize( float one, float _min, float _max, float dither_amplitude ) = 0;
		
		//	Miscellaneous:
		virtual void ei_face( int type ) = 0;

		virtual void ei_approx(const eiApprox *approx) = 0;

		virtual void ei_end_options() = 0;


		//	Cameras:
		virtual void ei_camera( const char *name ) = 0;
		// virtual void ei_incremental_camera( const char *name ) = 0;

		//	Output Statements:
		virtual void ei_output( const char *filename, const char *fileformat, const eiInt datatype) = 0;
		virtual void ei_output_variable(const char *name, const eiInt datatype) = 0;
		virtual void ei_end_output() = 0;
		//virtual void ei_imager( const char *shader_name, int end_=ei_end, ... ) = 0;

		//	Other Camera Statements:
		virtual void ei_focal( float distance ) = 0;
		virtual void ei_aperture( float aperture ) = 0;
		virtual void ei_aspect( float aspect ) = 0;
		virtual void ei_resolution( eiInt x, eiInt y ) = 0;
		//virtual void ei_offset( int x, int y ) = 0;
		virtual void ei_window( eiInt xmin, eiInt xmax, eiInt ymin, eiInt ymax ) = 0;
		virtual void ei_clip( float hither, float yon ) = 0;
		virtual void ei_lens_shader(const char *shader_name) = 0;
		virtual void ei_imager_shader(const char *shader_name) = 0;

		//virtual void ei_atmosphere( const char *shader_name, int end_=ei_end,... ) = 0;
		//virtual void ei_ambience( const char *shader_name, int end_=ei_end,... ) = 0;

		virtual void ei_end_camera() = 0;


		//	Textures:
		virtual void ei_make_texture( const char *picturename, const char *texturename, 
			int swrap, int twrap, int filter, float swidth, float twidth ) = 0;


		//	Materials:
		virtual void ei_material( const char *name ) = 0;

		virtual void ei_surface_shader( const char *shader_name ) = 0;
		virtual void ei_displace_shader( const char *shader_name ) = 0;
		virtual void ei_shadow_shader( const char *shader_name ) = 0;
		virtual void ei_volume_shader( const char *shader_name ) = 0;
		virtual void ei_environment_shader( const char *shader_name ) = 0;

		virtual void ei_end_material() = 0;


		//	Lights:
		virtual void ei_light( const char *name ) = 0;
		// virtual void ei_incremental_light( const char *name ) = 0;

		virtual void ei_light_shader( const char *shader_name ) = 0;
		virtual void ei_origin(eiScalar x, eiScalar y, eiScalar z) = 0;


		//	Light Source Types:
		virtual void ei_area_samples( eiInt samples ) = 0;

		virtual void ei_end_light() = 0;


		//	Objects:
		virtual void ei_object( const char *type, const char *name ) = 0;
		virtual void ei_box(eiScalar xmin, eiScalar ymin, eiScalar zmin, 
			eiScalar xmax, eiScalar ymax, eiScalar zmax) = 0;
		virtual void ei_motion_box(eiScalar xmin, eiScalar ymin, eiScalar zmin, 
			eiScalar xmax, eiScalar ymax, eiScalar zmax) = 0;
		/* procedural objects */
		virtual void ei_geometry_shader(const char *shader_name) = 0;
		/* polygon objects */
		virtual void ei_pos_list(const eiTag tab) = 0;
		virtual void ei_motion_pos_list(const eiTag tab) = 0;
		virtual void ei_triangle_list(const eiTag tab) = 0;



		virtual void ei_end_object() = 0;


		//	Instances:
		virtual void ei_instance( const char *name ) = 0;
		// virtual void ei_incremental_instance( const char *name ) = 0;

		virtual void ei_element( const char *element_name ) = 0;


		virtual void ei_transform( 
			float t00,float t01,float t02,float t03,
			float t10,float t11,float t12,float t13,
			float t20,float t21,float t22,float t23,
			float t30,float t31,float t32,float t33 
			) = 0;
		virtual void ei_motion_transform( 
			float t00,float t01,float t02,float t03,
			float t10,float t11,float t12,float t13,
			float t20,float t21,float t22,float t23,
			float t30,float t31,float t32,float t33
			) = 0;

		virtual void ei_mtl( const char *material_name ) = 0;

		virtual void ei_end_instance() = 0;


		//	Instance Groups:
		virtual void ei_instgroup( const char *name ) = 0;

		virtual void ei_add_instance( const char *name ) = 0;

		virtual void ei_end_instgroup() = 0;
		//
		virtual void ei_shader(const char *type_name, const char *instance_name) = 0;
		virtual void ei_shader_param(
			const char *param_name, 
			const void *param_value) = 0;
		// virtual void ei_shader_param_string(
		// 								  const char *param_name, 
		// 								  const char *param_value) = 0;
		virtual void ei_shader_param_int(
			const char *param_name, 
			const eiInt param_value) = 0;
		virtual void ei_shader_param_scalar(
			const char *param_name, 
			const eiScalar param_value) = 0;
		virtual void ei_shader_param_vector(
			const char *param_name, 
			const eiScalar x, const eiScalar y, const eiScalar z) = 0;
		virtual void ei_shader_param_vector4(
			const char *param_name, 
			const eiScalar x, const eiScalar y, const eiScalar z, const eiScalar w) = 0;
		virtual void ei_shader_param_tag(
			const char *param_name, 
			const eiTag param_value) = 0;
		virtual void ei_shader_param_texture(
			const char *param_name, 
			const char *texture_name) = 0;
		virtual void ei_shader_param_index(
			const char *param_name, 
			const eiIndex param_value) = 0;
		virtual void ei_shader_param_bool(
			const char *param_name, 
			const eiBool param_value) = 0;
		virtual void ei_shader_link_param(
			const char *param_name, 
			const char *src_shader_name, 
			const char *src_param_name) = 0;

		virtual void ei_end_shader() = 0;

		virtual void ei_declare(const char *name, const eiInt storage_class, const eiInt type, const void *default_value) = 0;
		virtual void ei_variable(const char *name, const void *value) = 0;
		virtual void ei_degree(const eiInt degree) = 0;

	};
}

#endif//_ER_OUTPUT_BASE_H_
#endif//_USE_ELVISHRAY_