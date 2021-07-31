/*! \file instanceschangename.h
    

    Describe a command, which changes name of animation instances
 */
#pragma once
#include "instanceschangeproperty.h"

namespace history
{

namespace instances
{
/*! A command, which change name of animation
 */
class ChangeName: public history::instances::ChangeProperty<sad::String>
{
public:
     /*! Constructs new command for animation
        \param[in] d a node
        \param[in] position a position of instance
        \param[in] old_value old text
        \param[in] new_value new text
      */
    ChangeName(sad::animations::Instance* d, int position, const sad::String& old_value, const sad::String& new_value);
    /*! Commits changes without updating linked text fields
        \param[in] ob editor
     */
    void commitWithoutUpdatingUI(core::Editor* ob) override;
    /*! Erases link to an animation
      */
    virtual ~ChangeName() override;
protected:
    /*! Updates item in main list
        \param[in] e editor
        \param[in] value a value
     */
    void updateItem(core::Editor* e, const sad::String& value);
    /*! Tries to update UI in case if node is selected
        \param[in] e editor
        \param[in] value a value
     */
    virtual void tryUpdateUI(core::Editor* e, const sad::String& value) override;
    /*!
     * Updates current text in field with object name
     * \param[in] e editor
     * \param[in] value a value
     */
    virtual void updateUI(core::Editor* e, const sad::String& value) override;
    /*! A position
     */
    int m_position;
    /*! Whether we should update linked text field
     */
    bool m_should_update_text_field;
};

}

}
