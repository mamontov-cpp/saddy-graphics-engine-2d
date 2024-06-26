#include "animations/animationscomposite.h"
#include "animations/animationsinstance.h"

#include "fuzzyequal.h"
#include "sadmutex.h"

#include "db/schema/schema.h"
#include "db/dbproperty.h"
#include "db/save.h"
#include "db/load.h"
#include "db/dbfield.h"
#include "db/dbmethodpair.h"
#include "db/dbtable.h"

#include <util/fs.h>

#include <resource/resourcefile.h>

#include <3rdparty/picojson/valuetotype.h>

#include <fstream>

#include <cstdio>
#include "opticksupport.h"

DECLARE_SOBJ_INHERITANCE(sad::animations::Composite, sad::animations::Animation);

// ====================================== PUBLIC METHODS ======================================

sad::animations::Composite::Composite()
{
    PROFILER_EVENT;

}

sad::animations::Composite::Composite(const sad::animations::Composite& a)
{
    PROFILER_EVENT;
    copy(a);
}

sad::animations::Composite& sad::animations::Composite::operator=(const sad::animations::Composite& a)
{
    PROFILER_EVENT;
    clear();
    copy(a);
    return *this;
}

sad::animations::Composite::~Composite()
{
    PROFILER_EVENT;
    clear();
    for(sad::Hash<sad::db::Object*, Commands>::iterator it = m_commands.begin();
        it != m_commands.end();
        ++it)
    {
        Commands& c = it.value();
        for(size_t i = 0; i < c.size(); i++)
        {
            delete c[i];
        }
    }
}

void sad::animations::Composite::setTable(sad::db::Table* t)
{
    PROFILER_EVENT;
    this->sad::resource::Resource::setTable(t);
    m_database.setValue(t->database());
    m_tree.clear();
    for(size_t i = 0; i < m_links.size(); i++)
    {
        m_links[i]->setDatabase(t->database());
    }
}

void sad::animations::Composite::setPhysicalFile(sad::resource::ResourceFile * file)
{
    PROFILER_EVENT;
    this->sad::resource::Resource::setPhysicalFile(file);
    m_tree.setValue(file->tree());
    m_database.clear();
    for(size_t i = 0; i < m_links.size(); i++)
    {
        m_links[i]->setTree(file->tree());
    }
}

static sad::db::schema::Schema* AnimationCompositeSchema = nullptr;

static sad::Mutex AnimationCompositeSchemaInit;

sad::db::schema::Schema* sad::animations::Composite::basicSchema()
{
    PROFILER_EVENT;
    if (AnimationCompositeSchema == nullptr)
    {
        AnimationCompositeSchemaInit.lock();
        if (AnimationCompositeSchema == nullptr)
        {
            AnimationCompositeSchema = new sad::db::schema::Schema();
            AnimationCompositeSchema->addParent(sad::animations::Animation::basicSchema());

            AnimationCompositeSchema->add(
                "list",
                new sad::db::MethodPair<sad::animations::Composite, sad::Vector<unsigned long long> >(
                    &sad::animations::Composite::animationMajorIds,
                    &sad::animations::Composite::setAnimationsMajorId
                )
            );
        
            sad::ClassMetaDataContainer::ref()->pushGlobalSchema(AnimationCompositeSchema);
        }
        AnimationCompositeSchemaInit.unlock();
    }
    return AnimationCompositeSchema;
}

sad::db::schema::Schema* sad::animations::Composite::schema() const
{
    PROFILER_EVENT;
    return sad::animations::Composite::basicSchema();
}

bool sad::animations::Composite::load(const picojson::value & v)
{
    PROFILER_EVENT;
    return this->sad::db::Object::load(v);
}

bool sad::animations::Composite::loadFromValue(const picojson::value& v)
{
    PROFILER_EVENT;
    bool result = this->sad::animations::Animation::loadFromValue(v);
    if (result)
    {
        result = false;
        sad::Maybe<sad::Vector<sad::String> > maybenames = picojson::to_type<sad::Vector<sad::String> >(
                picojson::get_property(v, "list")
        );
        if (maybenames.exists())
        {
            setAnimationsNames(maybenames.value());
            result = true;
        }
    }
    return result;
}

