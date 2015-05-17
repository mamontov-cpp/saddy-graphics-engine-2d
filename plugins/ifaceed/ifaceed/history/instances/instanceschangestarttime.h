/*! \file instanceschangestarttime.h
    \author HiddenSeeker

    Describe a command, which changes start time for animation instance
 */
#pragma once
#include "instanceschangeproperty.h"

namespace history
{

namespace instances
{
/*! A command, which changes start time for animation instance
 */
class ChangeStartTime: public history::instances::ChangeProperty<double>
{
public:
     /*! Constructs new command for animation
        \param[in] d a node
        \param[in] oldvalue old text
        \param[in] newvalue new text
      */
    ChangeStartTime(sad::animations::Instance* d, double oldvalue, double newvalue);
    /*! Erases link to an animation
      */
    virtual ~ChangeStartTime();
protected:
    /*!
     * Updates current text in field with object name
     * \param[in] e editor
     * \param[in] value a value
     */
    virtual void updateUI(core::Editor* e, const double& value);
};

}

}
