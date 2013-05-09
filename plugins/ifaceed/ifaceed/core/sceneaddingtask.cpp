#include <algorithm>
#include "sceneaddingtask.h"
#include "../editorcore/editor.h"
#include "../objects/abstractscreenobject.h"

SceneAddingTask::~SceneAddingTask()
{

}

SceneAddingTask::SceneAddingTask(ScreenTemplate * _template, InterlockedScene * scene)
{
	m_template = _template;
	m_scene = scene;
}




bool LayerPropertyComparator::operator() (AbstractScreenObject * o1, AbstractScreenObject * o2) 
{ 
	unsigned int i1 = o1->prop<unsigned int>("layer", sad::Log::ref());
	unsigned int i2 = o2->prop<unsigned int>("layer", sad::Log::ref());
	return i1 > i2;
}

void SceneAddingTask::perform()
{
	if (m_scene->objectCount() == 0)
	{
		// Populate scene here
		std::vector<AbstractScreenObject*> m_simple_list;
		AbstractScreenObject * o = m_template->templateBegin();
		while(o)
		{
			m_simple_list.push_back(o);
			o = m_template->templateNext();
		}
		LayerPropertyComparator c;
		std::sort(m_simple_list.begin(), m_simple_list.end(), c);
		for(int i = 0; i < m_simple_list.size(); i++)
		{
			this->m_scene->add(m_simple_list[i]);
		}
		die();
	}
}
