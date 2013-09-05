#include <p2d/weight.h>
#include <limits>

p2d::Weight::Weight(double value, bool finite) 
: m_value(value), m_finite(finite)
{
	
}

bool p2d::Weight::isInfinite() const
{
	return !m_finite;
}

double p2d::Weight::value() const
{
	double result = std::numeric_limits<double>::max();
	if (m_finite)
		result = m_value;
	return result;
}

p2d::Weight p2d::Weight::constant(double value)
{
	return p2d::Weight(value, true);
}

p2d::Weight p2d::Weight::infinite()
{
	return p2d::Weight(0.0, false);
}

void p2d::Weight::setValue(double value)
{
	m_value = value;
	m_finite = true;
}
