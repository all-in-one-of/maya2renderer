import sys
import os
import cutResources

#-----------------
class TestBase:
    m_mayafile = ""
    m_projectDir = ""

    def __init__ (self):
	pass

    def setFile (self, mayafile):
        self.m_mayafile = mayafile

    def setProjectDirectory (self, projdir):
        self.m_projectDir = projdir

    def getProjectDirectory(self):
        return self.m_projectDir
	
    def cutResources (self):
	pass
	
    def getFiles (self, dir_, wildcard_):
	import glob
	file_names = glob.glob(dir_+"/"+wildcard_)
	return file_names

    def cutFiles (self, srcdir_, wildcard_, desdir_):
        # copy logs
        for src in self.getFiles(srcdir_, wildcard_):
            basename = os.path.basename( src )
            des = desdir_+"/"+basename
            #copy src to des
            import shutil
            print("copying: \n"+src+" ==> "+des)
            shutil.copy (src, des)
            #remove src
            os.remove( src )

#-----------------
class RendermanTest(TestBase):
    def __init__ (self):
	pass

    def cutResources (self):
        desdir = os.path.dirname(self.m_mayafile)

        #scene files
        ribdir = self.getProjectDirectory()+"/rib"
        self.cutFiles(ribdir, "*.rib", desdir)

        # copy logs
        logdir = self.getProjectDirectory()+"/rmanpix"
        self.cutFiles(logdir, "*.log", desdir)
        self.cutFiles(logdir, "*.bmp", desdir)
        self.cutFiles(logdir, "*.png", desdir)
        self.cutFiles(logdir, "*.jpg", desdir)
	self.cutFiles(logdir, "*.jpeg", desdir)

        # copy tex
        logdir = self.getProjectDirectory()+"/rmantex"
        self.cutFiles(logdir, "*.tdl", desdir)

        #shaders
        shaderdir = self.getProjectDirectory()+"/generatedShader"
        self.cutFiles(shaderdir, "*.sl_my", desdir)
	
#-----------------
class ElvishrayTest(TestBase):
    def __init__ (self):
	pass

    def cutResources (self):
        desdir = os.path.dirname(self.m_mayafile)

        #scene files
        ribdir = self.getProjectDirectory()+"/rib"
        self.cutFiles(ribdir, "*.er", desdir)

        # copy logs
        logdir = self.getProjectDirectory()+"/rmanpix"
        self.cutFiles(logdir, "*.log", desdir)
        self.cutFiles(logdir, "*.bmp", desdir)
        self.cutFiles(logdir, "*.png", desdir)
        self.cutFiles(logdir, "*.jpg", desdir)
	self.cutFiles(logdir, "*.jpeg", desdir)

#-----------------
def createTestFactory ( renderer_ ):

    if renderer_ == "renderman" :
	return RendermanTest()

    if renderer_ == "elvishray" :
	return ElvishrayTest()

