/*! \file animationschangelist.h
    

    Describes a command, which changes list for any kind of animation
 */
#pragma once
#include "animationschangeproperty.h"

#include <sadvector.h>
#include <sadstring.h>

#include <QTextEdit>

namespace history
{

namespace animations
{
/*! A command, which changes font list for font list animation
 */
class ChangeList: public history::animations::ChangeProperty< sad::Vector<sad::String> >
{
public:
     /*! Constructs new command for animation
        \param[in] d an animation
        \param[in] property_name a property for editing
        \param[in] view a view for editing
        \param[in] old_value old value
        \param[in] new_value new value
      */
    ChangeList(
        sad::animations::Animation* d, 
        const sad::String& property_name,
        QTextEdit* view,
        const sad::Vector<sad::String>& old_value, 
        const sad::Vector<sad::String>& new_value
    );
    /*! Commits changes without updating widget
        \param[in] ob editor
     */
    void commitWithoutUpdatingUI(core::Editor* ob) override;
    /*! Erases link to an animation
      */
    virtual ~ChangeList();
protected:
    /*!
     * Updates current text in field with object name
     * \param[in] e editor
     * \param[in] value a value
     */
    virtual void updateUI(core::Editor* e, const sad::Vector<sad::String>& value) override;
    /*! A view for text edit
     */
    QTextEdit* m_view;
    /*! A flag, which describes, whether we should change widget
     */
    bool m_should_change_widget;
};

}

}
