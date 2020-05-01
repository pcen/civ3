#include "math.h"

int z_to_n(int num)
{
	return num < 0 ? (-num * 2) - 1 : num * 2;
}

int dovetail(int k1, int k2)
{
	k1 = z_to_n(k1);
	k2 = z_to_n(k2);
	return (((k1 + k2) * (k1 + k2 + 1)) / 2) + k2;
}
