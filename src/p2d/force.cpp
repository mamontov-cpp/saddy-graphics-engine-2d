#include <p2d/force.h>
#include <p2d/angularforce.h>


double sad::p2d::Force<double>::value() const 
{ 
    return  m_value; 
}


void sad::p2d::Force<double>::setValue(double value) 
{ 
    m_value = value; 
}


void sad::p2d::Force<double>::step(double time) 
{

}


void sad::p2d::ImpulseForce<double>::step(double time) 
{ 
    die(); 
}

