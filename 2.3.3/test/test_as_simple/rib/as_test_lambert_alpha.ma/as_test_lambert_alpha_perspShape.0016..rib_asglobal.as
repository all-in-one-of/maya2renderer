info   : loaded mesh file E:/MyDocuments/maya/projects/default/as_mesh/pPlane6\pPlaneShape3.obj (1 object, 4 vertices, 2 triangles) in 0 ms.
info   : loaded mesh file E:/MyDocuments/maya/projects/default/as_mesh/pPlane7\pPlaneShape4.obj (1 object, 4 vertices, 2 triangles) in 0 ms.
info   : writing project file E:/MyDocuments/maya/projects/default/rib/as_test_lambert_alpha_perspShape.0016..rib.appleseed...
error  : failed to copy pPlaneShape4.obj to E:/MyDocuments/maya/projects/default/rib\pPlaneShape4.obj: boost::filesystem::copy_file: 系统找不到指定的文件。: "pPlaneShape4.obj", "E:/MyDocuments/maya/projects/default/rib\pPlaneShape4.obj".
info   : wrote project file E:/MyDocuments/maya/projects/default/rib/as_test_lambert_alpha_perspShape.0016..rib.appleseed.
info   : opening texture file E:/dev/Autodesk/maya/myplugin/project/liquid_/2.3.3/test/test_as_simple/check11.png.exr for reading...
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
info   : effective subpixel grid size: min: 1x1
debug  : the scene uses alpha mapping; using standard tracing.
info   : effective subpixel grid size: min: 1x1
info   : using 2 threads for rendering.
info   : collecting geometry for triangle tree #197...
debug  : triangle_keys: size 4 (48 bytes)  capacity 4 (48 bytes)  overhead 0 (0 byte)
debug  : triangle_vertex_infos: size 4 (32 bytes)  capacity 4 (32 bytes)  overhead 0 (0 byte)
debug  : triangle_bboxes: size 4 (96 bytes)  capacity 4 (96 bytes)  overhead 0 (0 byte)
info   : building bvh triangle tree #197 (4 triangles)...
debug  : triangle_vertices: size 12 (144 bytes)  capacity 12 (144 bytes)  overhead 0 (0 byte)
debug  : triangle tree #197 statistics:
debug  :   build time       0 ms
debug  :   size             480 bytes
debug  :   nodes            total 3  interior 1  leaves 2
debug  :   leaf volume      0.0%
debug  :   leaf depth       avg 2.0  min 2  max 2  dev 0.0
debug  :   leaf size        avg 2.0  min 2  max 2  dev 0.0
debug  :   sibling overlap  avg 0.0%  min 0.0%  max 0.0%  dev 0.0%
debug  :   fat leaves       100.0%
debug  :   inter. filters   1
debug  :   nodes alignment  64 bytes
debug  : texture cache statistics:
debug  :   performances     efficiency 100.0%  accesses 64,233  hits 64,210  misses 23
debug  : intersection statistics:
debug  :   total rays       925,085
debug  :   shading rays     925,085 (100.0%)
debug  :   probe rays       0 (0.0%)
debug  : region tree access cache statistics:
debug  :   combined         n/a
debug  :   stage-0          n/a
debug  :   stage-1          n/a
debug  : triangle tree access cache statistics:
debug  :   combined         efficiency 100.0%  accesses 925,085  hits 925,083  misses 2
debug  :   stage-0          efficiency 100.0%  accesses 925,085  hits 925,083  misses 2
debug  :   stage-1          efficiency 0.0%  accesses 2  hits 0  misses 2
debug  : region kit access cache statistics:
debug  :   combined         efficiency 100.0%  accesses 129,918  hits 129,914  misses 4
debug  :   stage-0          efficiency 100.0%  accesses 129,918  hits 129,914  misses 4
debug  :   stage-1          efficiency 0.0%  accesses 4  hits 0  misses 4
debug  : tessellation access cache statistics:
debug  :   combined         efficiency 100.0%  accesses 129,918  hits 129,914  misses 4
debug  :   stage-0          efficiency 100.0%  accesses 129,918  hits 129,914  misses 4
debug  :   stage-1          efficiency 0.0%  accesses 4  hits 0  misses 4
debug  : path tracing statistics:
debug  :   path count       129,756
debug  :   path length      avg 2.0  min 2  max 2  dev 0.0
debug  : texture store statistics:
debug  :   performances     efficiency 47.8%  accesses 23  hits 11  misses 12
debug  :   peak size        96.5 KB
info   : wrote image file E:/MyDocuments/maya/projects/default/rmanpix/as_test_lambert_alpha.perspShape.16.appleseed.png in 187 ms.
