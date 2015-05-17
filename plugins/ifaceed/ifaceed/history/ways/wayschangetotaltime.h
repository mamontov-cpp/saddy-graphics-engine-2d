/*! \file wayschangetotaltime.h
    \author HiddenSeeker

    Describe a command, which change total time for way
 */
#pragma once
#include "wayschangeproperty.h"

namespace history
{

namespace ways
{
/*! A command, which which change total time for way
 */
class ChangeTotalTime: public history::ways::ChangeProperty<double>
{
public:
     /*! Constructs new command for way
        \param[in] d a node
        \param[in] oldvalue old time
        \param[in] newvalue new time
      */
    ChangeTotalTime(sad::p2d::app::Way* d, double oldvalue, double newvalue);
    /*! Erases link to a node
      */
    virtual ~ChangeTotalTime();
protected:
    /*!
     * Updates current text in field of total time
     * \param[in] e editor
     * \param[in] value a value
     */
    virtual void updateUI(core::Editor* e, const double& value);
};

}

}
