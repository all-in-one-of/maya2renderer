info   : loaded mesh file E:/MyDocuments/maya/projects/default/as_mesh/pPlane7\pPlaneShape4.obj (1 object, 4 vertices, 2 triangles) in 0 ms.
info   : loaded mesh file E:/MyDocuments/maya/projects/default/as_mesh/pCube2\pCubeShape2.obj (1 object, 8 vertices, 12 triangles) in 0 ms.
info   : loaded mesh file E:/MyDocuments/maya/projects/default/as_mesh/pSphere1\pSphereShape1.obj (1 object, 382 vertices, 760 triangles) in 16 ms.
info   : writing project file E:/MyDocuments/maya/projects/default/rib/as_test_lambert_alpha2_perspShape.0016..rib.appleseed...
error  : failed to copy pPlaneShape4.obj to E:/MyDocuments/maya/projects/default/rib\pPlaneShape4.obj: boost::filesystem::copy_file: 系统找不到指定的文件。: "pPlaneShape4.obj", "E:/MyDocuments/maya/projects/default/rib\pPlaneShape4.obj".
error  : failed to copy pCubeShape2.obj to E:/MyDocuments/maya/projects/default/rib\pCubeShape2.obj: boost::filesystem::copy_file: 系统找不到指定的文件。: "pCubeShape2.obj", "E:/MyDocuments/maya/projects/default/rib\pCubeShape2.obj".
error  : failed to copy pSphereShape1.obj to E:/MyDocuments/maya/projects/default/rib\pSphereShape1.obj: boost::filesystem::copy_file: 系统找不到指定的文件。: "pSphereShape1.obj", "E:/MyDocuments/maya/projects/default/rib\pSphereShape1.obj".
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
info   : collecting geometry for triangle tree #332...
debug  : triangle_keys: size 774 (9.1 KB)  capacity 774 (9.1 KB)  overhead 0 (0 byte)
debug  : triangle_vertex_infos: size 774 (6.0 KB)  capacity 774 (6.0 KB)  overhead 0 (0 byte)
debug  : triangle_bboxes: size 774 (18.1 KB)  capacity 774 (18.1 KB)  overhead 0 (0 byte)
info   : building bvh triangle tree #332 (774 triangles)...
debug  : triangle_vertices: size 2,322 (27.2 KB)  capacity 2,322 (27.2 KB)  overhead 0 (0 byte)
debug  : triangle tree #332 statistics:
debug  :   build time       0 ms
debug  :   size             145.0 KB
debug  :   nodes            total 781  interior 390  leaves 391
debug  :   leaf volume      6.4%
debug  :   leaf depth       avg 10.0  min 4  max 11  dev 0.8
debug  :   leaf size        avg 2.0  min 1  max 3  dev 0.4
debug  :   sibling overlap  avg 11.9%  min 0.0%  max 100.0%  dev 19.2%
debug  :   fat leaves       91.8%
debug  :   inter. filters   0
debug  :   nodes alignment  256 bytes
debug  : texture cache statistics:
debug  :   performances     n/a
debug  : intersection statistics:
debug  :   total rays       8,946,074
debug  :   shading rays     8,946,074 (100.0%)
debug  :   probe rays       0 (0.0%)
debug  : region tree access cache statistics:
debug  :   combined         n/a
debug  :   stage-0          n/a
debug  :   stage-1          n/a
debug  : triangle tree access cache statistics:
debug  :   combined         efficiency 100.0%  accesses 8,946,074  hits 8,946,072  misses 2
debug  :   stage-0          efficiency 100.0%  accesses 8,946,074  hits 8,946,072  misses 2
debug  :   stage-1          efficiency 0.0%  accesses 2  hits 0  misses 2
debug  : region kit access cache statistics:
debug  :   combined         efficiency 100.0%  accesses 3,973,787  hits 3,973,781  misses 6
debug  :   stage-0          efficiency 100.0%  accesses 3,973,787  hits 3,973,781  misses 6
debug  :   stage-1          efficiency 0.0%  accesses 6  hits 0  misses 6
debug  : tessellation access cache statistics:
debug  :   combined         efficiency 100.0%  accesses 3,973,787  hits 3,973,781  misses 6
debug  :   stage-0          efficiency 100.0%  accesses 3,973,787  hits 3,973,781  misses 6
debug  :   stage-1          efficiency 0.0%  accesses 6  hits 0  misses 6
debug  : path tracing statistics:
debug  :   path count       1,634,790
debug  :   path length      avg 2.2  min 2  max 7  dev 0.4
debug  : texture store statistics:
debug  :   performances     n/a
debug  :   peak size        0 byte
info   : wrote image file E:/MyDocuments/maya/projects/default/rmanpix/as_test_lambert_alpha2.perspShape.16.appleseed.png in 281 ms.
