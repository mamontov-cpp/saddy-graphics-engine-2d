/*! \file packer.h
    
    Describes a packer for algorithm
 */
#pragma once
// ReSharper disable once CppUnusedIncludeDirective
#include <QImage>
#include "atlas.h"

/*! An abstract packer algorithm 
 */
class Packer
{
public:
    /*! A packer data
     */
    Packer();
    /*! Packs an images to atlas
        \param[in,out] atlas a source atlas
        \param[out] image an image with data of packer
     */
    virtual void pack(Atlas& atlas, QImage*& image) = 0;
    /*! Sets options for packer
        \param[in] options an inner options
     */
    void setOptions(QHash<QString, QVariant>* options);
    /*! Should be inherited
     */
    virtual ~Packer();
protected:
    /*! Copies images from specified atlas to image
        \param[in] atlas an atlas
        \param[out] image an image
     */
    void copyImages(Atlas& atlas, QImage*& image);
    /*! A link to options, which could alter behaviour of packer
     */
    QHash<QString, QVariant>* m_options;
};
