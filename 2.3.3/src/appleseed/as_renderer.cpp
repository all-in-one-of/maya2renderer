#include <liqConfig.h>
#ifdef _USE_APPLESEED_

#include "as_renderer.h"

#include "boost/format.hpp"

// Maya headers
#include "../common/prerequest_maya.h"
// Liquid headers
#include <liquid.h>
#include <liqRibHT.h>
#include <liqRenderer.h>
#include <liqRibLightData.h>
#include <liqGlobalHelpers.h>
#include <liqProcessLauncher.h>
#include <liqCustomNode.h>
#include <liqShaderFactory.h>
#include <liqExpression.h>
#include <liqRenderScript.h>
#include <liqGlobalVariable.h>
#include <liqJobScriptMgr.h>
#include <liqFrameScriptMgr.h>
#include <liqLightMgr.h>
#include <liqLocatorMgr.h>
#include <liqShadowRibWriterMgr.h>
#include <liqHeroRibWriterMgr.h>
#include <liqRibCamera.h>
#include <liqRibTranslator.h>

#include "../renderermgr.h"
#include "../common/mayacheck.h"
#include "as_connection.h"
#include "as_log_helper.h"
#include "as_GlobalNodeHelper.h"
#include "as_helper.h"

extern void build_project(
	asf::auto_release_ptr<asr::Project> &project,
	asf::auto_release_ptr<asr::Assembly> &assembly
);

namespace appleseed
{
	liquid::LogMgr Renderer::m_log;

	Renderer::Renderer()
	{
		//m_groupMgr = new GroupMgr(this);
		m_globalNodeName = "liqGlobalsNodeRenderer_appleseed";
		m_gnode = new GlobalNodeHelper(m_globalNodeName.asChar());
	}
	//
	Renderer::~Renderer()
	{
		delete m_gnode;
		m_gnode = 0;

		current_assembly.release();
		project.release();
		m_log_target.release();
		//delete m_groupMgr;
		//m_groupMgr = 0;
	}
	//
	void Renderer::test()
	{
		liquidMessage2(messageInfo, "this is %s.", "appleseed" );

	}
	//
	void Renderer::setRenderScriptFormatAndCompress(const bool bbinary, const bool bcompress)
	{
		CM_TRACE_FUNC("Renderer::setRenderScriptFormatAndCompress("<<bbinary<<","<<bcompress<<")");
		//todo...
	}
	//
	MStatus Renderer::worldPrologue(const structJob& currentJob)
	{
		CM_TRACE_FUNC("Renderer::worldPrologue("<<currentJob.name.asChar()<<")");

		_s("\n//worldPrologue");



		return MS::kSuccess;
	}
	//
	MStatus Renderer::worldEpilogue()
	{
		CM_TRACE_FUNC("Renderer::worldEpilogue()");
		_s("\n//Renderer::worldEpilogue()");

		return MS::kSuccess;
	}
	//
	//MStatus Renderer::exportLight(const liqRibNodePtr& ribNode, const structJob &currentJob)
	//{
	//	RtString RibNodeName = getLiquidRibName( ribNode->name.asChar() );
	//	_s("\n// Renderer::exportLight(\""+std::string(RibNodeName)+"\");");
	//	
	//	ribNode->object(0)->writeObject("", currentJob, false);//call liqRibLightData::_write(...)
	//	ribNode->object(0)->written = 1;

	//	return MS::kSuccess;
	//}
	void Renderer::openLog(const MString &imageFullPath)
	{
		CM_TRACE_FUNC("Renderer::openLog("<<imageFullPath.asChar()<<")(but do nothing now)");
	}
	void Renderer::closeLog()
	{
		CM_TRACE_FUNC("Renderer::closeLog()(but do nothing now)");
	}

	liqLightHandle Renderer::exportShadowPassLight(
		const std::string &shadertype, 
		const std::string &shaderinstance, 
		const liqString & shadowname,
		const liqMatrix &t)
	{
		CM_TRACE_FUNC("Renderer::exportShadowPassLight("<<shadertype<<","<<shaderinstance<<","<<shadowname<<",liqMatrix t)");
		_s("\n// Renderer::exportShadowPassLight()");

		std::string shaderinstanceFullPath( toFullDagPath(shaderinstance) );
		//todo

		return (liqLightHandle)(0);
	}

	void Renderer::transform_(const liqMatrix& transform)
	{
		CM_TRACE_FUNC("Renderer::transform_(liqMatrix transform)");
	}
	MStatus Renderer::liqRibLightData_write(const liqRibLightData *lightdata, const structJob &currentJob)
	{
		CM_TRACE_FUNC("Renderer::liqRibLightData_write("<<lightdata->getFullPathName()<<","<<currentJob.name.asChar()<<")");
		return MS::kSuccess;
	}

