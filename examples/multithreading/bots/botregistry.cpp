#include "botregistry.h"

// ========================================== PUBLIC METHODS ==========================================

bots::BotRegistry::BotRegistry(const bots::BotRegistry& o)
{
    this->copy(o);
}

bots::BotRegistry& bots::BotRegistry::operator=(const bots::BotRegistry& o)
{
    this->destroy();
    this->copy(o);
    return *this;
}

bots::BotRegistry::~BotRegistry()
{
    this->destroy();
}

bool bots::BotRegistry::insert(const sad::String& key, bots::AbstractBot* bot)
{
    if (!bot)
    {
        return false;
    }
    if (m_bots.contains(key))
    {
        return false;
    }
    m_bots.insert(key, bot);
    return true;
}

bots::AbstractBot* bots::BotRegistry::get(const sad::String& key) const
{
    if (m_bots.contains(key))
    {
        return m_bots[key];
    }
    return nullptr;
}

bool bots::BotRegistry::contains(const sad::String& key) const
{
    return m_bots.contains(key);
}


// ========================================== PRIVATE METHODS ==========================================

void bots::BotRegistry::copy(const bots::BotRegistry& o)
{
    for (auto it = o.m_bots.const_begin(); it != o.m_bots.const_end(); ++it)
    {
        m_bots.insert(it.key(), it.value()->clone());
    }
}

void bots::BotRegistry::destroy()
{
    for (auto it = m_bots.begin(); it != m_bots.end(); ++it)
    {
        delete it.value();
    }
    m_bots.clear();
}