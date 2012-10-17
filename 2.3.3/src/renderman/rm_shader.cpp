#include <liqConfig.h>
#ifdef _USE_RENDERMAN_
#include "../common/prerequest_maya.h"
#include "rm_renderer.h"
#include <liqShader.h>
#include <liqShaderFactory.h>
namespace renderman
{
	void Renderer::shader_transformBegin(const liqString  shaderSpace)
	{
		CM_TRACE_FUNC("Renderer::shader_transformBegin("<<shaderSpace<<")");

		RiTransformBegin();
		RiCoordSysTransform( /*( RtString )*/shaderSpace );
	}
	void Renderer::shader_transformEnd(const liqString  shaderSpace)
	{
		CM_TRACE_FUNC("Renderer::shader_transformEnd("<<shaderSpace<<")");

		RiTransformEnd();
	}
	void Renderer::shader_surface(
		const liqShader &shader,
		const std::vector<liqTokenPointer> &tokenPointerArray
		)
	{
		CM_TRACE_FUNC("Renderer::shader_surface("<<shader.getName()<<", tokenPointerArray)");

		int shaderParamCount = tokenPointerArray.size() - 1;
		boost::scoped_array< RtToken > tokenArray( new RtToken[ tokenPointerArray.size() ] );
		boost::scoped_array< RtPointer > pointerArray( new RtPointer[ tokenPointerArray.size() ] );
		assignTokenArrays( tokenPointerArray.size(), &tokenPointerArray[ 0 ], tokenArray.get(), pointerArray.get() );

		RiSurfaceV ( const_cast<char *>(shader.getShaderFileName().c_str()), shaderParamCount, tokenArray.get() , pointerArray.get() );

	}
	liqLightHandle Renderer::shader_light(
		const liqShader &shader,
		const std::vector<liqTokenPointer> &tokenPointerArray
		)
	{
		CM_TRACE_FUNC("Renderer::shader_light("<<shader.getName()<<", tokenPointerArray)");

		int shaderParamCount = tokenPointerArray.size() - 1;
		boost::scoped_array< RtToken > tokenArray( new RtToken[ tokenPointerArray.size() ] );
		boost::scoped_array< RtPointer > pointerArray( new RtPointer[ tokenPointerArray.size() ] );
		assignTokenArrays( tokenPointerArray.size(), &tokenPointerArray[ 0 ], tokenArray.get(), pointerArray.get() );

		return RiLightSourceV( const_cast<char *>(shader.getShaderFileName().c_str()), shaderParamCount, tokenArray.get(), pointerArray.get() );
	}
	void Renderer::shader_displacement(
		const liqShader &shader,
		const std::vector<liqTokenPointer> &tokenPointerArray
		)
	{
		CM_TRACE_FUNC("Renderer::shader_displacement("<<shader.getName()<<", tokenPointerArray)");

		int shaderParamCount = tokenPointerArray.size() - 1;
		boost::scoped_array< RtToken > tokenArray( new RtToken[ tokenPointerArray.size() ] );
		boost::scoped_array< RtPointer > pointerArray( new RtPointer[ tokenPointerArray.size() ] );
		assignTokenArrays( tokenPointerArray.size(), &tokenPointerArray[ 0 ], tokenArray.get(), pointerArray.get() );

		RiDisplacementV( const_cast<char *>(shader.getShaderFileName().c_str()), shaderParamCount, tokenArray.get(), pointerArray.get());

	}
	void Renderer::shader_volume(
		const liqShader &shader, 
		const std::vector<liqTokenPointer> &tokenPointerArray
		)
	{
		CM_TRACE_FUNC("Renderer::shader_volume("<<shader.getName()<<", tokenPointerArray)");

		int shaderParamCount = tokenPointerArray.size() - 1;
		boost::scoped_array< RtToken > tokenArray( new RtToken[ tokenPointerArray.size() ] );
		boost::scoped_array< RtPointer > pointerArray( new RtPointer[ tokenPointerArray.size() ] );
		assignTokenArrays( tokenPointerArray.size(), &tokenPointerArray[ 0 ], tokenArray.get(), pointerArray.get() );

		RiAtmosphereV ( const_cast<char *>(shader.getShaderFileName().c_str()), shaderParamCount,  tokenArray.get(), pointerArray.get() );

	}
	//co-shader
	void Renderer::shader_shader(
		const liqShader &shader, 
		const std::vector<liqTokenPointer> &tokenPointerArray
		)
	{
		CM_TRACE_FUNC("Renderer::shader_shader("<<shader.getName()<<", tokenPointerArray)");

		int shaderParamCount = tokenPointerArray.size() - 1;
		boost::scoped_array< RtToken > tokenArray( new RtToken[ tokenPointerArray.size() ] );
		boost::scoped_array< RtPointer > pointerArray( new RtPointer[ tokenPointerArray.size() ] );
		assignTokenArrays( tokenPointerArray.size(), &tokenPointerArray[ 0 ], tokenArray.get(), pointerArray.get() );

		RiShaderV ( const_cast<char *>(shader.getShaderFileName().c_str()), 
			const_cast<char*>(shader.shaderHandler.asChar()), 
			shaderParamCount,  tokenArray.get(), pointerArray.get() );
	}
	void Renderer::shader_UserDefinedShader(const liqShader* liqshader)
	{
		CM_TRACE_FUNC("Renderer::shader_UserDefinedShader("<<liqshader->getName()<<")");
		std::vector<MString> yetExportedShaders;
		return shader_UserDefinedShader(liqshader, /*indentLevel,*/ yetExportedShaders/*, liqshader->forceAs*/);
	}

