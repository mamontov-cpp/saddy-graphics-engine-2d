/*! \file animationsinstance.h
    \author HiddenSeeker

    Defines a basic class for all animations instances
 */
#pragma once

#include "../sadptrvector.h"
#include "../pausabletimer.h"

#include "../db/dbobject.h"
#include "../db/dblink.h"
#include "../db/dbvariant.h"


#include "../resource/link.h"

#include "animationsanimation.h"
#include "animationscallback.h"

namespace sad
{

namespace animations
{

class Instance: public sad::db::Object
{
public:
    /*! Creates new  empty instance
     */
    Instance();
    /*! Could be inherited
     */
    virtual ~Instance();
    /*! A basic schema for object
        \return a schema
     */
    static sad::db::schema::Schema* basicSchema();
    /*! Returns schema for an object
        \return schema
     */
    virtual sad::db::schema::Schema* schema() const;
    /*! Sets a tree name for object with specified renderer
        \param[in] r renderer, which tree should be fetched from
        \param[in] tree_name a name for an item for object
     */
    virtual void setTreeName(sad::Renderer* r, const sad::String & tree_name);
    /*! Sets a table
        \param[in] t a table
     */
    virtual void setTable(sad::db::Table* t);
    /*! Returns serializable name for an instance
        \return a serializable name
     */
    virtual const sad::String& serializableName() const;
    /*! Sets animation object for link
        \param[in] o animation object
     */
    void setAnimation(sad::animations::Animation* o);
    /*! Returns animation object
        \return animation object
     */
    sad::animations::Animation* animation() const;
    /*! Sets object
        \param[in] o object
     */
    void setObject(sad::db::Object* o);
    /*! Returns object
        \return object
     */
    sad::db::Object* object() const;
    /*! Sets animation name
        \param[in] name name for animation
     */
    void setAnimationName(const sad::String& name);
    /*! Returns animation name
        \return animation name
     */
    const sad::String& animationName() const;
    /*! Sets object id for animation instance
        \param[in] id an id for object
     */
    void setObjectId(unsigned long long id);
    /*! Returns object id
        \return object id
     */
    unsigned long long objectId() const;
    /*! Returns old state of object for instance
        \return old state of object
     */
    sad::Vector<sad::db::Variant>& oldState();
    /*! Returns old ststae of object for instance
        \return old state of object
     */
    const sad::Vector<sad::db::Variant>& oldState() const;
    /*! Adds new callback in animation instance, which should be called,
        when finished c callback
        \param[in] c
     */
    void addCallbackOnEnd(sad::animations::Callback* c);
    /*! Adds a callback, which should be called on end
        \param[in] f function
     */
    template<typename _Fun>
    void end(_Fun f)
    {
        addCallback(new sad::animations::FunctionCall<_Fun>(f));
    }
    /*! Adds a callback, which should be called on end
        \param[in] o object
        \param[in] m method
     */
    template<typename _Object, typename _Method>
    void end(_Object* o, _Method m)
    {
        addCallback(new sad::animations::MethodCall0<_Object, _Method>(o, m));
    }
    /*! Adds a callback, which should be called on end
        \param[in] o object
        \param[in] m method
        \param[in] a argument
     */
    template<typename _Object, typename _Method, typename _Arg>
    void end(_Object* o, _Method m, const _Arg& a)
    {
        addCallback(new sad::animations::MethodCall1<_Object, _Method, _Arg>(o, m, a));
    }
    /*! Adds a callback, which should be called on end
        \param[in] o object
        \param[in] m method
        \param[in] a1 first argument
        \param[in] a2 second argument
     */
    template<typename _Object, typename _Method, typename _Arg1, typename _Arg2>
    void end(_Object* o, _Method m, const _Arg1& a1,const _Arg2& a2)
    {
        addCallback(new sad::animations::MethodCall2<_Object, _Method, _Arg1, _Arg2>(o, m, a1, a2));
    }
    /*! Adds a callback, which should be called on end
        \param[in] o object
        \param[in] m method
        \param[in] a1 first argument
        \param[in] a2 second argument
        \param[in] a3 third argument
     */
    template<typename _Object, typename _Method, typename _Arg1, typename _Arg2, typename _Arg3>
    void end(_Object* o, _Method m, const _Arg1& a1,const _Arg2& a2, const _Arg3& a3)
    {
        addCallback(new sad::animations::MethodCall3<_Object, _Method, _Arg1, _Arg2, _Arg3>(o, m, a1, a2, a3));
    }
    /*! Clears finished flag
     */
    void clearFinished();
    /*! Whether animation is finished
     */
    bool finished() const;
    /*! Called on every step of instance work
     */
    virtual void process();
    /*! Pauses an instance
     */
    virtual void pause();
    /*! Resumes an instance
     */
    virtual void resume();
    /*! Cancels an animation
     */
    virtual void cancel();
protected:
    /*! A custom processing part, which could be reimplemented to synchronize
        something
     */
    virtual void _process();
    /*! A linked object
     */
    sad::db::Link m_object;
    /*! A linked animation for instance
     */
    sad::resource::Link<sad::animations::Animation> m_animation;
    /*! Whether instance is paused
     */
    bool m_paused;
    /*! Whether instance is started
     */
    bool m_started;
    /*! Whether instance is done working
     */
    bool m_finished;
    /*! A timer, which could be paused
     */
    sad::PausableTimer m_timer;
    /*! A callbacks for ending an instance
     */
    sad::PtrVector<sad::animations::Callback> m_callbacks_on_end;
    /*! A lists of variants for saving old dstate
     */
    sad::Vector<sad::db::Variant> m_oldstate;
};

}

}
