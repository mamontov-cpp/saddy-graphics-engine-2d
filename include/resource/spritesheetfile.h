/*! \file spritesheetfile.h
	\author HiddenSeeker

	Describes a file, which contains a Sprite2D::Options - a spritesheet file 
 */
#pragma once
#include "physicalfile.h"
#include "../sprite2d.h"

namespace sad
{

namespace resource
{

/*! A sprit sheet file, which contains a list of Sprite2D::Options
 */
class SpriteSheetFile: public sad::resource::PhysicalFile
{
public:
	/*! Creates new flle with specified name. Supposedly it must be path to specified file.
		\param[in] name a filename (with or without path) to it
	 */
	SpriteSheetFile(const sad::String& name = "");	
	/*! Loads a file. 
		\param[in] parent a parent folder, where all created resources should be stored
	 */
	virtual sad::Vector<sad::resource::Error*> load(sad::resource::Folder * parent);
	/*! Reloads all resources from a file
		\return errors if any occured on resources
	 */
	virtual sad::Vector<sad::resource::Error*> reload();
};

}

}