	//
	void Renderer::exportOneObject_data(
		const liqRibNodePtr &ribNode__,  
		const structJob &currentJob__
		)
	{
		CM_TRACE_FUNC("Renderer::exportOneObject_data("<<ribNode__->name.asChar()<<","<<currentJob__.name.asChar()<<")");

		unsigned int sample_first = 0;
		unsigned int sample_last = liqglo.liqglo_motionSamples -1;

		const bool bMotionBlur =
			ribNode__->motion.transformationBlur &&
			( ribNode__->object( 1 ) ) &&
			//( ribNode__->object(0)->type != MRT_Locator ) && // Why the fuck do we not allow motion blur for locators?
			( currentJob__.pass != rpShadowMap || currentJob__.shadowType == stDeep );

		bool bGeometryMotion = 
			liqglo.liqglo_doDef 
			&& bMotionBlur
			&& ( ribNode__->object(0)->type != MRT_RibGen );

		if( bGeometryMotion )
		{
			this->_writeObject(ribNode__, currentJob__, bGeometryMotion, 0/*unused*/, false);
			//exportOneGeometry_Mesh(ribNode__, currentJob__ , sample_first, sample_last);
		}else{
			this->_writeObject(ribNode__, currentJob__, bGeometryMotion, 0/*unused*/, false);
			//exportOneGeometry_Mesh(ribNode__, currentJob__ , sample_first, sample_first);
		}
	}
	void Renderer::exportOneObject_reference(
		const liqRibNodePtr &ribNode__,  
		const structJob &currentJob__
		)
	{
		CM_TRACE_FUNC("Renderer::exportOneObject_reference("<<ribNode__->name.asChar()<<","<<currentJob__.name.asChar()<<")");
		MStatus status;

		const bool bMotionBlur =
			ribNode__->motion.transformationBlur &&
			( ribNode__->object( 1 ) ) &&
			//( ribNode__->object(0)->type != MRT_Locator ) && // Why the fuck do we not allow motion blur for locators?
			( currentJob__.pass != rpShadowMap || currentJob__.shadowType == stDeep );

		bool bGeometryMotion = 
			liqglo.liqglo_doDef 
			&& bMotionBlur
			&& ( ribNode__->object(0)->type != MRT_RibGen );

		unsigned int sample_first = 0;
		unsigned int sample_last = bGeometryMotion? (liqglo.liqglo_motionSamples - 1):sample_first;

		//exportOneGeometry_Mesh(ribNode__, currentJob__ , sample_first, bGeometryMotion?sample_last:sample_first);

		_s("//--------------------------");
		{
			_s("//ribNode->name="<<ribNode__->name.asChar());
			_s("//ribNode's transform node="<<ribNode__->getTransformNodeFullPath().asChar());
			//print children
			MStringArray childrenMsg(ribNode__->getChildrenMsgOfTransformNode());
			unsigned int childCount = ribNode__->getChildrenCountOfTransformNode();
			if( childCount != 1 )
			{
				_s("//childCount="<<childCount);
				for(int i=0; i<childCount; ++i){
					_s( "//child("<<i<<"):"<<childrenMsg[i].asChar() );
				}
			}
			_s("//ribNode->object("<<sample_first<<")->getDataPtr()->getFullPathName()="<<ribNode__->object(sample_first)->getDataPtr()->getFullPathName());
		
		}

		const liqRibDataPtr mesh = ribNode__->object(sample_first)->getDataPtr();
#ifdef TRANSFORM_SHAPE_PAIR
		std::string instanceName(ribNode__->getTransformNodeFullPath().asChar());//transform
#else// SHAPE SHAPE_object PAIR
		std::string instanceName(ribNode__->name.asChar());//shape
#endif


#ifdef TRANSFORM_SHAPE_PAIR
		const std::string objectName(ribNode__->name.asChar());//shape
#else// SHAPE SHAPE_object PAIR
		const std::string objectName(getObjectName(ribNode__->name.asChar()));//shape+"_object"
#endif

		MString materialName;
		{//material
			MStringArray shadingGroupNodes;
			{
				MString cmd;
				cmd = "listConnections -type \"shadingEngine\" -destination on (\""+MString(ribNode__->name.asChar())+"\" + \".instObjGroups\")";
				IfMErrorWarn(MGlobal::executeCommand( cmd, shadingGroupNodes));
			}

			materialName = shadingGroupNodes[0];
		}

		//element
		_s("//shape name="<<mesh->getFullPathName());
		_s("//shape full path name="<<mesh->getFullPathName());

		MMatrix matrix;
		matrix = ribNode__->object( sample_first )->matrix( ribNode__->path().instanceNumber() );
		liqMatrix m;		
		IfMErrorWarn(matrix.get(m));
		
		//transform motion
		const bool bMatrixMotionBlur = 
			liqglo.liqglo_doMotion 
			&& bMotionBlur;
 		if( bMatrixMotionBlur )
 		{
			matrix = ribNode__->object( liqglo.liqglo_motionSamples -1/*sample_last*/ )->matrix( ribNode__->path().instanceNumber() );
			IfMErrorWarn(matrix.get(m));
 		}

		//toggle motion on this instance
		int bMotion = (ribNode__->doDef || ribNode__->doMotion);
		_s("//ribNode->doDef="<<ribNode__->doDef<<", ribNode->doMotion="<<ribNode__->doMotion);

		_s("{//light group(light-link group)");
		//const char* lg = getLightGroupName(instanceName.c_str()).asChar();

		_s("//");
		//
		//m_groupMgr->addObjectInstance( currentJob__.name.asChar(), instanceName, GIT_Geometry );//_S( ei_init_instance( currentJob.camera[0].name.asChar() ) );
	
		// Appleseed stuff
		{
			boost::filesystem::path objOSFullFileName(get_filesys_fullPathName(mesh->getFullPathName()));
			if( ! boost::filesystem::exists( objOSFullFileName ) )
			{
				liquidMessage2(messageError, "mesh file not exist: %s", objOSFullFileName.string().c_str() );
				return;
			}
			
			MString dirname;
			//IfMErrorWarn(MGlobal::executeCommand( "as_get_mesh_dirname(\""+MString(mesh->getFullPathName())+"\")", dirname));
			dirname = get_mesh_dirname(mesh->getFullPathName()).c_str();
			MString basename;
			//IfMErrorWarn(MGlobal::executeCommand( "as_get_mesh_basename(\""+MString(mesh->getFullPathName())+"\")", basename));
			basename = get_mesh_basename(mesh->getFullPathName()).c_str();

			asf::SearchPaths search_paths;
			search_paths.push_back( dirname.asChar() );
			
			asr::ParamArray objFileNameParam;
			if( sample_first == sample_last )
			{
				//no motion blur
				objFileNameParam.insert( "filename", basename.asChar() );
			}else{
				// for motion blur, there will be several obj files
				asr::ParamArray motionblurParam;
				motionblurParam.insert( "0", basename.asChar() );
				motionblurParam.insert( "1", (basename+"_mb.obj").asChar() );
				//motionblurParam.insert( "0", "square.0.obj" );
				//motionblurParam.insert( "1", "square.1.obj_mb.obj" );
				
				objFileNameParam.insert("filename", motionblurParam);
			}


			asr::MeshObjectArray objects;
			if( ! asr::MeshObjectReader::read( search_paths, 
				mesh->getFullPathName(), objFileNameParam, objects) )
			{
				liquidMessage2( messageError, "read mesh:%s",  );
				return;
			}
			
			//
			// Insert all the objects into the assembly.
			for (size_t i = 0; i < objects.size(); ++i)
			{
				// Insert this object into the scene.
				asr::MeshObject* object = objects[i];
				current_assembly->objects().insert(asf::auto_release_ptr<asr::Object>(object));

				// Create the array of material names.
				asf::StringDictionary material_names;
				material_names.insert("default", materialName.asChar() );
				
				// Create the array of backface material names.
				asf::StringDictionary backface_material_names;
				if( needToCreateBackfaceMaterial(materialName.asChar()) )
				{
					backface_material_names.insert("default_back", getBackfaceMaterial(materialName.asChar()).c_str() );
				}else{
					//backe-face material equals to front-face material
					backface_material_names.insert("default_back", materialName.asChar() );
				}
				
				// Create an instance of this object and insert it into the assembly.
				const std::string instance_name = std::string(object->get_name()) + "_inst";
				current_assembly->object_instances().insert(
						asr::ObjectInstanceFactory::create(
							instance_name.c_str(),
							asr::ParamArray(),
							object->get_name(),
							asf::Transformd(asf::Matrix4d::identity()),
							material_names,
							backface_material_names
						)
					);
			}//for
			
		}// Appleseed stuff

	}
	//
	void Renderer::ribPrologue_comment(const char* liqversion, 
		const char* scenename, const char* user, const time_t &now)
	{
		CM_TRACE_FUNC("Renderer::ribPrologue_comment("<<liqversion<<","<<scenename<<","<<user<<",time)");

		// general info for traceability
		//
		_s("//    Generated by Liquid v"<<liqversion );
		_s("//    Scene : "<< scenename );

		if( user )
			_s("//    User  : "<< user );

		_s("//    Time  : "<< ctime(&now) );

	}
	//
	MStatus Renderer::ribPrologue_begin(const structJob& currentJob)
	{
		CM_TRACE_FUNC("Renderer::ribPrologue_begin("<<currentJob.name.asChar()<<")");

		//////////////////////////////////////////////////////////////////////////
		//open script log file
		m_log.open((currentJob.ribFileName+".as").asChar(), std::ios_base::out);

		//appleseed
		m_log_target.reset(asf::create_file_log_target());
		m_log_target->open((currentJob.ribFileName+"_asglobal.as").asChar());
		asr::global_logger().add_target(m_log_target.get());

		project = asr::ProjectFactory::create("test project");
		// Create a scene.
		asf::auto_release_ptr<asr::Scene> _scene(asr::SceneFactory::create());
		// Bind the scene to the project.
		project->set_scene(_scene);
		// Create an assembly.
		current_assembly = asr::AssemblyFactory::create(
			"assembly",
			asr::ParamArray());

		createEnvironment();
		createOutput();
		createConfigurations();
		//////////////////////////////////////////////////////////////////////////

		_s("//### SCENE BEGIN ###");

		m_root_group = currentJob.name.asChar();
		//m_groupMgr->createGroup(m_root_group);//

		return MS::kSuccess;
	}
	MStatus Renderer::ribPrologue_end(const structJob& currentJob)
	{ 
		CM_TRACE_FUNC("Renderer::ribPrologue_end("<<currentJob.name.asChar()<<")");

		cookInstanceGroup();

		assert(!m_root_group.empty());
		assert(currentJob.camera[0].name.length());
		assert(!m_option.empty());


		//////////////////////////////////////////////////////////////////////////
		// Save the project to disk.
		asr::ProjectFileWriter::write(project.ref(), (currentJob.ribFileName+".appleseed").asChar());
		//////////////////////////////////////////////////////////////////////////

		if( isBatchMode() )
		{
			// render....
			Connection::getInstance()->render(
				project
				,project->configurations().get_by_name( m_gnode->getBool("useFinal")?"liqFinal":"liqInteractive")->get_inherited_parameters()
				,true);
			Connection::delInstance();
		}else{
			// start render
			if (Connection::getInstance()->startRender( currentJob.width, currentJob.height, false, false) != MS::kSuccess)
			{
				_s( "//MayaConnection: error occured in startRender." );
				Connection::delInstance();				
				return MS::kFailure;
			}
			// render...
			Connection::getInstance()->render(
				project
				,project->configurations().get_by_name( m_gnode->getBool("useFinal")?"liqFinal":"liqInteractive")->get_inherited_parameters()
				,false);

			// end render
			if (Connection::getInstance()->endRender() != MS::kSuccess)
			{
				_s( "//MayaConnection: error occured in endRender." );
				Connection::delInstance();
				return MS::kFailure;
			}
			Connection::delInstance();
		}

		// save the image.
		MString imageName(
			liqglo.m_pixDir + parseString( liqglo.m_displays[ 0 ].name, false )
			);
		int i = imageName.rindex('.');
		MString pngName(imageName.substring(0,i)+"png");
		project->get_frame()->write_main_image(pngName.asChar());

		//////////////////////////////////////////////////////////////////////////
		asr::global_logger().remove_target(m_log_target.get());

		m_log_target->close();
		//close script log file
		m_log.close();
		//////////////////////////////////////////////////////////////////////////
		return MS::kSuccess;
	}
	//
	MStatus Renderer::ribPrologue_options(const structJob& currentJob)
	{
		CM_TRACE_FUNC("Renderer::ribPrologue_options("<<currentJob.name.asChar()<<")");

		_s("\n//############################### option #");
		std::string tmp( currentJob.path.fullPathName().asChar() );//_s( (MString("// current job path = ")+).asChar() );

		m_option = std::string(currentJob.name.asChar()) + "_option";

		// Add default configurations to the project.
		project->add_default_configurations();

		// Set the number of samples. This is basically the quality parameter: the higher the number
		// of samples, the smoother the image but the longer the rendering time.
		project->configurations()
			.get_by_name("final")->get_parameters()
			.insert_path("generic_tile_renderer.min_samples", "25")
			.insert_path("generic_tile_renderer.max_samples", "25");

		return MStatus::kSuccess;
	}
	//
	MStatus Renderer::framePrologue(long lframe, const structJob &currentJob)
	{
		CM_TRACE_FUNC("Renderer::framePrologue("<<lframe<<","<<currentJob.name.asChar()<<")");

		framePrologue_camera(lframe, currentJob);

		return MStatus::kSuccess;
	}
	MStatus Renderer::framePrologue_camera(long lframe, const structJob &currentJob)
	{
		CM_TRACE_FUNC("Renderer::framePrologue_camera("<<lframe<<","<<currentJob.name.asChar()<<")");

		MStatus status;
		MFnCamera fnCamera(currentJob.path, &status);
		IfMErrorWarn(status);

		//
		float focal;
		float aperture;
		float aspect;
		float width=currentJob.width, height=currentJob.height;


		// use maya settings
		//NOTE: we will caculate these setting from structJob data.[yaoyansi]
		focal = fnCamera.focalLength(&status);
		IfMErrorWarn(status);
		double horizontalFOV,verticalFOV;
		fnCamera.getPortFieldOfView(width, height, horizontalFOV,verticalFOV);
		aperture = 2.0f * focal * tan(horizontalFOV /2.0f);
		aspect = aperture / (2.0f * focal * tan(verticalFOV / 2.0f));
		_s("// maya settings: focal="<<focal <<", aperture = "<<aperture<<", aspect="<<aspect );
		//e.g. focal=35, aperture = 36, aspect=1.33333.

		//get camera transform matrix
		//currentJob.camera[0].mat.get( m ) );
		MObject transformNode = currentJob.path.transform(&status );
		IfMErrorWarn(status);
		MFnDagNode transform (transformNode, &status);
		IfMErrorWarn(status);
		MTransformationMatrix   m0(transform.transformationMatrix());
		liqMatrix m;		
		IfMErrorWarn(m0.asMatrix().get(m));

		bool bDepthOfField;//enable DOF on this camera?
		liquidGetPlugValue(fnCamera,"depthOfField", bDepthOfField, status);
		_s("//Depth of Field on camera \""<<currentJob.camera[0].name.asChar()<<"\" is turned "<< (bDepthOfField?"on":"off")<<" in Maya");
		bDepthOfField = bDepthOfField && liqglo.doDof && currentJob.pass != rpShadowMap;


		_s("\n//############################### camera #");
		std::string sCameraObjectName(std::string(currentJob.camera[0].name.asChar())+"_object");
		//todo
		_s("//----------------------------------camera instance: currentJob.camera[0].name");
		//todo		
		//m_groupMgr->addObjectInstance(currentJob.name.asChar(), currentJob.camera[0].name.asChar(), GIT_Camera);//_S( ei_init_instance( currentJob.camera[0].name.asChar() ) );
		float film_width = 0.0f, film_height = 0.0f;
// 		if( currentJob.isShadow == false && liqglo.liqglo_rotateCamera == true ) {
// 			res0 = height; res1 = width;
// 		}else{ 
// 			res0 = width;  res1 = height;
// 		}
		float hFA = fnCamera.horizontalFilmAperture(&status);
		IfMErrorWarn(status);
		float vFA = fnCamera.verticalFilmAperture(&status);
		IfMErrorWarn(status);

		const float inch_to_meter = 0.02539999983236f;
		float maya_resolution_aspect = float(currentJob.width)/float(currentJob.height);
		float maya_film_aspect = hFA /vFA;

		MFnCamera::FilmFit filmFit = fnCamera.filmFit(&status);
		IfMErrorWarn(status);

		if ( MFnCamera::kHorizontalFilmFit == filmFit ){
			film_width  = hFA * inch_to_meter;
			film_height = film_width / maya_resolution_aspect; 
		}
		else if( MFnCamera::kVerticalFilmFit == filmFit ){
			film_height = vFA * inch_to_meter;
			film_width  = film_height * maya_resolution_aspect;
		}
		else if( MFnCamera::kFillFilmFit == filmFit ){//not correct
			film_width  = hFA * inch_to_meter;
			film_height = vFA * inch_to_meter;
		}
		else if( MFnCamera::kOverscanFilmFit == filmFit ){//not correct
// 			double hSize, vSize, hOffset, vOffset;
// 			IfMErrorWarn( fnCamera.getFilmFrustum( focal/1000.0f, hSize,vSize,hOffset,vOffset) );
// 			film_width  = hSize * inch_to_meter;
// 			film_height = vSize * inch_to_meter; 
			double left, right, bottom, top;
			IfMErrorWarn( fnCamera.getViewingFrustum( maya_resolution_aspect, left, right, bottom, top) );
			film_width  = (right - left) * inch_to_meter;
			film_height = (top - bottom) * inch_to_meter; 
		}
		else{
			liquidMessage2(messageError,"filmFit=%d is not implement for AS.", filmFit );
			//dummy film_width/film_height
			film_width  = hFA * inch_to_meter;
			film_height = film_width / maya_resolution_aspect; 
		}

		// Create a camera with film
		asf::auto_release_ptr<asr::Camera> _camera(
			asr::PinholeCameraFactory().create(
			currentJob.camera[0].name.asChar(),//camera instance name
			asr::ParamArray()
			.insert("film_dimensions", boost::format("%f %f") %(film_width) %(film_height))//""
			.insert("focal_length", boost::format("%f") %(focal/1000.0f))));
		// Bind the camera to the scene.
		project->get_scene()->set_camera(_camera);

		// Place and orient the camera. By default cameras are located in (0.0, 0.0, 0.0)
		// and are looking toward Z- (0.0, 0.0, -1.0).
		asf::Matrix4d a;
		convertMatrix<double>(a, m);
		project->get_scene()->get_camera()->transform_sequence().clear();
		project->get_scene()->get_camera()->transform_sequence().set_transform( 0/*lframe*/, asf::Transformd( a ) );

// 		project->get_scene()->get_camera()->transform_sequence().set_transform(
// 			0.0,
// 			asf::Transformd(
// 			asf::Matrix4d::rotation(asf::Vector3d(1.0, 0.0, 0.0), asf::deg_to_rad(-20.0)) *
// 			asf::Matrix4d::translation(asf::Vector3d(0.0, 0.8, 11.0))));

		// Create a frame and bind it to the project.
		project->set_frame(
			asr::FrameFactory::create(
			"beauty",
			asr::ParamArray()
			.insert("camera", project->get_scene()->get_camera()->get_name())
			.insert("resolution", boost::format("%f %f") %currentJob.width %currentJob.height)
			.insert("color_space", "srgb")));

		return MStatus::kSuccess;
	}
	//
	MStatus Renderer::frameEpilogue(const long scanTime)
	{
		CM_TRACE_FUNC("Renderer::frameEpilogue("<<scanTime<<")");

		return MStatus::kSuccess;
	}
	//
	MStatus Renderer::doTextures(const std::vector<structJob> &txtList_)
	{
		CM_TRACE_FUNC("Renderer::frameEpilogue(txtList_.size="<<txtList_.size()<<")");
		return MStatus::kSuccess;
	}
	MStatus Renderer::doShadows(const std::vector<structJob> &shadowList_)
	{
		CM_TRACE_FUNC("Renderer::doShadows(shadowList_.size="<<shadowList_.size()<<")");
		return MStatus::kSuccess;
	}
	MStatus Renderer::doRenderView()
	{
		CM_TRACE_FUNC("Renderer::doRenderView()");
		return MStatus::kSuccess;
	}
	MStatus Renderer::renderAll_local(const MString& ribFileName)
	{
		CM_TRACE_FUNC("Renderer::renderAll_local("<<ribFileName.asChar()<<")");
		return MStatus::kSuccess;
	}
	MStatus Renderer::renderAll_remote(const MString& ribFileName)
	{
		CM_TRACE_FUNC("Renderer::renderAll_remote("<<ribFileName.asChar()<<")");
		return MStatus::kSuccess;
	}
	MStatus Renderer::render(const structJob& currentJob)
	{
		CM_TRACE_FUNC("Renderer::render("<<currentJob.name.asChar()<<")");

		return MStatus::kSuccess;
	}
	void Renderer::cookInstanceGroup()
	{
		CM_TRACE_FUNC("Renderer::cookInstanceGroup()");

		// Create an instance of the assembly and insert it into the scene.
		asf::auto_release_ptr<asr::AssemblyInstance> current_assembly_instance(
			asr::AssemblyInstanceFactory::create(
				(std::string(current_assembly->get_name())+"_inst").c_str(),
				asr::ParamArray(),
				current_assembly->get_name()
			)
		);
		//assembly instance sets transform
		current_assembly_instance->transform_sequence()
			.set_transform(
				0.0,
				asf::Transformd(asf::Matrix4d::identity())
			);
		//add assembly instance
		project->get_scene()->assembly_instances()
			.insert(current_assembly_instance);
		// Insert the assembly into the scene.
		project->get_scene()->assemblies().insert(current_assembly);
	}

