#include "sprite2dobserver.h"
#include "sprite2dconfig.h"
#include <assert.h>

Sprite2DConfigObserver::Sprite2DConfigObserver(const hst::string & spritegroup, int index, 
											   Sprite2DConfig * config,
										       Sprite2DConfigObserverPolicy policy
											  )
{
	assert( config!=NULL ); 
	
	m_spritegroup=spritegroup;
	m_index=index;
	m_config=config;
	m_policy=policy;

	//By default sprite is not created
	m_sprite=NULL;

	//Register observer
	config->addObserver(this);
}

Sprite2DConfigObserver::~Sprite2DConfigObserver()
{
	if (m_sprite)
	{
		delete m_sprite;
	}
	m_config->removeObserver(this);
}

bool Sprite2DConfigObserver::createSprite(const hPointF & center)
{
	if ( m_config->getTemplates().contains(m_spritegroup) )
	{
		const hst::hash<int,Sprite2DTemplate> & hash=m_config->getTemplates()[m_spritegroup];
		if (hash.contains(m_index))
		{
			const Sprite2DTemplate & sprite_template=hash[m_index];	
			sad::Texture  * texture=m_config->getTextures()->get(sprite_template.textureName());
			if (texture!=NULL)
			{
				hRectF bounding_box(center-sprite_template.size()/2,center+sprite_template.size()/2);
				m_sprite=new Sprite2DController(texture,sprite_template.textureRect(),bounding_box);
				return true;
			}
		}	
	}
	return false;
}

