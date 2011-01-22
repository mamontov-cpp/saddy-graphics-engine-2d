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
	size_t res=fread(&data.m_TGA_data[0], 1, data.m_TGA_imageSize, hFile);
	if (res != data.m_TGA_imageSize)
		return false;
		
  return true;
}

// Reading and decode pixels 
bool tga::loadCompressed(tga::Info & data, FILE *hFile)
{
	bool result = true;				// Result
	fpos_t currentPos, eofPos;		// Current position in file and EOF 
	size_t RLESize;					// Number of compressed bytes
	unsigned char *RLEBuffer;				// Pointer to array of compressed bytes
	unsigned int count   = 0,				// Counter of pixels			
		 rcount  = 1,				// Counter of RLE pixels
		 rloop	 = 0,				// Length of the block
		 pakleft = 0;				// Number of pixels in the current block
	bool flag;
	unsigned int bpp8 = data.m_TGA_bpp / 8;	// Bytes per pixel

	fgetpos(hFile, &currentPos);					// Save the current position

	fseek(hFile, 0, SEEK_END);						// Move to the end of file

	fgetpos(hFile,&eofPos);							// Save the position
#ifdef WIN32
	fseek(hFile, currentPos, SEEK_SET);				// Move back to the current position
#else
	fseek(hFile, *((long*)&currentPos), SEEK_SET);
#endif
	RLESize = (*((size_t*)&eofPos) - *((size_t*)&currentPos)) + 1;	// Save the size of RLE bytes

	RLEBuffer = new unsigned char[(size_t)RLESize];					// Allocate memory for the array of pixels

	fread(RLEBuffer, (size_t)1, *((size_t*)&eofPos)+1, hFile); // Reading pixels

	pakleft=1+RLEBuffer[rcount-1];					// Bytes before the next block

	size_t curindex=0;     //Pointer in RLE
	size_t dataptr=0;    //Pointer in texturedata
	while (curindex<RLESize && dataptr<data.m_TGA_imageSize)
	{
	  if (RLEBuffer[curindex]<128) //RAW section
	  {
		  unsigned char raw=RLEBuffer[curindex]+1;//RAW buffer size;
		  curindex+=1;                            //To next byte
		  if (RLESize-curindex<bpp8)              //Handle buffer error
		  { delete RLEBuffer; return false; }
          //Copy data
		  for (unsigned char i=0;i<raw;i++)
		  {
             data.m_TGA_data[dataptr++]=RLEBuffer[curindex];      //Copy R
			 data.m_TGA_data[dataptr++]=RLEBuffer[curindex+1];    //Copy G
			 data.m_TGA_data[dataptr++]=RLEBuffer[curindex+2];    //Copy B		 
			 if (bpp8==4) { data.m_TGA_data[dataptr++]=RLEBuffer[curindex+3]; } //Copy A
             
			 curindex+=bpp8;
		  }
	  }
	  else            //RLE section
	  {
		  unsigned char rle=RLEBuffer[curindex]-127;//RLE component size;
		  curindex+=1;                              //To next byte
		  unsigned char r=RLEBuffer[curindex],g=RLEBuffer[curindex+1],
		  	            b=RLEBuffer[curindex+2],a=0;
		  if (bpp8==4) a=RLEBuffer[curindex+3];

		  for (unsigned char i=0;i<rle;i++)
		  {
			  data.m_TGA_data[dataptr++]=r;
			  data.m_TGA_data[dataptr++]=g;
			  data.m_TGA_data[dataptr++]=b;
              if (bpp8==4) 
			  { data.m_TGA_data[dataptr++]=a; } 
             
		  }

		  curindex+=bpp8;
	  }
	}
	result=true;
	
    delete RLEBuffer;  // Frees memory of RLE pixels

	return result;
}
