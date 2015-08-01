/*! \file dialoguesphrasechangephrase.h
 *  
 *
 * Describes a simple command for changing text of phrase
 */
#pragma once
#include "../command.h"
#include <dialogue/dialogue.h>

namespace history
{

namespace dialogues
{

/*! \class history::dialogues::PhraseChangePhrase
 *   A simple command for changing text of phrase
 */
class PhraseChangePhrase: public history::Command
{
public:
    /*! Creates new command
        \param[in] dialogue dialogue
		\param[in] i position of phrase in dialogue
		\param[in] oldvalue old value 
		\param[in] newvalue new value 
     */
	PhraseChangePhrase(
		sad::dialogue::Dialogue* dialogue, 
		int i,
		const sad::String& oldvalue, 
		const sad::String& newvalue
	);
    /*! Could be inherited
     */
    virtual ~PhraseChangePhrase();
    /*! Makes way active, adds it to list
        \param[in] ob an observer for looking for command
     */
    virtual void commit(core::Editor * ob = NULL);
    /*! Makes way inactive, removes it from list
        \param[in] ob an observer for looking for command
     */
    virtual void rollback(core::Editor * ob = NULL);
protected:
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
