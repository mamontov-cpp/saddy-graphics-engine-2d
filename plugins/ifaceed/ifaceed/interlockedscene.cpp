#include "interlockedscene.h"

InterlockedScene::InterlockedScene():sad::Scene()
{
}

InterlockedScene::~InterlockedScene()
{
	clearContainers();
}

void InterlockedScene::render()
{
	RenderMutex.lock();
	this->sad::Scene::render();
	RenderMutex.unlock();
}

ISceneObjectContainer::~ISceneObjectContainer()
{
	
}

void InterlockedScene::clearContainers()
{
	for(hst::hash<hst::string,ISceneObjectContainer *>::iterator it=m_containers.begin();
		it!=m_containers.end();
		it++)
	{
		delete it.value();
	}
	m_containers.clear();
}