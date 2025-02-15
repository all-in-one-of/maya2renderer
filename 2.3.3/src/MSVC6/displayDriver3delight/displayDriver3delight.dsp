# Microsoft Developer Studio Project File - Name="displayDriver3delight" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=displayDriver3delight - Win32 3Delight Release
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "displayDriver3delight.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "displayDriver3delight.mak" CFG="displayDriver3delight - Win32 3Delight Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "displayDriver3delight - Win32 3Delight Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe
# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "3Delight Release"
# PROP BASE Intermediate_Dir "3Delight Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "../../../bin/3Delight/win32/Release/Objects"
# PROP Intermediate_Dir "../../../bin/3Delight/win32/Release/Objects"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "DISPLAYDRIVER3DELIGHT_EXPORTS" /YX /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /I "../../../include" /I "$(DELIGHT)/include" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "_WIN32" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x40c /d "NDEBUG"
# ADD RSC /l 0x40c /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ws2_32.lib /nologo /dll /machine:I386 /out:"../../../bin/3Delight/win32/Release/liqmaya.dpy" /libpath:"$(DELIGHT)/lib"
# SUBTRACT LINK32 /pdb:none
# Begin Target

# Name "displayDriver3delight - Win32 3Delight Release"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\..\liqMayaDisplayDriver3Delight.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# Begin Source File

SOURCE=..\..\..\shaders\src\liquidarea.sl
# PROP Ignore_Default_Tool 1
# Begin Custom Build
InputPath=..\..\..\shaders\src\liquidarea.sl

"..\..\..\shaders\liquidarea.sdl" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	"$(DELIGHT)\bin\shaderdl.exe" $(InputPath) -o ..\..\..\shaders\liquidarea.sdl

# End Custom Build
# End Source File
# Begin Source File

SOURCE=..\..\..\shaders\src\liquidchecker.sl
# PROP Ignore_Default_Tool 1
# Begin Custom Build
InputPath=..\..\..\shaders\src\liquidchecker.sl

"..\..\..\shaders\liquidchecker.sdl" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	"$(DELIGHT)\bin\shaderdl.exe" $(InputPath) -o ..\..\..\shaders\liquidchecker.sdl

# End Custom Build
# End Source File
# Begin Source File

SOURCE=..\..\..\shaders\src\liquiddistant.sl
# PROP Ignore_Default_Tool 1
# Begin Custom Build
InputPath=..\..\..\shaders\src\liquiddistant.sl

"..\..\..\shaders\liquiddistant.sdl" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	"$(DELIGHT)\bin\shaderdl.exe" $(InputPath) -o ..\..\..\shaders\liquiddistant.sdl

# End Custom Build
# End Source File
# Begin Source File

SOURCE=..\..\..\shaders\src\liquidpoint.sl
# PROP Ignore_Default_Tool 1
# Begin Custom Build
InputPath=..\..\..\shaders\src\liquidpoint.sl

"..\..\..\shaders\liquidpoint.sdl" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	"$(DELIGHT)\bin\shaderdl.exe" $(InputPath) -o ..\..\..\shaders\liquidpoint.sdl

# End Custom Build
# End Source File
# Begin Source File

SOURCE=..\..\..\shaders\src\liquidspot.sl
# PROP Ignore_Default_Tool 1
# Begin Custom Build
InputPath=..\..\..\shaders\src\liquidspot.sl

"..\..\..\shaders\liquidspot.sdl" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	"$(DELIGHT)\bin\shaderdl.exe" $(InputPath) -o ..\..\..\shaders\liquidspot.sdl

# End Custom Build
# End Source File
# End Target
# End Project
