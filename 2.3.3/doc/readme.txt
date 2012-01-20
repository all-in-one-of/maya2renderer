- Development environment
   - msvc 2008(sp1)
   - Boost(1.46.1 or later), set BOOST_ROOT in system environment variables, e.g. BOOST_ROOT=E:/dev/boost/1_46_1
   - Maya2009 x32, set MAYA_PATH2009 in system environment variables, e.g. MAYA_PATH2009=D:\Program Files\Autodesk\Maya2009
   - (For renderman renderer only) Renderman(http://code.google.com/p/hojocn/downloads/detail?name=pixar_win32.7z&can=2&q=#makechanges)

- Build
  - access https://github.com/yaoyansi/maya2renderer, check out the code to your local directory, say it $(LiquidRoot).
    The directory structure should be��
       $(LiquidRoot)\
                 +---2.3.3\
                 +---dependence\
	         ...
  - check out Elvishray http://elvishrender.googlecode.com/svn/trunk (r1014 for now) to $(LiquidRoot)/dependence/elvishray/trunk/
    The directory structure should be:
       $(LiquidRoot)\
                 +---2.3.3\
                 +---dependence/elvishray/trunk/
                                            +-- eiAPI/
                                            +-- eiCOMMON/
                                            +-- eiCORE/
                                            ...
  - open $(LiquidRoot)\2.3.3\src\MSVC2005\liquid.sln, choose ��Win32�� and ��Prman2009Debug�� in Configuration Manager��
    - (For renderman renderer only)
      - build displayDriverAqsis project, liqmaya.dll will be generated;
      - make sure liqmaya.dll is copied to "$(RMANTREE)\etc\d_liqmaya.dll"��
    - build liquid project��

- Install
  - backup your original (My_Documents_DIR)\maya\2009\Maya.env
  - copy $(LiquidRoot)\2.3.3\bin\Maya.env to (My_Documents_DIR)\maya\2009\Maya.env
  - open (My_Documents_DIR)\maya\2009\Maya.env, set LIQUID_ROOT to your liquid directory $(LiquidRoot),
     e.g.  LIQUID_ROOT = E:/dev/autodesk/maya/myplugin/project/liquid_
  - (For Elvishray renderer only)
    you must copy manager.ini of Elvishray to $(MAYA_PATH2009)/bin/
     - copy $(LiquidRoot)\dependence\elvishray\trunk\build\r1008\x86_Debug\manager.ini to $(MAYA_PATH2009)/bin/
     - open $(MAYA_PATH2009)/bin/manager.ini, set searchpath to the directory which contain eiIMG.dll and eiSHADER.dll
	e.g. if $(MAYA_PATH2009)/bin/manager.ini doesn't has searchpath, you can append the follow line to manager.ini
	searchpath E:/dev/Autodesk/maya/myplugin/project/liquid_/dependence/elvishray/trunk/build/r1008/x86_Debug

- Test 
  - open maya2009, load $(LiquidRoot)\2.3.3\test\er.ma,
  - in "Render Settings" panel
    - select Liquid renderer
    - in liquid tab��
       - set "Debug-->NewTranslator(refactoring)" checked.
       - set "Renderer-->Renderer" to elvishray
  - render.


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


- er batch render
  - test case: maya_render_er.bat, 2.3.3\test\test_er_shader\er_shader_lambert.ma







