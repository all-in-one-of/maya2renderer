import os
import testone_by_mayapy as MyTestOne


def test_all_scene():
    LIQUID_ROOT= os.environ["LIQUID_ROOT"]

    testFile=LIQUID_ROOT+"/2.3.3/test/test_rm_texture/check12_bmp.ma"
    MyTestOne.test_one_scene(testFile, "renderman")
    #testFile=LIQUID_ROOT+"/2.3.3/test/test_lightlink/test_lightlink.ma"
    #MyTestOne.test_one_scene(testFile, "")

    #testFile=LIQUID_ROOT+"/test/animation_test/Trex/Trex/scenes/TrexAnimation_liq_anim_800x600.ma"
    #MyTestOne.test_one_scene(testFile, "renderman")

