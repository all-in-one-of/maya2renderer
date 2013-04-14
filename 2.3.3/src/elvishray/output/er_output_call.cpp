#include <liqConfig.h>
#ifdef _USE_ELVISHRAY_
#include "er_output_call.h"
#include <trace/trace.hpp>
#include <liqlog.h>

namespace elvishray
{
	OutputCall::OutputCall()
		:OutputBase()
	{
		CM_TRACE_FUNC("OutputCall::OutputCall()");

	}
	OutputCall::~OutputCall()
	{
		CM_TRACE_FUNC("OutputCall::~OutputCall()");
	}
	void OutputCall::init()
	{
		CM_TRACE_FUNC("OutputCall::init()");
		null_token = NULL;
		defaultInt = 0;
	}
	//----------------------------------------------------
	// ER API interfaces
	//----------------------------------------------------
	void OutputCall::ln()
	{
		//CM_TRACE_FUNC("OutputCall::ln()");
	}
	void OutputCall::s(const std::string &msg)
	{
		//CM_TRACE_FUNC("OutputCall::s("<<msg<<")");
	}
	void OutputCall::a(const std::string &msg)
	{
		//CM_TRACE_FUNC("OutputCall::a("<<msg<<")");
	}
	void OutputCall::ei_context()
	{
		//CM_TRACE_FUNC("OutputCall::ei_context()");
		::ei_context();
	}
	void OutputCall::ei_end_context()
	{
		//CM_TRACE_FUNC("OutputCall::ei_end_context()");
		::ei_end_context();
	}
	//	Client application connection:
	void OutputCall::ei_connection( eiConnection *con )
	{
		//CM_TRACE_FUNC("OutputCall::ei_connection(...)");
		::ei_connection(con);
	}
	//	Shader Linking:
	void OutputCall::ei_link( const char *filename )
	{
		//CM_TRACE_FUNC("OutputCall::ei_link("<<filename<<")");
		::ei_link(filename);
	}
	//	Commands:
	void OutputCall::ei_verbose( int level )
	{
		//CM_TRACE_FUNC("OutputCall::ei_verbose("<<level<<")");
		::ei_verbose(level);
	}
	void OutputCall::ei_delete( const char *element_name )
	{
		//CM_TRACE_FUNC("OutputCall::ei_delete( \""<<element_name<<"\" )");
		::ei_delete(element_name);
	}
	void OutputCall::ei_render( const char *root_instgroup, const char *camera_inst, const char *options )
	{
		//CM_TRACE_FUNC("OutputCall::ei_render(\""<<root_instgroup<<"\", \""<<camera_inst<<"\", \""<<options<<"\")");
		::ei_render( root_instgroup, camera_inst, options );
	}

	eiTag OutputCall::ei_tab(const eiInt type, const eiInt items_per_slot)
	{
		//CM_TRACE_FUNC("OutputCall::ei_tab("<<type<<","<<items_per_slot<<")");
		return ::ei_tab( type, items_per_slot );
	}
	//void OutputCall::ei_tab_add(const void *value);
	//void OutputCall::ei_tab_add_int(const eiInt value);
	void OutputCall::ei_tab_add_scalar(const eiScalar value)
	{
		//CM_TRACE_FUNC("OutputCall::ei_tab_add_scalar("<<value<<")");
		::ei_tab_add_scalar( value );
	}
	void OutputCall::ei_tab_add_vector(const eiScalar x, const eiScalar y, const eiScalar z)
	{
		//CM_TRACE_FUNC("OutputCall::ei_tab_add_vector("<<x<<","<<y<<","<<z<<")");
		::ei_tab_add_vector( x, y, z );
	}
	void OutputCall::ei_tab_add_vector2(const eiScalar x, const eiScalar y)
	{
		//CM_TRACE_FUNC("OutputCall::ei_tab_add_vector2("<<x<<","<<y<<")");
		::ei_tab_add_vector2( x, y );
	}
	void OutputCall::ei_tab_add_index(const eiIndex value)
	{
		//CM_TRACE_FUNC("OutputCall::ei_tab_add_index("<<value<<")");
		::ei_tab_add_index( value );
	}
	//void OutputCall::ei_tab_add_bool(const eiBool value);
	void OutputCall::ei_end_tab()
	{
		//CM_TRACE_FUNC("OutputCall::ei_end_tab()");
		::ei_end_tab();
	}


