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
			opts->Texture = result.p1();
			if (maybetransparent.exists())
			{
				opts->Transparent = true;
				opts->TransparentColor = maybetransparent.value();
				static_cast<sad::Texture*>(linkedresource)->setAlpha(
					255, 
					opts->TransparentColor,
					opts->TextureRectangle
				);
			}
		}

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
			parent->addResources(resourcelist);
			for(int i = 0 ; i < resourcelist.size() - 1 ; i++)
			{
				m_resources << resourcelist[i].p2();
			}
		}
	}
	return errors;
}

sad::Vector<sad::resource::Error*> sad::resource::TextureAtlasFile::reload()
{
	// TODO: Implement
	sad::Vector<sad::resource::Error*> errors;
	sad::resource::TextureAtlasFile::parse_result result;
	this->tryParsePartial(result, errors);
	if (errors.size() == 0)
	{
		
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
						schemeok = schemeok 
							    && maybename.exists() 
								&& maybesize.exists()
								&& maybetexrect.exists();
					}
					if (schemeok)
					{
						result.set1(mayberesource.value());
						result.set2(maybefile.value());
						result.set3(list);
					}
					else
					{
						errors << new sad::resource::JSONParseError();
					}
				}
				else
				{
					errors << new sad::resource::JSONParseError();
				}
			}
			else
			{
				errors << new sad::resource::JSONParseError();
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
