/*! \file instanceschangeanimation.h
    

    Describes a command, which performs changing animation for instance
 */
#pragma once
#include "../command.h"

#include <maybe.h>
#include <sadstring.h>

#include <animations/animationsinstance.h>

#include <QRadioButton>

namespace history
{
    
namespace instances
{
/*! A command, which performs changing animation
 */
class ChangeAnimation: public history::Command
{
public:
     /*! Constructs new command for node
        \param[in] d a node
        \param[in] button a button to be checked
        \param[in] old_button an old button to be unchecked
        \param[in] old_name an old name for animation
        \param[in] old_id a major id for animation
        \param[in] new_name a new name for animation
        \param[in] new_id a new major id for animation
      */
     ChangeAnimation(
         sad::animations::Instance* d,
         QRadioButton* button,
         QRadioButton* old_button,
         const sad::String& old_name,
         unsigned long long old_id,
         const sad::String& new_name,
         unsigned long long new_id
     );
     /*! Constructs new command for node
        \param[in] d a node
        \param[in] old_name an old name for animation
        \param[in] new_name a new name for animation
      */
     ChangeAnimation(
         sad::animations::Instance* d,
         const sad::String& old_name,
         const sad::String& new_name
     );
     /*! Constructs new command for node
        \param[in] d a node
        \param[in] old_id an old major id for animation
        \param[in] new_id a new major id for animation
      */
     ChangeAnimation(
         sad::animations::Instance* d,
         unsigned long long old_id,
         unsigned long long new_id
     );
     /*! Erases link to a node
      */
     virtual ~ChangeAnimation();
     /*! Applies changes, described in command
         \param[in] ob an observer for looking for command
      */
     virtual void commit(core::Editor * ob = nullptr) override;
     /*! Reverts changes, described in command
         \param[in] ob an observer for looking for command
      */
     virtual void rollback(core::Editor * ob = nullptr) override;
protected:
    /*! Updates tree combo with specific value
        \param[in] e editor
        \param[in] value a value for combo
     */
    void updateTreeComboValue(core::Editor* e, const sad::String& value);
    /*! Updates tree combo with specific value
        \param[in] e editor
        \param[in] value a value for combo
     */
    void updateDatabaseComboValue(core::Editor* e, unsigned long long value);	
    /*! A node, which must be added to an item
     */
    sad::animations::Instance* m_node;
    /*! A button to be checked in case of changing data
     */
    QRadioButton* m_button;
    /*! An old button to be checked in case of changing data
     */
    QRadioButton* m_old_button;
    /*! An old name for tree animation
     */
    sad::Maybe<sad::String> m_maybe_old_name;
    /*! An old id for database animation
     */
    sad::Maybe<unsigned long long> m_maybe_old_id;
    /*! A new name for tree animation
     */
    sad::Maybe<sad::String> m_maybe_new_name;
    /*! An new id for database animation
     */
    sad::Maybe<unsigned long long> m_maybe_new_id;
};

}

}
