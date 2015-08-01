/*! \file animationschangepropertyaspoint2displayedintwospinboxes.h
    

    Describes a command, which depicts changing property as point2d, displayed in two 
    double spinboxes
 */
#pragma once
#include "animationschangeproperty.h"

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
        \param[in] oldvalue old value of property
        \param[in] newvalue new value of property
        \param[in] xwidget a widget for displaying x coordinate
        \param[in] ywidget a widget for displaying y coordinate
      */
    ChangePropertyAsPoint2DDisplayedInTwoSpinboxes(
        sad::animations::Animation* d,
        const sad::String& property,
        const sad::Point2D& oldvalue,
        const sad::Point2D& newvalue,
        QDoubleSpinBox* xwidget,
        QDoubleSpinBox* ywidget
    );
    /*! Erases link to an animation
      */
    ~ChangePropertyAsPoint2DDisplayedInTwoSpinboxes();
protected:
    /*!
     * Descendants must reimplement this method to make UI update
     * actually happen
     * \param e editor
     * \param value a value of property
     */
    virtual void updateUI(core::Editor* e, const sad::Point2D& value);
    /*! A widget for displaying x coordinate
     */
    QDoubleSpinBox* m_xwidget;
    /*! A widget for displaying y coordinate
     */
    QDoubleSpinBox* m_ywidget;
};

}

}
