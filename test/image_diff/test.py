import sys
import os
import liqTestLog as mLiqlog

def getHTMLViewerPath_windows ():

	import _winreg

	IEApp_key = _winreg.OpenKey(_winreg.HKEY_CURRENT_USER, r"Software\Clients\StartMenuInternet")
	IEApp_value, IEApp_type = _winreg.QueryValueEx(IEApp_key, "")
	#print("IE="+IEApp_value+"\n")

	IEFullPath_key = _winreg.OpenKey(_winreg.HKEY_LOCAL_MACHINE,r"SOFTWARE\Clients\StartMenuInternet\\"+IEApp_value+"\shell\open\command")
	IEFullPath_value, IEFullPath_type = _winreg.QueryValueEx(IEFullPath_key, "")
	#print("IEFullPath="+IEFullPath_value+"\n")

	return IEFullPath_value


HTMLViewerPath = getHTMLViewerPath_windows();
pyFile  = "testall.py";

def main():

    command = "mayapy.exe "+pyFile;
    os.system(command)

    # open the log file
    if HTMLViewerPath=="":
        os.system("explorer.exe .\\_liqTestLog.htm")
    else:
        os.system("\""+HTMLViewerPath+"\" ./_liqTestLog.htm")
    raw_input("main() done. Press ENTER to exit.")

if __name__ == '__main__':
    main()