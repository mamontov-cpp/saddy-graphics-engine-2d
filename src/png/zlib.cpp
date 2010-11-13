#include "png/zlib.h"

sad::Chunk bitstream::readBit(size_t & bit_position, const bytestream stream)
{
	sad::Chunk r = (stream[bit_position >> 3] >> (bit_position & 0x7)) & 1; 
	++bit_position; 
	return r;
}

sad::Chunk bitstream::readBits(size_t & bit_position, const bytestream stream, size_t amount)
{
	unsigned long result=0;
	
	for (size_t cur_pos=0; cur_pos<amount;cur_pos++)
		result+=bitstream::readBit(bit_position,stream) << cur_pos;

	return result;
}

sad::Chunk bitstream::readRevBit(size_t & bit_position, const bytestream stream)
{
	unsigned long r;
	r = (stream[bit_position >> 3] >> (7 - (bit_position & 0x7))) & 1; 
	bit_position++; 
	return r;
}

sad::Chunk bitstream::readRevBits(size_t & bit_position, const bytestream stream, size_t amount)
{
	unsigned long result=0;
	
	for (size_t cur_pos=amount-1; cur_pos<amount;cur_pos--)
		result+=(bitstream::readRevBit(bit_position,stream) << cur_pos);

	return result;
}

sad::Chunk bitstream::read32(const Uint8 * stream)
{
	return (stream[0]<<24) | (stream[1]<<16) | (stream[2]<<8) | (stream[3]);
}

void bitstream::setBit(size_t & bitpos, bytestream bits, sad::Chunk bit)
{
	bits[bitpos >> 3] |=  (bit << (7 - (bitpos & 0x7))); 
	bitpos++;;
}