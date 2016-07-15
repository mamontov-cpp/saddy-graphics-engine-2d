#include "scenesadd.h"

#include "../../core/editor.h"

#include "../../gui/actions/actions.h"
#include "../../gui/actions/sceneactions.h"

#include <QListWidget>
#include <QListWidgetItem>

history::scenes::Add::Add(sad::Scene * s) : m_scene(s)
{
    m_scene->addRef();
}

history::scenes::Add::~Add()
{
    m_scene->delRef();
}

void history::scenes::Add::commit(core::Editor * ob)
{
    m_scene->setActive(true);
    m_scene->Active = true;

    if (ob)
    {
        ob->actions()->sceneActions()->addSceneToSceneList(m_scene);
    }
}

void history::scenes::Add::rollback(core::Editor * ob)
{
    m_scene->setActive(false);
    m_scene->Active = false;

    if (ob)
    {
        ob->actions()->sceneActions()->removeLastSceneFromSceneList();
    }
}
