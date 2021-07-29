#include "p2d/collisionhandler.h"


sad::p2d::BasicCollisionHandler::~BasicCollisionHandler()
{

}


void sad::p2d::UntypedCollisionHandler::invoke(
    const sad::p2d::BasicCollisionEvent & ev
)
{
    if (ev.m_object_1 != nullptr && ev.m_object_2 != nullptr)
    {
        m_f(ev);
    }
}


void sad::p2d::TypedCollisionHandler<sad::p2d::Body, sad::p2d::Body>::invoke(
    const sad::p2d::BasicCollisionEvent & ev
)
{
    if (ev.m_object_1 != nullptr && ev.m_object_2 != nullptr)
    {
        m_f(ev);
    }
}

