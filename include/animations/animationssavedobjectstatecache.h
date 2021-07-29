/*! \file animations/animationssavedobjectstatecache.h
    

    Declares a cache of saved object states
 */
#pragma once
#include "animationssavedobjectstate.h"
#include "animationsanimation.h"

#include "../sadhash.h"

namespace sad
{
    
namespace animations
{
/*! Creates new cache of saved object states
 */
class SavedObjectStateCache
{
public:
    /*! Creates new empty state cache
     */
    SavedObjectStateCache();
    /*! Saved object states cannot be copied, so copied state cache must be empty
        \param[in] s other cache
     */
    SavedObjectStateCache(const sad::animations::SavedObjectStateCache& s);
    /*! Saved object states cannot be copied, so copied state cache must be empty
        \param[in] s other cache
        \return *this
     */
    sad::animations::SavedObjectStateCache& operator=(const sad::animations::SavedObjectStateCache& s);
    /*! Could be inherited
     */
    virtual ~SavedObjectStateCache();
    /*! Lookups, whether saved state is present for cache
        \param[in] o object
        \param[in] name name of state name
        \return whether saved state is present
     */
    bool lookup(sad::db::Object* o, const sad::String& name) const;
    /*! Increments reference count to saved state
        \param[in] o object
        \param[in] name name of state name
     */
    void increment(sad::db::Object* o, const sad::String& name) const;
    /*! Caches a saved state if needed
        \param[in] o object
        \param[in] name a name to be saved
        \param[in] state a saved state
     */ 
    void saveState(sad::db::Object* o, const sad::String& name, sad::animations::SavedObjectState* state);
    /*! Tries to restore a saved object state for object, from an animation
        \param[in] o object
        \param[in] name a name of key of state to be restored
     */
    void restore(sad::db::Object* o, const sad::String& name);
protected:
    /*! Destroys a cache 
     */
    void clear();
    /*! A cache of saved object states
     */
    sad::Hash<sad::db::Object*, sad::Hash<sad::String, sad::animations::SavedObjectState*> > m_cache;
};

}

}
