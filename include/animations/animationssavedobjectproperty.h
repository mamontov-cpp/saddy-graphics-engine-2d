/*! \file animations/animationssavedobjectproperty.h
    

    Defines a saved object property, which could be restored, when animation is finished
 */
#pragma once 
#include "animationssavedobjectstate.h"

namespace sad
{

namespace animations
{
    
/*! Defines a saved object property, which could be restored, when animation is finished
 */
template<
    typename  T
>
class SavedObjectProperty: public sad::animations::SavedObjectState
{
public:
    /*! Creates new saved object state
        \param[in] o object
        \param[in] name name of property
     */
    SavedObjectProperty(sad::db::Object* o, const sad::String& name)
    : sad::animations::SavedObjectState(o), m_property_name(name)
    {
        m_property_value = o->getProperty<T>(name).value();
    }
    /*! Can be inherited
     */
    virtual ~SavedObjectProperty() override
    {
        
    }
    /*! Restores last object state
     */
    virtual void restore() override
    {
        m_object->setProperty(m_property_name, m_property_value);
    }
protected:
    /*! A property name to be set, when restoring
     */
    sad::String m_property_name;
    /*! A property value for an object
     */
    T m_property_value;
};

}

}
