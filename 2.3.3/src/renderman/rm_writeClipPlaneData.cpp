#include "rm_renderer.h"
#include <liqRibClipPlaneData.h>
#include "rm_helper.h"
#include <liqGlobalVariable.h>

namespace renderman
{
	static void _write(liqRibClipPlaneData* pData, const structJob &currentJob);
	//
	void Renderer::write(
		liqRibClipPlaneData* pData,
		const MString &ribFileName,
		const structJob &currentJob,
		const bool bReference)
	{
		CM_TRACE_FUNC("Renderer::write("<<pData->getFullPathName().asChar()<<","<<ribFileName.asChar()<<","<<currentJob.name.asChar()<<",ref="<<bReference<<")");

		assert(liqglo.m_ribFileOpen &&"rm_writeClipPlaneData.cpp");

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
	static void _write(liqRibClipPlaneData* pData, const structJob &currentJob__)
	{
		CM_TRACE_FUNC("rm_writeClipPlaneData.cpp::_write("<<pData->getFullPathName().asChar()<<","<<currentJob__.name.asChar()<<",...)");

		LIQDEBUGPRINTF("-> writing clipPlane");
		RiArchiveRecord( RI_VERBATIM, "ClippingPlane 0 0 -1 0 0 0\n" );
	}
}//namespace renderman
