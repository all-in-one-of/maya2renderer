#include <liqConfig.h>
#ifdef _USE_ELVISHRAY_

#include "er_output_mgr.h"
#include <liqlog.h>
#include "output/er_output_base.h"
#include "output/er_output_call.h"
#include "output/er_output_ess.h"
#include "output/er_output_esa.h"

namespace elvishray
{
	OutputMgr::OutputMgr()
	{
		//CM_TRACE_FUNC("OutputMgr::OutputMgr()");
	}
	OutputMgr::~OutputMgr()
	{
		//CM_TRACE_FUNC("OutputMgr::~OutputMgr()");
	}
	void OutputMgr::setOutputImagePath(const std::string &output_image_path)
	{
		CM_TRACE_FUNC("OutputMgr::setOutputImagePath("<<output_image_path<<")");
		m_output_image_path = output_image_path;
	}
	OutputBase* OutputMgr::createOutput(OutputType outputtype)
	{
		CM_TRACE_FUNC("OutputMgr::createOutput("<<outputtype<<")");
		OutputBase* ret = NULL;

		switch(outputtype)
		{
		case OutputMgr::OT_CALL:
			ret = new OutputCall();
			break;
		case OutputMgr::OT_ESS:
			ret = new OutputESS(m_output_image_path);
			break;
		case OutputMgr::OT_ESA:
			ret = new OutputESA(m_output_image_path);
			break;
		default:
			liquidMessage2(messageError, "OutputMgr::createOutput()> unknown output type \"%d\"", outputtype);
			ret = NULL;
			break;	
		}
		return ret;
	}
	int OutputMgr::add(OutputType outputtype)
	{
		CM_TRACE_FUNC("OutputMgr::add("<<outputtype<<")");
		if(m_receiver_types.end() == std::find(m_receiver_types.begin(), m_receiver_types.end(), outputtype) )
		{
			m_receiver_types.push_back(outputtype);
			return 0;
		}

		liquidMessage2(messageError, "OutputMgr::add()> output type \"%d\" already exists.", outputtype);
		return -1;//already exist.
	}
	int OutputMgr::init()
	{
		CM_TRACE_FUNC("OutputMgr::init()");

		//create receivers
		for(std::size_t i=0; i<m_receiver_types.size(); ++i)
		{
			OutputBase* newreceiver = createOutput( m_receiver_types[i] );
			m_receivers.push_back(newreceiver);
		}

		if(m_receiver_types.size() != m_receivers.size())
		{
			liquidMessage2(messageError, "OutputMgr::init(): m_receiver_types.size() != m_receivers.SIZE()");
		}

		//call init()
		std::vector<elvishray::OutputBase*>::iterator i= m_receivers.begin();
		std::vector<elvishray::OutputBase*>::iterator e= m_receivers.end();
		for(; i!=e; ++i)
		{
			(*i)->init();
		}
		return 0;
	}
	int OutputMgr::uninit()
	{
		CM_TRACE_FUNC("OutputMgr::uninit()");
		//destruct m_receivers
		std::vector<elvishray::OutputBase*>::iterator i= m_receivers.begin();
		std::vector<elvishray::OutputBase*>::iterator e= m_receivers.end();
		for(; i!=e; ++i)
		{
			delete (*i);
		}
		m_receivers.clear();

		//destruct m_receiver_types
		m_receiver_types.clear();

		return 0;
	}
	//----------------------------------------------------
	// ER API interfaces
	//----------------------------------------------------
	void OutputMgr::ln()
	{
		//CM_TRACE_FUNC("OutputMgr::annotation()");
		std::vector<elvishray::OutputBase*>::iterator i= m_receivers.begin();
		std::vector<elvishray::OutputBase*>::iterator e= m_receivers.end();
		for(; i!=e; ++i)
		{
			(*i)->ln();
		}
	}
	void OutputMgr::s(const std::string &msg)
	{
		//CM_TRACE_FUNC("OutputMgr::annotation()");
		std::vector<elvishray::OutputBase*>::iterator i= m_receivers.begin();
		std::vector<elvishray::OutputBase*>::iterator e= m_receivers.end();
		for(; i!=e; ++i)
		{
			(*i)->s(msg);
		}
	}
	void OutputMgr::a(const std::string &msg)
	{
		//CM_TRACE_FUNC("OutputMgr::annotation()");
		std::vector<elvishray::OutputBase*>::iterator i= m_receivers.begin();
		std::vector<elvishray::OutputBase*>::iterator e= m_receivers.end();
		for(; i!=e; ++i)
		{
			(*i)->a(msg);
		}
	}
	void OutputMgr::ei_context()
	{
		//CM_TRACE_FUNC("OutputMgr::ei_context()");
		std::vector<elvishray::OutputBase*>::iterator i= m_receivers.begin();
		std::vector<elvishray::OutputBase*>::iterator e= m_receivers.end();
		for(; i!=e; ++i)
		{
			(*i)->ei_context();
		}
	}
	void OutputMgr::ei_end_context()
	{
		//CM_TRACE_FUNC("OutputMgr::ei_end_context()");
		std::vector<elvishray::OutputBase*>::iterator i= m_receivers.begin();
		std::vector<elvishray::OutputBase*>::iterator e= m_receivers.end();
		for(; i!=e; ++i)
		{
			(*i)->ei_end_context();
		}
	}
	//	Client application connection:
	void OutputMgr::ei_connection( eiConnection *con )
	{
		//CM_TRACE_FUNC("OutputMgr::ei_connection(...)");
		std::vector<elvishray::OutputBase*>::iterator i= m_receivers.begin();
		std::vector<elvishray::OutputBase*>::iterator e= m_receivers.end();
		for(; i!=e; ++i)
		{
			(*i)->ei_connection(con);
		}
	}
	//	Shader Linking:
	void OutputMgr::ei_link( const char *filename )
	{
		//CM_TRACE_FUNC("OutputMgr::ei_link("<<filename<<")");
		std::vector<elvishray::OutputBase*>::iterator i= m_receivers.begin();
		std::vector<elvishray::OutputBase*>::iterator e= m_receivers.end();
		for(; i!=e; ++i)
		{
			(*i)->ei_link(filename);
		}
	}
	//	Commands:
	void OutputMgr::ei_verbose( int level )
	{
		//CM_TRACE_FUNC("OutputMgr::ei_verbose("<<level<<")");
		std::vector<elvishray::OutputBase*>::iterator i= m_receivers.begin();
		std::vector<elvishray::OutputBase*>::iterator e= m_receivers.end();
		for(; i!=e; ++i)
		{
			(*i)->ei_verbose(level);
		}
	}
	void OutputMgr::ei_delete( const char *element_name )
	{
		//CM_TRACE_FUNC("OutputMgr::ei_delete( \""<<element_name<<"\" )");
		std::vector<elvishray::OutputBase*>::iterator i= m_receivers.begin();
		std::vector<elvishray::OutputBase*>::iterator e= m_receivers.end();
		for(; i!=e; ++i)
		{
			(*i)->ei_delete(element_name);
		}
	}
	void OutputMgr::ei_render( const char *root_instgroup, const char *camera_inst, const char *options )
	{
		//CM_TRACE_FUNC("OutputMgr::ei_render(\""<<root_instgroup<<"\", \""<<camera_inst<<"\", \""<<options<<"\")");
		std::vector<elvishray::OutputBase*>::iterator i= m_receivers.begin();
		std::vector<elvishray::OutputBase*>::iterator e= m_receivers.end();
		for(; i!=e; ++i)
		{
			(*i)->ei_render(root_instgroup, camera_inst, options);
		}
	}

