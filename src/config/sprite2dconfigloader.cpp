#include "sprite2dconfigloader.h"

using namespace sad;

void Sprite2DConfigLoader::insertTemplate(const sad::String & group, 
						        	      int index,
								          const Sprite2DTemplate & stemplate,
								          Sprite2DTemplateContainer & container)
{
	if (container.contains(group)==false)
	{
		container.insert(group,sad::Hash<int,Sprite2DTemplate>());
	}
	container[group].insert(index,stemplate);
}

Sprite2DConfigLoader::~Sprite2DConfigLoader()
{
}
