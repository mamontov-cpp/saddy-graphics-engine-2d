#include "animations/animationsgroup.h"

#include <util/fs.h>

#include <resource/physicalfile.h>

#include <3rdparty/picojson/valuetotype.h>

#include <fuzzyequal.h>

#include <math.h>
#include <fstream>

#include "db/schema/schema.h"
#include "db/dbproperty.h"
#include "db/save.h"
#include "db/load.h"
#include "db/dbfield.h"
#include "db/dbmethodpair.h"
#include "db/dbtable.h"

// =========================== PUBLIC METHODS ===========================

sad::animations::Group::Group() : m_started(true), m_looped(false), m_parent(NULL)
{
	
}

sad::animations::Group::Group(const sad::animations::Group& g)
{
	m_instance_links = g.m_instance_links;
	m_instances.clear();
	m_referenced = g.m_referenced;
	m_looped = g.m_looped;
	for(size_t i = 0; i < m_referenced.size(); i++)
	{
		m_referenced[i]->addRef();
	}
}

sad::animations::Group::~Group()
{
	for(size_t i = 0; i < m_referenced.size(); i++)
	{
		m_referenced[i]->delRef();
	}
}

sad::animations::Group& sad::animations::Group::operator=(const sad::animations::Group& g)
{
	// Delete old values
	for(size_t i = 0; i < m_referenced.size(); i++)
	{
		m_referenced[i]->delRef();
	}
	// Copy values
	m_instance_links = g.m_instance_links;
	m_instances.clear();
	m_referenced = g.m_referenced;
	m_looped = g.m_looped;
	for(size_t i = 0; i < m_referenced.size(); i++)
	{
		m_referenced[i]->addRef();
	}
	return *this;
}

static sad::db::schema::Schema* AnimationGroupSchema = NULL;

sad::db::schema::Schema* sad::animations::Group::basicSchema()
{
    if (AnimationGroupSchema == NULL)
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
        
        sad::ClassMetaDataContainer::ref()->pushGlobalSchema(AnimationGroupSchema);
    }
    return AnimationGroupSchema;
}


sad::db::schema::Schema* sad::animations::Group::schema() const
{
	return sad::animations::Group::basicSchema();
}

void sad::animations::Group::setTable(sad::db::Table* t)
{
	for(size_t i = 0; i < m_instance_links.size(); i++)
	{
		m_instance_links[i].setTable(t);
	}
}

static sad::String AnimationsGroupSerializableName = "sad::animations::Group";

const sad::String& sad::animations::Group::serializableName() const
{
	return AnimationsGroupSerializableName;
}

void sad::animations::Group::setInstances(const sad::Vector<unsigned long long>& v)
{
	m_instance_links.clear();

	for(size_t i = 0; i < m_instance_links.size(); i++)
	{
		sad::db::Link l;
		if (this->table())
		{
			l.setDatabase(this->table()->database());
		}
		l.setTableName("animationinstances");
		l.setMajorId(v[i]);
		m_instance_links << l;
	}
}

sad::Vector<unsigned long long> sad::animations::Group::instances() const
{
	sad::Vector<unsigned long long> result;

	for(size_t i = 0; i < m_instance_links.size(); i++)
	{
		result << m_instance_links[i].majorId();
	}

	return result;
}

int sad::animations::Group::findInstance(unsigned long long id)
{
	for(size_t i = 0; i < m_instance_links.size(); i++)
	{
		if (m_instance_links[i].majorId() == id)
		{
			return i;
		}
	}
	return -1;
}

void sad::animations::Group::setLooped(bool looped)
{
	m_looped = looped;	
}

bool sad::animations::Group::looped() const
{
	return m_looped;
}

void sad::animations::Group::addAsLink(sad::animations::Instance* i)
{
	if (i)
	{
		sad::db::Link l;
		l.setDatabase(this->table()->database());
		l.setTableName("animationinstances");
		l.setMajorId(i->MajorId);
		m_instance_links << l;

		if (i && m_instances.count()) 
		{
			m_instances << i;
		}
	}
}

