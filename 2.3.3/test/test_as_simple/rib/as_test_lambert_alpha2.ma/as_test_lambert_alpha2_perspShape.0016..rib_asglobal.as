info   : loaded mesh file E:/MyDocuments/maya/projects/default/as_mesh/pPlane7\pPlaneShape4.obj (1 object, 4 vertices, 2 triangles) in 0 ms.
info   : loaded mesh file E:/MyDocuments/maya/projects/default/as_mesh/pCube1\pCubeShape1.obj (1 object, 8 vertices, 12 triangles) in 0 ms.
info   : writing project file E:/MyDocuments/maya/projects/default/rib/as_test_lambert_alpha2_perspShape.0016..rib.appleseed...
error  : failed to copy pPlaneShape4.obj to E:/MyDocuments/maya/projects/default/rib\pPlaneShape4.obj: boost::filesystem::copy_file: 系统找不到指定的文件。: "pPlaneShape4.obj", "E:/MyDocuments/maya/projects/default/rib\pPlaneShape4.obj".
info   : wrote project file E:/MyDocuments/maya/projects/default/rib/as_test_lambert_alpha2_perspShape.0016..rib.appleseed.
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
debug  : the scene uses alpha mapping; using standard tracing.
info   : effective subpixel grid size: min: 2x2
debug  : the scene uses alpha mapping; using standard tracing.
info   : effective subpixel grid size: min: 2x2
info   : using 2 threads for rendering.
info   : collecting geometry for triangle tree #57...
debug  : triangle_keys: size 14 (168 bytes)  capacity 14 (168 bytes)  overhead 0 (0 byte)
debug  : triangle_vertex_infos: size 14 (112 bytes)  capacity 14 (112 bytes)  overhead 0 (0 byte)
debug  : triangle_bboxes: size 14 (336 bytes)  capacity 14 (336 bytes)  overhead 0 (0 byte)
info   : building bvh triangle tree #57 (14 triangles)...
debug  : triangle_vertices: size 42 (504 bytes)  capacity 42 (504 bytes)  overhead 0 (0 byte)
debug  : triangle tree #57 statistics:
debug  :   build time       0 ms
debug  :   size             1.7 KB
debug  :   nodes            total 13  interior 6  leaves 7
debug  :   leaf volume      0.0%
debug  :   leaf depth       avg 4.9  min 2  max 7  dev 1.8
debug  :   leaf size        avg 2.0  min 2  max 2  dev 0.0
debug  :   sibling overlap  avg 0.0%  min 0.0%  max 0.0%  dev 0.0%
debug  :   fat leaves       100.0%
debug  :   inter. filters   0
debug  :   nodes alignment  128 bytes
debug  : texture cache statistics:
debug  :   performances     n/a
debug  : intersection statistics:
debug  :   total rays       4,797,206
debug  :   shading rays     4,797,206 (100.0%)
debug  :   probe rays       0 (0.0%)
debug  : region tree access cache statistics:
debug  :   combined         n/a
debug  :   stage-0          n/a
debug  :   stage-1          n/a
debug  : triangle tree access cache statistics:
debug  :   combined         efficiency 100.0%  accesses 4,797,206  hits 4,797,204  misses 2
debug  :   stage-0          efficiency 100.0%  accesses 4,797,206  hits 4,797,204  misses 2
debug  :   stage-1          efficiency 0.0%  accesses 2  hits 0  misses 2
debug  : region kit access cache statistics:
debug  :   combined         efficiency 100.0%  accesses 2,067,826  hits 2,067,822  misses 4
debug  :   stage-0          efficiency 100.0%  accesses 2,067,826  hits 2,067,822  misses 4
debug  :   stage-1          efficiency 0.0%  accesses 4  hits 0  misses 4
debug  : tessellation access cache statistics:
debug  :   combined         efficiency 100.0%  accesses 2,067,826  hits 2,067,822  misses 4
debug  :   stage-0          efficiency 100.0%  accesses 2,067,826  hits 2,067,822  misses 4
debug  :   stage-1          efficiency 0.0%  accesses 4  hits 0  misses 4
debug  : path tracing statistics:
debug  :   path count       583,863
debug  :   path length      avg 2.2  min 2  max 9  dev 0.5
debug  : texture store statistics:
debug  :   performances     n/a
debug  :   peak size        0 byte
info   : wrote image file E:/MyDocuments/maya/projects/default/rmanpix/as_test_lambert_alpha2.perspShape.16.appleseed.png in 141 ms.
