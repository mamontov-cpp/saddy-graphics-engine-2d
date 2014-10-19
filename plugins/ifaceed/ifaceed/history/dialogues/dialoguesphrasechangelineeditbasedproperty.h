/*! \file dialoguesphrasechangelineeditbasedproperty.h
 *  \author HiddenSeeker
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
		\param[in] oldvalue old value 
		\param[in] newvalue new value 
     */
	PhraseChangeLineEditBasedProperty(
		QLineEdit* widget,
		void  (sad::dialogue::Phrase::*setter)(const sad::String&),
		bool update,
		sad::dialogue::Dialogue* dialogue, 
		int i,
		const sad::String& oldvalue, 
		const sad::String& newvalue
	);
    /*! Could be inherited
     */
    virtual ~PhraseChangeLineEditBasedProperty();
    /*! Makes way active, adds it to list
        \param[in] ob an observer for looking for command
     */
    virtual void commit(core::Editor * ob = NULL);
    /*! Makes way inactive, removes it from list
        \param[in] ob an observer for looking for command
     */
    virtual void rollback(core::Editor * ob = NULL);
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
	sad::String m_oldvalue;
	/*! New value for name
	 */
	sad::String m_newvalue;
	/*! Tries to update UI with new value
		\param[in] e editor
		\param[in] v value
	 */
	void tryUpdateUI(core::Editor* e, const sad::String& v);
};

}

}
