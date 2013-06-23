#include "er_renderer.h"
#include <liqRibClipPlaneData.h>
#include <common/prerequest_maya.h>
#include <common/mayacheck.h>
#include "ercall.h"

//#include <liqRibNode.h>
#include <liqRibTranslator.h>

namespace elvishray
{
	static void _write(liqRibClipPlaneData* pData, const structJob &currentJob);

	void Renderer::write(
		/*const*/ liqRibClipPlaneData* pData,
		const MString &fileName, 
		const structJob &currentJob,
		const bool bReference)
	{
		CM_TRACE_FUNC("Renderer::write("<<pData->getFullPathName()<<","<<fileName.asChar()<<","<<currentJob.name.asChar()<<",ref="<<bReference<<")");

		if( !bReference ){//write data at first time
			assert(pData->getRibFileFullPath().length()==0);
			pData->setRibFileFullPath(fileName);

// 			renderman::Helper o;
// 			o.RiBeginRef(pData->getRibFileFullPath().asChar());
 			_write(pData, currentJob);
// 			o.RiEndRef();

		}else{
			//write the reference
			assert(pData->getRibFileFullPath() == fileName);
//			RiReadArchive( const_cast< RtToken >( pData->getRibFileFullPath().asChar() ), NULL, RI_NULL );
		}
	}
	//
	static void _write(liqRibClipPlaneData* pData, const structJob &currentJob__)
	{
		CM_TRACE_FUNC("er_writeClipPlaneData.cpp::write("<<pData->getFullPathName()<<","<<currentJob__.name.asChar()<<",...)");
		OutputMgr &o = Renderer::getOutputMgr();

		o.a(boost::str(boost::format("Locator(ClipPlane) is not implemented yet. [%s]")%pData->getFullPathName()));
		liquidMessage2(messageError, "Locator(ClipPlane) is not implemented yet. [%s]", pData->getFullPathName());
	}
}//namespace elvishray
