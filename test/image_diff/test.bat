@echo off

echo Automation test for maya2renderer
echo .
echo .

IF EXIST %CD%\..\..\dependence/python26 (
   %CD%\..\..\dependence/python26/python.exe test.py
) ELSE (
   ECHO ..\..\dependence/python26/python.exe not exist, 
   ECHO Please unzip ..\..\dependence/python26.7z to that directory, and make sure ..\..\dependence/python26/python.exe exists.
)

pause