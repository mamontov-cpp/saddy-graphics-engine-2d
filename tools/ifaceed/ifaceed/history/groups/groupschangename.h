/*! \file groupschangename.h
    

    Describe a command, which changes name of animation group
 */
#pragma once
#include "groupschangeproperty.h"

namespace history
{

namespace groups
{
/*! A command, which change name of animation group
 */
class ChangeName: public history::groups::ChangeProperty<sad::String>
{
public:
     /*! Constructs new command for animation
        \param[in] d a node
        \param[in] position a position of instance
        \param[in] old_value old text
        \param[in] new_value new text
      */
    ChangeName(sad::animations::Group* d, int position, const sad::String& old_value, const sad::String& new_value);
    /*! Commits, without updating interface
        \param[in] ob editor
     */
    virtual void commitWithoutUpdatingUI(core::Editor* ob) override;
    /*! Erases link to an animation
      */
    virtual ~ChangeName();
protected:
    /*! Updates item in main list
        \param[in] e editor
        \param[in] value a value
     */
    void updateItem(core::Editor* e, const sad::String& value) const;
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
    /*! Whether we should update user interface
     */
    bool m_should_update_ui;
};

}

}
