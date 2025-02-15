#include "objExport.h"

#include <trace/trace.hpp>
#include <liqlog.h>
//#define _POSIX_
//#include <limits.h>
#define _POSIX_PATH_MAX     255
//////////////////////////////////////////////////////////////

const char *const objOptionScript = "objExportOptions";
const char *const objDefaultOptions =
    "groups=1;"
    "ptgroups=1;"
    "materials=1;"
    "smoothing=1;"
    "normals=1;"
    ;

//////////////////////////////////////////////////////////////
#if defined (OSMac_)

// Convert file system representations
// Possible styles: kCFURLHFSPathStyle, kCFURLPOSIXPathStyle
// kCFURLHFSPathStyle = Emerald:aw:Maya:projects:default:scenes:eagle.ma
// kCFURLPOSIXPathStyle = /Volumes/Emerald/aw/Maya/projects/default/scenes/eagle.ma
// The conversion will be done in place, so make sure fileName is big enough
// to hold the result
//
static Boolean
convertFileRepresentation (char *fileName, short inStyle, short outStyle)
{
	CM_TRACE_FUNC("convertFileRepresentation("<<fileName<<","<<inStyle<<","<<outStyle<<")");

	if (fileName == NULL) {
		return (false);
	}
	if (inStyle == outStyle) {
		return (true);
	}

	CFStringRef rawPath = CFStringCreateWithCString (NULL, fileName, kCFStringEncodingUTF8);
	if (rawPath == NULL) {
		return (false);
	}
	CFURLRef baseURL = CFURLCreateWithFileSystemPath (NULL, rawPath, (CFURLPathStyle)inStyle, false);
	CFRelease (rawPath);
	if (baseURL == NULL) {
		return (false);
	}
	CFStringRef newURL = CFURLCopyFileSystemPath (baseURL, (CFURLPathStyle)outStyle);
	CFRelease (baseURL);
	if (newURL == NULL) {
		return (false);
	}
	char newPath[MAXPATHLEN];
	CFStringGetCString (newURL, newPath, MAXPATHLEN, kCFStringEncodingUTF8);
	CFRelease (newURL);
	strcpy (fileName, newPath);
	return (true);
}

#endif

void ObjTranslator::write(const MString& fileName)
{	
	CM_TRACE_FUNC("ObjTranslator::write("<<fileName.asChar()<<")");

	beforeExport(fileName);
	exportAll();
	afterExport();
}
void ObjTranslator::write(const MString& fileName, const MString& meshFullPathName)
{
	CM_TRACE_FUNC("ObjTranslator::write("<<fileName.asChar()<<","<<meshFullPathName.asChar()<<")");
	
	if(meshFullPathName.length()==0)
	{
		liquidMessage2(messageError, "meshFullPathName is empty.");
		return;
	}
	beforeExport(fileName);
	exportSelected(meshFullPathName);
	afterExport();
}
MStatus ObjTranslator::beforeExport ( const MString& fileName )
{
	//CM_TRACE_FUNC("ObjTranslator::beforeExport("<<fileName.asChar()<<")");

    MStatus status;
    
    MString mname = fileName, unitName;
    

#if defined (OSMac_)
	char fname[MAXPATHLEN];
	strcpy (fname, file.fullName().asChar());
	FSRef notUsed;
	//Create a file else convertFileRep will fail.
	createMacFile (fname, &notUsed, 0, 0);
	convertFileRepresentation (fname, kCFURLPOSIXPathStyle, kCFURLHFSPathStyle);
    fp = fopen(fname,"wb");//MAYAMACTODO
#else
    const char *fname = mname.asChar();
	// check the file name length
	if( strlen(fname)>=_POSIX_PATH_MAX )//file name is too long
	{
		liquidMessage2( messageError, "file name is too long(>=%d(_POSIX_PATH_MAX)):%s", _POSIX_PATH_MAX, fname );
		assert(0&&"FATAL ERROE: file name is too long!");
	}

    fp = fopen(fname,"w");
#endif

    if (fp == NULL)
    {
        cerr << "Error: The file " << fname << " could not be opened for writing." << endl;
		assert(fp);
        return MS::kFailure;
    }

    // Options
    //
    //set(true, true, true, true, true);

    /* print current linear units used as a comment in the obj file */
    setToLongUnitName(MDistance::uiUnit(), unitName);
    //fprintf( fp, "# This file uses %s as units for non-parametric coordinates.\n\n", unitName.asChar() ); 
    fprintf( fp, "# The units used in this file are %s.\n", unitName.asChar() );
 
	return MS::kSuccess;
}
MStatus ObjTranslator::afterExport()
{
	//CM_TRACE_FUNC("ObjTranslator::afterExport()");

    fclose(fp);

    return MS::kSuccess;
}
//////////////////////////////////////////////////////////////

void ObjTranslator::setToLongUnitName(const MDistance::Unit &unit, MString& unitName)
{
	//CM_TRACE_FUNC("ObjTranslator::setToLongUnitName("<<unit<<","<<unitName.asChar()<<")");

    switch( unit ) 
	{
	case MDistance::kInches:
		/// Inches
		unitName = "inches";
		break;
	case MDistance::kFeet:
		/// Feet
		unitName = "feet";
		break;
	case MDistance::kYards:
		/// Yards
		unitName = "yards";
		break;
	case MDistance::kMiles:
		/// Miles
		unitName = "miles";
		break;
	case MDistance::kMillimeters:
		/// Millimeters
		unitName = "millimeters";
		break;
	case MDistance::kCentimeters:
		/// Centimeters
		unitName = "centimeters";
		break;
	case MDistance::kKilometers:
		/// Kilometers
		unitName = "kilometers";
		break;
	case MDistance::kMeters:
		/// Meters
		unitName = "meters";
		break;
	default:
		break;
	}
}
//////////////////////////////////////////////////////////////

