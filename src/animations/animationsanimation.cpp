#include "animations/animationsanimation.h"

#include "renderer.h"

#include "fuzzyequal.h"

#include <util/fs.h>

#include <resource/physicalfile.h>

#include <3rdparty/picojson/valuetotype.h>

#include <fstream>

DECLARE_SOBJ_INHERITANCE(sad::animations::Animation, sad::resource::Resource);

// =============================== PUBLIC METHODS ==========================

sad::animations::Animation::Animation() : m_looped(false), m_time(0), m_valid(true), m_inner_valid(true)
{

}

sad::animations::Animation::~Animation()
{

}

void sad::animations::Animation::setLooped(bool looped)
{
    m_looped = looped;
}

bool sad::animations::Animation::looped() const
{
    return m_looped;
}

void sad::animations::Animation::setTime(double time)
{
    m_time = time;
	this->updateValidFlag();	
}

double sad::animations::Animation::time() const
{
    return m_time;
}

void sad::animations::Animation::start(sad::animations::Instance* i)
{

}

bool sad::animations::Animation::load(const picojson::value & v)
{
	return this->loadFromValue(v);
}

bool sad::animations::Animation::loadFromValue(const picojson::value& v)
{
    sad::Maybe<bool> looped = picojson::to_type<bool>(
                picojson::get_property(v, "looped")
    );
    sad::Maybe<double> time = picojson::to_type<double>(
                picojson::get_property(v, "time")
    );
    bool result = looped.exists() && time.exists();
    if (result)
    {
        m_looped = looped.value();
        m_time = time.value();
		this->updateValidFlag();
    }
    return result;
}

const sad::animations::SavedObjectStateCreators& sad::animations::Animation::creators() const
{
    return m_creators;
}

// =============================== PROTECTED METHODS ==========================

void sad::animations::Animation::updateValidFlag()
{
	m_valid = m_inner_valid && !sad::is_fuzzy_zero(m_time);
}

bool sad::animations::Animation::load(
    const sad::resource::PhysicalFile & file,
    sad::Renderer * r,
    const picojson::value& options
)
{
    std::ifstream stream(file.name().c_str());
    if (stream.bad())
    {
        sad::String newpath = util::concatPaths(r->executablePath(), file.name());
        stream.open(newpath.c_str());
    }
    bool result = false;
    if (stream.good())
    {
        picojson::value v;
        stream >> v;
        if (picojson::get_last_error().size() == 0)
        {
            result  = this->loadFromValue(v);
        }
    }
    return result;
}
