#include <liqConfig.h>
#ifdef _USE_ELVISHRAY_
#include "er_output_ess.h"
#include <liqlog.h>
#include <trace/trace.hpp>

namespace elvishray
{
	OutputESS::OutputESS(const std::string &filefullpath)
		:OutputBase()
	{
		CM_TRACE_FUNC("OutputESS::OutputESS("<<filefullpath<<")");

		const std::string logFileName(filefullpath+".ess");
		m_outputfile.open(logFileName.c_str(), std::ios_base::out);
		if( !m_outputfile.is_open() )
		{
			liquidMessage2(messageError,"can't open file: %s.\n", logFileName.c_str() );
		}
	}
	OutputESS::~OutputESS()
	{	
		CM_TRACE_FUNC("OutputESS::~OutputESS()");

		m_outputfile.close();
	}
	void OutputESS::init()
	{
		CM_TRACE_FUNC("OutputESS::init()");
	}
	//----------------------------------------------------
	// ER API interfaces
	//----------------------------------------------------
	void OutputESS::ln()
	{
		//CM_TRACE_FUNC("OutputESS::ln("<<msg<<")");
		m_outputfile<<std::endl;
	}
	void OutputESS::s(const std::string &msg)
	{
		//CM_TRACE_FUNC("OutputESS::s("<<msg<<")");
		m_outputfile<<msg<<std::endl;
	}
	void OutputESS::a(const std::string &msg)
	{
		//CM_TRACE_FUNC("OutputESS::a("<<msg<<")");
		m_outputfile<<"##"<<msg<<std::endl;
	}
	void OutputESS::ei_context()
	{		
		//CM_TRACE_FUNC("OutputESS:ei_context()");
		m_outputfile<<"//context"<<std::endl;
	}
	void OutputESS::ei_end_context()
	{	
		//CM_TRACE_FUNC("OutputESS::ei_end_context()");
		m_outputfile<<"//end context"<<std::endl;
	}
	//	Client application connection:
	void OutputESS::ei_connection( eiConnection *con )
	{
		//CM_TRACE_FUNC("OutputESS::ei_connection(...)");
		m_outputfile<<"//ei_connection(con);"<<std::endl;
	}
	//	Shader Linking:
	void OutputESS::ei_link( const char *filename )
	{
		//CM_TRACE_FUNC("OutputESS::ei_link("<<filename<<")");
		m_outputfile<<"ei_link(\""<<filename<<"\");"<<std::endl;
	}
	//	Commands:
	void OutputESS::ei_verbose( int level )
	{
		//CM_TRACE_FUNC("OutputESS::ei_verbose("<<level<<")");
		m_outputfile<<"ei_verbose("<<level<<");"<<std::endl;
	}
	void OutputESS::ei_delete( const char *element_name )
	{
		//CM_TRACE_FUNC("OutputESS::ei_delete( \""<<element_name<<"\" )");
		m_outputfile<<"ei_delete(\""<<element_name<<"\");"<<std::endl;
	}
	void OutputESS::ei_render( const char *root_instgroup, const char *camera_inst, const char *options )
	{
		//CM_TRACE_FUNC("OutputESS::ei_render(\""<<root_instgroup<<"\", \""<<camera_inst<<"\", \""<<options<<"\")");
		m_outputfile<<"ei_render( \""<<root_instgroup<<"\", \""<<camera_inst<<"\", \""<<options<<"\" );"<<std::endl;
	}

