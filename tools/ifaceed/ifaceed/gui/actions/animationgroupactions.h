/*! \file animationgroupactions.h
    

    Describes a group of actions, linked to instances
 */
#pragma once
#include <QObject>

#include "abstractactions.h"

#include "../animationgroupprocess.h"

// ReSharper disable once CppUnusedIncludeDirective
#include <animations/animationsgroup.h>


class MainPanel;

namespace gui
{

namespace actions
{

/*! A group of actions, linked to animation instances
 */	
class AnimationGroupActions: public QObject, public gui::actions::AbstractActions
{
Q_OBJECT
public:
    /*! Creates new label actions
        \param[in] parent a parent object
     */
    AnimationGroupActions(QObject* parent = nullptr);
    /*! Sets inner editor link of process
        \param[in] e editor
     */
    virtual void setEditor(core::Editor* e) override;
    /*! This class could be inherited
     */
    virtual ~AnimationGroupActions();
    /*! Removes group from database
        \param[in] g group
        \param[in] from_editor whether group is being editor
        \param[in] row a row (-1 if function should determine it)
     */
    void removeFromDatabase(
        sad::animations::Group* g,
        bool from_editor,
        int row = -1
    );
    /*! Adds group to group list
        \param[in] g group
     */
    void addGroupToList(sad::animations::Group* g);
    /*! Removes group from group list
     */
    void removeLastGroupFromList();
    /*! Insert group to group list
        \param[in] pos a position
        \param[in] g group
     */
    void insertGroupToList(int pos, sad::animations::Group* g);
    /*! Removes group from group list
     */
    void removeGroupFromList(int pos);
    /*! Returns name for animation group
        \param[in] g group
        \return name for group
     */
    QString nameForGroup(sad::animations::Group* g) const;
    /*! Emitted, when changed, whether current animation group play mode is set
        to sequential or not
        \param[in] sequential whether sequential
     */
    void changedIsSequential(bool sequential);
public slots:
    /*! Tries to add new animation group
     */
    void addGroup();
    /*! Current changed group
        \param[in] new_row new row
     */ 
    void currentGroupChanged(int new_row);
    /*! Tries to remove animation group
     */
    void removeGroup();
    /*!  Called, when user changes name of instance
        \param[in] name a name of instance
     */
    void nameChanged(const QString& name);
    /*! Called, when user changes "Whether animation group is looped" flag
        \param[in] new_value a new value
     */
    void loopedChanged(bool new_value);
    /*! Tries to add instance to an animation group
     */
    void addInstance();
    /*! Tries to remove instance from an animation group
     */
    void removeInstance();
    /*! Starts running selected animation group
     */
    void start();
    /*! Stop running selected animation group
     */
    void stop();
    /*! User toggles a "Sequential" radio button
     */
    void toggledSequential(bool toggled);
    /*! User toggles a "Parallel" radio button
     */
    void toggledParallel(bool toggled);
private:
    /*! A running animation process 
     */
    gui::AnimationGroupProcess* m_animation;
};

}

}
