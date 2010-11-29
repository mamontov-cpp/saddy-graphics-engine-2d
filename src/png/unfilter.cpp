#include "png/zlib.h"
#include "png/png.h"

void png::unfilter(
		           unsigned char *       rec, 
		           const unsigned char * scanline,
				   const unsigned char * prec,
				   size_t                bwidth,
				   unsigned long         filterType,
				   size_t                length
			      )
{
	switch(filterType)
    {
        case 0: for(size_t i = 0; i < length; i++) rec[i] = scanline[i]; break;
        case 1:
          for(size_t i =         0; i < bwidth; i++) rec[i] = scanline[i];
          for(size_t i = bwidth; i <    length; i++) rec[i] = scanline[i] + rec[i - bwidth];
          break;
        case 2:
          if(prec) for(size_t i = 0; i < length; i++) rec[i] = scanline[i] + prec[i];
          else       for(size_t i = 0; i < length; i++) rec[i] = scanline[i];
          break;
        case 3:
          if(prec)
          {
            for(size_t i =         0; i < bwidth; i++) rec[i] = scanline[i] + prec[i] / 2;
            for(size_t i = bwidth; i <    length; i++) rec[i] = scanline[i] + ((rec[i - bwidth] + prec[i]) / 2);
          }
          else
          {
            for(size_t i =         0; i < bwidth; i++) rec[i] = scanline[i];
            for(size_t i = bwidth; i <    length; i++) rec[i] = scanline[i] + rec[i - bwidth] / 2;
          }
          break;
        case 4:
          if(prec)
          {
            for(size_t i =         0; i < bwidth; i++) rec[i] = (unsigned char)(scanline[i] + paethPredictor(0, prec[i], 0));
            for(size_t i = bwidth; i <    length; i++) rec[i] = (unsigned char)(scanline[i] + paethPredictor(rec[i - bwidth], prec[i], prec[i - bwidth]));
          }
          else
          {
            for(size_t i =         0; i < bwidth; i++) rec[i] = scanline[i];
            for(size_t i = bwidth; i <    length; i++) rec[i] = (unsigned char)(scanline[i] + paethPredictor(rec[i - bwidth], 0, 0));
          }
          break;
		default: png::error()= 36; return; //error: unexisting filter type given
   };
}


void  png::adam7deinterlace(
		                    unsigned char * out,
							unsigned char * linen, 
							const unsigned char * lineo,
						    const unsigned char * in,
							unsigned long w,
							size_t passleft,
							size_t passtop,
							size_t spacex,
							size_t spacey,
							size_t passw,
							size_t passh,
							unsigned long bpp
						   )
{
      if(passw == 0) return;
      size_t bytewidth = (bpp + 7) / 8, linelength = 1 + ((bpp * passw + 7) / 8);
      for(unsigned long y = 0; y < passh; y++)
      {
        unsigned char filterType = in[y * linelength], *prevline = (y == 0) ? 0 : const_cast<unsigned char*>(lineo);
		png::unfilter(linen, &in[y * linelength + 1], prevline, bytewidth, filterType, (w * bpp + 7) / 8); if(png::error()) return;
        if(bpp >= 8) for(size_t i = 0; i < passw; i++) for(size_t b = 0; b < bytewidth; b++) //b = current byte of this pixel
          out[bytewidth * w * (passtop + spacey * y) + bytewidth * (passleft + spacex * i) + b] = linen[bytewidth * i + b];
        else for(size_t i = 0; i < passw; i++)
        {
          size_t obp = bpp * w * (passtop + spacey * y) + bpp * (passleft + spacex * i), bp = i * bpp;
		  for(size_t b = 0; b < bpp; b++) bitstream::setBit(obp, out, bitstream::readRevBit(bp, &linen[0]));
        }
        unsigned char* temp = linen; 
		linen = const_cast<unsigned char*>(lineo); 
		lineo = temp; //swap the two buffer pointers "line old" and "line new"
      }
}