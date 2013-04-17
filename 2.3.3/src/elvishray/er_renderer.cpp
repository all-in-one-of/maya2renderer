#include <liqConfig.h>
#ifdef _USE_ELVISHRAY_

#include "er_renderer.h"

#include "boost/format.hpp"

// Maya headers
#include "../common/prerequest_maya.h"
// Liquid headers
#include <liquid.h>
#include <liqRibHT.h>
#include <liqRenderer.h>
#include <liqRibLightData.h>
#include <liqGlobalHelpers.h>
#include <liqGlobalHelpers_refactor.h>
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
#include "ercall.h"
#include "MayaConnection.h"
#include "er_groupmgr.h"

#include "er_GlobalNodeHelper.h"
#include "er_iprMgr.h"
#include "er_helper.h"

namespace elvishray
{
	OutputMgr Renderer::o;

	Renderer::Renderer()
	{
		CM_TRACE_FUNC("Renderer::Renderer()");
		m_groupMgr = new GroupMgr(this);

// 		liquid::RendererMgr::getInstancePtr()->registerRenderer(
// 			"elvishray", this
// 			);
		m_gnode = new GlobalNodeHelper("liqGlobalsNodeRenderer_elvishray");
		m_iprMgr = NULL;

		//ei_context();
	}
	//
	Renderer::~Renderer()
	{
		CM_TRACE_FUNC("Renderer::~Renderer()");

		//ei_end_context();

		delete m_gnode;
		m_gnode = 0;

		delete m_groupMgr; 
		m_groupMgr = 0;
	}
	//
	void Renderer::test()
	{
		liquidMessage2(messageInfo, "this is %s.", "elvishray" );

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

		o.ln();
		o.a("worldPrologue");



		return MS::kSuccess;
	}
	//
	MStatus Renderer::worldEpilogue()
	{
		CM_TRACE_FUNC("Renderer::worldEpilogue()");
		o.a("Renderer::worldEpilogue()");

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
		CM_TRACE_FUNC("Renderer::openLog("<<imageFullPath.asChar()<<")");

		//////////////////////////////////////////////////////////////////////////
		//open script log file
		//m_log.open((currentJob.ribFileName+".er").asChar());
		MString tmp;
		
		MString tmp_path( 
			liquidSanitizePath( 
				liquidGetRelativePath( liqglo.liqglo_relativeFileNames,
				"rib", liqglo.liqglo_projectDir )
			) 
		);

		IfMErrorWarn(MGlobal::executeCommand("getAttr \"liquidGlobals.ddImageName[0]\";", tmp));		
		MString displayName0(parseString_refactor(tmp));

		int frame = MAnimControl::currentTime().as( MTime::uiUnit() );



		MString projdir = liqglo.liqglo_projectDir;
		MString filepath;
		filepath += liqglo.liqglo_ribDir;
		filepath += "/"+liquidTransGetSceneName();
		MString filename;
		filename = liquidSanitizePath( filepath );
		filename = getFullPathFromRelative ( filename );


		//construct output mgr
		o.setOutputImagePath(imageFullPath.asChar());
		o.add(OutputMgr::OT_CALL);
		o.add(OutputMgr::OT_ESS);
		o.add(OutputMgr::OT_ESA);
		o.init();

		//////////////////////////////////////////////////////////////////////////
		//_s("//### SCENE BEGIN ###");
		//_S( ei_context() );
		
	}
	void Renderer::closeLog()
	{
		CM_TRACE_FUNC("Renderer::closeLog()(but do nothing now)");

		//_S( ei_end_context() );

		//////////////////////////////////////////////////////////////////////////
		//destruct output mgr
		o.uninit();
		//////////////////////////////////////////////////////////////////////////
	}

