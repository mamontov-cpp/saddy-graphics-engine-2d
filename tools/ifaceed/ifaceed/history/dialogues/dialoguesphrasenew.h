/*! \file dialoguesphrasenew.h
 *  
 *
 * Describes a simple command for adding a new phrase
 */
#pragma once
#include "../command.h"
#include <dialogue/dialogue.h>

namespace history
{

namespace dialogues
{

/*! \class history::dialogues::PhraseNew
 *   A simple command for adding a new dialogues to list
 */
class PhraseNew: public history::Command
{
public:
    /*! Creates new command
        \param[in] dialogue dialogue
        \param[in] p phrase
     */
    PhraseNew(sad::dialogue::Dialogue* dialogue, const sad::dialogue::Phrase& p);
    /*! Could be inherited
     */
    virtual ~PhraseNew();
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
    /*! A phrase to be added into dialogue
     */
    sad::dialogue::Phrase m_phrase;
};

}

}
