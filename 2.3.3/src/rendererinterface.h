#ifndef _RendererInterface_H_
#define _RendererInterface_H_

#include <common/prerequest_std.h>
#include <common/prerequest_maya.h>

#include <liqRibNode.h>
#include <liqRibMeshData.h>
#include <liqTokenPointer.h>
#include <liqRibHT.h>
#include "liqtypes.h"

//struct structJob;
class liqRibLightData;
class liqShader;
class liqRibMeshData;
class liqRibLocatorData;
class liqRibPfxHairData;
class liqRibPfxData;
class liqRibShaveData;
class liqRibClipPlaneData;
class liqRibCoordData;
class liqRibCurvesData;
class liqRibHierarchicalSubdivisionData;
class liqRibImplicitSphereData;
class liqRibMayaSubdivisionData;
class liqRibNuCurveData;
class liqRibParticleData;
class liqRibPfxToonData;
class liqRibSubdivisionData;
class liqRibSurfaceData;
class liqRibCameraData;
class liqPreviewShaderOptions;

namespace liquid
{

class RendererInterface
{
public:
	RendererInterface(){}
	virtual ~RendererInterface() = 0 {};

//interface:
	virtual void test() = 0;
	virtual bool canExport() = 0;
	
	// RenderScript means .rib files for renderman, and .er files for elvishray. and etc.
	virtual void setRenderScriptFormatAndCompress(const bool bbinary, const bool bcompress) = 0;

//	virtual MStatus ribPrologue(const structJob& currentJob) = 0;
	virtual MStatus ribPrologue_begin(const structJob& currentJob) = 0;

	virtual void ribPrologue_comment(const char* liqversion, 
		const char* scenename, const char* user, const time_t &time) = 0;
	virtual MStatus ribPrologue_options(const structJob& currentJob) = 0;
	virtual MStatus framePrologue(const long scanTime,const structJob& currentJob) = 0;
	virtual MStatus framePrologue_camera(long lframe, const structJob &currentJob) = 0;
	virtual MStatus worldPrologue(const structJob& currentJob) = 0;
//	virtual MStatus lightBlock(const structJob& currentJob) = 0;
//	virtual MStatus coordSysBlock(const structJob& currentJob) = 0;
//	virtual MStatus objectBlock(const structJob& currentJob) = 0;
	virtual MStatus worldEpilogue() = 0;
	virtual MStatus frameEpilogue(const long scanTime) = 0;
	virtual MStatus ribPrologue_end(const structJob& currentJob) = 0;
//	virtual MStatus ribEpilogue() = 0;
	virtual MStatus doRenderView() = 0;
	virtual MStatus doTextures(const std::vector<structJob> &txtList_) = 0;
	virtual MStatus doShadows(const std::vector<structJob> &shadowList_) = 0;
	virtual MStatus renderAll_local(const MString& ribFileName) = 0;
	virtual MStatus renderAll_remote(const MString& ribFileName) = 0;

	virtual void openLog(const MString &imageFullPath) = 0;
	virtual void closeLog() = 0;

	virtual MStatus render(const structJob& currentJob) = 0;

