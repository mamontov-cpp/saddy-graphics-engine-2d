#include <resource/textureatlasfile.h>
#include <resource/tree.h>
#include <resource/error.h>

#include <renderer.h>
#include <texture.h>

#include <util/fs.h>
#include <util/free.h>

#include <3rdparty/picojson/getproperty.h>
#include <3rdparty/picojson/valuetotype.h>

#include <fstream>

sad::resource::TextureAtlasFile::TextureAtlasFile(const sad::String& name) 
: sad::resource::PhysicalFile(name), m_my_texture(NULL)
{
	
}

sad::resource::TextureAtlasFile::~TextureAtlasFile()
{
	if (m_my_texture)
	{
		delete m_my_texture;
	}
}

sad::Vector<sad::resource::Error*> sad::resource::TextureAtlasFile::load(
	sad::resource::Folder * parent
)
{
	sad::Vector<sad::resource::Error*> errors;
	sad::resource::TextureAtlasFile::parse_result result;
	this->tryParsePartial(result, errors);
	if (errors.size() == 0)
	{
		if (m_my_texture)
		{
			delete m_my_texture;
		}
		sad::resource::Resource * linkedresource = parent->resource(result.p1());
		if (linkedresource)
		{
			if (linkedresource->metaData()->name() != "sad::Texture")
			{
				errors << new sad::resource::ResourceLoadError(m_name);
				return errors;
			}
		}
		else
		{
			m_my_texture = new sad::resource::PhysicalFile(result.p2());
			linkedresource  = new sad::Texture();
			bool ok = linkedresource->tryLoad(
				*m_my_texture, 
				this->tree()->renderer(), 
				picojson::value(picojson::object()), 
				true
			);
			if (!ok)
			{
				delete linkedresource;
				delete m_my_texture;
				errors << new sad::resource::ResourceLoadError(result.p1());
				m_my_texture = NULL;
				return errors;
			}
		}

		const picojson::array & list = result.p3();
		sad::resource::ResourceEntryList resourcelist;
		fillOptionsList(result, resourcelist, linkedresource);
		errors << this->tree()->duplicatesToErrors(parent->duplicatesBetween(resourcelist));

		if (errors.size() != 0)
		{
			if (m_my_texture)
			{
				delete m_my_texture;
				delete linkedresource;
			}
		}
		else
		{
			if (m_my_texture)
			{
				parent->addResource(result.p1(), linkedresource);
			}
			this->replaceResources(resourcelist);
		}
	}
	return errors;
}

sad::Vector<sad::resource::Error*> sad::resource::TextureAtlasFile::reload()
{
	sad::Vector<sad::resource::Error*> errors;
	sad::resource::TextureAtlasFile::parse_result result;
	this->tryParsePartial(result, errors);
	if (errors.size() == 0)
	{
		sad::resource::TextureAtlasFile::TextureLoadResult textureloadresult;
		sad::resource::ResourceEntryList resourcelist;
		sad::resource::ResourceEntryList tobeadded, tobereplaced, toberemoved;
		this->tryLoadNewTexture(result, textureloadresult, errors);
		if (errors.size() == 0)
		{			
			this->fillOptionsList(
				result, 
				resourcelist,
				textureloadresult.NewTexture
			);
			sad::resource::ResourceEntryList oldresourcelist;
			this->createOldResourceList(oldresourcelist);
			this->diffResourcesLists(oldresourcelist, resourcelist, tobeadded, tobereplaced, toberemoved);
			errors << this->tree()->duplicatesToErrors(this->tree()->root()->duplicatesBetween(tobeadded));
			convertReferencedOptionsToBeRemovedToErrors(toberemoved, errors);
		}

		if (errors.size() == 0)
		{
			sad::resource::Folder * root = this->tree()->root();
			// Add an added resources
			root->addResources(tobeadded);
			// Replace replaced resources
			root->replaceResources(tobereplaced);
			// Remove removable resources
			root->removeResources(toberemoved, true);
			// Replace own resource list, setting correct reference to this
			this->replaceResources(resourcelist);
			// Commit linked texture, storing it in tree
			this->commit(result, textureloadresult);
		}
		else
		{
			sad::resource::free(resourcelist);
			this->free(textureloadresult);
		}

	}
	return errors;	
}


