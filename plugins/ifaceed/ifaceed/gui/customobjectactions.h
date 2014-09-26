/*! \file customobjectactions.h
	\author HiddenSeeker

	Describes a group of actions, linked to sprited
 */
#pragma once
#include <QObject>

#include <input/events.h>

class MainPanel;

namespace gui
{
/*! A group of actions, linked to custom objects
 */	
class CustomObjectActions: public QObject
{
Q_OBJECT
public:
	/*! Creates new actions for sprites
		\param[in] parent a parent object
	 */
	CustomObjectActions(QObject* parent = NULL);
	/*! This class could be inherited
	 */
	virtual ~CustomObjectActions();
	/*! Sets panel, where actions belong to
	 */
	void setPanel(MainPanel* e);
	/*! Returns panel, where actions belong to
	 */
	MainPanel* panel() const;	
private:
	/*! An panel, which actions are belong to
	 */
	MainPanel* m_panel;
};

}
