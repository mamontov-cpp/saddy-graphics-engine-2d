/*! \file abstractsetter.h
    

    Defines a basic setter for property
 */
#pragma once
#include "dukqtcontext.h"
#include <functional>

#include "maybe.h"
#include "callable.h"

#include "../qstdstring.h"

#include <equalto.h>

#include <db/dbproperty.h>
#include <db/dbobject.h>

namespace scripting
{

class Scripting;

/*! A setter, that can be used to set property of object of
    particular type.
 */
template<
    typename _ObjectType,
    typename _PropertyType
>
class AbstractSetter: public dukpp03::qt::FunctionCallable
{
public:
    /*! A converter function, that can be used to convert values for properties
     */
    typedef std::function<
               dukpp03::Maybe<_PropertyType>(
                   dukpp03::qt::BasicContext* ctx,
                   duk_idx_t pos
              )
            >  Converter;
    /*! A condition, that can be used to check values for matching
     */
    typedef std::function<
              dukpp03::Maybe<sad::String>(
                  const _PropertyType& type
              )
            >  Condition;

    /*! A change of property action, that should be invoked on success
     */
    typedef std::function<
                void(scripting::Scripting* s, _ObjectType obj, const sad::String& propertyname, _PropertyType oldvalue,  _PropertyType newvalue)
            >  Action;

    /*! Constructs new setter
        \param[in] scripting a scripting object
     */
    AbstractSetter(scripting::Scripting* scripting) : dukpp03::qt::FunctionCallable(), m_scripting(scripting), m_match_all_properties(false)
    {

    }

    /*! Clones an object
        \return copy of object
     */
    dukpp03::qt::Callable* clone()
    {
        return new scripting::AbstractSetter<_ObjectType, _PropertyType>(*this);
    }

    /*! Returns amount of required arguments
        \return amount of required arguments
     */
    virtual int requiredArguments()
    {
        return 3; // object, name of property, value
    }

    /*! Destroys a setter
     */
    virtual ~AbstractSetter()
    {
        
    }
    /*! Add new converter for values, passed to setter
        \param[in] c converter
     */
    void addConverter(Converter c)
    {
        m_converters.push_back(c);
    }
    /*! Adds new condition for values, passed to setter
        \param[in] c condition
     */
    void addCondition(Condition c)
    {
        m_conditions.push_back(c);
    }
    /*! An action to pe performed on values
        \param[in] a an action
     */
    void addAction(Action a)
    {
        m_actions.push_back(a);
    }

    /*! Makes setter match all properties
     */
    void matchAllProperties()
    {
        m_match_all_properties = true;
    }

    /*! Sets property name for a setter
        \param[in] name a property name for a setter
     */
    void setPropertyName(const sad::String& name)
    {
        m_match_all_properties = false;
        m_matched_property = name;
    }
    /*! Adds property name to list of property names, that are excluded
        \param[in] name a property name
     */
    void addExcludedPropertyName(const sad::String& name)
    {
        m_excluded_properties.push_back(name);
    }

    /*! Adds excluded property names
        \param[in] names excluded property names
     */
    void addExcludedPropertyNames(const char* names[])
    {
        size_t i = 0;
        while(names[i])
        {
            addExcludedPropertyName(names[i]);
            ++i;
        }
    }

