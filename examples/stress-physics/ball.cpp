#include "ball.h"
#include <renderer.h>
#include <3rdparty/format/format.h>

DECLARE_SOBJ_INHERITANCE(Ball, sad::p2d::app::Object)

Ball::Ball()
{
	this->initFromConstants<Ball>();

	sad::log::FileTarget * t = new sad::log::FileTarget();
	t->open("ball.txt");

	m_log = new sad::log::Log();
	m_log->addTarget(t);
}

void Ball::render()
{
	this->sad::p2d::app::Object::render();
	sad::p2d::Point p = this->body()->position();
	sad::p2d::Point v = this->body()->tangentialVelocity();
	std::string s = str(fmt::Format("{0} {1} {2} {3}") << p.x() << p.y() << v.x() << v.y() << sad::Renderer::ref()->fps());
	m_log->debug(s);
}


Ball::~Ball()
{
	
}
