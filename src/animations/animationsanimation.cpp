#include "animations/animationsanimation.h"

#include "renderer.h"

#include "fuzzyequal.h"

#include <util/fs.h>

#include "db/schema/schema.h"
#include "db/dbproperty.h"
#include "db/save.h"
#include "db/load.h"
#include "db/dbfield.h"
#include "db/dbmethodpair.h"
#include "db/dbtable.h"

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

static sad::db::schema::Schema* AnimationAnimationSchema = NULL;

sad::db::schema::Schema* sad::animations::Animation::basicSchema()
{
    if (AnimationAnimationSchema == NULL)
    {
        AnimationAnimationSchema = new sad::db::schema::Schema();
        AnimationAnimationSchema->addParent(sad::db::Object::basicSchema());

        AnimationAnimationSchema->add(
            "looped",
            new sad::db::MethodPair<sad::animations::Animation, bool>(
				&sad::animations::Animation::looped,
                &sad::animations::Animation::setLooped
            )
        );
		AnimationAnimationSchema->add(
            "time",
            new sad::db::MethodPair<sad::animations::Animation, double>(
                &sad::animations::Animation::time,
                &sad::animations::Animation::setTime
            )
        );
        
        sad::ClassMetaDataContainer::ref()->pushGlobalSchema(AnimationAnimationSchema);
    }
    return AnimationAnimationSchema;
}

sad::db::schema::Schema* sad::animations::Animation::schema() const
{
    return sad::animations::Animation::basicSchema();
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

const sad::Vector<sad::animations::AbstractSavedObjectStateCreator*>& sad::animations::Animation::creators() const
{
    return m_creators;
}

void sad::animations::Animation::updateBeforePlaying()
{
	
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