    /*! Determines, whether it can be called with this context
        \param[in] ctx context
        \return result of check
     */
    virtual std::pair<int, bool>  canBeCalled(dukpp03::qt::BasicContext* ctx)
    {
        int required_args = this->requiredArguments();
        if (ctx->getTop() != required_args)
        {
            return std::make_pair(-1, false);
        }
        int a = 0;
        dukpp03::Maybe<_ObjectType> maybe_object = this->tryGetObjectFromStack(ctx, 0, false);
        dukpp03::Maybe<sad::String> maybe_prop_name = dukpp03::GetValue< sad::String, dukpp03::qt::BasicContext >::perform(ctx, 1);
        if (maybe_object.exists())
        {
            a += 1;
            if (maybe_prop_name.exists())
            {
                a += 1;
                if (this->isCorrespondsToPropertyName(maybe_prop_name.value()))
                {
                    a += 1;
                    dukpp03::Maybe<_PropertyType> maybe_value = this->tryGetPropertyValueFromStack(ctx, 2);
                    if (maybe_value.exists())
                    {
                        a += 1;
                        if (this->tryGetPropertyError(maybe_value.value()).exists() == false)
                        {
                            a += 1;
                            if (this->hasProperty(maybe_object.value(), maybe_prop_name.value()))
                            {
                                a += 1;
                            }
                        }
                    }
                }
            }
        }
        return std::make_pair(a, a == 6);
    }
    /*! Invoked, when setter is actually called
        \param[in] ctx context
        \return amount of values in context
     */
    virtual int call(dukpp03::qt::BasicContext* ctx)
    {
        int required_args = this->requiredArguments();
        if (ctx->getTop() != required_args)
        {
            ctx->throwInvalidArgumentCountError(ctx->getTop(), 3);
            throw new dukpp03::ArgumentException();
            return 0;
        }
        dukpp03::Maybe<_ObjectType> maybe_object = this->tryGetObjectFromStack(ctx, 0, true);
        dukpp03::Maybe<sad::String> maybe_prop_name = dukpp03::GetValue< sad::String, dukpp03::qt::BasicContext >::perform(ctx, 1);
        if (maybe_object.exists())
        {
            if (maybe_prop_name.exists())
            {
                if (this->isCorrespondsToPropertyName(maybe_prop_name.value()))
                {
                    dukpp03::Maybe<_PropertyType> maybe_value = this->tryGetPropertyValueFromStack(ctx, 2);
                    if (maybe_value.exists())
                    {
                        dukpp03::Maybe<sad::String> property_error = this->tryGetPropertyError(maybe_value.value());
                        if (property_error.exists() == false)
                        {
                            if (this->hasProperty(maybe_object.value(), maybe_prop_name.value()))
                            {
                                _PropertyType oldvalue = this->getOldPropertyValue(maybe_object.value(), maybe_prop_name.value());
                                std::equal_to<_PropertyType> comparator;
                                if (comparator(oldvalue, maybe_value.value()) == false)
                                {
                                    callActions(
                                        maybe_object.value(),
                                        maybe_prop_name.value(),
                                        oldvalue,
                                        maybe_value.value()
                                    );
                                }
                            }
                            else
                            {
                                ctx->throwError(std::string("Property \"") + maybe_prop_name.value() + std::string("\" is not writeable"));
                                throw new dukpp03::ArgumentException();
                            }
                        }
                        else
                        {
                            ctx->throwError(property_error.value());
                            throw new dukpp03::ArgumentException();
                        }
                    }
                    else
                    {
                        std::string name = dukpp03::qt::BasicContext::template typeName< _PropertyType >();
                        ctx->throwInvalidTypeError(3, name);
                        throw dukpp03::ArgumentException();
                    }
                }
                else
                {
                    ctx->throwError(std::string("Property \"") + maybe_prop_name.value() + std::string("\" is not writeable"));
                    throw new dukpp03::ArgumentException();
                }
            }
            else
            {
                std::string name = dukpp03::qt::BasicContext::template typeName< sad::String >();
                ctx->throwInvalidTypeError(2, name);
                throw dukpp03::ArgumentException();
            }
        }
        else
        {
            std::string name = dukpp03::qt::BasicContext::template typeName< _ObjectType >();
            ctx->throwInvalidTypeError(1, name);
            throw dukpp03::ArgumentException();
        }
        return 0;
    }
protected:
    /*! Tries to get object from a stack
        \param ctx context
        \param id id of object
        \param is_being_called whether a setter is being called
        \return a value
     */
    virtual dukpp03::Maybe<_ObjectType> tryGetObjectFromStack(dukpp03::qt::BasicContext* ctx, duk_idx_t id, bool is_being_called)
    {
        return dukpp03::GetValue<_ObjectType, dukpp03::qt::BasicContext >::perform(ctx, id);
    }
    /*! Returns true if setter corresponds to property name
        \param[in] name a name
        \return  whether setter corresponds to it
     */
    virtual bool isCorrespondsToPropertyName(const sad::String& name)
    {
        bool result = true;
        if (!m_match_all_properties)
        {
            result = (m_matched_property == name);
        }
        if (result)
        {
            result = std::find(
                m_excluded_properties.begin(),
                m_excluded_properties.end(),
                name
            ) == m_excluded_properties.end();
        }
        return result;
    }

