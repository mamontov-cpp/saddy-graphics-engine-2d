/*! \file animations/animationssavedobjectposition.h
    

    Defines a saved size as a saved object position
 */
#pragma once
#include "animationssavedobjectstate.h"

namespace sad
{

namespace p2d
{
class CollisionShape;
}

namespace animations
{

/*! Defines a saved position as a saved object state
 */
class SavedObjectPosition: public sad::animations::SavedObjectState
{
public:
    /*! Creates new saved object position
        \param[in] o object
     */
    SavedObjectPosition(sad::db::Object* o);
    /*! Can be inherited
     */
    virtual ~SavedObjectPosition() override;
    /*! Restores last object state
     */
    virtual void restore() override;
    /*! Stores body position to restore it, after animation is done.
        Default implementation does nothing
        \param[in] b body
     */
    virtual void storeBodyState(sad::p2d::Body* b) override;
protected:
    /*! Old area of object
     */
    sad::Point2D m_old_center;
    /*! A body, which must be reset
     */
    sad::p2d::Body* m_body;
    /*! An old position for body
     */ 
    sad::Point2D m_old_position;
};

}

}