void sad::animations::Group::removeAsLink(sad::animations::Instance* inst)
{
	if (!inst)
	{
		return;
	}
	for(size_t i = 0; i < m_instance_links.size(); ++i)
	{
		if (m_instance_links[i].majorId() == inst->MajorId)
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
	if (i)
	{
		sad::db::Link l;
		l.setDatabase(this->table()->database());
		l.setTableName("animationinstances");
		l.setMajorId(i->MajorId);
		m_instance_links.insert(l, pos);

		if (i && m_instances.count()) 
		{
			m_instances.insert(i, pos);
		}
	}
}

void sad::animations::Group::restart(sad::animations::Animations* animations)
{
	for(size_t i = 0; i < m_instances.size(); i++)
	{
        m_instances[i]->cancel(animations);
	}

	m_instances.clear();
	m_started = true;
}

void sad::animations::Group::clearFinished()
{
	getInstances(m_instances);
}

bool sad::animations::Group::finished() const
{
	return m_instances.size() == 0;
}

void sad::animations::Group::process(sad::animations::Animations* animations)
{
	if (finished())
	{
		if (m_started || m_looped)
		{
			clearFinished();
			m_started = false;
		}
	}

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

void sad::animations::Group::pause()
{
	for(size_t i = 0; i < m_instances.size(); i++)
	{
		m_instances[i]->pause();
	}
}

void sad::animations::Group::resume()
{
	for(size_t i = 0; i < m_instances.size(); i++)
	{
		m_instances[i]->resume();
	}
}

void sad::animations::Group::cancel(sad::animations::Animations* animations)
{
	for(size_t i = 0; i < m_instances.size(); i++)
	{
		m_instances[i]->cancel(animations);
	}

	m_instances.clear();
}

void sad::animations::Group::addedToPipeline()
{
	this->addRef();
}

void sad::animations::Group::removedFromPipeline()
{
	this->delRef();
}

// =========================== PROTECTED METHODS ===========================

void sad::animations::Group::getInstances(sad::Vector<sad::animations::Instance*> & result)
{
	result.clear();

	for(size_t i = 0; i < m_instance_links.size(); i++)
	{
		sad::db::Object* o  = m_instance_links[i].get();
		if (o)
		{
			if (o->serializableName() == "sad::animations::Instance" || o->serializableName() == "sad::animations::WayInstance")
			{
				m_instances << static_cast<sad::animations::Instance*>(o);
			}
		}
	}

	for(size_t i = 0; i < m_referenced.size(); i++)
	{
		m_instances << m_referenced[i];
	}
}


void sad::animations::Group::addNow(sad::animations::Instance* o)
{
	if (o)
	{
		m_referenced << o;
		o->addRef();

		if (m_instances.size() != 0)
		{
			m_instances << o;
		}
	}
}

void sad::animations::Group::removeNow(sad::animations::Instance* o)
{
	for(size_t i = 0; i < m_referenced.size(); i++)
	{
		if (m_referenced[i] == o)
		{
			m_referenced.removeAt(i);
			if (o) 
			{
				o->delRef();
			}
			--i;
		}
	}

	if (m_instances.size() != 0)
	{
		for(size_t i = 0; i < m_instances.size(); i++)
		{
			if (m_instances[i] == o)
			{
                m_instances[i]->cancel(m_parent);
				m_instances.removeAt(i);
				--i;
			}
		}
	}

	for(size_t i = 0; i < m_instance_links.size(); i++)
	{
		if (m_instance_links[i].get() == o)
		{
			m_instance_links.removeAt(i);
			--i;
		}
	}
}

void sad::animations::Group::clearNow()
{
	for(size_t i = 0; i < m_instances.size(); i++)
	{
        m_instances[i]->cancel(m_parent);
	}
	m_instances.clear();
	for(size_t i = 0; i < m_referenced.size(); i++)
	{
		m_referenced[i]->delRef();
	}
	m_referenced.clear();
	m_instance_links.clear();
}
