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


		std::string getPlugBSDF(const std::string &plug, const std::string &bsdfmodel);
		std::string createPlugBRDF(const std::string &plug, const std::string &bsdfmodel);

		bool isLinked(const std::string &node, const std::string &plug)const;
		bool isLinkedOut(const std::string &node, const std::string &plug)const;
		bool isLinkedIn(const std::string &node, const std::string &plug)const;
		bool isOutputPlug(const std::string &node, const std::string &plug)const;

		void begin();
		void addBSDF(const std::string &plug);
		void end(const std::string &lastBSDF);
		std::string addBSDFToNode(const std::string &bsdf0, const float weight0, 
			const std::string &bsdf1, const float weight1);
		//void createNodeBSDF();
		std::string GetOrCreateDummyBSDF();

	protected:
		void _createBSDF(const std::string& plug, const std::string& bsdfname, const std::string &model);

		asr::Assembly* m_assembly;

		const std::string m_nodename;
		const std::string DummyBSDFName;
		const std::string DummyBSDFColorName;

		//std::vector<std::string> m_bsdf;
		std::size_t m_level;

	};


}//namespace appleseed
