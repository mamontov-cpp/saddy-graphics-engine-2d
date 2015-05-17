/*! \file animationschangecameraoffset.h
    \author HiddenSeeker

    Describes a command, which changes camera offset for CameraShaking
 */
#pragma once
#include "animationschangeproperty.h"

#include <sadpoint.h>

namespace history
{

namespace animations
{
/*! A command,which changes camera offset for CameraShaking
 */
class ChangeCameraOffset: public history::animations::ChangeProperty<sad::Point2D>
{
public:
     /*! Constructs new command for animation
        \param[in] d an animation
        \param[in] oldvalue old value
        \param[in] newvalue new value
      */
    ChangeCameraOffset(sad::animations::Animation* d, const sad::Point2D& oldvalue, const sad::Point2D& newvalue);
    /*! Erases link to an animation
      */
    virtual ~ChangeCameraOffset();
protected:
    /*!
     * Updates current text in field with object name
     * \param[in] e editor
     * \param[in] value a value
     */
    virtual void updateUI(core::Editor* e, const sad::Point2D& value);
};

}

}
