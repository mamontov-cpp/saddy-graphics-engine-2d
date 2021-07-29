/*! \file animations/animationssavedobjectpropertycreator.h
    

    Defines a creator, which creates a SavedObjectProperty
 */
#pragma once
#include "animationsabstractsavedobjectstatecreator.h"
#include "animationssavedobjectproperty.h"

namespace sad
{

namespace animations
{

/*! This class is used to creates a saved object properties for object
 */
template<
    typename T
>
class SavedObjectPropertyCreator: public sad::animations::AbstractSavedObjectStateCreator
{
public:
    /*! Creates new property creator
        \param[in] name name for a stored key
        \param[in] propertyname a name for stored property
     */
    SavedObjectPropertyCreator(const sad::String& name, const sad::String& propertyname)
    : sad::animations::AbstractSavedObjectStateCreator(name), m_property_name(propertyname)
    {

    }
    /*! Could be inherited
     */
    virtual ~SavedObjectPropertyCreator() override
    {

    }
    /*! Sets name for animated property
        \param[in] name name for property
     */
    inline void setPropertyName(const sad::String& name)
    {
        m_property_name = name;
    }
    /*! Clones a saved object state creator
        \return new copy of creator
     */
    virtual AbstractSavedObjectStateCreator* clone() const override
    {
        return new sad::animations::SavedObjectPropertyCreator<T>(m_name, m_property_name);
    }
    /*! Creates a state for specified object
        \param o a specified object for creator
        \return a saved object state
     */
    virtual sad::animations::SavedObjectState* create(sad::db::Object* o) override
    {
        return new sad::animations::SavedObjectProperty<T>(o, m_property_name);
    }
protected:
    /*! A property name, which should be used to create state
     */
    sad::String m_property_name;
};

}

}
