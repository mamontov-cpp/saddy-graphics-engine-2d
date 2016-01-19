/*! \file jsonreader.h
    

    Defines a reader, which reads an atlas from JSON
 */
#pragma once
#include "atlas.h"
#include "reader.h"

#include <QtCore/QFile>

#include "../../include/3rdparty/picojson/valuetotype.h"

/*! A reader, which reads file for JSON format
 */
class JSONReader: public Reader 
{
public:
    /*! Constructs new reader
     */
    JSONReader();
    /*! Destroys a reader
     */
    virtual ~JSONReader();
    /*! Reads a file with specified name
     *  \param[in] name a reading name
     *  \param[in] atlas an  atlas to be read
     */
    virtual void read(const QString& name, Atlas* atlas);
    /*! Reads a DOM element
     *  \param[in] e element
     *  \param[in] atlas an atlas to be read
     */
    void readElement(picojson::object& e, Atlas* atlas);
};