MStatus ObjTranslator::OutputPolygons( 
        MDagPath& mdagPath,
        MObject&  mComponent
)
{
	//CM_TRACE_FUNC("ObjTranslator::OutputPolygons("<<mdagPath.fullPathName().asChar()<<",mComponent)");

	MStatus stat = MS::kSuccess;
	MSpace::Space space = MSpace::kWorld;
	int i;

	MFnMesh fnMesh( mdagPath, &stat );
	if ( MS::kSuccess != stat) {
		fprintf(stderr,"Failure in MFnMesh initialization.\n");
		return MS::kFailure;
	}

	MItMeshPolygon polyIter( mdagPath, mComponent, &stat );
	if ( MS::kSuccess != stat) {
		fprintf(stderr,"Failure in MItMeshPolygon initialization.\n");
		return MS::kFailure;
	}
	MItMeshVertex vtxIter( mdagPath, mComponent, &stat );
	if ( MS::kSuccess != stat) {
		fprintf(stderr,"Failure in MItMeshVertex initialization.\n");
		return MS::kFailure;
	}

	int objectIdx = -1, length;
	MString mdagPathNodeName = fnMesh.name();
	// Find i such that objectGroupsTablePtr[i] corresponds to the
	// object node pointed to by mdagPath
	length = objectNodeNamesArray.length();
	for( i=0; i<length; i++ ) {
		if( objectNodeNamesArray[i] == mdagPathNodeName ) {
			objectIdx = i;
			break;
		}
	}

    // Write out the vertex table
    //
	bool hasMotionBlur = !position.empty();
	//const int N = vtxIter.count();
	//const int M = motion_blur_vertex.size();
	for ( std::size_t pI = 0; !vtxIter.isDone(); ++pI, vtxIter.next() ) 
	{
		MPoint p;
		if(hasMotionBlur)
		{
			p.x = position[ 3*pI ];
			p.y = position[ 3*pI + 1 ];
			p.z = position[ 3*pI + 2 ];
		}else{
			p = vtxIter.position( space );
		}
		if (ptgroups && groups && (objectIdx >= 0)) {
			int compIdx = vtxIter.index();
			outputSetsAndGroups( mdagPath, compIdx, true, objectIdx );
		}
		// convert from internal units to the current ui units
		p.x = MDistance::internalToUI(p.x);
		p.y = MDistance::internalToUI(p.y);
		p.z = MDistance::internalToUI(p.z);
		fprintf(fp,"v %f %f %f\n",p.x,p.y,p.z);
		v++;
	}

    // Write out the uv table
    //
	MFloatArray uArray, vArray;
	fnMesh.getUVs( uArray, vArray );
    int uvLength = uArray.length();
	for ( int x=0; x<uvLength; x++ ) {
		fprintf(fp,"vt %f %f\n",uArray[x],vArray[x]);
		vt++;
	}

    // Write out the normal table
    //
    if ( normals ) {
	    MFloatVectorArray norms;
	    fnMesh.getNormals( norms, MSpace::kWorld );
        int normsLength = norms.length();
	    for ( int t=0; t<normsLength; t++ ) {
	    	MFloatVector tmpf = norms[t];
	    	fprintf(fp,"vn %f %f %f\n",tmpf[0],tmpf[1],tmpf[2]);
	    	vn++;
	    }
    }

    // For each polygon, write out: 
    //    s  smoothing_group
    //    sets/groups the polygon belongs to 
    //    f  vertex_index/uvIndex/normalIndex
    //
    int lastSmoothingGroup = INITIALIZE_SMOOTHING;

	for ( ; !polyIter.isDone(); polyIter.next() )
	{
        // Write out the smoothing group that this polygon belongs to
        // We only write out the smoothing group if it is different
        // from the last polygon.
        //
        if ( smoothing ) {
           	int compIdx = polyIter.index();
            int smoothingGroup = polySmoothingGroups[ compIdx ];
            
            if ( lastSmoothingGroup != smoothingGroup ) {
                if ( NO_SMOOTHING_GROUP == smoothingGroup ) {
                    fprintf(fp,"s off\n");
                }
                else {
                    fprintf(fp,"s %d\n", smoothingGroup );
                }
                lastSmoothingGroup = smoothingGroup;
            }
        }
        
        // Write out all the sets that this polygon belongs to
        //
		if ((groups || materials) && (objectIdx >= 0)) {
			int compIdx = polyIter.index();
			outputSetsAndGroups( mdagPath, compIdx, false, objectIdx );
		}
                
        // Write out vertex/uv/normal index information
        //
		fprintf(fp,"f");
        int polyVertexCount = polyIter.polygonVertexCount();
		for ( int vtx=0; vtx<polyVertexCount; vtx++ ) {
			fprintf(fp," %d", polyIter.vertexIndex( vtx ) +1 +voff);

            bool noUV = true;
			if ( fnMesh.numUVs() > 0 ) {
    			int uvIndex;
                // If the call to getUVIndex fails then there is no
                // mapping information for this polyon so we don't write
                // anything
                //
    			if ( polyIter.getUVIndex(vtx,uvIndex) ) {
    			    fprintf(fp,"/%d",uvIndex+1 +vtoff);
                    noUV = false;
                }
			}
            
			if ( (normals) && (fnMesh.numNormals() > 0) ) {
                if ( noUV ) {
                    // If there are no UVs then our polygon is written
                    // in the form vertex//normal
                    //
                    fprintf(fp,"/");
                }
                fprintf(fp,"/%d",polyIter.normalIndex( vtx ) +1 +vnoff);
            }
		}
		fprintf(fp,"\n");
		fflush(fp);
	}
	return stat;
}
//////////////////////////////////////////////////////////////

