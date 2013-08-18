/*---------------------------------------------------------------------------------------------
    This file is part of Luna - the elvish ray renderer plug-in for Maya.
    Copyright (C) 2010  yaoyansi

    Luna is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Luna is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
---------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------

File		:	MayaConnection.cpp
Classes		:	
Description	:	
Comments	:	
History		:	Created by yaoyansi, 2010.04.16.

---------------------------------------------------------------------------------------------*/
#include "MayaConnection.h"

//#include "../common/prerequest_local.h"
#include <common/mayacheck.h>
#include "liqRibTranslator.h"

typedef float ChannelType;

void color128to64( eiScalar & from, ChannelType &to )
{
	clampi( from, 0.0f, 1.0f );
	to = ( 255.0f * from );
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
//
void MayaConnection::Print( const eiInt severity, const char *message )
{
	//_logFunctionCall("MayaConnection::Print()");

	static const int	NUM_SEVERITY_LEVELS = 5;
	static const char	*severity_strings[ NUM_SEVERITY_LEVELS ] = { "FATAL",
		"ERROR",
		"WARNING",
		"PROGRESS",
		"DEBUG" };

	if( severity < 1 || severity > NUM_SEVERITY_LEVELS )
		return;

	printf("%s : %s\n", severity_strings[severity - 1], message);
}
//
bool MayaConnection::Progress( const eiScalar percent )
{
	//_logFunctionCall("MayaConnection::Progress(" << percent << " %)");
	m_percent = percent/100.0f;

	bool isInterrupted = liqRibTranslator::getInstancePtr()->isInterruptRequested();
	if (isInterrupted){
		liquidMessage2(messageInfo, "Liquid is interrputed at %f%%.\n", percent);
	}
	return !isInterrupted;
}
//
void MayaConnection::ClearTile( const eiInt left, const eiInt right, 
							   const eiInt top, const eiInt bottom,
							   const eiHostID host )
{
	//_logFunctionCall("MayaConnection::ClearTile()");
	if( !isInteractiveRenderingMode() )
		return;

	//
	MStatus status;

	const RV_PIXEL BACKGROUND={         (1.0f-m_percent)*255.0f,   0.0f,   0.0f, 125.0f};
	const RV_PIXEL MARK={fmodf(((float)host)/3.14f,1.0f)*255.0f, 255.0f, 255.0f, 255.0f};

	const unsigned int tile_width  = (right>left)?(right-left):(left-right);
	const unsigned int tile_height = (bottom>top)?(bottom-top):(top-bottom);
	assert( tile_width>=1 && tile_height>=1 );

	unsigned int min_x, min_y, max_x, max_y;
	getMin(min_x, min_y, left, right, bottom, top);
	max_x = min_x + tile_width;
	max_y = min_y + tile_height;
	if(max_x>width || max_y>height){
		return;
	}

#define TARGET_LEN 4
	RV_PIXEL pixels[TARGET_LEN];
	for(std::size_t i = 0; i<TARGET_LEN; ++i)
	{
		pixels[i].r = MARK.r;
		pixels[i].g = MARK.g;
		pixels[i].b = MARK.b;
		pixels[i].a = MARK.a;
	}

	//set mark
	IfMErrorWarn(MRenderView::updatePixels(min_x, min_x, min_y, min_y+TARGET_LEN-1, pixels));
	IfMErrorWarn(MRenderView::updatePixels(min_x, min_x+TARGET_LEN-1, min_y, min_y, pixels));

	IfMErrorWarn(MRenderView::updatePixels(max_x-1, max_x-1, min_y, min_y+TARGET_LEN-1, pixels));
	IfMErrorWarn(MRenderView::updatePixels(max_x-TARGET_LEN, max_x-1, min_y, min_y, pixels));

	IfMErrorWarn(MRenderView::updatePixels(min_x, min_x, max_y-TARGET_LEN, max_y-1, pixels));
	IfMErrorWarn(MRenderView::updatePixels(min_x, min_x+TARGET_LEN-1, max_y-1, max_y-1, pixels));

	IfMErrorWarn(MRenderView::updatePixels(max_x-TARGET_LEN, max_x-1, max_y-1, max_y-1, pixels));
	IfMErrorWarn(MRenderView::updatePixels(max_x-1, max_x-1, max_y-TARGET_LEN, max_y-1, pixels));

	// Force the Render View to refresh the display of the affected region.
	IfMErrorWarn(MRenderView::refresh(min_x, min_x, min_y, min_y+TARGET_LEN-1));
	IfMErrorWarn(MRenderView::refresh(min_x, min_x+TARGET_LEN-1, min_y, min_y));

	IfMErrorWarn(MRenderView::refresh(max_x-1, max_x-1, min_y, min_y+TARGET_LEN-1));
	IfMErrorWarn(MRenderView::refresh(max_x-TARGET_LEN, max_x-1, min_y, min_y));

	IfMErrorWarn(MRenderView::refresh(min_x, min_x, max_y-TARGET_LEN, max_y-1));
	IfMErrorWarn(MRenderView::refresh(min_x, min_x+TARGET_LEN-1, max_y-1, max_y-1));

	IfMErrorWarn(MRenderView::refresh(max_x-TARGET_LEN, max_x-1, max_y-1, max_y-1));
	IfMErrorWarn(MRenderView::refresh(max_x-1, max_x-1, max_y-TARGET_LEN, max_y-1));
#undef TARGET_LEN
}
// Note:
// the tile of elvishray range from [left right) to [top bottom)
void MayaConnection::UpdateTile( const eiInt job_state, 
								eiFrameBufferCache *colorFrameBuffer, 
								eiFrameBufferCache *opacityFrameBuffer, 
								ei_array *frameBuffers, 
								const eiInt left, const eiInt right, 
								const eiInt top, const eiInt bottom )
{
	//_logFunctionCall("MayaConnection::UpdateTile(...)");
	MStatus status;

	if( !isInteractiveRenderingMode() )
		return;

	const unsigned int tile_width  = (right>left)?(right-left):(left-right);
	const unsigned int tile_height = (bottom>top)?(bottom-top):(top-bottom);
	
	assert( tile_width>=1 && tile_height>=1 );
	//_LogDebug("tile<"<<tile_width <<", "<< tile_height<<">, w*h="<< tile_width*tile_height);

	
	RV_PIXEL* pixels = new RV_PIXEL[(tile_width)*(tile_height)];

	const eiInt colorBufH = ei_framebuffer_cache_get_height(colorFrameBuffer);
	const eiInt colorBufW = ei_framebuffer_cache_get_width(colorFrameBuffer);
	for (int j = 0; j < colorBufH; ++j)
	{
		for (int i = 0; i < colorBufW; ++i)
		{
			//rgb
			eiVector color;
			ChannelType r,g,b;
			ei_framebuffer_cache_get_final(colorFrameBuffer, i, j, &color);
			color128to64(color.r, r);
			color128to64(color.g, g);
			color128to64(color.b, b);
			//alpha
			eiVector opacity;
			ChannelType a;
			ei_framebuffer_cache_get_final(opacityFrameBuffer, i, j, &opacity);
			eiScalar average((opacity.r+opacity.g+opacity.b)/3.0f);
			color128to64( average, a);
			//
			setPixel(pixels, tile_width, tile_height,
				i, tile_height-j-1,
				r,g,b,a);
		}
	}
	//const eiInt opacityBufH = ei_framebuffer_cache_get_height(opacityFrameBuffer);
	//const eiInt opacityBufW = ei_framebuffer_cache_get_width(opacityFrameBuffer);
	//if( colorBufH != opacityBufH ||  colorBufW != opacityBufW )
	//{
	//	liquidMessage2(messageError, "color tile<%f,%f>, opacity tile<%f, %f>",colorBufW, colorBufH, opacityBufW, opacityBufH);
	//}
	//for (int j = 0; j < opacityBufH; ++j)
	//{
	//	for (int i = 0; i < opacityBufW; ++i)
	//	{
	//		eiVector opacity;
	//		ChannelType a;
	//		ei_framebuffer_cache_get_final(opacityFrameBuffer, i, j, &opacity);
	//		eiScalar average((opacity.r+opacity.g+opacity.b)/3.0f);
	//		color128to64( average, a);

	//		setPixelA(pixels, tile_width, tile_height,
	//			i, tile_height-j-1,
	//			a);
	//	}
	//}
// 	_LogDebug("frame buffer<"<<colorFrameBuffer->get_width()<<","<<colorFrameBuffer->get_height()<<">" );
// 	_LogDebug("width*height = "<<colorFrameBuffer->get_width() * colorFrameBuffer->get_height() );
// 	_LogDebug("index = "<< index);

	unsigned int min_x, min_y, max_x, max_y;
	getMin(min_x, min_y, left, right, bottom, top);

	max_x = min_x + tile_width;
	max_y = min_y + tile_height;

	// Send the data to the render view.
	if ( (status = MRenderView::updatePixels(min_x, max_x-1, min_y, max_y-1, pixels)) != MS::kSuccess)
	{
		liquidMessage2(messageError, "MRenderView::updatePixels(%d,%d,%d,%d,...)", min_x, max_x-1, min_y, max_y-1);
		IfMErrorWarn(status);
		//_LogError( "MayaConnection: error occured in updatePixels." );
		delete [] pixels;
		return ;
	}
	delete [] pixels;
	// Force the Render View to refresh the display of the affected region.
	if ( (status = MRenderView::refresh(min_x, max_x-1, min_y, max_y-1)) != MS::kSuccess)
	{
		liquidMessage2(messageError, "MRenderView::refresh(%d,%d,%d,%d,...)", min_x, max_x-1, min_y, max_y-1);
		IfMErrorWarn(status);
		//_LogError( "MayaConnection: error occured in refresh." );
		return ;
	}
}
//
void MayaConnection::DrawPixel( const eiInt x, const eiInt y, const eiVector *_color )
{	
	//_logFunctionCall("MayaConnection::DrawPixel()");

	if( !isInteractiveRenderingMode() )
		return;

	liquidMessage2(messageWarning, "MayaConnection::DrawPixel(...) is not implemented yet.");
}
//
void MayaConnection::UpdateSubWindow( const eiInt left, const eiInt right, 
									 const eiInt top, const eiInt bottom )
{
	//_logFunctionCall("MayaConnection::UpdateSubWindow()");

	if( !isInteractiveRenderingMode() )
		return;

	liquidMessage2(messageWarning, "MayaConnection::UpdateSubWindow(...) is not implemented yet.");
}

MStatus MayaConnection::startRender( unsigned int w, unsigned int h,
					bool doNotClearBackground ,
					bool immediateFeedback)
{
	//_logFunctionCall("MayaConnection::startRender()");
	width  = w;
	height = h;
	m_percent = 0.0f;

	if( liqglo.m_renderViewCrop )
	{
		return startRegionRender( width, height, 
			doNotClearBackground, immediateFeedback);
	} else {
		return MRenderView::startRender( width, height, 
			doNotClearBackground, immediateFeedback);
	}
}
//
MStatus MayaConnection::startRegionRender(
	unsigned int w, unsigned int h,
	bool doNotClearBackground ,
	bool immediateFeedback)
{
	//_logFunctionCall("MayaConnection::startRenderRegion()");
	width  = w;
	height = h;
	m_percent = 0.0f;

	unsigned int left, right, bottom, top;
	MRenderView::getRenderRegion(left, right, bottom, top);

	return MRenderView::startRegionRender( width, height, left, right, bottom, top, 
		doNotClearBackground, immediateFeedback);
}
//
MStatus MayaConnection::endRender()
{
	//_logFunctionCall("MayaConnection::endRender()");
	width  = 0;
	height = 0;
	return MRenderView::endRender();
}
//
//
MayaConnection* MayaConnection::m_instance = NULL;
MayaConnection::MayaConnection()
{
	//_logFunctionCall("MayaConnection::MayaConnection()");
	setConnection();
}
//
MayaConnection::~MayaConnection()
{
	//_logFunctionCall("MayaConnection::~MayaConnection()");
}
//
MayaConnection* MayaConnection::getInstance()
{
	//_logFunctionCall("MayaConnection::getInstance()");
	if(!m_instance){
		m_instance = new MayaConnection();
	}
	return m_instance;
}
//
void MayaConnection::delInstance()
{
	//_logFunctionCall("MayaConnection::delInstance()");
	if(m_instance){
		delete m_instance;
		m_instance = NULL;
	}
}
//
bool MayaConnection::isInteractiveRenderingMode()
{
	return MRenderView::doesRenderEditorExist();
}
void MayaConnection::getMin(unsigned int &min_x, unsigned int &min_y,
			const eiInt left, const eiInt right,
			const eiInt bottom, const eiInt top)
{
#define ELIMINATE_OFFSET
#if defined(ELIMINATE_OFFSET)
	//motivation:
	// image A : the scene rendered by elvishray;
	// image B : the scene renderer by Maya software;
	// A has a translate offset (0,1) relative to B under Maya window coordinate.
	// to eliminate this offset, we set:
	// ( see ermaya\test\discontinuesPixel2_er.jpg and discontinuesPixel2_maya.jpg)
	// int _min_x = left;
	// int _min_y = height - (bottom - 1)-1;
	// to translate image A by (0, -1),

	// but, the first column will not be updated correctly, because their _min_x and _min_y are -1.
	// So, we clamp min_x and min_y greater than 0:
	// min_x = _min_x>=0 ? _min_x : 0;
	// min_y = _min_y>=0 ? _min_y : 0;

	// But we fail again, because the pixels on the edge of column 0 and column 1 are not continuous any more.
	// (see test\discontinuesPixel.jpg for details)
	int _min_x = left;
	int _min_y = height - (bottom - 1)-1;

	min_x = _min_x>=0 ? _min_x : 0;
	min_y = _min_y>=0 ? _min_y : 0;
#else
	//Note:
	// image A : the scene rendered by elvishray;
	// image B : the scene renderer by Maya software;
	// A has a translate offset (0,1) relative to B under Maya window coordinate.
	// see ermaya\test\discontinuesPixel2_er.jpg and discontinuesPixel2_maya.jpg
	min_x = left;
	min_y = height - (bottom - 1);
#endif
}
//////////////////////////////////////////////////////////////////////////
//	Max connection implementation

static void maya_connection_print(
								 eiConnection *connection, 
								 const eiInt severity, 
								 const char *message)
{
	MayaConnection *con = ((eiMayaConnection *)connection)->object;

	con->Print(severity, message);
}

static eiBool maya_connection_progress(
									  eiConnection *connection, 
									  const eiScalar percent)
{
	MayaConnection *con = ((eiMayaConnection *)connection)->object;

	return con->Progress(percent);
}

static void maya_connection_clear_tile(
									  eiConnection *connection, 
									  const eiInt left, 
									  const eiInt right, 
									  const eiInt top, 
									  const eiInt bottom, 
									  const eiHostID host)
{
	MayaConnection *con = ((eiMayaConnection *)connection)->object;

	con->ClearTile(left, right, top, bottom, host);
}

static void maya_connection_update_tile(
									   eiConnection *connection, 
									   const eiInt job_state,
									   eiFrameBufferCache *colorFrameBuffer, 
									   eiFrameBufferCache *opacityFrameBuffer, 
									   ei_array *frameBuffers, 
									   const eiInt left, 
									   const eiInt right, 
									   const eiInt top, 
									   const eiInt bottom)
{
	MayaConnection *con = ((eiMayaConnection *)connection)->object;

	con->UpdateTile(job_state, colorFrameBuffer, opacityFrameBuffer, 
		frameBuffers, left, right, top, bottom);
}

static void maya_connection_update_sub_window(
	eiConnection *connection, 
	const eiInt left, 
	const eiInt right, 
	const eiInt top, 
	const eiInt bottom)
{
	MayaConnection *con = ((eiMayaConnection *)connection)->object;

	con->UpdateSubWindow(left, right, top, bottom);
}

//////////////////////////////////////////////////////////////////////////
void MayaConnection::setConnection()
{
	connection.object = this;
	connection.base.print = maya_connection_print;
	connection.base.progress = maya_connection_progress;
	connection.base.clear_tile = maya_connection_clear_tile;
	connection.base.update_tile = maya_connection_update_tile;
	connection.base.update_sub_window = maya_connection_update_sub_window;

}
