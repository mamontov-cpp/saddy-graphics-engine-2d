#include "../../include/extra/fuzzy_equal.h"
#include <math.h>

bool is_fuzzy_equal(double x1, double x2, double precision)
{
	return is_fuzzy_zero(x1 - x2, precision);
}

bool is_fuzzy_zero(double x, double precision)
{
	return fabs(x) <= precision;
}

bool non_fuzzy_zero(double x, double precision)
{
	return !is_fuzzy_zero(x, precision);
}