void ObjTranslator::outputSetsAndGroups( 
    MDagPath & mdagPath, 
	int cid,
	bool isVertexIterator,
	int objectIdx
	
)
{
	//CM_TRACE_FUNC("ObjTranslator::OutputPolygons("<<mdagPath.fullPathName().asChar()<<","<<cid<<","<<isVertexIterator<<","<<objectIdx<<")");

    MStatus stat;
	
    int i, length;
	MIntArray * currentSets = new MIntArray;
	MIntArray * currentMaterials = new MIntArray;
	MStringArray gArray, mArray;


	if (groups || materials) {

		for ( i=0; i<numSets; i++ )
		{
			if ( lookup(mdagPath,i,cid,isVertexIterator) ) {
			
				MFnSet fnSet( (*sets)[i] );
				if ( MFnSet::kRenderableOnly == fnSet.restriction(&stat) ) {
					currentMaterials->append( i );
					mArray.append( fnSet.name() );
				}
				else {
					currentSets->append( i );
					gArray.append( fnSet.name() );
				}
			}
		}

		if( !isVertexIterator ) {
			// export group nodes (transform DAG nodes) in Maya that
			// the current object is a
			// child/grandchild/grandgrandchild/... of
			bool *objectGroupTable = objectGroupsTablePtr[objectIdx];
			length = transformNodeNameArray.length();
			for( i=0; i<length; i++ ) {
				if( objectGroupTable[i] ) {
					currentSets->append( numSets + i );
					gArray.append(transformNodeNameArray[i]);
				}
			}
		}

		// prevent grouping incoherence, use tav default group schema.
		//
		if (0 == currentSets->length())
		{
			currentSets->append( 0 );
			gArray.append( "default" );
		}
		
					
		// Test for equivalent sets
		//
		bool setsEqual = false;
		if ( (lastSets != NULL) && 
			  (lastSets->length() == currentSets->length())
		) {
			setsEqual = true;
			length = lastSets->length();
			for ( i=0; i<length; i++ )
			{
				if ( (*lastSets)[i] != (*currentSets)[i] ) {
					setsEqual = false;
					break;
				}
			}	
		}

		if ( !setsEqual ) {
			if ( lastSets != NULL )
				delete lastSets;
		
			lastSets = currentSets;		
		
			if (groups) {
				int gLength = gArray.length();
			    if ( gLength > 0  ) {
			        fprintf(fp,"g");
			        for ( i=0; i<gLength; i++ ) {
			            fprintf(fp," %s",gArray[i].asChar());
			        }
			        fprintf(fp,"\n");
			    }
			}
		}
		else
		{
			delete currentSets;
		}

		


		// Test for equivalent materials
		//
		bool materialsEqual = false;
		if ( (lastMaterials != NULL) && 
			  (lastMaterials->length() == currentMaterials->length())
		) {
			materialsEqual = true;
			length = lastMaterials->length();
			for ( i=0; i<length; i++ )
			{
				if ( (*lastMaterials)[i] != (*currentMaterials)[i] ) {
					materialsEqual = false;
					break;
				}
			}			
		}

		if ( !materialsEqual ) {
			if ( lastMaterials != NULL )
				delete lastMaterials;
	
			lastMaterials = currentMaterials;
	
			if (materials) {


				int mLength = mArray.length();

				if ( mLength > 0  ) {
			    	fprintf(fp,"usemtl");
			    	for ( i=0; i<mLength; i++ ) {
			        	fprintf(fp," %s",mArray[i].asChar());
			    	}
			    	fprintf(fp,"\n");
				}
			}
		}
		else
		{
			delete currentMaterials;
		}
	}	
}

//////////////////////////////////////////////////////////////

