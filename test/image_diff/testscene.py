import os
import testone_by_mayapy as MyTestOne


def test_all_scene():
    LIQUID_ROOT= os.environ["LIQUID_ROOT"]
    '''
    ------------------------ renderman ----------------------------------
    '''
    '''
    testFile=LIQUID_ROOT+"/test/unittest/rm/motionblur/deform/deform.ma"
    MyTestOne.test_one_scene(testFile, "renderman")
    
    testFile=LIQUID_ROOT+"/test/unittest/rm/motionblur/transform/transform.ma"
    MyTestOne.test_one_scene(testFile, "renderman")
    
    testFile=LIQUID_ROOT+"/test/unittest/rm/motionblur/deform_tex/deform_tex.ma"
    MyTestOne.test_one_scene(testFile, "renderman")
    
    testFile=LIQUID_ROOT+"/test/unittest/rm/motionblur/transform_tex/transform_tex.ma"
    MyTestOne.test_one_scene(testFile, "renderman")
    ''''''
    testFile=LIQUID_ROOT+"/test/unittest/rm/texture/texture.ma"
    MyTestOne.test_one_scene(testFile, "renderman")
    
    testFile=LIQUID_ROOT+"/test/unittest/rm/texture/texture_liquidShader_tif/texture_liquidShader_tif.ma"
    MyTestOne.test_one_scene(testFile, "renderman")    
     
    testFile=LIQUID_ROOT+"/test/unittest/rm/texture/texture_liquidShader_png/texture_liquidShader_png.ma"
    MyTestOne.test_one_scene(testFile, "renderman")  
    
    testFile=LIQUID_ROOT+"/test/unittest/rm/texture/texture_liquidSurface_tif/texture_liquidSurface_tif.ma"
    MyTestOne.test_one_scene(testFile, "renderman")  
    
    testFile=LIQUID_ROOT+"/test/maya_shader/file_place2dTexture/file_place2dTexture.ma"
    MyTestOne.test_one_scene(testFile, "renderman")
     
    testFile=LIQUID_ROOT+"/test/maya_shader/file_place2dTexture/sequence/file_place2dTexture_seq.ma"
    MyTestOne.test_one_scene(testFile, "renderman")
    
    testFile=LIQUID_ROOT+"/test/maya_shader/file_place2dTexture/uvCoord/uvCoord.ma"
    MyTestOne.test_one_scene(testFile, "renderman")
    
    testFile=LIQUID_ROOT+"/test/unittest/rm/test_pl/test_pl_type.ma"
    MyTestOne.test_one_scene(testFile, "renderman")
    
    testFile=LIQUID_ROOT+"/test/unittest/rm/test_pl/test_pl_type_array.ma"
    MyTestOne.test_one_scene(testFile, "renderman")
    
    testFile=LIQUID_ROOT+"/test/unittest/rm/test_pl/test_pl_type_liquidShader.ma"
    MyTestOne.test_one_scene(testFile, "renderman")
    
    #test_pl_type_array_liquidShader.ma is BAD
    #testFile=LIQUID_ROOT+"/test/unittest/rm/test_pl/test_pl_type_array_liquidShader.ma"
    #MyTestOne.test_one_scene(testFile, "renderman")
    

    # limitation:
    # - If  a plug of liquidShader is input AND output
    # - If colorR instead of color in connected.
    testFile=LIQUID_ROOT+"/test/unittest/rm/test_pl/test_pl_type_liquidShader_con.ma"
    MyTestOne.test_one_scene(testFile, "renderman")
    
    testFile=LIQUID_ROOT+"/test/unittest/rm/test_pl/test_pl_type_liquidShader_con_colorr.ma"
    MyTestOne.test_one_scene(testFile, "renderman")
    
    testFile=LIQUID_ROOT+"/test/unittest/rm/test_pl/test_pl_type_liquidShader_conIO.ma"
    MyTestOne.test_one_scene(testFile, "renderman")
    
    testFile=LIQUID_ROOT+"/test/unittest/rm/test_pl/test_pl_liquidShader_con_ry/test_pl_liquidShader_con_ry.ma"
    MyTestOne.test_one_scene(testFile, "renderman")
    
    testFile=LIQUID_ROOT+"/test/unittest/rm/test_pl/test_pl_liquidShader_con_tx/test_pl_liquidShader_con_tx.ma"
    MyTestOne.test_one_scene(testFile, "renderman")
    
    # bad, it has a global displacement shader
    testFile=LIQUID_ROOT+"/test/unittest/rm/displacement/displacement.ma"
    MyTestOne.test_one_scene(testFile, "renderman")
    
    #testFile=LIQUID_ROOT+"/test/maya_shader/shader_graph/shader_graph/shader_graph.ma"
    #MyTestOne.test_one_scene(testFile, "renderman")
    
    testFile=LIQUID_ROOT+"/test/maya_shader/shader_graph/shader_graph_phong/shader_graph_phong.ma"
    MyTestOne.test_one_scene(testFile, "renderman")
     
    testFile=LIQUID_ROOT+"/test/maya_shader/shader_graph/shader_graph_phong_transparent/shader_graph_phong_transparent.ma"
    MyTestOne.test_one_scene(testFile, "renderman")
    
    testFile=LIQUID_ROOT+"/test/maya_shader/shader_graph/three_boxes/three_boxes.ma"
    MyTestOne.test_one_scene(testFile, "renderman")
    
    testFile=LIQUID_ROOT+"/test/maya_shader/lambert/simple/lambert.ma"
    MyTestOne.test_one_scene(testFile, "renderman")
    
    testFile=LIQUID_ROOT+"/test/maya_shader/lambert/texture/lambert_tex.ma"
    MyTestOne.test_one_scene(testFile, "renderman")
    
    testFile=LIQUID_ROOT+"/test/maya_shader/lambert/transparent/lambert_transparent.ma"
    MyTestOne.test_one_scene(testFile, "renderman")
    
    testFile=LIQUID_ROOT+"/test/maya_shader/phong/simple/phong.ma"
    MyTestOne.test_one_scene(testFile, "renderman")
    
    testFile=LIQUID_ROOT+"/test/maya_shader/phong/texture/phong_tex.ma"
    MyTestOne.test_one_scene(testFile, "renderman")
    
    testFile=LIQUID_ROOT+"/test/maya_shader/phong/transparent/phong_transparent.ma"
    MyTestOne.test_one_scene(testFile, "renderman")
    
    testFile=LIQUID_ROOT+"/test/maya_shader/blinn/simple/blinn.ma"
    MyTestOne.test_one_scene(testFile, "renderman")
    
    testFile=LIQUID_ROOT+"/test/maya_shader/blinn/texture/blinn_tex.ma"
    MyTestOne.test_one_scene(testFile, "renderman")
    
    testFile=LIQUID_ROOT+"/test/maya_shader/blinn/transparent/blinn_transparent.ma"
    MyTestOne.test_one_scene(testFile, "renderman")
    
    testFile=LIQUID_ROOT+"/test/maya_shader/pointlight/pointlight.ma"
    MyTestOne.test_one_scene(testFile, "renderman")
    
    testFile=LIQUID_ROOT+"/test/maya_shader/spotlight/spotlight.ma"
    MyTestOne.test_one_scene(testFile, "renderman")

    testFile=LIQUID_ROOT+"/test/maya_shader/directionallight/directionallight.ma"
    MyTestOne.test_one_scene(testFile, "renderman")

    testFile=LIQUID_ROOT+"/test/maya_shader/ambientlight/ambientlight.ma"
    MyTestOne.test_one_scene(testFile, "renderman")
    
    #Bad, it is black
    #3DL WARNING P1165: undefined coordinate system '_areaLight1_areaLightShape1CoordSys' in shader (in shader 'liquidarea' on object '_pPlane2_pPlaneShape2')
    testFile=LIQUID_ROOT+"/test/maya_shader/arealight/arealight.ma"
    MyTestOne.test_one_scene(testFile, "renderman")
    
    testFile=LIQUID_ROOT+"/test/unittest/rm/dof/dof.ma"
    MyTestOne.test_one_scene(testFile, "renderman")
    
    testFile=LIQUID_ROOT+"/test/maya_shader/checker/checker.ma"
    MyTestOne.test_one_scene(testFile, "renderman")
    
    #animation
    testFile=LIQUID_ROOT+"/test/unittest/rm/animation_checker/checker_anim.ma"
    MyTestOne.test_one_scene(testFile, "renderman")
    
    testFile=LIQUID_ROOT+"/test/unittest/rm/pfxhair/onehair.ma"
    MyTestOne.test_one_scene(testFile, "renderman")
    
    testFile=LIQUID_ROOT+"/test/unittest/rm/subd/simple/subd.ma"
    MyTestOne.test_one_scene(testFile, "renderman")

    testFile=LIQUID_ROOT+"/test/unittest/rm/subd/tex/subd_tex.ma"
    MyTestOne.test_one_scene(testFile, "renderman")
    
    testFile=LIQUID_ROOT+"/test/unittest/rm/vertex_normal/simple/vertex_normal.ma"
    MyTestOne.test_one_scene(testFile, "renderman")
    
    testFile=LIQUID_ROOT+"/test/unittest/rm/deepshadow/deepshadow.ma"
    MyTestOne.test_one_scene(testFile, "renderman")
    
    # The dependence on mtomr.mll leads to a crash
    #testFile=LIQUID_ROOT+"/test/maya_shader/mib_amb_occlusion/mib_amb_occlusion.ma"
    #MyTestOne.test_one_scene(testFile, "renderman")
    
    testFile=LIQUID_ROOT+"/test/unittest/rm/object_hierarchy/object_hierarchy/object_hierarchy.ma"
    MyTestOne.test_one_scene(testFile, "renderman")

    testFile=LIQUID_ROOT+"/test/unittest/rm/object_hierarchy/object_hierarchy_mb/object_hierarchy_mb.ma"
    MyTestOne.test_one_scene(testFile, "renderman")

    testFile=LIQUID_ROOT+"/test/unittest/rm/object_hierarchy/object_hierarchy_simple/object_hierarchy_simple.ma"
    MyTestOne.test_one_scene(testFile, "renderman")
    
    testFile=LIQUID_ROOT+"/test/unittest/rm/lightlink/lightlink.ma"
    MyTestOne.test_one_scene(testFile, "renderman")

    testFile=LIQUID_ROOT+"/test/animation_test/MoxMotionMeetsFBIK/MoxMotionMeetsFBIK/scenes/texture_anim2/texture_anim2.ma"
    MyTestOne.test_one_scene(testFile, "renderman")
    
    testFile=LIQUID_ROOT+"/test/animation_test/MoxMotionMeetsFBIK/MoxMotionMeetsFBIK/scenes/texture_anim1/texture_anim1.ma"
    MyTestOne.test_one_scene(testFile, "renderman")
    testFile=LIQUID_ROOT+"/test/animation_test/HappyWalk/HappyWalk_liq_simple.ma"
    MyTestOne.test_one_scene(testFile, "renderman")

    
    testFile=LIQUID_ROOT+"/test/unittest/rm/particle/simple/particle.ma"
    MyTestOne.test_one_scene(testFile, "renderman")

    testFile=LIQUID_ROOT+"/test/unittest/rm/particle/ParticleColorChangeOnCollision/ParticleColorChangeOnCollision.ma"
    MyTestOne.test_one_scene(testFile, "renderman")

    testFile=LIQUID_ROOT+"/test/unittest/rm/particle/TextureBaseParticleRamp/TextureBaseParticleRamp.ma"
    MyTestOne.test_one_scene(testFile, "renderman")

    testFile=LIQUID_ROOT+"/test/unittest/rm/particle/particleColorChangeOnCollisionMethod1/particleColorChangeOnCollisionMethod1.ma"
    MyTestOne.test_one_scene(testFile, "renderman")

    testFile=LIQUID_ROOT+"/test/unittest/rm/aov/aov.ma"
    MyTestOne.test_one_scene(testFile, "renderman")

    testFile=LIQUID_ROOT+"/test/unittest/rm/procedural/runprogram/runprogram_polygen.ma"
    MyTestOne.test_one_scene(testFile, "renderman")

    testFile=LIQUID_ROOT+"/test/unittest/rm/procedural/dynamicload/dynamicload.ma"
    MyTestOne.test_one_scene(testFile, "renderman")
    
    testFile=LIQUID_ROOT+"/test/unittest/common/nurbs_surface/nurbs_sphere/nurbs_sphere.ma"
    MyTestOne.test_one_scene(testFile, "renderman")

    testFile=LIQUID_ROOT+"/test/unittest/common/nurbs_surface/nurbs_cone/nurbs_cone.ma"
    MyTestOne.test_one_scene(testFile, "renderman")

    testFile=LIQUID_ROOT+"/test/unittest/common/nurbs_surface/nurbs_surface/nurbs_surface.ma"
    MyTestOne.test_one_scene(testFile, "renderman")
    
    testFile=LIQUID_ROOT+"/test/maya_shader/bump2d/colorbump/bump2d_colorbump.ma"
    MyTestOne.test_one_scene(testFile, "renderman")
    
    testFile=LIQUID_ROOT+"/test/unittest/common/subd_surface/subd_sphere/subd_sphere.ma"
    MyTestOne.test_one_scene(testFile, "renderman")
    
    testFile=LIQUID_ROOT+"/test/unittest/rm/motionblur/blur_camera/blur_camera.ma"
    MyTestOne.test_one_scene(testFile, "renderman")
    '''
    


    '''
    ------------------------ elvishray ----------------------------------
    '''

    '''
    testFile=LIQUID_ROOT+"/test/unittest/er/motionblur/deform/deform.ma"
    MyTestOne.test_one_scene(testFile, "elvishray")
    
    testFile=LIQUID_ROOT+"/test/unittest/er/motionblur/transform/transform.ma"
    MyTestOne.test_one_scene(testFile, "elvishray")
    
    testFile=LIQUID_ROOT+"/test/unittest/er/motionblur/deform_tex/deform_tex.ma"
    MyTestOne.test_one_scene(testFile, "elvishray")
    
    testFile=LIQUID_ROOT+"/test/unittest/er/motionblur/transform_tex/transform_tex.ma"
    MyTestOne.test_one_scene(testFile, "elvishray")
    
    testFile=LIQUID_ROOT+"/test/maya_shader/file_place2dTexture/file_place2dTexture.ma"
    MyTestOne.test_one_scene(testFile, "elvishray")
     
    testFile=LIQUID_ROOT+"/test/maya_shader/file_place2dTexture/sequence/file_place2dTexture_seq.ma"
    MyTestOne.test_one_scene(testFile, "elvishray")
    
    testFile=LIQUID_ROOT+"/test/maya_shader/file_place2dTexture/uvCoord/uvCoord.ma"
    MyTestOne.test_one_scene(testFile, "elvishray")
    
    testFile=LIQUID_ROOT+"/test/unittest/er/texture/bmp.ma"
    MyTestOne.test_one_scene(testFile, "elvishray")

    testFile=LIQUID_ROOT+"/test/unittest/er/texture/tex.ma"
    MyTestOne.test_one_scene(testFile, "elvishray")

    testFile=LIQUID_ROOT+"/test/unittest/er/texture/tif.ma"
    MyTestOne.test_one_scene(testFile, "elvishray")
    
    testFile=LIQUID_ROOT+"/test/unittest/er/texture/jpg.ma"
    MyTestOne.test_one_scene(testFile, "elvishray")
    
    testFile=LIQUID_ROOT+"/test/unittest/er/test_pl/test_pl_type.ma"
    MyTestOne.test_one_scene(testFile, "elvishray")
    
    testFile=LIQUID_ROOT+"/test/unittest/er/test_pl/test_pl_type_liquidShader.ma"
    MyTestOne.test_one_scene(testFile, "elvishray")
    
    testFile=LIQUID_ROOT+"/test/unittest/er/test_pl/test_pl_type_liquidShader_con.ma"
    MyTestOne.test_one_scene(testFile, "elvishray")

    testFile=LIQUID_ROOT+"/test/unittest/er/test_pl/test_pl_type_liquidShader_conIO.ma"
    MyTestOne.test_one_scene(testFile, "elvishray")
    
    testFile=LIQUID_ROOT+"/test/unittest/er/displace/simple_displace.ma"
    MyTestOne.test_one_scene(testFile, "elvishray")
    
    # it is different from /test\test_shader_graph\output_img_std/er_rm_shader_graph_perspShape_1_elvishray.bmp.
    # maybe the lambert shader is not correct.
    # maya_blinn shader is needed.
    testFile=LIQUID_ROOT+"/test/maya_shader/shader_graph/shader_graph/shader_graph.ma"
    MyTestOne.test_one_scene(testFile, "elvishray")
    
    testFile=LIQUID_ROOT+"/test/maya_shader/shader_graph/shader_graph_phong/shader_graph_phong.ma"
    MyTestOne.test_one_scene(testFile, "elvishray")
    
    # not correct
    testFile=LIQUID_ROOT+"/test/maya_shader/shader_graph/shader_graph_phong_transparent/shader_graph_phong_transparent.ma"
    MyTestOne.test_one_scene(testFile, "elvishray")
    
    testFile=LIQUID_ROOT+"/test/maya_shader/shader_graph/three_boxes/three_boxes.ma"
    MyTestOne.test_one_scene(testFile, "elvishray")
    
    testFile=LIQUID_ROOT+"/test/maya_shader/lambert/simple/lambert.ma"
    MyTestOne.test_one_scene(testFile, "elvishray")
    
    testFile=LIQUID_ROOT+"/test/maya_shader/lambert/texture/lambert_tex.ma"
    MyTestOne.test_one_scene(testFile, "elvishray")
    
    testFile=LIQUID_ROOT+"/test/maya_shader/lambert/transparent/lambert_transparent.ma"
    MyTestOne.test_one_scene(testFile, "elvishray")
    
    testFile=LIQUID_ROOT+"/test/maya_shader/phong/simple/phong.ma"
    MyTestOne.test_one_scene(testFile, "elvishray")
    
    testFile=LIQUID_ROOT+"/test/maya_shader/phong/texture/phong_tex.ma"
    MyTestOne.test_one_scene(testFile, "elvishray")
    
    testFile=LIQUID_ROOT+"/test/maya_shader/phong/transparent/phong_transparent.ma"
    MyTestOne.test_one_scene(testFile, "elvishray")
    
    testFile=LIQUID_ROOT+"/test/maya_shader/blinn/simple/blinn.ma"
    MyTestOne.test_one_scene(testFile, "elvishray")
    
    testFile=LIQUID_ROOT+"/test/maya_shader/blinn/texture/blinn_tex.ma"
    MyTestOne.test_one_scene(testFile, "elvishray")
    
    testFile=LIQUID_ROOT+"/test/maya_shader/blinn/transparent/blinn_transparent.ma"
    MyTestOne.test_one_scene(testFile, "elvishray")
    
    testFile=LIQUID_ROOT+"/test/maya_shader/pointlight/pointlight.ma"
    MyTestOne.test_one_scene(testFile, "elvishray")
    testFile=LIQUID_ROOT+"/test/maya_shader/spotlight/spotlight.ma"
    MyTestOne.test_one_scene(testFile, "elvishray")
    
    testFile=LIQUID_ROOT+"/test/maya_shader/directionallight/directionallight.ma"
    MyTestOne.test_one_scene(testFile, "elvishray")
    
    # er use skylight for ambient light, but skylight shader needs a env shader or it will lead a crash
    #testFile=LIQUID_ROOT+"/test/maya_shader/ambientlight/ambientlight.ma"
    #MyTestOne.test_one_scene(testFile, "elvishray")
    
    testFile=LIQUID_ROOT+"/test/maya_shader/arealight/arealight.ma"
    MyTestOne.test_one_scene(testFile, "elvishray")
    testFile=LIQUID_ROOT+"/test/unittest/er/light/quardlight/quardlight.ma"
    MyTestOne.test_one_scene(testFile, "elvishray")
     
    testFile=LIQUID_ROOT+"/test/unittest/er/light/liquidlight/liquidlight.ma"
    MyTestOne.test_one_scene(testFile, "elvishray")
    testFile=LIQUID_ROOT+"/test/unittest/er/light/liquidshader_pointlight/liquidshader_pointlight.ma"
    MyTestOne.test_one_scene(testFile, "elvishray")

    testFile=LIQUID_ROOT+"/test/unittest/er/light/liquidshader_quadlight/liquidshader_quadlight.ma"
    MyTestOne.test_one_scene(testFile, "elvishray")

    testFile=LIQUID_ROOT+"/test/unittest/er/light/liquidshader_spotlight/liquidshader_spotlight.ma"
    MyTestOne.test_one_scene(testFile, "elvishray")

    testFile=LIQUID_ROOT+"/test/unittest/er/light/liquidshader_disclight/liquidshader_disclight.ma"
    MyTestOne.test_one_scene(testFile, "elvishray")

    testFile=LIQUID_ROOT+"/test/unittest/er/light/liquidshader_cylinderlight/liquidshader_cylinderlight.ma"
    MyTestOne.test_one_scene(testFile, "elvishray")

    testFile=LIQUID_ROOT+"/test/unittest/er/light/volumelight/simple/volumelight.ma"
    MyTestOne.test_one_scene(testFile, "elvishray")
    
    testFile=LIQUID_ROOT+"/test/unittest/er/light/volumelight/liquidshader_spherelight/liquidshader_spherelight.ma"
    MyTestOne.test_one_scene(testFile, "elvishray")

    testFile=LIQUID_ROOT+"/test/unittest/er/light/volumelight/liquidshader_cylinderlight/liquidshader_cylinderlight.ma"
    MyTestOne.test_one_scene(testFile, "elvishray")
    
    testFile=LIQUID_ROOT+"/test/unittest/er/light/liquidshader_liq_skylight/liquidshader_liq_skylight.ma"
    MyTestOne.test_one_scene(testFile, "elvishray")
    
    #crash, use liquidshader_liq_skylight.ma as a substitute
    #testFile=LIQUID_ROOT+"/test/unittest/er/light/liquidshader_skylight/liquidshader_skylight.ma"
    # MyTestOne.test_one_scene(testFile, "elvishray")
    
    testFile=LIQUID_ROOT+"/test/unittest/er/dof/dof.ma"
    MyTestOne.test_one_scene(testFile, "elvishray")

    testFile=LIQUID_ROOT+"/test/unittest/er/dof/dof_liquidShader/dof_liquidShader.ma"
    MyTestOne.test_one_scene(testFile, "elvishray")   
    
    testFile=LIQUID_ROOT+"/test/unittest/er/env/physicalsky/physicalsky.ma"
    MyTestOne.test_one_scene(testFile, "elvishray")
    testFile=LIQUID_ROOT+"/test/unittest/er/env/simple_env/simple_env.ma"
    MyTestOne.test_one_scene(testFile, "elvishray")
    
    testFile=LIQUID_ROOT+"/test/maya_shader/checker/checker.ma"
    MyTestOne.test_one_scene(testFile, "elvishray")
    
    testFile=LIQUID_ROOT+"/test/unittest/er/animation_checker/checker_anim.ma"
    MyTestOne.test_one_scene(testFile, "elvishray")
    
    testFile=LIQUID_ROOT+"/test/unittest/er/pfxhair/onehair.ma"
    MyTestOne.test_one_scene(testFile, "elvishray")
    
    testFile=LIQUID_ROOT+"/test/unittest/er/vertex_normal/simple/vertex_normal.ma"
    MyTestOne.test_one_scene(testFile, "elvishray")
    testFile=LIQUID_ROOT+"/test/unittest/er/volume/simple/volume.ma"
    MyTestOne.test_one_scene(testFile, "elvishray")
    testFile=LIQUID_ROOT+"/test/unittest/er/volume/volume_liquidShader/volume_liquidShader.ma"
    MyTestOne.test_one_scene(testFile, "elvishray")
    
    #testFile=LIQUID_ROOT+"/test/unittest/er/volume/animation/volume_liquidShader_anim.ma"
    #MyTestOne.test_one_scene(testFile, "elvishray")
    
    #it depends on mentralray, crash
    #testFile=LIQUID_ROOT+"/test/maya_shader/mib_amb_occlusion/mib_amb_occlusion.ma"
    #MyTestOne.test_one_scene(testFile, "elvishray")
    
    testFile=LIQUID_ROOT+"/test/unittest/er/object_hierarchy/object_hierarchy/object_hierarchy.ma"
    MyTestOne.test_one_scene(testFile, "elvishray")
    testFile=LIQUID_ROOT+"/test/unittest/er/object_hierarchy/object_hierarchy_mb/object_hierarchy_mb.ma"
    MyTestOne.test_one_scene(testFile, "elvishray")
    testFile=LIQUID_ROOT+"/test/unittest/er/object_hierarchy/object_hierarchy_simple/object_hierarchy_simple.ma"
    MyTestOne.test_one_scene(testFile, "elvishray")
    
    testFile=LIQUID_ROOT+"/test/unittest/er/lightlink/lightlink.ma"
    MyTestOne.test_one_scene(testFile, "elvishray")
    
    testFile=LIQUID_ROOT+"/test/animation_test/MoxMotionMeetsFBIK/MoxMotionMeetsFBIK/scenes/texture_anim2/texture_anim2.ma"
    MyTestOne.test_one_scene(testFile, "elvishray")
    
    # rendering hangs, refer to bug list on google code
    #testFile=LIQUID_ROOT+"/test/animation_test/MoxMotionMeetsFBIK/MoxMotionMeetsFBIK/scenes/texture_anim1/texture_anim1.ma"
    #MyTestOne.test_one_scene(testFile, "elvishray")
    
    #right side of the body is almost black
    testFile=LIQUID_ROOT+"/test/animation_test/HappyWalk/HappyWalk_liq_simple.ma"
    MyTestOne.test_one_scene(testFile, "elvishray")
    
    testFile=LIQUID_ROOT+"/test/unittest/er/aov/aov.ma"
    MyTestOne.test_one_scene(testFile, "elvishray")
    
    testFile=LIQUID_ROOT+"/test/unittest/er/procedural/simple_procedural/simple_procedural.ma"
    MyTestOne.test_one_scene(testFile, "elvishray")
    
    testFile=LIQUID_ROOT+"/test/unittest/er/surface_shader/architectural/architectural.ma"
    MyTestOne.test_one_scene(testFile, "elvishray")

    testFile=LIQUID_ROOT+"/test/unittest/common/nurbs_surface/nurbs_sphere/nurbs_sphere.ma"
    MyTestOne.test_one_scene(testFile, "elvishray")
    '''
    testFile=LIQUID_ROOT+"/test/unittest/common/nurbs_surface/nurbs_cone/nurbs_cone.ma"
    MyTestOne.test_one_scene(testFile, "elvishray")
    
    testFile=LIQUID_ROOT+"/test/unittest/common/nurbs_surface/nurbs_surface/nurbs_surface.ma"
    MyTestOne.test_one_scene(testFile, "elvishray")
    '''

    
    testFile=LIQUID_ROOT+"/test/maya_shader/bump2d/colorbump/bump2d_colorbump.ma"
    MyTestOne.test_one_scene(testFile, "elvishray")
    
    testFile=LIQUID_ROOT+"/test/unittest/common/subd_surface/subd_sphere/subd_sphere.ma"
    MyTestOne.test_one_scene(testFile, "elvishray")
    
    testFile=LIQUID_ROOT+"/test/unittest/er/motionblur/blur_camera/blur_camera.ma"
    MyTestOne.test_one_scene(testFile, "elvishray")
    '''

    '''
    ------------------------ appleseed ----------------------------------
    '''
    '''
    testFile=LIQUID_ROOT+"/test/unittest/as/motionblur/deform/deform.ma"
    MyTestOne.test_one_scene(testFile, "appleseed")
    testFile=LIQUID_ROOT+"/test/unittest/as/motionblur//transform/transform.ma"
    MyTestOne.test_one_scene(testFile, "appleseed")
    '''




    '''
    ------------------------ else ----------------------------------
    '''
    
    #testFile="G:/tmp/d_tmp/maya_test/tutorial/Transforming.Robot/8-Animation/scenes/transformingRobot_anim_liq.ma"
    #MyTestOne.test_one_scene(testFile, "elvishray")
    
    #testFile="E:/dev/Autodesk/maya/myplugin/project/liquid_/tomaya/mayabatch_crash/test.ma"
    #MyTestOne.test_one_scene(testFile, "mentalRay")

    '''



    #testFile=LIQUID_ROOT+"/2.3.3/test/test_shader_pl/test_shader_pl.ma"
    #MyTestOne.test_one_scene(testFile, "elvishray")


    testFile=LIQUID_ROOT+"/2.3.3/test/test_shader_pl/test_type_3delight.ma"
    MyTestOne.test_one_scene(testFile, "renderman")

    testFile=LIQUID_ROOT+"/2.3.3/test/test_shader_pl/test_type_array_3delight.ma"
    MyTestOne.test_one_scene(testFile, "renderman")

    er internal shaders

    #[Warning] test_displace_pl.ma lead mayabatch crash!
    #testFile=LIQUID_ROOT+"/2.3.3/test/test_shader_pl/test_displace_pl.ma"
    #MyTestOne.test_one_scene(testFile, "elvishray")









    #testFile=LIQUID_ROOT+"/2.3.3/test/hair/test_shaveHair(shave_v61)_er.ma"
    #MyTestOne.test_one_scene(testFile, "elvishray")
    #testFile=LIQUID_ROOT+"/2.3.3/test/hair/test_shaveHair(shave_v61)_rm.ma"
    #MyTestOne.test_one_scene(testFile, "renderman")








    testFile=LIQUID_ROOT+"/2.3.3/test/test_animation/er_anim.ma"
    MyTestOne.test_one_scene(testFile, "elvishray")

    #testFile=LIQUID_ROOT+"/2.3.3/test/test_golbalnode/test_simple.ma"
    #MyTestOne.test_one_scene(testFile, "elvishray")
    #MyTestOne.test_one_scene(testFile, "renderman")



    #MyTestOne.test_one_scene("D:/tmp/maya_test/Trex/scenes/TrexAnimation_er.ma", "elvishray")






 


    #testFile=LIQUID_ROOT+"/test/animation_test/Trex/Trex/scenes/TrexAnimation_liq_anim.ma"
    #MyTestOne.test_one_scene(testFile, "elvishray")
    #testFile=LIQUID_ROOT+"/test/animation_test/Trex/Trex/scenes/TrexAnimation_liq_f357_tri_lambert.ma"
    #MyTestOne.test_one_scene(testFile, "renderman")











    #-----------------------------------------
    testFile=LIQUID_ROOT+"/2.3.3/test/test_shader_graph/er_rm_shader_graph_phong_simple.ma"
    MyTestOne.test_one_scene(testFile, "renderman")




    testFile=LIQUID_ROOT+"/2.3.3/test/test_er_shader/phong_reflect.ma"
    MyTestOne.test_one_scene(testFile, "elvishray")



    testFile=LIQUID_ROOT+"/2.3.3/test/test_er_shader/phong.ma"
    MyTestOne.test_one_scene(testFile, "elvishray")

    testFile=LIQUID_ROOT+"/2.3.3/test/test_er_shader/er_displace.ma"
    MyTestOne.test_one_scene(testFile, "elvishray")

    #testFile=LIQUID_ROOT+"/2.3.3/test/test_er_light/er_liquidshader_skylight.ma"
    #MyTestOne.test_one_scene(testFile, "elvishray")

    testFile=LIQUID_ROOT+"/2.3.3/test/test_er_light/er_liquidshader_spherelight.ma"
    MyTestOne.test_one_scene(testFile, "elvishray")

    testFile=LIQUID_ROOT+"/2.3.3/test/test_er_light/er_liquidshader_quadlight.ma"
    MyTestOne.test_one_scene(testFile, "elvishray")

    testFile=LIQUID_ROOT+"/2.3.3/test/test_er_light/er_lightshader.ma"
    MyTestOne.test_one_scene(testFile, "elvishray")

    testFile=LIQUID_ROOT+"/2.3.3/test/test_rm_light/rm_liquidShader.ma"
    MyTestOne.test_one_scene(testFile, "renderman")

    testFile=LIQUID_ROOT+"/2.3.3/test/test_er_light/er_volumelight.ma"
    MyTestOne.test_one_scene(testFile, "elvishray")

    #testFile=LIQUID_ROOT+"/2.3.3/test/test_er_light/er_ambientlight.ma"
    #MyTestOne.test_one_scene(testFile, "elvishray")

    testFile=LIQUID_ROOT+"/2.3.3/test/test_er_light/er_quardlight.ma"
    MyTestOne.test_one_scene(testFile, "elvishray")
 
    testFile=LIQUID_ROOT+"/test/unittest/er/light/quardlight/er_quardlight.ma"
    MyTestOne.test_one_scene(testFile, "elvishray")

    - bump2d bump3d ,
    - (done)disclight, cylinderlight
    - (done)mySkyLight with string type evn_shader parameter
      (done)evn_shader param's UI has a problem
    - add log(warn/error) at the coshader actions
    - (done)rShader is replaced by rString
    - affect_diffuse, affect_specular, see architectural's light loop
    - stereo camera
    - blinn
    - (done)nurbs surface
    '''   





