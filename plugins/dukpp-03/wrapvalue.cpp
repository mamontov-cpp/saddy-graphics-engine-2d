#include "dukpp-03/context.h"
#include "dukpp-03/classbinding.h"

void sad::dukpp03::WrapValue::perform(void* context, void* variant, bool wrapped)
{
    sad::db::Variant* v = reinterpret_cast<sad::db::Variant*>(variant);
    sad::Maybe<sad::db::Object*> mo =  v->get<sad::db::Object*>(false);
    if (mo.exists())
    {
        sad::db::Object* object = mo.value();
        const sad::String& object_name = object->serializableName();
        if (!wrapped || ((v->typeName() != object_name) && (v->typeName() != object_name + " *")))
        {
            sad::dukpp03::BasicContext* ctx = reinterpret_cast<sad::dukpp03::BasicContext*>(context);
            ::dukpp03::ClassBinding<dukpp03::BasicContext>* ctxbinding = ctx->getClassBinding(object->serializableName());
            if (ctxbinding == NULL)
            {
                sad::dukpp03::ClassBinding* binding = new sad::dukpp03::ClassBinding();
                binding->registerSchema(object->schema());
                binding->wrapValue(ctx);
                ctx->addClassBinding(object->serializableName(), binding);  
            }
            else
            {
                ctxbinding->wrapValue(ctx);       
            }
        }
    }
}