    /*!
     * Tries to get property value from stack
     * \param[in] ctx context
     * \param[in] obj_id object id
     * \return a value or nothing
     */
    dukpp03::Maybe<_PropertyType> tryGetPropertyValueFromStack(
        dukpp03::qt::BasicContext* ctx,
        duk_idx_t obj_id
    )
    {
        dukpp03::Maybe<_PropertyType> value = dukpp03::GetValue<_PropertyType, dukpp03::qt::BasicContext>::perform(ctx, obj_id);
        if (value.exists() == false)
        {
            for(size_t i = 0; (i < m_converters.size()) && (value.exists() == false); i++)
            {
                value = (m_converters[i])(ctx, obj_id);
            }
        }
        return value;
    }

    /*! Checks value for property type
        \param[in] value a value
        \return an error if value is invalid
     */
    dukpp03::Maybe<sad::String> tryGetPropertyError(const _PropertyType& value)
    {
         dukpp03::Maybe<sad::String> result;
         for(size_t i = 0; (i < m_conditions.size()) && (result.exists() == false); i++)
         {
             result = (m_conditions[i])(value);
         }
         return result;
    }

    /*! Checks if property exists in object
        \param[in] value object
        \param[in] property_name a name of property
        \return true, if exists
    */
    virtual bool hasProperty(_ObjectType value, const sad::String& property_name)
    {
        return value->template getProperty<_PropertyType>(property_name).exists();
    }

    /*! Returns old property value
        \param[in] value object
        \param property_name a name of property
        \return property value
     */
    virtual  _PropertyType getOldPropertyValue(_ObjectType value, const sad::String& property_name)
    {
        return value->template getProperty<_PropertyType>(property_name).value();
    }

    /*! Calls all corresponding actions, setting property or performing other actions
        \param[in] obj an object to be set
        \param[in] property_name a property for object
        \param[in] old_value old value
        \param[in] new_value new value
     */
    virtual void callActions(_ObjectType obj, const sad::String& property_name, _PropertyType old_value,  _PropertyType new_value)
    {
        for(size_t i = 0; i < m_actions.size(); i++)
        {
            (m_actions[i])(m_scripting, obj, property_name, old_value, new_value);
        }
    }

    /*! An inner scripting link
     */
    scripting::Scripting* m_scripting;
    /*! A converter for values
     */
    sad::Vector<Converter> m_converters;
    /*! A conditions for checking values
     */
    sad::Vector<Condition> m_conditions;
    /*! An actions to be performed on values
     */
    sad::Vector<Action> m_actions;
    /*! Makes setter match all properties for object
     */
    bool m_match_all_properties;
    /*! A property name, that setter corresponds to (if it doesn't corresponds to all properties of same type)
     */
    sad::String m_matched_property;
    /*! A list of excluded properties
     */
    sad::Vector<sad::String> m_excluded_properties;
};

/*! Constructs setter for specified property
    \param scripting a scripting object
    \param name a name of property
    \return a setter
 */
template<
    typename _ObjectType,
    typename _PropertyType
>
inline scripting::AbstractSetter<_ObjectType, _PropertyType>* setterForProperty(scripting::Scripting* scripting, const sad::String& name)
{
    scripting::AbstractSetter<_ObjectType, _PropertyType>* result = new scripting::AbstractSetter<_ObjectType, _PropertyType>(scripting);
    result->setPropertyName(name);
    return result;
}

}
