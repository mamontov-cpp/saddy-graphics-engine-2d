/*! \file animationschangewaymovingway.h
    

    Describes a command, which changes way for WayMoving animation
 */
#pragma once
#include "animationschangeproperty.h"

namespace history
{

namespace animations
{
/*! A command,which changes ay for WayMoving animation
 */
class ChangeWayMovingWay: public history::animations::ChangeProperty<unsigned long long>
{
public:
     /*! Constructs new command for animation
        \param[in] d an animation
        \param[in] old_value old value
        \param[in] new_value new value
      */
    ChangeWayMovingWay(sad::animations::Animation* d, unsigned long long old_value, unsigned long long new_value);
    /*! Erases link to an animation
      */
    virtual ~ChangeWayMovingWay() override;
protected:
    /*!
     * Updates current text in field with object name
     * \param[in] e editor
     * \param[in] value a value
     */
    void performUpdateUI(core::Editor* e, unsigned long long value);
    /*!
     * Updates current text in field with object name
     * \param[in] e editor
     * \param[in] value a value
     */
    virtual void updateUI(core::Editor* e, const unsigned long long& value) override;
};

}

}
