#include "geometry3d.h"
#include "matrix3x3.h"
#include <cassert>
#include "opticksupport.h"

double sad::scalar(const sad::Point3D & p1, const sad::Point3D & p2)
{
    PROFILER_EVENT;
    return p1.x() * p2.x() + p1.y() * p2.y() + p1.z() * p2.z();
}

bool sad::isOnSamePlane(const sad::Rect<sad::Point3D> & rect)
{
    PROFILER_EVENT;
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
    PROFILER_EVENT;
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
    // Enlarged precision, because does not worked in some stupid kind of cases
    return sad::is_fuzzy_equal(distances[0], distances[1], 0.01)
        && sad::is_fuzzy_equal(distances[0], distances[2], 0.01)
        && sad::is_fuzzy_equal(distances[0], distances[3], 0.01);
}

void sad::rotate(
    const sad::Rect<sad::Point3D> & rect,
    sad::Rect<sad::Point3D> & out_rectangle,
    double alpha,
    double theta
)
{
    PROFILER_EVENT;
    sad::Point3D middle = rect[0];
    for(int i = 1; i < 4; i++)
    {
        middle += rect[i];
    }
    middle /= 4.0;

    sad::Matrix3x3<double> m = sad::Matrix3x3<double>::rotationXandZ(alpha, theta);

    for(int i = 0; i < 4; i++)
    {
        sad::Point3D p = rect[i] - middle;
        out_rectangle[i] = m * p ;
        out_rectangle[i] += middle;
    }
}

// -----             sad::getBaseRect implementation            ----- 

// An utility function for getBaseRect which allows to recover after
// some kind of mistake with theta computation was done, choosing other solution 
// for theta (see getBaseRect for details)
static inline void chooseOtherThetaInGetBaseRect(
    const sad::Rect<sad::Point3D> & rect, 
    sad::Rect<sad::Point3D> & base,
    double & alpha,
    double & theta,
    bool   * error,	
    const sad::Point3D & r,
    double xs,
    double y
)
{
    PROFILER_EVENT;
    theta = M_PI - theta;
    theta = sad::normalizeAngle(theta);
    double ys = y * cos(theta);
    double d = sqrt(xs * xs + ys * ys);
    sad::Maybe<double> maybealphapphi = sad::findAngle(r.y() / d, r.x() / d);
    sad::Maybe<double> maybephi = sad::findAngle(ys / d , xs /d);
    if (maybealphapphi.exists() && maybephi.exists())
    {
        alpha = maybealphapphi.value() - maybephi.value();
        alpha = sad::normalizeAngle(alpha);
    }
    else
    {
        if (error)
            *error = true;
    }
}

void sad::getBaseRect(
    const sad::Rect<sad::Point3D> & rect, 
    sad::Rect<sad::Point3D> & base,
    double & alpha,
    double & theta,
    bool   * error
)
{
    PROFILER_EVENT;
#define SET_ERROR { if (error) *error = true; }
    if (error)
    {
        *error = false;
    }
    base = rect;
    alpha = 0;
    theta = 0;
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
    double x = -distx;
    double y = -disty;
    if (sad::is_fuzzy_zero(disty))
    {
        sad::Maybe<double> maybealpha = sad::findAngle(r.y() / x, r.x() / x);		
        if (maybealpha.exists())
            alpha = maybealpha.value();
        else
        {
            SET_ERROR;
        }
        return;
    }
    // Try first theta value
    theta = sad::asin(r.z() / y);
    theta = sad::normalizeAngle(theta);
    sad::Maybe<double> maybealpha;
    if (sad::is_fuzzy_zero(x))
    {
        double ys = y * cos(theta);
        maybealpha = sad::findAngle(r.x() / ys / -1, r.y() / ys);
        if (maybealpha.exists() == false)
        {
            theta = sad::normalizeAngle(M_PI - theta);
            ys = y * cos(theta);
            maybealpha = sad::findAngle(r.x() / ys / -1, r.y() / ys);
            if (maybealpha.exists())
            {
                alpha = maybealpha.value();
            }
            else
            {
                SET_ERROR;
            }
        }
        else
        {
            alpha = maybealpha.value();
        }
        return;
    }
    else
    {
        double xs = x;
        double ys = y * cos(theta);
        double d = sqrt(xs * xs + ys * ys);

        sad::Maybe<double> maybealphapphi = sad::findAngle(r.y() / d, r.x() / d);
        sad::Maybe<double> maybephi = sad::findAngle(ys / d , xs /d);
        if (maybealphapphi.exists() && maybephi.exists())
        {
            alpha = maybealphapphi.value() - maybephi.value();
            alpha = sad::normalizeAngle(alpha);

            // Test if other point is not valid and if so, pick other solution for theta
            sad::Point3D nr = (rect[1] - p);
            double px = distx;
            double py = -disty;
            
            sad::Point3D nrx(
                px * cos(alpha) - py * sin(alpha) * cos(theta),
                px * sin(alpha) + py * cos(alpha) * cos(theta),
                py * sin(theta)
            );
            // Enlarged precision, due to some implementation problems
            if (sad::equal(nr, nrx, 0.001f) == false)
            {
                chooseOtherThetaInGetBaseRect(rect, base, alpha, theta, error, r, xs, y);
            }
        }
        else
        {
            chooseOtherThetaInGetBaseRect(rect, base, alpha, theta, error, r, xs, y);
        }
    }
#undef SET_ERROR
}