	liqLightHandle Renderer::exportShadowPassLight(
		const std::string &shadertype, 
		const std::string &shaderinstance, 
		const liqString & shadowname,
		const liqMatrix &t)
	{
		CM_TRACE_FUNC("Renderer::exportShadowPassLight("<<shadertype<<","<<shaderinstance<<","<<shadowname<<",liqMatrix t)");
		o.a("Renderer::exportShadowPassLight()");

		std::string shaderinstanceFullPath( toFullDagPath(shaderinstance) );

		std::string sShaderInstanceName(shaderinstanceFullPath+"_shader");
		o.ei_shader( "shadowlight", sShaderInstanceName.c_str() );//shader 
		o.ei_shader_param_scalar("intensity", 3.0 );
		o.ei_end_shader();

		std::string sLightObjectName(shaderinstanceFullPath+"_object");
		o.ei_light( sLightObjectName.c_str() );//object
		o.ei_light_shader(	sShaderInstanceName.c_str() );//shader
		o.ei_origin( t[3][0],t[3][1],t[3][2] );
		o.ei_end_light();

		o.ei_instance(  shaderinstanceFullPath.c_str() );
		o.ei_element(	 sLightObjectName.c_str() );//object
		o.ei_end_instance();

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

		//exportOneGeometry_Mesh(ribNode__, currentJob__ , sample_first, bGeometryMotion?sample_last:sample_first);
		
		if( ribNode__->liqGeoShaderNodeName.length()>0 )//has procedural object
		{
			exportOneObject_procedural(ribNode__, currentJob__);
		}


		o.a("--------------------------");
		{
			o.a("ribNode->name="+std::string(ribNode__->name.asChar()));
			o.a("ribNode's transform node="+std::string(ribNode__->getTransformNodeFullPath().asChar()));
			//print children
			MStringArray childrenMsg(ribNode__->getChildrenMsgOfTransformNode());
			unsigned int childCount = ribNode__->getChildrenCountOfTransformNode();
			if( childCount != 1 )
			{
				o.a(boost::str(boost::format("childCount=%d")%childCount));
				for(int i=0; i<childCount; ++i){
					o.a(boost::str(boost::format("child(%d):%s")%i %childrenMsg[i].asChar()) );
				}
			}
			o.a(boost::str(boost::format("ribNode->object(%d)->getDataPtr()->getFullPathName()=%s")%sample_first %ribNode__->object(sample_first)->getDataPtr()->getFullPathName()));
		
		}

		const liqRibDataPtr mesh = ribNode__->object(sample_first)->getDataPtr();
#ifdef TRANSFORM_SHAPE_PAIR
		std::string instanceName(ribNode__->getTransformNodeFullPath().asChar());//transform
#else// SHAPE SHAPE_object PAIR
		std::string instanceName(ribNode__->name.asChar());//shape
#endif
		o.ei_instance( instanceName.c_str() );
		//_S( ei_visible( on ) );
		// 		ei_shadow( on );
		// 		ei_trace( on );
#ifdef TRANSFORM_SHAPE_PAIR
		std::string objectName(ribNode__->name.asChar());//shape
#else// SHAPE SHAPE_object PAIR
		std::string objectName(getObjectName(ribNode__->name.asChar()));
#endif	
		if( ribNode__->liqGeoShaderNodeName.length()>0 )//has procedural object
		{
			objectName = getProceduralObjectName(objectName.c_str());//shape+"_object_procedural"
		}

		{//material
			MStringArray shadingGroupNodes;
			{
				MString cmd;
				cmd = "listConnections -type \"shadingEngine\" -destination on (\""+MString(ribNode__->name.asChar())+"\" + \".instObjGroups\")";
				IfMErrorWarn(MGlobal::executeCommand( cmd, shadingGroupNodes));
			}
			if( shadingGroupNodes.length()==0 )
			{
				liquidMessage2(messageError, "%s's shading group is empty, so I use \"%s\" to avoid crash", ribNode__->name.asChar(), getTestMaterialName().asChar());
				o.a("shadingGroupNodes is empty, use \""+std::string(getTestMaterialName().asChar())+"\" to avoid crash");
				o.ei_mtl(getTestMaterialName().asChar());
			} else if( shadingGroupNodes[0].length()==0 ){
				liquidMessage2(messageError, "%s's shadingGroupNode[0] is empty, so I use \"%s\" to avoid crash", ribNode__->name.asChar(), getTestMaterialName().asChar());
				o.a("shadingGroupNodes[0] is empty, use \""+std::string(getTestMaterialName().asChar())+"\" to avoid crash" );
				o.ei_mtl(getTestMaterialName().asChar());
			} else {
				o.ei_mtl( shadingGroupNodes[0].asChar() );
			}
		}
		
		//element
		o.a("shape name="+std::string(mesh->getFullPathName()));
		o.a("shape full path name="+std::string(mesh->getFullPathName()));
		o.ei_element( objectName.c_str() );

		MMatrix matrix;
		matrix = ribNode__->object( sample_first )->matrix( ribNode__->path().instanceNumber() );
		liqMatrix m;		
		IfMErrorWarn(matrix.get(m));
		o.ei_transform( m[0][0], m[0][1], m[0][2], m[0][3],   m[1][0], m[1][1], m[1][2], m[1][3],   m[2][0], m[2][1], m[2][2], m[2][3],   m[3][0], m[3][1], m[3][2], m[3][3] );
		
		//transform motion
		const bool bMatrixMotionBlur = 
			liqglo.liqglo_doMotion 
			&& bMotionBlur;
 		if( bMatrixMotionBlur )
 		{
			matrix = ribNode__->object( sample_last )->matrix( ribNode__->path().instanceNumber() );
			IfMErrorWarn(matrix.get(m));
			o.ei_motion_transform( m[0][0], m[0][1], m[0][2], m[0][3],   m[1][0], m[1][1], m[1][2], m[1][3],   m[2][0], m[2][1], m[2][2], m[2][3],   m[3][0], m[3][1], m[3][2], m[3][3] );
 		}

		//toggle motion on this instance
		int bMotion = (ribNode__->doDef || ribNode__->doMotion);
		o.a(boost::str(boost::format("ribNode->doDef=%f, ribNode->doMotion=%f")%ribNode__->doDef %ribNode__->doMotion));
		o.ei_motion( bMotion );

		//light links
		o.liq_lightgroup_in_object_instance(getLightGroupName(instanceName.c_str()).asChar());

		o.ei_end_instance();
		o.ln();
		//
		m_groupMgr->addObjectInstance( currentJob__.name.asChar(), instanceName, GIT_Geometry );//_S( ei_init_instance( currentJob.camera[0].name.asChar() ) );
	}
	//
	void Renderer::exportOneObject_procedural(
			const liqRibNodePtr &ribNode__,  
			const structJob &currentJob__)
	{
		CM_TRACE_FUNC("Renderer::exportOneObject_procedural("<<ribNode__->name.asChar()<<","<<currentJob__.name.asChar()<<")");

		//phong1
		liqShader &geometryShader = liqShaderFactory::instance().getShader( ribNode__->liqGeoShaderNodeName.asChar() );
		geometryShader.write();

#ifdef TRANSFORM_SHAPE_PAIR
		std::string objectName(ribNode__->name.asChar());//shape
#else// SHAPE SHAPE_object PAIR
		std::string objectName(getObjectName(ribNode__->name.asChar()));//shape+"_object"
#endif
		objectName = getProceduralObjectName(objectName.c_str());

		
		MVector BBoxMin, BBoxMax;
		{
			MDoubleArray tmp;

			MString getBBoxMin("getAttr "+ribNode__->getTransformNodeFullPath()+".boundingBoxMin");
			IfMErrorWarn( MGlobal::executeCommand(getBBoxMin, tmp, true, true));
			BBoxMin.x = tmp[0]; BBoxMin.y = tmp[1]; BBoxMin.z = tmp[2];

			MString getBBoxMax("getAttr "+ribNode__->getTransformNodeFullPath()+".boundingBoxMax");
			IfMErrorWarn( MGlobal::executeCommand(getBBoxMax, tmp, true, true));
			BBoxMax.x = tmp[0]; BBoxMax.y = tmp[1]; BBoxMax.z = tmp[2];
		}

		eiBound bbox;
		bbox.xmin = BBoxMin.x;
		bbox.ymin = BBoxMin.y;
		bbox.zmin = BBoxMin.z;
		bbox.xmax = BBoxMax.x;
		bbox.ymax = BBoxMax.y;
		bbox.zmax = BBoxMax.z;
		// add the bound for max displacement
		addbf(&bbox, 1.0f);

		o.ei_object("proc", objectName.c_str());
			// define the bounding box of the procedural object
			o.ei_box(bbox.xmin, bbox.ymin, bbox.zmin, bbox.xmax, bbox.ymax, bbox.zmax);
			// assign a geometry shader to create procedural geometry on-demand
			o.ei_geometry_shader( geometryShader.getName().c_str() );
		o.ei_end_object();
	}
	//
	void Renderer::ribPrologue_comment(const char* liqversion, 
		const char* scenename, const char* user, const time_t &now)
	{
		CM_TRACE_FUNC("Renderer::ribPrologue_comment("<<liqversion<<","<<scenename<<","<<user<<",time)");

		// general info for traceability
		//
		o.a("    Generated by Liquid v"+std::string(liqversion) );
		o.a("    Scene : "+std::string(scenename) );

		if( user )
			o.a("    User  : "+std::string(user) );

		//o.a("    Time  : "<< ctime(&now) );

	}
	//
	MStatus Renderer::ribPrologue_begin(const structJob& currentJob)
	{
		CM_TRACE_FUNC("Renderer::ribPrologue_begin("<<currentJob.name.asChar()<<")");

		currentJob.ribFileName;

		o.ei_context();

		//verbose
		o.ei_verbose(	m_gnode->getInt("verbose") );

		//link
		MStringArray link(m_gnode->getStringArray("link"));
		for(std::size_t i=0; i< link.length(); ++i)
		{
			o.ei_link( link[i].asChar() );
		}

		m_root_group = currentJob.name.asChar();
		m_groupMgr->createGroup(m_root_group);//

		dummyPhongShader();//for test only, must be removed when the shader export is done.

		return MS::kSuccess;
	}
	MStatus Renderer::ribPrologue_end(const structJob& currentJob)
	{ 
		CM_TRACE_FUNC("Renderer::ribPrologue_end("<<currentJob.name.asChar()<<")");

		cookInstanceGroup();

		assert(!m_root_group.empty());
		assert(currentJob.camera[0].name.length());
		assert(!m_option.empty());

		render( currentJob );

		o.ei_end_context();
		o.a("------------------------------------------------");
		o.ln();

		return MS::kSuccess;
	}
	//
	MStatus Renderer::ribPrologue_options(const structJob& currentJob)
	{
		CM_TRACE_FUNC("Renderer::ribPrologue_options("<<currentJob.name.asChar()<<")");

		o.ln();
		o.a("############################### option #");
		std::string tmp( currentJob.path.fullPathName().asChar() );//_s( (MString("// current job path = ")+).asChar() );

		m_option = std::string(currentJob.name.asChar()) + "_option";
		o.ei_options( m_option.c_str() );
		//ei_incremental_options( const char *name );

		//	Sampling Quality:
		MFloatPoint contrast(m_gnode->getVector("contrast"));
		o.ei_contrast( contrast.x );
		
		if( currentJob.pass == rpShadowMap ){
			o.a("this is a shadow pass, how to deal with the samples and filter?");
			o.ei_samples( currentJob.samples, currentJob.samples );
			o.ei_filter( EI_FILTER_BOX, 1 );
			//_S( ei_shading_rate( currentJob.shadingRateFactor ) );

		}else{
			//sample
			MFloatPoint sample(m_gnode->getVector("samples"));
			o.ei_samples(sample.x, sample.y);//_S("ei_Samples("<< liqglo.pixelSamples<<","<<liqglo.pixelSamples<<");");//4,4
			
			//filter
			eiInt filterType = m_gnode->getInt("filterType");
			eiScalar filterSize = m_gnode->getFloat("filterSize");
			o.ei_filter( filterType, filterSize );
			//_S( ei_shading_rate( liqglo.shadingRate ) );
		}
//		_S("ei_bucket_Size( int size );");
		

		//	Tessellation Quality:
// 		_S("ei_Shading_rate( float rate );");
// 		_S("ei_motion_factor( float factor );");
// 		_S("ei_max_displace( float dist );");

		//	Motion Blur:
//		_S("ei_Shutter( float open, float close );");
		o.a(boost::str(boost::format("transform motion=%d, deform motion=%d")%liqglo.liqglo_doMotion %liqglo.liqglo_doDef));
		o.ei_motion( liqglo.liqglo_doMotion||liqglo.liqglo_doDef );
//		_S("ei_motion_Segments( int num );");

		//	Trace Depth:
		liquidMessage2(messageError, "ei_trace_depth() need feed the parameters");
		MVector tracedepth(m_gnode->getVector("trace_depth"));
		o.ei_trace_depth( 4,4,4, tracedepth.x, tracedepth.y, tracedepth.z );

		//	Shadow:
//		_S("ei_Shadow( int type );");

		//	Rendering Algorithms:
// 		_S("ei_trace( int type );");
// 		_S("ei_Scanline( int type );");
// 		_S("ei_hardware( int type );");
// 		_S("ei_acceleration( int type );");
// 		_S("ei_Simd( int type );");
// 		_S("ei_bsp_Size( int size );");
// 		_S("ei_bsp_depth( int depth );");
// 		_S("ei_bsp_memory( int size );");

		//	Feature Disabling:
// 		_S("ei_lens( int type );");
// 		_S( ei_volume( name, ei_end );");
// 		_S("ei_geometry( int type );");

		bool displace = m_gnode->getBool("displace");
 		o.ei_displace( displace );
		if(displace)
		{
			float max_displace = m_gnode->getFloat("max_displace");
 			o.ei_max_displace(max_displace);
			
 			eiApprox	approx;
// 			_s( "eiApprox	approx" );

			//output the default
			ei_approx_set_defaults(&approx);
			{
				char buf[1024];
				sprintf_s(buf, 1024,
					"default approx: method=%d,any=%x,view_dep=%x,args=[%f,%f,%f,%f],   sharp=%f,min_subdiv=%d,max_subdiv=%d,max_grid_size=%d,motion_factor=%f",
					approx.method, approx.any, approx.view_dep, approx.args[0],approx.args[1],approx.args[2],approx.args[3],
					approx.sharp,  approx.min_subdiv, approx.max_subdiv, approx.max_grid_size, approx.motion_factor);
				liquidMessage2(messageInfo, "default approx: %s", buf );
				o.a(buf);
			}

			//set approx
			if(true)
			{
				approx.method	= m_gnode->getInt("approx_method") ;//EI_APPROX_METHOD_LENGTH
				approx.any		= m_gnode->getInt("approx_any");
				approx.view_dep = m_gnode->getInt("approx_view_dep");//eiTRUE
				MFloatPoint args(m_gnode->getVector("approx_args"));
				approx.args[0]	= args.x;
				approx.args[1]	= args.y;
				approx.args[2]	= args.z;
				approx.args[3]	= args.w;
				approx.sharp	= m_gnode->getFloat("approx_sharp");
				approx.min_subdiv		= m_gnode->getInt("approx_min_subdiv");
				approx.max_subdiv		= m_gnode->getInt("approx_max_subdiv");
				approx.max_grid_size	= m_gnode->getInt("approx_max_grid_size");
				approx.motion_factor	= m_gnode->getFloat("approx_motion_factor");//16.0f
			}
			else{//for test only - beg	

					 approx.method = EI_APPROX_METHOD_REGULAR;
				//_s( "approx.method = EI_APPROX_METHOD_REGULAR;" );

					 approx.view_dep = eiFALSE;
				//_s( "approx.view_dep = eiFALSE;" );

					 approx.args[ EI_APPROX_U ] = 8.0f;
				//_s( "approx.args[ EI_APPROX_U ] = 8.0f;" );

					 approx.args[ EI_APPROX_V ] = 8.0f;
				//_s( "approx.args[ EI_APPROX_V ] = 8.0f;" );

					 approx.motion_factor = 16.0f;
				//_s( "approx.motion_factor = 16.0f;" );
				//for test only - end
			}

			o.ei_approx(&approx);

		}


// 		_S("ei_imager( name , ei_end);");
// 		_S("ei_imager( type );");
		//	Caustics:
// 		_S("ei_caustic( int type );");
// 		_S("ei_caustic_photons( int photons );");
// 		_S("ei_caustic_accuracy( int samples, float radius );");
// 		_S("ei_caustic_Scale( float r, float g, float b );");
// 		_S("ei_caustic_filter( int filter, float filter_const );");
// 		_S("ei_photon_trace_depth( int reflect, int refract, int sum );");
// 		_S("ei_photon_decay( float decay );");
// 
		//	Global Illumination:
// 		_S("ei_globillum( int type );");
// 		_S("ei_globillum_photons( int photons );");
// 		_S("ei_globillum_accuracy( int samples, float radius );");
// 		_S("ei_globillum_Scale( float r, float g, float b );");
// 		_S("ei_photonvol_accuracy( int samples, float radius );");
// 		_S("ei_finalgather( int type );");
// 		_S("ei_finalgather_precompute( int type );");
// 		_S("ei_finalgather_accuracy( int rays, int samples, float density, float radius );");
// 		_S("ei_finalgather_falloff( int type );");
// 		_S("ei_finalgather_falloff( float start, float stop );");
// 		_S("ei_finalgather_filter( float size );");
// 		_S("ei_finalgather_trace_depth( int reflect, int refract, int diffuse, int sum );");
// 		_S("ei_finalgather_Scale( float r, float g, float b );");

		//	Frame Buffer Control:
// 		_S("ei_exposure( float gain, float gamma );");
// 		_S("ei_quantize( float one, float _min, float _max, float dither_amplitude );");
// 		_S("ei_frame_buffer( const char *name, int datatype, int interpolation );");

		//	Miscellaneous:
		int face = m_gnode->getInt("face");
		assert( (face!=EI_FACE_NONE) && (face<EI_FACE_COUNT) );
		o.ei_face( face );
//		_S( ei_memory( int size ) );
		//_S( ei_ambient( 0.12f, 0.13f, 0.05f ) );
		


		o.ei_end_options();

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

// 		// use liquid settings
// 		focal = currentJob.camera[0].focalLength;
// 		aperture = 2.0 * focal * tan(currentJob.camera[0].hFOV/2.0);
// 		aspect = currentJob.aspectRatio;
// 		_s("// liquid settings: focal="<<focal <<", aperture = "<<aperture<<", aspect="<<aspect );
// 		//e.g. focal=3.5, aperture = 2.7, aspect=1
		// use maya settings
		//NOTE: we will caculate these setting from structJob data.[yaoyansi]
		focal = fnCamera.focalLength(&status);
		IfMErrorWarn(status);
		double horizontalFOV,verticalFOV;
		fnCamera.getPortFieldOfView(width, height, horizontalFOV,verticalFOV);
		aperture = 2.0f * focal * tan(horizontalFOV /2.0f);
		aspect = aperture / (2.0f * focal * tan(verticalFOV / 2.0f));
		o.a(boost::str(boost::format(" maya settings: focal=%f, aperture = %f, aspect=%f")%focal %aperture %aspect ));
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
		o.a(boost::str(boost::format("Depth of Field on camera \"%s\" is turned %s in Maya") %currentJob.camera[0].name.asChar() %(bDepthOfField?"on":"off")));
		bDepthOfField = bDepthOfField && liqglo.doDof && currentJob.pass != rpShadowMap;

		MStringArray LensShaders, EnvironmentShaders;
		{
			//lens shader
			if(bDepthOfField)
			{
				gatherCameraShaders(LensShaders, currentJob.camera[0].name, "liqLensShader");

			}
			//env shader
			gatherCameraShaders(EnvironmentShaders, currentJob.camera[0].name, "liqEnvironmentShader");
		}

		o.ln();
		o.a("############################### camera #");
		std::string sCameraObjectName(std::string(currentJob.camera[0].name.asChar())+"_object");
		o.ei_camera( sCameraObjectName.c_str() );
			//_S( ei_frame( lframe, off ) );
			
			cameraOutput(currentJob);

			o.ei_focal( focal );
			o.ei_aperture( aperture / fnCamera.overscan() );
			o.ei_aspect( aspect );
			if( currentJob.pass != rpShadowMap && liqglo.liqglo_rotateCamera  == true ) {
				o.ei_resolution(height, width);
			}else{ 
				o.ei_resolution(width, height);
			}

			if( liqglo.m_renderViewCrop )
			{
				unsigned int left, right, bottom, top;
				MRenderView::getRenderRegion(left, right, bottom, top);
				o.ei_window(left, right+1, height-top, height-bottom+1);
			} 

			o.ei_clip( currentJob.camera[0].neardb, currentJob.camera[0].fardb);
			
			//lens shader
			for( std::size_t i = 0; i<LensShaders.length(); ++i)
			{
				o.ei_lens_shader( LensShaders[i].asChar() );
			}
			//env shader
			for( std::size_t i = 0; i<EnvironmentShaders.length(); ++i)
			{
				o.ei_environment_shader(EnvironmentShaders[i].asChar());
			}
		o.ei_end_camera();
		o.a("----------------------------------");
		o.ei_instance( currentJob.camera[0].name.asChar()	);
			o.ei_element(	sCameraObjectName.c_str() );
			o.ei_transform( m[0][0], m[0][1], m[0][2], m[0][3],   m[1][0], m[1][1], m[1][2], m[1][3],   m[2][0], m[2][1], m[2][2], m[2][3],   m[3][0], m[3][1], m[3][2], m[3][3] );
			//_S( ei_transform(  1.0f, 0.0f, 0.0f, 0.0f,  0.0f, 1.0f, 0.0f, 0.0f,  0.0f, 0.0f, 1.0f, 0.0f,  -1.95384,-2.76373,16.1852, 1.0f ) );
		o.ei_end_instance();
		o.ln();		
		m_groupMgr->addObjectInstance(currentJob.name.asChar(), currentJob.camera[0].name.asChar(), GIT_Camera);//_S( ei_init_instance( currentJob.camera[0].name.asChar() ) );

		return MStatus::kSuccess;
	}
	void Renderer::cameraOutput(const structJob &currentJob)
	{
		CM_TRACE_FUNC("Renderer::cameraOutput("<<currentJob.name.asChar()<<")");
	
		//beauty channel
		o.a("beauty channel");
		const std::size_t i = 0;
		{
			MString imageName;
			std::string imageType;
			std::string imageMode;

			if( liqglo.m_renderView ) 
			{
				//I use liqglo.m_displays[ 0 ].name for maya2renderer - yaoyansi
				imageName = liqglo.m_pixDir + parseString( liqglo.m_displays[ i ].name, false );

				// in this case, override the launch render settings
				if( liqglo.launchRender == false ) 
					liqglo.launchRender = true;
			}

			imageName = liqglo.m_pixDir + parseString( liqglo.m_displays[ i ].name, false );
			o.ei_output( imageName.asChar(), liqglo.m_displays[i].type.asChar(), EI_IMG_DATA_RGB );
			o.a(boost::str(boost::format("mode=%s")%liqglo.m_displays[i].mode.asChar() ) );
			o.ei_output_variable("color", EI_TYPE_VECTOR);
			o.ei_end_output();
		}

		//AOV channels
		o.a(" AOV");
		//refactor 19
		for(std::size_t i = 1; i < liqglo.m_displays.size(); ++i)
		{
			MString imageName;
			std::string imageType;
			std::string imageMode;

			// check if additionnal displays are enabled
			// if not, call it off after the 1st iteration.
			if( liqglo.m_ignoreAOVDisplays && i > 0 ) 
				break;

			// This is the override for the primary DD
			// when you render to maya's renderview.
			{
				// check if display is enabled
				if( !liqglo.m_displays[i].enabled ) 
				{
					o.a(boost::str(boost::format("%s channel is disabled.")%liqglo.m_displays[i].name.asChar()));
					continue;
				}

// 				if ( i == 0 ) {
// 					//imageName << liqRibTranslator::getInstancePtr()->generateImageName( "", currentJob, currentJob.format );  
// 					//I use liqglo.m_displays[ 0 ].name for maya2renderer - yaoyansi
//					imageName = liqglo.m_pixDir + parseString( liqglo.m_displays[i].name, false );					
// 					if( liqRibTranslator::getInstancePtr()->m_isStereoCamera )
// 						imageName += ".left.tif";
// 				}else{
// 					//imageName << "+" << liqRibTranslator::getInstancePtr()->generateImageName( (*m_displays_iterator).name, currentJob, imageType.c_str() ) ;
// 					//I use liqglo.m_displays[ 0 ].name for maya2renderer - yaoyansi
// 					imageName = liqglo.m_pixDir + parseString( liqglo.m_displays[i].name, false );
// 					if( liqRibTranslator::getInstancePtr()->m_isStereoCamera )
// 						imageName += ".right.tif";
//				}

// 				// get display type ( tiff, openexr, etc )
// 				if( !isBatchMode() ){
// 					imageType = (liqglo.m_displays[i].type == "")? 
// 						"framebuffer" : liqglo.m_displays[i].type.asChar();
// 				}else {// if in batch mode, we always use "file" -by yaoyansi
// 					imageType = liqglo.m_displays[i].type.asChar();
// 				}
				o.a( "make sure the specific aov macro is defined in liquidAOVMacroDef.h, and recomplie the shader again.");

				imageName = liqglo.m_pixDir + parseString( liqglo.m_displays[i].name, false );
				o.ei_output(imageName.asChar(), liqglo.m_displays[i].type.asChar(), EI_IMG_DATA_RGB);
				o.ei_output_variable(liqglo.m_displays[i].mode.asChar(), EI_TYPE_VECTOR);
				o.ei_end_output();
				o.ln();
			}

		}//for
		o.a(" AOV end");
		//refactor 19
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

		std::map<GroupID, Group>::iterator group_i = m_groupMgr->groups.begin();
		std::map<GroupID, Group>::iterator group_e = m_groupMgr->groups.end();
		for(; group_i!=group_e; ++group_i)
		{
			//each group
			o.ei_instgroup( group_i->first.name.c_str() );// not id
			{
				Group &group = group_i->second;

				//camera
				o.a("camera");
				o.ei_add_instance( group.getCamera().c_str());
				
				//light(s)
				o.a("light(s)");
				elvishray::LightNames lights = group.gatherLights();
				elvishray::LightNames::iterator l_i = lights.begin();
				elvishray::LightNames::iterator l_e = lights.end();
				for(; l_i != l_e; ++l_i)
				{
					o.ei_add_instance( (*l_i).c_str());
				}

				//mesh(s)
				o.a("mesh(s)");
				elvishray::MeshNames meshs = group.gatherMeshs();
				elvishray::MeshNames::iterator m_i = meshs.begin();
				elvishray::MeshNames::iterator m_e = meshs.end();
				for(; m_i != m_e; ++m_i)
				{
					o.ei_add_instance( (*m_i).c_str());
				}
			}
			o.ei_end_instgroup();
		}//for group

	}

