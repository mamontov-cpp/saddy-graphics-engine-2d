#include "tga/tga.h"


using namespace tga;

void  sad::Texture::copyTGA(const tga::Info & textureInfo)
{
 unsigned int startrow=0;
 unsigned int decrow=0;
 if (textureInfo.m_vertflip
	 || (!textureInfo.m_vertflip && !textureInfo.m_vertflip) //Handle zeroimage descriptor
	 )  {startrow=(m_height-1)*(m_width)*(m_bpp/8); decrow=-1*m_width*(m_bpp/8); } 
 else                        {startrow=0; decrow=m_width*(m_bpp/8); }
	
 unsigned int startcol=0;
 unsigned int deccol;
 if (textureInfo.m_horzflip) {startcol=m_width-m_bpp/8;deccol=-1*m_bpp/8;}
 else                        {startcol=0; deccol=m_bpp/8;}
	
 unsigned int rowcheckval=m_height*(m_width*m_bpp/8);
 unsigned int colcheckval=m_width*(m_bpp/8);
#define CHECKROW (i>=0) && (i<rowcheckval)
#define CHECKCOL (j>=0) && (j<colcheckval)
 m_data.resize(textureInfo.m_TGA_height * textureInfo.m_TGA_width * 4);
 unsigned int texpos = 0;
 for (unsigned int i=startrow;CHECKROW;i+=decrow )
 {
		for (unsigned int j=startcol;CHECKCOL;j+=deccol)
		{
			unsigned int offset = i+j;
			memcpy(&(m_data[texpos]), &(textureInfo.m_TGA_data[offset]), 3 * sizeof(Uint8));
			texpos+=3;
			if (m_bpp==32) 
			{	
				m_data[texpos] = textureInfo.m_TGA_data[offset + 3];
			}
			else
			{
				m_data[texpos] = 255;
			}
		    texpos += 1; 			
		}
 }
#undef CHECKROW
#undef CHECKCOL
 
}
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
	    m_data.clear();
        this->loadDefaultTGATexture();
		return false;
	}

	// Copying data of TGA texture to object
	m_bpp = textureInfo.m_TGA_bpp;
	m_height = textureInfo.m_TGA_height;
	m_width = textureInfo.m_TGA_width;
    
	if (result)
	{
		copyTGA(textureInfo);
		if (m_bpp==24) m_bpp=32;
	}
	

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
#ifndef MINGW32
	hFile = _wfopen(filename.data(), L"rb");	// Open file by reading bytes
#else
	char * newfname=new char[2*filename.length()+2];
	wcstombs(newfname,filename.data(),2*filename.length()+2);
	hFile=fopen(newfname,"rb");
	delete newfname;
#endif
#else
	char * newfname=new char[2*filename.length()+2];
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

	if (result)
	{
		copyTGA(textureInfo);
		if (m_bpp==24) m_bpp=32;
	}

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
	data.m_vertflip   = ( header.imageDescriptor & 32 )?true:false;
    data.m_horzflip   = ( header.imageDescriptor & 16 )?true:false;

	data.m_TGA_imageSize = data.m_TGA_height * data.m_TGA_width * data.m_TGA_bpp / 8;
	data.m_TGA_data.resize(data.m_TGA_imageSize);	// Allocate memory for pixels

	if (header.imageType == TGA_TRUECOLOR_RLE)		// If the texture was compressed
		result = loadCompressed(data,handler);

	else if ( header.imageType == TGA_TRUECOLOR )	// Else if the texture wasn't compressed 
		result = loadUnCompressed(data, handler);

	else											// Other types of this texture format isn't supported
		result = false;

	// Convert pixels from BRG or BRGA to RGB or RGBA
	for (unsigned int i=0;i<data.m_TGA_imageSize; i+=data.m_TGA_bpp / 8)
	{
	    Uint8 t=data.m_TGA_data[i];
		data.m_TGA_data[i]=data.m_TGA_data[i+2];
		data.m_TGA_data[i+2]=t;
	}

	return result;
}


sad::TGATextureLoader::~TGATextureLoader()
{


}

bool sad::TGATextureLoader::load(FILE * file, sad::Texture * texture)
{
	tga::Info textureInfo;		// Object with data of texture.
	bool result = readTGA(file, textureInfo);
	texture->vdata().clear();
	if (result)
	{
		texture->bpp() = textureInfo.m_TGA_bpp;
		texture->height() = textureInfo.m_TGA_height;
		texture->width() = textureInfo.m_TGA_width;
		texture->copyTGA(textureInfo);
		if (texture->bpp()==24) texture->bpp()=32;
	}
	else
	{
		texture->vdata().clear();
	}
	return result;
}