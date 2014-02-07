#include "resource/folder.h"
#include "resource/resource.h"

sad::resource::Folder::Folder() : m_parent(NULL)
{

}

sad::resource::Folder::~Folder()
{

}

bool sad::resource::Folder::hasFolders() const
{
	return m_subfolders.count() != 0;
}

bool sad::resource::Folder::hasResources() const
{
	return m_resources.count() != 0;
}

bool sad::resource::Folder::addFolder(const sad::String& path, sad::resource::Folder* folder)
{
	sad::String foldername;
	sad::resource::Folder * parent = navigateParentFolder(path, true, foldername);
	if (parent == NULL)
	{
		return false;
	}
	if (parent->m_subfolders.contains(foldername) != NULL)
	{
		delete parent->m_subfolders[foldername];
	}
	parent->m_subfolders.insert(foldername, folder);
	folder->setParent(parent);
	return true;
}

bool sad::resource::Folder::addResource(const sad::String & path, sad::resource::Resource* r)
{
	sad::String resourcename;
	sad::resource::Folder * parent = navigateParentFolder(path, true, resourcename);
	if (parent == NULL)
	{
		return false;
	}
	if (parent->m_resources.contains(resourcename) != NULL)
	{
		delete parent->m_resources[resourcename];
	}
	parent->m_resources.insert(resourcename, r);
	r->setParentFolder(parent);
	return true;	
}

void sad::resource::Folder::removeFolder(const sad::String& path, bool free)
{
	sad::String foldername;
	sad::resource::Folder * parent = navigateParentFolder(path, false, foldername);
	if (parent == NULL)
	{
		return;
	}
	if (parent->m_subfolders.contains(foldername))
	{
		parent->setParent(NULL);
		if (free) 
		{
			delete parent->m_subfolders[foldername];
		}
		parent->m_subfolders.remove(foldername);
	}
}

void sad::resource::Folder::removeResource(const sad::String& path, bool free)
{
	sad::String resourcename;
	sad::resource::Folder * parent = navigateParentFolder(path, false, resourcename);
	if (parent == NULL)
	{
		return;
	}
	if (parent->m_resources.contains(resourcename))
	{
		parent->setParent(NULL);
		if (free) 
		{
			delete parent->m_resources[resourcename];
		}
		parent->m_resources.remove(resourcename);
	}
			
}

sad::resource::Folder* sad::resource::Folder::folder(const sad::String& path)
{
	sad::String foldername;
	resource::Folder * parent = this->navigateParentFolder(path, false, foldername);
	resource::Folder * result = NULL;
	if (parent)
	{
		if (parent->m_subfolders.contains(foldername))
		{
			result = parent->m_subfolders[foldername];
		}
	}
	return result;
}

sad::resource::Resource* sad::resource::Folder::resource(const sad::String& path)
{
	sad::String foldername;
	resource::Folder * parent = this->navigateParentFolder(path, false, foldername);
	resource::Resource * result = NULL;
	if (parent)
	{
		if (parent->m_resources.contains(foldername))
		{
			result = parent->m_resources[foldername];
		}
	}
	return result;	
}

void sad::resource::Folder::replaceResource(const sad::String& name, resource::Resource* r)
{
	sad::resource::Resource * old = resource(name);
	if (old)
	{
		old->replaceWith(r);
		this->removeResource(name, true);
	}
	this->addResource(name, r);
}

sad::resource::FolderIterator sad::resource::Folder::foldersBegin()
{
	return m_subfolders.begin();
}

sad::resource::FolderIterator sad::resource::Folder::foldersEnd()
{
	return m_subfolders.end();
}

sad::resource::ResourceIterator sad::resource::Folder::resourceBegin()
{
	return m_resources.begin();
}

sad::resource::ResourceIterator sad::resource::Folder::resourceEnd()
{
	return m_resources.end();
}

void sad::resource::Folder::setParent(sad::resource::Folder * folder)
{
	m_parent = folder;
}

sad::resource::Folder * sad::resource::Folder::parent() const
{
	return m_parent;
}

sad::resource::Folder * sad::resource::Folder::navigateParentFolder(
		const sad::String & path, 
		bool create,
		sad::String & name
)
{
	if (path.size() == 0 )
		return NULL;
	if (path.getOccurences("/") > 1024)
		return NULL;
	sad::Vector<sad::String> splitpath = path.split("/");
	if (splitpath.size() == 0)
		return NULL;
	sad::resource::Folder * parent = this;
	for(int i = 0; i < (int)(splitpath.size()) - 2; i++)
	{
		if (parent->m_subfolders.contains(splitpath[i]) == NULL)
		{
			if (create)
			{
				parent->m_subfolders.insert(splitpath[i], new sad::resource::Folder());
			}
			else
			{
				return NULL;
			}
		}
		parent = parent->m_subfolders[splitpath[i]];
	}
	name = splitpath[splitpath.count() - 1];
	return parent;
}
