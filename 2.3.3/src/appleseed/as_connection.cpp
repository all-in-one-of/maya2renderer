#include "as_connection.h"
#include "../common/mayacheck.h"
#include <trace/trace.hpp>
#include "as_renderer.h"
#include "as_log_helper.h"

namespace appleseed
{
	typedef float ChannelType;

	void color128to64( asf::Color3f & color, ChannelType &r, ChannelType &g, ChannelType &b, ChannelType &a )
	{
		asf::Color3f c = asf::clamp( color, 0.0f, 1.0f );

		r = ( 255.0f * c[0] );
		g = ( 255.0f * c[1] );
		b = ( 255.0f * c[2] );
		a = 255.0f;
	}
	//
	void setPixel(RV_PIXEL*pixels, 
		const unsigned int tile_width, const unsigned int tile_height, 
		const int ix, const int iy, 
		const ChannelType r, const ChannelType g, const ChannelType b, const ChannelType a)
	{
		unsigned int offset = iy*tile_width + ix;
		RV_PIXEL *target = pixels + offset;
		target->r = r;
		target->g = g;
		target->b = b;
		target->a = a;
	}
	void setPixel(RV_PIXEL*pixels, 
		const unsigned int tile_width, 
		const unsigned int tile_height, 
		const int ix, const int iy, 
		const RV_PIXEL &color )
	{
		setPixel(pixels, tile_width, tile_height, ix, iy, color.r, color.g, color.b, color.a );
	}
	//////////////////////////////////////////////////////////////////////////
	Connection* Connection::m_instance = NULL;
	//Singleton pattern interface
	Connection::Connection()
	{
		//_logFunctionCall("Connection::Connection()");
	}
	//
	Connection::~Connection()
	{
		//_logFunctionCall("Connection::~Connection()");
	}
	//
	Connection* Connection::getInstance()
	{
		//_logFunctionCall("Connection::getInstance()");
		if(!m_instance){
			m_instance = new Connection();
		}
		return m_instance;
	}
	//
	void Connection::delInstance()
	{
		//_logFunctionCall("Connection::delInstance()");
		if(m_instance){
			delete m_instance;
			m_instance = NULL;
		}
	}
	//////////////////////////////////////////////////////////////////////////
	MStatus Connection::startRender( unsigned int width, unsigned int height,
		bool doNotClearBackground ,
		bool immediateFeedback)
	{
		CM_TRACE_FUNC("Connection::startRender("<<width<<","<<height<<")");

		return MRenderView::startRender( width, height, doNotClearBackground, immediateFeedback);
	}
	//
	MStatus Connection::endRender()
	{
		CM_TRACE_FUNC("Connection::endRender()");
		return MRenderView::endRender();
	}
	//
	void Connection::render(
		const asf::auto_release_ptr<asr::Project> &project,
		const asr::ParamArray &configuration,
		const bool bBatchMode
		)
	{
		CM_TRACE_FUNC("Renderer::render(..., batch="<<bBatchMode<<")");
		
		asr::DefaultRendererController renderer_controller;

		if(bBatchMode)
		{
			// Create the master renderer.
			asr::MasterRenderer renderer(
				project.ref()
				,configuration
				,&renderer_controller
				);
			// Render the frame.
			renderer.render();
		}else{

			QtTileCallbackFactory m_tile_callback_factory(true/*highlight_tiles*/);
			// Create the master renderer.
			asr::MasterRenderer renderer(
				project.ref()
				,configuration
				,&renderer_controller
				,&m_tile_callback_factory);
			// Render the frame.
			renderer.render();
		}

	}
	//
	void UpdateTile(
		const asr::Frame&    frame,
		const size_t    tile_x,
		const size_t    tile_y,
		asf::uint8*          float_tile_storage,
		asf::uint8*          uint8_tile_storage
		)
	{
		CM_TRACE_FUNC("UpdateTile(frame, "<<tile_x<<","<<tile_y<<",...)");
		MStatus status;

		const asf::Tile& tile = frame.image().tile(tile_x, tile_y);

		// Convert the tile to 32-bit floating point.
		asf::Tile fp_rgb_tile(
			tile,
			asf::PixelFormatFloat,
			float_tile_storage);
		// Transform the tile to the color space of the frame.
		frame.transform_to_output_color_space(fp_rgb_tile);
		// Convert the tile to 8-bit RGB for display.
		static const size_t ShuffleTable[] = { 0, 1, 2, asf::Pixel::SkipChannel };
		const asf::Tile uint8_rgb_tile(
			fp_rgb_tile,
			asf::PixelFormatUInt8,
			ShuffleTable,
			uint8_tile_storage);
		
		std::size_t tile_width  = uint8_rgb_tile.get_width();
		std::size_t tile_height = uint8_rgb_tile.get_height();
		const std::size_t channel_count = uint8_rgb_tile.get_channel_count();

		RV_PIXEL* pixels = new RV_PIXEL[tile_width*tile_height];

		for (std::size_t j = 0; j < tile_height; ++j)
		{
			for (std::size_t i = 0; i < tile_width; ++i)
			{
				ChannelType r,g,b,a;
				asf::Color3f color;

				uint8_rgb_tile.get_pixel(i, j, color);

				color128to64(color, r,g,b,a);

				setPixel(pixels, tile_width, tile_height,
					i, tile_height-j-1,
					r,g,b,a);
			}
		}
		// 	_LogDebug("frame buffer<"<<colorFrameBuffer->get_width()<<","<<colorFrameBuffer->get_height()<<">" );
		// 	_LogDebug("width*height = "<<colorFrameBuffer->get_width() * colorFrameBuffer->get_height() );
		// 	_LogDebug("index = "<< index);
		const std::size_t image_width  = frame.image().properties().m_canvas_width;
		const std::size_t image_height = frame.image().properties().m_canvas_height;
		_s( "//image width/height="<<image_width<<","<< image_height);

		std::size_t min_x, min_y, max_x, max_y;
		min_x = tile_x*tile_width;
		min_y = tile_y*tile_height;
		max_x = min_x + tile_width;
		max_y = min_y + tile_height;
		_s( "//min["<<min_x<<","<<min_y<<"], max["<<max_x<<","<<max_y<<"]");
		_s( "//updatePixels["<<min_x<<","<<max_x-1<<", "<<image_height - min_y<<","<<image_height -(max_y-1)<<"]");
		// Send the data to the render view.
		if ( (status = MRenderView::updatePixels(min_x, max_x-1, image_height-max_y, image_height-min_y-1, pixels)) != MS::kSuccess)
		{
			IfErrorWarn(status);
			CM_TRACE_FUNC( "MayaConnection: error occured in updatePixels." );
			delete [] pixels;
			return ;
		}
		delete [] pixels;
		// Force the Render View to refresh the display of the affected region.
		if ( (status = MRenderView::refresh(min_x, max_x-1, image_height-max_y, image_height-min_y-1)) != MS::kSuccess)
		{
			IfErrorWarn(status);
			CM_TRACE_FUNC( "MayaConnection: error occured in refresh." );
			return ;
		}

	}

}//namespace appleseed