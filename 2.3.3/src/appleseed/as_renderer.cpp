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
//#include "MayaConnection.h"
#include "log_helper.h"
#include "as_GlobalNodeHelper.h"

extern void build_project(asf::auto_release_ptr<asr::Project>&);

namespace appleseed
{
	liquid::LogMgr Renderer::m_log;

	Renderer::Renderer()
	{
		//m_groupMgr = new GroupMgr(this);

		m_gnode = new GlobalNodeHelper("liqGlobalsNodeRenderer_appleseed");
	}
	//
	Renderer::~Renderer()
	{
		delete m_gnode;
		m_gnode = 0;

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
	void Renderer::openLog()
	{
		CM_TRACE_FUNC("Renderer::openLog()(but do nothing now)");
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
			( !currentJob__.isShadow || currentJob__.deepShadows );

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

		unsigned int sample_first = 0;
		unsigned int sample_last = liqglo.liqglo_motionSamples -1;

		const bool bMotionBlur =
			ribNode__->motion.transformationBlur &&
			( ribNode__->object( 1 ) ) &&
			//( ribNode__->object(0)->type != MRT_Locator ) && // Why the fuck do we not allow motion blur for locators?
			( !currentJob__.isShadow || currentJob__.deepShadows );

		bool bGeometryMotion = 
			liqglo.liqglo_doDef 
			&& bMotionBlur
			&& ( ribNode__->object(0)->type != MRT_RibGen );

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
		{//material
			MStringArray shadingGroupNodes;
			{
				MString cmd;
				cmd = "listConnections -type \"shadingEngine\" -destination on (\""+MString(ribNode__->name.asChar())+"\" + \".instObjGroups\")";
				IfMErrorWarn(MGlobal::executeCommand( cmd, shadingGroupNodes));
			}
			MString materialName(shadingGroupNodes[0]);
			//todo
		}
		
		//element
		_s("//shape name="<<mesh->getFullPathName());
		_s("//shape full path name="<<mesh->getFullPathName());

		MMatrix matrix;
		matrix = ribNode__->object( sample_first )->matrix( ribNode__->path().instanceNumber() );
		RtMatrix m;		
		IfMErrorWarn(matrix.get(m));
		
		//transform motion
		const bool bMatrixMotionBlur = 
			liqglo.liqglo_doMotion 
			&& bMotionBlur;
 		if( bMatrixMotionBlur )
 		{
			matrix = ribNode__->object( sample_last )->matrix( ribNode__->path().instanceNumber() );
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
		m_log.open((currentJob.ribFileName+".as").asChar());

		//appleseed
		project = asr::ProjectFactory::create("test project");
		project->set_path((currentJob.ribFileName+".appleseed").asChar());
		build_project(project);
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


		if( isBatchMode() )
		{
			_s("// in batch render mode");
			//todo
		}else{
			// start render
			// Create the master renderer.
			asr::DefaultRendererController renderer_controller;
			asr::MasterRenderer renderer(
				project.ref(),
				project->configurations().get_by_name("final")->get_inherited_parameters(),
				&renderer_controller);

			// Render the frame.
			renderer.render();

			// Save the frame to disk.
			MString imageName(
				liqglo.m_pixDir + parseString( liqglo.m_displays[ 0 ].name, false )
				);
			project->get_frame()->write(imageName.asChar());

		}
		//////////////////////////////////////////////////////////////////////////
		// Save the project to disk.
		asr::ProjectFileWriter::write(project.ref());
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
		RtMatrix m;		
		IfMErrorWarn(m0.asMatrix().get(m));

		bool bDepthOfField;//enable DOF on this camera?
		liquidGetPlugValue(fnCamera,"depthOfField", bDepthOfField, status);
		_s("//Depth of Field on camera \""<<currentJob.camera[0].name.asChar()<<"\" is turned "<< (bDepthOfField?"on":"off")<<" in Maya");
		bDepthOfField = bDepthOfField && liqglo.doDof && !currentJob.isShadow;


		_s("\n//############################### camera #");
		std::string sCameraObjectName(std::string(currentJob.camera[0].name.asChar())+"_object");
		//todo
		_s("//----------------------------------camera instance: currentJob.camera[0].name");
		//todo		
		//m_groupMgr->addObjectInstance(currentJob.name.asChar(), currentJob.camera[0].name.asChar(), GIT_Camera);//_S( ei_init_instance( currentJob.camera[0].name.asChar() ) );

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
	void Renderer::cookInstanceGroup()
	{
		CM_TRACE_FUNC("Renderer::cookInstanceGroup()");
		//todo
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
					MObject shaderObj;
					getDependNodeByName( shaderObj,startingNode.asChar());
					liqShader &currentShader = liqShaderFactory::instance().getShader( shaderObj );
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
	//
	bool Renderer::canExport()
	{
		CM_TRACE_FUNC("Renderer::canExport()");
		//todo

		return true;
	}
}//namespace appleseed
