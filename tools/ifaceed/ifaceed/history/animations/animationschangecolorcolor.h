/*! \file animationschangecolorcolor.h
    

    Describe a command, which changes color for an animation
 */
#pragma once
#include "animationschangeproperty.h"

// ReSharper disable once CppUnusedIncludeDirective
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
         \param[in] property_name a name for property
         \param[in] view a view, that should be updated
         \param[in] old_value old value
         \param[in] new_value new value
      */
    ChangeColorColor(
        sad::animations::Animation* d, 
        const sad::String& property_name,
        gui::colorview::ColorView* view, 
        const sad::AColor& old_value, 
        const sad::AColor& new_value
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
    virtual void updateUI(core::Editor* e, const sad::AColor& value) override;
    /*! A color view, which should be updated
     */
    gui::colorview::ColorView* m_view;
};

}

}