void sad::animations::Composite::start(sad::animations::Instance* i)
{
    PROFILER_EVENT;
    if (this->applicableTo(i->object()))
    {
        sad::Vector<sad::animations::setstate::AbstractSetStateCommand* > commands;
        for(size_t ii = 0; ii < m_links.size(); ii++)
        {
            sad::animations::Animation* a = m_links[ii]->object(true);
            if (a)
            {
                a->start(i);
                commands << a->stateCommand(i->object());
            }            
        }
        if (m_commands.contains(i->object()))
        {
            Commands& c = m_commands[i->object()];
            for(size_t ii = 0; ii < c.size(); ii++)
            {
                delete c[ii];
            }
            m_commands[i->object()] = commands;
        }
        else
        {
            m_commands.insert(i->object(), commands);
        }
    }
}

bool sad::animations::Composite::applicableTo(sad::db::Object* o)
{
    PROFILER_EVENT;
    bool result = false;
    if (o)
    {
        result = true;
        for(size_t ii = 0; ii < m_links.size(); ii++)
        {
            sad::animations::Animation* a = m_links[ii]->object(true);
            if (a)
            {
                result = result && a->applicableTo(o);
            }
            else
            {
                result = false;
                m_inner_valid = false;
                this->updateValidFlag();
                //printf("Updated valid to false\n");
            }
        }
    }
    return result;
}

const sad::Vector<sad::animations::AbstractSavedObjectStateCreator*>& sad::animations::Composite::creators() const
{
    PROFILER_EVENT;
    sad::animations::Composite* c = const_cast<sad::animations::Composite*>(this);
    c->m_temp_creators.clear();
    for(size_t ii = 0; ii < m_links.size(); ii++)
    {
        sad::animations::Animation* a = m_links[ii]->object(true);
        if (a)
        {
            const sad::Vector<sad::animations::AbstractSavedObjectStateCreator*>& v = a->creators();
            for(size_t jj = 0; jj < v.size(); jj++)
            {
                c->m_temp_creators << v[jj];
            }
        }
    }
    return m_temp_creators;
}

void sad::animations::Composite::add(unsigned long long majorid)
{
    PROFILER_EVENT;
    sad::TreeDbLink<sad::animations::Animation>* link = new sad::TreeDbLink<sad::animations::Animation>();
    link->setParent(this);
    if (m_database.exists())
    {
        link->setDatabase(m_database.value());
    }
    if (m_tree.exists())
    {
        link->setTree(m_tree.value());
    }

    link->setMajorId(majorid);
    m_links << link;
    m_inner_valid = m_links.size() != 0;
    this->updateValidFlag();
}

void sad::animations::Composite::insert(unsigned long long majorid, int pos)
{
    PROFILER_EVENT;
    sad::TreeDbLink<sad::animations::Animation>* link = new sad::TreeDbLink<sad::animations::Animation>();
    if (m_database.exists())
    {
        link->setDatabase(m_database.value());
    }
    if (m_tree.exists())
    {
        link->setTree(m_tree.value());
    }
    link->setParent(this);
    link->setMajorId(majorid);
    m_links.insert(link, pos);
    m_inner_valid = m_links.size() != 0;
    this->updateValidFlag();
}

void sad::animations::Composite::insert(sad::animations::Animation* o, int pos)
{
    PROFILER_EVENT;
    sad::TreeDbLink<sad::animations::Animation>* link = new sad::TreeDbLink<sad::animations::Animation>();
    for (sad::Hash<sad::MRObject*, size_t>::iterator iter = m_parents.begin(); iter != m_parents.end(); ++iter)
    {
        if (o->isParent(iter.key()))
        {
             delete link;
             throw std::logic_error("Detected loop in sad::animations::Composite::insert");
        }
    }
    link->setParent(this);
    if (m_database.exists())
    {
        link->setDatabase(m_database.value());
    }
    if (m_tree.exists())
    {
        link->setTree(m_tree.value());
    }
    link->setObject(o);
    m_links.insert(link, pos);
    m_inner_valid = m_links.size() != 0;
    this->updateValidFlag();
}

void sad::animations::Composite::swap(int pos1, int pos2)
{
    PROFILER_EVENT;
    if (pos1 >=0 
        && pos2 >= 0 
        && pos1 < static_cast<int>(m_links.size()) 
        && pos2 < static_cast<int>(m_links.size()))
    {
        sad::TreeDbLink<sad::animations::Animation>* link = m_links[pos1];
        m_links[pos1] =  m_links[pos2];
        m_links[pos2] = link;
    }
}

