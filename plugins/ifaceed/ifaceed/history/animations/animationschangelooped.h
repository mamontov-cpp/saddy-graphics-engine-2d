/*! \file animationschangelooped.h
    \author HiddenSeeker

    Describes a command, which changes flag, which indicates, whether animation is looped
 */
#pragma once
#include "animationschangeproperty.h"

namespace history
{

namespace animations
{
/*! A command,which changes flag, which indicates, whether animation is looped
 */
class ChangeLooped: public history::animations::ChangeProperty<bool>
{
public:
     /*! Constructs new command for animation
        \param[in] d an animation
        \param[in] oldvalue old value
        \param[in] newvalue new value
      */
    ChangeLooped(sad::animations::Animation* d, bool oldvalue, bool newvalue);
    /*! Erases link to an animation
      */
    virtual ~ChangeLooped();
protected:
    /*!
     * Updates current text in field with object name
     * \param[in] e editor
     * \param[in] value a value
     */
    virtual void updateUI(core::Editor* e, const bool& value);
};

}

}
