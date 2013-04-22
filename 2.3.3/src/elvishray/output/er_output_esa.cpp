#include <liqConfig.h>
#ifdef _USE_ELVISHRAY_
#include "er_output_esa.h"
#include <liqlog.h>
#include <trace/trace.hpp>
#include "../../common/mayacheck.h"

namespace elvishray
{
	OutputESA::OutputESA(const std::string &filefullpath)
		:OutputBase()
	{
		CM_TRACE_FUNC("OutputESA::OutputESA("<<filefullpath<<")");

		const std::string logFileName(filefullpath+".esa");
		m_outputfile.open(logFileName.c_str(), std::ios_base::out);
		if( !m_outputfile.is_open() )
		{
			liquidMessage2(messageError,"can't open file: %s.\n", logFileName.c_str() );
		}
	}
	OutputESA::~OutputESA()
	{
		CM_TRACE_FUNC("OutputESA::~OutputESA()");

		m_outputfile.close();
	}
	void OutputESA::init()
	{
		CM_TRACE_FUNC("OutputESA::init()");
		a("------------------------------------------------");
		m_outputfile<<"char *null_token = NULL;"<<std::endl;
		m_outputfile<<"eiInt defaultInt = 0;"<<std::endl;
		a("------------------------------------------------");
		ln();
	}
	//----------------------------------------------------
	// ER API interfaces
	//----------------------------------------------------
	void OutputESA::ln()
	{
		//CM_TRACE_FUNC("OutputCall::ln()");
		m_outputfile<<std::endl;
	}
	void OutputESA::s(const std::string &msg)
	{
		//CM_TRACE_FUNC("OutputESA::s("<<msg<<")");
		m_outputfile<<msg<<std::endl;
	}
	void OutputESA::a(const std::string &msg)
	{
		//CM_TRACE_FUNC("OutputESA::a("<<msg<<")");
		m_outputfile<<"//"<<msg<<std::endl;
	}
	void OutputESA::ei_context()
	{
		//CM_TRACE_FUNC("OutputESA::ei_context()");
		m_outputfile<<"ei_context();"<<std::endl;
	}
	void OutputESA::ei_end_context()
	{
		//CM_TRACE_FUNC("OutputESA::ei_end_context()");
		m_outputfile<<"ei_end_context();"<<std::endl;
	}
	//	Client application connection:
	void OutputESA::ei_connection( eiConnection *con )
	{
		//CM_TRACE_FUNC("OutputESA::ei_connection(...)");
		m_outputfile<<"//ei_connection(con);"<<std::endl;
	}
	//	Shader Linking:
	void OutputESA::ei_link( const char *filename )
	{
		//CM_TRACE_FUNC("OutputESA::ei_link("<<filename<<")");
		m_outputfile<<"ei_link(\""<<filename<<"\");"<<std::endl;
	}
	//	Commands:
	void OutputESA::ei_verbose( int level )
	{
		//CM_TRACE_FUNC("OutputESA::ei_verbose("<<level<<")");
		m_outputfile<<"ei_verbose("<<level<<");"<<std::endl;
	}
	void OutputESA::ei_delete( const char *element_name )
	{
		//CM_TRACE_FUNC("OutputESA::ei_delete( \""<<element_name<<"\" )");
		m_outputfile<<"ei_delete(\""<<element_name<<"\");"<<std::endl;
	}
	void OutputESA::ei_render( const char *root_instgroup, const char *camera_inst, const char *options )
	{
		//CM_TRACE_FUNC("OutputESA::ei_render(\""<<root_instgroup<<"\", \""<<camera_inst<<"\", \""<<options<<"\")");
		m_outputfile<<"ei_render( \""<<root_instgroup<<"\", \""<<camera_inst<<"\", \""<<options<<"\" );"<<std::endl;
	}

	eiTag OutputESA::ei_tab(const eiInt type, const eiInt items_per_slot)
	{
		//CM_TRACE_FUNC("OutputESA::ei_tab("<<type<<","<<items_per_slot<<")");
		m_outputfile<<"ei_tab( "<<type<<", "<<items_per_slot<<" );"<<std::endl;
		liquidMessage2(messageError, "OutputESA::ei_tab(), return 0;");
		return 0;
	}
	//void OutputESA::ei_tab_add(const void *value);
	//void OutputESA::ei_tab_add_int(const eiInt value);
	void OutputESA::ei_tab_add_scalar(const eiScalar value)
	{
		//CM_TRACE_FUNC("OutputESA::ei_tab_add_scalar("<<value<<")");
		m_outputfile<<"ei_tab_add_scalar("<<value<<");"<<std::endl;
	}
	void OutputESA::ei_tab_add_vector(const eiScalar x, const eiScalar y, const eiScalar z)
	{
		//CM_TRACE_FUNC("OutputESA::ei_tab_add_vector("<<x<<","<<y<<","<<z<<")");
		m_outputfile<<"ei_tab_add_vector("<<x<<","<<y<<","<<z<<");"<<std::endl;
	}
	void OutputESA::ei_tab_add_vector2(const eiScalar x, const eiScalar y)
	{
		//CM_TRACE_FUNC("OutputESA::ei_tab_add_vector2("<<x<<","<<y<<")");
		m_outputfile<<"ei_tab_add_vector2("<<x<<","<<y<<");"<<std::endl;
	}
	void OutputESA::ei_tab_add_index(const eiIndex value)
	{
		//CM_TRACE_FUNC("OutputESA::ei_tab_add_index("<<value<<")");
		m_outputfile<<"ei_tab_add_index("<<value<<");"<<std::endl;
	}
	//void OutputESA::ei_tab_add_bool(const eiBool value);
	void OutputESA::ei_end_tab()
	{
		//CM_TRACE_FUNC("OutputESA::ei_end_tab()");
		m_outputfile<<"ei_end_tab();"<<std::endl;
	}


