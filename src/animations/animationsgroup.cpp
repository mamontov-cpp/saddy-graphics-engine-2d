#include "animations/animationsgroup.h"
#include "animations/setstate/dummycommand.h"

#include <util/fs.h>

#include <resource/resourcefile.h>

#include <3rdparty/picojson/valuetotype.h>

#include <fuzzyequal.h>

#include <cmath>
#include <fstream>

#include "sadmutex.h"
#include "sadscopedlock.h"

#include "db/schema/schema.h"
#include "db/dbproperty.h"
#include "db/save.h"
#include "db/load.h"
#include "db/dbfield.h"
#include "db/dbmethodpair.h"
#include "db/dbtable.h"
#include "opticksupport.h"


DECLARE_SOBJ_INHERITANCE(sad::animations::Group, sad::animations::Process)

// =========================== PUBLIC METHODS ===========================

sad::animations::Group::Group() : 
m_looped(false), 
m_started(true), 
m_parent(nullptr),
m_sequential(false), 
m_current_instance(0)
{
    PROFILER_EVENT;
    
}

sad::animations::Group::Group(const sad::animations::Group& g) //-V730
{
    PROFILER_EVENT;
    copyState(g);
}

sad::animations::Group& sad::animations::Group::operator=(const sad::animations::Group& o)
{
    PROFILER_EVENT;
    clearLinks();
    copyState(o);
    return *this;
}

sad::animations::Group::~Group()
{
    PROFILER_EVENT;
    clearLinks();
}

static sad::db::schema::Schema* AnimationGroupSchema = nullptr;

static sad::Mutex AnimationGroupSchemaInit;

sad::db::schema::Schema* sad::animations::Group::basicSchema()
{
    PROFILER_EVENT;
    if (AnimationGroupSchema == nullptr)
    {
        AnimationGroupSchemaInit.lock();
        if (AnimationGroupSchema == nullptr)
        {
            AnimationGroupSchema = new sad::db::schema::Schema();
            AnimationGroupSchema->addParent(sad::db::Object::basicSchema());

            AnimationGroupSchema->add(
                "looped",
                new sad::db::MethodPair<sad::animations::Group, bool >(
                    &sad::animations::Group::looped,
                    &sad::animations::Group::setLooped
                )
            );
            AnimationGroupSchema->add(
                "instances",
                new sad::db::MethodPair<sad::animations::Group, sad::Vector<unsigned long long> >(
                    &sad::animations::Group::instances,
                    &sad::animations::Group::setInstances
                )
            );
            
            sad::db::Property* s_property = new sad::db::MethodPair<sad::animations::Group, bool>(
                &sad::animations::Group::isSequential,
                &sad::animations::Group::toggleIsSequential
            );
            s_property->makeNonRequiredWithDefaultValue(new sad::db::Variant(false));
            AnimationGroupSchema->add("sequential", s_property);
        
            sad::ClassMetaDataContainer::ref()->pushGlobalSchema(AnimationGroupSchema);
        }
        AnimationGroupSchemaInit.unlock();
    }
    return AnimationGroupSchema;
}


sad::db::schema::Schema* sad::animations::Group::schema() const
{
    PROFILER_EVENT;
    return sad::animations::Group::basicSchema();
}

bool sad::animations::Group::isSequential() const
{
    PROFILER_EVENT;
    return m_sequential;
}

void sad::animations::Group::toggleIsSequential(bool flag)
{
    PROFILER_EVENT;
    m_sequential = flag;
}

void sad::animations::Group::clear()
{
    PROFILER_EVENT;
    sad::ScopedLock own_lock(&m_lock);
    for (size_t i = 0; i < m_instances.size(); i++)
    {
        m_instances[i]->cancel(m_parent);
    }
    m_instances.clear();
    clearLinks();
}

