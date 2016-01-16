/*! \file jsonwriter.h
    

    Defines a writer for writing output configs atlases in JSON Format
 */
#pragma once
#include "atlas.h"
#include "writer.h"

#include <QtCore/QFile>
#include <QtCore/QTextStream>

#include "../../include/3rdparty/picojson/valuetotype.h"


/*! Writes output atlas config as JSON
 */
class JSONWriter: public Writer
{
public:
    /*! Constructs new reader
     */
    JSONWriter();
    /*! Can be inherited
     */
    virtual ~JSONWriter();
    /*! Writes an atlas to output file
        \param[in] name a reading name
        \param[in] filename a name for file
        \param[in] outputTexture a file name for output texture
        \param[in] withindex should we write an index to file
        \return result
     */
    virtual bool write(
        const Atlas& atlas,
        const QString& filename,
        const QString& outputTexture,
        bool withindex
    );
};
