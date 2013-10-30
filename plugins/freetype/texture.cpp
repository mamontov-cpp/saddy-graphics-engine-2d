#include "texture.h"
#include "nextpoweroftwo.h"


sad::freetype::Texture::Texture()
: Width(0), Height(0), IsOnGPU(false), Id(0)
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

	unsigned int w = sad::freetype::next_power_of_two( bitmap.width );
	unsigned int h = sad::freetype::next_power_of_two( bitmap.rows );
	Width = (float)w;
	Height = (float)h;
	Pixels.resize(2 * w * h);

	//Fill the data
	for(unsigned int j = 0; j < h; j++) 
	{
		for(unsigned int i = 0; i < w; i++)
		{
			Pixels[2*(i+j*w)]= Pixels[2*(i+j*w)+1] = 
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
	
	glGenTextures(1, &Id );

	glBindTexture(GL_TEXTURE_2D, Id);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	
	glTexImage2D( GL_TEXTURE_2D, 
				  0, 
				  GL_RGBA2, 
				  (unsigned int)Width, 
				  (unsigned int)Height, 
				  0, 
				  GL_LUMINANCE_ALPHA, 
				  GL_UNSIGNED_BYTE, 
				  &(Pixels[0]) 
				);	
}

void sad::freetype::Texture::bind()
{
	if (IsOnGPU == false)
	{
		upload();
	}

	glBindTexture( GL_TEXTURE_2D, Id);
}
