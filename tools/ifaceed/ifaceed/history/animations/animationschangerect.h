/*! \file animationschangerect.h
    

    Describes a command, which changes rectangle for specified property
 */
#pragma once
#include "animationschangeproperty.h"

#include <sadvector.h>
#include <sadstring.h>

#include "../../gui/rectwidget/rectwidget.h"

namespace history
{

namespace animations
{
/*! A command, which changes rectangle for specified property
 */
class ChangeRect: public history::animations::ChangeProperty< sad::Rect2D >
{
public:
     /*! Constructs new command for animation
        \param[in] d an animation
        \param[in] property_name a property for editing
        \param[in] view a view for editing
        \param[in] old_value old value
        \param[in] new_value new value
      */
    ChangeRect(
        sad::animations::Animation* d, 
        const sad::String& property_name,
        gui::rectwidget::RectWidget* view,
        const sad::Rect2D& old_value, 
        const sad::Rect2D& new_value
    );
    /*! Erases link to an animation
      */
    virtual ~ChangeRect() override;
protected:
    /*!
     * Updates current text in field with object name
     * \param[in] e editor
     * \param[in] value a value
     */
    virtual void updateUI(core::Editor* e, const sad::Rect2D& value) override;
    /*! A view for text edit
     */
    gui::rectwidget::RectWidget* m_view;
};

}

}
