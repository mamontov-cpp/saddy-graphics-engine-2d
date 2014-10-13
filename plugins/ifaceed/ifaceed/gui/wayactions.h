/*! \file wayactions.h
	\author HiddenSeeker

	Describes a group of actions, linked to ways
 */
#pragma  once

#include <QObject>

#include <input/events.h>

class MainPanel;

namespace gui
{
/*! A group of actions, linked to ways
 */	
class WayActions: public QObject
{
Q_OBJECT
public:
	/*! Creates new wat actions
		\param[in] parent a parent object
	 */
	WayActions(QObject* parent = NULL);
	/*! This class could be inherited
	 */
	virtual ~WayActions();
	/*! Sets panel, where actions belong to
	 */
	void setPanel(MainPanel* e);
	/*! Returns panel, where actions belong to
	 */
	MainPanel* panel() const;
public slots:
    /*! Adds new way to a list
     */
    void addWay();
    /*! Called, when a way changed in list
     */
    void wayChanged(int i);
private:
	/*! An panel, which actions are belong to
	 */
	MainPanel* m_panel;
};

}
