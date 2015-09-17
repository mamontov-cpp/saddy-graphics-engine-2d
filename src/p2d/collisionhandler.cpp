#include "p2d/collisionhandler.h"


sad::p2d::BasicCollisionHandler::~BasicCollisionHandler()
{
}


void sad::p2d::FunctionCollisionHandler<sad::p2d::Body, sad::p2d::Body>::invoke(
    const sad::p2d::BasicCollisionEvent & ev
)
{
    m_p(ev);
}

