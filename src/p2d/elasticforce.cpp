#include <p2d/elasticforce.h>
#include <log/log.h>

DECLARE_SOBJ_INHERITANCE(sad::p2d::ElasticForce, sad::p2d::Force<sad::p2d::Vector>)


sad::p2d::ElasticForce::ElasticForce(
                           sad::p2d::Body * first, 
                           sad::p2d::Body * second,
                           double elasticity, 
                           double resistance
)
{
    m_second = second;
    if (m_second)
    {
        m_second->addRef();
    }
    m_default_distance = m_second->currentShape()->center()
                        .distance(first->currentShape()->center());
    m_elasticity = elasticity;
    m_resistance = resistance;
}

sad::p2d::ElasticForce::~ElasticForce()
{
    if (m_second)
    {
        m_second->delRef();
    }
}

const sad::p2d::Vector & sad::p2d::ElasticForce::value(sad::p2d::Body* first) const
{
    sad::p2d::ElasticForce * force = const_cast<sad::p2d::ElasticForce *>(this);
    p2d::Point s1 = m_second->currentShape()->center();    
    p2d::Point s2 = first->currentShape()->center();
    
    double distance = s1.distance(s2);
    double delta = distance - m_default_distance;

    force->m_value = s1;
    force->m_value -= s2;
    if (fabs(distance) > 0)
        force->m_value *= m_elasticity * delta / distance;
    else
    {
        force->m_value = p2d::Vector();
        return force->m_value;
    }
    p2d::Vector v = first->tangentialVelocity();
    v *= p2d::modulo(v);
    v *= m_resistance;
    force->m_value -= v;

    return m_value;
}


sad::p2d::Body* sad::p2d::ElasticForce::dependsFromBody() const
{
    return m_second;
}

