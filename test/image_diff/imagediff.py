import sys
import os
import hashlib
import liqTestLog as mLiqlog


def getDiffImageName(fileFullPath):
    diff_postfix="_diff";
    dot_i=fileFullPath.rfind('.')
    fileFullPath_withoutExt = fileFullPath[ 0 : dot_i ]
    fileExt = fileFullPath[(dot_i+1):]
    return (fileFullPath_withoutExt+diff_postfix+"."+fileExt)

def doDiff(filenameA, filenameB, diffFileName):
    cmd=".\\ImageMagick\\compare.exe "+filenameA+' '+filenameB+' '+diffFileName
    #print("cmd="+cmd+"\n")
    os.system(cmd)


def _compare(filenameA, filenameB):
    dataA = open(filenameA, 'rb').read()
    dataB = open(filenameB, 'rb').read()

    return (hashlib.md5(dataA).hexdigest() == hashlib.md5(dataB).hexdigest())==True


def compare(filenameA, filenameB):
    #print("compare("+filenameA+","+filenameB+")\n")
    if os.path.isfile(filenameA) == False:
        mLiqlog.warn(filenameA+" not exist.")
        return
    if os.path.isfile(filenameB) == False:
        mLiqlog.warn(filenameB+" not exist.")
        return

    if _compare(filenameA, filenameB)==True:
        mLiqlog.flog("two images are equal.")
    else:
        diffFileName = getDiffImageName(filenameB)
        doDiff( filenameA, filenameB, diffFileName )

        mLiqlog.imgdiff_beg("WARNING: Two images are NOT equal! see the diff-image:", diffFileName)
        mLiqlog.imgdiff_end()