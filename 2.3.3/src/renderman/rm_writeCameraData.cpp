#include "rm_renderer.h"
#include <liqRibCameraData.h>
#include <liqGlobalVariable.h>
#include "rm_helper.h"

namespace renderman
{
	static void _write(liqRibCameraData* pData, const structJob &currentJob);
	//
	void Renderer::write(
		/*const*/ liqRibCameraData* pData,
		const MString &ribFileName,
		const structJob &currentJob,
		const bool bReference)
	{
		CM_TRACE_FUNC("Renderer::write("<<pData->getFullPathName().asChar()<<","<<ribFileName.asChar()<<","<<currentJob.name.asChar()<<",ref="<<bReference<<")");

		assert(liqglo.m_ribFileOpen&&"rm_writeCameraData.cpp");

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
					assert(pData->getRibFileFullPath().length()==0&&"rm_writeCameraData.cpp");
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
	static void _write(liqRibCameraData* pData, const structJob &currentJob)
	{
		CM_TRACE_FUNC("rm_writeCameraData.cpp::_write("<<pData->getFullPathName().asChar()<<","<<currentJob.name.asChar()<<")");

		//
		RiArchiveRecord( RI_COMMENT, "_write(liqRibCameraData*...) not implemented yet." );
	}
}//namespace renderman
