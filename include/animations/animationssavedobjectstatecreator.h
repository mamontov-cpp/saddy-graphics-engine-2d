/*! \file animations/animationssavedobjectstatecreator.h
    

    Defines a creator, which creates a specific object state
 */
#pragma once
#include "animationsabstractsavedobjectstatecreator.h"

namespace sad
{

namespace animations
{

/*! This class is used to creates a saved object properties for object
 */
template<
    typename T
>
class SavedObjectStateCreator: public sad::animations::AbstractSavedObjectStateCreator
{
public:
    /*! Creates new creator
        \param[in] name name for a stored key
     */
    SavedObjectStateCreator(const sad::String& name)
    : sad::animations::AbstractSavedObjectStateCreator(name)
    {

    }
    /*! Could be inherited
     */
    virtual ~SavedObjectStateCreator()
    {

    }
    /*! Clones a saved object state creator
        \return returns new copy of creator
     */
    virtual AbstractSavedObjectStateCreator* clone() const override
    {
        return new sad::animations::SavedObjectStateCreator<T>(m_name);
    }
    /*! Creates a state for specified object
        \param o a specified object for creator
        \return a saved object state
     */
    virtual sad::animations::SavedObjectState* create(sad::db::Object* o) override
    {
        return new T(o);
    }
protected:
    /*! A property name, which should be used to create state
     */
    sad::String m_property_name;
};

}

}
