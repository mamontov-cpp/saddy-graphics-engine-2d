#include "sprite2dconfig.h"
#include "sprite2dobserver.h"
#include <texturemanager.h>
#include <assert.h>

sad::TextureContainer * Sprite2DConfig::getTextures()
{
	return sad::TextureManager::instance()->getContainer(m_texturecontainername);
}

bool Sprite2DObserverContainer::fireSpriteConsistencyCheck(const Sprite2DTemplateContainer & container)
{
	bool ok=true;
	for (hst::hash<Sprite2DConfigObserver *,Sprite2DConfigObserver *>::iterator it=m_observers.begin();
		 it!=m_observers.end();
		 it++)
	{
		ok=ok && it.value()->canBeUpdatedFrom(container);
	}
	return ok;
}

void Sprite2DObserverContainer::fireReloadSprite()
{
 for (hst::hash<Sprite2DConfigObserver *,Sprite2DConfigObserver *>::iterator it=m_observers.begin();
	  it!=m_observers.end();
	  it++)
 {
	 it.value()->notifyConfigChanged();
 }
}