	eiTag OutputMgr::ei_tab(const eiInt type, const eiUint nkeys, const eiInt items_per_slot)
	{
		//CM_TRACE_FUNC("OutputMgr::ei_tab("<<type<<","<<items_per_slot<<")");
		eiTag ret = 0;
		for(std::size_t i=0; i<m_receiver_types.size(); ++i)
		{
			eiTag tag = m_receivers[i]->ei_tab(type, nkeys, items_per_slot);
			if( m_receiver_types[i] == OT_CALL)
			{
				ret = tag;
			}
		}
 		return ret;
	}
	//void OutputMgr::ei_tab_add(const void *value);
	//void OutputMgr::ei_tab_add_int(const eiInt value);
	void OutputMgr::ei_tab_add_scalar(const eiScalar value)
	{
		//CM_TRACE_FUNC("OutputMgr::ei_tab_add_scalar("<<value<<")");
		std::vector<elvishray::OutputBase*>::iterator i= m_receivers.begin();
		std::vector<elvishray::OutputBase*>::iterator e= m_receivers.end();
		for(; i!=e; ++i)
		{
			(*i)->ei_tab_add_scalar(value);
		}
	}
	void OutputMgr::ei_tab_add_vector(const eiScalar x, const eiScalar y, const eiScalar z)
	{
		//CM_TRACE_FUNC("OutputMgr::ei_tab_add_vector("<<x<<","<<y<<","<<z<<")");
		std::vector<elvishray::OutputBase*>::iterator i= m_receivers.begin();
		std::vector<elvishray::OutputBase*>::iterator e= m_receivers.end();
		for(; i!=e; ++i)
		{
			(*i)->ei_tab_add_vector(x, y, z);
		}
	}
	void OutputMgr::ei_tab_add_vector2(const eiScalar x, const eiScalar y)
	{
		//CM_TRACE_FUNC("OutputMgr::ei_tab_add_vector2("<<x<<","<<y<<")");
		std::vector<elvishray::OutputBase*>::iterator i= m_receivers.begin();
		std::vector<elvishray::OutputBase*>::iterator e= m_receivers.end();
		for(; i!=e; ++i)
		{
			(*i)->ei_tab_add_vector2(x, y);
		}
	}
	void OutputMgr::ei_tab_add_index(const eiIndex value)
	{
		//CM_TRACE_FUNC("OutputMgr::ei_tab_add_index("<<value<<")");
		std::vector<elvishray::OutputBase*>::iterator i= m_receivers.begin();
		std::vector<elvishray::OutputBase*>::iterator e= m_receivers.end();
		for(; i!=e; ++i)
		{
			(*i)->ei_tab_add_index(value);
		}
	}
	//void OutputMgr::ei_tab_add_bool(const eiBool value);
	void OutputMgr::ei_end_tab()
	{
		//CM_TRACE_FUNC("OutputMgr::ei_end_tab()");
		std::vector<elvishray::OutputBase*>::iterator i= m_receivers.begin();
		std::vector<elvishray::OutputBase*>::iterator e= m_receivers.end();
		for(; i!=e; ++i)
		{
			(*i)->ei_end_tab();
		}
	}


	//	Options:
	void OutputMgr::ei_options( const char *name )
	{
		//CM_TRACE_FUNC("OutputMgr::ei_options(\""<<name<<"\")");
		std::vector<elvishray::OutputBase*>::iterator i= m_receivers.begin();
		std::vector<elvishray::OutputBase*>::iterator e= m_receivers.end();
		for(; i!=e; ++i)
		{
			(*i)->ei_options(name);
		}
	}

	//	Sampling Quality:
	void OutputMgr::ei_contrast( eiScalar value )
	{
		//CM_TRACE_FUNC("OutputMgr::ei_contrast("<<value<<")");
		std::vector<elvishray::OutputBase*>::iterator i= m_receivers.begin();
		std::vector<elvishray::OutputBase*>::iterator e= m_receivers.end();
		for(; i!=e; ++i)
		{
			(*i)->ei_contrast(value);
		}
	}
	void OutputMgr::ei_samples( int _min, int _max )
	{
		//CM_TRACE_FUNC("OutputMgr::ei_samples("<<_min<<","<<_max<<")");
		std::vector<elvishray::OutputBase*>::iterator i= m_receivers.begin();
		std::vector<elvishray::OutputBase*>::iterator e= m_receivers.end();
		for(; i!=e; ++i)
		{
			(*i)->ei_samples(_min,_max);
		}
	}
	void OutputMgr::ei_bucket_size( int size )
	{
		//CM_TRACE_FUNC("OutputMgr::ei_bucket_size("<<size<<")");
		std::vector<elvishray::OutputBase*>::iterator i= m_receivers.begin();
		std::vector<elvishray::OutputBase*>::iterator e= m_receivers.end();
		for(; i!=e; ++i)
		{
			(*i)->ei_bucket_size(size);
		}
	}
	void OutputMgr::ei_filter( int filter, float size )
	{
		//CM_TRACE_FUNC("OutputMgr::ei_filter("<<filter<<","<<size<<")");
		std::vector<elvishray::OutputBase*>::iterator i= m_receivers.begin();
		std::vector<elvishray::OutputBase*>::iterator e= m_receivers.end();
		for(; i!=e; ++i)
		{
			(*i)->ei_filter(filter,size);
		}
	}
	//	Motion Blur:
	void OutputMgr::ei_shutter( float open, float close )
	{
		//CM_TRACE_FUNC("OutputMgr::ei_shutter("<<open<<", "<<close<<")");
		std::vector<elvishray::OutputBase*>::iterator i= m_receivers.begin();
		std::vector<elvishray::OutputBase*>::iterator e= m_receivers.end();
		for(; i!=e; ++i)
		{
			(*i)->ei_shutter(open, close);
		}
	}
	void OutputMgr::ei_motion( int type )
	{
		//CM_TRACE_FUNC("OutputMgr::ei_motion("<<type<<")");
		std::vector<elvishray::OutputBase*>::iterator i= m_receivers.begin();
		std::vector<elvishray::OutputBase*>::iterator e= m_receivers.end();
		for(; i!=e; ++i)
		{
			(*i)->ei_motion(type);
		}
	}

