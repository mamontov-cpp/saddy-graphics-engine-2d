/*! \file instanceactions.h
	\author HiddenSeeker

	Describes a group of actions, linked to instances
 */
#pragma once
#include <QObject>
#include <QRectF>

#include <sadhash.h>
#include <animations/animationscomposite.h>
#include <animations/animationswayinstance.h>

#include "animationinstanceprocess.h"

class MainPanel;

namespace gui
{

class InstanceProcess;

/*! A group of actions, linked to animation instances
 */	
class InstanceActions: public QObject
{
Q_OBJECT
public:
	/*! Creates new label actions
		\param[in] parent a parent object
	 */
	InstanceActions(QObject* parent = NULL);
	/*! This class could be inherited
	 */
	virtual ~InstanceActions();
	/*! Sets panel, where actions belong to
	 */
	void setPanel(MainPanel* e);
	/*! Returns panel, where actions belong to
	 */
	MainPanel* panel() const;
	/*! Updates instance list for groups
	 */
	void updateGroupInstanceList();
	/*! Updates animation for current instance
		\param[in] a instance
	 */
	void updateCurrentInstanceAnimation(sad::animations::Instance* a);
public slots:
	/*! Tries to add new animation instance
	 */
	void addInstance();
	/*! Tries to remove animation instance
	 */
	void removeInstance();
	/*! Called, when current instance is changed
	 */
	void currentInstanceChanged(int row);
	/*!  Called, when user changes name of instance
		\param[in] name a name of instance
	 */
	void nameChanged(const QString& name);
	/*! Called, when user changes state for tree link
		\param[in] state a state of button
	 */
	void treeLinkStateChanged(bool state);
	/*! Called, when user changes state for database link
		\param[in] state a state of link
	 */
	void databaseLinkStateChanged(bool state);
	/*! Called, whether tree element changed
		\param[in] newrow a row 
	 */
	void treeElementChanged(int newrow);
	/*! Called, whether database element changed
		\param[in] newrow a row 
	 */
	void databaseElementChanged(int newrow);
	/*! Called, when object element changed
		\param[in] newrow a row 
	 */
	void objectChanged(int newrow);
	/*! Called, when start time changed
		\param[in] newvalue new value
	 */
	void startTimeChanged(double newvalue);
	/*! Called, when object element changed
		\param[in] newrow a row 
	 */
	void wayChanged(int newrow);
	/*! Starts an instance
	 */
	void start();
	/*! Stops an instance 
	 */
	void stop();
private:
	/*! An panel, which actions are belong to
	 */
	MainPanel* m_panel;
	/*! A running animation process 
	 */
	gui::AnimationInstanceProcess* m_animation;
};

}
