
#include <liqFrameScriptMgr.h>

#include <common/prerequest_maya.h>

#include <liqRenderScript.h>
#include <liqGlobalHelpers.h>
#include <liqGlobalVariable.h>
#include <liqDefine.h>

tFrameScriptJobMgr::tFrameScriptJobMgr(liqRenderScript::Job& script)
:m_frameScriptJob(script)
{

}
//
tFrameScriptJobMgr::~tFrameScriptJobMgr()
{

}
//
void tFrameScriptJobMgr::setCommonParameters(const std::string &title )
{ 
	CM_TRACE_FUNC("tFrameScriptJobMgr::setCommonParameters("<<title<<")");

	m_frameScriptJob.title = title;
}
//
void tFrameScriptJobMgr::addInstanceJob(  const bool isInstance,
					const std::string &title
					)
{ 
	CM_TRACE_FUNC("tFrameScriptJobMgr::addInstanceJob("<<isInstance<<","<<title<<")");

	liqRenderScript::Job instanceJob__;
	instanceJob__.isInstance = isInstance;
	instanceJob__.title = title;
	m_frameScriptJob.childJobs.push_back(instanceJob__);
}
//
// void tFrameScriptJobMgr::addJob(
// 						   const std::string &title, 
// 						   const std::string &command, const bool remote
// 						   )
// {
// 	liqRenderScript::Job job;
// 	job.title = title;
// 	job.commands.push_back( liqRenderScript::Cmd( command, remote ) );
// 	m_frameScriptJob.addJob( job );
// }

