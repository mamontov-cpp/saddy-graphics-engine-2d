/*! \file xmlreader.h
    

    Defines a reader, which reads an atlas from XML
 */
#pragma once
#include "atlas.h"
#include "reader.h"

// ReSharper disable once CppUnusedIncludeDirective
#include <QtXml/QDomDocument>
#include <QtXml/QDomNode>
// ReSharper disable once CppUnusedIncludeDirective
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
     *  \param[in] name a reading name
     *  \param[in] atlas an atlas to be read
     */
    virtual void read(const QString& name, Atlas* atlas) override;
    /*! Reads a DOM element
     *  \param[in] e element
     *  \param[in] atlas an atlas to be read
     */
    void readElement(const QDomElement& e, Atlas* atlas);
};
