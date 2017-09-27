/*! \file databasepropertygetter.h
    

    Defines a property getter for database properties
 */
#pragma once
#include "../dukqtcontext.h"
#include <sadstring.h>
#include <renderer.h>

#include <db/dbdatabase.h>

namespace scripting
{

namespace database
{


/*! Defines a get method overload for database property
 */
template<
    typename T
>
class PropertyGetter: public dukpp03::qt::FunctionCallable
{
public:
    /*! Constructs new object
     */
    PropertyGetter() : dukpp03::qt::FunctionCallable()
    {

    }

    /*! Returns amount of required arguments
        \return amount of required arguments
     */
    virtual int requiredArguments()
    {
        return 1;
    }

    /*! Destroys a getter
     */
    virtual ~PropertyGetter()
    {

    }

    /*! Clones an object
        \return copy of object
     */
    dukpp03::qt::Callable* clone()
    {
        return new scripting::database::PropertyGetter<T>();
    }

    /*! Determines, whether it can be called with this context
        \param[in] ctx context
     */
    virtual std::pair<int, bool> canBeCalled(dukpp03::qt::BasicContext* ctx)
    {
        int required_args = this->requiredArguments();
        if (ctx->getTop() != required_args)
        {
            return std::make_pair(-1, false);
        }
        int a = 0;
        dukpp03::Maybe<sad::String> maybe_prop_name = dukpp03::GetValue< sad::String, dukpp03::qt::BasicContext >::perform(ctx, 0);
        if (maybe_prop_name.exists())
        {
            a += 1;
            sad::db::TypeName<T>::init();
            sad::String prop_name = maybe_prop_name.value();
            sad::db::Database* me = sad::Renderer::ref()->database("");
            sad::db::Property* prop = me->propertyByName(prop_name);
            if (prop)
            {
                a += 1;
                if (prop->baseType() == sad::db::TypeName<T>::baseName() && prop->pointerStarsCount() == 0)
                {
                   a += 1;
                }
            }
        }
        return std::make_pair(a, a == 3);
    }
    /*! Calls an actual function, performong data
        \param[in] ctx context
        \return amount of values on stack
     */
    virtual int call(dukpp03::qt::BasicContext* ctx)
    {
        int required_args = this->requiredArguments();
        if (ctx->getTop() != required_args)
        {
            ctx->throwInvalidArgumentCountError(ctx->getTop(), 1);
            throw new dukpp03::ArgumentException();
            return 0;
        }
        dukpp03::Maybe<sad::String> maybe_prop_name = dukpp03::GetValue< sad::String, dukpp03::qt::BasicContext >::perform(ctx, 0);
        if (maybe_prop_name.exists())
        {
            sad::db::TypeName<T>::init();
            sad::String prop_name = maybe_prop_name.value();
            sad::db::Database* me = sad::Renderer::ref()->database("");
            sad::db::Property* prop = me->propertyByName(prop_name);
            if (prop)
            {
                if (prop->baseType() == sad::db::TypeName<T>::baseName() && prop->pointerStarsCount() == 0)
                {
                    dukpp03::PushValue<T, dukpp03::qt::BasicContext>::perform(ctx, me->getProperty<T>(prop_name).value());
                    return 1;
                }
                else
                {
                    ctx->throwError("Invalid property value");
                    throw new dukpp03::ArgumentException();
                    return 0;
                }
            }
            else
            {
                ctx->throwError(std::string("Property \"") + prop_name + std::string("\" is not readable"));
                throw new dukpp03::ArgumentException();
                return 0;
            }
        }
        else
        {
            ctx->throwInvalidTypeError(1, "sad::String");
            throw new dukpp03::ArgumentException();
            return 0;
        }
        return 1;
    }
};

}

}
