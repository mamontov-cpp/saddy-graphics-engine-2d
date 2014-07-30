/*! \file customschemafile.h
	\author HiddenSeeker
	
	A custom schema file, which holds all custom schemas
 */
#pragma once
#include "../resource/physicalfile.h"
#include "../resource/tree.h"
#include "customschema.h"

namespace sad
{
	
namespace db
{
	
namespace custom
{
	
/*! A schema file, which contains a list of sad::db::custom::Schema, which can be used to create custom
	object for files.
 */
class SchemaFile: public sad::resource::PhysicalFile
{
public:
	/*! An entry for a parse result
	 */
	typedef sad::Triplet<
		sad::String,
		sad::String,
		sad::Vector< sad::Pair<sad::String, sad::String> >
	> parse_result_entry;
	/*! A partial result to schema file
	 */
	typedef sad::Vector< parse_result_entry > parse_result;
	/*! Creates new flle with specified name. Supposedly it must be path to specified file.
		\param[in] name a filename (with or without path) to it
	 */
	SchemaFile(const sad::String& name = "");	
	/*! Whether texture atlas file is found
	 */
	~SchemaFile();
	/*! Copies new factory into file
		\param[in] factory a factory for file
	 */
	void setFactory(sad::db::StoredPropertyFactory * factory);
	/*! Returns factory for properties in file
		\return factory
	 */
	sad::db::StoredPropertyFactory * factory() const;
	/*! Loads a file. 
		\param[in] parent a parent folder, where all created resources should be stored
	 */
	virtual sad::Vector<sad::resource::Error*> load(sad::resource::Folder * parent);
	/*! Reloads all resources from a file
		\return errors if any occured on resources
	 */
	virtual sad::Vector<sad::resource::Error*> reload();
protected:
	/*! Parses file with schemas
		\param[out] result a parsed data
		\param[out] errors a errors list
	 */
	void tryParsePartial(
		sad::db::custom::SchemaFile::parse_result & result,
		sad::Vector<sad::resource::Error *> & errors 
	);
	/*! Tries to parse entry by schema
		\param[out] parse_result a parsed entry 
		\param[out] errors  a errors list
		\param[in] v a parsed value
		\return whether it was successfull
	 */
	bool tryParseEntry(
		sad::db::custom::SchemaFile::parse_result_entry & parse_result,
		sad::Vector<sad::resource::Error *> & errors,
		const picojson::value & v
	);
	/*! Validates references to tree resources in schema
		\param[in] parse_result a result of parsing entry
		\param[out] errors list of errors
		\return whether it was successfull
	 */
	bool validateTreeReferences(
		sad::db::custom::SchemaFile::parse_result_entry & parse_result,
		sad::Vector<sad::resource::Error *> & errors
	);
	/*! Tries to read a file to string
	 */
	sad::Maybe<sad::String> tryReadToString();
	/*! Commits resources, allocated as a loading result
		\param[in] parsed a result of parsing
		\param[in] result a result of loading a new texture
	 */
	void commit(
		sad::db::custom::SchemaFile::parse_result & parsed
	);
	/*! Fills list of resources  with parsed options from array, altering a texture
		\param[in] parsed a parsed list
		\param[out] resources a resource list from data
	 */
	void fillOptionsList(
		sad::db::custom::SchemaFile::parse_result & parsed,
		sad::resource::ResourceEntryList & resources
	);
	/*! Converts old resources to a resources
		\param[out] resources a resource list
	 */
	void createOldResourceList(
		sad::resource::ResourceEntryList & resources
	);
	/*! Computes differences between two resource lists
		\param[in] oldlist an old list of resources
		\param[in] newlist a  new list of resources
		\param[out] tobeadded a resources, that should be added to tree
		\param[out] tobereplaced a resources from new list, that should replace old list
		\param[out] toberemoved a resources from old list, that should be removed
	 */
	void diffResourcesLists(
		const sad::resource::ResourceEntryList & oldlist,
		const sad::resource::ResourceEntryList & newlist,
		sad::resource::ResourceEntryList & tobeadded,
		sad::resource::ResourceEntryList & tobereplaced,
		sad::resource::ResourceEntryList & toberemoved
	);
	/*! Converts referenced options to be removed to CannotDeleteReferencedResource errors, 
		appending them to a vector
		\param[in] toberemoved a list of resources
		\param[in] errors an occured errors
	 */
	void convertReferencedOptionsToBeRemovedToErrors(
		const sad::resource::ResourceEntryList & toberemoved,
		sad::Vector<sad::resource::Error *> & errors
	);
	/*! Replaces resources of texture atlas file with list
		\param[in] resourcelist a list of resources
	 */
	void replaceResources(
		const sad::resource::ResourceEntryList & resourcelist
	);

	/*! A factory to test properties against
	 */
	sad::db::StoredPropertyFactory * m_factory;
};

}

}

}