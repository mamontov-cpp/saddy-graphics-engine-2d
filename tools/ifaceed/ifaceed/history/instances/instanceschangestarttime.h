/*! \file instanceschangestarttime.h
    

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
        \param[in] old_value old text
        \param[in] new_value new text
      */
    ChangeStartTime(sad::animations::Instance* d, double old_value, double new_value);
    /*! Erases link to an animation
      */
    virtual ~ChangeStartTime() override;
protected:
    /*!
     * Updates current text in field with object name
     * \param[in] e editor
     * \param[in] value a value
     */
    virtual void updateUI(core::Editor* e, const double& value) override ;
};

}

}