	//	Options:
	void OutputCall::ei_options( const char *name )
	{
		//CM_TRACE_FUNC("OutputCall::ei_options(\""<<name<<"\")");
		::ei_options( name );
	}

	//	Sampling Quality:
	void OutputCall::ei_contrast( eiScalar value )
	{
		//CM_TRACE_FUNC("OutputCall::ei_contrast("<<value<<")");
		::ei_contrast(value);
	}
	void OutputCall::ei_samples( int _min, int _max )
	{
		//CM_TRACE_FUNC("OutputCall::ei_samples("<<_min<<","<<_max<<")");
		::ei_samples(_min,_max);
	}
	void OutputCall::ei_bucket_size( int size )
	{
		//CM_TRACE_FUNC("OutputCall::ei_bucket_size("<<size<<")");
		::ei_bucket_size(size);
	}
	void OutputCall::ei_filter( int filter, float size )
	{
		//CM_TRACE_FUNC("OutputCall::ei_filter("<<filter<<","<<size<<")");
		::ei_filter(filter,size);
	}
	//	Motion Blur:
	void OutputCall::ei_shutter( float open, float close )
	{
		//CM_TRACE_FUNC("OutputCall::ei_shutter("<<open<<", "<<close<<")");
		::ei_shutter(open, close);
	}
	void OutputCall::ei_motion( int type )
	{
		//CM_TRACE_FUNC("OutputCall::ei_motion("<< type <<")");
		::ei_motion(type);
	}

