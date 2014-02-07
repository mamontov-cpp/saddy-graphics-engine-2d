#include "resource/tree.h"
#include "resource/physicalfile.h"

#include "renderer.h"

#include "3rdparty/picojson/picojson.h"
#include "3rdparty/picojson/valuetotype.h"

#include <fstream>

sad::resource::Tree::Tree(sad::Renderer * r)
: 
m_renderer(r), 
m_root(new sad::resource::Folder()), 
m_factory(new sad::resource::Factory()),
m_storelinks(false)
{
	if (r == NULL)
	{
		m_renderer = sad::Renderer::ref();
	}
}

sad::resource::Tree::~Tree()
{
	delete m_root;
	delete m_factory;
}


bool sad::resource::Tree::initFromString(const sad::String & string)
{
	bool result = false;
	picojson::value v = picojson::parse_string(string);
	if (picojson::get_last_error().size() == 0 && v.is<picojson::array>())
	{
		sad::resource::Folder * newroot = new sad::resource::Folder();
		sad::Vector<sad::resource::PhysicalFile *> newfiles;

		sad::resource::Folder * oldroot = m_root;
		m_root = newroot;

		result = true;
		picojson::array & resourcelist = v.get<picojson::array>();		
		for(int i = 0 ; i < resourcelist.size() && result; i++)
		{
			sad::Maybe<sad::String>  maybetype = picojson::to_type<sad::String>(
				picojson::get_property(resourcelist[i], "type")
			);
			sad::Maybe<sad::String>  maybename = picojson::to_type<sad::String>(
				picojson::get_property(resourcelist[i], "filename")
			);
			if (maybetype.exists() && maybename.exists())
			{
				sad::resource::PhysicalFile * file = m_factory->fileByType(maybetype.value());
				if (file)
				{
					file->setTree(this);
					sad::resource::Resource  * resource = m_factory->create(maybetype.value());
					if (resource)
					{
						result = resource->tryLoad(*file, m_renderer, resourcelist[i], m_storelinks);
						file->add(resource);
						resource->setPhysicalFile(file);
						sad::Maybe<sad::String>  mayberesourcename = picojson::to_type<sad::String>(
							picojson::get_property(resourcelist[i], "name")
						);
						if (mayberesourcename.exists())
						{
							m_root->addResource(mayberesourcename.value(), resource);
						}
						else
						{
							delete resource;
						}

					}
					else
					{
						result = file->load();
					}
					newfiles << file;
				}
			}
		}

		if (result)
		{
			delete oldroot;
			for(int i = 0; i < m_files.size(); i++)
				delete m_files[i];
			m_files.clear();
			for(int i = 0; i < newfiles.size(); i++)
				m_files << newfiles[i];
		}
		else
		{
			delete newroot;
			for(int i = 0; i < newfiles.size(); i++)
				delete newfiles[i];
		}
	}
	return result;
}

bool sad::resource::Tree::initFromFile(const sad::String& string)
{
	std::ifstream stream(string.c_str());
	if (stream.good())
	{
		std::string alldata(
			(std::istreambuf_iterator<char>(stream)), 
			std::istreambuf_iterator<char>()
		);
		return initFromString(string);
	}
	return false;
}

bool sad::resource::Tree::load(const sad::String& typehint, const sad::String& file, const sad::String& name)
{
	// TODO: Actually implement
	return false;
}

bool sad::resource::Tree::unload(const sad::String& file)
{
	sad::resource::PhysicalFile * f = this->file(file);
	if (f)
	{
		sad::Vector<sad::resource::Resource*>  list = f->resources();
		bool referenced = false;
		for(int i = 0; i < list.size(); i++)
		{
			referenced = referenced || list[i]->referenced();
		}
		if (referenced)
			return false;
		for(int i = 0; i < list.size(); i++)
		{
			sad::Maybe<sad::String> path = this->root()->find(list[i]);
			if (path.exists())
			{
				this->root()->removeResource(path.value(), true);
			} 
			else
			{
				delete list[i];
			}
		}
		m_files.removeAll(f);
		delete f;
		return true;
	}
	return false;
}

sad::resource::Folder* sad::resource::Tree::root() const
{
	return m_root;
}

sad::resource::PhysicalFile* sad::resource::Tree::file(const sad::String& name)
{
	for(int i = 0; i < m_files.size(); i++)
	{
		if (m_files[i]->name() == name)
			return m_files[i];
	}
	return NULL;
}

const sad::Vector<sad::resource::PhysicalFile*>& sad::resource::Tree::files() const
{
	return m_files;
}

sad::resource::Factory* sad::resource::Tree::factory()
{
	return m_factory;
}

void sad::resource::Tree::setFactory(sad::resource::Factory* factory)
{
	delete m_factory;
	m_factory = factory;
}

void sad::resource::Tree::setRenderer(sad::Renderer * renderer)
{
	m_renderer = renderer;
}

sad::Renderer * sad::resource::Tree::renderer() const
{
	return m_renderer;
}

bool sad::resource::Tree::shouldStoreLinks() const
{
	return m_storelinks;
}

void sad::resource::Tree::setStoreLinks(bool store)
{
	m_storelinks = store;
}
