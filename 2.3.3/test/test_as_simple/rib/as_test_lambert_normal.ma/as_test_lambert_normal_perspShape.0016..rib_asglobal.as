info   : loaded mesh file E:/MyDocuments/maya/projects/default/as_mesh/pPlane7\pPlaneShape4.obj (1 object, 4 vertices, 2 triangles) in 0 ms.
info   : writing project file E:/MyDocuments/maya/projects/default/rib/as_test_lambert_normal_perspShape.0016..rib.appleseed...
error  : failed to copy pPlaneShape4.obj to E:/MyDocuments/maya/projects/default/rib\pPlaneShape4.obj: boost::filesystem::copy_file: 系统找不到指定的文件。: "pPlaneShape4.obj", "E:/MyDocuments/maya/projects/default/rib\pPlaneShape4.obj".
info   : wrote project file E:/MyDocuments/maya/projects/default/rib/as_test_lambert_normal_perspShape.0016..rib.appleseed.
info   : opening texture file E:/dev/Autodesk/maya/myplugin/project/liquid_/2.3.3/test/test_shader_graph/texture/mold_normals.jpg.exr for reading...
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
debug  :   leaf volume      n/a
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
warning: color space for normal map "file3" should be "linear_rgb" but is "srgb" instead; expect artifacts and/or slowdowns.
info   : collecting geometry for triangle tree #77...
debug  : triangle_keys: size 2 (24 bytes)  capacity 2 (24 bytes)  overhead 0 (0 byte)
debug  : triangle_vertex_infos: size 2 (16 bytes)  capacity 2 (16 bytes)  overhead 0 (0 byte)
debug  : triangle_bboxes: size 2 (48 bytes)  capacity 2 (48 bytes)  overhead 0 (0 byte)
info   : building bvh triangle tree #77 (2 triangles)...
debug  : triangle_vertices: size 6 (72 bytes)  capacity 6 (72 bytes)  overhead 0 (0 byte)
debug  : triangle tree #77 statistics:
debug  :   build time       0 ms
debug  :   size             224 bytes
debug  :   nodes            total 1  interior 0  leaves 1
debug  :   leaf volume      n/a
debug  :   leaf depth       avg 1.0  min 1  max 1  dev 0.0
debug  :   leaf size        avg 2.0  min 2  max 2  dev 0.0
debug  :   sibling overlap  avg 0.0%  min 0.0%  max 0.0%  dev 0.0%
debug  :   fat leaves       100.0%
debug  :   inter. filters   0
debug  :   nodes alignment  64 bytes
debug  : texture cache statistics:
debug  :   performances     efficiency 100.0%  accesses 262,018  hits 261,970  misses 48
debug  : intersection statistics:
debug  :   total rays       904,608
debug  :   shading rays     575,163 (63.6%)
debug  :   probe rays       329,445 (36.4%)
debug  : region tree access cache statistics:
debug  :   combined         n/a
debug  :   stage-0          n/a
debug  :   stage-1          n/a
debug  : triangle tree access cache statistics:
debug  :   combined         efficiency 100.0%  accesses 904,608  hits 904,606  misses 2
debug  :   stage-0          efficiency 100.0%  accesses 904,608  hits 904,606  misses 2
debug  :   stage-1          efficiency 0.0%  accesses 2  hits 0  misses 2
debug  : region kit access cache statistics:
debug  :   combined         efficiency 100.0%  accesses 219,793  hits 219,791  misses 2
debug  :   stage-0          efficiency 100.0%  accesses 219,793  hits 219,791  misses 2
debug  :   stage-1          efficiency 0.0%  accesses 2  hits 0  misses 2
debug  : tessellation access cache statistics:
debug  :   combined         efficiency 100.0%  accesses 219,793  hits 219,791  misses 2
debug  :   stage-0          efficiency 100.0%  accesses 219,793  hits 219,791  misses 2
debug  :   stage-1          efficiency 0.0%  accesses 2  hits 0  misses 2
debug  : path tracing statistics:
debug  :   path count       219,793
debug  :   path length      avg 1.6  min 1  max 2  dev 0.5
debug  : texture store statistics:
debug  :   performances     efficiency 50.0%  accesses 48  hits 24  misses 24
debug  :   peak size        108.9 KB
info   : wrote image file E:/MyDocuments/maya/projects/default/rmanpix/as_test_lambert_normal.perspShape.16.appleseed.png in 344 ms.
