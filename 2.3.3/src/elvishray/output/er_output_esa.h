#include <liqConfig.h>
#ifdef _USE_ELVISHRAY_

#ifndef _ER_OUTPUT_ESA_H_
#define _ER_OUTPUT_ESA_H_
#include "../../common/prerequest_std.h"
#include "er_output_base.h"

namespace elvishray
{	
	class OutputESA : public OutputBase
	{
	public:
		OutputESA(const std::string &filefullpath);
		~OutputESA();

	protected:
		OutputESA();
		OutputESA(const OutputESA &o);
		OutputESA& operator=(const OutputESA &o);

	protected:
		std::ofstream m_outputfile;

		//----------------------------------------------------
		// ER API interfaces
		//----------------------------------------------------
	public:
		virtual void ei_context();
		virtual void ei_end_context();


		//	Client application connection:
		virtual void ei_connection( eiConnection *con );

		//	Network Rendering:
		//	 virtual void ei_hosts( const char *host_name, const int port_number, const char *remote_params, ... );
		//	 virtual void ei_run_server( const int port_number, const int max_num_clients );

		//	Scene Descriptions:
		//virtual void ei_scene();
		//virtual void ei_end_scene();

		//	Shader Definitions:
		// virtual void ei_shader( const char *instance_name, int end=ei_end, ... );
		// virtual void ei_incremental_shader( const char *named_shader, ... );


		//	Shader Linking:
		virtual void ei_link( const char *filename );


		//	Commands:
		virtual void ei_verbose( int level );
		virtual void ei_delete( const char *element_name );
		virtual void ei_render( const char *root_instgroup, const char *camera_inst, const char *options );

		eiTag ei_tab(const eiInt type, const eiInt items_per_slot);
		//virtual void ei_tab_add(const void *value);
		//virtual void ei_tab_add_int(const eiInt value);
		virtual void ei_tab_add_scalar(const eiScalar value);
		virtual void ei_tab_add_vector(const eiScalar x, const eiScalar y, const eiScalar z);
		virtual void ei_tab_add_vector2(const eiScalar x, const eiScalar y);
		//virtual void ei_tab_add_vector4(const eiScalar x, const eiScalar y, const eiScalar z, const eiScalar w);
		//virtual void ei_tab_add_tag(const eiTag value);
		virtual void ei_tab_add_index(const eiIndex value);
		//virtual void ei_tab_add_bool(const eiBool value);
		virtual void ei_end_tab();

		//	Options:
		virtual void ei_options( const char *name );

		//	Sampling Quality:
		virtual void ei_contrast( eiScalar value );
		virtual void ei_samples( int _min, int _max );
		virtual void ei_bucket_size( int size );
		virtual void ei_filter( int filter, float size );

		//	Tessellation Quality:
		// virtual void ei_shading_rate( float rate );
		// virtual void ei_motion_factor( float factor );
		virtual void ei_max_displace( float dist );

		//	Motion Blur:
		virtual void ei_shutter( float open, float close );
		virtual void ei_motion( int type );

		//	Trace Depth:
		virtual void ei_trace_depth( int reflect, int refract, int sum );

		//	Shadow:
		virtual void ei_shadow( int type );

		//	Rendering Algorithms:
		// 	 virtual void ei_trace( int type );
		// 	 virtual void ei_scanline( int type );
		// 	 virtual void ei_hardware( int type );
		//	 virtual void ei_acceleration( int type );
		//virtual void ei_simd( int type );
		//	 virtual void ei_bsp_size( int size );
		//	 virtual void ei_bsp_depth( int depth );
		//virtual void ei_bsp_memory( int size );

		//	Feature Disabling:
		virtual void ei_lens( int type );
		virtual void ei_volume( int type );
		virtual void ei_geometry( int type );
		virtual void ei_displace( int type );
		virtual void ei_imager( int type );

		//	Caustics:
		//	 virtual void ei_caustic( int type );
		//	 virtual void ei_caustic_accuracy( int samples, float radius );

		//	Global Illumination:
		//	 virtual void ei_globillum( int type );
		//	 virtual void ei_globillum_accuracy( int samples, float radius );
		//	 virtual void ei_photonvol_accuracy( int samples, float radius );

		//	Frame Buffer Control:
		virtual void ei_exposure( float gain, float gamma );
		virtual void ei_quantize( float one, float _min, float _max, float dither_amplitude );
		
		//	Miscellaneous:
		virtual void ei_face( int type );

		virtual void ei_approx(const eiApprox *approx);

		virtual void ei_end_options();


		//	Cameras:
		virtual void ei_camera( const char *name );
		// virtual void ei_incremental_camera( const char *name );

		//	Output Statements:
		virtual void ei_output( const char *filename, const char *fileformat, const eiInt datatype);
		virtual void ei_output_variable(const char *name, const eiInt datatype);
		virtual void ei_end_output();
		//virtual void ei_imager( const char *shader_name, int end_=ei_end, ... );