void sad::animations::Group::add(sad::animations::Instance* i)
{
    PROFILER_EVENT;
    if (!i)
    {
        return;
    }
    sad::ScopedLock own_lock(&m_lock);
    InstanceLink* l = new InstanceLink();
    if (this->table())
    {
        l->setDatabase(this->table()->database());
    }
    l->setTableName("animationinstances");
    l->setMajorId(i->MajorId);
    l->setObject(i);
    m_instance_links.push_back(l);

    if (i && m_instances.count())
    {
        m_instances.push_back(i);
    }
}

void  sad::animations::Group::insert(int pos, sad::animations::Instance* i)
{
    PROFILER_EVENT;
    if (!i)
    {
        return;
    }
    if (pos < 0)
    {
        pos = 0;
    }
    sad::ScopedLock own_lock(&m_lock);
    InstanceLink* l = new InstanceLink();
    if (this->table())
    {
        l->setDatabase(this->table()->database());
    }
    l->setTableName("animationinstances");
    l->setMajorId(i->MajorId);
    l->setObject(i);
    if (pos >= m_instance_links.size())
    {
        m_instance_links.push_back(l);
    }
    else
    { 
        m_instance_links.insert(l, pos);
    }
    if (i && m_instances.count())
    {
        m_instances.push_back(i);
    }
}

void sad::animations::Group::remove(sad::animations::Instance* instance)
{
    PROFILER_EVENT;
    if (!instance)
    {
        return;
    }
    sad::ScopedLock own_lock(&m_lock);
    size_t i;
    if (m_instances.size() != 0)
    {
        for (i = 0; i < m_instances.size(); i++)
        {
            if (m_instances[i] == instance)
            {
                m_instances[i]->cancel(m_parent);
                m_instances.removeAt(i);
                --i;
            }
        }
    }
    {
        for (i = 0; i < m_instance_links.size(); i++)
        {
            if (m_instance_links[i]->get() == instance)
            {
                delete m_instance_links[i];
                m_instance_links.removeAt(i);
                --i;
            }
        }
    }
}

size_t sad::animations::Group::count()
{
    PROFILER_EVENT;
    sad::ScopedLock own_lock(&m_lock);
    return m_instance_links.size();
}


sad::animations::Instance* sad::animations::Group::get(int pos)
{
    PROFILER_EVENT;
    sad::ScopedLock own_lock(&m_lock);
    if ((pos < 0) || (pos >= m_instance_links.size()))
    {
        return nullptr;
    }
    return m_instance_links[pos]->get();
}


unsigned long long sad::animations::Group::getMajorId(int pos)
{
    PROFILER_EVENT;
    sad::ScopedLock own_lock(&m_lock);
    if ((pos < 0) || (pos >= m_instance_links.size()))
    {
        return 0;
    }
    return m_instance_links[pos]->majorId();
}

void sad::animations::Group::setTable(sad::db::Table* t)
{
    PROFILER_EVENT;
    sad::ScopedLock own_lock(&m_lock);
    this->sad::db::Object::setTable(t);
    for(size_t i = 0; i < m_instance_links.size(); i++)
    {
        m_instance_links[i]->setTable(t);
    }
}

static sad::String AnimationsGroupSerializableName = "sad::animations::Group";

const sad::String& sad::animations::Group::serializableName() const
{
    PROFILER_EVENT;
    return AnimationsGroupSerializableName;
}

void sad::animations::Group::setInstances(const sad::Vector<unsigned long long>& v)
{
    PROFILER_EVENT;
    sad::ScopedLock own_lock(&m_lock);
    m_instance_links.clear();

    sad::Vector<unsigned long long> uniques =  v;
    for(size_t i = 0; i < uniques.size(); ++i)
    {
        for(size_t j = i + 1; j < uniques.size(); j++)
        {
            if (uniques[i] == uniques[j])
            {
                uniques.removeAt(j);
                --j;
            }
        }
    }

    for(size_t i = 0; i < uniques.size(); i++)
    {
        InstanceLink* l = new InstanceLink();
        if (this->table())
        {
            l->setDatabase(this->table()->database());
        }
        l->setTableName("animationinstances");
        l->setMajorId(uniques[i]);
        m_instance_links.push_back(l);
    }
}

