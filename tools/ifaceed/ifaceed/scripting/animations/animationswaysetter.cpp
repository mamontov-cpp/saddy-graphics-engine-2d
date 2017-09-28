#include "animationswaysetter.h"

// ReSharper disable once CppUnusedIncludeDirective
#include <db/save.h>


#include <renderer.h>

#include <db/dbdatabase.h>

#include <p2d/app/way.h>

// ================================= scripting::animations::WaySetter =================================

scripting::animations::WaySetter::WaySetter(
    scripting::Scripting* scripting
) :  scripting::animations::Setter<sad::animations::WayMoving, unsigned long long, history::animations::ChangeWayMovingWay>(scripting, "way")
{
    Converter converter = [](dukpp03::qt::BasicContext* ctx, duk_idx_t pos) {
        dukpp03::Maybe<sad::p2d::app::Way*> maybe_way = dukpp03::GetValue<sad::p2d::app::Way*, dukpp03::qt::BasicContext>::perform(ctx, pos);
        if (maybe_way.exists())
        {
            return  dukpp03::Maybe<unsigned long long>(maybe_way.value()->MajorId);
        }
        // We don't do nothing their, so 0 will fall through to condition, which in turn will return error
        return dukpp03::Maybe<unsigned long long>(0ull);
    };
    Condition condition = [](const unsigned long long& a) {
        dukpp03::Maybe<sad::String> result;
        sad::db::Object* o = sad::Renderer::ref()->database("")->queryByMajorId(a);
        bool valid = false;
        if (o)
        {
            if (o->isInstanceOf("sad::p2d::app::Way"))
            {
                valid = true;
            }
        }

        if (!valid)
        {
            result.setValue("set() : argument 3 is not a reference to way");
        }
        return result;
    };
    addConverter(converter);
    addCondition(condition);
}

dukpp03::qt::Callable* scripting::animations::WaySetter::clone()
{
    return new scripting::animations::WaySetter(*this);
}

scripting::animations::WaySetter::~WaySetter()
{

}