	//	Trace Depth:
	void OutputMgr::ei_trace_depth(eiInt transp, 
		eiInt glossy_reflect, eiInt diffuse_reflect, 
		eiInt glossy_refract, eiInt diffuse_refract, eiInt sum)
	{
		//CM_TRACE_FUNC("OutputMgr::ei_trace_depth(4,4,4,"<<reflect<<","<<refract<<","<<sum<<")");
		std::vector<elvishray::OutputBase*>::iterator i= m_receivers.begin();
		std::vector<elvishray::OutputBase*>::iterator e= m_receivers.end();
		for(; i!=e; ++i)
		{
			(*i)->ei_trace_depth(transp, 
				glossy_reflect, diffuse_reflect, 
				glossy_refract, diffuse_refract, sum);
		}
	}
	//	Shadow:
	void OutputMgr::ei_shadow( int type )
	{
		//CM_TRACE_FUNC("OutputMgr::ei_shadow("<<type<<")");
		std::vector<elvishray::OutputBase*>::iterator i= m_receivers.begin();
		std::vector<elvishray::OutputBase*>::iterator e= m_receivers.end();
		for(; i!=e; ++i)
		{
			(*i)->ei_shadow(type);
		}
	}
	//	Feature Disabling:
	void OutputMgr::ei_lens(eiInt type)
	{
		//CM_TRACE_FUNC("OutputMgr::ei_lens("<<type<<" )");
		std::vector<elvishray::OutputBase*>::iterator i= m_receivers.begin();
		std::vector<elvishray::OutputBase*>::iterator e= m_receivers.end();
		for(; i!=e; ++i)
		{
			(*i)->ei_lens(type );
		}
	}
	void OutputMgr::ei_volume( int type )
	{
		//CM_TRACE_FUNC("OutputMgr::ei_volume("<<type<<")");
		std::vector<elvishray::OutputBase*>::iterator i= m_receivers.begin();
		std::vector<elvishray::OutputBase*>::iterator e= m_receivers.end();
		for(; i!=e; ++i)
		{
			(*i)->ei_volume(type);
		}
	}
	void OutputMgr::ei_geometry( int type )
	{
		//CM_TRACE_FUNC("OutputMgr::ei_geometry("<<type<<")");
		std::vector<elvishray::OutputBase*>::iterator i= m_receivers.begin();
		std::vector<elvishray::OutputBase*>::iterator e= m_receivers.end();
		for(; i!=e; ++i)
		{
			(*i)->ei_geometry(type);
		}
	}
	void OutputMgr::ei_displace( int type )
	{
		//CM_TRACE_FUNC("OutputMgr::ei_displace("<<type<<")");
		std::vector<elvishray::OutputBase*>::iterator i= m_receivers.begin();
		std::vector<elvishray::OutputBase*>::iterator e= m_receivers.end();
		for(; i!=e; ++i)
		{
			(*i)->ei_displace(type);
		}
	}
	void OutputMgr::ei_max_displace(eiScalar dist)
	{
		//CM_TRACE_FUNC("OutputMgr::ei_max_displace("<<dist<<")");
		std::vector<elvishray::OutputBase*>::iterator i= m_receivers.begin();
		std::vector<elvishray::OutputBase*>::iterator e= m_receivers.end();
		for(; i!=e; ++i)
		{
			(*i)->ei_max_displace(dist);
		}
	}
	void OutputMgr::ei_imager( int type )
	{
		//CM_TRACE_FUNC("OutputMgr::ei_imager("<<type<<")");
		std::vector<elvishray::OutputBase*>::iterator i= m_receivers.begin();
		std::vector<elvishray::OutputBase*>::iterator e= m_receivers.end();
		for(; i!=e; ++i)
		{
			(*i)->ei_imager(type);
		}
	}
	//	Frame Buffer Control:
	void OutputMgr::ei_exposure( float gain, float gamma )
	{
		//CM_TRACE_FUNC("OutputMgr::ei_exposure("<<gain<<","<<gamma<<")");
		std::vector<elvishray::OutputBase*>::iterator i= m_receivers.begin();
		std::vector<elvishray::OutputBase*>::iterator e= m_receivers.end();
		for(; i!=e; ++i)
		{
			(*i)->ei_exposure(gain, gamma);
		}
	}
	void OutputMgr::ei_quantize( float one, float _min, float _max, float dither_amplitude )
	{
		//CM_TRACE_FUNC("OutputMgr::ei_quantize("<<one<<","<<_min<<","<<_max<<","<<dither_amplitude<<")");
		std::vector<elvishray::OutputBase*>::iterator i= m_receivers.begin();
		std::vector<elvishray::OutputBase*>::iterator e= m_receivers.end();
		for(; i!=e; ++i)
		{
			(*i)->ei_quantize(one,  _min, _max, dither_amplitude);
		}
	}
	//	Miscellaneous:
	void OutputMgr::ei_face( int type )
	{
		//CM_TRACE_FUNC("OutputMgr::ei_face("<<type<<")");
		std::vector<elvishray::OutputBase*>::iterator i= m_receivers.begin();
		std::vector<elvishray::OutputBase*>::iterator e= m_receivers.end();
		for(; i!=e; ++i)
		{
			(*i)->ei_face(type);
		}
	}
	void OutputMgr::ei_approx(const eiApprox *approx)
	{
		//CM_TRACE_FUNC("OutputMgr::ei_approx(approx)");
		std::vector<elvishray::OutputBase*>::iterator i= m_receivers.begin();
		std::vector<elvishray::OutputBase*>::iterator e= m_receivers.end();
		for(; i!=e; ++i)
		{
			(*i)->ei_approx(approx);
		}
	}
	void OutputMgr::ei_end_options()
	{
		//CM_TRACE_FUNC("OutputMgr::ei_end_options()");
		std::vector<elvishray::OutputBase*>::iterator i= m_receivers.begin();
		std::vector<elvishray::OutputBase*>::iterator e= m_receivers.end();
		for(; i!=e; ++i)
		{
			(*i)->ei_end_options();
		}
	}
	//	Cameras:
	void OutputMgr::ei_camera( const char *name )
	{
		//CM_TRACE_FUNC("OutputMgr::ei_camera(\""<< name <<"\")");
		std::vector<elvishray::OutputBase*>::iterator i= m_receivers.begin();
		std::vector<elvishray::OutputBase*>::iterator e= m_receivers.end();
		for(; i!=e; ++i)
		{
			(*i)->ei_camera(name);
		}
	}
	//	Output Statements:
	void OutputMgr::ei_output( const char *filename, const char *fileformat, const eiInt datatype)
	{
		//CM_TRACE_FUNC("OutputMgr::ei_output(\""<< filename <<"\",\""<<fileformat<<"\","<<datatype<<")");
		std::vector<elvishray::OutputBase*>::iterator i= m_receivers.begin();
		std::vector<elvishray::OutputBase*>::iterator e= m_receivers.end();
		for(; i!=e; ++i)
		{
			(*i)->ei_output(filename, fileformat, datatype);
		}
	}
	void OutputMgr::ei_output_variable(const char *name, const eiInt datatype)
	{
		//CM_TRACE_FUNC("OutputMgr::ei_output_variable(\""<< name <<"\","<<datatype<<")");
		std::vector<elvishray::OutputBase*>::iterator i= m_receivers.begin();
		std::vector<elvishray::OutputBase*>::iterator e= m_receivers.end();
		for(; i!=e; ++i)
		{
			(*i)->ei_output_variable(name, datatype);
		}
	}
	void OutputMgr::ei_end_output()
	{
		//CM_TRACE_FUNC("OutputMgr::ei_end_output()");
		std::vector<elvishray::OutputBase*>::iterator i= m_receivers.begin();
		std::vector<elvishray::OutputBase*>::iterator e= m_receivers.end();
		for(; i!=e; ++i)
		{
			(*i)->ei_end_output();
		}
	}
	//	Other Camera Statements:
	void OutputMgr::ei_focal( float distance )
	{
		//CM_TRACE_FUNC("OutputMgr::ei_focal("<< distance <<")");
		std::vector<elvishray::OutputBase*>::iterator i= m_receivers.begin();
		std::vector<elvishray::OutputBase*>::iterator e= m_receivers.end();
		for(; i!=e; ++i)
		{
			(*i)->ei_focal(distance);
		}
	}
	void OutputMgr::ei_aperture( float aperture )
	{
		//CM_TRACE_FUNC("OutputMgr::ei_aperture("<< aperture <<")");
		std::vector<elvishray::OutputBase*>::iterator i= m_receivers.begin();
		std::vector<elvishray::OutputBase*>::iterator e= m_receivers.end();
		for(; i!=e; ++i)
		{
			(*i)->ei_aperture(aperture);
		}
	}
	void OutputMgr::ei_aspect( float aspect )
	{
		//CM_TRACE_FUNC("OutputMgr::ei_aspect("<< aspect <<")");
		std::vector<elvishray::OutputBase*>::iterator i= m_receivers.begin();
		std::vector<elvishray::OutputBase*>::iterator e= m_receivers.end();
		for(; i!=e; ++i)
		{
			(*i)->ei_aspect(aspect);
		}
	}
	void OutputMgr::ei_resolution( eiInt x, eiInt y )
	{
		//CM_TRACE_FUNC("OutputMgr::ei_resolution("<< x<<","<<y <<")");
		std::vector<elvishray::OutputBase*>::iterator i= m_receivers.begin();
		std::vector<elvishray::OutputBase*>::iterator e= m_receivers.end();
		for(; i!=e; ++i)
		{
			(*i)->ei_resolution(x, y);
		}
	}
	void OutputMgr::ei_window( eiInt xmin, eiInt xmax, eiInt ymin, eiInt ymax )
	{
		//CM_TRACE_FUNC("OutputMgr::ei_window("<< xmin<<","<<xmax<<","<<ymin<<","<<ymax<<")");
		std::vector<elvishray::OutputBase*>::iterator i= m_receivers.begin();
		std::vector<elvishray::OutputBase*>::iterator e= m_receivers.end();
		for(; i!=e; ++i)
		{
			(*i)->ei_window(xmin, xmax, ymin, ymax);
		}
	}
	void OutputMgr::ei_clip( float hither, float yon )
	{
		//CM_TRACE_FUNC("OutputMgr::ei_clip("<<hither<<","<<yon<<")");
		std::vector<elvishray::OutputBase*>::iterator i= m_receivers.begin();
		std::vector<elvishray::OutputBase*>::iterator e= m_receivers.end();
		for(; i!=e; ++i)
		{
			(*i)->ei_clip(hither,yon);
		}
	}
	void OutputMgr::ei_lens_shader(const char *shader_name)
	{
		//CM_TRACE_FUNC("OutputMgr::ei_lens_shader(\""<<shader_name<<"\")");
		std::vector<elvishray::OutputBase*>::iterator i= m_receivers.begin();
		std::vector<elvishray::OutputBase*>::iterator e= m_receivers.end();
		for(; i!=e; ++i)
		{
			(*i)->ei_lens_shader(shader_name);
		}
	}
	void OutputMgr::ei_imager_shader(const char *shader_name)
	{
		//CM_TRACE_FUNC("OutputMgr::ei_imager_shader(\""<<shader_name<<"\")");
		std::vector<elvishray::OutputBase*>::iterator i= m_receivers.begin();
		std::vector<elvishray::OutputBase*>::iterator e= m_receivers.end();
		for(; i!=e; ++i)
		{
			(*i)->ei_imager_shader(shader_name);
		}
	}
	void OutputMgr::ei_end_camera()
	{
		//CM_TRACE_FUNC("OutputMgr::ei_end_camera()");
		std::vector<elvishray::OutputBase*>::iterator i= m_receivers.begin();
		std::vector<elvishray::OutputBase*>::iterator e= m_receivers.end();
		for(; i!=e; ++i)
		{
			(*i)->ei_end_camera();
		}
	}
	//	Textures:
	void OutputMgr::ei_make_texture( const char *picturename, const char *texturename, 
		int swrap, int twrap, int filter, float swidth, float twidth )
	{
		//CM_TRACE_FUNC("OutputMgr::ei_make_texture(\""<<picturename<<"\", \""<< texturename<<"\","<<swrap<<","<< twrap<<","<<filter<<", "<<swidth<<", "<<twidth<<")");
		std::vector<elvishray::OutputBase*>::iterator i= m_receivers.begin();
		std::vector<elvishray::OutputBase*>::iterator e= m_receivers.end();
		for(; i!=e; ++i)
		{
			(*i)->ei_make_texture(picturename,  texturename, swrap, twrap, filter, swidth, twidth);
		}
	}
	void OutputMgr::ei_texture(const char *name)
	{
		//CM_TRACE_FUNC("OutputMgr::ei_texture("<<name<<")");
		std::vector<elvishray::OutputBase*>::iterator i= m_receivers.begin();
		std::vector<elvishray::OutputBase*>::iterator e= m_receivers.end();
		for(; i!=e; ++i)
		{
			(*i)->ei_texture(name);
		}
	}
	void OutputMgr::ei_file_texture(const char *filename, const eiBool local)
	{
		//CM_TRACE_FUNC("OutputMgr::ei_texture("<<filename<<","<<local<<")");
		std::vector<elvishray::OutputBase*>::iterator i= m_receivers.begin();
		std::vector<elvishray::OutputBase*>::iterator e= m_receivers.end();
		for(; i!=e; ++i)
		{
			(*i)->ei_file_texture(filename, local);
		}
	}
	void OutputMgr::ei_end_texture()
	{
		//CM_TRACE_FUNC("OutputMgr::ei_end_texture()");
		std::vector<elvishray::OutputBase*>::iterator i= m_receivers.begin();
		std::vector<elvishray::OutputBase*>::iterator e= m_receivers.end();
		for(; i!=e; ++i)
		{
			(*i)->ei_end_texture();
		}
	}
	//	Materials:
	void OutputMgr::ei_material( const char *name )
	{
		//CM_TRACE_FUNC("OutputMgr::ei_material(\""<<name<<"\")");
		std::vector<elvishray::OutputBase*>::iterator i= m_receivers.begin();
		std::vector<elvishray::OutputBase*>::iterator e= m_receivers.end();
		for(; i!=e; ++i)
		{
			(*i)->ei_material(name);
		}
	}
	void OutputMgr::ei_surface_shader( const char *shader_name )
	{
		//CM_TRACE_FUNC("OutputMgr::ei_surface_shader(\""<<shader_name <<"\")");
		std::vector<elvishray::OutputBase*>::iterator i= m_receivers.begin();
		std::vector<elvishray::OutputBase*>::iterator e= m_receivers.end();
		for(; i!=e; ++i)
		{
			(*i)->ei_surface_shader(shader_name);
		}
	}
	void OutputMgr::ei_displace_shader( const char *shader_name )
	{
		//CM_TRACE_FUNC("OutputMgr::ei_displace_shader(\""<<shader_name <<"\")");
		std::vector<elvishray::OutputBase*>::iterator i= m_receivers.begin();
		std::vector<elvishray::OutputBase*>::iterator e= m_receivers.end();
		for(; i!=e; ++i)
		{
			(*i)->ei_displace_shader(shader_name);
		}
	}
	void OutputMgr::ei_shadow_shader( const char *shader_name )
	{
		//CM_TRACE_FUNC("OutputMgr::ei_shadow_shader(\""<<shader_name <<"\")");
		std::vector<elvishray::OutputBase*>::iterator i= m_receivers.begin();
		std::vector<elvishray::OutputBase*>::iterator e= m_receivers.end();
		for(; i!=e; ++i)
		{
			(*i)->ei_shadow_shader(shader_name);
		}
	}
	void OutputMgr::ei_volume_shader( const char *shader_name )
	{
		//CM_TRACE_FUNC("OutputMgr::ei_volume_shader(\""<<shader_name <<"\")");
		std::vector<elvishray::OutputBase*>::iterator i= m_receivers.begin();
		std::vector<elvishray::OutputBase*>::iterator e= m_receivers.end();
		for(; i!=e; ++i)
		{
			(*i)->ei_volume_shader(shader_name);
		}
	}
	void OutputMgr::ei_environment_shader( const char *shader_name )
	{
		//CM_TRACE_FUNC("OutputMgr::ei_environment_shader(\""<<shader_name <<"\")");
		std::vector<elvishray::OutputBase*>::iterator i= m_receivers.begin();
		std::vector<elvishray::OutputBase*>::iterator e= m_receivers.end();
		for(; i!=e; ++i)
		{
			(*i)->ei_environment_shader(shader_name);
		}
	}