void ObjTranslator::initializeSetsAndLookupTables( bool exportAll, const MString& meshFullPathName)
//
// Description :
//    Creates a list of all sets in Maya, a list of mesh objects,
//    and polygon/vertex lookup tables that will be used to
//    determine which sets are referenced by the poly components.
//
{
	//CM_TRACE_FUNC("ObjTranslator::initializeSetsAndLookupTables("<<exportAll<<")");
	
	///DEBUG
	//const MString __DebugMeshFullPath("|Man:JAP|Man:JapSkeleton|Man:hips|Man:spine|Man:spine1|Man:xxy|Man:xxx|Man:lS|Man:lA|Man:lFa|Man:lFAR|Man:lH|Man:joint13|Man:lHP1|Man:lHP2|Man:lHP3|Man:pCube11|Man:pCubeShape11");

	int i=0,j=0, length;
	MStatus stat;
	
	// Initialize class data.
	// Note: we cannot do this in the constructor as it
	// only gets called upon registry of the plug-in.
	//
	numSets = 0;
	sets = NULL;
	lastSets = NULL;
	lastMaterials = NULL;
	objectId = 0;
	objectCount = 0;
	polygonTable = NULL;
	vertexTable = NULL;
	polygonTablePtr = NULL;
	vertexTablePtr = NULL;
	objectGroupsTablePtr = NULL;
	objectNodeNamesArray.clear();
	transformNodeNameArray.clear();

	//////////////////////////////////////////////////////////////////
	//
	// Find all sets in Maya and store the ones we care about in
	// the 'sets' array. Also make note of the number of sets.
	//
	//////////////////////////////////////////////////////////////////
	
	// Get all of the sets in maya and put them into
	// a selection list
	// 
	MStringArray result;
	MGlobal::executeCommand( "ls -sets", result );
	MSelectionList * setList = new MSelectionList;
	length = result.length();
	for ( i=0; i<length; i++ )
	{	
		setList->add( result[i] );
	}
	
	// Extract each set as an MObject and add them to the
	// sets array.
	// We may be excluding groups, matierials, or ptGroups
	// in which case we can ignore those sets. 
	//
	MObject mset;
	sets = new MObjectArray();
	length = setList->length();
	for ( i=0; i<length; i++ )
	{
		setList->getDependNode( i, mset );
		
		MFnSet fnSet( mset, &stat );
		if ( stat ) {
			if ( MFnSet::kRenderableOnly == fnSet.restriction(&stat) ) {
				if ( materials ) {
					sets->append( mset );
				}
			} 
			else {
				if ( groups ) {
					sets->append( mset );
				}
			}
		}	
	}
	delete setList;
	
	numSets = sets->length();
			
	//////////////////////////////////////////////////////////////////
	//
	// Do a dag-iteration and for every mesh found, create facet and
	// vertex look-up tables. These tables will keep track of which
	// sets each component belongs to.
	//
	// If exportAll is false then iterate over the activeSelection 
	// list instead of the entire DAG.
	//
	// These arrays have a corrisponding entry in the name
	// stringArray.
	//
	//////////////////////////////////////////////////////////////////
	MIntArray vertexCounts;
	MIntArray polygonCounts;	
			
	if ( exportAll ) {
		MItDag dagIterator( MItDag::kBreadthFirst, MFn::kInvalid, &stat);

    	if ( MS::kSuccess != stat) {
    	    fprintf(stderr,"Failure in DAG iterator setup.\n");
    	    return;
    	}
		
		objectNames = new MStringArray;
		
    	for ( ; !dagIterator.isDone(); dagIterator.next() ) 
		{
    	    MDagPath dagPath;
    	    stat = dagIterator.getPath( dagPath );

			if ( stat ) 
			{
				// skip over intermediate objects
				//
				MFnDagNode dagNode( dagPath, &stat );
				if (dagNode.isIntermediateObject()) 
				{
					continue;
				}

				if (( dagPath.hasFn(MFn::kMesh)) &&
					( dagPath.hasFn(MFn::kTransform)))
				{
					// We want only the shape, 
					// not the transform-extended-to-shape.
					continue;
				}
				else if ( dagPath.hasFn(MFn::kMesh))
				{
					// We have a mesh so create a vertex and polygon table
					// for this object.
					//
					MFnMesh fnMesh( dagPath );
					int vtxCount = fnMesh.numVertices();
					int polygonCount = fnMesh.numPolygons();
					// we do not need this call anymore, we have the shape.
					// dagPath.extendToShape();
					MString name = dagPath.fullPathName();
					///DEBUG
					//if(name==__DebugMeshFullPath){
					//	liquidMessage2(messageInfo,"__DebugMeshFullPath");
					//}
					objectNames->append( name );
					objectNodeNamesArray.append( name );// use fullPathName instead of fnMesh.name()
					
					vertexCounts.append( vtxCount );
					polygonCounts.append( polygonCount );

					objectCount++;
				}
			}
		}	
	}//if ( exportAll )
	else 
	{
		MSelectionList slist;
    	MGlobal::getActiveSelectionList( slist );
    	MItSelectionList iter( slist );
		MStatus status;

		objectNames = new MStringArray;

		// We will need to interate over a selected node's heirarchy
		// in the case where shapes are grouped, and the group is selected.
		MItDag dagIterator( MItDag::kDepthFirst, MFn::kInvalid, &status);

    	for ( ; !iter.isDone(); iter.next() ) 
		{
			MDagPath objectPath;
			stat = iter.getDagPath( objectPath );

			// reset iterator's root node to be the selected node.
			status = dagIterator.reset (objectPath.node(), 
										MItDag::kDepthFirst, MFn::kInvalid );

			// DAG iteration beginning at at selected node
			for ( ; !dagIterator.isDone(); dagIterator.next() )
			{
				MDagPath dagPath;
				MObject  component = MObject::kNullObj;
				status = dagIterator.getPath(dagPath);

				if (!status) {
					fprintf(stderr,"Failure getting DAG path.\n");
					freeLookupTables();
					return ;
				}

                // skip over intermediate objects
                //
                MFnDagNode dagNode( dagPath, &stat );
                if (dagNode.isIntermediateObject()) 
                {
                    continue;
                }


				if (( dagPath.hasFn(MFn::kMesh)) &&
					( dagPath.hasFn(MFn::kTransform)))
				{
					// We want only the shape, 
					// not the transform-extended-to-shape.
					continue;
				}
				else if ( dagPath.hasFn(MFn::kMesh))
				{
					// We have a mesh so create a vertex and polygon table
					// for this object.
					//
					MFnMesh fnMesh( dagPath );
					int vtxCount = fnMesh.numVertices();
					int polygonCount = fnMesh.numPolygons();

					// we do not need this call anymore, we have the shape.
					// dagPath.extendToShape();
					MString name = dagPath.fullPathName();
					///DEBUG
					//if(name==__DebugMeshFullPath){
					//	liquidMessage2(messageInfo,"__DebugMeshFullPath");
					//}
					objectNames->append( name );
					objectNodeNamesArray.append( name );//use fullPathName instead of fnMesh.name()
									
					vertexCounts.append( vtxCount );
					polygonCounts.append( polygonCount );

					objectCount++;	
				}
    		}
		}
	}

	// Now we know how many objects we are dealing with 
	// and we have counts of the vertices/polygons for each
	// object so create the maya group look-up table.
	//
	if( objectCount > 0 ) {

		// To export Maya groups we traverse the hierarchy starting at
		// each objectNodeNamesArray[i] going towards the root collecting transform
		// nodes as we go.
		length = objectNodeNamesArray.length();
		for( i=0; i<length; i++ ) {
			MIntArray transformNodeNameIndicesArray;
			recFindTransformDAGNodes( objectNodeNamesArray[i], transformNodeNameIndicesArray );
		}

		if( transformNodeNameArray.length() > 0 ) {
			objectGroupsTablePtr = (bool**) malloc( sizeof(bool*)*objectCount );
			if(objectGroupsTablePtr==NULL){
				///DEBUG
				//std::cerr <<       "ERROR> ObjTranslator::initializeSetsAndLookupTables(), malloc(sizeof(bool*)*"<<objectCount<<") return NULL."<<std::endl;
				liquidMessage2(messageError,"ObjTranslator::initializeSetsAndLookupTables(), malloc(sizeof(bool*)*%d) return NULL.", objectCount);
			}
			length = transformNodeNameArray.length();
			for ( i=0; i<objectCount; i++ )
			{
				objectGroupsTablePtr[i] =
					(bool*)calloc( length, sizeof(bool) );	
				
				if ( objectGroupsTablePtr[i] == NULL ) {
					///DEBUG
					//std::cerr <<       "ERROR> ObjTranslator::initializeSetsAndLookupTables(), calloc("<<length<<",sizeof(bool*)) return NULL."<<std::endl;
					liquidMessage2(messageError,"ObjTranslator::initializeSetsAndLookupTables(), calloc(%d,sizeof(bool*)) return NULL.",length);
					return;
				}
			}
		}else{
			///DEBUG
			//std::cerr <<       "ERROR> ObjTranslator::initializeSetsAndLookupTables(), objectGroupsTablePtr is not malloc-ed, "<< meshFullPathName.asChar()<<std::endl;
			liquidMessage2(messageError,"ObjTranslator::initializeSetsAndLookupTables(), objectGroupsTablePtr is not malloc-ed, [%s]", meshFullPathName.asChar() );
		}
	}

	// Create the vertex/polygon look-up tables.
	//
	if ( objectCount > 0 ) {
		
		vertexTablePtr = (bool**) malloc( sizeof(bool*)*objectCount );
		polygonTablePtr = (bool**) malloc( sizeof(bool*)*objectCount );
	
		for ( i=0; i<objectCount; i++ )
		{
			vertexTablePtr[i] =
				 (bool*)calloc( vertexCounts[i]*numSets, sizeof(bool) );	

			if ( vertexTablePtr[i] == NULL ) {
				cerr << "Error: calloc returned NULL (vertexTable)\n";
				return;
			}
	
			polygonTablePtr[i] =
				 (bool*)calloc( polygonCounts[i]*numSets, sizeof(bool) );
			if ( polygonTablePtr[i] == NULL ) {
				cerr << "Error: calloc returned NULL (polygonTable)\n";
				return;
			}
		}	
	}

	// If we found no meshes then return
	//	
	if ( objectCount == 0 ) {
		return;
	}
	
	//////////////////////////////////////////////////////////////////
	//
	// Go through all of the set members (flattened lists) and mark
	// in the lookup-tables, the sets that each mesh component belongs
	// to.
	//
	//
	//////////////////////////////////////////////////////////////////
	bool flattenedList = true;
	MDagPath object;
	MObject component;
	MSelectionList memberList;
	
	
	for ( i=0; i<numSets; i++ )
	{
		MFnSet fnSet( (*sets)[i] );		
		memberList.clear();
		stat = fnSet.getMembers( memberList, flattenedList );

		if (MS::kSuccess != stat) {
			fprintf(stderr,"Error in fnSet.getMembers()!\n");
		}

		int m, numMembers;
		numMembers = memberList.length();
		for ( m=0; m<numMembers; m++ )
		{
			if ( memberList.getDagPath(m,object,component) ) {

				if ( (!component.isNull()) && (object.apiType() == MFn::kMesh) )
				{
					if (component.apiType() == MFn::kMeshVertComponent) {
						MItMeshVertex viter( object, component );	
						for ( ; !viter.isDone(); viter.next() )
						{
							int compIdx = viter.index();
							MString name = object.fullPathName();
							
							// Figure out which object vertexTable
							// to get.
							//

							int o, numObjectNames;
							numObjectNames = objectNames->length();
							for ( o=0; o<numObjectNames; o++ ) {
								if ( (*objectNames)[o] == name ) {
									// Mark set i as true in the table
									//		
									vertexTable = vertexTablePtr[o];
									*(vertexTable + numSets*compIdx + i) = true;
									break;
								}
							}
						}
					}
					else if (component.apiType() == MFn::kMeshPolygonComponent) 
					{
						MItMeshPolygon piter( object, component );
						for ( ; !piter.isDone(); piter.next() )
						{
							int compIdx = piter.index();
							MString name = object.fullPathName();
							
							// Figure out which object polygonTable
							// to get.
							//							
							int o, numObjectNames;
							numObjectNames = objectNames->length();
							for ( o=0; o<numObjectNames; o++ ) {
								if ( (*objectNames)[o] == name ) {
									
									// Mark set i as true in the table
									//

// Check for bad components in the set
//									
if ( compIdx >= polygonCounts[o] ) {
	cerr << "Error: component in set >= numPolygons, skipping!\n";
	cerr << "  Component index    = " << compIdx << endl;
	cerr << "  Number of polygons = " << polygonCounts[o] << endl;
	break;
}
									
									polygonTable = polygonTablePtr[o];
									*(polygonTable + numSets*compIdx + i) = true;
									break;
								}
							}	
						}
					}										
				}
				else { 

				// There are no components, therefore we can mark
				// all polygons as members of the given set.
				//

				if (object.hasFn(MFn::kMesh)) {

					MFnMesh fnMesh( object, &stat );
					if ( MS::kSuccess != stat) {
						fprintf(stderr,"Failure in MFnMesh initialization.\n");
						return;
					}

					// We are going to iterate over all the polygons.
					//
					MItMeshPolygon piter( object, MObject::kNullObj, &stat );
					if ( MS::kSuccess != stat) {
						fprintf(stderr,
								"Failure in MItMeshPolygon initialization.\n");
						return;
					}
					for ( ; !piter.isDone(); piter.next() )
					{
						int compIdx = piter.index();
						MString name = object.fullPathName();

						// Figure out which object polygonTable to get.
						//
						int o, numObjectNames;
						numObjectNames = objectNames->length();
						for ( o=0; o<numObjectNames; o++ ) {
							if ( (*objectNames)[o] == name ) {

// Check for bad components in the set
//
if ( compIdx >= polygonCounts[o] ) {
	cerr << "Error: component in set >= numPolygons, skipping!\n";
	cerr << "  Component index    = " << compIdx << endl;
	cerr << "  Number of polygons = " << polygonCounts[o] << endl;
	break;
}
								// Mark set i as true in the table
								//
								polygonTable = polygonTablePtr[o];
								*(polygonTable + numSets*compIdx + i) = true;
								break;
							}
						}
					} // end of piter.next() loop
				} // end of condition if (object.hasFn(MFn::kMesh))
				} // end of else condifion if (!component.isNull()) 
			} // end of memberList.getDagPath(m,object,component)
		} // end of memberList loop
	} // end of for-loop for sets

	if(objectGroupsTablePtr==NULL){
		///DEBUG
		//std::cerr        <<"ERROR> ObjTranslator::initializeSetsAndLookupTables(), objectGroupsTablePtr is NULL, return.  "<< meshFullPathName.asChar()<<std::endl;
		liquidMessage2(messageError,"ObjTranslator::initializeSetsAndLookupTables(), objectGroupsTablePtr is NULL, return.  [%s]",meshFullPathName.asChar());
		return;
	}else{
		///DEBUG
		//std::cout<<"ObjTranslator::initializeSetsAndLookupTables("<<exportAll<<","<< meshFullPathName.asChar()<<")"<<std::endl;
	}
	// Go through all of the group members and mark in the
	// lookup-table, the group that each shape belongs to.
	length = objectNodeNamesArray.length();
	for( i=0; i<length; i++ ) {
		MIntArray groupTableIndicesArray;
		bool *objectGroupTable = objectGroupsTablePtr[i];
		int length2;
		recFindTransformDAGNodes( objectNodeNamesArray[i], groupTableIndicesArray );
		length2 = groupTableIndicesArray.length();
		for( j=0; j<length2; j++ ) {
			int groupIdx = groupTableIndicesArray[j];
			objectGroupTable[groupIdx] = true;
		}
	}
}