	//	Trace Depth:
	void OutputCall::ei_trace_depth( int reflect, int refract, int sum )
	{
		//CM_TRACE_FUNC("OutputCall::ei_trace_depth(4,4,4,"<<reflect<<","<<refract<<","<<sum<<")");
		liquidMessage2(messageError, "OutputCall::ei_trace_depth() need feed the parameters");
		::ei_trace_depth(4,4,4,reflect,refract,sum);
	}
	//	Shadow:
	void OutputCall::ei_shadow( int type )
	{
		//CM_TRACE_FUNC("OutputCall::ei_shadow("<<type<<")");
		::ei_shadow(type);
	}
	//	Feature Disabling:
	void OutputCall::ei_lens(eiInt type)
	{
		//CM_TRACE_FUNC("OutputCall::ei_lens("<<type<<" )");
		::ei_lens(type );
	}
	void OutputCall::ei_volume( int type )
	{
		//CM_TRACE_FUNC("OutputCall::ei_volume("<<type<<")");
		::ei_volume(type);
	}
	void OutputCall::ei_geometry( int type )
	{
		//CM_TRACE_FUNC("OutputCall::ei_geometry("<<type<<")");
		::ei_geometry(type);
	}
	void OutputCall::ei_displace( int type )
	{
		//CM_TRACE_FUNC("OutputCall::ei_displace("<<type<<")");
		::ei_displace(type);
	}
	void OutputCall::ei_max_displace(eiScalar dist)
	{
		//CM_TRACE_FUNC("OutputCall::ei_max_displace("<<dist<<")");
		::ei_max_displace(dist);
	}
	void OutputCall::ei_imager( int type )
	{
		//CM_TRACE_FUNC("OutputCall::ei_imager("<<type<<")");
		::ei_imager(type);
	}
	//	Frame Buffer Control:
	void OutputCall::ei_exposure( float gain, float gamma )
	{
		//CM_TRACE_FUNC("OutputCall::ei_exposure("<<gain<<","<<gamma<<")");
		::ei_exposure(gain, gamma);
	}
	void OutputCall::ei_quantize( float one, float _min, float _max, float dither_amplitude )
	{
		//CM_TRACE_FUNC("OutputCall::ei_quantize("<<one<<","<<_min<<","<<_max<<","<<dither_amplitude<<")");
		::ei_quantize(one,  _min, _max, dither_amplitude);
	}
	//	Miscellaneous:
	void OutputCall::ei_face( int type )
	{
		//CM_TRACE_FUNC("OutputCall::ei_face("<< type <<")");
		::ei_face(type);
	}
	void OutputCall::ei_approx(const eiApprox *approx)
	{
		//CM_TRACE_FUNC("OutputCall::ei_approx(approx)");
		::ei_approx(approx);
	}
	void OutputCall::ei_end_options()
	{
		//CM_TRACE_FUNC("OutputCall::ei_end_options()");
		::ei_end_options();
	}
	//	Cameras:
	void OutputCall::ei_camera( const char *name )
	{
		//CM_TRACE_FUNC("OutputCall::ei_camera(\""<< name <<"\")");
		::ei_camera(name);
	}
	//	Output Statements:
	void OutputCall::ei_output( const char *filename, const char *fileformat, const eiInt datatype)
	{
		//CM_TRACE_FUNC("OutputCall::ei_output(\""<< filename <<"\",\""<<fileformat<<"\","<<datatype<<")");
		::ei_output(filename, fileformat, datatype);
	}
	void OutputCall::ei_output_variable(const char *name, const eiInt datatype)
	{
		//CM_TRACE_FUNC("OutputCall::ei_output_variable(\""<< name <<"\","<<datatype<<")");
		::ei_output_variable(name, datatype);
	}
	void OutputCall::ei_end_output()
	{
		//CM_TRACE_FUNC("OutputCall::ei_end_output()");
		::ei_end_output();
	}
	//	Other Camera Statements:
	void OutputCall::ei_focal( float distance )
	{
		//CM_TRACE_FUNC("OutputCall::ei_focal("<< distance <<")");
		::ei_focal(distance);
	}
	void OutputCall::ei_aperture( float aperture )
	{
		//CM_TRACE_FUNC("OutputCall::ei_aperture("<< aperture <<")");
		::ei_aperture(aperture);
	}
	void OutputCall::ei_aspect( float aspect )
	{
		//CM_TRACE_FUNC("OutputCall::ei_aspect("<< aspect <<")");
		::ei_aspect(aspect);
	}
	void OutputCall::ei_resolution( eiInt x, eiInt y )
	{
		//CM_TRACE_FUNC("OutputCall::ei_resolution("<< x<<","<<y <<")");
		::ei_resolution(x, y);
	}
	void OutputCall::ei_window( eiInt xmin, eiInt xmax, eiInt ymin, eiInt ymax )
	{
		//CM_TRACE_FUNC("OutputCall::ei_window("<< xmin<<","<<xmax<<","<<ymin<<","<<ymax<<")");
		::ei_window(xmin, xmax, ymin, ymax );
	}
	void OutputCall::ei_clip( float hither, float yon )
	{
		//CM_TRACE_FUNC("OutputCall::ei_clip("<<hither<<","<<yon<<")");
		::ei_clip(hither,yon);
	}
	void OutputCall::ei_lens_shader(const char *shader_name)
	{
		//CM_TRACE_FUNC("OutputCall::ei_lens_shader(\""<<shader_name<<"\")");
		::ei_lens_shader(shader_name);
	}
	void OutputCall::ei_imager_shader(const char *shader_name)
	{
		//CM_TRACE_FUNC("OutputCall::ei_imager_shader(\""<<shader_name<<"\")");
		::ei_imager_shader(shader_name);
	}
	void OutputCall::ei_end_camera()
	{
		//CM_TRACE_FUNC("OutputCall::ei_end_camera()");
		::ei_end_camera();
	}
	//	Textures:
	void OutputCall::ei_make_texture( const char *picturename, const char *texturename, 
		int swrap, int twrap, int filter, float swidth, float twidth )
	{
		//CM_TRACE_FUNC("OutputCall::ei_make_texture(\""<<picturename<<"\", \""<< texturename<<"\","<<swrap<<","<< twrap<<","<<filter<<", "<<swidth<<", "<<twidth<<")");
		::ei_make_texture(picturename,  texturename, swrap, twrap, filter, swidth, twidth);
	}
	void OutputCall::ei_texture(const char *name)
	{
		//CM_TRACE_FUNC("OutputCall::ei_texture("<<name<<")");
		::ei_texture(name);
	}
	void OutputCall::ei_file_texture(const char *filename, const eiBool local)
	{
		//CM_TRACE_FUNC("OutputCall::ei_file_texture("<<filename<<","<<local<<")");
		::ei_file_texture(filename, local);
	}
	void OutputCall::ei_end_texture()
	{
		//CM_TRACE_FUNC("OutputCall::ei_end_texture()");
		::ei_end_texture();
	}
	//	Materials:
	void OutputCall::ei_material( const char *name )
	{
		//CM_TRACE_FUNC("OutputCall::ei_material(\""<<name<<"\")");
		::ei_material(name);
	}
	void OutputCall::ei_surface_shader( const char *shader_name )
	{
		//CM_TRACE_FUNC("OutputCall::ei_surface_shader(\""<<shader_name <<"\")");
		::ei_surface_shader(shader_name );
	}
	void OutputCall::ei_displace_shader( const char *shader_name )
	{
		//CM_TRACE_FUNC("OutputCall::ei_displace_shader(\""<<shader_name <<"\")");
		::ei_displace_shader(shader_name );
	}
	void OutputCall::ei_shadow_shader( const char *shader_name )
	{
		//CM_TRACE_FUNC("OutputCall::ei_shadow_shader(\""<<shader_name <<"\")");
		::ei_shadow_shader(shader_name );
	}
	void OutputCall::ei_volume_shader( const char *shader_name )
	{
		//CM_TRACE_FUNC("OutputCall::ei_volume_shader(\""<<shader_name <<"\")");
		::ei_volume_shader(shader_name );
	}
	void OutputCall::ei_environment_shader( const char *shader_name )
	{
		//CM_TRACE_FUNC("OutputCall::ei_environment_shader(\""<<shader_name <<"\")");
		::ei_environment_shader(shader_name );
	}