	void OutputMgr::ei_end_material()
	{
		//CM_TRACE_FUNC("OutputMgr::ei_end_material()");
		std::vector<elvishray::OutputBase*>::iterator i= m_receivers.begin();
		std::vector<elvishray::OutputBase*>::iterator e= m_receivers.end();
		for(; i!=e; ++i)
		{
			(*i)->ei_end_material();
		}
	}
	//	Lights:
	void OutputMgr::ei_light( const char *name )
	{
		//CM_TRACE_FUNC("OutputMgr::ei_light(\""<<name<<"\")");
		std::vector<elvishray::OutputBase*>::iterator i= m_receivers.begin();
		std::vector<elvishray::OutputBase*>::iterator e= m_receivers.end();
		for(; i!=e; ++i)
		{
			(*i)->ei_light(name);
		}
	}
	void OutputMgr::ei_light_shader( const char *shader_name )
	{
		//CM_TRACE_FUNC("OutputMgr::ei_light_shader(\""<<shader_name<<"\" )");
		std::vector<elvishray::OutputBase*>::iterator i= m_receivers.begin();
		std::vector<elvishray::OutputBase*>::iterator e= m_receivers.end();
		for(; i!=e; ++i)
		{
			(*i)->ei_light_shader(shader_name);
		}
	}
	void OutputMgr::ei_origin(eiScalar x, eiScalar y, eiScalar z)
	{
		//CM_TRACE_FUNC("OutputMgr::ei_origin("<<x<<","<<y<<","<<z<<")");
		std::vector<elvishray::OutputBase*>::iterator i= m_receivers.begin();
		std::vector<elvishray::OutputBase*>::iterator e= m_receivers.end();
		for(; i!=e; ++i)
		{
			(*i)->ei_origin(x,y,z);
		}
	}
	void OutputMgr::ei_area_samples( eiInt samples )
	{
		//CM_TRACE_FUNC("OutputMgr::ei_area_samples("<<samples<<" )");
		std::vector<elvishray::OutputBase*>::iterator i= m_receivers.begin();
		std::vector<elvishray::OutputBase*>::iterator e= m_receivers.end();
		for(; i!=e; ++i)
		{
			(*i)->ei_area_samples(samples);
		}
	}
	void OutputMgr::ei_adaptive(const eiBool adaptive)
	{
		//CM_TRACE_FUNC("OutputMgr::ei_adaptive("<<adaptive<<" )");
		std::vector<elvishray::OutputBase*>::iterator i= m_receivers.begin();
		std::vector<elvishray::OutputBase*>::iterator e= m_receivers.end();
		for(; i!=e; ++i)
		{
			(*i)->ei_adaptive(adaptive);
		}
	}
	void OutputMgr::ei_end_light()
	{
		//CM_TRACE_FUNC("OutputMgr::ei_end_light()");
		std::vector<elvishray::OutputBase*>::iterator i= m_receivers.begin();
		std::vector<elvishray::OutputBase*>::iterator e= m_receivers.end();
		for(; i!=e; ++i)
		{
			(*i)->ei_end_light();
		}
	}
	//	Objects:
	void OutputMgr::ei_object( const char *type, const char *name )
	{
		//CM_TRACE_FUNC("OutputMgr::ei_object(\""<< type <<"\",\""<<name<<"\")" ); 
		std::vector<elvishray::OutputBase*>::iterator i= m_receivers.begin();
		std::vector<elvishray::OutputBase*>::iterator e= m_receivers.end();
		for(; i!=e; ++i)
		{
			(*i)->ei_object(type, name);
		}
	}
	void OutputMgr::ei_box(eiScalar xmin, eiScalar ymin, eiScalar zmin, 
		eiScalar xmax, eiScalar ymax, eiScalar zmax)
	{
		//CM_TRACE_FUNC("OutputMgr::ei_box("<<xmin<<","<<ymin<<","<<zmin<<", "<<xmax<<","<<ymax<<","<<zmax<<")");
		std::vector<elvishray::OutputBase*>::iterator i= m_receivers.begin();
		std::vector<elvishray::OutputBase*>::iterator e= m_receivers.end();
		for(; i!=e; ++i)
		{
			(*i)->ei_box(xmin,ymin,zmin,xmax,ymax,zmax);
		}

	}
	void OutputMgr::ei_motion_box(eiScalar xmin, eiScalar ymin, eiScalar zmin, 
		eiScalar xmax, eiScalar ymax, eiScalar zmax)
	{
		//CM_TRACE_FUNC("OutputMgr::ei_motion_box("<< xmin<<","<<ymin<<","<<zmin<<", "<<xmax<<","<<ymax<<","<<zmax<<")");
		std::vector<elvishray::OutputBase*>::iterator i= m_receivers.begin();
		std::vector<elvishray::OutputBase*>::iterator e= m_receivers.end();
		for(; i!=e; ++i)
		{
			(*i)->ei_motion_box(xmin,ymin,zmin, xmax,ymax,zmax);
		}
	}
	/* procedural objects */
	void OutputMgr::ei_geometry_shader(const char *shader_name)
	{
		//CM_TRACE_FUNC("OutputMgr::ei_geometry_shader(\""<<shader_name<<"\")");
		std::vector<elvishray::OutputBase*>::iterator i= m_receivers.begin();
		std::vector<elvishray::OutputBase*>::iterator e= m_receivers.end();
		for(; i!=e; ++i)
		{
			(*i)->ei_geometry_shader(shader_name);
		}
	}
	void OutputMgr::ei_pos_list(const eiTag tab)
	{
		//CM_TRACE_FUNC("OutputMgr::ei_pos_list(tag)" );
		std::vector<elvishray::OutputBase*>::iterator i= m_receivers.begin();
		std::vector<elvishray::OutputBase*>::iterator e= m_receivers.end();
		for(; i!=e; ++i)
		{
			(*i)->ei_pos_list(tab);
		}
	}
	void OutputMgr::ei_triangle_list(const eiTag tab)
	{
		//CM_TRACE_FUNC("OutputMgr::ei_triangle_list(tag)" );
		std::vector<elvishray::OutputBase*>::iterator i= m_receivers.begin();
		std::vector<elvishray::OutputBase*>::iterator e= m_receivers.end();
		for(; i!=e; ++i)
		{
			(*i)->ei_triangle_list(tab);
		}
	}
	void OutputMgr::ei_end_object()
	{
		//CM_TRACE_FUNC("OutputMgr::ei_end_object()"); 
		std::vector<elvishray::OutputBase*>::iterator i= m_receivers.begin();
		std::vector<elvishray::OutputBase*>::iterator e= m_receivers.end();
		for(; i!=e; ++i)
		{
			(*i)->ei_end_object();
		}
	}


