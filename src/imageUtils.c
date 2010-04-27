#include "../include/imageUtils.h"

#define byte sizeof(char)
#define word sizeof(unsigned short int)
#define dword sizeof(unsigned int)

imageInfo
GetImageInfo(FILE * imageFile)
{
    imageInfo info;
    info=calloc(1,sizeof(struct imageInfoT));
    if(info==NULL)
    {
        return NULL;
    }
    fread(info->identifier,2*byte,1,imageFile);
    
    fread(&(info->fileSize),dword,1,imageFile);
    
    fseek(imageFile,dword,SEEK_CUR);
    
    fread(&(info->dataOffset),dword,1,imageFile);
    
    fread(&(info->headerSize),dword,1,imageFile);
    
    fread(&(info->width),dword,1,imageFile);
    
    fread(&(info->height),dword,1,imageFile);
    
    fread(&(info->planes),word,1,imageFile);
    
    fread(&(info->bpp),word,1,imageFile);
    
    fread(&(info->compression),dword,1,imageFile);
    
    fread(&(info->dataSize),dword,1,imageFile);
    
    fread(&(info->hResolution),dword,1,imageFile);
    
    fread(&(info->vResolution),dword,1,imageFile);
    
    fread(&(info->colors),dword,1,imageFile);
    
    fread(&(info->importantColors),dword,1,imageFile);
    
    return info;
}

void
PrintImageInfo(imageInfo info)
{
    printf("\n\nInformacion de la imagen.\n");
    printf("=========================\n");
    
    printf("\tIdentificador: %s.\n",info->identifier);
    
    printf("\tTamano del archivo: %d bytes.\n",info->fileSize);
    
    printf("\tBitmap Data Offset: %d bytes.\n",info->dataOffset);
    
    printf("\tBitmap Header Size %d bytes.\n",info->headerSize);
    
    printf("\tWidth: %d pixels.\n",info->width);
    
    printf("\tHeight: %d pixels\n",info->height);
    
    printf("\tPlanes: %d.\n",info->planes);
    
    printf("\tBits Per Pixel: %d.\n",info->bpp);
    
    printf("\tCompression: %d.\n",info->compression);
    
    printf("\tBitmap Data Size: %d.\n",info->dataSize);
    
    printf("\tHResolution: %d.\n",info->hResolution);
    
    printf("\tVResolution: %d.\n",info->vResolution);
    
    printf("\tColors: %d.\n",info->colors);
    
    printf("\tImportant Color: %d.\n",info->importantColors);    
}