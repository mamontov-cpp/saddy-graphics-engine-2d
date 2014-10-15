/*! \file wayschangename.h
    \author HiddenSeeker

    Describe a command, which change name of way
 */
#pragma once
#include "wayschangeproperty.h"

namespace history
{

namespace ways
{
/*! A command, which  which change name of way
 */
class ChangeName: public history::ways::ChangeProperty<sad::String>
{
public:
     /*! Constructs new command for way
        \param[in] d a node
        \param[in] oldvalue old text
        \param[in] newvalue new text
      */
    ChangeName(sad::p2d::app::Way* d, const sad::String oldvalue, sad::String newvalue);
    /*! Erases link to a node
      */
    virtual ~ChangeName();
protected:
    /*!
     * Updates current text in field of object name
     * \param[in] e editor
     * \param[in] value a value
     */
    virtual void updateUI(core::Editor* e, const sad::String& value);
};

}

}
