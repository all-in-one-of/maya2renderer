#include "rm_export.h"
#include "shaderOutputRSL.h"
//
std::ofstream& getOutfstreamRef()
{
	return RSL::Visitor::getInstancePtr()->getOutfstreamRef();
}
renderman::NodePlugInfo& getNodePlugInfoRef()
{
	return RSL::Visitor::getInstancePtr()->getNodePlugInfoRef();
}
