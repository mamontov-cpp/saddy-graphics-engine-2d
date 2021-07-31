/** \file batchcommand.h
    

    Describes a complex command, which can consist of several commands an apply all of them at the same time
    when committing
 */
#pragma once

#include "command.h"

#include <sadvector.h>

namespace core
{
class Editor;
}


namespace history
{
/*! A batch command, which can apply a lot of operations at the same time
 */
class BatchCommand: public history::Command
{
public:
    /*! Creates new empty batch command
     */
    BatchCommand();
    /*! Must be inherited
    */
    virtual ~BatchCommand();
    /*! Applies changes, described in command
        \param[in] ob an observer for looking for command
    */
    virtual void commit(core::Editor * ob = nullptr) override;
    /*! Reverts changes, described in command
        \param[in] ob an observer for looking for command
    */
    virtual void rollback(core::Editor * ob = nullptr) override;
    /*! Adds a command to batch
        \param[in] c command
     */
    void add(history::Command* c);
    /*! Removes a command from batch
        \param[in] c command
     */
    void remove(history::Command* c);
    /*! Returns count of commands inside of editor
        \return count of commands
     */
    size_t count() const;
protected:
    /*! A list of commands to be applied
     */
    sad::Vector<history::Command*> m_commands;
};

}
