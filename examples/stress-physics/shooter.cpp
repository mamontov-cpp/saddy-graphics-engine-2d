#include "shooter.h"
#include <renderer.h>
#include <periodicalevent.h>
#include <pipeline/pipeline.h>
#include <p2d/circle.h>

DECLARE_SOBJ_INHERITANCE(Shooter, sad::phy52d::Object)

Shooter::Shooter()
{
    sad::Sprite2D::Options options(
        "objects",
        sad::Rect2D(sad::Point2D(177, 0), sad::Point2D(265, 88)),
        sad::Rect2D(sad::Point2D(-10, -10), sad::Point2D(10, 10))
    );
    this->setOptions(&options);
    this->setShape(new sad::p2d::Circle(20));
    m_gun = new Shooter::Gun(this);
}


void Shooter::startShooting() const
{
    sad::pipeline::Pipeline* pipeline = sad::Renderer::ref()->pipeline();
    pipeline->append(new sad::PeriodicalEventPollProcess( m_gun ));
}


Shooter::Gun::Gun(Shooter * parent) : m_parent(parent)
{
    this->setInterval(500);
    this->setMinSpeed(sad::p2d::Vector(0, 100));
    this->setMaxSpeed(sad::p2d::Vector(0, 100));
    this->setMaxCount(2);
}

sad::phy52d::App * Shooter::Gun::app()
{
    return m_parent->app();
}

void Shooter::Gun::perform()
{
    this->setMinPosition(m_parent->position() - sad::p2d::Vector(5, 0));
    this->setMaxPosition(m_parent->position() + sad::p2d::Vector(5, 0));
    this->setInterval(200.0 +  800.0 * this->prand());
    this->sad::phy52d::RandomDirectionObjectEmitter<ColoredBullet>::perform();
}



