#include "tga/tga.h"

using namespace tga;

// Loading TGA texture.
bool sad::Texture::loadTGA(const hst::string & filename)
{
	bool result = false;	// Result of this function's work
	FILE* hFile;			// File descriptor
	Info textureInfo;		// Object with data of texture.

	hFile = fopen(filename.data(),"rb");					// Open file by reading bytes

	if (hFile)				// If file was opened successfuly
	{
		result = readTGA(hFile, textureInfo);
		fclose(hFile);		// Close the file
	}
	else
	{
        this->loadDefaultTGATexture();
		return false;
	}

	// Copying data of TGA texture to object
	m_bpp = textureInfo.m_TGA_bpp;
	m_height = textureInfo.m_TGA_height;
	m_width = textureInfo.m_TGA_width;
	m_id = sad::globalID++;

	for (UINT i=0; i<textureInfo.m_TGA_imageSize; i++)
		m_data.add( textureInfo.m_TGA_data[i] );

	if (!result)
		this->loadDefaultTGATexture();

	return result;
}

// Loading TGA texture with wide name.
bool sad::Texture::loadTGA(const hst::wstring & filename)
{
	bool result = false;	// Result of this function's work
	FILE* hFile;			// File descriptor
	Info textureInfo;		// Object with data of texture.

#ifdef WIN32
	hFile = _wfopen(filename.data(), L"rb");	// Open file by reading bytes
#else
	char * newfname=new char[2*filename.length()+2);
	wcstombs(newfname,filename.data(),2*filename.length()+2);
	hFile=fopen(newfname,"rb");
	delete newfname;
#endif
	if (hFile)				// If file was opened successfuly
	{
		result = readTGA(hFile, textureInfo);
		fclose(hFile);		// Close the file
	}
	else
	{
        this->loadDefaultTGATexture();
		return false;
	}

	// Copying data of TGA texture to object
	m_bpp = textureInfo.m_TGA_bpp;
	m_height = textureInfo.m_TGA_height;
	m_width = textureInfo.m_TGA_width;
	m_id = sad::globalID++;

	for (UINT i=0; i<textureInfo.m_TGA_imageSize; i++)
		m_data.add( textureInfo.m_TGA_data[i] );

	if (!result)
		sad::Texture::loadDefaultTGATexture();

	return result;
}

// Reads data from TGA file.
bool tga::readTGA(FILE *handler, tga::Info & data)
{
	bool result = true;
	Uint8 headerBuf[18];			// Bytes of TGA header
	TGAHeader header;				// Header of TGA file

	// Reading bytes of header
	fread(headerBuf, sizeof(Uint8), sizeof(headerBuf), handler);

	header = getHeader(headerBuf);	// Get the object of header

	// Fill the jbject of texture's data
	data.m_TGA_bpp	  = header.bitsPerPix;
	data.m_TGA_height = header.height;
	data.m_TGA_width  = header.width;

	data.m_TGA_imageSize = data.m_TGA_height * data.m_TGA_width * data.m_TGA_bpp / 8;
	data.m_TGA_data.resize(data.m_TGA_imageSize);	// Allocate memory for pixels

	if (header.imageType == TGA_TRUECOLOR_RLE)		// If the texture was compressed
		result = loadCompressed(data,handler);

	else if ( header.imageType == TGA_TRUECOLOR )	// Else if the texture wasn't compressed 
		result = loadUnCompressed(data, handler);

	else											// Other types of this texture format isn't supported
		result = false;

	// Convert pixels from BRG or BRGA to RGB or RGBA
	for (UINT i=0;i<data.m_TGA_imageSize; i+=data.m_TGA_bpp / 8)
	{
		data.m_TGA_data[i]^=data.m_TGA_data[i+2]^=data.m_TGA_data[i]^=data.m_TGA_data[i+2];
	}

	return result;
}