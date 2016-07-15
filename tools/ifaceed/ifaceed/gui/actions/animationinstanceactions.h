/*! \file animationinstanceactions.h
    

    Describes a group of actions, linked to instances
 */
#pragma once
#include <QObject>
#include <QRectF>

#include <sadhash.h>
#include <animations/animationscomposite.h>
#include <animations/animationswayinstance.h>

#include "abstractactions.h"

#include "../animationinstanceprocess.h"

class MainPanel;

namespace gui
{

class InstanceProcess;

namespace uiblocks
{

class UIAnimationInstanceBlock;

}

namespace actions
{


/*! A group of actions, linked to animation instances
 */	
class AnimationInstanceActions: public QObject, public gui::actions::AbstractActions
{
Q_OBJECT
public:
    /*! Creates new label actions
        \param[in] parent a parent object
     */
    AnimationInstanceActions(QObject* parent = NULL);
    /*! This class could be inherited
     */
    virtual ~AnimationInstanceActions();
    /*! Sets inner editor link of process
        \param[in] e editor
     */
    virtual void setEditor(core::Editor* e);
    /*! Updates instance list for groups
     */
    void updateGroupInstanceList();
    /*! Updates animation for current instance
        \param[in] a instance
     */
    void updateCurrentInstanceAnimation(sad::animations::Instance* a);
    /*! Tries to remove animation instance from list
        \param[in] a instance
        \param[in] fromeditor whether instance is being removed from editor
        \param[in] row a row, where instance is removed from
     */
    void removeInstanceFromDatabase(
        sad::animations::Instance* a,
        bool fromeditor,
        int row = -1
    );
    /*! Returns name for animation instance
        \param[in] i instance
        \return name for instance
     */
    QString nameForInstance(sad::animations::Instance* i) const;
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
    /*! A running animation process 
     */
    gui::AnimationInstanceProcess* m_animation;
    /*! Block for editor
     */
    gui::uiblocks::UIAnimationInstanceBlock* m_blk;
};

}

}
