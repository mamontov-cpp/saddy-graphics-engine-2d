#include "sceneaddingtask.h"
#include "../editorcore/editor.h"

SceneAddingTask::~SceneAddingTask()
{

}

SceneAddingTask::SceneAddingTask(ScreenTemplate * _template, InterlockedScene * scene)
{
	m_template = _template;
	m_scene = scene;
}

void SceneAddingTask::perform()
{
	if (m_scene->objectCount() == 0)
	{
		// Populate scene here
		die();
	}
}
