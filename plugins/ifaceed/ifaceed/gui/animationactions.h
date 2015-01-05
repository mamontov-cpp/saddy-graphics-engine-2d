/*! \file animationactions.h
	\author HiddenSeeker

	Describes a group of actions, linked to animations
 */
#pragma once
#include <QObject>
#include <QRectF>

#include <input/events.h>

class MainPanel;

namespace gui
{

class AnimationProcess;

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
	/*! Starts a selected animation as instance on selected object
	 */
	void startOnObject();
	/*! Stops a selected animation as instance on selected object
	 */
	void stopOnObject();
	/*! Changes starting color for color animations
	 */
	void colorChangeStartingColor();
	/*! Changes ending color for color animations
	 */
	void colorChangeEndingColor();
	/*! Called, when resizing changes vector of resize coordinates
		for X coordinate
		\param[in] newvalue a new value
	 */
	void resizeChangeVectorX(double newvalue);
	/*! Called, when resizing changes vector of resize coordinates
		for Y coordinate
		\param[in] newvalue a new value
	 */
	void resizeChangeVectorY(double newvalue);
	/*! Called, when user changes starting angle for rotation angle
		\param[in] newvalue new value for angle
	 */ 
	void rotateChangeStartingAngle(double newvalue);
	/*! Called, when user changes ending angle for rotation angle
		\param[in] newvalue new value for angle
	 */ 
	void rotateChangeEndingAngle(double newvalue);
	/*! Called, when user changes way for way moving animation
		\param[in] row a row
	 */
	void wayMovingChangeWay(int row);
	/*! Called, when font list editing  finished (timed event, see implementation of
		gui::textedit::TextEdit for detail)
	 */
	void fontListEditingFinished();
	/*! Called, when font size starting size changed
		\param[in] newvalue new value
	 */
	void fontSizeChangeStartingSize(int newvalue);
	/*! Called, when font size ending size changed
		\param[in] newvalue new value
	 */
	void fontSizeChangeEndingSize(int newvalue);
	/*! Called, when option list editing  finished (timed event, see implementation of
		gui::textedit::TextEdit for detail)
	 */
	void optionListEditingFinished();
	/*! Called, when texture coordinates list editing  finished (timed event, see implementation of
		gui::textedit::TextEdit for detail)
	 */
	void textureCoordinatesListEditingFinished();
	/*! Called, when user changes starting rect for a value
		\param[in] value new value
	 */
	void textureCoordinatesChangeStartRect(QRectF value);
	/*! Called, when user changes ending rect for a value
		\param[in] value new value
	 */
	void textureCoordinatesChangeEndRect(QRectF value);
	/*! Called, when user changes x coordinate of camera rotation pivot 
		\param[in] newx new value
	 */
	void cameraRotationChangePivotX(double newx);
	/*! Called, when user changes y coordinate of camera rotation pivot 
		\param[in] newy new value
	 */
	void cameraRotationChangePivotY(double newy);
	/*! Called, when user changes starting angle of camera rotation animation
		\param[in] newvalue a new value for animation
	 */
	void cameraRotationChangeStartingAngle(double newvalue);
	/*! Called, when user changes ending angle of camera rotation animation
		\param[in] newvalue a new value for animation
	 */
	void cameraRotationChangeEndingAngle(double newvalue);
	/*! Called, when user changes x coordinate of camera shaking offset 
		\param[in] newx new value
	 */
	void cameraShakingChangeOffsetX(double newx);
	/*! Called, when user changes y coordinate of camera shaking offset
		\param[in] newy new value
	 */
	void cameraShakingChangeOffsetY(double newy);
	/*! Called, when user changes frequency of camera shaking
		\param[in] newvalue a new value
	 */
	void cameraShakingChangeFrequency(int newvalue);
private:
	/*! An panel, which actions are belong to
	 */
	MainPanel* m_panel;
	/*! A running animation process 
	 */
	gui::AnimationProcess* m_animation;
};

}
