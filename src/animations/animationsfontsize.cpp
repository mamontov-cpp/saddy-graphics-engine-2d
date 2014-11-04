#include "animations/animationsfontsize.h"
#include "animations/animationsinstance.h"

#include "db/schema/schema.h"
#include "db/dbproperty.h"
#include "db/save.h"
#include "db/load.h"

#include <util/fs.h>

#include <resource/physicalfile.h>

#include <3rdparty/picojson/valuetotype.h>

#include <fstream>



DECLARE_SOBJ_INHERITANCE(sad::animations::FontSize, sad::animations::Animation);

// =============================== PUBLIC METHODS ==========================

sad::animations::FontSize::FontSize() : m_min_size(0), m_max_size(0)
{
	
}

sad::animations::FontSize::~FontSize()
{
	
}

bool sad::animations::FontSize::loadFromValue(const picojson::value& v)
{
	bool flag = this->sad::animations::Animation::loadFromValue(v);
	if (flag)
	{
		 sad::Maybe<unsigned int> minsize = picojson::to_type<unsigned int>(
												picojson::get_property(v, "min_size")
											);
		 sad::Maybe<unsigned int> maxsize = picojson::to_type<unsigned int>(
												picojson::get_property(v, "max_size")
											);
		bool result = minsize.exists() && maxsize.exists();
		if (result)
		{
			m_min_size = minsize.value();
			m_max_size = maxsize.value();
		}

		flag = flag && result;
	}
	return flag;
}


void sad::animations::FontSize::setMinSize(unsigned int size)
{
	m_min_size = size;
}

unsigned int sad::animations::FontSize::minSize() const
{
	return m_min_size;
}

void sad::animations::FontSize::setMaxSize(unsigned int size)
{
	m_max_size = size;
}

unsigned int sad::animations::FontSize::maxSize() const
{
	return m_max_size;
}

void sad::animations::FontSize::setState(sad::db::Object* o, double time)
{
	if (sad::is_fuzzy_zero(m_time))
		return;

	double min = m_min_size;
	double max = m_max_size;
	double value = min + (max - min) * time / m_time;
	unsigned int kvalue = static_cast<unsigned int>(value);
	if (o)
	{
		o->setProperty("fontsize", kvalue);
	}
}

bool sad::animations::FontSize::saveState(sad::animations::Instance* i)
{
	sad::db::Object* o = i->object();
	bool result = false;
	if (o)
	{
		sad::Maybe<unsigned int> maybesize = o->getProperty<unsigned int>("fontsize"); 
		if (maybesize.exists())
		{
			result = true;

			i->oldState().clear();
			i->oldState() << sad::db::Variant(maybesize.value());
		}
	}
	return result;
}


void sad::animations::FontSize::resetState(sad::animations::Instance* i)
{
	sad::db::Object* o = i->object();
	if (o && i->oldState().size() == 1)
	{
		sad::Maybe<unsigned int> value = i->oldState()[0].get<unsigned int>();
		if (value.exists())
		{
			o->setProperty("fontsize", value.value());
		}
	}
}
