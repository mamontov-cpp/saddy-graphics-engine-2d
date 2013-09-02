#include "p2d/collisionhandler.h"


p2d::BasicCollisionHandler::~BasicCollisionHandler()
{
}


void p2d::FunctionCollisionHandler<p2d::Body, p2d::Body>::invoke(const p2d::BasicCollisionEvent & ev)
{
	m_p(ev);
}

