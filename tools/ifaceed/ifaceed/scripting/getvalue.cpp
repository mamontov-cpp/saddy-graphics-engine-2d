#include "scripting.h"
// ReSharper disable once CppUnusedIncludeDirective
#include "constructorcall.h"
#include "makeconstructor.h"
#include "scriptinglog.h"
#include "multimethod.h"
#include "makescriptingcall.h"
#include "makefunctioncall.h"
#include "abstractgetter.h"
#include "queryresource.h"
#include "isaabb.h"
#include "point2d.h"

#include <renderer.h>
#include <db/dbdatabase.h>


dukpp03::Maybe<sad::String> dukpp03::GetValue<sad::String, dukpp03::qt::BasicContext>::perform(
    dukpp03::qt::BasicContext* ctx,
    duk_idx_t pos
)
{
    dukpp03::Maybe<sad::String> result;
    if (duk_is_string(ctx->context(), pos))
    {
        result.setValue(duk_to_string(ctx->context(), pos));
    }
    return result;
}

dukpp03::Maybe<QStringList> dukpp03::GetValue<QStringList, dukpp03::qt::BasicContext>::perform(
    dukpp03::qt::BasicContext* c,
    duk_idx_t pos
)
{
    dukpp03::Maybe<QStringList> result;
    duk_context* ctx = c->context();
    if (duk_is_array(ctx, pos))
    {
        result.setValue(QStringList());
        // ReSharper disable once CppInitializedValueIsAlwaysRewritten
        duk_size_t i = 0, n = duk_get_length(ctx, pos);

        for (i = 0; i < n; i++) {
            duk_get_prop_index(ctx, pos, i);
            dukpp03::Maybe<QString> val = dukpp03::GetValue<QString, dukpp03::qt::BasicContext>::perform(c, -1);
            if (val.exists())
            {
                result.mutableValue().push_back(val.value());
            }
            else
            {
                result.clear();
                return result;
            }
            duk_pop(ctx);
        }
    }
    return result;
}


dukpp03::Maybe<sad::Vector<sad::String> > dukpp03::GetValue<sad::Vector<sad::String>, dukpp03::qt::BasicContext>::perform(
    dukpp03::qt::BasicContext* c,
    duk_idx_t pos
)
{
    dukpp03::Maybe<sad::Vector<sad::String> > result;
    duk_context* ctx = c->context();
    if (duk_is_array(ctx, pos))
    {
        result.setValue(sad::Vector<sad::String>());
        // ReSharper disable once CppInitializedValueIsAlwaysRewritten
        duk_size_t i = 0, n = duk_get_length(ctx, pos);

        for (i = 0; i < n; i++) {
            duk_get_prop_index(ctx, pos, i);
            dukpp03::Maybe<sad::String> val = dukpp03::GetValue<sad::String, dukpp03::qt::BasicContext>::perform(c, -1);
            if (val.exists())
            {
                result.mutableValue().push_back(val.value());
            }
            else
            {
                result.clear();
                return result;
            }
            duk_pop(ctx);
        }
    }
    return result;
}


dukpp03::Maybe<sad::db::Object*> dukpp03::GetValue<sad::db::Object*, dukpp03::qt::BasicContext>::perform(
    dukpp03::qt::BasicContext* ctx,
    duk_idx_t pos
)
{
    dukpp03::Maybe<sad::db::Object*> result;
    dukpp03::Maybe<sad::String> maybe_name = dukpp03::GetValue<sad::String, dukpp03::qt::BasicContext>::perform(ctx, pos);
    if (maybe_name.exists())
    {
        sad::Vector<sad::db::Object*> vector = sad::Renderer::ref()->database("")->queryByName(maybe_name.value());
        if (vector.size())
        {
            for (size_t i = 0; (i < vector.size()) && (result.exists() == false); i++)
            {
                if (vector[i]->Active) {
                    result.setValue(vector[i]);
                }
            }
        }
    }

    if (result.exists() == false)
    {
        dukpp03::Maybe<unsigned long long> maybe_major_id = dukpp03::GetValue<unsigned long long, dukpp03::qt::BasicContext>::perform(ctx, pos);
        if (maybe_major_id.exists())
        {
            sad::db::Object* object = sad::Renderer::ref()->database("")->queryByMajorId(maybe_major_id.value());
            if (object)
            {
                if (object->Active)
                {
                    result.setValue(object);
                }
            }
        }
    }
    return result;
}


#define OVERLOAD_GET_AS_QUERY(T)                                                 \
dukpp03::Maybe<T*> dukpp03::GetValue<T*, dukpp03::qt::BasicContext>::perform(    \
    dukpp03::qt::BasicContext* ctx,                                              \
    duk_idx_t pos                                                                \
)                                                                                \
{                                                                                \
    dukpp03::Maybe<T*> result;                                          \
    dukpp03::Maybe<sad::db::Object*> mb = dukpp03::GetValue<sad::db::Object*, dukpp03::qt::BasicContext>::perform(ctx, pos); \
    if (mb.exists())                                                             \
    {                                                                            \
        sad::db::TypeName<T>::init();                                            \
        if (mb.value()->isInstanceOf(sad::db::TypeName<T>::baseName()))          \
        {                                                                        \
            result.setValue(static_cast<T*>(mb.value()));                        \
        }                                                                        \
    }                                                                            \
    return result;                                                               \
}

OVERLOAD_GET_AS_QUERY(sad::Scene)
OVERLOAD_GET_AS_QUERY(sad::SceneNode)
