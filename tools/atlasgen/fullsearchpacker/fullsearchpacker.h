/*! \file fullsearchpacker.h
    
    A class of packer, which performs full search for performing image packing
 */
#pragma once
#include "../packer.h"

namespace fullsearchpacker
{
    
/*! A packer, which performs full search for performing
    image packing
 */
class FullSearchPacker: public Packer
{
public:
    /*! Constructs new packer
     */
    FullSearchPacker();
    /*! Can be inherited
     */
    virtual ~FullSearchPacker() override;
    /*! Packs an images to atlas
        \param[in,out] atlas a source atlas
        \param[out] image an image with data of packer
     */
    virtual void pack(Atlas& atlas, QImage*& image) override;
};

}
