info   : loaded mesh file E:/MyDocuments/maya/projects/default/as_mesh/pPlane8\pPlaneShape5.obj (1 object, 4 vertices, 2 triangles) in 0 ms.
info   : loaded mesh file E:/MyDocuments/maya/projects/default/as_mesh/pPlane9\pPlaneShape6.obj (1 object, 4 vertices, 2 triangles) in 0 ms.
info   : writing project file E:/MyDocuments/maya/projects/default/rib/as_test_spotlight_perspShape.0016..rib.appleseed...
error  : failed to copy pPlaneShape5.obj to E:/MyDocuments/maya/projects/default/rib\pPlaneShape5.obj: boost::filesystem::copy_file: 系统找不到指定的文件。: "pPlaneShape5.obj", "E:/MyDocuments/maya/projects/default/rib\pPlaneShape5.obj".
error  : failed to copy pPlaneShape6.obj to E:/MyDocuments/maya/projects/default/rib\pPlaneShape6.obj: boost::filesystem::copy_file: 系统找不到指定的文件。: "pPlaneShape6.obj", "E:/MyDocuments/maya/projects/default/rib\pPlaneShape6.obj".
info   : wrote project file E:/MyDocuments/maya/projects/default/rib/as_test_spotlight_perspShape.0016..rib.appleseed.
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
info   : effective subpixel grid size: min: 1x1
debug  : the scene does not rely on alpha mapping; using probe tracing.
info   : effective subpixel grid size: min: 1x1
info   : using 2 threads for rendering.
info   : collecting geometry for triangle tree #178...
debug  : triangle_keys: size 4 (48 bytes)  capacity 4 (48 bytes)  overhead 0 (0 byte)
debug  : triangle_vertex_infos: size 4 (32 bytes)  capacity 4 (32 bytes)  overhead 0 (0 byte)
debug  : triangle_bboxes: size 4 (96 bytes)  capacity 4 (96 bytes)  overhead 0 (0 byte)
info   : building bvh triangle tree #178 (4 triangles)...
debug  : triangle_vertices: size 12 (144 bytes)  capacity 12 (144 bytes)  overhead 0 (0 byte)
debug  : triangle tree #178 statistics:
debug  :   build time       0 ms
debug  :   size             480 bytes
debug  :   nodes            total 3  interior 1  leaves 2
debug  :   leaf volume      90.8%
debug  :   leaf depth       avg 2.0  min 2  max 2  dev 0.0
debug  :   leaf size        avg 2.0  min 2  max 2  dev 0.0
debug  :   sibling overlap  avg 9.0%  min 9.0%  max 9.0%  dev 0.0%
debug  :   fat leaves       100.0%
debug  :   inter. filters   0
debug  :   nodes alignment  64 bytes
debug  : texture cache statistics:
debug  :   performances     n/a
debug  : intersection statistics:
debug  :   total rays       1,403,702
debug  :   shading rays     702,104 (50.0%)
debug  :   probe rays       701,598 (50.0%)
debug  : region tree access cache statistics:
debug  :   combined         n/a
debug  :   stage-0          n/a
debug  :   stage-1          n/a
debug  : triangle tree access cache statistics:
debug  :   combined         efficiency 100.0%  accesses 1,403,702  hits 1,403,700  misses 2
debug  :   stage-0          efficiency 100.0%  accesses 1,403,702  hits 1,403,700  misses 2
debug  :   stage-1          efficiency 0.0%  accesses 2  hits 0  misses 2
debug  : region kit access cache statistics:
debug  :   combined         efficiency 100.0%  accesses 280,714  hits 280,710  misses 4
debug  :   stage-0          efficiency 100.0%  accesses 280,714  hits 280,710  misses 4
debug  :   stage-1          efficiency 0.0%  accesses 4  hits 0  misses 4
debug  : tessellation access cache statistics:
debug  :   combined         efficiency 100.0%  accesses 280,714  hits 280,710  misses 4
debug  :   stage-0          efficiency 100.0%  accesses 280,714  hits 280,710  misses 4
debug  :   stage-1          efficiency 0.0%  accesses 4  hits 0  misses 4
debug  : path tracing statistics:
debug  :   path count       196,012
debug  :   path length      avg 2.3  min 2  max 6  dev 0.5
debug  : texture store statistics:
debug  :   performances     n/a
debug  :   peak size        0 byte
info   : wrote image file E:/MyDocuments/maya/projects/default/rmanpix/as_test_spotlight.perspShape.16.appleseed.png in 266 ms.
