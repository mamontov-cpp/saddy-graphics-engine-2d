#include "scripting.h"
// ReSharper disable once CppUnusedIncludeDirective
#include "multimethod.h"
#include "abstractgetter.h"
#include "queryresource.h"

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
OVERLOAD_GET_AS_QUERY(sad::layouts::Grid)
OVERLOAD_GET_AS_QUERY(sad::p2d::app::Way)
OVERLOAD_GET_AS_QUERY(sad::dialogue::Dialogue)
OVERLOAD_GET_AS_QUERY(sad::animations::Animation)
OVERLOAD_GET_AS_QUERY(sad::animations::Composite)
OVERLOAD_GET_AS_QUERY(sad::animations::Blinking)
OVERLOAD_GET_AS_QUERY(sad::animations::CameraShaking)
OVERLOAD_GET_AS_QUERY(sad::animations::CameraRotation)
OVERLOAD_GET_AS_QUERY(sad::animations::Color)
OVERLOAD_GET_AS_QUERY(sad::animations::FontList)
OVERLOAD_GET_AS_QUERY(sad::animations::FontSize)
OVERLOAD_GET_AS_QUERY(sad::animations::Resize)
OVERLOAD_GET_AS_QUERY(sad::animations::Rotate)
OVERLOAD_GET_AS_QUERY(sad::animations::OptionList)
OVERLOAD_GET_AS_QUERY(sad::animations::TextureCoordinatesList)
OVERLOAD_GET_AS_QUERY(sad::animations::SimpleMovement)
OVERLOAD_GET_AS_QUERY(sad::animations::TextureCoordinatesContinuous)
OVERLOAD_GET_AS_QUERY(sad::animations::WayMoving)
OVERLOAD_GET_AS_QUERY(sad::animations::Instance)
OVERLOAD_GET_AS_QUERY(sad::animations::WayInstance)
OVERLOAD_GET_AS_QUERY(sad::animations::Group)


/*! Tries to get property for specified object
    \param[in] ctx context
    \param[in] pos position
    \param[in] property_name a name of property
    \return result
 */
template<
    typename T
>
inline dukpp03::Maybe<T> tryGetProperty(
    dukpp03::qt::BasicContext* ctx,
    duk_idx_t pos,
    const char* property_name
)
{
    duk_context* c = ctx->context();
    dukpp03::Maybe<T> result;
    if (duk_has_prop_string(c, pos, property_name))
    {
        if (duk_get_prop_string(c, pos, property_name))
        {
            result = dukpp03::GetValue<T, dukpp03::qt::BasicContext>::perform(ctx, -1);
            duk_pop(c);
        }
    }
    return result;
}

dukpp03::Maybe<sad::dialogue::Phrase> dukpp03::GetValue<sad::dialogue::Phrase, dukpp03::qt::BasicContext>::perform(
    dukpp03::qt::BasicContext* ctx,
    duk_idx_t pos
)
{
    dukpp03::Maybe<sad::dialogue::Phrase> result;
    duk_context* c = ctx->context();
    if (duk_is_object(c, pos))
    {
        dukpp03::Maybe<sad::String> actor_name = tryGetProperty<sad::String>(ctx, pos, "actorName");
        dukpp03::Maybe<sad::String> actor_portrait = tryGetProperty<sad::String>(ctx, pos, "actorPortrait");
        dukpp03::Maybe<sad::String> phrase = tryGetProperty<sad::String>(ctx, pos, "text");
        dukpp03::Maybe<double> duration = tryGetProperty<double>(ctx, pos, "duration");
        dukpp03::Maybe<sad::String> viewHint = tryGetProperty<sad::String>(ctx, pos, "viewHint");
        if (actor_name.exists() && actor_portrait.exists() && phrase.exists() && duration.exists() && viewHint.exists())
        {
            sad::dialogue::Phrase result_value;
            result_value.setActorName(actor_name.value());
            result_value.setActorPortrait(actor_portrait.value());
            result_value.setPhrase(phrase.value());
            result_value.setDuration(duration.value());
            result_value.setViewHint(viewHint.value());
            result.setValue(result_value);
        }
    }
    return result;
}