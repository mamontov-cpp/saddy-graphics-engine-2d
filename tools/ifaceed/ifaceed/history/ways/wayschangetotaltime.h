/*! \file wayschangetotaltime.h
    

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
        \param[in] old_value old time
        \param[in] new_value new time
      */
    ChangeTotalTime(sad::Way* d, double old_value, double new_value);
    /*! Erases link to a node
      */
    virtual ~ChangeTotalTime() override;
protected:
    /*!
     * Updates current text in field of total time
     * \param[in] e editor
     * \param[in] value a value
     */
    virtual void updateUI(core::Editor* e, const double& value) override ;
};

}

}