//////////////////////////////////////////////////////////////

void ObjTranslator::freeLookupTables()
//
// Frees up all tables and arrays allocated by this plug-in.
//
{
	//CM_TRACE_FUNC("ObjTranslator::freeLookupTables()");

	for ( int i=0; i<objectCount; i++ ) {
		if ( vertexTablePtr[i] != NULL ) {
			free( vertexTablePtr[i] );
		}
		if ( polygonTablePtr[i] != NULL ) {
			free( polygonTablePtr[i] );
		}
	}	

	if( objectGroupsTablePtr != NULL ) {
		for ( int i=0; i<objectCount; i++ ) {
			if ( objectGroupsTablePtr[i] != NULL ) {
				free( objectGroupsTablePtr[i] );
			}
		}
		free( objectGroupsTablePtr );
		objectGroupsTablePtr = NULL;
	}
	
	if ( vertexTablePtr != NULL ) {
		free( vertexTablePtr );
		vertexTablePtr = NULL;
	}
	if ( polygonTablePtr != NULL ) {
		free( polygonTablePtr );
		polygonTablePtr = NULL;
	}

	if ( lastSets != NULL ) {
		delete lastSets;
		lastSets = NULL;
	}
	
	if ( lastMaterials != NULL ) {
		delete lastMaterials;
		lastMaterials = NULL;
	}
	
	if ( sets != NULL ) {
		delete sets;
		sets = NULL;
	}
	
	if ( objectNames != NULL ) {
		delete objectNames;
		objectNames = NULL;
	}		
}

