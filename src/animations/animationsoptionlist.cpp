#include "animations/animationsoptionlist.h"
#include "animations/animationsinstance.h"

#include "db/schema/schema.h"
#include "db/dbproperty.h"
#include "db/save.h"
#include "db/load.h"

#include <sprite2d.h>

#include <util/fs.h>

#include <resource/physicalfile.h>

#include <3rdparty/picojson/valuetotype.h>

#include <fstream>

DECLARE_SOBJ_INHERITANCE(sad::animations::OptionList, sad::animations::Animation);

// =============================== PUBLIC METHODS ==========================

sad::animations::OptionList::OptionList()
{
	
}

sad::animations::OptionList::~OptionList()
{
	
}

bool sad::animations::OptionList::loadFromValue(const picojson::value& v)
{
	bool flag = this->sad::animations::Animation::loadFromValue(v);
	if (flag)
	{
		sad::Maybe<sad::Vector<sad::String> > list = picojson::to_type<sad::Vector<sad::String> >(
														picojson::get_property(v, "list")
													  );
		bool result = list.exists();
		if (result)
		{
			m_list = list.value();
		}

		flag = flag && result;
	}
	return flag;
}

void sad::animations::OptionList::setList(const sad::Vector<sad::String>& list)
{
	m_list = list;
}

const sad::Vector<sad::String> & sad::animations::OptionList::list() const
{
	return m_list;
}


void sad::animations::OptionList::setState(sad::db::Object* o, double time)
{
	if (sad::is_fuzzy_zero(m_time) || m_list.size() == 0)
		return;

	double value = static_cast<double>(m_list.size()) * time / m_time;
	unsigned int kvalue = static_cast<unsigned int>(value);
	if (kvalue < m_list.size())
	{
		o->setProperty("options", m_list[kvalue]);
	}
}

bool sad::animations::OptionList::saveState(sad::animations::Instance* i)
{
	sad::db::Object* o = i->object();
	bool result = false;
	if (o)
	{
		sad::Maybe<sad::String> maybeopts = o->getProperty<sad::String>("options"); 
		if (maybeopts.exists())
		{
			result = true;

			i->oldState().clear();
			i->oldState() << sad::db::Variant(maybeopts.value());
		}
	}
	return result;
}

void sad::animations::OptionList::resetState(sad::animations::Instance* i)
{
	sad::db::Object* o = i->object();
	if (o && i->oldState().size() == 1)
	{
		sad::Maybe<sad::String> value = i->oldState()[0].get<sad::String>();
		if (value.exists())
		{
			o->setProperty("options", value.value());
		}
	}
}
