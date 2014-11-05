#include "animations/animationsfontsize.h"
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

void sad::animations::FontSize::setState(sad::animations::Instance* i, double time)
{	
	double min = m_min_size;
	double max = m_max_size;
	double value = min + (max - min) * time / m_time;
	unsigned int kvalue = static_cast<unsigned int>(value);
	static_cast<sad::animations::AnimationFastCallFor<unsigned int> *>(i->fastCall())->call(kvalue);
}

bool sad::animations::FontSize::saveState(sad::animations::Instance* i)
{
	sad::db::Object* o = i->object();
	bool result = false;
	if (o && m_valid)
	{
		sad::Maybe<unsigned int> maybesize = o->getProperty<unsigned int>("fontsize"); 
		if (maybesize.exists())
		{
			result = true;

			i->oldState().clear();
			i->oldState() << sad::db::Variant(maybesize.value());

			if (o->isInstanceOf("sad::Label"))
			{
				i->setFastCall( sad::animations::make_fastcall(o, &sad::Label::setSize));
			}
			else
			{
				if (o->isInstanceOf("sad::db::custom::Object"))
				{
					i->setFastCall( sad::animations::make_fastcall(o, &sad::db::custom::Object::setFontSize));
				}
				else
				{
					i->setFastCall( new sad::animations::SetProperty<unsigned int>(o, "fontsize"));
				}
			}
		}
	}

	if (!result)
	{
		i->setFastCall( new sad::animations::AnimationFastCallFor<unsigned int>() );
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
