/*! \file history.h
    

    Describes an editor history
 */
#pragma once
#include <QMutex>

#include <sadlinkedlist.h>

#include "command.h"

namespace history
{

/*! A history of all saved changes
 */
class History
{
public:
    /*! Constructs empty history	
     */
    History();
    /*! Frees memory from all commands
     */
    ~History();
    /*! Add new command to history
        \param[in] c command
     */
    void add(history::Command * c);
    /*! Commits changes, moving to next non-applied command
     */
    void commit(core::Editor * ob = nullptr);
    /*! Rollbacks changed, moving to previous command
     */
    void rollback(core::Editor * ob = nullptr);
    /*! Clears a history
     */
    void clear();
private:
    /*! A saved commands
     */
    sad::LinkedList<history::Command *> m_commands;
    /*! Current saved command
     */
    int  m_current;
    /*! A lock for reading and writing
     */
    QMutex m_lock;
};

}
