#include "imageformats/tgaloader.h"

bool sad::imageformats::TGALoader::load(FILE * file, sad::Texture * texture)
{
	if (file == NULL || texture == NULL)
		return false;

	return true;
}


sad::imageformats::TGALoader::~TGALoader()
{

}