	void Renderer::exportLightLinks(
		const structJob &currentJob__,
		const liqRibNodePtr ribNode__, 
		const MStringArray& lightedByWhichLightShapes)
	{
		CM_TRACE_FUNC("Renderer::exportLightLinks("<<currentJob__.name.asChar()<<","<<ribNode__->name.asChar()<<",lightedByWhichLightShapes.size="<<lightedByWhichLightShapes.length()<<")");

		if(lightedByWhichLightShapes.length() == 0){
			o.a(boost::str(boost::format("%s is not lighted.") %ribNode__->name.asChar() ));
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
			m_groupMgr->addLightLink(currentJob__.name.asChar(), 
				instanceName,
				lightedByWhichLightShapes[i].asChar()
				);
		}

	}
	/// shader interfaces are moved to er_shader.cpp

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

				if(nodetype=="liquidSurface" || nodetype=="liquidShader")
				{
					//liquidMessage2(messageInfo, (startingNode+"'s type is "+nodetype+", no need to convert").asChar());
					liqShader &currentShader = liqShaderFactory::instance().getShader( startingNode.asChar() );
					currentShader.write();

					IfMErrorWarn( cameraShaders.append(startingNode) );
				}else{
					//convertShadingNetworkToRSL(startingNode, node);
					liquidMessage2(messageError,"%s.%s is not liquidSurface and liquidShader.",node.asChar(), plug_.c_str());
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

		o.a(msg);
	}
	void Renderer::oneObjectBlock_reference_attribute_block2_writeShader_RibBox(const char* msg)
	{
		CM_TRACE_FUNC("Renderer::oneObjectBlock_reference_attribute_block2_writeShader_RibBox("<<msg<<")");

		o.a("writeShaderRibBox()");
		o.a(msg);
	}
	void Renderer::oneObjectBlock_reference_attribute_block2_writeShader_RegularShader(
		const liqRibNodePtr &ribNode__,
		const structJob &currentJob )
	{	
		CM_TRACE_FUNC("Renderer::oneObjectBlock_reference_attribute_block2_writeShader_RegularShader("<<ribNode__->name.asChar()<<","<<currentJob.name.asChar()<<")");


	}
	void Renderer::oneObjectBlock_reference_attribute_block2_writeShader_HasNoSurfaceShaderORIngoreSurface(
		const liqRibNodePtr &ribNode__, const MDagPath &path__, const bool m_shaderDebug
		)
	{
		CM_TRACE_FUNC("Renderer::oneObjectBlock_reference_attribute_block2_writeShader_HasNoSurfaceShaderORIngoreSurface("
			<<ribNode__->name.asChar()<<","<<path__.fullPathName().asChar()<<","<<m_shaderDebug<<")");

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

		//whether manager.ini exist---------------------------------------
		MString MAYA_PATH;
		IfMErrorWarn(MGlobal::executeCommand( "getenv(\"MAYA_LOCATION\")", MAYA_PATH));
		MString manager_ini_path(MAYA_PATH+"/bin/manager.ini");

		FILE *manager_ini = NULL;
		if( !(manager_ini = fopen(manager_ini_path.asChar(),"r")) )
		{
			liquidMessage2(messageError,
				"%s/bin/manager.ini is not found. Please copy Elvishray's manager.ini to %s/bin/,\n"
				"Make sure the searchpath in Maya.env is set to the directory which contains eiIMG.dll and eiSHADER.dll.\n"
				"e.g. :\n"
				"searchpath <LiquidRoot>/dependence/elvishray/bin\n",
				MAYA_PATH.asChar(),
				MAYA_PATH.asChar()
				);

			//assert(0&&"[ERROR] $(MAYA_LOCATION)/bin/manager.ini is not found. Please copy Elvishray's manager.ini to $(MAYA_LOCATION)/bin/\n" 
			//	"See Maya Script Editor Window for more details.");
			return false;
		}
		fclose(manager_ini);

		//-------------------------------------
		int face = m_gnode->getInt("face");
		if( face<=EI_FACE_NONE || EI_FACE_COUNT<=face ){
			liquidMessage2(messageError,"face(%d), must in scope (%d,%d).", face, EI_FACE_NONE, EI_FACE_COUNT);
			assert( 0 && "face is invalid." );
			return false;
		}

		return true;
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

		return "tex";
	}
	bool Renderer::isTextureTypeSupported(const MString &textureType)const
	{
		CM_TRACE_FUNC("Renderer::getTextureExt("<<textureType.asChar() <<")");

		if(textureType=="tex"){
			return true;
		}
		return false;
	}
	MStatus Renderer::render(const structJob&)
	{
		CM_TRACE_FUNC("Renderer::render()");

		MString cameraFullPath;
		int width=0, height=0;
		{
			MStringArray cameraFullPaths;
			IfMErrorWarn(MGlobal::executeCommand("string $cam = `getAttr liquidGlobals.renderCamera`; ls -long $cam;", cameraFullPaths));
			cameraFullPath = cameraFullPaths[0];
			IfMErrorWarn(MGlobal::executeCommand("getAttr liquidGlobals.xResolution",width));
			IfMErrorWarn(MGlobal::executeCommand("getAttr liquidGlobals.yResolution",height));
		}

		//
		if( isBatchMode() )
		{
			o.a(" in batch render mode");
			o.ei_render( m_root_group.c_str(), cameraFullPath.asChar(), m_option.c_str() );
		}else{
			//set callback function for ER
			o.ei_connection(&(MayaConnection::getInstance()->connection.base));

			renderPreview(width, height, false, false, 
				m_root_group.c_str(), cameraFullPath, m_option.c_str());
		}

		return MS::kSuccess;
	}
	MStatus Renderer::render_ipr()
	{
		CM_TRACE_FUNC("Renderer::render_ipr()");

		MString cameraFullPath;
		int width=0, height=0;
		{
			MStringArray cameraFullPaths;
			IfMErrorWarn(MGlobal::executeCommand("string $cam = `getAttr liquidGlobals.renderCamera`; ls -long $cam;", cameraFullPaths));
			cameraFullPath = cameraFullPaths[0];
			IfMErrorWarn(MGlobal::executeCommand("getAttr liquidGlobals.xResolution",width));
			IfMErrorWarn(MGlobal::executeCommand("getAttr liquidGlobals.yResolution",height));
		}

		//set callback function for ER
		o.ei_connection(&(MayaConnection::getInstance()->connection.base));

		renderPreview(width, height, true, false, 
			 m_root_group.c_str(), cameraFullPath, m_option.c_str());

		return MS::kSuccess;
	}
	void Renderer::renderPreview(unsigned int width, unsigned int height,
		bool doNotClearBackground, bool immediateFeedback,
		const MString &renderGroup, const MString &cameraName, const MString &optionName)
	{
		CM_TRACE_FUNC("Renderer::renderPreview("<<width<<","<<height<<","
			<<doNotClearBackground<<","<<immediateFeedback<<","
			<<renderGroup.asChar()<<","<<renderGroup.asChar()<<","<<optionName.asChar()<<")");

		if( true )
		{
			_renderPreview(width, height, doNotClearBackground, immediateFeedback, 
				renderGroup, cameraName, optionName);
		}else{
			m_renderingThread = boost::thread(
					boost::bind(&elvishray::Renderer::_renderPreview, this,
						width, height, doNotClearBackground, immediateFeedback, 
						renderGroup, cameraName, optionName
					)
				);
			m_renderingThread.detach();
		}
	}
	void Renderer::_renderPreview( unsigned int width, unsigned int height,
		bool doNotClearBackground, bool immediateFeedback,
		const MString &renderGroup, const MString &cameraName, const MString &optionName)
	{
		CM_TRACE_FUNC("Renderer::_renderPreview("<<width<<","<<height<<","
			<<doNotClearBackground<<","<<immediateFeedback<<","
			<<renderGroup.asChar()<<","<<cameraName.asChar()<<","<<optionName.asChar()<<")");

		// start render - region render, don't clear background, don't immediate feedback
		if (MayaConnection::getInstance()->startRender( width, height, doNotClearBackground, immediateFeedback) != MS::kSuccess)
		{
			assert(0&&"MayaConnection: error occured in startRenderRegion.");
			o.a("MayaConnection: error occured in startRenderRegion." );
			//MayaConnection::delInstance();				
			return /*MS::kFailure*/;
		}

		if( true )
		{
			o.ei_render( renderGroup.asChar(), cameraName.asChar(), optionName.asChar() );
		} else {
			m_renderingThread = boost::thread(
					boost::bind(&ei_render, 
						renderGroup.asChar(), cameraName.asChar(), optionName.asChar()
					)
				);
			//m_renderingThread.join();
			//m_renderingThread.detach();
		}
		
		// end render
		if (MayaConnection::getInstance()->endRender() != MS::kSuccess)
		{
			assert(0&&"MayaConnection: error occured in endRender.");
			o.a( "MayaConnection: error occured in endRender." );
			//MayaConnection::delInstance();
			return /*MS::kFailure*/;
		}
		//MayaConnection::delInstance();
	}
	//
	MStatus Renderer::iprBegin()
	{
		CM_TRACE_FUNC("Renderer::iprBegin()");

		m_iprMgr = new IprMgr();
		return MS::kSuccess;
	}

	MStatus Renderer::iprEnd()
	{
		CM_TRACE_FUNC("Renderer::iprEnd()");

		delete m_iprMgr;
		m_iprMgr = NULL;
				
		return MS::kSuccess;
	}
	//IPR callback functions
	MStatus Renderer::IPR_AttributeChangedCallback( MNodeMessage::AttributeMessage msg, 
		MPlug & plug, MPlug & otherPlug, void* userData)
	{
		CM_TRACE_FUNC("Renderer::IPR_AttributeChangedCallback("<<msg<<","<<plug.name().asChar()<<","<<otherPlug.name().asChar()<<",userData)");
		liquidMessage2(messageInfo, "Renderer::IPR_AttributeChangedCallback()");
		
		o.ln();
		o.ln();
		o.ln();
		o.ln();
		o.a(" Renderer::IPR_AttributeChangedCallback");		
		
		m_iprMgr->onAttributeChanged(msg, plug, otherPlug, userData );


		m_root_group = "perspShape";
		m_option     = "perspShape_option";

		render_ipr();

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

}//namespace

#endif//_USE_ELVISHRAY_