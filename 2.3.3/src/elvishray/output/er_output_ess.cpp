#include "er_output_ess.h"
#include "../er_log.h"

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
			liquidMessage2(messageError,"can't open file: [%s]", logFileName.c_str() );
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
		m_outputfile<<"##context"<<std::endl;
	}
	void OutputESS::ei_end_context()
	{	
		//CM_TRACE_FUNC("OutputESS::ei_end_context()");
		m_outputfile<<"##end context"<<std::endl;
	}
	//	Client application connection:
	void OutputESS::ei_connection( eiConnection *con )
	{
		//CM_TRACE_FUNC("OutputESS::ei_connection(...)");
		m_outputfile<<"##ei_connection(con);"<<std::endl;
	}
	//	Shader Linking:
	void OutputESS::ei_link( const char *filename )
	{
		//CM_TRACE_FUNC("OutputESS::ei_link("<<filename<<")");
		m_outputfile<<"link \""<<filename<<"\""<<std::endl;
	}
	//	Commands:
	void OutputESS::ei_verbose( int level )
	{
		//CM_TRACE_FUNC("OutputESS::ei_verbose("<<level<<")");
		m_outputfile<<"##ei_verbose("<<level<<");"<<std::endl;
	}
	void OutputESS::ei_delete( const char *element_name )
	{
		//CM_TRACE_FUNC("OutputESS::ei_delete( \""<<element_name<<"\" )");
		m_outputfile<<"delete \""<<element_name<<"\""<<std::endl;
	}
	void OutputESS::ei_render( const char *root_instgroup, const char *camera_inst, const char *options )
	{
		//CM_TRACE_FUNC("OutputESS::ei_render(\""<<root_instgroup<<"\", \""<<camera_inst<<"\", \""<<options<<"\")");
		m_outputfile<<"render \""<<root_instgroup<<"\" \""<<camera_inst<<"\" \""<<options<<"\""<<std::endl;
	}

	eiTag OutputESS::ei_tab(const eiInt type, const eiUint nkeys, const eiInt items_per_slot)
	{
		//CM_TRACE_FUNC("OutputESS::ei_tab("<<type<<","<<nkeys<<","<<items_per_slot<<")");
		m_outputfile<<"##ei_tab( "<<type<<", "<<nkeys<<","<<items_per_slot<<" );"<<std::endl;
		liquidMessage2(messageError, "OutputESS::ei_tab(), return 0;");
		return 0;
	}
	//void OutputESS::ei_tab_add(const void *value);
	//void OutputESS::ei_tab_add_int(const eiInt value);
	void OutputESS::ei_tab_add_scalar(const eiScalar value)
	{
		//CM_TRACE_FUNC("OutputESS::ei_tab_add_scalar("<<value<<")");
		m_outputfile<<"##ei_tab_add_scalar("<<value<<");"<<std::endl;
	}
	void OutputESS::ei_tab_add_vector(const eiScalar x, const eiScalar y, const eiScalar z)
	{
		//CM_TRACE_FUNC("OutputESS::ei_tab_add_vector("<<x<<","<<y<<","<<z<<")");
		m_outputfile<<"##ei_tab_add_vector("<<x<<","<<y<<","<<z<<");"<<std::endl;
	}
	void OutputESS::ei_tab_add_vector2(const eiScalar x, const eiScalar y)
	{
		//CM_TRACE_FUNC("OutputESS::ei_tab_add_vector2("<<x<<","<<y<<")");
		m_outputfile<<"##ei_tab_add_vector2("<<x<<","<<y<<");"<<std::endl;
	}
	void OutputESS::ei_tab_add_index(const eiIndex value)
	{
		//CM_TRACE_FUNC("OutputESS::ei_tab_add_index("<<value<<")");
		m_outputfile<<"##ei_tab_add_index("<<value<<");"<<std::endl;
	}
	//void OutputESS::ei_tab_add_bool(const eiBool value);
	void OutputESS::ei_end_tab()
	{
		//CM_TRACE_FUNC("OutputESS::ei_end_tab()");
		m_outputfile<<"##ei_end_tab();"<<std::endl;
	}


	//	Options:
	void OutputESS::ei_options( const char *name )
	{
		//CM_TRACE_FUNC("OutputESS::ei_options(\""<<name<<"\")");
		m_outputfile<<"options \""<<name<<"\" "<<std::endl;
	}

	//	Sampling Quality:
	void OutputESS::ei_contrast( eiScalar value )
	{
		//CM_TRACE_FUNC("OutputESS::ei_contrast("<<value<<")");
		m_outputfile<<"    contrast "<<value<<" "<<std::endl;
	}
	void OutputESS::ei_samples( int _min, int _max )
	{
		//CM_TRACE_FUNC("OutputESS::ei_samples("<<_min<<","<<_max<<")");
		m_outputfile<<"    samples "<<_min<<" "<<_max<<" "<<std::endl;
	}
	void OutputESS::ei_bucket_size( int size )
	{
		//CM_TRACE_FUNC("OutputESS::ei_bucket_size("<<size<<")");
		m_outputfile<<"    bucket_size "<<size<<" "<<std::endl;
	}
	void OutputESS::ei_filter( int filter, float size )
	{
		//CM_TRACE_FUNC("OutputESS::ei_filter("<<filter<<","<<size<<")");
		m_outputfile<<"    filter "<<filter<<" "<<size<<" "<<std::endl;
	}
	void OutputESS::ei_progressive( eiBool progressive )
	{
		//CM_TRACE_FUNC("OutputESS::ei_progressive("<<progressive<<")");
		m_outputfile<<"    ei_progressive "<<progressive<<" "<<std::endl;
	}
	//	Motion Blur:
	void OutputESS::ei_shutter( float open, float close )
	{
		//CM_TRACE_FUNC("OutputESS::ei_shutter("<<open<<", "<<close<<")");
		m_outputfile<<"    shutter "<<open<<" "<<close<<" "<<std::endl;
	}
	void OutputESS::ei_motion( int type )
	{
		//CM_TRACE_FUNC("OutputESS::ei_motion("<<type<<")");
		m_outputfile<<"    motion "<<type<<" "<<std::endl;
	}

	//	Trace Depth:
	void OutputESS::ei_trace_depth(eiInt transp, 
		eiInt glossy_reflect, eiInt diffuse_reflect, 
		eiInt glossy_refract, eiInt diffuse_refract, eiInt sum)
	{
		//CM_TRACE_FUNC("OutputESS::ei_trace_depth(4,4,4,"<<reflect<<","<<refract<<","<<sum<<")");
		m_outputfile<<"    trace_depth "<<transp<<" "
			<<glossy_reflect<<" "<<diffuse_reflect<<" "
			<<glossy_refract<<" "<<diffuse_refract<<" "<<sum<<" "<<std::endl;
	}
	//	Shadow:
	void OutputESS::ei_shadow( int type )
	{
		//CM_TRACE_FUNC("OutputESS::ei_shadow("<<type<<")");
		m_outputfile<<"    shadow "<<type<<" "<<std::endl;
	}
	//	Feature Disabling:
	void OutputESS::ei_lens(eiInt type)
	{
		//CM_TRACE_FUNC("OutputESS::ei_lens("<<type<<" )");
		m_outputfile<<"    lens "<<type<<" "<<std::endl;
	}
	void OutputESS::ei_volume( int type )
	{
		//CM_TRACE_FUNC("OutputESS::ei_volume("<<type<<")");
		m_outputfile<<"    volume "<<type<<" "<<std::endl;
	}
	void OutputESS::ei_geometry( int type )
	{
		//CM_TRACE_FUNC("OutputESS::ei_geometry("<<type<<")");
		m_outputfile<<"    geometry "<<type<<" "<<std::endl;
	}
	void OutputESS::ei_displace( int type )
	{
		//CM_TRACE_FUNC("OutputESS::ei_displace("<<type<<")");
		m_outputfile<<"    displace "<<type<<" "<<std::endl;
	}
	void OutputESS::ei_max_displace(eiScalar dist)
	{
		//CM_TRACE_FUNC("OutputESS::ei_max_displace("<<dist<<")");
		m_outputfile<<"    max_displace "<<dist<<" "<<std::endl;
	}
	void OutputESS::ei_imager( int type )
	{
		//CM_TRACE_FUNC("OutputESS::ei_imager("<<type<<")");
		m_outputfile<<"    imager "<<type<<" "<<std::endl;
	}
	//	Frame Buffer Control:
	void OutputESS::ei_exposure( float gain, float gamma )
	{
		//CM_TRACE_FUNC("OutputESS::ei_exposure("<<gain<<","<<gamma<<")");
		m_outputfile<<"    exposure "<<gain<<" "<<gamma<<" "<<std::endl;
	}
	void OutputESS::ei_quantize( float one, float _min, float _max, float dither_amplitude )
	{
		//CM_TRACE_FUNC("OutputESS::ei_quantize("<<one<<","<<_min<<","<<_max<<","<<dither_amplitude<<")");
		m_outputfile<<"    quantize "<<one<<" "<<_min<<" "<<_max<<" "<<dither_amplitude<<" "<<std::endl;
	}
	//	Miscellaneous:
	void OutputESS::ei_face( int type )
	{
		//CM_TRACE_FUNC("OutputESS::ei_face("<<type<<")");
		m_outputfile<<"    face "<<type<<" "<<std::endl;
	}
	void OutputESS::ei_approx(const eiApprox *approx)
	{
		//CM_TRACE_FUNC("OutputESS::ei_approx(approx)");
		a("ei_approx(...)");
	}
	void OutputESS::ei_end_options()
	{
		//CM_TRACE_FUNC("OutputESS::ei_end_options()");
		m_outputfile<<"end options"<<std::endl;
	}
	//	Cameras:
	void OutputESS::ei_camera( const char *name )
	{
		//CM_TRACE_FUNC("OutputESS::ei_camera(\""<< name <<"\")");
		m_outputfile<<"camera \""<<name<<"\" "<<std::endl;
	}
	//	Output Statements:
	void OutputESS::ei_output( const char *filename, const char *fileformat, const eiInt datatype)
	{
		//CM_TRACE_FUNC("OutputESS::ei_output(\""<< filename <<"\",\""<<fileformat<<"\","<<datatype<<")");
		std::string str_datatype;
		switch(datatype)
		{
		case EI_IMG_DATA_NONE:	str_datatype = "";		break;
		case EI_IMG_DATA_RGB:	str_datatype = "rgb";	break;
		case EI_IMG_DATA_RGBA:	str_datatype = "rgba";	break;
		case EI_IMG_DATA_RGBAZ: str_datatype = "rgbaz";	break;
		default:
			liquidMessage2(messageError, "error: ei_output(): image data type [%d] is unknown", datatype);
			a( boost::str(boost::format("error: ei_output(): image data type [%d] is unknown")%datatype) );
			break;
		}
		m_outputfile<<"    output \""<< filename <<"\" \""<<fileformat<<"\" \""<<str_datatype<<"\" "<<std::endl;
	}
	void OutputESS::ei_output_variable(const char *name, const eiInt datatype)
	{
		//CM_TRACE_FUNC("OutputESS::ei_output_variable(\""<< name <<"\","<<datatype<<")");
		std::string str_datatype;
		switch(datatype)
		{
		case EI_TYPE_NONE:		str_datatype = "";			break;
		/* atomic types */
		case EI_TYPE_TOKEN:		str_datatype = "token";		break;
		case EI_TYPE_BYTE:		str_datatype = "byte";		break;
		case EI_TYPE_SHORT:		str_datatype = "short";		break;
		case EI_TYPE_INT:		str_datatype = "int";		break;
		case EI_TYPE_BOOL:		str_datatype = "bool";		break;
		case EI_TYPE_TAG:		str_datatype = "tag";		break;
		case EI_TYPE_INDEX:		str_datatype = "index";		break;
		case EI_TYPE_LONG:		str_datatype = "long";		break;
		case EI_TYPE_SCALAR:	str_datatype = "scalar";	break;
		case EI_TYPE_GEOSCALAR:	str_datatype = "geoscalar";	break;
		case EI_TYPE_VECTOR:	str_datatype = "vector";	break;
		case EI_TYPE_VECTOR2:	str_datatype = "vector2";	break;
		case EI_TYPE_VECTOR4:	str_datatype = "vector4";	break;
		case EI_TYPE_MATRIX:	str_datatype = "matrix";	break;
		case EI_TYPE_BOUND:		str_datatype = "bound";		break;
		case EI_TYPE_RECT:		str_datatype = "rect";		break;
		case EI_TYPE_RECT4I:	str_datatype = "rect4i";	break;
		case EI_TYPE_INTARRAY:	str_datatype = "intarray";	break;
			/* compound types */
		case EI_TYPE_JOB_TEST:	str_datatype = "jobtest";	break;
		case EI_TYPE_ARRAY:		str_datatype = "array";		break;
		case EI_TYPE_TABLE:		str_datatype = "table";		break;
		case EI_TYPE_BLOCK:		str_datatype = "block";		break;
		case EI_TYPE_USER:		str_datatype = "user";		break;
		default:
			liquidMessage2(messageError, "error: ei_output_variable(): image data type [%d] is unknown", datatype);
			a( boost::str(boost::format("error: ei_output_variable(): image data type [%d] is unknown")%datatype) );
			break;
		}
		m_outputfile<<"    output_variable \""<< name <<"\" \""<<str_datatype<<"\" "<<std::endl;
	}
	void OutputESS::ei_end_output()
	{
		//CM_TRACE_FUNC("OutputESS::ei_end_output()");
		m_outputfile<<"end output"<<std::endl;
	}
	//	Other Camera Statements:
	void OutputESS::ei_focal( float distance )
	{
		//CM_TRACE_FUNC("OutputESS::ei_focal("<< distance <<")");
		m_outputfile<<"    focal "<< distance <<" "<<std::endl;
	}
	void OutputESS::ei_aperture( float aperture )
	{
		//CM_TRACE_FUNC("OutputESS::ei_aperture("<< aperture <<")");
		m_outputfile<<"    aperture "<< aperture <<" "<<std::endl;
	}
	void OutputESS::ei_aspect( float aspect )
	{
		//CM_TRACE_FUNC("OutputESS::ei_aspect("<< aspect <<")");
		m_outputfile<<"    aspect "<< aspect <<" "<<std::endl;
	}
	void OutputESS::ei_resolution( eiInt x, eiInt y )
	{
		//CM_TRACE_FUNC("OutputESS::ei_resolution("<< x<<","<<y <<")");
		m_outputfile<<"    resolution "<< x<<" "<<y <<" "<<std::endl;
	}
	void OutputESS::ei_window( eiInt xmin, eiInt xmax, eiInt ymin, eiInt ymax )
	{
		//CM_TRACE_FUNC("OutputESS::ei_window("<< xmin<<","<<xmax<<","<<ymin<<","<<ymax<<")");
		m_outputfile<<"    window "<< xmin<<" "<<xmax<<" "<<ymin<<" "<<ymax<<" "<<std::endl;
	}
	void OutputESS::ei_clip( float hither, float yon )
	{
		//CM_TRACE_FUNC("OutputESS::ei_clip("<<hither<<","<<yon<<")");
		m_outputfile<<"    clip "<<hither<<" "<<yon<<" "<<std::endl;
	}
	void OutputESS::ei_lens_shader(const char *shader_name)
	{
		//CM_TRACE_FUNC("OutputESS::ei_lens_shader(\""<<shader_name<<"\")");
		m_outputfile<<"lens_shader \""<<shader_name<<"\" "<<std::endl;
	}
	void OutputESS::ei_imager_shader(const char *shader_name)
	{
		//CM_TRACE_FUNC("OutputESS::ei_imager_shader(\""<<shader_name<<"\")");
		m_outputfile<<"imager_shader \""<<shader_name<<"\" "<<std::endl;
	}
	void OutputESS::ei_end_camera()
	{
		//CM_TRACE_FUNC("OutputESS::ei_end_camera()");
		m_outputfile<<"end camera"<<std::endl;
	}
	//	Textures:
	void OutputESS::ei_make_texture( const char *picturename, const char *texturename, 
		int swrap, int twrap, int filter, float swidth, float twidth )
	{
		//CM_TRACE_FUNC("OutputESS::ei_make_texture(\""<<picturename<<"\", \""<< texturename<<"\","<<swrap<<","<< twrap<<","<<filter<<", "<<swidth<<", "<<twidth<<")");
		m_outputfile<<"##ei_make_texture(\""<<picturename<<"\", \""<< texturename<<"\","<<swrap<<","<< twrap<<","<<filter<<", "<<swidth<<", "<<twidth<<");"<<std::endl;
	}
	void OutputESS::ei_texture(const char *name)
	{
		//CM_TRACE_FUNC("OutputESS::ei_texture("<<name<<")");
		m_outputfile<<"texture \""<<name<<"\" "<<std::endl;
	}
	void OutputESS::ei_file_texture(const char *filename, const eiBool local)
	{
		//CM_TRACE_FUNC("OutputESS::ei_file_texture("<<filename<<","<<local<<")");
		m_outputfile<<"file_texture \""<<filename<<"\" "<<local<<" "<<std::endl;
	}
	void OutputESS::ei_end_texture()
	{
		//CM_TRACE_FUNC("OutputESS::ei_end_texture()");
		m_outputfile<<"end texture"<<std::endl;
	}
	//	Materials:
	void OutputESS::ei_material( const char *name )
	{
		//CM_TRACE_FUNC("OutputESS::ei_material(\""<<name<<"\")");
		m_outputfile<<"material \""<<name<<"\" "<<std::endl;
	}
	void OutputESS::ei_surface_shader( const char *shader_name )
	{
		//CM_TRACE_FUNC("OutputESS::ei_surface_shader(\""<<shader_name <<"\")");
		m_outputfile<<"surface_shader \""<<shader_name <<"\" "<<std::endl;
	}
	void OutputESS::ei_displace_shader( const char *shader_name )
	{
		//CM_TRACE_FUNC("OutputESS::ei_displace_shader(\""<<shader_name <<"\")");
		m_outputfile<<"displace_shader \""<<shader_name <<"\" "<<std::endl;
	}
	void OutputESS::ei_shadow_shader( const char *shader_name )
	{
		//CM_TRACE_FUNC("OutputESS::ei_shadow_shader(\""<<shader_name <<"\")");
		m_outputfile<<"shadow_shader \""<<shader_name <<"\" "<<std::endl;
	}
	void OutputESS::ei_volume_shader( const char *shader_name )
	{
		//CM_TRACE_FUNC("OutputESS::ei_volume_shader(\""<<shader_name <<"\")");
		m_outputfile<<"volume_shader \""<<shader_name <<"\" "<<std::endl;
	}
	void OutputESS::ei_environment_shader( const char *shader_name )
	{
		//CM_TRACE_FUNC("OutputESS::ei_environment_shader(\""<<shader_name <<"\")");
		m_outputfile<<"environment_shader \""<<shader_name <<"\" "<<std::endl;
	}

	void OutputESS::ei_end_material()
	{
		//CM_TRACE_FUNC("OutputESS::ei_end_material()");
		m_outputfile<<"end material"<<std::endl;
	}
	//	Lights:
	void OutputESS::ei_light( const char *name )
	{
		//CM_TRACE_FUNC("OutputESS::ei_light(\""<<name<<"\")");
		m_outputfile<<"light \""<<name<<"\" "<<std::endl;
	}
	void OutputESS::ei_light_shader( const char *shader_name )
	{
		//CM_TRACE_FUNC("OutputESS::ei_light_shader(\""<<shader_name<<"\" )");
		m_outputfile<<"light_shader \""<<shader_name<<"\" "<<std::endl;
	}
	void OutputESS::ei_origin(eiScalar x, eiScalar y, eiScalar z)
	{
		//CM_TRACE_FUNC("OutputESS::ei_origin("<<x<<","<<y<<","<<z<<")");
		m_outputfile<<"origin "<<x<<" "<<y<<" "<<z<<" "<<std::endl;
	}
	void OutputESS::ei_area_samples( eiInt samples )
	{
		//CM_TRACE_FUNC("OutputESS::ei_area_samples("<<samples<<" )");
		m_outputfile<<"area_samples "<<samples<<" "<<std::endl;
	}
	void OutputESS::ei_adaptive(const eiBool adaptive)
	{
		//CM_TRACE_FUNC("OutputESS::ei_adaptive("<<adaptive<<" )");
		m_outputfile<<"ei_adaptive "<<adaptive<<" "<<std::endl;
	}
	void OutputESS::ei_end_light()
	{
		//CM_TRACE_FUNC("OutputESS::ei_end_light()");
		m_outputfile<<"end light"<<std::endl;
	}
	//	Objects:
	void OutputESS::ei_object( const char *type, const char *name )
	{
		//CM_TRACE_FUNC("OutputESS::ei_object(\""<< type <<"\",\""<<name<<"\")" ); 
		m_outputfile<<"object \""<< type <<"\" \""<<name<<"\" "<<std::endl;
	}
	void OutputESS::ei_box(eiScalar xmin, eiScalar ymin, eiScalar zmin, 
		eiScalar xmax, eiScalar ymax, eiScalar zmax)
	{
		//CM_TRACE_FUNC("OutputESS::ei_box("<<xmin<<","<<ymin<<","<<zmin<<", "<<xmax<<","<<ymax<<","<<zmax<<")");
		m_outputfile<<"box "<<xmin<<" "<<ymin<<" "<<zmin<<"   "<<xmax<<" "<<ymax<<" "<<zmax<<" "<<std::endl;

	}
	void OutputESS::ei_motion_box(eiScalar xmin, eiScalar ymin, eiScalar zmin, 
		eiScalar xmax, eiScalar ymax, eiScalar zmax)
	{
		//CM_TRACE_FUNC("OutputESS::ei_motion_box("<< xmin<<","<<ymin<<","<<zmin<<", "<<xmax<<","<<ymax<<","<<zmax<<")");
		m_outputfile<<"motion_box "<< xmin<<" "<<ymin<<" "<<zmin<<"   "<<xmax<<" "<<ymax<<" "<<zmax<<" "<<std::endl;
	}
	/* procedural objects */
	void OutputESS::ei_geometry_shader(const char *shader_name)
	{
		//CM_TRACE_FUNC("OutputESS::ei_geometry_shader(\""<<shader_name<<"\")");
		m_outputfile<<"geometry_shader \""<<shader_name<<"\" "<<std::endl;
	}
	void OutputESS::ei_pos_list(const eiTag tab)
	{
		//CM_TRACE_FUNC("OutputESS::ei_pos_list(tag)" );
		liquidMessage2(messageError,"OutputESS::ei_pos_list(tab)");
		m_outputfile<<"pos_list size=?"<<std::endl;
	}
	void OutputESS::ei_triangle_list(const eiTag tab)
	{
		//CM_TRACE_FUNC("OutputESS::ei_triangle_list(tag)" );
		liquidMessage2(messageError,"OutputESS::ei_triangle_list(tab)");
		m_outputfile<<"triangle_list size=?"<<std::endl;
	}
	void OutputESS::ei_end_object()
	{
		//CM_TRACE_FUNC("OutputESS::ei_end_object()"); 
		m_outputfile<<"end object"<<std::endl;
	}


	//	Instances:
	void OutputESS::ei_instance( const char *name )
	{
		//CM_TRACE_FUNC("OutputESS::ei_instance(\""<<name<<"\")");
		m_outputfile<<"instance \""<<name<<"\" "<<std::endl;
	}
	void OutputESS::ei_element( const char *element_name )
	{
		//CM_TRACE_FUNC("OutputESS::ei_element(\""<<element_name<<"\")");
		m_outputfile<<"element \""<<element_name<<"\" "<<std::endl;
	}
	void OutputESS::ei_transform( 
		float t00,float t01,float t02,float t03,
		float t10,float t11,float t12,float t13,
		float t20,float t21,float t22,float t23,
		float t30,float t31,float t32,float t33 
		)
	{
		//CM_TRACE_FUNC("OutputESS::ei_transform("<< t00<<","<<t01<<","<<t02<<","<<t03<<",   "<<t10<<","<<t11<<","<<t12<<","<<t13<<",   "<<  t20<<","<<t21<<","<<t22<<","<<t23<<",   "<<  t30<<","<<t31<<","<<t32<<","<<t33 <<")");
		m_outputfile<<"transform "
			<<t00<<" "<<t01<<" "<<t02<<" "<<t03<<"    "
			<<t10<<" "<<t11<<" "<<t12<<" "<<t13<<"    "
			<<t20<<" "<<t21<<" "<<t22<<" "<<t23<<"    "
			<<t30<<" "<<t31<<" "<<t32<<" "<<t33<<" "<<std::endl;
	}
	void OutputESS::ei_motion_transform( 
		float t00,float t01,float t02,float t03,
		float t10,float t11,float t12,float t13,
		float t20,float t21,float t22,float t23,
		float t30,float t31,float t32,float t33
		)
	{
		//CM_TRACE_FUNC("OutputESS::ei_motion_transform("<< t00<<","<<t01<<","<<t02<<","<<t03<<",   "<<t10<<","<<t11<<","<<t12<<","<<t13<<",   "<<  t20<<","<<t21<<","<<t22<<","<<t23<<",   "<<  t30<<","<<t31<<","<<t32<<","<<t33 <<")");
		m_outputfile<<"motion_transform "
			<<t00<<" "<<t01<<" "<<t02<<" "<<t03<<"    "
			<<t10<<" "<<t11<<" "<<t12<<" "<<t13<<"    "
			<<t20<<" "<<t21<<" "<<t22<<" "<<t23<<"    "
			<<t30<<" "<<t31<<" "<<t32<<" "<<t33<<" "<<std::endl;	
	}
	void OutputESS::ei_mtl( const char *material_name )
	{
		//CM_TRACE_FUNC("OutputESS::ei_mtl(\""<<material_name<<"\")");
		m_outputfile<<"mtl \""<<material_name<<"\" "<<std::endl;
	}

	void OutputESS::ei_end_instance()
	{
		//CM_TRACE_FUNC("OutputESS::ei_end_instance()");
		m_outputfile<<"end instance"<<std::endl;
	}


	//	Instance Groups:
	void OutputESS::ei_instgroup( const char *name )
	{
		//CM_TRACE_FUNC("OutputESS::ei_instgroup(\""<<name<<"\")");
		m_outputfile<<"instgroup \""<<name<<"\" "<<std::endl;
	}
	void OutputESS::ei_add_instance( const char *name )
	{
		//CM_TRACE_FUNC("OutputESS::ei_add_instance(\""<<name<<"\")");
		m_outputfile<<"add_instance \""<<name<<"\" "<<std::endl;
	}
	void OutputESS::ei_end_instgroup()
	{
		//CM_TRACE_FUNC("OutputESS::ei_end_instgroup( )");
		m_outputfile<<"end instgroup"<<std::endl;
	}
	//
	void OutputESS::ei_shader(const char *type_name, const char *instance_name)
	{
		//CM_TRACE_FUNC("OutputESS::ei_shader(\""<<type_name<<"\", \""<<instance_name<<"\")");
		m_outputfile<<"shader \""<<type_name<<"\" \""<<instance_name<<"\" "<<std::endl;
	}
	void OutputESS::ei_shader_param(
		const char *param_name, 
		const void *param_value)
	{
		//CM_TRACE_FUNC("OutputESS::ei_shader_param(\""<<param_name<<"\", \""<<param_value<<"\") not implemented");
		liquidMessage2(messageError, "OutputESS::ei_shader_param() not implemented");
		a("error: ei_shader_param(): OutputESS::ei_shader_param() not implemented");
		m_outputfile<<"    shader_param \""<<param_name<<"\" \""<<param_value<<"\" "<<std::endl;
	}
	void OutputESS::ei_shader_param_token(
		const char *param_name, 
		const char *param_value)
	{
		//CM_TRACE_FUNC("OutputESS::ei_shader_param_token(\""<<param_name<<"\", \""<<param_value<<"\") not implemented");
		m_outputfile<<"    param_token \""<<param_name<<"\" \""<<param_value<<"\" "<<std::endl;
	}
	void OutputESS::ei_shader_param_int(
		const char *param_name, 
		const eiInt param_value)
	{
		//CM_TRACE_FUNC("OutputESS::ei_shader_param_int(\""<<param_name<<"\","<<param_value<<")");
		m_outputfile<<"    param_int \""<<param_name<<"\" "<<param_value<<" "<<std::endl;
	}
	void OutputESS::ei_shader_param_scalar(
		const char *param_name, 
		const eiScalar param_value)
	{
		//CM_TRACE_FUNC("OutputESS::ei_shader_param_scalar(\""<<param_name<<"\","<<param_value<<")");
		m_outputfile<<"    param_scalar \""<<param_name<<"\" "<<param_value<<" "<<std::endl;
	}
	void OutputESS::ei_shader_param_vector(
		const char *param_name, 
		const eiScalar x, const eiScalar y, const eiScalar z)
	{
		//CM_TRACE_FUNC("OutputESS::ei_shader_param_vector(\""<<param_name<<"\","<<x<<","<<y<<","<<z<<")");
		m_outputfile<<"    "<<boost::str(boost::format("param_vector \"%s\" %f %f %f")%param_name %x %y %z )<<std::endl;
	}
	void OutputESS::ei_shader_param_vector4(
		const char *param_name, 
		const eiScalar x, const eiScalar y, const eiScalar z, const eiScalar w)
	{
		//CM_TRACE_FUNC("OutputESS::ei_shader_param_vector4(\""<<param_name<<"\","<<x<<","<<y<<","<<z<<","<<w<<")");
		m_outputfile<<"    "<<boost::str(boost::format("param_vector4 \"%s\" %f %f %f %f")%param_name %x %y %z %w)<<std::endl;
	}
	void OutputESS::ei_shader_param_tag(
		const char *param_name, 
		const eiTag param_value)
	{
		//CM_TRACE_FUNC("OutputESS::ei_shader_param_tag(\""<<param_name<<"\","<<param_value<<")");
		m_outputfile<<"    param_tag \""<<param_name<<"\" "<<param_value<<" "<<std::endl;
	}
	void OutputESS::ei_shader_param_texture(
		const char *param_name, 
		const char *texture_name)
	{
		//CM_TRACE_FUNC("OutputESS::ei_shader_param_texture(\""<<param_name<<"\",\""<<texture_name<<"\")");
		m_outputfile<<"    param_texture \""<<param_name<<"\" \""<<texture_name<<"\" "<<std::endl;
	}
	void OutputESS::ei_shader_param_index(
		const char *param_name, 
		const eiIndex param_value)
	{
		//CM_TRACE_FUNC("OutputESS::ei_shader_param_index(\""<<param_name<<"\","<<param_value<<")");
		m_outputfile<<"    param_index \""<<param_name<<"\" "<<param_value<<" "<<std::endl;
	}
	void OutputESS::ei_shader_param_bool(
		const char *param_name, 
		const eiBool param_value)
	{
		//CM_TRACE_FUNC("OutputESS::ei_shader_param_bool(\""<<param_name<<"\","<<param_value<<")");
		m_outputfile<<"    param_bool \""<<param_name<<"\" "<<param_value<<" "<<std::endl;
	}
	void OutputESS::ei_shader_link_param(
		const char *param_name, 
		const char *src_shader_name, 
		const char *src_param_name)
	{
		//CM_TRACE_FUNC("OutputESS::ei_shader_link_param(\""<<param_name<<"\",\""<<src_shader_name<<"\",\""<<src_param_name<<"\")");
		m_outputfile<<"    link_param \""<<param_name<<"\" \""<<src_shader_name<<"\" \""<<src_param_name<<"\" "<<std::endl;
	}
	void OutputESS::ei_end_shader()
	{
		//CM_TRACE_FUNC("OutputESS::ei_end_shader()");
		m_outputfile<<"end shader"<<std::endl;
	}

	void OutputESS::ei_declare(const char *name, const eiInt storage_class, const eiInt type/*, const void *tag*/)
	{
		//CM_TRACE_FUNC("OutputESS::ei_declare(\""<<name<<"\","<<storage_class<<","<<type<<", &tag)");
		std::string str_storage_class;
		switch(storage_class)
		{
		case EI_CONSTANT: str_storage_class="const";   break;
		case EI_UNIFORM:  str_storage_class="uniform"; break;
		case EI_VARYING:  str_storage_class="varying"; break;
		case EI_VERTEX:   str_storage_class="vertex";  break;
		default:
			liquidMessage2(messageError, "ei_declare(%s, %d(unknown), %d), ", name, storage_class, type);
			a(boost::str(boost::format("error: ei_declare(%s, %d(unknown), %d)")%name %storage_class %type));
			break;
		}

		std::string str_type;
		switch(type)
		{
		case EI_TYPE_NONE:		str_type = "";			break;
			/* atomic types */
		case EI_TYPE_TOKEN:		str_type = "token";		break;
		case EI_TYPE_BYTE:		str_type = "byte";		break;
		case EI_TYPE_SHORT:		str_type = "short";		break;
		case EI_TYPE_INT:		str_type = "int";		break;
		case EI_TYPE_BOOL:		str_type = "bool";		break;
		case EI_TYPE_TAG:		str_type = "tag";		break;
		case EI_TYPE_INDEX:		str_type = "index";		break;
		case EI_TYPE_LONG:		str_type = "long";		break;
		case EI_TYPE_SCALAR:	str_type = "scalar";	break;
		case EI_TYPE_GEOSCALAR:	str_type = "geoscalar";	break;
		case EI_TYPE_VECTOR:	str_type = "vector";	break;
		case EI_TYPE_VECTOR2:	str_type = "vector2";	break;
		case EI_TYPE_VECTOR4:	str_type = "vector4";	break;
		case EI_TYPE_MATRIX:	str_type = "matrix";	break;
		case EI_TYPE_BOUND:		str_type = "bound";		break;
		case EI_TYPE_RECT:		str_type = "rect";		break;
		case EI_TYPE_RECT4I:	str_type = "rect4i";	break;
		case EI_TYPE_INTARRAY:	str_type = "intarray";	break;
			/* compound types */
		case EI_TYPE_JOB_TEST:	str_type = "jobtest";	break;
		case EI_TYPE_ARRAY:		str_type = "array";		break;
		case EI_TYPE_TABLE:		str_type = "table";		break;
		case EI_TYPE_BLOCK:		str_type = "block";		break;
		case EI_TYPE_USER:		str_type = "user";		break;
		default:
			liquidMessage2(messageError, "ei_declare(%s, %d, %d(unknown)), ", name, storage_class, type);
			a(boost::str(boost::format("error: ei_declare(%s, %d, %d(unknown))")%name %storage_class %type));
			break;
		}
		m_outputfile<<"declare \""<<name<<"\" \""<<str_storage_class<<"\" \""<<str_type<<"\" "<<std::endl;
	}
	void OutputESS::ei_variable(const char *name, const void *tag)
	{
		//CM_TRACE_FUNC("OutputESS::ei_variable(\""<<name<<"\", &tag)");
		liquidMessage2(messageError, "OutputESS::ei_declare(&tag)");
		a("error: ei_variable(&tag)");
		m_outputfile<<"variable \""<<name<<"\"  &tag);"<<std::endl;
	}
	void OutputESS::ei_degree(const eiInt degree)
	{
		//CM_TRACE_FUNC("OutputESS::ei_degree("<<degree<<")");
		m_outputfile<<"degree "<<degree<<" "<<std::endl;
	}

	//----------------------------------------------------
	// Warped ER API interfaces
	//----------------------------------------------------
	void OutputESS::liq_lightgroup_in_object_instance(const char *light_group_name)
	{
		//CM_TRACE_FUNC("OutputESS::liq_lightgroup_in_object_instance("<<light_group_name<<")");
		s("declare \"lightgroup\"  \"const\"  \"token\" ");
		s("variable \"lightgroup\" \""+std::string(light_group_name)+"\" ");
	}
	void OutputESS::liq_lightgroup_in_light_instance_beg()
	{
		//CM_TRACE_FUNC("OutputESS::liq_lightgroup_in_light_instance_beg()");

	}
	void OutputESS::liq_lightgroup_in_light_instance(const char *light_group_name)
	{
		//CM_TRACE_FUNC("OutputESS::liq_lightgroup_in_light_instance("<<light_group_name<<")");
		s("declare \""+std::string(light_group_name)+"\" \"const\" \"int\" ");

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
		s("object \""+objname+"\" \"poly\" " );
		//
		s(boost::str(boost::format("pos_list %d")%POSITION.size()));
		for(std::size_t i=0; i<POSITION.size(); ++i)
		{
			s(boost::str(boost::format("%f %f %f")%POSITION[i].x %POSITION[i].y %POSITION[i].z ) );
		}
		//
		if( POSITION_mb.size() > 0 )
		{
			s(boost::str(boost::format("motion_pos_list %d")%POSITION_mb.size()));
			for(std::size_t i=0; i<POSITION.size(); ++i)
			{
				s(boost::str(boost::format("%f %f %f")%POSITION_mb[i].x %POSITION_mb[i].y %POSITION_mb[i].z ) );
			}
		}
		//
		if( NORMAL.size() >0 )
		{
			s(boost::str(boost::format("nrm_list %d")%NORMAL.size()));
			for(std::size_t i=0; i<NORMAL.size(); ++i)
			{
				s(boost::str(boost::format("%f %f %f")%NORMAL[i].x %NORMAL[i].y %NORMAL[i].z ) );
			}
		}
		//
		if( UV.size() >0 )
		{
			s(boost::str(boost::format("uv_list %d")%UV.size()));
			for(std::size_t i=0; i<UV.size(); ++i)
			{
				s(boost::str(boost::format("%f %f %f")%UV[i].x %UV[i].y %UV[i].z ) );
			}
		}
		//
		s(boost::str(boost::format("triangle_list %d")%INDEX.size()));
		for(std::size_t i=0; i<INDEX.size()/3; ++i)
		{
			s(boost::str(boost::format("%d %d %d")%INDEX[3*i] %INDEX[3*i+1] %INDEX[3*i+2] ) );
		}
		//
		s("end object");
	}
	void OutputESS::liq_hair(
		const std::string &objname,
		MFnPfxGeometry &fnPfxGeometry,
		int degree,
		unsigned setOn)
	{
		//CM_TRACE_FUNC("OutputESS::liq_hair(objname)");
		liquidMessage2(messageError, "OutputESS::liq_hair() is not implemented.");
		a("OutputESS::liq_hair() is not implemented.");
	}
}//namespace elvishray