#ifndef _IMAGE_UTIL_
#define _IMAGE_UTIL_

typedef struct {
    int num;
} imageInfoT;

imageInfoT * GetImageInfo(FILE * imageFile);

void PrintImageInfo(imageInfoT * image);

#endif
