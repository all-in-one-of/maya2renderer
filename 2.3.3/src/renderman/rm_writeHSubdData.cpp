#include "rm_renderer.h"
#include <liqRibHierarchicalSubdivisionData.h>
#include "rm_helper.h"
#include <liqGlobalVariable.h>
#include <liqGlobalHelpers.h>

namespace renderman
{
	static void _write(liqRibHierarchicalSubdivisionData* pData, const structJob &currentJob);
	//
	void Renderer::write(
		liqRibHierarchicalSubdivisionData* pData,
		const MString &ribFileName,
		const structJob &currentJob,
		const bool bReference)
	{
		CM_TRACE_FUNC("Renderer::write("<<pData->getFullPathName().asChar()<<","<<ribFileName.asChar()<<","<<currentJob.name.asChar()<<",ref="<<bReference<<")");

		assert(liqglo.m_ribFileOpen &&"rm_writeHSubdData.cpp");

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
	static void _write(liqRibHierarchicalSubdivisionData* pData, const structJob &currentJob__)
	{
		CM_TRACE_FUNC("rm_writeHSubdData.cpp::_write("<<pData->getFullPathName().asChar()<<","<<currentJob__.name.asChar()<<",...)");

		unsigned numTokens( pData->tokenPointerArray.size() );
		boost::scoped_array< RtToken > tokenArray( new RtToken[ numTokens ] );
		boost::scoped_array< RtPointer > pointerArray( new RtPointer[ numTokens ] );
		assignTokenArraysV( pData->tokenPointerArray, tokenArray.get(), pointerArray.get() );

		RiHierarchicalSubdivisionMeshV(	
			pData->m_subdivScheme, 
			pData->numFaces,
			pData->nverts.get(), 
			pData->verts.get(),
			pData->m_subdivNTags,
			pData->m_subdivTags,
			pData->m_subdivNArgs,
			pData->m_subdivIntArgs,
			pData->m_subdivFloatArgs,
			pData->m_subdivStringArgs,
			numTokens,
			tokenArray.get(),
			pointerArray.get() );
	}
}//namespace renderman
