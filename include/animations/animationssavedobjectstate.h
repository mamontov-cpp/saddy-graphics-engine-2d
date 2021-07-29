/*! \file animations/animationssavedobjectstate.h
    

    A saved object state, which could be restored, when animation is finished
 */
#pragma once
// ReSharper disable once CppUnusedIncludeDirective
#include "../db/dbobject.h"

namespace sad
{

namespace p2d
{
class Body;
}
    
namespace animations
{
    
class SavedObjectState
{
public:
    /*! Creates new saved object state
        \param[in] o object
     */
    SavedObjectState(sad::db::Object* o);
    /*! Can be inherited
     */
    virtual ~SavedObjectState();
    /*! Restores last object state
     */
    virtual void restore() = 0;
    /*! Stores body state to restore it, after animation is done.
        Default implementation does nothing
        \param[in] b body
     */
    virtual void storeBodyState(sad::p2d::Body* b);
    /*! Increments references to state
     */
    inline void increment()
    {
        ++m_references;
    }
    /*! Decrements references to state
        \return whether we should delete a state (true if need to)
     */
    inline bool decrement()
    {
        bool result = false;
        --m_references;
        if (m_references <= 0)
        {
            result = true;
            this->restore();
        }
        return result;
    }
protected:
    /*! Counts references for resetting state. A state should be restored, when references is zero
     */
    int m_references;
    /*! An object, which state is saved
     */
    sad::db::Object* m_object;
};

}

}