sad::Vector<unsigned long long> sad::animations::Group::instances() const
{
    PROFILER_EVENT;
    sad::ScopedLock own_lock(&(const_cast<sad::animations::Group*>(this)->m_lock));
    sad::Vector<unsigned long long> result;

    for(size_t i = 0; i < m_instance_links.size(); i++)
    {
        result << m_instance_links[i]->majorId();
    }

    return result;
}

int sad::animations::Group::findInstance(unsigned long long id)
{
    PROFILER_EVENT;
    sad::ScopedLock own_lock(&m_lock);
    for(size_t i = 0; i < m_instance_links.size(); i++)
    {
        if (m_instance_links[i]->majorId() == id)
        {
            return i;
        }
    }
    return -1;
}

void sad::animations::Group::setLooped(bool looped)
{
    PROFILER_EVENT;
    sad::ScopedLock own_lock(&m_lock);
    m_looped = looped;
}

bool sad::animations::Group::looped() const
{
    PROFILER_EVENT;
    return m_looped;
}

void sad::animations::Group::addAsLink(sad::animations::Instance* i)
{
    PROFILER_EVENT;
    sad::ScopedLock own_lock(&m_lock);
    if (i)
    {
        InstanceLink* l = new InstanceLink();
        if (this->table())
        {
            l->setDatabase(this->table()->database());
        }
        l->setTableName("animationinstances");
        l->setMajorId(i->MajorId);
        m_instance_links.push_back(l);

        if (i && m_instances.count()) 
        {
            m_instances.push_back(i);
        }
    }
}

void sad::animations::Group::removeAsLink(sad::animations::Instance* inst)
{
    PROFILER_EVENT;
    sad::ScopedLock own_lock(&m_lock);
    if (!inst)
    {
        return;
    }
    for(size_t i = 0; i < m_instance_links.size(); ++i)
    {
        if (m_instance_links[i]->majorId() == inst->MajorId)
        {
            m_instance_links.removeAt(i);
            --i;
        }
    }

    sad::Vector<sad::animations::Instance*>::iterator it = std::find(m_instances.begin(), m_instances.end(), inst);
    if (it != m_instances.end())
    {
        m_instances.erase(it);
    }
}

void sad::animations::Group::insertAsLink(int pos, sad::animations::Instance* i)
{
    PROFILER_EVENT;
    sad::ScopedLock own_lock(&m_lock);
    if (i)
    {
        InstanceLink* l = new InstanceLink();
        l->setDatabase(this->table()->database());
        l->setTableName("animationinstances");
        l->setMajorId(i->MajorId);
        m_instance_links.insert(l, pos);

        if (i && m_instances.count()) 
        {
            m_instances.insert(i, pos);
        }
    }
}

void sad::animations::Group::restart(sad::animations::Animations* animations)
{
    PROFILER_EVENT;
    sad::ScopedLock own_lock(&m_lock);
    if (m_sequential)
    {
       if (m_current_instance < m_instances.size())
       {
           m_instances[m_current_instance]->cancel(animations);
           m_instances[m_current_instance]->restart(animations);
       }
    }
    else
    {
        for(size_t i = 0; i < m_instances.size(); i++)
        {
            m_instances[i]->cancel(animations);
        }
        for(size_t i = 0; i < m_instances.size(); i++)
        {
            m_instances[i]->restart(animations);
        }
    }
    m_instances.clear();
    m_started = true;
}

void sad::animations::Group::clearFinished()
{
    PROFILER_EVENT;
    sad::ScopedLock own_lock(&m_lock);
    if (m_instances.size() == 0) 
    {
        getInstances(m_instances);
    }
    if (m_sequential)
    {
        m_current_instance = 0;
        if (m_instances.size() != 0)
        {
            m_instances[m_current_instance]->clearFinished();
        }
        m_finished_instances.clear();
    }
    else 
    {
        for(size_t i = 0; i < m_instances.size(); i++)
        {
            m_instances[i]->clearFinished();
        }
    }
}