	eiTag OutputESS::ei_tab(const eiInt type, const eiInt items_per_slot)
	{
		//CM_TRACE_FUNC("OutputESS::ei_tab("<<type<<","<<items_per_slot<<")");
		m_outputfile<<"ei_tab( "<<type<<", "<<items_per_slot<<" );"<<std::endl;
		liquidMessage2(messageError, "OutputESS::ei_tab(), return 0;");
		return 0;
	}
	//void OutputESS::ei_tab_add(const void *value);
	//void OutputESS::ei_tab_add_int(const eiInt value);
	void OutputESS::ei_tab_add_scalar(const eiScalar value)
	{
		//CM_TRACE_FUNC("OutputESS::ei_tab_add_scalar("<<value<<")");
		m_outputfile<<"ei_tab_add_scalar("<<value<<");"<<std::endl;
	}
	void OutputESS::ei_tab_add_vector(const eiScalar x, const eiScalar y, const eiScalar z)
	{
		//CM_TRACE_FUNC("OutputESS::ei_tab_add_vector("<<x<<","<<y<<","<<z<<")");
		m_outputfile<<"ei_tab_add_vector("<<x<<","<<y<<","<<z<<");"<<std::endl;
	}
	void OutputESS::ei_tab_add_vector2(const eiScalar x, const eiScalar y)
	{
		//CM_TRACE_FUNC("OutputESS::ei_tab_add_vector2("<<x<<","<<y<<")");
		m_outputfile<<"ei_tab_add_vector2("<<x<<","<<y<<");"<<std::endl;
	}
	void OutputESS::ei_tab_add_index(const eiIndex value)
	{
		//CM_TRACE_FUNC("OutputESS::ei_tab_add_index("<<value<<")");
		m_outputfile<<"ei_tab_add_index("<<value<<");"<<std::endl;
	}
	//void OutputESS::ei_tab_add_bool(const eiBool value);
	void OutputESS::ei_end_tab()
	{
		//CM_TRACE_FUNC("OutputESS::ei_end_tab()");
		m_outputfile<<"ei_end_tab();"<<std::endl;
	}


	//	Options:
	void OutputESS::ei_options( const char *name )
	{
		//CM_TRACE_FUNC("OutputESS::ei_options(\""<<name<<"\")");
		m_outputfile<<"ei_options(\""<<name<<"\");"<<std::endl;
	}

	//	Sampling Quality:
	void OutputESS::ei_contrast( eiScalar value )
	{
		//CM_TRACE_FUNC("OutputESS::ei_contrast("<<value<<")");
		m_outputfile<<"ei_contrast("<<value<<");"<<std::endl;
	}
	void OutputESS::ei_samples( int _min, int _max )
	{
		//CM_TRACE_FUNC("OutputESS::ei_samples("<<_min<<","<<_max<<")");
		m_outputfile<<"ei_samples("<<_min<<","<<_max<<");"<<std::endl;
	}
	void OutputESS::ei_bucket_size( int size )
	{
		//CM_TRACE_FUNC("OutputESS::ei_bucket_size("<<size<<")");
		m_outputfile<<"ei_bucket_size("<<size<<");"<<std::endl;
	}
	void OutputESS::ei_filter( int filter, float size )
	{
		//CM_TRACE_FUNC("OutputESS::ei_filter("<<filter<<","<<size<<")");
		m_outputfile<<"ei_filter("<<filter<<","<<size<<");"<<std::endl;
	}
	//	Motion Blur:
	void OutputESS::ei_shutter( float open, float close )
	{
		//CM_TRACE_FUNC("OutputESS::ei_shutter("<<open<<", "<<close<<")");
		m_outputfile<<"ei_shutter("<<open<<", "<<close<<");"<<std::endl;
	}
	void OutputESS::ei_motion( int type )
	{
		//CM_TRACE_FUNC("OutputESS::ei_motion("<<type<<")");
		m_outputfile<<"ei_motion("<<type<<");"<<std::endl;
	}

