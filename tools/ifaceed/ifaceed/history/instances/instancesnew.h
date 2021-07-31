/*! \file instancesnew.h
    

    Describes a command, which performs creating new animation instance
 */
#pragma once
#include "../command.h"

#include <animations/animationsinstance.h>

#include <QListWidget>

namespace history
{
    
namespace instances
{
/*! A command, which performs creating new animation instance
 */
class New: public history::Command
{
public:
     /*! Constructs new command for node
        \param[in] d a node
      */
     New(sad::animations::Instance* d);
     /*! Erases link to a node
      */
     virtual ~New();
     /*! Applies changes, described in command
         \param[in] ob an observer for looking for command
      */
     virtual void commit(core::Editor * ob = nullptr) override;
     /*! Reverts changes, described in command
         \param[in] ob an observer for looking for command
      */
     virtual void rollback(core::Editor * ob = nullptr) override;
protected:
    /*! Erases last item from list
     */
    void eraseFromList(QListWidget* list);
    /*! A node, which must be added to an item
     */
    sad::animations::Instance* m_node;
};

}

}
