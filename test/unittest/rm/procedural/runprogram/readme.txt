- build $(liquid_root)\2.3.3\lib\plugins\src\polygon
- add $(liquid_root)\2.3.3\lib\plugins\ to PATH, so prman(or 3delight) can find these exe and dll files
- add $(liquid_root)\2.3.3\lib\plugins\ to MAYA_SCRIPT_PATH, so maya can find mel scripts (e.g. test_generator.mel);




getAttr pCone1.liqRIBBox;//
getAttr pCone1.liqRIBGenerator;// eval("source test_generator.mel"); test_polygon("pCone1"); // 
getAttr pCone1.liqRIBReadArchive;//
getAttr pCone1.liqRIBDelayedReadArchive;//
getAttr pCone1.liqRIBDelayedReadArchiveBBoxScale;//1
getAttr pCone1.liqProceduralAttribute;//

//getAttr -type pCone1.liqRIBGenerator;//string
//setAttr -type "string" pCone1.liqRIBGenerator "eval(\"source test_generator.mel\"); test_polygon(\"pCone1\");"; // 