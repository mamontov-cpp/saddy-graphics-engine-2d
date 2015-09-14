/*! \file animations/animationsgroup.h
	

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
    /*! Copies instance
        \param[in] o instance
        \return reference to self
     */
    sad::animations::Group& operator=(const sad::animations::Group& o);
	/*! Another group
	 */
	~Group();
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
	/*! Tries to find instances in group
		\param[in] id id of instance
		\return position of instance (-1 if not found)
	 */
	int findInstance(unsigned long long id);
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
	/*! Inserts instance as link
		\param[in] pos position of instance in group
		\param[in] i instance
	 */
	void insertAsLink(int pos, sad::animations::Instance* i);
	/*! Restarts an animation group
        \param[in] animations an animations process
	 */
    virtual void restart(sad::animations::Animations* animations);
	/*! Restarts every animation, which was finished
     */
    virtual void clearFinished();
    /*! Whether group is finished
     */
    virtual bool finished() const;
    /*! Called on every step of group work
        \param[in] animations an animation pipeline, which should hold a state cache
     */
    virtual void process(sad::animations::Animations* animations);
    /*! Pauses a group
     */
    virtual void pause();
    /*! Resumes a group
     */
    virtual void resume();
    /*! Cancels an animation group
		\param[in] animations an animations pipeline
     */
    virtual void cancel(sad::animations::Animations* animations);
	/*! Called, when process is added to pipeline
	 */
	virtual void addedToPipeline();
	/*! Called, when process is removed from pipeline
	 */
	virtual void removedFromPipeline();
    /*! Adds new callback in animation group, which should be called,
        when group is done playing
        \param[in] c
     */
    void addCallbackOnEnd(sad::animations::Callback* c);
    /*! Adds a callback, which should be called, when group is done playing
        \param[in] f function
     */
    template<typename _Fun>
    void end(_Fun f)
    {
        addCallbackOnEnd(new sad::animations::FunctionCall<_Fun>(f));
    }
    /*! Adds a callback, which should be called, when group is done playing
        \param[in] o object
        \param[in] m method
     */
    template<typename _Object, typename _Method>
    void end(_Object* o, _Method m)
    {
        addCallbackOnEnd(new sad::animations::MethodCall0<_Object, _Method>(o, m));
    }
    /*! Adds a callback, which should be called, when group is done playing
        \param[in] o object
        \param[in] m method
        \param[in] a argument
     */
    template<typename _Object, typename _Method, typename _Arg>
    void end(_Object* o, _Method m, const _Arg& a)
    {
        addCallbackOnEnd(new sad::animations::MethodCall1<_Object, _Method, _Arg>(o, m, a));
    }
    /*! Adds a callback, which should be called, when group is done playing
        \param[in] o object
        \param[in] m method
        \param[in] a1 first argument
        \param[in] a2 second argument
     */
    template<typename _Object, typename _Method, typename _Arg1, typename _Arg2>
    void end(_Object* o, _Method m, const _Arg1& a1,const _Arg2& a2)
    {
        addCallbackOnEnd(new sad::animations::MethodCall2<_Object, _Method, _Arg1, _Arg2>(o, m, a1, a2));
    }
    /*! Adds a callback, which should be called, when group is done playing
        \param[in] o object
        \param[in] m method
        \param[in] a1 first argument
        \param[in] a2 second argument
        \param[in] a3 third argument
     */
    template<typename _Object, typename _Method, typename _Arg1, typename _Arg2, typename _Arg3>
    void end(_Object* o, _Method m, const _Arg1& a1,const _Arg2& a2, const _Arg3& a3)
    {
        addCallbackOnEnd(new sad::animations::MethodCall3<_Object, _Method, _Arg1, _Arg2, _Arg3>(o, m, a1, a2, a3));
    }
    /*! Adds new callback in animation group, which should be called,
        when group is started being played
        \param[in] c a new callback
     */
    void addCallbackOnStart(sad::animations::Callback* c);
    /*! Adds a callback, which should be called, when group is started being played
        \param[in] f function
     */
    template<typename _Fun>
    void start(_Fun f)
    {
        addCallbackOnStart(new sad::animations::FunctionCall<_Fun>(f));
    }
    /*! Adds a callback, which should be called, when group is started being played
        \param[in] o object
        \param[in] m method
     */
    template<typename _Object, typename _Method>
    void start(_Object* o, _Method m)
    {
        addCallbackOnStart(new sad::animations::MethodCall0<_Object, _Method>(o, m));
    }
    /*! Adds a callback, which should be called, when group is started being played
        \param[in] o object
        \param[in] m method
        \param[in] a argument
     */
    template<typename _Object, typename _Method, typename _Arg>
    void start(_Object* o, _Method m, const _Arg& a)
    {
        addCallbackOnStart(new sad::animations::MethodCall1<_Object, _Method, _Arg>(o, m, a));
    }
    /*! Adds a callback, which should be called, when group is started being played
        \param[in] o object
        \param[in] m method
        \param[in] a1 first argument
        \param[in] a2 second argument
     */
    template<typename _Object, typename _Method, typename _Arg1, typename _Arg2>
    void start(_Object* o, _Method m, const _Arg1& a1,const _Arg2& a2)
    {
        addCallbackOnStart(new sad::animations::MethodCall2<_Object, _Method, _Arg1, _Arg2>(o, m, a1, a2));
    }
    /*! Adds a callback, which should be called, when group is started being played
        \param[in] o object
        \param[in] m method
        \param[in] a1 first argument
        \param[in] a2 second argument
        \param[in] a3 third argument
     */
    template<typename _Object, typename _Method, typename _Arg1, typename _Arg2, typename _Arg3>
    void start(_Object* o, _Method m, const _Arg1& a1,const _Arg2& a2, const _Arg3& a3)
    {
        addCallbackOnStart(new sad::animations::MethodCall3<_Object, _Method, _Arg1, _Arg2, _Arg3>(o, m, a1, a2, a3));
    }
	/*! If current group played instances is related to specified object, stops related part.
		Not all instances are being scanned, only played one.
		\param[in] object a related object
		\param[in] a animations list
	 */
	virtual void stopInstanceRelatedToObject(sad::db::Object* object, sad::animations::Animations* a);
	/*! Returns true of if object is related to this group's played instances.
		Not all instances are being scanned, only played one.
		\param[in] object a tested object
		\return true if related
	 */
	virtual bool isRelatedToObject(sad::db::Object* object);
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
     /*! Clear references
      */
     void clearReferences();
     /*! Copies state from other group
         \param[in] o group
      */
     void copyState(const sad::animations::Group& o);
     /*! Fires callbacks on start for animation groups
      */
     void fireOnStartCallbacks();
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
     /*! A cached parent animations to made cancelling possible
      */
     sad::animations::Animations* m_parent;
     /*! A callbacks, that should be called, when group starts being played
      */
    sad::PtrVector<sad::animations::Callback> m_callbacks_on_start;
     /*! A callbacks, that should be called, when group stops being played
      */
    sad::PtrVector<sad::animations::Callback> m_callbacks_on_end;
};

}

}
