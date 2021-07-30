/*! \file texturearray.h
    

    Describes a list of textures, supporting uniqueness preservation and search by their names
 */
#pragma once
#include "texture.h"

#include <QtCore/QVector>


class TextureArray: public QVector<Texture*>
{
public:
    /*! Constructs a new empty array
     */
    TextureArray();
    /*! Frees memory from textures
     */
    virtual ~TextureArray();
    /*! Adds a texture, if it isn't already in container
        \param[in] t texture
     */
    bool pushUnique(Texture* t);
    /*! Returns true if array contains texture with name
        \param[in] name a texture name
        \return true if contains
     */
    bool contains(const QString& name) const;
    /*! Returns texture by it's name
        \param[in] name name of texture
        \return texture or nullptr if not found
     */
    Texture* get(const QString& name) const;
};