	//	Trace Depth:
	void OutputESS::ei_trace_depth( int reflect, int refract, int sum )
	{
		//CM_TRACE_FUNC("OutputESS::ei_trace_depth(4,4,4,"<<reflect<<","<<refract<<","<<sum<<")");
		liquidMessage2(messageError, "OutputESS::ei_trace_depth() need feed the parameters");
		m_outputfile<<"ei_trace_depth(4,4,4,"<<reflect<<","<<refract<<","<<sum<<");"<<std::endl;
	}
	//	Shadow:
	void OutputESS::ei_shadow( int type )
	{
		//CM_TRACE_FUNC("OutputESS::ei_shadow("<<type<<")");
		m_outputfile<<"ei_shadow("<<type<<");"<<std::endl;
	}
	//	Feature Disabling:
	void OutputESS::ei_lens(eiInt type)
	{
		//CM_TRACE_FUNC("OutputESS::ei_lens("<<type<<" )");
		m_outputfile<<"ei_lens("<<type<<");"<<std::endl;
	}
	void OutputESS::ei_volume( int type )
	{
		//CM_TRACE_FUNC("OutputESS::ei_volume("<<type<<")");
		m_outputfile<<"ei_volume("<<type<<");"<<std::endl;
	}
	void OutputESS::ei_geometry( int type )
	{
		//CM_TRACE_FUNC("OutputESS::ei_geometry("<<type<<")");
		m_outputfile<<"ei_geometry("<<type<<");"<<std::endl;
	}
	void OutputESS::ei_displace( int type )
	{
		//CM_TRACE_FUNC("OutputESS::ei_displace("<<type<<")");
		m_outputfile<<"ei_displace("<<type<<");"<<std::endl;
	}
	void OutputESS::ei_max_displace(eiScalar dist)
	{
		//CM_TRACE_FUNC("OutputESS::ei_max_displace("<<dist<<")");
		m_outputfile<<"ei_max_displace("<<dist<<");"<<std::endl;
	}
	void OutputESS::ei_imager( int type )
	{
		//CM_TRACE_FUNC("OutputESS::ei_imager("<<type<<")");
		m_outputfile<<"ei_imager("<<type<<");"<<std::endl;
	}
	//	Frame Buffer Control:
	void OutputESS::ei_exposure( float gain, float gamma )
	{
		//CM_TRACE_FUNC("OutputESS::ei_exposure("<<gain<<","<<gamma<<")");
		m_outputfile<<"ei_exposure("<<gain<<","<<gamma<<");"<<std::endl;
	}
	void OutputESS::ei_quantize( float one, float _min, float _max, float dither_amplitude )
	{
		//CM_TRACE_FUNC("OutputESS::ei_quantize("<<one<<","<<_min<<","<<_max<<","<<dither_amplitude<<")");
		m_outputfile<<"ei_quantize("<<one<<","<<_min<<","<<_max<<","<<dither_amplitude<<");"<<std::endl;
	}
	//	Miscellaneous:
	void OutputESS::ei_face( int type )
	{
		//CM_TRACE_FUNC("OutputESS::ei_face("<<type<<")");
		m_outputfile<<"ei_face("<<type<<");"<<std::endl;
	}
	void OutputESS::ei_approx(const eiApprox *approx)
	{
		char buf[1024];
		sprintf_s(buf, 1024,
			"//method=%d,any=%x,view_dep=%x,args=[%f,%f,%f,%f],\n"\
			"//sharp=%f,min_subdiv=%d,max_subdiv=%d,max_grid_size=%d,motion_factor=%f",
			approx->method,approx->any,approx->view_dep,approx->args[0],approx->args[1],approx->args[2],approx->args[3],
			approx->sharp,approx->min_subdiv,approx->max_subdiv,approx->max_grid_size,approx->motion_factor);

		//CM_TRACE_FUNC("OutputESS::ei_approx(approx)");
		liquidMessage2(messageError, "OutputESS::ei_approx()");
		m_outputfile<<"ei_approx(approx);"<<std::endl;
	}
	void OutputESS::ei_end_options()
	{
		//CM_TRACE_FUNC("OutputESS::ei_end_options()");
		m_outputfile<<"ei_end_options();"<<std::endl;
	}
	//	Cameras:
	void OutputESS::ei_camera( const char *name )
	{
		//CM_TRACE_FUNC("OutputESS::ei_camera(\""<< name <<"\")");
		m_outputfile<<"ei_camera(\""<<name<<"\");"<<std::endl;
	}
	//	Output Statements:
	void OutputESS::ei_output( const char *filename, const char *fileformat, const eiInt datatype)
	{
		//CM_TRACE_FUNC("OutputESS::ei_output(\""<< filename <<"\",\""<<fileformat<<"\","<<datatype<<")");
		m_outputfile<<"ei_output(\""<< filename <<"\",\""<<fileformat<<"\","<<datatype<<");"<<std::endl;
	}
	void OutputESS::ei_output_variable(const char *name, const eiInt datatype)
	{
		//CM_TRACE_FUNC("OutputESS::ei_output_variable(\""<< name <<"\","<<datatype<<")");
		m_outputfile<<"ei_output_variable(\""<< name <<"\","<<datatype<<");"<<std::endl;
	}
	void OutputESS::ei_end_output()
	{
		//CM_TRACE_FUNC("OutputESS::ei_end_output()");
		m_outputfile<<"ei_end_output();"<<std::endl;
	}
	//	Other Camera Statements:
	void OutputESS::ei_focal( float distance )
	{
		//CM_TRACE_FUNC("OutputESS::ei_focal("<< distance <<")");
		m_outputfile<<"ei_focal("<< distance <<");"<<std::endl;
	}
	void OutputESS::ei_aperture( float aperture )
	{
		//CM_TRACE_FUNC("OutputESS::ei_aperture("<< aperture <<")");
		m_outputfile<<"ei_aperture("<< aperture <<");"<<std::endl;
	}
	void OutputESS::ei_aspect( float aspect )
	{
		//CM_TRACE_FUNC("OutputESS::ei_aspect("<< aspect <<")");
		m_outputfile<<"ei_aspect("<< aspect <<");"<<std::endl;
	}
	void OutputESS::ei_resolution( eiInt x, eiInt y )
	{
		//CM_TRACE_FUNC("OutputESS::ei_resolution("<< x<<","<<y <<")");
		m_outputfile<<"ei_resolution("<< x<<","<<y <<");"<<std::endl;
	}
	void OutputESS::ei_window( eiInt xmin, eiInt xmax, eiInt ymin, eiInt ymax )
	{
		//CM_TRACE_FUNC("OutputESS::ei_window("<< xmin<<","<<xmax<<","<<ymin<<","<<ymax<<")");
		m_outputfile<<"ei_window("<< xmin<<","<<xmax<<","<<ymin<<","<<ymax<<");"<<std::endl;
	}
	void OutputESS::ei_clip( float hither, float yon )
	{
		//CM_TRACE_FUNC("OutputESS::ei_clip("<<hither<<","<<yon<<")");
		m_outputfile<<"ei_clip("<<hither<<","<<yon<<");"<<std::endl;
	}
	void OutputESS::ei_lens_shader(const char *shader_name)
	{
		//CM_TRACE_FUNC("OutputESS::ei_lens_shader(\""<<shader_name<<"\")");
		m_outputfile<<"ei_lens_shader(\""<<shader_name<<"\");"<<std::endl;
	}
	void OutputESS::ei_imager_shader(const char *shader_name)
	{
		//CM_TRACE_FUNC("OutputESS::ei_imager_shader(\""<<shader_name<<"\")");
		m_outputfile<<"ei_imager_shader(\""<<shader_name<<"\");"<<std::endl;
	}
	void OutputESS::ei_end_camera()
	{
		//CM_TRACE_FUNC("OutputESS::ei_end_camera()");
		m_outputfile<<"ei_end_camera();"<<std::endl;
	}
	//	Textures:
	void OutputESS::ei_make_texture( const char *picturename, const char *texturename, 
		int swrap, int twrap, int filter, float swidth, float twidth )
	{
		//CM_TRACE_FUNC("OutputESS::ei_make_texture(\""<<picturename<<"\", \""<< texturename<<"\","<<swrap<<","<< twrap<<","<<filter<<", "<<swidth<<", "<<twidth<<")");
		m_outputfile<<"ei_make_texture(\""<<picturename<<"\", \""<< texturename<<"\","<<swrap<<","<< twrap<<","<<filter<<", "<<swidth<<", "<<twidth<<");"<<std::endl;
	}
	void OutputESS::ei_texture(const char *name)
	{
		//CM_TRACE_FUNC("OutputESS::ei_texture("<<name<<")");
		m_outputfile<<"ei_texture("<<name<<");"<<std::endl;
	}
	void OutputESS::ei_file_texture(const char *filename, const eiBool local)
	{
		//CM_TRACE_FUNC("OutputESS::ei_file_texture("<<filename<<","<<local<<")");
		m_outputfile<<"ei_file_texture("<<filename<<", "<<local<<");"<<std::endl;
	}
	void OutputESS::ei_end_texture()
	{
		//CM_TRACE_FUNC("OutputESS::ei_end_texture()");
		m_outputfile<<"ei_end_texture();"<<std::endl;
	}
	//	Materials:
	void OutputESS::ei_material( const char *name )
	{
		//CM_TRACE_FUNC("OutputESS::ei_material(\""<<name<<"\")");
		m_outputfile<<"ei_material(\""<<name<<"\");"<<std::endl;
	}
	void OutputESS::ei_surface_shader( const char *shader_name )
	{
		//CM_TRACE_FUNC("OutputESS::ei_surface_shader(\""<<shader_name <<"\")");
		m_outputfile<<"ei_surface_shader(\""<<shader_name <<"\");"<<std::endl;
	}
	void OutputESS::ei_displace_shader( const char *shader_name )
	{
		//CM_TRACE_FUNC("OutputESS::ei_displace_shader(\""<<shader_name <<"\")");
		m_outputfile<<"ei_displace_shader(\""<<shader_name <<"\" );"<<std::endl;
	}
	void OutputESS::ei_shadow_shader( const char *shader_name )
	{
		//CM_TRACE_FUNC("OutputESS::ei_shadow_shader(\""<<shader_name <<"\")");
		m_outputfile<<"ei_shadow_shader(\""<<shader_name <<"\" );"<<std::endl;
	}
	void OutputESS::ei_volume_shader( const char *shader_name )
	{
		//CM_TRACE_FUNC("OutputESS::ei_volume_shader(\""<<shader_name <<"\")");
		m_outputfile<<"ei_volume_shader(\""<<shader_name <<"\" );"<<std::endl;
	}
	void OutputESS::ei_environment_shader( const char *shader_name )
	{
		//CM_TRACE_FUNC("OutputESS::ei_environment_shader(\""<<shader_name <<"\")");
		m_outputfile<<"ei_environment_shader(\""<<shader_name <<"\");"<<std::endl;
	}

