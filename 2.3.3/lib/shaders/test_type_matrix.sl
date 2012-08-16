#include "test_type_matrix.impl"

surface test_type_matrix(
	vector v0=vector(1, 5, 3.0);
	matrix m0 = 0;
	matrix m1 = matrix (0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15);
) 
{
	test_type_matrix(
		v0,
		m0,
		m1
        );
}
