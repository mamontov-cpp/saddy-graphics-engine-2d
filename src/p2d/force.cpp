#include <p2d/force.h>
#include <p2d/angularforce.h>


double p2d::Force<double>::value() const { return  m_value; }


void p2d::Force<double>::setValue(double value) { m_value = value; }


void p2d::Force<double>::step(double time) {}


void p2d::ImpulseForce<double>::step(double time) { die(); }

