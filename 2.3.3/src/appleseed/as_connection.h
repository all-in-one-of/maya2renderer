#include <liqConfig.h>
#ifdef _USE_APPLESEED_

#pragma once

#include "../common/prerequest_std.h"
#include "prerequest_as.h"
#include "qttilecallbackfactory.h"
#include "../common/prerequest_maya.h"

namespace appleseed
{

class Connection
{
	//Singleton pattern interface
public:
	static Connection* getInstance();
	static void  delInstance();
	virtual ~Connection();

protected:
	Connection();
	static Connection *m_instance;
	
	//
public:
	//interfaces for maya
	MStatus startRender( unsigned int width,
		unsigned int height,
		bool doNotClearBackground = false,
		bool immediateFeedback = false );
	MStatus endRender();
	void render(
		const asf::auto_release_ptr<asr::Project> &project,
		const renderer::ParamArray &params,
		const bool bBatchMode
		);
private:
	//std::auto_ptr<studio::QtTileCallbackFactory>m_tile_callback_factory;
	//std::auto_ptr<renderer::MasterRenderer>     m_master_renderer;
};

}

#endif//_USE_APPLESEED_