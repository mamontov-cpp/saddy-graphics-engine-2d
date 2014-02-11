/*! \file spritesheetfile.h
	\author HiddenSeeker

	Describes a file, which contains a Sprite2D::Options - a spritesheet file 
 */
#pragma once
#include "physicalfile.h"
#include "../sprite2d.h"
#include "../sadpair.h"

namespace sad
{

namespace resource
{

/*! A sprit sheet file, which contains a list of Sprite2D::Options
 */
class TextureAtlasFile: public sad::resource::PhysicalFile
{
public:
	/*! A partial result to atlas file
	 */
	typedef sad::Triplet<
		sad::String,
		sad::String,
		picojson::array
	> parse_result;
	/*! Creates new flle with specified name. Supposedly it must be path to specified file.
		\param[in] name a filename (with or without path) to it
	 */
	TextureAtlasFile(const sad::String& name = "");	
	/*! Whether texture atlas file is found
	 */
	~TextureAtlasFile();
	/*! Loads a file. 
		\param[in] parent a parent folder, where all created resources should be stored
	 */
	virtual sad::Vector<sad::resource::Error*> load(sad::resource::Folder * parent);
	/*! Reloads all resources from a file
		\return errors if any occured on resources
	 */
	virtual sad::Vector<sad::resource::Error*> reload();
protected:
	/*! Parses file with texture atlas
		\param[out] result a texture atlas file
		\param[out] errors a error file
	 */
	void tryParsePartial(
		sad::resource::TextureAtlasFile::parse_result & result,
		sad::Vector<sad::resource::Error *> & errors 
	);
	/*! Tries to read a file to string
	 */
	sad::Maybe<sad::String> tryReadToString();
	/*! Whether texture is found
	 */
	sad::resource::PhysicalFile * m_my_texture;
};

}

}
