/*! \file instanceschangeobject.h
    

    Describe a command, which changes object for animation instance
 */
#pragma once
#include "instanceschangeproperty.h"

namespace history
{

namespace instances
{
/*! A command, which changes object for animation instance
 */
class ChangeObject: public history::instances::ChangeProperty<unsigned long long>
{
public:
     /*! Constructs new command for animation
        \param[in] d a node
        \param[in] old_value old text
        \param[in] new_value new text
      */
    ChangeObject(sad::animations::Instance* d, unsigned long long old_value, unsigned long long new_value);
    /*! Erases link to an animation
      */
    virtual ~ChangeObject() override;
protected:
    /*! Updates object in main list
        \param[in] e editor
        \param[in] value a value
     */
    void updateItem(core::Editor* e, unsigned long long value);
    /*!
     * Updates current text in field with object name
     * \param[in] e editor
     * \param[in] value a value
     */
    virtual void updateUI(core::Editor* e, const unsigned long long& value) override;
};

}

}
