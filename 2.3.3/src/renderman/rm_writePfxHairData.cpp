#include "rm_renderer.h"
#include <liqRibPfxHairData.h>
#include <liqGlobalVariable.h>
#include "rm_helper.h"

namespace renderman
{
	static void _write(liqRibPfxHairData* pData, const structJob &currentJob);
	//
	void Renderer::write(
		/*const*/ liqRibPfxHairData* pData,
		const MString &ribFileName,
		const structJob &currentJob,
		const bool bReference)
	{
		CM_TRACE_FUNC("Renderer::write("<<pData->getFullPathName()<<","<<ribFileName.asChar()<<","<<currentJob.name.asChar()<<",ref="<<bReference<<")");

		assert(liqglo.m_ribFileOpen&&"rm_writePfxHairData.cpp");

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
					assert(pData->getRibFileFullPath().length()==0&&"rm_writePfxHairData.cpp");
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
	static void _write(liqRibPfxHairData* pData, const structJob &currentJob)
	{
		CM_TRACE_FUNC("rm_writePfxHairData.cpp::_write("<<pData->getFullPathName()<<","<<currentJob.name.asChar()<<")");

		//
		if( pData->isEmpty() ) 
		{
			RiIdentity(); // In case we're in a motion block!
			liquidMessage2(messageWarning, "pfxHair is empty %s",  pData->getFullPathName());
			return;
		}

		//
		unsigned numTokens( pData->tokenPointerArray.size() );
		boost::scoped_array< RtToken > tokenArray( new RtToken[ numTokens ] );
		boost::scoped_array< RtPointer > pointerArray( new RtPointer[ numTokens ] );
		assignTokenArraysV( pData->tokenPointerArray, tokenArray.get(), pointerArray.get() );
		RiCurvesV( "cubic", pData->ncurves, pData->nverts.get(), "nonperiodic", numTokens, tokenArray.get(), pointerArray.get() );

	}
}//namespace renderman
