/*! \file atlas.h
    

    Defines a simple atlas as stored in file
 */
#pragma once
// ReSharper disable once CppUnusedIncludeDirective
#include <QtCore/QHash>
#include <QtCore/QString>
// ReSharper disable once CppUnusedIncludeDirective
#include <QtCore/QVariant>
#include "atlasentry.h"
#include "texturearray.h"


struct OutputOptions;

/*! A simple atlas, as it stored in file
 */
class Atlas
{
public:
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
        \param[in] entry An entry to be added to atlas
     */
    void pushEntry(const AtlasEntry& entry);
    /*! Returns texture array for atlas
        \return texture array
     */
    const ::TextureArray& textures() const;
    /*! Returns texture array for atlas
        \return texture
     */ 
    ::TextureArray& textures();
    /*! Returns entries for atlas
        \return entries for atlas
     */ 
    const QVector<AtlasEntry>& entries() const;
    /*! Returns entries for atlas
        \return entries for atlas
     */ 
    QVector<AtlasEntry>& entries();
    /*! Prepares an atlas for output, copying a texture data and size, if not
        specified.
        \param[in] options an options for atlas
     */
    void prepareForOutput(const OutputOptions& options);
    /*! Sets output name for config
        \param[in] name  name for output config
     */
    void setOutputName(const QString& name);
    /*! Returns output name
        \return output name
     */ 
    const QString& outputName() const;
    /*! Sets output texture file name for config
        \param[in] name a file name for output texture
        \param[in] ignore_settings whether we should ignore settings and set the name anyway
     */
    void setOutputTexture(const QString& name, bool ignore_settings = false);
    /*! Returns output texture file name
        \return output texture file name
     */
    const QString& outputTexture() const;
    /*! Sets resource name
        \param[in] name a resource name
     */
    void setResourceName(const QString& name);
    /*! Returns texture resource name
        \return a resource name
     */
    const QString& resourceName() const;
    /*! Returns true if resource name for texture exists
        \return whether resource name exists
     */
    bool hasResourceName() const;
    /*! Returns true if has texture named with specified name
        \param[in] name a name for texture
     */
    bool hasTexture(const QString& name) const;
    /*! Adds new texture
        \param[in] t texture
     */
    void pushTexture(Texture* t);
    /*! Returns texture by name
        \return name a texture by name
     */
    Texture* getTexture(const QString& name) const;
    /*! Toggle flags for changing output properties only once
        \param[in] flag a flag
     */
    void toggleFlagForChangingOutputPropertiesOnlyOnce(bool flag);
protected:
    /*! An output name
     */
    QString m_output_name;
    /*! An output texture
     */
    QString m_output_texture;
    /*! A texture resource name data
     */
    sad::Maybe<QString> m_texture_resource_name;
    /*! A texture array as array of textures
     */
    TextureArray m_textures;
    /*! An array of entries
     */
    QVector<AtlasEntry> m_entries;
    /*! Whether we should take options only from first file
     */
    bool m_take_first;
};
