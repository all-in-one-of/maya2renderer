#include <liqConfig.h>
#ifdef _USE_RENDERMAN_

#include "rm_renderer.h"

#include "../common/prerequest_std.h"

// Renderman Headers
//extern "C" {
#include "ri_interface.h"
//}

// Maya headers
#include "../common/prerequest_maya.h"
#include "../common/mayacheck.h"
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
#include "rm_shadowRibWriterMgr.h"//#include <liqShadowRibWriterMgr.h>
#include "rm_heroRibWriterMgr.h"//#include <liqHeroRibWriterMgr.h>
#include <liqRibCamera.h>
#include <liqRibTranslator.h>

#include "../renderermgr.h"
#include "rm_helper.h"
#include "shaderOutputRSL.h"

#if defined(_WIN32)/* && !defined(DEFINED_LIQUIDVERSION)*/
// unix build gets this from the Makefile
extern const char *LIQUIDVERSION;
#endif

namespace renderman
{
	Renderer::Renderer()
	{
// 		liquid::RendererMgr::getInstancePtr()->registerRenderer(
// 			"renderman", this
// 		);
	}
	//
	Renderer::~Renderer()
	{

	}
	//
	void Renderer::test()
	{
		liquidMessage2(messageInfo, "this is %s.", "renderman" );
	}
	//
	void Renderer::setRenderScriptFormatAndCompress(const bool bbinary, const bool bcompress)
	{
		CM_TRACE_FUNC("Renderer::setRenderScriptFormatAndCompress("<<bbinary<<","<<bcompress<<")");

		// Rib client file creation options MUST be set before RiBegin
#if defined(PRMAN) || defined(DELIGHT)
		/* THERE IS A RIBLIB BUG WHICH PREVENTS THIS WORKING */
		_RiOption_format_compress(liqglo.liqglo_doBinary, liqglo.liqglo_doCompression);
#endif // PRMAN || DELIGHT
	}
	//
	RtToken g_typeAscii       = tokenCast("Ascii");
	RtToken g_typeBinary      = tokenCast("Binary");
	RtToken g_compressionNone = tokenCast("None");
	RtToken g_compressionGzip = tokenCast("Gzip");
	RtToken g_indentSpace = tokenCast("Space");
	RtToken g_indentTab   = tokenCast("Tab");
	void Renderer::_RiOption_format_compress(bool bBinary, bool bCompress)
	{
		CM_TRACE_FUNC("Renderer::_RiOption_format_compress("<<bBinary<<","<<bCompress<<")");

	 #ifdef RIBLIB_AQSIS
  		LIQDEBUGPRINTF( "-> setting binary option\n" );
  		RtToken binary[1]; binary[0] = g_typeBinary;
  		RtToken ascii[1] ; ascii[0]  = g_typeAscii;
  		RiOption( tokenCast("RI2RIB_Output"), "Type", ( RtPointer )(bBinary?&binary:&ascii),        RI_NULL );
	  
  		LIQDEBUGPRINTF( "-> setting compression option\n" );
  		RtToken gzip[1]; gzip[0] = g_compressionGzip;
  		RtToken none[1]; none[0] = g_compressionNone;
  		RiOption( tokenCast("RI2RIB_Output"), "Compression", ( RtPointer )(bCompress?&gzip:&none), RI_NULL );

  		LIQDEBUGPRINTF( "-> setting indent option\n" );
		RtToken itype[1];			RtInt isize[1];
		//itype[0] = g_indentTab;		isize[0] = 1;
		itype[0] = g_indentSpace;	isize[0] = 4;
		RiOption( tokenCast("RI2RIB_Indentation"), "Type", &itype, RI_NULL );
		RiOption( tokenCast("RI2RIB_Indentation"), "Size", &isize, RI_NULL );

	 #else
		LIQDEBUGPRINTF( "-> setting binary option\n" );
		//[refactor][1.9.2.3 begin] from ribOutput()
		if( liqglo_doBinary ) 
		{
			RtString format[ 1 ] = { "binary" };
			RiOption( "rib", "format", ( RtPointer )format, RI_NULL );
		}else{
			RtString format[ 1 ] = { "ascii" };
			RiOption( "rib", "format", ( RtPointer )format, RI_NULL );

	#ifdef PRMAN
			RtString style = "indented";
			RiOption( "rib", "string asciistyle", &style, RI_NULL );
			//std::cout <<"[liquid DEBUG] RiOption( rib, string asciistyle, &style, RI_NULL );"<<std::endl;
	#endif
		}

		LIQDEBUGPRINTF( "-> setting compression option\n" );
		if( liqglo_doCompression ) 
		{
			RtString comp[ 1 ] = { "gzip" };
			RiOption( "rib", "compression", ( RtPointer )comp, RI_NULL );
		}else{
			RtString comp[ 1 ] = { "none" };
			RiOption( "rib", "compression", ( RtPointer )comp, RI_NULL );
		}
		//[refactor][1.9.2.3 end] from ribOutput()
	#endif
	}
	//
	MStatus Renderer::worldPrologue(const structJob& currentJob)
	{
		CM_TRACE_FUNC("Renderer::worldPrologue("<<currentJob.name.asChar()<<")");

		MStatus returnStatus = MS::kSuccess;
		LIQDEBUGPRINTF( "-> Writing world prologue.\n" );
		// if this is a readArchive that we are exporting then ingore this header information
		if( !liqglo.m_exportReadArchive )
		{
			MFnDependencyNode globalsNode( liqglo.rGlobalObj );
			MPlug prePostplug( globalsNode.findPlug( "preWorldMel" ) );
			MString melcommand( prePostplug.asString() );
			// put in pre-worldbegin statements
			if(liqglo.m_preWorldRIB != "" || melcommand.length() )
			{
				RiArchiveRecord(RI_COMMENT,  " Pre-WorldBegin RIB from liquid globals");
				MGlobal::executeCommand( melcommand );
				RiArchiveRecord(RI_VERBATIM, ( char* )liqglo.m_preWorldRIB.asChar());
				RiArchiveRecord(RI_VERBATIM, "\n");
			}
			// output the arbitrary clipping planes here /////////////
			//
			boost::shared_ptr< liqRibHT > &htable = liqRibTranslator::getInstancePtr()->htable;
			{
				for ( RNMAP::iterator rniter( htable->RibNodeMap.begin() ); rniter != htable->RibNodeMap.end(); rniter++ ) 
				{
					LIQ_CHECK_CANCEL_REQUEST;
					liqRibNodePtr   ribNode = (*rniter).second;
					if( ribNode->object(0)->ignore || ribNode->object(0)->type != MRT_ClipPlane ) 
						continue;
					RiTransformBegin();
					if( liqglo.m_outputComments )
						RiArchiveRecord( RI_COMMENT, "Clipping Plane: %s", ribNode->name.asChar(), RI_NULL );
					RtMatrix ribMatrix;
					MMatrix matrix;
					MDagPath path;

					matrix = ribNode->object(0)->matrix( path.instanceNumber() );
					matrix.get( ribMatrix );
					RiConcatTransform( ribMatrix );

					ribNode->object(0)->writeObject("", currentJob, false);
					ribNode->object(0)->written = 1;

					RiTransformEnd();
				}
			}
			RiWorldBegin();
			// set attributes from the globals
#ifdef GENERIC_RIBLIB      
			extern int useAdvancedVisibilityAttributes;
			useAdvancedVisibilityAttributes = false;
#endif
			if( liqglo.rt_useRayTracing )
			{
				RiArchiveRecord(RI_COMMENT,  " Ray-Tracing Attributes from liquid globals");
				RtInt on( 1 );

				if ( !liqglo.liquidRenderer.supports_ADVANCED_VISIBILITY )
				{
					RtString trans = "shader";
					RiAttribute( "visibility", "int trace", &on, RI_NULL );
					RiAttribute( "visibility", "string transmission", &trans, RI_NULL );
				}
				else
				{
#ifdef GENERIC_RIBLIB         
					useAdvancedVisibilityAttributes = true;
#endif
					RiAttribute( "visibility", "int diffuse", &on, RI_NULL );
					RiAttribute( "visibility", "int specular", &on, RI_NULL );
					RiAttribute( "visibility", "int transmission", &on, RI_NULL );
				}

				if( liqglo.rt_traceDisplacements )
					RiAttribute("trace", "int displacements", &on, RI_NULL );
				if( liqglo.rt_traceSampleMotion )
					RiAttribute("trace", "int samplemotion", &on, RI_NULL );
				if( liqglo.rt_traceBias != 0 )
					RiAttribute("trace", "float bias", &liqglo.rt_traceBias, RI_NULL );
				RiAttribute("trace", "int maxdiffusedepth", &liqglo.rt_traceMaxDiffuseDepth, RI_NULL);
				RiAttribute("trace", "int maxspeculardepth", &liqglo.rt_traceMaxSpecularDepth, RI_NULL);
				if( liqglo.rt_irradianceMaxError != -1.0 )
					RiAttribute( "irradiance", (RtToken) "float maxerror", &liqglo.rt_irradianceMaxError, RI_NULL );
				if( liqglo.rt_irradianceMaxPixelDist != -1.0 )
					RiAttribute( "irradiance", (RtToken) "float maxpixeldist", &liqglo.rt_irradianceMaxPixelDist, RI_NULL );

				// ymesh: add photon/caustic map attribites
				if (  liqglo.rt_photonGlobalHandle != "" || liqglo.rt_causticGlobalHandle != "") 
				{
					MString parsedName = parseString( liqglo.rt_photonGlobalHandle, false );  //  doEscaped = false

					RtString photon_map = const_cast< char* >( parsedName.asChar() );
					RiAttribute( "photon", (RtToken) "globalmap", &photon_map, RI_NULL );

					parsedName = parseString( liqglo.rt_causticGlobalHandle, false );  //  doEscaped = false
					RtString caustic_map = const_cast< char* >( parsedName.asChar() );
					RiAttribute( "photon", (RtToken) "causticmap", &caustic_map, RI_NULL );

					RtString model;
					switch ( liqglo.rt_photonShadingModel  ) 
					{
					case liqRibNode::photon::SHADINGMODEL_GLASS:
						model = "glass";
						break;
					case liqRibNode::photon::SHADINGMODEL_WATER:
						model = "water";
						break;
					case liqRibNode::photon::SHADINGMODEL_CHROME:
						model = "chrome";
						break;
					case liqRibNode::photon::SHADINGMODEL_TRANSPARENT:
						model = "chrome";
						break;
					case liqRibNode::photon::SHADINGMODEL_DIALECTRIC:
						model = "dielectric";
						break;
					case liqRibNode::photon::SHADINGMODEL_MATTE:
					default:
						model = "matte";
					}
					RiAttribute( "photon", (RtToken) "shadingmodel", &model, RI_NULL );

					RtInt estimator = liqglo.rt_photonEstimator;
					RiAttribute( "photon", (RtToken) "estimator", &estimator, RI_NULL );
				}
			}
			// put in post-worldbegin statements
			prePostplug = globalsNode.findPlug( "postWorldMel" );
			melcommand = prePostplug.asString();
			if(liqglo.m_postWorldRIB != "" || melcommand.length() )
			{
				RiArchiveRecord( RI_COMMENT,  " Post-WorldBegin RIB from liquid globals" );
				MGlobal::executeCommand( melcommand );
				RiArchiveRecord( RI_VERBATIM, ( char* )liqglo.m_postWorldRIB.asChar() );
				RiArchiveRecord( RI_VERBATIM, "\n");
			}
			RiTransformBegin();
			RiCoordinateSystem( "worldspace" );
			RiTransformEnd();

			RiTransformBegin();
			RiRotate( -90., 1., 0., 0. );
			RiCoordinateSystem( "_environment" );
			RiTransformEnd();
		}//if( !m_exportReadArchive )
		//RiReverseOrientation();//???
		return returnStatus;
	}
	//
	MStatus Renderer::worldEpilogue()
	{
		CM_TRACE_FUNC("Renderer::worldEpilogue()");

		MStatus returnStatus = MS::kSuccess;
		LIQDEBUGPRINTF( "-> Writing world epilogue.\n" );
		// If this is a readArchive that we are exporting there's no world block
		if( !liqglo.m_exportReadArchive ) 
			RiWorldEnd();
		return returnStatus;
	}
	//
//	MStatus Renderer::exportLight(const liqRibNodePtr& ribNode, const structJob &currentJob)
//	{
////moved to Renderer::writeLight_pre()
//// 		RtInt on( 1 );
//// 			// We need to enclose lights in attribute blocks because of the
//// 			// new added attribute support
//// 			RiAttributeBegin();
//// 
//// 			// All this stuff below should be handled by a new attribute class
//// 			LIQDEBUGPRINTF( "-> RibNodeName " );
//// 			RtString RibNodeName = getLiquidRibName( ribNode->name.asChar() );
//// 			LIQDEBUGPRINTF( "= %s.\n", (char *)RibNodeName  );
//// 			RiAttribute( "identifier", "name", &RibNodeName, RI_NULL );
//// 			if( ribNode->trace.sampleMotion )
//// 				RiAttribute( "trace", (RtToken) "samplemotion", &on, RI_NULL );
//// 			if( ribNode->trace.displacements )
//// 				RiAttribute( "trace", (RtToken) "displacements", &on, RI_NULL );
//// 			if( ribNode->trace.bias != 0.01f )
//// 			{
//// 				RtFloat bias = ribNode->trace.bias;
//// 				RiAttribute( "trace", (RtToken) "bias", &bias, RI_NULL );
//// 			}
//// 			if( ribNode->trace.maxDiffuseDepth != 1 )
//// 			{
//// 				RtInt mddepth = ribNode->trace.maxDiffuseDepth;
//// 				RiAttribute( "trace", (RtToken) "maxdiffusedepth", &mddepth, RI_NULL );
//// 			}
//// 			if( ribNode->trace.maxSpecularDepth != 2 )
//// 			{
//// 				RtInt msdepth = ribNode->trace.maxSpecularDepth;
//// 				RiAttribute( "trace", (RtToken) "maxspeculardepth", &msdepth, RI_NULL );
//// 			}
//			ribNode->object(0)->writeObject("", currentJob, false);//call liqRibLightData::_write(...)
//			ribNode->object(0)->written = 1;
////moved to Renderer::writeLight_post()
//// 			// The next line pops the light...
//// 			RiAttributeEnd();
//// 			// close light by default // [1/30/2012 yaoyansi]
//// 			RiIlluminate( ribNode->object(0)->lightHandle(), 0 );//
//			return MStatus::kSuccess;
//	}
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
		const liqString & shadowName,
		const liqMatrix &transform)
	{
		CM_TRACE_FUNC("Renderer::exportShadowPassLight("<<shadertype<<","<<shaderinstance<<","<<shadowName<<",liqMatrix t)");

		RiConcatTransform( * const_cast< RtMatrix* >( &transform ) );
		return RiLightSource( "liquidshadowpasslight", "string shadowname", &shadowName, RI_NULL );
	}

	void Renderer::transform_(const liqMatrix& transform)
	{
		CM_TRACE_FUNC("Renderer::transform_(liqMatrix transform)");
	}
	MStatus Renderer::liqRibLightData_write(
		const liqRibLightData *lightdata, const structJob &currentJob)
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

		// transform ////////////////////////////////////////////////////////////////////////
		//{// transform motion blur
		MDagPath path = ribNode__->path();
		MObject transform = path.transform();
		// Alf: preTransformMel
