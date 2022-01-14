/*! \file growingbinpacker.h
    
    Describes a packer, based on growing bin algorithm
 */
#pragma once
#include "../packer.h"
#include "../texture.h"

namespace growingbinpacker
{
    
/*! A packer, which performs full search for performing
    image packing
 */
class GrowingBinPacker: public Packer
{
public:
/*! A class of texture container for packer 
 */
class T
{
public:
    /*! Creates a new texture wrapper
     */
    T();
    /*! Creates a new texture wrapper for texture
        \param[in] t texture
        \param[in] pad_x horizontal padding
        \param[in] pad_y vertical padding
     */
    T(Texture* t, double pad_x = 0, double pad_y = 0);
    /*! Returns width for texture
        \return width
     */
    double width() const;
    /*! Returns height for texture
        \return height
     */
    double height() const;
    /*! Returns area for texture 
     */
    double area() const;
    /*! "Resizes" area. Just sets inner properties
        \param[in] w width
        \param[in] h height
     */
    void resize(double w, double h);
    /*! Just returns zero
        \param[in] x x value
        \param[in] y y value
     */
    int pixel(int x, int y);
    /*! Does nothing
        \param[in] x x value
        \param[in] y y value
        \param[in] p pixel
     */ 
    void pixel(int x, int y, int p);
    /*! Just returns false
        \param[in] x x value
        \param[in] y y value
     */
    bool is_transparent(int x, int y);
    /*! Copies data
        \param[in] block a block value
        \param[in] x x value
        \param[in] y y value
        \param[in] width width
        \param[in] height height
        \param[in] px pad x
        \param[in] py pad y
     */
    void copy_from(
        growingbinpacker::GrowingBinPacker::T block,
        int x, 
        int y,
        int width,
        int height,
        int px, 
        int py
    );
protected:
    /*! A width for container
     */
    double m_width;
    /*! A height for container
     */
    double m_height;
    /*! A texture 
     */
    Texture* m_t;
};
    /*! Constructs new packer
     */
    GrowingBinPacker();
    /*! Can be inherited
     */
    virtual ~GrowingBinPacker() override;
    /*! Packs an images to atlas
        \param[in,out] atlas a source atlas
        \param[out] image an image with data of packer
     */
    virtual void pack(Atlas& atlas, QImage*& image) override;
};

}