bool sad::animations::Group::finished() const
{
    PROFILER_EVENT;
    sad::ScopedLock own_lock(&(const_cast<sad::animations::Group*>(this)->m_lock));
    return m_instances.size() == 0;
}

void sad::animations::Group::process(sad::animations::Animations* animations)
{
    PROFILER_EVENT;
    sad::ScopedLock own_lock(&m_lock);
    if (finished())
    {
        if (!m_looped)
        {
            fireOnStartCallbacks();
        }
        if (m_started || m_looped)
        {
            clearFinished();
            m_started = false;
        }
    }

    size_t old_size = m_instances.size();
    if (m_sequential)
    {
        if (m_current_instance < m_instances.size())
        {
           sad::animations::Instance* instance = m_instances[m_current_instance];
           instance->process(animations, false);
           if (instance->finished())
           {
               m_finished_instances << m_instances[m_current_instance];
               m_instances.removeAt(m_current_instance);
               if (m_instances.size() != 0)
               {
                   m_instances[m_current_instance]->clearFinished();
               }
           }
        }
    }
    else
    {
        for(size_t i = 0; i < m_instances.size(); i++)
        {
            m_instances[i]->process(animations);
            if (m_instances[i]->finished())
            {
                m_instances.removeAt(i);
                --i;
            }
        }
    }


    if (old_size != m_instances.size() && m_instances.size() == 0)
    {
        if (m_sequential)
        {
            for(size_t i = 0; i < m_finished_instances.size(); i++)
            {
                m_finished_instances[i]->restoreObjectState(animations);
            }
        }
        if (!m_looped)
        {        
            for(size_t i = 0; i < m_callbacks_on_end.size(); i++)
            {
                m_callbacks_on_end[i]->invoke();
            }
        }
        else
        {
            this->clearFinished();
        }
    }
}

void sad::animations::Group::pause()
{
    PROFILER_EVENT;
    sad::ScopedLock own_lock(&m_lock);
    if (m_sequential)
    {
        if (m_current_instance < m_instances.size())
        {
            m_instances[m_current_instance]->pause();
        }
    }
    else
    {
        for(size_t i = 0; i < m_instances.size(); i++)
        {
            m_instances[i]->pause();
        }
    }
}

void sad::animations::Group::resume()
{
    PROFILER_EVENT;
    sad::ScopedLock own_lock(&m_lock);
    if (m_sequential)
    {
        if (m_current_instance < m_instances.size())
        {
            m_instances[m_current_instance]->resume();
        }
    }
    else
    {
        for(size_t i = 0; i < m_instances.size(); i++)
        {
            m_instances[i]->resume();
        }
    }
}

void sad::animations::Group::cancel(sad::animations::Animations* animations)
{
    PROFILER_EVENT;
    sad::ScopedLock own_lock(&m_lock);
    if (m_sequential)
    {
        for(size_t i = 0; i < m_finished_instances.size(); i++)
        {
            m_finished_instances[i]->restoreObjectState(animations);
        }
        m_finished_instances.clear();
        if (m_current_instance < m_instances.size())
        {
            m_instances[m_current_instance]->cancel(animations);
        }        
    }
    else
    {
        for(size_t i = 0; i < m_instances.size(); i++)
        {
            m_instances[i]->cancel(animations);
        }
    }

    m_instances.clear();
    m_started = true;
}

void sad::animations::Group::addedToPipeline()
{
    PROFILER_EVENT;
     this->clearFinished();
}

void sad::animations::Group::removedFromPipeline()
{
    PROFILER_EVENT;

}

