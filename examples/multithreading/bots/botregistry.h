/*! \file botregistry.h
    
    A bot registry for storing all AI algorithms and getting them by name
 */
#pragma once
#include "abstractbot.h"

#include <sadstring.h>
#include <sadhash.h>

namespace bots
{

/*! A bot registry stores all data for bots
 */
class BotRegistry
{
public:
    /*! By default bot registry is empty
     */
    BotRegistry() = default;
    /*! Copies other registry
        \param[in] o other object
     */
    BotRegistry(const bots::BotRegistry& o);
    /*! Copies state of other registry, destroying local state
        \param[in] o other object
        \return self-reference
     */
    bots::BotRegistry& operator=(const bots::BotRegistry& o);
    /*! Destroys all stored bots
     */
    ~BotRegistry();
    /*! Tries to insert bot into registry
        \param[in] key a name for bot
        \param[in] bot a bot
        \return true on success, false if bot is nullptr or bots with same key already exists
     */
    bool insert(const sad::String& key, bots::AbstractBot* bot);
    /*! Returns a bot by key
        \param[in] key a name for bot
        \return bot by name or null, if no such bot is registered
     */
    bots::AbstractBot* get(const sad::String& key) const;
    /*! Returns true if bot is in registry
        \param[in] key a name for bot
     */
    bool contains(const sad::String& key) const;
private:
    /*! Copies state from other object
        \param[in] o other object
     */
    void copy(const bots::BotRegistry& o);
    /*! Destructs local state for bot registry
     */
    void destroy();
    /*! A local bots storage
     */
    sad::Hash<sad::String, bots::AbstractBot*> m_bots;
};

}
