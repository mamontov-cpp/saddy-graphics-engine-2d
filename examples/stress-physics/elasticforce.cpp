#include "elasticforce.h"

ElasticForce::ElasticForce(p2d::Body * first, p2d::Body * second)
{
	m_first = first;
	m_second = second;
	m_defaultdistance = m_second->currentShape()->center() - m_first->currentShape()->center();
}

const p2d::Vector & ElasticForce::value() const
{
	ElasticForce * force = const_cast<ElasticForce *>(this);
	double k = 0.1;
	
	force->m_value = m_first->currentShape()->center();
	force->m_value  += m_defaultdistance;
	force->m_value  -= m_second->currentShape()->center();
	force->m_value *= k;

	return m_value;
}


