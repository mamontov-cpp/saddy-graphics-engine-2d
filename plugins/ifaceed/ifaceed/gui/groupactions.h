/*! \file groupactions.h
	\author HiddenSeeker

	Describes a group of actions, linked to instances
 */
#pragma once
#include <QObject>

#include <animations/animationsgroup.h>


class MainPanel;

namespace gui
{

/*! A group of actions, linked to animation instances
 */	
class GroupActions: public QObject
{
Q_OBJECT
public:
	/*! Creates new label actions
		\param[in] parent a parent object
	 */
	GroupActions(QObject* parent = NULL);
	/*! This class could be inherited
	 */
	virtual ~GroupActions();
	/*! Sets panel, where actions belong to
	 */
	void setPanel(MainPanel* e);
	/*! Returns panel, where actions belong to
	 */
	MainPanel* panel() const;
public slots:
	/*! Tries to add new animation group
	 */
	void addGroup();
	/*! Current changed group
		\param[in] newrow new row
	 */ 
	void currentGroupChanged(int newrow);
	/*! Tries to remove animation group
	 */
	void removeGroup();
	/*!  Called, when user changes name of instance
		\param[in] name a name of instance
	 */
	void nameChanged(const QString& name);
	/*! Called, when user changes "Whether animation group is looped" flag
		\param[in] newvalue a new value
	 */
	void loopedChanged(bool newvalue);
private:
	/*! An panel, which actions are belong to
	 */
	MainPanel* m_panel;
};

}
