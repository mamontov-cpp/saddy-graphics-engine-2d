#include "shooter.h"
#include <renderer.h>
#include <periodicalevent.h>
#include <pipeline/pipeline.h>

DECLARE_SOBJ_INHERITANCE(Shooter, sad::p2d::app::Object)

Shooter::Shooter()
{
	this->initFromConstants<Shooter>();
	m_gun = new Shooter::Gun(this);
}


void Shooter::startShooting()
{
	sad::Renderer::ref()->pipeline()->append(
			new sad::PeriodicalEventPollProcess( m_gun )
		);
}


Shooter::Gun::Gun(Shooter * parent) : m_parent(parent)
{
	this->setInterval(500);
	this->setMinSpeed(sad::p2d::Vector(0, 100));
	this->setMaxSpeed(sad::p2d::Vector(0, 100));
	this->setMaxCount(2);
}

sad::p2d::app::App * Shooter::Gun::app()
{
	return m_parent->app();
}

void Shooter::Gun::perform()
{
	this->setMinPosition(m_parent->position() - sad::p2d::Vector(5, 0));
	this->setMaxPosition(m_parent->position() + sad::p2d::Vector(5, 0));
	this->setInterval(200.0 +  800.0 * this->prand());
	this->sad::p2d::app::RandomDirectionObjectEmitter<ColoredBullet>::perform();
}



