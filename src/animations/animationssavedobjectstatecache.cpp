#include "animations/animationssavedobjectstatecache.h"

// ========================== PUBLIC METHODS ==========================

sad::animations::SavedObjectStateCache::SavedObjectStateCache()
{
	
}

sad::animations::SavedObjectStateCache::SavedObjectStateCache(const sad::animations::SavedObjectStateCache& s)
{
	
}


sad::animations::SavedObjectStateCache& sad::animations::SavedObjectStateCache::operator=(const sad::animations::SavedObjectStateCache& s)
{
	clear();
	return *this;
}

sad::animations::SavedObjectStateCache::~SavedObjectStateCache()
{
	clear();
}

void sad::animations::SavedObjectStateCache::saveState(sad::db::Object* o, sad::animations::Animation* s)
{
	const sad::String& name = s->metaData()->name();
	if (m_cache.contains(o))
	{
		sad::Hash<sad::String, sad::animations::SavedObjectState*> &c = m_cache[o];
		if (c.contains(name))
		{
			c[name]->increment();
		}
		else
		{
			c.insert(name, s->saveState(o));
		}
	}
	else
	{
		sad::Hash<sad::String, sad::animations::SavedObjectState*> hash;
		hash.insert(name, s->saveState(o));
		m_cache.insert(o, hash);
	}
}

void sad::animations::SavedObjectStateCache::restore(sad::db::Object* o, sad::animations::Animation* s)
{
	const sad::String& name = s->metaData()->name();
	if (m_cache.contains(o))
	{
		sad::Hash<sad::String, sad::animations::SavedObjectState*> &c = m_cache[o];
		if (c.contains(name))
		{
			sad::animations::SavedObjectState* state = c[name];
			if (state->decrement())
			{
				delete state;
				c.remove(name);
			}
		}
	}
}

// ========================== PROTECTED METHODS ==========================

void sad::animations::SavedObjectStateCache::clear()
{
	for (sad::Hash<sad::db::Object*, sad::Hash<sad::String, sad::animations::SavedObjectState*> >::iterator ii = m_cache.begin();
		 ii != m_cache.end();
		 ++ii)
	{
		sad::Hash<sad::String, sad::animations::SavedObjectState*>& part = ii.value();
		for(sad::Hash<sad::String, sad::animations::SavedObjectState*>::iterator jj = part.begin();
			jj != part.end();
			++jj)
		{
			delete jj.value();
		}
	}

	m_cache.clear();
}