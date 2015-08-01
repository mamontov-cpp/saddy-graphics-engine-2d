/*! \file animationschangecolorcolor.h
    

    Describe a command, which changes color for an animation
 */
#pragma once
#include "animationschangeproperty.h"

#include "../../gui/colorview/colorview.h"

namespace history
{

namespace animations
{
/*! A command,which changes color of specified animations
 */
class ChangeColorColor: public history::animations::ChangeProperty<sad::AColor>
{
public:
     /*! Constructs new command for animation
         \param[in] d an animation
		 \param[in] propertyname a name for property
         \param[in] view a view, that should be updated
		 \param[in] oldvalue old value
         \param[in] newvalue new value
      */
    ChangeColorColor(
		sad::animations::Animation* d, 
		const sad::String& propertyname,
		gui::colorview::ColorView* view, 
		const sad::AColor& oldvalue, 
		const sad::AColor& newvalue
	);
    /*! Erases link to an animation
      */
    virtual ~ChangeColorColor();
protected:
    /*!
     * Updates current text in field with object name
     * \param[in] e editor
     * \param[in] value a value
     */
    virtual void updateUI(core::Editor* e, const sad::AColor& value);
	/*! A color view, which should be updated
	 */
	gui::colorview::ColorView* m_view;
};

}

}