sad::animations::Callback*  sad::animations::Group::addCallbackOnEnd(sad::animations::Callback* c)
{
    PROFILER_EVENT;
    sad::ScopedLock own_lock(&m_lock);
    m_callbacks_on_end << c;
    return c;
}

sad::animations::Callback* sad::animations::Group::addCallbackOnStart(sad::animations::Callback* c)
{
    PROFILER_EVENT;
    sad::ScopedLock own_lock(&m_lock);
    m_callbacks_on_start << c;
    return c;
}


void sad::animations::Group::removeCallbackOnStart(sad::animations::Callback* c)
{
    PROFILER_EVENT;
    sad::ScopedLock own_lock(&m_lock);
    sad::PtrVector<sad::animations::Callback>::iterator it = std::find(m_callbacks_on_start.begin(), m_callbacks_on_start.end(), c);
    if (it != m_callbacks_on_start.end())
    {
        m_callbacks_on_start.erase(it);
        delete c;
    }
}

void sad::animations::Group::removeCallbackOnEnd(sad::animations::Callback* c)
{
    PROFILER_EVENT;
    sad::ScopedLock own_lock(&m_lock);
    sad::PtrVector<sad::animations::Callback>::iterator it = std::find(m_callbacks_on_end.begin(), m_callbacks_on_end.end(), c);
    if (it != m_callbacks_on_end.end())
    {
        m_callbacks_on_end.erase(it);
        delete c;
    }
}

void sad::animations::Group::removeCallback(sad::animations::Callback* c)
{
    PROFILER_EVENT;
    removeCallbackOnStart(c);
    removeCallbackOnEnd(c);
}

void sad::animations::Group::clearCallbacksOnStart()
{
    PROFILER_EVENT;
    sad::ScopedLock own_lock(&m_lock);
    for (size_t i = 0; i < m_callbacks_on_start.size(); i++)
    {
        delete m_callbacks_on_start[i];
    }
    m_callbacks_on_start.clear();
}

void sad::animations::Group::clearCallbacksOnEnd()
{
    PROFILER_EVENT;
    sad::ScopedLock own_lock(&m_lock);
    for (size_t i = 0; i < m_callbacks_on_end.size(); i++)
    {
        delete m_callbacks_on_end[i];
    }
    m_callbacks_on_end.clear();
}

void sad::animations::Group::clearCallbacks()
{
    PROFILER_EVENT;
    clearCallbacksOnStart();
    clearCallbacksOnEnd();
}

bool sad::animations::Group::isRelatedToMatchedObject(const std::function<bool(sad::db::Object*)>& f)
{
    PROFILER_EVENT;
    bool is_related = false;
    for(size_t i = 0; i < m_instances.size(); i++)
    {
        is_related = is_related || m_instances[i]->isRelatedToMatchedObject(f);
    }
    return is_related;
}


void sad::animations::Group::stopInstancesRelatedToMatchedObject(const std::function<bool(sad::db::Object*)>& f, sad::animations::Animations* a)
{
    PROFILER_EVENT;
    sad::ScopedLock own_lock(&m_lock);
    if (m_instances.size() == 0)
    {
        getInstances(m_instances);
    }
    for(size_t i = 0; i < m_instances.size(); i++)
    {
        if (m_instances[i]->isRelatedToMatchedObject(f))
        {
            m_instances[i]->cancel(a);
            m_instances.removeAt(i);
            --i;
        }
    }
}

bool sad::animations::Group::isRelatedToMatchedAnimation(const std::function<bool(sad::animations::Animation*)>& f)
{
    PROFILER_EVENT;
    sad::ScopedLock own_lock(&m_lock);
    bool is_related = false;
    for(size_t i = 0; i < m_instances.size(); i++)
    {
        is_related = is_related || m_instances[i]->isRelatedToMatchedAnimation(f);
    }
    return is_related;
}


