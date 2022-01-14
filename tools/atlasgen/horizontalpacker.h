/*! \file horizontalpacker.h

    A class of packer, which just aligns textures horizontally
 */
#pragma once
#include "packer.h"


/*! A packer, which performs horizontal pack for performing
    image packing
 */
class HorizontalPacker : public Packer
{
public:
    /*! Constructs new packer
     */
    HorizontalPacker() = default;
    /*! Can be inherited
     */
    virtual ~HorizontalPacker() override = default;
    /*! Packs an images to atlas
        \param[in,out] atlas a source atlas
        \param[out] image an image with data of packer
     */
    virtual void pack(Atlas& atlas, QImage*& image) override;
};

