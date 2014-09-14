/*! \file labelactions.h
	\author HiddenSeeker

	Describes a group of actions, linked to label
 */
#pragma once
#include <QObject>

#include <input/events.h>

class MainPanel;

namespace gui
{
/*! A group of actions, linked to labels
 */	
class LabelActions: public QObject
{
Q_OBJECT
public:
	/*! Creates new label actions
		\param[in] parent a parent object
	 */
	LabelActions(QObject* parent = NULL);
	/*! This class could be inherited
	 */
	virtual ~LabelActions();
	/*! Sets panel, where actions belong to
	 */
	void setPanel(MainPanel* e);
	/*! Returns panel, where actions belong to
	 */
	MainPanel* panel() const;
	/*! Updates panel's region for active label
	 */
	void updateRegionForLabel();
	/*! Cancels adding label state, enters previous state
	 */
	void cancelAddLabel();
	/*! Moves label to a mouse position
		\param[in] e event
	 */
	void moveLabel(const sad::input::MouseMoveEvent& e);
	/*! Commits label adding to a scene
		\param[in] e event
	 */
	void commitLabelAdd(const sad::input::MousePressEvent& e);
public slots:
	/*! Adds new label to a scene, if all parameters in form are valid
	 */
	void addLabel();
private:
	/*! An panel, which actions are belong to
	 */
	MainPanel* m_panel;
};

}
