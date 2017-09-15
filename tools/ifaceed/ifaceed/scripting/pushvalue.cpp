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