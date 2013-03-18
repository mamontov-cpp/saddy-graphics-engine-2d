#include "sprite2dconfigloader.h"


void Sprite2DConfigLoader::insertTemplate(const hst::string & group, 
						        	      int index,
								          const Sprite2DTemplate & stemplate,
								          Sprite2DTemplateContainer & container)
{
	if (container.contains(group)==false)
	{
		container.insert(group,hst::hash<int,Sprite2DTemplate>());
	}
	container[group].insert(index,stemplate);
}

Sprite2DConfigLoader::~Sprite2DConfigLoader()
{
}
