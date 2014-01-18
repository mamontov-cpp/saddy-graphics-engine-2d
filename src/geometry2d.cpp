#include "geometry2d.h"
#include "sadrect.h"

#include "p2d/collides1d.h"
#include "p2d/point.h"
#include "p2d/axle.h"

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