		//	Other Camera Statements:
		virtual void ei_focal( float distance );
		virtual void ei_aperture( float aperture );
		virtual void ei_aspect( float aspect );
		virtual void ei_resolution( eiInt x, eiInt y );
		//virtual void ei_offset( int x, int y );
		virtual void ei_window( eiInt xmin, eiInt xmax, eiInt ymin, eiInt ymax );
		virtual void ei_clip( float hither, float yon );
		virtual void ei_lens_shader(const char *shader_name);
		virtual void ei_imager_shader(const char *shader_name);

		//virtual void ei_atmosphere( const char *shader_name, int end_=ei_end,... );
		//virtual void ei_ambience( const char *shader_name, int end_=ei_end,... );

		virtual void ei_end_camera();


		//	Textures:
		virtual void ei_make_texture( const char *picturename, const char *texturename, 
			int swrap, int twrap, int filter, float swidth, float twidth );


		//	Materials:
		virtual void ei_material( const char *name );

		virtual void ei_surface_shader( const char *shader_name );
		virtual void ei_displace_shader( const char *shader_name );
		virtual void ei_shadow_shader( const char *shader_name );
		virtual void ei_volume_shader( const char *shader_name );
		virtual void ei_environment_shader( const char *shader_name );

		virtual void ei_end_material();


		//	Lights:
		virtual void ei_light( const char *name );
		// virtual void ei_incremental_light( const char *name );

		virtual void ei_light_shader( const char *shader_name );
		virtual void ei_origin(eiScalar x, eiScalar y, eiScalar z);


		//	Light Source Types:
		virtual void ei_area_samples( eiInt samples );

		virtual void ei_end_light();


		//	Objects:
		virtual void ei_object( const char *type, const char *name );
		virtual void ei_box(eiScalar xmin, eiScalar ymin, eiScalar zmin, 
			eiScalar xmax, eiScalar ymax, eiScalar zmax);
		virtual void ei_motion_box(eiScalar xmin, eiScalar ymin, eiScalar zmin, 
			eiScalar xmax, eiScalar ymax, eiScalar zmax);
		/* procedural objects */
		virtual void ei_geometry_shader(const char *shader_name);
		/* polygon objects */
		virtual void ei_pos_list(const eiTag tab);
		virtual void ei_motion_pos_list(const eiTag tab);
		virtual void ei_triangle_list(const eiTag tab);


		//virtual void ei_approximate( int param, int end_=ei_end, ... );

		virtual void ei_end_object();


		//	Instances:
		virtual void ei_instance( const char *name );
		// virtual void ei_incremental_instance( const char *name );

		virtual void ei_element( const char *element_name );


		virtual void ei_transform( 
			float t00,float t01,float t02,float t03,
			float t10,float t11,float t12,float t13,
			float t20,float t21,float t22,float t23,
			float t30,float t31,float t32,float t33 
			);
		virtual void ei_motion_transform( 
			float t00,float t01,float t02,float t03,
			float t10,float t11,float t12,float t13,
			float t20,float t21,float t22,float t23,
			float t30,float t31,float t32,float t33
			);

		virtual void ei_mtl( const char *material_name );

		virtual void ei_end_instance();


		//	Instance Groups:
		virtual void ei_instgroup( const char *name );

		virtual void ei_add_instance( const char *name );

		virtual void ei_end_instgroup();
		//
		virtual void ei_shader(const char *type_name, const char *instance_name);
		virtual void ei_shader_param(
			const char *param_name, 
			const void *param_value);
		// virtual void ei_shader_param_string(
		// 								  const char *param_name, 
		// 								  const char *param_value);
		virtual void ei_shader_param_int(
			const char *param_name, 
			const eiInt param_value);
		virtual void ei_shader_param_scalar(
			const char *param_name, 
			const eiScalar param_value);
		virtual void ei_shader_param_vector(
			const char *param_name, 
			const eiScalar x, const eiScalar y, const eiScalar z);
		virtual void ei_shader_param_vector4(
			const char *param_name, 
			const eiScalar x, const eiScalar y, const eiScalar z, const eiScalar w);
		virtual void ei_shader_param_tag(
			const char *param_name, 
			const eiTag param_value);
		virtual void ei_shader_param_texture(
			const char *param_name, 
			const char *texture_name);
		virtual void ei_shader_param_index(
			const char *param_name, 
			const eiIndex param_value);
		virtual void ei_shader_param_bool(
			const char *param_name, 
			const eiBool param_value);
		virtual void ei_shader_link_param(
			const char *param_name, 
			const char *src_shader_name, 
			const char *src_param_name);

		virtual void ei_end_shader();

		virtual void ei_declare(const char *name, const eiInt storage_class, const eiInt type, const void *default_value);
		virtual void ei_variable(const char *name, const void *value);
		virtual void ei_degree(const eiInt degree);



	};
}

#endif//_ER_OUTPUT_ESA_H_
#endif//_USE_ELVISHRAY_