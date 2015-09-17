#include <p2d/weight.h>

#include <limits>

sad::p2d::Weight::Weight(double value, bool finite) 
: m_finite(finite), m_value(value)
{
    
}

sad::p2d::Weight::~Weight()
{
    
}

bool sad::p2d::Weight::isInfinite() const
{
    return !m_finite;
}

double sad::p2d::Weight::value() const
{
    double result = std::numeric_limits<double>::max();
    if (m_finite)
        result = m_value;
    return result;
}

sad::p2d::Weight * sad::p2d::Weight::constant(double value)
{
    return new p2d::Weight(value, true);
}

sad::p2d::Weight * sad::p2d::Weight::infinite()
{
    return new p2d::Weight(0.0, false);
}

void sad::p2d::Weight::setValue(double value)
{
    m_value = value;
    m_finite = true;
}
