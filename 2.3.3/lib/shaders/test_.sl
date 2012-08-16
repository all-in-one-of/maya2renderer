#include "test_.impl"

surface test_(
	point p1 = point "raster" (1, 1, 1);
	point a_p[2]= {(0,0,0), (4,5,6)};
) 
{
	test_(
	   p1,
	   a_p
        );
}
