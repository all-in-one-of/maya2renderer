
#include <liqJobScriptMgr.h>
 
#include <sstream>
#include <liqRenderScript.h>
#include <liqGlobalVariable.h>


tJobScriptMgr::tJobScriptMgr(liqRenderScript& script)
:m_jobScript(script)
{
	
}
//
tJobScriptMgr::~tJobScriptMgr()
{

}
//
void tJobScriptMgr::setCommonParameters(
	const std::string &renderJobName__,
	const bool useNetRman__,
	const liquidlong m_minCPU__,
	const liquidlong m_maxCPU__,
	const MString &m_dirmaps__
	)
{
	CM_TRACE_FUNC("tJobScriptMgr::setCommonParameters("<<renderJobName__<<","<<useNetRman__<<","<<m_minCPU__<<","<<m_maxCPU__<<","<<m_dirmaps__.asChar()<<")");

	//[refactor][1.4 begin] from _doIt()
	m_jobScript.title = renderJobName__;

	if( useNetRman__ ) 
	{
		m_jobScript.minServers = m_minCPU__;
		m_jobScript.maxServers = m_maxCPU__;
	} else {
		m_jobScript.minServers = 1;
		m_jobScript.maxServers = 1;
	}

	if ( m_dirmaps__.length() )//added in ymesh
		m_jobScript.dirmaps = m_dirmaps__.asChar();
	//[refactor][1.4 end] from _doIt()
}
//
void tJobScriptMgr::addJob(
	const std::string &title, 
	const std::string &command, const bool remote
)
{
	CM_TRACE_FUNC("tJobScriptMgr::addJob("<<title<<","<<command<<","<<remote<<")");

	//[refactor][1.5 begin] in _doIt()
	liqRenderScript::Job job;
	job.title = title;
	liqRenderScript::Cmd cmd( command, remote );
	cmd.alfredServices = liqglo.m_alfredServices.asChar();
	cmd.alfredTags = liqglo.m_alfredTags.asChar(); 
	job.commands.push_back( cmd );
	m_jobScript.addJob( job );
	//[refactor][1.5 end] in _doIt()
}
//
void tJobScriptMgr::addDefferedJob(const int currentBlock__,
								   const unsigned int frameIndex__,
								   const MString &framePreCommand__,
								   const MString &frameRibgenCommand__)
{
	CM_TRACE_FUNC("tJobScriptMgr::addDefferedJob("<<currentBlock__<<","<<frameIndex__<<","<<framePreCommand__.asChar()<<","<<frameRibgenCommand__.asChar()<<")");

	liqRenderScript::Job deferredJob;
	addDefferedJob(deferredJob, currentBlock__, frameIndex__,
		framePreCommand__, frameRibgenCommand__			
		);
	m_jobScript.addJob(deferredJob);
}
//
void tJobScriptMgr::addDefferedJob(
									  liqRenderScript::Job &deferredJob__,
									  const int currentBlock__,
									  const unsigned int frameIndex__,
									  const MString &framePreCommand__,
									  const MString &frameRibgenCommand__
									  )
{
	CM_TRACE_FUNC("tJobScriptMgr::addDefferedJob("<<deferredJob__.title<<","<<currentBlock__<<","<<frameIndex__<<","<<framePreCommand__.asChar()<<","<<frameRibgenCommand__.asChar()<<")");
		
	//[refactor] [1.8.2 begin] from _doIt()
	std::stringstream ribGenExtras;
	//Note:  -ribName is set to liqglo.liqglo_sceneName originally. 
	//       Because we replace liqglo.liqglo_sceneName with liquidTransGetSceneName(), and delete liqglo.liqglo_sceneName,
	//       we use liqglo.liqglo_ribName to store the ribName which passed by commandline parameter '-ribName'
	//  [2/17/2012 yaoyansi]
	//---------------------------------------------------
#if 1
	//---ymesh r775
	assert(0&&"warrning: ribName is store in liqglo.liqglo_ribName instead of liqglo.liqglo_sceneName.[2/17/2012 yaoyansi]");
	// ribGenExtras << " -progress -noDef -nop -noalfred -projectDir " << liqglo_projectDir.asChar() << " -ribName " << liqglo.liqglo_ribName.asChar() << " -mf " << liqglo.tempDefname.asChar() << " -t ";
	if ( debugMode ) ribGenExtras << " -debug";
	ribGenExtras << " -progress -noDef -ribGenOnly -noLaunchRender";
	ribGenExtras << " -projectDir \""<< liqglo.liqglo_projectDir.asChar() << "\" -ribName \"" << liqglo.liqglo_ribName.asChar() << "\" -fl ";

	unsigned lastGenFrame( ( frameIndex__ + liqglo.m_deferredBlockSize ) < liqglo.frameNumbers.size() ? frameIndex__ + liqglo.m_deferredBlockSize : liqglo.frameNumbers.size() );

	//liquidMessage2(messageInfo, ">>> frameIndex = %d m_deferredBlockSize = %d frameNumbers.size = %d lastGenFrame = %d\n",frameIndex,m_deferredBlockSize,frameNumbers.size(),lastGenFrame );

	for( unsigned outputFrame( frameIndex__ ); outputFrame < lastGenFrame; outputFrame++ )
	{  
		ribGenExtras << liqglo.frameNumbers[ outputFrame ];
		ribGenExtras << (( outputFrame != ( lastGenFrame - 1 ) )? ", " : " ");
		// liquidMessage2(messageInfo, "\t outputFrame = %d\n", outputFrame );
	}
#else					// r773
	//ribGenExtras << " -progress -noDef -nop -noalfred -projectDir " << liqglo_projectDir.asChar() << " -ribName " << liqglo_sceneName.asChar() << " -mf " << tempDefname.asChar() << " -t ";
	// Ensure deferred rib gen and use render script are off
	// Project dir argument must be first, in case -GL uses paths relative to the current project

	//string preFrameCmd = "if(exists(\"userSetup\")){source \"userSetup\";}";
	ribGenExtras << "-projectDir " << liqglo_projectDir.asChar() << " -GL -noDef -noLaunchRender -noRenderScript -ribGenOnly -ribdir "<< liqglo_ribDir.asChar() <<" -ribName " << liqglo_sceneName.asChar() << " " << frameRangePart.asChar() << " " << tempDefname.asChar();
#endif
	//-----------------------------------------------------
	std::stringstream titleStream;
	titleStream << liquidTransGetSceneName().asChar() << "FrameRIBGEN" << currentBlock__;
	deferredJob__.title = titleStream.str();

	std::stringstream ss;
	ss << framePreCommand__.asChar() << " " << frameRibgenCommand__.asChar() << ribGenExtras.str() << " " <<  liqglo.tempDefname.asChar();
	liqRenderScript::Cmd cmd( ss.str(), liqglo.remoteRender );
	cmd.alfredServices = liqglo.m_defGenService.asChar();
	cmd.alfredTags     = liqglo.m_defGenKey.asChar();
	if( liqglo.m_alfredExpand ) 
		cmd.alfredExpand = true;

	deferredJob__.commands.push_back(cmd);
	//[refactor] [1.8.2 end] from _doIt()
}
//
void tJobScriptMgr::cleanupDefferedJob()
{
	CM_TRACE_FUNC("tJobScriptMgr::cleanupDefferedJob()");
	//[refactor][1.12 begin]
	std::string cmd = RM_CMD  + std::string( MString( " \""  + liqglo.tempDefname + "\"" ).asChar() );
	liqRenderScript::Cmd jobCleanCmd( cmd, 0 );
	jobCleanCmd.alfredServices =  liqglo.m_alfredServices.asChar();
	jobCleanCmd.alfredTags =  liqglo.m_alfredTags.asChar();
	m_jobScript.cleanupCommands.push_back( jobCleanCmd );
	//[refactor][1.12 end]
}
void tJobScriptMgr::cleanupRenderScript(const MString &renderScriptName__)
{
	CM_TRACE_FUNC("tJobScriptMgr::cleanupRenderScript("<<renderScriptName__.asChar()<<")");
	//[refactor][1.13 ]
	std::string cmd = RM_CMD  + std::string( MString( " \""  + renderScriptName__ + "\"" ).asChar() );
	liqRenderScript::Cmd jobCleanCmd( cmd, 0 );
	jobCleanCmd.alfredServices =  liqglo.m_alfredServices.asChar();
	jobCleanCmd.alfredTags =  liqglo.m_alfredTags.asChar();
	m_jobScript.cleanupCommands.push_back( jobCleanCmd );
	//[refactor][1.13 ]
}
void tJobScriptMgr::cleanupPostJob(const MString &m_postJobCommand__)
{
	CM_TRACE_FUNC("tJobScriptMgr::cleanupPostJob("<<m_postJobCommand__.asChar()<<")");
	//[refactor][1.14 ]
	std::string cmd = m_postJobCommand__.asChar();
	liqRenderScript::Cmd jobCleanCmd( cmd, 0 );
	jobCleanCmd.alfredServices =  liqglo.m_alfredServices.asChar();
	jobCleanCmd.alfredTags =  liqglo.m_alfredTags.asChar();
	m_jobScript.cleanupCommands.push_back( jobCleanCmd );
	//[refactor][1.14 ]
}
//
void tJobScriptMgr::writeRenderScript(const renderScriptFormat format, const MString &renderScriptName__)
{
	CM_TRACE_FUNC("tJobScriptMgr::writeRenderScript("<<format<<","<<renderScriptName__.asChar()<<")");

	if( format == ALFRED ) 
		m_jobScript.writeALF( liquidGetRelativePath( liqglo.liqglo_relativeFileNames, renderScriptName__, liqglo.liqglo_projectDir ).asChar() );
	if( format == XML ) 
		m_jobScript.writeXML( liquidGetRelativePath( liqglo.liqglo_relativeFileNames, renderScriptName__, liqglo.liqglo_projectDir ).asChar() );

}