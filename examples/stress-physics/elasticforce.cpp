#include "elasticforce.h"
#include <log/log.h>

ElasticForce::ElasticForce(p2d::Body * first, p2d::Body * second)
{
	m_first = first;
	m_second = second;
	m_defaultdistance = m_second->currentShape()->center().distanceTo(m_first->currentShape()->center());
}

const p2d::Vector & ElasticForce::value() const
{
	ElasticForce * force = const_cast<ElasticForce *>(this);
	p2d::Point s1 = m_first->currentShape()->center();	
	p2d::Point s2 = m_second->currentShape()->center();
	double k = 1;
	double distance = s1.distanceTo(s2);
	double delta = distance - m_defaultdistance;

	force->m_value = s1;
	force->m_value -= s2;
	force->m_value = p2d::unit(force->m_value);
	force->m_value *= k * delta;
	return m_value;
}


