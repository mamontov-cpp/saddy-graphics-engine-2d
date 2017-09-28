/*! \file abstractgetter.h
    

    Defines a basic getter for property
 */
#pragma once
#include "dukqtcontext.h"
#include <functional>

#include "maybe.h"


#include "../qstdstring.h"

#include <db/dbproperty.h>
#include <db/dbobject.h>

namespace scripting
{

/*! An abstract getter defines an invokable object, that can be used
    to get a property of specified type from an object on called stack.
 */
template<
    typename _ObjectType,
    typename _PropertyType
>
class AbstractGetter: public dukpp03::qt::FunctionCallable
{
public:
    /*! A default constructor for this type
     */
    AbstractGetter() : dukpp03::qt::FunctionCallable(), m_match_all_properties(false)
    {
        
    }

    /*! Attaches a getter to a specified property
        \param[in] name name of property
     */
    AbstractGetter(
        const sad::String& name
    ) :  dukpp03::qt::FunctionCallable(), m_match_all_properties(false)
    {
        setPropertyName(name);
    }

    /*! Could be inherited
     */
    virtual ~AbstractGetter()
    {
        
    }

    /*! Clones an object
        \return copy of object
     */
    dukpp03::qt::Callable* clone()
    {
        return new scripting::AbstractGetter<_ObjectType, _PropertyType>(*this);
    }

    /*! Returns amount of required arguments
        \return amount of required arguments
     */
    virtual int requiredArguments()
    {
        return 2; // object, name of property
    }

    /*! Makes getter match all properties
     */
    void matchAllProperties()
    {
        m_match_all_properties = true;
    }

    /*! Sets property name for a getter
        \param[in] name a property name for a getter
     */
    void setPropertyName(const sad::String& name)
    {
        m_match_all_properties = false;
        m_matched_property = name;
    }

    /*! Adds a property name, that will be excluded from matching
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
        dukpp03::Maybe<_ObjectType> maybe_object = dukpp03::GetValue< _ObjectType, dukpp03::qt::BasicContext >::perform(ctx, 0);
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
                    if (this->tryGetPropertyValue(maybe_object.value(), maybe_prop_name.value()).exists())
                    {
                        a += 1;
                    }
                }
            }
        }
        return std::make_pair(a, a == 4);
    }

    /*! Invoked, when getter is actually called
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
        dukpp03::Maybe<_ObjectType> maybe_object = dukpp03::GetValue< _ObjectType, dukpp03::qt::BasicContext >::perform(ctx, 0);
        dukpp03::Maybe<sad::String> maybe_prop_name = dukpp03::GetValue< sad::String, dukpp03::qt::BasicContext >::perform(ctx, 1);
        if (maybe_object.exists())
        {
            if (maybe_prop_name.exists())
            {
                if (this->isCorrespondsToPropertyName(maybe_prop_name.value()))
                {
                    sad::Maybe<_PropertyType> maybe_prop_value = this->tryGetPropertyValue(maybe_object.value(), maybe_prop_name.value());
                    if (maybe_prop_value.exists())
                    {
                        dukpp03::PushValue<_PropertyType, dukpp03::qt::BasicContext>::perform(ctx, maybe_prop_value.value());
                        return 1;
                    }
                    else
                    {
                        ctx->throwError(std::string("Property \"") + maybe_prop_name.value() + std::string("\" is not readable"));
                        throw new dukpp03::ArgumentException();
                    }
                }
                else
                {
                    ctx->throwError(std::string("Property \"") + maybe_prop_name.value() + std::string("\" is not readable"));
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
    /*! Tries to return property value
        \param[in] value object
        \param[in] property_name a name of property
        \return property value if it exists, nothing otherwise
    */
    virtual  sad::Maybe<_PropertyType> tryGetPropertyValue(_ObjectType value, const sad::String& property_name)
    {
        return value->template getProperty<_PropertyType>(property_name);
    }
    /*! Returns true if getter corresponds to property name
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

    /*! Makes getter match all properties for object
     */
    bool m_match_all_properties;
    /*! A property name, that getter corresponds to (if it doesn't corresponds to all properties of same type)
     */
    sad::String m_matched_property;
    /*! A property names, that should be excluded from handling by the getter
     */
    sad::Vector<sad::String> m_excluded_properties;
};

}
