#include "animations/animationssavedobjectstatecreators.h"

sad::animations::SavedObjectStateCreators::SavedObjectStateCreators()
{

}

sad::animations::SavedObjectStateCreators::SavedObjectStateCreators(
    const sad::animations::SavedObjectStateCreators& o
)
{
    for(size_t i = 0; i < o.size(); i++)
    {
        this->push_back(o[i]->clone());
    }
}

sad::animations::SavedObjectStateCreators& sad::animations::SavedObjectStateCreators::operator=(const sad::animations::SavedObjectStateCreators& o)
{
    for(size_t i = 0; i < this->size(); i++)
    {
        delete (*this)[i];
    }
    this->clear();
    for(size_t i = 0; i < o.size(); i++)
    {
        this->push_back(o[i]->clone());
    }
}

sad::animations::SavedObjectStateCreators::~SavedObjectStateCreators()
{

}

