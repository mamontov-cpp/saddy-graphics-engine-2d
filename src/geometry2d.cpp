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

void sad::rotate(float angle, sad::Rect2D & r)
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

void sad::moveAndRotateNormalized(float angle, sad::Point2D & result, sad::Rect2D & v)
{
	sad::p2d::Matrix2x2 m = sad::p2d::Matrix2x2::counterclockwise(angle);
	for(int i = 0; i < 4; i++) {
		sad::Point2D r = v[i] * m;
		v[i] = result + r;
	}
}

double sad::angle_of(double x, double y)
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

bool sad::scalar(const sad::Point3D & p1, const sad::Point3D & p2)
{
	return p1.x() * p2.x() + p1.y() * p2.y() + p1.z() * p2.z();
}

bool sad::isOnSamePlane(const sad::Rect<sad::Point3D> & rect)
{
	// Compute coefficients for determining plane
	// see http://www.pm298.ru/plosk3.php formula
	// Here second and third determinants are stored

	double a[2][3] = {
		{ rect[1].x() - rect[0].x(), rect[1].y() - rect[0].y(), rect[1].z() - rect[0].z() },
		{ rect[2].x() - rect[0].x(), rect[2].y() - rect[0].y(), rect[2].z() - rect[0].z() }
	};

	double cofactors[3] = {
		a[0][1] * a[1][2] - a[1][1] * a[0][2],
		(a[0][0] * a[1][2] - a[0][2] * a[1][0]) * (-1),
		a[0][0] * a[1][1] - a[0][1] * a[1][0]
	};
	// Substitute fourth point to plane equation
	double compared = cofactors[0] * (rect[3].x() - rect[0].x())
					+ cofactors[1] * (rect[3].y() - rect[0].y())
					+ cofactors[2] * (rect[3].z() - rect[0].z());

	return  sad::is_fuzzy_zero(compared);
}

bool sad::isValid(const sad::Rect<sad::Point3D> & rect)
{
	if (!sad::isOnSamePlane(rect))
		return false;

	bool equal[4] = {
		sad::is_fuzzy_zero(sad::scalar(rect[1] - rect[0], rect[3] - rect[0])),
		sad::is_fuzzy_zero(sad::scalar(rect[0] - rect[1], rect[2] - rect[1])),
		sad::is_fuzzy_zero(sad::scalar(rect[3] - rect[2], rect[1] - rect[2])),
		sad::is_fuzzy_zero(sad::scalar(rect[0] - rect[3], rect[2] - rect[3]))
	};

	return equal[0] && equal[1] && equal[2] && equal[3];
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

void sad::getBaseRect(
	const sad::Rect<sad::Point3D> & rect, 
	sad::Rect<sad::Point3D> & base,
	double & alpha,
	double & theta
)
{
	if (sad::isValid(rect) == false)
	{
		base = rect;
		alpha = 0;
		theta = 0;
		return;
	}
	
	// Handle degraded case, when rect is zero
	if (sad::equal(rect[0], rect[1])
		&& sad::equal(rect[1], rect[2])
		&& sad::equal(rect[2], rect[3])
		&& sad::equal(rect[3], rect[0]))
	{
		base = rect;
		alpha = 0;
		theta = 0;
		return;
	}

	sad::Point3D p = rect.p0();
	p += rect.p1();
	p += rect.p2();
	p += rect.p3();
	p /= 4.0;


	double distx = p.distance((rect[0] + rect[3]) / 2);
	double disty = p.distance((rect[0] + rect[1]) / 2);

	sad::Point3D v(-distx, -disty, 0.0);

	base[0]= p + sad::Point3D(-distx, -disty, 0);
	base[1]= p + sad::Point3D(distx, -disty, 0);
	base[2]= p + sad::Point3D(distx, disty, 0);
	base[3]= p + sad::Point3D(-distx, disty, 0);

	sad::Point3D r = (rect[0] - p);
	if (sad::is_fuzzy_zero(distx))
	{
		double sina = -(r.x() / disty);
		assert(abs(sina) <= 1);
		alpha = atan2(sina, sqrt(1 - sina * sina));
		if (is_fuzzy_zero(cos(alpha)) != 0)
		{
			theta = sad::acos(r.y() / disty / cos (alpha) );
		}
		else
		{
			theta = 0;
		}
		return;
	}
	
	if (sad::is_fuzzy_zero(disty))
	{
		double cosa = r.x() / distx;
		assert(abs(cosa) <= 1);
		alpha = sad::acos(cosa);
		if (is_fuzzy_zero(cos(alpha)) != 0)
		{
			theta = sad::acos(r.y() / distx / sin (alpha) );
		}
		else
		{
			theta = 0;
		}
		return;
	}

	double d = sqrt(distx * distx + disty * disty);
	assert(r.x() < d);
	alpha = sad::acos(r.x() / d ) - sad::acos(- distx / d);

	double dk = - distx * sin(alpha) - disty * cos(alpha);
	theta = sad::acos(r.y() / dk); 
}