//////////////////////////////////////////////////////////////

bool ObjTranslator::lookup( MDagPath& dagPath, 
							int setIndex,
							int compIdx,
							bool isVtxIter )
{
	//CM_TRACE_FUNC("ObjTranslator::lookup(...)");

	if (isVtxIter) {
		vertexTable = vertexTablePtr[objectId];
		bool ret = *(vertexTable + numSets*compIdx + setIndex);
		return ret;
	}
	else  {				
		polygonTable = polygonTablePtr[objectId];
		bool ret = *(polygonTable + numSets*compIdx + setIndex);			
		return ret;
	}
}	

//////////////////////////////////////////////////////////////

void ObjTranslator::buildEdgeTable( MDagPath& mesh )
{
	//CM_TRACE_FUNC("ObjTranslator::buildEdgeTable(...)");

    if ( !smoothing )
        return;
    
    // Create our edge lookup table and initialize all entries to NULL
    //
    MFnMesh fnMesh( mesh );
    edgeTableSize = fnMesh.numVertices();
    edgeTable = (EdgeInfoPtr*) calloc( edgeTableSize, sizeof(EdgeInfoPtr) );

    // Add entries, for each edge, to the lookup table
    //
    MItMeshEdge eIt( mesh );
    for ( ; !eIt.isDone(); eIt.next() )
    {
        bool smooth = eIt.isSmooth();
        addEdgeInfo( eIt.index(0), eIt.index(1), smooth );
    }

    // Fill in referenced polygons
    //
    MItMeshPolygon pIt( mesh );
    for ( ; !pIt.isDone(); pIt.next() )
    {
        int pvc = pIt.polygonVertexCount();
        for ( int v=0; v<pvc; v++ )
        {
            int a = pIt.vertexIndex( v );
            int b = pIt.vertexIndex( v==(pvc-1) ? 0 : v+1 );

            EdgeInfoPtr elem = findEdgeInfo( a, b );
            if ( NULL != elem ) {
                int edgeId = pIt.index();
                
                if ( INVALID_ID == elem->polyIds[0] ) {
                    elem->polyIds[0] = edgeId;
                }
                else {
                    elem->polyIds[1] = edgeId;
                }                
                    
            }
        }
    }

    // Now create a polyId->smoothingGroup table
    //   
    int numPolygons = fnMesh.numPolygons();
    polySmoothingGroups = (int*)malloc( sizeof(int) *  numPolygons );
    for ( int i=0; i< numPolygons; i++ ) {
        polySmoothingGroups[i] = NO_SMOOTHING_GROUP;
    }    
    
    // Now call the smoothingAlgorithm to fill in the polySmoothingGroups
    // table.
    // Note: we have to traverse ALL polygons to handle the case
    // of disjoint polygons.
    //
    nextSmoothingGroup = 1;
    currSmoothingGroup = 1;
    for ( int pid=0; pid<numPolygons; pid++ ) {
        newSmoothingGroup = true;
        // Check polygon has not already been visited
        if ( NO_SMOOTHING_GROUP == polySmoothingGroups[pid] ) {
           if ( !smoothingAlgorithm(pid,fnMesh) ) {
               // No smooth edges for this polygon so we set
               // the smoothing group to NO_SMOOTHING_GROUP (off)
               polySmoothingGroups[pid] = NO_SMOOTHING_GROUP;
           }
        }
    }
}

//////////////////////////////////////////////////////////////

