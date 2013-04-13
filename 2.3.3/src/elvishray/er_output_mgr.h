#include <liqConfig.h>
#ifdef _USE_ELVISHRAY_

#ifndef _ER_OUTPUT_MGR_H_
#define _ER_OUTPUT_MGR_H_
#include "output/er_output_base.h"

namespace elvishray
{	
	
	//
	class OutputMgr
	{
	public:
		enum OutputType
		{
			OT_CALL = 0,
			OT_ESS  = 1,
			OT_ESA  = 2,
			OT_COUNT
		};

	public:
		OutputMgr();
		~OutputMgr();
		
		void setOutputImagePath(const std::string &output_image_path);
		int add(OutputType outputtype);

		int init();
		int uninit();

	protected:
		OutputMgr(const OutputMgr& o);
		OutputMgr& operator=(const OutputMgr& o);

		OutputBase* createOutput(OutputType outputtype);
	protected:
		std::vector<OutputType> m_receiver_types;
		std::vector<elvishray::OutputBase*> m_receivers;
		std::string m_output_image_path;



		//----------------------------------------------------
		// ER API interfaces
		//----------------------------------------------------
	public:
		void annotation(const std::string &msg);

		void ei_context();
		void ei_end_context();

		//	Client application connection:
		void ei_connection( eiConnection *con );

		//	Network Rendering:
		//	 void ei_hosts( const char *host_name, const int port_number, const char *remote_params, ... );
		//	 void ei_run_server( const int port_number, const int max_num_clients );

		//	Scene Descriptions:
		//void ei_scene();
		//void ei_end_scene();

		//	Shader Definitions:
		// void ei_shader( const char *instance_name, int end=ei_end, ... );
		// void ei_incremental_shader( const char *named_shader, ... );


		//	Shader Linking:
		void ei_link( const char *filename );


		//	Commands:
		void ei_verbose( int level );
		void ei_delete( const char *element_name );
		void ei_render( const char *root_instgroup, const char *camera_inst, const char *options );

		eiTag ei_tab(const eiInt type, const eiInt items_per_slot);
		//void ei_tab_add(const void *value);
		//void ei_tab_add_int(const eiInt value);
		void ei_tab_add_scalar(const eiScalar value);
		void ei_tab_add_vector(const eiScalar x, const eiScalar y, const eiScalar z);
		void ei_tab_add_vector2(const eiScalar x, const eiScalar y);
		//void ei_tab_add_vector4(const eiScalar x, const eiScalar y, const eiScalar z, const eiScalar w);
		//void ei_tab_add_tag(const eiTag value);
		void ei_tab_add_index(const eiIndex value);
		//void ei_tab_add_bool(const eiBool value);
		void ei_end_tab();

		//	Options:
		void ei_options( const char *name );

		//	Sampling Quality:
		void ei_contrast( eiScalar value );
		void ei_samples( int _min, int _max );
		void ei_bucket_size( int size );
		void ei_filter( int filter, float size );

		//	Tessellation Quality:
		// void ei_shading_rate( float rate );
		// void ei_motion_factor( float factor );
		void ei_max_displace( float dist );

		//	Motion Blur:
		void ei_shutter( float open, float close );
		void ei_motion( int type );

		//	Trace Depth:
		void ei_trace_depth( int reflect, int refract, int sum );

		//	Shadow:
		void ei_shadow( int type );

		//	Rendering Algorithms:
		// 	 void ei_trace( int type );
		// 	 void ei_scanline( int type );
		// 	 void ei_hardware( int type );
		//	 void ei_acceleration( int type );
		//void ei_simd( int type );
		//	 void ei_bsp_size( int size );
		//	 void ei_bsp_depth( int depth );
		//void ei_bsp_memory( int size );

		//	Feature Disabling:
		void ei_lens( int type );
		void ei_volume( int type );
		void ei_geometry( int type );
		void ei_displace( int type );
		void ei_imager( int type );

		//	Caustics:
		//	 void ei_caustic( int type );
		//	 void ei_caustic_accuracy( int samples, float radius );

		//	Global Illumination:
		//	 void ei_globillum( int type );
		//	 void ei_globillum_accuracy( int samples, float radius );
		//	 void ei_photonvol_accuracy( int samples, float radius );

		//	Frame Buffer Control:
		void ei_exposure( float gain, float gamma );
		void ei_quantize( float one, float _min, float _max, float dither_amplitude );
		
		//	Miscellaneous:
		void ei_face( int type );

		void ei_approx(const eiApprox *approx);

		void ei_end_options();


		//	Cameras:
		void ei_camera( const char *name );
		// void ei_incremental_camera( const char *name );

