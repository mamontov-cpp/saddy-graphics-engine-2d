/*! \file animationschangepropertyaspoint2displayedintwospinboxes.h
    

    Describes a command, which depicts changing property as point2d, displayed in two 
    double spinboxes
 */
#pragma once
#include "animationschangeproperty.h"
#include <QDoubleSpinBox>

namespace history
{
    
namespace animations
{
    
class ChangePropertyAsPoint2DDisplayedInTwoSpinboxes: public history::animations::ChangeProperty<sad::Point2D>
{
public:
     /*! Constructs new command for animation
        \param[in] d an animation
        \param[in] property a property name
        \param[in] old_value old value of property
        \param[in] new_value new value of property
        \param[in] x_widget a widget for displaying x coordinate
        \param[in] y_widget a widget for displaying y coordinate
      */
    ChangePropertyAsPoint2DDisplayedInTwoSpinboxes(
        sad::animations::Animation* d,
        const sad::String& property,
        const sad::Point2D& old_value,
        const sad::Point2D& new_value,
        QDoubleSpinBox* x_widget,
        QDoubleSpinBox* y_widget
    );
    /*! Erases link to an animation
      */
    ~ChangePropertyAsPoint2DDisplayedInTwoSpinboxes() override;
protected:
    /*!
     * Descendants must re-implement this method to make UI update
     * actually happen
     * \param e editor
     * \param value a value of property
     */
    virtual void updateUI(core::Editor* e, const sad::Point2D& value) override;
    /*! A widget for displaying x coordinate
     */
    QDoubleSpinBox* m_x_widget;
    /*! A widget for displaying y coordinate
     */
    QDoubleSpinBox* m_y_widget;
};

}

}
