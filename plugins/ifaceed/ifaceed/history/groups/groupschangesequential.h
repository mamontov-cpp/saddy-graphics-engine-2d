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
        \param[in] oldvalue old text
        \param[in] newvalue new text
      */
    ChangeSequential(sad::animations::Group* d, bool oldvalue, bool newvalue);
    /*! Erases link to an animation
      */
    virtual ~ChangeSequential();
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