	void OutputESS::ei_end_material()
	{
		//CM_TRACE_FUNC("OutputESS::ei_end_material()");
		m_outputfile<<"ei_end_material();"<<std::endl;
	}
	//	Lights:
	void OutputESS::ei_light( const char *name )
	{
		//CM_TRACE_FUNC("OutputESS::ei_light(\""<<name<<"\")");
		m_outputfile<<"ei_light(\""<<name<<"\");"<<std::endl;
	}
	void OutputESS::ei_light_shader( const char *shader_name )
	{
		//CM_TRACE_FUNC("OutputESS::ei_light_shader(\""<<shader_name<<"\" )");
		m_outputfile<<"ei_light_shader(\""<<shader_name<<"\");"<<std::endl;
	}
	void OutputESS::ei_origin(eiScalar x, eiScalar y, eiScalar z)
	{
		//CM_TRACE_FUNC("OutputESS::ei_origin("<<x<<","<<y<<","<<z<<")");
		m_outputfile<<"ei_origin("<<x<<","<<y<<","<<z<<");"<<std::endl;
	}
	void OutputESS::ei_area_samples( eiInt samples )
	{
		//CM_TRACE_FUNC("OutputESS::ei_area_samples("<<samples<<" )");
		m_outputfile<<"ei_area_samples("<<samples<<");"<<std::endl;
	}

