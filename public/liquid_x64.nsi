RequestExecutionLevel user

!include WinMessages.nsh

!define LiqPlatform	 "x64"
!define LiqConfiguration "M2R2013Release"

!define LIQUID_ROOT "E:\dev\autodesk\maya\myplugin\project\liquid_"
!define MyInstallDir "E:\maya2renderer_install_root"
!define /date CUR_TIME "%Y_%m_%d__%H_%M_%S"

outfile "liquid_${LiqPlatform}_${LiqConfiguration}_${CUR_TIME}.exe"
;installDir "$PROGRAMFILES\liquid_install_root" 
installDir ${MyInstallDir}


Page directory
Page instfiles
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
section

setOutPath $INSTDIR\2.3.3\bin\${LiqPlatform}\${LiqConfiguration}
file /x *.pdb /x *.lib /x *.exp   ${LIQUID_ROOT}\2.3.3\bin\${LiqPlatform}\*.*

; copy liquidDLL.mod to 
;setOutPath $DOCUMENTS\maya\2013-x64\modules
;file ${LIQUID_ROOT}\2.3.3\bin\${LiqPlatform}\${LiqConfiguration}\liquidDLL.mod
setOutPath $INSTDIR\2.3.3\bin\${LiqPlatform}\${LiqConfiguration}
file ${LIQUID_ROOT}\2.3.3\bin\${LiqPlatform}\${LiqConfiguration}\liquidDLL.mod

setOutPath $INSTDIR\2.3.3\bin\${LiqPlatform}\${LiqConfiguration}\plug-ins
file /x *.pdb /x *.lib /x *.exp   ${LIQUID_ROOT}\2.3.3\bin\${LiqPlatform}\${LiqConfiguration}\plug-ins\*.*

setOutPath $INSTDIR\2.3.3\doc
file ${LIQUID_ROOT}\2.3.3\doc\*.txt
file ${LIQUID_ROOT}\2.3.3\doc\*.pdf
file ${LIQUID_ROOT}\2.3.3\doc\*.ma


setOutPath $INSTDIR\2.3.3\icons
file ${LIQUID_ROOT}\2.3.3\icons\*.*


setOutPath $INSTDIR\2.3.3\mel
file ${LIQUID_ROOT}\2.3.3\mel\*.mel

setOutPath $INSTDIR\2.3.3\mel\2011
file ${LIQUID_ROOT}\2.3.3\mel\2011\*.mel

setOutPath $INSTDIR\2.3.3\mel\2012
file ${LIQUID_ROOT}\2.3.3\mel\2012\*.mel

setOutPath $INSTDIR\2.3.3\mel\2013
file ${LIQUID_ROOT}\2.3.3\mel\2013\*.mel

setOutPath $INSTDIR\2.3.3\rendererDesc
file ${LIQUID_ROOT}\2.3.3\rendererDesc\liquidRenderer.xml

;---------------- ElvishRenderer -----------------------
setOutPath    $INSTDIR\2.3.3\src\elvishray\mel
file /r ${LIQUID_ROOT}\2.3.3\src\elvishray\mel\*.mel
setOutPath    $INSTDIR\2.3.3\src\elvishray\mel\alias
file /r ${LIQUID_ROOT}\2.3.3\src\elvishray\mel\alias\*.mel

; mod file
;setOutPath $DOCUMENTS\maya\2013-x64\modules
;file    ${LIQUID_ROOT}\2.3.3\src\elvishray\eiSHADER_user\bin\${LiqPlatform}\${LiqConfiguration}\eiSHADER_user_mayaplugin.mod
setOutPath    $INSTDIR\2.3.3\src\elvishray\eiSHADER_user\bin\${LiqPlatform}\${LiqConfiguration}
file    ${LIQUID_ROOT}\2.3.3\src\elvishray\eiSHADER_user\bin\${LiqPlatform}\${LiqConfiguration}\eiSHADER_user_mayaplugin.mod

setOutPath    $INSTDIR\2.3.3\src\elvishray\eiSHADER_user\bin\${LiqPlatform}\${LiqConfiguration}\plug-ins
file    ${LIQUID_ROOT}\2.3.3\src\elvishray\eiSHADER_user\bin\${LiqPlatform}\${LiqConfiguration}\plug-ins\*.*

setOutPath    $INSTDIR\2.3.3\src\elvishray\eiSHADER_user\src
file /r ${LIQUID_ROOT}\2.3.3\src\elvishray\eiSHADER_user\src\*.*

setOutPath $INSTDIR\dependence\elvishray\r274\bin_x64
file ${LIQUID_ROOT}\dependence\elvishray\r274\bin_x64\*.*


;---------------- 3delight ----------------------------
setOutPath    $INSTDIR\2.3.3\src\renderman\mel
file /r ${LIQUID_ROOT}\2.3.3\src\renderman\mel\*.mel

; mod file
;setOutPath $DOCUMENTS\maya\2013-x64\modules
;file    ${LIQUID_ROOT}\2.3.3\src\renderman\rmSHADER_user\bin\${LiqPlatform}\${LiqConfiguration}\rmSHADER_user_mayaplugin.mod
setOutPath    $INSTDIR\2.3.3\src\renderman\rmSHADER_user\bin\${LiqPlatform}\${LiqConfiguration}
file    ${LIQUID_ROOT}\2.3.3\src\renderman\rmSHADER_user\bin\${LiqPlatform}\${LiqConfiguration}\rmSHADER_user_mayaplugin.mod

setOutPath    $INSTDIR\2.3.3\src\renderman\rmSHADER_user\bin\${LiqPlatform}\${LiqConfiguration}\plug-ins
file    ${LIQUID_ROOT}\2.3.3\src\renderman\rmSHADER_user\bin\${LiqPlatform}\${LiqConfiguration}\plug-ins\*.*

setOutPath    $INSTDIR\2.3.3\src\renderman\rmSHADER_user\src
file /r ${LIQUID_ROOT}\2.3.3\src\renderman\rmSHADER_user\src\*.*

setOutPath    $INSTDIR\2.3.3\lib\shaders
file /r ${LIQUID_ROOT}\2.3.3\lib\shaders\*.*

setOutPath $INSTDIR\dependence\_3delight
file ${LIQUID_ROOT}\dependence\_3delight\*.*



;copy manage.ini to $(MAYA_ROOT)/bin
;read env MAYA_PATH2013
ReadRegStr $0 HKCU "Environment" "MAYA_PATH2013"
setOutPath $0\bin
file ${LIQUID_ROOT}\dependence\elvishray\r274\bin_x64\manager.ini

;append searchpath to manager.ini
Sleep 100
FileOpen  $1 $0\bin\manager.ini a
FileSeek  $1 0 END
FileWrite $1 "$\r$\nsearchpath       $INSTDIR/dependence/elvishray/r274/bin_x64"
FileClose $1

MessageBox MB_OK "Please update your *.mod files(e.g. liquidDLL.mod, eiSHADER_user_mayaplugin.mod, rmSHADER_user_mayaplugin.mod)"

sectionEnd