	void Renderer::exportLightLinks(
		const structJob &currentJob__,
		const liqRibNodePtr ribNode__, 
		const MStringArray& lightedByWhichLightShapes)
	{
		CM_TRACE_FUNC("Renderer::exportLightLinks("<<currentJob__.name.asChar()<<","<<ribNode__->name.asChar()<<",lightedByWhichLightShapes.size="<<lightedByWhichLightShapes.length()<<")");

		if(lightedByWhichLightShapes.length() == 0){
			_s("//"<< ribNode__->name.asChar() << " is not lighted." );
			return;
		}

		const liqRibDataPtr mesh = ribNode__->object(0)->getDataPtr();
#ifdef TRANSFORM_SHAPE_PAIR
		std::string instanceName(ribNode__->getTransformNodeFullPath().asChar());//transform
#else// SHAPE SHAPE_object PAIR
		std::string instanceName(ribNode__->name.asChar());//shape
#endif

		for(size_t i=0; i<lightedByWhichLightShapes.length(); ++i)
		{
			//m_groupMgr->addLightLink(currentJob__.name.asChar(), 
			//	instanceName,
			//	lightedByWhichLightShapes[i].asChar()
			//	);
		}

	}
	//
	void Renderer::gatherCameraShaders(
		MStringArray& cameraShaders,
		const MString& node,
		const std::string& plug_
		)
	{
		CM_TRACE_FUNC("Renderer::gatherCameraShaders(cameraShaders.size="<<cameraShaders.length()<<","<<node.asChar()<<","<<plug_<<")");

		const MString plug(plug_.c_str());
		MString cmd;

		int isShaderPlugExist;
		cmd = "attributeQuery -node \""+node+"\" -ex \""+plug+"\"";
		IfMErrorMsgWarn(MGlobal::executeCommand( cmd, isShaderPlugExist), cmd);
		if(isShaderPlugExist)
		{
			MStringArray connectedShaders;
			cmd = "listConnections (\""+node+"\" + \"."+plug+"\")";
			IfMErrorMsgWarn(MGlobal::executeCommand( cmd, connectedShaders), cmd);

			if( connectedShaders.length() != 0 )
			{
				const MString startingNode(connectedShaders[0]);

				MString nodetype;
				cmd = "nodeType \""+startingNode+"\"";
				IfMErrorWarn(MGlobal::executeCommand( cmd, nodetype));

				if(nodetype=="liquidSurface"){
					//liquidMessage2(messageInfo, (startingNode+"'s type is "+nodetype+", no need to convert").asChar());
					liqShader &currentShader = liqShaderFactory::instance().getShader( startingNode.asChar() );
					currentShader.write();

					IfMErrorWarn( cameraShaders.append(startingNode) );
				}else{
					//convertShadingNetworkToRSL(startingNode, node);
					liquidMessage2(messageError,"%s.%s is not liquidSurface.",node.asChar(), plug_.c_str());
				}
			}
		}
	}
	void Renderer::_writeObject(
		const liqRibNodePtr& ribNode, 
		const structJob &currentJob,
		const bool bGeometryMotion,
		const unsigned int msampleOn,
		const bool bReference
		)
	{
		CM_TRACE_FUNC("Renderer::_writeObject("<<ribNode->name.asChar()<<","<<currentJob.name.asChar()<<","<<bGeometryMotion<<","<<msampleOn<<","<<bReference<<")");

		MString MotionPostfix;
		unsigned int sample;
		if( bGeometryMotion ){
			MString MSampleOn;
			MSampleOn.set((int)msampleOn);
			MotionPostfix = ".m"+MSampleOn;
			sample        = msampleOn;
		}else{
			MotionPostfix = "";
			sample        = 0;
		}

		MString geometryRibFile( 
			liquidGetRelativePath( 
			false, 
			getLiquidRibName( ribNode->name.asChar() ), 
			liqglo.liqglo_ribDir 
			) +"."+(int)liqglo.liqglo_lframe+MotionPostfix+".rib" 
			);



		//=====================================================
		// Export rib data
		//=====================================================
		ribNode->object( sample )->writeObject(geometryRibFile, currentJob, bReference);

	}
	//
	bool Renderer::isHeroPassReady(const structJob &currentJob)
	{
		CM_TRACE_FUNC("Renderer::isHeroPassReady("<<currentJob.name.asChar()<<")");

		return true;
	}
	void Renderer::HeroPassBegin(const structJob &currentJob___)
	{
		CM_TRACE_FUNC("Renderer::HeroPassBegin("<<currentJob___.name.asChar()<<")");

		//m_log.open() is moved to ribPrologue_begin()
	}
	void Renderer::HeroPassEnd(const structJob &currentJob)
	{
		CM_TRACE_FUNC("Renderer::HeroPassEnd("<<currentJob.name.asChar()<<")");

		//m_log.close() is moved to ribPrologue_end()
	}
	//
	bool Renderer::isBaseShadowReady(const structJob &currentJob___)
	{
		CM_TRACE_FUNC("Renderer::isBaseShadowReady("<<currentJob___.name.asChar()<<")");

		return !currentJob___.shadowArchiveRibDone;
	}
	void Renderer::BaseShadowBegin(const structJob &currentJob___)
	{
		CM_TRACE_FUNC("Renderer::BaseShadowBegin("<<currentJob___.name.asChar()<<")");

		MString     baseShadowName__(getBaseShadowName(currentJob___));
		liquidMessage2(messageInfo, "open base shadow file %s\n", baseShadowName__.asChar());
	}
	void Renderer::BaseShadowEnd(const structJob &currentJob___)
	{
		CM_TRACE_FUNC("Renderer::BaseShadowEnd("<<currentJob___.name.asChar()<<")");
	
		MString     baseShadowName__(getBaseShadowName(currentJob___));
		liquidMessage2(messageInfo, "close base shadow file %s\n", baseShadowName__.asChar());

	}

