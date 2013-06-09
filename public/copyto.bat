rem ---------------------------zip-----------------------------------------------
set ZIP="C:\Program Files\7-Zip\7z.exe"

rem ---------------------------soruce-----------------------------------------------
set SRC_ROOT=E:\dev\Autodesk\maya\myplugin\project\liquid_


rem ---------------------------destination------------------------------------------
set DES_ROOT=E:\dev\Autodesk\maya\myplugin\project\liquid_\public\v1








rem ---- 2.3.3 ----
xcopy    /d /y /i /EXCLUDE:exclude.txt   %SRC_ROOT%\2.3.3\bin\*.*                    %DES_ROOT%\2.3.3\bin\
xcopy    /d /y /i /EXCLUDE:exclude.txt   %SRC_ROOT%\2.3.3\bin\Prman\win32\*.*        %DES_ROOT%\2.3.3\bin\Prman\win32\
xcopy    /d /y /i /EXCLUDE:exclude.txt   %SRC_ROOT%\2.3.3\bin\Prman\win32\M2R2012Release\*.*        %DES_ROOT%\2.3.3\bin\Prman\win32\M2R2012Release\
xcopy    /d /y /i /EXCLUDE:exclude.txt   %SRC_ROOT%\2.3.3\doc\*.txt                  %DES_ROOT%\2.3.3\doc\
xcopy    /d /y /i /EXCLUDE:exclude.txt   %SRC_ROOT%\2.3.3\doc\*.pdf                  %DES_ROOT%\2.3.3\doc\
xcopy    /d /y /i /EXCLUDE:exclude.txt   %SRC_ROOT%\2.3.3\doc\*.ma                   %DES_ROOT%\2.3.3\doc\
xcopy    /d /y /i /EXCLUDE:exclude.txt   %SRC_ROOT%\2.3.3\icons\*.*                  %DES_ROOT%\2.3.3\icons\
xcopy /s /d /y /i /EXCLUDE:exclude.txt   %SRC_ROOT%\2.3.3\mel\*.*                    %DES_ROOT%\2.3.3\mel\
xcopy    /d /y /i /EXCLUDE:exclude.txt   %SRC_ROOT%\2.3.3\rendererDesc\*.*           %DES_ROOT%\2.3.3\rendererDesc\
xcopy    /d /y /i /EXCLUDE:exclude.txt   %SRC_ROOT%\2.3.3\renderers\*.*              %DES_ROOT%\2.3.3\renderers\
xcopy /s /d /y /i /EXCLUDE:exclude.txt   %SRC_ROOT%\2.3.3\src\elvishray\mel\*.mel    %DES_ROOT%\2.3.3\src\elvishray\mel\
xcopy    /d /y /i /EXCLUDE:exclude.txt   %SRC_ROOT%\2.3.3\src\renderman\mel\*.mel    %DES_ROOT%\2.3.3\src\renderman\mel\
xcopy /s /d /y /i /EXCLUDE:exclude.txt   %SRC_ROOT%\2.3.3\lib\shaders\*.*            %DES_ROOT%\2.3.3\lib\shaders\


rem ---- dependence ----
xcopy    /d /y /i /EXCLUDE:exclude.txt   %SRC_ROOT%\dependence\_3delight\*.*         %DES_ROOT%\dependence\_3delight\
xcopy    /d /y /i /EXCLUDE:exclude.txt   %SRC_ROOT%\dependence\elvishray\r274\bin\*.*         %DES_ROOT%\dependence\elvishray\r274\bin\




%ZIP%  u  .\maya2renderer.7z  %DES_ROOT%\









