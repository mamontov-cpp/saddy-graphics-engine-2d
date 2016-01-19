/*! \file reader.h
    

    Defines a reader for atlases
 */
#pragma once
#include "atlas.h"

#include <QtCore/QFile>
#include <QtCore/QTextStream>


/*! Performs reading configuration for files
 */
class Reader
{
public:
    /*! Whether reading was successfull
     */
    bool Successfull;
    /*! A resulting atlas
     */
    Atlas* Result;
    /*! A list of errors
     */
    QVector<QString> Errors;
    /*! An output file name for atlas config
     */
    QString OutputName;
    /*! An output file name for atlas texture
     */
    QString OutputTexture;

    /*! Constructs new reader
     */
    Reader();
    /*! Must be inherited
     */
    virtual ~Reader();
    /*! Reads a file with specified name
       \param name a reading name
     */
    virtual void read(const QString& name)  = 0;
	/*! Returns whether we should preserve unique textures
		\return whether we should preserve unique textures or not
	 */
	bool shouldPreserveUniqueTextures();
	/*! Sets, whether we should preserve unique textures or not
		\param[in] flag a value
	 */
	void toggleShouldPreserveUniqueTextures(bool flag);
protected:
	/*! Whether reader should preserve a unique textures
	 */
	bool m_preserve_unique_textures;
};
