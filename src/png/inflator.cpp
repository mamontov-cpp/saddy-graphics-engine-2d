#include "png/zlib.h"

void Inflator::inflate(uchrstream & out,const uchrstream & in, size_t inpos)
{
	zlib::temporary * state=new zlib::temporary(in,out);
	state->m_in_pos=(Uint8*)(&in[inpos]);
	this->m_error=0;
	sad::Chunk final_code=0;
	while( !final_code &&  !m_error)
	{
		//Handle, use ptr hump
		if (state->m_bitp >> 3 >= state->m_in_size) { this->m_error=52; return; }
		final_code=bitstream::readBit(state);
		state->m_BTYPE=bitstream::readBit(state);
		state->m_BTYPE+=2*bitstream::readBit(state);

		if         (state->m_BTYPE==3)   
		{ 
			this->m_error=20; 
			return; 
		}
		else  if   (state->m_BTYPE==0)   
		{ 
			this->inflateWithoutCompression(state); 
		}
		else                             
		{ 
			this->inflateBlock(*state->m_out,
				               state->m_in_pos,
							   state->m_bitp,
							   state->m_position,
							   state->m_in_size,
							   state->m_BTYPE
							  ); 
		};
	}
	if (!m_error) out.resize(state->m_position);
	delete state;
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
 sad::Chunk tmp=ctree.size()/2;
 for (size_t tree_position=0;;)
 {
	if (tree_position>=tmp) {this->m_error=11; return 0;}
	
	bool cmp_bp=(bitp & 0x07) ==0;
    bool cmp_in=(bitp>>3) > inl;
    
	if (cmp_bp && cmp_in) { this->m_error=10; return 0; }
     
    ct=ctree[2*tree_position+bitstream::readBit(bitp,(const bytestream)in)];
	decoded= ( (ct)< tmp);
	tree_position= ( decoded )? 0: ct-tmp;

	if (decoded) return ct;
 }
}



void Inflator::inflateWithoutCompression( zlib::temporary * state )
{
	while ( (state->m_bitp & 0x7) !=0 ) (state->m_bitp)++;
	size_t p=(state->m_bitp)/8;

	//Handle error;
	if (p>=state->m_BTYPE-4) { m_error=52; return; }

	sad::Chunk len=(state->m_in_pos)[p]+256*(state->m_in_pos)[p+1], nlen=(state->m_in_pos)[p+2]+256*(state->m_in_pos)[p+3]; 
	p+=4;

	if (len+nlen!=65535) { m_error=21; return;}

	if (state->m_position+len>= state->m_out->size()) state->m_out->resize(state->m_position+len);

	if (p+len>state->m_BTYPE) { m_error=23; return; }
	for (sad::Chunk n=0; n<len; n++) (*(state->m_out))[(state->m_position)++]=(state->m_in_pos)[p++];

	state->m_bitp=p*8;
}
