/*! \file animations/animationsgroup.h
    

    Defines an animations group as a list of running animation instances
 */
#pragma once
#include <functional>
#include "animationsinstance.h"

#include "../db/dbstronglink.h"

#include "../sadvector.h"
#include "../sadmutex.h"

namespace sad
{
    
namespace animations
{

/*! An animation group as a list of running animation instances
 */
class Group: public sad::animations::Process
{
SAD_OBJECT
public:
    /*! A link instance
     */
    typedef sad::db::StrongLink<sad::animations::Instance> InstanceLink;
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
    /*! Returns whether instances in group should be played sequentially
        \return flag value
    */
    bool isSequential() const;
    /*! Sets an attribute, which makes group play instances sequentially to true or false
        \param[in] flag a flag
    */
    void toggleIsSequential(bool flag);
    /*! Clears a group
     */
    void clear();
    /*! Adds new instance to group
        \param[in] i instance
     */
    void add(sad::animations::Instance* i);
    /*! Inserts an instance to position
        \param[in] pos position
        \param[in] i instance
     */
    void insert(int pos, sad::animations::Instance* i);
    /*! Removes an instance from group
        \param[in] i instance
     */
    void remove(sad::animations::Instance* i);
    /*! Amount of instances in group
     */
    size_t count();
    /*! Returns instance with position
        \param[in] pos a position
        \return an instance
     */
    sad::animations::Instance* get(int pos);
    /*! Returns instance with position
        \param[in] pos a position
        \return an instance's major id
    */
    unsigned long long getMajorId(int pos);
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
        \return callback
     */
    sad::animations::Callback* addCallbackOnEnd(sad::animations::Callback* c);
    /*! Adds a callback, which should be called, when group is done playing
        \param[in] f function
     */
    template<typename _Fun>
    sad::animations::Callback*  end(_Fun f)
    {
        return addCallbackOnEnd(new sad::animations::FunctionCall<_Fun>(f));
    }
    /*! Adds a callback, which should be called, when group is done playing
        \param[in] o object
        \param[in] m method
     */
    template<typename _Object, typename _Method>
    sad::animations::Callback*  end(_Object* o, _Method m)
    {
        return addCallbackOnEnd(new sad::animations::MethodCall0<_Object, _Method>(o, m));
    }
    /*! Adds a callback, which should be called, when group is done playing
        \param[in] o object
        \param[in] m method
        \param[in] a argument
     */
    template<typename _Object, typename _Method, typename _Arg>
    sad::animations::Callback*  end(_Object* o, _Method m, const _Arg& a)
    {
        return addCallbackOnEnd(new sad::animations::MethodCall1<_Object, _Method, _Arg>(o, m, a));
    }
    /*! Adds a callback, which should be called, when group is done playing
        \param[in] o object
        \param[in] m method
        \param[in] a1 first argument
        \param[in] a2 second argument
     */
    template<typename _Object, typename _Method, typename _Arg1, typename _Arg2>
    sad::animations::Callback*  end(_Object* o, _Method m, const _Arg1& a1,const _Arg2& a2)
    {
        return addCallbackOnEnd(new sad::animations::MethodCall2<_Object, _Method, _Arg1, _Arg2>(o, m, a1, a2));
    }
    /*! Adds a callback, which should be called, when group is done playing
        \param[in] o object
        \param[in] m method
        \param[in] a1 first argument
        \param[in] a2 second argument
        \param[in] a3 third argument
     */
    template<typename _Object, typename _Method, typename _Arg1, typename _Arg2, typename _Arg3>
    sad::animations::Callback*  end(_Object* o, _Method m, const _Arg1& a1,const _Arg2& a2, const _Arg3& a3)
    {
        return addCallbackOnEnd(new sad::animations::MethodCall3<_Object, _Method, _Arg1, _Arg2, _Arg3>(o, m, a1, a2, a3));
    }
    /*! Adds new callback in animation group, which should be called,
        when group is started being played
        \param[in] c a new callback
        \return callback
     */
    sad::animations::Callback* addCallbackOnStart(sad::animations::Callback* c);
    /*! Adds a callback, which should be called, when group is started being played
        \param[in] f function
     */
    template<typename _Fun>
    sad::animations::Callback*  start(_Fun f)
    {
        return addCallbackOnStart(new sad::animations::FunctionCall<_Fun>(f));
    }
    /*! Adds a callback, which should be called, when group is started being played
        \param[in] o object
        \param[in] m method
     */
    template<typename _Object, typename _Method>
    sad::animations::Callback*  start(_Object* o, _Method m)
    {
        return addCallbackOnStart(new sad::animations::MethodCall0<_Object, _Method>(o, m));
    }
    /*! Adds a callback, which should be called, when group is started being played
        \param[in] o object
        \param[in] m method
        \param[in] a argument
     */
    template<typename _Object, typename _Method, typename _Arg>
    sad::animations::Callback*  start(_Object* o, _Method m, const _Arg& a)
    {
        return  addCallbackOnStart(new sad::animations::MethodCall1<_Object, _Method, _Arg>(o, m, a));
    }
    /*! Adds a callback, which should be called, when group is started being played
        \param[in] o object
        \param[in] m method
        \param[in] a1 first argument
        \param[in] a2 second argument
     */
    template<typename _Object, typename _Method, typename _Arg1, typename _Arg2>
    sad::animations::Callback*  start(_Object* o, _Method m, const _Arg1& a1,const _Arg2& a2)
    {
        return addCallbackOnStart(new sad::animations::MethodCall2<_Object, _Method, _Arg1, _Arg2>(o, m, a1, a2));
    }
    /*! Adds a callback, which should be called, when group is started being played
        \param[in] o object
        \param[in] m method
        \param[in] a1 first argument
        \param[in] a2 second argument
        \param[in] a3 third argument
     */
    template<typename _Object, typename _Method, typename _Arg1, typename _Arg2, typename _Arg3>
    sad::animations::Callback*  start(_Object* o, _Method m, const _Arg1& a1,const _Arg2& a2, const _Arg3& a3)
    {
        return addCallbackOnStart(new sad::animations::MethodCall3<_Object, _Method, _Arg1, _Arg2, _Arg3>(o, m, a1, a2, a3));
    }

