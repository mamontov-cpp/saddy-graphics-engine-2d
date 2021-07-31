/*! \file dialoguesphrasechangelineeditbasedproperty.h
 *  
 *
 * Describes a simple command for changing property of phrase, based on line edit
 */
#pragma once
#include "../command.h"
#include <dialogue/dialogue.h>
#include <QLineEdit>

namespace history
{

namespace dialogues
{

/*! \class history::dialogues::PhraseChangeLineEditBasedProperty
 *   A simple command for changing property of phrase, based on line edit
 */
class PhraseChangeLineEditBasedProperty: public history::Command
{
public:
    /*! Creates new command
        \param[in] widget a widget to be edited
        \param[in] setter a setter for property
        \param[in] update whether we should update list on commit/rollback
        \param[in] dialogue dialogue
        \param[in] i position of phrase in dialogue
        \param[in] old_value old value 
        \param[in] new_value new value 
     */
    PhraseChangeLineEditBasedProperty(
        QLineEdit* widget,
        void  (sad::dialogue::Phrase::*setter)(const sad::String&),
        bool update,
        sad::dialogue::Dialogue* dialogue, 
        int i,
        const sad::String& old_value, 
        const sad::String& new_value
    );
    /*! Could be inherited
     */
    virtual ~PhraseChangeLineEditBasedProperty();
    /*! Changes property to a new value, without updating property
        \param[in] ob editor
     */
    void commitWithoutUpdatingUI(core::Editor * ob = nullptr) override;
    /*! Changes property to a new value
        \param[in] ob editor
     */
    virtual void commit(core::Editor * ob = nullptr) override;
    /*! Changed property to an old value
        \param[in] ob editor
     */
    virtual void rollback(core::Editor * ob = nullptr) override;
protected:
    /*! A widget to be updated
     */
    QLineEdit* m_widget;
    /*! Setter
     */
    void  (sad::dialogue::Phrase::*m_setter)(const sad::String&);
    /*! Whether we should update list on commit/rollback
     */
    bool m_update;
    /*! A current dialogue
     */
    sad::dialogue::Dialogue* m_dialogue;
    /*! A position of phrase in dialogue
     */
    int m_position;
    /*! Old value for name
     */
    sad::String m_old_value;
    /*! New value for name
     */
    sad::String m_new_value;
    /*! Tries to update UI with new value
        \param[in] e editor
        \param[in] v value
        \param[in] update_ui whether we should update ui
     */
    void tryUpdateUI(core::Editor* e, const sad::String& v, bool update_ui) const;
};

}

}
