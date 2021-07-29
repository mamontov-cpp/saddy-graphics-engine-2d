/*! \file animations/animationsfactory.h
    

    Describes an animation factory for creating animations
 */
#pragma  once
#include "animationsanimation.h"

#include "../sadptrhash.h"

namespace sad
{
    
namespace animations
{
    
/*! A class for creating animations by their names
 */
class Factory //-V690
{
public:	
/*! An  abstract delegate for creating an animation
 */
class AbstractDelegate
{
public:
    /*! Must be inherited
     */
    virtual ~AbstractDelegate();
    /*! Copies a delegate 
        \return copy of delegate
     */
    virtual sad::animations::Factory::AbstractDelegate* clone() const = 0;
    /*! Makes new animation
        \return new animation
     */
    virtual sad::animations::Animation* create() const = 0;
};

/*! A delegate for creating animation for specific type
 */
template<
    typename T
>
class Delegate: public sad::animations::Factory::AbstractDelegate
{
public:
    /*! Does nothing
     */
    inline Delegate()
    = default;
    /*! Can be inherited
     */
    virtual ~Delegate() override
    = default;
    /*! Copies a delegate 
        \return copy of delegate
     */
    virtual sad::animations::Factory::AbstractDelegate* clone() const override
    {
        return new sad::animations::Factory::Delegate<T>();
    }
    /*! Makes new animation
        \return new animation
     */
    virtual sad::animations::Animation* create() const override
    {
        return new T();
    }	
};
    /*! Constructs default factory
     */
    Factory();
    /*! Constructs default factory from existing
        \param[in] f other factory
     */
    Factory(const sad::animations::Factory& f);
    /*! Copies one factory to another
        \param[in] f other factory
     */
    Factory& operator=(const sad::animations::Factory& f);
    /*! Could be inherited
     */
    virtual ~Factory();
    /*! Creates new animations by name
        \param[in] s string
        \return new animation (nullptr if cannot be created)
     */
    virtual sad::animations::Animation* create(const sad::String& s);
    /*! Clears a factory, deleting all items
     */
    void clear();
    /*! Pushes into factory a delegate for creating animations
        of specified types
        \param[in] name name of type
     */
    template<typename _Animation>
    void push(const sad::String & name)
    {
        if (m_delegates.contains(name))
        {
            delete m_delegates[name]; //-V515
        }
        m_delegates.insert(name, new sad::animations::Factory::Delegate<_Animation>());
    }
    /*! Returns list of registered animations
        \return list of registered animation
     */
    sad::Vector<sad::String> registeredClasses() const;
protected:
    /*! Copies a delegates from factory
        \param[in] f factory
     */
    void copy(const sad::animations::Factory& f);
    /*! A list of delegates
     */
    sad::PtrHash<sad::String, sad::animations::Factory::AbstractDelegate> m_delegates;
};


}

}