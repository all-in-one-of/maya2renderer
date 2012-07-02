#include <liqConfig.h>
#ifdef _USE_APPLESEED_

#pragma once
//#ifndef NR_Renderer_H//macro guard for your newrenderer
//#define NR_Renderer_H
#include "prerequest_as.h"
#include "../rendererinterface.h"
#include "../common/logmgr.h"


namespace liquid{
	class LogMgr;
}

class liqGlobalNodeHelper;

namespace appleseed
{
	class GroupMgr;

	class Renderer : public liquid::RendererInterface
	{
	public:
		Renderer();
		virtual ~Renderer();

		virtual void test();
		virtual bool canExport();

		virtual void setRenderScriptFormatAndCompress(const bool bbinary, const bool bcompress);
		//		virtual MStatus ribPrologue(const structJob& currentJob);
		virtual MStatus ribPrologue_begin(const structJob& currentJob);
		virtual MStatus ribPrologue_options(const structJob& currentJob);
		virtual void ribPrologue_comment(const char* liqversion, 
			const char* scenename, const char* user, const time_t &time);
		virtual MStatus framePrologue(const long scanTime,const structJob& currentJob);
		virtual MStatus framePrologue_camera(long lframe, const structJob &currentJob);
		virtual MStatus worldPrologue(const structJob& currentJob);
		//		virtual MStatus lightBlock(const structJob& currentJob);
		//		virtual MStatus coordSysBlock(const structJob& currentJob);
		//		virtual MStatus objectBlock(const structJob& currentJob);
		virtual MStatus worldEpilogue();
		virtual MStatus frameEpilogue(const long scanTime);
		virtual MStatus ribPrologue_end(const structJob& currentJob);
		//		virtual MStatus ribEpilogue();
		virtual MStatus doTextures(const std::vector<structJob> &txtList_);
		virtual MStatus doShadows(const std::vector<structJob> &shadowList_);
		virtual MStatus doRenderView();
		virtual MStatus renderAll_local(const MString& ribFileName);
		virtual MStatus renderAll_remote(const MString& ribFileName);

		virtual void openLog();
		virtual void closeLog();

		//
		//light
		//
		virtual bool writeLight_pre(const liqRibNodePtr& ribNode, const structJob &currentJob);
		//virtual MStatus exportLight(const liqRibNodePtr& light, const structJob &currentJob);
		virtual void writeLight_post(const liqRibNodePtr& ribNode, const structJob &currentJob);		
		virtual MStatus liqRibLightData_write(const liqRibLightData *lightdata, const structJob &currentJob);
		virtual liqLightHandle exportShadowPassLight(
			const std::string &shadertype, 
			const std::string &shaderinstance, 
			const liqString & shadowname,
			const liqMatrix &transform);
		virtual liqLightHandle exportAmbientLight(
			const std::string &shadertype, 
			const std::string &shaderinstance, 
			const liqFloat & intensity,
			const liqColor & color,
			const liqMatrix &transform);
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
			const liqMatrix &transform);
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
			const liqMatrix &transform);
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
			const liqMatrix &transform);
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
			const liqMatrix &transform);
		virtual void transform_(const liqMatrix& transform);
		//
		//
		//
		virtual void exportOneObject_reference(
			const liqRibNodePtr &ribNode__, 
			const structJob &currentJob__
			);
		virtual void exportOneObject_data(
			const liqRibNodePtr &ribNode__, 
			const structJob &currentJob__
			);
		virtual void exportLightLinks(
			const structJob &currentJob__,
			const liqRibNodePtr mesh, 
			const MStringArray& lightedByWhichLightShapes);
		// Shader
		virtual void shader_UserDefinedShader(const liqShader* liqshader);
		virtual void getValidShaderPlugsInShadingGroup(std::vector<std::string>& plugs)const;
		//mesh
		virtual void write(
			/*const*/ liqRibMeshData* meshdata,
			const MString &fileName, 
			const structJob &currentJob,
			const bool bReference);
		virtual void write(
			/*const*/ liqRibLocatorData* meshdata,
			const MString &fileName, 
			const structJob &currentJob,
			const bool bReference);
		virtual void write(
			/*const*/ liqRibPfxHairData* meshdata,
			const MString &fileName, 
			const structJob &currentJob,
			const bool bReference);
		virtual void write(
			/*const*/ liqRibPfxData* data,
			const MString &fileName, 
			const structJob &currentJob,
			const bool bReference);
#ifdef _USE_SHAVE_
		virtual void write(
			/*const*/ liqRibShaveData* data,
			const MString &fileName, 
			const structJob &currentJob,
			const bool bReference);
