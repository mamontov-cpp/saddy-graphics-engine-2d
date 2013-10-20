#include "imageformats/bmploader.h"

bool sad::imageformats::BMPLoader::load(FILE * file, sad::Texture * texture)
{
	if (file == NULL || texture == NULL)
		return false;

	return true;
}


sad::imageformats::BMPLoader::~BMPLoader()
{

}
