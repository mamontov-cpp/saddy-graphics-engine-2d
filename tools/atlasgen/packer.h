/*! \file packer.h
    
    Describes a packer for algorithm
 */
#pragma once
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
        \param[in,out] atlas a souce atlas
        \param[out] image an image with data of packer
     */
    virtual void pack(Atlas& atlas, QImage*& image) = 0;
    /*! Should be inherited
     */
    virtual ~Packer();
};