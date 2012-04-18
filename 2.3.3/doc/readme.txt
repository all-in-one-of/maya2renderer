 ---------------------------------------------------------------------
                          CONTENTS
 ---------------------------------------------------------------------
- License
- Build/Install/Test
- Others
- NOTES
- F.A.Q.


 ---------------------------------------------------------------------
                          License
 ---------------------------------------------------------------------
The contents of this file are subject to the Mozilla Public License Version 1.1
(the "License"); you may not use this file except in compliance with the
License. You may obtain a copy of the License at http://www.mozilla.org/MPL/

Software distributed under the License is distributed on an "AS IS" basis,
WITHOUT WARRANTY OF ANY KIND, either express or implied. See the License for
the specific language governing rights and limitations under the License.

The Original Code is the Liquid Rendering Toolkit.
The Initial Developer of the Original Code is Colin Doncaster. Portions created
by Colin Doncaster are Copyright (C) 2002. All Rights Reserved.

Contributor(s): Dan Bethell, Berj Bannayan, Andrew Chapman, Colin Doncaster,
St�phane Deverly, George Harker, Jamie McCarter, Ken McGaugh, Moritz Moeller,
Michael Duffy, Richard Pickler, Rob Pieke, Paul Scruby, Yuri Meshalkin,
Alf Kraus, Leon Tony Atkinson.

The RenderMan (R) Interface Procedures and Protocol are:
Copyright 1988, 1989, Pixar
All Rights Reserved

RenderMan (R) is a registered trademark of Pixar


 ---------------------------------------------------------------------
                          Build/Install/Test
 ---------------------------------------------------------------------
- Development environment
   - msvc 2008(sp1)
   - Boost(1.46.1 or later), set BOOST_ROOT in system environment variables, e.g. BOOST_ROOT=E:/dev/boost/1_46_1
   - Maya2009 x32, set MAYA_PATH2009 in system environment variables, e.g. MAYA_PATH2009=D:\Program Files\Autodesk\Maya2009
   - (For renderman renderer only) Renderman Pro Server.

- Build
  - access https://github.com/yaoyansi/maya2renderer, check out the code to your local directory, say it $(LiquidRoot).
    The directory structure should be��
       $(LiquidRoot)\
                 +---2.3.3\
                 +---dependence\
	         ...
  - check out Elvishray http://elvishrender.googlecode.com/svn/trunk (r1117 for now) to $(LiquidRoot)/dependence/elvishray/trunk/
    The directory structure should be:
       $(LiquidRoot)\
                 +---2.3.3\
                 +---dependence/elvishray/trunk/
                                            +-- eiAPI/
                                            +-- eiCOMMON/
                                            +-- eiCORE/
                                            ...
  - build ER. 
    - When you configure cmake for ER, set CMAKE_INSTALL_PREFIX to $(LiquidRoot)\dependence\elvishray, so ER will copy *.dll to $(LiquidRoot)\dependence\elvishray\bin\$(ConfigurationName)
    - select a ConfigurationName, e.g. Release. 
    - build ER, and copy generated *.lib to $(LiquidRoot)\dependence\elvishray\bin\$(ConfigurationName)
  - build $(LiquidRoot)\2.3.3\src\elvishray\eiSHADER_maya\build\eiSHADER_maya.sln. eiSHADER_maya.dll will be generated at $(LiquidRoot)\dependence\elvishray\bin\$(ConfigurationName)
    NOTE: you'd better set the ConfigurationName same with ER project. e.g. Release.
  - copy $(LiquidRoot)\dependence\elvishray\bin\$(ConfigurationName)\*.* to $(LiquidRoot)\dependence\elvishray\bin\$(ConfigurationName)\*.*
    e.g. you can choose Release as the ConfigurationName.
  - open $(LiquidRoot)\2.3.3\include\liqConfig.h, set _USE_SHAVE_ to 1 if shave(v61 or later) is installed, or set _USE_SHAVE_ to 0 if shave isn't installed.
  - open $(LiquidRoot)\2.3.3\src\MSVC2005\liquid.sln, choose ��Win32�� and ��Prman2009Debug�� in Configuration Manager��
    - (For renderman renderer only)
      - make sure Prman is installed, and $(RMANTREE) is set(e.g. RMANTREE=D:\Program Files\Pixar\RenderManProServer-13.5.2),
        $(RMANTREE) contains 3 sub-directories, bin\, etc\ and lib\.
      - build displayDriverAqsis project, liqmaya.dll will be generated;
      - make sure liqmaya.dll is copied to "$(RMANTREE)\etc\d_liqmaya.dll"��
    - build liquid project��

- Install
  - backup your original (My_Documents_DIR)\maya\2009\Maya.env
  - copy $(LiquidRoot)\2.3.3\bin\Maya.env to (My_Documents_DIR)\maya\2009\Maya.env
    make sure ELVISHRAY_BIN point to the ER's DLL directory which you are using.
  - open (My_Documents_DIR)\maya\2009\Maya.env, set LIQUID_ROOT to your liquid directory $(LiquidRoot),
     e.g.  LIQUID_ROOT = E:/dev/autodesk/maya/myplugin/project/liquid_
  - (For Elvishray renderer only)
    you must copy manager.ini of Elvishray to $(MAYA_PATH2009)/bin/
     - copy $(LiquidRoot)\dependence\elvishray\bin\manager.ini to $(MAYA_PATH2009)/bin/
     - open $(MAYA_PATH2009)/bin/manager.ini, set searchpath to the directory which contain eiIMG.dll and eiSHADER.dll
	e.g. if $(MAYA_PATH2009)/bin/manager.ini doesn't has searchpath, you can append the follow line to manager.ini
	searchpath E:/dev/Autodesk/maya/myplugin/project/liquid_/dependence/elvishray/bin

- Test 
  - open maya2009, load $(LiquidRoot)\2.3.3\test\er.ma,
  - load liquid plugin:
    Menu "Window" --> "Settings/Preferences" --> "Plug-in Manager"
    Liquid plugin would be listed in "Plug-in Manager" window if you use $(LiquidRoot)\2.3.3\bin\Maya.env,
    If liquid plugin is not listed, load $(LiquidRoot)\2.3.3\bin\Prman\win32\liquid_2009x32d.mll manually please.
  - in "Render Settings" panel
    - select Liquid renderer
    - in liquid tab��
       - set "Debug-->NewTranslator(refactoring)" checked.
       - set "Renderer-->Renderer" to elvishray
  - render.



 ---------------------------------------------------------------------
                            Others
 ---------------------------------------------------------------------
- link error boost::system::get_system_category() �Ҳ���
  �����������liquid��Ŀ���\boost\1_39\libs\system\src\error_code.cpp������ȥ


- liquid���ɵ�rib�ļ�����renderman studio it.exe ����
  ����liquid���ɵ�rib�ű�,��Display���"liqmaya"�ĳ�"it"
  ����$(LiquidRoot)\2.3.3\test\render_with_rms_it\_perspShape.0010.rib��ġ�liqmaya���Ѿ���Ϊ"it"��
  - ����liquid���ɵ�ribʹ�þ���·�������Խ�$(LiquidRoot)\2.3.3\test\render_with_rms_it\*.*������E:\MyDocuments\maya\projects\default\rib��
  - �����ȴ�it����D:\Program Files\Pixar\RenderMan-Studio-1.0.1-Maya2008\bin\it.exe��
  - ����test.bat


-deepShadow����
 - ��$(LiquidRoot)\2.3.3\test\test_deepshadow\test_deepshadow.ma
 - �˵�Liquid-��Helpers-��Convert Shading Network To RSL
 - RenderSettings���
   - ѡ��Liquid��Ⱦ��
   - liquid������ѡNewTranslator(refactoring)��Renderer��ѡ��renderman��
 - ��Ⱦ


