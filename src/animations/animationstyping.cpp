#include "animations/animationstyping.h"
#include "animations/animationsinstance.h"

#include "animations/setstate/methodcall.h"
#include "animations/setstate/setproperty.h"

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

#include <resource/physicalfile.h>

#include <3rdparty/picojson/valuetotype.h>

#include <fstream>


DECLARE_SOBJ_INHERITANCE(sad::animations::Typing, sad::animations::Animation);

// =============================== PUBLIC METHODS ==========================


sad::animations::Typing::Typing()
{
	m_creators.pushProperty<sad::String>("text", "text");
}

sad::animations::Typing::~Typing()
{
	
}


void sad::animations::Typing::start(sad::animations::Instance* i)
{
	sad::Maybe<sad::String> text = i->object()->getProperty<sad::String>("text");
	i->setBasicString(text.value());
}

void sad::animations::Typing::setState(sad::animations::Instance* i, double time)
{
	if (i->basicString().size() != 0)
	{
		// Make it possible to reach end
		double pos = ((time + 100) / m_time) * i->basicString().size();
		sad::String text = i->basicString().subString(0, static_cast<long>(pos));
		i->stateCommandAs<sad::String>()->call(text);
	}
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
					&sad::Label::setString
                );
        }
        else
        {
			c = new sad::animations::setstate::SetProperty<sad::String>(o, "text");
        }
        return c;
    }
    return new sad::animations::setstate::DummyCommand<sad::String>();
}


bool sad::animations::Typing::applicableTo(sad::db::Object* o)
{
	bool result = false;
    if (o && m_valid)
    {
		result = o->getProperty<sad::String>("text").exists();
    }
    return result;
}

