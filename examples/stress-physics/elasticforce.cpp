#include "elasticforce.h"
#include <log/log.h>

p2d::ElasticForce::ElasticForce(
	   					   p2d::Body * first, 
						   p2d::Body * second,
						   double elasticity, 
						   double resistance
						  )
{
	m_first = first;
	m_second = second;
	m_defaultdistance = m_second->currentShape()->center()
						.distanceTo(m_first->currentShape()->center());
	m_elasticity = elasticity;
	m_resistance = resistance;
}

const p2d::Vector & p2d::ElasticForce::value() const
{
	ElasticForce * force = const_cast<ElasticForce *>(this);
	p2d::Point s1 = m_first->currentShape()->center();	
	p2d::Point s2 = m_second->currentShape()->center();
	double k = m_elasticity;
	double b = m_resistance;
	double distance = s1.distanceTo(s2);
	double delta = distance - m_defaultdistance;

	force->m_value = s1;
	force->m_value -= s2;
	force->m_value = p2d::unit(force->m_value);
	force->m_value *= k * delta;

	p2d::Vector v = m_second->tangentialVelocity();
	v *= p2d::modulo(v) * -1;
	v *= b;
	force->m_value += v;

	return m_value;
}


