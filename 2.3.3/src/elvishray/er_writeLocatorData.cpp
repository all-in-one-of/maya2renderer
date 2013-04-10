#include <liqConfig.h>
#ifdef _USE_ELVISHRAY_

#include "er_renderer.h"
#include <liqRibLocatorData.h>
#include "../common/prerequest_maya.h"
#include "../common/mayacheck.h"
#include "ercall.h"
#include "er_log_helper.h"
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
	static void _write(liqRibLocatorData* pData, const structJob &currentJob__)
	{
		CM_TRACE_FUNC("er_writeLocatorData.cpp::write("<<pData->getFullPathName()<<","<<currentJob__.name.asChar()<<",...)");

		_s("\n\n\n//locator "<<pData->getFullPathName());

#ifdef TRANSFORM_SHAPE_PAIR
		const std::string objectName(pData->getFullPathName());//shape
#else// SHAPE SHAPE_object PAIR
		const std::string objectName(getObjectName(pData->getFullPathName()));//shape+"_object"
#endif
		_S( ei_object( "poly", objectName.c_str() ) );
		_s("{");
		_d( eiTag tag );

		_s("//vertex positions" );
		_d( tag = ei_tab(EI_TYPE_VECTOR, 1024) )
		_S( ei_pos_list( tag ) );
		_s("// empty list" );
		_S( ei_end_tab() );

// 		_s("//uv" );
// 		_d( tag = eiNULL_TAG );
// 		_S( ei_declare("uv", EI_VARYING, EI_TYPE_TAG, &tag) );
// 		_d( tag = ei_tab(EI_TYPE_VECTOR2, 1024) )
// 		_S( ei_variable("uv", &tag) );
// 		_s("// empty list" );
// 		_S( ei_end_tab() );

		_s("//triangles");
		_d( tag = ei_tab(EI_TYPE_INDEX, 1024) )
		_S( ei_triangle_list( tag ) );
		_s("// empty list" );
		_S( ei_end_tab() );
		_s("}//"<<objectName);
		_S( ei_end_object() );
	}
}//namespace elvishray
#endif//_USE_ELVISHRAY_
