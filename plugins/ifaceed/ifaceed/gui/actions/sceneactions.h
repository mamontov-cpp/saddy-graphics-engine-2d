/*! \file sceneactions.h
	
	Defines a basic scene actions
 */
#pragma once
#include <QObject>
#include <QString>
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
    SceneActions(QObject* parent = NULL);
    /*! This class could be inherited
     */
    virtual ~SceneActions();
	/*! Returns name for a scene
        \param[in] scene a scene name
     */
    QString nameForScene(sad::Scene* scene);
};

}

}