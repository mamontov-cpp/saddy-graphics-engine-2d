#include "png/zlib.h"
#include "png/png.h"

static int m_error=0;
static png::Info m_info;

int & png::error()
{
  return m_error;
}

png::Info & png::info()
{
	return m_info;
}

unsigned long png::bpp(const png::Info & i)
{
	if (i.colorType==2)      return (3 * i.bitDepth);
	else if (i.colorType>=4) return (i.colorType-2)* i.bitDepth;
	else return i.bitDepth;
}

#define CHECK_COND(X) if (!(X)) return 37; else return 0;

int png::checkColor(unsigned long colorType, unsigned long bd)
{
	if      ( (colorType==2 || colorType==4 || colorType==6)) 
	{ 
		 CHECK_COND(bd==8 || bd==16);
	}
	else if ( colorType==0 )
	{
		  CHECK_COND(bd==1 || bd==2 || bd==4 || bd==8 || bd==16);
	}
	else if ( colorType==3 )
	{
         CHECK_COND(bd==1 || bd==2 || bd==4 || bd==8);
	}
	return 31;
}
#define PAETH(C,A,B)  long C=(A>B)?(A-B):(B-A);

long png::paethPredictor(long a,long b, long c)
{
	long p=a+b-c;
	PAETH(pa,p,a);
	PAETH(pb,p,b);
	PAETH(pc,p,c);
	return (pa<=pb && pa<=pc)?(a): ((pb<=pc)? b:c);
}

void png::readHeader(const unsigned char * in, size_t inl)
{
	if(inl < 29) { error() = 27; return; } 
    if(in[0] != 137 || in[1] != 80 || in[2] != 78 || in[3] != 71 || in[4] != 13 || in[5] != 10 || in[6] != 26 || in[7] != 10) { error() = 28; return; } //no PNG signature
    if(in[12] != 'I' || in[13] != 'H' || in[14] != 'D' || in[15] != 'R') { error() = 29; return; } //error: it doesn't start with a IHDR chunk!
	info().width = bitstream::read32((&(in[16]))); 
	info().height = bitstream::read32((&(in[20])));
    info().bitDepth = in[24]; info().colorType = in[25];
    info().compressionMethod = in[26]; if(in[26] != 0) { error() = 32; return; } 
    info().filterMethod = in[27]; if(in[27] != 0) { error() = 33; return; } 
    info().interlaceMethod = in[28]; if(in[28] > 1) { error() = 34; return; } 
	error() = png::checkColor(info().colorType, info().bitDepth);
}