	//	Options:
	void OutputESA::ei_options( const char *name )
	{
		//CM_TRACE_FUNC("OutputESA::ei_options(\""<<name<<"\")");
		m_outputfile<<"ei_options(\""<<name<<"\");"<<std::endl;
	}

	//	Sampling Quality:
	void OutputESA::ei_contrast( eiScalar value )
	{
		//CM_TRACE_FUNC("OutputESA::ei_contrast("<<value<<")");
		m_outputfile<<"ei_contrast("<<value<<");"<<std::endl;
	}
	void OutputESA::ei_samples( int _min, int _max )
	{
		//CM_TRACE_FUNC("OutputESA::ei_samples("<<_min<<","<<_max<<")");
		m_outputfile<<"ei_samples("<<_min<<","<<_max<<");"<<std::endl;
	}
	void OutputESA::ei_bucket_size( int size )
	{
		//CM_TRACE_FUNC("OutputESA::ei_bucket_size("<<size<<")");
		m_outputfile<<"ei_bucket_size("<<size<<");"<<std::endl;
	}
	void OutputESA::ei_filter( int filter, float size )
	{
		//CM_TRACE_FUNC("OutputESA::ei_filter("<<filter<<","<<size<<")");
		m_outputfile<<"ei_filter("<<filter<<","<<size<<");"<<std::endl;
	}
	//	Motion Blur:
	void OutputESA::ei_shutter( float open, float close )
	{
		//CM_TRACE_FUNC("OutputESA::ei_shutter("<<open<<", "<<close<<")");
		m_outputfile<<"ei_shutter("<<open<<", "<<close<<");"<<std::endl;
	}
	void OutputESA::ei_motion( int type )
	{
		//CM_TRACE_FUNC("OutputESA::ei_motion("<<type<<")");
		m_outputfile<<"ei_motion("<<type<<");"<<std::endl;
	}

