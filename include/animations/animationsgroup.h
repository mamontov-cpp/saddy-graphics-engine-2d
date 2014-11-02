/*! \file animations/animationsgroup.h
	\author HiddenSeeker

	Defines an animations group as a list of running animation instances
 */
#pragma once
#include "animationsinstance.h"

#include "../db/dblink.h"

#include "../temporarilyimmutablecontainer.h"
#include "../sadvector.h"

namespace sad
{
	
namespace animations
{

/*! An animation group as a list of running animation instances
 */
class Group: public sad::db::Object, 
public sad::animations::Process,
public sad::TemporarilyImmutableContainer<sad::animations::Instance>
{
public:
	/*! Constructs new empty group
	 */
	Group();
	/*! Copies other group, preserving references
		\param[in] g groups
	 */
	Group(const sad::animations::Group& g);
	/*! Another group
	 */
	~Group();
	/*! Copies other group, preserving references
		\param[in] g groups
		\return *this
	 */
	sad::animations::Group& operator=(const sad::animations::Group& g);
	/*! A basic schema for object
        \return a schema
     */
    static sad::db::schema::Schema* basicSchema();
    /*! Returns schema for an object
        \return schema
     */
    virtual sad::db::schema::Schema* schema() const;
    /*! Sets a table
        \param[in] t a table
     */
    virtual void setTable(sad::db::Table* t);
    /*! Returns serializable name for an instance
        \return a serializable name
     */
    virtual const sad::String& serializableName() const;
	/*! Sets instances links
		\param[in] v vector
	 */
	void setInstances(const sad::Vector<unsigned long long>& v);
	/*! Returns instances
		\return instances list
	 */
	sad::Vector<unsigned long long> instances() const;
	/*! Whether group is looped
		\param[in] looped is animation looped
	 */
	void setLooped(bool looped);
	/*! Returns whether group is looped
		\return whether group is looped
	 */
	bool looped() const;
	/*! Adds object as a link to object
		\param[in] i instance
	 */
	void addAsLink(sad::animations::Instance* i);
	/*! Removes object as link to object
		\param[in] inst instance
	 */
	void removeAsLink(sad::animations::Instance* inst);
	/*! Restarts every animation, which was finished
     */
    virtual void clearFinished();
    /*! Whether group is finished
     */
    virtual bool finished() const;
    /*! Called on every step of group work
     */
    virtual void process();
    /*! Pauses a group
     */
    virtual void pause();
    /*! Resumes a group
     */
    virtual void resume();
    /*! Cancels an animation group
     */
    virtual void cancel();
	/*! Called, when process is added to pipeline
	 */
	virtual void addedToPipeline();
	/*! Called, when process is removed from pipeline
	 */
	virtual void removedFromPipeline();
protected:
	 /*! Immediately adds an object to container
		  \param[in] o object
	  */
	 virtual void addNow(sad::animations::Instance* o);
	 /*! Immediately removed an object from container
		  \param[in] o object
	  */
	 virtual void removeNow(sad::animations::Instance* o);
	 /*! Immediately clears a container
	  */
	 virtual void clearNow();
	 /*! Fetches instances for animations group
		\param[out] result a result data
	  */
	 void getInstances(sad::Vector<sad::animations::Instance*> & result);
	 /*! A links for instances
	  */
	 sad::Vector<sad::db::Link> m_instance_links;
	 /*! An instances to be processed
	  */
	 sad::Vector<sad::animations::Instance*> m_instances; 
	 /*! A referenced links
	  */
	 sad::Vector<sad::animations::Instance*> m_referenced;
	 /*! Whether animation is looped
	  */
	 bool m_looped;
	 /*! Whether animation is started
	  */
	 bool m_started;
};

}

}