void sad::resource::TextureAtlasFile::tryParsePartial(
		sad::resource::TextureAtlasFile::parse_result & result,
		sad::Vector<sad::resource::Error *> & errors 
)
{
	sad::Maybe<sad::String> maybecontent = this->tryReadToString();
	if (maybecontent.exists())
	{
		picojson::value rootvalue = picojson::parse_string(maybecontent.value());
		if (picojson::get_last_error().size() == 0)
		{
			sad::Maybe<sad::String> mayberesource = picojson::to_type<sad::String>(
				picojson::get_property(rootvalue, "resource")
			);
			sad::Maybe<sad::String> maybefile = picojson::to_type<sad::String>(
				picojson::get_property(rootvalue, "file")
			);
			picojson::value const * atlas = picojson::get_property(rootvalue, "atlas");
			if (atlas && mayberesource.exists() && maybefile.exists())
			{
				if (atlas->is<picojson::array>())
				{
					bool schemeok = true;
					const picojson::array & list = atlas->get<picojson::array>();
					for (size_t i = 0;  (i < list.size()) && schemeok; i++)
					{
						sad::Maybe<sad::String> maybename = picojson::to_type<sad::String>(
							picojson::get_property(list[i], "name")
						);
						sad::Maybe<sad::Size2D> maybesize = picojson::to_type<sad::Size2D>(
							picojson::get_property(list[i], "size")
						);
						sad::Maybe<sad::Rect2D> maybetexrect = picojson::to_type<sad::Rect2D>(
							picojson::get_property(list[i], "texrect")
						);
						bool entryisvalid = maybename.exists() 
								&& maybesize.exists()
								&& maybetexrect.exists();
						schemeok = schemeok && entryisvalid;
						if (!entryisvalid)
						{
							errors << new sad::resource::MalformedResourceEntry(list[i]);
						}
					}
					if (schemeok)
					{
						result.set1(mayberesource.value());
						result.set2(maybefile.value());
						result.set3(list);
					}
				}
				else
				{
					errors << new sad::resource::MalformedResourceEntry(*atlas);
				}
			}
			else
			{
				errors << new sad::resource::MalformedResourceEntry(rootvalue);
			}
		}
		else
		{
			errors << new sad::resource::JSONParseError();
		}
	}
	else
	{
		errors << new sad::resource::FileLoadError(m_name);
	}
}

sad::Maybe<sad::String> sad::resource::TextureAtlasFile::tryReadToString()
{
	sad::Maybe<sad::String> result;
	std::ifstream stream(m_name.c_str());
	if (stream.good())
	{
		std::string alldata(
			(std::istreambuf_iterator<char>(stream)), 
			std::istreambuf_iterator<char>()
		);
		result.setValue(alldata);
	}
	else
	{
		if (util::isAbsolutePath(m_name) == false)
		{
			sad::String path = util::concatPaths(m_tree->renderer()->executablePath(), m_name);
			stream.open(path.c_str());
			if (stream.good())
			{
				std::string alldata(
					(std::istreambuf_iterator<char>(stream)), 
					 std::istreambuf_iterator<char>()
					);
				result.setValue(alldata);
			}
		}
	}
	return result;
}

void sad::resource::TextureAtlasFile::tryLoadNewTexture(
	sad::resource::TextureAtlasFile::parse_result & parsed,
	sad::resource::TextureAtlasFile::TextureLoadResult & result,
	sad::Vector<sad::resource::Error *> & errors
)
{
	result.OldTexture = this->tree()->root()->resource(parsed.p1());
	result.NewTexture = new sad::Texture();
	sad::resource::PhysicalFile * file  = NULL;
	// Try load texture
	if (result.OldTexture)
	{
		if (result.OldTexture->metaData()->canBeCastedTo("sad::Texture") == false)
		{
			errors << new sad::resource::ResourceAlreadyExists(parsed.p1());
			return;
		}
		if (m_my_texture)
		{
			result.NewTextureFile = new sad::resource::PhysicalFile(parsed.p2());
			file = result.NewTextureFile;
		}
		else
		{
			result.NewTextureFile = result.OldTexture->file();
		}
	}
	else
	{
		result.NewTextureFile  = new sad::resource::PhysicalFile(parsed.p2());
		file = result.NewTextureFile;
	}
	sad::Renderer * r = this->tree()->renderer();
	picojson::value v(picojson::object_type, false);
	v.insert("type",     picojson::value(sad::Texture::globalMetaData()->name()));
	v.insert("filename", picojson::value(result.NewTextureFile->name()));
	v.insert("name", picojson::value(parsed.p1()));
	bool ok = result.NewTexture->tryLoad(*file, r, v, true);
	if (!ok)
	{
		errors << new sad::resource::ResourceLoadError(parsed.p1());
	}
}

