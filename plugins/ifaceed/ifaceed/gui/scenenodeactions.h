/*! \file scenenodeactions.h
	\author HiddenSeeker

	Describes a group of actions, linked to scene node
 */
#pragma once
#include <QObject>
#include <QColor>
#include <QRectF>

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
	/*! Called, when user changes visibility of node
		\param[in] state a state
	 */
	void visibilityChanged(bool state);
    /*!
     * Called, when user changes color of node
     * \param[in] newcolor a new color
     */
    void colorChanged(QColor newcolor);
    /*!
     * Called, when uuser changes area of node
     * \param newarea
     */
    void areaChanged(QRectF newarea);
private:
	/*! An panel, which actions are belong to
	 */
	MainPanel* m_panel;
};

}
