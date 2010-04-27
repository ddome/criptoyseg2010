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

/* Extrae informacion importante del header
 * imageFile: Puntero al archivo que contiene la imagen
 *
 * Devuelve NULL en caso de error. En caso contrario devuelve
 * imageInfo con la informacion del header de la imagen 
 */
imageInfo GetImageInfo(FILE * imageFile);

/* Imprime en la salida estandar la informacion contenida en
 * imageInfo
 */
void PrintImageInfo(imageInfo image);

#endif
