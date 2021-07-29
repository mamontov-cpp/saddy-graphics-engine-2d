/*! \file customschemafile.h
    
    
    A custom schema file, which holds all custom schemas
 */
#pragma once
#include "../../resource/resourcefile.h"
#include "../../resource/tree.h"
#include "../../maybe.h"
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
class SchemaFile: public sad::resource::ResourceFile
{
public:
    /*! An entry for a parse result
     */
    typedef sad::Triplet<
        sad::String,
        sad::String,
        sad::Vector< sad::Quadruplet<sad::String, sad::String, picojson::value, sad::Maybe<picojson::value> > >
    > parse_result_entry;
    /*! A partial result to schema file
     */
    typedef sad::Vector< parse_result_entry > parse_result;
    /*! Creates new file with specified name. Supposedly it must be path to specified file.
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
    virtual sad::Vector<sad::resource::Error*> load(sad::resource::Folder * parent) override;
    /*! Reloads all resources from a file
        \return errors if any occurred on resources
     */
    virtual sad::Vector<sad::resource::Error*> reload()  override;
    /*! Returns whether resource supports loading from archive
        \return true. This resource supports loading from archives.
     */
    virtual bool supportsLoadingFromTar7z() const override;
protected:
    /*! Parses file with schemas
        \param[out] result a parsed data
        \param[out] errors a errors list
        \param[in] parent a parent folder to load
     */
    void tryParsePartial(
        sad::db::custom::SchemaFile::parse_result & result,
        sad::Vector<sad::resource::Error *> & errors,
        sad::resource::Folder * parent
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
        \param[in] parent a parent folder
        \return whether it was successfull
     */
    bool validateTreeReferences(
        sad::db::custom::SchemaFile::parse_result_entry & parse_result,
        sad::Vector<sad::resource::Error *> & errors,
        sad::resource::Folder * parent
    );  
    /*! Converts non-unique resource names to errors
        \param[in] parse_result a result of parsing entry
        \param[out] errors list of errors
     */
    void convertNonUniqueResourceNamesToErrors(
        sad::db::custom::SchemaFile::parse_result & parse_result,
        sad::Vector<sad::resource::Error *> & errors
    );
    /*! Fills list of resources  with parsed options from array, altering a texture
        \param[in] parsed a parsed list
        \param[out] resources a resource list from data
     */
    void fillOptionsList(
        sad::db::custom::SchemaFile::parse_result & parsed,
        sad::resource::ResourceEntryList & resources
    );      
    /*! A factory to test properties against
     */
    sad::db::StoredPropertyFactory * m_factory;
};

}

}

}
