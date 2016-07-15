/*! \file databasepropertysetter.h
    

    Defines a property setter for database properties
 */
#pragma once
#include <QSet>

#include <sadstring.h>
#include <renderer.h>
#include <equalto.h>

#include <db/dbdatabase.h>

#include "../tovalue.h"
#include "../fromvalue.h"
#include "../callable.h"
#include "../scripting.h"

#include "../../core/editor.h"

#include "../../gui/mainpanelproxy.h"

#include "../../history/database/changeproperty.h"

#include <QSet>

namespace scripting
{

namespace database
{

/*! Returns list of invisible properties
    \return reference for invisible properties
 */
const QSet<QString>& getInvisibleProperties();

/*! Defines a setter for database property
 */
template<
    typename T
>
class PropertySetter: public scripting::Callable
{
public:
    /*! Represents a setter call for a data
        \param[in] e engine
     */
    PropertySetter(QScriptEngine* e) : scripting::Callable(e, "set", 2)
    {

    }
    /*! Destroys a setter
     */
    virtual ~PropertySetter()
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
        checkArgument<T>(result, 1, ctx);
        if (result._2().exists() == false)
        {
            result._1() += 1;
            sad::db::TypeName<T>::init();
            sad::db::Database* me = sad::Renderer::ref()->database("");
            sad::Maybe<sad::String> propname = scripting::ToValue<sad::String>::perform(ctx->argument(0));
            sad::db::Property* prop = me->propertyByName(propname.value());
            
            const QSet<QString>&  invisible_properties = scripting::database::getInvisibleProperties();
            if (invisible_properties.contains(STD2QSTRING(propname.value())))
            {
                prop = NULL;
            }
            if (prop)
            {
                result._1() += 1;
                if (prop->baseType() != sad::db::TypeName<T>::baseName() || prop->pointerStarsCount() != 0)
                {
                    result._1() += 1;
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
        sad::Maybe<T> newvalue = scripting::ToValue<T>::perform(ctx->argument(1));

        sad::db::Database* me = sad::Renderer::ref()->database("");
        sad::Maybe<T> oldvalue = me->getProperty<T>(propname.value());
        std::equal_to<T> comparator;
        if (comparator(newvalue.value(), oldvalue.value()) == false)
        {
            core::Editor* p = static_cast<scripting::Scripting*>(engine->globalObject().property("---").toQObject())->editor();
            gui::table::Delegate* d = p->panelProxy()->delegatesByName()[propname.value()];
            history::Command* c = new history::database::ChangeProperty<T>(oldvalue.value(), newvalue.value(), d);
            c->commit(p);
            p->currentBatchCommand()->add(c);
        }
        return ctx->thisObject();
    }
};

}

}
