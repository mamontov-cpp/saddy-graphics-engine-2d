/*! \file animations/animationssavedobjectstatecache.h
	\author HiddenSeeker

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
	/*! Caches a saved state if needed 
		\param[in] o object
		\param[in] s an animation object
	 */ 
	void saveState(sad::db::Object* o, sad::animations::Animation* s);
	/*! Tries to restore a saved object state for object, from an animation
		\param[in] o object
		\param[in] s an animation object		
	 */
	void restore(sad::db::Object* o, sad::animations::Animation* s);
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