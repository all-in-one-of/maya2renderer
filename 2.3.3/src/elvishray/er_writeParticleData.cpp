#include "er_renderer.h"
#include <liqRibParticleData.h>
#include <common/prerequest_maya.h>
#include <common/mayacheck.h>
#include "ercall.h"

//#include <liqRibNode.h>
#include <liqRibTranslator.h>

namespace elvishray
{
	static void _write(liqRibParticleData* pData, const structJob &currentJob);

	void Renderer::write(
		/*const*/ liqRibParticleData* pData,
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
	static void _write(liqRibParticleData* pData, const structJob &currentJob__)
	{
		CM_TRACE_FUNC("er_writeParticleData.cpp::write("<<pData->getFullPathName().asChar()<<","<<currentJob__.name.asChar()<<",...)");
		OutputMgr &o = Renderer::getOutputMgr();

		o.a(boost::str(boost::format("particle %s is not implemented yet.")%pData->getFullPathName().asChar()));
		liquidMessage2(messageError, "Particle type is not implemented yet. [%s]", pData->getFullPathName().asChar());
	}
}//namespace elvishray
