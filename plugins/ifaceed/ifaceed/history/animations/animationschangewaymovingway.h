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
        \param[in] oldvalue old value
        \param[in] newvalue new value
      */
    ChangeWayMovingWay(sad::animations::Animation* d, unsigned long long oldvalue, unsigned long long newvalue);
    /*! Erases link to an animation
      */
    virtual ~ChangeWayMovingWay();
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
    virtual void updateUI(core::Editor* e, const unsigned long long& value);
};

}

}
