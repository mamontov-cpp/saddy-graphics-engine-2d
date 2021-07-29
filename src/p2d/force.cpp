#include <p2d/force.h>
#include <p2d/angularforce.h>

#include <db/dbtypename.h>


DECLARE_SOBJ(sad::p2d::Force<double>)
DECLARE_SOBJ_INHERITANCE(sad::p2d::ImpulseForce<double>, sad::p2d::Force<double>)

DECLARE_TYPE_AS_SAD_OBJECT(sad::p2d::Force<sad::p2d::Vector>);
DECLARE_TYPE_AS_SAD_OBJECT(sad::p2d::ImpulseForce<sad::p2d::Vector>);


double sad::p2d::Force<double>::value(sad::p2d::Body* body) const
{ 
    return  m_value; 
}


void sad::p2d::Force<double>::setValue(double value) 
{ 
    m_value = value; 
}


void sad::p2d::Force<double>::step(sad::p2d::Body* body, double time)
{

}

sad::p2d::Body* sad::p2d::Force<double>::dependsFromBody() const
{
    return nullptr;
}

void sad::p2d::ImpulseForce<double>::step(sad::p2d::Body* body, double time)
{ 
    die(); 
}

