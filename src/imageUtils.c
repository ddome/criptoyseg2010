#include "imageUtils.h"

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
    printf("%s\n",info->identifier);
    
    fread(&(info->fileSize),dword,1,imageFile);
    printf("%d\n",info->fileSize);
    
    fseek(imageFile,dword,SEEK_CUR);
    
    fread(&(info->dataOffset),dword,1,imageFile);
    printf("%d\n",info->dataOffset);
    
    fread(&(info->headerSize),dword,1,imageFile);
    printf("%d\n",info->headerSize);
    
    fread(&(info->width),dword,1,imageFile);
    printf("%d\n",info->width);
    
    fread(&(info->height),dword,1,imageFile);
    printf("%d\n",info->height);
    
    fread(&(info->planes),word,1,imageFile);
    printf("%d\n",info->planes);
    
    fread(&(info->bpp),word,1,imageFile);
    printf("%d\n",info->bpp);
    
    fread(&(info->compression),dword,1,imageFile);
    printf("%d\n",info->compression);
    
    fread(&(info->dataSize),dword,1,imageFile);
    printf("%d\n",info->dataSize);
    
    fread(&(info->hResolution),dword,1,imageFile);
    printf("%d\n",info->hResolution);
    
    fread(&(info->vResolution),dword,1,imageFile);
    printf("%d\n",info->vResolution);
    
    fread(&(info->colors),dword,1,imageFile);
    printf("%d\n",info->colors);
    
    fread(&(info->importantColors),dword,1,imageFile);
    printf("%d\n",info->importantColors);
    
    return info;
}

void
PrintImageInfo(imageInfo info)
{
    printf("Identificador: %s.\n",info->identifier);
    
    printf("Tamano del archivo: %d bytes.\n",info->fileSize);
    
    printf("Bitmap Data Offset: %d bytes.\n",info->dataOffset);
    
    printf("Bitmap Header Size %d bytes.\n",info->headerSize);
    
    printf("Width: %d pixels.\n",info->width);
    
    printf("Height: %d pixels\n",info->height);
    
    printf("Planes: %d.\n",info->planes);
    
    printf("Bits Per Pixel: %d.\n",info->bpp);
    
    printf("Compression: %d.\n",info->compression);
    
    printf("Bitmap Data Size: %d.\n",info->dataSize);
    
    printf("HResolution: %d.\n",info->hResolution);
    
    printf("VResolution: %d.\n",info->vResolution);
    
    printf("Colors: %d.\n",info->colors);
    
    printf("Important Color: %d.\n",info->importantColors);    
}