//		liqRibTranslator::getInstancePtr()->preTransformMel(transform);

		const bool bMotionBlur =
			ribNode__->motion.transformationBlur &&
			( ribNode__->object( 1 ) ) &&
			//( ribNode__->object(0)->type != MRT_Locator ) && // Why the fuck do we not allow motion blur for locators?
			( currentJob__.pass != rpShadowMap || currentJob__.shadowType == stDeep );

// 		const bool bMatrixMotionBlur = 
// 			liqglo.liqglo_doMotion 
// 			&& bMotionBlur;

//		liqRibTranslator::getInstancePtr()->MaxtrixMotionBlur(ribNode__, path, bMatrixMotionBlur);

		// Alf: postTransformMel
//		liqRibTranslator::getInstancePtr()->postTransformMel(transform);
		//}

		//////////////////////////////////////////////////////////////////////////
// 		if( ribNode__->rib.hasBox() ) 
// 			RiArchiveRecord( RI_COMMENT, " RIB Box:\n%s", ribNode__->rib.box.asChar() );
// 		if( ribNode__->rib.hasGenerator() ){
// 			MGlobal::executeCommand( ribNode__->rib.generator, false, false );
// 		}
// 		if( ribNode__->rib.hasReadArchive() ) 
// 			RiArchiveRecord( RI_VERBATIM, " ReadArchive \"%s\" \n", ribNode__->rib.readArchive.asChar() );
// 		if( ribNode__->rib.hasDelayedReadArchive() ) 
// 		{
// 			RiArchiveRecord( RI_VERBATIM, " Procedural \"DelayedReadArchive\" [ \"%s\" ] [ %f %f %f %f %f %f ] \n", ribNode__->rib.delayedReadArchive.asChar(), ribNode__->bound[0],ribNode__->bound[3],ribNode__->bound[1],ribNode__->bound[4],ribNode__->bound[2],ribNode__->bound[5] );
// 			// should be using the bounding box node - Alf
// 		}

		// Geometry ////////////////////////////////////////////////////////////////////////
		// Alf: preShapeMel
