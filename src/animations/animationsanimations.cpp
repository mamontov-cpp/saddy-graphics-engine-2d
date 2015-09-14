#include "animations/animationsanimations.h"

// ========================= PUBLIC METHODS =========================

sad::animations::Animations::Animations()
{
	
}

sad::animations::Animations::~Animations()
{
	this->clearNow();
}

sad::animations::SavedObjectStateCache& sad::animations::Animations::cache()
{
	return m_cache;
}

sad::Vector<sad::animations::Process*> sad::animations::Animations::queryProcessesRelatedToObject(sad::db::Object* o)
{
	sad::Vector<sad::animations::Process*> result;
	for(size_t i = 0; i < m_command_queue.size(); i++)
	{
		if (m_command_queue[i].Type == CT_ADD)
		{
			if (m_command_queue[i].Added->isRelatedToObject(o))
			{
				result << m_command_queue[i].Added;
			}
		}
	}
	for(size_t i = 0; i < m_list.size(); i++)
	{
		if (m_list[i]->isRelatedToObject(o))
		{
			result << m_list[i];
		}
	}
	return result;
}


void sad::animations::Animations::stopProcessesRelatedToObject(sad::db::Object* o)
{
	sad::Vector<sad::animations::Process*> result;
	for(size_t i = 0; i < m_command_queue.size(); i++)
	{
		if (m_command_queue[i].Type == CT_ADD)
		{
			if (m_command_queue[i].Added->isRelatedToObject(o))
			{
				m_command_queue[i].Added->stopInstanceRelatedToObject(o, this);
			}
		}
	}
	for(size_t i = 0; i < m_list.size(); i++)
	{
		if (m_list[i]->isRelatedToObject(o))
		{
			m_list[i]->stopInstanceRelatedToObject(o, this);
		}
	}
}

// ========================= PROTECTED METHODS =========================

void sad::animations::Animations::_process()
{
	performQueuedActions();
	lockChanges();

	for(size_t i = 0; i < m_list.size(); i++)
	{
		sad::animations::Process * p = m_list[i];
        p->process(this);
		if (p->finished())
		{
			p->removedFromPipeline();
			m_list.removeAt(i);
			--i;
		}
	}

	unlockChanges();
	performQueuedActions();
}

void sad::animations::Animations::addNow(sad::animations::Process* o)
{
	if (o)
	{
		o->addedToPipeline();
		m_list << o;
	}
}

void sad::animations::Animations::removeNow(sad::animations::Process* o)
{
	if (o)
	{
        sad::Vector<sad::animations::Process*>::iterator it = std::find(
            m_list.begin(), 
            m_list.end(),
            o
        );
        if (it != m_list.end())
        {
            o->cancel(this);
		    o->removedFromPipeline();
		    m_list.erase(it);
        }
	}
}

void sad::animations::Animations::clearNow()
{
	for(size_t i = 0; i < m_list.size(); i++)
	{
        m_list[i]->cancel(this);		    
		m_list[i]->removedFromPipeline();
	}
	m_list.clear();
}
