/*! \file animationschangerect.h
    

    Describes a command, which changes rectangle for specifict property
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
/*! A command, which changes rectangle for specifict property
 */
class ChangeRect: public history::animations::ChangeProperty< sad::Rect2D >
{
public:
     /*! Constructs new command for animation
        \param[in] d an animation
		\param[in] propertyname a property for editing
		\param[in] view a view for editing
        \param[in] oldvalue old value
        \param[in] newvalue new value
      */
    ChangeRect(
		sad::animations::Animation* d, 
		const sad::String& propertyname,
		gui::rectwidget::RectWidget* view,
		const sad::Rect2D& oldvalue, 
		const sad::Rect2D& newvalue
	);
    /*! Erases link to an animation
      */
    virtual ~ChangeRect();
protected:
    /*!
     * Updates current text in field with object name
     * \param[in] e editor
     * \param[in] value a value
     */
    virtual void updateUI(core::Editor* e, const sad::Rect2D& value);
	/*! A view for text edit
	 */
	gui::rectwidget::RectWidget* m_view;
};

}

}