	bool Renderer::isShadowPassReady(const structJob &currentJob)
	{
		CM_TRACE_FUNC("Renderer::isShadowPassReady("<<currentJob.name.asChar()<<")");
		return true;
	}
	void Renderer::ShadowPassBegin(const structJob &currentJob___)
	{
		CM_TRACE_FUNC("Renderer::ShadowPassBegin("<<currentJob___.name.asChar()<<")");
	
		liquidMessage2(messageInfo, "open shadow pass file %s\n", currentJob___.ribFileName.asChar() );
	}
	void Renderer::ShadowPassEnd(const structJob &currentJob___)
	{
		CM_TRACE_FUNC("Renderer::ShadowPassEnd("<<currentJob___.name.asChar()<<")");

		liquidMessage2(messageInfo, "close shadow pass file %s\n", currentJob___.ribFileName.asChar() );
	}
	void Renderer::readBaseShadow(const structJob &currentJob___)
	{
		CM_TRACE_FUNC("Renderer::readBaseShadow("<<currentJob___.name.asChar()<<")");

	}
	//
	void Renderer::oneObjectBlock_reference_attribute_begin(
		const liqRibNodePtr &ribNode,
		const structJob &currentJob )
	{
		CM_TRACE_FUNC("Renderer::oneObjectBlock_reference_attribute_begin("<<ribNode->name.asChar()<<","<<currentJob.name.asChar()<<")");
	}
	void Renderer::oneObjectBlock_reference_attribute_end(
		const liqRibNodePtr &ribNode,
		const structJob &currentJob )
	{
		CM_TRACE_FUNC("Renderer::oneObjectBlock_reference_attribute_end("<<ribNode->name.asChar()<<","<<currentJob.name.asChar()<<")");
	}
	void Renderer::oneObjectBlock_reference_attribute_block0(
		const liqRibNodePtr &ribNode,
		const structJob &currentJob )
	{
		CM_TRACE_FUNC("Renderer::oneObjectBlock_reference_attribute_block0("<<ribNode->name.asChar()<<","<<currentJob.name.asChar()<<")");
	}
	void Renderer::oneObjectBlock_reference_attribute_block1(
		const liqRibNodePtr &ribNode,
		const structJob &currentJob )
	{
		CM_TRACE_FUNC("Renderer::oneObjectBlock_reference_attribute_block1("<<ribNode->name.asChar()<<","<<currentJob.name.asChar()<<")");
	}
	void Renderer::logFrame(const char* msg)
	{
		CM_TRACE_FUNC("Renderer::logFrame("<<msg<<")");

		assert( m_log.get().is_open() );
		_s("//"<<msg);
	}
	void Renderer::oneObjectBlock_reference_attribute_block2_writeShader_RibBox(const char* msg)
	{
		CM_TRACE_FUNC("Renderer::oneObjectBlock_reference_attribute_block2_writeShader_RibBox("<<msg<<")");

		assert( m_log.get().is_open() );
		_s("//writeShaderRibBox()");
		_s(msg);
	}
	void Renderer::oneObjectBlock_reference_attribute_block2_writeShader_RegularShader(
		const liqRibNodePtr &ribNode__,
		const structJob &currentJob )
	{	
		CM_TRACE_FUNC("Renderer::oneObjectBlock_reference_attribute_block2_writeShader_RegularShader("<<ribNode__->name.asChar()<<","<<currentJob.name.asChar()<<")");

		assert( m_log.get().is_open() );

	}
	void Renderer::oneObjectBlock_reference_attribute_block2_writeShader_HasNoSurfaceShaderORIngoreSurface(
		const liqRibNodePtr &ribNode__, const MDagPath &path__, const bool m_shaderDebug
		)
	{
		CM_TRACE_FUNC("Renderer::oneObjectBlock_reference_attribute_block2_writeShader_HasNoSurfaceShaderORIngoreSurface("
			<<ribNode__->name.asChar()<<","<<path__.fullPathName().asChar()<<","<<m_shaderDebug<<")");

		assert( m_log.get().is_open() );

		//_s("//I bet it will never goes here.Renderer::writeShader_HasNoSurfaceShaderORIngoreSurface(ribNode="<<ribNode__->name <<",)" );
		//assert(0 && "I bet it will never goes here.rm::Renderer::writeShader_HasNoSurfaceShaderORIngoreSurface()" );

	}
	void Renderer::oneObjectBlock_reference_attribute_block3_ShadingGroup(
		const MString& meshname
		)
	{
		CM_TRACE_FUNC("Renderer::oneObjectBlock_reference_attribute_block3_ShadingGroup("<<meshname.asChar()<<")");
	}
	void Renderer::oneObjectBlock_reference_attribute_block4_strategy(
		const liqRibNodePtr &ribNode)
	{
		CM_TRACE_FUNC("Renderer::oneObjectBlock_reference_attribute_block4_strategy("<<ribNode->name.asChar()<<")");
	}
	//
	bool Renderer::canExport()
	{
		CM_TRACE_FUNC("Renderer::canExport()");
		
		//load objexport plugin
		MString MAYA_PATH;
		IfMErrorWarn(MGlobal::executeCommand( "getenv(\"MAYA_LOCATION\")", MAYA_PATH));
		MString objExport(MAYA_PATH+"/bin/plug-ins/objExport.so");

		int loadObjExportSucess;
		IfMErrorWarn(MGlobal::executeCommand( "catch(`loadPlugin \""+objExport+"\"`)", loadObjExportSucess));
		if( loadObjExportSucess != 0 ){
			liquidMessage2(messageError,"load plugin fail: %s ",objExport.asChar());
			return false;
		}
		// .export_mesh_to
		MString export_mesh_to(m_gnode->getString("export_mesh_to"));
		if( export_mesh_to.length() == 0 ){
			liquidMessage2(messageError,".export_mesh_to is not set. See Appleseed Global Values --> others --> export_mesh_to.");
			return false;
		}
		int bExist;
		IfMErrorWarn(MGlobal::executeCommand( "filetest -d \""+export_mesh_to+"\"", bExist));
		if(!bExist){
			liquidMessage2(messageError,".export_mesh_to=[%s] does not exists.", export_mesh_to.asChar());
			return false;
		}
		//todo

		return true;
	}
	
