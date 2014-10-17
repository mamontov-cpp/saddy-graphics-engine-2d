/*! \file dialogueactions.h
	\author HiddenSeeker

	Describes a group of actions, linked to dialogues
 */
#pragma  once

#include <QObject>

#include <input/events.h>

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
public slots:
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