	void OutputCall::ei_end_material()
	{
		//CM_TRACE_FUNC("OutputCall::ei_end_material()");
		::ei_end_material();
	}
	//	Lights:
	void OutputCall::ei_light( const char *name )
	{
		//CM_TRACE_FUNC("OutputCall::ei_light(\""<<name<<"\")");
		::ei_light(name);
	}
	void OutputCall::ei_light_shader( const char *shader_name )
	{
		//CM_TRACE_FUNC("OutputCall::ei_light_shader(\""<<shader_name<<"\" )");
		::ei_light_shader(shader_name);
	}
	void OutputCall::ei_origin(eiScalar x, eiScalar y, eiScalar z)
	{
		//CM_TRACE_FUNC("OutputCall::ei_origin("<<x<<","<<y<<","<<z<<")");
		::ei_origin(x,y,z);
	}
	void OutputCall::ei_area_samples( eiInt samples )
	{
		//CM_TRACE_FUNC("OutputCall::ei_area_samples("<<samples<<" )");
		::ei_area_samples(samples);
	}

	void OutputCall::ei_end_light()
	{
		//CM_TRACE_FUNC("OutputCall::ei_end_light()");
		::ei_end_light();
	}
	//	Objects:
	void OutputCall::ei_object( const char *type, const char *name )
	{
		//CM_TRACE_FUNC("OutputCall::ei_object(\""<< type <<"\",\""<<name<<"\")" ); 
		::ei_object(type, name);
	}
	void OutputCall::ei_box(eiScalar xmin, eiScalar ymin, eiScalar zmin, 
		eiScalar xmax, eiScalar ymax, eiScalar zmax)
	{
		//CM_TRACE_FUNC("OutputCall::ei_box("<< xmin<<","<<ymin<<","<<zmin<<", "<<xmax<<","<<ymax<<","<<zmax<<")");
		::ei_box(xmin,ymin,zmin,xmax,ymax,zmax);

	}
	void OutputCall::ei_motion_box(eiScalar xmin, eiScalar ymin, eiScalar zmin, 
		eiScalar xmax, eiScalar ymax, eiScalar zmax)
	{
		//CM_TRACE_FUNC("OutputCall::ei_motion_box("<< xmin<<","<<ymin<<","<<zmin<<", "<<xmax<<","<<ymax<<","<<zmax<<")");
		::ei_motion_box(xmin,ymin,zmin, xmax,ymax,zmax);
	}
	/* procedural objects */
	void OutputCall::ei_geometry_shader(const char *shader_name)
	{
		//CM_TRACE_FUNC("OutputCall::ei_geometry_shader(\""<<shader_name<<"\")");
		::ei_geometry_shader(shader_name);
	}
	void OutputCall::ei_pos_list(const eiTag tab)
	{
		//CM_TRACE_FUNC("OutputCall::ei_pos_list(tag)" ); 
		::ei_pos_list(tab);
	}
	void OutputCall::ei_motion_pos_list(const eiTag tab)
	{
		//CM_TRACE_FUNC("OutputCall::ei_motion_pos_list(tag)" ); 
		::ei_motion_pos_list(tab);
	}
	void OutputCall::ei_triangle_list(const eiTag tab)
	{
		//CM_TRACE_FUNC("OutputCall::ei_triangle_list(tag)" ); 
		::ei_triangle_list(tab);
	}
	void OutputCall::ei_end_object()
	{
		//CM_TRACE_FUNC("OutputCall::ei_end_object()"); 
		::ei_end_object();
	}


