#include "scripting.h"
// ReSharper disable once CppUnusedIncludeDirective
#include "abstractgetter.h"
#include "queryresource.h"



void dukpp03::PushValue<sad::String, dukpp03::qt::BasicContext>::perform(
    dukpp03::qt::BasicContext* ctx,
    const sad::String& v
)
{
    duk_push_string(ctx->context(), v.c_str());
}

void dukpp03::PushValue<QStringList, dukpp03::qt::BasicContext>::perform(
    dukpp03::qt::BasicContext* c,
    const QStringList& v
)
{
    duk_context* ctx = c->context();
    int arr_idx = duk_push_array(ctx);
    int index = 0;
    for (QStringList::const_iterator it = v.begin(); it != v.end(); ++it)
    {
        dukpp03::PushValue<QString, dukpp03::qt::BasicContext>::perform(c, *it);
        duk_put_prop_index(ctx, arr_idx, index);
        ++index;
    }
}

void dukpp03::PushValue<sad::Vector<sad::String>, dukpp03::qt::BasicContext>::perform(
    dukpp03::qt::BasicContext* c,
    const sad::Vector<sad::String>& v
)
{
    duk_context* ctx = c->context();
    int arr_idx = duk_push_array(ctx);
    int index = 0;
    for (size_t i = 0; i < v.size(); i++)
    {
        dukpp03::PushValue<sad::String, dukpp03::qt::BasicContext>::perform(c, v[i]);
        duk_put_prop_index(ctx, arr_idx, index);
        ++index;
    }
}

void dukpp03::PushValue<sad::Vector<unsigned long long>, dukpp03::qt::BasicContext>::perform(
    dukpp03::qt::BasicContext* c,
    const sad::Vector<unsigned long long>& v
)
{
    duk_context* ctx = c->context();
    int arr_idx = duk_push_array(ctx);
    int index = 0;
    for (size_t i = 0; i < v.size(); i++)
    {
        dukpp03::PushValue<unsigned long long, dukpp03::qt::BasicContext>::perform(c, v[i]);
        duk_put_prop_index(ctx, arr_idx, index);
        ++index;
    }
}




void dukpp03::PushValue<sad::dialogue::Phrase, dukpp03::qt::BasicContext>::perform(
    dukpp03::qt::BasicContext* c,
    const sad::dialogue::Phrase& v
)
{
    duk_context* ctx = c->context();
    int obj_idx = duk_push_object(ctx);

    dukpp03::PushValue<sad::String, dukpp03::qt::BasicContext>::perform(c, v.actorName());
    duk_put_prop_string(ctx, obj_idx, "actorName");

    dukpp03::PushValue<sad::String, dukpp03::qt::BasicContext>::perform(c, v.actorPortrait());
    duk_put_prop_string(ctx, obj_idx, "actorPortrait");

    dukpp03::PushValue<sad::String, dukpp03::qt::BasicContext>::perform(c, v.phrase());
    duk_put_prop_string(ctx, obj_idx, "text");

    dukpp03::PushValue<double, dukpp03::qt::BasicContext>::perform(c, v.duration());
    duk_put_prop_string(ctx, obj_idx, "duration");

    dukpp03::PushValue<sad::String, dukpp03::qt::BasicContext>::perform(c, v.viewHint());
    duk_put_prop_string(ctx, obj_idx, "viewHint");
}