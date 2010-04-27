#ifndef _IMAGE_UTIL_H_
#define _IMAGE_UTIL_H_

#include <stdio.h>
#include <stdlib.h>

struct imageInfoT{
    char identifier[3];
    unsigned int fileSize;
    unsigned int dataOffset;
    unsigned int headerSize;
    unsigned int width;
    unsigned int height;
    unsigned short int planes;
    unsigned short int bpp;
    unsigned int compression;
    unsigned int dataSize;
    unsigned int hResolution;
    unsigned int vResolution;
    unsigned int colors;
    unsigned int importantColors;
};

typedef struct imageInfoT * imageInfo;

imageInfo GetImageInfo(FILE * imageFile);

void PrintImageInfo(imageInfo image);

#endif