	void OutputESS::ei_end_light()
	{
		//CM_TRACE_FUNC("OutputESS::ei_end_light()");
		m_outputfile<<"ei_end_light();"<<std::endl;
	}
	//	Objects:
	void OutputESS::ei_object( const char *type, const char *name )
	{
		//CM_TRACE_FUNC("OutputESS::ei_object(\""<< type <<"\",\""<<name<<"\")" ); 
		m_outputfile<<"ei_object(\""<< type <<"\",\""<<name<<"\");"<<std::endl;
	}
	void OutputESS::ei_box(eiScalar xmin, eiScalar ymin, eiScalar zmin, 
		eiScalar xmax, eiScalar ymax, eiScalar zmax)
	{
		//CM_TRACE_FUNC("OutputESS::ei_box("<<xmin<<","<<ymin<<","<<zmin<<", "<<xmax<<","<<ymax<<","<<zmax<<")");
		m_outputfile<<"ei_box("<<xmin<<","<<ymin<<","<<zmin<<", "<<xmax<<","<<ymax<<","<<zmax<<");"<<std::endl;

	}
	void OutputESS::ei_motion_box(eiScalar xmin, eiScalar ymin, eiScalar zmin, 
		eiScalar xmax, eiScalar ymax, eiScalar zmax)
	{
		//CM_TRACE_FUNC("OutputESS::ei_motion_box("<< xmin<<","<<ymin<<","<<zmin<<", "<<xmax<<","<<ymax<<","<<zmax<<")");
		m_outputfile<<"ei_motion_box("<< xmin<<","<<ymin<<","<<zmin<<", "<<xmax<<","<<ymax<<","<<zmax<<");"<<std::endl;
	}
	/* procedural objects */
	void OutputESS::ei_geometry_shader(const char *shader_name)
	{
		//CM_TRACE_FUNC("OutputESS::ei_geometry_shader(\""<<shader_name<<"\")");
		m_outputfile<<"ei_geometry_shader(\""<<shader_name<<"\");"<<std::endl;
	}
	void OutputESS::ei_pos_list(const eiTag tab)
	{
		//CM_TRACE_FUNC("OutputESS::ei_pos_list(tag)" );
		liquidMessage2(messageError,"OutputESS::ei_pos_list(tab)");
		m_outputfile<<"ei_pos_list(tab);"<<std::endl;
	}
	void OutputESS::ei_motion_pos_list(const eiTag tab)
	{
		//CM_TRACE_FUNC("OutputESS::ei_motion_pos_list(tag)" ); 
		liquidMessage2(messageError,"OutputESS::ei_motion_pos_list(tab)");
		m_outputfile<<"ei_motion_pos_list(tab);"<<std::endl;
	}
	void OutputESS::ei_triangle_list(const eiTag tab)
	{
		//CM_TRACE_FUNC("OutputESS::ei_triangle_list(tag)" );
		liquidMessage2(messageError,"OutputESS::ei_triangle_list(tab)");
		m_outputfile<<"ei_triangle_list(tab);"<<std::endl;
	}
	void OutputESS::ei_end_object()
	{
		//CM_TRACE_FUNC("OutputESS::ei_end_object()"); 
		m_outputfile<<"ei_end_object();"<<std::endl;
	}