	//	Instances:
	void OutputMgr::ei_instance( const char *name )
	{
		//CM_TRACE_FUNC("OutputMgr::ei_instance(\""<<name<<"\")");
		std::vector<elvishray::OutputBase*>::iterator i= m_receivers.begin();
		std::vector<elvishray::OutputBase*>::iterator e= m_receivers.end();
		for(; i!=e; ++i)
		{
			(*i)->ei_instance(name);
		}
	}
	void OutputMgr::ei_element( const char *element_name )
	{
		//CM_TRACE_FUNC("OutputMgr::ei_element(\""<<element_name<<"\")");
		std::vector<elvishray::OutputBase*>::iterator i= m_receivers.begin();
		std::vector<elvishray::OutputBase*>::iterator e= m_receivers.end();
		for(; i!=e; ++i)
		{
			(*i)->ei_element(element_name);
		}
	}
	void OutputMgr::ei_transform( 
		float t00,float t01,float t02,float t03,
		float t10,float t11,float t12,float t13,
		float t20,float t21,float t22,float t23,
		float t30,float t31,float t32,float t33 
		)
	{
		//CM_TRACE_FUNC("OutputMgr::ei_transform("<< t00<<","<<t01<<","<<t02<<","<<t03<<",   "<<t10<<","<<t11<<","<<t12<<","<<t13<<",   "<<  t20<<","<<t21<<","<<t22<<","<<t23<<",   "<<  t30<<","<<t31<<","<<t32<<","<<t33 <<")");
		std::vector<elvishray::OutputBase*>::iterator i= m_receivers.begin();
		std::vector<elvishray::OutputBase*>::iterator e= m_receivers.end();
		for(; i!=e; ++i)
		{
			(*i)->ei_transform(t00,t01,t02,t03,t10,t11,t12,t13,t20,t21,t22,t23,t30,t31,t32,t33);
		}
	}
	void OutputMgr::ei_motion_transform( 
		float t00,float t01,float t02,float t03,
		float t10,float t11,float t12,float t13,
		float t20,float t21,float t22,float t23,
		float t30,float t31,float t32,float t33
		)
	{
		//CM_TRACE_FUNC("OutputMgr::ei_motion_transform("<< t00<<","<<t01<<","<<t02<<","<<t03<<",   "<<t10<<","<<t11<<","<<t12<<","<<t13<<",   "<<  t20<<","<<t21<<","<<t22<<","<<t23<<",   "<<  t30<<","<<t31<<","<<t32<<","<<t33 <<")");
		std::vector<elvishray::OutputBase*>::iterator i= m_receivers.begin();
		std::vector<elvishray::OutputBase*>::iterator e= m_receivers.end();
		for(; i!=e; ++i)
		{
			(*i)->ei_motion_transform(t00,t01,t02,t03,t10,t11,t12,t13,t20,t21,t22,t23,t30,t31,t32,t33);
		}
	}
	void OutputMgr::ei_mtl( const char *material_name )
	{
		//CM_TRACE_FUNC("OutputMgr::ei_mtl(\""<<material_name<<"\")");
		std::vector<elvishray::OutputBase*>::iterator i= m_receivers.begin();
		std::vector<elvishray::OutputBase*>::iterator e= m_receivers.end();
		for(; i!=e; ++i)
		{
			(*i)->ei_mtl(material_name);
		}
	}

