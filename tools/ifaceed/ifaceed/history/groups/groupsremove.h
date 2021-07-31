/*! \file groupsremove.h
    

    Describes a command, which performs removing animation group
 */
#pragma once
#include "../command.h"

#include <animations/animationsgroup.h>

namespace history
{
    
namespace groups
{
/*! A command, which performs removing animation group
 */
class Remove: public history::Command
{
public:
     /*! Constructs new command for node
         \param[in] d a node
         \param[in] position a position for node
      */
     Remove(sad::animations::Group* d, int position);
     /*! Erases link to a node
      */
     virtual ~Remove() override;
     /*! Applies changes, described in command
         \param[in] ob an observer for looking for command
      */
     virtual void commit(core::Editor * ob = nullptr) override;
     /*! Reverts changes, described in command
         \param[in] ob an observer for looking for command
      */
     virtual void rollback(core::Editor * ob = nullptr) override;
protected:
    /*! A node, which must be added to an item
     */
    sad::animations::Group* m_node;
    /*! A position for node
     */
    int m_position;
};

}

}
