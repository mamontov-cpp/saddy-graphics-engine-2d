/*! \file texture.h
	\author HiddenSeeker

	A mini-texture class, needed to provide lazy upload of texture for building a
	call list for freetype
 */
#pragma once
#include <ft2build.h>
#include <freetype/freetype.h>
#include <freetype/ftglyph.h>
#include <freetype/ftoutln.h>
#include <freetype/fttrigon.h>
#include <vector>

namespace sad
{

namespace freetype
{
/*! A mini-texture class, needed to provide lazy upload of textures
 */
class Texture
{
public:
	/*! A pixels, stored in RGB2
	 */
	std::vector<unsigned char> Pixels;
	/*! A width of texture
	 */
	unsigned int Width;
	/*! A height of texture
	 */
	unsigned int Height;

	/*! A width of bitmap, stored in texture
	 */
	unsigned int BitmapWidth;
	/*! A rows of bitmap, stored in texture
	 */
	unsigned int BitmapRows;
	
	/*! Determines, whether texture is uploaded on GPU
	 */
	bool IsOnGPU;
	/*! An Id of Texture on GPU
	 */
	unsigned int  Id;

	/*! Creates empty texture
	 */
	Texture();
	/*! Frees a texture id if on GPU
	 */
	~Texture();
	
	/*! Stores a bitmap in texture, providing necessary operations
		\param[in] bitmap a bitmap to store
	 */
	void storeBitmap(FT_Bitmap & bitmap);
	/*! Uploads a texture, cleaning pixel field
	 */
	void upload();
	/*! Marks texture as current
	 */
	void bind();
};

}

}