#endif
		//
		virtual bool isHeroPassReady(const structJob &currentJob);
		virtual void HeroPassBegin(const structJob &currentJob);
		virtual void HeroPassEnd(const structJob &currentJob);

		virtual bool isBaseShadowReady(const structJob &currentJob);
		virtual void BaseShadowBegin(const structJob &currentJob);
		virtual void BaseShadowEnd(const structJob &currentJob);

		virtual bool isShadowPassReady(const structJob &currentJob);
		virtual void ShadowPassBegin(const structJob &currentJob);
		virtual void readBaseShadow(const structJob &currentJob);
		virtual void ShadowPassEnd(const structJob &currentJob);

		virtual void oneObjectBlock_reference_attribute_begin(
			const liqRibNodePtr &ribNode,
			const structJob &currentJob
			);
		virtual void oneObjectBlock_reference_attribute_end(
			const liqRibNodePtr &ribNode,
			const structJob &currentJob
			);
		virtual void oneObjectBlock_reference_attribute_block0(
			const liqRibNodePtr &ribNode,
			const structJob &currentJob );
		virtual void oneObjectBlock_reference_attribute_block1(
			const liqRibNodePtr &ribNode,
			const structJob &currentJob );
		virtual void logFrame(const char* msg);
		virtual void oneObjectBlock_reference_attribute_block2_writeShader_RibBox(const char* msg);
		virtual void oneObjectBlock_reference_attribute_block2_writeShader_RegularShader(
			const liqRibNodePtr &ribNode,
			const structJob &currentJob 
			);
		virtual void oneObjectBlock_reference_attribute_block2_writeShader_HasNoSurfaceShaderORIngoreSurface(
			const liqRibNodePtr &ribNode, const MDagPath &path__, const bool m_shaderDebug
			);
		virtual void oneObjectBlock_reference_attribute_block3_ShadingGroup(
			const MString& meshname
			);
		//
		//std::ofstream& get() { return m_log.get(); }
 		static liquid::LogMgr m_log;
		asf::auto_release_ptr<asr::Project> project;
		asf::auto_release_ptr<asr::Assembly> current_assembly;

	protected:
		Renderer(const Renderer&);
		Renderer& operator=(const Renderer&);


		std::string m_root_group;
		std::string m_option;

		//GroupMgr *m_groupMgr;

 		//void _Script(const std::string &data){ m_log.log___(data); }
		void cookInstanceGroup();
		void dummyPhongShader();//for test only

		void _UserDefinedShader(
			const liqShader &shader,
			const std::vector<liqTokenPointer> &tokenPointerArray
			);
		void _writeObject(
			const liqRibNodePtr& ribNode, 
			const structJob &currentJob,
			const bool bGeometryMotion,
			const unsigned int msampleOn,
			const bool bReference
			);
		virtual void shader_transformBegin(const liqString  shaderSpace);
		virtual void shader_transformEnd(const liqString  shaderSpace);
		virtual void shader_surface(
			const liqShader &shader,
			const std::vector<liqTokenPointer> &tokenPointerArray
			);
		virtual liqLightHandle shader_light(
			const liqShader &shader,
			const std::vector<liqTokenPointer> &tokenPointerArray
			);
		virtual void shader_displacement(
			const liqShader &shader,
			const std::vector<liqTokenPointer> &tokenPointerArray
			);
		virtual void shader_volume(
			const liqShader &shader,
			const std::vector<liqTokenPointer> &tokenPointerArray
			);
		void shader_shadow(
			const liqShader &shader,
			const std::vector<liqTokenPointer> &tokenPointerArray
			);
		void shader_environment(
			const liqShader &shader,
			const std::vector<liqTokenPointer> &tokenPointerArray
			);
		void shader_lens(
			const liqShader &shader,
			const std::vector<liqTokenPointer> &tokenPointerArray
			);
		void gatherCameraShaders(
			MStringArray& cameraShaders,
			const MString& node,
			const std::string& plug_
			);
		void _write_pfxhair(liqRibPfxHairData* pData, const structJob &currentJob__);
		void generate_pfxhair(liqRibNodePtr &ribNode__, liqRibPfxHairData* phair, const int degree);
		void _write_pfx(liqRibPfxData* pData, const structJob &currentJob__);
		void generate_pfx(liqRibNodePtr &ribNode__, liqRibPfxData* phair, const int degree);
		void _write_shavehair(liqRibShaveData* pData, const structJob &currentJob__);
		void generate_shavehair(liqRibNodePtr &ribNode__, liqRibShaveData* pData, const int degree);

		liqGlobalNodeHelper *m_gnode;

		//
		void _writeRef(liqRibMeshData* pData, const structJob &currentJob__);

	};


}//namespace appleseed
//#endif//NR_Renderer_H
#endif//_USE_APPLESEED_