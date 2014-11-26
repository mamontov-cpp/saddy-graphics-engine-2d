/*! \file xlwriter.h
    \author HiddenSeeker

    Defines a writer for writing output configs atlases in XML Format
 */
#pragma once
#include "atlas.h"
#include "writer.h"

#include <QtXml/QDomDocument>
#include <QtXml/QDomNode>
#include <QtXml/QDomElement>


/*! Writes output atlas config as XML
 */
class XMLWriter: public Writer
{
public:
    /*! Constructs new reader
     */
    XMLWriter();
    /*! Can be inherited
     */
    virtual ~XMLWriter();
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
