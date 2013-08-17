#include "er_renderer.h"
#include <liqRibLocatorData.h>
#include <common/prerequest_maya.h>
#include <common/mayacheck.h>
#include "ercall.h"

//#include <liqRibNode.h>
#include <liqRibTranslator.h>

namespace elvishray
{
	static void _write(liqRibLocatorData* pData, const structJob &currentJob);

	void Renderer::write(
		/*const*/ liqRibLocatorData* pData,
		const MString &fileName, 
		const structJob &currentJob,
		const bool bReference)
	{
		CM_TRACE_FUNC("Renderer::write("<<pData->getFullPathName().asChar()<<","<<fileName.asChar()<<","<<currentJob.name.asChar()<<",ref="<<bReference<<")");

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
	static void _write(liqRibLocatorData* pData, const structJob &currentJob__)
	{
		CM_TRACE_FUNC("er_writeLocatorData.cpp::write("<<pData->getFullPathName().asChar()<<","<<currentJob__.name.asChar()<<",...)");
		OutputMgr &o = Renderer::getOutputMgr();

		o.ln();
		o.ln();
		o.ln();
		o.a(boost::str(boost::format("locator %s")%pData->getFullPathName().asChar()));

#ifdef TRANSFORM_SHAPE_PAIR
		const std::string objectName(pData->getFullPathName().asChar());//shape
#else// SHAPE SHAPE_object PAIR
		const std::string objectName(getObjectName(pData->getFullPathName().asChar()));//shape+"_object"
#endif

		std::vector<MVector> POSITION;
		std::vector<MVector> POSITION_mb;//motion blur position
		std::vector<std::size_t> INDEX;//global vertex index
		std::vector<MVector> NORMAL;
		std::vector<MVector> UV;

		o.liq_object(objectName.c_str(),
			POSITION, POSITION_mb, INDEX, NORMAL, UV
			);
	}
}//namespace elvishray
