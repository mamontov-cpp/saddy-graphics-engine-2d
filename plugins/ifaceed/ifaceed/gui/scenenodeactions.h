/*! \file scenenodeactions.h
	\author HiddenSeeker

	Describes a group of actions, linked to scene node
 */
#pragma once
#include <QObject>

class MainPanel;

namespace gui
{
/*! A group of actions, linked to scene nodes
 */
class SceneNodeActions: public QObject
{
Q_OBJECT
public:
	/*! Creates new action for scene nodes
		\param[in] parent a parent object
	 */
	SceneNodeActions(QObject* parent = NULL);
	/*! This class could be inherited
	 */
	virtual ~SceneNodeActions();
	/*! Sets panel, where actions belong to
	 */
	void setPanel(MainPanel* e);
	/*! Returns panel, where actions belong to
	 */
	MainPanel* panel() const;
public slots:
	/*! Called, when node name is edited
		\param[in] name a name for action
	 */
	void nameEdited(const QString& name);
private:
	/*! An panel, which actions are belong to
	 */
	MainPanel* m_panel;
};

}
