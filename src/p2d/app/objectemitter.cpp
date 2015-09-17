#include <p2d/app/objectemitter.h>


void sad::p2d::app::AbstractObjectEmitter::perform()
{
    sad::p2d::app::Object * o = this->produce();
    this->app()->addObject(o);
            
    sad::p2d::Point p;
    this->position(p);
    o->setPosition(p);

    sad::p2d::Vector v;
    this->tangentialVelocity(v);
    o->setTangentialVelocity(v);

    o->setAngle(this->angle());
    o->setAngularVelocity(this->angularVelocity());
}


double sad::p2d::app::AbstractObjectEmitter::angle()
{
    return 0;
}

