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
// ReSharper disable once CppInconsistentNaming
const QSet<QString>& getInvisibleProperties();  // NOLINT(readability-redundant-declaration)

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
    virtual int requiredArguments() override
    {
        return 2;
    }

    /*! Destroys a setter
     */
	virtual ~PropertySetter() override = default;

    /*! Clones an object
        \return copy of object
     */
    dukpp03::qt::Callable* clone() override
    {
        return new scripting::database::PropertySetter<T>(m_scripting);
    }

    /*! Determines, whether it can be called with this context
        \param[in] ctx context
        \return result of check
     */
    virtual std::pair<int, bool> canBeCalled(dukpp03::qt::BasicContext* ctx) override
    {
        const int required_args = this->requiredArguments();
        if (ctx->getTop() != required_args)
        {
            return std::make_pair(-1, false);
        }
        int a = 0;
        const dukpp03::Maybe<sad::String> maybe_prop_name = dukpp03::GetValue< sad::String, dukpp03::qt::BasicContext >::perform(ctx, 0);
        if (maybe_prop_name.exists())
        {
            a += 1;
            sad::db::TypeName<T>::init();
            const sad::String& prop_name = maybe_prop_name.value();
            sad::db::Database* me = sad::Renderer::ref()->database("");
            sad::db::Property* prop = me->propertyByName(prop_name);
            const QSet<QString>&  invisible_properties = scripting::database::getInvisibleProperties();
            if (invisible_properties.contains(STD2QSTRING(prop_name)))
            {
                prop = nullptr;
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
    /*! Calls an actual function, performing action
        \param[in] ctx context
        \return amount of values on stack
     */
    virtual int call(dukpp03::qt::BasicContext* ctx) override
    {
        const int required_args = this->requiredArguments();
        if (ctx->getTop() != required_args)
        {
            ctx->throwInvalidArgumentCountError(ctx->getTop(), 2);
            throw dukpp03::ArgumentException();
            // ReSharper disable once CppUnreachableCode
            return 0;
        }
        const dukpp03::Maybe<sad::String> maybe_prop_name = dukpp03::GetValue< sad::String, dukpp03::qt::BasicContext >::perform(ctx, 0);
        if (maybe_prop_name.exists())
        {
            sad::db::TypeName<T>::init();
            const sad::String& prop_name = maybe_prop_name.value();
            sad::db::Database* me = sad::Renderer::ref()->database("");
            sad::db::Property* prop = me->propertyByName(prop_name);
            if (prop)
            {
                if (prop->baseType() == sad::db::TypeName<T>::baseName() && prop->pointerStarsCount() == 0)
                {
                    dukpp03::Maybe<T> new_value = dukpp03::GetValue< T, dukpp03::qt::BasicContext >::perform(ctx, 1);
                    if (new_value.exists())
                    {
                        sad::db::Database* db = sad::Renderer::ref()->database("");
                        sad::Maybe<T> old_value = db->getProperty<T>(prop_name);
                        std::equal_to<T> comparator;
                        if (comparator(new_value.value(), old_value.value()) == false)
                        {
                            core::Editor* p = m_scripting->editor();
                            gui::table::Delegate* d = p->panelProxy()->delegatesByName()[prop_name];
                            history::Command* c = new history::database::ChangeProperty<T>(old_value.value(), new_value.value(), d);
                            c->commit(p);
                            p->currentBatchCommand()->add(c);
                        }
                    }
                    else
                    {
                        // ReSharper disable once CppRedundantTemplateKeyword
                        const std::string name = dukpp03::qt::BasicContext::template typeName< T >();
                        ctx->throwInvalidTypeError(2, name);
                        throw dukpp03::ArgumentException();
                    }
                    return 0;
                }
                else
                {
                    ctx->throwError("Invalid property value");
                    throw dukpp03::ArgumentException();
                    // ReSharper disable once CppUnreachableCode
                    return 0;
                }
            }
            else
            {
                ctx->throwError(std::string("Property \"") + prop_name + std::string("\" is not write-able"));
                throw dukpp03::ArgumentException();
                // ReSharper disable once CppUnreachableCode
                return 0;
            }
        }
        else
        {
            ctx->throwInvalidTypeError(1, "sad::String");
            throw dukpp03::ArgumentException();
            // ReSharper disable once CppUnreachableCode
            return 0;
        }
        // ReSharper disable once CppUnreachableCode
        return 1;
    }
protected:
    /*! An inner scripting link
     */
    scripting::Scripting* m_scripting;
};

}

}
