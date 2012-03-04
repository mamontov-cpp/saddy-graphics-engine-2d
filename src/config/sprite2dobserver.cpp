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
