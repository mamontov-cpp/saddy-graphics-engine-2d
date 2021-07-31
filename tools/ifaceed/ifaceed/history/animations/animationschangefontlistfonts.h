/*! \file animationschangefontlistfonts.h
    

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
        \param[in] old_value old value
        \param[in] new_value new value
      */
    ChangeFontListFonts(
        sad::animations::Animation* d, 
        const sad::Vector<sad::String>& old_value, 
        const sad::Vector<sad::String>& new_value
    );
    /*! Erases link to an animation
      */
    virtual ~ChangeFontListFonts() override;
protected:
    /*!
     * Updates current text in field with object name
     * \param[in] e editor
     * \param[in] value a value
     */
    virtual void updateUI(core::Editor* e, const sad::Vector<sad::String>& value) override;
};

}

}
