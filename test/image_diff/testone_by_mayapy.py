import sys
import os
import maya.standalone
import maya.OpenMaya    as OpenMaya
import maya.cmds        as cmds
import maya.mel         as mel
import liqGlobalHelpers as gHelper
import liqTestLog       as mLiqlog
import render_tester    as rt
import elvishray        as er

def parseString(strdata):
    import maya.cmds as cmds
    import re

    if strdata.find('$CAM')!=-1:
        strinfo = re.compile('\$CAM')
        strdata = strinfo.sub(cmds.getAttr('liquidGlobals.renderCamera'), strdata )

    if strdata.find('$F')!=-1:
        strinfo = re.compile('\$F')
        frame = "%d" % cmds.currentTime( query=True ) 
        strdata = strinfo.sub( frame, strdata)

    if strdata.find('$SCN')!=-1:
        strinfo = re.compile('\$SCN')
        strdata = strinfo.sub( gHelper.liquidTransGetSceneName(), strdata)

    if strdata.find('$IMG')!=-1:
        strinfo = re.compile('\$IMG')
        strdata = strinfo.sub( cmds.getAttr('liquidGlobals.ddImageName[0]'), strdata)

    if strdata.find('$PDIR')!=-1:
        strinfo = re.compile('\$PDIR')
        strdata = strinfo.sub( cmds.workspace( q=True, dir=True ), strdata)

    if strdata.find('$RND')!=-1:
        strinfo = re.compile('\$RND')
        strdata = strinfo.sub( cmds.getAttr('defaultRenderGlobals.currentRenderer'), strdata)

    if strdata.find('#')!=-1:
        strinfo = re.compile('\#')
        frame = "%04d" % cmds.currentTime( query=True ) 
        #frame = "{0:04d}".format(cmds.currentTime( query=True )) 
        strdata = strinfo.sub( frame, strdata)

    # TODO ...

    return strdata


def getOutputImageName(mayaFile, liqRenderer):
    cmds.file(mayaFile, force=True, open=True)
    output_image_name = parseString( cmds.getAttr('liquidGlobals.ddImageName[0]') )
    #cmds.file(save=True, force=True)
    return output_image_name


def getProjectDirectory(mayaFile):
    cmds.file(mayaFile, force=True, open=True)
    projectDir = cmds.workspace( q=True, fullName=True )
    #cmds.file(save=True, force=True)
    return projectDir+"/"


def getImageOutputDirectory(mayaFile):
    return getProjectDirectory(mayaFile)+"rmanpix/"

def getFileDirectory(mayaFile):
    slat_i=mayaFile.rfind('/')
    return mayaFile[ 0 : slat_i ]


def getStanderImage(mayaFile, liqRenderer):
    directory=getFileDirectory(mayaFile)
    imageName=getOutputImageName(mayaFile, liqRenderer)
    return (directory+"/"+imageName)


def _render(mayaFile, liqRenderer):
    batchRenderCmd="mayabatch.exe "
    command = """ "source \\"liquidStartup.mel\\"; "source \\"liq_RendererName.mel\\"; liquidStartup(); catch(`loadPlugin """+liqRenderer+"""`); mayaBatchRenderProcedure(0,\\"\\",\\"\\",liq_getRendererName(),\\"\\");" """;
    cmd = batchRenderCmd+" -file "+mayaFile+" -command "+command;

    #mLiqlog.flog("batchRenderCmd="+cmd)
    os.system(cmd)


def _render2(mayafile, liqRenderer):
    cmds.file( mayafile, f=True,  options="v=0", typ="mayaAscii", o=True)
    command = 'catch(`loadPlugin '+liqRenderer+'`); '
    mel.eval(command)
    
    mel.eval('mayaBatchRenderProcedure(0,"","","'+liqRenderer+'","")')


def _test(mayaFile, liqRenderer):
    print("\n\n\n\n\n\n")
    print("----------------------------------------------------------\n")
    print("test begin <"+mayaFile+">, <"+liqRenderer+">\n")
    print("----------------------------------------------------------\n")
    mLiqlog.renderer_beg(liqRenderer)

    output_image_fullpath = getImageOutputDirectory(mayaFile)+getOutputImageName(mayaFile, liqRenderer)
    mLiqlog.output_img_beg(getStanderImage(mayaFile, liqRenderer))
    mLiqlog.output_img_end()

    #_render(mayaFile, liqRenderer)
    _render2(mayaFile, liqRenderer)

    import time
    time.sleep(4)

    std_image_fullpath = getStanderImage(mayaFile, liqRenderer);
    import imagediff
    imagediff.compare(output_image_fullpath, std_image_fullpath)
    
    mLiqlog.renderer_end()
    print("test <"+mayaFile+"> \nwith <"+liqRenderer+">  is done.\n")
    
    #raw_input("Press ENTER to continue.")


def test_one_scene(mayaFile, liqRenderer):
    mLiqlog.scene_beg(mayaFile)

    #create tester
    tester = rt.createTestFactory( liqRenderer )
    tester.setFile( mayaFile )
    tester.setProjectDirectory( getProjectDirectory(mayaFile) )

    if liqRenderer == "":
        _test(mayaFile, er.getRendererName())
        _test(mayaFile, "renderman")
        _test(mayaFile, "appleseed")
    else:
        _test(mayaFile, liqRenderer)
    
    #DEBUG
    #import pdb 
    #pdb.set_trace() 

    #clean resources
    import time
    time.sleep(2)
    tester.cutResources()

    mLiqlog.scene_end()


def test_getStanderImage():
    configMayaEnv()
    initialize()
    print getStanderImage("E:/dev/Autodesk/maya/myplugin/project/liquid_/2.3.3/test/test_motionblur/transform/deform.ma", "renderman");