- compile $(LiquidRoot)\2.3.3\lib\shaders\*.sl
  - You must set RMS_ROOT to you RendermanStudio install directory(e.g.D:\Program Files\Pixar\RenderMan-Studio-1.0.1-Maya2008) in system enviroment variables.
  - run $(LiquidRoot)\2.3.3\lib\shaders\compile.cmd


- MakeTextute ����
  - ��$(LiquidRoot)\2.3.3\test\test_rm_texture\textures\*.* ������E:\MyDocuments\maya\projects\default\rmantex�£�������test case
  - ���е�texture·����ʽ
      E:/MyDocuments/maya/projects/default/rmantex/check10.tex
      E:/MyDocuments/maya/projects/default/rmantex/check11.bmp (Note: have to set "Map&Texture" to "rmantex")
      E:/MyDocuments/maya/projects/default/rmantex/a.b/check.14.bmp (Note: have to set "Map&Texture" to "rmantex/a.b")
  - �����е�texture·����ʽ
      E:\MyDocuments\maya\projects\default\rmantex/check10.tex(��б�ܡ�\��)
  - �������� $(LiquidRoot)\2.3.3\test\test_rm_texture\*.*


- rm ao���ԣ�test/test_ao2/occlude2.ma����ע���RenderSettings->Liquid->Rays->RayTracing->Enable


- er��Ⱦ����mesh���ű�����mesh���ݣ���mesh��material����Ϊ�գ����磺
    ei_instance("pCubeShape3");
       ei_add_material("");
       ei_element("pCubeShape3_object");
       ei_motion(1);
    ei_end_instance();
  ���������ѡ���mesh������˵�Liquid-��Helpers-��Convert Shading Network To RSL��


- export shader graph with new code(C++)
  - select "Render Settings-->Liquid-->Frame-->Debug-->OutputShaderGraph(ER)"
    Note: This option will output the shader graph but not render the scene.
    So, if you render with renderman, it will complain:
        "Error: liquidNewShaderNode : shader is not readable >
         E:/MyDocuments/maya/projects/default/generatedShader/<YourShaderName>.slo //"
    Just ingore this error, it will disappear after you uncheck the "OutputShaderGraph(ER)" option.
  - set "Render Settings-->Liquid-->File-->Directories-->GeneratedShaders" to "generatedShader"
  - edit the shader or shader graph
  - assign the shader to a mesh
  - select the mesh
  - render. The shader will be generated at ,e.g. E:/MyDocuments/maya/projects/default/generatedShader/<YourShaderName>.er
  - test case: $(LiquidRoot)\2.3.3\test\test_shader_graph\er_rm_shader_graph.ma

- liquidDisplacement����
  - test case: 2.3.3\test\test_rm_displacement\test_displacement.ma
  - ��Ⱦʱ�����liquidDisplacement1��Ч����������
    ��Ҫ��liquidDisplacement1�ı�ǩҳ������Reload����ť��
    ��ʱ��liquidDisplacement1-->RIB����ǩҳ�Ĳ����Ϳ�����ʾ������
    �ٵ����Ⱦ���Ϳ����ˡ�
  - δ��ѡ�е�mesh��*.erapi,*.rmsg,*.sl_my���޷������

- ei_material(), shading group����
  - test case:2.3.3\test\test_shadinggroup\er_rm_shadinggroup.ma
  - ei_material�൱��maya���shading group��
    surface shader��volume shader��displament shader��ʹ��maya������ݣ�
    liqShadowShader��liqEnvironmentShader��liqPhotonShader��erʹ�ã�
    ��mesh shape��Ӧ��shadinggroup��ǩ�ġ�ExtraAttributes������liqShadowShader��liqEnvironmentShader��liqPhotonShader


- batch render
  - er  test case: maya_render_er.bat, 2.3.3\test\test_er_shader\er_shader_lambert.ma
  - cmd batch render script: $(LiquidRoot)\2.3.3\test\mayabatch_template.bat