	void OutputMgr::ei_end_instance()
	{
		//CM_TRACE_FUNC("OutputMgr::ei_end_instance()");
		std::vector<elvishray::OutputBase*>::iterator i= m_receivers.begin();
		std::vector<elvishray::OutputBase*>::iterator e= m_receivers.end();
		for(; i!=e; ++i)
		{
			(*i)->ei_end_instance();
		}
	}


	//	Instance Groups:
	void OutputMgr::ei_instgroup( const char *name )
	{
		//CM_TRACE_FUNC("OutputMgr::ei_instgroup(\""<<name<<"\")");
		std::vector<elvishray::OutputBase*>::iterator i= m_receivers.begin();
		std::vector<elvishray::OutputBase*>::iterator e= m_receivers.end();
		for(; i!=e; ++i)
		{
			(*i)->ei_instgroup(name);
		}
	}
	void OutputMgr::ei_add_instance( const char *name )
	{
		//CM_TRACE_FUNC("OutputMgr::ei_add_instance(\""<<name<<"\")");
		std::vector<elvishray::OutputBase*>::iterator i= m_receivers.begin();
		std::vector<elvishray::OutputBase*>::iterator e= m_receivers.end();
		for(; i!=e; ++i)
		{
			(*i)->ei_add_instance(name);
		}
	}
	void OutputMgr::ei_end_instgroup()
	{
		//CM_TRACE_FUNC("OutputMgr::ei_end_instgroup( )");
		std::vector<elvishray::OutputBase*>::iterator i= m_receivers.begin();
		std::vector<elvishray::OutputBase*>::iterator e= m_receivers.end();
		for(; i!=e; ++i)
		{
			(*i)->ei_end_instgroup();
		}
	}
	//
	void OutputMgr::ei_shader(const char *type_name, const char *instance_name)
	{
		//CM_TRACE_FUNC("OutputMgr::ei_shader(\""<<type_name<<"\", \""<<instance_name<<"\")");
		std::vector<elvishray::OutputBase*>::iterator i= m_receivers.begin();
		std::vector<elvishray::OutputBase*>::iterator e= m_receivers.end();
		for(; i!=e; ++i)
		{
			(*i)->ei_shader(type_name, instance_name);
		}
	}
	void OutputMgr::ei_shader_param(
		const char *param_name, 
		const void *param_value)
	{
		//CM_TRACE_FUNC("OutputMgr::ei_shader_param(\""<<param_name<<"\", \""<<param_value<<"\") not implemented");
		std::vector<elvishray::OutputBase*>::iterator i= m_receivers.begin();
		std::vector<elvishray::OutputBase*>::iterator e= m_receivers.end();
		for(; i!=e; ++i)
		{
			(*i)->ei_shader_param(param_name,param_value);
		}
	}
	void OutputMgr::ei_shader_param_token(
		const char *param_name, 
		const char *param_value)
	{
		//CM_TRACE_FUNC("OutputMgr::ei_shader_param_token(\""<<param_name<<"\", \""<<param_value<<"\") not implemented");
		std::vector<elvishray::OutputBase*>::iterator i= m_receivers.begin();
		std::vector<elvishray::OutputBase*>::iterator e= m_receivers.end();
		for(; i!=e; ++i)
		{
			(*i)->ei_shader_param_token(param_name,param_value);
		}
	}
	void OutputMgr::ei_shader_param_int(
		const char *param_name, 
		const eiInt param_value)
	{
		//CM_TRACE_FUNC("OutputMgr::ei_shader_param_int(\""<<param_name<<"\","<<param_value<<")");
		std::vector<elvishray::OutputBase*>::iterator i= m_receivers.begin();
		std::vector<elvishray::OutputBase*>::iterator e= m_receivers.end();
		for(; i!=e; ++i)
		{
			(*i)->ei_shader_param_int(param_name,param_value);
		}
	}
	void OutputMgr::ei_shader_param_scalar(
		const char *param_name, 
		const eiScalar param_value)
	{
		//CM_TRACE_FUNC("OutputMgr::ei_shader_param_scalar(\""<<param_name<<"\","<<param_value<<")");
		std::vector<elvishray::OutputBase*>::iterator i= m_receivers.begin();
		std::vector<elvishray::OutputBase*>::iterator e= m_receivers.end();
		for(; i!=e; ++i)
		{
			(*i)->ei_shader_param_scalar(param_name,param_value);
		}
	}
	void OutputMgr::ei_shader_param_vector(
		const char *param_name, 
		const eiScalar x, const eiScalar y, const eiScalar z)
	{
		//CM_TRACE_FUNC("OutputMgr::ei_shader_param_vector(\""<<param_name<<"\","<<x<<","<<y<<","<<z<<")");
		std::vector<elvishray::OutputBase*>::iterator i= m_receivers.begin();
		std::vector<elvishray::OutputBase*>::iterator e= m_receivers.end();
		for(; i!=e; ++i)
		{
			(*i)->ei_shader_param_vector(param_name,x,y,z);
		}
	}
	void OutputMgr::ei_shader_param_vector4(
		const char *param_name, 
		const eiScalar x, const eiScalar y, const eiScalar z, const eiScalar w)
	{
		//CM_TRACE_FUNC("OutputMgr::ei_shader_param_vector4(\""<<param_name<<"\","<<x<<","<<y<<","<<z<<","<<w<<")");
		std::vector<elvishray::OutputBase*>::iterator i= m_receivers.begin();
		std::vector<elvishray::OutputBase*>::iterator e= m_receivers.end();
		for(; i!=e; ++i)
		{
			(*i)->ei_shader_param_vector4(param_name,x,y,z,w);
		}
	}
	void OutputMgr::ei_shader_param_tag(
		const char *param_name, 
		const eiTag param_value)
	{
		//CM_TRACE_FUNC("OutputMgr::ei_shader_param_tag(\""<<param_name<<"\","<<param_value<<")");
		std::vector<elvishray::OutputBase*>::iterator i= m_receivers.begin();
		std::vector<elvishray::OutputBase*>::iterator e= m_receivers.end();
		for(; i!=e; ++i)
		{
			(*i)->ei_shader_param_tag(param_name,param_value);
		}
	}
	void OutputMgr::ei_shader_param_texture(
		const char *param_name, 
		const char *texture_name)
	{
		//CM_TRACE_FUNC("OutputMgr::ei_shader_param_texture(\""<<param_name<<"\",\""<<texture_name<<"\")");
		std::vector<elvishray::OutputBase*>::iterator i= m_receivers.begin();
		std::vector<elvishray::OutputBase*>::iterator e= m_receivers.end();
		for(; i!=e; ++i)
		{
			(*i)->ei_shader_param_texture(param_name,texture_name);
		}
	}
	void OutputMgr::ei_shader_param_index(
		const char *param_name, 
		const eiIndex param_value)
	{
		//CM_TRACE_FUNC("OutputMgr::ei_shader_param_index(\""<<param_name<<"\","<<param_value<<")");
		std::vector<elvishray::OutputBase*>::iterator i= m_receivers.begin();
		std::vector<elvishray::OutputBase*>::iterator e= m_receivers.end();
		for(; i!=e; ++i)
		{
			(*i)->ei_shader_param_index(param_name,param_value);
		}
	}
	void OutputMgr::ei_shader_param_bool(
		const char *param_name, 
		const eiBool param_value)
	{
		//CM_TRACE_FUNC("OutputMgr::ei_shader_param_bool(\""<<param_name<<"\","<<param_value<<")");
		std::vector<elvishray::OutputBase*>::iterator i= m_receivers.begin();
		std::vector<elvishray::OutputBase*>::iterator e= m_receivers.end();
		for(; i!=e; ++i)
		{
			(*i)->ei_shader_param_bool(param_name,param_value);
		}
	}
	void OutputMgr::ei_shader_link_param(
		const char *param_name, 
		const char *src_shader_name, 
		const char *src_param_name)
	{
		//CM_TRACE_FUNC("OutputMgr::ei_shader_link_param(\""<<param_name<<"\",\""<<src_shader_name<<"\",\""<<src_param_name<<"\")");
		std::vector<elvishray::OutputBase*>::iterator i= m_receivers.begin();
		std::vector<elvishray::OutputBase*>::iterator e= m_receivers.end();
		for(; i!=e; ++i)
		{
			(*i)->ei_shader_link_param(param_name,src_shader_name,src_param_name);
		}
	}
	void OutputMgr::ei_end_shader()
	{
		//CM_TRACE_FUNC("OutputMgr::ei_end_shader()");
		std::vector<elvishray::OutputBase*>::iterator i= m_receivers.begin();
		std::vector<elvishray::OutputBase*>::iterator e= m_receivers.end();
		for(; i!=e; ++i)
		{
			(*i)->ei_end_shader();
		}
	}

	void OutputMgr::ei_declare(const char *name, const eiInt storage_class, const eiInt type/*, const void *tag*/)
	{
		//CM_TRACE_FUNC("OutputMgr::ei_declare(\""<<name<<"\","<<storage_class<<","<<type<<", &tag)");
		liquidMessage2(messageError, "OutputMgr::ei_declare(&tag)");
		std::vector<elvishray::OutputBase*>::iterator i= m_receivers.begin();
		std::vector<elvishray::OutputBase*>::iterator e= m_receivers.end();
		for(; i!=e; ++i)
		{
			(*i)->ei_declare(name,storage_class,type/*, tag*/);
		}
	}
	void OutputMgr::ei_variable(const char *name, const void *tag)
	{
		//CM_TRACE_FUNC("OutputMgr::ei_variable(\""<<name<<"\", &tag)");
		liquidMessage2(messageError, "OutputMgr::ei_declare(&tag)");
		std::vector<elvishray::OutputBase*>::iterator i= m_receivers.begin();
		std::vector<elvishray::OutputBase*>::iterator e= m_receivers.end();
		for(; i!=e; ++i)
		{
			(*i)->ei_variable(name, tag);
		}
	}
	void OutputMgr::ei_degree(const eiInt degree)
	{
		//CM_TRACE_FUNC("OutputMgr::ei_degree("<<degree<<")");
		std::vector<elvishray::OutputBase*>::iterator i= m_receivers.begin();
		std::vector<elvishray::OutputBase*>::iterator e= m_receivers.end();
		for(; i!=e; ++i)
		{
			(*i)->ei_degree(degree);
		}
	}

