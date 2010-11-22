#include "tga/tga.h"

using namespace tga;

// Writes bytes of TGA header to object
TGAHeader tga::getHeader(Uint8 *buf)
{
	TGAHeader newHeader;

	// Reading bytes
    newHeader.idLength			= buf[0];
	newHeader.colorMapType		= buf[1];
	newHeader.imageType			= buf[2];
	newHeader.colorMapOrigin	= buf[3]  + 256 * buf[4];
	newHeader.colorMapSize		= buf[5]  + 256 * buf[6];
    newHeader.colorMapEntrySize	= buf[7];
    newHeader.XOrigin			= buf[8]  + 256 * buf[9];
	newHeader.YOrigin			= buf[10] + 256 * buf[11];
	newHeader.width				= buf[12] + 256 * buf[13];
	newHeader.height			= buf[14] + 256 * buf[15];
	newHeader.bitsPerPix		= buf[16];
    newHeader.imageDescriptor	= buf[17];

	return newHeader;
}

// Reading pixels from texture's file
bool tga::loadUnCompressed(tga::Info & data, FILE *hFile)
{
	if (fread(&data.m_TGA_data[0], 1, data.m_TGA_imageSize, hFile) != data.m_TGA_imageSize)
		return false;

	return true;
}

// Reading and decode pixels 
bool tga::loadCompressed(tga::Info & data, FILE *hFile)
{
	bool result = true;				// Result
	fpos_t currentPos, eofPos;		// Current position in file and EOF 
	size_t RLESize;					// Number of compressed bytes
	BYTE *RLEBuffer;				// Pointer to array of compressed bytes
	UINT count   = 0,				// Counter of pixels			
		 rcount  = 1,				// Counter of RLE pixels
		 rloop	 = 0,				// Length of the block
		 pakleft = 0;				// Number of pixels in the current block
	bool flag;

	UINT bpp8 = data.m_TGA_bpp / 8;	// Bytes per pixel

	fgetpos(hFile, &currentPos);					// Save the current position

	fseek(hFile, 0, SEEK_END);						// Move to the end of file

	fgetpos(hFile,&eofPos);							// Save the position

	fseek(hFile, currentPos, SEEK_SET);				// Move back to the current position

	RLESize = (size_t)(eofPos - currentPos) + 1;	// Save the size of RLE bytes

	RLEBuffer = new BYTE[RLESize];					// Allocate memory for the array of pixels

	fread(RLEBuffer, (size_t)1, (size_t)eofPos+1, hFile); // Reading pixels

	pakleft=1+RLEBuffer[rcount-1];					// Bytes before the next block

	// Unpacking
	while (count < (data.m_TGA_width * data.m_TGA_height - 1) * bpp8)
	{
		flag = false;

		// If the next block
		if (pakleft==0)
		{
			// Reading a block
			while (RLEBuffer[rcount] > 0x7F && !flag)
			{
				// block size
				rloop = 1 + (RLEBuffer[rcount] & 0x7F);

				// If the size is more, than we needed
				if ((rloop*bpp8+count)>=(data.m_TGA_width*data.m_TGA_height-1)*bpp8)
				{
					rloop=(data.m_TGA_width*data.m_TGA_height-1)-count/bpp8;
				}

				rcount++;

				for (int I=0;I<rloop;I++)
				{
					memcpy(&data.m_TGA_data[count],RLEBuffer+rcount,bpp8);	// Copyes a block	

					count += bpp8;
				}

				rcount += bpp8;	

				if (count>=bpp8*data.m_TGA_width*data.m_TGA_height-1)
					flag = true;	// End of "while"
			}
			
			pakleft=1+RLEBuffer[rcount];	// Bytes before the next block

			rcount++;						// Increment the RLE counter
		}
		else	// Block hasn't ended
		{
			memcpy( &data.m_TGA_data[count], RLEBuffer + rcount, bpp8 * pakleft );	// Copies data

			rcount	+= bpp8 * pakleft;	// Increment the RLE counter
			
			count	+= bpp8 * pakleft;	// Increment the counter
			
			pakleft = 0;
		} 
	} 

    delete RLEBuffer;  // Frees memory of RLE pixels

	return result;
}