	//
	// Light
	//
	virtual bool writeLight_pre(const liqRibNodePtr& ribNode, const structJob &currentJob) = 0;
	//virtual MStatus exportLight(const liqRibNodePtr& light, const structJob &currentJob) = 0;
	virtual void writeLight_post(const liqRibNodePtr& ribNode, const structJob &currentJob) = 0;	
	virtual MStatus liqRibLightData_write(const liqRibLightData *lightdata, const structJob &currentJob) = 0;
	virtual liqLightHandle exportShadowPassLight(
		const std::string &shadertype, 
		const std::string &shaderinstance, 
		const liqString & shadowname,
		const liqMatrix &transform) = 0;
	virtual liqLightHandle exportAmbientLight(
		const std::string &shadertype, 
		const std::string &shaderinstance, 
		const liqFloat & intensity,
		const liqColor & color,
		const liqMatrix &transform) = 0;
	virtual liqLightHandle exportAmbientLight(const liqRibLightData *lightdata, const structJob &currentJob) = 0;
	virtual liqLightHandle exportDistantLight(
		const std::string &shadertype, 
		const std::string &shaderinstance, 
		const liqFloat &i_intensity,
		const liqColor &i_lightcolor,
		const liqString &i_shadowname,
		const liqFloat &i_shadowbias,
		const liqFloat &i_shadowblur,
		const liqFloat &i_shadowsamples,
		const liqFloat &i_shadowfiltersize,
		const liqColor &i_shadowcolor,
		const liqFloat  &i_lightID,
		const liqString &o_category,
		const liqFloat &o_shadowF,
		const liqColor &o_shadowC,
		const liqColor &o_unshadowed_Cl,
		const liqFloat &o_nondiffuse,
		const liqFloat &o_nonspecular,
		const liqMatrix &transform) = 0;
	virtual liqLightHandle exportPointLight(
		const std::string &shadertype, 
		const std::string &shaderinstance, 
		const liqFloat &i_intensity,
		const liqColor &i_lightcolor,
		const liqFloat &i_decay,
		const liqString &i_shadownamepx,
		const liqString &i_shadownamenx,
		const liqString &i_shadownamepy,
		const liqString &i_shadownameny,
		const liqString &i_shadownamepz,
		const liqString &i_shadownamenz,
		const liqFloat &i_shadowbias,
		const liqFloat &i_shadowblur,
		const liqFloat &i_shadowsamples,
		const liqFloat &i_shadowfiltersize,
		const liqColor &i_shadowcolor,
		const liqFloat  &i_lightID,
		const liqString &o_category,
		const liqFloat &o_shadowF,
		const liqColor &o_shadowC,
		const liqColor &o_unshadowed_Cl,
		const liqFloat &o_nondiffuse,
		const liqFloat &o_nonspecular,
		const liqMatrix &transform) = 0;
	virtual liqLightHandle exportSpotLight(
		const std::string &shadertype, 
		const std::string &shaderinstance, 
		const liqFloat &i_intensity,
		const liqColor &i_lightcolor,
		const liqFloat &i_coneangle,
		const liqFloat &i_penumbraangle,
		const liqFloat &i_dropoff,
		const liqFloat &i_decay,

		const liqFloat &i_barndoors,
		const liqFloat &i_leftbarndoor            ,
		const liqFloat &i_rightbarndoor           ,
		const liqFloat &i_topbarndoor             ,
		const liqFloat &i_bottombarndoor          ,

		const liqFloat &i_decayRegions            ,
		const liqFloat &i_startDistance1          ,
		const liqFloat &i_endDistance1            ,
		const liqFloat &i_startDistance2          ,
		const liqFloat &i_endDistance2            ,
		const liqFloat &i_startDistance3          ,
		const liqFloat &i_endDistance3            ,
		const liqFloat &i_startDistanceIntensity1,
		const liqFloat &i_endDistanceIntensity1,
		const liqFloat &i_startDistanceIntensity2,
		const liqFloat &i_endDistanceIntensity2,
		const liqFloat &i_startDistanceIntensity3,
		const liqFloat &i_endDistanceIntensity3,

		const liqString &i_shadowname,
		const liqFloat &i_shadowbias,
		const liqFloat &i_shadowblur,
		const liqFloat &i_shadowsamples,
		const liqFloat &i_shadowfiltersize,
		const liqColor &i_shadowcolor,
		const liqColor &i_shadowcolorsurf,
		const liqFloat &i_shadowcolormix,

		const liqFloat  &i_lightID,
		const liqString &i_category,
		const liqColor &o_shadowC,
		const liqFloat &o_shadowF,
		const liqFloat &o_barn,
		const liqColor &o_unshadowed_Cl,
		const liqFloat &o_nondiffuse,
		const liqFloat &o_nonspecular,
		const liqMatrix &transform) = 0;
	virtual liqLightHandle exportAreaLight(const liqRibLightData *lightdata, const structJob &currentJob) = 0;
	virtual liqLightHandle exportAreaLight(
		const std::string &shadertype, 
		const std::string &shaderinstance, 
		const liqFloat &i_intensity,
		const liqColor &i_lightcolor,
		const liqFloat &i_decay,
		const liqString  &i_coordsys,
		const liqFloat   &i_lightsamples,
		const liqFloat &i_doublesided,
		const liqString &i_shadowname,
		const liqColor &i_shadowcolor,
		const liqString &i_hitmode,
		const liqString &i_lightmap,
		const liqFloat &i_lightmapsaturation,
		const liqFloat  &i_lightID,
		const liqString &i_category,
		const liqFloat &o_nonspecular,
		const liqFloat &o_shadowF,
		const liqColor &o_shadowC,
		const liqColor &o_unshadowed_Cl,
		const liqFloat &o_arealightIntensity,
		const liqColor &o_arealightColor,
		const liqMatrix &transform) = 0;
	virtual liqLightHandle exportVolumeLight(const liqRibLightData *lightdata, const structJob &currentJob) = 0;
	virtual liqLightHandle exportUserDefinedLight(const liqRibLightData *lightdata, const structJob &currentJob) = 0;
	virtual void transform_(const liqMatrix& transform) = 0;
	//
	// Object
	//
	virtual void exportOneObject_reference(
		const liqRibNodePtr &ribNode__, 
		const structJob &currentJob__
		) = 0;
	virtual void exportOneObject_data(
		const liqRibNodePtr &ribNode__, 
		const structJob &currentJob__
		) = 0;
// 	virtual void exportOneGeometry_Mesh(
// 		const liqRibMeshData *mesh, 
// 		const structJob &currentJob
// 		) = 0;
	virtual void exportLightLinks(
		const structJob &currentJob__,
		const liqRibNodePtr mesh, 
		const MStringArray& lightedByWhichLightShapes) = 0;
	// Shader
	virtual void shader_UserDefinedShader(const liqShader* liqshader) = 0;
	virtual void getValidShaderPlugsInShadingGroup(std::vector<std::string>& plugs)const = 0 ;
	//mesh
	virtual void write(
		/*const*/ liqRibMeshData* data,
		const MString &fileName, 
		const structJob &currentJob,
		const bool bReference) = 0;
	virtual void write(
		/*const*/ liqRibLocatorData* data,
		const MString &fileName, 
		const structJob &currentJob,
		const bool bReference) = 0;
	virtual void write(
		/*const*/ liqRibPfxHairData* data,
		const MString &fileName, 
		const structJob &currentJob,
		const bool bReference) = 0;
	virtual void write(
		/*const*/ liqRibPfxData* data,
		const MString &fileName, 
		const structJob &currentJob,
		const bool bReference) = 0;
#ifdef _USE_SHAVE_
	virtual void write(
		/*const*/ liqRibShaveData* data,
		const MString &fileName, 
		const structJob &currentJob,
		const bool bReference) = 0;
#endif
	virtual void write(
		liqRibClipPlaneData* data,
		const MString &fileName, 
		const structJob &currentJob,
		const bool bReference) = 0;
	virtual void write(
		liqRibCoordData* data,
		const MString &fileName, 
		const structJob &currentJob,
		const bool bReference) = 0;
	virtual void write(
		liqRibCurvesData* data,
		const MString &fileName, 
		const structJob &currentJob,
		const bool bReference) = 0;
	virtual void write(
		liqRibHierarchicalSubdivisionData* data,
		const MString &fileName, 
		const structJob &currentJob,
		const bool bReference) = 0;
	virtual void write(
		liqRibImplicitSphereData* data,
		const MString &fileName, 
		const structJob &currentJob,
		const bool bReference) = 0;
	virtual void write(
		liqRibLightData* data,
		const MString &fileName, 
		const structJob &currentJob,
		const bool bReference) = 0;
	virtual void write(
		liqRibMayaSubdivisionData* data,
		const MString &fileName, 
		const structJob &currentJob,
		const bool bReference) = 0;
	virtual void write(
		liqRibNuCurveData* data,
		const MString &fileName, 
		const structJob &currentJob,
		const bool bReference) = 0;
	virtual void write(
		liqRibParticleData* data,
		const MString &fileName, 
		const structJob &currentJob,
		const bool bReference) = 0;
	virtual void write(
		liqRibPfxToonData* data,
		const MString &fileName, 
		const structJob &currentJob,
		const bool bReference) = 0;
	virtual void write(
		liqRibSubdivisionData* data,
		const MString &fileName, 
		const structJob &currentJob,
		const bool bReference) = 0;
	virtual void write(
		liqRibSurfaceData* data,
		const MString &fileName, 
		const structJob &currentJob,
		const bool bReference) = 0;
	virtual void write(
		liqRibCameraData* data,
		const MString &fileName, 
		const structJob &currentJob,
		const bool bReference) = 0;

