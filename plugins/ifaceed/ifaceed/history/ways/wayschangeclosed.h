/*! \file wayschangeclosed.h
    \author HiddenSeeker

    Describe a command, which change closedness for way
 */
#pragma once
#include "wayschangeproperty.h"

namespace history
{

namespace ways
{
/*! A command, which which change closedness for way
 */
class ChangeClosed: public history::ways::ChangeProperty<bool>
{
public:
     /*! Constructs new command for way
        \param[in] d a node
        \param[in] oldvalue old time
        \param[in] newvalue new time
      */
    ChangeClosed(sad::p2d::app::Way* d, bool oldvalue, bool newvalue);
    /*! Erases link to a node
      */
    virtual ~ChangeClosed();
protected:
    /*!
     * Updates current text in field of total time
     * \param[in] e editor
     * \param[in] value a value
     */
    virtual void updateUI(core::Editor* e, const bool& value);
};

}

}
