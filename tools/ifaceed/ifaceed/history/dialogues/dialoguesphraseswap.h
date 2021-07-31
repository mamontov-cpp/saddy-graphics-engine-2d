/*! \file dialoguesphraseswap.h
 *  
 *
 * Describes a simple command for swapping two phrases
 */
#pragma once
#include "../command.h"
#include <dialogue/dialogue.h>

namespace history
{

namespace dialogues
{

/*! \class history::dialogues::PhraseSwap
 *   A simple command for swapping two phrases
 */
class PhraseSwap: public history::Command
{
public:
    /*! Creates new command
        \param[in] dialogue dialogue
        \param[in] p1 a first position
        \param[in] p2 a second position
     */
    PhraseSwap(sad::dialogue::Dialogue* dialogue, int p1, int p2);
    /*! Could be inherited
     */
    virtual ~PhraseSwap();
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
    /*! A first position
     */
    int m_position1;
    /*! A second position
     */
    int m_position2;	
};

}

}