	//	Instances:
	void OutputCall::ei_instance( const char *name )
	{
		//CM_TRACE_FUNC("OutputCall::ei_instance(\""<<name<<"\")");
		::ei_instance(name);
	}
	void OutputCall::ei_element( const char *element_name )
	{
		//CM_TRACE_FUNC("OutputCall::ei_element(\""<<element_name<<"\")");
		::ei_element(element_name);
	}
	void OutputCall::ei_transform( 
		float t00,float t01,float t02,float t03,
		float t10,float t11,float t12,float t13,
		float t20,float t21,float t22,float t23,
		float t30,float t31,float t32,float t33 
		)
	{
		//CM_TRACE_FUNC("OutputCall::ei_transform("<< t00<<","<<t01<<","<<t02<<","<<t03<<",   "<<t10<<","<<t11<<","<<t12<<","<<t13<<",   "<<  t20<<","<<t21<<","<<t22<<","<<t23<<",   "<<  t30<<","<<t31<<","<<t32<<","<<t33 <<")");
		::ei_transform(t00,t01,t02,t03,t10,t11,t12,t13,t20,t21,t22,t23,t30,t31,t32,t33);
	}
	void OutputCall::ei_motion_transform( 
		float t00,float t01,float t02,float t03,
		float t10,float t11,float t12,float t13,
		float t20,float t21,float t22,float t23,
		float t30,float t31,float t32,float t33
		)
	{
		//CM_TRACE_FUNC("OutputCall::ei_motion_transform("<< t00<<","<<t01<<","<<t02<<","<<t03<<",   "<<t10<<","<<t11<<","<<t12<<","<<t13<<",   "<<  t20<<","<<t21<<","<<t22<<","<<t23<<",   "<<  t30<<","<<t31<<","<<t32<<","<<t33 <<")");
		::ei_motion_transform(t00,t01,t02,t03,t10,t11,t12,t13,t20,t21,t22,t23,t30,t31,t32,t33);
	}
	void OutputCall::ei_mtl( const char *material_name )
	{
		//CM_TRACE_FUNC("OutputCall::ei_mtl(\""<<material_name<<"\")");
		::ei_mtl(material_name);
	}

	void OutputCall::ei_end_instance()
	{
		//CM_TRACE_FUNC("OutputCall::ei_end_instance()");
		::ei_end_instance();
	}


