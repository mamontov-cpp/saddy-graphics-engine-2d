/*! \file instancesnew.h
    

    Describes a command, which performs removing animation instance
 */
#pragma once
#include "../command.h"

#include <animations/animationsinstance.h>
#include <animations/animationsgroup.h>

#include <sadpair.h>
#include <sadvector.h>

#include <QListWidget>

namespace history
{
    
namespace instances
{
/*! A command, which performs removing animation instance
 */
class Remove: public history::Command
{
public:
     /*! Constructs new command for node
        \param[in] d a node
        \param[in] position a position in a main list
        \param[in] dependent_groups a list of dependent groups
      */
     Remove(
         sad::animations::Instance* d,
         int position,
         const sad::Vector<sad::Pair<sad::animations::Group*, int> >& dependent_groups
    );
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
    /*! Removes an item from a list
        \param[in] widget a widget to be inserted
     */
    void removeItem(QListWidget* widget);
    /*! Inserts an item to a list
        \param[in] widget a widget to be inserted
        \param[in] name name for an item
     */
    void insertItem(QListWidget* widget, const QString& name);
    /*! A node, which must be added to an item
     */
    sad::animations::Instance* m_node;
    /*! A position in a main list
     */ 
    int m_position_in_main_list;
    /*! A dependent groups for instance
     */
    sad::Vector<sad::Pair<sad::animations::Group*, int> > m_dependent_groups;
};

}

}
