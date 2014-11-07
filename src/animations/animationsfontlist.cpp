#include "animations/animationsfontlist.h"
#include "animations/animationsinstance.h"

#include "animations/setstate/methodcall.h"
#include "animations/setstate/setproperty.h"

#include "label.h"
#include "db/custom/customobject.h"

#include "db/schema/schema.h"
#include "db/dbproperty.h"
#include "db/save.h"
#include "db/load.h"

#include <util/fs.h>

#include <resource/physicalfile.h>

#include <3rdparty/picojson/valuetotype.h>

#include <fstream>



DECLARE_SOBJ_INHERITANCE(sad::animations::FontList, sad::animations::Animation);

// =============================== PUBLIC METHODS ==========================


sad::animations::FontList::FontList()
{
    m_creators.pushProperty<sad::String>("font", "font");
}

sad::animations::FontList::~FontList()
{
	
}

bool sad::animations::FontList::loadFromValue(const picojson::value& v)
{
	bool flag = this->sad::animations::Animation::loadFromValue(v);
	if (flag)
	{
		sad::Maybe<sad::Vector<sad::String> > fonts = picojson::to_type<sad::Vector<sad::String> >(
														picojson::get_property(v, "fonts")
													  );
		bool result = fonts.exists();
		if (result)
		{
			m_fonts = fonts.value();
			m_inner_valid = m_fonts.size() != 0;
			this->updateValidFlag();
		}

		flag = flag && result;
	}
	return flag;
}

void sad::animations::FontList::setFonts(const sad::Vector<sad::String>& fonts)
{
	m_fonts = fonts;
}

const sad::Vector<sad::String> & sad::animations::FontList::fonts() const
{
	return m_fonts;
}

void sad::animations::FontList::setState(sad::animations::Instance* i, double time)
{
	double value = static_cast<double>(m_fonts.size()) * time / m_time;
	unsigned int kvalue = static_cast<unsigned int>(value);
	if (kvalue < m_fonts.size())
	{
        i->stateCommandAs<sad::String>()->call(m_fonts[kvalue]);
	}
}

sad::animations::setstate::AbstractSetStateCommand* sad::animations::FontList::stateCommand(sad::db::Object* o)
{
    if (this->applicableTo(o))
    {
        sad::animations::setstate::AbstractSetStateCommand* c = NULL;
        if (o->isInstanceOf("sad::Label"))
        {
            c = sad::animations::setstate::make(
                    o,
                    &sad::Label::setFontName
                );
        }
        else
        {
            if (o->isInstanceOf("sad::db::custom::Object"))
            {
                c = sad::animations::setstate::make(
                        o,
                        &sad::db::custom::Object::setFontName
                    );
            }
            else
            {
                c = new sad::animations::setstate::SetProperty<sad::String>(o, "font");
            }
        }
        return c;
    }
    return new sad::animations::setstate::DummyCommand<sad::String>();
}


bool sad::animations::FontList::applicableTo(sad::db::Object* o)
{
    bool result = false;
    if (o && m_valid)
    {
        result = o->getProperty<sad::String>("font").exists();
    }
    return result;
}
