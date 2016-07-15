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
        \param[in] oldvalue old value
        \param[in] newvalue new value
      */
    ChangeCameraPivot(sad::animations::Animation* d, const sad::Point3D& oldvalue, const sad::Point3D& newvalue);
    /*! Erases link to an animation
      */
    virtual ~ChangeCameraPivot();
protected:
    /*!
     * Updates current text in field with object name
     * \param[in] e editor
     * \param[in] value a value
     */
    virtual void updateUI(core::Editor* e, const sad::Point3D& value);
};

}

}
