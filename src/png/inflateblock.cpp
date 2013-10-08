#include "png/zlib.h"

static const sad::Chunk lbase[29]=
{3,4,5,6,7,8,9,10,11,13,15,17,19,23,27,31,35,43,51,59,67,83,99,115,131,163,195,227,258};

static const sad::Chunk lextra[29]=
{0,0,0,0,0,0,0,0,1,1,1,1,2,2,2,2,3,3,3,3,4,4,4,4,5,5,5,5,0};

static const sad::Chunk dbase[30]=
{1,2,3,4,5,7,9,13,17,25,33,49,65,97,129,193,257,385,513,769,1025,1537,2049,3073,4097,6145,8193,12289,16385,24577};

static const sad::Chunk dextra[30]=
{0,0,0,0,1,1,2,2,3,3,4,4,5,5,6,6,7,7,8,8,9,9,10,10,11,11,12,12,13,13};

void Inflator::inflateBlock( 
			                 uchrstream &          out,
			                 const unsigned char * in,
							 size_t &              bitp,
							 size_t &              position,
							 size_t                inl,
							 sad::Chunk            enctype
					       )
{
	if (enctype==1)        { this->createFixed(this->m_tree,this->m_treeD); }
	else if (enctype==2)   { this->inflateDynamic(this->m_tree,this->m_treeD,in,bitp,inl); }

	for (;;)
	{
		sad::Chunk code=this->decodeSym(in,bitp,this->m_tree,inl);
		if (code==256) return;
		//Literal
		else if (code<=255)
		{
			if (position>=out.size()) out.resize((position+1)*2);
			out[position++]=(unsigned char)(code);
		}
		//Length code
		else if (code>=257 && code<=285)
		{
			size_t length=lbase[code-257], nexts=lextra[code-257];
			//Handle error
			if ( (bitp>>3) >=inl ) {m_error=51; return ;}

			length+=bitstream::readBits(bitp,(const bytestream)in,nexts);
			
			sad::Chunk codeD=this->decodeSym(in,bitp,this->m_treeD,inl);
			if (this->m_error) return;

			if (codeD>29) { this->m_error=18; return; }
			
			unsigned long dist=dbase[codeD], nextD=dextra[codeD];

			if ( (bitp>>3) >=inl ) {m_error=51; return ;}
			dist+=bitstream::readBits(bitp,(const bytestream)in,nextD);

            size_t start=position, back=start-dist;

            if (position+length>=out.size()) out.resize((position+length)*2);
			
			sad::uchar * pos1=&(out[position]);
			sad::uchar * pos2=&(out[back]);
            for (size_t i=0; i< length ; i++)
			{
				*(pos1++)=*(pos2++);
				++position;
				++back;
				if (back>=start)
				{
					back=start-dist;
					pos2=&(out[back]);
				}
			}
		}
	}
}
