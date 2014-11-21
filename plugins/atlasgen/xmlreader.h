/*! \file xmlreader.h
    \author HiddenSeeker

    Defines a reader, which reads an atlas from XML
 */
#pragma once
#include "atlas.h"

#include <QtXml/QDomDocument>
#include <QtXml/QDomNode>
#include <QtXml/QDomElement>

/*! A reader, which reads file for XML format
 */
class XMLReader
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
};
