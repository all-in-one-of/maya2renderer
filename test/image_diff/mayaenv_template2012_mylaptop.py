import os

def configMayaEnv():
    #liquid maya
    os.environ["LIQUID_ROOT"]    = "E:/dev/autodesk/maya/myplugin/project/liquid_"
    os.environ["LIQUID_MEL"]     = os.environ["LIQUID_ROOT"]+"/2.3.3/mel"
    os.environ["LIQUID_AS_MEL"]  = os.environ["LIQUID_ROOT"]+"/2.3.3/src/appleseed/mel"
    os.environ["LIQUID_RM_MEL"]  = os.environ["LIQUID_ROOT"]+"/2.3.3/src/renderman/mel"
    os.environ["LIQUID_ER_MEL"]  = os.environ["LIQUID_ROOT"]+"/2.3.3/src/elvishray/mel"
    os.environ["LIQUID_LUX_MEL"] = os.environ["LIQUID_ROOT"]+"/2.3.3/src/luxrenderer/mel"
    os.environ["LIQUID_GEN"]     = os.environ["LIQUID_ROOT"]+"/2.3.3/lib/plugins"
    #Debug
    #os.environ["LIQUID_MLL"]     = os.environ["LIQUID_ROOT"]+"/2.3.3/bin/prman/win32"
    #raw_input("this is Debug version. Press ENTER to continue.")
    #Release
    os.environ["LIQUID_MLL"]     = os.environ["LIQUID_ROOT"]+"/2.3.3/bin/prman/win32/M2R2012Release"
    raw_input("this is Release version. Press ENTER to continue.")

    os.environ["LIQUID_ICON"]    = os.environ["LIQUID_ROOT"]+"/2.3.3/icons"
    os.environ["LIQUIDHOME"]     = os.environ["LIQUID_ROOT"]+"/2.3.3"
    os.environ["LIQUID_GLOBALS_OVERRIDE"] = os.environ["LIQUID_ROOT"]+"/2.3.3/renderers"
    #elvishray
    os.environ["ELVISHRAY_BIN"] = os.environ["LIQUID_ROOT"]+"/dependence/elvishray/r274/bin"
    #appleseed
    os.environ["APPLESEED_BIN"] = os.environ["LIQUID_ROOT"]+"/dependence/appleseed/appleseed-1.1.0-alpha-14-0-g4e46ed9-win32-vs100-devkit/bin/Release" 
    
    # PATH
    os.environ["PATH"] = os.environ["APPLESEED_BIN"]+";"+os.environ["ELVISHRAY_BIN"]+";"+os.environ["PATH"]
    # MAYA_SCRIPT_PATH
    if os.environ.get('MAYA_SCRIPT_PATH') == None:
        os.environ["MAYA_SCRIPT_PATH"] =os.environ["LIQUID_AS_MEL"]+";"+os.environ["LIQUID_RM_MEL"]+";"+os.environ["LIQUID_ER_MEL"]+";"+os.environ["LIQUID_LUX_MEL"]+";"+os.environ["LIQUID_GEN"]+";"+os.environ["LIQUID_MEL"];
    else:
        os.environ["MAYA_SCRIPT_PATH"] =os.environ["LIQUID_AS_MEL"]+";"+os.environ["LIQUID_RM_MEL"]+";"+os.environ["LIQUID_ER_MEL"]+";"+os.environ["LIQUID_LUX_MEL"]+";"+os.environ["LIQUID_GEN"]+";"+os.environ["LIQUID_MEL"]+";"+os.environ["MAYA_SCRIPT_PATH"];
    # MAYA_PLUG_IN_PATH
    if os.environ.get('MAYA_PLUG_IN_PATH') == None:
        os.environ["MAYA_PLUG_IN_PATH"]=os.environ["LIQUID_MLL"]
    else:
        os.environ["MAYA_PLUG_IN_PATH"]=os.environ["LIQUID_MLL"]+";"+os.environ["MAYA_PLUG_IN_PATH"]
    # XBMLANGPATH
    #if os.environ.get('MAYA_PLUG_IN_PATH') == None:
    #    os.environ["XBMLANGPATH"]=os.environ["LIQUID_ICON"]
    #else:
    #    os.environ["XBMLANGPATH"]=os.environ["LIQUID_ICON"]+";"+os.environ["XBMLANGPATH"]
    #
    os.environ["MAYA_LOCATION"] = "C:/Program Files (x86)/Autodesk/Maya2012"
    os.environ["MAYA_PATH2012"] = "C:/Program Files (x86)/Autodesk/Maya2012"
    os.environ["PYTHONHOME"]    = "C:/Program Files (x86)/Autodesk/Maya2012/Python"
    os.environ["PYTHONPATH"]    = "C:/Program Files (x86)/Autodesk/Maya2012/bin/python26.zip;C:/Program Files (x86)/Autodesk/Maya2012/Python/lib/site-packages;"
    os.environ["MI_CUSTOM_SHADER_PATH"]    = "C:/Program Files (x86)/Autodesk/Maya2012/mentalray/include"
    os.environ["MI_CUSTOM_SHADER_SUPPRESS"]= ";mayabase.mi;mayahair.mi;AdskShaderSDK.mi;MayaShaders.mi;AdskShaders.mi;adsk_Utility_Shaders.mi;adsk_Utility_Spiders.mi;lume.mi"
