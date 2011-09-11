#include "png/zlib.h"

//Code length code lengths
//                               0   1  2 3 4 5 6 7 8  9 10 11 12 13 14 15 16 17 18
static const sad::Chunk CLL[19]={16,17,18,0,8,7,9,6,10,5,11,4 ,12,3 ,13,2 ,14,1 ,15};


void  Inflator::inflateDynamic(
			                          HuffmanTree & ktree,
									  HuffmanTree & ktreeD,
									  const unsigned char * in,
									  size_t & bitp,
									  size_t inl
							  )
{
	std::vector<sad::Chunk> bits(288,0), bitsD(32,0);
	
	//Bit pointer will go past the memory
	if (bitp >>3 >= inl -2) { this->m_error=49; return ;}

	size_t hlit=bitstream::readBits(bitp,(const bytestream)in,5)+257;
	size_t hdst=bitstream::readBits(bitp,(const bytestream)in,5)+1;
    size_t hlen=bitstream::readBits(bitp,(const bytestream)in,4)+4;

	//Create code length codes
	std::vector<sad::Chunk> codelc(19);
	for (size_t i=0;i<19;i++)
		codelc[CLL[i]]=(i<hlen)? (bitstream::readBits(bitp,(const bytestream)in,3)) : 0;

	this->m_error=huffman::generateFromLengthArray(this->m_lengthcodetree,codelc,7);
	if (m_error)
		return;
	size_t i=0, replen;

	while( i< hlit+hdst )
	{
		unsigned long code=this->decodeSym(in,bitp,m_lengthcodetree,inl);
		
		//A length code
		if (code<=15)
		{
			if (i<hlit)
				bits[i++]=code;
			else
				bitsD[i++ - hlit]=code;
		}
        //Repeat previous
		else if (code==16)
		{
			if (bitp>>3 >= inl) { this->m_error=50; return; }
			replen=3+bitstream::readBits(bitp,(const bytestream)in,2);
			unsigned long value;
			
			if ((i-1)<hlit)
				value=bits[i-1];
			else
				value=bitsD[i-hlit-1];

			for(size_t n = 0; n < replen; n++) //repeat this value in the next lengths
            {
              if(i >= hlit + hdst) { m_error = 13; return; } //error: i is larger than the amount of codes
              if(i < hlit) bits[i++] = value; else bitsD[i++ - hlit] = value;
            }
			
			
		}
		//Repeat 0 3--10
		else if (code==17)
		{
			if (bitp>>3 >= inl) { this->m_error=50; return; }
			replen=3+bitstream::readBits(bitp,(const bytestream)in,3);
			for(size_t n = 0; n < replen; n++) //repeat this value in the next lengths
            {
              if(i >= hlit + hdst) { m_error = 14; return; } //error: i is larger than the amount of codes
              if(i < hlit) bits[i++] = 0; else bitsD[i++ - hlit] = 0;
            }
			
			
		}
        //Repeat 0 11-138
		else if (code==18)
		{
			if (bitp>>3 >= inl) { this->m_error=50; return; }
			replen=11+bitstream::readBits(bitp,(const bytestream)in,7);
			
			for(size_t n = 0; n < replen; n++) //repeat this value in the next lengths
            {
              if(i >= hlit + hdst) { m_error = 15; return; } //error: i is larger than the amount of codes
              if(i < hlit) bits[i++] = 0; else bitsD[i++ - hlit] = 0;
            }

		}
		//Error
		else 
		{ 
			this->m_error=16; 
			return; 
		}
	}

	if (bits[256]==0) { this->m_error=64; return; }

	this->m_error=huffman::generateFromLengthArray(ktree,bits,15);
	if (m_error) return; 
    this->m_error=huffman::generateFromLengthArray(ktreeD,bitsD,15);
}
