#include "test_type.impl"

surface test_type(
	float frequency = 7;
	string s0="test stringATT";
	color c0 = color(1.0, 1.0, 1.0);
	point p0 = point(2, 2, 2);
	vector v0=vector(3, 3, 3);
	normal n0 = normal (4, 4, 4);
	matrix m0 = 0;
	matrix m1 = matrix (0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15);
) 
{
	test_type(
		frequency,
		s0,
		c0,
		p0,
		v0,
		n0,
		m0,
		m1
        ) ;
}
