/*! \file animationschangefontlistfonts.h
    \author HiddenSeeker

    Describes a command, which changes font list for font list animation
 */
#pragma once
#include "animationschangelist.h"

#include <sadvector.h>
#include <sadstring.h>

namespace history
{

namespace animations
{
/*! A command, which changes font list for font list animation
 */
class ChangeFontListFonts: public history::animations::ChangeList
{
public:
     /*! Constructs new command for animation
        \param[in] d an animation
        \param[in] oldvalue old value
        \param[in] newvalue new value
      */
    ChangeFontListFonts(
		sad::animations::Animation* d, 
		const sad::Vector<sad::String>& oldvalue, 
		const sad::Vector<sad::String>& newvalue
	);
    /*! Erases link to an animation
      */
    virtual ~ChangeFontListFonts();
protected:
    /*!
     * Updates current text in field with object name
     * \param[in] e editor
     * \param[in] value a value
     */
    virtual void updateUI(core::Editor* e, const sad::Vector<sad::String>& value);
};

}

}
