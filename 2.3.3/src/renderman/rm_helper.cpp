#include <liqConfig.h>
#ifdef _USE_RENDERMAN_

#include "rm_helper.h"

#include "../common/prerequest_maya.h"
#include "liqGlobalHelpers.h"
#include <liqRibTranslator.h>
#include "ri_interface.h"

namespace renderman
{
	Helper::Helper()
	{

	}
	//
	Helper::Helper(const std::string& ribFile)
		:m_ribFile(ribFile)
	{

	}
	Helper::~Helper()
	{

	}
	//
	void Helper::RiBeginRef(const std::string& ribFile)
	{
		CM_TRACE_FUNC("Helper::RiBeginRef("<<ribFile<<")");

		m_ribFile = ribFile;

		//if m_ribFileFullPath is "", write the data into the current rib file.
		if( m_ribFile != "" ){
			//log
			RiArchiveRecord( RI_COMMENT, "output: %s", m_ribFile.c_str() );
			//write the data into another rib file.
			m_contex = RiGetContext();//push context
			liquidMessage("output rib: "+ MString(m_ribFile.c_str()) , messageInfo);
			RiBegin_liq( const_cast< RtToken >( m_ribFile.c_str() ) );
		}
	}
	//
	void Helper::RiEndRef()
	{
		CM_TRACE_FUNC("Helper::RiEndRef()");

		if( m_ribFile != "" ){
			RiEnd();
			RiContext(m_contex);//pop context
		}
	}
	//-------------------------------------------------------
	//
	//-------------------------------------------------------
	RibDataExportHelper::RibDataExportHelper(){}
	RibDataExportHelper::~RibDataExportHelper(){}
	//-------------------------------------------------------
	//
	//-------------------------------------------------------
	MString getShadingGroupFilePath(const MString &shadingGroupNodeName)
	{
		CM_TRACE_FUNC("getShadingGroupFilePath("<<shadingGroupNodeName.asChar()<<")");
		return getShaderDirectory()+sanitizeNodeName(shadingGroupNodeName+".rmsg");
	}
	MString getShaderFilePath_NoExt(const MString &shaderNodeName)
	{
		CM_TRACE_FUNC("getShaderFilePath_NoExt("<<shaderNodeName.asChar()<<")");
		return getShaderDirectory()+sanitizeNodeName(shaderNodeName);
	}
	MString getShaderFilePath_SRC(const MString &shaderNodeName)
	{
		CM_TRACE_FUNC("getShaderFilePath_SRC("<<shaderNodeName.asChar()<<")");
		return getShaderFilePath_NoExt(shaderNodeName)+".sl_my";
	}
	MString getShaderFilePath_SLO(const MString &shaderNodeName)
	{
		CM_TRACE_FUNC("getShaderFilePath_SLO("<<shaderNodeName.asChar()<<")");
		return getShaderFilePath_NoExt(shaderNodeName)+".slo";
	}
	//----------------------------------------------------------------------
	void exportJobCamera(const structJob &job, const structCamera camera[])
	{
		CM_TRACE_FUNC("exportJobCamera("<<job.name.asChar()<<","<<",)");

		if ( camera[0].isOrtho )
		{
			liqFloat frameWidth, frameHeight;
			// the whole frame width has to be scaled according to the UI Unit
			frameWidth  = camera[0].orthoWidth  * 0.5 ;
			frameHeight = camera[0].orthoHeight * 0.5 ;
			RiProjection( "orthographic", RI_NULL );
			// if we are describing a shadow map camera,
			// we need to set the screenwindow to the default,
			// as shadow maps are always square.
			if( job.pass == rpShadowMap ) 
				RiScreenWindow( -frameWidth, frameWidth, -frameHeight, frameHeight );
			else 			                 
				RiScreenWindow( -1.0, 1.0, -1.0, 1.0 );
		}
		else
		{
			liqFloat fieldOfView = camera[0].hFOV * 180.0 / M_PI ;
			if ( job.pass == rpShadowMap && job.isPoint ) fieldOfView = job.camera[0].hFOV;

			RiProjection( "perspective", RI_FOV, &fieldOfView, RI_NULL );

			// if we are describing a shadow map camera,
			// we need to set the screenwindow to the default,
			// as shadow maps are always square.

			if ( job.pass == rpShadowMap == false )
			{
				double ratio = (double)job.width / (double)job.height;
				double left, right, bottom, top;
				if ( ratio <= 0 )
				{
					left    = -1 + camera[0].horizontalFilmOffset;
					right   =  1 + camera[0].horizontalFilmOffset;
					bottom  = -1 / ratio + camera[0].verticalFilmOffset;
					top     =  1 / ratio + camera[0].verticalFilmOffset;
				}
				else
				{
					left    = -ratio + camera[0].horizontalFilmOffset;
					right   =  ratio + camera[0].horizontalFilmOffset;
					bottom  = -1 + camera[0].verticalFilmOffset;
					top     =  1 + camera[0].verticalFilmOffset;
				}
				RiScreenWindow( left, right, bottom, top );
			}
			else
			{
				RiScreenWindow( -1.0, 1.0, -1.0, 1.0 );
			}
		}
		RiClipping( camera[0].neardb, camera[0].fardb );
		if ( liqglo.doDof && job.pass != rpShadowMap ) 
			RiDepthOfField( camera[0].fStop, camera[0].focalLength, camera[0].focalDistance );
		// if we motion-blur the cam, open the motion block
		//
		if ( liqglo.doCameraMotion && ( job.pass != rpShadowMap || job.shadowType == stDeep) )
		{
			if ( liqglo.liqglo_relativeMotion ) 
				RiMotionBeginV( liqglo.liqglo_motionSamples, liqglo.liqglo_sampleTimesOffsets );
			else 		                     
				RiMotionBeginV( liqglo.liqglo_motionSamples, liqglo.liqglo_sampleTimes );
		}

		// write the camera transform
		//
		liqMatrix cameraMatrix;
		camera[0].mat.get( cameraMatrix );
		RiTransform( cameraMatrix );

		// if we motion-blur the cam, write the subsequent motion samples and close the motion block
		//
		if ( liqglo.doCameraMotion && ( job.pass != rpShadowMap || job.shadowType == stDeep ) )
		{
			int mm = 1;
			while ( mm < liqglo.liqglo_motionSamples )
			{
				camera[mm].mat.get( cameraMatrix );
				RiTransform( cameraMatrix );
				++mm;
			}
			RiMotionEnd();
		}
	}

}
#endif//_USE_RENDERMAN_