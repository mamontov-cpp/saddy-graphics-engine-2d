/*! \file force.h
    

    Describes a force, acting on object
 */
#pragma once
#include <stdexcept>

#include "vector.h"

#include "../sadvector.h"
#include "../object.h"
#include "../db/dbtypename.h"
#include "../fuzzyequal.h"

namespace sad
{

namespace p2d
{

class Body;


/*! A class for getting default value, for tickable, used also in
    movement. Should return something like zero.
*/
template<
    typename _Value
>
class TickableDefaultValue
{
public:
    static _Value get();
    /*! Returns a zero for current type
    */
    static _Value zero();
};

template <typename _Value>
_Value TickableDefaultValue<_Value>::get()
{
    return _Value{};
}

template <typename _Value>
_Value TickableDefaultValue<_Value>::zero()
{
	return _Value{};
}


/*! A class for getting default value for a tickable state
 */
template<>
class TickableDefaultValue<p2d::Vector>
{
public:
    static inline p2d::Vector get() { return {0, 0}; }
    static inline p2d::Vector zero() { return {0, 0}; } //-V524
};


/*! Describes a specific force with value type T.
 */
template<
    typename T
>
class Force: public sad::Object
{
public:
    /*! Creates zero force
     */
    inline Force() { m_alive = true; m_value = TickableDefaultValue<T>::zero(); }
    /*! Could be inherited
     */ 
	virtual ~Force()  override = default;
    /*! Creates a force with specific value
        \param[in] v value
    */
    inline Force(const T & v) : m_alive(true), m_value(v) {}
    /*! Forces a container to remove this force
     */
    inline void die() { m_alive = false; }
    /*! Whether force is alive
        \return whether it's alive
     */
    inline bool isAlive() const { return m_alive; }
    /*! Returns a value
        \param[in] body a body, to which force is applied to
        \return value of force
     */
    virtual const T & value(sad::p2d::Body* body) const { return  m_value; }
    /*! Sets a value for force
        \param[in] value a new value of force
     */
    virtual void setValue(const T & value) { m_value = value; }
    /*! Steps a force to next iteration
        \param[in] body a body, to which force is applied to
        \param[in] time a time step size
     */
    virtual void step(sad::p2d::Body* body, double time) {}
    
    /** Returns global meta data for force
        \return global meta data
     */
    static sad::ClassMetaData* globalMetaData()
    {
        sad::db::TypeName<T>::init();
        sad::String myName = "sad::p2d::Force<"; 
        myName += sad::db::TypeName<T>::name();
        myName += ">";
        return sad::ClassMetaDataContainer::ref()->getWithParent<sad::Object>(myName);  
    }
    /** Returns global meta data for force
        \return global meta data
     */
    virtual sad::ClassMetaData * metaData() const override
    {
        return sad::p2d::Force<T>::globalMetaData();
    }
    /** Returns a pointer to body, which force depends from (but not applied to).
        This body should be strong-referenced to ensure some memory-related checks
     */
    virtual sad::p2d::Body* dependsFromBody() const
    {
        return nullptr;
    }
protected:
    bool m_alive;  //!< If false, this force should be removed from container 
    T    m_value;  //!< A value of force
};

/*! Describes a simple impulse force with value T
 */ 
template<
    typename T
>
class ImpulseForce: public p2d::Force<T>
{
public:
    /*! Creates zero force
     */
    inline ImpulseForce() : p2d::Force<T>() {  }
    /*! Creates a force with specific value
        \param[in] v value
     */
    inline ImpulseForce(const T & v) : p2d::Force<T>(v) {}

    /*! Steps a force to next iteration
        \param[in] body a body
        \param[in] time a time step size
     */
    virtual void step(sad::p2d::Body* body, double time) override { this->die(); }

    /** Returns global meta data for force
        \return global meta data
     */
    static sad::ClassMetaData* globalMetaData()
    {
        sad::db::TypeName<T>::init();
        sad::String myName = "sad::p2d::ImpulseForce<"; 
        myName += sad::db::TypeName<T>::name();
        myName += ">";
        return sad::ClassMetaDataContainer::ref()->getWithParent<sad::p2d::Force<T> >(myName);  
    }
    /** Returns global meta data for force
        \return global meta data
     */
    virtual sad::ClassMetaData * metaData() const override
    {
        return sad::p2d::Force<T>::globalMetaData();
    }
};


typedef p2d::Force<p2d::Vector>        TangentialForce;
typedef p2d::ImpulseForce<p2d::Vector> TangentialImpulseForce;


/*! Describes an acting forces on body
 */
template<
    typename T
>
class ActingForces
{
public:
    ActingForces() : m_body(nullptr) {}
    /*! Force container owns all forces, belonging to it
     */
    virtual ~ActingForces() { clear(); }
    /*! Immediately adds new force to container
        \param[in] force new force
     */
    void add(sad::p2d::Force<T> * force) 
    { 
        if (force) 
        {
            checkBody(force);
            force->addRef(); 
            m_forces.push_back(force); 
        } 
    }
    /*! Schedules adding a force to container
        \param[in] force new force
     */
    void scheduleAdd(sad::p2d::Force<T>* force)  
    { 
        if (force)
        {
            checkBody(force);
            m_queued << new ScheduledAdd(force);
        }
    }
    /*! Schedules adding a force to container
        \param[in] force new force
        \param[in] time a current time
     */
    void scheduleAdd(sad::p2d::Force<T> * force, double time)  
    {
        if (force)
        {
            checkBody(force);
            m_queued << new ScheduledAddAt(force, time);
        }
    }

