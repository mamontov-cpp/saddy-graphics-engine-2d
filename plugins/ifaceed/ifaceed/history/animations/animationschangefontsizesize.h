/*! \file animationschangefontsizesize.h
    \author HiddenSeeker

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
		 \param[in] propertyname a name for property
         \param[in] view a view, that should be updated
		 \param[in] oldvalue old value
         \param[in] newvalue new value
      */
    ChangeFontSizeSize(
		sad::animations::Animation* d, 
		const sad::String& propertyname,
		QSpinBox* view, 
		unsigned int oldvalue, 
		unsigned int newvalue
	);
    /*! Erases link to an animation
      */
    virtual ~ChangeFontSizeSize();
protected:
    /*!
     * Updates current text in field with object name
     * \param[in] e editor
     * \param[in] value a value
     */
    virtual void updateUI(core::Editor* e, const unsigned int& value);
	/*! A color view, which should be updated
	 */
	QSpinBox* m_view;
};

}

}
