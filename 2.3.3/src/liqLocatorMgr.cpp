#include <liqLocatorMgr.h>
#include <liqGlobalHelpers.h>
//
tLocatorMgr::tLocatorMgr()
{

}
//
tLocatorMgr::~tLocatorMgr()
{

}
//
void tLocatorMgr::scanScene(const float lframe__, const int sample__,
						  boost::shared_ptr< liqRibHT > &htable__,
						  int &count__,
						  MStatus &returnStatus__)
{
	CM_TRACE_FUNC("tLocatorMgr::scanScene("<<lframe__<<","<<sample__<<",htable__,count__,returnStatus__)");

	//[refactor 10] beg from scanScene()
	MItDag dagCoordSysIterator( MItDag::kDepthFirst, MFn::kLocator, &returnStatus__);

	for (; !dagCoordSysIterator.isDone(); dagCoordSysIterator.next()) 
	{
#if (Refactoring == 0)
		LIQ_CHECK_CANCEL_REQUEST;
#endif
		MDagPath path;
		MObject currentNode;
		currentNode = dagCoordSysIterator.item();
		MFnDagNode dagNode;
		dagCoordSysIterator.getPath( path );
		if(MS::kSuccess != returnStatus__) 
			continue;
		if(!currentNode.hasFn(MFn::kDagNode)) 
			continue;
		returnStatus__ = dagNode.setObject( currentNode );
		if(MS::kSuccess != returnStatus__) 
			continue;

		// scanScene: if it's a coordinate system then insert it into the hash table
		if( dagNode.typeName() == "liquidCoordSys" ) 
		{
			int coordType = 0;
			MPlug typePlug = dagNode.findPlug( "type", &returnStatus__ );
			if( MS::kSuccess == returnStatus__ ) 
				typePlug.getValue( coordType );

			bool useSamples( ( sample__ > 0 ) && isObjectMotionBlur( path ) );

			ObjectType mrttype = getMRTType(currentNode, coordType);
			ObjectType mrttype_shouldbe = ( coordType == 5 )? MRT_ClipPlane : MRT_Coord;
			if( mrttype != mrttype_shouldbe ){
				liquidMessage2(messageError, "mrttype[%d] should be %d", mrttype_shouldbe);
			}
			htable__->insert( path, 
				lframe__, 
				( useSamples )? sample__ : 0, 
				mrttype, //( coordType == 5 )? MRT_ClipPlane : MRT_Coord, 
				count__++ );
			continue;
		}
	}
	//[refactor 10] end from scanScene()
}