	//preview
	virtual int preview(const std::string& fileName, const liqPreviewShaderOptions& options) = 0;

	//

	virtual bool isHeroPassReady(const structJob &currentJob) = 0;
	virtual void HeroPassBegin(const structJob &currentJob) = 0;
	virtual void HeroPassEnd(const structJob &currentJob) = 0;

	virtual bool isBaseShadowReady(const structJob &currentJob) = 0;
	virtual void BaseShadowBegin(const structJob &currentJob) = 0;
	virtual void BaseShadowEnd(const structJob &currentJob) = 0;

	virtual bool isShadowPassReady(const structJob &currentJob) = 0;
	virtual void ShadowPassBegin(const structJob &currentJob) = 0;
	virtual void readBaseShadow(const structJob &currentJob) = 0;
	virtual void ShadowPassEnd(const structJob &currentJob) = 0;

	virtual void oneObjectBlock_reference_attribute_begin(
		const liqRibNodePtr &ribNode,
		const structJob &currentJob
		) = 0;
	virtual void oneObjectBlock_reference_attribute_end(
		const liqRibNodePtr &ribNode,
		const structJob &currentJob
		) = 0;
	virtual void oneObjectBlock_reference_attribute_block0(
		const liqRibNodePtr &ribNode,
		const structJob &currentJob
		) = 0;
	virtual void oneObjectBlock_reference_attribute_block1(
		const liqRibNodePtr &ribNode,
		const structJob &currentJob
		) = 0;

