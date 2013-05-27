#include "rm_renderer.h"
#include <liqRibImplicitSphereData.h>
#include "rm_helper.h"
#include <liqGlobalVariable.h>

namespace renderman
{
	static void _write(liqRibImplicitSphereData* pData, const structJob &currentJob);
	//
	void Renderer::write(
		liqRibImplicitSphereData* pData,
		const MString &ribFileName,
		const structJob &currentJob,
		const bool bReference)
	{
		CM_TRACE_FUNC("Renderer::write("<<pData->getFullPathName()<<","<<ribFileName.asChar()<<","<<currentJob.name.asChar()<<",ref="<<bReference<<")");

		assert(liqglo.m_ribFileOpen &&"rm_writeImplicitSphereData.cpp");

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
	static void _write(liqRibImplicitSphereData* pData, const structJob &currentJob__)
	{
		CM_TRACE_FUNC("rm_writeImplicitSphereData.cpp::_write("<<pData->getFullPathName()<<","<<currentJob__.name.asChar()<<",...)");

		unsigned numTokens( pData->tokenPointerArray.size() );
		boost::scoped_array< RtToken > tokenArray( new RtToken[ numTokens ] );
		boost::scoped_array< RtPointer > pointerArray( new RtPointer[ numTokens ] );
		assignTokenArraysV( pData->tokenPointerArray, tokenArray.get(), pointerArray.get() );

		RiSphereV( pData->getRadius(), -pData->getRadius(), pData->getRadius(), 360, numTokens, tokenArray.get(), pointerArray.get() );
		LIQDEBUGPRINTF( "-> writing locator" );
	}
}//namespace renderman
