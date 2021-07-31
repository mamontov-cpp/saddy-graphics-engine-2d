/*! \file animationschangefontsizesize.h
    

    Describe a command, which changes size for FontSize animation
 */
#pragma once
#include "animationschangeproperty.h"

#include <QSpinBox>

namespace history
{

namespace animations
{
/*! A command,which changes rotation for an animation
 */
class ChangeFontSizeSize: public history::animations::ChangeProperty<unsigned int>
{
public:
     /*! Constructs new command for animation
         \param[in] d an animation
         \param[in] property_name a name for property
         \param[in] view a view, that should be updated
         \param[in] old_value old value
         \param[in] new_value new value
      */
    ChangeFontSizeSize(
        sad::animations::Animation* d, 
        const sad::String& property_name,
        QSpinBox* view, 
        unsigned int old_value, 
        unsigned int new_value
    );
    /*! Erases link to an animation
      */
    virtual ~ChangeFontSizeSize() override;
protected:
    /*!
     * Updates current text in field with object name
     * \param[in] e editor
     * \param[in] value a value
     */
    virtual void updateUI(core::Editor* e, const unsigned int& value) override;
    /*! A color view, which should be updated
     */
    QSpinBox* m_view;
};

}

}
