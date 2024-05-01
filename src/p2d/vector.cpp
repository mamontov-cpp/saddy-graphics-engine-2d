#include <p2d/vector.h>
#include <fuzzyequal.h>

#define _USE_MATH_DEFINES
#include <cmath>
#include "opticksupport.h"

double sad::p2d::modulo(const sad::p2d::Vector & v)
{
    PROFILER_EVENT;
    return sqrt(v.x() * v.x() + v.y() * v.y());
}

sad::p2d::Vector sad::p2d::basis() 
{
    PROFILER_EVENT;
    return sad::p2d::Vector(M_SQRT1_2, M_SQRT1_2);
}

sad::p2d::Vector sad::p2d::unit(const p2d::Vector & v)
{
    PROFILER_EVENT;
    double modulo = sad::p2d::modulo(v);
    if (sad::non_fuzzy_zero(modulo)) 
        return v / modulo;
    else
        return p2d::basis();
}


void sad::p2d::mutableUnit(sad::p2d::Vector & v)
{
    PROFILER_EVENT;
    if (fabs(v.x()) > S2D_FP_PRECISION || fabs(v.y()) > S2D_FP_PRECISION)
    {
        double d = sqrt(v.x() * v.x() + v.y() * v.y());
        v /= d;
        return;
    }
    v  = sad::p2d::basis();
}

sad::p2d::Vector sad::p2d::ortho(const sad::p2d::Vector & v, sad::p2d::OrthoVectorIndex i)
{
    PROFILER_EVENT;
    p2d::Vector unit = sad::p2d::unit(v);
    if (i == p2d::OrthoVectorIndex::OVI_DEG_90) 
    {
        return sad::p2d::Vector(unit.y(), - unit.x());
    }
    return sad::p2d::Vector(- unit.y(), unit.x());
}


void sad::p2d::mutableNormalizedOrtho(sad::p2d::Vector & v, sad::p2d::OrthoVectorIndex i)
{
    PROFILER_EVENT;
    if (i == sad::p2d::OrthoVectorIndex::OVI_DEG_90) 
    {
        double x = v.x();
        v.setX(v.y());
        v.setY(-x);
    }
    else
    {
        double x = v.x();
        v.setX(v.y() * (-1));
        v.setY(x);
    }
}

double sad::p2d::scalar(const sad::p2d::Vector & v1, const sad::p2d::Vector &  v2)
{
    PROFILER_EVENT;
    return v1.x() * v2.x() + v1.y() * v2.y();
}