//		liqRibTranslator::getInstancePtr()->preShapeMel(transform);

		if( !ribNode__->ignoreShapes ) 
		{
			liqRIBMsg("ribNode__->object(0)->type= %d, path=%s",ribNode__->object(0)->type, ribNode__->path().fullPathName().asChar() );
			// check to see if we are writing a curve to set the proper basis
			if( ribNode__->object(0)->type == MRT_NuCurve
				|| ribNode__->object(0)->type == MRT_PfxHair
				|| ribNode__->object(0)->type == MRT_PfxTube
				|| ribNode__->object(0)->type == MRT_PfxLeaf
				|| ribNode__->object(0)->type == MRT_PfxPetal 
				|| ribNode__->object(0)->type == MRT_Curves )
			{
//				RiBasis( RiBSplineBasis, 1, RiBSplineBasis, 1 );
			} 
			bool hasRibBoxData=ribNode__->rib.hasGenerator()
				|| ribNode__->rib.hasReadArchive()  
				|| ribNode__->rib.hasDelayedReadArchive();
			if(hasRibBoxData)
			{
				//if ribNode is tagged as readArchive or delayedReadArchive, 
				//we do not output its geometry data.
				liqRIBMsg("%s has ribbox data,so we do not output the deometry.", ribNode__->name.asChar());
			}else{
				bool bGeometryMotion = 
					liqglo.liqglo_doDef 
					&& bMotionBlur
					&& ( ribNode__->object(0)->type != MRT_RibGen );

				if( bGeometryMotion )
				{
//					if(liqglo.liqglo_relativeMotion)
//						RiMotionBeginV( liqglo.liqglo_motionSamples, liqglo.liqglo_sampleTimesOffsets );
//					else
//						RiMotionBeginV( liqglo.liqglo_motionSamples, liqglo.liqglo_sampleTimes );

					for ( unsigned msampleOn( 0 ); msampleOn < liqglo.liqglo_motionSamples; msampleOn++ )
					{ 
						this->_writeObject(ribNode__, currentJob__, bGeometryMotion, msampleOn, false);
						//_writeObject() will call Renderer::exportOneGeometry_Mesh()
					}
//					RiMotionEnd();
				}else{
					this->_writeObject(ribNode__, currentJob__, bGeometryMotion, 0, false);
					//_writeObject() will call Renderer::exportOneGeometry_Mesh()
				}// if(bGeometryMotion)
			}//if(hasRibBoxData)


			// Alf: postShapeMel
//			liqRibTranslator::getInstancePtr()->postShapeMel(transform);
		} // else RiArchiveRecord( RI_COMMENT, " Shapes Ignored !!" );

	}
	void Renderer::exportOneObject_reference(
		const liqRibNodePtr &ribNode__, 
		const structJob &currentJob__
		)
	{
		CM_TRACE_FUNC("Renderer::exportOneObject_reference("<<ribNode__->name.asChar()<<","<<currentJob__.name.asChar()<<")");

		// transform ////////////////////////////////////////////////////////////////////////
		//{// transform motion blur
		MDagPath path = ribNode__->path();
		MObject transform = path.transform();
		// Alf: preTransformMel
		liqRibTranslator::getInstancePtr()->preTransformMel(transform);

		const bool bMotionBlur =
			ribNode__->motion.transformationBlur &&
			( ribNode__->object( 1 ) ) &&
			//( ribNode__->object(0)->type != MRT_Locator ) && // Why the fuck do we not allow motion blur for locators?
			( currentJob__.pass != rpShadowMap || currentJob__.shadowType == stDeep );

		const bool bMatrixMotionBlur = 
			liqglo.liqglo_doMotion 
			&& bMotionBlur;

		MaxtrixMotionBlur(ribNode__, path, bMatrixMotionBlur);

		// Alf: postTransformMel
		liqRibTranslator::getInstancePtr()->postTransformMel(transform);
		//}

		//////////////////////////////////////////////////////////////////////////
		if( ribNode__->rib.hasBox() ) 
			RiArchiveRecord( RI_COMMENT, " RIB Box:\n%s", ribNode__->rib.box.asChar() );
		if( ribNode__->rib.hasGenerator() ){
			MGlobal::executeCommand( ribNode__->rib.generator, false, false );
		}
		if( ribNode__->rib.hasReadArchive() ) 
			RiArchiveRecord( RI_VERBATIM, " ReadArchive \"%s\" \n", ribNode__->rib.readArchive.asChar() );
		if( ribNode__->rib.hasDelayedReadArchive() ) 
		{
			RiArchiveRecord( RI_VERBATIM, " Procedural \"DelayedReadArchive\" [ \"%s\" ] [ %f %f %f %f %f %f ] \n", ribNode__->rib.delayedReadArchive.asChar(), ribNode__->bound[0],ribNode__->bound[3],ribNode__->bound[1],ribNode__->bound[4],ribNode__->bound[2],ribNode__->bound[5] );
			// should be using the bounding box node - Alf
			/* {
			// this is a visual display of the archive's bounding box
			RiAttributeBegin();
			RtColor debug;
			debug[0] = debug[1] = 1;
			debug[2] = 0;
			RiColor( debug );
			debug[0] = debug[1] = debug[2] = 0.250;
			RiOpacity( debug );
			RiSurface( "defaultsurface", RI_NULL );
			RiArchiveRecord( RI_VERBATIM, "Attribute \"visibility\" \"int diffuse\" [0]\n" );
			RiArchiveRecord( RI_VERBATIM, "Attribute \"visibility\" \"int specular\" [0]\n" );
			RiArchiveRecord( RI_VERBATIM, "Attribute \"visibility\" \"int transmission\" [0]\n" );
			float xmin = ribNode->bound[0];
			float ymin = ribNode->bound[1];
			float zmin = ribNode->bound[2];
			float xmax = ribNode->bound[3];
			float ymax = ribNode->bound[4];
			float zmax = ribNode->bound[5];
			RiSides( 2 );
			RiArchiveRecord( RI_VERBATIM, "Polygon \"P\" [ %f %f %f  %f %f %f  %f %f %f  %f %f %f ]\n", xmin,ymax,zmin, xmax,ymax,zmin, xmax,ymax,zmax, xmin,ymax,zmax );
			RiArchiveRecord( RI_VERBATIM, "Polygon \"P\" [ %f %f %f  %f %f %f  %f %f %f  %f %f %f ]\n", xmin,ymin,zmin, xmax,ymin,zmin, xmax,ymin,zmax, xmin,ymin,zmax );
			RiArchiveRecord( RI_VERBATIM, "Polygon \"P\" [ %f %f %f  %f %f %f  %f %f %f  %f %f %f ]\n", xmin,ymax,zmax, xmax,ymax,zmax, xmax,ymin,zmax, xmin,ymin,zmax );
			RiArchiveRecord( RI_VERBATIM, "Polygon \"P\" [ %f %f %f  %f %f %f  %f %f %f  %f %f %f ]\n", xmin,ymax,zmin, xmax,ymax,zmin, xmax,ymin,zmin, xmin,ymin,zmin );
			RiAttributeEnd();
			} */
		}

		// Geometry ////////////////////////////////////////////////////////////////////////
		// Alf: preShapeMel
		liqRibTranslator::getInstancePtr()->preShapeMel(transform);
		//refactor 31 begin from liqRibTranslator::objectBlock()
		// receive shadows ?   =>   Attribute "user" "int receivesShadows" [0/1]
		if( !ribNode__->object(0)->receiveShadow )
		{
			int receiveShadows = ribNode__->object(0)->receiveShadow;
			RiAttribute("user", (RtToken)"int receivesShadows", &receiveShadows, RI_NULL);
		}
		//refactor 31 end

		if( !ribNode__->ignoreShapes ) 
		{
			liqRIBMsg("ribNode__->object(0)->type= %d, path=%s",ribNode__->object(0)->type, ribNode__->path().fullPathName().asChar() );
			// check to see if we are writing a curve to set the proper basis
			if( ribNode__->object(0)->type == MRT_NuCurve
				|| ribNode__->object(0)->type == MRT_PfxHair
				|| ribNode__->object(0)->type == MRT_PfxTube
				|| ribNode__->object(0)->type == MRT_PfxLeaf
				|| ribNode__->object(0)->type == MRT_PfxPetal 
				|| ribNode__->object(0)->type == MRT_Curves )
			{
				RiBasis( RiBSplineBasis, 1, RiBSplineBasis, 1 );
			} 
			bool hasRibBoxData=ribNode__->rib.hasGenerator()
							|| ribNode__->rib.hasReadArchive()  
							|| ribNode__->rib.hasDelayedReadArchive();
			if(hasRibBoxData)
			{
				//if ribNode is tagged as readArchive or delayedReadArchive, 
				//we do not output its geometry data.
				liqRIBMsg("%s has ribbox data,so we do not output the deometry.", ribNode__->name.asChar());
			}else{
				bool bGeometryMotion = 
					liqglo.liqglo_doDef 
					&& bMotionBlur
					&& ( ribNode__->object(0)->type != MRT_RibGen );

				if( bGeometryMotion )
				{
#define GeometryMotionBlur_SimpleEdition
#ifndef GeometryMotionBlur_SimpleEdition
					// For each grain, open a new motion block...
					for( unsigned i( 0 ); i < ribNode__->object( 0 )->granularity(); i++ ) //granularity() is always 1.
					{
						if( ribNode__->object( 0 )->isNextObjectGrainAnimated() ) //isNextObjectGrainAnimated() is always true.
						{
							RiMotionBeginV( liqglo.liqglo_motionSamples, 
								( liqglo.liqglo_relativeMotion )? 
								liqglo.liqglo_sampleTimesOffsets : liqglo.liqglo_sampleTimes);

							for ( unsigned msampleOn( 0 ); msampleOn < liqglo.liqglo_motionSamples; msampleOn++ )
							{ 
								liqRibTranslator::getInstancePtr()->_writeObject(ribNode__, currentJob__, true, msampleOn);
							}
							RiMotionEnd();
						}else {
							RiArchiveRecord( RI_COMMENT, "the the next object grain is not animated" );

							liqRibTranslator::getInstancePtr()->_writeObject(ribNode__, currentJob__, false, 0);
						}
					}
#else
					if(liqglo.liqglo_relativeMotion)
						RiMotionBeginV( liqglo.liqglo_motionSamples, liqglo.liqglo_sampleTimesOffsets );
					else
						RiMotionBeginV( liqglo.liqglo_motionSamples, liqglo.liqglo_sampleTimes );

					for ( unsigned msampleOn( 0 ); msampleOn < liqglo.liqglo_motionSamples; msampleOn++ )
					{ 
						this->_writeObject(ribNode__, currentJob__, true, msampleOn, true);
						//_writeObject() will call Renderer::exportOneGeometry_Mesh()
					}
					RiMotionEnd();
#endif
				}else{
					this->_writeObject(ribNode__, currentJob__, false, 0, true);
					//_writeObject() will call Renderer::exportOneGeometry_Mesh()
				}// if(bGeometryMotion)
			}//if(hasRibBoxData)


			// Alf: postShapeMel
			liqRibTranslator::getInstancePtr()->postShapeMel(transform);
		} // else RiArchiveRecord( RI_COMMENT, " Shapes Ignored !!" );

	}
	//void Renderer::exportOneGeometry_Mesh(
	//	const liqRibMeshData *meshdata,  
	//	const structJob &currentJob
	//	)
	//{
	//	//
	//	//mesh data begin
	//	//
	//	// Each loop has one polygon, so we just want an array of 1's of
	//	// the correct size. Stack version.
	//	//vector< RtInt > nloops( numFaces, 1 );
	//	// Alternatively (heap version):
	//	boost::scoped_array< RtInt > nloops( new RtInt[ meshdata->getNumFaces() ] );
	//	std::fill( nloops.get(), nloops.get() + meshdata->getNumFaces(), ( RtInt )1 );

	//	unsigned numTokens( meshdata->tokenPointerArray.size() );
	//	boost::scoped_array< RtToken > tokenArray( new RtToken[ numTokens ] );
	//	boost::scoped_array< RtPointer > pointerArray( new RtPointer[ numTokens ] );
	//	assignTokenArraysV( meshdata->tokenPointerArray, tokenArray.get(), pointerArray.get() );

	//	RiPointsGeneralPolygonsV( meshdata->getNumFaces(),
	//		&nloops[ 0 ],
	//		meshdata->getNverts().get(),
	//		meshdata->getVerts().get(),
	//		numTokens,
	//		tokenArray.get(),
	//		pointerArray.get() );
	//	//mesh data end
	//}
	//
	void Renderer::ribPrologue_comment(const char* liqversion, 
		const char* scenename, const char* user, const time_t &now)
	{
		CM_TRACE_FUNC("Renderer::ribPrologue_comment("<<liqversion<<","<<scenename<<","<<user<<",time)");
		
		//[refactor 5] begin 
		// general info for traceability
		//
		RiArchiveRecord( RI_COMMENT, "    Generated by Liquid v%s", liqversion );
		RiArchiveRecord( RI_COMMENT, "    Scene : %s", scenename );

		if( user )
			RiArchiveRecord( RI_COMMENT, "    User  : %s", user );

		RiArchiveRecord( RI_COMMENT, "    Time  : %s", ctime(&now) );
		//[refactor 5] end 
	}
	//
	MStatus Renderer::ribPrologue_begin(const structJob& currentJob___)
	{
		CM_TRACE_FUNC("Renderer::ribPrologue_begin("<<currentJob___.name.asChar()<<")");

#ifndef RENDER_PIPE
		liquidMessage( "Beginning RIB output to '" + currentJob___.ribFileName + "'", messageInfo );
		RiBegin_liq( const_cast< RtToken >( currentJob___.ribFileName.asChar() ) );
		liqglo.m_ribFileOpen = true;
#else//RENDER_PIPE
		liqglo___.liqglo_ribFP = fopen( currentJob___.ribFileName.asChar(), "w" );
		if( liqglo___.liqglo_ribFP ) 
		{
			RtInt ribFD = fileno( liqglo___.liqglo_ribFP );
			RiOption( ( RtToken )"rib", ( RtToken )"pipe", &ribFD, RI_NULL );
		} 
		else
		{
			liquidMessage( "Error opening RIB -- writing to stdout.\n", messageError );
		}

		liquidMessage( "Beginning RI output directly to renderer", messageInfo );
		RiBegin_liq( RI_NULL );
#endif//RENDER_PIPE
		return MS::kSuccess;
	}
	MStatus Renderer::ribPrologue_end(const structJob& currentJob)
	{
		CM_TRACE_FUNC("Renderer::ribPrologue_end("<<currentJob.name.asChar()<<")");
		
		RiEnd();
		liqglo.m_ribFileOpen = false;

		return MS::kSuccess;
	}
	//
	MStatus Renderer::ribPrologue_options(const structJob& currentJob)
	{
		CM_TRACE_FUNC("Renderer::ribPrologue_options("<<currentJob.name.asChar()<<")");

		LIQDEBUGPRINTF( "-> beginning to write prologue\n" );


		// set any rib options
		//
		writeStatisticsOptions();

		//limits options
		writeLimitsOptions();

		// set search paths
		ribPrologue_writeSearthPath();

		//RiOrientation( RI_RH ); // Right-hand coordinates
		if( currentJob.pass == rpShadowMap ) 
		{
			//[refactor 7] 
			tShadowRibWriterMgr::ribPrologue_samples(currentJob.samples, currentJob.samples);
			tShadowRibWriterMgr::ribPrologue_shadingrate(currentJob.shadingRateFactor);
			// Need to use Box filter for deep shadows.			
			tShadowRibWriterMgr::ribPrologue_filter( pfBoxFilter, 1, 1);
			
			RtString option;
			if ( currentJob.shadowType == stDeep ) 
				option = "deepshadow";
			else                                 
				option = "shadow";
			tShadowRibWriterMgr::ribPrologue_pass(option);
			//[refactor 7] 
		} 
		else 
		{
			tHeroRibWriterMgr::ribPrologue_hider(liqglo.liqglo_hider);
			//samples 
			tHeroRibWriterMgr::ribPrologue_samples( liqglo.pixelSamples, liqglo.pixelSamples );
			tHeroRibWriterMgr::ribPrologue_shadingrate( liqglo.shadingRate );
			//filter options
			if( liqglo.m_rFilterX > 1 || liqglo.m_rFilterY > 1 ) 
			{
				tHeroRibWriterMgr::ribPrologue_filter(liqglo.m_rFilter, liqglo.m_rFilterX, liqglo.m_rFilterY);
			}
			tHeroRibWriterMgr::ribPrologue_pass("beauty");
		}

		// RAYTRACING OPTIONS
		if( liqglo.liquidRenderer.supports_RAYTRACE && liqglo.rt_useRayTracing ) 
		{
			RiArchiveRecord( RI_COMMENT, "Ray Tracing : ON" );
			RiOption( "trace",   "int maxdepth",                ( RtPointer ) &liqglo.rt_traceMaxDepth,            RI_NULL );
#if defined ( DELIGHT ) || defined ( PRMAN ) || defined ( GENERIC_RIBLIB )
			RiOption( "trace",   "float specularthreshold",     ( RtPointer ) &liqglo.rt_traceSpecularThreshold, RI_NULL );
			RiOption( "trace",   "int continuationbydefault",   ( RtPointer ) &liqglo.rt_traceRayContinuation,   RI_NULL );
			RiOption( "limits",  "int geocachememory",          ( RtPointer ) &liqglo.rt_traceCacheMemory,       RI_NULL );
			RiOption( "user",    "float traceBreadthFactor",    ( RtPointer ) &liqglo.rt_traceBreadthFactor,     RI_NULL );
			RiOption( "user",    "float traceDepthFactor",      ( RtPointer ) &liqglo.rt_traceDepthFactor,       RI_NULL );
#endif
		} 
		else 
		{
			if( !liqglo.liquidRenderer.supports_RAYTRACE ) 
				RiArchiveRecord( RI_COMMENT, "Ray Tracing : NOT SUPPORTED" );
			else 
			{
				RiArchiveRecord( RI_COMMENT, "Ray Tracing : OFF" );
				RtInt maxDepth = 0;
				RiOption( "trace",   "int maxdepth",                ( RtPointer ) &maxDepth,                  RI_NULL );
			}
		}

		// CUSTOM OPTIONS
		MFnDependencyNode globalsNode( liqglo.rGlobalObj );
		MPlug prePostplug( globalsNode.findPlug( "preFrameBeginMel" ) );
		MString melcommand( prePostplug.asString() );
		if( liqglo.m_preFrameRIB != "" || melcommand.length() )
		{
			RiArchiveRecord(RI_COMMENT,  " Pre-FrameBegin RIB from liquid globals" );
			MGlobal::executeCommand( melcommand );
			RiArchiveRecord(RI_VERBATIM, ( char* )liqglo.m_preFrameRIB.asChar() );
			RiArchiveRecord(RI_VERBATIM, "\n");
		}
		if( liqglo.m_bakeNonRasterOrient || liqglo.m_bakeNoCullHidden || liqglo.m_bakeNoCullBackface ) 
		{
			RiArchiveRecord( RI_COMMENT, "Bake Attributes" );
			RtInt zero( 0 );
			if( liqglo.m_bakeNonRasterOrient )
				RiAttribute( "dice","int rasterorient", &zero, RI_NULL );
			if( liqglo.m_bakeNoCullBackface )
				RiAttribute( "cull","int backfacing", &zero, RI_NULL );
			if( liqglo.m_bakeNoCullHidden )
				RiAttribute( "cull","int hidden", &zero, RI_NULL );
		}
		if ( liqglo.liqglo_doMotion ) 
			RiGeometricApproximation( "motionfactor", liqglo.liqglo_motionFactor );
		return MStatus::kSuccess;
	}
	void Renderer::ribPrologue_writeSearthPath()
	{
		CM_TRACE_FUNC("Renderer::ribPrologue_writeSearthPath()");
	
		//[refactor 6] begin from ribPrologue()
		if ( liqglo.m_dirmaps.length() )
		{
			using namespace std;
			using namespace boost;

			const string str( liqglo.m_dirmaps.asChar() );
			stringstream ss;
			vector< string > names;
			typedef tokenizer< char_separator< char > > tokenizer;
			char_separator< char > sep("{ }");
			tokenizer tokens(str, sep);
			for ( tokenizer::iterator tok_iter = tokens.begin(); tok_iter != tokens.end(); ++tok_iter )
				names.push_back( *tok_iter );
			for ( unsigned i( 0 ); i < names.size(); i+= 3 )
			{
				// names.at(i+2)  - zone
				// names.at(i)    - from
				// names.at(i+1)  - to
				// [\"UNC\" \"/from_path/\" \"//comp/to_path/\"]
#ifdef GENERIC_RIBLIB
				ss << "[\\\"" << names.at(i+2) << "\\\" \\\"" << names.at(i) << "\\\" \\\"" << names.at(i+1) << "\\\"] ";
#else
				ss << "[\"" << names.at(i+2) << "\" \"" << names.at(i) << "\" \"" << names.at(i+1) << "\"] ";
#endif
			}
			printf("%s\n", ss.str().c_str());
			string dirmapsPath ( ss.str() );
			RtString list = const_cast< char* > ( dirmapsPath.c_str() );
			RiOption( "searchpath", "dirmap", &list, RI_NULL );
		}
		RtString list = const_cast< char* > ( liqglo.liqglo_shaderPath.asChar() );
		RiOption( "searchpath", "shader", &list, RI_NULL );

		MString texturePath = liqglo.liqglo_texturePath + ":" + liquidSanitizeSearchPath( liqglo.liqglo_textureDir );
		list = const_cast< char* > ( texturePath.asChar() );
		RiOption( "searchpath", "texture", &list, RI_NULL );

		MString archivePath = liqglo.liqglo_archivePath + ":" + liquidSanitizeSearchPath( liqglo.liqglo_ribDir );
		list = const_cast< char* > ( archivePath.asChar() );
		RiOption( "searchpath", "archive", &list, RI_NULL );

		list = const_cast< char* > ( liqglo.liqglo_proceduralPath.asChar() );
		RiOption( "searchpath", "procedural", &list, RI_NULL );

		// if rendering to the renderview, add a path to the liqmaya display driver
		if( liqglo.m_renderView ) 
		{
			MString home( getenv( "LIQUIDHOME" ) );
			MString displaySearchPath;
			MString rendererDisplays = liquidSanitizeSearchPath ( liqglo.liquidRenderer.renderHome );
			MString liquidDisplays = liquidSanitizeSearchPath ( home ) + "/displayDrivers/" + liqglo.liquidRenderer.renderName + "/";

			if( (liqglo.liquidRenderer.renderName == MString("Pixie")) || (liqglo.liquidRenderer.renderName == MString("Air")) || (liqglo.liquidRenderer.renderName == MString("3Delight")) )
				rendererDisplays += "/displays";
			else 
				rendererDisplays += "/etc";
			displaySearchPath = ".:@:" + rendererDisplays + ":" + liquidDisplays;


			list = const_cast< char* > ( displaySearchPath.asChar() );
			RiArchiveRecord( RI_VERBATIM, "Option \"searchpath\" \"display\" [\"%s\"]\n", list );
		}
		//[refactor 6] end from ribPrologue()
	}
	//
	MStatus Renderer::framePrologue(long lframe, const structJob &currentJob)
	{
		CM_TRACE_FUNC("Renderer::framePrologue("<<lframe<<","<<currentJob.name.asChar()<<")");

		RiFrameBegin( lframe );

		if( currentJob.pass == rpShadowMap )
		{
			tShadowRibWriterMgr::framePrologue_display(currentJob);
		}//if( liqglo.liqglo_currentJob.isShadow )
		else
		{
			tHeroRibWriterMgr::framePrologue_display(currentJob);
		}

		this->framePrologue_camera(lframe, currentJob);

		return MStatus::kSuccess;
	}
	MStatus Renderer::framePrologue_camera(long lframe, const structJob &currentJob)
	{
		CM_TRACE_FUNC("Renderer::framePrologue_camera("<<lframe<<","<<currentJob.name.asChar()<<")");

		//tRibCameraMgr::framePrologue_camera(lframe, currentJob);
		//refactor 20 begin from liqRibTranslator.framePrologue()
		LIQDEBUGPRINTF( "-> Setting Resolution\n" );
		// moved to "refactor 14-1"(tShadowRibWriterMgr::framePrologue_display()/tHeroRibWriterMgr::framePrologue_display())
		// 	// philippe : Rotated Camera Case
		// 	if( /*liqglo.liqglo_*/currentJob.isShadow == false && liqglo.liqglo_rotateCamera  == true ) 
		// 		RiFormat( /*liqglo.liqglo_*/currentJob.height, /*liqglo.liqglo_*/currentJob.width, /*liqglo.liqglo_*/currentJob.aspectRatio );
		// 	else 
		// 		RiFormat( /*liqglo.liqglo_*/currentJob.width, /*liqglo.liqglo_*/currentJob.height, /*liqglo.liqglo_*/currentJob.aspectRatio );
		//omitted in r773
		// 	if( /*liqglo.liqglo_*/currentJob.camera[0].isOrtho )
		// 	{
		// 		RtFloat frameWidth, frameHeight;
		// 		// the whole frame width has to be scaled according to the UI Unit
		// 		frameWidth  = /*liqglo.liqglo_*/currentJob.camera[0].orthoWidth  * 0.5 ;
		// 		frameHeight = /*liqglo.liqglo_*/currentJob.camera[0].orthoHeight * 0.5 ;
		// 		RiProjection( "orthographic", RI_NULL );
		// 
		// 		// if we are describing a shadow map camera,
		// 		// we need to set the screenwindow to the default,
		// 		// as shadow maps are always square.
		// 		if( /*liqglo.liqglo_*/currentJob.isShadow == true )
		// 			RiScreenWindow( -frameWidth, frameWidth, -frameHeight, frameHeight );
		// 		else
		// 			RiScreenWindow( -frameWidth, frameWidth, -frameHeight, frameHeight );
		// 	} 
		// 	else 
		// 	{
		// 		RtFloat fieldOfView = /*liqglo.liqglo_*/currentJob.camera[0].hFOV * 180.0 / M_PI ;
		// 		if( /*liqglo.liqglo_*/currentJob.isShadow && /*liqglo.liqglo_*/currentJob.isPoint ) 
		// 			fieldOfView = /*liqglo.liqglo_*/currentJob.camera[0].hFOV;
		// 
		// 		RiProjection( "perspective", RI_FOV, &fieldOfView, RI_NULL );
		// 
		// 		// if we are describing a shadow map camera,
		// 		// we need to set the screenwindow to the default,
		// 		// as shadow maps are always square.
		// 		if( /*liqglo.liqglo_*/currentJob.isShadow == false )
		// 		{
		// 			double ratio = (double)/*liqglo.liqglo_*/currentJob.width / (double)/*liqglo.liqglo_*/currentJob.height;
		// 			double left, right, bottom, top;
		// 			if( ratio <= 0 ) 
		// 			{
		// 				left    = -1 + currentJob.camera[0].horizontalFilmOffset;
		// 				right   =  1 + currentJob.camera[0].horizontalFilmOffset;
		// 				bottom  = -1 / ratio + currentJob.camera[0].verticalFilmOffset;
		// 				top     =  1 / ratio + currentJob.camera[0].verticalFilmOffset;
		// 			} 
		// 			else 
		// 			{
		// 				left    = -ratio + currentJob.camera[0].horizontalFilmOffset;
		// 				right   =  ratio + currentJob.camera[0].horizontalFilmOffset;
		// 				bottom  = -1 + currentJob.camera[0].verticalFilmOffset;
		// 				top     =  1 + currentJob.camera[0].verticalFilmOffset;
		// 			}
		// 			RiScreenWindow( left, right, bottom, top );
		// 		} else 
		// 			RiScreenWindow( -1.0, 1.0, -1.0, 1.0 );
		// 	}
		// 
		// 	RiClipping( currentJob.camera[0].neardb, currentJob.camera[0].fardb );
		// 	if( liqglo.doDof && !currentJob.isShadow ) 
		// 		RiDepthOfField( currentJob.camera[0].fStop, currentJob.camera[0].focalLength, currentJob.camera[0].focalDistance );

		// Set up for camera motion blur
		/* doCameraMotion = liqglo_currentJob.camera[0].motionBlur && liqglo_doMotion; */
		float frameOffset = 0;
		if( liqglo.doCameraMotion || liqglo.liqglo_doMotion || liqglo.liqglo_doDef ) 
		{
			switch( liqglo.shutterConfig ) 
			{
			case OPEN_ON_FRAME:
			default:
				if(liqglo.liqglo_relativeMotion)
					RiShutter( 0, currentJob.camera[0].shutter );
				else
					RiShutter( lframe, lframe + currentJob.camera[0].shutter );
				frameOffset = lframe;
				break;
			case CENTER_ON_FRAME:
				if(liqglo.liqglo_relativeMotion)
					RiShutter(  - ( currentJob.camera[0].shutter * 0.5 ),  ( currentJob.camera[0].shutter * 0.5 ) );
				else
					RiShutter( ( lframe - ( currentJob.camera[0].shutter * 0.5 ) ), ( lframe + ( currentJob.camera[0].shutter * 0.5 ) ) );
				frameOffset = lframe - ( currentJob.camera[0].shutter * 0.5 );
				break;
			case CENTER_BETWEEN_FRAMES:
				if(liqglo.liqglo_relativeMotion)
					RiShutter( + ( 0.5 * ( 1 - currentJob.camera[0].shutter ) ), + currentJob.camera[0].shutter + ( 0.5 * ( 1 - currentJob.camera[0].shutter ) ) );
				else
					RiShutter( lframe + ( 0.5 * ( 1 - currentJob.camera[0].shutter ) ), lframe + currentJob.camera[0].shutter + ( 0.5 * ( 1 -currentJob.camera[0].shutter ) ) );
				frameOffset = lframe + ( 0.5 * ( 1 - currentJob.camera[0].shutter ) );
				break;
			case CLOSE_ON_NEXT_FRAME:
				if(liqglo.liqglo_relativeMotion)
					RiShutter( + ( 1 - currentJob.camera[0].shutter ),  1 );
				else
					RiShutter( lframe + ( 1 - currentJob.camera[0].shutter ), lframe + 1 );
				frameOffset = lframe + ( 1 - currentJob.camera[0].shutter );
				break;
			}
		} 
		else 
		{
			if(liqglo.liqglo_relativeMotion)
				RiShutter( 0, 0);
			else
				RiShutter( lframe, lframe );
			frameOffset = lframe;
		}
		// relative motion
		if(liqglo.liqglo_relativeMotion)    
			RiOption( "shutter", "offset", &frameOffset, RI_NULL);

#ifdef DELIGHT
		RiOption( "shutter", "efficiency", &liqglo.liqglo_shutterEfficiency, RI_NULL );
#endif
		if( currentJob.gotJobOptions ) 
			RiArchiveRecord( RI_COMMENT, "jobOptions: \n%s", currentJob.jobOptions.asChar() );

		if( ( liqglo.liqglo_preRibBox.length() > 0 ) && currentJob.pass != rpShadowMap ) 
			for ( unsigned ii(0); ii < liqglo.liqglo_preRibBox.length(); ii++ ) 
				RiArchiveRecord( RI_COMMENT, "Additional Rib:\n%s", liqglo.liqglo_preRibBox[ii].asChar() );

		if( ( liqglo.liqglo_preReadArchive.length() > 0 ) && currentJob.pass != rpShadowMap ) 
			for ( unsigned ii(0); ii < liqglo.liqglo_preReadArchive.length(); ii++ ) 
				RiArchiveRecord( RI_COMMENT, "Read Archive Data: \nReadArchive \"%s\"", liqglo.liqglo_preReadArchive[ii].asChar() );

		if( ( liqglo.liqglo_preRibBoxShadow.length() > 0 ) && currentJob.pass != rpShadowMap ) 
			for ( unsigned ii(0); ii < liqglo.liqglo_preRibBoxShadow.length(); ii++ ) 
				RiArchiveRecord( RI_COMMENT, "Additional Rib:\n%s", liqglo.liqglo_preRibBoxShadow[ii].asChar() );

		if( ( liqglo.liqglo_preReadArchiveShadow.length() > 0 ) && currentJob.pass == rpShadowMap ) 
			for ( unsigned ii(0); ii < liqglo.liqglo_preReadArchiveShadow.length(); ii++ ) 
				RiArchiveRecord( RI_COMMENT, "Read Archive Data: \nReadArchive \"%s\"", liqglo.liqglo_preReadArchiveShadow[ii].asChar() );
		//omitted in r773
		// 	RiArchiveRecord( RI_COMMENT, "camera %s's transform", currentJob.camera[0].name.asChar());
		// 	// if we motion-blur the cam, open the motion block
		// 	//
		// 	if( liqglo.doCameraMotion && ( !currentJob.isShadow || currentJob.deepShadows) ) 
		// 		if(liqglo.liqglo_relativeMotion)
		// 			RiMotionBeginV( liqglo.liqglo_motionSamples, liqglo.liqglo_sampleTimesOffsets );
		// 		else
		// 			RiMotionBeginV( liqglo.liqglo_motionSamples, liqglo.liqglo_sampleTimes );
		// 
		// 	// write the camera transform
		// 	//
		// 	RtMatrix cameraMatrix;
		// 	currentJob.camera[0].mat.get( cameraMatrix );
		// 	RiTransform( cameraMatrix );
		// 
		// 	// if we motion-blur the cam, write the subsequent motion samples and close the motion block
		// 	//
		// 	if( liqglo.doCameraMotion && ( !currentJob.isShadow || currentJob.deepShadows ) ) 
		// 	{
		// 		int mm = 1;
		// 		while ( mm < liqglo.liqglo_motionSamples ) 
		// 		{
		// 			currentJob.camera[mm].mat.get( cameraMatrix );
		// 			RiTransform( cameraMatrix );
		// 			++mm;
		// 		}
		// 		RiMotionEnd();
		// 	}
		//refactor 20 end

		return MStatus::kSuccess;
	}
	//
	MStatus Renderer::frameEpilogue(const long scanTime)
	{
		CM_TRACE_FUNC("Renderer::frameEpilogue("<<scanTime<<")");

		if( !liqglo.m_exportReadArchive ) 
			RiFrameEnd();
		
		return MStatus::kSuccess;
	}
	//
	MStatus Renderer::doTextures(const std::vector<structJob> &txtList_)
	{ 
		CM_TRACE_FUNC(boost::format("rm::Renderer::doTextures(txtList.size()=%d)") % txtList_.size());

		//[refactor][1.17 ]
		std::vector<structJob>::const_iterator iter = txtList_.begin();
		while ( iter != txtList_.end() ) 
		{
			liquidMessage( "Making textures '" + iter->imageName + "'", messageInfo );
#ifdef _WIN32
			if( liqglo.liquidRenderer.renderName == MString("PRMan") )
			{
				liqProcessLauncher::execute( iter->renderName, 
					(" -progress \"" + iter->ribFileName + "\""), 
					liqglo.liqglo_projectDir, true 
					);
			}
			else if( liqglo.liquidRenderer.renderName == MString("3Delight") ){
				liqProcessLauncher::execute( iter->renderName, 
					(" -progress " + iter->ribFileName + ""), 
					liqglo.liqglo_projectDir, true 
					);
			}
			//else if( liqglo.liquidRenderer.renderName == MString("Aqsis") ){...}
			//else if( liqglo.liquidRenderer.renderName == MString("Pixie") ){...}
#else
			liqProcessLauncher::execute( iter->renderName, 
				(" -progress " + iter->ribFileName), 
				liqglo.liqglo_projectDir, true 
				);
#endif
			++iter;
		}
		//[refactor][1.17 ]
		return MStatus::kSuccess;
	}
	MStatus Renderer::doShadows(const std::vector<structJob> &shadowList_)
	{
		CM_TRACE_FUNC(boost::format("rm::Renderer::doShadows(shadowList_.size()=%d)")%shadowList_.size());

		//[refactor][1.18 ]
		liquidMessage( "Rendering shadow maps... ", messageInfo );
		std::vector<structJob>::const_iterator iter = shadowList_.begin();
		while ( iter != shadowList_.end() ) 
		{
			if( iter->skip ) 
			{
				liquidMessage( "    - skipping '" + iter->ribFileName + "'", messageInfo );
				++iter;
				continue;
			}
			liquidMessage( "    + '" + iter->ribFileName + "'", messageInfo );
			if ( 
				!liqProcessLauncher::execute(  liqglo.liquidRenderer.renderCommand,  liqglo.liquidRenderer.renderCmdFlags + " " +
#ifdef _WIN32
				"\"" + iter->ribFileName + "\"", 
#else
				iter->ribFileName, 
#endif
				liqglo.liqglo_projectDir, true 
				) 
			) break;
			++iter;
		} // while ( iter != shadowList_.end() )
		//[refactor][1.18 ]
		return MStatus::kSuccess;
	}
	MStatus Renderer::doRenderView()
	{
		CM_TRACE_FUNC("Renderer::doRenderView()");

		//[refactor][1.21 ]
		MString displayCmd = "liquidRenderView -c " + liqglo.liqglo_renderCamera;
		displayCmd += " -l " + MString( ( liqglo.m_renderViewLocal )? "1":"0" );
		displayCmd += " -port ";
		displayCmd += (int)liqglo.m_renderViewPort;
		displayCmd += " -timeout ";
		displayCmd += (int)liqglo.m_renderViewTimeOut;
		if ( liqglo.m_renderViewCrop ) 
			displayCmd += " -doRegion";
		displayCmd += ";liquidSaveRenderViewImage();";
		MGlobal::executeCommand( displayCmd );
		//[refactor][1.21 ]

		return MStatus::kSuccess;
	}
	MStatus Renderer::renderAll_local(const MString& ribFileName)
	{
		CM_TRACE_FUNC(boost::format("rm::Renderer::renderAll_local(%s)")%ribFileName.asChar());

		//[refactor][1.20 ]
		liquidMessage( "    + '" + ribFileName + "'", messageInfo );
		liqProcessLauncher::execute( 
			liqglo.liquidRenderer.renderCommand, 
#ifdef _WIN32
			" "+liqglo.liqglo_rifParams+" "+ liqglo.liquidRenderer.renderCmdFlags + " \"" + ribFileName + "\"", 
			"\"" + liqglo.liqglo_projectDir + "\"", 
#else
			" "+liqglo.liqglo_rifParams+" "+ liqglo.liquidRenderer.renderCmdFlags + " " + ribFileName, 
			liqglo.liqglo_projectDir,
#endif
			false
		);
		//[refactor][1.20 ]
		/*  philippe: here we launch the liquidRenderView command which will listen to the liqmaya display driver
		to display buckets in the renderview.
		*/
		if( liqglo.m_renderView ) 
		{
			doRenderView();
		}
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

	void Renderer::exportLightLinks(
		const structJob &currentJob__,
		const liqRibNodePtr mesh, 
		const MStringArray& lightedByWhichLightShapes)
	{
		CM_TRACE_FUNC("Renderer::exportLightLinks("<<currentJob__.name.asChar()<<","<<mesh->name.asChar()<<",lightedByWhichLightShapes.size="<<lightedByWhichLightShapes.length()<<")");

		if(lightedByWhichLightShapes.length() == 0){
			_liqRIBMsg((mesh->name+" is not lighted.").asChar());
			return;
		}

		boost::shared_ptr< liqRibHT > &htable = liqRibTranslator::getInstancePtr()->htable;
		for ( RNMAP::iterator rniter( htable->RibNodeMap.begin() ); rniter != htable->RibNodeMap.end(); rniter++ ) 
		{
			liqRibNodePtr   ribNode = (*rniter).second;

			if( ribNode->object(0)->ignore || ribNode->object(0)->type != MRT_Light )
				continue;
			
			if(-1 != find_first_of(ribNode->name.asChar(), lightedByWhichLightShapes) ){
				RiIlluminate( ribNode->object(0)->lightHandle(), 1 );
			}else{
				RiIlluminate( ribNode->object(0)->lightHandle(), 0 );
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

		MStatus status;

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


	}
	void Renderer::HeroPassEnd(const structJob &currentJob)
	{
		CM_TRACE_FUNC("Renderer::HeroPassEnd("<<currentJob.name.asChar()<<")");



		//
		if( currentJob.skip ) 
		{
			//[refactor][1.19 ]
			liquidMessage( "    - skipping '" + currentJob.ribFileName + "'", messageInfo );
			//[refactor][1.19 ]
		}else {
			renderAll_local(currentJob.ribFileName);
		}
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
		//
		//  create the read-archive shadow files
		//
		//[refactor][1.9.2.4 begin] from ribOutput(baseShadowName__)
#ifndef RENDER_PIPE
		liquidMessage( "Beginning RIB output to '" + baseShadowName__ + "'", messageInfo );
		RiBegin_liq( const_cast< RtToken >( baseShadowName__.asChar() ) );
		liqglo.m_ribFileOpen = true;
#else
		liqglo__.liqglo_ribFP = fopen( baseShadowName.asChar(), "w" );
		if( liqglo__.liqglo_ribFP ) {
			LIQDEBUGPRINTF( "-> setting pipe option\n" );
			RtInt ribFD( fileno( liqglo__.liqglo_ribFP ) );
			RiOption( "rib", "pipe", &ribFD, RI_NULL );
		}
		else
		{
			liquidMessage( "Error opening RIB -- writing to stdout.\n", messageError );
		}
		liquidMessage( "Beginning RI output directly to renderer", messageInfo );
		RiBegin_liq( RI_NULL );
#endif
		//[refactor][1.9.2.4 end] from ribOutput()
	}
	void Renderer::BaseShadowEnd(const structJob &currentJob___)
	{
		CM_TRACE_FUNC("Renderer::BaseShadowEnd("<<currentJob___.name.asChar()<<")");

		RiEnd();
		liqglo.m_ribFileOpen = false;
#ifdef RENDER_PIPE  
		fclose( liqglo.liqglo_ribFP );
#endif
		liqglo.liqglo_ribFP = NULL;
	}

	bool Renderer::isShadowPassReady(const structJob &currentJob___)
	{
		CM_TRACE_FUNC("Renderer::isShadowPassReady("<<currentJob___.name.asChar()<<")");

		return true;
	}
	void Renderer::ShadowPassBegin(const structJob &currentJob___)
	{
		CM_TRACE_FUNC("Renderer::ShadowPassBegin("<<currentJob___.name.asChar()<<")");

		//move RiBegin() to ribPrologue_begin()
	}
	void Renderer::ShadowPassEnd(const structJob &currentJob___)
	{
		CM_TRACE_FUNC("Renderer::ShadowPassEnd("<<currentJob___.name.asChar()<<")");
		
		//move RiEnd() to ribPrologue_end()

		//------------------------------------------------------------
		//rendering
		this->renderAll_local(currentJob___.ribFileName);
		//------------------------------------------------------------
	}
	void Renderer::readBaseShadow(const structJob &currentJob___)
	{
		CM_TRACE_FUNC("Renderer::readBaseShadow("<<currentJob___.name.asChar()<<")");
		
		//MString realShadowName( liquidSanitizePath( liquidGetRelativePath( liqglo_relativeFileNames, baseShadowName, liqglo_projectDir ) ) );
		MString     baseShadowName__(getBaseShadowName(currentJob___));
		//[refactor][1.9.2.8 begin] from ribOutput()
		RiArchiveRecord( RI_COMMENT, "Read Archive Data:\n" );
		RiReadArchive( const_cast< RtToken >( baseShadowName__.asChar() ), NULL, RI_NULL );
		//[refactor][1.9.2.8 end] from ribOutput()
	}
	//
	void Renderer::oneObjectBlock_reference_attribute_begin(
		const liqRibNodePtr &ribNode,
		const structJob &currentJob )
	{
		CM_TRACE_FUNC("Renderer::oneObjectBlock_reference_attribute_begin("<<ribNode->name.asChar()<<","<<currentJob.name.asChar()<<")");

		if( liqglo.m_outputComments ) 
			RiArchiveRecord( RI_COMMENT, "Name: %s", ribNode->name.asChar(), RI_NULL );

		RiAttributeBegin();
	}
	void Renderer::oneObjectBlock_reference_attribute_end(
		const liqRibNodePtr &ribNode,
		const structJob &currentJob )
	{
		CM_TRACE_FUNC("Renderer::oneObjectBlock_reference_attribute_end("<<ribNode->name.asChar()<<","<<currentJob.name.asChar()<<")");

		RiAttributeEnd();
	}
	void Renderer::oneObjectBlock_reference_attribute_block0(
		const liqRibNodePtr &ribNode,
		const structJob &currentJob )
	{
		CM_TRACE_FUNC("Renderer::oneObjectBlock_reference_attribute_block0("<<ribNode->name.asChar()<<","<<currentJob.name.asChar()<<")");

		RiAttribute( "identifier", "name", &getLiquidRibName( ribNode->name.asChar() ), RI_NULL );

		if( !ribNode->grouping.membership.empty() ) 
		{
			RtString members( const_cast< char* >( ribNode->grouping.membership.c_str() ) );
			RiAttribute( "grouping", "membership", &members, RI_NULL );
		}

		if ( !liqRibTranslator::getInstancePtr()->m_skipShadingAttributes )
		if( ribNode->shading.matte || ribNode->mayaMatteMode ) 
			RiMatte( RI_TRUE );

		// If this is a single sided object, then turn that on (RMan default is Sides 2)
		if( !ribNode->doubleSided ) 
			RiSides( 1 );

		if( ribNode->reversedNormals ) 
			RiReverseOrientation();
	}
	void Renderer::oneObjectBlock_reference_attribute_block1(
		const liqRibNodePtr &ribNode,
		const structJob &currentJob )
	{
		CM_TRACE_FUNC("Renderer::oneObjectBlock_reference_attribute_block1("<<ribNode->name.asChar()<<","<<currentJob.name.asChar()<<")");

		// displacement bounds £¨ommited in r773£©
		//liqRibTranslator::getInstancePtr()->displacementBounds(ribNode);

		LIQDEBUGPRINTF( "-> writing node attributes\n" );
		// if the node's shading rate == -1,
		// it means it hasn't been overriden by a liqShadingRate attribute.
		// No need to output it then.
		if ( !liqRibTranslator::getInstancePtr()->m_skipShadingAttributes )//!m_skipShadingAttributes
		if( ribNode->shading.shadingRate > 0 )
			RiShadingRate ( ribNode->shading.shadingRate );

		if( currentJob.pass == rpShadowMap ) 
		{
			liqRibTranslator::getInstancePtr()->objectShadowAttribute(ribNode);
		}else{
			objectNonShadowAttribute(ribNode);
		}
	}
	void Renderer::logFrame(const char* msg)
	{
		CM_TRACE_FUNC("Renderer::logFrame("<<msg<<")");

		RiArchiveRecord( RI_COMMENT, "\t\t\t\t[DEBUG] %s", msg );
	}
	void Renderer::oneObjectBlock_reference_attribute_block2_writeShader_RibBox(const char* msg)
	{
		CM_TRACE_FUNC("Renderer::oneObjectBlock_reference_attribute_block2_writeShader_RibBox("<<msg<<")");

		RiArchiveRecord( RI_VERBATIM, ( char* )msg );
		RiArchiveRecord( RI_VERBATIM, "\n" );
	}
	void Renderer::oneObjectBlock_reference_attribute_block2_writeShader_RegularShader(
		const liqRibNodePtr &ribNode__,
		const structJob &currentJob 
		)
	{
		CM_TRACE_FUNC("Renderer::oneObjectBlock_reference_attribute_block2_writeShader_RegularShader("<<ribNode__->name.asChar()<<","<<currentJob.name.asChar()<<")");

		//liqShader& currentShader( liqGetShader( ribNode__->assignedShader.object() ) );
		liqShader& currentShader = liqShaderFactory::instance().getShader( ribNode__->assignedShader.object() );

		F1(ribNode__, currentShader);

		liqRIBMsg("[2] liqglo_currentJob.isShadow=%d, currentShader.outputInShadow=%d", currentJob.pass, currentShader.outputInShadow );
		// per shader shadow pass override
		if( currentJob.pass != rpShadowMap || currentShader.outputInShadow )
		{
			//currentShader.write();//use ShadingGroup file reference(e.g. *.erapi/*.rmsg) instead.
		}
	}
	void Renderer::oneObjectBlock_reference_attribute_block2_writeShader_HasNoSurfaceShaderORIngoreSurface(
		const liqRibNodePtr &ribNode__, const MDagPath &path__, const bool m_shaderDebug
		)
	{
		CM_TRACE_FUNC("Renderer::oneObjectBlock_reference_attribute_block2_writeShader_HasNoSurfaceShaderORIngoreSurface("
			<<ribNode__->name.asChar()<<","<<path__.fullPathName().asChar()<<","<<m_shaderDebug<<")");

		F2(m_shaderDebug, ribNode__);

		if( !liqRibTranslator::getInstancePtr()->m_ignoreSurfaces ) 
		{
			MObject shadingGroup = ribNode__->assignedShadingGroup.object();
			MObject shader = ribNode__->findShader();
			//
			// here we check for regular shader nodes first
			// and assign default shader to shader-less nodes.
			//
			if( m_shaderDebug ) {
				liqRIBMsg("shader debug is turned on, so the surface is constant.");
				RiSurface( "constant", RI_NULL );
				LIQDEBUGPRINTF("add more constant parameters here. take /RMS-1.0.1-Maya2008/lib/shaders/src/mtorBlinn.sl as an example.(?)");
			}
			//else if( shader.apiType() == MFn::kLambert ){ 
			//	RiSurface( "matte", RI_NULL );
			//	LIQDEBUGPRINTF("add more lambert parameters here. take //RMS-1.0.1-Maya2008/lib/shaders/src/mtorLambert.sl as an example.");
			//}else if( shader.apiType() == MFn::kPhong ) {
			//	RiSurface( "plastic", RI_NULL );
			//	LIQDEBUGPRINTF("add more phong parameters here. take /RMS-1.0.1-Maya2008/lib/shaders/src/mtorPhong.sl as an example.");
			//}
			else if( path__.hasFn( MFn::kPfxHair ) ) 
			{
				// get some of the hair system parameters
				RtFloat translucence = 0, specularPower = 0;
				RtColor specularColor;

				liqRibTranslator::getInstancePtr()->getPfxHairData(path__, translucence, specularPower, specularColor);

				RiSurface(  "liquidpfxhair",
					"float specularpower", &specularPower,
					"float translucence",  &translucence,
					"color specularcolor", &specularColor,
					RI_NULL );
			} 
			else if( path__.hasFn( MFn::kPfxToon ) ) {
				RiSurface( "liquidpfxtoon", RI_NULL );
			}else if( path__.hasFn( MFn::kPfxGeometry ) ){
				RiSurface( "liquidpfx", RI_NULL );
			}else {
				//RiSurface( "plastic", RI_NULL );//ymesh-branch r773 use this
				//MFnDependencyNode shaderFn(shader);
				//RiSurface( const_cast<char*>(shaderFn.name().asChar()), RI_NULL );//use ShadingGroup file reference(e.g. *.erapi/*.rmsg) instead.
			}
		}

	}
	void Renderer::oneObjectBlock_reference_attribute_block3_ShadingGroup(
		const MString& meshname
		)
	{
		CM_TRACE_FUNC(boost::format("Renderer::oneObjectBlock_reference_attribute_block3_ShadingGroup(%s)")%meshname.asChar());
		MStringArray shadingGroupNode;
		{
			MString cmd = "listConnections -type \"shadingEngine\" -destination on (\""+meshname+"\" + \".instObjGroups\")";
			IfMErrorWarn(MGlobal::executeCommand( cmd, shadingGroupNode));
		}

		if(shadingGroupNode.length()==0){
			RiArchiveRecord( RI_COMMENT, "has no shadingEngine" );
			return;
		}
		if(shadingGroupNode[0].length()==0){
			RiArchiveRecord( RI_COMMENT, "shadingEngine[0] is empty" );
			return;
		}

		RSL::Visitor::outputShadingGroup(shadingGroupNode[0].asChar(), true);
	}
	void Renderer::oneObjectBlock_reference_attribute_block4_strategy(
		const liqRibNodePtr &ribNode)
	{
		CM_TRACE_FUNC("Renderer::oneObjectBlock_reference_attribute_block4_strategy("<<ribNode->name.asChar()<<")");
	
		//[refactor 34] begin
		liqString mode;
		// new prman 16.x shade attributes group 
		if ( ribNode->shade.strategy != liqRibNode::shade::SHADE_STRATEGY_GRIDS )
		{
			mode = "vpvolumes"; 
			RiAttribute( "shade", (liqToken) "strategy", &mode, RI_NULL );
		}
		if ( ribNode->shade.volumeIntersectionStrategy != liqRibNode::shade::SHADE_VOLUMEINTERSECTIONSTRATEGY_EXCLUSIVE )
		{
			mode = "additive"; 
			RiAttribute( "shade", (liqToken) "volumeintersectionstrategy", &mode, RI_NULL );
		}
		if ( ribNode->shade.volumeIntersectionPriority != 0.0 )
		{
			liqFloat value= ribNode->shade.volumeIntersectionPriority; 
			RiAttribute( "shade", (liqToken) "volumeintersectionpriority", &value, RI_NULL );
		}
		//[refactor 34] end
	}
	//
	bool Renderer::canExport()
	{
		CM_TRACE_FUNC("Renderer::canExport()");

		return true;
	}
	void Renderer::writeLimitsOptions()
	{
		if( liqglo.bucketSize != 0 )    
			RiOption( "limits", "bucketsize", ( liqPointer ) &liqglo.bucketSize, RI_NULL );
		if( liqglo.gridSize != 0 )      
			RiOption( "limits", "gridsize", ( liqPointer ) &liqglo.gridSize, RI_NULL );
		if( liqglo.textureMemory != 0 ) 
			RiOption( "limits", "texturememory", ( liqPointer) &liqglo.textureMemory, RI_NULL );
		if( liqglo.liquidRenderer.supports_EYESPLITS ) 
			RiOption( "limits", "eyesplits", ( liqPointer ) &liqglo.eyeSplits, RI_NULL );

		if(liqglo.liquidRenderer.renderName == MString("PRMan") || liqglo.liquidRenderer.renderName == MString("3Delight") )
		{
			liqColor othresholdC = {liqglo.othreshold[0], liqglo.othreshold[1], liqglo.othreshold[2]};
			RiOption( "limits", "othreshold", &othresholdC, RI_NULL );
			liqColor zthresholdC = {liqglo.zthreshold[0], liqglo.zthreshold[1], liqglo.zthreshold[2]};
			RiOption( "limits", "zthreshold", &zthresholdC, RI_NULL );
		}
	}
	void Renderer::writeStatisticsOptions()
	{
		if( liqglo.m_statistics != 0 )  
		{
			if( liqglo.m_statistics < 4 ) 
				RiOption( "statistics", "endofframe", ( liqPointer ) &liqglo.m_statistics, RI_NULL );
			else 
			{
				//cout <<"xml stats "<<endl;
				int stats = 1;
				RiOption( "statistics", "int endofframe", ( liqPointer ) &stats, RI_NULL );
				RiArchiveRecord( RI_VERBATIM, "Option \"statistics\" \"xmlfilename\" [\"%s\"]\n", const_cast< char* > ( liqglo.m_statisticsFile.asChar() ) );
			}
		}
	}
	MStatus Renderer::coordSysBlock__(const structJob &currentJob, boost::shared_ptr< liqRibHT > &htable)
	{
		CM_TRACE_FUNC("Renderer::coordSysBlock__("<<currentJob.name.asChar()<<")");

		MStatus returnStatus = MS::kSuccess;
		LIQDEBUGPRINTF( "-> Writing coordinate systems.\n" );
		RNMAP::iterator rniter;
		for ( rniter = htable->RibNodeMap.begin(); rniter != htable->RibNodeMap.end(); rniter++ ) 
		{
			LIQ_CHECK_CANCEL_REQUEST;
			liqRibNodePtr   ribNode = (*rniter).second;
			if( ribNode->object(0)->ignore || ribNode->object(0)->type != MRT_Coord ) 
				continue;
			if( liqglo.m_outputComments )
				RiArchiveRecord( RI_COMMENT, "Name: %s", ribNode->name.asChar(), RI_NULL );

			RiAttributeBegin();
			RiAttribute( "identifier", "name", &getLiquidRibName( ribNode->name.asChar() ), RI_NULL );

			liqMatrix ribMatrix;
			MMatrix matrix;
			MDagPath path;

			matrix = ribNode->object(0)->matrix( path.instanceNumber() );
			matrix.get( ribMatrix );
			if( liqglo.liqglo_relativeTransforms ) 
				RiConcatTransform( ribMatrix ); 
			else 
				RiTransform( ribMatrix );

			ribNode->object(0)->writeObject("", currentJob, false);
			ribNode->object(0)->written = 1;
			RiAttributeEnd();
		}
		return returnStatus;
	}
	MStatus Renderer::preGeometryMel(const MString &m_preGeomRIB)
	{
		CM_TRACE_FUNC("Renderer::preGeometryMel()");
		// Moritz: Added Pre-Geometry RIB for insertion right before any primitives
		MFnDependencyNode globalsNode( liqglo.rGlobalObj );
		MPlug prePostplug( globalsNode.findPlug( "preGeomMel" ) );
		MString melcommand( prePostplug.asString() );
		if( m_preGeomRIB != "" || melcommand.length() )
		{
			RiArchiveRecord( RI_COMMENT,  " Pre-Geometry RIB from liquid globals");
			MGlobal::executeCommand( melcommand );
			RiArchiveRecord( RI_VERBATIM, ( char* ) m_preGeomRIB.asChar() );
			RiArchiveRecord( RI_VERBATIM, "\n");
		}
		return MS::kSuccess;
	}
	//
	void Renderer::F1(const liqRibNodePtr &ribNode__,  liqShader &currentShader)
	{
		CM_TRACE_FUNC("Renderer::F1("<<ribNode__->name.asChar()<<","<<currentShader.getName()<<")");
		// Output color overrides or color ====>>>>>  WILL BE DONE IN liqShader::write -begin //r773 going to omit in r773
		if(ribNode__->shading.color.r != -1.0)
		{
			liqColor rColor;
			rColor[0] = ribNode__->shading.color[0];
			rColor[1] = ribNode__->shading.color[1];
			rColor[2] = ribNode__->shading.color[2];
			RiArchiveRecord( RI_COMMENT, "mark10" );
			RiColor( rColor );
		}
		else{
			RiArchiveRecord( RI_COMMENT, "mark11" );
			RiColor( currentShader.rmColor );
		}

		if(ribNode__->shading.opacity.r != -1.0)
		{
			liqColor rOpacity;
			rOpacity[0] = ribNode__->shading.opacity[0];
			rOpacity[1] = ribNode__->shading.opacity[1];
			rOpacity[2] = ribNode__->shading.opacity[2];
			RiOpacity( rOpacity );
		}
		else
			RiOpacity( currentShader.rmOpacity );
		//====>>>>>  WILL BE DONE IN liqShader::write  -end
	}

	////////////////////////////////////////
	void Renderer::F2(const bool m_shaderDebug, const liqRibNodePtr &ribNode__)
	{ 
		CM_TRACE_FUNC("Renderer::F2("<<m_shaderDebug<<","<<ribNode__->name.asChar()<<")");
		if( m_shaderDebug ) 
		{
			liqColor rColor,rOpacity;
			liqRIBMsg("shader debug is turned on, so the object is red.");
			// shader debug on !! everything goes red and opaque !!!
			rColor[0] = 1.;
			rColor[1] = 0.;
			rColor[2] = 0.;
			RiArchiveRecord( RI_COMMENT, "mark12" );
			RiColor( rColor );

			rOpacity[0] = 1.;
			rOpacity[1] = 1.;
			rOpacity[2] = 1.;
			RiOpacity( rOpacity );
		}else{
			if(ribNode__->shading.color.r != -1.0) 
			{
				liqColor rColor;
				rColor[0] = ribNode__->shading.color[0];
				rColor[1] = ribNode__->shading.color[1];
				rColor[2] = ribNode__->shading.color[2];
				RiArchiveRecord( RI_COMMENT, "mark13" );
				RiColor( rColor );
			} 
			else if( ( ribNode__->color.r != AS_NotEXist )&&( ribNode__->color.r != AS_ConnectedAsDes )) 
			{
				liqColor rColor;
				rColor[0] = ribNode__->color[0];
				rColor[1] = ribNode__->color[1];
				rColor[2] = ribNode__->color[2];
				RiArchiveRecord( RI_COMMENT, "mark14" );
				RiColor( rColor );
			}

			if(ribNode__->shading.opacity.r != -1.0) 
			{
				liqColor rOpacity;
				rOpacity[0] = ribNode__->shading.opacity[0];
				rOpacity[1] = ribNode__->shading.opacity[1];
				rOpacity[2] = ribNode__->shading.opacity[2];
				RiOpacity( rOpacity );
			} 
			else if( ( ribNode__->opacity.r != AS_NotEXist )&&( ribNode__->opacity.r != AS_ConnectedAsDes )) 
			{	
				liqColor rOpacity;
				rOpacity[0] = ribNode__->opacity[0];
				rOpacity[1] = ribNode__->opacity[1];
				rOpacity[2] = ribNode__->opacity[2];
				RiOpacity( rOpacity );
			}
		}
	}
	void Renderer::writeUserAttributes(const liqRibNode *ribNode__)
	{
		CM_TRACE_FUNC("Renderer::writeUserAttributes()");

		unsigned numTokens( ribNode__->tokenPointerMap.size() );
		if( numTokens ) {
		  boost::scoped_array< liqToken > tokenArray( new liqToken[ numTokens ] );
		  boost::scoped_array< liqPointer > pointerArray( new liqPointer[ numTokens ] );
		  // Can't use assignTokenArraysV() since we're dealing with std::map
		  std::size_t i( 0 );
		  for ( std::map<const std::string, liqTokenPointer >::const_iterator iter( ribNode__->tokenPointerMap.begin() ); iter != ribNode__->tokenPointerMap.end(); iter++, i++ ) 
		  {
		    tokenArray[ i ] = const_cast< liqString >( const_cast< liqTokenPointer* >( &( iter->second ) )->getDetailedTokenName().c_str() );
		    pointerArray[ i ] = const_cast< liqTokenPointer* >( &( iter->second ) )->getRtPointer();
		  }

		  RiAttributeV( "user", numTokens, tokenArray.get(), pointerArray.get() );
		}
	}
	void Renderer::objectBlock_reference_begin()
	{
		CM_TRACE_FUNC("Renderer::objectBlock_reference_begin()");

		if( liqRibTranslator::getInstancePtr()->m_ignoreSurfaces && !liqglo.liqglo_skipDefaultMatte )
		{
			RiSurface( "matte", RI_NULL );
		}
	}
	void Renderer::objectBlock_reference_end()
	{
		CM_TRACE_FUNC("Renderer::objectBlock_reference_end()");

		assert(liqRibTranslator::getInstancePtr()->attributeDepth==0);
		while ( liqRibTranslator::getInstancePtr()->attributeDepth > 0 ) 
		{
			RiAttributeEnd();
			liqRibTranslator::getInstancePtr()->attributeDepth--;
		}
	}
	void Renderer::writeShader_forShadow_ribbox(const MString & text)
	{
		CM_TRACE_FUNC("Renderer::writeShader_forShadow_ribbox(text)");
		//refactor 37 begin
		// Default : just write the contents of the rib box
		RiArchiveRecord( RI_VERBATIM, ( char* )text.asChar() );
		RiArchiveRecord( RI_VERBATIM, "\n" );
		//refactor 37 end
	}
	void Renderer::writeShader_forShadow_forSpecialTypes(const liqRibNodePtr &ribNode__, bool m_shaderDebug, const MDagPath & path__)
	{
		CM_TRACE_FUNC("Renderer::writeShader_forShadow_forSpecialTypes("<<ribNode__->name.asChar()<<","<<m_shaderDebug <<",path__)");
	
		//refactor 38 begin
		MObject shadingGroup = ribNode__->assignedShadingGroup.object();
		MObject shader = ribNode__->findShader();
		//
		// here we check for regular shader nodes first
		// and assign default shader to shader-less nodes.
		//

		if( m_shaderDebug ) {
			liqRIBMsg("shader debug is turned on, so the surface is constant.");
			RiSurface( "constant", RI_NULL );
			LIQDEBUGPRINTF("add more constant parameters here. take /RMS-1.0.1-Maya2008/lib/shaders/src/mtorBlinn.sl as an example.(?)");
		}
		//else if( shader.apiType() == MFn::kLambert ){ 
		//	RiSurface( "matte", RI_NULL );
		//	LIQDEBUGPRINTF("add more lambert parameters here. take //RMS-1.0.1-Maya2008/lib/shaders/src/mtorLambert.sl as an example.");
		//}else if( shader.apiType() == MFn::kPhong ) {
		//	RiSurface( "plastic", RI_NULL );
		//	LIQDEBUGPRINTF("add more phong parameters here. take /RMS-1.0.1-Maya2008/lib/shaders/src/mtorPhong.sl as an example.");
		//}
		else if( path__.hasFn( MFn::kPfxHair ) ) 
		{
			// get some of the hair system parameters
			liqFloat translucence = 0, specularPower = 0;
			liqColor specularColor;

			liqRibTranslator::getInstancePtr()->getPfxHairData(path__, translucence, specularPower, specularColor);

			RiSurface(  "liquidpfxhair",
				"float specularpower", &specularPower,
				"float translucence",  &translucence,
				"color specularcolor", &specularColor,
				RI_NULL );
		} 
		else if( path__.hasFn( MFn::kPfxToon ) ) {
			RiSurface( "liquidpfxtoon", RI_NULL );
		}else if( path__.hasFn( MFn::kPfxGeometry ) ){
			RiSurface( "liquidpfx", RI_NULL );
		}else {
			//RiSurface( "plastic", RI_NULL );//ymesh-branch r773 use this
			MFnDependencyNode shaderFn(shader);
			RiSurface( const_cast<char*>(shaderFn.name().asChar()), RI_NULL );
		}
		//refactor 38 end

	}
	void Renderer::writeShader_forDeepShadow_forSpecialTypes(const liqRibNodePtr &ribNode__, const MDagPath & path__)
	{
		CM_TRACE_FUNC("Renderer::writeShader_forDeepShadow_forSpecialTypes("<<ribNode__->name.asChar() <<",path__)");
	
		if( path__.hasFn( MFn::kPfxHair ) ) 
		{
			// get some of the hair system parameters
			liqFloat translucence = 0, specularPower = 0;
			liqColor specularColor;

			liqRibTranslator::getInstancePtr()->getPfxHairData(path__, translucence, specularPower, specularColor);

			RiSurface(  "liquidPfxHair",
				"float specularPower", &specularPower,
				"float translucence",  &translucence,
				"color specularColor", &specularColor,
				RI_NULL );
		}
	}
	void Renderer::writeShader_forShadow_NullShader(const liqRibNodePtr &ribNode__, const MDagPath & path__)
	{
		CM_TRACE_FUNC("Renderer::writeShader_forShadow_NullShader("<<ribNode__->name.asChar() <<",path__)");
	
		RiSurface( "null", RI_NULL );
	}
	MString Renderer::getTextureExt()const
	{
		CM_TRACE_FUNC("Renderer::getTextureExt()");

		if( liqglo.liquidRenderer.renderName == MString("PRMan") )
		{
			return "tex";
		}
		else if( liqglo.liquidRenderer.renderName == MString("3Delight") )
		{
			return "tif";
		}
		//
		return "unhandled";
	}
	bool Renderer::isTextureTypeSupported(const MString &textureType)const
	{
		CM_TRACE_FUNC("Renderer::getTextureExt("<<textureType.asChar() <<")");

		if( liqglo.liquidRenderer.renderName == MString("PRMan") )
		{
			if(textureType=="tex"){
				return true;
			}
			return false;
		}
		else if( liqglo.liquidRenderer.renderName == MString("3Delight") )
		{
			if(textureType=="tif"){
				return true;
			}
			return false;
		}

		return false;
	}
	//
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
}//namespace

#endif//_USE_RENDERMAN_