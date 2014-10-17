/*! \file dialogueactions.h
	\author HiddenSeeker

	Describes a group of actions, linked to dialogues
 */
#pragma  once

#include <QObject>

#include <input/events.h>
#include <dialogue/dialogue.h>

class MainPanel;

namespace gui
{
/*! A group of actions, linked to dialogues
 */	
class DialogueActions: public QObject
{
Q_OBJECT
public:
	/*! Creates new dialogue actions
		\param[in] parent a parent object
	 */
	DialogueActions(QObject* parent = NULL);
	/*! This class could be inherited
	 */
	virtual ~DialogueActions();
	/*! Sets panel, where actions belong to
	 */
	void setPanel(MainPanel* e);
	/*! Returns panel, where actions belong to
	 */
	MainPanel* panel() const;
	/*! Updates UI with dialogue properties
		\param[in] d dialogue
	 */
	void viewDialogue(sad::dialogue::Dialogue* d);
	/*! Toggles UI with phrase properties
	 */
	void viewPhrase(sad::dialogue::Phrase* p);
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
private:
	/*! An panel, which actions are belong to
	 */
	MainPanel* m_panel;
};

}
