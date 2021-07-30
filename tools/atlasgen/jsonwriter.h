/*! \file jsonwriter.h
    

    Defines a writer for writing output configs atlases in JSON Format
 */
#pragma once
#include "atlas.h"
#include "writer.h"

// ReSharper disable once CppUnusedIncludeDirective
#include <QtCore/QFile>
// ReSharper disable once CppUnusedIncludeDirective
#include <QtCore/QTextStream>

// ReSharper disable once CppUnusedIncludeDirective
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
        \param[in] atlas atlas data
        \param[in,out] opts options
        \return result
     */
    virtual bool write(
        const Atlas& atlas,
        OutputOptions& opts
    ) override;
};
