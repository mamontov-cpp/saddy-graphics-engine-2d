/*! \file databasepropertysetter.h
    

    Defines a property setter for database properties
 */
#pragma once
#include <QSet>

#include "../dukqtcontext.h"
#include <sadstring.h>
#include <renderer.h>
#include <equalto.h>

#include <db/dbdatabase.h>

#include "../scripting.h"

#include "../../core/editor.h"

#include "../../gui/mainpanelproxy.h"

#include "../../history/database/changeproperty.h"

namespace scripting
{

namespace database
{

/*! Returns list of invisible properties
    \return reference for invisible properties
 */
const QSet<QString>& getInvisibleProperties();

/*! Defines a set method overload for setting database property of specified type
 */
template<
    typename T
>
class PropertySetter: public dukpp03::qt::FunctionCallable
{
public:
    /*! Constructs new setter object
        \param[in] scripting a scripting object
     */
    PropertySetter(scripting::Scripting* scripting) : dukpp03::qt::FunctionCallable(), m_scripting(scripting)
    {

    }

    /*! Returns amount of required arguments
        \return amount of required arguments
     */
    virtual int requiredArguments()
    {
        return 2;
    }

    /*! Destroys a setter
     */
    virtual ~PropertySetter()
    {

    }

    /*! Clones an object
        \return copy of object
     */
    dukpp03::qt::Callable* clone()
    {
        return new scripting::database::PropertySetter<T>(m_scripting);
    }

    /*! Determines, whether it can be called with this context
        \param[in] ctx context
        \return result of check
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
            const QSet<QString>&  invisible_properties = scripting::database::getInvisibleProperties();
            if (invisible_properties.contains(STD2QSTRING(prop_name)))
            {
                prop = NULL;
            }
            if (prop)
            {
                a += 1;
                if (prop->baseType() == sad::db::TypeName<T>::baseName() && prop->pointerStarsCount() == 0)
                {
                   a += 1;
                   dukpp03::Maybe<T> maybe_value = dukpp03::GetValue< T, dukpp03::qt::BasicContext >::perform(ctx, 1);
                   if (maybe_value.exists())
                   {
                       a += 1;
                   }
                }
            }
        }
        return std::make_pair(a, a == 4);
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
            ctx->throwInvalidArgumentCountError(ctx->getTop(), 2);
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
                    dukpp03::Maybe<T> newvalue = dukpp03::GetValue< T, dukpp03::qt::BasicContext >::perform(ctx, 1);
                    if (newvalue.exists())
                    {
                        sad::db::Database* me = sad::Renderer::ref()->database("");
                        sad::Maybe<T> oldvalue = me->getProperty<T>(prop_name);
                        std::equal_to<T> comparator;
                        if (comparator(newvalue.value(), oldvalue.value()) == false)
                        {
                            core::Editor* p = m_scripting->editor();
                            gui::table::Delegate* d = p->panelProxy()->delegatesByName()[prop_name];
                            history::Command* c = new history::database::ChangeProperty<T>(oldvalue.value(), newvalue.value(), d);
                            c->commit(p);
                            p->currentBatchCommand()->add(c);
                        }
                    }
                    else
                    {
                        std::string name = dukpp03::qt::BasicContext::template typeName< T >();
                        ctx->throwInvalidTypeError(2, name);
                        throw dukpp03::ArgumentException();
                    }
                    return 0;
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
                ctx->throwError(std::string("Property \"") + prop_name + std::string("\" is not writeable"));
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
protected:
    /*! An inner scripting link
     */
    scripting::Scripting* m_scripting;
};

}

}
