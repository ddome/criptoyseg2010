#include "crypt.h"

#define CBC 1
#define ECB 2
#define CFB 3
#define OFB 4
/*Las siguientes funciones devuelve 0 o NULL en caso de error. Si todo sale como planeado devuelven 1 o un valor distinto de NULL*/
/*Prototipo de funciones para encriptar y desencriptar con AES*/
int CryptAES(char * key,char * method,char * password,char * vi,FILE * srcFile,FILE * dstFile,imageInfo info,int type);

int AES_CBC_crypt(FILE * srcFile,FILE * dstFile,imageInfo info,char * key,char * vi,int type);

int AES_ECB_crypt(FILE * srcFile,FILE * dstFile,imageInfo info,char * password,int type);

int AES_CFB_crypt(FILE * srcFile,FILE * dstFile,imageInfo info,char * key,char * vi,int type);

int AES_OFB_crypt(FILE * srcFile,FILE * dstFile,imageInfo info,char * key,char * vi,int type);

/*Prototipo de funciones para encriptar y desencriptar con DES*/
int CryptDES(char * key,char * method,char * password,char * vi,FILE * srcFile,FILE * dstFile,imageInfo info,int type);

int DES_ECB_crypt(FILE * srcFile,FILE * dstFile,imageInfo info,char * password,int type);

int DES_CBC_crypt(FILE * srcFile,FILE * dstFile,imageInfo info,char * key,char * vi,int type);

int DES_CFB_crypt(FILE * srcFile,FILE * dstFile,imageInfo info,char * key,char * vi,int type);

int DES_OFB_crypt(FILE * srcFile,FILE * dstFile,imageInfo info,char * key,char * vi,int type);

/*Prototipo de funciones auxiliares*/
int GetMethod(char * method);

void CopyHeader(FILE * srcFile,FILE * dstFile,imageInfo info);

int
Start(int encrypt,int decrypt,char * key,char * method,char * alg,char * password,char * vi,FILE * srcFile,FILE * dstFile,imageInfo info)
{
    int status=1;
    if(encrypt)
    {
        if(strncmp(alg,"aes",3)==0)
        {
	status=CryptAES(key,method,password,vi,srcFile,dstFile,info,AES_ENCRYPT);
        }
        else if(strncmp(alg,"des",3)==0)
        {
	status=CryptDES(key,method,password,vi,srcFile,dstFile,info,DES_ENCRYPT);
        }
        else
        {
	status=0;
        }
    }
    else if(decrypt)
    {
        if(strncmp(alg,"aes",3)==0)
        {
	status=CryptAES(key,method,password,vi,srcFile,dstFile,info,AES_DECRYPT);
        }
        else if(strncmp(alg,"des",3)==0)
        {
	status=CryptDES(key,method,password,vi,srcFile,dstFile,info,DES_DECRYPT);
        }
        else
        {
	status=0;
        }
    }
    else
    {
        status=0;
    }
    
    return status;
}

int
CryptAES(char * key,char * method,char * password,char * vi,FILE * srcFile,FILE * dstFile,imageInfo info,int type)
{
    int methodAux=0;
    printf("Bien1\n");
    methodAux=GetMethod(method);
    printf("Bien2\n");
    int status=1;
    if(methodAux==CBC)
    {
        status=AES_CBC_crypt(srcFile,dstFile,info,key,vi,type);
    }
    else if(methodAux==ECB)
    {
        status=AES_ECB_crypt(srcFile,dstFile,info,password,type);
    }
    else if(methodAux==CFB)
    {
        status=AES_CFB_crypt(srcFile,dstFile,info,key,vi,type);
    }
    else if(methodAux==OFB)
    {
        status=AES_OFB_crypt(srcFile,dstFile,info,key,vi,type);
    }
    else
    {
        status=0;
    }
    
    return status;
}

int
AES_CBC_crypt(FILE * srcFile,FILE * dstFile,imageInfo info,char * key,char * vi,int type)
{
    /*AES CBC*/
    return 1;
}

int
AES_ECB_crypt(FILE * srcFile,FILE * dstFile,imageInfo info,char * password,int type)
{
    /*AES ECB*/
    return 1;
}

int
AES_CFB_crypt(FILE * srcFile,FILE * dstFile,imageInfo info,char * key,char * vi,int type)
{
    /*AES CFB*/
    return 1;
}

int
AES_OFB_crypt(FILE * srcFile,FILE * dstFile,imageInfo info,char * key,char * vi,int type)
{
    /*AES OFB*/
    return 1;
}

