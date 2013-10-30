#include "texture.h"
#include "nextpoweroftwo.h"

#ifdef WIN32
	#define NOMINMAX
	#include <windows.h>
#endif
#include <GL/gl.h>
#include <GL/glu.h>

sad::freetype::Texture::Texture()
: Width(0), Height(0), BitmapWidth(0), BitmapRows(0), IsOnGPU(false), Id(0)
{

}

sad::freetype::Texture::~Texture()
{
	if (IsOnGPU)
	{
		glDeleteTextures(1, &Id);
	}
}

void sad::freetype::Texture::storeBitmap(FT_Bitmap & bitmap)
{
	// storeBitmap is not re-entrant
	if (IsOnGPU)
		return;

	Width = sad::freetype::next_power_of_two( bitmap.width );
	Height = sad::freetype::next_power_of_two( bitmap.rows );
	Pixels.resize(2 * Width * Height);

	//Fill the data
	for(unsigned int j = 0; j < Height; j++) 
	{
		for(unsigned int i = 0; i < Width; i++)
		{
			Pixels[2*(i+j*Width)]= Pixels[2*(i+j*Width)+1] = 
				((int)i>=bitmap.width || (int)j>=bitmap.rows) ?
				0 : bitmap.buffer[i + bitmap.width*j];
		}
	}
}


void sad::freetype::Texture::upload()
{
	// upload
	if (IsOnGPU)
		return;

	IsOnGPU = true;
	
	glGenTextures( 1, &Id );

	glBindTexture( GL_TEXTURE_2D, Id);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	
	glTexImage2D( GL_TEXTURE_2D, 
				  0, 
				  GL_RGBA2, 
				  Width, 
				  Height, 
				  0, 
				  GL_LUMINANCE_ALPHA, 
				  GL_UNSIGNED_BYTE, 
				  &(Pixels[0]) 
				);	
}

void sad::freetype::Texture::bind()
{
	glBindTexture( GL_TEXTURE_2D, Id);
}
