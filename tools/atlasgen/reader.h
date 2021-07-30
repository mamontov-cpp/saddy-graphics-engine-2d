/*! \file reader.h
    

    Defines a reader for atlases
 */
#pragma once
#include "atlas.h"

// ReSharper disable once CppUnusedIncludeDirective
#include <QtCore/QFile>
// ReSharper disable once CppUnusedIncludeDirective
#include <QtCore/QTextStream>


/*! Performs reading configuration for files
 */
class Reader
{
public:
    /*! Constructs new reader
     */
    Reader();
    /*! Must be inherited
     */
    virtual ~Reader();
    /*! Reads a file with specified name
        \param[in] name a reading name
        \param[in] atlas an atlas to be read
     */
    virtual void read(const QString& name, Atlas* atlas)  = 0;
    /*! Returns whether we should preserve unique textures
        \return whether we should preserve unique textures or not
     */
    bool shouldPreserveUniqueTextures() const;
    /*! Sets, whether we should preserve unique textures or not
        \param[in] flag a value
     */
    void toggleShouldPreserveUniqueTextures(bool flag);
    /*! Returns errors for reader
        \return errors
     */
    QVector<QString>& errors();
    /*! Returns errors for reader 
        \return errors
     */
    const QVector<QString>& errors() const;
    /*! Whether loading was successfull
        \return whether it was successfull
     */
    bool ok() const;
protected:
    /*! Whether reader should preserve a unique textures
     */
    bool m_preserve_unique_textures;
    /*! A list of errors
     */
    QVector<QString> m_errors;
    /*! Whether loading was successfull
     */
    bool m_ok;
};
