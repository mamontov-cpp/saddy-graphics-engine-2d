#ifndef _LOAD_H
#define _LOAD_H

typedef struct _IMAGE
{
	int width;
	int height;
	unsigned char* data;
} IMAGE;

int LoadBMP (const char* filename,IMAGE* out_img);

#endif

