/*! \file databasepropertygetter.h
    \author HiddenSeeker

    Defines a property getter for database properties
 */
#pragma once
#include <sadstring.h>
#include <renderer.h>

#include <db/dbdatabase.h>

#include "../tovalue.h"
#include "../fromvalue.h"
#include "../callable.h"
#include "../scripting.h"

namespace scripting
{

namespace database
{


/*! Defines a getter for database property
 */
template<
    typename T
>
class PropertyGetter: public scripting::Callable
{
public:
    /*! Represents a setter call for a data
        \param[in] e engine
     */
    PropertyGetter(QScriptEngine* e) : scripting::Callable(e, "get", 1)
    {

    }
    /*! Destroys a setter
     */
    virtual ~PropertyGetter()
    {

    }
    /*! Determines, whether it can be called with this context
        \param[in] ctx context
     */
    virtual scripting::MatchResult canBeCalled(QScriptContext* ctx)
    {
        scripting::MatchResult result;
		result._1() = 0;
        checkArgumentCount(result, ctx);
        checkArgument<sad::String>(result, 0, ctx);
        if (result._2().exists() == false)
        {
			result._1() += 1;
            sad::db::TypeName<T>::init();
            sad::db::Database* me = sad::Renderer::ref()->database("");
            sad::Maybe<sad::String> propname = scripting::ToValue<sad::String>::perform(ctx->argument(0));
            sad::db::Property* prop = me->propertyByName(propname.value());
            if (prop)
            {
				result._1() += 1;
				if (prop->baseType() != sad::db::TypeName<T>::baseName() || prop->pointerStarsCount() != 0)
                {
                    QString qpropname = propname.value().c_str();
                    QString basetype = sad::db::TypeName<T>::baseName().c_str();
                    result._2().setValue(QString("property ") + qpropname + QString(" is not of type ") + basetype);
                }
            }
            else
            {
                result._2().setValue(QString(propname.value().c_str()) + " is not writeable");
            }
        }
        return result;
    }
    /*! Calls actually a function
        \param[in] ctx context
        \param[in] engine engine
     */
    virtual QScriptValue call(QScriptContext* ctx, QScriptEngine* engine)
    {
        sad::Maybe<sad::String> propname = scripting::ToValue<sad::String>::perform(ctx->argument(0));

        sad::db::Database* me = sad::Renderer::ref()->database("");
        sad::Maybe<T> oldvalue = me->getProperty<T>(propname.value());

        return scripting::FromValue<T>::perform(oldvalue.value(), engine);
    }
};

}

}