	//	Trace Depth:
	void OutputESA::ei_trace_depth(eiInt transp, 
		eiInt glossy_reflect, eiInt diffuse_reflect, 
		eiInt glossy_refract, eiInt diffuse_refract, eiInt sum)
	{
		//CM_TRACE_FUNC("OutputESA::ei_trace_depth(4,4,4,"<<reflect<<","<<refract<<","<<sum<<")");
		m_outputfile<<"ei_trace_depth("<<transp<<","
			<<glossy_reflect<<","<<diffuse_reflect<<","
			<<glossy_refract<<","<<diffuse_refract<<","<<sum<<");"<<std::endl;
	}
	//	Shadow:
	void OutputESA::ei_shadow( int type )
	{
		//CM_TRACE_FUNC("OutputESA::ei_shadow("<<type<<")");
		m_outputfile<<"ei_shadow("<<type<<");"<<std::endl;
	}
	//	Feature Disabling:
	void OutputESA::ei_lens(eiInt type)
	{
		//CM_TRACE_FUNC("OutputESA::ei_lens("<<type<<" )");
		m_outputfile<<"ei_lens("<<type<<");"<<std::endl;
	}
	void OutputESA::ei_volume( int type )
	{
		//CM_TRACE_FUNC("OutputESA::ei_volume("<<type<<")");
		m_outputfile<<"ei_volume("<<type<<");"<<std::endl;
	}
	void OutputESA::ei_geometry( int type )
	{
		//CM_TRACE_FUNC("OutputESA::ei_geometry("<<type<<")");
		m_outputfile<<"ei_geometry("<<type<<");"<<std::endl;
	}
	void OutputESA::ei_displace( int type )
	{
		//CM_TRACE_FUNC("OutputESA::ei_displace("<<type<<")");
		m_outputfile<<"ei_displace("<<type<<");"<<std::endl;
	}
	void OutputESA::ei_max_displace(eiScalar dist)
	{
		//CM_TRACE_FUNC("OutputESA::ei_max_displace("<<dist<<")");
		m_outputfile<<"ei_max_displace("<<dist<<");"<<std::endl;
	}
	void OutputESA::ei_imager( int type )
	{
		//CM_TRACE_FUNC("OutputESA::ei_imager("<<type<<")");
		m_outputfile<<"ei_imager("<<type<<");"<<std::endl;
	}
	//	Frame Buffer Control:
	void OutputESA::ei_exposure( float gain, float gamma )
	{
		//CM_TRACE_FUNC("OutputESA::ei_exposure("<<gain<<","<<gamma<<")");
		m_outputfile<<"ei_exposure("<<gain<<","<<gamma<<");"<<std::endl;
	}
	void OutputESA::ei_quantize( float one, float _min, float _max, float dither_amplitude )
	{
		//CM_TRACE_FUNC("OutputESA::ei_quantize("<<one<<","<<_min<<","<<_max<<","<<dither_amplitude<<")");
		m_outputfile<<"ei_quantize("<<one<<","<<_min<<","<<_max<<","<<dither_amplitude<<");"<<std::endl;
	}
	//	Miscellaneous:
	void OutputESA::ei_face( int type )
	{
		//CM_TRACE_FUNC("OutputESA::ei_face("<<type<<")");
		m_outputfile<<"ei_face("<<type<<");"<<std::endl;
	}
	void OutputESA::ei_approx(const eiApprox *approx)
	{
		//CM_TRACE_FUNC("OutputESA::ei_approx(approx)");
		s("eiApprox	approx;");
		s("ei_approx_set_defaults(&approx);");

		char buf_any[16],buf_view_dep[16];
		sprintf_s(buf_any,      16, "%x", approx->any);
		sprintf_s(buf_view_dep, 16, "%x", approx->view_dep);

		m_outputfile<<"approx.method        = "<<approx->method			<<";"<<std::endl;
		m_outputfile<<"approx.any           = "<<buf_any				<<";"<<std::endl;
		m_outputfile<<"approx.view_dep      = "<<buf_view_dep			<<";"<<std::endl;
		m_outputfile<<"approx.args[0]       = "<<approx->args[0]		<<";"<<std::endl;
		m_outputfile<<"approx.args[1]       = "<<approx->args[1]		<<";"<<std::endl;
		m_outputfile<<"approx.args[2]       = "<<approx->args[2]		<<";"<<std::endl;
		m_outputfile<<"approx.args[3]       = "<<approx->args[3]		<<";"<<std::endl;
		m_outputfile<<"approx.sharp         = "<<approx->sharp			<<";"<<std::endl;
		m_outputfile<<"approx.min_subdiv    = "<<approx->min_subdiv		<<";"<<std::endl;
		m_outputfile<<"approx.max_subdiv    = "<<approx->max_subdiv		<<";"<<std::endl;
		m_outputfile<<"approx.max_grid_size = "<<approx->max_grid_size	<<";"<<std::endl;
		m_outputfile<<"approx.motion_factor = "<<approx->motion_factor	<<";"<<std::endl;
	
		s("ei_approx(&approx);");
	}
	void OutputESA::ei_end_options()
	{
		//CM_TRACE_FUNC("OutputESA::ei_end_options()");
		m_outputfile<<"ei_end_options();"<<std::endl;
	}
	//	Cameras:
	void OutputESA::ei_camera( const char *name )
	{
		//CM_TRACE_FUNC("OutputESA::ei_camera(\""<< name <<"\")");
		m_outputfile<<"ei_camera(\""<<name<<"\");"<<std::endl;
	}
	//	Output Statements:
	void OutputESA::ei_output( const char *filename, const char *fileformat, const eiInt datatype)
	{
		//CM_TRACE_FUNC("OutputESA::ei_output(\""<< filename <<"\",\""<<fileformat<<"\","<<datatype<<")");
		m_outputfile<<"ei_output(\""<< filename <<"\",\""<<fileformat<<"\","<<datatype<<");"<<std::endl;
	}
	void OutputESA::ei_output_variable(const char *name, const eiInt datatype)
	{
		//CM_TRACE_FUNC("OutputESA::ei_output_variable(\""<< name <<"\","<<datatype<<")");
		m_outputfile<<"ei_output_variable(\""<< name <<"\","<<datatype<<");"<<std::endl;
	}
	void OutputESA::ei_end_output()
	{
		//CM_TRACE_FUNC("OutputESA::ei_end_output()");
		m_outputfile<<"ei_end_output();"<<std::endl;
	}
	//	Other Camera Statements:
	void OutputESA::ei_focal( float distance )
	{
		//CM_TRACE_FUNC("OutputESA::ei_focal("<< distance <<")");
		m_outputfile<<"ei_focal("<< distance <<");"<<std::endl;
	}
	void OutputESA::ei_aperture( float aperture )
	{
		//CM_TRACE_FUNC("OutputESA::ei_aperture("<< aperture <<")");
		m_outputfile<<"ei_aperture("<< aperture <<");"<<std::endl;
	}
	void OutputESA::ei_aspect( float aspect )
	{
		//CM_TRACE_FUNC("OutputESA::ei_aspect("<< aspect <<")");
		m_outputfile<<"ei_aspect("<< aspect <<");"<<std::endl;
	}
	void OutputESA::ei_resolution( eiInt x, eiInt y )
	{
		//CM_TRACE_FUNC("OutputESA::ei_resolution("<< x<<","<<y <<")");
		m_outputfile<<"ei_resolution("<< x<<","<<y <<");"<<std::endl;
	}
	void OutputESA::ei_window( eiInt xmin, eiInt xmax, eiInt ymin, eiInt ymax )
	{
		//CM_TRACE_FUNC("OutputESA::ei_window("<< xmin<<","<<xmax<<","<<ymin<<","<<ymax<<")");
		m_outputfile<<"ei_window("<< xmin<<","<<xmax<<","<<ymin<<","<<ymax<<");"<<std::endl;
	}
	void OutputESA::ei_clip( float hither, float yon )
	{
		//CM_TRACE_FUNC("OutputESA::ei_clip("<<hither<<","<<yon<<")");
		m_outputfile<<"ei_clip("<<hither<<","<<yon<<");"<<std::endl;
	}
	void OutputESA::ei_lens_shader(const char *shader_name)
	{
		//CM_TRACE_FUNC("OutputESA::ei_lens_shader(\""<<shader_name<<"\")");
		m_outputfile<<"ei_lens_shader(\""<<shader_name<<"\");"<<std::endl;
	}
	void OutputESA::ei_imager_shader(const char *shader_name)
	{
		//CM_TRACE_FUNC("OutputESA::ei_imager_shader(\""<<shader_name<<"\")");
		m_outputfile<<"ei_imager_shader(\""<<shader_name<<"\");"<<std::endl;
	}
	void OutputESA::ei_end_camera()
	{
		//CM_TRACE_FUNC("OutputESA::ei_end_camera()");
		m_outputfile<<"ei_end_camera();"<<std::endl;
	}
	//	Textures:
	void OutputESA::ei_make_texture( const char *picturename, const char *texturename, 
		int swrap, int twrap, int filter, float swidth, float twidth )
	{
		//CM_TRACE_FUNC("OutputESA::ei_make_texture(\""<<picturename<<"\", \""<< texturename<<"\","<<swrap<<","<< twrap<<","<<filter<<", "<<swidth<<", "<<twidth<<")");
		m_outputfile<<"ei_make_texture(\""<<picturename<<"\", \""<< texturename<<"\","<<swrap<<","<< twrap<<","<<filter<<", "<<swidth<<", "<<twidth<<");"<<std::endl;
	}
	void OutputESA::ei_texture(const char *name)
	{
		//CM_TRACE_FUNC("OutputESA::ei_texture("<<name<<")");
		m_outputfile<<"ei_texture(\""<<name<<"\");"<<std::endl;
	}
	void OutputESA::ei_file_texture(const char *filename, const eiBool local)
	{
		//CM_TRACE_FUNC("OutputESA::ei_file_texture("<<filename<<","<<local<<")");
		m_outputfile<<"ei_file_texture(\""<<filename<<"\", "<<local<<");"<<std::endl;
	}
	void OutputESA::ei_end_texture()
	{
		//CM_TRACE_FUNC("OutputESA::ei_end_texture()");
		m_outputfile<<"ei_end_texture();"<<std::endl;
	}
	//	Materials:
	void OutputESA::ei_material( const char *name )
	{
		//CM_TRACE_FUNC("OutputESA::ei_material(\""<<name<<"\")");
		m_outputfile<<"ei_material(\""<<name<<"\");"<<std::endl;
	}
	void OutputESA::ei_surface_shader( const char *shader_name )
	{
		//CM_TRACE_FUNC("OutputESA::ei_surface_shader(\""<<shader_name <<"\")");
		m_outputfile<<"ei_surface_shader(\""<<shader_name <<"\");"<<std::endl;
	}
	void OutputESA::ei_displace_shader( const char *shader_name )
	{
		//CM_TRACE_FUNC("OutputESA::ei_displace_shader(\""<<shader_name <<"\")");
		m_outputfile<<"ei_displace_shader(\""<<shader_name <<"\" );"<<std::endl;
	}
	void OutputESA::ei_shadow_shader( const char *shader_name )
	{
		//CM_TRACE_FUNC("OutputESA::ei_shadow_shader(\""<<shader_name <<"\")");
		m_outputfile<<"ei_shadow_shader(\""<<shader_name <<"\" );"<<std::endl;
	}
	void OutputESA::ei_volume_shader( const char *shader_name )
	{
		//CM_TRACE_FUNC("OutputESA::ei_volume_shader(\""<<shader_name <<"\")");
		m_outputfile<<"ei_volume_shader(\""<<shader_name <<"\" );"<<std::endl;
	}
	void OutputESA::ei_environment_shader( const char *shader_name )
	{
		//CM_TRACE_FUNC("OutputESA::ei_environment_shader(\""<<shader_name <<"\")");
		m_outputfile<<"ei_environment_shader(\""<<shader_name <<"\");"<<std::endl;
	}