	//----------------------------------------------------
	// Warped ER API interfaces
	//----------------------------------------------------
	void OutputMgr::liq_lightgroup_in_object_instance(const char *light_group_name)
	{
		//CM_TRACE_FUNC("OutputMgr::liq_lightgroup_in_object_instance("<<light_group_name<<")");
		std::vector<elvishray::OutputBase*>::iterator i= m_receivers.begin();
		std::vector<elvishray::OutputBase*>::iterator e= m_receivers.end();
		for(; i!=e; ++i)
		{
			(*i)->liq_lightgroup_in_object_instance(light_group_name);
		}
	}
	void OutputMgr::liq_lightgroup_in_light_instance_beg()
	{
		//CM_TRACE_FUNC("OutputMgr::liq_lightgroup_in_light_instance_beg()");
		std::vector<elvishray::OutputBase*>::iterator i= m_receivers.begin();
		std::vector<elvishray::OutputBase*>::iterator e= m_receivers.end();
		for(; i!=e; ++i)
		{
			(*i)->liq_lightgroup_in_light_instance_beg();
		}
	}
	void OutputMgr::liq_lightgroup_in_light_instance(const char *light_group_name)
	{
		//CM_TRACE_FUNC("OutputMgr::liq_lightgroup_in_light_instance("<<light_group_name<<")");
		std::vector<elvishray::OutputBase*>::iterator i= m_receivers.begin();
		std::vector<elvishray::OutputBase*>::iterator e= m_receivers.end();
		for(; i!=e; ++i)
		{
			(*i)->liq_lightgroup_in_light_instance(light_group_name);
		}
	}
	void OutputMgr::liq_lightgroup_in_light_instance_end()
	{
		//CM_TRACE_FUNC("OutputMgr::liq_lightgroup_in_light_instance_end()");
		std::vector<elvishray::OutputBase*>::iterator i= m_receivers.begin();
		std::vector<elvishray::OutputBase*>::iterator e= m_receivers.end();
		for(; i!=e; ++i)
		{
			(*i)->liq_lightgroup_in_light_instance_end();
		}
	}
	void OutputMgr::liq_object(
		const std::string &objname,
		const std::vector<MVector> &POSITION,
		const std::vector<MVector> &POSITION_mb,//motion blur position
		const std::vector<std::size_t> &INDEX,//global vertex index
		const std::vector<MVector> &NORMAL,
		const std::vector<MVector> &UV)
	{
		//CM_TRACE_FUNC("OutputMgr::liq_object("<<light_group_name<<")");
		std::vector<elvishray::OutputBase*>::iterator i= m_receivers.begin();
		std::vector<elvishray::OutputBase*>::iterator e= m_receivers.end();
		for(; i!=e; ++i)
		{
			(*i)->liq_object(objname, POSITION, POSITION_mb, INDEX, NORMAL, UV);
		}
	}
	void OutputMgr::liq_hair(
		const std::string &objname,
		MFnPfxGeometry &fnPfxGeometry,
		int degree,
		unsigned setOn)
	{
		//CM_TRACE_FUNC("OutputMgr::liq_hair("<<objname<<",...)");
		std::vector<elvishray::OutputBase*>::iterator i= m_receivers.begin();
		std::vector<elvishray::OutputBase*>::iterator e= m_receivers.end();
		for(; i!=e; ++i)
		{
			(*i)->liq_hair(objname, fnPfxGeometry, degree, setOn);
		}
	}
}//namespace elvishray
#endif//_USE_ELVISHRAY_