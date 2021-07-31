/*! \file sceneactions.h
    
    Defines a basic scene actions
 */
#pragma once
#include <QObject>
#include "abstractactions.h"

namespace sad
{

class Scene;

}

namespace gui
{
    
namespace actions
{

/*! A group of actions, linked to scene nodes
 */
class SceneActions: public QObject, public gui::actions::AbstractActions
{
Q_OBJECT
public:
    /*! Creates new action for scene nodes
        \param[in] parent a parent object
     */
    SceneActions(QObject* parent = nullptr);
    /*! This class could be inherited
     */
    virtual ~SceneActions();
    /*! Returns name for a scene
        \param[in] scene a scene name
     */
    QString nameForScene(sad::Scene* scene);
    /*! Adds scene to scene list
        \param[in] s scene 
     */
    void addSceneToSceneList(sad::Scene* s);
    /*! Removes last scene from scene list
     */
    void removeLastSceneFromSceneList();
    /*! Inserts scene to a scene list
        \param[in] s scene
        \param[in] position a position in scene list
     */
    void insertSceneToSceneList(sad::Scene* s, int position);
    /*! Removes scene from a scene list
        \param[in] position a position, where scene must be removed
     */
    void removeSceneFromSceneList(int position);
    /*! Returns current scene for main panel
        \return current scene
     */
    sad::Scene* currentScene();
    /*! Updates scene name, finding it in list, setting it to current name
        \param[in] s scene
        \param[in] update_ui whether we should update a text node
     */
    void updateSceneName(sad::Scene* s, bool update_ui = true);
    /*! Finds scene in scene list
        \param[in] s scene
        \return scene row
     */
    int findSceneInList(sad::Scene* s);
    /*! Sets scenes' positions in list
        \param[in] s1 first scene
        \param[in] s2 second scene
        \param[in] pos1 position of first scene
        \param[in] pos2 position of second scene
     */
    void setScenesInList(sad::Scene* s1, sad::Scene* s2, int pos1, int pos2);
    /*! Adds scene with name 
        \param[in] name a name for a scene
        \param[in] from_editor whether it's run from editor or script
     */
    unsigned long long addSceneWithName(const QString& name, bool from_editor);
    /*! Removes scene. Could be scripted
        \param[in] scene a scene
        \param[in] from_editor whether it's run from editor or script
     */
    void scriptableRemoveScene(sad::Scene* scene, bool from_editor);
public slots:
    /*! Adds new scene to a database
     */
    void addScene();
    /*! Emitted, when current scene is changed
        \param[in] index index of scene in list
     */
    void currentSceneChanged(int index);
    /*! Emitted, when scene name is changed
        \param[in] name new scene name
     */
    void sceneNameChanged(const QString& name);
    /*! Removes scene from a database (making it inactive)
     */
    void removeScene();
    /*! Moves scene back
     */
    void sceneMoveBack();
    /*! Moves scene front
     */
    void sceneMoveFront();
    /*! Clears scene
     */
    void clearScene();
};

}

}
