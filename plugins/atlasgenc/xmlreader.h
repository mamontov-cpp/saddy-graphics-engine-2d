/*! \file xmlreader.h
    

    Defines a reader, which reads an atlas from XML
 */
#pragma once
#include "atlas.h"
#include "reader.h"

#include <QtXml/QDomDocument>
#include <QtXml/QDomNode>
#include <QtXml/QDomElement>

/*! A reader, which reads file for XML format
 */
class XMLReader: public Reader 
{
public:
    /*! Constructs new reader
     */
    XMLReader();
    /*! Destroys a reader
     */
    virtual ~XMLReader();
    /*! Reads a file with specified name
       \param name a reading name
     */
    virtual void read(const QString& name);
	/*! Reads a DOM element
		\param[in] e element
	 */
	void readElement(const QDomElement& e);
};
