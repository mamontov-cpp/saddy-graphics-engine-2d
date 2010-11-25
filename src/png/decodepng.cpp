#include "png/zlib.h"
#include "png/png.h"

void png::decode(std::vector<unsigned char>& out, const unsigned char* in, unsigned long size)
{
	  error() = 0;
	  info().palette.clear();
      if(size == 0 || in == 0) { error() = 48; return; } //the given data is empty
	  png::readHeader(&(in[0]), size); if(error()) return;
      size_t pos = 33; //first byte of the first chunk after the header
      std::vector<unsigned char> idat; //the data from idat chunks
      bool IEND = false, known_type = true;
      info().key_defined = false;
      while(!IEND) //loop through the chunks, ignoring unknown chunks and stopping at IEND chunk. IDAT data is put at the start of the in buffer
      {
        if(pos + 8 >= size) { error() = 30; return; } //error: size of the in buffer too small to contain next chunk
		size_t chunkLength = bitstream::read32((const bytestream)(&in[pos])); pos += 4;
        if(chunkLength > 2147483647) { error() = 63; return; }
        if(pos + chunkLength >= size) { error() = 35; return; } //error: size of the in buffer too small to contain next chunk
        if(in[pos + 0] == 'I' && in[pos + 1] == 'D' && in[pos + 2] == 'A' && in[pos + 3] == 'T') //IDAT chunk, containing compressed image data
        {
          idat.insert(idat.end(), &in[pos + 4], &in[pos + 4 + chunkLength]);
          pos += (4 + chunkLength);
        }
        else if(in[pos + 0] == 'I' && in[pos + 1] == 'E' && in[pos + 2] == 'N' && in[pos + 3] == 'D')  { pos += 4; IEND = true; }
        else if(in[pos + 0] == 'P' && in[pos + 1] == 'L' && in[pos + 2] == 'T' && in[pos + 3] == 'E') //palette chunk (PLTE)
        {
          pos += 4; //go after the 4 letters
          info().palette.resize(4 * (chunkLength / 3));
          if(info().palette.size() > (4 * 256)) { error() = 38; return; } //error: palette too big
          for(size_t i = 0; i < info().palette.size(); i += 4)
          {
            for(size_t j = 0; j < 3; j++) info().palette[i + j] = in[pos++]; //RGB
            info().palette[i + 3] = 255; //alpha
          }
        }
        else if(in[pos + 0] == 't' && in[pos + 1] == 'R' && in[pos + 2] == 'N' && in[pos + 3] == 'S') //palette transparency chunk (tRNS)
        {
          pos += 4; //go after the 4 letters
          if(info().colorType == 3)
          {
            if(4 * chunkLength > info().palette.size()) { error() = 39; return; } //error: more alpha values given than there are palette entries
            for(size_t i = 0; i < chunkLength; i++) info().palette[4 * i + 3] = in[pos++];
          }
          else if(info().colorType == 0)
          {
            if(chunkLength != 2) { error() = 40; return; } //error: this chunk must be 2 bytes for greyscale image
            info().key_defined = 1; info().key_r = info().key_g = info().key_b = 256 * in[pos] + in[pos + 1]; pos += 2;
          }
          else if(info().colorType == 2)
          {
            if(chunkLength != 6) { error() = 41; return; } //error: this chunk must be 6 bytes for RGB image
            info().key_defined = 1;
            info().key_r = 256 * in[pos] + in[pos + 1]; pos += 2;
            info().key_g = 256 * in[pos] + in[pos + 1]; pos += 2;
            info().key_b = 256 * in[pos] + in[pos + 1]; pos += 2;
          }
          else { error() = 42; return; } //error: tRNS chunk not allowed for other color models
        }
        else //it's not an implemented chunk type, so ignore it: skip over the data
        {
          if(!(in[pos + 0] & 32)) 
		  { 
			  error() = 69; 
			  return; 
		  } //error: unknown critical chunk (5th bit of first byte of chunk type is 0)
          pos += (chunkLength + 4); //skip 4 letters and uninterpreted data of unimplemented chunk
          known_type = false;
        }
        pos += 4; //step over CRC (which is ignored)
      }
	  unsigned long bpp = png::bpp(png::info());
      std::vector<unsigned char> scanlines(((info().width * (info().height * bpp + 7)) / 8) + info().height); //now the out buffer will be filled
      error() = zlib::decompress(scanlines, idat); 
	  if(error()) return;    //stop if the zlib decompressor returned an error
      size_t bytewidth = (bpp + 7) / 8, outlength = (info().height * info().width * bpp + 7) / 8;
      out.resize(outlength); //time to fill the out buffer
      unsigned char* out_ = outlength ? &out[0] : 0; //use a regular pointer to the std::vector for faster code if compiled without optimization
      if(info().interlaceMethod == 0) //no interlace, just filter
      {
        size_t linestart = 0, linelength = (info().width * bpp + 7) / 8; //length in bytes of a scanline, excluding the filtertype byte
        if(bpp >= 8) //byte per byte
        for(unsigned long y = 0; y < info().height; y++)
        {
          unsigned long filterType = scanlines[linestart];
          const unsigned char* prevline = (y == 0) ? 0 : &out_[(y - 1) * info().width * bytewidth];
		  png::unfilter(&out_[linestart - y], &scanlines[linestart + 1], prevline, bytewidth, filterType,  linelength); 
		  if (error()) return;
          linestart += (1 + linelength); //go to start of next scanline
        }
        else //less than 8 bits per pixel, so fill it up bit per bit
        {
          std::vector<unsigned char> templine((info().width * bpp + 7) >> 3); //only used if bpp < 8
          for(size_t y = 0, obp = 0; y < info().height; y++)
          {
            unsigned long filterType = scanlines[linestart];
            const unsigned char* prevline = (y == 0) ? 0 : &out_[(y - 1) * info().width * bytewidth];
			png::unfilter(&templine[0], &scanlines[linestart + 1], prevline, bytewidth, filterType, linelength); 
			if(error()) return;
			
			for(size_t bp = 0; bp < info().width * bpp;) 
				bitstream::setBit(obp, out_, bitstream::readRevBit(bp, (const bytestream)(&templine[0])));
            
			linestart += (1 + linelength); //go to start of next scanline
          }
        }
      }
      else //Adam 7 interlacing
      {
        size_t passw[7] = { (info().width + 7) / 8, (info().width + 3) / 8, (info().width + 3) / 4, (info().width + 1) / 4, (info().width + 1) / 2, (info().width + 0) / 2, (info().width + 0) / 1 };
        size_t passh[7] = { (info().height + 7) / 8, (info().height + 7) / 8, (info().height + 3) / 8, (info().height + 3) / 4, (info().height + 1) / 4, (info().height + 1) / 2, (info().height + 0) / 2 };
        size_t passstart[7] = {0};
        size_t pattern[28] = {0,4,0,2,0,1,0,0,0,4,0,2,0,1,8,8,4,4,2,2,1,8,8,8,4,4,2,2}; //values for the adam7 passes
        for(int i = 0; i < 6; i++) passstart[i + 1] = passstart[i] + passh[i] * ((passw[i] ? 1 : 0) + (passw[i] * bpp + 7) / 8);
        std::vector<unsigned char> scanlineo((info().width * bpp + 7) / 8), scanlinen((info().width * bpp + 7) / 8); //"old" and "new" scanline
        for(int i = 0; i < 7; i++)
			png::adam7deinterlace(&out_[0], &scanlinen[0], &scanlineo[0], &scanlines[passstart[i]], info().width, pattern[i], pattern[i + 7], pattern[i + 14], pattern[i + 21], passw[i], passh[i], bpp);
      }
      if(info().colorType != 6 || info().bitDepth != 8) //conversion needed
      {
        std::vector<unsigned char> data = out;
		error() = png::convert(out, &data[0], info(), info().width, info().height);
      }
}