	void Renderer::createOutput()
	{
		CM_TRACE_FUNC("Renderer::createOutput()");

	}
	void Renderer::createConfigurations()
	{
		CM_TRACE_FUNC("Renderer::createConfigurations()");

		{
			asf::auto_release_ptr<asr::Configuration> configuration;
			
			configuration = asr::ConfigurationFactory::create("liqFinal");

			asr::ParamArray& parameters = configuration->get_parameters();

			const std::string category("final");
			parameters.insert("frame_renderer", getConfiguration(m_gnode,category,"frame_renderer"));//generic,progressive,
			parameters.insert("tile_renderer", getConfiguration(m_gnode, category,"tile_renderer"));//generic,blank,debug,ewatest
			parameters.insert("sample_renderer", getConfiguration(m_gnode, category,"sample_renderer"));//generic
			parameters.insert("sample_generator", getConfiguration(m_gnode, category,"sample_generator"));//generic,lighttracing
			parameters.insert("lighting_engine", getConfiguration(m_gnode, category,"lighting_engine"));//dr,pt
			parameters.insert("aaa", "111");

			asr::ParamArray generic_tile_renderer_params;
			generic_tile_renderer_params.insert("min_samples", getConfiguration(m_gnode, category,"min_samples"));
			generic_tile_renderer_params.insert("max_samples", getConfiguration(m_gnode, category,"max_samples"));
			generic_tile_renderer_params.insert("sample_filter_size", getConfiguration(m_gnode, category,"sample_filter_size"));
			generic_tile_renderer_params.insert("sample_filter_type", getConfiguration(m_gnode, category,"sample_filter_type"));
			parameters.dictionaries().insert("generic_tile_renderer", generic_tile_renderer_params);
		
			project->configurations().insert( configuration );
		}
		{
			asf::auto_release_ptr<asr::Configuration> configuration;
			
			configuration = asr::ConfigurationFactory::create("liqInteractive");

			asr::ParamArray& parameters = configuration->get_parameters();

			const std::string category("interactive");
			parameters.insert("frame_renderer", getConfiguration(m_gnode,category,"frame_renderer"));//generic,progressive,
			parameters.insert("tile_renderer", getConfiguration(m_gnode, category,"tile_renderer"));//generic,blank,debug,ewatest
			parameters.insert("sample_renderer", getConfiguration(m_gnode, category,"sample_renderer"));//generic
			parameters.insert("sample_generator", getConfiguration(m_gnode, category,"sample_generator"));//generic,lighttracing
			parameters.insert("lighting_engine", getConfiguration(m_gnode, category,"lighting_engine"));//dr,pt
			parameters.insert("bbb", "222");

			asr::ParamArray generic_tile_renderer_params;
			generic_tile_renderer_params.insert("min_samples", getConfiguration(m_gnode, category,"min_samples"));
			generic_tile_renderer_params.insert("max_samples", getConfiguration(m_gnode, category,"max_samples"));
			generic_tile_renderer_params.insert("sample_filter_size", getConfiguration(m_gnode, category,"sample_filter_size"));
			generic_tile_renderer_params.insert("sample_filter_type", getConfiguration(m_gnode, category,"sample_filter_type"));
			parameters.dictionaries().insert("generic_tile_renderer", generic_tile_renderer_params);
		
			project->configurations().insert( configuration );		
		}

	}
	MStatus Renderer::coordSysBlock__(const structJob &currentJob, boost::shared_ptr< liqRibHT > &htable)
	{
		CM_TRACE_FUNC("Renderer::coordSysBlock__("<<currentJob.name.asChar()<<")");
		return MS::kSuccess;
	}
	MStatus Renderer::preGeometryMel(const MString &m_preGeomRIB)
	{
		CM_TRACE_FUNC("Renderer::preGeometryMel()");
		return MS::kSuccess;
	}
	void Renderer::F1(const liqRibNodePtr &ribNode__,  liqShader &currentShader)
	{
		CM_TRACE_FUNC("Renderer::F1("<<ribNode__->name.asChar()<<","<<currentShader.getName()<<")");
	}
	void Renderer::F2(const bool m_shaderDebug, const liqRibNodePtr &ribNode__)
	{
		CM_TRACE_FUNC("Renderer::F2("<<m_shaderDebug<<","<<ribNode__->name.asChar()<<")");
	}
	void Renderer::writeUserAttributes(const liqRibNode *ribNode__)
	{
		CM_TRACE_FUNC("Renderer::writeUserAttributes()");
	}
	void Renderer::objectBlock_reference_begin()
	{
		CM_TRACE_FUNC("Renderer::objectBlock_reference_begin()");
	}
	void Renderer::objectBlock_reference_end()
	{
		CM_TRACE_FUNC("Renderer::objectBlock_reference_end()");
	}
	void Renderer::writeShader_forShadow_ribbox(const MString & text)
	{
		CM_TRACE_FUNC("Renderer::writeShader_forShadow_ribbox(text)");

	}
	void Renderer::writeShader_forShadow_forSpecialTypes(const liqRibNodePtr &ribNode__, bool m_shaderDebug, const MDagPath & path__)
	{
		CM_TRACE_FUNC("Renderer::writeShader_forShadow_forSpecialTypes("<<ribNode__->name.asChar()<<","<<m_shaderDebug <<",path__)");
	}
	void Renderer::writeShader_forDeepShadow_forSpecialTypes(const liqRibNodePtr &ribNode__, const MDagPath & path__)
	{
		CM_TRACE_FUNC("Renderer::writeShader_forDeepShadow_forSpecialTypes("<<ribNode__->name.asChar() <<",path__)");
	}
	void Renderer::writeShader_forShadow_NullShader(const liqRibNodePtr &ribNode__, const MDagPath & path__)
	{
		CM_TRACE_FUNC("Renderer::writeShader_forShadow_NullShader("<<ribNode__->name.asChar() <<",path__)");
	}
	MString Renderer::getTextureExt()const
	{
		CM_TRACE_FUNC("Renderer::getTextureExt()");

		return "anyone";
	}
	bool Renderer::isTextureTypeSupported(const MString &textureType)const
	{
		CM_TRACE_FUNC("Renderer::getTextureExt("<<textureType.asChar() <<")");

		return false;
	}
	//
	MStatus Renderer::iprBegin()
	{
		CM_TRACE_FUNC("Renderer::iprBegin()");

		return MS::kSuccess;
	}

