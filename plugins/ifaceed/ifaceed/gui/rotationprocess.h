/*! \file rotationprocess.h
	\author HiddenSeeker

	Describes a simple model of rotation process, which delays adding of angle change
	for scene node to history
 */
#pragma once
#include <QObject>
#include <QTimer>

#include <scenenode.h>

namespace history
{
class History;	
}

namespace gui
{
	
/*! Defines a simple model of process of rotation, which delays adding of angle change
	for scene node to history
 */
class RotationProcess:public QObject
{
Q_OBJECT
public:
	/*! Creates new rotation process
	 */
	RotationProcess();
	/*! Could be inherited
	 */
	virtual ~RotationProcess();
	/*! Starts new rotation process
		\param[in] node a node
		\param[in] oldvalue an old value for process
		\param[in] newvalue a new value for process
	 */
	void start(sad::SceneNode* node, float oldvalue, float newvalue);
	/*! Sets a history for rotation process
		\param[in] h history
	 */
	void setHistory(history::History* h);
    /*! Defines a timeout for rotation process in milliseconds
	 */
	static const int TIMEOUT; 
public slots:
	/*! Called, when timer is expired, makes process add angle change for
		scene node to history
	 */
	void timerExpired();
protected:
	/*! A timer, which determines, whether rotation process is pending
	 */
	QTimer m_timer;
	/*! Whether rotation process is pending
	 */
	bool m_pending;
	/*! A linked history
	 */
	history::History* m_history;
	/*! A pending node
	 */
	sad::SceneNode* m_node;
	/*! An old value for angle
	 */
	bool m_oldvalue;
	/*! A new value for angle
	 */
	bool m_newvalue;
};

}
