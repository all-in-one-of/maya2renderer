#include "rm_renderer.h"
#include <liqRibSurfaceData.h>
#include "rm_helper.h"
#include <liqGlobalVariable.h>
#include <liqGlobalHelpers.h>

namespace renderman
{
	static void _write(liqRibSurfaceData* pData, const structJob &currentJob);
	//
	void Renderer::write(
		liqRibSurfaceData* pData,
		const MString &ribFileName,
		const structJob &currentJob,
		const bool bReference)
	{
		CM_TRACE_FUNC("Renderer::write("<<pData->getFullPathName().asChar()<<","<<ribFileName.asChar()<<","<<currentJob.name.asChar()<<",ref="<<bReference<<")");

		assert(liqglo.m_ribFileOpen &&"rm_writeSurfaceData.cpp");

		if(liqglo.m_writeDataIntoMainRibFile)
		{
			if( bReference ){
				_write(pData, currentJob);
			}else{
				//do nothing
			}
		}else{
	 		if( !bReference ){//write data at first time
				if( currentJob.pass != rpShadowMap ){
					assert(pData->getRibFileFullPath().length()==0&&"rm_writeClipPlaneData.cpp");
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
	static void _write(liqRibSurfaceData* pData, const structJob &currentJob__)
	{
		CM_TRACE_FUNC("rm_writeSurfaceData.cpp::_write("<<pData->getFullPathName().asChar()<<","<<currentJob__.name.asChar()<<",...)");

		LIQDEBUGPRINTF( "-> writing nurbs surface\n" );

		LIQDEBUGPRINTF( "-> writing nurbs surface trims\n" );
		if ( pData->hasTrims ) 
		{
			RiTrimCurve( pData->nloops,
				const_cast< RtInt* >( &pData->ncurves[ 0 ] ),
				const_cast< RtInt* >( &pData->order[ 0 ] ),
				const_cast< RtFloat* >( &pData->knot[ 0 ] ),
				const_cast< RtFloat* >( &pData->minKnot[ 0 ] ),
				const_cast< RtFloat* >( &pData->maxKnot[ 0 ] ),
				const_cast< RtInt* >( &pData->numCVs[ 0 ] ),
				const_cast< RtFloat* >( &pData->u[ 0 ] ),
				const_cast< RtFloat* >( &pData->v[ 0 ] ),
				const_cast< RtFloat* >( &pData->w[ 0 ] ) );
		}

		if ( !pData->tokenPointerArray.empty() ) 
		{
			unsigned numTokens( pData->tokenPointerArray.size() );
			boost::scoped_array< RtToken > tokenArray( new RtToken[ numTokens ] );
			boost::scoped_array< RtPointer > pointerArray( new RtPointer[ numTokens ] );
			assignTokenArraysV( pData->tokenPointerArray, tokenArray.get(), pointerArray.get() );

			RiNuPatchV(
				pData->nu,
				pData->uorder,
				pData->uknot.get(),
				pData->umin,
				pData->umax,

				pData->nv,
				pData->vorder,
				pData->vknot.get(),
				pData->vmin,
				pData->vmax,

				numTokens,
				tokenArray.get(),
				pointerArray.get() );
		} 
		else 
		{
			LIQDEBUGPRINTF( "-> ignoring nurbs surface\n" );
		}
		LIQDEBUGPRINTF( "-> done writing nurbs surface\n" );
	}
}//namespace renderman