	void OutputESA::ei_end_material()
	{
		//CM_TRACE_FUNC("OutputESA::ei_end_material()");
		m_outputfile<<"ei_end_material();"<<std::endl;
	}
	//	Lights:
	void OutputESA::ei_light( const char *name )
	{
		//CM_TRACE_FUNC("OutputESA::ei_light(\""<<name<<"\")");
		m_outputfile<<"ei_light(\""<<name<<"\");"<<std::endl;
	}
	void OutputESA::ei_light_shader( const char *shader_name )
	{
		//CM_TRACE_FUNC("OutputESA::ei_light_shader(\""<<shader_name<<"\" )");
		m_outputfile<<"ei_light_shader(\""<<shader_name<<"\");"<<std::endl;
	}
	void OutputESA::ei_origin(eiScalar x, eiScalar y, eiScalar z)
	{
		//CM_TRACE_FUNC("OutputESA::ei_origin("<<x<<","<<y<<","<<z<<")");
		m_outputfile<<"ei_origin("<<x<<","<<y<<","<<z<<");"<<std::endl;
	}
	void OutputESA::ei_area_samples( eiInt samples )
	{
		//CM_TRACE_FUNC("OutputESA::ei_area_samples("<<samples<<" )");
		m_outputfile<<"ei_area_samples("<<samples<<");"<<std::endl;
	}

	void OutputESA::ei_end_light()
	{
		//CM_TRACE_FUNC("OutputESA::ei_end_light()");
		m_outputfile<<"ei_end_light();"<<std::endl;
	}
	//	Objects:
	void OutputESA::ei_object( const char *type, const char *name )
	{
		//CM_TRACE_FUNC("OutputESA::ei_object(\""<< type <<"\",\""<<name<<"\")" ); 
		m_outputfile<<"ei_object(\""<< type <<"\",\""<<name<<"\");"<<std::endl;
	}
	void OutputESA::ei_box(eiScalar xmin, eiScalar ymin, eiScalar zmin, 
		eiScalar xmax, eiScalar ymax, eiScalar zmax)
	{
		//CM_TRACE_FUNC("OutputESA::ei_box("<<xmin<<","<<ymin<<","<<zmin<<", "<<xmax<<","<<ymax<<","<<zmax<<")");
		m_outputfile<<"ei_box("<<xmin<<","<<ymin<<","<<zmin<<", "<<xmax<<","<<ymax<<","<<zmax<<");"<<std::endl;

	}
	void OutputESA::ei_motion_box(eiScalar xmin, eiScalar ymin, eiScalar zmin, 
		eiScalar xmax, eiScalar ymax, eiScalar zmax)
	{
		//CM_TRACE_FUNC("OutputESA::ei_motion_box("<< xmin<<","<<ymin<<","<<zmin<<", "<<xmax<<","<<ymax<<","<<zmax<<")");
		m_outputfile<<"ei_motion_box("<< xmin<<","<<ymin<<","<<zmin<<", "<<xmax<<","<<ymax<<","<<zmax<<");"<<std::endl;
	}
	/* procedural objects */
	void OutputESA::ei_geometry_shader(const char *shader_name)
	{
		//CM_TRACE_FUNC("OutputESA::ei_geometry_shader(\""<<shader_name<<"\")");
		m_outputfile<<"ei_geometry_shader(\""<<shader_name<<"\");"<<std::endl;
	}
	void OutputESA::ei_pos_list(const eiTag tab)
	{
		//CM_TRACE_FUNC("OutputESA::ei_pos_list(tag)" );
		liquidMessage2(messageError,"OutputESA::ei_pos_list(tab)");
		m_outputfile<<"ei_pos_list(tab);"<<std::endl;
	}
	void OutputESA::ei_motion_pos_list(const eiTag tab)
	{
		//CM_TRACE_FUNC("OutputESA::ei_motion_pos_list(tag)" ); 
		liquidMessage2(messageError,"OutputESA::ei_motion_pos_list(tab)");
		m_outputfile<<"ei_motion_pos_list(tab);"<<std::endl;
	}
	void OutputESA::ei_triangle_list(const eiTag tab)
	{
		//CM_TRACE_FUNC("OutputESA::ei_triangle_list(tag)" );
		liquidMessage2(messageError,"OutputESA::ei_triangle_list(tab)");
		m_outputfile<<"ei_triangle_list(tab);"<<std::endl;
	}
	void OutputESA::ei_end_object()
	{
		//CM_TRACE_FUNC("OutputESA::ei_end_object()"); 
		m_outputfile<<"ei_end_object();"<<std::endl;
	}


