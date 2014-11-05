#include "animations/animationsfontlist.h"
#include "animations/animationsinstance.h"

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
		static_cast<sad::animations::AnimationFastCallFor<sad::String>*>(i->fastCall())->call(m_fonts[kvalue]);
	}
}


bool sad::animations::FontList::saveState(sad::animations::Instance* i)
{
	sad::db::Object* o = i->object();
	bool result = false;
	if (o && m_valid)
	{
		sad::Maybe<sad::String> maybefont = o->getProperty<sad::String>("font"); 
		if (maybefont.exists())
		{
			result = true;

			i->oldState().clear();
			i->oldState() << sad::db::Variant(maybefont.value());

			if (o->isInstanceOf("sad::Label"))
			{
				i->setFastCall( sad::animations::make_fastcall(o, &sad::Label::setFontName));
			}
			else
			{
				if (o->isInstanceOf("sad::db::custom::Object"))
				{
					i->setFastCall( sad::animations::make_fastcall(o, &sad::db::custom::Object::setFontName));
				}
				else
				{
					i->setFastCall( new sad::animations::SetProperty<sad::String>(o, "font"));
				}
			}
		}
	}

	if (!result)
	{
		i->setFastCall( new sad::animations::AnimationFastCallFor<sad::String>() );
	}
	return result;
}


void sad::animations::FontList::resetState(sad::animations::Instance* i)
{
	sad::db::Object* o = i->object();
	if (o && i->oldState().size() == 1)
	{
		sad::Maybe<sad::String> value = i->oldState()[0].get<sad::String>();
		if (value.exists())
		{
			o->setProperty("font", value.value());
		}
	}
}
