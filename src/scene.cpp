#include "scene.h"

int sad::BasicNode::Type=0;
int sad::BasicNode::type() const { return m_type;}

sad::BasicNode::BasicNode()
{
	m_type=sad::BasicNode::Type;
}
sad::BasicNode::~BasicNode()
{

}
sad::Scene::Scene()
{

}
sad::Scene::~Scene()
{
	for (unsigned long i=0;i<m_layers.count();i++)
		delete m_layers[i];
}
void sad::Scene::add(
		             BasicNode * node, 
		             const hst::string & name,
		             unsigned long lay
		            )
{
	unsigned long pos;

	if (lay==(unsigned long)-1)
	{
		this->m_layers.add(node);
		pos=m_layers.count()-1;
	}
	else
	{
		this->m_layers.insert(node,lay);
		pos=lay;
	}
	
	if (name.empty()) return;
    this->m_nodehash.insert(name,pos);
}

void sad::Scene::remove(const hst::string & name)
{
	if (m_nodehash.contains(name))
	{
		m_layers.removeAt(m_nodehash[name]);
		m_nodehash.remove(name);
	}
}

void sad::Scene::render()
{
  for (unsigned long i=0;i<m_layers.count();++i)
  {
	  unsigned long  cc=m_layers.count();
	  m_layers[i]->render();
	  //If we are removed sth from
	  if (m_layers.count()<cc) --i;
  }
}