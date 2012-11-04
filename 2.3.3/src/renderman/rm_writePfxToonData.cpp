#include <liqConfig.h>
#ifdef _USE_RENDERMAN_

#include "rm_renderer.h"
#include <liqRibPfxToonData.h>
#include "rm_helper.h"
#include <liqGlobalVariable.h>

namespace renderman
{
	static void _write(liqRibPfxToonData* pData, const structJob &currentJob);
	//
	void Renderer::write(
		liqRibPfxToonData* pData,
		const MString &ribFileName,
		const structJob &currentJob,
		const bool bReference)
	{
		CM_TRACE_FUNC("Renderer::write("<<pData->getFullPathName()<<","<<ribFileName.asChar()<<","<<currentJob.name.asChar()<<",ref="<<bReference<<")");

		assert(liqglo.m_ribFileOpen &&"rm_writePfxToonData.cpp");

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
	static void _write(liqRibPfxToonData* pData, const structJob &currentJob__)
	{
		CM_TRACE_FUNC("rm_writePfxToonData.cpp::_write("<<pData->getFullPathName()<<","<<currentJob__.name.asChar()<<",...)");

		LIQDEBUGPRINTF( "-> writing pfxToon curve\n" );

		if ( !pData->isEmpty() ) 
		{
			unsigned numTokens( pData->tokenPointerArray.size() );
			boost::scoped_array< RtToken > tokenArray( new RtToken[ numTokens ] );
			boost::scoped_array< RtPointer > pointerArray( new RtPointer[ numTokens ] );
			assignTokenArraysV( pData->tokenPointerArray, tokenArray.get(), pointerArray.get() );

			RiCurvesV( "linear", pData->ncurves, pData->nverts.get(), "nonperiodic", numTokens, tokenArray.get(), pointerArray.get() );
		} else 
			RiIdentity(); // Make sure we don't create empty motion blocks

	}
}//namespace renderman

#endif//_USE_RENDERMAN_
