/*! \file animations/setstate/setproperty.h
    

    Defines a command, which sets property on an object
 */
#pragma once
#include "typedcommand.h"

#include "../../db/dbobject.h"

namespace sad
{

namespace animations
{

namespace setstate
{

/*! Defines a call command, as setting a property of object
 */
template<
    typename _Argument
>
class SetProperty : public sad::animations::setstate::TypedCommand<_Argument>
{
public:
    /*! Constructs new empty cached call
        \param[in]  o object object, which method should be called upon
        \param[in]  prop a property name to be set
     */
    inline SetProperty(sad::db::Object* o, const sad::String& prop) : m_o(o), m_property_name(prop) { }
    /*! Clones command
        \return command
     */
    virtual sad::animations::setstate::AbstractSetStateCommand* clone() const override
    {
        return new sad::animations::setstate::SetProperty<_Argument>(m_o, m_property_name);
    }    
    /*! Calls an animation delegate for specified argument
        \param[in] a argument
     */
    virtual void call(const _Argument& a) override
    {
        m_o->setProperty(m_property_name, a);
    }
    virtual ~SetProperty() { }
protected:
    /*! An object link for fast call
     */
    sad::db::Object* m_o;
    /*! A method to be called on object
     */
    sad::String m_property_name;
};

}

}

}