	//	Instances:
	void OutputESS::ei_instance( const char *name )
	{
		//CM_TRACE_FUNC("OutputESS::ei_instance(\""<<name<<"\")");
		m_outputfile<<"ei_instance(\""<<name<<"\");"<<std::endl;
	}
	void OutputESS::ei_element( const char *element_name )
	{
		//CM_TRACE_FUNC("OutputESS::ei_element(\""<<element_name<<"\")");
		m_outputfile<<"ei_element(\""<<element_name<<"\");"<<std::endl;
	}
	void OutputESS::ei_transform( 
		float t00,float t01,float t02,float t03,
		float t10,float t11,float t12,float t13,
		float t20,float t21,float t22,float t23,
		float t30,float t31,float t32,float t33 
		)
	{
		//CM_TRACE_FUNC("OutputESS::ei_transform("<< t00<<","<<t01<<","<<t02<<","<<t03<<",   "<<t10<<","<<t11<<","<<t12<<","<<t13<<",   "<<  t20<<","<<t21<<","<<t22<<","<<t23<<",   "<<  t30<<","<<t31<<","<<t32<<","<<t33 <<")");
		m_outputfile<<"ei_transform("<< t00<<","<<t01<<","<<t02<<","<<t03<<",   "<<t10<<","<<t11<<","<<t12<<","<<t13<<",   "<<  t20<<","<<t21<<","<<t22<<","<<t23<<",   "<<  t30<<","<<t31<<","<<t32<<","<<t33 <<");"<<std::endl;
	}
	void OutputESS::ei_motion_transform( 
		float t00,float t01,float t02,float t03,
		float t10,float t11,float t12,float t13,
		float t20,float t21,float t22,float t23,
		float t30,float t31,float t32,float t33
		)
	{
		//CM_TRACE_FUNC("OutputESS::ei_motion_transform("<< t00<<","<<t01<<","<<t02<<","<<t03<<",   "<<t10<<","<<t11<<","<<t12<<","<<t13<<",   "<<  t20<<","<<t21<<","<<t22<<","<<t23<<",   "<<  t30<<","<<t31<<","<<t32<<","<<t33 <<")");
		m_outputfile<<"ei_motion_transform("<< t00<<","<<t01<<","<<t02<<","<<t03<<",   "<<t10<<","<<t11<<","<<t12<<","<<t13<<",   "<<  t20<<","<<t21<<","<<t22<<","<<t23<<",   "<<  t30<<","<<t31<<","<<t32<<","<<t33 <<");"<<std::endl;
	}
	void OutputESS::ei_mtl( const char *material_name )
	{
		//CM_TRACE_FUNC("OutputESS::ei_mtl(\""<<material_name<<"\")");
		m_outputfile<<"ei_mtl(\""<<material_name<<"\");"<<std::endl;
	}

	void OutputESS::ei_end_instance()
	{
		//CM_TRACE_FUNC("OutputESS::ei_end_instance()");
		m_outputfile<<"ei_end_instance();"<<std::endl;
	}