    /*! Removes specified force from a list
        \param[in] force a force
     */
    void remove(sad::p2d::Force<T>* force)
    {
        for (size_t i = 0; i < m_forces.size(); i++)
        {
            if (force == m_forces[i])
            {
                force->delRef();
                m_forces.removeAt(i);
                --i;
            }
        }
    }
    /*! Clears a force container
     */
    void clear() 
    {
        for(size_t i = 0; i < m_forces.size(); i++)
        {
            m_forces[i]->delRef();
        }
        m_forces.clear();
        for(size_t i = 0; i < m_queued.size(); i++)
        {
            delete m_queued[i];
        }
        m_queued.clear();
    }
    /*! Steps an acting forces on time
        \param[in] time a specific time
     */
    void step(double time)
    {
        for(size_t i = 0; i < m_forces.size(); i++)
        {
            p2d::Force<T> * p = m_forces[i];
            p->step(m_body, time);
            if (p->isAlive() == false)
            {
                m_forces[i]->delRef();
                m_forces.removeAt(i);
                --i;
            }
        }
        for(size_t i = 0; i < m_queued.size(); i++)
        {
            Command * c = m_queued[i];
            if (c->perform(time, this))
            {
                delete m_queued[i];
                m_queued.removeAt(i);
                --i;
            }
        }
    }
    /*! Computes a value for force
        \param[in] accumulator an accumulator
     */
    void value(T & accumulator) const
    {
        for(size_t i = 0; i < m_forces.size(); i++)
        {
            accumulator += m_forces[i]->value(m_body);
        }
    }
    /*! Whether container has a forces
        \return whether container has a forces
     */
    bool hasForces() const { return m_forces.size() != 0; }
    /*! Returns list of forces, acting on body
        \return list of forces
     */
    const sad::Vector<sad::p2d::Force<T>* > & forces() const
    {
        return m_forces;
    }
    /*! Set body for forces container. Note, that movement stores data by weak reference, so
        this class should not be exposed to some script data
        \param[in] body a body
     */
    void setBody(sad::p2d::Body* body)
    {
        m_body = body;
    }
protected:
    /*! Checks body consistency for added force
        \param[in] force added force
     */
    void checkBody(sad::p2d::Force<T> * force)
    {
        if ((force->dependsFromBody() == m_body) && m_body != nullptr)
        {
            throw std::logic_error("Attempted to add force, related to current body");
        }
    }
    /*! A queued command for performing of data
     */
    class Command
    {
    public:
        /*! Performs a command on container
          \param[in] time current time step size
          \param[in] container a container
          \return whether it was performed
         */
        virtual bool perform(double time, ActingForces<T> * container)
        {
            return true;
        }
        virtual ~Command() {}
    };
    /*! A scheduled adding, without time checking
     */
    class ScheduledAdd: public sad::p2d::ActingForces<T>::Command
    {
    public:
        ScheduledAdd(p2d::Force<T> * f) : m_force(f) { if (f) f->addRef(); }
        /*! Adds a force to container
          \param[in] time current time step size
          \param[in] container a container
          \return whether it was performed
        */
        virtual bool perform(double time, ActingForces<T> * container) override
        {
            container->add(m_force);
            return true;
        }
        virtual ~ScheduledAdd() { if (m_force) m_force->delRef(); }
    protected:
        p2d::Force<T> * m_force; //!< A force         
    };
    /*! A scheduled adding, without time checking
     */
    class ScheduledAddAt: public sad::p2d::ActingForces<T>::Command
    {
    public:
        ScheduledAddAt(p2d::Force<T>* f, double time) : m_force(f), m_time(time) { if (f) f->addRef();}
        /*! Adds a force to container
          \param[in] time current time step size
          \param[in] container a container
          \return whether it was performed
        */
        virtual bool perform(double time, ActingForces<T> * container) override
        {
            if (time > m_time || ::sad::is_fuzzy_equal(time, m_time))
            {
                container->add(m_force);
                return true;
            }
            return false;
        }
        virtual ~ScheduledAddAt() { if (m_force) m_force->delRef(); }
    protected:
        p2d::Force<T> * m_force; //!< A force
        double  m_time;          //!< A time, when should be added a force           
    };
    typedef p2d::Force<T> * force_t;
    typedef typename p2d::ActingForces<T>::Command * command_t;
protected:
    /*! A body to be set as main for container
     */
    sad::p2d::Body* m_body;
    sad::Vector< force_t >    m_forces;                       //!< A forces list, acting on body
    sad::Vector< command_t >  m_queued;                       //!< A queued list of commands
};


typedef p2d::ActingForces<p2d::Vector> TangentialActingForces;

}

}

DECLARE_TYPE_AS_SAD_OBJECT_ENUM(sad::p2d::Force<p2d::Vector>)
DECLARE_TYPE_AS_SAD_OBJECT_ENUM(sad::p2d::ImpulseForce<p2d::Vector>)
