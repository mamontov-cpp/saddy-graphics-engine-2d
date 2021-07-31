/*! \file groupsaddinstance.h
    

    Describes a command, which performs adding an instance into animation group
 */
#pragma once
#include "../command.h"

#include <animations/animationsgroup.h>

namespace history
{
    
namespace groups
{
/*! A command, which performs adding an instance into animation group
 */
class AddInstance: public history::Command
{
public:
     /*! Constructs new command for node
         \param[in] d a node
         \param[in] i instance
         \param[in] pos position
      */
     AddInstance(
         sad::animations::Group* d,
         sad::animations::Instance* i,
         int pos
    );
     /*! Erases link to a node
      */
     virtual ~AddInstance() override;
     /*! Applies changes, described in command
         \param[in] ob an observer for looking for command
      */
     virtual void commit(core::Editor * ob = nullptr) override;
     /*! Reverts changes, described in command
         \param[in] ob an observer for looking for command
      */
     virtual void rollback(core::Editor * ob = nullptr) override;
protected:
    /*! Update an added instance, while committing
        \param[in] e editor
     */
    void updateUIWhileCommitting(core::Editor* e);
    /*! Update an added instance, while rolling back
        \param[in] e editor
     */
    void updateUIWhileRollingBack(core::Editor* e);
    /*! A node, which must be added to an item
     */
    sad::animations::Group* m_node;
    /*! A selected instance to be added
     */
    sad::animations::Instance* m_instance;
    /*! An insertion position
     */
    int m_position;
};

}

}