	//	Instance Groups:
	void OutputCall::ei_instgroup( const char *name )
	{
		//CM_TRACE_FUNC("OutputCall::ei_instgroup(\""<<name<<"\")");
		::ei_instgroup(name);
	}
	void OutputCall::ei_add_instance( const char *name )
	{
		//CM_TRACE_FUNC("OutputCall::ei_add_instance(\""<<name<<"\")");
		::ei_add_instance(name);
	}
	void OutputCall::ei_end_instgroup()
	{
		//CM_TRACE_FUNC("OutputCall::ei_end_instgroup( )");
		::ei_end_instgroup();
	}
	//
	void OutputCall::ei_shader(const char *type_name, const char *instance_name)
	{
		//CM_TRACE_FUNC("OutputCall::ei_shader(\""<<type_name<<"\", \""<<instance_name<<"\")");
		::ei_shader(type_name, instance_name);
	}
	void OutputCall::ei_shader_param(
		const char *param_name, 
		const void *param_value)
	{
		//CM_TRACE_FUNC("OutputCall::ei_shader_param(\""<<param_name<<"\", param_value ) not implemented");
		::ei_shader_param(param_name, param_value);
	}
	void OutputCall::ei_shader_param_token(
		const char *param_name, 
		const char *param_value)
	{
		//CM_TRACE_FUNC("OutputCall::ei_shader_param_token(\""<<param_name<<"\", param_value ) not implemented");
		::ei_shader_param_token( param_name, ei_token(param_value) );
	}
	void OutputCall::ei_shader_param_int(
		const char *param_name, 
		const eiInt param_value)
	{
		//CM_TRACE_FUNC("OutputCall::ei_shader_param_int(\""<<param_name<<"\","<<param_value<<")");
		::ei_shader_param_int(param_name,param_value);
	}
	void OutputCall::ei_shader_param_scalar(
		const char *param_name, 
		const eiScalar param_value)
	{
		//CM_TRACE_FUNC("OutputCall::ei_shader_param_scalar(\""<<param_name<<"\","<<param_value<<")");
		::ei_shader_param_scalar(param_name,param_value);
	}
	void OutputCall::ei_shader_param_vector(
		const char *param_name, 
		const eiScalar x, const eiScalar y, const eiScalar z)
	{
		//CM_TRACE_FUNC("OutputCall::ei_shader_param_vector(\""<<param_name<<"\","<<x<<","<<y<<","<<z<<")");
		::ei_shader_param_vector(param_name,x,y,z);
	}
	void OutputCall::ei_shader_param_vector4(
		const char *param_name, 
		const eiScalar x, const eiScalar y, const eiScalar z, const eiScalar w)
	{
		//CM_TRACE_FUNC("OutputCall::ei_shader_param_vector4(\""<<param_name<<"\","<<x<<","<<y<<","<<z<<","<<w<<")");
		::ei_shader_param_vector4(param_name,x,y,z,w);
	}
	void OutputCall::ei_shader_param_tag(
		const char *param_name, 
		const eiTag param_value)
	{
		//CM_TRACE_FUNC("OutputCall::ei_shader_param_tag(\""<<param_name<<"\","<<param_value<<")");
		::ei_shader_param_tag(param_name,param_value);
	}
	void OutputCall::ei_shader_param_texture(
		const char *param_name, 
		const char *texture_name)
	{
		//CM_TRACE_FUNC("OutputCall::ei_shader_param_texture(\""<<param_name<<"\",\""<<texture_name<<"\")");
		::ei_shader_param_texture(param_name,texture_name);
	}
	void OutputCall::ei_shader_param_index(
		const char *param_name, 
		const eiIndex param_value)
	{
		//CM_TRACE_FUNC("OutputCall::ei_shader_param_index(\""<<param_name<<"\","<<param_value<<")");
		::ei_shader_param_index(param_name,param_value);
	}
	void OutputCall::ei_shader_param_bool(
		const char *param_name, 
		const eiBool param_value)
	{
		//CM_TRACE_FUNC("OutputCall::ei_shader_param_bool(\""<<param_name<<"\","<<param_value<<")");
		::ei_shader_param_bool(param_name,param_value);
	}
	void OutputCall::ei_shader_link_param(
		const char *param_name, 
		const char *src_shader_name, 
		const char *src_param_name)
	{
		//CM_TRACE_FUNC("OutputCall::ei_shader_link_param(\""<<param_name<<"\",\""<<src_shader_name<<"\",\""<<src_param_name<<"\")");
		::ei_shader_link_param(param_name,src_shader_name,src_param_name);
	}
	void OutputCall::ei_end_shader()
	{
		//CM_TRACE_FUNC("OutputCall::ei_end_shader()");
		::ei_end_shader();
	}

	void OutputCall::ei_declare(const char *name, const eiInt storage_class, const eiInt type/*, const void *tag*/)
	{
		//CM_TRACE_FUNC("OutputCall::ei_declare(\""<<name<<"\","<<storage_class<<","<<type<<", &tag)");
		liquidMessage2(messageWarning,"OutputCall::ei_declare(), tag is not used");
		::ei_declare(name,storage_class,type, &null_token);
	}
	void OutputCall::ei_variable(const char *name, const void *tag)
	{
		//CM_TRACE_FUNC("OutputCall::ei_variable(\""<<name<<"\", &tag)");
		::ei_variable(name, tag);
	}
	void OutputCall::ei_degree(const eiInt degree)
	{
		//CM_TRACE_FUNC("OutputCall::ei_degree("<<degree<<")");
		::ei_degree(degree);
	}

