/*! \file dialoguesremove.h
 *  
 *
 * Describes a simple command for removing a dialogue
 */
#pragma once
#include "../command.h"
#include <dialogue/dialogue.h>

namespace history
{

namespace dialogues
{

/*! \class history::dialogues::Remove
 *   A simple command for removing a new way to list
 */
class Remove: public history::Command
{
public:
    /*! Creates new command
        \param[in] w dialogue
        \param[in] pos a position of way in source list
     */
    Remove(sad::dialogue::Dialogue* w, int pos);
    /*! Could be inherited
     */
    virtual ~Remove() override;
    /*! Makes way inactive, removes it from list
        \param[in] ob an observer for looking for command
     */
    virtual void commit(core::Editor * ob = nullptr) override;
    /*! Makes way active, adds it to list
        \param[in] ob an observer for looking for command
     */
    virtual void rollback(core::Editor * ob = nullptr) override;
protected:
    /*! A removed dialogue
     */
    sad::dialogue::Dialogue* m_d;
    /*! A position of item in list
     */
    int m_position;
};

}

}