void tFrameScriptJobMgr::makeTexture(
	std::vector<struct structJob>  &txtList___,
	bool &alf_textures__,
	bool &alf_shadows__,
	bool &alf_refmaps__
)
{
	CM_TRACE_FUNC("tFrameScriptJobMgr::makeTexture(txtList___,"<<alf_textures__<<","<<alf_shadows__<<","<<alf_refmaps__<<")");

	if( txtList___.size() ) 
	{
		liqRenderScript::Job textureJob;
		makeTexturePass(txtList___, textureJob, 
			alf_textures__, alf_shadows__, alf_refmaps__ 
			);
		//[refactor][1.10.2.2 begin] from _doIt()
		m_frameScriptJob.childJobs.push_back( textureJob );
		//[refactor][1.10.2.2 end] from _doIt()
	}//if( txtList.size() )
}
//
void tFrameScriptJobMgr::makeTexturePass(
									   std::vector<struct structJob> &txtList__, 
									   liqRenderScript::Job &textureJob__,
									   bool &alf_textures__,
									   bool &alf_shadows__,
									   bool &alf_refmaps__
									   )
{
	CM_TRACE_FUNC("tFrameScriptJobMgr::makeTexturePass(txtList___,textureJob__,"<<alf_textures__<<","<<alf_shadows__<<","<<alf_refmaps__<<")");
	//[refactor][1.10.2 begin] from _doIt()
	std::vector<structJob>::iterator iter = txtList__.begin();
	
	//[refactor][1.10.2.1 begin]from _doIt()
	alf_textures__ = true;
	std::stringstream ts;
	ts << "Textures." << liqglo.liqglo_lframe;
	textureJob__.title = ts.str();
	//[refactor][1.10.2.1 end]from _doIt()

	while ( iter != txtList__.end() ) 
	{
		liqRenderScript::Job textureSubtask;
		std::stringstream ts;
		ts << textureJob__.title << " " << iter->imageName.asChar();
		textureSubtask.title = ts.str();
// 		if( m_deferredGen ) {
// 
// 		}
		std::stringstream ss;
		ss << iter->renderName.asChar() << " " << iter->ribFileName.asChar();
		liqRenderScript::Cmd cmd( ss.str(), ( liqglo.remoteRender && !liqglo.useNetRman ) );

		if( liqglo.m_alfredExpand ) 
			cmd.alfredExpand = true;

		cmd.alfredServices = liqglo.m_alfredServices.asChar();
		cmd.alfredTags     = liqglo.m_alfredTags.asChar();
		textureSubtask.commands.push_back( cmd );
		textureSubtask.chaserCommand = ( std::string( "sho \"" ) + liqglo.liqglo_textureDir.asChar() + " " + iter->imageName.asChar() + "\"" );
		++iter;
		textureJob__.childJobs.push_back( textureSubtask );
	}
	//[refactor][1.10.2 end] from _doIt()

}
//
void tFrameScriptJobMgr::makeShadow(
				std::vector<struct structJob> &shadowList__,
				bool &alf_textures__,
				bool &alf_shadows__,
				bool &alf_refmaps__,
				const int currentBlock__,
				const MString& m_renderCommand__
				)
{
	CM_TRACE_FUNC("tFrameScriptJobMgr::makeShadow(shadowList__,"<<alf_textures__<<","<<alf_shadows__<<","<<alf_refmaps__<<","<<currentBlock__<<","<<m_renderCommand__.asChar()<<")");
	//[refactor][1.7.2 begin] from _doIt()
	const MString framePreCommand(parseString( liqglo.m_preCommand, false));
	//[refactor][1.7.2 end] from _doIt()
	//[refactor][1.7.1 begin] from _doIt()
	const MString frameRenderCommand( parseString( m_renderCommand__ + " " + liqglo.liquidRenderer.renderCmdFlags, false ));
	//[refactor][1.7.1 end] from _doIt()

	//[refactor][1.10.3 begin]
	if( shadowList__.size() ) 
	{
		liqRenderScript::Job shadowJob;
		makeShadowPass(shadowList__, shadowJob, 
			alf_textures__, alf_shadows__, alf_refmaps__, 
			framePreCommand, frameRenderCommand, 
			currentBlock__
		);
		m_frameScriptJob.childJobs.push_back( shadowJob );
	}//if( shadowList.size() )
	//[refactor][1.10.3 end]
}
//
void tFrameScriptJobMgr::makeShadowPass(
										std::vector<struct structJob> &shadowList__, 
										liqRenderScript::Job &shadowJob__,
										bool &alf_textures__,
										bool &alf_shadows__,
										bool &alf_refmaps__,
										const MString &framePreCommand__,
										const MString &frameRenderCommand__,
										const int currentBlock__
										)
{
	CM_TRACE_FUNC("tFrameScriptJobMgr::makeShadowPass(shadowList__,shadowJob__,"<<alf_textures__<<","<<alf_shadows__<<","<<alf_refmaps__<<","
		<<framePreCommand__.asChar()<<", "<<frameRenderCommand__.asChar()<<", "<<currentBlock__<<")");

	std::vector<struct structJob >::iterator iter = shadowList__.begin();

	alf_shadows__ = true;
	std::stringstream ts;
	ts << "Shadows." << liqglo.liqglo_lframe;
	shadowJob__.title = ts.str();
	while ( iter != shadowList__.end() ) 
	{
		alf_shadows__ = true;
		liqRenderScript::Job shadowSubtask;
		shadowSubtask.title = iter->name.asChar();
		if( alf_textures__ ) 
		{
			std::stringstream ss;
			ss << "Textures." << liqglo.liqglo_lframe;
			liqRenderScript::Job instanceJob;
			instanceJob.isInstance = true;
			instanceJob.title = ss.str();
			shadowSubtask.childJobs.push_back(instanceJob);
		}
		if( liqglo.m_deferredGen ) 
		{
			std::stringstream ss;
			ss << liquidTransGetSceneName().asChar() << "FrameRIBGEN" << currentBlock__;
			liqRenderScript::Job instanceJob;
			instanceJob.isInstance = true;
			instanceJob.title = ss.str();
			shadowSubtask.childJobs.push_back(instanceJob);
		}
		std::stringstream ss;
		if( liqglo.useNetRman ) 
		{
			if ( liqglo.useNetRman ) 
				ss << framePreCommand__.asChar() << " netrender %H ";
			else
				ss << framePreCommand__.asChar() << " " << frameRenderCommand__.asChar() << " ";
			ss << "-Progress ";

			if ( liqglo.m_dirmaps.length() ) //use_dirmaps
				ss << "%D(" << iter->ribFileName.asChar() << ")";
			else
#ifdef _WIN32
				ss << "\"" << iter->ribFileName.asChar() << "\"";
#else
				ss << iter->ribFileName.asChar();
#endif
		}
		liqRenderScript::Cmd cmd(ss.str(), (liqglo.remoteRender && !liqglo.useNetRman));
		if( liqglo.m_alfredExpand ) 
			cmd.alfredExpand = true;

		cmd.alfredServices = liqglo.m_alfredServices.asChar();
		cmd.alfredTags     = liqglo.m_alfredTags.asChar();
		shadowSubtask.commands.push_back(cmd);

 		if(liqglo.cleanRib)  
 		{
 			std::stringstream ss;
			ss << framePreCommand__.asChar() << " " << RM_CMD << " ";
#ifdef _WIN32
			ss << "\"" << iter->ribFileName.asChar() << "\"";
#else
			ss << iter->ribFileName.asChar();
#endif
			liqRenderScript::Cmd jobShdCommand( ss.str(), liqglo.remoteRender );
			jobShdCommand.alfredServices = liqglo.m_alfredServices.asChar();
			jobShdCommand.alfredTags     = liqglo.m_alfredTags.asChar();
			shadowSubtask.cleanupCommands.push_back( jobShdCommand );
		}
		shadowSubtask.chaserCommand = ( std::string( "sho \"" ) + iter->imageName.asChar() + "\"" );
		++iter;
		if( !liqglo.m_alfShadowRibGen && !liqglo.fullShadowRib ) 
			liqglo.m_alfShadowRibGen = true;
		shadowJob__.childJobs.push_back( shadowSubtask );
	}
}
//
void tFrameScriptJobMgr::makeReflection(
					std::vector<struct structJob> &refList__,
					bool &alf_textures__,
					bool &alf_shadows__,
					bool &alf_refmaps__
					)
{
	CM_TRACE_FUNC("tFrameScriptJobMgr::makeReflection(refList__,"<<alf_textures__<<","<<alf_shadows__<<","<<alf_refmaps__<<")");
	//[refactor][1.10.4 begin] from _doIt()
	if( refList__.size() ) 
	{	
		liqRenderScript::Job reflectJob;
		makeReflectionPass(refList__, reflectJob, 
			alf_textures__, alf_shadows__, alf_refmaps__
			);
		m_frameScriptJob.childJobs.push_back( reflectJob );
	}	
	//[refactor][1.10.4 end] from _doIt()
}
//
void tFrameScriptJobMgr::makeReflectionPass(
	std::vector<struct structJob> &refList__, 
	liqRenderScript::Job &reflectJob__,
	bool &alf_textures__,
	bool &alf_shadows__,
	bool &alf_refmaps__
	)
{
	CM_TRACE_FUNC("tFrameScriptJobMgr::makeReflectionPass(refList__,reflectJob__,"<<alf_textures__<<","<<alf_shadows__<<","<<alf_refmaps__<<")");

	LIQDEBUGPRINTF( "-> Generating job for ReflectionMap pass\n" );
	std::vector<struct structJob>::iterator iter = refList__.begin();

	alf_refmaps__ = true;

	std::stringstream ts;
	ts << "Reflections." << liqglo.liqglo_lframe;
	reflectJob__.title = ts.str();

	while ( iter != refList__.end() ) 
	{
		liqRenderScript::Job reflectSubtask;
		std::stringstream ts;
		ts << reflectJob__.title << " " << iter->imageName.asChar();
		reflectSubtask.title = ts.str();
		if( liqglo.m_deferredGen ) {

		}
		if( alf_textures__ ) 
		{
			std::stringstream ss;
			ss << "Textures." << liqglo.liqglo_lframe;
			liqRenderScript::Job instanceJob;
			instanceJob.isInstance = true;
			instanceJob.title = ss.str();
			reflectJob__.childJobs.push_back( instanceJob );
		}
		if( alf_shadows__ ) 
		{
			std::stringstream ss;
			ss << "Shadows." << liqglo.liqglo_lframe;
			liqRenderScript::Job instanceJob;
			instanceJob.isInstance = true;
			instanceJob.title = ss.str();
			reflectJob__.childJobs.push_back( instanceJob );
		}

		std::stringstream ss;
		ss << iter->renderName.asChar() << " " << iter->ribFileName.asChar();
		liqRenderScript::Cmd cmd( ss.str(), (liqglo.remoteRender && !liqglo.useNetRman) );

		if( liqglo.m_alfredExpand ) 
			cmd.alfredExpand = true;

		cmd.alfredServices = liqglo.m_alfredServices.asChar();
		cmd.alfredTags     = liqglo.m_alfredTags.asChar();
		reflectSubtask.commands.push_back( cmd );
		reflectSubtask.chaserCommand = ( std::string( "sho \"" ) + liqglo.liqglo_textureDir.asChar() + " " + iter->imageName.asChar() + "\"" );
		++iter;
		reflectJob__.childJobs.push_back( reflectSubtask );
	}

}
//

