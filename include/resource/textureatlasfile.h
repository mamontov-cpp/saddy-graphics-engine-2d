/*! \file textureatlasfile.h
    

    Describes a file, which contains a Sprite2D::Options - a spritesheet file 
 */
#pragma once
#include "resourcefile.h"
#include "tree.h"
#include "../sprite2d.h"
#include "../sadpair.h"

namespace sad
{

namespace resource
{

/*! A texture atlas file, which contains a list of Sprite2D::Options, which can be used to initialize
    texture atlas. Please note, that file reloading could break program, if storing links is disabled,
    when loading a tree.
 */
class TextureAtlasFile: public sad::resource::ResourceFile
{
public:
    /*! A partial result of parsing atlas file
     */
    struct parse_result
    {
       sad::String ResourceName;   //!< A resource name for texture
       sad::String FileName;       //!< A name of file for texture
       bool        NoMipMaps;      //!< Whether we should not build mip-maps for file
       picojson::array EntryList;  //!< A list of entries for atlas
    };
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
    /*! Returns whether resource supports loading from archive
        \return true. This resource supports loading from archives.
     */
    virtual bool supportsLoadingFromTar7z() const;
protected:
    /*! Parses file with texture atlas
        \param[out] result a texture atlas file
        \param[out] errors a errors list
        \param[in] force_reload force reload atlas
     */
    void tryParsePartial(
        sad::resource::TextureAtlasFile::parse_result & result,
        sad::Vector<sad::resource::Error *> & errors,
        bool force_reload = false
    ) const;
    /*! Whether texture is found
     */
    sad::resource::ResourceFile * m_my_texture;
    /*! A simple result for texture loading
     */
    struct TextureLoadResult
    {
        /*! Old texture resource, used in tree
         */
        sad::resource::Resource * OldTexture;
        /*! New texture resource, used in tree 
         */
        sad::resource::Resource * NewTexture; 
        /*! A new texture file, used data
         */
        sad::resource::ResourceFile * NewTextureFile;
        /*! Creates new texture load
         */ 
        inline TextureLoadResult() : OldTexture(nullptr), NewTexture(nullptr), NewTextureFile(nullptr)
        {
            
        }
    };
    /*! Tries loading new texture
        \param[in] parsed a parsed value result
        \param[out] result of loading result
        \param[out] errors a error list
        \param[in] force_reload whether we should reload texture
     */
    void tryLoadNewTexture(
        sad::resource::TextureAtlasFile::parse_result & parsed,
        sad::resource::TextureAtlasFile::TextureLoadResult & result,
        sad::Vector<sad::resource::Error *> & errors,
        bool force_reload = false
    ) const;
    /*! Commits resources, allocated as a loading result
        \param[in] parsed a result of parsing
        \param[in] result a result of loading a new texture
     */
    void commit(
        sad::resource::TextureAtlasFile::parse_result & parsed,
        const sad::resource::TextureAtlasFile::TextureLoadResult & result
    );
    /*! Frees resources, allocated as a loading result
        \param[in] result a loaded result
     */
    static void free(const sad::resource::TextureAtlasFile::TextureLoadResult & result);
    /*! Fills list of resources  with parsed options from array, altering a texture
        \param[in] parsed a parsed list
        \param[out] resources a resource list from data
        \param[out] texture a texture, which should be altered
     */
    void fillOptionsList(
        sad::resource::TextureAtlasFile::parse_result & parsed,
        sad::resource::ResourceEntryList & resources,
        sad::resource::Resource * texture
    ) const;
};

}

}