void sad::resource::TextureAtlasFile::commit(
	sad::resource::TextureAtlasFile::parse_result & parsed,
	const sad::resource::TextureAtlasFile::TextureLoadResult & result
)
{
	sad::resource::PhysicalFile * oldfile = NULL;
	if (result.OldTexture)
	{
		oldfile = result.OldTexture->file();
		this->tree()->root()->replaceResource(parsed.p1(), result.NewTexture);
		if (m_my_texture == NULL)
		{
			oldfile->replace(result.OldTexture, result.NewTexture);
			result.NewTexture->setPhysicalFile(oldfile);
		}
		else
		{
			delete m_my_texture;
			m_my_texture = result.NewTextureFile;
			m_my_texture->setTree(this->tree());
			result.NewTexture->setPhysicalFile(m_my_texture);
		}
	}
	else
	{
		this->tree()->root()->addResource(parsed.p1(), result.NewTexture);

		delete m_my_texture;
		m_my_texture = result.NewTextureFile;
		m_my_texture->setTree(this->tree());
		result.NewTexture->setPhysicalFile(m_my_texture);
	}
}

void sad::resource::TextureAtlasFile::free(
	const sad::resource::TextureAtlasFile::TextureLoadResult & result
)
{
	if (result.NewTexture)
	{
		delete result.NewTexture;
	}
	if (result.NewTextureFile)
	{
		delete result.NewTextureFile;
	}
}

void sad::resource::TextureAtlasFile::fillOptionsList(
	sad::resource::TextureAtlasFile::parse_result & parsed,
	sad::resource::ResourceEntryList & resources,
	sad::resource::Resource * texture
)
{
	const picojson::array & list = parsed.p3();
	for(size_t i = 0; i < list.size(); i++)
	{
		sad::Maybe<sad::String> maybename = picojson::to_type<sad::String>(
			picojson::get_property(list[i], "name")
			);
		sad::Maybe<sad::Size2D> maybesize = picojson::to_type<sad::Size2D>(
			picojson::get_property(list[i], "size")
			);
		sad::Maybe<sad::Rect2D> maybetexrect = picojson::to_type<sad::Rect2D>(
			picojson::get_property(list[i], "texrect")
			);
		sad::Maybe<sad::Color> maybetransparent = picojson::to_type<sad::Color>(
			picojson::get_property(list[i], "transparent")
			);
		sad::Sprite2D::Options * opts = new sad::Sprite2D::Options();
		opts->Rectangle = sad::Rect2D(0, 0,  maybesize.value().Width, maybesize.value().Height);
		opts->TextureRectangle = maybetexrect.value();
		opts->Texture = parsed.p1();
		if (maybetransparent.exists())
		{
			opts->Transparent = true;
			opts->TransparentColor = maybetransparent.value();
			static_cast<sad::Texture*>(texture)->setAlpha(
				255, 
				opts->TransparentColor,
				opts->TextureRectangle
				);
		}
	}
}

void sad::resource::TextureAtlasFile::createOldResourceList(
	sad::resource::ResourceEntryList & resources
)
{
	for(size_t i = 0; i < m_resources.size(); i++)
	{
		sad::Maybe<sad::String> name = this->tree()->root()->find(m_resources[i]);
		if (name.exists())
		{
			resources << sad::resource::ResourceEntry(name.value(), m_resources[i]);
		}
	}
}

void sad::resource::TextureAtlasFile::diffResourcesLists(
		const sad::resource::ResourceEntryList & oldlist,
		const sad::resource::ResourceEntryList & newlist,
		sad::resource::ResourceEntryList & tobeadded,
		sad::resource::ResourceEntryList & tobereplaced,
		sad::resource::ResourceEntryList & toberemoved
)
{
	sad::resource::ResourceEntryList noldlist = oldlist;
	for(size_t i = 0 ; i < newlist.size(); i++)
	{
		bool exists = false;
		for(size_t j = 0 ; j < noldlist.size() && !exists; j++)
		{
			if (newlist[i].p2() == noldlist[j].p2())
			{
				tobereplaced << newlist[i];
				exists = true;
				noldlist.removeAt(j);
			}
		}
		if (exists == false)
		{
			tobeadded << newlist[i];
		}
	}
	for(size_t i = 0; i < noldlist.size(); i++)
	{
		toberemoved << noldlist[i];
	}
}

void sad::resource::TextureAtlasFile::convertReferencedOptionsToBeRemovedToErrors(
	const sad::resource::ResourceEntryList & toberemoved,
	sad::Vector<sad::resource::Error *> & errors
)
{
	for(size_t i = 0 ; i < toberemoved.size(); i++)
	{
		if (toberemoved[i].p2()->referenced())
		{
			errors << new sad::resource::CannotDeleteReferencedResource(toberemoved[i].p1());
		}
	}
}

void sad::resource::TextureAtlasFile::replaceResources(
	const sad::resource::ResourceEntryList & resourcelist
)
{
	m_resources.clear();
	for(int i = 0 ; i < resourcelist.size() - 1 ; i++)
	{
		m_resources << resourcelist[i].p2();
		resourcelist[i].p2()->setPhysicalFile(this);
	}
}