void tFrameScriptJobMgr::try_addPreFrameCommand(const MString &framePreFrameCommand__)
{
	CM_TRACE_FUNC("tFrameScriptJobMgr::try_addPreFrameCommand("<<framePreFrameCommand__.asChar()<<")");
	//[refactor][1.10.4 begin] from _doIt()
	if( framePreFrameCommand__ != MString("") ) 
	{
		liqRenderScript::Cmd cmd(framePreFrameCommand__.asChar(), (liqglo.remoteRender && !liqglo.useNetRman));
		cmd.alfredServices = liqglo.m_alfredServices.asChar();
		cmd.alfredTags     = liqglo.m_alfredTags.asChar();
		m_frameScriptJob.commands.push_back(cmd);
	}
	//[refactor][1.10.4 end] from _doIt()
}
//
void tFrameScriptJobMgr::try_addPostFrameCommand(const MString &framePostFrameCommand__)
{
	CM_TRACE_FUNC("tFrameScriptJobMgr::try_addPostFrameCommand("<<framePostFrameCommand__.asChar()<<")");
	//[refactor][1.10.13 begin]from
	if( framePostFrameCommand__ != MString("") ) 
	{
		liqRenderScript::Cmd cmd( framePostFrameCommand__.asChar(), ( liqglo.remoteRender && !liqglo.useNetRman) );
		cmd.alfredServices =  liqglo.m_alfredServices.asChar();
		cmd.alfredTags =  liqglo.m_alfredTags.asChar();	
		m_frameScriptJob.cleanupCommands.push_back(cmd);// why cleanupCommand? not command or childJob? Is it a bug?

	}//[refactor][1.10.13 end]from
}
//
void tFrameScriptJobMgr::addHeroPass(
	const MString &ribFileName__t,
	const MString &framePreCommand__, 
	const MString &frameRenderCommand__
)
{
	CM_TRACE_FUNC("tFrameScriptJobMgr::addHeroPass("<<ribFileName__t.asChar()<<","<<framePreCommand__.asChar()<<","<<frameRenderCommand__.asChar()<<")");

	std::stringstream ss;
	//[refactor][1.10.5 begin]
	MString ribFileName__ = ribFileName__t;
	//[refactor][1.10.5 end]

	//[refactor][1.10.7 begin]
	if ( liqglo.useNetRman ) 
		ss << framePreCommand__.asChar() << " netrender %H ";
	else
		ss << framePreCommand__.asChar() << " " << frameRenderCommand__.asChar();
	ss << " -Progress ";

	if ( liqglo.m_dirmaps.length() ) 
		ss << "%D("  << ribFileName__.asChar()  << ")";
	else
#ifdef _WIN32            
		ss << "\"" << ribFileName__.asChar()  << "\"";
#else           
		ss << ribFileName__.asChar() ;
#endif
	//[refactor][1.10.7 end]

	liqRenderScript::Cmd cmd(ss.str(), (liqglo.remoteRender && !liqglo.useNetRman));
	if( liqglo.m_alfredExpand ) 
		cmd.alfredExpand = true;

	cmd.alfredServices = liqglo.m_alfredServices.asChar();
	cmd.alfredTags     = liqglo.m_alfredTags.asChar();
	m_frameScriptJob.commands.push_back(cmd);
}
//
void tFrameScriptJobMgr::addShadowPass(
				   const MString &ribFileName__,
				   const MString &framePreCommand__, 
				   const MString &frameRenderCommand__
				   )
{
	CM_TRACE_FUNC("tFrameScriptJobMgr::addShadowPass("<<ribFileName__.asChar()<<","<<framePreCommand__.asChar()<<","<<frameRenderCommand__.asChar()<<")");

	this->addHeroPass(ribFileName__, framePreCommand__, frameRenderCommand__);
	
	//[refactor][1.10.8 begin] from _doIt()
	std::stringstream ss;
	if( liqglo.useNetRman ) 
	{
#ifdef _WIN32
		ss << framePreCommand__.asChar() << " netrender %H -Progress \"" << ribFileName__.asChar() << "\"";
#else
		ss << framePreCommand__.asChar() << " netrender %H -Progress " << ribFileName__.asChar();
#endif
	} else {
#ifdef _WIN32
		ss << framePreCommand__.asChar() << " " << frameRenderCommand__.asChar() << " \"" << ribFileName__.asChar() << "\"";
#else
		ss << framePreCommand__.asChar() << " " << frameRenderCommand__.asChar() << " " << ribFileName__.asChar();
#endif
	}
	liqRenderScript::Cmd cmd(ss.str(), (liqglo.remoteRender && !liqglo.useNetRman));
	if( liqglo.m_alfredExpand ) 
		cmd.alfredExpand = true;

	cmd.alfredServices = liqglo.m_alfredServices.asChar();
	cmd.alfredTags     = liqglo.m_alfredTags.asChar();
	m_frameScriptJob.commands.push_back(cmd);
	//[refactor][1.10.8 end] from _doIt()
}
//
void tFrameScriptJobMgr::cleanHeroPass(const MString &framePreCommand__,
									   const MString &ribFileName__
									   )
{
	CM_TRACE_FUNC("tFrameScriptJobMgr::cleanHeroPass("<<framePreCommand__.asChar()<<","<<ribFileName__.asChar()<<")");
	
	std::stringstream ss;

	//[refactor][1.10.12 begin]from
	ss << framePreCommand__.asChar() << " " << RM_CMD << " ";
	if (  liqglo.m_dirmaps.length() ) 
		ss << "%D(" << ribFileName__.asChar() << ")";
	else
#ifdef _WIN32
		ss << "\"" << ribFileName__.asChar() << "\"";
#else
		ss << ribFileName__.asChar();
#endif

	// frameScriptJob.cleanupCommands.push_back(liqRenderScript::Cmd(ss.str(), remoteRender));
	liqRenderScript::Cmd jobCleanCommand( ss.str(),  liqglo.remoteRender );
	jobCleanCommand.alfredServices =  liqglo.m_alfredServices.asChar();
	jobCleanCommand.alfredTags =  liqglo.m_alfredTags.asChar();
	m_frameScriptJob.cleanupCommands.push_back( jobCleanCommand );
	//[refactor][1.10.12 end] from
}
//
void tFrameScriptJobMgr::cleanShadowPass(const MString &framePreCommand__,
										 const MString &ribFileName__)
{
	CM_TRACE_FUNC("tFrameScriptJobMgr::cleanShadowPass("<<framePreCommand__.asChar()<<","<<ribFileName__.asChar()<<")");

	std::stringstream ss;

	//[refactor][1.10.12 begin]from
	ss << framePreCommand__.asChar() << " " << RM_CMD << " ";
	if (  liqglo.m_dirmaps.length() ) 
		ss << "%D(" << ribFileName__.asChar() << ")";
	else
#ifdef _WIN32
		ss << "\"" << ribFileName__.asChar() << "\"";
#else
		ss << ribFileName__.asChar();
#endif

	// frameScriptJob.cleanupCommands.push_back(liqRenderScript::Cmd(ss.str(), remoteRender));
	liqRenderScript::Cmd jobCleanCommand( ss.str(),  liqglo.remoteRender );
	jobCleanCommand.alfredServices =  liqglo.m_alfredServices.asChar();
	jobCleanCommand.alfredTags =  liqglo.m_alfredTags.asChar();
	m_frameScriptJob.cleanupCommands.push_back( jobCleanCommand );
	//[refactor][1.10.12 end] from
}
//
void tFrameScriptJobMgr::cleanShadowRibGen(const MString &framePreCommand__,
										   const MString &ribFileName__)
{
	CM_TRACE_FUNC("tFrameScriptJobMgr::cleanShadowRibGen("<<framePreCommand__.asChar()<<","<<ribFileName__.asChar()<<")");

	std::stringstream ss;

	//[refactor][1.10.12 begin]from
	ss << framePreCommand__.asChar() << " " << RM_CMD << " ";
	if (  liqglo.m_dirmaps.length() ) 
		ss << "%D(" << ribFileName__.asChar() << ")";
	else
#ifdef _WIN32
		ss << "\"" << ribFileName__.asChar() << "\"";
#else
		ss << ribFileName__.asChar();
#endif

	// frameScriptJob.cleanupCommands.push_back(liqRenderScript::Cmd(ss.str(), remoteRender));
	liqRenderScript::Cmd jobCleanCommand( ss.str(),  liqglo.remoteRender );
	jobCleanCommand.alfredServices =  liqglo.m_alfredServices.asChar();
	jobCleanCommand.alfredTags =  liqglo.m_alfredTags.asChar();
	m_frameScriptJob.cleanupCommands.push_back( jobCleanCommand );
	//[refactor][1.10.12 end] from
}
//
void tFrameScriptJobMgr::viewHeroPassImage(const MString &imageName__)
{
	CM_TRACE_FUNC("tFrameScriptJobMgr::viewHeroPassImage("<<imageName__.asChar()<<")");

	m_frameScriptJob.chaserCommand = (std::string( "sho \"" ) + imageName__.asChar() + "\"" );
}
//
void tFrameScriptJobMgr::viewShadowPassImage(const MString &imageName__)
{
	CM_TRACE_FUNC("tFrameScriptJobMgr::viewShadowPassImage("<<imageName__.asChar()<<")");

	m_frameScriptJob.chaserCommand = (std::string( "sho \"" ) + imageName__.asChar() + "\"" );
}