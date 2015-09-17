/*! \file atlas.h
    

    Defines a simple atlas as stored in file
 */
#pragma once
#include "atlasentry.h"
#include "texturearray.h"


/*! A simple atlas, as it stored in file
 */
class Atlas
{
public:
    /*! A texture array as array of textures
     */
    TextureArray Textures;
    /*! An array of entries
     */
    QVector<AtlasEntry> Entries;
    /*! List of errors, which had occured when loading
     */
    QVector<QString> Errors;
    /*! A texture resource name data
     */
    sad::Maybe<QString> TextureResourceName;

    /*! Constructs new empty atlas
     */
    Atlas();
    /*! Searches entry in list
        \param[in] name a name for entry
        \param[in] index index, if it's exists
        \return whether entry list contains it
     */
    bool hasEntry(const QString& name, const sad::Maybe<int>& index);
    /*! Adds new entry to the atlas
        \param[in] entry. An entry to be added to atlas
     */
    void pushEntry(const AtlasEntry& entry);
    /*! Returns texture array for atlas
        \return texture array
     */
    const ::TextureArray& textures() const;
    /*! Returns entries for atlas
        \return entries for atlas
     */ 
    const QVector<AtlasEntry>& entries() const;
    /*! Returns errors for loaded atlas
        \return errors
     */
    const QVector<QString>& errors() const;
    /*! Tries to load a texture from file
        \param[in] filename a name of file, where texture is located
        \return whether loading was successfull
     */
    bool loadTexture(const QString& filename);
    /*! Prepares an atlas for output, copying a texture data and size, if not
        specified.
        \param[in] outputTextureName an output texture name,
     */
    void prepareForOutput(const QString& outputTextureName);
};
