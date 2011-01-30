/*! \file zlib.h
	Contains a based on picoPNG implementation of Zlib functions.
*/
#include "types.h"
#include <vector>
#pragma once


/*! We describe a bytestream, as a bunch of bytes
*/
typedef Uint8 * bytestream;

/*! Unsigned char stream
*/
typedef std::vector<Uint8> uchrstream;

namespace bitstream
{
	/*! Reads a bit
	    \param[in,out] bit_position position of bit
		\param[in]     stream       stream
	*/
	inline sad::Chunk readBit(size_t & bit_position, const bytestream stream);
	/*! Reads a bit
	    \param[in,out] bit_position position of bit
		\param[in]     stream       stream
		\param[in]     amount       amount of bits 
	*/
	inline sad::Chunk readBits(size_t & bit_position, const bytestream stream, size_t amount);
	/*! Reads a bit
	    \param[in,out] bit_position position of bit
		\param[in]     stream       stream
	*/
	inline sad::Chunk readRevBit(size_t & bit_position, const bytestream stream);
	/*! Reads a bit
	    \param[in,out] bit_position position of bit
		\param[in]     stream       stream
		\param[in]     amount       amount of bits 
	*/
	inline sad::Chunk readRevBits(size_t & bit_position, const bytestream stream, size_t amount);
	/*! Reads a 32 bit ints
	    \param[in] stream stream
	*/
	inline sad::Chunk read32(const Uint8 * stream);
	/*! Sets a bit
	    \param[in,out]  bitpos bit position
		\param[in,out]  bits  stream
		\param[in]      bit   bit
	*/
	inline void setBit(size_t & bitpos, bytestream bits, sad::Chunk bit);
}

/*! Contsins a 2d representation of a tree.
    One element is 0 or 1, and other contains all nodes and leaves
*/
typedef std::vector<sad::Chunk> HuffmanTree;

namespace huffman
{
	/*! Generates a huffman tree from given lengeh array
	    \param[out] tree         resulting tree
		\param[in]  bit_lengths  lengths array
		\param[in]  max_lengths  maximum length
		\return error code, 0 if ok
	*/
	int generateFromLengthArray(
		                        HuffmanTree &  tree, 
								const std::vector<unsigned long> & bit_lengths, 
								unsigned long max_length
							   );
	/*! Decodes a tree
	    \param[in]     tree          huffman tree
		\param[in,out] decoded       decoding flag
		\param[in,out] result        result of decoding
		\param[in,out] tree_position current tree position
		\param[in]     bit           current pos  
	*/
	inline int decode ( 
		          const HuffmanTree & tree,
				  bool        & decoded,
				  sad::Chunk  & result,
				  size_t      & tree_position,
				  sad::Chunk  bit
		       );
}

//==== Huffman inlined functions  ====
int huffman::decode ( 
		             const HuffmanTree & tree,
				     bool        & decoded,
				     sad::Chunk  & result,
				     size_t      & tree_position,
				     sad::Chunk  bit
		           )
{
	sad::Chunk tmp=(sad::Chunk) tree.size()/2;
	
	//Handle error, when we appear outside
	if (tree_position>=tmp) 
		return 11;

	result=tree[2*tree_position+bit];
	decoded= ( (result)< tmp);
	tree_position= ( decoded )? 0: result-tmp;

	return 0;
}

/*! Provides a decompression operstors
*/
class Inflator
{
 private:
	      int m_error;                  //!< Error

		  HuffmanTree m_tree;           //!< Tree
		  HuffmanTree m_treeD;          //!< Temp tree
          
		  HuffmanTree m_lengthcodetree; //!< Other tree

		  /*! Gets the tree of a deflated block
		      \param[out] ctree   first  tree
			  \param[out] ctreeD  second tree
		  */
		  static void createFixed(HuffmanTree & ctree, HuffmanTree & ctreeD);
		  /*! Symbol decoding
		      \param[in]      in    input stream
			  \param[in,out]  bitp  bit position
			  \param[in]      ctree code tree
			  \param[in]      inl   input length
		  */
		  sad::Chunk decodeSym(const Uint8 * in, size_t & bitp, const HuffmanTree & ctree, size_t inl );
          /*! Inflates dynamically and returns the tree
		      \param[out]      ktree  first tree
			  \param[out]      ktreeD second tree
			  \param[in]       in    input stream
			  \param[in]       bitp  bit position
			  \param[in]       inl   input length
		  */
		  void         inflateDynamic(
			                          HuffmanTree & ktree,
									  HuffmanTree & ktreeD,
									  const unsigned char * in,
									  size_t & bitp,
									  size_t inl
									 );
		  /*! Inflates a huffman block
		      \param[out]     out   output vector
			  \param[in,out]  in    input stream
			  \param[in,out]  bitp  position
			  \param[in]      inl   input length
			  \param[in]      enctype encoded type
		  */
		  void inflateBlock( 
			                 uchrstream &          out,
			                 const unsigned char * in,
							 size_t &              bitp,
							 size_t &              position,
							 size_t                inl,
							 sad::Chunk            enctype
						   );
		  /*! Inflates a huffman block
		      \param[out]     out   output vector
			  \param[in,out]  in    input stream
			  \param[in,out]  bitp  position
			  \param[in]      inl   input length
			  \param[in]      enctype encoded type
		  */
		  void inflateWithoutCompression( 
			                             uchrstream &          out,
			                             const unsigned char * in,
							             size_t &              bitp,
							             size_t &              position,
							             size_t                inl
						                );
public:
	     //! Inline accessors
         //!\{
	     inline int           error() const { return m_error; }
		 inline HuffmanTree & tree()        { return m_tree;  }
		 inline HuffmanTree & treeD()       { return m_treeD; }
		 inline HuffmanTree & codes()       { return m_lengthcodetree; }
		 //!\}
         
		 /*! Main inflating function
		     \param[out] out   decomressed stream
			 \param[in]  in    input stream
			 \param[in]  inpos begin position
		 */
		 void inflate(uchrstream & out,const uchrstream & in, size_t inpos=0);
};

namespace zlib
{
	/*! Decompress a stream 
	    \param[in]   in    input bytestream
		\param[out]  out   output bytestream
	*/
	int decompress( uchrstream & out, const uchrstream & in);
}



//====== Source code goes here ======

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
