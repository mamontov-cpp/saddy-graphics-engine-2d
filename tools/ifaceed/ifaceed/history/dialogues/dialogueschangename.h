/*! \file dialogueschangename.h
 *  
 *
 * Describes a simple command for changing name of dialogue
 */
#pragma once
#include "../command.h"
#include <dialogue/dialogue.h>

namespace history
{

namespace dialogues
{

/*! \class history::dialogues::ChangeName
 *   A simple command for changing name of dialogue
 */
class ChangeName: public history::Command
{
public:
    /*! Creates new command
        \param[in] dialogue dialogue
        \param[in] old_value old value for dialogue
        \param[in] new_value new value for dialogue
     */
    ChangeName(
        sad::dialogue::Dialogue* dialogue, 
        const sad::String& old_value, 
        const sad::String& new_value
    );
    /*! Could be inherited
     */
    virtual ~ChangeName() override;
    /*! Makes way active, adds it to list
        \param[in] ob an observer for looking for command
     */
    virtual void commit(core::Editor * ob = nullptr) override;
    /*! Makes way inactive, removes it from list
        \param[in] ob an observer for looking for command
     */
    virtual void rollback(core::Editor * ob = nullptr) override;
protected:
    /*! Updates UI elements for command
        \param[in] e editor
        \param[in] str string for editor
     */
    void updateUI(core::Editor* e, const sad::String& str) const;
    /*! A current dialogue
     */
    sad::dialogue::Dialogue* m_dialogue;
    /*! Old value for name
     */
    sad::String m_old_value;
    /*! New value for name
     */
    sad::String m_new_value;
};

}

}
