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
        /*Modo de encriptacion*/
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
        /*Modo de desencriptacion*/
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

/**********************************/
/*      Funciones para AES        */
/**********************************/

int
CryptAES(char * key,char * method,char * password,char * vi,FILE * srcFile,FILE * dstFile,imageInfo info,int type)
{
    int methodAux=0;
    int status=1;
    
    methodAux=GetMethod(method);
    
    /*Elijo el metodo a utilizar para encriptar o desencriptar con AES*/
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
    char block[16]={0};
    char src_block[16]={0};
    AES_KEY pass;
    
    CopyHeader(srcFile,dstFile,info);
    strncpy((char *)block,key,16);
    
    if( type == AES_ENCRYPT )
        AES_set_encrypt_key((const unsigned char *)&block,128,&pass);
    else
        AES_set_decrypt_key((const unsigned char *)&block,128,&pass);

    
    while((fread(src_block,16*sizeof(char),1,srcFile))==1)
    {
        AES_cbc_encrypt((const unsigned char *)&src_block,(unsigned char *)&block,16,&pass,(unsigned char * )vi,type);
        fwrite(&block,16*sizeof(char),1,dstFile);
    }
    
    return 1;
}

int
AES_ECB_crypt(FILE * srcFile,FILE * dstFile,imageInfo info,char * password,int type)
{
    char block[16]={0};
    char src_block[16]={0};
    AES_KEY pass;
    
    CopyHeader(srcFile,dstFile,info);
    strncpy((char *)block,password,16);
    
    if( type == AES_ENCRYPT )
        AES_set_encrypt_key((const unsigned char *)&block,128,&pass);
    else
        AES_set_decrypt_key((const unsigned char *)&block,128,&pass);

    
    while((fread(src_block,16*sizeof(char),1,srcFile))==1)
    {
        AES_ecb_encrypt((const unsigned char *)&src_block,(unsigned char *)&block,&pass,type);
        fwrite(&block,16*sizeof(char),1,dstFile);
    }
    
    return 1;
}

int
AES_CFB_crypt(FILE * srcFile,FILE * dstFile,imageInfo info,char * key,char * vi,int type)
{
    char block[16]={0};
    char src_block[16]={0};
    char vi_aux[16]={0};
    char key_aux[16]={0};
    AES_KEY pass;
    int num=15;
    
    CopyHeader(srcFile,dstFile,info);
    memcpy(vi_aux,vi,16);
    memcpy(key_aux,key,16);
    printf("AES CFB Tipo: %d\n",type);
    printf("Key: %s",key);
    if( type == AES_ENCRYPT )
        AES_set_encrypt_key((const unsigned char *)&key_aux,128,&pass);
    else
        AES_set_decrypt_key((const unsigned char *)&key_aux,128,&pass);

    while((fread(src_block,16*sizeof(char),1,srcFile))==1)
    {
        AES_cfb128_encrypt((const unsigned char *)&src_block,(unsigned char *)&block,16,&pass,(unsigned char *)vi_aux,&num,type);
        fwrite(&block,16*sizeof(char),1,dstFile);
    }
    
    return 1;
}

int
AES_OFB_crypt(FILE * srcFile,FILE * dstFile,imageInfo info,char * key,char * vi,int type)
{
    unsigned char block[16]={0};
    unsigned char src_block[16]={0};
    AES_KEY pass;
    int num=15;
    
    CopyHeader(srcFile,dstFile,info);
    strncpy((char *)block,key,16);
    printf("AES OFB Tipo: %d\n",type);
    if( type == AES_ENCRYPT )
        AES_set_encrypt_key((const unsigned char *)&block,128,&pass);
    else
        AES_set_decrypt_key((const unsigned char *)&block,128,&pass);

    while((fread(src_block,16*sizeof(char),1,srcFile))==1)
    {
        AES_ofb128_encrypt((const unsigned char *)&src_block,(unsigned char *)&block,16,&pass,(unsigned char * )vi,&num);
        fwrite(&block,16*sizeof(char),1,dstFile);
    }
    
    return 1;
}

/**********************************/
/*      Funciones para DES        */
/**********************************/

int
CryptDES(char * key,char * method,char * password,char * vi,FILE * srcFile,FILE * dstFile,imageInfo info,int type)
{
    int methodAux=0;
    int status=1;
    
    methodAux=GetMethod(method);
    
    /*Elijo el metodo a utilizar para encriptar o desencriptar con DES*/
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
    DES_set_odd_parity(&block);
    DES_set_key_checked(&block,&pass);
    
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
    unsigned char block[8]={0};
    unsigned char src_block[8]={0};
    DES_cblock vi_block;
    CopyHeader(srcFile,dstFile,info);
    DES_key_schedule pass;
    strncpy((char *)block,key,8);
    strncpy((char *)vi_block,vi,8);
    DES_set_odd_parity(&block);
    DES_set_key_checked(&block,&pass);
    
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
    unsigned char block[8]={0};
    unsigned char src_block[8]={0};
    DES_cblock vi_block;
    CopyHeader(srcFile,dstFile,info);
    DES_key_schedule pass;
    strncpy((char *)block,key,8);
    strncpy((char *)vi_block,vi,8);
    DES_set_odd_parity(&block);
    DES_set_key_checked(&block,&pass);
    
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
    unsigned char block[8]={0};
    unsigned char src_block[8]={0};
    DES_cblock vi_block;
    CopyHeader(srcFile,dstFile,info);
    DES_key_schedule pass;
    strncpy((char *)block,key,8);
    strncpy((char *)vi_block,vi,8);
    DES_set_odd_parity(&block);
    DES_set_key_checked(&block,&pass);

    while((fread(src_block,8*sizeof(char),1,srcFile))==1)
    {
        DES_ofb_encrypt(src_block,block,8,8,&pass,&vi_block);
        fwrite(&block,8*sizeof(char),1,dstFile);
    }

    return 1;
}

/**********************************/
/*      Funciones auxiliares      */
/**********************************/

/*Asocio el string representando el metodo de encriptacion con un entero*/
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


/*Copio el header de la imagen sin hacer alteraciones.*/
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
