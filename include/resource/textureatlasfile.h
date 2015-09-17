/*! \file textureatlasfile.h
    

    Describes a file, which contains a Sprite2D::Options - a spritesheet file 
 */
#pragma once
#include "physicalfile.h"
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
        \param[out] errors a errors list
     */
    void tryParsePartial(
        sad::resource::TextureAtlasFile::parse_result & result,
        sad::Vector<sad::resource::Error *> & errors 
    );
    /*! Whether texture is found
     */
    sad::resource::PhysicalFile * m_my_texture;
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
        sad::resource::PhysicalFile * NewTextureFile;
        /*! Creates new texture load
         */ 
        inline TextureLoadResult() : OldTexture(NULL), NewTexture(NULL), NewTextureFile(NULL)
        {
            
        }
    };
    /*! Tries loading new texture
        \param[in] parsed a parsed value result
        \param[out] result of loading result
        \param[out] errors a error list
     */
    void tryLoadNewTexture(
        sad::resource::TextureAtlasFile::parse_result & parsed,
        sad::resource::TextureAtlasFile::TextureLoadResult & result,
        sad::Vector<sad::resource::Error *> & errors
    );
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
    void free(const sad::resource::TextureAtlasFile::TextureLoadResult & result);
    /*! Fills list of resources  with parsed options from array, altering a texture
        \param[in] parsed a parsed list
        \param[out] resources a resource list from data
        \param[out] texture a texture, which should be altered
     */
    void fillOptionsList(
        sad::resource::TextureAtlasFile::parse_result & parsed,
        sad::resource::ResourceEntryList & resources,
        sad::resource::Resource * texture
    );
};

}

}
