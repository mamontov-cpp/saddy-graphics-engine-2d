#include "sprite2dconfig.h"
#include "sprite2dobserver.h"
#include <texturemanager.h>
#include <assert.h>

sad::TextureContainer * Sprite2DConfig::getTextures()
{
	return sad::TextureManager::instance()->getContainer(m_texturecontainername);
}

