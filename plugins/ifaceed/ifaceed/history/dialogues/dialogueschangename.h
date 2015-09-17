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
        \param[in] oldvalue old value for dialogue
        \param[in] newvalue new value for dialogue
     */
    ChangeName(
        sad::dialogue::Dialogue* dialogue, 
        const sad::String& oldvalue, 
        const sad::String& newvalue
    );
    /*! Could be inherited
     */
    virtual ~ChangeName();
    /*! Makes way active, adds it to list
        \param[in] ob an observer for looking for command
     */
    virtual void commit(core::Editor * ob = NULL);
    /*! Makes way inactive, removes it from list
        \param[in] ob an observer for looking for command
     */
    virtual void rollback(core::Editor * ob = NULL);
protected:
    /*! A current dialogue
     */
    sad::dialogue::Dialogue* m_dialogue;
    /*! Old value for name
     */
    sad::String m_oldvalue;
    /*! New value for name
     */
    sad::String m_newvalue;
};

}

}
