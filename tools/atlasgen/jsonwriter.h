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
        \param[in, out] opts options
        \return result
     */
    virtual bool write(
        const Atlas& atlas,
        OutputOptions& opts
    );
};
