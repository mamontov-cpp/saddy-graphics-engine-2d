/*! \file animationschangelooped.h
    

    Describes a command, which changes flag, which indicates, whether animation is looped
 */
#pragma once
#include "animationschangeproperty.h"

namespace history
{

namespace animations
{
/*! A command, which changes flag, which indicates, whether animation is looped
 */
class ChangeLooped: public history::animations::ChangeProperty<bool>
{
public:
     /*! Constructs new command for animation
        \param[in] d an animation
        \param[in] old_value old value
        \param[in] new_value new value
      */
    ChangeLooped(sad::animations::Animation* d, bool old_value, bool new_value);
    /*! Erases link to an animation
      */
    virtual ~ChangeLooped() override;
protected:
    /*!
     * Updates current text in field with object name
     * \param[in] e editor
     * \param[in] value a value
     */
    virtual void updateUI(core::Editor* e, const bool& value) override;
};

}

}
