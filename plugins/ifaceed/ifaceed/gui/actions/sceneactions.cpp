#include "sceneactions.h"

#include <scene.h>


Q_DECLARE_METATYPE(sad::Scene*) //-V566

// ============================= PUBLIC METHODS =============================

gui::actions::SceneActions::SceneActions(QObject* parent) : QObject(parent), gui::actions::AbstractActions()
{

}

gui::actions::SceneActions::~SceneActions()
{

}

QString gui::actions::SceneActions::nameForScene(sad::Scene* scene)
{
    return this->viewableObjectName(scene);
}