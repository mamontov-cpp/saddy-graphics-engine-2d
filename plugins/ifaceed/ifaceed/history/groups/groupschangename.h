/*! \file groupschangename.h
    \author HiddenSeeker

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
        \param[in] oldvalue old text
        \param[in] newvalue new text
      */
    ChangeName(sad::animations::Group* d, int position, const sad::String& oldvalue, const sad::String& newvalue);
    /*! Erases link to an animation
      */
    virtual ~ChangeName();
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
    virtual void tryUpdateUI(core::Editor* e, const sad::String& value);
    /*!
     * Updates current text in field with object name
     * \param[in] e editor
     * \param[in] value a value
     */
    virtual void updateUI(core::Editor* e, const sad::String& value);
	/*! A position
	 */
	int m_position;
};

}

}