	MStatus Renderer::iprEnd()
	{
		CM_TRACE_FUNC("Renderer::iprEnd()");

		return MS::kSuccess;
	}
	//IPR callback functions
	MStatus Renderer::IPR_AttributeChangedCallback( MNodeMessage::AttributeMessage msg, 
		MPlug & plug, MPlug & otherPlug, void* userData)
	{
		CM_TRACE_FUNC("Renderer::IPR_AttributeChangedCallback("<<msg<<","<<plug.name().asChar()<<","<<otherPlug.name().asChar()<<",userData)");
		liquidMessage2(messageInfo, "Renderer::IPR_AttributeChangedCallback()");

		return MS::kSuccess;
	}
	MStatus Renderer::IPR_NodeDirtyCallback( MObject& node,void *userData )
	{
		CM_TRACE_FUNC("Renderer::IPR_NodeDirtyCallback("<<MFnDependencyNode(node).name().asChar()<<",userData)");
		liquidMessage2(messageInfo, "Renderer::IPR_NodeDirtyCallback()");

		return MS::kSuccess;
	}
	MStatus Renderer::IPR_NodeDirtyPlugCallback( MObject& node,MPlug& plug,void* userData )
	{
		CM_TRACE_FUNC("Renderer::IPR_NodeDirtyPlugCallback("<<MFnDependencyNode(node).name().asChar()<<",userData)");
		liquidMessage2(messageInfo, ("Renderer::IPR_NodeDirtyPlugCallback("+MFnDependencyNode(node).name()+","+plug.name()+", userData)").asChar());

		return MS::kSuccess;
	}

}//namespace appleseed

#endif//_USE_APPLESEED_