int
CryptDES(char * key,char * method,char * password,char * vi,FILE * srcFile,FILE * dstFile,imageInfo info,int type)
{
    int methodAux=0;
    int status=1;
    
    methodAux=GetMethod(method);
    
    if(methodAux==CBC)
    {
        status=DES_CBC_crypt(srcFile,dstFile,info,key,vi,type);
    }
    else if(methodAux==ECB)
    {
        status=DES_ECB_crypt(srcFile,dstFile,info,password,type);
    }
    else if(methodAux==CFB)
    {
        status=DES_CFB_crypt(srcFile,dstFile,info,key,vi,type);
    }
    else if(methodAux==OFB)
    {
        status=DES_OFB_crypt(srcFile,dstFile,info,key,vi,type);
    }
    else
    {
        status=0;
    }
    
    return status;
}

int
DES_ECB_crypt(FILE * srcFile,FILE * dstFile,imageInfo info,char * password,int type)
{
    DES_cblock block;
    DES_cblock src_block;
    CopyHeader(srcFile,dstFile,info);
    DES_key_schedule pass;
    strncpy((char *)block,password,8);
    DES_set_key(&block,&pass);
    
    while((fread(src_block,8*sizeof(char),1,srcFile))==1)
    {
        DES_ecb_encrypt(&src_block,&block,&pass,type);
        fwrite(&block,8*sizeof(char),1,dstFile);
    }
    return 1;
}

int
DES_CBC_crypt(FILE * srcFile,FILE * dstFile,imageInfo info,char * key,char * vi,int type)
{
    unsigned char block[8];
    unsigned char src_block[8];
    DES_cblock vi_block;
    CopyHeader(srcFile,dstFile,info);
    DES_key_schedule pass;
    strncpy((char *)block,key,8);
    strncpy((char *)vi_block,vi,8);
    DES_set_key(&block,&pass);
    
    while((fread(src_block,8*sizeof(char),1,srcFile))==1)
    {
        DES_ncbc_encrypt(src_block,block,8,&pass,&vi_block,type);
        fwrite(&block,8*sizeof(char),1,dstFile);
    }
    return 1;
}

int
DES_CFB_crypt(FILE * srcFile,FILE * dstFile,imageInfo info,char * key,char * vi,int type)
{
    unsigned char block[8];
    unsigned char src_block[8];
    DES_cblock vi_block;
    CopyHeader(srcFile,dstFile,info);
    DES_key_schedule pass;
    strncpy((char *)block,key,8);
    strncpy((char *)vi_block,vi,8);
    DES_set_key(&block,&pass);
    
    while((fread(src_block,8*sizeof(char),1,srcFile))==1)
    {
        DES_cfb_encrypt(src_block,block,8,8,&pass,&vi_block,type);
        fwrite(&block,8*sizeof(char),1,dstFile);
    }
    return 1;
}

int
DES_OFB_crypt(FILE * srcFile,FILE * dstFile,imageInfo info,char * key,char * vi,int type)
{
    /*NO ANDA*/
    unsigned char block[8];
    unsigned char src_block[8];
    DES_cblock vi_block;
    CopyHeader(srcFile,dstFile,info);
    DES_key_schedule pass;
    strncpy((char *)block,key,8);
    strncpy((char *)vi_block,vi,8);
    DES_set_key(&block,&pass);

    while((fread(src_block,8*sizeof(char),1,srcFile))==1)
    {
        DES_ofb_encrypt(src_block,block,8,8,&pass,&vi_block);
        fwrite(&block,8*sizeof(char),1,dstFile);
    }

    return 1;
}


int
GetMethod(char * method)
{
    int methodRet;
    if(strncmp(method,"cbc",3)==0)
    {
        methodRet=CBC;
    }
    else if(strncmp(method,"ecb",3)==0)
    {
        methodRet=ECB;
    }
    else if(strncmp(method,"cfb",3)==0)
    {
        methodRet=CFB;
    }
    else if(strncmp(method,"ofb",3)==0)
    {
        methodRet=OFB;
    }
    else
    {
        methodRet=0;
    }
    
    return methodRet;
}

void
CopyHeader(FILE * srcFile,FILE * dstFile,imageInfo info)
{
    char * aux;
    aux=calloc(1,54*sizeof(char));
    if(aux==NULL)
    {
        fclose(srcFile);
        fclose(dstFile);
        free(info);
        printf("Error de memoria.\n");
        exit(EXIT_FAILURE);
    }
    
    rewind(srcFile);
    rewind(dstFile);
    fread(aux,54,1,srcFile);
    fwrite(aux,54,1,dstFile);
    free(aux);
}


















