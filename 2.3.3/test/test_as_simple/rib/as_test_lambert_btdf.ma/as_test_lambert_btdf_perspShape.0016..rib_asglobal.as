info   : loaded mesh file E:/MyDocuments/maya/projects/default/as_mesh/pPlane7\pPlaneShape4.obj (1 object, 4 vertices, 2 triangles) in 0 ms.
info   : loaded mesh file E:/MyDocuments/maya/projects/default/as_mesh/pCube1\pCubeShape1.obj (1 object, 8 vertices, 12 triangles) in 0 ms.
info   : loaded mesh file E:/MyDocuments/maya/projects/default/as_mesh/pCube2\pCubeShape2.obj (1 object, 8 vertices, 12 triangles) in 0 ms.
info   : writing project file E:/MyDocuments/maya/projects/default/rib/as_test_lambert_btdf_perspShape.0016..rib.appleseed...
error  : failed to copy pPlaneShape4.obj to E:/MyDocuments/maya/projects/default/rib\pPlaneShape4.obj: boost::filesystem::copy_file: 系统找不到指定的文件。: "pPlaneShape4.obj", "E:/MyDocuments/maya/projects/default/rib\pPlaneShape4.obj".
error  : failed to copy pCubeShape2.obj to E:/MyDocuments/maya/projects/default/rib\pCubeShape2.obj: boost::filesystem::copy_file: 系统找不到指定的文件。: "pCubeShape2.obj", "E:/MyDocuments/maya/projects/default/rib\pCubeShape2.obj".
info   : wrote project file E:/MyDocuments/maya/projects/default/rib/as_test_lambert_btdf_perspShape.0016..rib.appleseed.
info   : collecting light emitters...
info   : found 1 light, 0 emitting triangle.
info   : path tracing settings:
info   :   direct lighting  on
info   :   ibl              on
info   :   caustics         on
info   :   max path length  infinite
info   :   rr min path len. 3
info   :   next event est.  on
info   :   dl light samples 1
info   :   ibl bsdf samples 1
info   :   ibl env samples  1
info   : building assembly tree (1 assembly instance)...
debug  : assembly tree statistics:
debug  :   build time       0 ms
debug  :   size             232 bytes
debug  :   nodes            total 1  interior 0  leaves 1
debug  :   leaf volume      100.0%
debug  :   leaf depth       avg 1.0  min 1  max 1  dev 0.0
debug  :   leaf size        avg 1.0  min 1  max 1  dev 0.0
debug  :   sibling overlap  avg 0.0%  min 0.0%  max 0.0%  dev 0.0%
debug  :   fat leaves       100.0%
debug  : data structures size:
debug  :   bvh::NodeType    128 bytes
debug  :   GTriangleType    36 bytes
debug  :   RegionInfo       32 bytes
debug  :   ShadingPoint     792 bytes
debug  :   ShadingRay       80 bytes
debug  :   TriangleKey      12 bytes
debug  : the scene does not rely on alpha mapping; using probe tracing.
info   : effective subpixel grid size: min: 2x2
debug  : the scene does not rely on alpha mapping; using probe tracing.
info   : effective subpixel grid size: min: 2x2
info   : using 2 threads for rendering.
info   : collecting geometry for triangle tree #366...
debug  : triangle_keys: size 26 (312 bytes)  capacity 26 (312 bytes)  overhead 0 (0 byte)
debug  : triangle_vertex_infos: size 26 (208 bytes)  capacity 26 (208 bytes)  overhead 0 (0 byte)
debug  : triangle_bboxes: size 26 (624 bytes)  capacity 26 (624 bytes)  overhead 0 (0 byte)
info   : building bvh triangle tree #366 (26 triangles)...
debug  : triangle_vertices: size 78 (936 bytes)  capacity 78 (936 bytes)  overhead 0 (0 byte)
debug  : triangle tree #366 statistics:
debug  :   build time       0 ms
debug  :   size             3.2 KB
debug  :   nodes            total 25  interior 12  leaves 13
debug  :   leaf volume      0.0%
debug  :   leaf depth       avg 5.6  min 3  max 8  dev 1.7
debug  :   leaf size        avg 2.0  min 2  max 2  dev 0.0
debug  :   sibling overlap  avg 8.3%  min 0.0%  max 100.0%  dev 27.6%
debug  :   fat leaves       100.0%
debug  :   inter. filters   0
debug  :   nodes alignment  128 bytes
debug  : texture cache statistics:
debug  :   performances     n/a
debug  : intersection statistics:
debug  :   total rays       1,164,851
debug  :   shading rays     673,568 (57.8%)
debug  :   probe rays       491,283 (42.2%)
debug  : region tree access cache statistics:
debug  :   combined         n/a
debug  :   stage-0          n/a
debug  :   stage-1          n/a
debug  : triangle tree access cache statistics:
debug  :   combined         efficiency 100.0%  accesses 1,164,851  hits 1,164,849  misses 2
debug  :   stage-0          efficiency 100.0%  accesses 1,164,851  hits 1,164,849  misses 2
debug  :   stage-1          efficiency 0.0%  accesses 2  hits 0  misses 2
debug  : region kit access cache statistics:
debug  :   combined         efficiency 100.0%  accesses 201,387  hits 201,381  misses 6
debug  :   stage-0          efficiency 100.0%  accesses 201,387  hits 201,381  misses 6
debug  :   stage-1          efficiency 0.0%  accesses 6  hits 0  misses 6
debug  : tessellation access cache statistics:
debug  :   combined         efficiency 100.0%  accesses 201,387  hits 201,381  misses 6
debug  :   stage-0          efficiency 100.0%  accesses 201,387  hits 201,381  misses 6
debug  :   stage-1          efficiency 0.0%  accesses 6  hits 0  misses 6
debug  : path tracing statistics:
debug  :   path count       158,702
debug  :   path length      avg 2.2  min 2  max 10  dev 0.5
debug  : texture store statistics:
debug  :   performances     n/a
debug  :   peak size        0 byte
info   : wrote image file E:/MyDocuments/maya/projects/default/rmanpix/as_test_lambert_btdf.perspShape.16.appleseed.png in 188 ms.
