#include "../../include/extra/fuzzy_equal.h"

#include <math.h>

bool sad::is_fuzzy_equal(double x1, double x2, double precision)
{
	return fabs(x1 - x2) < precision;
}

bool sad::is_fuzzy_zero(double x, double precision)
{
	return fabs(x) < precision;
}

bool sad::non_fuzzy_zero(double x, double precision)
{
	return !sad::is_fuzzy_zero(x, precision);
}