	//	Instance Groups:
	void OutputESS::ei_instgroup( const char *name )
	{
		//CM_TRACE_FUNC("OutputESS::ei_instgroup(\""<<name<<"\")");
		m_outputfile<<"ei_instgroup(\""<<name<<"\");"<<std::endl;
	}
	void OutputESS::ei_add_instance( const char *name )
	{
		//CM_TRACE_FUNC("OutputESS::ei_add_instance(\""<<name<<"\")");
		m_outputfile<<"ei_add_instance(\""<<name<<"\");"<<std::endl;
	}
	void OutputESS::ei_end_instgroup()
	{
		//CM_TRACE_FUNC("OutputESS::ei_end_instgroup( )");
		m_outputfile<<"ei_end_instgroup();"<<std::endl;
	}
	//
	void OutputESS::ei_shader(const char *type_name, const char *instance_name)
	{
		//CM_TRACE_FUNC("OutputESS::ei_shader(\""<<type_name<<"\", \""<<instance_name<<"\")");
		m_outputfile<<"ei_shader(\""<<type_name<<"\", \""<<instance_name<<"\");"<<std::endl;
	}
	void OutputESS::ei_shader_param(
		const char *param_name, 
		const void *param_value)
	{
		//CM_TRACE_FUNC("OutputESS::ei_shader_param(\""<<param_name<<"\", \""<<param_value<<"\") not implemented");
		liquidMessage2(messageError, "OutputESS::ei_shader_param() not implemented");
		m_outputfile<<"ei_shader_param(\""<<param_name<<"\", \""<<param_value<<"\");"<<std::endl;
	}
	void OutputESS::ei_shader_param_token(
		const char *param_name, 
		const char *param_value)
	{
		//CM_TRACE_FUNC("OutputESS::ei_shader_param_token(\""<<param_name<<"\", \""<<param_value<<"\") not implemented");
		liquidMessage2(messageError, "OutputESS::ei_shader_param() not implemented");
		m_outputfile<<"ei_shader_param_token(\""<<param_name<<"\", \""<<param_value<<"\");"<<std::endl;
	}
	void OutputESS::ei_shader_param_int(
		const char *param_name, 
		const eiInt param_value)
	{
		//CM_TRACE_FUNC("OutputESS::ei_shader_param_int(\""<<param_name<<"\","<<param_value<<")");
		m_outputfile<<"ei_shader_param_int(\""<<param_name<<"\","<<param_value<<");"<<std::endl;
	}
	void OutputESS::ei_shader_param_scalar(
		const char *param_name, 
		const eiScalar param_value)
	{
		//CM_TRACE_FUNC("OutputESS::ei_shader_param_scalar(\""<<param_name<<"\","<<param_value<<")");
		m_outputfile<<"ei_shader_param_scalar(\""<<param_name<<"\","<<param_value<<");"<<std::endl;
	}
	void OutputESS::ei_shader_param_vector(
		const char *param_name, 
		const eiScalar x, const eiScalar y, const eiScalar z)
	{
		//CM_TRACE_FUNC("OutputESS::ei_shader_param_vector(\""<<param_name<<"\","<<x<<","<<y<<","<<z<<")");
		m_outputfile<<"ei_shader_param_vector(\""<<param_name<<"\","<<x<<","<<y<<","<<z<<");"<<std::endl;
	}
	void OutputESS::ei_shader_param_vector4(
		const char *param_name, 
		const eiScalar x, const eiScalar y, const eiScalar z, const eiScalar w)
	{
		//CM_TRACE_FUNC("OutputESS::ei_shader_param_vector4(\""<<param_name<<"\","<<x<<","<<y<<","<<z<<","<<w<<")");
		m_outputfile<<"ei_shader_param_vector4(\""<<param_name<<"\","<<x<<","<<y<<","<<z<<","<<w<<");"<<std::endl;
	}
	void OutputESS::ei_shader_param_tag(
		const char *param_name, 
		const eiTag param_value)
	{
		//CM_TRACE_FUNC("OutputESS::ei_shader_param_tag(\""<<param_name<<"\","<<param_value<<")");
		m_outputfile<<"ei_shader_param_tag(\""<<param_name<<"\","<<param_value<<");"<<std::endl;
	}
	void OutputESS::ei_shader_param_texture(
		const char *param_name, 
		const char *texture_name)
	{
		//CM_TRACE_FUNC("OutputESS::ei_shader_param_texture(\""<<param_name<<"\",\""<<texture_name<<"\")");
		m_outputfile<<"ei_shader_param_texture(\""<<param_name<<"\",\""<<texture_name<<"\");"<<std::endl;
	}
	void OutputESS::ei_shader_param_index(
		const char *param_name, 
		const eiIndex param_value)
	{
		//CM_TRACE_FUNC("OutputESS::ei_shader_param_index(\""<<param_name<<"\","<<param_value<<")");
		m_outputfile<<"ei_shader_param_index(\""<<param_name<<"\","<<param_value<<");"<<std::endl;
	}
	void OutputESS::ei_shader_param_bool(
		const char *param_name, 
		const eiBool param_value)
	{
		//CM_TRACE_FUNC("OutputESS::ei_shader_param_bool(\""<<param_name<<"\","<<param_value<<")");
		m_outputfile<<"ei_shader_param_bool(\""<<param_name<<"\","<<param_value<<");"<<std::endl;
	}
	void OutputESS::ei_shader_link_param(
		const char *param_name, 
		const char *src_shader_name, 
		const char *src_param_name)
	{
		//CM_TRACE_FUNC("OutputESS::ei_shader_link_param(\""<<param_name<<"\",\""<<src_shader_name<<"\",\""<<src_param_name<<"\")");
		m_outputfile<<"ei_shader_link_param(\""<<param_name<<"\",\""<<src_shader_name<<"\",\""<<src_param_name<<"\");"<<std::endl;
	}
	void OutputESS::ei_end_shader()
	{
		//CM_TRACE_FUNC("OutputESS::ei_end_shader()");
		m_outputfile<<"ei_end_shader();"<<std::endl;
	}

