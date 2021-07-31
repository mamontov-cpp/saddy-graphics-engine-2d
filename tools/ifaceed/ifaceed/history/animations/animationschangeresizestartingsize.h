/*! \file animationschangeresizestartingsize.h
    

    Describe a command, which changes starting size of resize animation
 */
#pragma once
#include "animationschangeproperty.h"

namespace history
{

namespace animations
{
/*! A command,which changes starting size of resize animation
 */
class ChangeResizeStartingSize: public history::animations::ChangeProperty<sad::Point2D>
{
public:
     /*! Constructs new command for animation
        \param[in] d an animation
        \param[in] old_value old value
        \param[in] new_value new value
      */
    ChangeResizeStartingSize(sad::animations::Animation* d, const sad::Point2D& old_value, const sad::Point2D& new_value);
    /*! Erases link to an animation
      */
    virtual ~ChangeResizeStartingSize() override;
protected:
    /*!
     * Updates current text in field with object name
     * \param[in] e editor
     * \param[in] value a value
     */
    virtual void updateUI(core::Editor* e, const sad::Point2D& value) override;
};

}

}
