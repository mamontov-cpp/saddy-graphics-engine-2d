#include "animations/animationsfontsize.h"

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

bool sad::animations::FontSize::saveState(sad::animations::Instance* i)
{
	sad::db::Object* o = i->object();

}
