/*! \file jsonreader.h
    

    Defines a reader, which reads an atlas from JSON
 */
#pragma once
#include "atlas.h"
#include "reader.h"

// ReSharper disable once CppUnusedIncludeDirective
#include <QtCore/QFile>
// ReSharper disable once CppUnusedIncludeDirective
#include <QtCore/QString>
#include <QtCore/QStringList>

#include "../../include/3rdparty/picojson/valuetotype.h"

class QImage;

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
     *  \param[in,out] atlas an  atlas to be read
     */
    virtual void read(const QString& name, Atlas* atlas) override;
    /*! Reads file as already created atlas
     *  \param[in] name an input name
     *  \param[in] ro object
     *  \param[in,out] atlas an atlas to be read
     */
    void readAsAtlas(const QString& name, picojson::object& ro, Atlas* atlas);	
    /*! Reads file as configuration for atlas generation
     *  \param[in] ro object
     *  \param[in,out] atlas an atlas to be read
     */
    void readAsAtlasConfig(picojson::object& ro, Atlas* atlas);
    /*! Reads a json element, which describes atlas config element
     *  \param[in] e element
     *  \param[in] atlas an atlas to be read
     */
    void readElement(picojson::object& e, Atlas* atlas);
    /*! Reads a json element, which describes atlas element
     *  \param[in] e element
     *  \param[in] atlas an atlas to be read
     *  \param[in] large a large image, which contains atlas
     */
    void readAtlasElement(picojson::object& e, Atlas* atlas, QImage& large);
    /**
     * Removes extension from name
     * \param[in] name string, from which extension should be removed
     * \return name parts
     */
    static QStringList removeExtension(const QString& name);
};
