#include "geometry3d.h"
#include <cassert>

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

	sad::Point3D middle = rect[0];
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

	return sad::is_fuzzy_equal(distances[0], distances[1])
		&& sad::is_fuzzy_equal(distances[0], distances[2])
		&& sad::is_fuzzy_equal(distances[0], distances[3]);
}

void sad::rotate(
	const sad::Rect<sad::Point3D> & rect,
	sad::Rect<sad::Point3D> & outrect,
	double alpha,
	double theta
)
{
	sad::Point3D middle = rect[0];
	for(int i = 1; i < 4; i++)
	{
		middle += rect[i];
	}
	middle /= 4.0;

	for(int i = 0; i < 4; i++)
	{
		sad::Point3D p = rect[i] - middle;
		outrect[i].set(
			p.x() * cos(alpha) - p.y() * sin(alpha) * cos(theta) + p.z() * sin(alpha) * sin(theta),
			p.x() * sin(alpha) + p.y() * cos(alpha) * cos(theta) - p.z() * cos(alpha) * sin(theta),
			p.y() * sin(theta) + p.z() * cos(theta)
		);
		outrect[i] += middle;
	}
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

	if (sad::is_fuzzy_zero(disty))
	{
		theta = 0;
		alpha = sad::acos(r.x() / (-distx));
		return;
	}
	theta = sad::asin(r.z() / -disty);
	if (sad::is_fuzzy_zero(distx))
	{
		alpha = sad::acos(r.y() / (-disty) / cos(theta));
		return;
	}

	double dx = -distx;
	double dy = -disty * cos(theta);
	double dr = sqrt(dx * dx + dy * dy);

	double phi = sad::acos(dx / dr);

	alpha = sad::acos(r.x() / dr) - phi;
	
	double testrx = dx * cos(alpha) - dy * sin(alpha);
	if (sad::is_fuzzy_equal(testrx, r.x()) == false)
	{
		alpha *= -1;
	}
}
