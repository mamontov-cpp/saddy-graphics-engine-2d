/*! \file atlasentry.h
    

    Describes an entry of atlas, as it was placed in file
 */
#pragma once

#include "../../include/maybe.h"

// ReSharper disable once CppUnusedIncludeDirective
#include "texturearray.h"

#include <QtCore/QRectF>
#include <QtCore/QSizeF>
#include <QtCore/QString>
#include <QtGui/QColor>

/*! Describes an atlas entry, as it's described in file
 */
class AtlasEntry
{
public:
    /*! An index of sprite in group (obsolete)
     */
    sad::Maybe<int> Index;
    /*! A size of sprite
     */
    sad::Maybe<QSizeF> Size;
    /*! An input texture name
     */
    sad::Maybe<QString> InputTextureName;
    /*! A name for atlas
     */
    sad::Maybe<QString> Name;
    /*! Mask color for transparency
     */
    sad::Maybe<QColor> Transparent;
    /*! A texture name for output
     */
    sad::Maybe<QString> OutputTextureName;
    /*! A position for atlas entry in output texture
     */
    sad::Maybe<QRectF> TextureRectangle;

    /*! Constructs new atlas entry
     */
    AtlasEntry();
    /*! Whether atlas entry is valid
        \return whether entry is valid
     */
    bool isValid() const;
    /*! Whether we can write this entry to file
        \return whether we can write this entry to file
     */
    bool canBeWritten() const;
    /*! Returns writable index for entry
     */
    int index() const;
    /*! Returns full name for atlas entry
        \return full name for atlas entry
     */
    QString getFullName() const;
};
