#include "geometry2d.h"
#include "sadrect.h"

#include "p2d/collides1d.h"
#include "p2d/point.h"
#include "p2d/axle.h"

#include <limits>
bool sad::projectionIsWithin(const sad::Point2D & test, const sad::Point2D & pivot1, const sad::Point2D & pivot2)
{
	sad::p2d::Axle axle = sad::p2d::axle(pivot1, pivot2);
	sad::p2d::Cutter1D c1 = p2d::project(test, test, axle); 
	sad::p2d::Cutter1D c2 = p2d::project(pivot1, pivot2, axle); 
	return p2d::collides(c1, c2);
}

bool sad::isWithin(const sad::Point2D & p, const sad::Rect2D & r)
{
	bool a1 = sad::projectionIsWithin(p, r[0], r[1]);
	bool a2 = sad::projectionIsWithin(p, r[1], r[2]);
	return a1 && a2; 
}

void sad::moveBy(const sad::Point2D & dp , sad::Rect2D & r)
{
	for(int i = 0; i < 4; i++)
	{
		r[i] += dp;
	}
}

void sad::rotate(sad::Rect2D & r, float angle)
{
	sad::Point2D c = r[0]; c += r[2]; c/=2;
	sad::p2d::Matrix2x2 m = p2d::Matrix2x2::counterclockwise(angle);
	for(int i = 0; i < 4; i++)
	{
		sad::Point2D vi = r[i] - c;
		sad::Point2D result = vi * m;
		r[i] = c; 
		r[i] += result;
	}
}

void sad::rotate(sad::Vector2D & v, float angle)
{
	sad::p2d::Matrix2x2 m = p2d::Matrix2x2::counterclockwise(angle);
	v = v * m;
}

void sad::moveAndRotateNormalized(float angle, sad::Point2D & result, sad::Rect2D & v)
{
	sad::p2d::Matrix2x2 m = sad::p2d::Matrix2x2::counterclockwise(angle);
	for(int i = 0; i < 4; i++) {
		sad::Point2D r = v[i] * m;
		v[i] = result + r;
	}
}

double sad::angleOf(double x, double y)
{
	if (sad::is_fuzzy_zero(y) && sad::is_fuzzy_zero(x))
	{
		return 0;
	}
	p2d::Vector u = p2d::unit(p2d::Vector(x, y));
	
	bool tests[4] = {
		sad::is_fuzzy_equal( u.x(), 1) && sad::is_fuzzy_equal( u.y(), 0),
		sad::is_fuzzy_equal( u.x(), 0) && sad::is_fuzzy_equal( u.y(), 1),
		sad::is_fuzzy_equal( u.x(), -1) && sad::is_fuzzy_equal( u.y(), 0),
		sad::is_fuzzy_equal( u.x(), 0) && sad::is_fuzzy_equal( u.y(), -1),
	};

	double angles[4] = { 0, M_PI / 2, M_PI, M_PI * 1.5};
	for(int i = 0; i < 4; i++)
	{
		if (tests[i]) return angles[i];
	}
	double angle =  0;
	double at = atan(y /x);
	if (x > 0)
	{
		if (y > 0 ) angle = at;
		else        angle = 2 * M_PI + at;
	}
	else
	{
		angle = M_PI + at;
	}
	return angle;
}

double sad::acos(double x)
{
	if (sad::is_fuzzy_zero(x))
	{
		return M_PI / 2.0;
	}
	if (abs(x) > 1)
	{
		return std::numeric_limits<double>::quiet_NaN();
	}

	double result = atan( sqrt(1 - x * x) / x);
	if (result < 0)
	{
		result += M_PI;
	}
	return result;
}

double sad::asin(double x)
{
	if (sad::is_fuzzy_equal(x, 1))
	{
		return M_PI / 2.0;
	}
	if (sad::is_fuzzy_equal(x, -1))
	{
		return M_PI / -2.0;
	}
	if (abs(x) > 1)
	{
		return std::numeric_limits<double>::quiet_NaN();
	}
	double result = atan( x / sqrt(1 - x * x) );
	return result;
}

double sad::normalizeAngle(double x)
{
	// Don't handler zero x
	if (sad::is_fuzzy_zero(x))
	{
		return x;
	}

	double result = x;
	if (result < 0)
	{
		int count = (int)(result / (-2 * M_PI)) + 1;
		result += count * 2* M_PI;
	}
	if (result > 0)
	{
		int count = (int)(result / (2 * M_PI));
		result -= count * 2 * M_PI;
	}
	return result;
}

sad::Maybe<double> sad::findAngle(double sina, double cosa)
{
	sad::Maybe<double> result;
	if (sad::is_fuzzy_equal(sina * sina + cosa * cosa, 1.0) == false)
	{
		return result;
	}
	double alpha = sad::acos(cosa);
	if (sad::is_fuzzy_equal(sin(alpha), sina) == false)
	{
		alpha *= -1.0;
	}
	alpha = sad::normalizeAngle(alpha);
	
	result.setValue(alpha);
	return result;
}

bool sad::isValid(const sad::Rect2D & rect)
{
	sad::Point2D middle = rect[0];
	for(int i = 1; i < 4; i++)
	{
		middle += rect[i];
	}
	middle /= 4.0;

	double distances[4];
	for(int i = 0; i < 4; i++)
	{
		distances[i] = middle.distance(rect[i]);
	}
	// Enlarged precision, because does not worked in some stupid kind of cases
	return sad::is_fuzzy_equal(distances[0], distances[1], 0.01)
		&& sad::is_fuzzy_equal(distances[0], distances[2], 0.01)
		&& sad::is_fuzzy_equal(distances[0], distances[3], 0.01);	
}

void sad::getBaseRect(
	const sad::Rect2D & rect, 
	sad::Rect2D & base,
	double & alpha,
	bool * error
)
{
#define SET_ERROR { if (error) *error = true; }
	if (error)
	{
		*error = false;
	}
	base = rect;
	alpha = 0;
	if (sad::isValid(rect) == false)
	{
		SET_ERROR;
		return;
	}
	
	// Handle degraded case, when rect is zero
	if (sad::equal(rect[0], rect[1])
		&& sad::equal(rect[1], rect[2])
		&& sad::equal(rect[2], rect[3])
		&& sad::equal(rect[3], rect[0]))
	{
		return;
	}

	sad::Point2D p = rect.p0();
	p += rect.p1();
	p += rect.p2();
	p += rect.p3();
	p /= 4.0;


	double distx = p.distance((rect[0] + rect[3]) / 2);
	double disty = p.distance((rect[0] + rect[1]) / 2);

	sad::Point3D v(-distx, -disty, 0.0);

	base[0]= p + sad::Point2D(-distx, -disty);
	base[1]= p + sad::Point2D(distx, -disty);
	base[2]= p + sad::Point2D(distx, disty);
	base[3]= p + sad::Point2D(-distx, disty);

#undef SET_ERROR
}