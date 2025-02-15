@echo off 
SET tmpdir=tmp

if exist %tmpdir% rmdir /s /q %tmpdir%
   mkdir %tmpdir%

cd %tmpdir%

SET LIQUIDHOME=c:/tools/liquidmaya
SET LIQPLUGSUFFIX=.mll

:set_maya_version
SET MAYA_VERSION=2008
rem SET MAYA_VERSION=2009
rem SET MAYA_VERSION=2010
rem SET MAYA_VERSION=2011

:set_maya_arch
SET ARCH=
rem SET ARCH=-x64

SET AW_LOCATION=c:/Autodesk
SET MAYALIBS=Image.lib Foundation.lib OpenMaya.lib OpenMayaRender.lib OpenMayaUI.lib OpenMayaAnim.lib OpenMayaFX.lib
SET LIBS=netapi32.lib;ws2_32.lib
SET BOOST_ROOT=C:/boost/boost_1_41_0
SET ZLIB_INCLUDE_DIR=C:/tools/zlib/include
SET VSINSTALLDIR="C:\Program Files (x86)\Microsoft Visual Studio 8"
SET GEN="NMake Makefiles"

@echo build for %MAYA_VERSION% %ARCH%


IF DEFINED ARCH (echo ARCH defined) ELSE ( goto set_x32 )
IF %ARCH% == -x64 GOTO set_x64 

:set_x32
@echo Setup VC variables for x86 mode
SET ZLIB_LIBRARIES=C:/tools/zlib/lib/zlib.lib
call %VSINSTALLDIR%\VC\vcvarsall.bat x86
goto set_renderer

:set_x64
@echo Setup VC variables for amd64 mode
SET ZLIB_LIBRARIES=C:/tools/zlib64/static_x64/zlibstat.lib
call %VSINSTALLDIR%\VC\vcvarsall.bat amd64


:set_renderer
SET LIQRMAN=generic
rem SET LIQRMAN=prman
rem SET LIQRMAN=3delight
rem SET LIQRMAN=pixie
rem SET LIQRMAN=aqsis
rem SET LIQRMAN=air

GOTO %LIQRMAN%

:generic
SET USE_RIBLIB=yes
SET LIQRMANPATH=%LIQUIDHOME%/ribLib
SET LIQRMANFLAGS=-DGENERIC_RIBLIB -DNO_RICMD 
SET LIQRMANLIBS=librib%ARCH%.lib
SET LIQRMANINC=%LIQRMANPATH%
SET LIQRMANLIB=%LIQRMANPATH%/lib
GOTO build

:air
SET LIQRMANPATH=%AIRHOME%
SET LIQRMANFLAGS=	-DAIR
SET LIQRMANLIBS=
SET LIQRMANINC=%LIQRMANPATH%/include
SET LIQRMANLIB=%LIQRMANPATH%/lib
GOTO build

:aqsis
SET LIQRMANPATH=%AQSISHOME%
SET LIQRMANFLAGS=	-DAQSIS
SET LIQRMANLIBS=ri2rib.lib
SET LIQRMANINC=%LIQRMANPATH%/include
SET LIQRMANLIB=%LIQRMANPATH%/lib
GOTO build

:pixie
SET LIQRMANPATH=%PIXIEHOME%
SET LIQRMANFLAGS=-DPIXIE
SET LIQRMANLIBS=ri.lib
SET LIQRMANINC=%LIQRMANPATH%/include
SET LIQRMANLIB=%LIQRMANPATH%/lib
GOTO build

:3delight
SET LIQRMANPATH=%DELIGHT%
SET LIQRMANFLAGS=-DDELIGHT
SET LIQRMANLIBS=3delight.lib
rem SET LIBS=ws2_32.lib
SET LIQRMANINC=%LIQRMANPATH%/include
SET LIQRMANLIB=%LIQRMANPATH%/lib
GOTO build

:prman
SET LIQRMANPATH=%RMANTREE%
SET LIQRMANFLAGS=-DPRMAN -DNO_RICMD
SET LIQRMANLIBS=prman.lib
SET LIQRMANINC=%LIQRMANPATH%/include
SET LIQRMANLIB=%RMANTREE%/lib
GOTO build

GOTO :EOF

:build
rem if exist override.cmd call override.cmd
echo Building "%LIQRMAN%" Liquid for Maya %MAYA_VERSION%%ARCH%
echo using %GEN% ...
SET MAYA_LOCATION=%AW_LOCATION%/Maya%MAYA_VERSION%%ARCH%
cmake -G %GEN% -D CMAKE_INSTALL_PREFIX:PATH=%LIQUIDHOME%/bin/Maya%MAYA_VERSION%%ARCH%/ -DMAYA_LOCATION=%MAYA_LOCATION% -DZLIB_LIBRARY=%ZLIB_LIBRARIES% -DZLIB_INCLUDE_DIR=%ZLIB_INCLUDE_DIR% %LIQUIDHOME%/src

nmake
nmake install
cd ..
