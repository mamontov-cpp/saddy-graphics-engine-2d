#include "png/zlib.h"
#include "png/png.h"



int png::convert(
		         uchrstream & out, 
				 const unsigned char * in, 
				 png::Info & inf, 
				 unsigned long w,
				 unsigned long h
			    )
{
      size_t numpixels=w*h, bp=0;
      out.resize(numpixels*4);
      unsigned char * out_=out.empty()? 0 : &out[0];
 
      if(inf.bitDepth == 8 && inf.colorType == 0) //greyscale
      for(size_t i = 0; i < numpixels; i++)
      {
        out_[4 * i + 0] = out_[4 * i + 1] = out_[4 * i + 2] = in[i];
        out_[4 * i + 3] = (inf.key_defined && in[i] == inf.key_r) ? 0 : 255;
      }
      else if(inf.bitDepth == 8 && inf.colorType == 2) //RGB color
      for(size_t i = 0; i < numpixels; i++)
      {
        for(size_t c = 0; c < 3; c++) out_[4 * i + c] = in[3 * i + c];
        out_[4 * i + 3] = (inf.key_defined == 1 && in[3 * i + 0] == inf.key_r && in[3 * i + 1] == inf.key_g && in[3 * i + 2] == inf.key_b) ? 0 : 255;
      }
      else if(inf.bitDepth == 8 && inf.colorType == 3) //indexed color (palette)
      for(size_t i = 0; i < numpixels; i++)
      {
        if(4U * in[i] >= inf.palette.size()) return 46;
        for(size_t c = 0; c < 4; c++) out_[4 * i + c] = inf.palette[4 * in[i] + c]; //get rgb colors from the palette
      }
      else if(inf.bitDepth == 8 && inf.colorType == 4) //greyscale with alpha
      for(size_t i = 0; i < numpixels; i++)
      {
        out_[4 * i + 0] = out_[4 * i + 1] = out_[4 * i + 2] = in[2 * i + 0];
        out_[4 * i + 3] = in[2 * i + 1];
      }
      else if(inf.bitDepth == 8 && inf.colorType == 6) for(size_t i = 0; i < numpixels; i++) for(size_t c = 0; c < 4; c++) out_[4 * i + c] = in[4 * i + c]; //RGB with alpha
      else if(inf.bitDepth == 16 && inf.colorType == 0) //greyscale
      for(size_t i = 0; i < numpixels; i++)
      {
        out_[4 * i + 0] = out_[4 * i + 1] = out_[4 * i + 2] = in[2 * i];
        out_[4 * i + 3] = (inf.key_defined && 256U * in[i] + in[i + 1] == inf.key_r) ? 0 : 255;
      }
      else if(inf.bitDepth == 16 && inf.colorType == 2) //RGB color
      for(size_t i = 0; i < numpixels; i++)
      {
        for(size_t c = 0; c < 3; c++) out_[4 * i + c] = in[6 * i + 2 * c];
        out_[4 * i + 3] = (inf.key_defined && 256U*in[6*i+0]+in[6*i+1] == inf.key_r && 256U*in[6*i+2]+in[6*i+3] == inf.key_g && 256U*in[6*i+4]+in[6*i+5] == inf.key_b) ? 0 : 255;
      }
      else if(inf.bitDepth == 16 && inf.colorType == 4) //greyscale with alpha
      for(size_t i = 0; i < numpixels; i++)
      {
        out_[4 * i + 0] = out_[4 * i + 1] = out_[4 * i + 2] = in[4 * i]; //most significant byte
        out_[4 * i + 3] = in[4 * i + 2];
      }
      else if(inf.bitDepth == 16 && inf.colorType == 6) for(size_t i = 0; i < numpixels; i++) for(size_t c = 0; c < 4; c++) out_[4 * i + c] = in[8 * i + 2 * c]; //RGB with alpha
      else if(inf.bitDepth < 8 && inf.colorType == 0) //greyscale
      for(size_t i = 0; i < numpixels; i++)
      {
		unsigned long value = (bitstream::readRevBits(bp, (const bytestream)in, inf.bitDepth) * 255) / ((1 << inf.bitDepth) - 1); //scale value from 0 to 255
        out_[4 * i + 0] = out_[4 * i + 1] = out_[4 * i + 2] = (unsigned char)(value);
        out_[4 * i + 3] = (inf.key_defined && value && ((1U << inf.bitDepth) - 1U) == inf.key_r && ((1U << inf.bitDepth) - 1U)) ? 0 : 255;
      }
      else if(inf.bitDepth < 8 && inf.colorType == 3) //palette
      for(size_t i = 0; i < numpixels; i++)
      {
		unsigned long value = bitstream::readRevBits(bp, (const bytestream)in, inf.bitDepth);
        if(4 * value >= inf.palette.size()) return 47;
        for(size_t c = 0; c < 4; c++) out_[4 * i + c] = inf.palette[4 * value + c]; //get rgb colors from the palette
      }
      return 0;
}