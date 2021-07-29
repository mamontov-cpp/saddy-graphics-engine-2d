/*! \file animationsfile.h
    

    Describes a file, which contains a list of animations
 */
#pragma once
#include "../resource/resourcefile.h"
#include "../resource/tree.h"

#include "animationsfactory.h"
// ReSharper disable once CppUnusedIncludeDirective
#include "animationsanimation.h"



namespace sad
{

namespace animations
{

/*! A texture atlas file, which contains a list of Sprite2D::Options, which can be used to initialize
    texture atlas. Please note, that file reloading could break program, if storing links is disabled,
    when loading a tree.
 */
class File: public sad::resource::ResourceFile
{
public:
    /*! A partial result to atlas file
     */
    typedef sad::Pair<
        sad::String,
        picojson::value
    > parse_result;
    /*! Creates new file for animations
        \param[in] name a filename (with or without path) to it
     */
    File(const sad::String& name = "");	
    /*! Destroys a file
     */
    virtual ~File() override;
    /*! Loads a file. 
        \param[in] parent a parent folder, where all created resources should be stored
     */
    virtual sad::Vector<sad::resource::Error*> load(sad::resource::Folder * parent) override;
    /*! Reloads all resources from a file
        \return errors if any occurred on resources
     */
    virtual sad::Vector<sad::resource::Error*> reload() override;
    /*! Returns whether resource supports loading from archive
        \return true. This resource supports loading from archives.
     */
    virtual bool supportsLoadingFromTar7z() const override;
protected:
    /*! Loads a file. 
        \param[in] parent a parent folder, where all created resources should be stored
        \param[in] force_reload whether we should force reload
     */
    virtual sad::Vector<sad::resource::Error*> load(sad::resource::Folder * parent, bool force_reload);
    /*! Parses file with texture atlas
        \param[out] result a texture atlas file
        \param[out] errors a errors list
        \param[in] force_reload whether we should force archives to reload
     */
    void tryParsePartial(
        sad::Vector<sad::animations::File::parse_result> & result,
        sad::Vector<sad::resource::Error *> & errors,
        bool force_reload = false
    ) const;
    
    /*! A factory for creating files
     */
    sad::animations::Factory m_factory;
};

}

}
