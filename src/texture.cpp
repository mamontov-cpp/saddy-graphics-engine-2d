#include "texture.h"
using namespace sad;

Texture::Texture()
{
	m_mode = BORDER_REPEAT;	/*!< Set default mode of texture.		*/
	m_filter = 10;			/*!< Set default filter of texture.		*/
}

Texture::~Texture()
{
	m_data.clear();			/*!< Cleaning the container of pixels.  */
}
