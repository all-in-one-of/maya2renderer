#include "rm_export.h"
#include "shaderOutputRSL.h"
#include <renderermgr.h>
#include "rm_factory.h"
//
std::ofstream& getOutfstreamRef()
{
	renderman::Factory* factory = dynamic_cast<renderman::Factory*>(
		liquid::RendererMgr::getInstancePtr()->getFactory("renderman")
	);
	assert(factory);

	RSL::Visitor* visitor = factory->getVisitor();
	assert(visitor);

	return visitor->getOutfstreamRef();

}
renderman::NodePlugInfo& getNodePlugInfoRef()
{
	renderman::Factory* factory = dynamic_cast<renderman::Factory*>(
		liquid::RendererMgr::getInstancePtr()->getFactory("renderman")
		);
	assert(factory);

	RSL::Visitor* visitor = factory->getVisitor();
	assert(visitor);

	return visitor->getNodePlugInfoRef();
}