	//	Instances:
	void OutputESA::ei_instance( const char *name )
	{
		//CM_TRACE_FUNC("OutputESA::ei_instance(\""<<name<<"\")");
		m_outputfile<<"ei_instance(\""<<name<<"\");"<<std::endl;
	}
	void OutputESA::ei_element( const char *element_name )
	{
		//CM_TRACE_FUNC("OutputESA::ei_element(\""<<element_name<<"\")");
		m_outputfile<<"ei_element(\""<<element_name<<"\");"<<std::endl;
	}
	void OutputESA::ei_transform( 
		float t00,float t01,float t02,float t03,
		float t10,float t11,float t12,float t13,
		float t20,float t21,float t22,float t23,
		float t30,float t31,float t32,float t33 
		)
	{
		//CM_TRACE_FUNC("OutputESA::ei_transform("<< t00<<","<<t01<<","<<t02<<","<<t03<<",   "<<t10<<","<<t11<<","<<t12<<","<<t13<<",   "<<  t20<<","<<t21<<","<<t22<<","<<t23<<",   "<<  t30<<","<<t31<<","<<t32<<","<<t33 <<")");
		m_outputfile<<"ei_transform("<< t00<<","<<t01<<","<<t02<<","<<t03<<",   "<<t10<<","<<t11<<","<<t12<<","<<t13<<",   "<<  t20<<","<<t21<<","<<t22<<","<<t23<<",   "<<  t30<<","<<t31<<","<<t32<<","<<t33 <<");"<<std::endl;
	}
	void OutputESA::ei_motion_transform( 
		float t00,float t01,float t02,float t03,
		float t10,float t11,float t12,float t13,
		float t20,float t21,float t22,float t23,
		float t30,float t31,float t32,float t33
		)
	{
		//CM_TRACE_FUNC("OutputESA::ei_motion_transform("<< t00<<","<<t01<<","<<t02<<","<<t03<<",   "<<t10<<","<<t11<<","<<t12<<","<<t13<<",   "<<  t20<<","<<t21<<","<<t22<<","<<t23<<",   "<<  t30<<","<<t31<<","<<t32<<","<<t33 <<")");
		m_outputfile<<"ei_motion_transform("<< t00<<","<<t01<<","<<t02<<","<<t03<<",   "<<t10<<","<<t11<<","<<t12<<","<<t13<<",   "<<  t20<<","<<t21<<","<<t22<<","<<t23<<",   "<<  t30<<","<<t31<<","<<t32<<","<<t33 <<");"<<std::endl;
	}
	void OutputESA::ei_mtl( const char *material_name )
	{
		//CM_TRACE_FUNC("OutputESA::ei_mtl(\""<<material_name<<"\")");
		m_outputfile<<"ei_mtl(\""<<material_name<<"\");"<<std::endl;
	}

	void OutputESA::ei_end_instance()
	{
		//CM_TRACE_FUNC("OutputESA::ei_end_instance()");
		m_outputfile<<"ei_end_instance();"<<std::endl;
	}


