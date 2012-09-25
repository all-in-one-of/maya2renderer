info   : loaded mesh file E:/MyDocuments/maya/projects/default/as_mesh/pCube1\pCubeShape1.obj (1 object, 8 vertices, 12 triangles) in 0 ms.
info   : loaded mesh file E:/MyDocuments/maya/projects/default/as_mesh/pCube1/pCone1\pConeShape1.obj (1 object, 4 vertices, 4 triangles) in 0 ms.
info   : loaded mesh file E:/MyDocuments/maya/projects/default/as_mesh/pCube1/group1/pCube2\pCubeShape2.obj (1 object, 8 vertices, 12 triangles) in 0 ms.
info   : loaded mesh file E:/MyDocuments/maya/projects/default/as_mesh/pCube1/group1/pPlane1\pPlaneShape1.obj (1 object, 4 vertices, 2 triangles) in 0 ms.
info   : writing project file E:/MyDocuments/maya/projects/default/rib/test_multiple_shapes_simple_perspShape.0001..rib.appleseed...
error  : failed to copy pCubeShape1.obj to E:/MyDocuments/maya/projects/default/rib\pCubeShape1.obj: boost::filesystem::copy_file: 系统找不到指定的文件。: "pCubeShape1.obj", "E:/MyDocuments/maya/projects/default/rib\pCubeShape1.obj".
error  : failed to copy pConeShape1.obj to E:/MyDocuments/maya/projects/default/rib\pConeShape1.obj: boost::filesystem::copy_file: 系统找不到指定的文件。: "pConeShape1.obj", "E:/MyDocuments/maya/projects/default/rib\pConeShape1.obj".
error  : failed to copy pCubeShape2.obj to E:/MyDocuments/maya/projects/default/rib\pCubeShape2.obj: boost::filesystem::copy_file: 系统找不到指定的文件。: "pCubeShape2.obj", "E:/MyDocuments/maya/projects/default/rib\pCubeShape2.obj".
error  : failed to copy pPlaneShape1.obj to E:/MyDocuments/maya/projects/default/rib\pPlaneShape1.obj: boost::filesystem::copy_file: 系统找不到指定的文件。: "pPlaneShape1.obj", "E:/MyDocuments/maya/projects/default/rib\pPlaneShape1.obj".
info   : wrote project file E:/MyDocuments/maya/projects/default/rib/test_multiple_shapes_simple_perspShape.0001..rib.appleseed.
info   : collecting light emitters...
info   : found 2 lights, 0 emitting triangle.
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
info   : collecting geometry for triangle tree #23...
debug  : triangle_keys: size 30 (360 bytes)  capacity 30 (360 bytes)  overhead 0 (0 byte)
debug  : triangle_vertex_infos: size 30 (240 bytes)  capacity 30 (240 bytes)  overhead 0 (0 byte)
debug  : triangle_bboxes: size 30 (720 bytes)  capacity 30 (720 bytes)  overhead 0 (0 byte)
info   : building bvh triangle tree #23 (30 triangles)...
debug  : triangle_vertices: size 90 (1.1 KB)  capacity 90 (1.1 KB)  overhead 0 (0 byte)
debug  : triangle tree #23 statistics:
debug  :   build time       0 ms
debug  :   size             3.7 KB
debug  :   nodes            total 21  interior 10  leaves 11
debug  :   leaf volume      16.5%
debug  :   leaf depth       avg 5.2  min 3  max 8  dev 1.7
debug  :   leaf size        avg 2.7  min 2  max 8  dev 1.8
debug  :   sibling overlap  avg 52.2%  min 0.0%  max 100.0%  dev 48.2%
debug  :   fat leaves       81.8%
debug  :   inter. filters   0
debug  :   nodes alignment  64 bytes
debug  : texture cache statistics:
debug  :   performances     n/a
debug  : intersection statistics:
debug  :   total rays       129,570
debug  :   shading rays     122,249 (94.3%)
debug  :   probe rays       7,321 (5.7%)
debug  : region tree access cache statistics:
debug  :   combined         n/a
debug  :   stage-0          n/a
debug  :   stage-1          n/a
debug  : triangle tree access cache statistics:
debug  :   combined         efficiency 100.0%  accesses 129,570  hits 129,568  misses 2
debug  :   stage-0          efficiency 100.0%  accesses 129,570  hits 129,568  misses 2
debug  :   stage-1          efficiency 0.0%  accesses 2  hits 0  misses 2
debug  : region kit access cache statistics:
debug  :   combined         efficiency 99.7%  accesses 2,249  hits 2,242  misses 7
debug  :   stage-0          efficiency 99.7%  accesses 2,249  hits 2,242  misses 7
debug  :   stage-1          efficiency 0.0%  accesses 7  hits 0  misses 7
debug  : tessellation access cache statistics:
debug  :   combined         efficiency 99.7%  accesses 2,249  hits 2,242  misses 7
debug  :   stage-0          efficiency 99.7%  accesses 2,249  hits 2,242  misses 7
debug  :   stage-1          efficiency 0.0%  accesses 7  hits 0  misses 7
debug  : path tracing statistics:
debug  :   path count       2,238
debug  :   path length      avg 2.0  min 2  max 3  dev 0.1
debug  : texture store statistics:
debug  :   performances     n/a
debug  :   peak size        0 byte
info   : wrote image file E:/MyDocuments/maya/projects/default/rmanpix/test_multiple_shapes_simple.perspShape.1.appleseed.png in 47 ms.