void sad::animations::Composite::add(sad::animations::Animation* o)
{
    PROFILER_EVENT;
    sad::TreeDbLink<sad::animations::Animation>* link = new sad::TreeDbLink<sad::animations::Animation>();
    for (sad::Hash<sad::MRObject*, size_t>::iterator iter = m_parents.begin(); iter != m_parents.end(); ++iter)
    {
        if (o->isParent(iter.key()))
        {
             delete link;
             throw std::logic_error("Detected loop in sad::animations::Composite::insert");
        }
    }
    link->setParent(this);
    if (m_database.exists())
    {
        link->setDatabase(m_database.value());
    }
    if (m_tree.exists())
    {
        link->setTree(m_tree.value());
    }
	link->setObject(o);
    m_links << link;
    m_inner_valid = m_links.size() != 0;
    this->updateValidFlag();
}

void sad::animations::Composite::remove(size_t i)
{
    PROFILER_EVENT;
    if (i < m_links.size())
    {
        delete m_links[i];
        m_links.removeAt(i);
        m_inner_valid = m_links.size() != 0;
        this->updateValidFlag();
    }
}

sad::animations::Animation* sad::animations::Composite::animation(size_t i) const
{
    PROFILER_EVENT;
    return m_links[i]->object(true);
}


size_t  sad::animations::Composite::size() const
{
    PROFILER_EVENT;
    return m_links.size();
}

void sad::animations::Composite::clear()
{
    PROFILER_EVENT;
    for(size_t i = 0; i < m_links.size(); i++)
    {
        delete m_links[i];
    }
    m_links.clear();
    m_inner_valid = m_links.size() != 0;
    this->updateValidFlag();
}

void sad::animations::Composite::setAnimationsNames(const sad::Vector<sad::String>& names)
{
    PROFILER_EVENT;
    this->clear();
    for(size_t i = 0; i < names.size(); i++)
    {
        sad::TreeDbLink<sad::animations::Animation>* link = new sad::TreeDbLink<sad::animations::Animation>();
        if (m_database.exists())
        {
            link->setDatabase(m_database.value());
        }
        if (m_tree.exists())
        {
            link->setTree(m_tree.value());
        }
        link->setName(names[i]);
        m_links << link;
    }
    m_inner_valid = names.size() != 0;
    this->updateValidFlag();
}

sad::Vector<sad::String> sad::animations::Composite::animationNames() const
{
    PROFILER_EVENT;
    sad::Vector<sad::String> result;
    for(size_t i = 0; i < m_links.size(); i++)
    {
        result << m_links[i]->name();
    }
    return result;
}

void sad::animations::Composite::setAnimationsMajorId(const sad::Vector<unsigned long long>& ids)
{
    PROFILER_EVENT;
    this->clear();
    for(size_t i = 0; i < ids.size(); i++)
    {
        sad::TreeDbLink<sad::animations::Animation>* link = new sad::TreeDbLink<sad::animations::Animation>();
        if (m_database.exists())
        {
            link->setDatabase(m_database.value());
        }
        if (m_tree.exists())
        {
            link->setTree(m_tree.value());
        }
        link->setMajorId(ids[i]);
        m_links << link;
    }
    m_inner_valid = ids.size() != 0;
    this->updateValidFlag();
}

sad::Vector<unsigned long long> sad::animations::Composite::animationMajorIds() const
{
    PROFILER_EVENT;
    sad::Vector<unsigned long long> result;
    for(size_t i = 0; i < m_links.size(); i++)
    {
        result << m_links[i]->majorId();
    }
    return result;
}

sad::animations::setstate::AbstractSetStateCommand* sad::animations::Composite::stateCommand(sad::db::Object* o)
{
    PROFILER_EVENT;
    return nullptr;
}

// ====================================== PROTECTED METHODS ======================================

void sad::animations::Composite::copy(const sad::animations::Composite& a)
{
    PROFILER_EVENT;
    m_links.clear();
    for(size_t i = 0; i < a.m_links.size(); i++)
    {
        m_links << new sad::TreeDbLink<sad::animations::Animation>(*(a.m_links[i]));
        m_links[i]->setParent(this);
    }

    m_database = a.m_database;
    m_tree = a.m_tree;

    m_temp_creators.clear();
    
}
