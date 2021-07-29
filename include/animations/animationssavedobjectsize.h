/*! \file animations/animationssavedobjectsize.h
    

    Defines a saved size as a saved object state
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

/*! Defines a saved size as a saved object state
 */
class SavedObjectSize: public sad::animations::SavedObjectState
{
public:
    /*! Creates new saved object state
        \param[in] o object
     */
    SavedObjectSize(sad::db::Object* o);
    /*! Can be inherited
     */
    virtual ~SavedObjectSize() override;
    /*! Restores last object state
     */
    virtual void restore() override;
    /*! Stores body state to restore it, after animation is done.
        Default implementation does nothing
        \param[in] b body
     */
    virtual void storeBodyState(sad::p2d::Body* b) override;
protected:
    /*! Old area of object
     */
    sad::Rect2D m_old_area;
    /*! An old center
     */
    sad::Point2D m_old_center;
    /*! A body, which must be reset
     */
    sad::p2d::Body* m_body;
    /*! A shape to be restored, when animation is reset
     */ 
    sad::p2d::CollisionShape* m_shape;
};

}

}