void sad::animations::Group::stopInstancesRelatedToMatchedAnimation(const std::function<bool(sad::animations::Animation*)>& f, sad::animations::Animations* a)
{
    PROFILER_EVENT;
    sad::ScopedLock own_lock(&m_lock);
    if (m_instances.size() == 0)
    {
        getInstances(m_instances);
    }
    for(size_t i = 0; i < m_instances.size(); i++)
    {
        if (m_instances[i]->isRelatedToMatchedAnimation(f))
        {
            m_instances[i]->cancel(a);
            m_instances.removeAt(i);
            --i;
        }
    }
}


bool sad::animations::Group::isRelatedToMatchedProcess(const std::function<bool(sad::animations::Process*)>& f)
{
    PROFILER_EVENT;
    sad::ScopedLock own_lock(&m_lock);
    bool is_related = false;
    for(size_t i = 0; i < m_instances.size(); i++)
    {
        is_related = is_related || m_instances[i]->isRelatedToMatchedProcess(f);
    }
    return is_related;
}


void sad::animations::Group::stopInstancesRelatedToMatchedProcess(const std::function<bool(sad::animations::Process*)>& f, sad::animations::Animations* a)
{
    PROFILER_EVENT;
    sad::ScopedLock own_lock(&m_lock);
    if (m_instances.size() == 0)
    {
        getInstances(m_instances);
    }
    for(size_t i = 0; i < m_instances.size(); i++)
    {
        if (m_instances[i]->isRelatedToMatchedProcess(f))
        {
            m_instances[i]->cancel(a);
            m_instances.removeAt(i);
            --i;
        }
    }
}


// =========================== PROTECTED METHODS ===========================

void sad::animations::Group::clearLinks()
{
    PROFILER_EVENT;
    sad::ScopedLock own_lock(&m_lock);
    for(size_t i = 0; i < m_instance_links.size(); i++)
    {
        delete m_instance_links[i];
    }
    m_instance_links.clear();
}

void sad::animations::Group::getInstances(sad::Vector<sad::animations::Instance*> & result)
{
    PROFILER_EVENT;
    sad::ScopedLock own_lock(&m_lock);
    result.clear();

    for(size_t i = 0; i < m_instance_links.size(); i++)
    {
        sad::animations::Instance* o  = m_instance_links[i]->get();
        if (o)
        {
            result.push_back(o);
        }
    }
}


void sad::animations::Group::copyState(const sad::animations::Group& o)
{
    PROFILER_EVENT;
    sad::ScopedLock lock(&(const_cast<sad::animations::Group&>(o)).m_lock);
    sad::ScopedLock own_lock(&m_lock);

    m_sequential = o.m_sequential;
    m_instances.clear();
    m_looped = o.m_looped;
    m_current_instance = o.m_current_instance;
    m_finished_instances.clear();
    // Copy end callbacks
    for(size_t i = 0; i < m_callbacks_on_end.size(); i++)
    {
        delete m_callbacks_on_end[i];
    }
    m_callbacks_on_end.clear();
    for(size_t i = 0; i < o.m_callbacks_on_end.size(); i++)
    {
        m_callbacks_on_end << o.m_callbacks_on_end[i]->clone();
    }
    // Copy start callbacks
     for(size_t i = 0; i < m_callbacks_on_start.size(); i++)
    {
        delete m_callbacks_on_start[i];
    }
    m_callbacks_on_start.clear();
    for(size_t i = 0; i < o.m_callbacks_on_start.size(); i++)
    {
        m_callbacks_on_start << o.m_callbacks_on_start[i]->clone();
    }
    for(size_t i = 0; i < o.m_instance_links.size(); i++)
    {
        m_instance_links.push_back(new InstanceLink(*(o.m_instance_links[i])));
    }
}

void sad::animations::Group::fireOnStartCallbacks()
{
    PROFILER_EVENT;
    sad::ScopedLock own_lock(&m_lock);
    for(size_t i = 0; i < m_callbacks_on_start.size(); i++)
    {
        m_callbacks_on_start[i]->invoke();
    }
}
