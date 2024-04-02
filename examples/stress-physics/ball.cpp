#include "ball.h"
#include <renderer.h>
#include <3rdparty/format/format.h>

#include <p2d/circle.h>

DECLARE_SOBJ_INHERITANCE(Ball, sad::p2d::app::Object)

Ball::Ball()
{
    sad::Sprite2D::Options options(
        "objects",
        sad::Rect2D(sad::Point2D(0, 88), sad::Point2D(88, 174)),
        sad::Rect2D(sad::Point2D(-11, -11), sad::Point2D(11, 11))
    );
    this->setOptions(&options);
    this->setShape(new sad::p2d::Circle(11));

#ifdef PHYSICS_ENGINE_DEBUG
    sad::log::FileTarget * t = new sad::log::FileTarget();
    t->open("ball.txt");

    m_log = new sad::log::Log();
    m_log->addTarget(t);
#endif
}


#ifdef PHYSICS_ENGINE_DEBUG
void Ball::render()
{
    this->sad::p2d::app::Object::render();
    sad::p2d::Point p = this->body()->position();
    sad::p2d::Point v = this->body()->tangentialVelocity();
    std::string s = str(fmt::Format("{0} {1} {2} {3} {4}") << p.x() << p.y() << v.x() << v.y() << sad::Renderer::ref()->fps());
    m_log->debug(s);
}
#endif


Ball::~Ball()
{
    
}
