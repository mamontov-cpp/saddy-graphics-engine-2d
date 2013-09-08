#include <p2d/app/objectemitter.h>

void p2d::app::AbstractObjectEmitter::perform()
{
	p2d::app::Object * o = this->produce();
	this->app()->addObject(o);
			
	p2d::Point p;
	this->position(p);
	o->setPosition(p);

	p2d::Vector v;
	this->tangentialVelocity(v);
	o->setTangentialVelocity(v);

	o->setAngle(this->angle());
	o->setAngularVelocity(this->angularVelocity());
}


double p2d::app::AbstractObjectEmitter::angle()
{
	return 0;
}