	virtual void logFrame(const char* msg) = 0;
	virtual void oneObjectBlock_reference_attribute_block2_writeShader_RibBox(const char* msg) = 0;
	virtual void oneObjectBlock_reference_attribute_block2_writeShader_RegularShader(
		const liqRibNodePtr &ribNode,
		const structJob &currentJob 
		) = 0;
	virtual void oneObjectBlock_reference_attribute_block2_writeShader_HasNoSurfaceShaderORIngoreSurface(
		const liqRibNodePtr &ribNode, const MDagPath &path__, const bool m_shaderDebug
		) = 0;
	virtual void oneObjectBlock_reference_attribute_block3_ShadingGroup(
		const MString& meshname
		) = 0;
	virtual void oneObjectBlock_reference_attribute_block4_strategy(
		const liqRibNodePtr &ribNode
		) = 0;
	virtual MStatus coordSysBlock__(const structJob &currentJob, boost::shared_ptr< liqRibHT > &htable) = 0;
	virtual MStatus preGeometryMel(const MString &m_preGeomRIB)=0;
	//virtual MStatus postGeometryMel();
	virtual void F1(const liqRibNodePtr &ribNode__,  liqShader &currentShader)=0;
	virtual void F2(const bool m_shaderDebug, const liqRibNodePtr &ribNode__)=0;
	virtual void writeUserAttributes(const liqRibNode *ribNode__) = 0;
	virtual void objectBlock_reference_begin() = 0;
	virtual void objectBlock_reference_end() = 0;
	virtual void writeShader_forShadow_ribbox(const MString & text) = 0;
	virtual void writeShader_forShadow_forSpecialTypes(const liqRibNodePtr &ribNode__, bool m_shaderDebug, const MDagPath & path__) = 0;
	virtual void writeShader_forDeepShadow_forSpecialTypes(const liqRibNodePtr &ribNode__, const MDagPath & path__) = 0;
	virtual void writeShader_forShadow_NullShader(const liqRibNodePtr &ribNode__, const MDagPath & path__) = 0;

	virtual MString getTextureExt()const = 0;
	virtual bool isTextureTypeSupported(const MString &textureType)const = 0;


	virtual MStatus iprBegin() = 0;
	virtual MStatus iprEnd() = 0;
	//IPR callback functions
	virtual MStatus IPR_AttributeChangedCallback( MNodeMessage::AttributeMessage msg, 
		MPlug & plug, MPlug & otherPlug, void* userData) = 0;
	virtual MStatus IPR_NodeDirtyCallback( MObject& node,void *userData ) = 0;
	virtual MStatus IPR_NodeDirtyPlugCallback( MObject& node,MPlug& plug,void* userData ) = 0;

	virtual void afterAllFrames() = 0;

protected:


};

}
#endif//_RendererInterface_H_