		//	Output Statements:
		void ei_output( const char *filename, const char *fileformat, const eiInt datatype);
		void ei_output_variable(const char *name, const eiInt datatype);
		void ei_end_output();
		//void ei_imager( const char *shader_name, int end_=ei_end, ... );

		//	Other Camera Statements:
		void ei_focal( float distance );
		void ei_aperture( float aperture );
		void ei_aspect( float aspect );
		void ei_resolution( eiInt x, eiInt y );
		//void ei_offset( int x, int y );
		void ei_window( eiInt xmin, eiInt xmax, eiInt ymin, eiInt ymax );
		void ei_clip( float hither, float yon );
		void ei_lens_shader(const char *shader_name);
		void ei_imager_shader(const char *shader_name);

		//void ei_atmosphere( const char *shader_name, int end_=ei_end,... );
		//void ei_ambience( const char *shader_name, int end_=ei_end,... );

		void ei_end_camera();


		//	Textures:
		void ei_make_texture( const char *picturename, const char *texturename, 
			int swrap, int twrap, int filter, float swidth, float twidth );


		//	Materials:
		void ei_material( const char *name );

		void ei_surface_shader( const char *shader_name );
		void ei_displace_shader( const char *shader_name );
		void ei_shadow_shader( const char *shader_name );
		void ei_volume_shader( const char *shader_name );
		void ei_environment_shader( const char *shader_name );

		void ei_end_material();


		//	Lights:
		void ei_light( const char *name );
		// void ei_incremental_light( const char *name );

		void ei_light_shader( const char *shader_name );
		void ei_origin(eiScalar x, eiScalar y, eiScalar z);


		//	Light Source Types:
		void ei_area_samples( eiInt samples );

		void ei_end_light();


		//	Objects:
		void ei_object( const char *type, const char *name );
		void ei_box(eiScalar xmin, eiScalar ymin, eiScalar zmin, 
			eiScalar xmax, eiScalar ymax, eiScalar zmax);
		void ei_motion_box(eiScalar xmin, eiScalar ymin, eiScalar zmin, 
			eiScalar xmax, eiScalar ymax, eiScalar zmax);
		/* procedural objects */
		void ei_geometry_shader(const char *shader_name);
		/* polygon objects */
		void ei_pos_list(const eiTag tab);
		void ei_motion_pos_list(const eiTag tab);
		void ei_triangle_list(const eiTag tab);



		void ei_end_object();


		//	Instances:
		void ei_instance( const char *name );
		// void ei_incremental_instance( const char *name );

		void ei_element( const char *element_name );

		void ei_transform( 
			float t00,float t01,float t02,float t03,
			float t10,float t11,float t12,float t13,
			float t20,float t21,float t22,float t23,
			float t30,float t31,float t32,float t33 
			);
		void ei_motion_transform( 
			float t00,float t01,float t02,float t03,
			float t10,float t11,float t12,float t13,
			float t20,float t21,float t22,float t23,
			float t30,float t31,float t32,float t33
			);

		void ei_mtl( const char *material_name );

		void ei_end_instance();


		//	Instance Groups:
		void ei_instgroup( const char *name );

		void ei_add_instance( const char *name );

		void ei_end_instgroup();
		//
		void ei_shader(const char *type_name, const char *instance_name);
		void ei_shader_param(
			const char *param_name, 
			const void *param_value);
		// void ei_shader_param_string(
		// 								  const char *param_name, 
		// 								  const char *param_value);
		void ei_shader_param_int(
			const char *param_name, 
			const eiInt param_value);
		void ei_shader_param_scalar(
			const char *param_name, 
			const eiScalar param_value);
		void ei_shader_param_vector(
			const char *param_name, 
			const eiScalar x, const eiScalar y, const eiScalar z);
		void ei_shader_param_vector4(
			const char *param_name, 
			const eiScalar x, const eiScalar y, const eiScalar z, const eiScalar w);
		void ei_shader_param_tag(
			const char *param_name, 
			const eiTag param_value);
		void ei_shader_param_texture(
			const char *param_name, 
			const char *texture_name);
		void ei_shader_param_index(
			const char *param_name, 
			const eiIndex param_value);
		void ei_shader_param_bool(
			const char *param_name, 
			const eiBool param_value);
		void ei_shader_link_param(
			const char *param_name, 
			const char *src_shader_name, 
			const char *src_param_name);

		void ei_end_shader();

		void ei_declare(const char *name, const eiInt storage_class, const eiInt type, const void *default_value);
		void ei_variable(const char *name, const void *value);
		void ei_degree(const eiInt degree);

	};
	//
}

#endif//_ER_OUTPUT_MGR_H_
#endif//_USE_ELVISHRAY_