#include "animationswaysetter.h"

#include "../tovalue.h"
#include "../queryobject.h"

#include <p2d/app/way.h>

scripting::animations::WaySetter::WaySetter(
    QScriptEngine* e,
) :  scripting::animations::Setter<sad::animations::WayMoving, unsigned long long, history::animations::ChangeWayMovingWay>(e, "way")
{

}

scripting::animations::WaySetter::~WaySetter()
{

}

sad::Maybe<QString> scripting::animations::WaySetter::canBeCalled(QScriptContext* ctx)
{
    sad::Maybe<QString> result = this->scripting::animations::Setter<sad::animations::WayMoving, unsigned long long, history::animations::ChangeWayMovingWay>::canBeCalled(ctx);
    if (result.exists() == false)
    {
        sad::Maybe<unsigned long long> value = scripting::ToValue<unsigned long long>::perform(ctx->argument(2));
        if (value.exists())
        {
            unsigned long long v = value.value();
            if (v != 0)
            {
                sad::Maybe<sad::p2d::app::Way*> w = scripting::query<sad::p2d::app::Way*>(v);
                if (w.exists() == false)
                {
                    result.setValue("Way must be major id for real way");
                }
            }
        }
    }
}
