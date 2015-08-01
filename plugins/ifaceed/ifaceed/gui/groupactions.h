/*! \file groupactions.h
	

	Describes a group of actions, linked to instances
 */
#pragma once
#include <QObject>

#include "animationgroupprocess.h"

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
	/*! Removes group from database
		\param[in] g group
		\param[in] fromeditor whether group is being editor
		\param[in] row a row (-1 if function should determine it)
	 */
	void removeFromDatabase(
		sad::animations::Group* g,
		bool fromeditor,
		int row = -1
	);
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
	/*! Tries to add instance to an animation group
	 */
	void addInstance();
	/*! Tries to remove instance from an animation group
	 */
	void removeInstance();
	/*! Starts running selected animation group
	 */
	void start();
	/*! Stop running selected animation group
	 */
	void stop();
private:
	/*! An panel, which actions are belong to
	 */
	MainPanel* m_panel;
	/*! A running animation process 
	 */
	gui::AnimationGroupProcess* m_animation;
};

}
