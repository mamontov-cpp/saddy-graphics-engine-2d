#include <p2d/weight.h>

#include <fuzzyequal.h>

#include <limits>
#include "opticksupport.h"

sad::p2d::Weight::Weight(double value, bool finite) 
: m_finite(finite), m_value(value)
{
    PROFILER_EVENT;
    
}

bool sad::p2d::Weight::isInfinite() const
{
    PROFILER_EVENT;
    return !m_finite;
}

double sad::p2d::Weight::value() const
{
    PROFILER_EVENT;
    double result = std::numeric_limits<double>::max();
    if (m_finite)
        result = m_value;
    return result;
}

sad::p2d::Weight sad::p2d::Weight::constant(double value)
{
    PROFILER_EVENT;
    return p2d::Weight(value, true);
}

sad::p2d::Weight sad::p2d::Weight::infinite()
{
    PROFILER_EVENT;
    return p2d::Weight(0.0, false);
}

void sad::p2d::Weight::setValue(double value)
{
    PROFILER_EVENT;
    m_value = value;
    if (sad::is_fuzzy_zero(m_value))
    {
        m_value = 0.0001;
    }
    m_finite = true;
}
