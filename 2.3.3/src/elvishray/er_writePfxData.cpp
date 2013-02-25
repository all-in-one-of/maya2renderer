#include <liqConfig.h>
#ifdef _USE_ELVISHRAY_

#include "er_renderer.h"
#include <liqRibPfxData.h>
#include "../common/prerequest_maya.h"
#include "../common/mayacheck.h"
#include "ercall.h"
#include "log_helper.h"
//#include <liqRibNode.h>
#include <liqRibTranslator.h>
#include <liqGlobalHelpers.h>
#include <eiCORE/ei_data_table.h>

namespace elvishray
{
	void Renderer::write(
		/*const*/ liqRibPfxData* pData,
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
 			_write_pfx(pData, currentJob);
// 			o.RiEndRef();

		}else{
			//write the reference
			assert(pData->getRibFileFullPath() == fileName);
//			RiReadArchive( const_cast< liqToken >( pData->getRibFileFullPath().asChar() ), NULL, RI_NULL );
		}
	}
	//
	void Renderer::_write_pfx(liqRibPfxData* pData, const structJob &currentJob__)
	{
		CM_TRACE_FUNC("_write_pfx("<<pData->getFullPathName()<<","<<currentJob__.name.asChar()<<")");
		//

		//
 		liqRibNodePtr ribNode__ = liqRibTranslator::getInstancePtr()->htable->find(
 			pData->objDagPath.fullPathName(), 
 			pData->objDagPath,
 			pData->type()
 			);
 		assert( ribNode__!=0 );
 		assert( ribNode__->path().fullPathName() == pData->objDagPath.fullPathName() );

		const bool bMotionBlur =
			ribNode__->motion.transformationBlur &&
			( ribNode__->object( 1 ) ) &&
			//( ribNode__->object(0)->type != MRT_Locator ) && // Why the fuck do we not allow motion blur for locators?
			( currentJob__.pass != rpShadowMap || currentJob__.shadowType == stDeep );

		bool bGeometryMotion = 
			liqglo.liqglo_doDef 
			&& bMotionBlur
			&& ( ribNode__->object(0)->type != MRT_RibGen );

		unsigned int sample_first = 0;
		unsigned int sample_last = bGeometryMotion? (liqglo.liqglo_motionSamples - 1):sample_first;

		_s("\n// Renderer::exportOneGeometry_Mesh("<<ribNode__->name.asChar()<<","<<sample_first<<","<<sample_last<<")");

		const liqRibDataPtr data = ribNode__->object(sample_first)->getDataPtr();

//
		MStatus status;

		int degree;
		MFnDagNode fnDN(data->objDagPath);
		IfMErrorWarn( liquidGetPlugValue(fnDN, "degree", degree, status) );

		// geometry data (shape)
		_s("\n//############################### mesh #");
		_s("//shape full path name="<<data->getFullPathName());
#ifdef TRANSFORM_SHAPE_PAIR
		const std::string objectName(ribNode__->name.asChar());//shape
#else// SHAPE SHAPE_object PAIR
		const std::string objectName(getObjectName(ribNode__->name.asChar()));//shape+"_object"
#endif
		_S( ei_object( "hair", objectName.c_str() ) );
		_s("{");
			_S( ei_degree(degree) );
			this->generate_pfx(ribNode__, pData, degree);
		_s("}//"<<objectName);
		_S( ei_end_object() );
	}
	//
	void Renderer::generate_pfx(liqRibNodePtr &ribNode__, liqRibPfxData* pData, const int degree)
	{
		CM_TRACE_FUNC("generate_pfxhair("<<ribNode__->getTransformNodeFullPath().asChar()<<")");

		MStatus status;
		MFnPfxGeometry pfx(pData->objDagPath, &status);
		IfMErrorWarn(status);

		bool uniformWidth[3] = { false, false, false };

		MRenderLineArray lines[ 3 ];//main/leaf/flower lines

		bool doLines          = true;
		bool doTwist          = true;
		bool doWidth          = true;
		bool doFlatness       = false;
		bool doParameter      = false;
		bool doColor          = true;
		bool doIncandescence  = false;
		bool doTransparency   = true;
		bool doWorldSpace     = false;

		IfMErrorWarn(pfx.getLineData( lines[ 0 ], lines[ 1 ], lines[ 2 ],
										true,  // lines
										true,  // twist
										true,  // width
										true,  // flatness
										false, // parameter
										true,  // color
										true,  // incandescence
										true,  // transparency
										false  // worldSpace
									)
		);
		// default: MRT_PfxTube
		unsigned setOn( 0 );
		if( pData->type() == MRT_PfxLeaf )
			setOn = 1;
		if( pData->type() == MRT_PfxPetal )
			setOn = 2;

		unsigned totalVertex( 0 );
		unsigned totalVarying( 0 );
		unsigned numLines( lines[ setOn ].length() );

		for( unsigned lineOn( 0 ); lineOn < numLines; lineOn++ )
		{
			MRenderLine pfxLine( lines[ setOn ].renderLine( lineOn, &status ) );
			MVectorArray pfxVerts( pfxLine.getLine() );
			totalVarying += pfxVerts.length();
		}
		totalVertex = totalVarying + ( 2 * numLines );

		if( totalVarying )
		{

			_d( eiTag vtx_list );
			_d( vtx_list = ei_tab(EI_TYPE_VECTOR4, 100000) );
			_d( ei_end_tab() );
			_d( eiTag hair_list );
			_d( hair_list = ei_tab(EI_TYPE_INDEX, 100000) );
			_d( ei_end_tab() );
		
			// read other attributes from the lines
			boost::shared_array< liqFloat > uniformCurveWidth(  new liqFloat[ numLines ] );
			boost::shared_array< liqFloat > curveWidth(         new liqFloat[ totalVarying ] );
			liqFloat* uniformWidthPtr( uniformCurveWidth.get() );
			liqFloat* widthPtr( curveWidth.get() );

			bool hasUniformWidth( false );
			bool hasWidth( false );


			//for each hair
			for( unsigned lineOn( 0 ); lineOn < lines[ setOn ].length(); lineOn++ )
			{
				_d( eiInt index ); 
				_d( index = ei_data_table_size(vtx_list) );
				_d( ei_data_table_push_back(hair_list, &index) );//start vertex index of this hair in vtx_list


				MRenderLine pfxLine( lines[ setOn ].renderLine( lineOn, &status ) );
				IfMErrorWarn(status);

				const MVectorArray& pfxVerts( pfxLine.getLine() );
				const MDoubleArray& pfxWidth( pfxLine.getWidth() );
				_d( eiIndex num_segments );
				num_segments = (pfxVerts.length() +2)/degree; _s("num_segments="<< num_segments);
				_d( ei_data_table_push_back(hair_list, &num_segments) );//how many segments this hair contains 
				
				_d( eiVector4 vtx );
				//for each vertex on this hair
				unsigned pOn( 0 );
				for( ; pOn < pfxVerts.length(); pOn++ )
				{
					if( pOn )
					{
						// some vertex in between start and end (pOn > 0)
 						if( ( 1 == setOn ) && ( pOn ==  pfxVerts.length() - 1 ) )
 						{
 							// leaves need to be capped
 							MVector compensate( pfxVerts[ pOn ] - pfxVerts[ pOn - 1 ] );
 							compensate.normalize();
 							const_cast< MVectorArray& >( pfxVerts )[ pOn ] += *( widthPtr - 1 ) * compensate;
 						}
						my_set_eiVector4("vtx", vtx, 
							pfxVerts[ pOn ].x, 
							pfxVerts[ pOn ].y, 
							pfxVerts[ pOn ].z, 
							pfxWidth[ pOn ] * 0.75);
						_d( ei_data_table_push_back(vtx_list, &vtx) );
					}else{
						// start vertices (pOn == 0)
 						if( 1 == setOn )
 						{
 							// leaves need to be capped
 							MVector compensate( pfxVerts[ 1 ] - pfxVerts[ 0 ] );
 							compensate.normalize();
 							const_cast< MVectorArray& >( pfxVerts )[ 0 ] += -*( widthPtr - 1 ) * compensate;
 						}
 						const MVector tmpVertex( pfxVerts[ 0 ] - ( pfxVerts[ 1 ] - pfxVerts[ 0 ] ) );
 						my_set_eiVector4("vtx", vtx, 
 							tmpVertex.x, 
 							tmpVertex.y, 
 							tmpVertex.z, 
 							pfxWidth[ pOn ] * 0.75);
 						_d( ei_data_table_push_back(vtx_list, &vtx) );

						my_set_eiVector4("vtx", vtx, 
							pfxVerts[ 0 ].x, 
							pfxVerts[ 0 ].y, 
							pfxVerts[ 0 ].z, 
							pfxWidth[ 0 ] * 0.75);
						_d( ei_data_table_push_back(vtx_list, &vtx) );
					}
				}//for( ; pOn < pfxVerts.length(); pOn++ ) 
				// end vertex
				// last vertex has already been pushed

				const MVector tmpVertex( pfxVerts[ pOn - 1 ] + ( pfxVerts[ pOn - 1 ] - pfxVerts[ pOn - 2 ] ) );
				const eiScalar width   ( pfxWidth[ pOn - 1 ] + ( pfxWidth[ pOn - 1 ] - pfxWidth[ pOn - 2 ] ) ); ;
				my_set_eiVector4("vtx", vtx, 
					tmpVertex.x, 
					tmpVertex.y, 
					tmpVertex.z, 
					width * 0.75);
				_d( ei_data_table_push_back(vtx_list, &vtx) );
			}//for lineon

			_d( ei_vertex_list(vtx_list) );
			_d( ei_hair_list(hair_list) );

		}//if( totalVarying ) 






		// free memory for lines arrays, are not freed by pfx destructor - Alf
		lines[0].deleteArray();
		lines[1].deleteArray();
		lines[2].deleteArray();
	}

}//namespace elvishray

#endif//_USE_ELVISHRAY_