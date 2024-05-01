#include "animations/animationssavedobjectstate.h"
#include "opticksupport.h"

sad::animations::SavedObjectState::SavedObjectState(sad::db::Object* o) : m_object(o), m_references(1)
{
    PROFILER_EVENT;
    
}

sad::animations::SavedObjectState::~SavedObjectState()
{
    PROFILER_EVENT;
    
}

void sad::animations::SavedObjectState::storeBodyState(sad::p2d::Body* b)
{
    PROFILER_EVENT;

}