	void OutputESS::ei_declare(const char *name, const eiInt storage_class, const eiInt type/*, const void *tag*/)
	{
		//CM_TRACE_FUNC("OutputESS::ei_declare(\""<<name<<"\","<<storage_class<<","<<type<<", &tag)");
		liquidMessage2(messageError, "OutputESS::ei_declare(&tag)");
		m_outputfile<<"ei_declare(\""<<name<<"\","<<storage_class<<","<<type<<", &tag);"<<std::endl;
	}
	void OutputESS::ei_variable(const char *name, const void *tag)
	{
		//CM_TRACE_FUNC("OutputESS::ei_variable(\""<<name<<"\", &tag)");
		liquidMessage2(messageError, "OutputESS::ei_declare(&tag)");
		m_outputfile<<"ei_variable(\""<<name<<"\", &tag);"<<std::endl;
	}
	void OutputESS::ei_degree(const eiInt degree)
	{
		//CM_TRACE_FUNC("OutputESS::ei_degree("<<degree<<")");
		m_outputfile<<"ei_degree("<<degree<<");"<<std::endl;
	}

	//----------------------------------------------------
	// Warped ER API interfaces
	//----------------------------------------------------
	void OutputESS::liq_lightgroup_in_object_instance(const char *light_group_name)
	{
		//CM_TRACE_FUNC("OutputESS::liq_lightgroup_in_object_instance("<<light_group_name<<")");

	}
	void OutputESS::liq_lightgroup_in_light_instance_beg()
	{
		//CM_TRACE_FUNC("OutputESS::liq_lightgroup_in_light_instance_beg()");

	}
	void OutputESS::liq_lightgroup_in_light_instance(const char *light_group_name)
	{
		//CM_TRACE_FUNC("OutputESS::liq_lightgroup_in_light_instance("<<light_group_name<<")");

	}
	void OutputESS::liq_lightgroup_in_light_instance_end()
	{
		//CM_TRACE_FUNC("OutputESS::liq_lightgroup_in_light_instance_end()");

	}
	void OutputESS::liq_object(
		const std::string &objname,
		const std::vector<MVector> &POSITION,
		const std::vector<MVector> &POSITION_mb,//motion blur position
		const std::vector<std::size_t> &INDEX,//global vertex index
		const std::vector<MVector> &NORMAL,
		const std::vector<MVector> &UV
		)
	{
		//CM_TRACE_FUNC("OutputESS::liq_object("<<light_group_name<<")");



	}
}//namespace elvishray
#endif//_USE_ELVISHRAY_