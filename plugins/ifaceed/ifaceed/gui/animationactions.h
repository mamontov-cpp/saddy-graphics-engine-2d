/*! \file animationactions.h
	\author HiddenSeeker

	Describes a group of actions, linked to animations
 */
#pragma once
#include <QObject>

#include <input/events.h>

class MainPanel;

namespace gui
{
/*! A group of actions, linked to animations
 */	
class AnimationActions: public QObject
{
Q_OBJECT
public:
	/*! Creates new label actions
		\param[in] parent a parent object
	 */
	AnimationActions(QObject* parent = NULL);
	/*! This class could be inherited
	 */
	virtual ~AnimationActions();
	/*! Sets panel, where actions belong to
	 */
	void setPanel(MainPanel* e);
	/*! Returns panel, where actions belong to
	 */
	MainPanel* panel() const;
public slots:
	/*! Tries to add new animation to a list
	 */
	void addAnimation();
	/*! Called, when user changes animation in list
		\param[in] row a new selected row in list
	 */
	void currentAnimationChanged(int row);
	/*! Called, when user changes name of animation
		\param[in] name a name of animation
	 */
	void nameChanged(const QString& name);
	/*! Called, when user changes time of animation
		\param[in] time a time of animation
	 */
	void timeChanged(double time);
	/*! Called, when user changes "Whether animation is looped" flag
		\param[in] newvalue a new value
	 */
	void loopedChanged(bool newvalue);
	/*! Called, when user changes frequency of blinking of animation
		\param[in] nvalue a value
	 */
	void blinkingFrequencyChanged(int nvalue);
private:
	/*! An panel, which actions are belong to
	 */
	MainPanel* m_panel;
};

}
