/*! \file collisionhandler.h
    

    Describes a collision handler, used to deal with collision actions
 */
#pragma once
#include "collisionevent.h"
#include "body.h"

#include <functional>
#include <utility>

#include "../refcountable.h"

namespace sad
{

namespace p2d
{
/*! A basic collision handler for invoking events
 */
class BasicCollisionHandler: public sad::RefCountable
{
public:
    /*! Calls a function for basic collision event
        \param[in] ev event
     */ 
    virtual void invoke(const sad::p2d::BasicCollisionEvent & ev) = 0;
    /*! Could be inherited
     */
    virtual ~BasicCollisionHandler();
};


/*! A specific collision handler for invoking events, for working with just bodies
 */
class UntypedCollisionHandler : public sad::p2d::BasicCollisionHandler
{
public:
      /*! Constructs new handler
       * \param[in] f inner functional handler
       */
      inline UntypedCollisionHandler(std::function<void(const sad::p2d::BasicCollisionEvent&)> f ) : m_f(std::move(f))
      {
      }
      /*! Calls an inner handler for basic collision event
          \param[in] ev event
       */
      virtual void invoke(const sad::p2d::BasicCollisionEvent & ev) override;
private:
    /*! An inner untyped handler
        \brief[in] m_f a handler
     */
    std::function<void(const sad::p2d::BasicCollisionEvent &)> m_f;
};


/*! A typed collision handler to call with specified arguments
 */
template<
    typename _Object1,
    typename _Object2
>
class TypedCollisionHandler: public sad::p2d::BasicCollisionHandler
{
public:
    /*! Constructs new collision handler
        \param[in] f function
        \param[in] checked whether we should type check a handler
     */
    inline TypedCollisionHandler(const std::function<void(const sad::p2d::CollisionEvent<_Object1, _Object2>&)>& f, bool checked = true) : m_checked(checked), m_f(f)
    {

    }

    /*! Calls an inner handler for basic collision event
        \param[in] ev event
     */
    virtual void invoke(const sad::p2d::BasicCollisionEvent & ev) override
    {
        if (ev.m_object_1 != nullptr && ev.m_object_2 != nullptr)
        {
              bool ok = true;
              if (m_checked)
              {
                  // We must check those, because, when multiple collisions are found with same object
                  // object and his userObject can be removed. So we MUST check userObject to make
                  // sure not to work with deleted metadata
                  ok = (ev.m_object_1->userObject() != nullptr) &&  (ev.m_object_2->userObject() != nullptr);
              }
              if (ok)
              {
                  sad::p2d::CollisionEvent<_Object1, _Object2> e;
                  e.sad::p2d::BasicCollisionEvent::m_object_1 = ev.m_object_1;
                  e.sad::p2d::BasicCollisionEvent::m_object_2 = ev.m_object_2;
                  e.m_time = ev.m_time;
                  try
                  {
                      e.m_object_1 = (ev.m_object_1->userObject()) ? sad::checked_cast<_Object1>(ev.m_object_1->userObject()) : nullptr;
                  }
                  catch (sad::InvalidCastException ex)
                  {
                      e.m_object_1 = nullptr;
                  }

                  try
                  {
                      e.m_object_2 = (ev.m_object_2->userObject()) ? sad::checked_cast<_Object2>(ev.m_object_2->userObject()) : nullptr;
                  }
                  catch (sad::InvalidCastException ex)
                  {
                      e.m_object_2 = nullptr;
                  }
                  // Exit if one of arguments is incorrect
                  if (e.m_object_1 == nullptr || e.m_object_2 == nullptr)
                  {
                      return;
                  }
                  m_f(e);
              }
        }
    }
private:
    /*! Whether we should check values of bodies to call with
     */
    bool m_checked;
    /*! A function to call
     */
    std::function<void(const sad::p2d::CollisionEvent<_Object1, _Object2>&)> m_f;
};

/*! A specific collision handler for invoking events, for working with bodies
 */
template<>
class TypedCollisionHandler<sad::p2d::Body, sad::p2d::Body> : public sad::p2d::BasicCollisionHandler
{
public:
      /*! Constructs new handler
       * \param[in] f inner functional handler
       */
      inline TypedCollisionHandler(std::function<void(const BasicCollisionEvent&)> f ) : m_f(std::move(f))
      {
      }
      /*! Calls an inner handler for basic collision event
          \param[in] ev event
       */ 
      virtual void invoke(const sad::p2d::BasicCollisionEvent & ev) override;
private:
    /*! An inner untyped handler
        \brief[in] m_f a handler
     */
    std::function<void(const sad::p2d::BasicCollisionEvent &)> m_f;
};

}

}
