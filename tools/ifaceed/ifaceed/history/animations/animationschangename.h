/*! \file animationschangename.h
    

    Describe a command, which changes name of animations
 */
#pragma once
#include "animationschangeproperty.h"

namespace history
{

namespace animations
{
/*! A command, which change name of animation
 */
class ChangeName: public history::animations::ChangeProperty<sad::String>
{
public:
     /*! Constructs new command for animation
        \param[in] d a node
        \param[in] oldvalue old text
        \param[in] newvalue new text
      */
    ChangeName(sad::animations::Animation* d, const sad::String& oldvalue, const sad::String& newvalue);
    /*! Commits command without updating a text field
        \param[in] ob editor
     */
    void commitWithoutUpdatingUI(core::Editor* ob);
    /*! Erases link to an animation
      */
    virtual ~ChangeName();
protected:
    /*! Tries to update UI in case if node is selected
        \param[in] e editor
        \param[in] value a value
     */
    virtual void tryUpdateUI(core::Editor* e, const sad::String& value);
    /*!
     * Updates current text in field with object name
     * \param[in] e editor
     * \param[in] value a value
     */
    virtual void updateUI(core::Editor* e, const sad::String& value);
    /*! Whether we should update local text field
     */
    bool m_should_update_text_field;
};

}

}
