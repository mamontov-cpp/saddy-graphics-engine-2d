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
private:
	/*! An panel, which actions are belong to
	 */
	MainPanel* m_panel;
};

}