	//	Instance Groups:
	void OutputESA::ei_instgroup( const char *name )
	{
		//CM_TRACE_FUNC("OutputESA::ei_instgroup(\""<<name<<"\")");
		m_outputfile<<"ei_instgroup(\""<<name<<"\");"<<std::endl;
	}
	void OutputESA::ei_add_instance( const char *name )
	{
		//CM_TRACE_FUNC("OutputESA::ei_add_instance(\""<<name<<"\")");
		m_outputfile<<"ei_add_instance(\""<<name<<"\");"<<std::endl;
	}
	void OutputESA::ei_end_instgroup()
	{
		//CM_TRACE_FUNC("OutputESA::ei_end_instgroup( )");
		m_outputfile<<"ei_end_instgroup();"<<std::endl;
	}
	//
	void OutputESA::ei_shader(const char *type_name, const char *instance_name)
	{
		//CM_TRACE_FUNC("OutputESA::ei_shader(\""<<type_name<<"\", \""<<instance_name<<"\")");
		m_outputfile<<"ei_shader(\""<<type_name<<"\", \""<<instance_name<<"\");"<<std::endl;
	}
	void OutputESA::ei_shader_param(
		const char *param_name, 
		const void *param_value)
	{
		//CM_TRACE_FUNC("OutputESA::ei_shader_param(\""<<param_name<<"\", \""<<param_value<<"\") not implemented");
		liquidMessage2(messageError, "OutputESA::ei_shader_param() not implemented");
		a("error: ei_shader_param(): OutputESA::ei_shader_param() not implemented");
		m_outputfile<<"ei_shader_param(\""<<param_name<<"\", \""<<param_value<<"\");"<<std::endl;
	}
	void OutputESA::ei_shader_param_token(
		const char *param_name, 
		const char *param_value)
	{
		//CM_TRACE_FUNC("OutputESA::ei_shader_param_token(\""<<param_name<<"\", \""<<param_value<<"\") not implemented");
		m_outputfile<<"ei_shader_param_token(\""<<param_name<<"\", ei_token(\""<<param_value<<"\") );"<<std::endl;
	}
	void OutputESA::ei_shader_param_int(
		const char *param_name, 
		const eiInt param_value)
	{
		//CM_TRACE_FUNC("OutputESA::ei_shader_param_int(\""<<param_name<<"\","<<param_value<<")");
		m_outputfile<<"ei_shader_param_int(\""<<param_name<<"\","<<param_value<<");"<<std::endl;
	}
	void OutputESA::ei_shader_param_scalar(
		const char *param_name, 
		const eiScalar param_value)
	{
		//CM_TRACE_FUNC("OutputESA::ei_shader_param_scalar(\""<<param_name<<"\","<<param_value<<")");
		m_outputfile<<"ei_shader_param_scalar(\""<<param_name<<"\","<<param_value<<");"<<std::endl;
	}
	void OutputESA::ei_shader_param_vector(
		const char *param_name, 
		const eiScalar x, const eiScalar y, const eiScalar z)
	{
		//CM_TRACE_FUNC("OutputESA::ei_shader_param_vector(\""<<param_name<<"\","<<x<<","<<y<<","<<z<<")");
		m_outputfile<<"ei_shader_param_vector(\""<<param_name<<"\","<<x<<","<<y<<","<<z<<");"<<std::endl;
	}
	void OutputESA::ei_shader_param_vector4(
		const char *param_name, 
		const eiScalar x, const eiScalar y, const eiScalar z, const eiScalar w)
	{
		//CM_TRACE_FUNC("OutputESA::ei_shader_param_vector4(\""<<param_name<<"\","<<x<<","<<y<<","<<z<<","<<w<<")");
		m_outputfile<<"ei_shader_param_vector4(\""<<param_name<<"\","<<x<<","<<y<<","<<z<<","<<w<<");"<<std::endl;
	}
	void OutputESA::ei_shader_param_tag(
		const char *param_name, 
		const eiTag param_value)
	{
		//CM_TRACE_FUNC("OutputESA::ei_shader_param_tag(\""<<param_name<<"\","<<param_value<<")");
		m_outputfile<<"ei_shader_param_tag(\""<<param_name<<"\","<<param_value<<");"<<std::endl;
	}
	void OutputESA::ei_shader_param_texture(
		const char *param_name, 
		const char *texture_name)
	{
		//CM_TRACE_FUNC("OutputESA::ei_shader_param_texture(\""<<param_name<<"\",\""<<texture_name<<"\")");
		m_outputfile<<"ei_shader_param_texture(\""<<param_name<<"\",\""<<texture_name<<"\");"<<std::endl;
	}
	void OutputESA::ei_shader_param_index(
		const char *param_name, 
		const eiIndex param_value)
	{
		//CM_TRACE_FUNC("OutputESA::ei_shader_param_index(\""<<param_name<<"\","<<param_value<<")");
		m_outputfile<<"ei_shader_param_index(\""<<param_name<<"\","<<param_value<<");"<<std::endl;
	}
	void OutputESA::ei_shader_param_bool(
		const char *param_name, 
		const eiBool param_value)
	{
		//CM_TRACE_FUNC("OutputESA::ei_shader_param_bool(\""<<param_name<<"\","<<param_value<<")");
		m_outputfile<<"ei_shader_param_bool(\""<<param_name<<"\","<<param_value<<");"<<std::endl;
	}
	void OutputESA::ei_shader_link_param(
		const char *param_name, 
		const char *src_shader_name, 
		const char *src_param_name)
	{
		//CM_TRACE_FUNC("OutputESA::ei_shader_link_param(\""<<param_name<<"\",\""<<src_shader_name<<"\",\""<<src_param_name<<"\")");
		m_outputfile<<"ei_shader_link_param(\""<<param_name<<"\",\""<<src_shader_name<<"\",\""<<src_param_name<<"\");"<<std::endl;
	}
	void OutputESA::ei_end_shader()
	{
		//CM_TRACE_FUNC("OutputESA::ei_end_shader()");
		m_outputfile<<"ei_end_shader();"<<std::endl;
	}

	void OutputESA::ei_declare(const char *name, const eiInt storage_class, const eiInt type/*, const void *tag*/)
	{
		//CM_TRACE_FUNC("OutputESA::ei_declare(\""<<name<<"\","<<storage_class<<","<<type<<", &tag)");
		liquidMessage2(messageError, "OutputESA::ei_declare(&tag)");
		m_outputfile<<"const char *tag = NULL"<<std::endl;
		m_outputfile<<"ei_declare(\""<<name<<"\","<<storage_class<<","<<type<<", &tag);"<<std::endl;
	}
	void OutputESA::ei_variable(const char *name, const void *tag)
	{
		//CM_TRACE_FUNC("OutputESA::ei_variable(\""<<name<<"\", &tag)");
		liquidMessage2(messageError, "OutputESA::ei_declare(&tag)");
		m_outputfile<<"ei_variable(\""<<name<<"\", &tag);"<<std::endl;
	}
	void OutputESA::ei_degree(const eiInt degree)
	{
		//CM_TRACE_FUNC("OutputESA::ei_degree("<<degree<<")");
		m_outputfile<<"ei_degree("<<degree<<");"<<std::endl;
	}

