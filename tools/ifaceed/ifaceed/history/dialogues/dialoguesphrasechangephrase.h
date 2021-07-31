/*! \file dialoguesphrasechangephrase.h
 *  
 *
 * Describes a simple command for changing text of phrase
 */
#pragma once
#include "../command.h"
#include <dialogue/dialogue.h>

namespace history
{

namespace dialogues
{

/*! \class history::dialogues::PhraseChangePhrase
 *   A simple command for changing text of phrase
 */
class PhraseChangePhrase: public history::Command
{
public:
    /*! Creates new command
        \param[in] dialogue dialogue
        \param[in] i position of phrase in dialogue
        \param[in] old_value old value 
        \param[in] new_value new value 
     */
    PhraseChangePhrase(
        sad::dialogue::Dialogue* dialogue, 
        int i,
        const sad::String& old_value, 
        const sad::String& new_value
    );
    /*! Could be inherited
     */
    virtual ~PhraseChangePhrase();
    /*! Makes way active, adds it to list
        \param[in] ob an observer for looking for command
     */
    virtual void commit(core::Editor * ob = nullptr) override;
    /*! Makes way inactive, removes it from list
        \param[in] ob an observer for looking for command
     */
    virtual void rollback(core::Editor * ob = nullptr) override;
protected:
    /*! A current dialogue
     */
    sad::dialogue::Dialogue* m_dialogue;
    /*! A position of phrase in dialogue
     */
    int m_position;
    /*! Old value for name
     */
    sad::String m_old_value;
    /*! New value for name
     */
    sad::String m_new_value;
    /*! Tries to update UI with new value
        \param[in] e editor
        \param[in] v value
     */
    void tryUpdateUI(core::Editor* e, const sad::String& v);
};

}

}
