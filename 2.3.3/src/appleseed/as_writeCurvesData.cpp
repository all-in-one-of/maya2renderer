#include "as_renderer.h"
#include <liqRibCurvesData.h>
#include "../common/prerequest_maya.h"
#include "../common/mayacheck.h"
#include "as_log_helper.h"
//#include <liqRibNode.h>
#include <liqRibTranslator.h>

namespace appleseed
{
	static void _write(liqRibCurvesData* pData, const structJob &currentJob);

	void Renderer::write(
		/*const*/ liqRibCurvesData* pData,
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
	static void _write(liqRibCurvesData* pData, const structJob &currentJob__)
	{
		CM_TRACE_FUNC("as_writeCurvesData.cpp::write("<<pData->getFullPathName()<<","<<currentJob__.name.asChar()<<",...)");

		//_s("// Clip Plane "<<pData->getFullPathName());
	}
}//namespace appleseed