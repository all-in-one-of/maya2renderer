
#include <liqlog.h>
#include <common/mayacheck.h>
#include "renderermgr.h"

LIQUID_EXPORT char gLogBuffer[LOG_BUFFER_LEN];

void _logFunctionCall(const char* log)
{
	if(liqglo.m_logFunctionCall)
	{
		printf("CALL:%s\n", log );
		if(liqglo.m_logMsgFlush)
		{
			_flushall();
		}
	}
}
void _liqRIBMsg(const char* msg)
{
	//RiArchiveRecord( RI_COMMENT, "\t\t\t\t[DEBUG] %s", msg );
	liquid::RendererMgr::getInstancePtr()->getRenderer()->logFrame(msg);
}

/** Standard function to send messages to either the
 *  maya console or the shell for user feedback.
 */
void liquidMessage( const MString &msg, liquidVerbosityType type ) 
{
  if ( liqglo_verbosity >= type || liqglo.liquidBin ) 
  {
    if ( !liqglo.liquidBin ) 
    {
      MString infoOutput( "[Liquid] " + msg );
      switch( type ) 
      {
        case messageInfo:
          MGlobal::displayInfo( infoOutput );
          break;
        case messageWarning:
          MGlobal::displayWarning( infoOutput );
		  liqAssert("Warning",infoOutput, "Yes");
          break;
        case messageError:
			//MString err("error(\""+infoOutput+"\");");MGlobal::executeCommand(err,true,false);
			MGlobal::displayError( infoOutput );
			liqAssert("Error",infoOutput, "Yes");
			break;
      }
    } 
    else 
    {
      MString infoOutput( "[Liquid] " + msg );
	  MString prefix;
      switch( type ) 
      {
        case messageWarning:
          prefix = "Warning: ";
          break;
        case messageError:
          prefix = "Error: ";
          break;
        case messageInfo:
        default:
          prefix = "Info: ";  
      }
	  liqAssert(prefix.asChar(),(prefix+infoOutput).asChar(), "Yes");
    }
  }
}
//
//
void liquidMessageBox(const MString& msg, const MString& title, int uType)
{
	if( liqglo.m_showliquidMessageBox )
	{
		IfMErrorWarn(MGlobal::executeCommand( "liqAssert1(\""+title+"\", \""+msg+"\", \"Yes\");"));
	}
}
//
bool liqAssert(const MString & msg)
{
	// 	int ret;
	// 	IfMErrorWarn(MGlobal::executeCommand( "liqAssert0("+msg+");", ret));
	// 	return (ret==1);
	liquidMessageBox(msg.asChar(), "Assert", MB_OK);
	return true;
}
bool liqAssert(const MString &title, const MString & msg, const MString &bYes)
{
	// 	int ret;
	// 	IfMErrorWarn(MGlobal::executeCommand( "liqAssert1("+title+","+msg+","+bYes+");", ret));
	// 	return (ret==1);
	liquidMessageBox(msg.asChar(), title.asChar(), MB_OK);
	return true;
}
bool liqAssert(const MString &title, const MString & msg, const MString &bYes, const MString &bNo)
{
	// 	int ret;
	// 	IfMErrorWarn(MGlobal::executeCommand( "liqAssert2("+title+","+msg+","+bYes+","+bNo+");", ret));
	// 	return (ret==1);
	liquidMessageBox(msg.asChar(), title.asChar(), MB_YESNO);
	return true;
}