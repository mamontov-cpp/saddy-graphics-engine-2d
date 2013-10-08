#include "png/picopngloader.h"
#include <vector>

PicoPNGTextureLoader::~PicoPNGTextureLoader()
{

}

int decodePNG(std::vector<unsigned char>& out_image, unsigned long& image_width, unsigned long& image_height, const unsigned char* in_png, size_t in_size, bool convert_to_rgba32 = true);

bool PicoPNGTextureLoader::load(FILE * file, sad::Texture * texture)
{
	std::vector<unsigned char> buffer;
	fseek(file, 0L, SEEK_END);
	unsigned int size = ftell(file);
	fseek(file, 0L, SEEK_SET);
	buffer.resize((size_t)size);
	fread((char*)(&buffer[0]), 1, size, file);

	std::vector<unsigned char> outimg;
	unsigned long width = 0;
	unsigned long height = 0;

	int result = decodePNG(outimg,width, height, &(buffer[0]), buffer.size());

	texture->width() = width;
	texture->height() = height;
	texture->bpp() = 32;

	bool ok = result == 0;
	if (ok)
	{
		texture->vdata().rescale(outimg.size());
		memcpy(&(texture->vdata()[0]), &(outimg[0]), outimg.size() * sizeof(sad::uchar));
	}
	return ok;
}

