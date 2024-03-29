/*! \file glyph.h
    

    Describes a glyph as a bitmap and it's metric
 */
#pragma once
#include "texture.h"
#include "sadstring.h"
#include "../../include/sadrect.h"

#include <maybe.h>

namespace sad
{

namespace freetype
{

class Glyph
{
public:
    /*! A tangent for 20 degrees
     */
    static const float tan_20_degrees;
    /*! A texture as a bitmap to be used, when rendering
     */
    sad::freetype::Texture Texture;
    /*! An  index of char, which can be passed as index of char in freetype
     */
    unsigned int  Index;
    /*! Bitmap width
     */
    double  Width;
    /*! Bitmap height
     */
    double  Height;
    /*! A width in texture coordinates
     */
    double TexCoordinateWidth;
    /*! A height in texture coordinates
     */
    double TexCoordinateHeight;
    /*! Texture rectangle for rendering glyph
     */
    sad::Rect2D TextureRectangle;
    /*! A vertical bearing  as
        distance from baseline to top point of glyph
     */ 
    double BearingY;
    /*! A vertical distance from baseline to bottom point
        of glyph
     */
    double Descender;
    /*! How long should pen move, after glyph was rendered
     */
    double AdvanceX;
    /*! Inner glyph
     */
    void* Data;

    /*! Maximum Y of bounding box
     */
    int YMax;
    /*! Minimum Y of bounding box
     */
    int YMin;

    /*! Default glyph fills with zeroes
     */
    Glyph();

    /*! Creates a new glyph for specified character, building a texture for it
        \param[in] face a face
        \param[in] c a character, which is stored in glyph
        \param[in] store_texture whether we should store texture
     */
    Glyph(FT_Face face, unsigned char c, bool store_texture);
    /*! Renders a glyph at specified baseline position. Note, that this
        is BASELINE position.
        \param[in] x X coordinate position
        \param[in] y Y coordinate position
        \param[in] top_offset a top offset for italic font
     */
    void render(float x, float y, float top_offset);
    /*! Fills geometries
        \param[in] x X coordinate position
        \param[in] y Y coordinate position
        \param[in] top_offset a top offset for italic font
        \param[in] vertexes vertexes
        \param[in] tcs texture coordinates
     */
    void fillGeometries(double x, double y, double top_offset, sad::Vector<double>& vertexes, sad::Vector<double>& tcs) const;
    /*! Tries to get a glyph for a face and char c
        \param[in] face face to be used
        \param[in] c character
        \param[out] index index of character in list
        \return glyph value
     */
    static sad::Maybe<FT_Glyph> glyph(FT_Face face, unsigned char c, unsigned int & index);
    /*! Dumps parameters to string
        \return parameters
     */
    sad::String dumpParameters() const;
    /*! Frees inner glyph
     */
    void freeInnerGlyph();
private:
    /*! Sets all metrics of glyph from specified freetype glyph
        \param[in] face  a global face
        \param[in] glyph a freetype glyph
        \param[in] store_texture whether we should store texture
     */
    void makeGlyph(FT_Face face, FT_Glyph glyph, bool store_texture);
    /*! Makes empty glyph, if no glyph data available
     */
    void makeEmptyGlyph();
};

}

}
