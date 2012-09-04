#include <liqConfig.h>
#ifdef _USE_APPLESEED_

#include "as_bsdfBuilder.h"

#include "../common/prerequest_maya.h"
#include "as_renderer.h"
#include "../renderermgr.h"
#include "../shadergraph/shadermgr.h"

namespace appleseed
{
	BSDFBuilder::BSDFBuilder(const char* nodename)
		:m_nodename(nodename),
		m_assembly(nullptr),
		m_level(0),
		DummyBSDFName("liquidDummyBSDF"),
		DummyBSDFColorName("liquidDummyBSDFColor")
	{
		CM_TRACE_FUNC("BSDFBuilder::BSDFBuilder("<<nodename<<")");

		Renderer* m_renderer = dynamic_cast<appleseed::Renderer*>( liquid::RendererMgr::getInstancePtr()->getRenderer() );
		assert(m_renderer != nullptr );

		m_assembly = m_renderer->getAssembly().get();
		assert(m_assembly != nullptr);

		createDummyBSDFName();
	}
	//
	BSDFBuilder::~BSDFBuilder()
	{
		CM_TRACE_FUNC("BSDFBuilder::~BSDFBuilder()");
	}
	//
	std::string BSDFBuilder::getBSDFName(const std::string &node)const
	{
		CM_TRACE_FUNC("BSDFBuilder::getBSDFName("<<node<<")");
		return node +"_BSDF";
	}
	//
	std::string BSDFBuilder::getPlugBSDFName(const std::string &node, const std::string &plug)const
	{
		CM_TRACE_FUNC("BSDFBuilder::getPlugBSDFName("<<node<<","<<plug<<")");
		if( isOutputPlug(node, plug) )
		{
			return getBSDFName(node);
		}else{
			return node+"_"+plug+"_BSDF";
		}
	}
	std::string BSDFBuilder::getPlugName(const std::string &node, const std::string &plug)const
	{
		CM_TRACE_FUNC("BSDFBuilder::getPlugName("<<node<<","<<plug<<")");
		return node+"_"+plug;
	}
	//
	std::string BSDFBuilder::getPlugBSDF(const std::string &plug, const std::string &bsdfmodel)
	{
		CM_TRACE_FUNC("BSDFBuilder::getPlugBSDF("<<plug<<")");
		if( ! isLinked(m_nodename, plug) )
		{
			return createPlugBRDF(plug, bsdfmodel);
		}
		else if( isLinkedIn(m_nodename, plug) )
		{
			//if the plug is connected in, we  create a BSDFMix to connect 
			//the srcPlugBSDF and this plug
			MStringArray srcNodePlug;
			IfMErrorWarn(MGlobal::executeCommand(("listConnections -destination false -source true -plugs true \""+m_nodename+"."+plug+"\"").c_str(), srcNodePlug));

			MStringArray src;
			srcNodePlug[0].split('.',src);
			MString srcNode(src[0]);
			MString srcPlug(src[1]);

			const std::string srcBSDFName(getPlugBSDFName(srcNode.asChar(), srcPlug.asChar()));

			// create BSDF for plug(not srcPlug!)
			//            +--(bsdf0)-- srcBSDF
			// plugBSDF --|
			//            +--(bsdf1)-- DummyBSDF
			const std::string plugBSDFName(getPlugBSDFName(m_nodename, plug));
			if(m_assembly->bsdfs().get_by_name(plugBSDFName.c_str()) == nullptr)
			{
				m_assembly->bsdfs().insert(
					asr::BSDFMixFactory().create(
					plugBSDFName.c_str(),
					asr::ParamArray()
						.insert("bsdf0", srcBSDFName.c_str())
						.insert("bsdf1", DummyBSDFName.c_str() )
						.insert("weight0", 1.0f)
						.insert("weight1", 0.0f)
					)
				);
			}
			return plugBSDFName;
		}
		else if( isLinkedOut(m_nodename, plug) )
		{
			if( isOutputPlug(m_nodename, plug) )
				return getBSDFName(m_nodename);//just return BSDF name of this node
			else
				return createPlugBRDF(plug, bsdfmodel);//create the BSDF for this plug
		}else{
			liquidMessage2(messageError,"\"%s.%s\"'s connection state is unhandled.", m_nodename.c_str(), plug.c_str());
			return "";
		}
	}
	bool BSDFBuilder::isLinked(const std::string &node, const std::string &plug)const
	{
		CM_TRACE_FUNC("BSDFBuilder::isLinked("<<node<<","<<plug<<")");
		MStringArray connections;
		getlistConnections(node.c_str(), plug.c_str(), connections);
		return connections.length() != 0;
	}
	bool BSDFBuilder::isLinkedOut(const std::string &node, const std::string &plug)const
	{
		CM_TRACE_FUNC("BSDFBuilder::isLinkedOut("<<node<<","<<plug<<")");
		MStringArray desNodePlug;
		IfMErrorWarn(MGlobal::executeCommand(("listConnections -destination true -source false -plugs false \""+node+"."+plug+"\"").c_str(), desNodePlug));
		return desNodePlug.length() != 0;
	}
	bool BSDFBuilder::isLinkedIn(const std::string &node, const std::string &plug)const
	{
		CM_TRACE_FUNC("BSDFBuilder::isLinkedIn("<<node<<","<<plug<<")");
		MStringArray srcNodePlug;
		IfMErrorWarn(MGlobal::executeCommand(("listConnections -destination false -source true -plugs false \""+node+"."+plug+"\"").c_str(), srcNodePlug));
		return srcNodePlug.length() != 0;
	}
	//
	std::string BSDFBuilder::createPlugBRDF(const std::string &plug, const std::string &bsdfmodel)
	{
		CM_TRACE_FUNC("BSDFBuilder::createPlugBRDF("<<plug<<")");
		const std::string bsdfname(getPlugBSDFName(m_nodename, plug));

		if(m_assembly->bsdfs().get_by_name(bsdfname.c_str()) == nullptr)
		{
			//create color
			const std::string bsdfplugname(getPlugName(m_nodename, plug));

			MStatus status;
			MObject mnode;
			getDependNodeByName(mnode, m_nodename.c_str());

			MVector plugvalue;
			IfMErrorWarn(liquidGetPlugValue(mnode, plug.c_str(), plugvalue, status));

			createColor3(m_assembly->colors(), bsdfplugname.c_str(), 
				plugvalue.x, plugvalue.y, plugvalue.z);

			//create bsdf
			_createBSDF(plug, bsdfname, bsdfmodel);
		}
		return bsdfname;
	}
	//
	bool BSDFBuilder::isOutputPlug(const std::string &node, const std::string &plug)const
	{
		CM_TRACE_FUNC("BSDFBuilder::isOutputPlug("<<node<<","<<plug<<")");
		return("outColor"        == plug 
			|| "outTransparency" == plug 
			|| "outGlowColor"    == plug 
			|| "outMatteOpacity "== plug );
	}
	void BSDFBuilder::createDummyBSDFName()
	{
		CM_TRACE_FUNC("BSDFBuilder::createDummyBSDFName()");
		if(m_assembly->bsdfs().get_by_name(DummyBSDFName.c_str()) == nullptr)
		{
			createColor3(m_assembly->colors(), 
				DummyBSDFColorName.c_str(), 0.0f, 0.0f, 0.0f );
			//
			m_assembly->bsdfs().insert(
				asr::LambertianBRDFFactory().create(
				DummyBSDFName.c_str(),
				asr::ParamArray()
					.insert("reflectance", DummyBSDFColorName.c_str())
				)
			);
		}
	}
	//
	void BSDFBuilder::begin()
	{
		CM_TRACE_FUNC("BSDFBuilder::begin()");
		//m_bsdf.clear();
		m_level = 0;
	}
	void BSDFBuilder::addBSDF(const std::string &plug)
	{
		CM_TRACE_FUNC("BSDFBuilder::addBSDF("<<plug<<")");
		//m_bsdf.push_back( getPlugBSDF(plug) );
	}
	void BSDFBuilder::end(const std::string &lastBSDF)
	{
		CM_TRACE_FUNC("BSDFBuilder::end()");
		//createNodeBSDF();
		
		// create the root BSDF for this node,
		// and connect oldBSDF to it.
		if(m_assembly->bsdfs().get_by_name( getBSDFName(m_nodename).c_str() ) == nullptr)
		{
			m_assembly->bsdfs().insert(
				asr::BSDFMixFactory().create(
				getBSDFName(m_nodename).c_str(),
				asr::ParamArray()
				.insert("bsdf0", lastBSDF.c_str())
				.insert("bsdf1", DummyBSDFName.c_str() )
				.insert("weight0", 1.0f)
				.insert("weight1", 0.0f)
				)
				);
		}
	}
	//
	std::string BSDFBuilder::addBSDFToNode(const std::string &bsdf0, const float weight0,
		const std::string &bsdf1, const float weight1)
	{
		CM_TRACE_FUNC("BSDFBuilder::addBSDFToNode("<<bsdf0<<","<<weight0<<","<<bsdf1<<","<<weight1<<")");
		
		m_level++;
		MString strLevel;
		strLevel.set(m_level);

		const std::string BSDFMixName(getBSDFName(m_nodename)+"_L"+strLevel.asChar());
		
		//            +--(bsdf0)-- bsdf0
		// plugBSDF --|
		//            +--(bsdf1)-- bsdf1
		if(m_assembly->bsdfs().get_by_name(BSDFMixName.c_str()) == nullptr)
		{
			m_assembly->bsdfs().insert(
				asr::BSDFMixFactory().create(
				BSDFMixName.c_str(),
				asr::ParamArray()
					.insert("bsdf0",   bsdf0.c_str())
					.insert("bsdf1",   bsdf1.c_str())
					.insert("weight0", weight0)
					.insert("weight1", weight1)//
				)
			);
		}
		return BSDFMixName;
	}
// 	void BSDFBuilder::createNodeBSDF()
// 	{
// 		CM_TRACE_FUNC("BSDFBuilder::createNodeBSDF()");
// 		std::string oldBSDF;
// 
// 		oldBSDF = DummyBSDFName;
// 		for(std::size_t i = 0; i<m_bsdf.size(); ++i)
// 		{
// 			oldBSDF = addBSDFToNode(oldBSDF, 1.0f - 1.0f/(i+1), m_bsdf[i], 1.0f/(i+1));
// 		}
// 	}
	void BSDFBuilder::_createBSDF(const std::string& plug, const std::string& bsdfname, const std::string &model)
	{
		CM_TRACE_FUNC("BSDFBuilder::_createBSDF("<<plug<<","<<bsdfname<<","<<model<<")");

		const std::string bsdfplugname(getPlugName(m_nodename, plug));

		if("lambertian_brdf"==model)
		{
			m_assembly->bsdfs().insert(
				asr::LambertianBRDFFactory().create(
				bsdfname.c_str(),
				asr::ParamArray()
					.insert("reflectance", bsdfplugname.c_str())
				)
			);
		}
		else if("specular_brdf"==model)
		{
			m_assembly->bsdfs().insert(
				asr::SpecularBRDFFactory().create(
				bsdfname.c_str(),
				asr::ParamArray()
					.insert("reflectance", bsdfplugname.c_str())
				)
			);
		}else{
			liquidMessage2( messageError, "\"%s\" is not implemented yet.", model.c_str() );

		}
	}

}//namespace appleseed
#endif//_USE_APPLESEED_