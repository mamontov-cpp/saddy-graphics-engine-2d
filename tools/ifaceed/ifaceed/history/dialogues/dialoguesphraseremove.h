/*! \file dialoguesphraseremove.h
 *  
 *
 * Describes a simple command for removing a phrase
 */
#pragma once
#include "../command.h"
#include <dialogue/dialogue.h>

namespace history
{

namespace dialogues
{

/*! \class history::dialogues::PhraseRemove
 *   A simple command for adding a new dialogues to list
 */
class PhraseRemove: public history::Command
{
public:
    /*! Creates new command
        \param[in] dialogue dialogue
        \param[in] position  position of phrase in dialogue
     */
    PhraseRemove(sad::dialogue::Dialogue* dialogue, int position);
    /*! Could be inherited
     */
    virtual ~PhraseRemove();
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
    /*! A position for phrase
     */
    int m_position;
    /*! A phrase to be added into dialogue
     */
    sad::dialogue::Phrase m_phrase;
};

}

}