bool ObjTranslator::smoothingAlgorithm( int polyId, MFnMesh& fnMesh )
{
	//CM_TRACE_FUNC("ObjTranslator::smoothingAlgorithm(...)");

    MIntArray vertexList;
    fnMesh.getPolygonVertices( polyId, vertexList );
    int vcount = vertexList.length();
    bool smoothEdgeFound = false;
    
    for ( int vid=0; vid<vcount;vid++ ) {
        int a = vertexList[vid];
        int b = vertexList[ vid==(vcount-1) ? 0 : vid+1 ];
        
        EdgeInfoPtr elem = findEdgeInfo( a, b );
        if ( NULL != elem ) {
            // NOTE: We assume there are at most 2 polygons per edge
            //       halfEdge polygons get a smoothing group of
            //       NO_SMOOTHING_GROUP which is equivalent to "s off"
            //
            if ( NO_SMOOTHING_GROUP != elem->polyIds[1] ) { // Edge not a border
                
                // We are starting a new smoothing group
                //                
                if ( newSmoothingGroup ) {
                    currSmoothingGroup = nextSmoothingGroup++;
                    newSmoothingGroup = false;
                    
                    // This is a SEED (starting) polygon and so we always
                    // give it the new smoothing group id.
                    // Even if all edges are hard this must be done so
                    // that we know we have visited the polygon.
                    //
                    polySmoothingGroups[polyId] = currSmoothingGroup;
                }
                
                // If we have a smooth edge then this poly must be a member
                // of the current smoothing group.
                //
                if ( elem->smooth ) {
                    polySmoothingGroups[polyId] = currSmoothingGroup;
                    smoothEdgeFound = true;
                }
                else { // Hard edge so ignore this polygon
                    continue;
                }
                
                // Find the adjacent poly id
                //
                int adjPoly = elem->polyIds[0];
                if ( adjPoly == polyId ) {
                    adjPoly = elem->polyIds[1];
                }                             
                
                // If we are this far then adjacent poly belongs in this
                // smoothing group.
                // If the adjacent polygon's smoothing group is not
                // NO_SMOOTHING_GROUP then it has already been visited
                // so we ignore it.
                //
                if ( NO_SMOOTHING_GROUP == polySmoothingGroups[adjPoly] ) {
                    smoothingAlgorithm( adjPoly, fnMesh );
                }
                else if ( polySmoothingGroups[adjPoly] != currSmoothingGroup ) {
                    cerr << "Warning: smoothing group problem at polyon ";
                    cerr << adjPoly << endl;
                }
            }
        }
    }
    return smoothEdgeFound;
}

//////////////////////////////////////////////////////////////

void ObjTranslator::addEdgeInfo( int v1, int v2, bool smooth )
//
// Adds a new edge info element to the vertex table.
//
{
	//CM_TRACE_FUNC("ObjTranslator::addEdgeInfo(...)");

    EdgeInfoPtr element = NULL;

    if ( NULL == edgeTable[v1] ) {
        edgeTable[v1] = (EdgeInfoPtr)malloc( sizeof(struct EdgeInfo) );
        element = edgeTable[v1];
    }
    else {
        element = edgeTable[v1];
        while ( NULL != element->next ) {
            element = element->next;
        }
        element->next = (EdgeInfoPtr)malloc( sizeof(struct EdgeInfo) );
        element = element->next;
    }

    // Setup data for new edge
    //
    element->vertId     = v2;
    element->smooth     = smooth;
    element->next       = NULL;
   
    // Initialize array of id's of polygons that reference this edge.
    // There are at most 2 polygons per edge.
    //
    element->polyIds[0] = INVALID_ID;
    element->polyIds[1] = INVALID_ID;
}

//////////////////////////////////////////////////////////////

EdgeInfoPtr ObjTranslator::findEdgeInfo( int v1, int v2 )
//
// Finds the info for the specified edge.
//
{
	//CM_TRACE_FUNC("ObjTranslator::findEdgeInfo(...)");

    EdgeInfoPtr element = NULL;
    element = edgeTable[v1];

    while ( NULL != element ) {
        if ( v2 == element->vertId ) {
            return element;
        }
        element = element->next;
    }
    
    if ( element == NULL ) {
        element = edgeTable[v2];

        while ( NULL != element ) {
            if ( v1 == element->vertId ) {
                return element;
            }
            element = element->next;
        }
    }

    return NULL;
}

//////////////////////////////////////////////////////////////

void ObjTranslator::destroyEdgeTable()
//
// Free up all of the memory used by the edgeTable.
//
{
	//CM_TRACE_FUNC("ObjTranslator::destroyEdgeTable()");

    if ( !smoothing )
        return;
    
    EdgeInfoPtr element = NULL;
    EdgeInfoPtr tmp = NULL;

    for ( int v=0; v<edgeTableSize; v++ )
    {
        element = edgeTable[v];
        while ( NULL != element )
        {
            tmp = element;
            element = element->next;
            free( tmp );
        }
    }

    if ( NULL != edgeTable ) {
        free( edgeTable );
        edgeTable = NULL;
    }
    
    if ( NULL != polySmoothingGroups ) {
        free( polySmoothingGroups );
        polySmoothingGroups = NULL;
    }
}

//////////////////////////////////////////////////////////////

