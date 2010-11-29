/*! \file png.h
    
	\TODO: Test
	Contains a based on picoPNG implementation of PNG loader.
*/
#include "types.h"
#include <vector>
#include <string.h>
#include <wchar.h>
#pragma once

namespace png
{
	class Info
	{
	 public:
	        unsigned long width;
	        unsigned long height;
	        unsigned long colorType;
	        unsigned long bitDepth;
	        unsigned long compressionMethod;
	        unsigned long filterMethod;
	        unsigned long interlaceMethod;
	        unsigned long key_r;
	        unsigned long key_g;
	        unsigned long key_b;
            bool key_defined;
			std::vector<Uint8> palette;
	};

	int & error();
	Info & info();

	/*! Returns a bpp
	    \param[in] info i
	*/
	unsigned long bpp(const Info & i);
	/*! Checks, whether color is valid
	    \param[in] type type
		\param[in] bd   BPP
	*/
	int checkColor(unsigned long colorType, unsigned long bd);
	/*! Paeth predictor function
	*/
	long paethPredictor(long a,long b, long c);
	/*! Converts from any color type to 32-bit. Return value is an error code
	*/
	int convert(
		        uchrstream & out, 
				const unsigned char * in, 
				Info & inf, 
				unsigned long w,
				unsigned long h
			   );
	/*! Unfilters a scanline with parameters
	*/
	void unfilter(
		          unsigned char *       rec, 
		          const unsigned char * scanline,
				  const unsigned char * prec,
				  size_t                bwidth,
				  unsigned long         filterType,
				  size_t                length
				 );
    /*! Deinterlace image, using Adam7 pass
	*/
	void  adam7deinterlace(
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
						   );
	/*! Reads a header
	*/
	void readHeader(const unsigned char * in, size_t inl);
	/*! Decodes a input stream
	*/
	void decode(std::vector<unsigned char>& out, const unsigned char* in, unsigned long size);
	/*! Loads a file to buffer
	*/
	void loadFile(std::vector<unsigned char> & buffer, const char *  filename);
	/*! Loads a file to buffer
	*/
	void loadFile(std::vector<unsigned char> & buffer, const wchar_t *  filename);
}
