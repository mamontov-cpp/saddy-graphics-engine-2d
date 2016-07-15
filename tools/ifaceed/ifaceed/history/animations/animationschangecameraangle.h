/*! \file animationschangecameraangle.h
    

    Describe a command, which changes angle for a camera rotation animation
 */
#pragma once
#include "animationschangeproperty.h"

#include <QDoubleSpinBox>

namespace history
{

namespace animations
{
/*! A command,  which changes angle for a camera rotation animation
 */
class ChangeCameraAngle: public history::animations::ChangeProperty<double>
{
public:
     /*! Constructs new command for animation
         \param[in] d an animation
         \param[in] propertyname a name for property
         \param[in] view a view, that should be updated
         \param[in] oldvalue old value
         \param[in] newvalue new value
      */
    ChangeCameraAngle(
        sad::animations::Animation* d, 
        const sad::String& propertyname,
        QDoubleSpinBox* view, 
        double oldvalue, 
        double newvalue
    );
    /*! Erases link to an animation
      */
    virtual ~ChangeCameraAngle();
protected:
    /*!
     * Updates current text in field with object name
     * \param[in] e editor
     * \param[in] value a value
     */
    virtual void updateUI(core::Editor* e, const double& value);
    /*! A color view, which should be updated
     */
    QDoubleSpinBox* m_view;
};

}

}