#include <iostream>
#include <fstream>

void png::loadFile(std::vector<unsigned char> & buffer, const char * filename)
{
  std::ifstream file(filename, std::ios::in|std::ios::binary|std::ios::ate);

  //get filesize
  std::streamsize size = 0;
  if(file.seekg(0, std::ios::end).good()) size = file.tellg();
  if(file.seekg(0, std::ios::beg).good()) size -= file.tellg();

  //read contents of the file into the vector
  if(size > 0)
  {
    buffer.resize((size_t)size);
    file.read((char*)(&buffer[0]), size);
  }
  else buffer.clear();
}


void png::loadFile(std::vector<unsigned char> & buffer, const wchar_t * filename)
{
#ifdef WIN32 
  std::ifstream file(filename, std::ios::in|std::ios::binary|std::ios::ate);
#else
  int len=wcslen(filename)+1;
  char * tmp=new char[len*2];
  wcstombs(tmp,filename,5000);
  std::ifstream file(tmp, std::ios::in|std::ios::binary|std::ios::ate);
  delete tmp;
#endif
  //get filesize
  std::streamsize size = 0;
  if(file.seekg(0, std::ios::end).good()) size = file.tellg();
  if(file.seekg(0, std::ios::beg).good()) size -= file.tellg();

  //read contents of the file into the vector
  if(size > 0)
  {
    buffer.resize((size_t)size);
    file.read((char*)(&buffer[0]), size);
  }
  else buffer.clear();
}
