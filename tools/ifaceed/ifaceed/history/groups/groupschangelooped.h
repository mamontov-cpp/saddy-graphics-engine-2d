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
        \param[in] old_value old text
        \param[in] new_value new text
      */
    ChangeLooped(sad::animations::Group* d, bool old_value, bool new_value);
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
