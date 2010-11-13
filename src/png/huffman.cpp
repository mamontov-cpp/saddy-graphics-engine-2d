#include "png/zlib.h"

//Define this, if want to debug generation of tree
//#define DBG

#ifdef DBG

#include <stdlib.h>

#define FOP          FILE * wr=fopen("uname.txt","wt");
#define FWR          fprintf(wr,"i:%u, n: %u, , tree: %u \n  ",i,n,tree_position);
#define FPR(A,X,STR)     fprintf(wr,"Tree changed, by %u to %u,with reason %s \n",A,X,STR);
#define FCL          fclose(wr);

#else

#define FOP
#define FWR
#define FCL
#define FPR(A,X,STR)

#endif

int huffman::generateFromLengthArray(
							 HuffmanTree &  tree, 
							 const std::vector<unsigned long> & bit_lengths, 
							 unsigned long max_length
						    )
{
	sad::Chunk codes_len=(sad::Chunk)bit_lengths.size();
	//Clearing and resizing tree
	tree.clear();tree.resize(codes_len*2,32767);


	unsigned long tree_position=0, node_filled=0;

	HuffmanTree tree1D(codes_len), bit_len_count(max_length+1,0);
	HuffmanTree next=bit_len_count;

	FOP; 

	//Count number of instances of code length
	for (sad::Chunk bits=0; bits < codes_len; bits++)
		 ++(bit_len_count[bit_lengths[bits]]);

	//Calculating next code, by increasing by bit len count
	for (sad::Chunk bits=1; bits <= max_length; bits++)
         next[bits]=(next[bits-1]+bit_len_count[bits-1])<<1;

	//Generate 1d count by all calculations
	for (sad::Chunk n=0;n < codes_len; n++)
		if (bit_lengths[n] != 0)
			tree1D[n]=next[bit_lengths[n]]++;

	//Creating codes
	for (sad::Chunk n=0; n<codes_len; n++)
	{
		for (sad::Chunk i=0;i<bit_lengths[n];i++)
		{
			FWR;
			sad::Chunk bit=(tree1D[n]>>(bit_lengths[n] -i -1)) & 1;
			
			//Handle error
			if (tree_position > codes_len-2) 
			{
				FCL;
				return 55;
			}

			if (tree[2*tree_position+bit]==32767)
			{
				if ( i+1 == bit_lengths[n] ) 
				{ 
					tree[2*tree_position+bit]=n; 
					tree_position=0;
					FPR(1234,tree_position,"Zeroing");
				}
				else                         
				{ 
					tree[2*tree_position+bit]=++node_filled+codes_len; 
					tree_position=node_filled;
					FPR(node_filled,tree_position,"Changing to node filled");
				}
			}
			else
			{
				unsigned long ltree_pos=tree_position;
				tree_position=(tree[2*tree_position+bit]>codes_len)?(tree[2*tree_position+bit]-codes_len):(codes_len-tree[2*tree_position+bit]);
				if (tree_position>tree.size()) { FPR(0,0,"Outbound!"); FCL; }
                FPR(tree[2*tree_position+bit],tree_position,"Changing to tree val by code len");
			}

		}
	}
 return 0;
}

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