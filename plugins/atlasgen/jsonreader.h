/*! \file jsonreader.h
    \author HiddenSeeker

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
       \param name a reading name
     */
    virtual void read(const QString& name);
	/*! Reads a DOM element
		\param[in] e element
	 */
	void readElement(const picojson::object& e);
};