    /*! Removes callback on start
        \param[in] c callback
    */
    void removeCallbackOnStart(sad::animations::Callback* c);
    /*! Removes callback on end
        \param[in] c callback
    */
    void removeCallbackOnEnd(sad::animations::Callback* c);
    /*! Removes callback from all vectors
        \param[in] c callback
    */
    void removeCallback(sad::animations::Callback* c);
    /*! Clears callbacks on start
     */
    void clearCallbacksOnStart();
    /*! Clears callbacks on end
     */
    void clearCallbacksOnEnd();
    /*! Clear all callbacks
     */
    void clearCallbacks();

    /*! Returns true of if process is related to object, matched by function
        \param[in] f function for testing
        \return true if related
     */
    virtual bool isRelatedToMatchedObject(const std::function<bool(sad::db::Object*)>& f);
    /*! If current instance is related to matched objects, stops related part
        \param[in] f function for testing
        \param[in] a animations list
     */
    virtual void stopInstancesRelatedToMatchedObject(const std::function<bool(sad::db::Object*)>& f, sad::animations::Animations* a);
    /*! Returns true of if process is related to animation, matched by function
        \param[in] f function for testing
        \return true if related
     */
    virtual bool isRelatedToMatchedAnimation(const std::function<bool(sad::animations::Animation*)>& f);
    /*! If current instance is related to matched objects, stops related part
        \param[in] f function for testing
        \param[in] a animations list
     */
    virtual void stopInstancesRelatedToMatchedAnimation(const std::function<bool(sad::animations::Animation*)>& f, sad::animations::Animations* a);
    /*! Returns true of if process is related to instance, matched by function
        \param[in] f function for testing
        \return true if related
     */
    virtual bool isRelatedToMatchedProcess(const std::function<bool(sad::animations::Process*)>& f);
    /*! If current instance is related to matched objects, stops related part
        \param[in] f function for testing
        \param[in] a animations list
     */
    virtual void stopInstancesRelatedToMatchedProcess(const std::function<bool(sad::animations::Process*)>& f, sad::animations::Animations* a);
protected:
    /*! Clears links in group
     */
    void clearLinks();
    /*! Fetches instances for animations group
        \param[out] result a result data
     */
    void getInstances(sad::Vector<sad::animations::Instance*> & result);
    /*! Copies state from other group
        \param[in] o group
     */
    void copyState(const sad::animations::Group& o);
    /*! Fires callbacks on start for animation groups
     */
    void fireOnStartCallbacks();
    /*! A links for instances
     */
    sad::Vector<InstanceLink*> m_instance_links;
    /*! An instances to be processed
     */
    sad::Vector<sad::animations::Instance*> m_instances; 
    /*! A finished instances for sequential animations
     */
    sad::Vector<sad::animations::Instance*> m_finished_instances;
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
    /*! Whether animations should be played sequentially
    */
    bool m_sequential;
    /*! A current instance
     */
    size_t m_current_instance;
    /*! A lock
     */
    sad::Mutex m_lock;
};

}

}


DECLARE_TYPE_AS_SAD_OBJECT_ENUM(sad::animations::Group)