	//----------------------------------------------------
	// Warped ER API interfaces
	//----------------------------------------------------
	void OutputCall::liq_lightgroup_in_object_instance(const char *light_group_name)
	{
		//CM_TRACE_FUNC("OutputCall::liq_lightgroup_in_object_instance("<<light_group_name<<")");
		//s("{//light group(light-link group)");
		const char *null_token = NULL;
		::ei_declare("lightgroup", EI_CONSTANT, EI_TYPE_TOKEN, &null_token);
		const char *lightgroup_token = ei_token(light_group_name);
		::ei_variable("lightgroup", &lightgroup_token );
		//s("}");
	}
	void OutputCall::liq_lightgroup_in_light_instance_beg()
	{
		//CM_TRACE_FUNC("OutputCall::liq_lightgroup_in_light_instance_beg()");
		//s("{");
	}
	void OutputCall::liq_lightgroup_in_light_instance(const char *light_group_name)
	{
		//CM_TRACE_FUNC("OutputCall::liq_lightgroup_in_light_instance("<<light_group_name<<")");
		::ei_declare(light_group_name, EI_CONSTANT, EI_TYPE_INT, &defaultInt);
	}
	void OutputCall::liq_lightgroup_in_light_instance_end()
	{
		//CM_TRACE_FUNC("OutputCall::liq_lightgroup_in_light_instance_end()");
		//s("}");
	}
	void OutputCall::liq_object(
		const std::string &objname,
		const std::vector<MVector> &POSITION,
		const std::vector<MVector> &POSITION_mb,//motion blur position
		const std::vector<std::size_t> &INDEX,//global vertex index
		const std::vector<MVector> &NORMAL,
		const std::vector<MVector> &UV
		)
	{
		//CM_TRACE_FUNC("OutputCall::liq_object("<<light_group_name<<")");
		eiTag tag;

		::ei_object( "poly", objname.c_str() );

		//vertex positions(required)
		tag = ::ei_tab(EI_TYPE_VECTOR, 1024);
		::ei_pos_list( tag );
		for(std::size_t i=0; i<POSITION.size(); ++i)
		{
			::ei_tab_add_vector( POSITION[i].x, POSITION[i].y, POSITION[i].z );
		}
		::ei_end_tab();

		//### vertex deform positions(optional)
		if( POSITION_mb.size() > 0 )
		{
			tag = ::ei_tab(EI_TYPE_VECTOR, 1024);
			::ei_motion_pos_list( tag );
			for(std::size_t i=0; i<POSITION_mb.size(); ++i)
			{
				::ei_tab_add_vector( POSITION_mb[i].x, POSITION_mb[i].y, POSITION_mb[i].z );
			}
			::ei_end_tab();
		}
				
		//### N ###"(optional)
		if( NORMAL.size() >0 )
		{
			tag = eiNULL_TAG;
			::ei_declare("N", EI_VARYING, EI_TYPE_TAG, &tag);
			tag = ::ei_tab(EI_TYPE_VECTOR, 1024);
			::ei_variable("N", &tag);
			for(size_t i=0; i<NORMAL.size(); ++i)
			{
				::ei_tab_add_vector(NORMAL[i].x, NORMAL[i].y, NORMAL[i].z);
			}
			::ei_end_tab();
		}
		//### N ### end

		//### UV (optional)
		if( UV.size() >0 )
		{
			tag = eiNULL_TAG;
			::ei_declare("uv", EI_VARYING, EI_TYPE_TAG, &tag);
			tag = ::ei_tab(EI_TYPE_VECTOR2, 1024);
			::ei_variable("uv", &tag);
			for(size_t i = 0; i<UV.size(); ++i)
			{
				::ei_tab_add_vector2(UV[i].x, UV[i].y);
			}
			::ei_end_tab();
		}


		//### triangles(required)
		//size="<< INDEX.size();
		if( INDEX.size()>0 )
		{
			tag = ::ei_tab(EI_TYPE_INDEX, 1024);
			::ei_triangle_list( tag );
			for(size_t i=0; i<INDEX.size(); ++i)
			{
				::ei_tab_add_index(INDEX[i]);
			}
			::ei_end_tab();
		}

		::ei_end_object();

	}
}//namespace elvishray
#endif//_USE_ELVISHRAY_