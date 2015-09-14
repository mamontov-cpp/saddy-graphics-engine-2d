/*! \file animationsanimations.h
	

	Defines an animations as pipeline step
 */
#pragma once

#include "../temporarilyimmutablecontainer.h"
#include "../sadvector.h"

#include "../pipeline/pipelineprocess.h"

#include "animationsprocess.h"
#include "animationssavedobjectstatecache.h"


namespace sad
{
	
namespace animations
{

/*! An animations, as a pipeline rendering step
 */
class Animations: public sad::pipeline::AbstractProcess, 
public sad::TemporarilyImmutableContainer<sad::animations::Process>
{
public:
	/*! Creates new empty animations list
	 */
	Animations();
	/*! Removes links to processes
	 */
	virtual ~Animations();
	/*! A cache, for saving object state
		\return cache for saving state
	 */
	sad::animations::SavedObjectStateCache& cache();
	/*! Queries processes, relates to specified object
		\param[in] o specified object
		\return list of related processes
	 */
	sad::Vector<sad::animations::Process*> queryProcessesRelatedToObject(sad::db::Object* o);
	/*! Stops processes, related to object
		\param[in] o object
	 */
	void stopProcessesRelatedToObject(sad::db::Object* o);
protected:
	/*! Runs every process
	 */ 
	virtual void _process();
	/*! Adds an animation process to list
		\param[in] o object
	 */
	void addNow(sad::animations::Process* o);
	/*! Removes an animation process to list
	 */
	void removeNow(sad::animations::Process* o);
	/*! Clears an animation list
	 */
	void clearNow();
	/*! A list of animations processes
	 */
	sad::Vector<sad::animations::Process*> m_list;
	/*! A cache, for saving an animations
	 */
	sad::animations::SavedObjectStateCache m_cache;
};

}

}
