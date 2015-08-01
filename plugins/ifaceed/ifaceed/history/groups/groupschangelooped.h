/*! \file groupschangelooped.h
    

    Describe a command, which changes looped flag of animation group
 */
#pragma once
#include "groupschangeproperty.h"

namespace history
{

namespace groups
{
/*! A command, which change looped flag of animation group
 */
class ChangeLooped: public history::groups::ChangeProperty<bool>
{
public:
     /*! Constructs new command for animation
        \param[in] d a node
        \param[in] oldvalue old text
        \param[in] newvalue new text
      */
    ChangeLooped(sad::animations::Group* d, bool oldvalue, bool newvalue);
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
