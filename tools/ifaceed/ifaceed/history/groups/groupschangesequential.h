/*! \file groupschangesequential.h
    
    Describes a command for changing flag, that makes animation group play instances sequentially
 */
#pragma once
#include "groupschangeproperty.h"

namespace history
{

namespace groups
{
/*! Describes a command for changing flag, that makes animation group play instances sequentially
 */
class ChangeSequential: public history::groups::ChangeProperty<bool>
{
public:
     /*! Constructs new command for animation
        \param[in] d a node
        \param[in] old_value old text
        \param[in] new_value new text
      */
    ChangeSequential(sad::animations::Group* d, bool old_value, bool new_value);
    /*! Erases link to an animation
      */
    virtual ~ChangeSequential() override;
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
