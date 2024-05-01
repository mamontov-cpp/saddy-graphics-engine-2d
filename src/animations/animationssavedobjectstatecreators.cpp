#include "animations/animationssavedobjectstatecreators.h"
#include "opticksupport.h"

sad::animations::SavedObjectStateCreators::SavedObjectStateCreators()
{
    PROFILER_EVENT;

}

sad::animations::SavedObjectStateCreators::SavedObjectStateCreators(
    const sad::animations::SavedObjectStateCreators& o
)
{
    PROFILER_EVENT;
    for(size_t i = 0; i < o.size(); i++)
    {
        this->push_back(o[i]->clone());
    }
}

sad::animations::SavedObjectStateCreators& sad::animations::SavedObjectStateCreators::operator=(const sad::animations::SavedObjectStateCreators& o)
{
    PROFILER_EVENT;
    for(size_t i = 0; i < this->size(); i++)
    {
        delete (*this)[i];
    }
    this->clear();
    for(size_t i = 0; i < o.size(); i++)
    {
        this->push_back(o[i]->clone());
    }
    return *this;
}

sad::animations::SavedObjectStateCreators::~SavedObjectStateCreators()
{
    PROFILER_EVENT;

}
