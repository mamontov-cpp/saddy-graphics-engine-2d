/*! \file wayschangeclosed.h
    

    Describe a command, which change closedness for way
 */
#pragma once
#include "wayschangeproperty.h"

namespace history
{

namespace ways
{
/*! A command, which which change closedness for way
 */
class ChangeClosed: public history::ways::ChangeProperty<bool>
{
public:
     /*! Constructs new command for way
        \param[in] d a node
        \param[in] old_value old time
        \param[in] new_value new time
      */
    ChangeClosed(sad::Way* d, bool old_value, bool new_value);
    /*! Erases link to a node
      */
    virtual ~ChangeClosed() override;
protected:
    /*!
     * Updates current text in field of total time
     * \param[in] e editor
     * \param[in] value a value
     */
    virtual void updateUI(core::Editor* e, const bool& value) override;
};

}

}
