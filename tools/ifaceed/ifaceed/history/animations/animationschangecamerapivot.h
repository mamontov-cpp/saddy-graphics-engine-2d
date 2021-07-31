/*! \file animationschangecamerapivot.h
    

    Describes a command, which changes camera pivot for CameraRotation
 */
#pragma once
#include "animationschangeproperty.h"

#include <sadpoint.h>

namespace history
{

namespace animations
{
/*! A command,which changes camera pivot for CameraRotation
 */
class ChangeCameraPivot: public history::animations::ChangeProperty<sad::Point3D>
{
public:
     /*! Constructs new command for animation
        \param[in] d an animation
        \param[in] old_value old value
        \param[in] new_value new value
      */
    ChangeCameraPivot(sad::animations::Animation* d, const sad::Point3D& old_value, const sad::Point3D& new_value);
    /*! Erases link to an animation
      */
    virtual ~ChangeCameraPivot();
protected:
    /*!
     * Updates current text in field with object name
     * \param[in] e editor
     * \param[in] value a value
     */
    virtual void updateUI(core::Editor* e, const sad::Point3D& value) override;
};

}

}
