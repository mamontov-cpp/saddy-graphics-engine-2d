#include "animations/animationsanimation.h"

#include "animations/easing/easingfunction.h"

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

#include <resource/resourcefile.h>

#include <3rdparty/picojson/valuetotype.h>

#include <fstream>

#include <sadmutex.h>

DECLARE_SOBJ_INHERITANCE(sad::animations::Animation, sad::resource::Resource);

// =============================== PUBLIC METHODS ==========================

sad::animations::Animation::Animation() : m_looped(false), m_time(0), m_inner_valid(true), m_valid(true)
{
    m_easing = new sad::animations::easing::Function();
    m_easing->addRef();
}

sad::animations::Animation::~Animation()
{
    m_easing->delRef();
}

static sad::db::schema::Schema* AnimationAnimationSchema = NULL;

static sad::Mutex AnimationAnimationSchemaInitLock;

sad::db::schema::Schema* sad::animations::Animation::basicSchema()
{
    if (AnimationAnimationSchema == NULL)
    {
        AnimationAnimationSchemaInitLock.lock();
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

            sad::db::Property* easing_prop = sad::animations::Animation::easingProperty();

             AnimationAnimationSchema->add(
                "easing",
                easing_prop
            );
        
            sad::ClassMetaDataContainer::ref()->pushGlobalSchema(AnimationAnimationSchema);
        }
        AnimationAnimationSchemaInitLock.unlock();
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

void sad::animations::Animation::setEasing(sad::animations::easing::Function* f)
{
    m_easing->delRef();
    m_easing = f;
    if (m_easing)
    {
        m_easing->addRef();
    }
}

sad::animations::easing::Function* sad::animations::Animation::easing() const
{
    return m_easing;
}

void sad::animations::Animation::start(sad::animations::Instance* i)
{

}

bool sad::animations::Animation::loadFromValue(const picojson::value& v)
{
    sad::Maybe<bool> looped = picojson::to_type<bool>(
                picojson::get_property(v, "looped")
    );
    sad::Maybe<double> time = picojson::to_type<double>(
                picojson::get_property(v, "time")
    );
    picojson::value const* maybeEasing = picojson::get_property(v, "easing");
    bool result = looped.exists() && time.exists();
    if (result)
    {
        m_looped = looped.value();
        m_time = time.value();
        if (maybeEasing)
        {
            sad::animations::easing::Function* oldeasing = m_easing;  
            result = sad::db::Load<sad::animations::easing::Function*>::perform(&m_easing, *maybeEasing);
            if (result)
            {
                delete oldeasing;
            }
            else
            {
                m_easing = oldeasing;
            }
        }
        else
        {
            setEasing(new sad::animations::easing::Function());
        }
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
    const sad::resource::ResourceFile & file,
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


// =============================== PRIVATE METHODS ==========================

sad::animations::Animation::Animation(const sad::animations::Animation& a)
: m_looped(a.m_looped),
m_time(a.m_time),
m_inner_valid(a.m_inner_valid),
m_valid(a.m_valid)
{
    m_easing = m_easing->clone();
}


sad::animations::Animation& sad::animations::Animation::operator=(const sad::animations::Animation& a)
{
    return *this;
}

// =============================== EASING PROPERTY METHODS ==========================

namespace sad
{

namespace animations
{

class EasingProperty: public sad::db::MethodPair<sad::animations::Animation, sad::animations::easing::Function*>
{
public:
    EasingProperty() : sad::db::MethodPair<sad::animations::Animation, sad::animations::easing::Function*>(
        &sad::animations::Animation::easing,
        &sad::animations::Animation::setEasing
    )
    {
        m_default_easing = new sad::animations::easing::Function();
        this->makeNonRequiredWithDefaultValue(new sad::db::Variant(m_default_easing));
    }

    virtual ~EasingProperty()
    {
        delete m_default_easing;
    }

    virtual bool copyAndSet(sad::db::Object * o, const sad::db::Variant & v)
    {
        sad::Maybe<sad::animations::easing::Function*> maybefn = v.get<sad::animations::easing::Function*>();
        if (maybefn.exists())
        {
            sad::db::Variant vv(maybefn.value()->clone());
            return this->set(o, vv);
        }
        return false;
    }

    virtual sad::db::Variant* defaultValue() const
    {
        sad::db::Variant* v = new sad::db::Variant(m_default_easing->clone());
        const_cast<sad::animations::EasingProperty*>(this)->m_variants.push_back(v);
        return v;
    }
protected:
    sad::animations::easing::Function* m_default_easing;    
    sad::PtrVector<sad::db::Variant> m_variants;
};

}

}

sad::db::Property* sad::animations::Animation::easingProperty()
{
    return new sad::animations::EasingProperty();
}