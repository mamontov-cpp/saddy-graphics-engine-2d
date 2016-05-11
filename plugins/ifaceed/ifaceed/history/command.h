/** \file command.h
    

    Describes an abstract command for history
 */
#pragma once
#include <stdlib.h>

namespace core
{
class Editor;
}


namespace history
{
/*! All history consists of those commands. A command must describe one user action
 */
class Command
{
public:
    /*! Constructs default command
     */
    Command();
    /*! Applies changes, described in command
        \param[in] ob an observer for looking for command
     */
    virtual void commit(core::Editor * ob = NULL)=0;
    /*! Reverts changes, described in command
         \param[in] ob an observer for looking for command
     */
    virtual void rollback(core::Editor * ob = NULL)=0;
    /*! Must be inherited
     */
    virtual ~Command();
    /*! Commits a change without updating UI. Useful, when command should not touch
        a user interface, see https://github.com/mamontov-cpp/saddy-graphics-engine-2d/issues/55 for 
        reason, why this function exists.

        By default it's a stub for calling a commit function.

        \param[in] e editor
     */
    virtual void commitWithoutUpdatingUI(core::Editor * e);
};

}
