#include <liqConfig.h>
#ifdef _USE_APPLESEED_

#pragma once
#include "prerequest_as.h"
#include "as_material.h"

namespace renderer{
	class Assembly;
}

namespace appleseed
{
	class Renderer;
	//
	class BSDFBuilder
	{
	public:
		BSDFBuilder(const char* nodename);
		~BSDFBuilder();

		std::string getBSDFName(const std::string &node)const;
		std::string getPlugBSDFName(const std::string &node, const std::string &plug)const;
		std::string getPlugName(const std::string &node, const std::string &plug)const;


		std::string getPlugBSDF(const std::string &plug);
		std::string BSDFBuilder::createPlugBRDF(const std::string &plug);

		bool isLinked(const std::string &node, const std::string &plug)const;
		bool isLinkedOut(const std::string &node, const std::string &plug)const;
		bool isLinkedIn(const std::string &node, const std::string &plug)const;
		bool isOutputPlug(const std::string &node, const std::string &plug)const;

		void begin();
		void addBSDF(const std::string &plug);
		void end();
		std::string addBSDFToNode(const std::string &oldBSDF, const std::string &brdf0, 
			const int level);
		void createNodeBSDF();

	protected:
		void createDummyBSDFName();

		asr::Assembly* m_assembly;

		const std::string m_nodename;
		const std::string DummyBSDFName;
		const std::string DummyBSDFColorName;

		std::vector<std::string> m_bsdf;

	};


}//namespace appleseed

#endif//_USE_APPLESEED_
