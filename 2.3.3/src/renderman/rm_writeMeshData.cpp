#include <liqConfig.h>
#ifdef _USE_RENDERMAN_

#include "rm_renderer.h"
#include <liqRibMeshData.h>
#include <liqGlobalVariable.h>
#include "rm_helper.h"

namespace renderman
{
	static void _write(liqRibMeshData* pData, const structJob &currentJob);
	//
	void Renderer::write(
		/*const*/ liqRibMeshData* pData,
		const MString &ribFileName,
		const structJob &currentJob,
		const bool bReference)
	{
		CM_TRACE_FUNC("Renderer::write("<<pData->getFullPathName()<<","<<ribFileName.asChar()<<","<<currentJob.name.asChar()<<",ref="<<bReference<<")");

		assert(liqglo.m_ribFileOpen&&"rm_writeMeshData.cpp");

		if(liqglo.m_writeDataIntoMainRibFile)
		{
			if( bReference ){
				_write(pData, currentJob);
			}else{
				//do nothing
			}
		}else{
	 		if( !bReference ){//write data at first time
				if( !currentJob.isShadow ){
					assert(pData->getRibFileFullPath().length()==0&&"rm_writeMeshData.cpp");
				}
				pData->setRibFileFullPath(ribFileName);

	 			renderman::Helper o;
	 			o.RiBeginRef(pData->getRibFileFullPath().asChar());
	 			_write(pData, currentJob);
	 			o.RiEndRef();
		 
	 		}else{
	 			//write the reference
	 			assert(pData->getRibFileFullPath() == ribFileName);
	 			RiReadArchive( const_cast< RtToken >( pData->getRibFileFullPath().asChar() ), NULL, RI_NULL );
	 		}
		}

	}
	//
	static void _write(liqRibMeshData* pData, const structJob &currentJob)
	{
		CM_TRACE_FUNC("rm_writeMeshData.cpp::_write("<<pData->getFullPathName()<<","<<currentJob.name.asChar()<<")");

		if( pData->isEmpty() )
		{
			liquidMessage( "Could not export degenerate mesh", messageError );
			return;
		}
		//
		//RiArchiveRecord( RI_COMMENT, "Sometimes the polygon cant be rendered correctly, so I try to reverse the normal. //- yaoyansi" );
		//RiReverseOrientation();

		if(pData->isAreaLight())
		{
			RtLightHandle handle = INVALID_LIGHT_INDEX;

			{ // What happens if we're inside a motion block????? This whole approach of Liquid is flawed...
				LIQDEBUGPRINTF( "-> mesh is area light\n" );
				//	RiAttributeBegin();
				RtString ribname = const_cast< char* >( pData->getFullPathName() );
				RiAttribute( "identifier", "name", &ribname, RI_NULL );
				RtMatrix tmp;
				memcpy( tmp, pData->getTransformationMatrixPtr(), sizeof( RtMatrix ) );
				RiTransform( tmp );
				float areaIntensity = pData->getAreaIntensity();
				handle = RiAreaLightSource( "arealight", "intensity", &areaIntensity, RI_NULL );
			}
			//
			//mesh data begin
			//
			// Each loop has one polygon, so we just want an array of 1's of
			// the correct size. Stack version.
			//vector< RtInt > nloops( numFaces, 1 );
			// Alternatively (heap version):
			boost::scoped_array< RtInt > nloops( new RtInt[ pData->getNumFaces() ] );
			std::fill( nloops.get(), nloops.get() + pData->getNumFaces(), ( RtInt )1 );

			unsigned numTokens( pData->tokenPointerArray.size() );
			boost::scoped_array< RtToken > tokenArray( new RtToken[ numTokens ] );
			boost::scoped_array< RtPointer > pointerArray( new RtPointer[ numTokens ] );
			assignTokenArraysV( pData->tokenPointerArray, tokenArray.get(), pointerArray.get() );

			RiPointsGeneralPolygonsV( pData->getNumFaces(),
				&nloops[ 0 ],
				pData->getNverts().get(),
				pData->getVerts().get(),
				numTokens,
				tokenArray.get(),
				pointerArray.get() );
			//mesh data end

			{
				// RiAttributeEnd();
				RiIlluminate( handle, 1 );
			}
		}else{
			//mesh data begin
			//
			// Each loop has one polygon, so we just want an array of 1's of
			// the correct size. Stack version.
			//vector< RtInt > nloops( numFaces, 1 );
			// Alternatively (heap version):
			boost::scoped_array< RtInt > nloops( new RtInt[ pData->getNumFaces() ] );
			std::fill( nloops.get(), nloops.get() + pData->getNumFaces(), ( RtInt )1 );

			unsigned numTokens( pData->tokenPointerArray.size() );
			boost::scoped_array< RtToken > tokenArray( new RtToken[ numTokens ] );
			boost::scoped_array< RtPointer > pointerArray( new RtPointer[ numTokens ] );
			assignTokenArraysV( pData->tokenPointerArray, tokenArray.get(), pointerArray.get() );

			RiPointsGeneralPolygonsV( pData->getNumFaces(),
				&nloops[ 0 ],
				pData->getNverts().get(),
				pData->getVerts().get(),
				numTokens,
				tokenArray.get(),
				pointerArray.get() );
			//mesh data end//	
		}
	}
}//namespace renderman

#endif//_USE_RENDERMAN_
