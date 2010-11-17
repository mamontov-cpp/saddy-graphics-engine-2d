#include <stdio.h>
#include <stdlib.h>
#include <memory.h>

#include "load.h"

#define BMP_SIZE_FILEHEADER 14
#define BMP_SIZE_INFOHEADER 40
#define BMP_COLOR_BITS_24 24

///////////////////////
static unsigned int uInt16Number(unsigned char buf[2])
{
	return (buf[1] << 8) | buf[0];
}

static unsigned int uInt32Number(unsigned char buf[4])
{
	unsigned numb = buf[3];
	numb =  (numb << 8) | buf[2];
	numb =  (numb << 8) | buf[1];
	numb =  (numb << 8) | buf[0];
	return numb;
}

int ReadFileHeader(FILE* f, int* bitmap_pos)
{
	unsigned char header[BMP_SIZE_FILEHEADER];
	int numb = 0;
	int offset = 0;

	if (fseek(f,0,SEEK_SET))
		return 0;

	numb = fread(header,BMP_SIZE_FILEHEADER,1,f);
	if (numb != 1)
		return 0;

	if (header[0] != 'B' || header[1] != 'M')
		return 0;

	offset = uInt32Number(header + 10);

	numb = fread(header,4,1,f);
	if (numb != 1)
		return 0;

	if (uInt32Number(header) != 40)
		return 0;

	*bitmap_pos = offset;
	return 1;
}

//////////////////////////

int LoadBMP(const char* file, IMAGE* out_img)
{
	FILE* f;
	int bitmap_pos;
	unsigned char buf[40];
	int numb;
	int x_res;
	int y_res;
	int n_bits;
	int compression;
	int size_image;
	int n_used_colors;

	f = fopen(file,"rb");

	if (!f)
		return 0;

	if (out_img == NULL)
		return 0;

	if (!ReadFileHeader(f,&bitmap_pos))
	{
		fclose(f);
		return 0;
	}

	if (fseek(f,BMP_SIZE_FILEHEADER,SEEK_SET))
	{
		fclose(f);
		return 0;
	}

	numb = fread(buf,40,1,f);
	if (numb != 1)
	{
		fclose(f);
		return 0;
	}

	x_res = (int) uInt32Number(buf + 4);
	y_res = (int) uInt32Number(buf + 8);

	n_bits			=	(int) uInt16Number(buf + 14);
	compression		=	(int) uInt32Number(buf + 16);
	size_image		=	(int) uInt32Number(buf + 20);
	n_used_colors	=	(int) uInt32Number(buf + 32);

	if (n_bits == BMP_COLOR_BITS_24)
	{
		int rgb_size;
		unsigned char* rgb;
		int y;
		unsigned char* line;
		int rest_4;
		
		if (bitmap_pos != BMP_SIZE_FILEHEADER + BMP_SIZE_INFOHEADER)
		{
			fclose(f);
			return 0;
		}

		if (fseek(f,bitmap_pos,SEEK_SET))
		{
			fclose(f);
			return 0;
		}

		rgb_size = 3 * x_res;
		rest_4 = rgb_size % 4;
		if (rest_4 > 0)
			rgb_size += 4 - rest_4;

		out_img->width = x_res;
		out_img->height = y_res;

		out_img->data = (unsigned char*)malloc(x_res * y_res * 3);

		if (out_img->data == NULL)
			return 0;

		rgb = (unsigned char*)malloc(rgb_size);

		for (y = 0; y< y_res; y++)
		{
			int numb = 0;
			int x = 0;

			numb = fread(rgb, rgb_size,1,f);
			if (numb != 1)
			{
				fclose(f);
				free(rgb);
				return 0;
			}

			numb = 0;
			line = out_img->data + x_res * 3 * y;
			for (x = 0; x < x_res; x++)
			{
				line[2] = rgb[numb++];
				line[1] = rgb[numb++];
				line[0] = rgb[numb++];
				line += 3;
			}
		}
		fclose(f);
		free(rgb);
	}
	else
		return 0;

	return 1;
}

void main(void)
{
}