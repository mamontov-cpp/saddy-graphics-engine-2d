/*! \file dialogueactions.h
    

    Describes a group of actions, linked to dialogues
 */
#pragma  once

#include <QObject>

#include <input/events.h>
#include <dialogue/dialogue.h>

#include "abstractactions.h"

namespace gui
{

namespace actions
{

/*! A group of actions, linked to dialogues
 */	
class DialogueActions: public QObject, public gui::actions::AbstractActions
{
Q_OBJECT
public:
    /*! Creates new dialogue actions
        \param[in] parent a parent object
     */
    DialogueActions(QObject* parent = nullptr);
    /*! This class could be inherited
     */
    virtual ~DialogueActions();
    /*! Updates UI with dialogue properties
        \param[in] d dialogue
     */
    void viewDialogue(sad::dialogue::Dialogue* d);
    /*! Toggles UI with phrase properties
     */
    void viewPhrase(sad::dialogue::Phrase* p);
    /*! Adds a command if needed, that phrase text is changed
        \param[in] d dialogue
        \param[in] pos position of phrase
        \param[in] new_value new value
        \param[in] from_editor whether it's done from editor
     */
    void changePhraseText(
        sad::dialogue::Dialogue* d, 
        int pos, 
        const sad::String& new_value, 
        bool from_editor
    );
    /*! Adds a command if needed, that phrase duration is changed
        \param[in] d dialogue
        \param[in] pos position of phrase
        \param[in] new_value new value
        \param[in] from_editor whether it's done from editor
     */
    void changePhraseDuration(
        sad::dialogue::Dialogue* d, 
        int pos, 
        double new_value, 
        bool from_editor
    );
    /*! Adds a command if needed, that phrase actor name is changed
        \param[in] d dialogue
        \param[in] pos position of phrase
        \param[in] new_value new value
        \param[in] from_editor whether it's done from editor
     */
    void changePhraseActorName(
        sad::dialogue::Dialogue* d, 
        int pos, 
        const sad::String& new_value, 
        bool from_editor
    );
    /*! Adds a command if needed, that phrase actor portrait is changed
        \param[in] d dialogue
        \param[in] pos position of phrase
        \param[in] new_value new value
        \param[in] from_editor whether it's done from editor
     */
    void changePhraseActorPortrait(
        sad::dialogue::Dialogue* d, 
        int pos, 
        const sad::String& new_value, 
        bool from_editor
    );
    /*! Adds a command if needed, that phrase view hint is changed
        \param[in] d dialogue
        \param[in] pos position of phrase
        \param[in] new_value new value
        \param[in] from_editor whether it's done from editor
     */
    void changePhraseViewHint(
        sad::dialogue::Dialogue* d, 
        int pos, 
        const sad::String& new_value, 
        bool from_editor
    );
    /*! Removes a dialogue from database
        \param[in] d dialogue
        \param[in] from_editor whether it's being removed from editor
        \param[in] row a row, where is dialogue located (-1 for custom)
     */
    void removeDialogueFromDatabase(
        sad::dialogue::Dialogue* d,
        bool from_editor,
        int row = -1
    );
    /*! Adds last dialogue to end of items
        \param[in] dialogue a dialogue
     */
    void addDialogueToDialogueList(sad::dialogue::Dialogue* dialogue);
    /*! Removes last dialogue from dialogue list
     */
    void removeLastDialogueFromDialogueList();
    /*! Inserts dialogue to a dialogue list
        \param[in] s dialogue
        \param[in] position a position in dialogue list
     */
    void insertDialogueToDialogueList(sad::dialogue::Dialogue* s, int position);
    /*! Removes dialogue from a dialogue list
        \param[in] position a position, where dialogue must be removed
     */
    void removeDialogueFromDialogueList(int position);
    /*! Removes way from a way list
        \param[in] s dialogue, to be removed
     */
    void removeDialogueFromDialogueList(sad::dialogue::Dialogue* s);
    /*! Finds dialogue in dialogue list
        \param[in] s scene
        \return scene row (-1 if not found)
     */
    int findDialogueInList(sad::dialogue::Dialogue* s);
    /*!  Updates dialogue in list
        \param[in] s a way
     */
    void updateDialogueName(sad::dialogue::Dialogue* s);
    /*! Removes phrase from a phrase list
        \param[in] row a row
     */
    void removePhraseFromPhraseList(int row);
    /*! Returns viewable name for a phrase
        \param[in] p phrase
        \return name for phrase
     */
    QString nameForPhrase(const sad::dialogue::Phrase& p) const;
public slots:
    /*! Called, when user clicks on button for adding a dialogue
     */
    void addDialogue();
    /*! Called, when user clicks on button for removing a dialogue
     */
    void removeDialogue();
    /*! Called, when user clicks on button for adding a phrase
     */
    void addPhrase();
    /*! Called, when user clicks on button for removing a phrase
     */
    void removePhrase();
    /*! Moves phrase earlier to timeline
     */
    void movePhraseBack();
    /*! Moves phrase later to timeline
     */
    void movePhraseFront();
    /*! Called, when dialogue name is edited
        \param[in] name a new name
     */
    void nameEdited(const QString& name);
    /*! Called, when a way changed in list
        \param[in] i new row index
     */
    void dialogueChanged(int i);
    /*! Called, when a phrase changed in list
        \param[in] i new row index
     */
    void phraseChanged(int i);
    /*! Called, when text of phrase is changed
     */
    void phraseTextChanged();
    /*! Called, when user changes duration
        \param[in] new_value new value for duration
     */
    void durationChanged(double new_value);
    /*! Called, when user changes actor name
        \param[in] new_value new value for actor name
     */
    void actorNameChanged(const QString& new_value);
    /*! Called, when user changes actor portrait
        \param[in] new_value new value for actor portrait
     */
    void actorPortraitChanged(const QString& new_value);
    /*! Called, when user changes view hint
        \param[in] new_value new value for view hint
     */
    void viewHintChanged(const QString& new_value);
};

}

}
