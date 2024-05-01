#include "animations/animationsabstractsavedobjectstatecreator.h"
#include "opticksupport.h"

sad::animations::AbstractSavedObjectStateCreator::AbstractSavedObjectStateCreator(const sad::String& name)
: m_name(name)
{
    PROFILER_EVENT;

}

sad::animations::AbstractSavedObjectStateCreator::~AbstractSavedObjectStateCreator()
{
    PROFILER_EVENT;

}