	void Renderer::shader_UserDefinedShader(const liqShader* liqshader, std::vector<MString> &yetExportedShaders/*, SHADER_TYPE forceAs*/)
	{
		CM_TRACE_FUNC("Renderer::shader_UserDefinedShader("<<liqshader->getName()<<",...)");

		SHADER_TYPE forceAs = liqshader->forceAs;

		// check if shader was yet exported (we don't want to export co-shaders more than one time)
		for(unsigned int i=0; i<yetExportedShaders.size(); i++)
		{
			if( yetExportedShaders[i] == liqshader->shaderHandler )
			{
				return; // won't export another time
			}
		}

		// force type : permit to write a co-shader as a Surface/Displace/...
		// use forceAs directly, instead of shaderType. //- yaoyansi
		//SHADER_TYPE shaderType = liqshader->shader_type;
		//if( forceAs != SHADER_TYPE_UNKNOWN )
		//{
		//	shaderType = forceAs;
		//}

		// write co-shaders before
		for(unsigned int i=0; i<liqshader->m_coShaderArray.size(); i++)
		{
			liqShader &coShader = liqShaderFactory::instance().getShader(liqshader->m_coShaderArray[i]);
			if( coShader.hasErrors )
			{
				char errorMsg[512];
				sprintf(errorMsg, "[liqShader::write] While initializing coShader for '%s', node couldn't be exported", coShader.getName().c_str());
				liquidMessage( errorMsg, messageError );
			}
			else
			{
				//this->writeAsCoShader(&coShader/*shortShaderNames, indentLevel*/);
				coShader.forceAs = SHADER_TYPE_SHADER;
				this->shader_UserDefinedShader(&coShader, yetExportedShaders /*, SHADER_TYPE_SHADER*/);
			}
		}

		// write rib attributes (but not for coshaders)
		RiArchiveRecord( RI_COMMENT, "writeRibAttributes() is omitted." );
		//writeRibAttributes(liqshader/*, shaderType*/);

		// write shader
		char* shaderFileName = const_cast<char*>(liqshader->getShaderFileName().c_str());
		if( liqshader->shaderSpace != "" )
		{
			this->shader_transformBegin((const liqString)liqshader->shaderSpace.asChar());
		}
		// output shader
		// its one less as the tokenPointerArray has a preset size of 1 not 0
		if(forceAs == SHADER_TYPE_UNKNOWN)
		{
		if( liqshader->shader_type_ex == "light" )
		{
				//outputIndentation(indentLevel);
				RtLightHandle ret = this->shader_light( *liqshader,  liqshader->tokenPointerArray );
#ifdef RIBLIB_AQSIS
				(const_cast<liqShader*>(liqshader))->shaderHandler.set( reinterpret_cast<ptrdiff_t>(static_cast<RtLightHandle>(ret)) );
#else
				liqshader->shaderHandler.set( ret );
#endif
		}else if(liqshader->shader_type_ex == "surface"){
				//outputIndentation(indentLevel);
				this->shader_surface( *liqshader,  liqshader->tokenPointerArray );
		}else if(liqshader->shader_type_ex == "displacement"){
				//outputIndentation(indentLevel);
				this->shader_displacement( *liqshader,  liqshader->tokenPointerArray );
		}else if(liqshader->shader_type_ex == "volume"){
				//outputIndentation(indentLevel);
				this->shader_volume( *liqshader,   liqshader->tokenPointerArray );
		}else if(liqshader->shader_type_ex == "shader"){
				this->shader_shader( *liqshader,   liqshader->tokenPointerArray );
		}else{
			char errorMsg[512];
			sprintf(errorMsg, "[liqShader::write] Unknown shader type for %s shader_type=%s", liqshader->getName().c_str(), liqshader->shader_type_ex.asChar());
			liquidMessage( errorMsg, messageError );
		}
		}else{//forceAs != SHADER_TYPE_UNKNOWN
			switch( forceAs )// use forceAs directly, instead of shaderType. //- yaoyansi
			{
			case SHADER_TYPE_SURFACE :
				this->shader_surface( *liqshader,  liqshader->tokenPointerArray );
				break;
			case SHADER_TYPE_DISPLACEMENT :
				this->shader_displacement( *liqshader,  liqshader->tokenPointerArray );
				break;
			case SHADER_TYPE_VOLUME :
				this->shader_volume( *liqshader,   liqshader->tokenPointerArray );
				break;
			case SHADER_TYPE_SHADER :
				this->shader_shader( *liqshader,   liqshader->tokenPointerArray );
				break;
			case SHADER_TYPE_LIGHT :
			{
				RtLightHandle ret = this->shader_light( *liqshader,  liqshader->tokenPointerArray );
#ifdef RIBLIB_AQSIS
				(const_cast<liqShader*>(liqshader))->shaderHandler.set( reinterpret_cast<ptrdiff_t>(static_cast<RtLightHandle>(ret)) );
#else
				liqshader->shaderHandler.set( ret );
#endif
				break;
			}
			default :
				char errorMsg[512];
				sprintf(errorMsg, "[liqShader::write] Unknown shader type for %s shader_type=%d", liqshader->getName().c_str(), forceAs);
				liquidMessage( errorMsg, messageError );
				break;
			}
		}
		//
		if( liqshader->shaderSpace != "" )
		{
			this->shader_transformEnd((const liqString)liqshader->shaderSpace.asChar());
		}

		yetExportedShaders.push_back(liqshader->shaderHandler);
	}
	//
	void Renderer::getValidShaderPlugsInShadingGroup(std::vector<std::string>& plugs)const
	{
		CM_TRACE_FUNC("Renderer::getValidShaderPlugsInShadingGroup()");

		plugs.clear();
		plugs.push_back("surfaceShader");
		plugs.push_back("displacementShader");
		plugs.push_back("volumeShader");
	}
	//
// 	void Renderer::writeAsCoShader(const liqShader* liqshader)
// 	{
// 		CM_TRACE_FUNC("Renderer::writeAsCoShader("<<liqshader->getName()<<")");
// 		//	unsigned int indentLevel = 0;
// 
// 		MFnDependencyNode node(liqshader->m_mObject);
// 		if( liqshader->hasErrors )
// 		{
// 			liquidMessage2(messageError, "[liqShader::write] Erros occured while initializing shader '%s', won't export shader", node.name().asChar());
// 			return;
// 		}
// 		// write up co-shaders before
// 		unsigned int i; 
// 		for(i=0; i<liqshader->m_coShaderArray.size(); i++)
// 		{
// 			//liqShader coShader(m_coShaderArray[i]);
// 			liqShader &coShader = liqShaderFactory::instance().getShader(liqshader->m_coShaderArray[i]);
// 			this->writeAsCoShader(&coShader);
// 		}
// 		// write co-shader
// 		boost::scoped_array< RtToken > tokenArray( new RtToken[ liqshader->tokenPointerArray.size() ] );
// 		boost::scoped_array< RtPointer > pointerArray( new RtPointer[ liqshader->tokenPointerArray.size() ] );
// 		assignTokenArrays( liqshader->tokenPointerArray.size(), &liqshader->tokenPointerArray[ 0 ], tokenArray.get(), pointerArray.get() );
// 
// 
// 		char* shaderFileName = const_cast<char*>(liqshader->getShaderFileName().c_str());
// 		if( liqshader->shaderSpace != "" )
// 		{
// 			this->shader_transformBegin((const liqString)liqshader->shaderSpace.asChar());
// 
// 		}
// 		// output shader
// 		// its one less as the tokenPointerArray has a preset size of 1 not 0
// 		int shaderParamCount = liqshader->tokenPointerArray.size() - 1;
// 		char *shaderHandlerPtr = const_cast<char*>(liqshader->shaderHandler.asChar());
// 
// 		// 	switch( shader_type )
// 		// 	{
// 		// 	case SHADER_TYPE_SHADER:case SHADER_TYPE_SURFACE:case SHADER_TYPE_DISPLACEMENT:case SHADER_TYPE_VOLUME:
// 		// 		//outputIndentation(indentLevel);
// 		// 		RiShaderV(shaderFileName, shaderHandlerPtr, shaderParamCount, tokenArray.get(), pointerArray.get());
// 		// 		break;
// 		// 	default :
// 		// 		char errorMsg[512];
// 		// 		sprintf(errorMsg, "[liqShader::writeAsCoShader] Unknown shader type for %s shader_type=%d (%s)", name.c_str(), shader_type, liqGetSloInfo::getTypeStr(shader_type).asChar());
// 		// 		liquidMessage( errorMsg, messageError );
// 		// 		break;
// 		// 	}
// 		if( liqshader->shader_type_ex=="shader"       ||
// 			liqshader->shader_type_ex=="surface"      ||
// 			liqshader->shader_type_ex=="displacement" ||
// 			liqshader->shader_type_ex=="volume"         )
// 		{
// 			//outputIndentation(indentLevel);
// 			RiShaderV(shaderFileName, shaderHandlerPtr, shaderParamCount, tokenArray.get(), pointerArray.get());
// 		}else{
// 			char errorMsg[512];
// 			sprintf(errorMsg, "[liqShader::writeAsCoShader] Unknown shader type for %s shader_type=%s", liqshader->getName().c_str(), liqshader->shader_type_ex.asChar());
// 			liquidMessage( errorMsg, messageError );
// 		}
// 
// 
// 		if( liqshader->shaderSpace != "" )
// 		{
// 			this->shader_transformEnd((const liqString)liqshader->shaderSpace.asChar());
// 
// 		}
// 	}
	//
	void Renderer::writeRibAttributes(const liqShader* liqshader/*, SHADER_TYPE shaderType*/)
	{
		CM_TRACE_FUNC("Renderer::writeRibAttributes("<<liqshader->getName()<<")");
		MFnDependencyNode node(liqshader->getMObject());

		MStatus status;
		if( liqshader->shader_type_ex == "shader" )//if( shaderType == SHADER_TYPE_SHADER )
		{
			return;
		}

		////////////////////////////////////////////
		//			COLOR
		if( liqshader->shader_type_ex == "surface" )//if( shaderType == SHADER_TYPE_SURFACE )
		{
			RiArchiveRecord( RI_COMMENT, "Renderer::writeRibAttributes()" );
			RiColor((RtFloat*)(liqshader->rmColor));
			RiOpacity((RtFloat*)(liqshader->rmOpacity));
		}

		////////////////////////////////////////////
		//			RIBBOX

		// Try to find a liqRIBBox attribute
		MString shaderRibBox;
		MPlug ribbPlug = node.findPlug( MString( "liqRIBBox" ), &status );
		if( status == MS::kSuccess )
		{
			ribbPlug.getValue( shaderRibBox );
		}
		// just write the contents of the rib box
		if( shaderRibBox != "" )
		{
			RiArchiveRecord( RI_VERBATIM, ( char* )shaderRibBox.asChar() );
			RiArchiveRecord( RI_VERBATIM, "\n" );
		}	

		////////////////////////////////////////////
		//			DISPLACE

		// displacement bounds
		float displacementBounds;
		MString displacementBoundsSpace;
		MPlug sDBPlug = node.findPlug( MString( "displacementBound" ), &status );
		if( status == MS::kSuccess )
		{
			sDBPlug.getValue( displacementBounds );
		}
		MPlug sDBSPlug = node.findPlug( MString( "displacementBoundSpace" ), &status );
		if( status == MS::kSuccess )
		{
			sDBSPlug.getValue( displacementBoundsSpace );
		}
		if( displacementBoundsSpace == "" )
		{
			displacementBoundsSpace = "shader";
		}
		if( displacementBounds != 0.0 )
		{
			RtString coordsys( const_cast< char* >( displacementBoundsSpace.asChar() ) );
			RiAttribute( "displacementbound", (RtToken) "sphere", &displacementBounds, "coordinatesystem", &coordsys, RI_NULL );
		}
	}

}//namespace renderman
#endif//_USE_RENDERMAN_