MStatus ObjTranslator::exportSelected(const MString& meshFullPathName)
{
	//CM_TRACE_FUNC("ObjTranslator::exportSelected("<<meshFullPathName.asChar()<<")");

	MStatus status;
	MString filename;

	initializeSetsAndLookupTables( false, meshFullPathName);

	IfMErrorMsgWarn( MGlobal::selectByName(meshFullPathName, MGlobal::kReplaceList) , meshFullPathName);
	// Create an iterator for the active selection list
	//
	MSelectionList slist;
	IfMErrorWarn( MGlobal::getActiveSelectionList( slist ) );
	MItSelectionList iter( slist );

	if (iter.isDone()) {
	    fprintf(stderr,"Error: Nothing is selected.\n");
	    return MS::kFailure;
	}

    // We will need to interate over a selected node's heirarchy 
    // in the case where shapes are grouped, and the group is selected.
    MItDag dagIterator( MItDag::kDepthFirst, MFn::kInvalid, &status);

	// reset counters
	v = vt = vn = 0;
	voff = vtoff = vnoff = 0;

	// Selection list loop
	for ( ; !iter.isDone(); iter.next() )
	{	 
		MDagPath objectPath;
		// get the selected node
		status = iter.getDagPath( objectPath);

		// reset iterator's root node to be the selected node.
		status = dagIterator.reset (objectPath.node(), 
									MItDag::kDepthFirst, MFn::kInvalid );	

		// DAG iteration beginning at at selected node
		for ( ; !dagIterator.isDone(); dagIterator.next() )
		{
			MDagPath dagPath;
			MObject  component = MObject::kNullObj;
			status = dagIterator.getPath(dagPath);

			if (!status) {
				fprintf(stderr,"Failure getting DAG path.\n");
				freeLookupTables();
				return MS::kFailure;
			}

			if (status ) 
			{
                // skip over intermediate objects
                //
                MFnDagNode dagNode( dagPath, &status );
                if (dagNode.isIntermediateObject()) 
                {
                    continue;
                }

				if (dagPath.hasFn(MFn::kNurbsSurface))
				{
					status = MS::kSuccess;
					fprintf(stderr,"Warning: skipping Nurbs Surface.\n");
				}
				else if ((  dagPath.hasFn(MFn::kMesh)) &&
						 (  dagPath.hasFn(MFn::kTransform)))
				{
					// We want only the shape, 
					// not the transform-extended-to-shape.
					continue;
				}
				else if (  dagPath.hasFn(MFn::kMesh))
				{
					// Build a lookup table so we can determine which 
					// polygons belong to a particular edge as well as
					// smoothing information
					//
					buildEdgeTable( dagPath );
					
					status = OutputPolygons(dagPath, component);
					objectId++;
					if (status != MS::kSuccess) {
						fprintf(stderr, "Error: exporting geom failed, check your selection.\n");
						freeLookupTables();
						destroyEdgeTable(); // Free up the edge table				
						return MS::kFailure;
					}
					destroyEdgeTable(); // Free up the edge table				
				}
				voff = v;
				vtoff = vt;
				vnoff = vn;
			}
		}
	}
	
	freeLookupTables();
	
	return status;
}

//////////////////////////////////////////////////////////////

MStatus ObjTranslator::exportAll( )
{
	//CM_TRACE_FUNC("ObjTranslator::exportAll()");

	MStatus status = MS::kSuccess;

	initializeSetsAndLookupTables( true, "" );

	MItDag dagIterator( MItDag::kBreadthFirst, MFn::kInvalid, &status);

	if ( MS::kSuccess != status) {
	    fprintf(stderr,"Failure in DAG iterator setup.\n");
	    return MS::kFailure;
	}
	// reset counters
	v = vt = vn = 0;
	voff = vtoff = vnoff = 0;

	for ( ; !dagIterator.isDone(); dagIterator.next() )
	{
		MDagPath dagPath;
		MObject  component = MObject::kNullObj;
		status = dagIterator.getPath(dagPath);

		if (!status) {
			fprintf(stderr,"Failure getting DAG path.\n");
			freeLookupTables();
			return MS::kFailure;
		}

		// skip over intermediate objects
		//
		MFnDagNode dagNode( dagPath, &status );
		if (dagNode.isIntermediateObject()) 
		{
			continue;
		}

		if ((  dagPath.hasFn(MFn::kNurbsSurface)) &&
			(  dagPath.hasFn(MFn::kTransform)))
		{
			status = MS::kSuccess;
			fprintf(stderr,"Warning: skipping Nurbs Surface.\n");
		}
		else if ((  dagPath.hasFn(MFn::kMesh)) &&
				 (  dagPath.hasFn(MFn::kTransform)))
		{
			// We want only the shape, 
			// not the transform-extended-to-shape.
			continue;
		}
		else if (  dagPath.hasFn(MFn::kMesh))
		{
            // Build a lookup table so we can determine which 
            // polygons belong to a particular edge as well as
            // smoothing information
            //
            buildEdgeTable( dagPath );
			
            // Now output the polygon information
            //
            status = OutputPolygons(dagPath, component);
			objectId++;
			if (status != MS::kSuccess) {
				fprintf(stderr,"Error: exporting geom failed.\n");
				freeLookupTables();                
                destroyEdgeTable(); // Free up the edge table				
                return MS::kFailure;
            }
            destroyEdgeTable(); // Free up the edge table
		}
		voff = v;
		vtoff = vt;
		vnoff = vn;
	}

	freeLookupTables();

	return status;
}

//////////////////////////////////////////////////////////////

void ObjTranslator::recFindTransformDAGNodes( MString& nodeName, MIntArray& transformNodeIndicesArray )
{
	//CM_TRACE_FUNC("ObjTranslator::recFindTransformDAGNodes(...)");

	// To handle Maya groups we traverse the hierarchy starting at
	// each objectNames[i] going towards the root collecting transform
	// nodes as we go.
	MStringArray result;
	MString cmdStr = "listRelatives -ap -fullPath " + nodeName;
	MGlobal::executeCommand( cmdStr, result );
	
	if( result.length() == 0 )
		// nodeName must be at the root of the DAG.  Stop recursing
		return;

	for( unsigned int j=0; j<result.length(); j++ ) {
		// check if the node result[i] is of type transform
		MStringArray result2;
		MGlobal::executeCommand( "nodeType " + result[j], result2 );
		
		if( result2.length() == 1 && result2[0] == "transform" ) {
			// check if result[j] is already in transformNodeNameArray[]
			bool found=false;
			unsigned int i;
			for( i=0; i<transformNodeNameArray.length(); i++) {
				if( transformNodeNameArray[i] == result[j] ) {
					found = true;
					break;
				}
			}

			if( !found ) {
				//push result[j] to transformNodeNameArray[]
				//and be recorded in transformNodeIndicesArray[]
				transformNodeIndicesArray.append(transformNodeNameArray.length());
				transformNodeNameArray.append(result[j]);
			}
			else {
				transformNodeIndicesArray.append(i);
			}
			recFindTransformDAGNodes(result[j], transformNodeIndicesArray);
		}
	}
}

void ObjTranslator::set(const bool groups, const bool ptgroups, 
	const bool materials,const bool smoothing, const bool normals )
{
	CM_TRACE_FUNC("ObjTranslator::set("<<groups<<","<<ptgroups<<","<<materials<<","<<smoothing<<","<<normals<<")");
	this->groups      = groups; 
	this->ptgroups    = ptgroups; 
	this->materials   = materials;
	this->smoothing   = smoothing; 
	this->normals     = normals; 
}
