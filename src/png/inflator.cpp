#include "png/zlib.h"

void Inflator::inflate(uchrstream & out,const uchrstream & in, size_t inpos)
{
	size_t bitp=0, position=0;
	this->m_error=0;
	sad::Chunk final_code=0;
	while( !final_code &&  !m_error)
	{
		//Handle, use ptr hump
		if (bitp >> 3 >= in.size()) { this->m_error=52; return; }
		
		final_code=bitstream::readBit(bitp,(const bytestream)(&in[inpos]));
		sad::Chunk BTYPE=bitstream::readBit(bitp,(const bytestream)(&in[inpos]));
		BTYPE+=2*bitstream::readBit(bitp,(const bytestream)(&in[inpos]));

		if         (BTYPE==3)   { this->m_error=20; return; }
		else  if   (BTYPE==0)   { this->inflateWithoutCompression(out,&in[inpos],bitp,position,in.size()); }
		else                    { this->inflateBlock(out,&in[inpos],bitp,position,in.size(),BTYPE); };
	}
	if (!m_error) out.resize(position);
}

#define MAKE_BITS(A1,A2,ARR,A3)  for(size_t i=A1; i<=A2;i++) ARR[i]=A3; 
void Inflator::createFixed(HuffmanTree & ctree, HuffmanTree & ctreeD)
{
	std::vector<sad::Chunk> bits(288,8), bitsD(32,5);
	MAKE_BITS(144,255,bits,9);
	MAKE_BITS(256,279,bitsD,7);
	huffman::generateFromLengthArray(ctree,bits,15);
	huffman::generateFromLengthArray(ctreeD,bitsD,15);
}

sad::Chunk Inflator::decodeSym(const Uint8 * in, size_t & bitp, const HuffmanTree & ctree, size_t inl )
{
 bool decoded=false; unsigned long ct=0;
 for (size_t tree_position=0;;)
 {
	 bool cmp_bp=(bitp & 0x07) ==0;
	 bool cmp_in=(bitp>>3) > inl;
    
	if (cmp_bp && cmp_in) { this->m_error=10; return 0; }

	//Decode, and exit if failed
	this->m_error=huffman::decode(ctree,decoded,ct,tree_position,bitstream::readBit(bitp,(const bytestream)in));
	if (m_error) {return 0;}

	if (decoded) return ct;
 }
}

void Inflator::inflateWithoutCompression( 
			                             uchrstream &          out,
			                             const unsigned char * in,
							             size_t &              bitp,
							             size_t &              position,
							             size_t                inl
						                )
{
	while ( (bitp & 0x7) !=0 ) bitp++;
	size_t p=bitp/8;

	//Handle error;
	if (p>=inl-4) { m_error=52; return; }

	sad::Chunk len=in[p]+256*in[p+1], nlen=in[p+2]+256*in[p+3]; 
	p+=4;

	if (len+nlen!=65535) { m_error=21; return;}

	if (position+len>= out.size()) out.resize(position+len);

	if (p+len>inl) { m_error=23; return; }
	for (sad::Chunk n=0; n<len; n++) out[position++]=in[p++];

	bitp=p*8;
}
