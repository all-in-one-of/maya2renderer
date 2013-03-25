import sys
import os

def getFiles (dir_, wildcard_):
	import glob
	file_names = glob.glob(dir_+"/"+wildcard_)
	return file_names

def getProjectDir():
    return "E:/MyDocuments/maya/projects/default/"


def cutFiles (srcdir_, wildcard_, desdir_):
    # copy logs
    for src in getFiles(srcdir_, wildcard_):
        basename = os.path.basename( src )
        des = desdir_+"/"+basename
        #copy src to des
        import shutil
        print("copying: \n"+src+" ==> "+des)
        shutil.copy (src, des)
        #remove src
        os.remove( src )

def _test (mayaFile):
    desdir = os.path.dirname(mayaFile)

    #scene files
    ribdir = getProjectDir()+"/rib"
    cutFiles(ribdir, "*.rib", desdir)

    # copy logs
    logdir = getProjectDir()+"/rmanpix"
    cutFiles(logdir, "*.log", desdir)
    cutFiles(logdir, "*.bmp", desdir)
    cutFiles(logdir, "*.png", desdir)
    cutFiles(logdir, "*.jpg", desdir)
    cutFiles(logdir, "*.jpeg", desdir)

    #shaders
    shaderdir = getProjectDir()+"/generatedShader"
    cutFiles(shaderdir, "*.sl_my", desdir)



if __name__ == '__main__':
    _test(
    "E:/dev/Autodesk/maya/myplugin/project/liquid_/2.3.3/test/test_rm_light/rib/rm_spotlight.ma/rm_spotlight.ma"
    )

