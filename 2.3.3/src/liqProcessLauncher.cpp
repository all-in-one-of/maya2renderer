/*
**
** The contents of this file are subject to the Mozilla Public License Version 1.1 (the
** "License"); you may not use this file except in compliance with the License. You may
** obtain a copy of the License at http://www.mozilla.org/MPL/
**
** Software distributed under the License is distributed on an "AS IS" basis, WITHOUT
** WARRANTY OF ANY KIND, either express or implied. See the License for the specific
** language governing rights and limitations under the License.
**
** The RenderMan (R) Interface Procedures and Protocol are:
** Copyright 1988, 1989, Pixar
** All Rights Reserved
**
**
** RenderMan (R) is a registered trademark of Pixar
*/

#include "liqProcessLauncher.h"

#include <common/prerequest_std.h>
#include <common/prerequest_maya.h>
#include "common/mayacheck.h"
#include <liqlog.h>
#include <liqGlobalHelpers.h>
/* ______________________________________________________________________
**
** Linux implementation of liqProcessLauncher::execute()
** ______________________________________________________________________
*/
#if defined(LINUX) || defined(OSX)

// #include <sys/types.h>
// #include <unistd.h>
// #include <stdlib.h>

bool liqProcessLauncher::_execute( const MString &command, const MString &arguments, const MString &path, const bool wait )
{
  chdir( path.asChar() );
  MString cmd = command + " " + arguments + ( wait ? "" : "&" );
  cout <<"[liqProcessLauncher] execute command : "<< cmd.asChar() <<endl;
  int returnCode = system( cmd.asChar() );
  if(returnCode==-1)
  {
      printf("Error while executing command '%s'\n", cmd.asChar());
  }
  return ( returnCode != -1 );
}
#endif // LINUX

/* ______________________________________________________________________
**
** Irix implementation of liqProcessLauncher::execute()
** ______________________________________________________________________
*/
#if defined(IRIX)

// #include <sys/types.h>
// #include <unistd.h>

bool liqProcessLauncher::_execute( const MString &command, const MString &arguments, const MString &path, const bool wait )
{
  chdir( path.asChar() );
  pcreatelp( command.asChar(), command.asChar(), arguments.asChar(), NULL );
  return true;
}
#endif // IRIX


/* ______________________________________________________________________
**
** Win32 implementation of liqProcessLauncher::execute()
** ______________________________________________________________________
*/
#if defined(_WIN32)
// #include <stdio.h>
// #include <direct.h>
// #include <process.h>
// #include <windows.h>
// #include <liqlog.h>
// #include<iostream>

