/*! \file animationactions.h
    

    Describes a group of actions, linked to animations
 */
#pragma once
#include <QObject>
#include <QRectF>

#include <sadhash.h>
#include <animations/animationscomposite.h>
#include <input/controls.h>

#include "abstractactions.h"

class MainPanel;

namespace gui
{

class AnimationProcess;

namespace actions
{

/*! A group of actions, linked to animations
 */	
class AnimationActions: public QObject, public gui::actions::AbstractActions
{
Q_OBJECT
public:
    /*! Creates new label actions
        \param[in] parent a parent object
     */
    AnimationActions(QObject* parent = NULL);
    /*! Sets inner editor link of process
        \param[in] e editor
     */
    virtual void setEditor(core::Editor* e);
    /*! This class could be inherited
     */
    virtual ~AnimationActions();
    /*! Returns true if adding second animation into children of first produces loop
        \param[in] first first loop
        \param[in] second second loop
     */
    bool producesLoop(sad::animations::Animation* first, sad::animations::Animation* second) const;
    /*! Returns true if adding second animation into children of first produces loop
        \param[in] current current visited animation
        \param[in] visited set of visited noded
        \param[in] first a first visited part
        \param[in] second a second visited part
     */
    static bool producesLoop(
        sad::animations::Composite* current,
        const sad::Hash<unsigned long long, unsigned long long>& visited,
        sad::animations::Animation* first,
        sad::animations::Animation* second
    );
    /*! Removes an animation from database
        \param[in] a animation
        \param[in] fromeditor whether it's being removed from editor
     */
    void removeAnimationFromDatabase(
        sad::animations::Animation* a,
        bool fromeditor
    ) const;
    /*! Adds animation to composite list
        \param[in] a composite list
        \param[in] addedanimation an added animation
        \param[in] fromeditor whether it's being added to a list
     */
    bool addAnimationToCompositeList(
        sad::animations::Composite* a,
        sad::animations::Animation* addedanimation,
        bool fromeditor
    ) const;
    /*! Removes animation from composite list
        \param[in] a composite list
        \param[in] addedanimation an added animation
        \param[in] fromeditor whether it's being added to a list
        \param[in] row a row (-1 if undefined)
     */
    bool removeAnimationFromCompositeList(
        sad::animations::Composite* a,
        sad::animations::Animation* addedanimation,
        bool fromeditor,
        int row = -1
    ) const;
    /*! Returns name for animation
        \param[in] a animation
        \return name for animation
     */
    QString nameForAnimation(sad::animations::Animation* a) const;
    /*! Adds animation to a lists
        \param[in] a animation an animation
     */
    void addAnimationToViewingLists(sad::animations::Animation* a) const;
    /*! Removes animation from a viewing lists
        \param[in] a animation an animation to be removed
     */
    void removeAnimationFromViewingLists(sad::animations::Animation* a) const;
    /*! Updates animation name in lists
        \param[in] a an animation
     */
    void updateAnimationName(sad::animations::Animation* a) const;
public slots:
    /*! Tries to add new animation to a list
     */
    void addAnimation() const;
    /*! Triews to remove animation from all related list
     */
    void removeAnimation() const;
    /*! Called, when user changes animation in list
        \param[in] row a new selected row in list
     */
    void currentAnimationChanged(int row) const;
    /*! Called, when user changes name of animation
        \param[in] name a name of animation
     */
    void nameChanged(const QString& name) const;
    /*! Called, when user changes time of animation
        \param[in] time a time of animation
     */
    void timeChanged(double time) const;
    /*! Called, when user changes "Whether animation is looped" flag
        \param[in] newvalue a new value
     */
    void loopedChanged(bool newvalue) const;
    /*! Called, when user changes frequency of blinking of animation
        \param[in] nvalue a value
     */
    void blinkingFrequencyChanged(int nvalue) const;
    /*! Starts a selected animation as instance on selected object
     */
    void startOnObject() const;
    /*! Stops a selected animation as instance on selected object
     */
    void stopOnObject() const;
    /*! Changes starting color for color animations
     */
    void colorChangeStartingColor() const;
    /*! Changes ending color for color animations
     */
    void colorChangeEndingColor() const;
    /*! Called, when user changes x coordinate of starting size for Resize type of animation
        for X coordinate
        \param[in] newvalue a new value
     */
    void resizeChangeStartingSizeX(double newvalue) const;
    /*! Called, when user changes y coordinate of starting size for Resize type of animation
        for Y coordinate
        \param[in] newvalue a new value
     */
    void resizeChangeStartingSizeY(double newvalue) const;
    /*! Called, when user changes x coordinate of ending size for Resize type of animation
        for X coordinate
        \param[in] newvalue a new value
     */
    void resizeChangeEndingSizeX(double newvalue) const;
    /*! Called, when user changes y coordinate of ending size for Resize type of animation
        for Y coordinate
        \param[in] newvalue a new value
     */
    void resizeChangeEndingSizeY(double newvalue) const;
    /*! Called, when user changes starting angle for rotation angle
        \param[in] newvalue new value for angle
     */ 
    void rotateChangeStartingAngle(double newvalue) const;
    /*! Called, when user changes ending angle for rotation angle
        \param[in] newvalue new value for angle
     */ 
    void rotateChangeEndingAngle(double newvalue) const;
    /*! Called, when user changes way for way moving animation
        \param[in] row a row
     */
    void wayMovingChangeWay(int row) const;
    /*! Called, when font list editing  finished (timed event, see implementation of
        gui::textedit::TextEdit for detail)
     */
    void fontListEditingFinished() const;
    /*! Called, when font size starting size changed
        \param[in] newvalue new value
     */
    void fontSizeChangeStartingSize(int newvalue) const;
    /*! Called, when font size ending size changed
        \param[in] newvalue new value
     */
    void fontSizeChangeEndingSize(int newvalue) const;
    /*! Called, when option list editing  finished (timed event, see implementation of
        gui::textedit::TextEdit for detail)
     */
    void optionListEditingFinished() const;
    /*! Called, when texture coordinates list editing  finished (timed event, see implementation of
        gui::textedit::TextEdit for detail)
     */
    void textureCoordinatesListEditingFinished() const;
    /*! Called, when user changes starting rect for a value
        \param[in] value new value
     */
    void textureCoordinatesChangeStartRect(QRectF value) const;
    /*! Called, when user changes ending rect for a value
        \param[in] value new value
     */
    void textureCoordinatesChangeEndRect(QRectF value) const;
    /*! Called, when user changes x coordinate of camera rotation pivot 
        \param[in] newx new value
     */
    void cameraRotationChangePivotX(double newx) const;
    /*! Called, when user changes y coordinate of camera rotation pivot 
        \param[in] newy new value
     */
    void cameraRotationChangePivotY(double newy) const;
    /*! Called, when user changes starting angle of camera rotation animation
        \param[in] newvalue a new value for animation
     */
    void cameraRotationChangeStartingAngle(double newvalue) const;
    /*! Called, when user changes ending angle of camera rotation animation
        \param[in] newvalue a new value for animation
     */
    void cameraRotationChangeEndingAngle(double newvalue) const;
    /*! Called, when user changes x coordinate of camera shaking offset 
        \param[in] newx new value
     */
    void cameraShakingChangeOffsetX(double newx) const;
    /*! Called, when user changes y coordinate of camera shaking offset
        \param[in] newy new value
     */
    void cameraShakingChangeOffsetY(double newy) const;
    /*! Called, when user changes frequency of camera shaking
        \param[in] newvalue a new value
     */
    void cameraShakingChangeFrequency(int newvalue) const;
    /*! Update composite elements list
     */
    void updateCompositeList() const;
    /*! Adds animation to composite animations list
     */
    void addAnimationToComposite() const;
    /*! Removes animation from composite animations list
     */
    void removeAnimationFromComposite() const;
    /*! Moves animation back in composite list
     */
    void moveBackInCompositeList() const;
    /*! Moves animation front in composite list
     */
    void moveFrontInCompositeList() const;
    /*! Called, when user changes x coordinates of starting point for simple
        movement animation type
       \param[in] newvalue a new value
     */
    void simpleMovementChangeStartingPointX(double newvalue) const;
    /*! Called, when user changes y coordinates of starting point for simple
        movement animation type
       \param[in] newvalue a new value
     */
    void simpleMovementChangeStartingPointY(double newvalue) const;
    /*! Called, when user changes x coordinates of ending point for simple
        movement animation type
       \param[in] newvalue a new value
     */
    void simpleMovementChangeEndingPointX(double newvalue) const;
    /*! Called, when user changes y coordinates of ending point for simple
        movement animation type
       \param[in] newvalue a new value
     */
    void simpleMovementChangeEndingPointY(double newvalue) const;
    /*! Starts picking starting point for simple movement
     */
    void startPickingStartingPointForSimpleMovement() const;
    /*! Starts picking ending point for simple movement
     */
    void startPickingEndingPointForSimpleMovement() const;
    /*! Cancels picking point
     */
    void cancelPickingPointForSimpleMovement() const;
    /*! Event, which is emitted, when user picks point for simple movement
        \param[in] e event
     */
    void pickedPointForSimpleMovement(const sad::input::MousePressEvent& e) const;
    /*! Called, when user changes overshoot amplitude of animation
        \param[in] newvalue a new value
     */
    void easingOvershootAmplitudeChanged(double newvalue) const;
    /*! Called, when user changes period of animation
        \param[in] newvalue a new value
     */
    void easingPeriodChanged(double newvalue) const;
    /*! Called, when user changes function type
        \param[in] newvalue a new value
     */
    void easingFunctionTypeChanged(int newvalue) const;
private:
    /*! A running animation process 
     */
    gui::AnimationProcess* m_animation;
};

}

}
