/*! \file dialoguesphrasechangeduration.h
 *  
 *
 * Describes a simple command for changing duration of phrase
 */
#pragma once
#include "../command.h"
#include <dialogue/dialogue.h>
// ReSharper disable once CppUnusedIncludeDirective
#include <QLineEdit>

namespace history
{

namespace dialogues
{

/*! \class history::dialogues::PhraseChangeDuration
 *   A simple command for changing duration
 */
class PhraseChangeDuration: public history::Command
{
public:
    /*! Creates new command
        \param[in] dialogue dialogue
        \param[in] i position of phrase in dialogue
        \param[in] old_value old value 
        \param[in] new_value new value 
     */
    PhraseChangeDuration(
        sad::dialogue::Dialogue* dialogue, 
        int i,
        double old_value, 
        double new_value
    );
    /*! Could be inherited
     */
    virtual ~PhraseChangeDuration() override;
    /*! Makes way active, adds it to list
        \param[in] ob an observer for looking for command
     */
    virtual void commit(core::Editor * ob = nullptr) override;
    /*! Makes way inactive, removes it from list
        \param[in] ob an observer for looking for command
     */
    virtual void rollback(core::Editor * ob = nullptr) override;
protected:
    /*! A current dialogue
     */
    sad::dialogue::Dialogue* m_dialogue;
    /*! A position of phrase in dialogue
     */
    int m_position;
    /*! Old value for name
     */
    double m_old_value;
    /*! New value for name
     */
    double m_new_value;
    /*! Tries to update UI with new value
        \param[in] e editor
        \param[in] v value
     */
    void tryUpdateUI(core::Editor* e, double v);
};

}

}
