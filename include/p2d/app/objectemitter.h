/*! \file objectemitter.h
    

    Describes a simple emitter for objects, which emits objects
    in specified direction
 */
#pragma once
#include "object.h"
#include "app.h"

#include "../../periodicalevent.h"

#include <cstdlib>

namespace sad
{

namespace p2d
{

namespace app
{


/*! A basic object emitter, that can emit a lot of objects
 */
class AbstractObjectEmitter: public sad::PeriodicalEvent
{
public:
        /*! Sets application for emitter
            \param[in] app application
         */
        inline AbstractObjectEmitter(p2d::app::App * app = nullptr) : m_app(app) {}
        /*! An emitter does not own application
         */
        virtual ~AbstractObjectEmitter()  override = default;
        /*! Sets an application
            \param[in] app application
         */
        inline void setApp(p2d::app::App * app) { m_app = app; }
        /*! Returns an application
            \return application
         */
        virtual p2d::app::App * app() { return m_app; }
        /*! Returns angular velocity
            \return angular velocity
         */
        virtual double angularVelocity() = 0;
        /*! Sets a tangential velocity
            \param[out] v velocity
         */
        virtual void tangentialVelocity(p2d::Vector & v) = 0;
        /*! Sets an angle
            \return angle
         */
        virtual double angle() = 0;
        /*! Returns a position
            \param[out] p position, where object will be emitted from
         */
        virtual void position(p2d::Point & p) = 0;
        /*! Produces an object
            \return object
         */
        virtual p2d::app::Object * produce() = 0;
        /*! Emits object
         */
        virtual void perform() override;
        /*! Adds an additional group, where objects should be stored
            \param[in] group_name a group name
         */
        void addAdditionalGroupForObjects(const sad::String& group_name);
protected:
        p2d::app::App * m_app;
        /*! An additional groups for adding into world
         */
        sad::Vector<sad::String> m_additional_groups;
};


/*! A random object emitter, emits objects in direction, specified by random
    objects data
 */
template<
    typename _Object
>
class RandomDirectionObjectEmitter: public p2d::app::AbstractObjectEmitter
{
public:
    RandomDirectionObjectEmitter(p2d::app::App * app = nullptr)
    : p2d::app::AbstractObjectEmitter(app)
    {
        m_min_angle = 0;
        m_max_angle = 0;
        m_min_angular = 0;
        m_max_angular = 0;
        m_min_count = 1;
        m_max_count = 1;
    }

    void setMinAngle(double a) { m_min_angle = a; }
    void setMaxAngle(double a) { m_max_angle = a; }

    void setMinAngularVelocity(double a) { m_min_angular = a; }
    void setMaxAngularVelocity(double a) { m_max_angular = a; }

    void setMinSpeed(const p2d::Vector & s) { m_min_speed = s; }
    void setMaxSpeed(const p2d::Vector & s) { m_max_speed = s; }

    void setMinPosition(const p2d::Vector & s) { m_min_position = s; }
    void setMaxPosition(const p2d::Vector & s) { m_max_position = s; }
    

    void setMinCount(unsigned int c) {m_min_count = c;}
    void setMaxCount(unsigned int c) {m_max_count = c;}


    /*! Returns angular velocity
        \return angular velocity
     */
    virtual double angularVelocity() override
    {
        return m_min_angular + (m_max_angular - m_min_angular) * this->prand();
    }

    /*! Sets a tangential velocity
        \param[out] v velocity
     */
    virtual void tangentialVelocity(p2d::Vector & v) override
    {
        v = m_min_speed + (m_max_speed - m_min_speed) * this->prand();
    }

    /*! Sets an angle
        \return angle
      */
    virtual double angle()  override
    {
        return m_min_angle + (m_max_angle - m_min_angle) * this->prand();
    }
    /*! Returns a position
        \param[out] p  a position, where object should be created
     */
    virtual void position(p2d::Point & p) override
    {
        p = m_min_position + (m_max_position - m_min_position) * this->prand();
    }

    /*! Produces an object
        \return object
     */
    virtual p2d::app::Object * produce() override
    {
        return new _Object();
    }

    /*! Emits objects
     */
    virtual void perform() override
    {
	    const unsigned int c = static_cast<unsigned>(m_min_count + (m_max_count - m_min_count) * this->prand());
        for(unsigned int i = 0; i < c; i++)
        {
            this->p2d::app::AbstractObjectEmitter::perform();
        }
    }
protected:
    double m_min_angle; //!< A minimal angle, which objects speed differs
    double m_max_angle; //!< A maximum angle, which objects speed differs

    p2d::Vector m_min_speed; //!< A minimal speed of object
    p2d::Vector m_max_speed; //!< A maximal speed of object

    double m_min_angular; //!< A minimal angular speed
    double m_max_angular; //!< A maximal angular speed

    p2d::Point  m_min_position; //!< A minimal position of point
    p2d::Point  m_max_position; //!< A minimal position of point

    unsigned int m_min_count;  //!< Determines, how many objects should be emitted (minimal)
    unsigned int m_max_count;  //!< Determines, how many objects should be emitted  (maximal)

    static inline double prand()
    {
        return static_cast<double>(rand()) / RAND_MAX;
    }
};


}

}

}