	//----------------------------------------------------
	// Warped ER API interfaces
	//----------------------------------------------------
	void OutputESA::liq_lightgroup_in_object_instance(const char *light_group_name)
	{
		//CM_TRACE_FUNC("OutputESA::liq_lightgroup_in_object_instance("<<light_group_name<<")");
		s("{//light group(light-link group)");
		s("const char *null_token = NULL;");
		s("ei_declare(\"lightgroup\", EI_CONSTANT, EI_TYPE_TOKEN, &null_token);");
		s("const char *lightgroup_token = ei_token(\""+std::string(light_group_name)+"\");");
		s("ei_variable(\"lightgroup\", &lightgroup_token );");
		s("}");
	}
	void OutputESA::liq_lightgroup_in_light_instance_beg()
	{
		//CM_TRACE_FUNC("OutputESA::liq_lightgroup_in_light_instance_beg()");
	}
	void OutputESA::liq_lightgroup_in_light_instance(const char *light_group_name)
	{
		//CM_TRACE_FUNC("OutputESA::liq_lightgroup_in_light_instance("<<light_group_name<<")");
		//s("{");
		s("ei_declare(\""+std::string(light_group_name)+"\", EI_CONSTANT, EI_TYPE_INT, &defaultInt);");
		//s("}");
	}
	void OutputESA::liq_lightgroup_in_light_instance_end()
	{
		//CM_TRACE_FUNC("OutputESA::liq_lightgroup_in_light_instance_end()");
	}
	void OutputESA::liq_object(
		const std::string &objname,
		const std::vector<MVector> &POSITION,
		const std::vector<MVector> &POSITION_mb,//motion blur position
		const std::vector<std::size_t> &INDEX,//global vertex index
		const std::vector<MVector> &NORMAL,
		const std::vector<MVector> &UV
		)
	{
		//CM_TRACE_FUNC("OutputESA::liq_object("<<light_group_name<<")");

		s("ei_object( \"poly\", \""+objname+"\" );" );
		s("{");
		s("eiTag tag;");

		a(boost::str(boost::format("vertex positions(required), size=%d")%POSITION.size()));
		s("tag = ei_tab(EI_TYPE_VECTOR, 1024);");
		s("ei_pos_list( tag );");
		for(std::size_t i=0; i<POSITION.size(); ++i)
		{
			s(boost::str(boost::format("ei_tab_add_vector( %f, %f, %f );")%POSITION[i].x %POSITION[i].y %POSITION[i].z ) );
		}
		s("ei_end_tab();");


		a("### vertex deform positions(optional)");
		if( POSITION_mb.size() > 0 )
		{
			s("tag = ei_tab(EI_TYPE_VECTOR, 1024);");
			s("ei_motion_pos_list( tag );");
			for(std::size_t i=0; i<POSITION_mb.size(); ++i)
			{
				s(boost::str(boost::format("ei_tab_add_vector( %f, %f, %f );")%POSITION_mb[i].x %POSITION_mb[i].y %POSITION_mb[i].z ) );
			}
			s("ei_end_tab();");
		}



		a("### N ###(optional)");
		if( NORMAL.size() >0 )
		{
			s("tag = eiNULL_TAG;");
			s("ei_declare(\"N\", EI_VARYING, EI_TYPE_TAG, &tag);");
			s("tag = ei_tab(EI_TYPE_VECTOR, 1024);");
			s("ei_variable(\"N\", &tag);");
			for(size_t i=0; i<NORMAL.size(); ++i)
			{
				s(boost::str(boost::format("ei_tab_add_vector( %f, %f, %f );")%NORMAL[i].x %NORMAL[i].y %NORMAL[i].z ) );
			}
			s("ei_end_tab();");
		}
		a("### N ### end");



		a("### UV (optional)");
		if( UV.size() >0 )
		{
			s("tag = eiNULL_TAG;");
			s("ei_declare(\"uv\", EI_VARYING, EI_TYPE_TAG, &tag);");
			s("tag = ei_tab(EI_TYPE_VECTOR2, 1024);");
			s("ei_variable(\"uv\", &tag);");
			for(size_t i = 0; i<UV.size(); ++i)
			{
				s(boost::str(boost::format("ei_tab_add_vector2( %f, %f );")%UV[i].x %UV[i].y ) );
			}
			s("ei_end_tab();");
		}


		a(boost::str(boost::format("### triangles(required) size=%d") %INDEX.size()));
		s("tag = ei_tab(EI_TYPE_INDEX, 1024);");
		s("ei_triangle_list( tag );");		
		if( INDEX.size()>0 )
		{
			for(size_t i=0; i<INDEX.size(); ++i)
			{
				s(boost::str(boost::format("ei_tab_add_index(%d);")%INDEX[i]));
			}
		}
		s("ei_end_tab();");
		s("}//"+objname);
		s("ei_end_object();");
	}
	void OutputESA::liq_hair(
		const std::string &objname,
		MFnPfxGeometry &fnPfxGeometry,
		int degree,
		unsigned setOn)
	{
		//CM_TRACE_FUNC("OutputESA::liq_hair(objname)");
		MStatus status;

		s("ei_object( \"hair\", \""+objname+"\" );");
		s("{");
		s(boost::str(boost::format("ei_degree(%d);")%degree));

		bool uniformWidth[3] = { false, false, false };

		MRenderLineArray lines[ 3 ];//main/leaf/flower lines

		bool doLines          = true;
		bool doTwist          = true;
		bool doWidth          = true;
		bool doFlatness       = false;
		bool doParameter      = false;
		bool doColor          = true;
		bool doIncandescence  = false;
		bool doTransparency   = true;
		bool doWorldSpace     = false;

		IfMErrorWarn(fnPfxGeometry.getLineData( lines[ 0 ], lines[ 1 ], lines[ 2 ],
			true,  // lines
			true,  // twist
			true,  // width
			true,  // flatness
			false, // parameter
			true,  // color
			true,  // incandescence
			true,  // transparency
			false  // worldSpace
			)
			);
		// default: MRT_PfxTube


		unsigned totalVertex( 0 );
		unsigned totalVarying( 0 );
		unsigned numLines( lines[ setOn ].length() );

		for( unsigned lineOn( 0 ); lineOn < numLines; lineOn++ )
		{
			MRenderLine pfxLine( lines[ setOn ].renderLine( lineOn, &status ) );
			MVectorArray pfxVerts( pfxLine.getLine() );
			totalVarying += pfxVerts.length();
		}
		totalVertex = totalVarying + ( 2 * numLines );

		if( totalVarying )
		{

			s("eiTag vtx_list;");
			s("vtx_list = ei_tab(EI_TYPE_VECTOR4, 100000);");
			s("ei_end_tab();");
			s("eiTag hair_list;");
			s("hair_list = ei_tab(EI_TYPE_INDEX, 100000);");
			s("ei_end_tab();");

			// read other attributes from the lines
			boost::shared_array< liqFloat > uniformCurveWidth(  new liqFloat[ numLines ] );
			boost::shared_array< liqFloat > curveWidth(         new liqFloat[ totalVarying ] );
			liqFloat* uniformWidthPtr( uniformCurveWidth.get() );
			liqFloat* widthPtr( curveWidth.get() );

			bool hasUniformWidth( false );
			bool hasWidth( false );


			//for each hair
			for( unsigned lineOn( 0 ); lineOn < lines[ setOn ].length(); lineOn++ )
			{
				s("eiInt index;");
				s("index = ei_data_table_size(vtx_list);");
				s("ei_data_table_push_back(hair_list, &index);");//start vertex index of this hair in vtx_list


				MRenderLine pfxLine( lines[ setOn ].renderLine( lineOn, &status ) );
				IfMErrorWarn(status);

				const MVectorArray& pfxVerts( pfxLine.getLine() );
				const MDoubleArray& pfxWidth( pfxLine.getWidth() );

				eiIndex num_segments = (pfxVerts.length() +2)/degree;
				s(boost::str(boost::format("eiIndex num_segments = %d;")%num_segments));
				s("ei_data_table_push_back(hair_list, &num_segments);");//how many segments this hair contains 

				s("eiVector4 vtx;");
				//for each vertex on this hair
				unsigned pOn( 0 );
				for( ; pOn < pfxVerts.length(); pOn++ )
				{
					if( pOn )
					{
						// some vertex in between start and end (pOn > 0)
						if( ( 1 == setOn ) && ( pOn ==  pfxVerts.length() - 1 ) )
						{
							// leaves need to be capped
							MVector compensate( pfxVerts[ pOn ] - pfxVerts[ pOn - 1 ] );
							compensate.normalize();
							const_cast< MVectorArray& >( pfxVerts )[ pOn ] += *( widthPtr - 1 ) * compensate;
						}
						s(boost::str(boost::format("vtx.x = %f;")%pfxVerts[ pOn ].x));
						s(boost::str(boost::format("vtx.y = %f;")%pfxVerts[ pOn ].y));
						s(boost::str(boost::format("vtx.z = %f;")%pfxVerts[ pOn ].z));
						s(boost::str(boost::format("vtx.w = %f;")%(pfxWidth[ pOn ] * 0.75)));
						s("ei_data_table_push_back(vtx_list, &vtx);");
					}else{
						// start vertices (pOn == 0)
						if( 1 == setOn )
						{
							// leaves need to be capped
							MVector compensate( pfxVerts[ 1 ] - pfxVerts[ 0 ] );
							compensate.normalize();
							const_cast< MVectorArray& >( pfxVerts )[ 0 ] += -*( widthPtr - 1 ) * compensate;
						}
						const MVector tmpVertex( pfxVerts[ 0 ] - ( pfxVerts[ 1 ] - pfxVerts[ 0 ] ) );
						s(boost::str(boost::format("vtx.x = %f;")%tmpVertex.x));
						s(boost::str(boost::format("vtx.y = %f;")%tmpVertex.y));
						s(boost::str(boost::format("vtx.z = %f;")%tmpVertex.z));
						s(boost::str(boost::format("vtx.w = %f;")%(pfxWidth[ pOn ] * 0.75)));		

						s("ei_data_table_push_back(vtx_list, &vtx);");

						s(boost::str(boost::format("vtx.x = %f;")%pfxVerts[ 0 ].x));
						s(boost::str(boost::format("vtx.y = %f;")%pfxVerts[ 0 ].y));
						s(boost::str(boost::format("vtx.z = %f;")%pfxVerts[ 0 ].z));
						s(boost::str(boost::format("vtx.w = %f;")%(pfxWidth[ 0 ] * 0.75)));

						s("ei_data_table_push_back(vtx_list, &vtx);");
					}
				}//for( ; pOn < pfxVerts.length(); pOn++ ) 
				// end vertex
				// last vertex has already been pushed

				const MVector tmpVertex( pfxVerts[ pOn - 1 ] + ( pfxVerts[ pOn - 1 ] - pfxVerts[ pOn - 2 ] ) );
				const eiScalar width   ( pfxWidth[ pOn - 1 ] + ( pfxWidth[ pOn - 1 ] - pfxWidth[ pOn - 2 ] ) ); ;
				s(boost::str(boost::format("vtx.x = %f;")%tmpVertex.x));
				s(boost::str(boost::format("vtx.y = %f;")%tmpVertex.y));
				s(boost::str(boost::format("vtx.z = %f;")%tmpVertex.z));
				s(boost::str(boost::format("vtx.w = %f;")%(width * 0.75)));
				s("ei_data_table_push_back(vtx_list, &vtx);");
			}//for lineon

			s("ei_vertex_list(vtx_list);");
			s("ei_hair_list(hair_list);");

		}//if( totalVarying ) 

		// free memory for lines arrays, are not freed by pfx destructor - Alf
		lines[0].deleteArray();
		lines[1].deleteArray();
		lines[2].deleteArray();
		s("}//"+objname);
		s("ei_end_object();");
	}
}//namespace elvishray
#endif//_USE_ELVISHRAY_