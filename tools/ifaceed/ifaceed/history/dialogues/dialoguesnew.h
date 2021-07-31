/*! \file dialoguesnew.h
 *  
 *
 * Describes a simple command for adding a dialogue
 */
#pragma once
#include "../command.h"
#include <dialogue/dialogue.h>

namespace history
{

namespace dialogues
{

/*! \class history::dialogues::New
 *   A simple command for adding a new dialogues to list
 */
class New: public history::Command
{
public:
    /*! Creates new command
        \param[in] dialogue dialogue
     */
    New(sad::dialogue::Dialogue* dialogue);
    /*! Could be inherited
     */
    virtual ~New() override;
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
};

}

}