- liquidSurfaceShader parameters list
  - ��HowToUseLiquid.pdf��liquidSurfaceShader�½�


- MotionBlur
  - ��motion_blur.pdf


- automation test
  - first, render you scene(test_motionblur\transform\deform.ma) manully, 
    and save the image as stander image to the sub-directory(output_img_std/) of your scene file, e.g. test_motionblur\transform\output_img_std/.
  - add your test scene in testall.py.test_all_scene()
        LIQUID_ROOT="E:/dev/Autodesk/maya/myplugin/project/liquid_"
        testFile=LIQUID_ROOT+"/2.3.3/test/test_motionblur/transform/deform.ma"
        liqRenderer=""
        MyTestOne.test_one_scene(testFile, liqRenderer)
    - if you set liqRenderer="elvishray", the test file will rendered with elvishray only,
      if you set liqRenderer="renderman", the test file will rendered with prman only,
      if you set liqRenderer="", the test file will rendered with both elvishray and prman.

  - run $(LiquidRoot)\test\image_diff\test.py, it will run the test and generate the log file _liqTestLog.htm



- press ESC to interrupt the rendering.


- *.pl �ļ���shaderType����ʹ��shadow
  - use liqShader.shader_type_ex instead of liqShader.shader_type.
  - shader_type_ex can be set by shaderType-section in *.pl
  - shader_type_ex is set by liqGetSloInfo::shaderTypeEx, which comes from "$node.rmanShaderType"
  - add ".rmanShaderType" to liqSurfaceNode/liqDisplacementNode/liqCoShaderNode/liqLightNode/liqVolumeNode
  - set ".rmanShaderType" in rmanParams_create()
  - use ".rmanShaderType" in liqGetSloInfo::setShaderNode()-->shaderNode.findPlug("rmanShaderType",...);


- Depth of Field
  - RM ��DOF ֻ����RenderSettings-->liquid-->Frame-->MotionBlur&DepthofField�ﹴѡ��Depth of Field��
  - ER ��DOF������Ҫ��ѡRenderSettings-->liquid-->Frame-->MotionBlur&DepthofField�ﹴѡ��Depth of Field����
    ����Ҫ����test_er_dof\er_dof2.maΪ������
    - ѡ��cameraShape1�ڵ㣬��Depth of Field�ﹴѡ��Depth of Field��
    - Menu -->Liquid-->ObjectAttributes-->Create CustomShaderForERCamera,��������cameraShape1�ڵ��Ͻ�����liqLensShader��liqEnvironmentShader����
    - ����DOF shader��
       - �½�liquidSurface2����cameraShape1.liqLensShader���ӵ�liquidSurface2�ϣ�
       - ����liquidSurface2�Ĳ���
          - ����shader=simple_dof.dll
	  - ����cameraShape1.nearClipPlane��liquidSurface2.fplane
	  - ����cameraShape1.fStop��liquidSurface2.fstop

  ---------------------------------------------------------------------
                              NOTES
  ---------------------------------------------------------------------
- liquidSurfaceShader ��Щ����û�������rib�ļ���
  ע�⣺���������Ĭ��ֵ���Ͳ��������rib�ļ��
  ����Ը��Ĳ�����ֵ�������Ƿ���Ȼû�������rib�ļ���


- �Զ���shader��string���Ͳ������������ơ�
  �����Զ���shader������liquidSurfaceShader�������б����string���Ͳ���(string a, string b)��
   - ����ò����ƶ�������(string a="/texturepath/a.tex"),
     ����Ҫ������������ǰ׺��texname����string texnamea="/texturepath/a.tex"��
   - ����ò�����ָ������(string b="teststr"),
     ��ò�����һ�������С�texname��ǰ׺��
     ����С�texname��ǰ׺�����綨����(string texnameb="teststr")��ͬʱtexnameb����������������·����
     ��ôliquid�Ὣstring texnameb="teststr.tex"�����rib�ļ����ɴ���
    see liqShader.cpp line306 for more details.
    // [2/1/2012 yaoyansi]
    // This is a restriction in liquid(maya2renderer).
    // - If the attribute name starts with 'texname',(e.g. texname, texname0, texname_0, and etc.)
    //   it is a texture name or texture full path, 
    //   so we MUST append '.tex' to the plug value.
    // - If the plug is not a texture name or texture full path,
    //   DO NOT let the attribute name starts with 'texname'.
   - �ַ����ﲻ�ܰ���'|'(��$gSeperator)�ַ�


- *.pl �ļ���matrix arrayû��ʵ��


- ÿ�θ���Maya.env����Ҫ��Ӧ����liquid_\test\image_diff\testall.py��configMayaEnv()�Ļ�������ELVISHRAY_BIN��ֵ
- ÿ�θ�ϸER��
  - ER binary files(*.lib/*.dll/*.exe)���ڵ�·���Ƿ�ı��ˣ�����ı��ˣ�����Ҫ���ģ�
    - eiSHADER_maya.sln��link lib��·���� post build event���·��
    - liquid.sln��link��·��
    - liquid_\2.3.3\bin\Maya.env��ELVISHRAY_BIN��·��������Maya.env����E:\MyDocuments\maya\2009\Maya.env
      ͬʱ����liquid_\test\image_diff\testall.py��configMayaEnv()�Ļ�������ELVISHRAY_BIN��ֵ
    - Maya2009\bin\manager.ini��searchpath��·��
  - ����er������eiSHADER_maya.sln������liquid.sln


- ����֡����ʱ��һ��Ҫ�ǵð�Animationѡ��ϣ�����ֻ�����ǰ֡��


- maya_phong ���ж�transparency�Ƿ�Ϊ0��if ( almost_zerov( &transparency(), 0.001f ) )


- may file node
  - num_channels�ݲ����ǣ��Ժ��ж�texture file�Ƿ��ǵ�ͨ������󣬲���3delight��ʵ�����޸�
  - �����������û���õ����Ժ����3delight��ʵ�����޸�
  - color_texture/scalar_texture�Ĳ���dsdx, dsdy, dtdx, dtdy��֪���������
  - liquid��  ei_make_texture(..., ..., eiInt swrap, eiInt twrap,
	eiInt filter, eiScalar swidth, eiScalar twidth);ʱ�⼸�������������

- Bump channel in Phong shader(for er)
  I make a hack to achieve the bump effect.The following steps will deliver (bump)file.outColor to phong.normalCamera
  - create bump2d.bumpNormal
  - file.outColor --> bump2d.bumpNormal
  - set bump2d.bumpInterp=2
  - in maya_bump2d: if(bumpInterp==2) outNormal()=bumpNormal();
  todo: 
  - delete bump2d.bumpNormal
  - recover bump2d.bumpInterp


- vertex normal 
  - smooth normal, like a sphere: fnMesh.getVertexNormal(vertex i, false, normal, MSpace::kObject)
  - sharp edge, like a cube
    in this case, like a cube, a vertex has a specified normal corresponding to each adjacent polygon.
    but elvishray's only allow a vertex to be assigned only one normal.
    so I can't generate the normal list for this case.


- place2dTexture
  - repeatUV is an output plug, so it is not set in OutputHelper::addRSLVariable()
    but repeatUV is set in maya directly. So repeateUV must be set manully. see RSL::Visitor::visitPlace2dTexture()

  ---------------------------------------------------------------------
                              F.A.Q.
  ---------------------------------------------------------------------
- Q: $(LiquidRoot)\2.3.3\renderers\*.lg �ļ���ʲô?
  A:renderman��һ����׼��֧�������׼����Ⱦ����prman��aqsis��3dlight�ȣ�
    lg�ļ�������������prman��aqsis��3dlight���ԵĲ�����
  
  Q:Aqsis��prman��������Ŀ�����������ʲô
  A:liquid���õ�aqsis��riboutģ�飬�����rib�ļ�
