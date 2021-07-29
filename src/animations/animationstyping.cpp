#include "animations/animationstyping.h"
#include "animations/animationsinstance.h"

#include "animations/easing/easingfunction.h"

#include "animations/setstate/methodcall.h"
#include "animations/setstate/dummycommand.h"

#include "animations/animationssavedrenderingstringlimit.h"

#include "fuzzyequal.h"
#include "scene.h"
#include "camera.h"
#include "geometry2d.h"

#include "db/schema/schema.h"
#include "db/dbproperty.h"
#include "db/save.h"
#include "db/load.h"

#include "label.h"
#include "sprite2d.h"
#include "db/custom/customobject.h"

#include <util/fs.h>

#include <resource/resourcefile.h>

#include <3rdparty/picojson/valuetotype.h>

#include <fstream>


DECLARE_SOBJ_INHERITANCE(sad::animations::Typing, sad::animations::Animation);

// =============================== PUBLIC METHODS ==========================


sad::animations::Typing::Typing()
{
    m_creators.pushCreator<sad::animations::SavedRenderingStringLimit>("sad::animations::SavedRenderingStringLimit");
}

sad::animations::Typing::~Typing()
{
    
}


void sad::animations::Typing::start(sad::animations::Instance* i)
{
    // We do not save state, because it could be restored via value
}

void sad::animations::Typing::setState(sad::animations::Instance* i, double time)
{
    // Make it possible to reach end
    double time_position = m_easing->eval(time + 100, m_time);
    i->stateCommandAs<double>()->call(time_position);
}

sad::animations::setstate::AbstractSetStateCommand* sad::animations::Typing::stateCommand(sad::db::Object* o)
{
    if (this->applicableTo(o))
    {
        sad::animations::setstate::AbstractSetStateCommand* c;
        if (o->isInstanceOf("sad::Label"))
        {
            c = sad::animations::setstate::make(
                    o,
                    &sad::Label::setRenderingStringLimitAsRatioToLength
                );
        }
        else
        {
            c = sad::animations::setstate::make(
                o,
                &sad::db::custom::Object::setRenderingStringLimitAsRatioToLength
            );
        }
        return c;
    }
    return new sad::animations::setstate::DummyCommand<double>();
}


bool sad::animations::Typing::applicableTo(sad::db::Object* o)
{
    bool result = false;
    if (o && m_valid)
    {
        result = o->isInstanceOf("sad::Label") || o->isInstanceOf("sad::db::custom::Object");
    }
    return result;
}