bool liqProcessLauncher::_execute( const MString &command, const MString &arguments, const MString &path, const bool wait )
{
	CM_TRACE_FUNC(boost::format("liqProcessLauncher::_execute(%s,%s,%s, wait=%d)")%command.asChar()%arguments.asChar()%path.asChar()%wait);
	
	MString err = "Render (" + MString( (!wait)? "no " : "" ) + "wait) "+ command + " "+ arguments +" "+ path + "\n";
	liquidMessage( err, messageInfo );

  if ( !wait ) {
    printf( "::=> Render (no wait) %s %s %s\n", command.asChar(), arguments.asChar(), path.asChar() );
    /* Doesn't work on Windows 7!!!!! 
    int returnCode = (int)ShellExecute( NULL, NULL, ( LPCTSTR )command.asChar(), ( LPCTSTR )arguments.asChar(), ( LPCTSTR )path.asChar(), SW_SHOWNORMAL );
    DWORD dw = GetLastError();
    printf( "::=> return value = %d GetLastError = %d\n", returnCode, dw );
    return true;
    */
    //_chdir( path.asChar() );
    /*
    MString cmd = command + " " + arguments;
	_flushall();
    int returnCode = system( cmd.asChar() );
	cout << "out:Return value = " << returnCode << endl << flush;
	return ( returnCode != -1 );
    */
    //  _P_DETACH -- good for MayaRenderView and it, bad for framebuffer and alfred
    
	int returnCode =  _spawnlp( _P_NOWAITO, command.asChar(), command.asChar(), arguments.asChar(), NULL );
    DWORD dw = GetLastError();
	MString err = "err:Return value = ";
	err += (int)returnCode;
	err += " GetLastError = ";
	err += (int)dw;
	liquidMessage( err, messageInfo );
	// cout << "out:Return value = " << returnCode << " GetLastError = " << dw << endl << flush;
    return ( returnCode != -1 );
    
  } else {
    PROCESS_INFORMATION pinfo;
    STARTUPINFO sinfo;
    HANDLE hErrReadPipe, hErrReadPipeDup;
    HANDLE hErrWritePipe;
    int ret;

    ZeroMemory( &pinfo, sizeof( PROCESS_INFORMATION) );
    ZeroMemory( &sinfo, sizeof( STARTUPINFO) );

    sinfo.cb = sizeof( STARTUPINFO );

    sinfo.dwFlags = STARTF_USESTDHANDLES | STARTF_USESHOWWINDOW;
    sinfo.wShowWindow = SW_HIDE; // ; SW_SHOWNORMAL

    SECURITY_ATTRIBUTES saAttr;

    saAttr.nLength = sizeof( SECURITY_ATTRIBUTES );
    saAttr.bInheritHandle = true;
    saAttr.lpSecurityDescriptor = NULL;

    HANDLE hSaveStdin = GetStdHandle( STD_INPUT_HANDLE );
    HANDLE hSaveStdout = GetStdHandle( STD_OUTPUT_HANDLE );
    HANDLE hSaveStderr = GetStdHandle( STD_ERROR_HANDLE );
    ret = SetStdHandle( STD_ERROR_HANDLE, hSaveStdout );

    ret = CreatePipe( &hErrReadPipe, &hErrWritePipe, &saAttr, 0L );
    ret = SetStdHandle( STD_ERROR_HANDLE, hErrWritePipe );

    ret = DuplicateHandle(	GetCurrentProcess(), hErrReadPipe,
                GetCurrentProcess(), &hErrReadPipeDup , 0,
                false,
                DUPLICATE_SAME_ACCESS );

      CloseHandle( hErrReadPipe );

    sinfo.hStdInput = GetStdHandle( STD_INPUT_HANDLE );
    sinfo.hStdError = GetStdHandle( STD_ERROR_HANDLE );
    sinfo.hStdOutput = GetStdHandle( STD_ERROR_HANDLE );

    MString cmdline = command + " " + arguments;
	printf("cmdline=%s\n", cmdline.asChar());
    ret = CreateProcess(
            NULL,                     // name of executable module
            (char *)cmdline.asChar(), // command line string
            NULL,                     // SD lpProcessAttributes
            NULL,                     // SD lpThreadAttributes
            true,                     // handle inheritance option
            CREATE_NO_WINDOW,         // CREATE_NEW_CONSOLE, //      // creation flags
            NULL,                     // new environment block
            NULL,                     // current directory name
            &sinfo,                   // startup information
            &pinfo                    // process information
          );
    if ( ret ) 
	{
      SetStdHandle( STD_ERROR_HANDLE, hSaveStderr ); // restore saved Stderr
      SetStdHandle( STD_OUTPUT_HANDLE, hSaveStdout ); // restore saved Stderr

      // Wait until child process exits.
      WaitForSingleObject( pinfo.hProcess, INFINITE );

      // Close process and thread handles.
      CloseHandle( pinfo.hProcess );
      CloseHandle( pinfo.hThread );

      CloseHandle( hErrWritePipe );
      for ( ;; ) {
        char buff[ 4096 ];
        DWORD readed, written;
        if( !ReadFile( hErrReadPipeDup, buff, 4096, &readed, NULL) || readed == 0 )
          break;
        if( !WriteFile( hSaveStdout, buff, readed, &written, NULL) )
          break;
      }
      fflush( stdout );
    }

    return ( ret )? true : false ;
  }
}
#endif // _WIN32

bool liqProcessLauncher::execute( const MString &command, const MString &arguments, const MString &path, const bool wait )
{
	CM_TRACE_FUNC(boost::format("liqProcessLauncher::_execute(%s,%s,%s, wait=%d)")%command.asChar()%arguments.asChar()%path.asChar()%wait);

	if( isBatchMode() ){
		printf("// in batch mode.\n");
		_chdir( path.asChar() );

		std::string arguments2(arguments.asChar());
		boost::replace_all(arguments2, "\"", "\\\"");

		MString cmd("system(\""+command+" "+MString(arguments2.c_str())+"\")");
		printf("%s\n", cmd.asChar());

		IfMErrorWarn(MGlobal::executeCommand(cmd, true));

		return true;

	}else{
		return _execute(command, arguments, path, wait);
	}
}


