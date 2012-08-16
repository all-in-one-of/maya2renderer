#include "test_type2.impl"

surface test_type2(

	//-------------------------------------------
	float frequency = 4;

	color c0 = color(0.0, 0.5, 0.5);
	color c1 = color "rgb" (0.0, 0.5, 0.5);
	color c2 = color "hsv" (125, 0, 255);
	color c3 = color "hsl" (0.0, 0.5, 0.5);
	color c4 = color "xyz" (0.0, 0.5, 0.5);
	color c5 = color "XYZ" (0.0, 0.5, 0.5);
	color c6 = color "YIQ" (0.0, 0.5, 0.5);

	point p0 = point(0, 0, 0);
	point p1 = point "raster" (1, 1, 1);
	point p2 = point "NDC"    (2, 2, 2);
	point p3 = point "screen" (3, 3, 3);
	point p4 = point "camera" (4, 4, 4);
	point p5 = point "world"  (5, 5, 5);
	point p6 = point "object" (6, 6, 6);
	point p7 = point "shader" (7, 7, 7);
	point p8 = point "current"(8, 8, 8);
	point p9 = point "string" (9, 9, 9);

	vector v0 = vector(0, 0, 0);
	vector v1 = vector "raster" (1, 1, 1);
	vector v2 = vector "NDC"    (2, 2, 2);
	vector v3 = vector "screen" (3, 3, 3);
	vector v4 = vector "camera" (4, 4, 4);
	vector v5 = vector "world"  (5, 5, 5);
	vector v6 = vector "object" (6, 6, 6);
	vector v7 = vector "shader" (7, 7, 7);
	vector v8 = vector "current"(8, 8, 8);
	vector v9 = vector "string" (9, 9, 9);

	normal n0 = normal (0, 0, 0);
	normal n1 = normal "raster" (1, 1, 1);
	normal n2 = normal "NDC"    (2, 2, 2);
	normal n3 = normal "screen" (3, 3, 3);
	normal n4 = normal "camera" (4, 4, 4);
	normal n5 = normal "world"  (5, 5, 5);
	normal n6 = normal "object" (6, 6, 6);
	normal n7 = normal "shader" (7, 7, 7);
	normal n8 = normal "current"(8, 8, 8);
	normal n9 = normal "string" (9, 9, 9);

	matrix m0 = 0;
	matrix m1 = matrix (0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15);
	matrix m2 = matrix "shader"  0;
	matrix m3 = matrix "shader" (0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15);
	matrix m4 = matrix "world" 0;
	matrix m5 = matrix "world" (0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15);

	string s0 = "testString";

	// varying ----------------------------------------------------------------------------------
	varying float vfrequency = 4;

	varying color vc0 = color(0.0, 0.5, 0.5);
	varying color vc1 = color "rgb" (0.0, 0.5, 0.5);
	varying color vc2 = color "hsv" (125, 0, 255);
	varying color vc3 = color "hsl" (0.0, 0.5, 0.5);
	varying color vc4 = color "xyz" (0.0, 0.5, 0.5);
	varying color vc5 = color "XYZ" (0.0, 0.5, 0.5);
	varying color vc6 = color "YIQ" (0.0, 0.5, 0.5);

	varying point vp0 = point(0, 0, 0);
	varying point vp1 = point "raster" (1, 1, 1);
	varying point vp2 = point "NDC"    (2, 2, 2);
	varying point vp3 = point "screen" (3, 3, 3);
	varying point vp4 = point "camera" (4, 4, 4);
	varying point vp5 = point "world"  (5, 5, 5);
	varying point vp6 = point "object" (6, 6, 6);
	varying point vp7 = point "shader" (7, 7, 7);
	varying point vp8 = point "current"(8, 8, 8);
	varying point vp9 = point "string" (9, 9, 9);

	varying vector vv0 = vector(0, 0, 0);
	varying vector vv1 = vector "raster" (1, 1, 1);
	varying vector vv2 = vector "NDC"    (2, 2, 2);
	varying vector vv3 = vector "screen" (3, 3, 3);
	varying vector vv4 = vector "camera" (4, 4, 4);
	varying vector vv5 = vector "world"  (5, 5, 5);
	varying vector vv6 = vector "object" (6, 6, 6);
	varying vector vv7 = vector "shader" (7, 7, 7);
	varying vector vv8 = vector "current"(8, 8, 8);
	varying vector vv9 = vector "string" (9, 9, 9);

	varying normal vn0 = normal (0, 0, 0);
	varying normal vn1 = normal "raster" (1, 1, 1);
	varying normal vn2 = normal "NDC"    (2, 2, 2);
	varying normal vn3 = normal "screen" (3, 3, 3);
	varying normal vn4 = normal "camera" (4, 4, 4);
	varying normal vn5 = normal "world"  (5, 5, 5);
	varying normal vn6 = normal "object" (6, 6, 6);
	varying normal vn7 = normal "shader" (7, 7, 7);
	varying normal vn8 = normal "current"(8, 8, 8);
	varying normal vn9 = normal "string" (9, 9, 9);

	varying matrix vm0 = 0;
	varying matrix vm1 = matrix (0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15);
	varying matrix vm2 = matrix "shader"  0;
	varying matrix vm3 = matrix "shader" (0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15);
	varying matrix vm4 = matrix "world" 0;
	varying matrix vm5 = matrix "world" (0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15);

	//varying string vs0 = "varyingTestString"; //NOTE:strings cannot be varying: "vs0"

	// uniform ---------------------------------------------------------------------
	uniform float ufrequency = 4;

	uniform color uc0 = color(0.0, 0.5, 0.5);
	uniform color uc1 = color "rgb" (0.0, 0.5, 0.5);
	uniform color uc2 = color "hsv" (125, 0, 255);
	uniform color uc3 = color "hsl" (0.0, 0.5, 0.5);
	uniform color uc4 = color "xyz" (0.0, 0.5, 0.5);
	uniform color uc5 = color "XYZ" (0.0, 0.5, 0.5);
	uniform color uc6 = color "YIQ" (0.0, 0.5, 0.5);

	uniform point up0 = point(0, 0, 0);
	uniform point up1 = point "raster" (1, 1, 1);
	uniform point up2 = point "NDC"    (2, 2, 2);
	uniform point up3 = point "screen" (3, 3, 3);
	uniform point up4 = point "camera" (4, 4, 4);
	uniform point up5 = point "world"  (5, 5, 5);
	uniform point up6 = point "object" (6, 6, 6);
	uniform point up7 = point "shader" (7, 7, 7);
	uniform point up8 = point "current"(8, 8, 8);
	uniform point up9 = point "string" (9, 9, 9);

	uniform vector uv0 = vector(0, 0, 0);
	uniform vector uv1 = vector "raster" (1, 1, 1);
	uniform vector uv2 = vector "NDC"    (2, 2, 2);
	uniform vector uv3 = vector "screen" (3, 3, 3);
	uniform vector uv4 = vector "camera" (4, 4, 4);
	uniform vector uv5 = vector "world"  (5, 5, 5);
	uniform vector uv6 = vector "object" (6, 6, 6);
	uniform vector uv7 = vector "shader" (7, 7, 7);
	uniform vector uv8 = vector "current"(8, 8, 8);
	uniform vector uv9 = vector "string" (9, 9, 9);

	uniform normal un0 = normal (0, 0, 0);
	uniform normal un1 = normal "raster" (1, 1, 1);
	uniform normal un2 = normal "NDC"    (2, 2, 2);
	uniform normal un3 = normal "screen" (3, 3, 3);
	uniform normal un4 = normal "camera" (4, 4, 4);
	uniform normal un5 = normal "world"  (5, 5, 5);
	uniform normal un6 = normal "object" (6, 6, 6);
	uniform normal un7 = normal "shader" (7, 7, 7);
	uniform normal un8 = normal "current"(8, 8, 8);
	uniform normal un9 = normal "string" (9, 9, 9);

	uniform matrix um0 = 0;
	uniform matrix um1 = matrix (0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15);
	uniform matrix um2 = matrix "shader"  0;
	uniform matrix um3 = matrix "shader" (0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15);
	uniform matrix um4 = matrix "world" 0;
	uniform matrix um5 = matrix "world" (0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15);

	uniform string us0 = "uniformTestString";

	//===== array ===================================


) 
{
    test_type2(

	//-------------------------------------------
	 frequency,

	 c0 ,
	 c1 ,
	 c2 ,
	 c3 ,
	 c4 ,
	 c5 ,
	 c6 ,

	 p0 ,
	 p1 ,
	 p2 ,
	 p3 ,
	 p4 ,
	 p5 ,
	 p6 ,
	 p7 ,
	 p8 ,
	 p9 ,

	 v0 ,
	 v1 ,
	 v2 ,
	 v3 ,
	 v4 ,
	 v5 ,
	 v6 ,
	 v7 ,
	 v8 ,
	 v9 ,

	 n0,
	 n1 ,
	 n2 ,
	 n3 ,
	 n4 ,
	 n5 ,
	 n6 ,
	 n7 ,
	 n8 ,
	 n9 ,

	 m0 ,
	 m1 ,
	 m2 ,
	 m3 ,
	 m4 ,
	 m5 ,

	 s0 ,

	//  ----------------------------------------------------------------------------------
	  vfrequency,

	  vc0 ,
	  vc1 ,
	  vc2 ,
	  vc3 ,
	  vc4 ,
	  vc5 ,
	  vc6 ,

	  vp0 ,
	  vp1 ,
	  vp2 ,
	  vp3,
	  vp4 ,
	  vp5 ,
	  vp6 ,
	  vp7 ,
	  vp8 ,
	  vp9 ,

	  vv0 ,
	  vv1 ,
	  vv2 ,
	  vv3 ,
	  vv4 ,
	  vv5 ,
	  vv6 ,
	  vv7 ,
	  vv8 ,
	  vv9 ,

	  vn0 ,
	  vn1 ,
	  vn2 ,
	  vn3 ,
	  vn4 ,
	  vn5 ,
	  vn6 ,
	  vn7 ,
	  vn8 ,
	  vn9 ,

	  vm0 ,
	  vm1 ,
	  vm2 ,
	  vm3 ,
	  vm4 ,
	  vm5 ,
	//  vs0 , //NOTE:s cannot be : "vs0"

	//  ---------------------------------------------------------------------
	  ufrequency ,

	  uc0 ,
	  uc1 ,
	  uc2 ,
	  uc3 ,
	  uc4 ,
	  uc5 ,
	  uc6 ,

	  up0 ,
	  up1 ,
	  up2 ,
	  up3 ,
	  up4 ,
	  up5 ,
	  up6 ,
	  up7 ,
	  up8 ,
	  up9 ,

	  uv0 ,
	  uv1 ,
	  uv2 ,
	  uv3 ,
	  uv4 ,
	  uv5 ,
	  uv6 ,
	  uv7 ,
	  uv8 ,
	  uv9 ,

	  un0 ,
	  un1 ,
	  un2 ,
	  un3 ,
	  un4 ,
	  un5 ,
	  un6 ,
	  un7 ,
	  un8 ,
	  un9 ,

	  um0 ,
	  um1 ,
	  um2 ,
	  um3 ,
	  um4 ,
	  um5 ,

	  us0
    );
}
