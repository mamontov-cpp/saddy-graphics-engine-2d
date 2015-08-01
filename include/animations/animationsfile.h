/*! \file animationsfile.h
	

	Describes a file, which contains a list of animations
 */
#pragma once
#include "../resource/physicalfile.h"
#include "../resource/tree.h"

#include "animationsfactory.h"
#include "animationsanimation.h"



namespace sad
{

namespace animations
{

/*! A texture atlas file, which contains a list of Sprite2D::Options, which can be used to initialize
	texture atlas. Please note, that file reloading could break program, if storing links is disabled,
	when loading a tree.
 */
class File: public sad::resource::PhysicalFile
{
public:
	/*! A partial result to atlas file
	 */
	typedef sad::Pair<
		sad::String,
		picojson::value
	> parse_result;
	/*! Creates new flle for animations
		\param[in] name a filename (with or without path) to it
	 */
	File(const sad::String& name = "");	
	/*! Destroys a file
	 */
	virtual ~File();
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
		\param[out] errors a errors list
	 */
	void tryParsePartial(
		sad::Vector<sad::animations::File::parse_result> & result,
		sad::Vector<sad::resource::Error *> & errors 
	);
	
	/*! A factory for creating files
	 */
	sad::animations::Factory m_factory;
};

}

}
