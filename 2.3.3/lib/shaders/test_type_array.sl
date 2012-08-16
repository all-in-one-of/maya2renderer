#include "test_type_array.impl"

surface test_type_array(
	float a_f[4] = {3.14, 2.17, 0, -1.0};
	string a_s[2]= {"AB0","CDE"};
	vector a_v[2]= {(0,0,0), (7,8,9)};
	normal a_n[2]= {(0,0,0), (1,2,3)};
	point a_p[2]= {(0,0,0), (4,5,6)};
	color a_c[2]= {(0,0,0), (1,0,1)};
	
	varying float a_vf[4] = {3.14, 2.17, 0, -1.0};
	//varying string a_vs[2]= {"AB0V","CDEV"}; //NOTE: strings cannot be varying
	varying vector a_vv[2]= {(1,0,0), (2,2,3)};
	varying normal a_vn[2]= {(1,0,0), (3,2,3)};
	varying point a_vp[2]= {(1,0,0), (4,2,3)};
	varying color a_vc__[2]= {(0,0,0), (1,0,1)};//NOTE: a_vc__ will cause a name collision
) 
{
	test_type_array(
	a_f ,
	a_s,
	a_v,
	a_n,
	a_p,
	a_c,
	
	a_vf,
	//a_vs, //NOTE: strings cannot be varying
	a_vv,
	a_vn,
	a_vp,
	a_vc__//NOTE: a_vc__ will cause a name collision
       );

}
