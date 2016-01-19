#include "reader.h"

Reader::Reader() : Successfull(false), m_preserve_unique_textures(true)
{

}

Reader::~Reader()
{

}

bool Reader::shouldPreserveUniqueTextures()
{
	return m_preserve_unique_textures;
}

void Reader::toggleShouldPreserveUniqueTextures(bool flag)
{
	m_preserve_unique_textures = flag;
}
