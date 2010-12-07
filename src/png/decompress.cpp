#include "png/zlib.h"

int zlib::decompress( uchrstream & out, const uchrstream & in)
{
	Inflator inf;

	if (in.size()<2) { return 53;} // error, size is too small
	
	if ( (in[0]*256+in[1]) % 31 !=0 ) { return 24; } //FCHECK

	sad::Chunk cm=in[0] & 15, cinf=(in[0] >> 4) & 15, fdict=(in[1] >>5 ) & 1;

	if (cm!=8 || cinf > 7)  { return 25; }
	if (fdict !=0)          { return 26; }

    inf.inflate(out,in,2);
	return inf.error();
}
