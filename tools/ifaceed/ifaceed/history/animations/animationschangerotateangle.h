/*! \file animationschangerotateangle.h
    

    Describe a command, which changes angle for a rotate animation
 */
#pragma once
#include "animationschangeproperty.h"

#include <QDoubleSpinBox>

namespace history
{

namespace animations
{
/*! A command, which changes rotation for an animation
 */
class ChangeRotateAngle: public history::animations::ChangeProperty<double>
{
public:
     /*! Constructs new command for animation
         \param[in] d an animation
         \param[in] property_name a name for property
         \param[in] view a view, that should be updated
         \param[in] old_value old value
         \param[in] new_value new value
      */
    ChangeRotateAngle(
        sad::animations::Animation* d, 
        const sad::String& property_name,
        QDoubleSpinBox* view, 
        double old_value, 
        double new_value
    );
    /*! Erases link to an animation
      */
    virtual ~ChangeRotateAngle() override;
protected:
    /*!
     * Updates current text in field with object name
     * \param[in] e editor
     * \param[in] value a value
     */
    virtual void updateUI(core::Editor* e, const double& value) override ;
    /*! A color view, which should be updated
     */
    QDoubleSpinBox* m_view;
};

}

}
