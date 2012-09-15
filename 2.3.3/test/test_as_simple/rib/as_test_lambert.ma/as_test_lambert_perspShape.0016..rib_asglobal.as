error  : parameter "sampling_method" not found, using default value "uniform".
info   : loaded mesh file E:/MyDocuments/maya/projects/default/as_mesh/pCube1\pCubeShape1.obj (1 object, 8 vertices, 12 triangles) in 0 ms.
info   : loaded mesh file E:/MyDocuments/maya/projects/default/as_mesh/pCube2\pCubeShape2.obj (1 object, 8 vertices, 10 triangles) in 0 ms.
info   : loaded mesh file E:/MyDocuments/maya/projects/default/as_mesh/pPlane4\pPlaneShape1.obj (1 object, 4 vertices, 2 triangles) in 0 ms.
info   : loaded mesh file E:/MyDocuments/maya/projects/default/as_mesh/pPlane5\pPlaneShape2.obj (1 object, 4 vertices, 2 triangles) in 0 ms.
info   : loaded mesh file E:/MyDocuments/maya/projects/default/as_mesh/pPlane6\pPlaneShape3.obj (1 object, 4 vertices, 2 triangles) in 0 ms.
info   : writing project file E:/MyDocuments/maya/projects/default/rib/as_test_lambert_perspShape.0016..rib.appleseed...
error  : failed to copy pCubeShape2.obj to E:/MyDocuments/maya/projects/default/rib\pCubeShape2.obj: boost::filesystem::copy_file: 系统找不到指定的文件。: "pCubeShape2.obj", "E:/MyDocuments/maya/projects/default/rib\pCubeShape2.obj".
info   : wrote project file E:/MyDocuments/maya/projects/default/rib/as_test_lambert_perspShape.0016..rib.appleseed.
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
info   : collecting geometry for triangle tree #75...
debug  : triangle_keys: size 28 (336 bytes)  capacity 28 (336 bytes)  overhead 0 (0 byte)
debug  : triangle_vertex_infos: size 28 (224 bytes)  capacity 28 (224 bytes)  overhead 0 (0 byte)
debug  : triangle_bboxes: size 28 (672 bytes)  capacity 28 (672 bytes)  overhead 0 (0 byte)
info   : building bvh triangle tree #75 (28 triangles)...
debug  : triangle_vertices: size 84 (1,008 bytes)  capacity 84 (1,008 bytes)  overhead 0 (0 byte)
debug  : triangle tree #75 statistics:
debug  :   build time       0 ms
debug  :   size             3.5 KB
debug  :   nodes            total 25  interior 12  leaves 13
debug  :   leaf volume      3.2%
debug  :   leaf depth       avg 5.4  min 3  max 8  dev 1.5
debug  :   leaf size        avg 2.2  min 2  max 4  dev 0.5
debug  :   sibling overlap  avg 1.6%  min 0.0%  max 19.7%  dev 5.5%
debug  :   fat leaves       92.3%
debug  :   inter. filters   0
debug  :   nodes alignment  128 bytes
debug  : texture cache statistics:
debug  :   performances     n/a
debug  : intersection statistics:
debug  :   total rays       2,511,840
debug  :   shading rays     672,687 (26.8%)
debug  :   probe rays       1,839,153 (73.2%)
debug  : region tree access cache statistics:
debug  :   combined         n/a
debug  :   stage-0          n/a
debug  :   stage-1          n/a
debug  : triangle tree access cache statistics:
debug  :   combined         efficiency 100.0%  accesses 2,511,840  hits 2,511,838  misses 2
debug  :   stage-0          efficiency 100.0%  accesses 2,511,840  hits 2,511,838  misses 2
debug  :   stage-1          efficiency 0.0%  accesses 2  hits 0  misses 2
debug  : region kit access cache statistics:
debug  :   combined         efficiency 100.0%  accesses 373,374  hits 373,364  misses 10
debug  :   stage-0          efficiency 100.0%  accesses 373,374  hits 373,364  misses 10
debug  :   stage-1          efficiency 0.0%  accesses 10  hits 0  misses 10
debug  : tessellation access cache statistics:
debug  :   combined         efficiency 100.0%  accesses 373,374  hits 373,364  misses 10
debug  :   stage-0          efficiency 100.0%  accesses 373,374  hits 373,364  misses 10
debug  :   stage-1          efficiency 0.0%  accesses 10  hits 0  misses 10
debug  : path tracing statistics:
debug  :   path count       124,168
debug  :   path length      avg 2.9  min 2  max 8  dev 0.7
debug  : texture store statistics:
debug  :   performances     n/a
debug  :   peak size        0 byte
info   : wrote image file E:/MyDocuments/maya/projects/default/rmanpix/as_test_lambert.perspShape.16.appleseed.png in 344 ms.
